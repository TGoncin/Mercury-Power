/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      CoreWarnings.h
|    @brief     Comprehensive mathematical compliance engine analyzing magnetic core optimization metrics.
|    @brief     Transformer::Validation     is "Quality Control" 

|           - EVALUATES:    magnetic flux density violations (for traditional or amorphous laminations)
|           - VALIDATES:    physical stacking limits & dimensional steel-to-stack layout ratios
|           - VALIDATES:    Restricts overall core geometric footprint bounds against standards
|           - VERIFICATION: flags auditing electrical transformer core physics
|___________________________________________________________________________________________________*/
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"
#include "enums.h"

namespace Transformer::Components   {   class Core;     } // home_Core* alias used pointing to class Core 


namespace Transformer::Validation
{
    class CoreWarnings
    {
        private:
            home_Core* coreParent;
        public:
            explicit    CoreWarnings        (home_Core* parent = nullptr);
            virtual     ~CoreWarnings()     = default;

            std::unordered_map <std::string, home_FieldValue> CoreWarningfields;

            home_Core* getParentCore() const;

            home_WarningSeverity warning_FluxDensity                (std::vector <double>& values);
            home_WarningSeverity warning_FluxDensityMin             (std::vector <double>& values);
            home_WarningSeverity warning_TotalStackMax              (std::vector <double>& values); 
            home_WarningSeverity warning_RatioSteelWidthTotalStack  (std::vector <double>& values);
            home_WarningSeverity warning_MaxLength                  (std::vector <double>& values);
            home_WarningSeverity warning_MaxHeight                  (std::vector <double>& values);

            inline void add_ErrorMessage(home_Core* core, const std::string& name, home_WarningSeverity type, const std::string& warningMessage);
    };

}