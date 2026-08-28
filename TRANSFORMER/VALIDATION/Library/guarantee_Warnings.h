/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      GuaranteeWarnings.h
|    @brief     Auditor system evaluating calculated performance data against commercial guarantees.
|    @brief     Transformer::Validation     is "Quality Control"

|           - EVALUATES:    no-load loss per unit deviations from design standards
|           - VALIDATES:    calculated electrical impedance against minimum contractual thresholds
|           - VALIDATES:    maximum impedance limits, downgrading errors to warnings on default fields
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
    class GuaranteeWarnings
    {
        private:
            home_Guarantee*                                     guarantee;

        public:
            explicit                                            GuaranteeWarnings           (home_Guarantee* parent = nullptr);
            
            home_Guarantee*                                     getParentGuarantee          () const;

            home_WarningSeverity                                warningValue_NoLossPerUnit  (std::vector<double>& values);
            home_WarningSeverity                                warning_Impedance           (std::vector<double>& values);
            home_WarningSeverity                                warning_MaxImpedance        (std::vector<double>& values);

            inline void                                         add_ErrorMessage            (home_Guarantee* guarantee, const std::string& name, home_WarningSeverity warningSeverity, const std::string& warningMessage) {}
    };
}