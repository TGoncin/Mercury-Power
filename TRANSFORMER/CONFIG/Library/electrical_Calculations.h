/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      electrical.h
|    @brief     Heavy engineering calculations with voltage, taps and phases.   
|    @brief     Transformer::Physics::Electrical     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/
#pragma once

#include <vector>

namespace Transformer::Physics::Electrical
{
    //      Engineering Calculations

    int kVlineListGenerator( int    tapChangerType, 
                            int     total_Taps, 
                            double  percent_Up, 
                            double  percent_Down,
                            double  kVline, 
                            std::vector<double>& kV_lineList 
                            );              //  List of line voltages. Based on: Tap settings, % deviations


    void kVPhaseDef ( int                           windingType, 
                    int                             connection, 
                    double                          kVline, 
                    const std::vector  <double>&    kV_lineList, 
                    int                             kVlineListLength, 
                    double                          kVlineLV, 
                    bool                            bMeetsVFVWTransformerScope, 
                    int                             kVlineListLengthVFVW, 
                    const std::vector <double>&     d_RelNPos, 
                    int                             iNominalPos, 
                    const std::vector <double>&     d_VTLV, 
                    double                          dTurns, 
                    std::vector       <double>&     kVphaseDefOutput, 
                    std::vector       <double>&     kVphaseDefOutputVFVW 
                    );                  //  Phase V deviations. Based on: Connection type, voltage levels, tap settings


    void relationvoltturns( double                        VxT, 
                            const std::vector   <double>& kVphaseH,
                            const std::vector   <double>& turnsH,
                            int                           lengthH,
                            std::vector         <double>& relvolt,
                            std::vector         <double>& error 
                            );          //  Relation between voltage & turns --> & error compared to actual phase voltage

}       // end of Transformer::Physics::Electrical