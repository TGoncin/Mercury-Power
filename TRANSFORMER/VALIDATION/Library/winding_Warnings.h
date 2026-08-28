/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      WindingWarnings.h
|    @brief     Structural layout and electrical insulation engine for transformer windings.
|    @brief     Transformer::Validation     is "Quality Control"

|           - VALIDATES:    low-voltage turn parity (ensuring even turn counts on single-phase units)
|           - EVALUATES:    internal and outer cooling duct distributions relative to coil layers
|           - VERIFICATION: Basic Insulation Level (BIL) limits under specified grounding rules
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"


using namespace Transformer::Config;
using namespace Transformer::Core;
namespace Transformer::Components     {   class Winding;    class Coil;  }
namespace Transformer::Config         {   enum class WarningSeverity;   }


namespace Transformer::Validation
{
    class WindingWarnings
    {
        private:
            home_Winding* winding = nullptr;

        public:
            explicit WindingWarnings(home_Winding* windingParent = nullptr);

            home_Winding* getParentWinding() const;

            WarningSeverity warningTurnsLV      (std::vector <double>& values);
            WarningSeverity warningDuctsInner   (std::vector <double>& values);
            WarningSeverity warningDuctsOuter   (std::vector <double>& values);
            WarningSeverity warningBILUnder     (std::vector <double>& values);
            WarningSeverity warningLeadsDensity (std::vector <double>& values);

            inline void add_ErrorMessage(home_Winding* winding, const std::string& name, home_WarningSeverity type, const std::string& warningMsg){};
    };

}