/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      MechanicalWarnings.h
|    @brief     Enclosure geometry & mechanical clearances compliance engine.
|    @brief     Transformer::Validation     is "Quality Control

|           - EVALUATES:    override physical area thresholds against design baselines
|           - VALIDATES:    LV and HV Average Winding Rise (AWR) against nameplate limits
|           - VALIDATES:    internal tank maximum length, depth, and height footprints against standard specs
|___________________________________________________________________________________________________*/
#pragma once 

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"
#include "enums.h"


namespace Transformer::Validation
{
    class MechanicalWarnings
    {
        private:
            home_Mechanical*                                mechanical;

        public:
            explicit                                        MechanicalWarnings          (home_Mechanical* parent = nullptr);
            
            home_Mechanical*                                getParentDevice             () const;

            home_WarningSeverity                            warning_OverrideA           (std::vector <double>& values);
            home_WarningSeverity                            warning_AwrLV               (std::vector <double>& values);
            home_WarningSeverity                            warning_AwrHV               (std::vector <double>& values);
            home_WarningSeverity                            warning_IntTankMaxLength    (std::vector <double>& values);
            home_WarningSeverity                            warning_IntTankMaxDepth     (std::vector <double>& values);
            home_WarningSeverity                            warning_IntTankMaxHeight    (std::vector <double>& values);

            inline void                                     addErrorMessage             (home_Mechanical* mechanical, const std::string& name, home_WarningSeverity warningSeverity, const std::string& warningMsg) {}
            void                                            logError_Mechanical         (home_Mechanical* mechanical, const std::string& name, home_WarningSeverity warningType,     const std::string& msg);
    };
}   // end of class MechanicalWarnings