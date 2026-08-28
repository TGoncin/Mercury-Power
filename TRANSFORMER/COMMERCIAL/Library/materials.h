
//    @date      August 24, 2026
//    @author    Tom Goncin
//    @file      materials.h
//    @brief     Materials specific implementor using 1D databases and template inheritance.

#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <stdexcept>

#include "alias.h"
#include "enums.h"





namespace Transformer::Commercial
{

    // TEMPLATE BASE CLASS (Eliminates hundreds of lines of repetitive getters/setters)

    template <typename T>
    class DataTable
    {
    public:
        std::vector<std::string> tableHeaders;
        std::vector<T>           tableData;

        // --- Getters ---
        std::vector<std::string> getTableHeaders() const { return tableHeaders; }
        std::vector<T>           getTableData()    const { return tableData; }

        // --- Setters ---
        void setTableHeaders(const std::vector<std::string>& v) { tableHeaders = v; }
        void setTableData(const std::vector<T>& v) { tableData = v; }
    };



    // STANDARD TABLES (Inheriting automatically gives them Headers, Data, Getters, and Setters)

    class ResistiveMaterial : public DataTable<std::vector<double>> { public: ResistiveMaterial(); };
    class MaterialConstants : public DataTable<std::vector<double>> { public: MaterialConstants(); };
    class AmHB1CoreStripStandardWidths : public DataTable<std::vector<double>> { public: AmHB1CoreStripStandardWidths(); };
    class ResistivityTable : public DataTable<std::vector<double>> { public: ResistivityTable(); };
    class ResistanceTable : public DataTable<std::vector<double>> { public: ResistanceTable(); };
    class CoilBil1Ph : public DataTable<std::vector<double>> { public: CoilBil1Ph(); };
    class DuctThicknessTable : public DataTable<std::vector<double>> { public: DuctThicknessTable(); };
    class BodyStayback : public DataTable<std::vector<double>> { public: BodyStayback(); };
    class PackingTable : public DataTable<std::vector<int>> { public: PackingTable(); };
    class LineVoltageLookup : public DataTable<std::vector<int>> { public: LineVoltageLookup(); };

    class InductionTable : public DataTable<std::vector<double>>
    {
    public:
        InductionTable(double factor);
    };



    // CORE STRUCTURES

    struct Row_Core
    {
        Types::home_LaminationType material;
        double                     price;
        std::optional<double>      thickness;
    };

    class table_Core : public DataTable<Row_Core>
    {
    public:
        table_Core();
        double                get_CorePrice(Types::home_LaminationType material) const;
        std::optional<double> get_CoreThickness(Types::home_LaminationType material) const;
    };

    class CoreFactor : public DataTable<std::pair<Types::home_LaminationType, double>>
    {
    public:
        CoreFactor();
        double get_Factor(Types::home_LaminationType type) const;
    };



    // CONDUCTOR STRUCTURES

    struct ConductorMagnetRow
    {
        int             no_conductor;
        std::string     wire_size;
        double          thick_wire;
        double          width_wire;
        double          area_in2;
        double          size;
        double          price_per_lb;
        std::string     sap_name;
    };

    struct ConductorStripRow
    {
        int                         no_Conductor;
        double                      thick_Wire;
        std::optional<double>       thickness;
        std::optional<double>       width;
        double                      area_Inch2;
        std::string                 size_Str;
        double                      price_Per_lb;
        std::string                 sap_Name;
        std::optional<std::string>  alternate_Sap;
        std::optional<double>       extra_value;
    };

    inline bool operator==(const ConductorStripRow& lhs, const ConductorStripRow& rhs)
    {
        return  lhs.no_Conductor == rhs.no_Conductor && lhs.thick_Wire == rhs.thick_Wire &&
            lhs.thickness == rhs.thickness && lhs.width == rhs.width &&
            lhs.area_Inch2 == rhs.area_Inch2 && lhs.size_Str == rhs.size_Str &&
            lhs.price_Per_lb == rhs.price_Per_lb && lhs.sap_Name == rhs.sap_Name &&
            lhs.alternate_Sap == rhs.alternate_Sap && lhs.extra_value == rhs.extra_value;
    }

    class ConductorTable
    {
    public:
        double copper_Surcharge;
        double aluminum_Surcharge;

        std::vector<std::string>        tableHeaders_Magnet;
        std::vector<std::string>        tableHeaders_Strip;
        std::vector<ConductorMagnetRow> tableData_CopperMagnet;
        std::vector<ConductorStripRow>  tableData_CopperStrip;
        std::vector<ConductorMagnetRow> tableData_AluminumMagnet;
        std::vector<ConductorStripRow>  tableData_AluminumStrip;

        ConductorTable();

        // Getters
        double getCopper_Surcharge() const { return copper_Surcharge; }
        double getAluminum_Surcharge() const { return aluminum_Surcharge; }
        std::vector<std::string> getTableHeaders_Magnet() const { return tableHeaders_Magnet; }
        std::vector<std::string> getTableHeaders_Strip() const { return tableHeaders_Strip; }
        std::vector<ConductorMagnetRow> getTableData_CopperMagnet() const { return tableData_CopperMagnet; }
        std::vector<ConductorStripRow> getTableData_CopperStrip() const { return tableData_CopperStrip; }
        std::vector<ConductorMagnetRow> getTableData_AluminumMagnet() const { return tableData_AluminumMagnet; }
        std::vector<ConductorStripRow> getTableData_AluminumStrip() const { return tableData_AluminumStrip; }

        // Setters
        void setCopper_Surcharge(double v) { copper_Surcharge = v; }
        void setAluminum_Surcharge(double v) { aluminum_Surcharge = v; }
        void setTableHeaders_Magnet(const std::vector<std::string>& v) { tableHeaders_Magnet = v; }
        void setTableHeaders_Strip(const std::vector<std::string>& v) { tableHeaders_Strip = v; }
        void setTableData_CopperMagnet(const std::vector<ConductorMagnetRow>& v) { tableData_CopperMagnet = v; }
        void setTableData_CopperStrip(const std::vector<ConductorStripRow>& v) { tableData_CopperStrip = v; }
        void setTableData_AluminumMagnet(const std::vector<ConductorMagnetRow>& v) { tableData_AluminumMagnet = v; }
        void setTableData_AluminumStrip(const std::vector<ConductorStripRow>& v) { tableData_AluminumStrip = v; }
    };



    // COMPLEX INSULATION STRUCTURES (CoilBil3Ph)

    class CoilBil3Ph
    {
    public:
        std::vector<std::string> tableHeaders;
        std::vector<std::vector<double>> tableData_CoilsC88;
        std::vector<std::vector<double>> tableData_CoilsC2;

        struct InsulationLVRow { double d_Il, lt500, ge500, le300, gt300_lt1500, ge1500, selected_Pb, duct, paper_Layers, crepe_Layers, d_In; std::string arrangement; };
        std::vector<std::string> tableHeaders_CoilsInsideC88Lv;
        std::vector<InsulationLVRow> tableData_CoilsInsideC88Lv;

        struct InsulationC2LVRow { double dil, lt500, ge500, le300, gt300_Lt1500, ge1500, selected_Pb, duct, lt500_P, ge500_P, le300_P, gt300_L1500_P, ge1500_P, selected_layers, crepe_Layers, d_In; std::string arrangement; };
        std::vector<std::string> tableHeaders_CoilsInsideC2Lv;
        std::vector<InsulationC2LVRow> tableData_CoilsInsideC2Lv;

        struct InsulationHVRow { double d_Il, paper_Layers1, duct, crepe_Layers, paper_Layers2, d_In; std::string arrangement; };
        std::vector<std::string> tableHeaders_CoilsInsideC88Hv;
        std::vector<InsulationHVRow> tableData_CoilsInsideC88Hv;
        std::vector<std::string> tableHeaders_CoilsInsideC2Hv;
        std::vector<InsulationHVRow> tableData_CoilsInsideC2Hv;

        struct InsulationOverHVRow { double d_Il, paper_Layers, d_Ln; std::string arrangement; };
        std::vector<std::string> tableHeaders_CoilsInsideC88OverHv;
        std::vector<InsulationOverHVRow> tableData_CoilsInsideC88OverHv;
        std::vector<std::string> tableHeaders_CoilsInsideC2OverHv;
        std::vector<InsulationOverHVRow> tableData_CoilsInsideC2OverHv;

        struct PackingRow { double d_Il, i_Packing, o_Packing, vo_Duct1, vo_Duct2, vi_Duct1, vi_Duct2, d_Ph_Limb, d_Ph_Ph; std::string isd_Drawing; };
        std::vector<std::string> tableHeaders_CoilsC2Packing;
        std::vector<PackingRow> tableData_CoilsC2Packing;
        std::vector<std::string> tableHeaders_CoilsC88Packing;
        std::vector<PackingRow> tableData_CoilsC88Packing;

        CoilBil3Ph();

        // For brevity, getters and setters are omitted here but accessible via standard struct public access.
    };



    // COIL INSULATION STRUCTS 

    class CoilInsulation
    {
    public:
        std::vector<std::string> tableHeaders_LayerCoil;
        std::vector<std::vector<double>> tableData_LayerCoil;
        std::vector<std::string> tableHeaders_RangeKv;
        std::vector<std::vector<double>> tableData_RangeKv;
        CoilInsulation();
    };

    class CoilInsulation3Ph
    {
    public:
        std::vector<std::string> tableHeaders_LayerCoil;
        std::vector<std::vector<double>> tableData_Coil;
        std::vector<std::string> tableHeaders_RangeKv;
        std::vector<std::vector<double>> tableData_RangeKv;
        CoilInsulation3Ph();
    };



    // WINDING FORM & BARRIER STRUCTS

    class WindingForm
    {
    public:
        std::vector<std::string> tableHeaders_Windform;
        struct Row { double bil; std::string selected; double rb; };
        std::vector<Row> tableData_WindingForm;
        WindingForm();
    };

    class BodyStaybackUnderHVBarrier
    {
    public:
        std::vector<std::string> tableHeaders_SBUnderHVBarrier;
        struct Row { double bil; double stayback; std::string insulation; double rb_Sides; int cp_Layers; };
        std::vector<Row> tableData_SBUnderHVBarrier;
        BodyStaybackUnderHVBarrier();
    };

    class OverHVBarrier
    {
    public:
        std::vector<std::string> tableHeaders_OverHVBarrier;
        struct Row { double bil; double stayback; std::string barrier; double rb_Sides; double nose_Reduction; int cp_Layers; };
        std::vector<Row> tableData_OverHVBarrier;
        OverHVBarrier();
    };



    // EFFICIENCY TABLES

    class EfficiencyTable_Doe216
    {
    public:
        std::vector<std::string> tableHeaders_Efficiency1Ph_Doe216;
        std::vector<std::vector<double>> tableData_Efficiency1Ph_Doe216;
        std::vector<std::vector<double>> tableData_Efficiency3Ph_Doe216;
        EfficiencyTable_Doe216();
    };

    class EfficiencyTable_8021
    {
    public:
        std::vector<std::string> tableHeaders_EfficiencyTable1Ph_8021;
        std::vector<std::vector<double>> tableData_EfficiencyTable1Ph_8021;
        std::vector<std::vector<double>> tableData_EfficiencyTable3Ph_8021;
        EfficiencyTable_8021();
    };

    class EfficiencyTable_8023
    {
    public:
        std::vector<std::string> tableHeaders_EfficiencyTable_8023;
        std::vector<std::vector<double>> tableData_EfficiencyTable_8023_1;
        std::vector<std::vector<double>> tableData_EfficiencyTable_8023_2;
        std::vector<std::vector<double>> tableData_EfficiencyTable_8023_3;
        std::vector<std::vector<double>> tableData_EfficiencyTable3Ph_8023_1;
        std::vector<std::vector<double>> tableData_EfficiencyTable3Ph_8023_2;
        std::vector<std::vector<double>> tableData_EfficiencyTable3Ph_8023_3;
        EfficiencyTable_8023();
    };



    // GLOBAL CONSTANTS

    struct Table11_Row
    {
        std::optional<int> v_Group_V;
        int applied_Kv;
        int bil_Kv;
    };

    extern const std::vector<Table11_Row> TABLE11_CRA_C2;
}