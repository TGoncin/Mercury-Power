/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Winding.h
|    @brief     Implementation manager for transformer winding parameter calculation loops.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|                   - INITIALIZATION:   winding config states (V, I, BIL, Taps)
|                   - EVALUATE:         line, phase, tap voltages to standards
|                   - COMPUTES:         current loads, automated cable/lead layout dimensions
|                   - RETURNS:          high performance data structure
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "definitions.h"
#include "system_Config.h"
#include "alias.h"
using namespace Transformer::Types;
#include "winding_Warnings.h"
#include "math_Utilities.h"

#include "device.h"
#include "cooling.h"
#include "coil.h"
#include "winding.h"
#include "bushing.h"

using namespace Transformer::Config;
using namespace Transformer::Core;




namespace Transformer::Components
{
    int Winding::id_Counter = 1;

    Winding::Winding() : changer(this), winding_Warnings(this), device(nullptr)
    {
        windingFields["id"]                = FieldValue (Winding:: id_Counter);
        windingFields["name"]              = FieldValue ("Winding " + std::to_string (Winding::id_Counter));
        windingFields["tDescription"]      = FieldValue (WindingDesc:: None);
        windingFields["dVoltage"]          = FieldValue (0.00);
        windingFields["type_connection"]   = FieldValue (ConnectionType::Wye);
        windingFields["iBIL"]              = FieldValue (BIL:: BIL_30);
        windingFields["iBILUnder"]         = FieldValue (30);
        windingFields["bHalfTurns"]        = FieldValue (false);
        windingFields["tCoating"]          = FieldValue (Coating:: Paper);
        windingFields["d_kVA"]             = FieldValue (FieldValue::List (5, 0.00));
        windingFields["d_line_kVoltage"]   = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["d_kVPhaseDef"]      = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["d_Turns"]           = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["dNomTurnsUI"]       = FieldValue (0.00);
        windingFields["pdPhaseAmp"]        = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["pdPhaseAmp1"]       = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["pdLineAmp"]         = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["pdLineAmp1"]        = FieldValue (FieldValue::List (MAX_TAPS, 0.00));
        windingFields["tTypeLeads"]        = FieldValue ("CABLE");
        windingFields["quantityLeads"]     = FieldValue (1);
        windingFields["thicknessLeads"]    = FieldValue (0.00);
        windingFields["widthLeads"]        = FieldValue (0.00);
        windingFields["dLeadsDensity"]     = FieldValue (0.00);
        windingFields["sLeadsDensity"]     = FieldValue ("");
        windingFields["iNoDuctInner"]      = FieldValue (0);
        windingFields["iNoDuctOuter"]      = FieldValue (0);
        windingFields["dDuctSize"]         = FieldValue (0.00);
        windingFields["sDuctPlacement"]    = FieldValue ("");
        windingFields["dOptStayback"]      = FieldValue (0.00);
        windingFields["dLayerInsulation"]  = FieldValue (0.00);
        windingFields["dEndInsulation"]    = FieldValue (0.00);
        windingFields["dBuildFactorUsed"]  = FieldValue (0.00);
        windingFields["dHLOverNoseReduction"]  = FieldValue (0.00);
        windingFields["sErrorName"]        = FieldValue (FieldValue::List (ERRORMESSAGES, "") );
        windingFields["tErrorEnum"]        = FieldValue (FieldValue::List (ERRORMESSAGES, static_cast<int> (home_WarningSeverity::None)));
        windingFields["sERRORMESSAGES"]    = FieldValue (FieldValue::List (ERRORMESSAGES, "") );

        Winding::id_Counter++;
    }

    Winding::~Winding() {}
        
    home_Device* Winding::getParentDevice() const
    {
        if (device == nullptr)    { throw std::runtime_error("This module is not attached to any Device");  }
        return device;
    }

    int Winding::get_NominalPosition() const
    {
        return changer.get_NumberSteps();  
    }
    int Winding::get_NumberSteps() const
    {
        return changer.get_NumberSteps(); 
    }
    int Winding::get_WindingType() const
    {
        auto description = windingFields.at("tDescription").get<home_WindingDesc>();

        if (description == home_WindingDesc::HV || description == home_WindingDesc::RegHV)
        {
            return static_cast<int>(home_WindingDesc::HV);  
        }
        return static_cast<int>(home_WindingDesc::LV);
    }

    bool Winding::have_Taps() const     {   return changer.get_NumberSteps() > 0;  }

    void Winding::add_Coils(Coil* coil)
    {
        coils   .push_back    (coil);
        coil->  assign_Winding(this);
    }


    void Winding::reset_WindingFields()
    {
									// 1. Protect the Winding's dictionary
        for (auto& windingField : windingFields) 
        {
            try 
            {
                windingField.second.reset();
            }
            catch (...) {}
        }

									// 2. Pass reset command down chain
        changer.reset_ChangerFields();

        for (auto& bushingField : bushings) 
        {
            bushingField.reset_BushingFields();
        }
    }


    void Winding::erase_Warnings()
    {
        for (int i = 0; i < ERRORMESSAGES; ++i)
    {
            windingFields["sErrorName"]       .set("", i);
            windingFields["tErrorEnum"]       .set(static_cast<int> (WarningSeverity::None, i));
            windingFields["sERRORMESSAGES"]   .set("", i);
        }
        changer.erase_Warnings();
    }
    void Winding::calc_Warnings()
    {
        std::vector<double> emptyValues = {0.0, 0.0};

        winding_Warnings.warningLeadsDensity(emptyValues);
        changer.calc_Warnings();
    }
    void Winding::calc_Winding()
    {
        calc_Amperage();
        calc_Leads();
        use_Ducts();
        changer.calc_Changer();
    }
    void Winding::calc_DefaultKVA (home_Device* device)
    {
        auto& self      = windingFields;
        double dkVANom  = device->deviceFields ["nominal_kva"].get<double>();
        int steps       = changer.get_NumberSteps();

        self["d_kVA"].set(dkVANom, steps);

        if (steps > 0)
        {
            double factorUp     = changer.get_PerStepUp();
            double factorDown   = changer.get_PerStepDown();
            
            for (int i = 0; i < steps; ++i) 
            {
                double stepValue = dkVANom * (1.0 + (i - steps / 2) * factorUp); // Dynamic balancing config maps
                self["d_kVA"].set(stepValue, i);
            }
        }
    }
    void Winding::calc_DefaultLineKV()
    {
        auto&   self            = windingFields;
        double  voltageBase     = self["dVoltage"]  .get<double>();
        int     steps           = changer           .get_NumberSteps();

        self["d_line_kVoltage"].set(voltageBase, steps);

        if (steps > 0)
        {
            double perStepUp = changer.get_PerStepUp();

            for (int i = 0; i < steps; ++i)
            {
                double value = voltageBase * (1.0 + (i - steps) * perStepUp);
                self["d_line_kVoltage"].set(value, i);
            }
        }
    }
    void Winding::calc_DefaultPhaseKV (home_Device* device, Winding* wind_Hi, Winding* wind_Lo, Winding* wind_Tertiary)
    {
        auto&   self            = windingFields;
        int     numberPhases    = device->deviceFields ["iNoPhases"]    .get <int>();
        auto    connection      = self ["type_connection"]              .get <home_ConnectionType>();
        auto    lineVoltages    = self ["d_line_kVoltage"]              .get <std::vector <double>>();

        for (size_t i = 0; i < lineVoltages.size(); ++i)
        {
            double phaseVoltage = lineVoltages[i];
        
            if (numberPhases == 3 && connection == home_ConnectionType::Wye)
            {
                phaseVoltage /= std::sqrt(3.0);
            }
            self ["d_kVPhaseDef"].set(phaseVoltage, static_cast<int> (i));
        }
    }
    void Winding::calc_Amperage()
    {
        auto&        self           = windingFields;
        home_Device* device         = getParentDevice();
        int          numberPhases   = device->deviceFields ["iNoPhases"].get<int>();
        int          nominalPosition= get_NominalPosition();
        auto         arrayKVA       = self ["d_kVA"]          .get<std::vector<double>>();
        auto         phaseVoltages  = self ["d_kVPhaseDef"]   .get<std::vector<double>>();
        double       currentKVA     = arrayKVA      [nominalPosition];
        double       currentPhV     = phaseVoltages [nominalPosition];

        double  phaseAmp       = safe_Division(currentKVA, (numberPhases * currentPhV));
        self["pdPhaseAmp"].set(phaseAmp, nominalPosition);

        double  lineAmp        =   ( numberPhases == 3 && self["type_connection"].get<home_ConnectionType>() ==home_ConnectionType::Delta) 
                                ? phaseAmp * std::sqrt(3.0) : phaseAmp;
        self["pdLineAmp"].set(lineAmp, nominalPosition);


        int coolingSteps = device->cooling->coolingFields["iCoolingSteps"].get<int>();    // cooling tracking loops (ONAN vs ONAF capacity levels)

        if (coolingSteps > 1)
        {
            double ratio =    device->cooling->coolingFields["dMVAOExc"].get<std::vector<double>>()[1] 
                            / device->cooling->coolingFields["dMVAOExc"].get<std::vector<double>>()[0];

            self["pdPhaseAmp1"] .set    (phaseAmp * ratio, nominalPosition);
            self["pdLineAmp1"]  .set    (lineAmp * ratio, nominalPosition);
        }
    }
    void Winding::calc_Leads()
    {
        auto& self = windingFields;

        if (self["tTypeLeads"].get<std::string>() != "BUSBAR")        {   return;   }

        int nominalPosition    = get_NominalPosition();
        double phaseAmperage   = self["pdPhaseAmp"] .get<std::vector <double>>()[nominalPosition];
        auto& quantityLeads    = self["quantityLeads"];
        auto& thicknessLeads   = self["thicknessLeads"];
        auto& widthLeads       = self["widthLeads"];

        double thicknessLeads2      = thicknessLeads   .get<double>();
        double widthLeads2          = widthLeads       .get<double>();
        double dDensLV              = 1500.0;
        double difference           = 0.25;
        double finalDensityResult   = 0.0;

        int    quantityLess            = 0;
        int    quantityLessPercent     = 0;


        for (int i = 1; i <= 20; ++i)
        {
            double result = safe_Division (phaseAmperage, (i * thicknessLeads2 * widthLeads2));

            if (result <= dDensLV && quantityLess == 0)
            {
                quantityLess = i;
                quantityLeads.set(i);
                finalDensityResult = result;
                break;
            }

            double currentDif = std::abs(result - dDensLV) / dDensLV;

            if (currentDif <= difference)
            {
                difference            = currentDif;
                quantityLessPercent   = i;
            }
        }

        if (quantityLess == 0)
        {
            int finalQty        = (quantityLessPercent > 0) ? quantityLessPercent : 20;
            quantityLeads.set(finalQty);

            finalDensityResult  = safe_Division (phaseAmperage, (finalQty * thicknessLeads2 * widthLeads2));
        }

        self["dLeadsDensity"].assign_Default (finalDensityResult);
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << finalDensityResult;

        self["sLeadsDensity"].assign_Default(ss.str());
    }
    void Winding::use_Ducts() {}
}
// Transformer::Components::Winding ends