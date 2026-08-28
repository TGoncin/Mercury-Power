/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Costs.cpp
|    @brief     Material cost submodule for calculations & manufacturing design evaluations.
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initializes localized state containers for component material tracking.
|                   - Calculates structural and bulk costs for the core, windings, insulation, and busbars.
|                   - Evaluates performance efficiency penalization matrices across no-load and load loss parameters.
|               Returns:
|                   - A localized, high-performance data structure representing total component manufacturing costs.
|___________________________________________________________________________________________________*/

#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "conversions.h"
#include "table_Search.h"

#include "costs.h"
#include "losses.h"
#include "core.h"
#include "guarantee.h"
#include "coil.h"
#include "costs.h"

namespace Transformer::Commercial
{

    int Costs::id_Counter = 1;

    Costs::Costs
    (
        home_Device*          parentDevice, 
        home_ConductorTable   conductorTable) 
        : 
        device          (parentDevice), 
        conductorTable  (conductorTable)
    {
        costFields["dLamination"]   = home_FieldValue (home_FieldValue::List(6, 0.0));
        costFields["dCore"]         = home_FieldValue (0.0);
        costFields["dKraft0022"]    = home_FieldValue (0.0);
        costFields["dKraft005"]     = home_FieldValue (0.0);
        costFields["dKraft010"]     = home_FieldValue (0.0);
        costFields["dKraft015"]     = home_FieldValue (0.0);
        costFields["dCrepe"]        = home_FieldValue (0.0);
        costFields["dPB1_16"]       = home_FieldValue (0.0);
        costFields["dPB1_8"]        = home_FieldValue (0.0);
        costFields["dDucts"]        = home_FieldValue (0.0);
        costFields["dLV"]           = home_FieldValue (0.0);
        costFields["dHV"]           = home_FieldValue (0.0);
        costFields["dCoilLeads"]    = home_FieldValue (0.0);
        costFields["dBusbar"]       = home_FieldValue (0.0);
        costFields["dPBHD_1_4"]     = home_FieldValue (0.0);

        costFields["dCostCore"]         = home_FieldValue (0.0);
        costFields["dCostKraft0022"]    = home_FieldValue (0.0);
        costFields["dCostKraft005"]     = home_FieldValue (0.0);
        costFields["dCostKraft010"]     = home_FieldValue (0.0);
        costFields["dCostKraft015"]     = home_FieldValue (0.0);
        costFields["dCostCrepe"]        = home_FieldValue (0.0);
        costFields["dCostPB1_16"]       = home_FieldValue (0.0);
        costFields["dCostPB1_8"]        = home_FieldValue (0.0);
        costFields["dCostDucts"]        = home_FieldValue (0.0);
        costFields["dCostLV"]           = home_FieldValue (0.0);
        costFields["dCostHV"]           = home_FieldValue (0.0);
        costFields["dCostCoilLeads"]    = home_FieldValue (0.0);
        costFields["dCostBusBar"]       = home_FieldValue (0.0);
        costFields["dCostPBHD_1_4"]     = home_FieldValue (0.0);
        costFields["dTotalCost"]        = home_FieldValue (0.0);
        costFields["dTotalCostRound"]   = home_FieldValue (0.0);
        costFields["dEvalNL"]           = home_FieldValue (0.0);
        costFields["dEvalLL"]           = home_FieldValue (0.0);
        costFields["dCCEval"]           = home_FieldValue (0.0);
    }

        home_Device* Costs::parent_device() const
        {
            if (device == nullptr)         throw std::runtime_error("This winding is not attached to any Device");
            return device;
        }

        void Costs::calc_Costs
        (
            home_Device*    device, 
            home_Winding*   winding_Hv, 
            home_Winding*   winding_Lv
        )
        {
            auto&    losses       = device->losses;
            auto&    guarantee    = device->guarantee;
            auto&    core         = device->core;

            auto    coilLv       = winding_Lv->coils [0];
            auto    coilHv       = winding_Hv->coils [0];

            int     numberPhases  = device->deviceFields ["iNoPhases"].get <int>();

            calc_CostsDefault      (device,       core.get(),      losses.get(),  winding_Hv, winding_Lv,   coilHv, coilLv);
            calc_CostsDesign       (device,       core.get(),      winding_Hv,    winding_Lv, numberPhases);
            calc_EvaluateDesign    (losses.get(), guarantee.get());
        }

        void Costs::calc_CostsDefault 
        (
            home_Device*    device, 
            home_Core*      core, 
            home_Losses*    losses, 
            home_Winding*   winding_Hv, 
            home_Winding*   winding_Lv, 
            home_Coil*      coil_Hv, 
            home_Coil*      coil_Lv
        )
        {
            auto&   self            = costFields;
            auto    conductor_Lo    = coil_Lv->conductor[0].conductorFields;
            auto    conductor_Hi    = coil_Hv->conductor[0].conductorFields;
            auto    winding_LvFields= winding_Lv->windingFields;
            auto    winding_HvFields= winding_Hv->windingFields;

            int     numberPhases    = device->deviceFields ["iNoPhases"]    .get<int>();
            int     busLdMaterial   = losses->lossFields ["iBusLdMat"]      .get<int>();

            auto    type_Lv                   = conductor_Lo   ["iType"]        .get<Config::ConductorType>();
            auto    typeMaterialAluminum_Lv   = conductor_Lo   ["tMatAluminum"] .get<Config::Material>();

            int     noConductor_Lv            = conductor_Lo   ["iNoConductor"] .get<int>();

            auto    typeMaterialAluminum_Hv   = conductor_Hi   ["tMatAluminum"] .get<Config::Material>();
            int     noConductor_Hv            = conductor_Hi   ["iNoConductor"] .get<int>();
            double  weight_Lv                 = conductor_Lo   ["dWeight"]      .get<double>();

            self["dLamination"].set (999.0, static_cast<int> (home_LaminationType::    Am));
            self["dLamination"].set (3.65,  static_cast<int> (home_LaminationType::    AmHB1));
            self["dLamination"].set (999.0, static_cast<int> (home_LaminationType::    M2));
            self["dLamination"].set (999.0, static_cast<int> (home_LaminationType::    M3));
            self["dLamination"].set (999.0, static_cast<int> (home_LaminationType::    M4));
            self["dLamination"].set (4.00,  static_cast<int> (home_LaminationType::    M0H));

            auto laminationType        = core->coreFields["tLaminationType"].get<home_LaminationType>();
            int  valueLaminationType   = static_cast<int>(home_LaminationType::None);

            if      (laminationType == home_LaminationType::  Am)       valueLaminationType = static_cast<int> (home_LaminationType::  Am);
            else if (laminationType == home_LaminationType::  AmHB1)    valueLaminationType = static_cast<int> (home_LaminationType::  AmHB1);
            else if (laminationType == home_LaminationType::  M0H)      valueLaminationType = static_cast<int> (home_LaminationType::  M0H);
            else if (laminationType == home_LaminationType::  M2)       valueLaminationType = static_cast<int> (home_LaminationType::  M2);
            else if (laminationType == home_LaminationType::  M3)       valueLaminationType = static_cast<int> (home_LaminationType::  M3);
            else if (laminationType == home_LaminationType::  M4)       valueLaminationType = static_cast<int> (home_LaminationType::  M4);

            self["dCore"].set(self["dLamination"].get<std::vector<double>>()[valueLaminationType]);

            double value = get_CoreLeadValue(device, "2KP");
            self["dKraft0022"].set(value != 0.0 ? value : 6.05);

            value = get_CoreLeadValue(device, "5KP");
            self["dKraft005"].set(value != 0.0 ? value : 2.9 * 1.25);

            value = get_CoreLeadValue(device, "10KP");
            self["dKraft010"].set(value != 0.0 ? value : 2.3 * 1.25);

            self["dKraft015"].set(0.0);

            value = get_CoreLeadValue(device, "510HC");
            self["dCrepe"].set(value != 0.0 ? value : 12.63);

            value = get_CoreLeadValue(device, "16PB");
            self["dPB1_16"].set(value != 0.0 ? value : 2.67);

            value = get_CoreLeadValue(device, "8PB");
            self["dPB1_8"].set(value != 0.0 ? value : 2.523);

            value = get_CoreLeadValue(device, "8DUCT");
            self["dDucts"].set(value != 0.0 ? value : 5.03);

            double result   = 0.0;
            double plus     = 0.0;

            if (type_Lv == home_ConductorType::Strip)
            {
                if (typeMaterialAluminum_Lv == home_Material::Copper)
                    { result = Config::lookup_Value(noConductor_Lv, conductorTable.tableData_CopperStrip);  }
                else
                    { result = Config::lookup_Value(noConductor_Lv, conductorTable.tableData_AluminumStrip);}  
            }
            else
            {
                if (typeMaterialAluminum_Lv == home_Material::Copper)
                    { result = Config::lookup_Value(noConductor_Hv, conductorTable.tableData_CopperMagnet);   }
                else
                    { result = Config::lookup_Value(noConductor_Hv, conductorTable.tableData_AluminumMagnet); }
            }

            self["dLV"].set(result + plus);

            result  = 0.0;
            plus   = 0.0;

            if (typeMaterialAluminum_Hv == home_Material::Copper)   { result = Config::lookup_Value(noConductor_Hv, conductorTable.tableData_CopperMagnet);   }
            else                                                    { result = Config::lookup_Value(noConductor_Hv, conductorTable.tableData_AluminumMagnet); }

            self["dHV"].set(result + plus);

            std::string typeLeads = winding_Lv->windingFields["tTypeLeads"].get<std::string>();
            value                 = get_CoreLeadValue(device, typeLeads);

            self["dCoilLeads"].set  ( value != 0.0 ? value : self["dLV"].get<double>());

            std::string typeBuswork = device->deviceFields["tTypeBuswork"].get<std::string>();
            value                   = get_CoreLeadValue(device, typeBuswork);

            if (value != 0.0)
            {
                auto materialBuswork = device->deviceFields["tMaterialBuswork"].get<Config::Material>();

                if (materialBuswork == Config::Material::Copper)
                {
                    double copperDensity = 0.323 * 12.0;
                    double lengthBusBar  = (numberPhases == 1) ? 1.0 : device->deviceFields ["dBusTotalLength"] .get<double>() / 12.0;
                    double weightBusBar  = winding_Lv->windingFields ["dWeightBusBar"] .get<double>();
                    double factor        = weightBusBar / lengthBusBar;

                    self["dBusbar"].set(value / factor * copperDensity);
                }
                else        
                {   self["dBusbar"].set(value); }
            }
            else    {   self["dBusbar"].set(busLdMaterial == 1 ? 5.25 : 9.7);   }

            std::string typePBHD   = "4PB_HD";
            value                  = get_CoreLeadValue(device, typePBHD);

            self["dPBHD_1_4"].set(value != 0.0 ? value : 27.87);
        }

        double Costs::get_CoreLeadValue(home_Device* device, const std::string& key)
        {
            std::string keyUpper = key;
            std::transform(keyUpper.begin(), keyUpper.end(), keyUpper.begin(), ::toupper);

            for (const auto& row : device->sapStandardCoreLeads.tableData)
            {
                std::string rowKeyUpper = row.sapName; 
                std::transform(rowKeyUpper.begin(), rowKeyUpper.end(), rowKeyUpper.begin(), ::toupper);

                if (rowKeyUpper == keyUpper)        {   return row.totalCostPerLb;  }
            }
            return 0.0;
        }

        void Costs::calc_CostsDesign(home_Device* device, home_Core* core, home_Winding* winding_Hv, home_Winding* winding_Lv, int numberPhases)
        {
            auto& self          = device->deviceFields;
            auto deviceFields   = device->deviceFields;
            auto coreFields     = core->coreFields;

            double dLVWeight = winding_Lv->windingFields["dWeight"].get<double>();
            double dHVWeight = winding_Hv->windingFields["dWeight"].get<double>();

            auto& costCore      = self["dCostCore"]; 
            auto& costKraft0022 = self["dCostKraft0022"];
            auto& costKraft005  = self["dCostKraft005"];
            auto& costKraft010  = self["dCostKraft010"];
            auto& costKraft015  = self["dCostKraft015"];
            auto& costCrepe     = self["dCostCrepe"];
            auto& costPB1_16    = self["dCostPB1_16"];
            auto& costPB1_8     = self["dCostPB1_8"];
            auto& costDucts     = self["dCostDucts"];
            auto& costLV        = self["dCostLV"];
            auto& costHV        = self["dCostHV"];
            auto& costCoilLeads = self["dCostCoilLeads"];
            auto& costBusBar    = self["dCostBusBar"];
            auto& costPBHD_1_4  = self["dCostPBHD_1_4"];

            costCore.set     (coreFields["dWeightTwoLoops"]         .get<double>()                  * self["dCore"]     .get<double>());
            costKraft0022.set(deviceFields["dWeightKr0022"]         .get<double>()                  * self["dKraft0022"].get<double>());
            costKraft005.set (deviceFields["dWeightKr005"]          .get<double>()                  * self["dKraft005"] .get<double>());
            costKraft010.set (deviceFields["dWeightKr010"]          .get<double>()                  * self["dKraft010"] .get<double>());
            costKraft015.set (deviceFields["dWeightKr015"]          .get<double>()                  * self["dKraft015"] .get<double>());
            costCrepe.set    (deviceFields["dWeightCrepe"]          .get<double>()                  * self["dCrepe"]    .get<double>());
            costPB1_16. set  (coreFields["dWeightsPB1_16"]          .get<double>()                  * self["dPB1_16"]   .get<double>());
            costPB1_8.set    (coreFields["dWeightsPB1_8"]           .get<double>()                  * self["dPB1_8"]    .get<double>());
            costDucts.set    (coreFields["d_DuctsDM"]               .get<std::vector<double>>()[0]  * self["dDucts"]    .get<double>());
            costLV.set       (dLVWeight * self["dLV"]               .get<double>());
            costHV.set       (dHVWeight * self["dHV"]               .get<double>());
            costCoilLeads.set(winding_Lv->windingFields["dWeightCoilLeads"].get<double>()                  * self["dCoilLeads"].get<double>());
            costBusBar.set   (winding_Lv->windingFields["dWeightBusBar"]   .get<double>()                  * self["dBusbar"]   .get<double>());
            costPBHD_1_4.set (coreFields["dWeightsPBHD_1_4"]        .get<double>()                  * self["dPBHD_1_4"] .get<double>());

            double result =(costCore.get<double>() + 
                            costKraft0022.get<double>() + 
                            costKraft005.get<double>() +
                            costKraft010.get<double>() +
                            costKraft015.get<double>() +
                            costCrepe.get<double>() +
                            costPB1_16.get<double>() +
                            costPB1_8.get<double>() +
                            costDucts.get<double>() +
                            costLV.get<double>() +
                            costHV.get<double>() +
                            costCoilLeads.get<double>() +
                            costBusBar.get<double>() +
                            costPBHD_1_4.get<double>()
                        );

            self["dTotalCost"].set(result);
            self["dTotalCostRound"].set(std::ceil(result / 10.0) * 10.0);
        }

        void Costs::calc_EvaluateDesign(home_Losses* losses, home_Guarantee* guarantee)
    {
        auto& guaranteeFields = guarantee->guaranteeFields;
        double evalRateNL     = guaranteeFields["dEvaluation_nl"].get<double>();
        double evalRateLL     = guaranteeFields["dEvaluation_ll"].get<double>();

        auto& lossesFields    = losses->lossFields; 
        double noLoadLoss     = lossesFields["dNLLosses"].get<double>(); 
        double loadLoss       = lossesFields["dLLosses"].get<double>();

        double calculatedNL   = evalRateNL * noLoadLoss;
        double calculatedLL   = evalRateLL * loadLoss;
        
        double totalCostRound = guaranteeFields["dTotalCostRound"].get<double>();

        guaranteeFields["dEvalNL"].set(calculatedNL);
        guaranteeFields["dEvalLL"].set(calculatedLL);
        
        guaranteeFields["dCCEval"].set(calculatedNL + calculatedLL + totalCostRound);
    }
}