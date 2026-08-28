/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Changer.cpp
|    @brief     Manageer of Tap changer voltage configurations, step evaluations, & structural lead calculations.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|                   - Initialization and tracking of tap changer fields utilizing localized FieldValues.
|                   - Calculations for tap capacities, percentage steps, and nominal step ratios.
|                   - Resolves geometric electrical lead placements out of the top of the physical transformer coil.
|               Returns:
|                   - A localized, high-performance representation of the tap changer system.
|___________________________________________________________________________________________________*/
#include <iomanip>
#include <sstream>
#include <iostream>

#include "alias.h"
using namespace Transformer::Types;
#include "system_Config.h"
#include "math_Utilities.h"
#include "field_Value.h"
#include "enums.h"

#include "winding.h"
#include "device.h"
#include "coil.h"

#include "changer.h"


namespace Transformer::Components
{
    int Changer::id_Counter = 1;

    Changer::Changer(Winding* parentWinding) : winding(parentWinding)
    {
        changerFields["id"]                     = FieldValue (Changer::id_Counter);
        changerFields["name"]                   = FieldValue ("Changer " + std::to_string (Changer::id_Counter));
        changerFields["dPerUp"]                 = FieldValue (0.0);
        changerFields["dPerDown"]               = FieldValue (0.0);
        changerFields["iNumSteps"]              = FieldValue (0);
        changerFields["tCapacity"]              = FieldValue (static_cast<int> (ChangerCapacity::None));
        changerFields["sCapactity"]             = FieldValue ("");
        changerFields["dPerTaps"]               = FieldValue (0.0);
        changerFields["sPerTaps"]               = FieldValue ("");
        changerFields["sInstructions"]          = FieldValue ("");
        changerFields["tModelChanger"]          = FieldValue (static_cast<int> (ChangerModel::None));
        changerFields["dSeriesRatio"]           = FieldValue (1.00);
        changerFields["tType"]                  = FieldValue (static_cast<int> (ChangerType::CSC));
        changerFields["sType"]                  = FieldValue ("");
        changerFields["bExtraCircuit"]          = FieldValue (false);
        changerFields["tTypeLR"]                = FieldValue (static_cast<int> (LRType::Linear));
        changerFields["tTypeCBC"]               = FieldValue (static_cast<int> (CBCType::None));
        changerFields["iRedCap"]                = FieldValue (false);
        changerFields["sLeadsOutTOPCoilHeater"] = FieldValue (FieldValue::Value( (8, "-")) );
        changerFields["sLeadsOutTOPCoil"]       = FieldValue (FieldValue::Value( (8, "-")) );
        changerFields["sErrorName"]             = FieldValue (std::vector<std::string>  (ERRORMESSAGES, ""));
        changerFields["tErrorEnum"]             = FieldValue (std::vector<int>          (ERRORMESSAGES, static_cast<int>(WarningSeverity::None)));
        changerFields["sErrorMessages"]         = FieldValue (std::vector<std::string>  (ERRORMESSAGES, ""));

        Changer::id_Counter++;
    }
    void Changer::calc_Warnings()
    {
        erase_Warnings();
        int warningIndex = 0;

// CONDITION CHECK 1:   Tap & error in validation
        if (get_NumberSteps() > 0 && get_PerStepUp() == 0.0 && get_PerStepDown() == 0.0)
        {
            if (warningIndex < ERRORMESSAGES)
            {
                changerFields ["sErrorName"]    .set ("Missing Tap Percentages", warningIndex);
                changerFields ["tErrorEnum"]    .set (static_cast<int> (WarningSeverity::Error), warningIndex); 
                changerFields ["sErrorMessages"].set ("Changer has steps defined but 0% change per step.", warningIndex);
                warningIndex++;
            }
        }


// CONDITION CHECK 2:   is the capacity enum is still set to None/Default
        if (changerFields["tCapacity"].get<int>() == static_cast<int>(ChangerCapacity::None))
        {
            if (warningIndex < ERRORMESSAGES)
            {
                changerFields ["sErrorName"]        .set("Undefined Capacity", warningIndex);
                changerFields ["tErrorEnum"]        .set(static_cast<int> (WarningSeverity::Warning), warningIndex);
                changerFields ["sErrorMessages"]    .set("Changer capacity has not been explicitly defined.", warningIndex);

                warningIndex++;
            }
        }


// CONDITION 3 CHECK:   parent winding mismatch
        if (winding != nullptr && !winding->have_Taps() && get_NumberSteps() > 0)
        {
            if (warningIndex < ERRORMESSAGES)
            {
                changerFields["sErrorName"]     .set ("Winding Mismatch", warningIndex);
                changerFields["tErrorEnum"]     .set (static_cast<int> (WarningSeverity::Error), warningIndex);
                changerFields["sErrorMessages"] .set ("Changer expects taps, but parent winding has none.", warningIndex);

                warningIndex++;
            }
        }


// CONDITION 4 CHECK:   Series ratio check
        if (changerFields["dSeriesRatio"].get<double>() < 0.80) 
        {
            if (warningIndex < ERRORMESSAGES)
            {
                changerFields["sErrorName"]     .set ("Low Series Ratio", warningIndex);
                changerFields["tErrorEnum"]     .set (static_cast<int> (WarningSeverity::Warning), warningIndex);
                changerFields["sErrorMessages"] .set ("The series ratio is operating below standard thresholds.", warningIndex);

                warningIndex++;
            }
        }
    }
    Winding* Changer::getParentWinding() const 
    {
        if (winding == nullptr)                 {   throw std::runtime_error("This conductor is not attached to any Winding");  }
        return winding;
    }
    int    Changer::    get_NumberSteps() const
    {
       return changerFields .at("iNumSteps")   .get<int>();            
    }
    std::string Changer::get_Type() const 
    {
        std::stringstream ss;
        ss << taps_Sup() << "A," << taps_Sup() << "B@" << safe_Division(changerFields.at("dPerUp").get<double>(), taps_Sup()) << "%";

        return ss.str();
    }
    double Changer::    get_PerStepUp() const
    {
       return changerFields .at("dPerUp")      .get<double>() * 0.01;  
    }
    double Changer::    get_PerStepDown() const
    {
       return changerFields .at("dPerDown")    .get<double>() * 0.01;  
    }
    double Changer::taps_Sup() const
    {
        int num_steps = get_NumberSteps();
        if (num_steps > 0)                      {   return std::round((get_PerStepUp() / (get_PerStepUp() + get_PerStepDown())) * num_steps);   }
        return 0.0;
    }
    double Changer::taps_Inf() const
    {
        int num_steps = get_NumberSteps();
        if (num_steps > 0)                      {   return std::round((get_PerStepDown() / (get_PerStepUp() + get_PerStepDown())) * num_steps); }
        return 0.0;
    }
    void Changer::reset_ChangerFields()
    {
        for (auto& pair : changerFields)
        {
            try {
                pair.second.reset();
            }
            catch (...) {} // Silently catch to prevent terminal spam
        }
    }
    void Changer::erase_Warnings()
    {
        for (int i = 0; i < ERRORMESSAGES; ++i) 
        {
            changerFields ["sErrorName"]    .set("", i);
            changerFields ["tErrorEnum"]    .set(static_cast<int> (WarningSeverity::None), i); 
            changerFields ["sErrorMessages"].set("", i);
        }
    }
    void Changer::calc_Changer()
    {
        calc_DefaultCapacity();
        calc_LeadsOutTopCoil();
        calc_Instructions();
        calc_PerTaps();
    }
    void Changer::calc_DefaultCapacity()
    {
        ChangerCapacity capcacity = changerFields["tCapacity"].get<ChangerCapacity>();
        if (capcacity != ChangerCapacity::None)
        {
            changerFields["sCapactity"].set(std::string(to_string(capcacity)));
        }
    }
    void Changer::calc_LeadsOutTopCoil()
    {
        double        turn_AB = 0.0;
        home_Winding* winding = getParentWinding();

        if ( !winding->have_Taps() )        { return;   }

        auto   device               = winding->getParentDevice();
        int    numberPositions      = changerFields                     ["iNumSteps"]    .get<int>() + 1;
        auto   kVPhaseDef           = winding->windingFields            ["d_kVPhaseDef"] .get <std::vector <double>>();
        auto   turns                = winding->windingFields            ["turns"]        .get<std::vector <double>>();
        double totalLayers          = winding-> coils[0]-> coilFields   ["dTotLayers"]   .get<double>();
        double turnsPerLayerBody    = winding-> coils[0]-> coilFields   ["iTurnsLayers"] .get<double>();
        double turnsPerLayerUnder   = 0.0;
        double numberLayersUnder    = 0.0;
        double correctTurnsLayersHV = winding-> coils[0]-> conductor[0].conductorFields ["dCorrTurnsLyrs"] .get<double>();
        double correctLayersHV      = winding-> coils[0]-> conductor[0].conductorFields ["dCorrLyrs"]      .get<double>();
        int    numberPhases         = device-> deviceFields ["numberPhases"]    .get<int>();

        if (numberPhases == 1)
        {
            if (numberPositions > 1) 
            {
                if ((totalLayers / 2.0) > std::floor(totalLayers / 2.0)) 
                {
                    turn_AB = ( std::floor(totalLayers / 2.0 + 1.0) - numberLayersUnder ) 
                            *   turnsPerLayerBody 
                            + ( numberLayersUnder * turnsPerLayerUnder );
                }
                else
                {
                    turn_AB = ( totalLayers / 2.0 - numberLayersUnder ) 
                            *   turnsPerLayerBody 
                            + ( numberLayersUnder * turnsPerLayerUnder );
                }
            }
        }
        else
        {
            int centerLapLay = 0;

            if (correctTurnsLayersHV == static_cast<int>(correctTurnsLayersHV)) 
            {
                double half_layers = correctLayersHV / 2.0;
            
                if (half_layers > static_cast<int>(half_layers))        centerLapLay = static_cast<int> (half_layers + 1.0);
                else                                                    centerLapLay = static_cast<int> (half_layers);
            }
            else        centerLapLay = static_cast<int>(std::ceil(correctLayersHV / 2.0 / 2.0) * 2.0);

            turn_AB = centerLapLay * correctTurnsLayersHV;
        }

 

        changerFields ["sLeadsOutTOPCoil"]      .set(format_Double(turn_AB, 0), 3);
        changerFields ["sLeadsOutTOPCoilHeater"].set(turn_AB != 0.0 ? "A" : "-", 3);
        changerFields ["sLeadsOutTOPCoil"]      .set(format_Double(turn_AB, 0), 4);
        changerFields ["sLeadsOutTOPCoilHeater"].set(turn_AB != 0.0 ? "B" : "-", 4);

        double dResult = 0.0;
        
// POSITION 2 CHECK

        if (numberPositions > 1 && kVPhaseDef[1] > 0)
        {
            dResult = turn_AB - (turns[0] - turns[1]);
            changerFields ["sLeadsOutTOPCoil"]       .set(format_Double(dResult, 0), 2);
            changerFields ["sLeadsOutTOPCoilHeater"] .set("C", 2);
        }
        else
        {
            changerFields ["sLeadsOutTOPCoil"]      .set("-", 2);
            changerFields ["sLeadsOutTOPCoilHeater"].set("-", 2);
        }

// POSITION 5 CHECK
        if (numberPositions > 2 && kVPhaseDef[2] > 0)
        {
            dResult = dResult + turns[0] - turns[2];
            changerFields["sLeadsOutTOPCoil"].set(format_Double(dResult, 0), 5);
            changerFields["sLeadsOutTOPCoilHeater"].set("D", 5);
        }
        else
        {
            changerFields["sLeadsOutTOPCoil"].set("-", 5);
            changerFields["sLeadsOutTOPCoilHeater"].set("-", 5);
        }

    // POSITION 1 CHECK
        if (numberPositions > 3 && kVPhaseDef[3] > 0)
        {
            dResult = dResult - (turns[0] - turns[3]);
            changerFields["sLeadsOutTOPCoil"].set(format_Double(dResult, 0), 1);
            changerFields["sLeadsOutTOPCoilHeater"].set("E", 1);
        } 
        else
        {
            changerFields["sLeadsOutTOPCoil"].set("-", 1);
            changerFields["sLeadsOutTOPCoilHeater"].set("-", 1);
        }

    // POSITION 6 CHECK
        if (numberPositions > 4 && kVPhaseDef[4] > 0)
        {
            dResult = dResult + turns[0] - turns[4];
            changerFields["sLeadsOutTOPCoil"].set(format_Double(dResult, 0), 6);
            changerFields["sLeadsOutTOPCoilHeater"].set("F", 6);
        }
        else
        {
            changerFields["sLeadsOutTOPCoil"].set("-", 6);
            changerFields["sLeadsOutTOPCoilHeater"].set("-", 6);
        }

    // POSITION 0 CHECK
        if (numberPositions > 5 && kVPhaseDef[5] > 0)
        {
            dResult = dResult - (turns[0] - turns[5]);
            changerFields["sLeadsOutTOPCoil"].set(format_Double(dResult, 0), 0);
            changerFields["sLeadsOutTOPCoilHeater"].set("G", 0);
        }
        else
        {
            changerFields["sLeadsOutTOPCoil"].set("-", 0);
            changerFields["sLeadsOutTOPCoilHeater"].set("-", 0);
        }

    // POSITION  7 CHECK
        if (numberPositions > 6 && kVPhaseDef[6] > 0)
        {
            dResult = dResult + turns[0] - turns[6];
            changerFields["sLeadsOutTOPCoil"].set(format_Double(dResult, 0), 7);
            changerFields["sLeadsOutTOPCoilHeater"].set("H", 7);
        }
        else
        {
            changerFields["sLeadsOutTOPCoil"].set("-", 7);
            changerFields["sLeadsOutTOPCoilHeater"].set("-", 7);
        }
    }
    void Changer::calc_Instructions()
    {
        Winding* winding        = getParentWinding();
        auto     device         = winding->getParentDevice();
        int      numberPhases   = device->deviceFields["numberPhases"].get<int>();

        if (numberPhases == 1)          {   changerFields["sInstructions"].assign_Default("Tap Leads Out TOP of Coil");     }
        else                            {   changerFields["sInstructions"].assign_Default("");                              }
    }
    void Changer::calc_PerTaps()
    {
        int     iNumSteps   = changerFields ["iNumSteps"]  .get<int>();
        double  perUp       = changerFields ["dPerUp"]     .get<double>();
        double  perDown     = changerFields ["dPerDown"]   .get<double>();
        double  perC        = (perUp + perDown)  / iNumSteps;

        std::vector<double> array;

        for (int i = 0; i <= iNumSteps; ++i)        { array.push_back((-1.0) * perDown + perC * i); }

        std::set<double> used;

        std::vector <std::string> result;

        for (double v : array)
        {
            if (v == 0.0 || used.count(v))      continue;

            bool has_neg = std::find(array.begin(), array.end(), -v) != array.end();            // Does reciprocal exist in the array sequence

            std::stringstream ss;

            if (has_neg)
            {
                ss << "+/-" << std::fixed << std::setprecision(1) << std::abs(v) << "%";
                result.push_back(ss.str());
                used.insert(v);
                used.insert(-v);
            } 
            else
            {
                ss << std::fixed << std::setprecision(1) << v << "%";
                result.push_back(ss.str());
                used.insert(v);
            }
        }

        std::string sresult = "";

        for (size_t i = 0; i < result.size(); ++i)
        {
            sresult += result[i];
            if (i != result.size() - 1)                sresult += ",";
        }

        changerFields["dPerTaps"].assign_Default(perC);
        changerFields["sPerTaps"].assign_Default(sresult);
    }
}   
// end of Transformer::Components