/**___________________________________________________________________________________________________
|    @date      May 22, 2026
|    @author    Tom Goncin
|    @file      Materials.cpp
|    @brief     Materials specific implementor through as 1D databases for calculations.
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initialization of localized 1D databases for core, conductor, and insulation metrics.
|                   - Loads predefined material pricing, structural geometries, and resistive constants.
|                   - Implements international efficiency tables (DOE 216, 8021, 8023).
|               Returns:
|                   - Direct data access to standardized manufacturing and supply chain configuration arrays.
|___________________________________________________________________________________________________*/
#include "materials.h"


using namespace Transformer::Types;






namespace Transformer::Commercial
{
    // Global Data Structures 
    const std::vector<Table11_Row> df_table11_C2 =
    {
        {347,          10, 30},
        {2400,         19, 60},
        {4800,         26, 75},
        {7200,         34, 95},
        {8000,         34, 95},
        {14400,        40, 125},
        {16000,        40, 125},
        {19900,        50, 150},
        {std::nullopt, 70, 200}
    };
    // ResistiveMaterial Implementation 
    ResistiveMaterial::ResistiveMaterial()
    {
        tableHeaders = { "copper", "aluminum" };

        tableData =
        {
            {0.003862, 0.0039},
            {1.68e-8, 2.82e-8},
            {1.0266, 0.9951},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0}
        };
    }
    // MaterialConstants Implementation 
    MaterialConstants::MaterialConstants()
    {
        tableHeaders = { "factor" };

        tableData =
        {
            {3.854},
            {1.02E-05},
            {234.5},
            {3.854},
            {0.00001021},
            {234.5},
            {0.965}
        };
    }
    // CoreTables Implementation 
    table_Core::table_Core()
    {
        tableHeaders = { "mat", "price", "thickness" };

        tableData =
        {
            {Config::LaminationType::None,      999.00, std::nullopt},
            {Config::LaminationType::M4,        999.00, 0.27},
            {Config::LaminationType::M3,        999.00, 0.23},
            {Config::LaminationType::M2,        999.00, std::nullopt},
            {Config::LaminationType::M0H,         4.03, 0.23},
            {Config::LaminationType::Am,        999.00, std::nullopt},
            {Config::LaminationType::AmHB1,       3.45, std::nullopt}
        };
    }
    double table_Core::get_CorePrice(Config::LaminationType material) const
    {
        for (const auto& row : tableData)
        {
            if (row.material == material)            return row.price;
        }
        throw std::invalid_argument("Core material not found");
    }
    std::optional<double> table_Core::get_CoreThickness(Config::LaminationType material) const
    {
        for (const auto& row : tableData)
        {
            if (row.material == material)             return row.thickness;
        }
        throw std::invalid_argument("Core material not found");
    }
    // CoreFactor Implementation 
    CoreFactor::CoreFactor()
    {
        tableHeaders = { "mat", "factor" };

        tableData =
        {
            {Config::LaminationType::None, 0.0},
            {Config::LaminationType::M4,   0.97},
            {Config::LaminationType::M3,   0.875},
            {Config::LaminationType::M2,   0.0},
            {Config::LaminationType::M0H,  0.76},
            {Config::LaminationType::Am,   0.0},
            {Config::LaminationType::AmHB1,0.0}
        };
    }
    //----------------------------------------------------------------------------------------------------------------> CoreFactor GETTER
    double CoreFactor::get_Factor(Config::LaminationType type) const
    {
        for (const auto& row : tableData) { if (row.first == type) { return row.second; } } return 0.0; // fallback
    }
    // AmHB1CoreStripStandardWidths Implementation 
    AmHB1CoreStripStandardWidths::AmHB1CoreStripStandardWidths()
    {
        tableHeaders = { "", "", "", "", "" };

        tableData =
        {
            {0.0, 5.6,  6.7,  7.6,  8.4},
            {5.6, 11.2, 0.0,  0.0,  0.0},
            {6.7, 12.3, 13.4, 0.0,  0.0},
            {7.6, 13.2, 14.3, 15.2, 0.0},
            {8.4, 14.0, 15.1, 16.0, 16.8}
        };
    }
    // Induction System Components 
    InductionTable::InductionTable(double factor)
    {
        tableHeaders = { "T", "M0H", "M2", "M3", "M4", "Am", "AmHB1" };
        // Base data --> 90, 95, 100, 115, 120 all share
        tableData =
        {
            {0.3, 0.029,  0.0, 0.028, 0.031,  0.0,                0.0},
            {0.4, 0.0485, 0.0, 0.044, 0.05,   0.0,                0.0},
            {0.5, 0.071,  0.0, 0.066, 0.0725, 0.0,                0.0},
            {0.6, 0.096,  0.0, 0.09,  0.104,  0.0,                0.0},
            {0.7, 0.123,  0.0, 0.127, 0.132,  0.0238135996610032, 0.0238135996610032},
            {0.8, 0.151,  0.0, 0.148, 0.166,  0.0299370967166897, 0.0299370967166897},
            {0.9, 0.184,  0.0, 0.184, 0.204,  0.0360605937723763, 0.0360605937723763},
            {1.0, 0.213,  0.0, 0.22,  0.237,  0.062,              0.06},
            {1.1, 0.26,   0.0, 0.265, 0.286,  0.088,              0.083},
            {1.2, 0.303,  0.0, 0.315, 0.348,  0.12,               0.108},
            {1.3, 0.345,  0.0, 0.385, 0.41,   0.16,               0.13},
            {1.4, 0.4,    0.0, 0.455, 0.505,  0.276,              0.193},
            {1.5, 0.47,   0.0, 0.57,  0.635,  0.8,                0.377},       // Index 12
            {1.6, 0.58,   0.0, 0.81,  0.863,  3.25,               1.505},       // Index 13
            {1.7, 0.78,   0.0, 1.5,   1.44,   0.0,                0.0},
            {1.8, 1.24,   0.0, 4.5,   4.3,    0.0,                0.0},         // Index 15
            {1.9, 3.9,    0.0, 17.0,  17.0,   0.0,                0.0}
        };

        int factorInt = static_cast<int>(factor * 100 + 0.1);                   // Convert factor to int (1.05 -> 105)

        if (factorInt == 105)                                                   // Override specific rows that differ
        {
            tableData[12] = { 1.5, 0.47, 0.0, 0.57, 0.0635, 3.25, 1.505 };        // Override row T = 1.5 (index 12)
            tableData[13] = { 1.6, 0.58, 0.0, 0.81, 0.863,  0.0,  0.0 };          // Override row T = 1.6 (index 13)
        }
        else if (factorInt == 110)
        {
            tableData[15] = { 1.8, 1.24, 0.0, 4.5, 4.5, 0.0, 0.0 };               // Override row T = 1.8 (index 15)
        }
    }
    // Conductor Tables Three Phases Layout 
    // IT HAS BEEN COMBINED WITH 1 PHASE TABLE THIS IS JUST AS RECORDS
    /*
        ConductorTables3Ph::ConductorTables3Ph()
        {
            tableHeaders_Magnet =
            {
                "no conductor",
                "wire size",
                "thick wire",
                "width wire",
                "area (in^2)",
                "size",
                "$/lbCu",
                "SAP_Name"
            };

            tableHeaders_Strip =
            {
                "no Conductor",
                "wire size",
                "thickness",
                "width",
                "area (in^2)",
                "size",
                "$/lbCu",
                "SAP_Name"
            };

            tableData_CopperMagnet =
            {
                {9,  "28", 0.0144, 0.0144, 0.000125, 0.0, 1000000.0, "28HMF"},
                {10, "27", 0.0160, 0.0160, 0.000158, 0.0, 8.83,      "27HMF"},
                {31, "0.072 x 0.144", 0.076, 0.148, 0.010025, 0.02, 9.45, "072X144HMF"}
            };

            tableData_CopperStrip =
            {
                {25, 0.062, std::nullopt, std::nullopt, 39.37, "0.062 x 12''", 8.25, "0.062"}
            };

            double iAlFactor1 = 2.19;
            double iAlFactor2 = 2.69;

            tableData_AluminumMagnet =
            {
                {80, "19", 0.0385, 0.0385, 0.00101, 0.0, 2.67 + iAlFactor1, "19HFA"},
                {94, "0.102 x 0.204", 0.106, 0.2080, 0.019929, 0.032, 4.36 + iAlFactor2, "102X204HFA"}
            };

            tableData_AluminumStrip =
            {
                { 1, 0.0625, std::nullopt, std::nullopt, 28.0, "0.0625 x 12''", 4.26, "0.0625" }
            };
        }
    */
    // Standard Single/Two/Multiple Surcharges Conductor Configuration
    ConductorTable::ConductorTable()
    {
        copper_Surcharge = 10.06;
        aluminum_Surcharge = 5.38;

        tableHeaders_Magnet =
        {
            "no conductor",
            "wire size",
            "thick wire",
            "width wire",
            "area (in^2)",
            "size",
            "$/lbCu",
            "SAP_Name"
        };

        tableHeaders_Strip =
        {
            "no Conductor",
            "wire size",
            "thickness",
            "width",
            "area (in^2)",
            "size",
            "$/lbCu",
            "SAP_Name"
        };

        tableData_CopperMagnet =
        {
            {9,  "28", 0.0144, 0.0144, 0.000125, 0.0, 1000000.0, "28HMF"},
            {46, "0.162 x 0.460", 0.166, 0.464, 0.073641, 0.032, 9.24, "162X460HMF"}
        };

        tableData_CopperStrip =
        {
            {25, 0.062, std::nullopt, std::nullopt, 39.37, "0.062 x 12''", 8.25, "0.062", "062", std::nullopt}
        };

        double iAluminumFactor1 = 2.19;
        double iAluminumFactor2 = 2.69;

        tableData_AluminumMagnet =
        {
            {80, "19", 0.0385, 0.0385, 0.00101, 0.0, 2.67 + iAluminumFactor1, "19HFA"},
            {106, "0.162 x 0.460", 0.166, 0.4640, 0.073641, 0.032, 3.11 + iAluminumFactor2, "162X460HFA"}
        };

        tableData_AluminumStrip =
        {
            {1, 0.0625, std::nullopt, std::nullopt, 28.0, "0.0625 x 12''", 4.26, "0.0625", "063", std::nullopt}
        };
    }
    // CoilBil1Phase Implementation 
    CoilBil1Ph::CoilBil1Ph()
    {
        tableHeaders =
        {
            "BIL",              "Stayback",     "Cooling duct",
            "Core Protection",  "Form",         "Duct",
            "0.01 Paper over",  "0.01 Paper",   "0.01 Paper",
            "Duct",             "Crepe paper",  "0.01 Paper",
            "Duct",             "Packing",      "Duct",
            "Packing",          "Duct"
        };
        tableData =
        {
            {30, 0.375, 0.125, 0.0625, 0.0625, 0, 3, 3, 6, 0, 0, 0, 0, 0.0625, 0, 0.0625, 0},
            {350, 2.875, 0.125, 0.0625, 0, 0, 0, 16, 22, 0.25, 0, 21, 0.25, 0.5625, 0.25, 0.125, 0.25}
        };
    }
    // CoilBil3Phases Implementation 
    CoilBil3Ph::CoilBil3Ph()
    {
        tableHeaders = { "DIL", "Stayback", "H-Duct", "C-Packing", "D(in)" };

        tableData_CoilsC88 =
        {
            { 30, 0.375, 0.125, 0.0625, 0.5825 },
            { 250, 3.5, 0.125, 0.0625, 3.7075  }
        };

        tableData_CoilsC2 =
        {
            { 30, 0.375, 0.125, 0.0625, 0.5825 },
            { 200, 1.625, 0.125, 0.0625, 1.8325}
        };

        tableHeaders_CoilsInsideC88Lv =
        {
            "DIL",                              "< 500 KVA",            ">= 500 kVA",
            "<= 300 kVA",                       "> 300 kVA < 1500 kVA", ">= 1500 kVA",
            "Selected PB (Form thickness)",     "Duct",                 "0.01'' paper layers",
            "0.012 Crepe paper layers",         "D (in)",               "Arrangement"
        };

        tableData_CoilsInsideC88Lv =
        {
            { 30, 0.125, 0.25, 0.0625, 0.125, 0.25, 0.0, 0.125, 2, 0, 0.0, ""   },
            { 170, 0.25, 0.25, 0.25, 0.25, 0.25, 0.0, 0.25, 11, 4, 0.0, ""      }
        };

        tableHeaders_CoilsInsideC2Lv =
        {
            "DIL",                          "< 500 KVA",                    ">= 500 kVA",
            "<= 300 kVA",                   "> 300 kVA < 1500 kVA",         ">= 1500 kVA",
            "Selected PB (Form thickness)", "Duct",                         "< 500 KVA",
            ">= 500 kVA",                   "<= 300 kVA",                   "> 300 kVA < 1500 kVA",
            ">= 1500 kVA",                  "Selected 0.01'' paper layers", "0.012 Crepe paper layers",
            "D (in)", "Arrangement"
        };

        tableData_CoilsInsideC2Lv =
        {
            {30, 0.125, 0.25, 0.0625, 0.125, 0.25, 0.0, 0, 2, 2, 3, 2, 2, 0, 0, 0.0, ""},
            {125, 0.25, 0.25, 0.25, 0.25, 0.25, 0.0, 0.125, 6, 6, 6, 6, 6, 0, 4, 0.0, ""}
        };

        tableHeaders_CoilsInsideC88Hv =
        {
            "DIL", "0.01 in paper layers", "Duct", "0.012 Crepe paper layers", "0.01 in paper layers", "Din", "Arrangement"
        };

        tableData_CoilsInsideC88Hv =
        {
            {30, 10, 0.0, 0, 0, 0.1, "10-0.01'\""},
            {250, 34, 0.25, 10, 10, 0.83, "34-0.01'\" + 1/4'\"Aduct + 10-CP + 10-0.01'\""}
        };

        tableHeaders_CoilsInsideC2Hv =
        {
            "DIL", "0.01 in paper layers", "Duct", "0.012 Crepe paper layers", "0.01 in paper layers", "Din", "Arrangement"
        };

        tableData_CoilsInsideC2Hv =
        {
            {30, 6, 0.0, 0, 0, 0.06, "6-0.01'\""},
            {200, 10, 0.25, 3, 8, 0.486, "10-0.01'\" + 1/4'\"Aduct + 3-CP + 8-0.01'\""}
        };

        tableHeaders_CoilsInsideC88OverHv =
        {
            "DIL", "0.01'' Paper layers", "Din", "Arrangement"
        };

        tableData_CoilsInsideC88OverHv =
        {
            { 30, 3, 0.03, "3-0.01\"\""}, {250, 3, 0.03, "3-0.01\"\""}
        };

        tableHeaders_CoilsInsideC2OverHv =
        {
            "DIL", "0.01'' Paper layers", "Din", "Arrangement"
        };

        tableData_CoilsInsideC2OverHv =
        {
            {30, 3, 0.03, "3-0.01\"\""},
            {250, 3, 0.03, "3-0.01\"\""}
        };

        tableHeaders_CoilsC2Packing =
        {
            "DIL", "I-Packing", "O-Packing", "VO-Duct-1", "VO-Duct-2", "VI-Duct-1", "VI-Duct-2", "D (PH-Limb, in)", "D (PH-PH, in)", "ISD Drawing #"
        };

        tableData_CoilsC2Packing =
        {
            {30, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0625, 0.125, "06-ISD0104 R0"},
            {200, 0.0625, 0.25, 0.25, 0, 0.1875, 0.1875, 0.5825, 0.6035, "06-ISD01010 R0"}
        };

        tableHeaders_CoilsC88Packing =
        {
            "DIL", "I-Packing", "O-Packing", "VO-Duct-1", "VO-Duct-2", "VI-Duct-1", "VI-Duct-2", "D (PH-Limb, in)", "D (PH-PH, in)", "ISD Drawing #"
        };

        tableData_CoilsC88Packing =
        {
            {30, 0.0, 0.03125, 0.0, 0.0, 0.0, 0.0, 0.09375, 0.125, "06-ISD0105 R0"},
            {250, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 1.1025, 0.915, "06-ISD01011 R0"}
        };
    }
    // Sub-Insulation Elements Implementations
    CoilInsulation::CoilInsulation()
    {
        tableHeaders_LayerCoil = { "layers/coil", "%voltage stress" };
        tableData_LayerCoil =
        {
            {3, 100},
            {20, 25}
        };

        tableHeaders_RangeKv = { "minkV", "maxkV", "Mils" };
        tableData_RangeKv =
        {
            {0, 30, 10},
            {87, 93, 50}
        };
    }
    CoilInsulation3Ph::CoilInsulation3Ph()
    {
        tableHeaders_LayerCoil = { "Layers", "Max. % Impulse @ first 3 layers" };
        tableData_Coil =
        {
            {3, 100},
            {20, 25}
        };

        tableHeaders_RangeKv = { "Imp. kV between layers", "Ins. Thickness (in)" };
        tableData_RangeKv =
        {
            { 18, 0.005 },
            { 86, 0.045 }
        };
    }
    DuctThicknessTable::DuctThicknessTable()
    {
        tableHeaders = { "Duct thickness (Nominal, in)", "Duct Thickness (Real, in)" };
        tableData=
        {
            {0.1250, 0.1450},
            {0.2500, 0.2700}
        };
    }
    WindingForm::WindingForm()
    {
        tableHeaders_Windform = { "PB for Form 1/16''", "Selected", "Material 1/16 in PB HD + 3-0.01", "Radial Build 0.0925" };
        tableData_WindingForm =
        {
            { 30, "1/16 PB + 4-0.005", 0.0825}, {75, "1/8 PB + 18-005", 0.2150 }
        };
    }
    BodyStayback::BodyStayback()
    {
        tableHeaders = { "BIL", "Stayback" };
        tableData = { {250, 4.000}, {60, 0.500} };
    }
    BodyStaybackUnderHVBarrier::BodyStaybackUnderHVBarrier()
    {
        tableHeaders_SBUnderHVBarrier = { "BIL", "Stayback", "Insulation", "RB-Sides", "CP Layers" };

        tableData_SBUnderHVBarrier =
        {
            {250, 4.0, "20-0.010+3/16 A Duct+20-0.010", 0.609, 0},
            {30, 0.5, "6-0.005", 0.03, 0}
        };
    }
    OverHVBarrier::OverHVBarrier()
    {
        tableHeaders_OverHVBarrier = { "BIL", "Stayback", "Barrier", "RB-Sides", "Nose reduc.", "CP Layers" };
        tableData_OverHVBarrier =
        {
            {250,   4.0, "15-.010+3/16A Duct+10CP+15-.010", 0.609,  0.12,   0 },
            {30,    0.5, "3 - 0.010",                       0.03,   0,      0 }
        };
    }
    ResistivityTable::ResistivityTable()
    {
        tableHeaders = { "Temp", "LV", "HV" };
        tableData = { {20.0, 0.0, 0.0}, {85.0, 0.0, 0.0} };
    }
    ResistanceTable::ResistanceTable()
    {
        tableHeaders = { "Temp", "LW w/o LEads", "LV Leads", "Total LV", "Total HV" };
        tableData =
        {
            {20.0, 0.0, 0.0, 0.0, 0.0},
            {85.0, 0.0, 0.0, 0.0, 0.0}
        };
    }
    EfficiencyTable_Doe216::EfficiencyTable_Doe216()
    {
        tableHeaders_Efficiency1Ph_Doe216 = { "kVA", "minimun efficiency" };
        tableData_Efficiency1Ph_Doe216 =
        {
            {5.0, 0.9865},
            {883.0, 0.9955}
        };
        tableData_Efficiency3Ph_Doe216 =
        {
            {15.0, 0.9865},
            {2500.0, 0.9953}
        };
    }
    EfficiencyTable_8021::EfficiencyTable_8021()
    {
        tableHeaders_EfficiencyTable1Ph_8021 = { "kVA", "Max Impedance", "Min efficiency" };
        tableData_EfficiencyTable1Ph_8021 =
        {
            {10.0, 4.5, 0.98600},
            {833.0, 7.5, 0.99510}
        };
        tableData_EfficiencyTable3Ph_8021 =
        {
            {15.0, 120.0, 0.9854},
            {3000.0, 277.0, 0.9951}
        };
    }
    EfficiencyTable_8023::EfficiencyTable_8023()
    {
        tableHeaders_EfficiencyTable_8023 = { "kVA", "Minimun LV", "Maximum LV", "HV requirement", "Minimum Eff." };
        tableData_EfficiencyTable_8023_1 = { {167.0, 480.0, 4800.0, 44.0, 0.9905} };
        tableData_EfficiencyTable_8023_2 = { {1001.0, 4160.0, 13860.0, 34.5, 0.9938} };
        tableData_EfficiencyTable_8023_3 = { {1001.0, 4160.0, 27600.0, 34.5, 0.9931} };
        tableData_EfficiencyTable3Ph_8023_1 = { {501.0, 480.0, 4800.0, 44.0, 0.9905} };
        tableData_EfficiencyTable3Ph_8023_2 = { {3001.0, 4160.0, 13860.0, 34.5, 0.9938} };
        tableData_EfficiencyTable3Ph_8023_3 = { {3001.0, 4160.0, 27600.0, 34.5, 0.9931} };
    }
    PackingTable::PackingTable()
    {
        tableHeaders = { "BIL", "O_1_16", "O_1_8", "O_1_4" };
        tableData=
        {
            {30, 0, 0, 0},
            {350, 0, 0, 1}
        };
    }
    LineVoltageLookup::LineVoltageLookup()
    {
        tableHeaders = { "Volts", "Line Volts" };
        tableData =
        {
            {120, 208},
            {19920, 34500}
        };
    }
}
// end of materials