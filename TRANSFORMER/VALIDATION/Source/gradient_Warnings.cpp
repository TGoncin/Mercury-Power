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

#include "gradient_Warnings.h"
#include "device.h"
#include "cooling.h"
#include "coil.h"
#include "gradient.h"
#include "core.h"
#include "winding.h"




namespace Transformer::Validation
{
    GradientWarnings::GradientWarnings (home_Gradients *parent) : gradients(parent) {}

    home_Gradients *GradientWarnings::getParentDevice() const
    {
        if (!gradients)        throw std::runtime_error("This conductor is not attached to any Device");
        return gradients;
    }

    home_WarningSeverity GradientWarnings::warning_ValueGradient(int targetIndex, std::vector<double> &values)
    {
        home_Gradients*      gradientsParent  = getParentDevice();
        auto&                gradientField   = gradientsParent -> gradientFields;
        home_Device*         device          = gradientsParent -> getParentDevice();

        int     numberPhases = static_cast<int>( device->deviceFields ["numberPhases"] .get<int>() );
        double  nominalKva   = gradientField ["nominal_kva"] .get<double>();

        home_WarningSeverity warningSeverity = home_WarningSeverity::None;

        values[0] = 0.0;
        values[1] = 0.0;

        int    index       = 0;
        int    windingType = -1;
        double gradient    = 0.0;

        for (auto& coil : device->coil)
        {
            if (index == targetIndex)       {   windingType = coil->getParentWinding()->get_WindingType();  }
            index++;
        }

        if (windingType != -1)
        {
            std::vector<double> gradientWindingList = gradientField["dGradientFinalWinding"].get<std::vector<double>>();

            if (windingType >= 0 && windingType < static_cast<int>(gradientWindingList.size()))
            {
                gradient = gradientWindingList[windingType];
            }
        }

        values[0]        = gradient;
        double gradLimit = (numberPhases == 1) ? 15.0 : (device->cooling->isKNAN() ? 17.0 : 15.0);
        bool   result    = ( gradient > gradLimit);

        if (result)
        {
            warningSeverity        = home_WarningSeverity::Error;
            std::string warningMsg =  "The gradient value "                             + Config::format_Double(gradient) 
                                      + "C exceeds the allowable limit for the rated "  + Config::format_Double(nominalKva) 
                                      + "kVA.";
            std::string name       = "tErrorGradientsWinding" + std::to_string(windingType);

            add_ErrorMessage (gradientsParent, name, warningSeverity, warningMsg);

            values[1] = gradLimit;
        }
        return warningSeverity;
    }

    home_WarningSeverity GradientWarnings::warning_ValueGradientCoil (int targetIndex, std::vector <double> &values)
    {
        home_Gradients* gradientsParent = getParentDevice();             //asking myself
        auto&           gradientField   = gradientsParent->gradientFields;
        auto*           device          = gradientsParent->getParentDevice();   // asking gradient

        int                  numberPhases       = static_cast<int> (device->deviceFields["numberPhases"].get<int>());
        home_WarningSeverity warningSeverity    = home_WarningSeverity::None;
        std::vector<double>  gradientFinalList  = gradientField ["dGradientFinal"].get<std::vector<double>>();
        values[0]       = 0.0;
        values[1]       = 0.0;
        double gradient = 0.0;

        if (targetIndex >= 0 && targetIndex < static_cast<int>(gradientFinalList.size()))       
        {
            gradient = gradientFinalList[targetIndex];
        }

        values[0]       = gradient;
        double gradLim  = (numberPhases == 1) ? 15.0 : (device->cooling->isKNAN() ? 17.0 : 15.0);
        bool result     = (gradient > gradLim);

        if (result)
        {
            warningSeverity         = home_WarningSeverity::Error;
            std::string warningMsg  = "The gradient value "                 + Config::format_Double(gradient) 
                                      +   "C exceeds the allowable limit "  + Config::format_Double(gradLim) 
                                      +   ".";
            std::string name        = "tErrorGradientsCoil" + std::to_string(targetIndex);

            add_ErrorMessage (gradientsParent, name, warningSeverity, warningMsg);
            values[1] = gradLim;
        }
        return warningSeverity;
    }
}
// end of home_GradientWarnings