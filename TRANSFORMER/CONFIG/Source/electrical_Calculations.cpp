/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      electrical.h
|    @brief     Heavy engineering calculations with voltage, taps and phases.   
|    @brief     Transformer::Physics::Electrical     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/

#include <cmath>

#include "electrical_Calculations.h"

#include "math_Utilities.h"




namespace Transformer::Physics::Electrical
{

/*--------------------------------------------------------------------------------------
|      LIST GENERATOR: Line Voltage list generator: based on tap changer type, total taps, percentage deviations, base line voltage
|          Relation between voltage & turns 
|               --> & error compared to actual phase voltage
|---------------------------------------------------------------------------------------*/
//                                                          Line Voltage list generator: based on tap changer type, total taps, percentage deviations, base line voltage

int kVlineListGenerator( int tapChangerType, int total_Taps, double percent_Up, double percent_Down, double kVline, std::vector<double> &kV_lineList)
{
    int limit = 0;

    if (tapChangerType == 1 && total_Taps > 0)
    {
        double tapPercentageStep    = (percent_Up + percent_Down) / total_Taps;
        int tapsUp                  = Config::getStepCount(percent_Up, tapPercentageStep);
        int tapsDown                = Config::getStepCount(percent_Down, tapPercentageStep);
        limit                       = tapsUp + tapsDown;

        kV_lineList.assign(limit + 1, 0.0);

        int j = 1;

        for (int i = 0; i <= limit; ++i)
        {
            if (i < tapsUp)
            {
                double stepFactor   = tapsUp - i;
                kV_lineList[i]      = kVline * (((tapPercentageStep * stepFactor) / 100.0) + 1.0);
            }
            else if (i == tapsUp)                {      kV_lineList[i]      = kVline;   }
            else
            {
                kV_lineList[i]      = kVline * (1.0 - ((tapPercentageStep * j) / 100.0));
                j++;
            }
        }
    }
    else            kV_lineList.assign(1, kVline);
    return limit;
}



/*--------------------------------------------------------------------------------------
|      CALCULATION:
|          Phase voltage deviations
|               --> From: connection type, voltage levels, tap settings, whether meets VFVW scope
|---------------------------------------------------------------------------------------*/

void kVPhaseDef(int                             windingType,
                int                             connection,
                double                          kVline,
                const std::vector   <double>    &kV_lineList,
                int                             kVlineListLength,
                double                          kVlineLV,
                bool                            bMeetsVFVWTransformerScope,
                int                             kVlineListLengthVFVW,
                const std::vector   <double>    &d_RelNPos,
                int                             iNominalPos,
                const std::vector   <double>    &d_VTLV,
                double                          dTurns,
                std::vector         <double>    &kVphaseDefOutput,
                std::vector         <double>    &kVphaseDefOutputVFVW
                ) 
{
    if (connection == 1 || ( connection == 2 && windingType == 1))                      // STAR or AUTO-LV
    {

// ___________________ VFVW scope: CALCULATE: based on tap position effects on voltage fluctuations 
//                                                               --> in addition to connection type & voltage levels
        if ( bMeetsVFVWTransformerScope)   
        {
            for ( int i = 0; i <= kVlineListLength; ++i)
            {
                kVphaseDefOutput[ i] = ( windingType == 0) ? kV_lineList[ iNominalPos] / std::sqrt( 3.0) : kV_lineList[ i] / std::sqrt( 3.0);
            }

// ___________________ AUTO-LV: CALCULATE: VFVW phase voltage deviations based on relative nominal position & voltage levels
//                                                                --> considering tap position effects on voltage fluctuations
            if ( windingType == 1)  
            {
                for ( int i = 0; i <= kVlineListLengthVFVW; ++i)                { kVphaseDefOutputVFVW[ i] = ( kVlineLV * d_RelNPos[ i]) / std::sqrt( 3.0); }
            }
        }

// ___________________ NOT VFVW scope: CALCULATE: based on connection type & voltage levels 
//                                                                --> w/ considering tap position effects on voltage fluctuations
        else    
        {
            for ( int i = 0; i <= kVlineListLength; ++i)            { kVphaseDefOutput[ i] = kV_lineList[ i];   }
        }
    }

// ___________________ DELTA or AUTO-HV w/ VFVW scope: CALCULATE: phase voltage deviations based on line voltage list 
//                                                                --> w/o considering tap position effects on voltage fluctuations
    else            
    {
        if ( bMeetsVFVWTransformerScope && windingType == 2)                             // TERTIARY
        {
            for (int i = 0; i <= kVlineListLengthVFVW; ++i)         { kVphaseDefOutputVFVW[ i] = d_VTLV[ i] * dTurns / 1000.0;  }
        }
        if ( connection == 2 && windingType == 0)                                        // AUTO-HV
        {
            for ( int i = 0; i <= kVlineListLength; ++i)            { kVphaseDefOutput[ i] = (kV_lineList[ i] / std::sqrt( 3.0)) - (kVlineLV / std::sqrt( 3.0)); }
        }
        else                                                                            // DELTA or AUTO-HV with TERTIARY
        {
            for ( int i = 0; i <= kVlineListLength; ++i)            { kVphaseDefOutput[ i] = kV_lineList[ i]; }
        }
    }
}



/*--------------------------------------------------------------------------------------
|      CALCULATION:
|          Relation between voltage & turns 
|               --> & error compared to actual phase voltage
|---------------------------------------------------------------------------------------*/

void relationVoltTurns( double voltPerTurn, const std::vector<double> &kVPhaseHv, const std::vector<double> &turnsHv, int lengthHv, std::vector<double> &relationVolt, std::vector<double> &error) 
{
    int iterations = (lengthHv > 0) ? lengthHv + 1 : 1;

    for (int i = 0; i < iterations; ++i)
    {
        relationVolt[i] = voltPerTurn * turnsHv [i];
        error[i] = (kVPhaseHv[i] > 0) ? (Config::safe_Division( (kVPhaseHv[i] - relationVolt[i]) , kVPhaseHv[i] , 0.0 )) * 100.0 : 0.0;
    }
}


}       // end of Transformer::Physics::Electrical