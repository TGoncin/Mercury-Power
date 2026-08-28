/**___________________________________________________________________________________________________
|
|    @date      May 21, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      definitions.h
|    @brief     Global Constant Variables,     Transformer::Config     for 'System & Setup'
|___________________________________________________________________________________________________*/
#pragma once

namespace Transformer::Config
{
    constexpr int NUM_DOUBLE_INPUTS     = 10;
    constexpr int NUM_INTEGER_INPUTS    = 8;
    constexpr int NUM_TADS_INT_INPUTS   = 3;
    constexpr int NUM_TADS_DBL_INPUTS   = 5;
    constexpr int NUM_TADS_OUTPUTS      = 71;

    constexpr double    NO_LOAD_LOSS_FACTOR       = 1.025;                      // NO-LOAD LOSSES FACTOR
    constexpr double    LOAD_LOSS_FACTOR          = 1.065;                      // LOAD LOSSES FACTOR
    constexpr double    EXCITATION_CURRENT_FACTOR = 1.05;                       // EXCITATION CURRENT FACTOR

    constexpr int       MAX_TAPS       = 33;
    constexpr double    EPSILON       = 1e-6;                       // Small value for numerical stability
    constexpr double    PI_VAL        = 3.14159265358979323846;
    constexpr int       MAX_WINDINGS  = 7;
}