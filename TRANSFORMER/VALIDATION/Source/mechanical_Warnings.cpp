/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      MechanicalWarnings.cpp
|    @brief     Enclosure geometry & mechanical clearances compliance engine.
|    @brief     Transformer::Validation     is "Quality Control

|           - EVALUATES:    override physical area thresholds against design baselines
|           - VALIDATES:    LV and HV Average Winding Rise (AWR) against nameplate limits
|           - VALIDATES:    internal tank maximum length, depth, and height footprints against standard specs
|___________________________________________________________________________________________________*/
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"
#include "materials.h"
#include "conversions.h"

#include "device.h"
#include "mechanical.h"
#include "winding.h"
#include "coil.h"
#include "guarantee.h"

#include "mechanical_Warnings.h"




namespace Transformer::Validation
{
    MechanicalWarnings::MechanicalWarnings (home_Mechanical* parent) : mechanical(parent) {}
    
    void MechanicalWarnings::logError_Mechanical (home_Mechanical* mechanical, const std::string& name, home_WarningSeverity warningType, const std::string& msg)
    {    
        mechanical->mechanicalFields["sErrorMessages"]  .set(msg);
        mechanical->mechanicalFields["tErrorEnum"]      .set(static_cast<int>(warningType));
    }
    
    home_Mechanical* MechanicalWarnings::getParentDevice() const
    {
        if (!mechanical)        throw std::runtime_error ("This warnings object is not attached to any Mechanical module");
        else                    return mechanical;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_OverrideA(std::vector<double>& values)
    {
        auto* mechanical = getParentDevice();
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;
        
        values.assign(2, 0.0);

        double  overRideAin     = mechanical->mechanicalFields["dOverrideAin"]  .get<double>();
        bool    boolOverRideAin = mechanical->mechanicalFields["dOverrideAin"]  .get<bool>();
        double  ain             = mechanical->mechanicalFields["dAin"]          .get<double>() - 3.0;
        
        if (overRideAin < ain && !boolOverRideAin)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg = "The override("                                    + Config::format_Double(overRideAin) 
                                   + "in2) value must be greater than or equal to the area(" + Config::format_Double(ain) 
                                   + "in2).";
            
            addErrorMessage(mechanical, "tErrorOverrideA", warningSeverity, warningMsg);
            
            values[0] = overRideAin;
            values[1] = ain;
        }
        return warningSeverity;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_AwrLV(std::vector<double>& values)
    {
        auto* mechanical = getParentDevice();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values.assign(2, 0.0);
        
        bool   calculoWithRad  = mechanical->mechanicalFields["calculateWithRad"].get<bool>();
        double namePlateRise   = calculoWithRad ? (mechanical->mechanicalFields["dAWRMAX"]       .get<double>() - 3.0) 
                                                : (mechanical->mechanicalFields["dNameplateRise"].get<double>() - 3.0);
        
        if (calculoWithRad)
        {
            auto awrLv = mechanical->mechanicalFields["dAWRLV0"].get<std::vector<double>>();

            if (awrLv.size() >= 2)
            {
                if (awrLv[0] > namePlateRise || awrLv[1] > namePlateRise)
                {
                    double aux             = (awrLv[0] > namePlateRise) ? awrLv[0] : awrLv[1];
                    warningSeverity        = home_WarningSeverity::Error;
                    
                    std::string warningMsg = "AWR LV("                                + Config::format_Double(awrLv[0]) 
                                           + "C/"                                     + Config::format_Double(awrLv[1]) 
                                           + "C) must not exceed the nameplate rise(" + Config::format_Double(namePlateRise) + "C).";
                    
                    addErrorMessage(mechanical, "tErrorAWRLV", warningSeverity, warningMsg);

                    values[0] = aux;
                    values[1] = namePlateRise;
                }
            }
        } 
        else
        {
            double awrLv = mechanical->mechanicalFields["dAWR_LV"].get<double>();

            if (awrLv > namePlateRise)
            {
                warningSeverity        = home_WarningSeverity::Error;
                std::string warningMsg = "AWR LV("                                + Config::format_Double(awrLv) 
                                       + "C) must not exceed the nameplate rise(" + Config::format_Double(namePlateRise) + "C).";

                addErrorMessage(mechanical, "tErrorAWRLV", warningSeverity, warningMsg);

                values[0] = awrLv;
                values[1] = namePlateRise;
            }
        }
        return warningSeverity;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_AwrHV(std::vector<double>& values)
    {
        auto* mechanical                     = getParentDevice();
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values.assign(2, 0.0);
        
        bool calculateWithRad = mechanical->mechanicalFields                    ["calculateWithRad"].get<bool>();
        double namePlateRise  = calculateWithRad ? (mechanical->mechanicalFields["dAWRMAX"]         .get<double>() - 3.0) 
                                                 : (mechanical->mechanicalFields["dNameplateRise"]  .get<double>() - 3.0);
        
        if (calculateWithRad)
        {
            auto awrHv = mechanical->mechanicalFields["dAWRHV0"].get<std::vector<double>>();

            if (awrHv.size() >= 2)
            {
                if (awrHv[0] > namePlateRise || awrHv[1] > namePlateRise) 
                {
                    double aux              = (awrHv[0] > namePlateRise) ? awrHv[0] : awrHv[1];
                    warningSeverity         = home_WarningSeverity::Error;
                    
                    std::string warningMsg  = "AWR HV("                                     + Config::format_Double(awrHv[0]) 
                                              + "C/"                                        + Config::format_Double(awrHv[1]) 
                                              + "C) must not exceed the nameplate rise("    + Config::format_Double(namePlateRise) 
                                              + "C).";
                    
                    addErrorMessage(mechanical, "tErrorAWRHV", warningSeverity, warningMsg);

                    values[0] = aux;
                    values[1] = namePlateRise;
                }
            }
        }
        else
        { 
            double awrHv = mechanical->mechanicalFields["dAWR_HV"].get<double>();

            if (awrHv > namePlateRise) 
            {
                warningSeverity        = home_WarningSeverity::Error;
                std::string warningMsg = "AWR HV("                                + Config::format_Double(awrHv) 
                                       + "C) must not exceed the nameplate rise(" + Config::format_Double(namePlateRise) + "C).";

                addErrorMessage(mechanical, "tErrorAWRHV", warningSeverity, warningMsg);

                values[0] = awrHv;
                values[1] = namePlateRise;
            }
        }
        return warningSeverity;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_IntTankMaxLength(std::vector<double>& values)
    {
        auto* mechanical = getParentDevice();
        auto* device     = mechanical->getParentDevice();
        auto  designSpec = device->guarantee->guaranteeFields["tDesignSpec"].get<home_DesignSpecStandards>();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values.assign(2, 0.0);

        if (designSpec != home_DesignSpecStandards::C227306 && designSpec != home_DesignSpecStandards::C227406 && 
            designSpec != home_DesignSpecStandards::C227421 && designSpec != home_DesignSpecStandards::C227508 &&
            designSpec != home_DesignSpecStandards::C301106 && designSpec != home_DesignSpecStandards::C301206)
        {
            return warningSeverity;
        }

        double nominalKva   = device->deviceFields["nominal_kva"].get<double>();
        auto*  windingLv    = device->get_WindingLV();
        auto*  windingHv    = device->get_WindingHV();    
        double bil_Lv       = windingLv->coils[0]->coilFields["iBil"].get<double>();
        double bil_Hv       = windingHv->coils[0]->coilFields["iBil"].get<double>();
        double bilMax       = std::max(bil_Lv, bil_Hv);
        
        double iTankIntLarge = Config::convert_CM_Inch(mechanical->mechanicalFields["iTankIntLarge"].get<double>() / 10.0, 1);
        double limit         = iTankIntLarge;

        if      (nominalKva <= 75   && nominalKva <= 300)                   {  if (bilMax <= 95)  limit = 54.125;  }
        else if (nominalKva >= 75   && nominalKva <= 500)                   {  if (bilMax <= 125) limit = 54.125;  }
        else if (nominalKva >= 500  && nominalKva <= 1000 && bilMax <= 125) {  limit = 64.125;  }
        else if (nominalKva >= 1500 && nominalKva <= 3000 && bilMax <= 125) {  limit = 64.125;  }

        if (iTankIntLarge > limit)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg = "The internal tank length "                    + Config::format_Double(iTankIntLarge) 
                                   + " exceeds the allowable limit "                + Config::format_Double(limit) 
                                   + " based on standard criteria for nominal kVA " + Config::format_Double(nominalKva) 
                                   + " and BIL "                                    + Config::format_Double(bilMax) + ".";

            values[0] = iTankIntLarge;
            values[1] = limit;
            addErrorMessage(mechanical, "tErrorIntTankMaxLength", warningSeverity, warningMsg);
        }
        return warningSeverity;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_IntTankMaxDepth(std::vector<double>& values)
    {
        auto* mechanical = getParentDevice();
        auto* device     = mechanical->getParentDevice();
        auto designSpec  = device->guarantee->guaranteeFields["tDesignSpec"].get<home_DesignSpecStandards>();
        
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;
        values.assign(2, 0.0);

        if (designSpec != home_DesignSpecStandards::C227306 && designSpec != home_DesignSpecStandards::C227406 && 
            designSpec != home_DesignSpecStandards::C227421 && designSpec != home_DesignSpecStandards::C227508 &&
            designSpec != home_DesignSpecStandards::C301106 && designSpec != home_DesignSpecStandards::C301206)
        {
            return warningSeverity;
        }

        double nominalKva   = device->deviceFields["nominal_kva"].get<double>();
        auto*  windingLv    = device->get_WindingLV();
        auto*  windingHv    = device->get_WindingHV();    

        double bil_Lv       = windingLv->coils[0]->coilFields["iBil"].get<int>();
        double bil_Hv       = windingHv->coils[0]->coilFields["iBil"].get<int>();
        double bilMax       = std::max(bil_Lv, bil_Hv);
        double tankIntWidth = Config::convert_CM_Inch(mechanical->mechanicalFields["tankIntWidth"].get<double>() / 10.0, 1);
        double limit        = tankIntWidth;

        if      (nominalKva <= 75   && nominalKva <= 300)                   {  if (bilMax <= 95)  limit = 27.5;  }
        else if (nominalKva >= 75   && nominalKva <= 500)                   {  if (bilMax <= 125) limit = 29.5;  }
        else if (nominalKva >= 500  && nominalKva <= 1000 && bilMax <= 125) {  limit = 29.5;  }
        else if (nominalKva >= 1500 && nominalKva <= 3000 && bilMax <= 125) {  limit = 40.5;  }

        if (tankIntWidth > limit)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg = "The internal tank depth "                     + Config::format_Double(tankIntWidth) 
                                   + " exceeds the allowable limit "                + Config::format_Double(limit) 
                                   + " based on standard criteria for nominal kVA " + Config::format_Double(nominalKva) 
                                   + " and BIL "                                    + Config::format_Double(bilMax) 
                                   + ".";

            values[0] = tankIntWidth;
            values[1] = limit;
            addErrorMessage(mechanical, "tErrorIntTankMaxDepth", warningSeverity, warningMsg);
        }
        return warningSeverity;
    }
    
    home_WarningSeverity MechanicalWarnings::warning_IntTankMaxHeight(std::vector<double>& values)
    {
        auto* mechanical = getParentDevice();
        auto* device     = mechanical->getParentDevice();
        auto designSpec  = device->deviceFields["tDesignSpec"].get<home_DesignSpecStandards>();
        
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;
        values.assign(2, 0.0);

        if (designSpec != home_DesignSpecStandards::C227306 && designSpec != home_DesignSpecStandards::C227406 && 
            designSpec != home_DesignSpecStandards::C227421 && designSpec != home_DesignSpecStandards::C227508 &&
            designSpec != home_DesignSpecStandards::C301106 && designSpec != home_DesignSpecStandards::C301206)
        {
            return warningSeverity;
        }

        double nominalKva    = device->deviceFields["nominal_kva"].get<double>();
        auto*  windingLv     = device->get_WindingLV();
        auto*  windingHv     = device->get_WindingHV();    

        double bilLv         = windingLv->coils[0]->coilFields["iBil"].get<int>();
        double bilHv         = windingHv->coils[0]->coilFields["iBil"].get<int>();
        double bilMax        = std::max(bilLv, bilHv);
        double tankIntHeight = Config::convert_CM_Inch(mechanical->mechanicalFields["iTankIntHeight"].get<double>() / 10.0, 1);
        double limit         = tankIntHeight;

        if      (nominalKva <= 75   && nominalKva <= 300)                   {  if (bilMax <= 95)  limit = 57.875; }
        else if (nominalKva >= 75   && nominalKva <= 500)                   {  if (bilMax <= 125) limit = 57.875; }
        else if (nominalKva >= 500  && nominalKva <= 1000 && bilMax <= 125) {  limit = 64.0;   }
        else if (nominalKva >= 1500 && nominalKva <= 3000 && bilMax <= 125) {  limit = 70.0;   }

        if (tankIntHeight > limit)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg = "The internal tank height "                    + Config::format_Double(tankIntHeight) 
                                   + " exceeds the allowable limit "                + Config::format_Double(limit) 
                                   + " based on standard criteria for nominal kVA " + Config::format_Double(nominalKva) 
                                   + " and BIL "                                    + Config::format_Double(bilMax) + ".";

            values[0] = tankIntHeight;
            values[1] = limit;
            addErrorMessage(mechanical, "tErrorIntTankMaxHeight", warningSeverity, warningMsg);
        }
        return warningSeverity;
    }
}
// end of class MechanicalWarnings