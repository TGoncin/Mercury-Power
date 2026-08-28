/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      math_Utilities.h
|    @brief     Floating point comparisions, basic rounding.   
|    @brief     Transformer::Core::Math     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/
#pragma once

#include <string>

namespace Transformer::Config
{

    bool        real_Equals         (double x1,         double x2);
    bool        real_Less           (double x1,         double x2);
    bool        real_Greater        (double x1,         double x2);

    int         getStepCount        (double value,      double div);

    double      round_Decimal       (double value,      int decimal_Places);
    double      safe_Division       (double numerator,  double denominator,     double defaultValue = 0.0);
    double      linearInterpolation (double x1,         double y1,  double x2,  double y2,  double targetX);

    std::string format_Double       (double value,      int precision = 2);
}
// End of Transformer::Config::Math