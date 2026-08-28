/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      conversions.h
|    @brief     Conversion utilities  
|    @brief     Transformer::Core::Math     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/
#pragma once

#include <string>

namespace Transformer::Config
{
    double convert_Inch_CM      (double inches, int power);
    double convert_CM_Inch      (double centi, int power);
    std::string format_Inches   (double value);

}   