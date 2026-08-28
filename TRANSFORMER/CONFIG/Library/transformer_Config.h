/**__________________________________________________________________________________________________________________________
|
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      transformerConfig.h
|    @brief     Global constant varaibles,           Transformer::Config     for 'System & Setup'
|___________________________________________________________________________________________________________________________*/
#pragma once

#include <string>
#include "alias.h"

using namespace Transformer::Types;


namespace Transformer::Config
{

//      Regulation Type
    constexpr int NOREG     = 0;        // No regulation applied
    constexpr int REG       = 1;        // IF regulation winding
    constexpr int TAPS      = 2;        // IF regulation winding


//      Conductor Type
    constexpr int TRIPLE        = 3; 
    constexpr int TWIN          = 2;   
    constexpr int CTC           = 1;    
    constexpr int FLAT_WIRE     = 0;  


//      Linear or Reversing
    constexpr int REV   = 0;    
    constexpr int LIN   = 1;    



/*__________________________________________________

|        FROM PHASE AMPERAGE FILE
____________________________________________________*/


//      Parameters
    constexpr int MAX_THICKNESSES   = 6;
    constexpr int NUM_VENT_DATA     = 27;
    constexpr int NOISE_TOLERANCE   = 2;


//      Insulation Stress limits
    constexpr double MAX_INTERLAYER_STRESS_C88 = 100.0; 
    constexpr double MAX_INTERLAYER_STRESS_1C2 = 200.0;
    constexpr double MAX_INTERLAYER_STRESS_C2  = 150.0;


//      Subversion Revision Info (Version control system)
    constexpr int SUBVERSION_REVISION_INT           = 2673;
    constexpr std::string_view SUBVERSION_REVISION  = "2673";

    inline void add_ErrorMessage(home_Conductor* conductor, const std::string& name, home_WarningSeverity type, const std::string& warningMsg){}

}