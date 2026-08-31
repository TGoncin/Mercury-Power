/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Device.cpp
|    @brief     Top-level orchestration layer driving the transformer structural validation framework.
|    @brielf    Transformer::Core       is "The Engine Framework"
|
|               Executes:
|                   - Global configuration mappings utilizing flexible multi-variant FieldValues.
|                   - Orchestrates sub-module pipelines across core, windings, thermal cooling, and losses.
|                   - Handles electrical simulation loops resolving performance efficiency and load metrics.
|               Returns:
|                   - A central unified simulation structure decoupled completely from global thread spaces.
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>


#include "system_Config.h"
#include "math_utilities.h"

#include "winding.h"
#include "coil.h"
#include "cooling.h"
#include "core.h"
#include "guarantee.h"
#include "mechanical.h"
#include "losses.h"
#include "costs.h"

#include "device.h"




namespace Transformer::Core
{
    Device::Device() : device_Warnings(WarningSeverity::None)
    {
        deviceFields["so_number"]                 = FieldValue ("Q00000");
        deviceFields["customer_name"]             = FieldValue ("");
        deviceFields["customer_code"]             = FieldValue ("");
        deviceFields["quantity"]                  = FieldValue (1);
        deviceFields["issued_by"]                 = FieldValue ("");
        deviceFields["original_issue_date"]       = FieldValue ("05/26/2026");
        deviceFields["sSalesPerson"]              = FieldValue ("");
        deviceFields["requested_due_date"]        = FieldValue ("05/26/2026");
        deviceFields["dLatestVersion"]            = FieldValue (0.20);
        deviceFields["bDesingSpecial"]            = FieldValue (false);
        deviceFields["transformer_family"]        = FieldValue (Config::Family::None);
        deviceFields["nominal_kva"]               = FieldValue (0.0);
        deviceFields["frequency"]                 = FieldValue (60);
        deviceFields["winding_description"]       = FieldValue ("Standard Shell Type, Layer Winding");
        deviceFields["iNoPhases"]                 = FieldValue (1);
        deviceFields["i_RevisionWS"]              = FieldValue (FieldValue::List (5, FieldValue::Value (0))  );
        deviceFields["data_description"]          = FieldValue (FieldValue::List (5, "") );
        deviceFields["s_DateDescr0"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DateDescr1"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DateDescr2"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DateDescr3"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DateDescr4"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DataDescr0"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DataDescr1"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DataDescr2"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DataDescr3"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["s_DataDescr4"]              = FieldValue (FieldValue::List (10, ""));
        deviceFields["previousarray_type"]        = FieldValue (ArrayType::Other);
        deviceFields["array_type"]                = FieldValue (ArrayType::LH);
        deviceFields["num_Coils"]                 = FieldValue (0);
        deviceFields["number_winding"]            = FieldValue (2);
        deviceFields["have_tertiary"]             = FieldValue (false);
        deviceFields["dual_voltage"]              = FieldValue (false);
        deviceFields["i_QuantityBuswork"]         = FieldValue (1);
        deviceFields["t_MaterialBuswork"]         = FieldValue (Material::None);
        deviceFields["s_MaterialBuswork"]         = FieldValue (Material::None);
        deviceFields["t_TypeBuswork"]             = FieldValue (Bars::None);
        deviceFields["d_ThicknessBuswork"]        = FieldValue (0.00);
        deviceFields["d_WidthBuswork"]            = FieldValue (0.0);
        deviceFields["d_DensityBuswork"]          = FieldValue (0.0);
        deviceFields["d_DensityBusworkOnaf"]      = FieldValue (0.0);
        deviceFields["s_DensityBuswork"]          = FieldValue ("");
        deviceFields["s_DensityBusworkOnaf"]      = FieldValue ("");
        deviceFields["dBus1Length"]               = FieldValue (0.0);
        deviceFields["dBus2Length"]               = FieldValue (0.0);
        deviceFields["dBus3Length"]               = FieldValue (0.0);
        deviceFields["dBusTotalLength"]           = FieldValue (0.0);
        deviceFields["dBusTotalLengthFeet"]       = FieldValue (0.0);
        deviceFields["dBusWatts/Feet"]            = FieldValue (0.0);
        deviceFields["dBusTotalWatts"]            = FieldValue (0.0);
        deviceFields["sConnectionDiagramsItems"]  = FieldValue ("");
        deviceFields["dOverallRBFactor"]          = FieldValue (0.0);
        deviceFields["dAltitude"]                 = FieldValue (0);
        deviceFields["sWindingSpecComments"]      = FieldValue ("");
        deviceFields["sPurchasingComments"]       = FieldValue ("");
        deviceFields["sInsulationComments"]       = FieldValue ("");
        deviceFields["dONANLosses"]               = FieldValue (0.0);
        deviceFields["dONANTOR"]                  = FieldValue (0.0);
        deviceFields["sFormMaterial"]             = FieldValue ("");
        deviceFields["dFormMaterialRB"]           = FieldValue (0.0);
        deviceFields["sFormComments"]             = FieldValue ("");
        deviceFields["iBILUnder"]                 = FieldValue (30);
        deviceFields["dVxTi"]                     = FieldValue (0.00);
        deviceFields["dPreviousLVNomTurns"]       = FieldValue (0.00);
        deviceFields["dVxT"]                      = FieldValue (0.00);
        deviceFields["sVxT"]                      = FieldValue ("");
        deviceFields["dOverCoilkvmm"]             = FieldValue (0.00);
        deviceFields["dOverCoilRadialBuilt"]      = FieldValue (0.00);
        deviceFields["sOverCoilDescription"]      = FieldValue ("");
        deviceFields["dCoreWHPackingkvmm"]        = FieldValue (0.00);
        deviceFields["dCoreWHPackingRadialBuilt"] = FieldValue (0.00);
        deviceFields["sCoreWHPackingDescription"] = FieldValue ("");
        deviceFields["dFormkvmm"]                 = FieldValue (0.00);
        deviceFields["dFormRadialBuilt"]          = FieldValue (0.00);
        deviceFields["sFormDescription"]          = FieldValue ("");
        deviceFields["dUnderHVkvmm"]              = FieldValue (0.00);
        deviceFields["dUnderHVRadialBuilt"]       = FieldValue (0.00);
        deviceFields["sUnderHVDescription"]       = FieldValue ("");
        deviceFields["dOverHVkvmm"]               = FieldValue (0.00);
        deviceFields["dOverHVRadialBuilt"]        = FieldValue (0.00);
        deviceFields["sOverHVDescription"]        = FieldValue ("");
        deviceFields["dOverOLVkvmm"]              = FieldValue (0.00);
        deviceFields["dOverOLVRadialBuilt"]       = FieldValue (0.00);
        deviceFields["sOverOLVDescription"]       = FieldValue ("");
        deviceFields["dCoilCorekvmm"]             = FieldValue (0.00);
        deviceFields["dCoilCoreRadialBuilt"]      = FieldValue (0.00);
        deviceFields["sCoilCoreDescription"]      = FieldValue ("");
        deviceFields["dCoilCoilkvmm"]             = FieldValue (0.00);
        deviceFields["dCoilCoilRadialBuilt"]      = FieldValue (0.00);
        deviceFields["sCoilCoilDescription"]      = FieldValue ("");
        deviceFields["dFormRB"]                   = FieldValue (0.00);
        deviceFields["sFormRBWS"]                 = FieldValue ("");
        deviceFields["sUnderInner"]               = FieldValue ("");
        deviceFields["iQtyCrepe"]                 = FieldValue (0);
        deviceFields["dPBWindingForm"]            = FieldValue (0.0);
        deviceFields["sWindingFormMaterial"]      = FieldValue ("");
        deviceFields["dRBWindingForm"]            = FieldValue (0.0);
        deviceFields["dRBOverOLV"]                = FieldValue (0.0);
        deviceFields["sErrorName"]                = FieldValue ( FieldValue:: List    (ERRORMESSAGES, ""));
        deviceFields["tErrorEnum"]                = FieldValue ( FieldValue:: List    (ERRORMESSAGES, WarningSeverity::None));
        deviceFields["sErrorMessages"]            = FieldValue ( FieldValue:: List    (ERRORMESSAGES, ""));
        deviceFields["iFluxDensityMax"]           = FieldValue (16500);
        deviceFields["iFluxDensityMin"]           = FieldValue (9500);
        deviceFields["iTotalStackMax"]            = FieldValue (230);
        deviceFields["iTotalStackMin"]            = FieldValue (125);



        cooling = std::make_unique<home_Cooling>(this);
        guarantee = std::make_unique<home_Guarantee>(this);


        core = std::make_unique<home_Core>(this);
        mechanical = std::make_unique<home_Mechanical>(this);

        // If losses, tests, gradients, or costs also require the parent pointer, update them too:
        losses = std::make_unique<home_Losses>(this);
        tests = std::make_unique<home_Tests>(this);
        gradients = std::make_unique<home_Gradients>(this);
        costs = std::make_unique<home_Costs>(this);


        load_ItemsSap();
        define_PricesSapTables();
        add_Winding         (home_WindingDesc::LV);
        add_Winding         (home_WindingDesc::HV);
        assign_CoilTypes    (ArrayType::LH);
    }

    Device::~Device() {}

    const std::vector<std::unique_ptr<home_Winding>>& Device::list_Windings() const
    {
        return windings;
    }

    home_Winding* Device::get_WindingHV() const     { return get_Winding(home_WindingDesc::HV); }

    home_Winding* Device::get_WindingLV() const     { return get_Winding(home_WindingDesc::LV); }

    home_Winding* Device::get_Winding(home_WindingDesc home_WindingDesc) const
    {
        for (auto& winding : windings)
        {

            if (    winding && winding->windingFields.count("tDescription") 
				&&  winding->windingFields.at("tDescription").get<WindingDesc>() 
				==  home_WindingDesc)
            {
                return winding.get();
            }
        }
        return nullptr;
    }

    home_Winding* Device::get_WindingTV() const
    {
        // Safety check: Ensure the key exists before calling .at()
        if (deviceFields.count("have_tertiary") > 0 && deviceFields.at("have_tertiary").get<bool>())
        {
            return get_Winding(home_WindingDesc::TV);
        }
        return nullptr;
    }

    home_Coil*    Device::get_LastCoil() const     { return coil.empty() ? nullptr : coil.back().get();    }
    home_Coil*    Device::get_Coil(int index) const
    {
        if (index >= 0 && index < static_cast<int>(coil.size()))        return coil[index].get();
        return nullptr;
    }

    void Device::load_ItemsSap() {}
    void Device::define_PricesSapTables() {}
    void Device::update_PriceConductors() {}
    void Device::reset_Windings()            {  windings.clear();    }  // Use clear() with unique_ptr and not delete!
    void Device::calc_PreviousDevice    (home_Winding* windingHv,     home_Winding* windingLv, home_Cooling* coolingModule) {}
    void Device::calc_DeviceLosses() {}
    void Device::ruleConductorSpace     (home_Winding* windHV,        home_Winding* coilCoilDuctwindingLv) {} 
    void Device::looking_kWSteps        (home_Cooling* coolingModule, home_Winding* windingHv) {}
    void Device::calc_DeviceOptimization(home_Winding* winding_Hv,    home_Winding* windingLv) {}

    void Device::calc_UnderInner()
    {
        std::string formMaterial = deviceFields["sFormMaterial"].get<std::string>();
        std::string formDescription = deviceFields["sFormDescription"].get<std::string>();
        std::string result = !formMaterial.empty() ? formMaterial : formDescription;

        int windingTypeLastCoil = get_WindingLastCoil(); // Adjust name if your helper function differs slightly

        if (windingTypeLastCoil == static_cast<int>(Transformer::Config::WindingDesc::LV)) // or your equivalent enum value
        {
            deviceFields["sUnderInner"].set("Under Inner:" + result);
        }
        else
        {
            deviceFields["sUnderInner"].set("Under:" + result);
        }
    }


    void Device::add_Winding(home_WindingDesc enums)
    {
        windings.push_back(std::make_unique<home_Winding>());      

        auto& activeWinding = windings.back();        
        activeWinding->windingFields["id"].set(static_cast<int>(windings.size()));
        activeWinding->windingFields["tDescription"] = FieldValue(enums);

        if      (enums == home_WindingDesc::HV)      { activeWinding->windingFields["name"] = FieldValue("High Voltage"); }
        else if (enums == home_WindingDesc::LV)      { activeWinding->windingFields["name"] = FieldValue("Low Voltage");  }    
    }
    void Device::calc_FormRB()
    {
        double result = (deviceFields["dFormMaterialRB"].get<double>() > 0.0) 
                         ? deviceFields["dFormMaterialRB"].get<double>() 
                         : deviceFields["dFormRadialBuilt"].get<double>();
         
        deviceFields["dFormRB"].assignDefault(result);
        
        std::stringstream ss;
        ss << "RB: " << std::fixed << std::setprecision(4) << result;
        deviceFields["sFormRBWS"].set(ss.str());
        
        calc_UnderInner();
    }
    void Device::calc_OverCoil()
    {
        home_Winding* windingHV = get_WindingHV();
        deviceFields["dOverCoilkvmm"].set(0.0);

        if (windingHV && !windingHV->coils.empty())
        {
            double crepePaper = windingHV->coils[0]->coilFields["dUOHVCrepePaper"].get<double>();
            double result     = 0.012 * crepePaper;
            deviceFields["dOverCoilRadialBuilt"].set(result);

            std::stringstream stringStream;
            if (crepePaper > 0.0)   {   stringStream << std::fixed << std::setprecision(2) << crepePaper << " Under HV Crepe";  }
            deviceFields["sOverCoilDescription"].set(stringStream.str());
        }
    }
    void Device::calc_CoilCore()
    {
        home_Winding* windHV = get_WindingHV();
        home_Winding* windLV = get_WindingLV();
        
        if (!windHV || !windLV || windHV->coils.empty() || windLV->coils.empty()) return;

        home_Coil* pCoilLV = windLV->coils[0];

        double coilCoreDuct       = pCoilLV->coilFields["dCoilCoreDuct"].get<double>();
        double coilCorePacking    = pCoilLV->coilFields["dCoilCorePacking"].get<double>();
        double overOLVRadialBuilt = deviceFields["dOverOLVRadialBuilt"].get<double>();
        int bilLVE                = pCoilLV->coilFields["iBil"].get<int>();

        double result   = (coilCoreDuct > 0.0) ? (coilCoreDuct + 0.02) : 0.0;
        result          += coilCorePacking;
        
        deviceFields["dCoilCoreRadialBuilt"].set(result);
        double coilCoreRadialBuilt = result;

        result = safe_Division(static_cast<double>(bilLVE), ((coilCoreRadialBuilt + overOLVRadialBuilt) * 25.4));
        deviceFields["dCoilCorekvmm"].set(result);

        std::stringstream stringStream;
        if (coilCoreDuct > 0.0)
        {
            stringStream << std::fixed << std::setprecision(3) << coilCoreDuct << "'' Duct + ";
        }
        stringStream << std::fixed << std::setprecision(3) << coilCorePacking << "'' Packing";
        deviceFields["sCoilCoreDescription"].set(stringStream.str());
    }
    void Device::reset_Device()
    {
        for (auto& pair : deviceFields)         pair.second.reset();
        windings.clear();
        coil.clear();
    }
	
	
	
    void Device::reset_Fields()
    {
        if (mechanical) 	{	mechanical	->reset_Fields();		}
        if (core)        	{	core		->reset_Fields();		}
        if (cooling)     	{	cooling		->reset_Fields();		}
        if (gradients)   	{	gradients	->reset_Fields();		}
        if (guarantee)   	{	guarantee	->reset_Fields();		}
        if (losses)      	{	losses		->reset_LossFields();	}
        if (tests)       	{	tests		->reset_Fields();		}

        if (deviceFields.count("array_type") > 0)
        {
            try 
            {
                assign_CoilTypes(deviceFields["array_type"].get<Config::ArrayType>());
            }
            catch (...) {}
        }

        for (auto& winds : windings) 
        {
            if (winds) winds->reset_WindingFields();
        }
        for (auto& coils : coil) 
        {
            if (coils) coils->reset_Fields();
        }
	}




	
	
	
    void Device::calc_Device(bool warnings)
    {
        home_Winding* windingHv = get_WindingHV();
        home_Winding* windingLv = get_WindingLV();

        if (!windingHv || !windingLv)       {   return;     }

        home_Winding*   windingTertiary  = get_WindingTV();
        int             numberPhases     = deviceFields["iNoPhases"].get<int>();

        cooling->calc_Cooling();

        if (cooling->coolingFields ["iCoolingSteps"] .get <int>() > 1 && numberPhases != 1)
        {
           calc_PreviousDevice (windingHv, windingLv, cooling.get());

        }
        auto laminationType = core->coreFields["tLaminationType"].get <LaminationType>();
        bool amorphous      = ( laminationType == LaminationType::Am ||  laminationType == LaminationType::AmHB1);

        tests-> calc_TestsDefault();
        guarantee-> calc_Guarantee();

        deviceFields["dLatestVersionNo"] .assign_Default (deviceFields["iNoPhases"] .get<int> () == 1 ? 0.20 : 0.18);
        core->coreFields ["dInitialFluxDensity"].set(amorphous ? 14000.0 : 16500.0);

        windingHv->calc_DefaultKVA(this);
        windingLv->calc_DefaultKVA(this);

        windingHv->calc_DefaultLineKV();
        windingLv->calc_DefaultLineKV();

        windingHv->calc_DefaultPhaseKV  (this, windingHv, windingLv, windingTertiary);
        windingLv->calc_DefaultPhaseKV  (this, windingHv, windingLv, windingTertiary);

        core->calc_AreaCore();
        double previousCoreArea = core->coreFields["dArea"].get<double>();
        calc_VxTi(previousCoreArea);
    }
    void Device::calc_VxTi(double areaNucleoPrevious)
    {
        double frequency                = deviceFields["frequency"].get<double>();
        double per                      = guarantee->guaranteeFields["dNL_pu"].get<double>();
        double initialFluxDensityValue  = core->coreFields["dInitialFluxDensity"].get<double>();
        double result                   = (0.002664 * areaNucleoPrevious * initialFluxDensityValue * frequency) / (per * 60.0);

        deviceFields["dVxTi"].set(result);
    }
    void Device::assign_CoilTypes(Config::ArrayType array)
    {
        deviceFields["array_type"].set(array);
        create_CoilArray(array);
        associate_CoilsWithWindings();
    }
    void Device::create_CoilArray(ArrayType array)
    {
        for (auto& coilFields : coil)     { coilFields.reset(); }

        if (array == ArrayType::LH)
        {
            coil.push_back  (std::make_unique <home_Coil>());
            coil.push_back  (std::make_unique <home_Coil>());

            coil[0] ->assign_Index(0);
            coil[1] ->assign_Index(1);
        }
    }
    home_Coil* Device::get_CoilByPosition(int position) const
    {
        for (auto& c : coil)
        {
            if (c && c->coilFields.count("id") && c->coilFields.at("id").get<int>() == position)
            {
                return c.get();
            }
        }
        return nullptr;
    }
    void Device::associate_CoilsWithWindings()
    {
        auto array = deviceFields["array_type"].get<Config::ArrayType>();

        if (array == Config::ArrayType::LH)
        {
            coil[0] ->assign_WindingDesc  (home_WindingDesc::LV);
            coil[1] ->assign_WindingDesc  (home_WindingDesc::HV);
        }
    }
    void Device::erase_Warnings()
    {
        for (int i = 0; i < ERRORMESSAGES; ++i)
        {
            deviceFields["sErrorName"]    .set("", i);
            deviceFields["tErrorEnum"].set(WarningSeverity::None, i);
            deviceFields["sErrorMessages"].set("", i);
        }
        cooling ->erase_Warnings();
        core    ->erase_Warnings();
    }
    void Device::calc_CoilCoil()
    {
        home_Winding* windHV = get_WindingHV();
        home_Winding* windLV = get_WindingLV();
        
        if(!windHV || !windLV || windHV->coils.empty() || windLV->coils.empty()) return;

        home_Coil* coilHV = windHV->coils[0];

        double coilCoilDuct         = coilHV->coilFields["dCoilCoilDuct"].get<double>();
        double coilCoilPacking      = coilHV->coilFields["dCoilCoilPacking"].get<double>();
        double coilCoilDuct2        = coilHV->coilFields["dCoilCoilDuct2"].get<double>();
        double overOLVRadialBuilt   = deviceFields["dOverOLVRadialBuilt"].get<double>();
        int bilHV                   = coilHV->coilFields["iBil"].get<int>();

        double result   = (coilCoilDuct > 0.0) ? (coilCoilDuct + 0.02) : 0.0;
        result          += coilCoilPacking;

        if (coilCoilDuct2 > 0.0)        result += (coilCoilDuct2 + 0.02);
        
        deviceFields["dCoilCoilRadialBuilt"].set(result);
        double coilCoilRadialBuilt  = result;
        result                      = safe_Division(static_cast<double>(bilHV), ((coilCoilRadialBuilt + 2.0 * overOLVRadialBuilt) * 25.4));
        deviceFields["dCoilCoilkvmm"].set(result);

        std::stringstream stringStream;

        if (coilCoilDuct > 0.0)
        {
           stringStream << std::fixed << std::setprecision(3) << coilCoilDuct << "'' Duct + "; 
        }
        stringStream << std::fixed << std::setprecision(3) << coilCoilPacking << "'' Packing";

        if (coilCoilDuct2 > 0.0)
        {
            stringStream << " + " << std::fixed << std::setprecision(3) << coilCoilDuct2 << "'' Duct "; 
        }
        deviceFields["sCoilCoilDescription"].set(stringStream.str());
    }

    int Device::get_WindingLastCoil() const
    {
        auto    lastCoil = get_LastCoil();
        return  lastCoil ? static_cast<int> (lastCoil->coilFields.at("tDescDevanado") .get<home_WindingDesc>()) : 0;
    }
    int Device::get_PositionAT() const
    {
        for (size_t i = 0; i < coil.size(); ++i)
        {
            auto description = coil[i]->coilFields.at("tDescDevanado")  .get<home_WindingDesc>();

            if (description == home_WindingDesc::HV || description == home_WindingDesc::RegHV)          {   return static_cast<int>(i);     }
        }

        return 0;
    }
    int Device::get_PositionBT() const
    {
        for (size_t i = 0; i < coil.size(); ++i)
        {
            auto desc = coil[i]-> coilFields.at("tDescDevanado") .get<home_WindingDesc>();
            if (desc == home_WindingDesc::LV || desc == home_WindingDesc::RegLV)            {   return static_cast<int>(i);     }
        }
        return 0;
    }

    bool Device::have_Tertiary() const          { return deviceFields.at("have_tertiary").get<bool>(); }
    bool Device::dual_Voltage()  const          { return deviceFields.at("dual_voltage") .get<bool>(); }

    double Device::get_Frequency() const          { return deviceFields.at("frequency")    .get<double>();           }
    double Device::calc_RBIWAux (home_Winding* windingHv, home_Winding* windingLv, home_Coil* coilHv, home_Coil* coilLv, int numberPhases)
    {

        double radialBuiltSidesLv   = coilLv->coilFields["dRadialBuiltSides"]   .get<double>();
        double radialBuiltSidesHv   = coilHv->coilFields["dRadialBuiltSides"]   .get<double>();
        double coilRadialBuilt      = deviceFields["dCoilCoilRadialBuilt"]      .get<double>();
        
        return radialBuiltSidesLv + radialBuiltSidesHv + coilRadialBuilt;
    }
}
// end of home_Device