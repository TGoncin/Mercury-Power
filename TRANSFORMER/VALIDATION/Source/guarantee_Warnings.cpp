/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      gradient_Warnings.cpp
|    @brief     Thermal evaluation manager auditing temperature gradients across transformer windings.
|    @brief     Transformer::Validation     is "Quality Control"

|           - EVALUATES:    final winding and individual coil thermal gradients
|           - VALIDATES:    temperature performance limits based on phase counts and cooling types (KNAN, etc.)
|           - VERIFICATION: protects structural insulation integrity against rated kVA over-temperature thresholds
|___________________________________________________________________________________________________*/
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"

#include "device.h"
#include "guarantee.h"

#include "guarantee_Warnings.h"



namespace Transformer::Validation
{


    GuaranteeWarnings::GuaranteeWarnings (home_Guarantee* parent) : guarantee(parent) {}
    home_Guarantee* GuaranteeWarnings::getParentGuarantee() const
    {
        if (!guarantee)        throw std::runtime_error("This Guarantee is not attached to any warning");
        return guarantee;
    }
    home_WarningSeverity GuaranteeWarnings::warningValue_NoLossPerUnit (std::vector <double>& values)
    {
        home_Guarantee* guarantee       = getParentGuarantee();
        auto&           guaranteeField  = guarantee->guaranteeFields;

        double noLossPerUnit             = guaranteeField["dNL_pu"].get<double>();
        double standard_NoLossPerUnit    = guaranteeField["dNL_pu"].get<double>();
    
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        if (noLossPerUnit < standard_NoLossPerUnit)
        {
            warningSeverity = home_WarningSeverity::Warning;

            std::string warningMessage = "The transformer's no-load loss per unit("              + Config::format_Double(noLossPerUnit) 
                                         + ") is different from the guaranteed design standard(" + Config::format_Double(standard_NoLossPerUnit) 
                                         + ").";

            std::string name = "tWarningValueNLPU";
        
            add_ErrorMessage (guarantee, name, warningSeverity, warningMessage);

            values[0] = noLossPerUnit;
            values[1] = standard_NoLossPerUnit;
        }
        return warningSeverity;
    }
    home_WarningSeverity GuaranteeWarnings::warning_Impedance (std::vector <double>& values)
    {
        home_Guarantee* guarantee       = getParentGuarantee();
        auto&           guaranteeField  = guarantee->guaranteeFields;
        home_Device*    device          = guarantee->getParentDevice();

        double impedanceGuarantee  = guaranteeField         ["dIZGuarantee"].get<double>();
        double impedanceWithLeads  = device->deviceFields   ["dIZWithLeads"].get<double>();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;
    
        values[0] = 0.0;
        values[1] = 0.0;

        if (impedanceWithLeads < impedanceGuarantee)
        {
            warningSeverity = home_WarningSeverity::Error;

            std::string warningMessage =      "The calculated impedance("                   + Config::format_Double(impedanceWithLeads) 
                                            + ") is lower than the guaranteed impedance("   + Config::format_Double(impedanceGuarantee) 
                                            + ").";
            std::string name = "tErrorImpedance";
        
            add_ErrorMessage(guarantee, name, warningSeverity, warningMessage);

            values[0] = impedanceWithLeads;
            values[1] = impedanceGuarantee;
        }
        return warningSeverity;
    }
    home_WarningSeverity GuaranteeWarnings::warning_MaxImpedance (std::vector <double>& values)
    {
        home_Guarantee*  guarantee       = getParentGuarantee();
        auto&            guaranteeField  = guarantee->guaranteeFields;
        home_Device*     device          = guarantee->getParentDevice();
    
        bool   deviceField            = device->deviceFields["dImpedanceMax"].get<bool>();
        double impedanceGuarantee   = 0.0;
 
        if (deviceField)    {   impedanceGuarantee = ( guarantee->guaranteeFields ["dIZGuarantee"] .get<double>() < 5.0) ? 5.5 : 6.5;     }
        else                {   impedanceGuarantee =   device->deviceFields       ["dImpedanceMax"].get<double>();   }

        double impedance = device->deviceFields ["dIZWithLeads"].get<double>();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        if (impedance > impedanceGuarantee)
        {
            if (deviceField)        warningSeverity = home_WarningSeverity::None;
            else                    warningSeverity = home_WarningSeverity::Error;
        
            std::string warningMessage =    "The calculated impedance("                     + Config::format_Double(impedance) 
                                            +   ") is greater than the limit impedance("    + Config::format_Double(impedanceGuarantee) 
                                            +   ").";
            std::string name = "tWarningImpedanceMax";
        
            add_ErrorMessage (guarantee, name, warningSeverity, warningMessage);

            values[0] = impedance;
            values[1] = impedanceGuarantee;
        }
        return warningSeverity;
    }
}
// End of GuaranteeWarnings class