/**___________________________________________________________________________________________________
|    @date      May 28, 2026
|    @author    Tom Goncin
|    @file      CoilWarnings.h
|    @brief     Manager of validation alerts, thresholds, & condition flags (transformer coil assemblies)
|    @brief     Transformer::Validation     is "Quality Control" 

|                   - INITIALIZING:     tracking maps of warning codes to specific FieldValues
|                   - BINDS:            safely to parent Coil object instance (via smart/raw ptr mapping
|                   - VALIDATES:        physical state issues preventing op anomalies
|                   - ERROR HANDLING:   structured & diagnostic warnings --> optimized coil config
|___________________________________________________________________________________________________*/

#include "alias.h"
using namespace Transformer::Types;

#include "coil_Warnings.h"




namespace Transformer::Validation
{

    CoilWarnings::CoilWarnings(Coil* parent) : parentCoil(parent) {}


    Coil* CoilWarnings::getParentDevice() const
    {
        if (this->parentCoil == nullptr)            throw std::runtime_error("This warnings object is not attached to any Coil");
        return this->parentCoil;
    }

    inline void errorMessage(Coil* coil, const std::string& name, home_WarningSeverity warningSeverity, const std::string& warningMsg) {}

}