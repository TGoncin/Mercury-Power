/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      LossesWarnings.h
|    @brief     Analytical calculation engine for structural, stray, and total transformer losses.
|    @brief     Transformer::Validation     is "Quality Control"

|           - EVALUATES:    calculated core (no-load) losses against contractual guarantees
|           - EVALUATES:    calculated load losses including stray and lead components
|           - VERIFICATION: total aggregated transformer loss boundaries to ensure grid spec compliance
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"

namespace Transformer::Core         { class Device;     class FieldValue;   }  
namespace Transformer::Config       { enum class WarningSeverity;   }
namespace Transformer::Physics      { class Losses; }
namespace Transformer::Commercial   { class Guarantee;  }
namespace Transformer::Commercial   { class Core;   }



namespace Transformer::Validation
{
    class LossWarnings
    {
        private:
            home_Losses* loss;

        public:
            explicit LossWarnings(home_Losses* parent = nullptr);

            home_Losses*         getParentDevice() const;
            home_WarningSeverity warning_ValueCoreLosses  (std::vector <double>& values);
            home_WarningSeverity warning_ValueLoadLosses  (std::vector <double>& values);
            home_WarningSeverity warning_ValueTotalLosses (std::vector <double>& values);
            inline void          add_ErrorMessage         (home_Losses* losses, const std::string& name, home_WarningSeverity warningSeverity, const std::string& WarningMsg) {};
    };
}
// end of LossWarnings class