/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      ConductorWarnings.h
|    @brief     Validator of current density limits & structural dimension tables for raw conductor layouts.
|    @brief     Transformer::Validation     is "Quality Control" 

|               - EVALUATES:    max & min current density constraints (based on material state)
|               - AUDITS:       strip width config against manufacturer tables & physical limits
|               - REJECTS:      warning structures to managing parent conductor framework
|               - RETURNS:      flags electrical conductor design infractions
|___________________________________________________________________________________________________*/
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"
#include "field_Value.h"




namespace Transformer::Validation
{
    class ConductorWarnings
    {
        public:
            std::unordered_map <std::string, home_FieldValue>   conductorFields;

            explicit                                            ConductorWarnings           (home_Conductor* parent = nullptr);
            virtual                                             ~ConductorWarnings          () = default;
            
            home_Conductor*                                     getConductorParent          () const;

            home_WarningSeverity                                warning_CurrentDensityMax   (std::vector<double>& values);
            home_WarningSeverity                                warning_CurrentDensityMin   (std::vector<double>& values);
            home_WarningSeverity                                warning_WidthTable          (std::vector<double>& values);
            home_WarningSeverity                                warning_Width               (std::vector<double>& values);

            inline void                                         errorMessage                (home_Conductor* conductor, const std::string& name, home_WarningSeverity type, const std::string& warningMsg);

        private:
            home_Conductor*                                     conductorParent;
    };
}