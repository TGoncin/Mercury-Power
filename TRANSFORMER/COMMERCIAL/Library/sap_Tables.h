/**___________________________________________________________________________________________________
|    @date      May 22, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      sap_Tables.h
|    @brief     SAP Tables implemented as 1D databases for calculations
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initialization of localized inventory and pricing tables for ERP integration.
|                   - Maps CAD pricing, purchase dates, suppliers, and dimensions into memory vectors.
|               Returns:
|                   - Direct structural access to standard costs and limits for commercial calculations.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include "sap_Tables.h"


namespace Transformer::Commercial
{

//                                               Table Rows


struct ItemsSap_Row
{
    std::string itemNo;
    double      price;
    std::string lastPurchaseDate;
    std::string itemDescription;
    int         qtyInStock;
    std::string vendor;
    std::string remarks;
    std::string itemGroup;
    std::string leadTime;
    double      itemCost;
};

struct SapStandardConductors_Row
{
    std::string sapName;
    double      fabCostPerKg;
    std::string lastPriceDate;
    std::string material;
    std::string type;
    std::string shape;
    double      fullThicknessIn;
    double      insWidthIn;
    double      factorCostPerKg;
    double      totalCostPerLb;
    int         code;
};

struct SapStandardInsCoreLeads_Row
{
    std::string sapName;
    double      totalCostPerLb;
    std::string lastPriceDate;
    std::string material;
    std::string type;
    double      fullThicknessIn;
};

struct SapStandardBushings_Row
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string type;
    std::string classKV;
    std::string internalHoles;
    std::string externalHoles;
    double      maxBilKV;
    double      maxAmps;
    std::string supplier;
    std::string mounting;
};

struct SapStandardSpades_Row
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string type;
    int         externalHoles;
    std::string supplier;
    std::string diamThreadIn;
};

struct SapStandardSwitches_Row
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string type;
    int         classKV;
    int         maxBilKV;
    std::string maxAmps;
    std::string supplier;
    int         positions;
};

struct SapStandardFuses_Row
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string type;
    double      classKV;
    std::string maxAmps;
    std::string supplier;
};

struct SapStandardGauges_Row
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string supplier;
    std::string contacts;
    std::string dialIn;
};

struct MechanicalTankStructuralSteel_ListRow
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    int         quantity;
    std::string partNoRev;
    std::string description;
    std::string material;
    double      qtyWt;
    double      wtLb;
    std::string issue;
};

struct Mechanical_CommonRow
{
    std::string sapName;
    double      cadPrice;
    std::string lastPriceDate;
    std::string component;
    std::string supplier;
};

/*____________________________________________________________________________________________________________________________
|
|                                            Class Definitions for SAP tables
|____________________________________________________________________________________________________________________________*/

    class ItemsSap
    {
        public:
            std::string                     name;
            std::vector<std::string>        tableHeaders;
            std::vector<ItemsSap_Row>       tableData;
            ItemsSap();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<ItemsSap_Row> getTableData() const                  { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<ItemsSap_Row>& v)           { tableData = v; }
    };


    class SapStandardConductors
    {
        public:
            std::string                                 name;
            std::vector<std::string>                    tableHeaders;
            std::vector<SapStandardConductors_Row>      tableData;
            SapStandardConductors();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardConductors_Row> getTableData() const     { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardConductors_Row>& v){ tableData = v; }
    };


    class SapStandardCoreLeads
    {
        public:
            std::string                                 name;
            std::vector<std::string>                    tableHeaders;
            std::vector<SapStandardInsCoreLeads_Row>    tableData;
            SapStandardCoreLeads();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardInsCoreLeads_Row> getTableData() const   { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardInsCoreLeads_Row>& v){ tableData = v; }
    };


    class SapStandardBushings
    {
        public:
            std::string                           name;
            std::vector<std::string>              tableHeaders;
            std::vector<SapStandardBushings_Row>  tableData;
            SapStandardBushings();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardBushings_Row> getTableData() const       { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardBushings_Row>& v){ tableData = v; }
    };


    class SapStandardSpades
    {
        public:
            std::string                           name;
            std::vector<std::string>              tableHeaders;
            std::vector<SapStandardSpades_Row>    tableData;
            SapStandardSpades();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardSpades_Row> getTableData() const         { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardSpades_Row>& v)  { tableData = v; }
    };


    class SapStandardSwitches
    {
        public:
            std::string                           name;
            std::vector<std::string>              tableHeaders;
            std::vector<SapStandardSwitches_Row>  tableData;
            SapStandardSwitches();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardSwitches_Row> getTableData() const       { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardSwitches_Row>& v){ tableData = v; }
    };


    class SapStandardFuses
    {
        public:
            std::string                         name;
            std::vector<std::string>            tableHeaders;
            std::vector<SapStandardFuses_Row>   tableData;
            SapStandardFuses();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardFuses_Row> getTableData() const          { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardFuses_Row>& v)   { tableData = v; }
    };


    class SapStandardGauges
    {
        public:
            std::string                         name;
            std::vector<std::string>            tableHeaders;
            std::vector<SapStandardGauges_Row>  tableData;
            SapStandardGauges();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<SapStandardGauges_Row> getTableData() const         { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<SapStandardGauges_Row>& v)  { tableData = v; }
    };


    class MechTankAccessoriesStructuralSteel
    {
        public:
            std::string                                           name;
            std::vector<std::string>                              tableHeaders;
            std::vector<MechanicalTankStructuralSteel_ListRow>    tableData;
            MechTankAccessoriesStructuralSteel();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                                 { return name; }
            std::vector<std::string> getTableHeaders() const                            { return tableHeaders; }
            std::vector<MechanicalTankStructuralSteel_ListRow> getTableData() const     { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                                          { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)                     { tableHeaders = v; }
            void setTableData(const std::vector<MechanicalTankStructuralSteel_ListRow>& v){ tableData = v; }
    };


    class MechComponents
    {
        public:
            std::string                            name;
            std::vector<std::string>               tableHeaders;
            std::vector<Mechanical_CommonRow>      tableData;
            MechComponents();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<Mechanical_CommonRow> getTableData() const          { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<Mechanical_CommonRow>& v)   { tableData = v; }
    };


    class MechPurchasedTankAccessories
    {
        public:
            std::string                            name;
            std::vector<std::string>               tableHeaders;
            std::vector<Mechanical_CommonRow>      tableData;
            MechPurchasedTankAccessories();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<Mechanical_CommonRow> getTableData() const          { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<Mechanical_CommonRow>& v)   { tableData = v; }
    };


    class MechTankSteelOil
    {
        public:
            std::string                            name;
            std::vector<std::string>               tableHeaders;
            std::vector<Mechanical_CommonRow>      tableData;
            MechTankSteelOil();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/
            std::string getName() const                                     { return name; }
            std::vector<std::string> getTableHeaders() const                { return tableHeaders; }
            std::vector<Mechanical_CommonRow> getTableData() const          { return tableData; }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/
            void setName(const std::string& v)                              { name = v; }
            void setTableHeaders(const std::vector<std::string>& v)         { tableHeaders = v; }
            void setTableData(const std::vector<Mechanical_CommonRow>& v)   { tableData = v; }
    };
}