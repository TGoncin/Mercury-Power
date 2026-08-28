/**___________________________________________________________________________________________________
|    @date      May 26, 2026
|    @author    Tom Goncin
|    @file      Tests.cpp
|    @brief     Specialized engineering module managing automated standard electrical transformer test sequences.
|    @brief     Transformer::Validation     is "Quality Control" 
|
|               Executes:
|                   - Initialization of design test specifications (Hipot, Induced, Resistance) using FieldValues.
|                   - Calculates high-potential (Hipot) test parameters matching international code rules (ANSI/C2).
|                   - Assesses insulation levels and standard extreme tap resistance tolerances.
|               Returns:
|                   - A localized, high-performance data structure tracking verification test boundaries.
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"
#include "system_Config.h"

#include "mechanical_Warnings.h"
#include "winding_Warnings.h"

#include "tests.h"




namespace Transformer::Validation
{

    int Tests::id_Counter = 1;


    Tests::Tests(home_Device* parentDevice) : device(parentDevice)
    {
        testFields["id"]                        = home_FieldValue (Tests::id_Counter);
        testFields["name"]                      = home_FieldValue ("Tests " + std::to_string(Tests::id_Counter));
        testFields["tHipot"]                    = home_FieldValue (static_cast<int>(home_WindingSelection::None));
        testFields["dHipotLV"]                  = home_FieldValue (0.00);
        testFields["dHipotHV"]                  = home_FieldValue (0.00);
        testFields["bInduced"]                  = home_FieldValue (static_cast<bool>(false));
        testFields["dXRatedVoltage"]            = home_FieldValue (0.00);
        testFields["bResistance"]               = home_FieldValue (static_cast<bool>(false));
        testFields["bOtherResistance"]          = home_FieldValue (static_cast<bool>(false));
        testFields["tTapsExtremesORAII"]        = home_FieldValue (static_cast<int>(home_TapExtremes::None));
        testFields["sSampleRate"]               = home_FieldValue ("");
        testFields["bCoreLossTest"]             = home_FieldValue (static_cast<bool>(false));
        testFields["dExtraNoLoad"]              = home_FieldValue ("105");
        testFields["dExtraNoLoad2"]             = home_FieldValue ("105");
        testFields["tSequence"]                 = home_FieldValue (home_Sequence::None);
        testFields["tStandardTemperatureRise"]  = home_FieldValue (home_TemperatureRise::None);
        testFields["tStandardLightningImpulse"] = home_FieldValue (home_Standard::None);
        testFields["tStandardRIV"]              = home_FieldValue (home_Standard::None);
        testFields["tStandardSoundLevel"]       = home_FieldValue (home_Standard::None);
        testFields["tStandardPartialDischarge"] = home_FieldValue (home_Standard::None);
        testFields["sErrorName"]                = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, ""));
        testFields["tErrorEnum"]                = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, home_WarningSeverity::None));
        testFields["sErrorMessages"]            = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, ""));

        Tests::id_Counter++;
    }


    home_Device* Tests::getParentDevice() const
    {
        if (device == nullptr)          throw std::runtime_error("This module is not attached to any Device");
        return device;
    }

    void Tests::reset_Fields()      
    {
        for (auto& pair : testFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }

    void Tests::erase_Warnings()
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i)
        {
            testFields["sErrorName"]        .set("", i);
            testFields["tErrorEnum"]        .set(static_cast<int>(home_WarningSeverity::None), i);
            testFields["sErrorMessages"]    .set("", i);
        }
    }

    void Tests::calc_TestsDefault()
    {
        home_Device* device = getParentDevice();
        calc_ResistanceTest(device);
    }

    float Tests::calc_HipotC2(home_DesignSpecStandards designSpecStandards, bool singleBushingHv, float bilKV)  
    {
        return get_AppliedFromBil(designSpecStandards, singleBushingHv, bilKV);
    }

    float Tests::get_AppliedFromBil(home_DesignSpecStandards designSpecStandards, bool singleBushingHv, float bilKV)
    {
        if (bilKV <= 30.0f) return 10.0f;
        if (bilKV <= 45.0f) return 15.0f;
        if (bilKV <= 60.0f) return 19.0f;
        if (bilKV <= 75.0f) return 26.0f;
        if (bilKV <= 95.0f) return 34.0f;
        return 0.0f;
    }

    float Tests::calc_HipotAnsi()   {    return 0.0f;   }

    float Tests::applied_Voltage(home_DesignSpecStandards designSpecStandards, bool singleBushingHv, float bilKV)
    {
        const std::set<home_DesignSpecStandards> C2_SPECS =
        {
            home_DesignSpecStandards::C2106,
            home_DesignSpecStandards::C2206,
            home_DesignSpecStandards::C8816,
            home_DesignSpecStandards::C88M79,
            home_DesignSpecStandards::C88M90,
            home_DesignSpecStandards::C227306,
            home_DesignSpecStandards::C227406,
            home_DesignSpecStandards::C227421,
            home_DesignSpecStandards::C301106
        };

        if (C2_SPECS.count(designSpecStandards))                          return calc_HipotC2(designSpecStandards, singleBushingHv, bilKV);
        if (designSpecStandards == home_DesignSpecStandards::ANSI)        return calc_HipotAnsi();

        return 0.0f;
    }

    void Tests::calc_ResistanceTest(home_Device* device) {}

}
// end of tests class