/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      math_Utilities.cpp
|    @brief     Floating point comparisions, basic rounding.   
|    @brief     Transformer::Core::Math     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/


#include <iomanip>
#include <cmath>
#include <sstream>

#include "math_Utilities.h"
#include "definitions.h"



namespace Transformer::Config
{
    bool    real_Equals    (double x1,         double x2)                                       { return std::abs (x1 -  x2)   <   EPSILON; }
    bool    real_Less      (double x1,         double x2)                                       { return ( x1 +  EPSILON)  <   x2; }
    bool    real_Greater   (double x1,         double x2)                                       { return   x1 >  (x2 +  EPSILON); }



    double safe_Division  (double numerator, double denominator, double defaultValue)
    {
        return ( denominator ==  0 )   ?   defaultValue :  numerator / denominator; 
    }



    double round_Decimal (double value, int decimal_Places)
    {
        double power = std::pow   (10.0, decimal_Places);
        return         std::round (value * power) / power;
    }


    double linearInterpolation (double x1, double y1, double x2, double y2, double targetX)    // Estimating an unknown value that falls between 2 values

    {
        double slope = safe_Division ( (y2 - y1), (x2 - x1), 0.0 );
        return slope * targetX + y2 - (slope * x2);
    }
   

    int getStepCount(double value, double div)       // Scales a # --> Extracts its decimal parts to decide how to round --> Applies Banker's Rounding (rounding halves to nearest even #)
    {
        if (div == 0)        return 0;

        double  scaledValue     = value / div;
        double  magnitude       = std::abs(scaledValue);
        int     integerParity   = static_cast<int> (scaledValue) % 2;
        int     fractionalPart  = static_cast<int> (magnitude * 100) % 100;
        double roundedResult;

        if (fractionalPart == 50)    {        roundedResult = (integerParity == 0)  ? std::floor(magnitude) : std::ceil(magnitude);    }
        else                         {        roundedResult = (fractionalPart < 50) ? std::floor(magnitude) : std::ceil(magnitude);    }

        return static_cast<int>(roundedResult);
    }



    std::string format_Double (double value, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << value;
        return stream.str();
    }
}
// End of Transformer::Config::Math 