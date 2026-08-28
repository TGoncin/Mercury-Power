/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      gradient_Warnings.h
|    @brief     Thermal evaluation manager auditing temperature gradients across transformer windings.
|    @brief     Transformer::Validation     is "Quality Control"

|           - EVALUATES:    final winding and individual coil thermal gradients
|           - VALIDATES:    temperature performance limits based on phase counts and cooling types (KNAN, etc.)
|           - VERIFICATION: protects structural insulation integrity against rated kVA over-temperature thresholds
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"



namespace Transformer::Validation
{
    class GradientWarnings
    {
        private:
            home_Gradients* gradients;

        public:
            explicit GradientWarnings(home_Gradients* parent = nullptr);

            home_Gradients* getParentDevice() const;

            home_WarningSeverity warning_ValueGradient       (int targetIndex, std::vector<double>& values);
            home_WarningSeverity warning_ValueGradientCoil   (int targetIndex, std::vector<double>& values);

            inline void add_ErrorMessage(home_Gradients* gradient, const std::string& name, home_WarningSeverity warningSeverity, const std::string& warningMsg){}
    };
}
//end of home_Cooling