/**__________________________________________________________________________________________________________________________
|    @date      May 21, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      enums.h
|    @brief     Value mappings for enumerated types.
|    @brief     Transformer::Config     for 'System & Setup'
|
|          Purpose: 
|               Enumerations DONT store strings, so functions below to match
|___________________________________________________________________________________________________________________________*/
#pragma once

#include <string_view>
#include "alias.h"


namespace Transformer::Config
{

/* ________________________________________________________________________________________________________________________
|
|                                                  Core Transformer & Phase Types
|________________________________________________________________________________________________________________________*/




    enum class Type    { SinglePhase, ThreePhase, AutoTransformer };            // Transformer Core types
    constexpr std::string_view to_string(Type t)
    {
        switch (t) 
        {
            case Type::SinglePhase:     return "Single Phase";
            case Type::ThreePhase:      return "Three Phase";
            case Type::AutoTransformer: return "Auto Transformer";
        }
        return "Unknown";
    }


    enum class Family    { None, MPM, PM, PLMT, PMT, DPM, OWP, SP, LPMT };      // Transformer families 
    constexpr std::string_view to_string(Family f)
    {
        switch (f)
        {
            case Family::None:  return "None";
            case Family::MPM:   return "Mini Pad Mount";
            case Family::PM:    return "Pole Mounted";
            case Family::PLMT:  return "Platform Mount";
            case Family::PMT:   return "Pad-Mounted";
            case Family::DPM:   return "DPM";
            case Family::OWP:   return "Outdoor WP";
            case Family::SP:    return "Submersible";
            case Family::LPMT:  return "Light Pad-Mounted";
        }
        return "Unknown";
    }


    enum class PhaseType { Single, Three };                     // Phase types
    constexpr std::string_view to_string(PhaseType p)
    {
        return (p == PhaseType::Single) ? "Single" : "Three";
    }


    enum class ConnectionType { None, Delta, Wye };             // Connection types
    constexpr std::string_view to_string(ConnectionType c)
    {
        switch (c) 
        {
            case ConnectionType::None:  return "None";
            case ConnectionType::Delta: return "Delta";
            case ConnectionType::Wye:   return "Wye";
        }
        return "Unknown";
    }



/* ________________________________________________________________________________________________________________________
|
|                                                         Windings & Conductors
|________________________________________________________________________________________________________________________*/



    enum class WindingDesc { None, HV, LV, TV, RegHV, RegLV, Series, Common };  // Various winding descriptions, including regulation and series/common windings
    constexpr std::string_view to_string(WindingDesc w)                         // Maps winding descriptions to their string representations for display purposes
    {
        switch (w) 
        {
            case WindingDesc::None:   return "None";                        // This is  0
            case WindingDesc::HV:     return "High Voltage";                //          1
            case WindingDesc::LV:     return "Low Voltage";                 //          2
            case WindingDesc::TV:     return "Tertiary Voltage";            //          ...
            case WindingDesc::RegHV:  return "Regulation High Voltage";
            case WindingDesc::RegLV:  return "Regulation Low Voltage";
            case WindingDesc::Series: return "Series Voltage";
            case WindingDesc::Common: return "Common Voltage";              //          7
        }
        return "Unknown";
    }
    constexpr int get_winding_index(WindingDesc w)                              // Maps winding descriptions to respective indices in TAD input/output arrays
    {
        switch (w)
        {
            case WindingDesc::None:   return -1;
            case WindingDesc::HV:     return 0;
            case WindingDesc::LV:     return 1;
            case WindingDesc::TV:     return 2;
            case WindingDesc::RegHV:  return 3;
            case WindingDesc::RegLV:  return 4;
            case WindingDesc::Series: return 0;
            case WindingDesc::Common: return 1;
        }
        return -1;
    }


    enum class Material { None, Copper, Aluminum };
    constexpr std::string_view to_string(Material m)
    {
        switch (m)
        {
            case Material::None:     return "None";
            case Material::Copper:   return "Copper";
            case Material::Aluminum: return "Aluminum";
        }
        return "Unknown";
    }
    constexpr int to_value(Material m)                          // Maps conductor materials to INT values for TAD input purposes
    {
        return static_cast<int>(m);         // Explicitly maps None=0, Copper=1, Aluminum=2
    }


    enum class ConductorType { None, Strip, Magnet };
    constexpr std::string_view to_string(ConductorType c)
    {
        switch (c)
        {
            case ConductorType::None:   return "None";
            case ConductorType::Strip:  return "Strip";
            case ConductorType::Magnet: return "Magnet";
        }
        return "Unknown";
    }


    enum class Coating { None, Enamel, Paper };
    constexpr std::string_view to_string(Coating c)
    {
        switch (c)
        {
            case Coating::None:   return "None";
            case Coating::Enamel: return "Enamel";
            case Coating::Paper:  return "Paper";
        }
        return "Unknown";
    }


    enum class WindingSelection { None, HV, LV, Both };
    constexpr std::string_view to_string(WindingSelection w)
    {
        switch (w) 
        {
            case WindingSelection::None: return "None";
            case WindingSelection::HV:   return "HV";
            case WindingSelection::LV:   return "LV";
            case WindingSelection::Both: return "Both";
        }
        return "Unknown";
    }



/* ________________________________________________________________________________________________________________________
|
|                                               Physical Dimensions & Cooling 
|________________________________________________________________________________________________________________________*/



    enum class WidthDucts { None, D1_8, D1_4, D3_16 };          // Cooling ducts widths --> ( None, 1/8", 1/4", 3/16" )
    constexpr std::string_view to_string(WidthDucts w)
    {
        switch (w)
        {
            case WidthDucts::None:  return "0.0";
            case WidthDucts::D1_8:  return "1/8";
            case WidthDucts::D1_4:  return "1/4";
            case WidthDucts::D3_16: return "3/16";
        }
        return "Unknown";
    }


    //                                                               Cooling types: Combinations for different forced cooling stages
    enum class CoolingType { None, ONAN, ONAN_ONAF, ONAN_ONAF_ONAF, KNAN, KNAN_KNAF, KNAN_KNAF_KNAF };
    constexpr std::string_view to_string(CoolingType c)
    {
        switch (c) 
        {
            case CoolingType::None:             return "None";
            case CoolingType::ONAN:             return "ONAN";
            case CoolingType::ONAN_ONAF:        return "ONAN/ONAF";
            case CoolingType::ONAN_ONAF_ONAF:   return "ONAN/ONAF/ONAF";
            case CoolingType::KNAN:             return "KNAN";
            case CoolingType::KNAN_KNAF:        return "KNAN/KNAF";
            case CoolingType::KNAN_KNAF_KNAF:   return "KNAN/KNAF/KNAF";
//          case CoolingType::LNAN:             return "LNAN";
//          case CoolingType::LNAN_LNAF:        return "LNAN/LNAF";
//          case CoolingType::LNAN_LNAF_ONAF:   return "LNAN/lNAF/LNAF;
        }
        return "Unknown";
    }

    enum class SingleCoolingType { None, ONAN, ONAF, KNAN, KNAF, LNAN, LNAF };
    constexpr std::string_view to_string(SingleCoolingType c)
    {
        switch (c) 
        {
            case SingleCoolingType::None: return "None";
            case SingleCoolingType::ONAN: return "ONAN";
            case SingleCoolingType::ONAF: return "ONAF";
            case SingleCoolingType::KNAN: return "KNAN";
            case SingleCoolingType::KNAF: return "KNAF";
            case SingleCoolingType::LNAN: return "LNAN";
            case SingleCoolingType::LNAF: return "LNAF";
        }
        return "Unknown";
    }

    enum class TemperatureRiseStandard { None, Temp_55, Temp_55_65, Temp_65 };      // Temperature rise classes (Celcius)
    constexpr std::string_view to_string(TemperatureRiseStandard t)
    {
        switch (t) 
        {
            case TemperatureRiseStandard::None:       return "None";
//          case TemperatureRiseStandard::Temp_45:    return "45";
//          case TemperatureRiseStandard::Temp_45_55: return "45/55";
//          case TemperatureRiseStandard::Temp_50:    return "50";
            case TemperatureRiseStandard::Temp_55:    return "55";
            case TemperatureRiseStandard::Temp_55_65: return "55/65";
//          case TemperatureRiseStandard::Temp_60:    return "60";
            case TemperatureRiseStandard::Temp_65:    return "65";
//          case TemperatureRiseStandard::Temp_65_75: return "65/75";
//          case TemperatureRiseStandard::Temp_70:    return "70";
//          case TemperatureRiseStandard::Temp_75:    return "75";
        }
        return "Unknown";
    }

    enum class Liquid { None, Oil, FR3 };                       // Cooling liquids types
    constexpr std::string_view to_string(Liquid l)
    {
        switch (l)
        {
            case Liquid::None: return "None";
            case Liquid::Oil:  return "Oil";
            case Liquid::FR3:  return "FR3";
        }
        return "Unknown";
    }

    enum class Radiator { None, Menk, Tranter };            // Radiator types
    constexpr std::string_view to_string(Radiator r)
    {
        switch (r)
        {
            case Radiator::None:    return "None";
            case Radiator::Menk:    return "Menk";
            case Radiator::Tranter: return "Tranter";
        }
        return "Unknown";
    }

    enum class CoolingMethod { None, Natural, Stage1Forced, Stage2Forced };     // Cooling methods types
    constexpr std::string_view to_string(CoolingMethod c) {
        switch (c) {
            case CoolingMethod::None:         return "None";
            case CoolingMethod::Natural:      return "Natural";
            case CoolingMethod::Stage1Forced: return "1-stage Forced";
            case CoolingMethod::Stage2Forced: return "2-stage Forced";
        }
        return "Unknown";
    }

    enum class ArrayType { HL, LH, LHL, LLH, Other };           // Array types for TAD input/output mapping
    constexpr std::string_view to_string(ArrayType a)
    {
        switch (a)
        {
            case ArrayType::HL:    return "H-L";
            case ArrayType::LH:    return "L-H";
            case ArrayType::LHL:   return "L-H-L";
            case ArrayType::LLH:   return "L-L-H";
            case ArrayType::Other: return "Other";

            // case ArrayType::L_HT: return "L-HT";
            // case ArrayType::RL_L_HT: return "RL-L-HT";
            // case ArrayType::L_H_RH: return "L-H-RH";
            // case ArrayType::T_L_HT: return "T-L-HT";
            // case ArrayType::T_L_H_RH: return "T-L-H-RH";
            // case ArrayType::L_H: return "L-H";
            // case ArrayType::RL_L_H: return "RL-L-H";
            // case ArrayType::T_RL_L_HT: return "T-RL-L-HT";
            // case ArrayType::T_RL_L_H: return "T-RL-L-H";
            // case ArrayType::RL_L_L_HT: return "RL-L-L-HT";
            // case ArrayType::H_L: return "H-L";
            // case ArrayType::RL_L_L_H: return "RL-L-L-H";
            // case ArrayType::T_RL_L_H_RH: return "T-RL-L-H-RH";
            // case ArrayType::H_L_H_RH: return "H-L-H-RH";
            // case ArrayType::L_L_HT: return "L-L-HT";
            // case ArrayType::RH_H_L_H: return "RH-H-L-H";
            // case ArrayType::T_RH_H_L_H: return "T-RH-H-L-H";
            // case ArrayType::RL_L_H_RH: return "RL-L-H-RH";
            // case ArrayType::L_H_RH_T: return "L-H-RH-T";
            // case ArrayType::L_L_H_RH: return "L-L-H-RH";
            // case ArrayType::L_RH_H_T: return "L-RH-H-T";
            // case ArrayType::T_RH_H_L: return "T-RH-H-L";
            // case ArrayType::L_H_RH_H_T: return "L-H-RH-H-T";
            // case ArrayType::T_H_RH_L: return "T-H-RH-L";
            // case ArrayType::L_L_H: return "L-L-H";
            // case ArrayType::L_RH_H: return "L-RH-H";
            // case ArrayType::L_RL_HT: return "L-RL-HT";
            // case ArrayType::T_RH_L_H: return "T-RH-L-H";
            // case ArrayType::T_H_L_RH: return "T-H-L-RH";
            // case ArrayType::RH_L_H: return "RH-L-H";
            // case ArrayType::H_L_H: return "H-L-H";
            // case ArrayType::RL_L_T_RH_H: return "RL-L-T-RH-H";
            // case ArrayType::L_RL_T_HT: return "L-RL-T-HT";
            // case ArrayType::T_L_RH_H: return "T-L-RH-H";
            // case ArrayType::L_T_L_H_RH: return "L-T-L-H-RH";
            // case ArrayType::T_RL_L_L_H: return "T-RL-L-L-H";
            // case ArrayType::T_H_RH_H_L: return "T-H-RH-H-L";
            // case ArrayType::T_H_L_HT: return "T-H-L-HT";
            // case ArrayType::L_H_RH_L: return "L-H-RH-L";
            // case ArrayType::H_L_HT: return "H-L-HT";
            // case ArrayType::T_RL_L_L_HT: return "T-RL-L-L-HT";
            // case ArrayType::T_H_T_L_RL: return "T-H-T-L-RL";
            // case ArrayType::T_L_T_HT: return "T-L-T-HT";
            // case ArrayType::RH_L_L_H: return "RH-L-L-H";
            // case ArrayType::L_RL_H_T: return "L-RL-H-T";
            // case ArrayType::T_L_L_HT: return "T-L-L-HT";
            // case ArrayType::T_L_H_RH_H: return "T-L-H-RH-H";
            // case ArrayType::L_HT_RL: return "L-HT-RL";
            // case ArrayType::T_RL_L_RH_H: return "T-RL-L-RH-H";
            // case ArrayType::RL_T_L_H_RH: return "RL-T-L-H-RH";
            // case ArrayType::T_T_L_HT: return "T-T-L-HT";
            // case ArrayType::H_L_RH_H: return "H-L-RH-H";
            // case ArrayType::T_L_H_H_RH: return "T-L-H-H-RH";
            // case ArrayType::T_L_H: return "T-L-H";
            // case ArrayType::T_L_L_H_RH: return "T-L-L-H-RH";
            // case ArrayType::HT_L_RL: return "HT-L-RL";
            // case ArrayType::T_H_L_H: return "T-H-L-H";
            // case ArrayType::RL_T_L_HT: return "RL-T-L-HT";
            // case ArrayType::L_RL_L_HT: return "L-RL-L-HT";
            // case ArrayType::T_L_H_H: return "T-L-H-H";
            // case ArrayType::L_H_H_RH: return "L-H-H-RH";
            // case ArrayType::H_L_L: return "H-L-L";
            // case ArrayType::RL_L_H_T_RH: return "RL-L-H-T-RH";
            // case ArrayType::T_L_RL_HT: return "T-L-RL-HT";
            // case ArrayType::RL_T_L_L_HT: return "RL-T-L-L-HT";
            // case ArrayType::L_H_L_HT: return "L-H-L-HT";
            // case ArrayType::RL_L_T_H: return "RL-L-T-H";
            // case ArrayType::H_L_L_H_RH: return "H-L-L-H-RH";
            // case ArrayType::T_RH_L_HT: return "T-RH-L-HT";
            // case ArrayType::L_H_HT: return "L-H-HT";
            // case ArrayType::L_H_RL: return "L-H-RL";
            // case ArrayType::T_L_RL_H_RH: return "T-L-RL-H-RH";
            // case ArrayType::T_H_RL_L: return "T-H-RL-L";
            // case ArrayType::RH_H_L: return "RH-H-L";
            // case ArrayType::T_L_RH_H_L: return "T-L-RH-H-L";
            // case ArrayType::L_T_HT: return "L-T-HT";
            // case ArrayType::L_H_T_RH: return "L-H-T-RH";
            // case ArrayType::T_RH_RL_H_L_H: return "T-RH-RL-H-L-H";
            // case ArrayType::H_RH_L: return "H-RH-L";
            // case ArrayType::H_RH_H_L_H: return "H-RH-H-L-H";
            // case ArrayType::RL_RH_L_H_T: return "RL-RH-L-H-T";
            // case ArrayType::T_C_ST: return "T-C-ST";
            // case ArrayType::T_C_RS_S: return "T-C-RS-S";
            // case ArrayType::T_RC_C_ST: return "T-RC-C-ST";
            // case ArrayType::T_C_S_RS: return "T-C-S-RS";
            // case ArrayType::T_RC_C_S: return "T-RC-C-S";
            // case ArrayType::T_RS_C_S: return "T-RS-C-S";
            // case ArrayType::C_S: return "C-S";
            // case ArrayType::T_RC_C_S_RS: return "T-RC-C-S-RS";
            // case ArrayType::C_ST: return "C-ST";
            // case ArrayType::T_C_RC_S: return "T-C-RC-S";
            // case ArrayType::T_C_S: return "T-C-S";
            // case ArrayType::T_S_C_RS_S: return "T-S-C-RS-S";
            // case ArrayType::C_C_RS_S: return "C-C-RS-S";
            // case ArrayType::T_RC_C_RS_S: return "T-RC-C-RS-S";
            // case ArrayType::T_RC_C_C_ST: return "T-RC-C-C-ST";
            // case ArrayType::T_C_S_RC: return "T-C-S-RC";
            // case ArrayType::C_RS_S: return "C-RS-S";
            // case ArrayType::T_C_C_RS_S: return "T-C-C-RS-S";
            // case ArrayType::C_C_ST: return "C-C-ST";
            // case ArrayType::C_RS_C_S: return "C-RS-C-S";
            // case ArrayType::RC_T_C_S: return "RC-T-C-S";
            // case ArrayType::C_S_RS: return "C-S-RS";
            // case ArrayType::RC_C_S: return "RC-C-S";
            // case ArrayType::T_C_RC_ST: return "T-C-RC-ST";
            // case ArrayType::RC_C_ST: return "RC-C-ST";
            // case ArrayType::T_RC_S_C_ST: return "T-RC-S-C-ST";
            // case ArrayType::T_RC_C_S_S: return "T-RC-C-S-S";
            // case ArrayType::T_C_RS_C_S: return "T-C-RS-C-S";
            // case ArrayType::T_C_S_RS_S: return "T-C-S-RS-S";
            // case ArrayType::C_C_S_RS: return "C-C-S-RS";
            // case ArrayType::T_C_C_ST: return "T-C-C-ST";
            // case ArrayType::T_C_RS_S_T: return "T-C-RS-S-T";
            // case ArrayType::T_C_C_S_RS: return "T-C-C-S-RS";
            // case ArrayType::T_S_C_RC: return "T-S-C-RC";
            // case ArrayType::T_RS_S_C_S: return "T-RS-S-C-S";
            // case ArrayType::T_S_RC_C_RS_RS: return "T-S-RC-C-RS-RS";
            // case ArrayType::C_T_S_RS_S: return "C-T-S-RS-S";
            // case ArrayType::T_S_C_ST: return "T-S-C-ST";
            // case ArrayType::T_RS_S_C: return "T-RS-S-C";
            // case ArrayType::RC_C_RS_S: return "RC-C-RS-S";
            // case ArrayType::T_S_C_S_RS: return "T-S-C-S-RS";
            // case ArrayType::C_C_S: return "C-C-S";
            // case ArrayType::T_C_S_S_RS: return "T-C-S-S-RS";
            // case ArrayType::T_RC_RC_C_ST: return "T-RC-RC-C-ST";
            // case ArrayType::T_RS_C_ST: return "T-RS-C-ST";
            // case ArrayType::T_S_RS_C_T: return "T-S-RS-C-T";
            // case ArrayType::T_S_RC_C_S_RS: return "T-S-RC-C-S-RS";
            // case ArrayType::C_S_S: return "C-S-S";
            // case ArrayType::S_C_RS: return "S-C-RS";
            // case ArrayType::RS_C_C_S: return "RS-C-C-S";
            // case ArrayType::T_RC_C_S_ST: return "T-RC-C-S-ST";
            // case ArrayType::T_RC_S_C_S: return "T-RC-S-C-S";
            // case ArrayType::T_RC_RS_C_S: return "T-RC-RS-C-S";
            // case ArrayType::C_S_T_ST: return "C-S-T-ST";
            // case ArrayType::T_T_C_RS_S: return "T-T-C-RS-S";
            // case ArrayType::S_C_RC_T: return "S-C-RC-T";
            // case ArrayType::C_S_RS_T: return "C-S-RS-T";
            // case ArrayType::T_C_ST_C_RC: return "T-C-ST-C-RC";
            // case ArrayType::S_C: return "S-C";
            // case ArrayType::RL_L_L_H_RH: return "RL-L-L-H-RH";
            // case ArrayType::T_C_ST_RS: return "T-C-ST-RS";
            // case ArrayType::T_L_HT_RH: return "T-L-HT-RH";
            // case ArrayType::T_C_RS_ST: return "T-C-RS-ST";
        }
        return "Unknown";
    }

    enum class Segment { Free, Block };                 // Segment types for TAD arrays' input/output mapping
    constexpr std::string_view to_string(Segment s)
    {
        return (s == Segment::Free) ? "Free" : "Block";
    }



/* ________________________________________________________________________________________________________________________
|
|                                         Tap Changers & Technical Specs 
|________________________________________________________________________________________________________________________*/

    enum class Leads 
    {
        LEADS_NONE,
        CU011,
        CU0931,
        CU052,
        CU050x4S,
        AL047X2,
        AL090X2,
        AL063X3,
        AL079X4
    };
    constexpr std::string_view to_string(Leads l)
    {
        switch (l)
        {
            case Leads::LEADS_NONE: return "None";
            case Leads::CU011:      return "011CU";      // 0.010x1"
            case Leads::CU0931:     return "0931CU";     // 0.093x1"
            case Leads::CU052:      return "052CU";      // 0.050x2"
            case Leads::CU050x4S:   return "050x4SCU";   // 0.050x4"
            case Leads::AL047X2:    return "047X2AL";    // 0.047x2"
            case Leads::AL090X2:    return "090X2AL";    // 0.090x2"
            case Leads::AL063X3:    return "063X3AL";    // 0.063x3"
            case Leads::AL079X4:    return "079X4AL";    // 0.0787x4"
        }
        return "Unknown";
    }

    enum class Bars
    {
        None,
        CU15,
        CU2,
        CU3,
        CU4,
        ALFRE2,
        ALFRE3,
        ALFRE4,
        CULEADS011,
        CULEADS0931,
        CULEADS052,
        CULEADS050x4S,
        ALLEADS047X2,
        ALLEADS090X2,
        ALLEADS063X3,
        ALLEADS079X4
    };
    constexpr std::string_view to_string(Bars b)
    {
        switch (b)
        {
            case Bars::None:          return "None";
            case Bars::CU15:          return "15CUBAR";    // 0.25x1.5"
            case Bars::CU2:           return "2CUBAR";     // 0.25x2"
            case Bars::CU3:           return "3CUBAR";     // 0.25x3"
            case Bars::CU4:           return "4CUBAR";     // 0.25x4"
            case Bars::ALFRE2:        return "2ALFRE";     // 0.25x2"
            case Bars::ALFRE3:        return "3ALFRE";     // 0.25x3"
            case Bars::ALFRE4:        return "4ALFRE";     // 0.25x4"
            case Bars::CULEADS011:    return "011CU";      // 0.010x1"
            case Bars::CULEADS0931:   return "0931CU";     // 0.093x1"
            case Bars::CULEADS052:    return "052CU";      // 0.050x2"
            case Bars::CULEADS050x4S: return "050X4SCU";   // 0.050x4"
            case Bars::ALLEADS047X2:  return "047X2AL";    // 0.047x2"
            case Bars::ALLEADS090X2:  return "090X2AL";    // 0.090x2"
            case Bars::ALLEADS063X3:  return "063X3AL";    // 0.063x3"
            case Bars::ALLEADS079X4:  return "079X4AL";    // 0.0787x4"
        }
        return "Unknown";
    }

    enum class CoilType { Default = -1, Helicoidal = 0, Discs = 1, Multicircuit=2 };    // Tap changer coil types 
    enum class RegulationType { Regulation = 1, Taps = 2, None = 3 };                   // Regulation type

    enum class ChangerCapacity { None, OLTC, OCTC, RCBN, FCBN };                        // Tap changer capacities
    constexpr std::string_view to_string(ChangerCapacity c)
    {
        switch (c) 
        {
            case ChangerCapacity::None: return "None";
            case ChangerCapacity::OLTC: return "OLTC";
            case ChangerCapacity::OCTC: return "OCTC";
            case ChangerCapacity::RCBN: return "RCBN";
            case ChangerCapacity::FCBN: return "FCBN";
        }
        return "Unknown";
    }

    enum class ChangerModel { None, Eaton100ACrimps, Orto0498_9T, Eaton300A, QualitySwitch };       // Tap changer models
    constexpr std::string_view to_string(ChangerModel m)
    {
        switch (m)
        {
            case ChangerModel::None:            return "None";
            case ChangerModel::Eaton100ACrimps: return "Eaton 100A Crimps";
            case ChangerModel::Orto0498_9T:     return "Orto 0498-9T";
            case ChangerModel::Eaton300A:       return "Eaton 300A";
            case ChangerModel::QualitySwitch:   return "Quality Switch";
        }
        return "Unknown";
    }

    enum class TapExtremes { None, E, A };                  // Tap extremes
    constexpr std::string_view to_string(TapExtremes t)
    {
        switch (t)
        {
            case TapExtremes::None: return "None";
            case TapExtremes::E:    return "E";
            case TapExtremes::A:    return "A";
        }
        return "Unknown";
    }

    enum class Sequence { None, R1, C2, F1, R1C2, R1F1, C2F1, R1C2F1 };     // Tap changer sequence types
    constexpr std::string_view to_string(Sequence s) 
    {
        switch (s)
        {
            case Sequence::None:   return "None";
            case Sequence::R1:     return "1R";
            case Sequence::C2:     return "2C";
            case Sequence::F1:     return "1F";
            case Sequence::R1C2:   return "1R,2C";
            case Sequence::R1F1:   return "1R,1F";
            case Sequence::C2F1:   return "2C,1F";
            case Sequence::R1C2F1: return "1R,2C,1F";
        }
        return "Unknown";
    }

    enum class ChangerType { None = -1, CSC = 0, CBC = 1 };     // Tap changer types (CSC = Capacitive Switching, CBC = Resistive/Reactive Switching)

    enum class LRType { None, Linear, Reversing };              // LR types for CBC tap changers (None, Linear, Reversing)
    constexpr std::string_view to_string(LRType l)
    {
        switch (l) 
        {
            case LRType::None:      return "None";
            case LRType::Linear:    return "Linear";
            case LRType::Reversing: return "Reversing";
        }
        return "Unknown";
    }

    enum class CBCType { None, Resistive, Reactive };       // CBC tap changer types
    constexpr std::string_view to_string(CBCType c)
    {
        switch (c) 
        {
            case CBCType::None:      return "None";
            case CBCType::Resistive: return "Resistive";
            case CBCType::Reactive:  return "Reactive";
        }
        return "Unknown";
    }

    enum class BIL { BIL_0, BIL_30, BIL_45, BIL_60, BIL_75, BIL_95, BIL_110, BIL_125, BIL_150, BIL_170, BIL_200, BIL_250 }; // Basic Insulation Levels for insulation class
    constexpr std::string_view to_string(BIL b)
    {
        switch (b)
        {
            case BIL::BIL_0:   return "0";
            case BIL::BIL_30:  return "30";
            case BIL::BIL_45:  return "45";
            case BIL::BIL_60:  return "60";
            case BIL::BIL_75:  return "75";
            case BIL::BIL_95:  return "95";
            case BIL::BIL_110: return "110";
            case BIL::BIL_125: return "125";
            case BIL::BIL_150: return "150";
            case BIL::BIL_170: return "170";
            case BIL::BIL_200: return "200";
            case BIL::BIL_250: return "250";
            // case BIL::BIL_325:  return "325";
            // case BIL::BIL_350:  return "350";
            // case BIL::BIL_450:  return "450";
            // case BIL::BIL_550:  return "550";
            // case BIL::BIL_650:  return "650";
            // case BIL::BIL_750:  return "750";
            // case BIL::BIL_825:  return "825";
            // case BIL::BIL_850:  return "850";
            // case BIL::BIL_900:  return "900";
            // case BIL::BIL_950:  return "950";
            // case BIL::BIL_1050: return "1050";
            // case BIL::BIL_1175: return "1175";
            // case BIL::BIL_1300: return "1300";
            // case BIL::BIL_1425: return "1425";
            // case BIL::BIL_1550: return "1550";
            // case BIL::BIL_1675: return "1675";
        }
        return "Unknown";
    }

    enum class LaminationType { None, M0H, M2, M3, M4, Am, AmHB1 };     // Lamination types
    constexpr std::string_view to_string(LaminationType l)
    {
        switch (l) 
        {
            case LaminationType::None:  return "None";
            case LaminationType::M0H:   return "MOH";
            case LaminationType::M2:    return "M2";
            case LaminationType::M3:    return "M3";
            case LaminationType::M4:    return "M4";
            case LaminationType::Am:    return "Am";
            case LaminationType::AmHB1: return "Am HB1";
        }
        return "Unknown";
    }
    // Maps lamination types to INT values. Shifts it left one, so: None = -1, M0H = 0, M2 = 1, etc.
    constexpr int to_value(LaminationType l)            {   return static_cast<int>(l) - 1; } 

    enum class CoreTolerance { None = -1, A = 0, C = 1, E = 2, Strip = 3 };     // Core tolerance types

/* ________________________________________________________________________________________________________________________
|
|                                               Standards & Norms
|________________________________________________________________________________________________________________________*/


    //          Testing and performance framework:  International/national test codes & performance standards
    enum class Standard { None, IEEEC571290, IEEC571291, IEC600762, IEC600763, IEC6007610, CSAC88, CISPR182, IEC60437, NEMA1071987, IEEC57113, IEC60270,IEEC571290 };
    constexpr std::string_view to_string(Standard s)
    {
        switch (s) 
        {
            case Standard::None:         return "None";
            case Standard::IEEEC571290:  return "IEEE C57.12.90";
            case Standard::IEEC571291:   return "IEEE C57.12.91";
            case Standard::IEC600762:    return "IEC 60076-2";
            case Standard::IEC600763:    return "IEC 60076-3";
            case Standard::IEC6007610:   return "IEC 60076-10";
            case Standard::CSAC88:       return "CSA C88";
            case Standard::CISPR182:     return "CISPR 18-2";
            case Standard::IEC60437:     return "IEC 60437";
            case Standard::NEMA1071987:  return "NEMA 107-1987";
            case Standard::IEEC57113:    return "IEEE C57.113";
            case Standard::IEC60270:     return "IEC 60270";
            case Standard::IEEC571290:   return "IEEE C57.12.90";
        }
        return "Unknown";
    }


  //          Governing body (Highest-level categorizatio defining regional engineering philosophy)       
    enum class DesignSpecCodes { None, ANSI, IEC };
    constexpr std::string_view to_string(DesignSpecCodes d)
    {
        switch (d) 
        {
            case DesignSpecCodes::None: return "None";
            case DesignSpecCodes::ANSI: return "ANSI";
            case DesignSpecCodes::IEC:  return "IEC";
        }
        return "Unknown";
    }

//          Physical Construction & Blueprint Standards:    Specific dimensional, physical & construction standaards
    enum class DesignSpecStandards
    {
        None,
        ANSI,
        C2106,
        C2124,
        C2206,
        C8816,
        C88M79,
        C88M90,
        C19908,
        C227306,
        C227406,
        C227421,
        C227508,
        C301106,
        C301206
    };
    constexpr std::string_view to_string(DesignSpecStandards d)
    {
        switch (d)
        {
            case DesignSpecStandards::None:    return "None";
            case DesignSpecStandards::ANSI:    return "ANSI";
            case DesignSpecStandards::C2106:   return "C2.1/06";
            case DesignSpecStandards::C2124:   return "C2.1/24";
            case DesignSpecStandards::C2206:   return "C2.2/06";
            case DesignSpecStandards::C8816:   return "C88-16";
            case DesignSpecStandards::C88M79:  return "C88.M79";
            case DesignSpecStandards::C88M90:  return "C88.M90";
            case DesignSpecStandards::C19908:  return "C199-08";
            case DesignSpecStandards::C227306: return "C227.3/06";
            case DesignSpecStandards::C227406: return "C227.4/06";
            case DesignSpecStandards::C227421: return "C227.4/21";
            case DesignSpecStandards::C227508: return "C227.5/08";
            case DesignSpecStandards::C301106: return "C301.1/06";
            case DesignSpecStandards::C301206: return "C301.2/06";
        }
        return "Unknown";
    }

//  Energy Losses & COre Matertials:    How transformer achieves energy efficiency requirements (Largely dependant of magnetic core material)
    enum class EfficiencySpec
    {
        None,
        ANSI,
        C2106,
        C2124,
        C2206,
        C8816,
        C88M79,
        C88M90,
        C19908,
        C227306,
        C227406,
        C227421,
        C227508,
        C301106,
        C301206,
        // BIL_825,
        // BIL_850,
        // BIL_900,
        // BIL_950,
        // BIL_1050,
        // BIL_1175,
        // BIL_1300,
        // BIL_1425,
        // BIL_1550,
        // BIL_1675,
        type_None,
        M0H,
        M2,
        M3,
        M4,
        Am,
        AmHB1
    };
    constexpr std::string_view to_string(EfficiencySpec e)
    {
        switch (e)
        {
            case EfficiencySpec::None:    return "None";
            case EfficiencySpec::ANSI:    return "ANSI";
            case EfficiencySpec::C2106:   return "C2.1/06";
            case EfficiencySpec::C2124:   return "C2.1/24";
            case EfficiencySpec::C2206:   return "C2.2/06";
            case EfficiencySpec::C8816:   return "C88-16";
            case EfficiencySpec::C88M79:  return "C88.M79";
            case EfficiencySpec::C88M90:  return "C88.M90";
            case EfficiencySpec::C19908:  return "C199-08";
            case EfficiencySpec::C227306: return "C227.3/06";
            case EfficiencySpec::C227406: return "C227.4/06";
            case EfficiencySpec::C227421: return "C227.4/21";
            case EfficiencySpec::C227508: return "C227.5/08";
            case EfficiencySpec::C301106: return "C301.1/06";
            case EfficiencySpec::C301206: return "C301.2/06";

            // case EfficiencySpec::BIL_825:  return "825";
            // case EfficiencySpec::BIL_850:  return "850";
            // case EfficiencySpec::BIL_900:  return "900";
            // case EfficiencySpec::BIL_950:  return "950";
            // case EfficiencySpec::BIL_1050: return "1050";
            // case EfficiencySpec::BIL_1175: return "1175";
            // case EfficiencySpec::BIL_1300: return "1300";
            // case EfficiencySpec::BIL_1425: return "1425";
            // case EfficiencySpec::BIL_1550: return "1550";
            // case EfficiencySpec::BIL_1675: return "1675";

            case EfficiencySpec::type_None: return "None";
            case EfficiencySpec::M0H:       return "MOH"; // Note: Preserved key `M0H` (zero) mapping to value "MOH" (letter O) from your Python script
            case EfficiencySpec::M2:        return "M2";
            case EfficiencySpec::M3:        return "M3";
            case EfficiencySpec::M4:        return "M4";
            case EfficiencySpec::Am:        return "Am";
            case EfficiencySpec::AmHB1:     return "Am HB1";
        }
        return "Unknown";
    }


/* ________________________________________________________________________________________________________________________
|
|                                               Diagnostics & Errors
|________________________________________________________________________________________________________________________*/



enum class WarningSeverity { None = -1, Warning = 0, Error = 1 };       // Warning severity levels for diagnostics (Former: eWarnings)

enum class DesignError : unsigned long long // Design error categories for diagnostics -->
{
    None                = 0,
    Cooling             = 400000000000000ULL,
    MechanicalDesign    = 500000000000000ULL,
    ElectricalDesign    = 600000000000000ULL,
    ConductorDimensions = 700000000000000ULL,
    Conductor           = 800000000000000ULL,
    Pc57                = 900000000000000ULL
};



/* ________________________________________________________________________________________________________________________
|
|                                               Logical Mapping
|________________________________________________________________________________________________________________________*/

    constexpr std::string_view to_string(bool value)    { return value ? "Yes" : "No"; }    
    }