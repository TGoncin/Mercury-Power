/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Tests.h
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
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>
#include <set>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"
#include "enums.h"


namespace Transformer::Core         {    class Device;      class FieldValue; }

namespace Transformer::Validation
{
    class Tests
    {
        private:
            static int id_Counter;

        public:
            std::map <std::string, home_FieldValue>             testFields;
            home_Device*                                        device;

            explicit                                            Tests                   (home_Device* parentDevice = nullptr);

            home_Device*                                        getParentDevice         () const;

            void                                                reset_Fields            ();
            void                                                erase_Warnings          ();
            void                                                calc_ResistanceTest     (home_Device* device);
            void                                                calc_TestsDefault       ();

            float                                               calc_HipotC2            (home_DesignSpecStandards designSpecstandards, bool singleBushingHV, float bilKV);
            float                                               get_AppliedFromBil      (home_DesignSpecStandards designSpecstandards, bool singleBushingHV, float bilKV);
            float                                               calc_HipotAnsi          ();
            float                                               applied_Voltage         (home_DesignSpecStandards designSpecstandards, bool singleBushingHV, float bilKV);

        // _________________________________________________________________________________________
        // |                                      GETTERS                                          |
        // |_______________________________________________________________________________________|

            int                             getId                           () const { return testFields.at("id")                           .get<int>(); }
            std::string                     getName                         () const { return testFields.at("name")                         .get<std::string>(); }
            
            int                             getTHipot                       () const { return testFields.at("tHipot")                       .get<int>(); }
            double                          getDHipotLV                     () const { return testFields.at("dHipotLV")                     .get<double>(); }
            double                          getDHipotHV                     () const { return testFields.at("dHipotHV")                     .get<double>(); }
            bool                            getBInduced                     () const { return testFields.at("bInduced")                     .get<bool>();   }
            double                          getDXRatedVoltage               () const { return testFields.at("dXRatedVoltage")               .get<double>(); }
            bool                            getBResistance                  () const { return testFields.at("bResistance")                  .get<bool>();   }
            bool                            getBOtherResistance             () const { return testFields.at("bOtherResistance")             .get<bool>();   }
            int                             getTTapsExtremesORAII           () const { return testFields.at("tTapsExtremesORAII")           .get<int>(); }
            std::string                     getSSampleRate                  () const { return testFields.at("sSampleRate")                  .get<std::string>(); }
            bool                            getBCoreLossTest                () const { return testFields.at("bCoreLossTest")                .get<bool>();   }
            
            std::string                     getDExtraNoLoad                 () const { return testFields.at("dExtraNoLoad")                 .get<std::string>(); }
            std::string                     getDExtraNoLoad2                () const { return testFields.at("dExtraNoLoad2")                .get<std::string>(); }
            
            home_Sequence                   getTSequence                    () const { return testFields.at("tSequence")                    .get<home_Sequence>(); }
            home_TemperatureRise            getTStandardTemperatureRise     () const { return testFields.at("tStandardTemperatureRise")     .get<home_TemperatureRise>(); }
            home_Standard                   getTStandardLightningImpulse    () const { return testFields.at("tStandardLightningImpulse")    .get<home_Standard>(); }
            home_Standard                   getTStandardRIV                 () const { return testFields.at("tStandardRIV")                 .get<home_Standard>(); }
            home_Standard                   getTStandardSoundLevel          () const { return testFields.at("tStandardSoundLevel")          .get<home_Standard>(); }
            home_Standard                   getTStandardPartialDischarge    () const { return testFields.at("tStandardPartialDischarge")    .get<home_Standard>(); }
            
            std::vector<std::string>        getSErrorName                   () const { return testFields.at("sErrorName")                   .get<std::vector<std::string>>(); }
            std::vector<home_WarningSeverity> getTErrorEnum                 () const { return testFields.at("tErrorEnum")                   .get<std::vector<home_WarningSeverity>>(); }
            std::vector<std::string>        getSErrorMessages               () const { return testFields.at("sErrorMessages")               .get<std::vector<std::string>>(); }

        // _________________________________________________________________________________________
        // |                                      SETTERS                                          |
        // |_______________________________________________________________________________________|

            void setId                          (int v)                                     { testFields["id"]                          = home_FieldValue(v);  }
            void setName                        (const std::string& v)                      { testFields["name"]                        = home_FieldValue(v);  }
            
            void setTHipot                      (int v)                                     { testFields["tHipot"]                      = home_FieldValue(v);  }
            void setDHipotLV                    (double v)                                  { testFields["dHipotLV"]                    = home_FieldValue(v);  }
            void setDHipotHV                    (double v)                                  { testFields["dHipotHV"]                    = home_FieldValue(v);  }
            void setBInduced                    (bool v)                                    { testFields["bInduced"]                    = home_FieldValue(v);  }
            void setDXRatedVoltage              (double v)                                  { testFields["dXRatedVoltage"]              = home_FieldValue(v);  }
            void setBResistance                 (bool v)                                    { testFields["bResistance"]                 = home_FieldValue(v);  }
            void setBOtherResistance            (bool v)                                    { testFields["bOtherResistance"]            = home_FieldValue(v);  }
            void setTTapsExtremesORAII          (int v)                                     { testFields["tTapsExtremesORAII"]          = home_FieldValue(v);  }
            void setSSampleRate                 (const std::string& v)                      { testFields["sSampleRate"]                 = home_FieldValue(v);  }
            void setBCoreLossTest               (bool v)                                    { testFields["bCoreLossTest"]               = home_FieldValue(v);  }
            
            void setDExtraNoLoad                (const std::string& v)                      { testFields["dExtraNoLoad"]                = home_FieldValue(v);  }
            void setDExtraNoLoad2               (const std::string& v)                      { testFields["dExtraNoLoad2"]               = home_FieldValue(v);  }
            
            void setTSequence                   (home_Sequence v)                           { testFields["tSequence"]                   = home_FieldValue(v);  }
            void setTStandardTemperatureRise    (home_TemperatureRise v)                    { testFields["tStandardTemperatureRise"]    = home_FieldValue(v);  }
            void setTStandardLightningImpulse   (home_Standard v)                           { testFields["tStandardLightningImpulse"]   = home_FieldValue(v);  }
            void setTStandardRIV                (home_Standard v)                           { testFields["tStandardRIV"]                = home_FieldValue(v);  }
            void setTStandardSoundLevel         (home_Standard v)                           { testFields["tStandardSoundLevel"]         = home_FieldValue(v);  }
            void setTStandardPartialDischarge   (home_Standard v)                           { testFields["tStandardPartialDischarge"]   = home_FieldValue(v);  }
            
            void setSErrorName                  (const std::vector<std::string>& v)         { testFields["sErrorName"]                  = home_FieldValue(v);  }
            void setTErrorEnum                  (const std::vector<home_WarningSeverity>& v){ testFields["tErrorEnum"]                  = home_FieldValue(v);  }
            void setSErrorMessages              (const std::vector<std::string>& v)         { testFields["sErrorMessages"]              = home_FieldValue(v);  }
    };
//  end of Tests class
}