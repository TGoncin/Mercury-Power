/**___________________________________________________________________________________________________
|    @date      May 28, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      CoilWarnings.h
|    @brief     Manager of validation alerts, thresholds, & condition flags (transformer coil assemblies)
|    @brief     Transformer::Validation     is "Quality Control" 
|
|                   - INITIALIZING:     tracking maps of warning codes to specific FieldValues
|                   - BINDS:            safely to parent Coil object instance (via smart/raw ptr mapping
|                   - VALIDATES:        physical state issues preventing op anomalies
|                   - ERROR HANDLING:   structured & diagnostic warnings --> optimized coil config
|___________________________________________________________________________________________________*/

#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>

#include "alias.h"
using namespace Transformer::Types;
#include "enums.h"
#include "field_Value.h"

#include "coil.h" 





namespace Transformer::Validation
{
    class Coil;
    class CoilWarnings
    {
        public:
            explicit    CoilWarnings        (Coil* parent = nullptr);
            virtual     ~CoilWarnings()     = default;
            Coil*       getParentDevice()   const;
            inline void errorMessage(Coil* coil, const std::string& name, home_WarningSeverity warningSeverity, const std::string& warningMsg); 
            std::unordered_map <std::string, Core::FieldValue> fields;
        private:
            Coil* parentCoil;
}   ;

}