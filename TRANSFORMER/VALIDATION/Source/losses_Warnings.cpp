/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      LossWarnings.cpp
|    @brief     Analytical calculation engine for structural, stray, and total transformer losses.
|    @brief     Transformer::Validation     is "Quality Control"

|           - EVALUATES:    calculated core (no-load) losses against contractual guarantees
|           - EVALUATES:    calculated load losses including stray and lead components
|           - VERIFICATION: total aggregated transformer loss boundaries to ensure grid spec compliance
|___________________________________________________________________________________________________*/

#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"
#include "field_Value.h"

#include "losses.h"
#include "device.h"
#include "guarantee.h"
#include "core.h"

#include "losses_Warnings.h"





namespace Transformer::Validation
{
    LossWarnings::LossWarnings(home_Losses* parent) : loss(parent) {}

    home_Losses* LossWarnings::getParentDevice() const
    {
        if (!loss)         throw std::runtime_error("This conductor is not attached to any Device");
        return loss;
    }

    home_WarningSeverity LossWarnings::warning_ValueCoreLosses(std::vector <double>& values)
    {
        home_Losses*    losses      = getParentDevice();
        auto            lossFields  = losses->lossFields; 
        home_Device*    device      = losses->getParentDevice();

        home_WarningSeverity warningSeverity  = home_WarningSeverity::None;
        values[0]         = 0.0;
        values[1]         = 0.0;

        double calcLoss      = device->core->coreFields             ["dCoreLoss"]   .get<double>();
        double guaranteeLoss = device->guarantee->guaranteeFields   ["dNLGuarantee"].get<double>();

        if ( (guaranteeLoss > 0.0 ) && ( calcLoss > guaranteeLoss ) )
        {
            warningSeverity = home_WarningSeverity::Error;
            std::string warningMsg =    "The calculated core losses("       + Config::format_Double(calcLoss) 
                                    +   ") exceed the guaranteed value("    + Config::format_Double(guaranteeLoss) 
                                    +   ").";
            std::string name = "tErrorCoreLosses";
            
            values[0] = calcLoss;
            values[1] = guaranteeLoss;
            add_ErrorMessage(losses, name, warningSeverity, warningMsg);
        }

        return warningSeverity;
    }

    home_WarningSeverity LossWarnings::warning_ValueLoadLosses (std::vector <double>& values)
    {
        home_Losses*         losses          = getParentDevice();
        auto                 lossField       = losses->lossFields;
        home_Device*         device          = losses->getParentDevice();
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        double calcLoss      = lossField["dTotalLLossStrayLead"]                 .get<double>();
        double guaranteeLoss = device->guarantee->guaranteeFields["dLLGuarantee"].get<double>();

        if (guaranteeLoss > 0.0 && calcLoss > guaranteeLoss)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg =    "The calculated load losses("                       + Config::format_Double(calcLoss) 
                                    +   ") are greater than the guaranteed load losses("    + Config::format_Double(guaranteeLoss) 
                                    +   ").";
            std::string name = "tErrorLoadLosses";
            
            values[0] = calcLoss;
            values[1] = guaranteeLoss;

            add_ErrorMessage(losses, name, warningSeverity, warningMsg);
        }

        return warningSeverity;
    }

    home_WarningSeverity LossWarnings::warning_ValueTotalLosses (std::vector <double>& values)
    {
        home_Losses*         losses          = getParentDevice();
        home_Device*         device          = losses->getParentDevice();
        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        double calcLoss             = losses->lossFields                 ["dTotalLLossStrayLead"].get<double>();
        double guaranteeLoss        = device->guarantee->guaranteeFields ["dLLGuarantee"]        .get<double>();
        double calcLossCore         = losses->lossFields                 ["dCoreLoss"]           .get<double>();
        double guaranteeLossCore    = device->guarantee->guaranteeFields ["dNLGuarantee"]        .get<double>();

        if ((guaranteeLoss > 0.0 || guaranteeLossCore > 0.0) && (calcLoss + calcLossCore) > (guaranteeLoss + guaranteeLossCore))\
        {
            warningSeverity = home_WarningSeverity::Error;
            std::string warningMsg =    "The calculated total losses("                      + Config::format_Double(calcLoss + calcLossCore) 
                                    +   ") are greater than the guaranteed total losses("   + Config::format_Double(guaranteeLoss + guaranteeLossCore) 
                                    +   ").";
            std::string name = "tErrorLoadLosses"; 
            
            values[0] = calcLoss + calcLossCore;
            values[1] = guaranteeLoss + guaranteeLossCore;

            add_ErrorMessage(losses, name, warningSeverity, warningMsg);
        }

        return warningSeverity;
    }
}
// end of the LossWarnings class