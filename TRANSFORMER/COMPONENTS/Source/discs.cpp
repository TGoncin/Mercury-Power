/**___________________________________________________________________________________________________
|    @date      May 20, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Discs.cpp
|    @brief     Represents 2ndary, asymmetrical winding distribution strategy acting as fallback if 1st attempt fails to hit the target turn count.
|
|               PRIMARY EVENT - Symmetrical:
|                   - Block before SCLR
|                   - Attempts division of physical sections in perfect halves to balance turns
|                   - Then runs loop in attempt to achieve targest turns without breaking mechanical constraints
|
|               SCLR Block - Asymmetrical:
|                   - IF 50:50 fails to resolve, SCLR block shifts weight to 60:40 ratio        
|___________________________________________________________________________________________________*/

#include "alias.h"
using namespace Transformer::Types;

#include "discs.h"



namespace Transformer::Components
{

    int Discs::id_Counter = 0;

    Discs::Discs()
    {
        discFields["id"]               = home_FieldValue(++id_Counter);
        discFields["name"]             = home_FieldValue(std::string("Discs"));
        discFields["tSuccess"]         = home_FieldValue(false);
        discFields["iWedgeCount"]      = home_FieldValue(0);
        discFields["iSections1"]       = home_FieldValue(0);
        discFields["iTurnsPerSecInit"] = home_FieldValue(0);
        discFields["dTargetTurns"]     = home_FieldValue(0.0);
    }

    DiscsResult Discs::arrangeDiscTurns(double targetTurns, int wedgeCount, int sections_1, int turnsPerSection_init)
    {
        DiscsResult result;

        double accumulated_turns        = 0.0;
        double MinFraction             = 0.75 * wedgeCount;
        int max_fraction                = wedgeCount - 1;

        result.turnsPerSection [ 0 ] = turnsPerSection_init;
        int insulatedSectionPosition          = 0;             // TRANSLATION: Equivalent to i_pos_secc_aisl
        int maxSectionPosition                = 3;             // TRANSLATION: Equivalent to i_pos_max_secc

        int maxFraction_2             = max_fraction;
        int MinFraction_2             = static_cast <int> (std::floor(MinFraction));

        result.sections_2 [ insulatedSectionPosition ]         = sections_1;
        result.fractionalTurn [ insulatedSectionPosition ]    = maxFraction_2;


        double    turn_difference =   accumulated_turns 
                                        + ((result.turnsPerSection [ insulatedSectionPosition ] 
                                        + static_cast <double> (result.fractionalTurn [ insulatedSectionPosition ]) 
                                        / wedgeCount) * sections_1) - targetTurns;

        double turn_fraction = turn_difference - static_cast <int> (turn_difference);

        int leftoverTurnsCount = static_cast <int> (turn_difference);

        if (leftoverTurnsCount % 2 != 0 && leftoverTurnsCount >  1)
        {
            leftoverTurnsCount -= 1;
        }

//      S P E C I A L    C A S E                                                                |
//                                   max_fraction == MinFraction       & ONLY 1 LEFTOVER TURN  |
//______________________________________________________________________________________________|

        if (max_fraction == static_cast <int> (MinFraction) && leftoverTurnsCount == 1)
        {
            result.success = false;

            return result;
        }

//        C A S E :                                     |
//                     AVG TURNS PER SECTION = 1        |
//______________________________________________________|

        if (result.turnsPerSection [ insulatedSectionPosition ] == 1)
        {
            int leftoverSections = static_cast <int> (std::round(turn_difference * wedgeCount));

            while (leftoverSections >  sections_1 && maxFraction_2 >= MinFraction)
            {
                maxFraction_2      -= 1;
                result.fractionalTurn [ insulatedSectionPosition ] = maxFraction_2;

                turn_difference     = accumulated_turns 
                                    + ((result.turnsPerSection [ insulatedSectionPosition ] 
                                    + static_cast <double> (result.fractionalTurn [ insulatedSectionPosition ]) 
                                    / wedgeCount) 
                                    * sections_1) - targetTurns;
                
                leftoverSections   = static_cast <int> (turn_difference * wedgeCount);
            }

            double tempAccumulatedTurns = accumulated_turns;

            if (0  < leftoverSections && leftoverSections  < sections_1 && maxFraction_2 >  MinFraction)
            {
                result.sections_2       [ insulatedSectionPosition + 1 ]     = leftoverSections;
                result.turnsPerSection  [ insulatedSectionPosition + 1 ]     = 1;
                result.fractionalTurn   [ insulatedSectionPosition + 1 ]     = result.fractionalTurn [ insulatedSectionPosition ] - 1;
                result.sections_2       [ insulatedSectionPosition ]         -= leftoverSections;

                tempAccumulatedTurns  += ((result.turnsPerSection [ insulatedSectionPosition ] 
                                          +   static_cast <double> (result.fractionalTurn [ insulatedSectionPosition ]) 
                                          /   wedgeCount) 
                                          *   result.sections_2 [ insulatedSectionPosition ]);

                tempAccumulatedTurns  += ((result.turnsPerSection [ insulatedSectionPosition + 1 ] 
                                          +   static_cast <double> (result.fractionalTurn [ insulatedSectionPosition + 1 ]) 
                                          /   wedgeCount) 
                                          * result.sections_2 [ insulatedSectionPosition + 1 ]);

                result.success = realEquals(tempAccumulatedTurns, targetTurns);

                return result;
            }
            else
            {
                tempAccumulatedTurns  += ((result.turnsPerSection [ insulatedSectionPosition ] 
                                        + static_cast <double> (result.fractionalTurn [ insulatedSectionPosition ]) 
                                        / wedgeCount) 
                                        * result.sections_2 [ insulatedSectionPosition ]);

                result.success = realEquals(tempAccumulatedTurns, targetTurns);

                return result;
            }
        }

        result.sections_2       [ insulatedSectionPosition ] = sections_1 / 2;
        result.sections_2       [ insulatedSectionPosition + 1 ] = sections_1 - result.sections_2 [ insulatedSectionPosition ];
        result.turnsPerSection  [ insulatedSectionPosition + 1 ] = (result.turnsPerSection [ insulatedSectionPosition ] - 1 >= 1) 
                                                                  ? (result.turnsPerSection [ insulatedSectionPosition ] - 1) 
                                                                  : 1;

        for (int j = maxFraction_2; j >= MinFraction_2; --j)
        {
            result.fractionalTurn [ insulatedSectionPosition + 1 ]  = j;
            double tempAccumulatedTurns                             = accumulated_turns;

            for (int i = 0; i  <= maxSectionPosition; ++i)
            {
                int index = insulatedSectionPosition + i;

                if (result.sections_2 [ index ]  < 0)
                {
                    result.sections_2 [ index ]   = 0;
                    result.success              = false;

                    return result;
                }

                result.turns [ index ]    = (result.turnsPerSection [ index ] + static_cast <double> (result.fractionalTurn [ index ]) / wedgeCount) * result.sections_2 [ index ];
                tempAccumulatedTurns    += result.turns [ index ];
            }

            if (realEquals(tempAccumulatedTurns, targetTurns))
            {
                result.success = true;

                return result;
            }
            else
            {
                for (int i = 0; i  <= maxSectionPosition; ++i)
                {
                    result.turns [ insulatedSectionPosition + i ] = 0.0;
                }
            }
        }

//  2 N D    B L O C K :                                                            |
//                  Secondary, Asymmetrical Winding Distribution Strategy (SLCR)    |
//__________________________________________________________________________________|

        int factor = 4;
        result.sections_2       [ insulatedSectionPosition ]      = static_cast <int> (std::ceil(0.6 * sections_1 / factor))  * factor;
        result.sections_2       [ insulatedSectionPosition + 1 ]  = sections_1 - result.sections_2 [ insulatedSectionPosition ];
        result.turnsPerSection  [ insulatedSectionPosition + 1 ]  = (result.turnsPerSection [ insulatedSectionPosition ] - 1 >= 1)
                                                                    ? (result.turnsPerSection [ insulatedSectionPosition ] - 1)  
                                                                    : 1;

        for (int j = maxFraction_2; j >= MinFraction_2; --j)
        {
            result.fractionalTurn [ insulatedSectionPosition ]        = j;
            result.fractionalTurn [ insulatedSectionPosition + 1 ]    = j - 1;

            double tempAccumulatedTurns = accumulated_turns;

            for (int i = 0; i  <= maxSectionPosition; ++i)
            {
                int index = insulatedSectionPosition + i;

                if (result.sections_2 [ index ]  < 0)
                {
                    result.sections_2 [ index ]   = 0;
                    result.success                = false;

                    return result;
                }

                result.turns [ index ]  = (result.turnsPerSection [ index ] + static_cast <double> (result.fractionalTurn [ index ]) / wedgeCount) * result.sections_2 [ index ];
                tempAccumulatedTurns    += result.turns [ index ];
            }

            if (realEquals(tempAccumulatedTurns, targetTurns))
                {
                    result.success = true;
                    return result;
                }
            else
                {
                    for (int i = 0; i  <= maxSectionPosition; ++i)            { result.turns [ insulatedSectionPosition + i ] = 0.0; }
                }
        }

//  F I N A L   C A S E:        DIRECT ASSIGNMENT   |
//__________________________________________________|

        result.sections_2 [ insulatedSectionPosition ]      = sections_1;
        result.sections_2 [ insulatedSectionPosition + 1 ]  = 0;

        result.turns [ insulatedSectionPosition ] = (result.turnsPerSection [ insulatedSectionPosition ] 
                                                     +   static_cast <double> (result.fractionalTurn [ insulatedSectionPosition ]) 
                                                     /   wedgeCount) 
                                                     *   result.sections_2 [ insulatedSectionPosition ];

        accumulated_turns += result.turns [ insulatedSectionPosition ];

        result.success = realEquals(accumulated_turns, targetTurns);
        return result;
    }

}