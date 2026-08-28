/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      device_Warnings.h
|    @brief     Compliance auditor for core ratings, naming syntax, & op efficiency.         
|    @brief     Transformer::Validation     is "Quality Control"

|           - PARSES:       data descriptions using strict regex
|           - VALIDATES:    overall op efficiency losses against guaranteed limits
|           - VERIFICATION: ensures manufacturing & performance tokens match specification
|___________________________________________________________________________________________________*/
#pragma once

#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"


namespace Transformer::Core     { class Device; }


namespace Transformer::Validation
{
    class DeviceWarnings
    {
        private:
            home_Device*                                    device;             

            static const std::regex                         DATA_DESC_REGEX;                    // Pre-compiled regex:  r"^_[A-Za-z]{2,4}_[A-Za-z]{3}_[0-9]{1,2},_[0-9]{2}; .+/$"     
            
            std::string                                     trim                        (const std::string& str) const;      // Trims leading & trailing whitespace from string

        public:
            explicit                                        DeviceWarnings              (home_Device* parent = nullptr);

            home_Device*                                    getParentDevice             () const;

            bool                                            validate_DataDescription    (const std::string& value) const;

            home_WarningSeverity                            warning_DataDescription     (int iIndex, std::vector <std::string>& values);
            home_WarningSeverity                            warning_Efficiency          (std::vector <double>& values);
            
            inline void                                     addErrorMessage             (home_Device* device, std::string name, home_WarningSeverity warningSeverity, std::string warningMessage) {}
    };
}
// end of DeviceWarnings class