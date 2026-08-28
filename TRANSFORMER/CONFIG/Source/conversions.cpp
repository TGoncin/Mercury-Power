/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      conversions.h
|    @brief     Conversion utilities  
|    @brief     Transformer::Core::Math     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/


#include <cmath>
#include <sstream>
#include <iomanip>

#include "conversions.h"



namespace Transformer::Config
{
    double convert_Inch_CM      ( double inches, int power)    { return inches *   std::pow (2.54, power); }
    double convert_CM_Inch      ( double centi,  int power)    { return centi  /   std::pow (2.54, power); }
    
    std::string format_Inches   (double value)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << value; 
        
        // Note: coil.cpp where its used appends the quote mark ("\""), just return # itself
        return oss.str(); };
}   // end of Transformer::Core::Math