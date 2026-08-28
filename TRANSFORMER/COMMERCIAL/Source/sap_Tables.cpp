/**___________________________________________________________________________________________________
|    @date      May 22, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      sap_Tables.cpp
|    @brief     SAP Tables implemented as 1D databases for calculations
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initialization of localized inventory and pricing tables for ERP integration.
|                   - Maps CAD pricing, purchase dates, suppliers, and dimensions into memory vectors.
|               Returns:
|                   - Direct structural access to standard costs and limits for commercial calculations.
|___________________________________________________________________________________________________*/

#include "sap_Tables.h"



/*_______________________________________________________________________________
|
|                         Constructor for SAP items table
_______________________________________________________________________________*/

namespace Transformer::Commercial
{

    ItemsSap::ItemsSap()
    {
        name = "Item SAP";
        tableHeaders = 
        {
            "Item No.",
            "Price",
            "Last Purchase Date",
            "Item Description",
            "Qty In Stock",
            "Vendor",
            "Remarks",
            "Item Group",
            "Lead Time",
            "Item Cost" 
        };
        tableData = {};
    }

    /*_______________________________________________________________________________
    |
    |                    Raw Magnet Wire Coils & Foil Conductors   
    |_______________________________________________________________________________*/


    SapStandardConductors::SapStandardConductors()
    {
        name = "SAP Standard Conductors";
        tableHeaders = 
        {
            "SAP_Name",
            "Fab_cost_per_kg-SAP",
            "Last_price_date",
            "Material",
            "Type",
            "Shape",
            "Full_thickness_in",
            "Ins_Width_in",
            "Factor_cost_per_kg",
            "Total_cost_per_lb",
            "Code"  
        };
/* *********************************************************************************************************************************

    |                                               - HISTORICAL SNAPSHOT -

    |                                       Standard Commercial Slit Material Ledger 

    |             I THINK THIS IS LEGACY DATA         NOT USED IN CALCULATIONS        JUST FOR REFERENCE.

************************************************************************************************************************************
        tb_data = 
        {
            { "0.0118",      0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0118, 0.0000, 4.82,  2.1909,  18  },
            { "0.018",       0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0180, 0.0000, 4.82,  2.1909,  16  },
            { "0.027",       0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0270, 0.0000, 4.82,  2.1909,  12  },
            { "0.0315",      0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0315, 0.0000, 4.82,  2.1909,  9   },
            { "0.045",       0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0450, 0.0000, 4.82,  2.1909,  6   },
            { "0.0625",      0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0625, 0.0000, 4.82,  2.1909,  1   },
            { "10.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0993, 0.0993, 4.81,  2.1864,  91  },
            { "102X204HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1060, 0.2080, 5.91,  7.1757,  95  },
            { "102X306HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1060, 0.3100, 5.91,  5.0857,  97  },
            { "10HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.1054, 0.1054, 4.81,  4.4699,  92  },
            { "11.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0887, 0.0887, 4.81,  2.1864,  89  },
            { "114X229HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1180, 0.2330, 5.91,  7.0368,  96  },
            { "116X300HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1200, 0.3040, 5.91,  2.6864,  94  },
                { "11HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0942, 0.0942, 4.81,  5.4720,  90  },
                { "12.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0793, 0.0793, 4.81,  3.9919,  87  },
                { "128X256HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.2600, 5.91,  6.8493,  98  },
                { "128X325HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.3290, 5.91,  6.7308,  99  },
                { "128X460HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.4640, 5.91,  6.4063,  103 },
                { "12HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0842, 0.0842, 4.81,  3.6847,  88  },
                { "13HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0754, 0.0754, 4.81,  5.4958,  86  },
                { "144X289HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.2930, 5.91,  5.6265,  100 },
                { "144X325HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.3290, 5.91,  6.7124,  101 },
                { "144X365HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.3690, 5.91,  6.3341,  102 },
                { "144X410HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.4140, 5.91,  5.0446,  104 },
                { "14HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0675, 0.0675, 4.81,  4.6335,  85  },
                { "15HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0603, 0.0603, 4.81,  5.6635,  84  },
                { "162X460HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1660, 0.4640, 5.91,  6.6255,  105 },
                { "16HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0539, 0.0539, 4.81,  2.1864,  83  },
                { "17HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0482, 0.0482, 4.81,  5.5984,  82  },
                { "18HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0431, 0.0431, 4.81,  5.4511,  81  },
                { "19HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0385, 0.0385, 4.81,  5.8402,  80  },
                { "9HFA",        0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.1179, 0.1179, 4.81,  4.8030,  93  },
                { "9HMF",        0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0000, 0.0000, 0.00,  0.0000,  0   },
                { "0.003",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0030, 0.0000, 14.01, 6.3682,  47  },
                { "0.005",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0050, 0.0000, 14.01, 6.3682,  46  },
                { "0.006",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0060, 0.0000, 14.01, 6.3682,  45  },
                { "0.007",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0070, 0.0000, 14.01, 6.3682,  44  },
                { "0.01",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0100, 0.0000, 14.01, 6.3682,  43  },
                { "0.012",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0120, 0.0000, 14.01, 6.3682,  42  },
                { "0.013",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0130, 0.0000, 14.01, 6.3682,  41  },
                { "0.0155",      0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0155, 0.0000, 14.01, 6.3682,  40  },
                { "0.02",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0200, 0.0000, 14.01, 6.3682,  38  },
                { "0.022",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0220, 0.0000, 14.01, 6.3682,  37  },
                { "0.025",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0250, 0.0000, 14.01, 6.3682,  36  },
                { "0.028",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0280, 0.0000, 14.01, 6.3682,  35  },
                { "0.03",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0300, 0.0000, 14.01, 6.3682,  34  },
                { "0.032",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0320, 0.0000, 14.01, 6.3682,  33  },
                { "0.035",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0350, 0.0000, 14.01, 6.3682,  32  },
                { "0.04",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0400, 0.0000, 14.01, 6.3682,  31  },
                { "0.044",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0440, 0.0000, 14.01, 6.3682,  30  },
                { "0.047",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0470, 0.0000, 14.01, 6.3682,  29  },
                { "0.05",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0500, 0.0000, 14.01, 6.3682,  28  },
                { "0.055",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0550, 0.0000, 14.01, 6.3682,  27  },
                { "0.06",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0600, 0.0000, 14.01, 6.3682,  26  },
                { "0.062",       0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0620, 0.0000, 14.01, 6.3682,  25  },
                { "115X190HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0000, 0.0000, 0.00,  0.0000,  0   },
                { "072X144HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0760, 0.1480, 16.76, 12.2788, 31  },
                { "081X162HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0850, 0.1660, 16.76, 12.4952, 32  },
                { "091X182HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0950, 0.1860, 16.76, 11.4939, 33  },
                { "10.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0993, 0.0993, 14.56, 9.2463,  29  },
                { "102X204HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1060, 0.2080, 16.76, 12.1296, 34  },
                { "102X306HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1060, 0.3100, 16.76, 11.9270, 36  },
                { "10HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.1050, 0.1050, 14.56, 9.3286,  30  },
                { "11.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0887, 0.0887, 14.56, 9.1950,  27  },
                { "114X229HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1180, 0.2330, 16.76, 12.2186, 35  },
                { "11HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0938, 0.0938, 14.56, 10.6288, 28  },
                { "12.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0793, 0.0793, 14.56, 9.1689,  25  },
                { "128X256HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.2600, 16.76, 12.2698, 37  },
                { "128X325HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.3290, 16.76, 12.3407, 38  },
                { "128X460HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.4640, 16.76, 12.2225, 42  },
                { "12HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0838, 0.0838, 14.56, 9.3033,  26  },
                { "13HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0749, 0.0749, 14.56, 10.3717, 24  },
                { "144X289HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.2930, 16.76, 10.7650, 39  },
                { "144X325HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.3290, 16.76, 12.0945, 40  },
                { "144X365HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.3690, 16.76, 12.0907, 41  },
                { "144X410HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.4140, 16.76, 10.4200, 43  },
                { "14HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0674, 0.0674, 14.56, 9.2334,  23  },
                { "15HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0602, 0.0602, 14.56, 10.6080, 22  },
                { "162X460HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1660, 0.4640, 16.76, 11.7012, 44  },
                { "16HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0538, 0.0538, 14.56, 10.7200, 21  },
                { "17HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0482, 0.0482, 14.56, 9.8610,  20  },
                { "18HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0431, 0.0431, 14.56, 9.3352,  19  },
                { "19HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0385, 0.0385, 14.56, 9.2725,  18  },
                { "20HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0345, 0.0345, 14.56, 9.3806,  17  },
                { "21HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0309, 0.0309, 14.56, 9.3020,  16  },
                { "22HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0276, 0.0276, 14.56, 9.2679,  15  },
                { "23HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0248, 0.0248, 14.56, 9.2456,  14  },
                { "24HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0222, 0.0222, 14.56, 9.4143,  13  },
                { "25HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0199, 0.0199, 14.56, 9.6682,  12  },
                { "26HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0178, 0.0178, 14.56, 9.0063,  11  },
                { "27HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0160, 0.0160, 14.56, 13.3939, 10  },
                { "28HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0144, 0.0144, 14.56, 10.3818, 9   }
            }; */
        tableData = 
        {
            { "28HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0144, 0.0144, 14.56, 10.3818, 9   },
            { "27HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0160, 0.0160, 14.56, 13.3938, 10  },
            { "26HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0178, 0.0178, 14.56, 9.0063,  11  },
            { "25HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0199, 0.0199, 14.56, 9.6682,  12  },
            { "24HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0222, 0.0222, 14.56, 9.4143,  13  },
            { "23HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0248, 0.0248, 14.56, 9.2456,  14  },
            { "22HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0276, 0.0276, 14.56, 9.2679,  15  },
            { "21HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0309, 0.0309, 14.56, 9.3020,  16  },
            { "20HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0345, 0.0345, 14.56, 9.3806,  17  },
            { "19HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0385, 0.0385, 14.56, 9.2725,  18  },
            { "18HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0431, 0.0431, 14.56, 9.3352,  19  },
            { "17HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0482, 0.0482, 14.56, 9.8610,  20  },
            { "16HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0538, 0.0538, 14.56, 10.7200, 21  },
            { "15HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0602, 0.0602, 14.56, 10.6080, 22  },
            { "14HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0674, 0.0674, 14.56, 9.2334,  23  },
            { "13HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0749, 0.0749, 14.56, 10.3717, 24  },
            { "12.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0793, 0.0793, 14.56, 9.1689,  25  },
            { "12HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0838, 0.0838, 14.56, 9.3033,  26  },
            { "11.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0887, 0.0887, 14.56, 9.1950,  27  },
            { "11HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0938, 0.0938, 14.56, 10.6288, 28  },
            { "10.5HMF",     0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.0993, 0.0993, 14.56, 9.2463,  29  },
            { "10HMF",       0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.1050, 0.1050, 14.56, 9.3286,  30  },
            { "9HMF",        0.00, "01/01/2000", "Copper",   "Conductor", "Circular",    0.1050, 0.1050, 14.56, 9.3286,  30  },
            { "072X144HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0760, 0.1480, 16.76, 12.2788, 31  },
            { "081X162HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0850, 0.1660, 16.76, 12.4952, 32  },
            { "091X182HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.0950, 0.1860, 16.76, 11.4939, 33  },
            { "102X204HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1060, 0.2080, 16.76, 12.1296, 34  },
            { "115X190HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1060, 0.2080, 16.76, 12.1296, 34  },
            { "114X229HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1180, 0.2330, 16.76, 12.2186, 35  },
            { "102X306HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1060, 0.3100, 16.76, 11.9270, 36  },
            { "128X256HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.2600, 16.76, 12.2698, 37  },
            { "128X325HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.3290, 16.76, 12.3407, 38  },
            { "144X289HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.2930, 16.76, 10.7650, 39  },
            { "144X325HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.3290, 16.76, 12.0945, 40  },
            { "144X365HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.3690, 16.76, 12.0907, 41  },
            { "128X460HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1320, 0.4640, 16.76, 12.2225, 42  },
            { "144X410HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1480, 0.4140, 16.76, 10.4200, 43  },
            { "162X460HMF",  0.00, "01/01/2000", "Copper",   "Conductor", "Rectangular", 0.1660, 0.4640, 16.76, 11.7012, 44  },
            { "062",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0620, 0.0000, 14.01, 6.3682,  25  },
            { "060",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0600, 0.0000, 14.01, 6.3682,  26  },
            { "055",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0550, 0.0000, 14.01, 6.3682,  27  },
            { "050",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0500, 0.0000, 14.01, 6.3682,  28  },
            { "047",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0470, 0.0000, 14.01, 6.3682,  29  },
            { "044",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0440, 0.0000, 14.01, 6.3682,  30  },
            { "040",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0400, 0.0000, 14.01, 6.3682,  31  },
            { "035",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0350, 0.0000, 14.01, 6.3682,  32  },
            { "032",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0320, 0.0000, 14.01, 6.3682,  33  },
            { "030",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0300, 0.0000, 14.01, 6.3682,  34  },
            { "028",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0280, 0.0000, 14.01, 6.3682,  35  },
            { "025",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0250, 0.0000, 14.01, 6.3682,  36  },
            { "022",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0220, 0.0000, 14.01, 6.3682,  37  },
            { "020",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0200, 0.0000, 14.01, 6.3682,  38  },
            { "018",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0180, 0.0000, 14.01, 6.3682,  39  },
            { "0155",        0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0155, 0.0000, 14.01, 6.3682,  40  },
            { "013",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0130, 0.0000, 14.01, 6.3682,  41  },
            { "012",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0120, 0.0000, 14.01, 6.3682,  42  },
            { "010",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0100, 0.0000, 14.01, 6.3682,  43  },
            { "007",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0070, 0.0000, 14.01, 6.3682,  44  },
            { "006",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0060, 0.0000, 14.01, 6.3682,  45  },
            { "005",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0050, 0.0000, 14.01, 6.3682,  46  },
            { "003",         0.00, "01/01/2000", "Copper",   "Conductor", "Strip",       0.0030, 0.0000, 14.01, 6.3682,  47  },
            { "19HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0385, 0.0385, 4.81,  5.8402,  80  },
            { "18HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0431, 0.0431, 4.81,  5.4511,  81  },
            { "17HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0482, 0.0482, 4.81,  5.5984,  82  },
            { "16HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0539, 0.0539, 4.81,  2.1864,  83  },
            { "15HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0603, 0.0603, 4.81,  5.6635,  84  },
            { "14HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0675, 0.0675, 4.81,  4.6335,  85  },
            { "13HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0754, 0.0754, 4.81,  5.4958,  86  },
            { "12.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0793, 0.0793, 4.81,  3.9919,  87  },
            { "12HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0842, 0.0842, 4.81,  3.6847,  88  },
            { "11.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0887, 0.0887, 4.81,  2.1864,  89  },
            { "11HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0942, 0.0942, 4.81,  5.4720,  90  },
            { "10.5HFA",     0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.0993, 0.0993, 4.81,  2.1864,  91  },
            { "10HFA",       0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.1054, 0.1054, 4.81,  4.4699,  92  },
            { "9HFA",        0.00, "01/01/2000", "Aluminum", "Conductor", "Circular",    0.1179, 0.1179, 4.81,  4.8030,  93  },
            { "116X300HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1200, 0.3040, 5.91,  2.6864,  94  },
            { "102X204HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1060, 0.2080, 5.91,  7.1757,  95  },
            { "114X229HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1180, 0.2330, 5.91,  7.0368,  96  },
            { "102X306HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1060, 0.3100, 5.91,  5.0857,  97  },
            { "128X256HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.2600, 5.91,  6.8493,  98  },
            { "128X325HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.3290, 5.91,  6.7308,  99  },
            { "144X289HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.2930, 5.91,  5.6265,  100 },
            { "144X325HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.3290, 5.91,  6.7124,  101 },
            { "144X365HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.3690, 5.91,  6.3341,  102 },
            { "128X460HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1320, 0.4640, 5.91,  6.4063,  103 },
            { "144X410HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1480, 0.4140, 5.91,  5.0446,  104 },
            { "162X460HFA",  0.00, "01/01/2000", "Aluminum", "Conductor", "Rectangular", 0.1660, 0.4640, 5.91,  6.6255,  105 },
            { "063",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0625, 0.0000, 4.82,  2.1909,  1   },
            { "060",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0600, 0.0000, 4.82,  2.1909,  2   },
            { "055",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0550, 0.0000, 4.82,  2.1909,  3   },
            { "050",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0500, 0.0000, 4.82,  2.1909,  4   },
            { "047",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0470, 0.0000, 4.82,  2.1909,  5   },
            { "045",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0450, 0.0000, 4.82,  2.1909,  6   },
            { "040",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0400, 0.0000, 4.82,  2.1909,  7   },
            { "035",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0350, 0.0000, 4.82,  2.1909,  8   },
            { "0315",        0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0315, 0.0000, 4.82,  2.1909,  9   },
            { "030",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0300, 0.0000, 4.82,  2.1909,  10  },
            { "028",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0280, 0.0000, 4.82,  2.1909,  11  },
            { "027",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0270, 0.0000, 4.82,  2.1909,  12  },
            { "025",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0250, 0.0000, 4.82,  2.1909,  13  },
            { "022",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0220, 0.0000, 4.82,  2.1909,  14  },
            { "020",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0200, 0.0000, 4.82,  2.1909,  15  },
            { "018",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0180, 0.0000, 4.82,  2.1909,  16  },
            { "0155",        0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0155, 0.0000, 4.82,  2.1909,  17  },
            { "0118",        0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0118, 0.0000, 4.82,  2.1909,  18  },
            { "010",         0.00, "01/01/2000", "Aluminum", "Conductor", "Strip",       0.0100, 0.0000, 4.82,  2.1909,  19  }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                             Leads, Internal Busbars & Heavy Insulation Materials
    |____________________________________________________________________________________________________________________________*/



    SapStandardCoreLeads::SapStandardCoreLeads()
    {
        name = "SAP Standard Internal Core Leads";
        tableHeaders = 
        {
            "SAP_Name",
            "Total_cost_per_lb- SAP",
            "Last_price_date",
            "Material",
            "Type",
            "Full_thickness_in"
        };
        tableData = 
        {
            { "047X2AL",   0.00, "01/01/2000", "Aluminum",      "Lead",       0.0470 },
            { "063X3AL",   0.00, "01/01/2000", "Aluminum",      "Lead",       0.0630 },
            { "079X4AL",   0.00, "01/01/2000", "Aluminum",      "Lead",       0.0790 },
            { "090X2AL",   0.00, "01/01/2000", "Aluminum",      "Lead",       0.0900 },
            { "15CUBAR",   0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "2ALFRE",    3.49, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "2CUBAR",    0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "3ALFRE",    0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "3CUBAR",    0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "4ALFRE",    0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "4CUBAR",    0.00, "01/01/2000", "Copper",        "Bar",        0.0000 },
            { "011CU",     0.00, "01/01/2000", "Copper",        "Lead",       0.0100 },
            { "050X3SCU",  0.00, "01/01/2000", "Copper",        "Lead",       0.0500 },
            { "050X4SCU",  0.00, "01/01/2000", "Copper",        "Lead",       0.0500 },
            { "052CU",     0.00, "01/01/2000", "Copper",        "Lead",       0.0500 },
            { "0931CU",    0.00, "01/01/2000", "Copper",        "Lead",       0.0000 },
            { "510HC",     0.00, "01/01/2000", "Crepe",         "Insulation", 0.0120 },
            { "316DUCT",   0.00, "01/01/2000", "Duct",          "Insulation", 0.1875 },
            { "4DUCT",     0.00, "01/01/2000", "Duct",          "Insulation", 0.2500 },
            { "8DUCT",     0.00, "01/01/2000", "Duct",          "Insulation", 0.1250 },
            { "10KP",      0.00, "01/01/2000", "Kraft",         "Insulation", 0.1000 },
            { "2KP",       0.00, "01/01/2000", "Kraft",         "Insulation", 0.0200 },
            { "5KP",       0.00, "01/01/2000", "Kraft",         "Insulation", 0.0500 },
            { "10KP",      0.00, "01/01/2000", "Kraft",         "Insulation", 0.1000 },
            { "NA",        0.00, "01/01/2000", "MOH",           "Core",       0.2300 },
            { "16PB",      0.00, "01/01/2000", "Pressboard",    "Insulation", 0.0625 },
            { "8PB",       0.00, "01/01/2000", "Pressboard",    "Insulation", 0.1250 },
            { "4PB_HD",    0.00, "01/01/2000", "Pressboard_HD", "Insulation", 0.2500 }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                             High-Voltage Porcelain Bushings & Low-Voltage Connectors
    |____________________________________________________________________________________________________________________________*/



    SapStandardBushings::SapStandardBushings()
    {
        name = "SAP Standard Bushings";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "Type",
            "Class_kV",
            "Internal Holes",
            "External Holes",
            "Max_BIL_kV",
            "Max_Amps",
            "Supplier",
            "Mounting"
        };
        tableData = 
        {
            { "17P0048B",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "2",   95.0,  50.0,    "PTI",      "Both"       },
            { "17P0048L",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "4",   95.0,  50.0,    "PTI",      "Both"       },
            { "17P0048M",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "2",   95.0,  50.0,    "PTI",      "Both"       },
            { "17P0099A",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "4",   95.0,  400.0,   "PTI",      "Both"       },
            { "17P0099B",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "2",   95.0,  400.0,   "PTI",      "Both"       },
            { "17P0099C",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "2",   95.0,  400.0,   "PTI",      "Both"       },
            { "17P0106A",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "15",  "0",   "2",   95.0,  400.0,   "PTI",      "Both"       },
            { "17P0021D",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "25",  "0",   "2",   125.0, 50.0,    "PTI",      "Both"       },
            { "17P0021E",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "25",  "0",   "4",   125.0, 50.0,    "PTI",      "Both"       },
            { "17P0101A",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "25",  "0",   "2",   125.0, 400.0,   "PTI",      "Both"       },
            { "17P0101B",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "25",  "0",   "2",   125.0, 400.0,   "PTI",      "Both"       },
            { "17P0101C",             0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "25",  "0",   "4",   125.0, 400.0,   "PTI",      "Both"       },
            { "17P0031D",             0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "15",  "0",   "2",   150.0, 50.0,    "PTI",      "Both"       },
            { "17P0031H",             0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "15",  "0",   "2",   150.0, 50.0,    "PTI",      "Both"       },
            { "17P0078A",             0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "15",  "0",   "2",   150.0, 50.0,    "PTI",      "Both"       },
            { "17P0078C",             0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "15",  "0",   "2",   150.0, 400.0,   "PTI",      "Both"       },
            { "17P0078D",             0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "15",  "0",   "2",   150.0, 400.0,   "PTI",      "Both"       },
            { "17P0100A",             0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "15",  "0",   "2",   150.0, 400.0,   "PTI",      "Both"       },
            { "17P0100B",             0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "15",  "0",   "2",   150.0, 400.0,   "PTI",      "Both"       },
            { "17P0100C",             0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "15",  "0",   "4",   150.0, 400.0,   "PTI",      "Both"       },
            { "17A0140",              0.0, "01/01/2000", "LV_BUSHING_TRICLAMP",     "LIVEFRONT", "0.6", "0",   "1",   30.0,  600.0,   "EATON",    "Horizontal" },
            { "EPC10-014-500E07T",    0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "4",   30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-501E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "6",   30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-502E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "8",   30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-503E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "10",  30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-504E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "12",  30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-511",        0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "6",   30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-512E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "8",   30.0,  1210.0,  "HJ",       "Horizontal" },
            { "EPC10-014-600E01T",    0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "4",   30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-601E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "6",   30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-601E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "6",   30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-602E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "8",   30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-603E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "10",  30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-604E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "12",  30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-014-612E01T-03", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "0.6", "2",   "8",   30.0,  2410.0,  "HJ",       "Horizontal" },
            { "EPC10-042-005E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "14",  45.0,  0.0,     "HJ",       "Horizontal" },
            { "EPC10-512-000E011-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "4",   45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-001E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "6",   45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-002E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "8",   45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-003E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "10",  45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-004E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "12",  45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-006E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "16",  45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-102E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "8",   45.0,  3010.0,  "HJ",       "Horizontal" },
            { "EPC10-512-008E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "20",  45.0,  3150.0,  "HJ",       "Horizontal" },
            { "EPC10-513-900E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "4",   45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-901E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "6",   45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-902E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "8",   45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-903E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "10",  45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-904E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "12",  45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-906E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "16",  45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-513-908E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "4",   "20",  45.0,  4300.0,  "HJ",       "Horizontal" },
            { "EPC10-505-110E07T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "16",  45.0,  4515.0,  "HJ",       "Horizontal" },
            { "EPC10-511-103 (002)",  0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "8",   45.0,  4515.0,  "HJ",       "Horizontal" },
            { "EPC10-511-105E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "8",   45.0,  4515.0,  "HJ",       "Horizontal" },
            { "EPC10-042-000E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "4",   45.0,  5000.0,  "HJ",       "Horizontal" },
            { "EPC10-042-001E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "6",   45.0,  5000.0,  "HJ",       "Horizontal" },
            { "EPC10-042-002E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "8",   45.0,  5000.0,  "HJ",       "Horizontal" },
            { "EPC10-042-003E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "10",  45.0,  5000.0,  "HJ",       "Horizontal" },
            { "EPC10-042-004E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "12",  45.0,  5000.0, " HJ",       "Horizontal" },
            { "EPC10-042-006E01T-01", 0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "2.5", "0",   "16",  45.0,  5000.0,  "HJ",       "Horizontal" },
            { "EPC10-500-037",        0.0, "01/01/2000", "LV_BUSHING_PORCELAIN",    "LIVEFRONT", "5",   "6",   "10",  60.0,  6500.0,  "HJ",       "Horizontal" },
            { "LBI215BP",             0.0, "01/01/2000", "BUSHING_INSERT",          "DEADFRONT", "15",  "N/A", "N/A", 95.0,  200.0,   "EATON",    "Horizontal" },
            { "LBI225BP",             0.0, "01/01/2000", "BUSHING_INSERT",          "DEADFRONT", "25",  "N/A", "N/A", 125.0, 200.0,   "EATON",    "Horizontal" },
            { "BW150R",               0.0, "01/01/2000", "BUSHING_WELL",            "DEADFRONT", "35",  "N/A", "N/A", 150.0, 200.0,   "EATON",    "Horizontal" },
            { "CM70191972",           0.0, "01/01/2000", "BUSHING_WELL",            "DEADFRONT", "35",  "N/A", "N/A", 150.0, 200.0,   "HJ",       "Horizontal" },
            { "LB235B150",            0.0, "01/01/2000", "BUSHING_INTEGRAL",        "DEADFRONT", "35",  "N/A", "N/A", 150.0, 200.0,   "EATON",    "Horizontal" },
            { "390780",               0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "46",  "0",   "0",   250.0, 994.0,   "FOSTORIA", "Horizontal" },
            { "45A0005574",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  785.0,   "WARCO",    "Vertical"   },
            { "45A03172‐4",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  785.0,   "WARCO",    "Vertical"   },
            { "45A0005575",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  994.0,   "WARCO",    "Vertical"   },
            { "45A03173‐2",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  994.0,   "WARCO",    "Vertical"   },
            { "45A0005576",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  1227.0,  "WARCO",    "Vertical"   },
            { "45A03174‐3",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  1227.0,  "WARCO",    "Vertical"   },
            { "45A0005577",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  1767.0,  "WARCO",    "Vertical"   },
            { "45A03175‐3",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  1767.0,  "WARCO",    "Vertical"   },
            { "45A0005578",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  2070.0,  "WARCO",    "Vertical"   },
            { "45A03176‐2",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  2070.0,  "WARCO",    "Vertical"   },
            { "45A0005579",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  2400.0,  "WARCO",    "Vertical"   },
            { "45A03177‐4",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  2400.0,  "WARCO",    "Vertical"   },
            { "45A0005580",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  3140.0,  "WARCO",    "Vertical"   },
            { "45A03178‐3",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  3140.0,  "WARCO",    "Vertical"   },
            { "45A0005581",           0.0, "01/01/2000", "BUSHING_PORCELAIN_SHORT", "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  3540.0,  "WARCO",    "Vertical"   },
            { "45A03179‐1",           0.0, "01/01/2000", "BUSHING_PORCELAIN_LONG",  "LIVEFRONT", "5",   "N/A", "N/A", 75.0,  3540.0,  "WARCO",    "Vertical"   },
            { "39A03064-2",           0.0, "01/01/2000", "BUSHING_PORCELAIN",       "LIVEFRONT", "46",  "0",   "4",   250.0, 306.0,   "WARCO",    "Vertical"   }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                             Terminal Spades & Structural Connector Plates       
    |____________________________________________________________________________________________________________________________*/



    SapStandardSpades::SapStandardSpades()
    {
        name = "SAP Standard Spades";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "Type",
            "External Holes",
            "Supplier",
            "Diam_Thread_in"
        };
        tableData = 
        {
            { "1235-1",  0.0, "01/01/2000", "SPADE", "INLINE",            7, "KENALLOY", "0.625-11" },
            { "1235-2",  0.0, "01/01/2000", "SPADE", "INLINE",            7, "KENALLOY", "0.625-11" },
            { "0763-0",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.75-12"   },
            { "0763-1",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "46065"     },
            { "0765-30", 0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "2.125-12"  },
            { "1081-2",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "46036"     },
            { "1081-3",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.125-12"  },
            { "1082-1",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.25-12"   },
            { "1082-2",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.5-12"    },
            { "1082-3",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.75-12"   },
            { "1082-4",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "46065"     },
            { "1082-5",  0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "2.125-12"  },
            { "1082-56", 0.0, "01/01/2000", "SPADE", "RECTANGULAR_FLAT", 4, "KENALLOY", "1.625-12"  }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                           Tap Changer Hardware, Switches & Rotary Assemblies    
    |____________________________________________________________________________________________________________________________*/



    SapStandardSwitches::SapStandardSwitches()
    {
        name = "SAP Standard Switches";
        tableHeaders =
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "Type",
            "Class_kV",
            "Max_BIL_kV",
            "Max_Amps",
            "Supplier",
            "Positions"
        };
        tableData = 
        {
            { "2237947A302H",        0.0, "01/01/2000", "TAPCHANGER_HARDWARE", "HOTSTICK_HANDLE_KIT", 35, 150, "N/A", "EATON",   5 },
            { "LS2R338H1N1A",        0.0, "01/01/2000", "SWITCH",              "LOADBREAK",           38, 150, "300", "EATON",   2 },
            { "1L0122505BG1-000-00", 0.0, "01/01/2000", "TAPCHANGER",          "ROTARY",              46, 250, "306", "Qswitch", 5 },
            { "2237179C01M",         0.0, "01/01/2000", "TAPCHANGER",          "ROTARY",              35, 150, "300", "EATON",   5 },
            { "2237472C01M",         0.0, "01/01/2000", "TAPCHANGER",          "ROTARY",              35, 150, "150", "EATON",   5 },
            { "2237500C03",          0.0, "01/01/2000", "TAPCHANGER",          "ROTARY",              35, 150, "100", "EATON",   5 }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                                Core Fuses & System Protection Accessories
    |____________________________________________________________________________________________________________________________*/



    SapStandardFuses::SapStandardFuses() 
    {
        name = "SAP Standard Fuses";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "Type",
            "Class_kV",
            "Max_Amps",
            "Supplier"
        };
        tableData = 
        {
            { "CBUC08030C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "30",   "EATON" },
            { "CBUC08040C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "40",   "EATON" },
            { "CBUC08050C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "50",   "EATON" },
            { "CBUC08065C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "65",   "EATON" },
            { "CBUC08080C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "80",   "EATON" },
            { "CBUC08100C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "100",  "EATON" },
            { "CBUC08125C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "125",  "EATON" },
            { "CBUC08150D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "150",  "EATON" },
            { "CBUC08165D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "165",  "EATON" },
            { "CBUC08180D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "180",  "EATON" },
            { "CBUC08250D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   8.3, "250",  "EATON" },
            { "CBUC15030C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "30",  "EATON" },
            { "CBUC15040C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "40",  "EATON" },
            { "CBUC15050C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "50",  "EATON" },
            { "CBUC15065C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "65",  "EATON" },
            { "CBUC15080C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "80",  "EATON" },
            { "CBUC15100C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "100", "EATON" },
            { "CBUC15125C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "125", "EATON" },
            { "CBUC15150D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "150", "EATON" },
            { "CBUC15165D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "165", "EATON" },
            { "CBUC15180D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   15.5, "180", "EATON" },
            { "CBUC17040C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   17.2, "40",  "EATON" },
            { "CBUC17050C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   17.2, "50",  "EATON" },
            { "CBUC17065C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   17.2, "65",  "EATON" },
            { "CBUC23030C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "30",  "EATON" },
            { "CBUC23040C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "40",  "EATON" },
            { "CBUC23050C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "50",  "EATON" },
            { "CBUC23065C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "65",  "EATON" },
            { "CBUC23080C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "80",  "EATON" },
            { "CBUC23100C100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "100", "EATON" },
            { "CBUC23125D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "125", "EATON" },
            { "CBUC23150D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "150", "EATON" },
            { "CBUC23165D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   23.0, "165", "EATON" },
            { "CBUC35150D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   35.0, "150", "EATON" },
            { "CBUC38050D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "50",  "EATON" },
            { "CBUC38065D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "65",  "EATON" },
            { "CBUC38080D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "80",  "EATON" },
            { "CBUC38100D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "100", "EATON" },
            { "CBUC38120D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "120", "EATON" },
            { "CBUC38140D100",        0.0, "01/01/2000", "CURRENT_LIMITING_FUSE", "CURRENT_LIMITING",   38.0, "140", "EATON" },
            { "4000353C04",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "6",   "EATON" },
            { "4000353C06",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "10",  "EATON" },
            { "4000353C08",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "15",  "EATON" },
            { "4000353C10",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "25",  "EATON" },
            { "4000353C12",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "40",  "EATON" },
            { "4000353C14",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "65",  "EATON" },
            { "4000353C16",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "100", "EATON" },
            { "4000353C17",           0.0, "01/01/2000", "BAYONET_FUSE",           "CURRENT_SENSING",   23.0, "140", "EATON" },
            { "4038108C03B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      23.0, "5",   "EATON" },
            { "4038108C04B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      23.0, "6",   "EATON" },
            { "4038108C05B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      23.0, "8",   "EATON" },
            { "4038108C06B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      23.0, "12",  "EATON" },
            { "4038108C07B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      23.0, "15",  "EATON" },
            { "4038108C09B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      15.5, "25",  "EATON" },
            { "4038108C11B",          0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_ELEMENT",      15.5, "40",  "EATON" },
            { "4000358C03",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "3",   "EATON" },
            { "4000358C05",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "8",   "EATON" },
            { "4000358C06",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "10",  "EATON" },
            { "4000358C08",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "15",  "EATON" },
            { "4000358C10",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "25",  "EATON" },
            { "4000358C12",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "50",  "EATON" },
            { "4000358C14",           0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "65",  "EATON" },
            { "4000358C16CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "100", "EATON" },
            { "4000358C18CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      23.0, "140", "EATON" },
            { "4000380C06CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "10",  "EATON" },
            { "4000380C08CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "15",  "EATON" },
            { "4000380C10CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "25",  "EATON" },
            { "4000380C11CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "30",  "EATON" },
            { "4000380C12CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "40",  "EATON" },
            { "4000380C14CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "DUAL_SENSING",      38.0, "65",  "EATON" },
            { "4038380B03M",          0.0, "01/01/2000", "BAYONET_FUSE_HOLDER",    "FUSE_ACCESSORY",    38.0, "N/A", "EATON" },
            { "4000361C89FV",         0.0, "01/01/2000", "BAYONET_FUSE_HOLDER",    "FUSE_HOLDER",       23.0, "140", "EATON" },
            { "4000361C99FV",         0.0, "01/01/2000", "BAYONET_FUSE_HOLDER",    "FUSE_HOLDER",       23.0, "50",  "EATON" },
            { "4038361C04CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "OVERLOAD",          15.5, "100", "EATON" },
            { "4038361C05CB",         0.0, "01/01/2000", "BAYONET_FUSE",           "OVERLOAD",          15.5, "125", "EATON" }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                           External Diagnostics, Gauges & Instrumentation     
    |____________________________________________________________________________________________________________________________*/




    SapStandardGauges::SapStandardGauges()
    {
        name = "SAP Sandard Gauges";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "Suppliet",
            "Contacts",
            "Dial_in"
        };
        tableData = 
        {
            { "IFD_ORCA_10PSI-A",      0.0000, "01/01/2000", "INTERNAL_FAULT_DETECTOR", "IFD_Tech",  "N/A", ""  },
            { "IFD_ORCA_45KPA-A",      0.0000, "01/01/2000", "INTERNAL_FAULT_DETECTOR", "IFD_Tech",  "N/A", ""  },
            { "19A0036A",             33.7200, "01/01/2000", "LIQUID_LEVEL_GAUGE",      "Qualitrol", "0",   "2" },
            { "19A0071Q",              0.0000, "01/01/2000", "LIQUID_LEVEL_GAUGE",      "Qualitrol", "0",   ""  },
            { "19A0087Q",            125.7200, "01/01/2000", "LIQUID_LEVEL_GAUGE",      "Qualitrol", "0",   "3" },
            { "19A0087",              45.4800, "01/01/2000", "LIQUID_LEVEL_GAUGE",      "Weidmann",  "0",   "3" },
            { "19A0072",             615.8200, "01/01/2000", "PRESSURE_RELIEF_DEVICE",  "Qualitrol", "",    ""  },
            { "19A0072Q",            548.9800, "01/01/2000", "PRESSURE_RELIEF_DEVICE",  "Qualitrol", "",    ""  },
            { "19A0086",             558.1700, "01/01/2000", "PRESSURE_RELIEF_DEVICE",  "Qualitrol", "",    ""  },
            { "19A0076",               2.0393, "01/01/2000", "PRESSURE_RELIEF_VALVE",   "Qualitrol", "",    ""  },
            { "19A0073Q",             43.1200, "01/01/2000", "PRESSURE_VAC_DEVICE",     "Qualitrol", "",    ""  },
            { "19A0074Q",             29.2500, "01/01/2000", "PRESSURE_VAC_DEVICE",     "Qualitrol", "",    ""  },
            { "19A0083",             269.2500, "01/01/2000", "PRESSURE_VAC_DEVICE",     "Qualitrol", "",    ""  },
            { "19-0258",             487.8400, "01/01/2000", "RTD_SENSOR",              "Qualitrol", "",    ""  },
            { "19A0027",              53.2100, "01/01/2000", "THERMOMETER",              "Qualitrol", "0",   "4" },
            { "19A0265",              89.0000, "01/01/2000", "THERMOMETER",              "Qualitrol", "0",   "4" },
            { "19A0094",               0.0000, "01/01/2000", "VALVE",                    "",          "",    ""  }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                            Constructor for SAP Standard Insulation Core Leads
    |____________________________________________________________________________________________________________________________*/




    MechTankAccessoriesStructuralSteel::MechTankAccessoriesStructuralSteel()  //MeTaSsList
    {
        name = "Mechanical Tank Structural Steel List";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "Quantity",
            "Part_No_Rev",
            "Description",
            "Material",
            "Qty*Wt",
            "Wt*Lb",
            "Issue"
        };
        tableData = 
        {
            { "x", 0.0, "01/01/2000", 1, "SPI-1-3 R0",   "Lifting Plate",      "0.135 Mild Steel Sheet, A569",         2.865, 2.865, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-2-2 R0",   "Shackle",            "0.120 Stainless Steel, GR 304",        0.064, 0.064, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-4-2 R0",   "Parking Stand",      "0.075 Mild Steel Sheet, A36",          0.202, 0.202, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-5-2 R1",   "Locking Plate",      "0.048 Mild Steel Sheet, A36/44W",      0.148, 0.148, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-6-2 R0",   "Hinge Plate Long",   "0.135 Mild Steel Sheet, A36",          0.430, 0.215, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-7-7 R0",   "Hinge Plate Short",  "0.135 Mild Steel Sheet, A36",          0.090, 0.045, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-9-2 R0",   "Drip Tray",          "0.075 Mild Steel Sheet, A569",         0.384, 0.384, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-11-2 R0",  "Grounding Spade",    "0.135 Stainless Steel, Gr. 304",       0.888, 0.444, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-12-2 R0",  "Plate",              "0.105 Mild Steel Sheet, A569",         0.304, 0.152, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-14-2 R0",  "Corner Plate",       "0.105 Mild Steel Sheet, A570, Gr 36",  0.063, 0.031, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-27-5 R0",  "Corner Plate",       "0.250 Mild Steel Sheet, A570, Gr 36",  0.215, 0.107, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-32-2 R0",  "Fuse Support",       "0.103 Mild Steel Sheet, A36",          0.166, 0.083, "*S" },
            { "x", 0.0, "01/01/2000", 4, "SPI-34-2 R0",  "Foundation Clamp",   "0.250 Mild Steel Plate, A36",          2.678, 0.669, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-35-2 R0",  "Washer",             "0.109 Mild Steel Sheet, A569",         0.027, 0.027, "*S" },
            { "x", 0.0, "01/01/2000", 2, "SPI-119-2 R0", "Blanking Plate",     "0.135 Mild Steel Sheet, A569",         0.772, 0.386, "*S" },
            { "x", 0.0, "01/01/2000", 1, "SPI-120-2 R0", "Boss Plate",         "0.135 Mild Steel Sheet, A36",          0.318, 0.318, "*S" }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                                    Package Parts and Pallets   
    |____________________________________________________________________________________________________________________________*/




    MechComponents::MechComponents()
    {
        name = "Mechanical Components";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "SUPPLIER"
        };
        tableData = 
        {
            { "PackageParts3", 112.55, "01/01/2000", "Package Part #3",                 "-"                              },
            { "1BGVS",          37.25, "01/01/2000", "Valve Globe",                     "H-J International Inc."         },
            { "HLG1000",        13.08, "01/01/2000", "Liquid Level Gauge Sight Glass",  "H-J International Inc."         },
            { "4660P",          69.69, "01/01/2000", "Pallet",                          "Cratex Industries Regina Ltd."  },
            { "4854P",          47.15, "01/01/2000", "Pallet",                          "Cratex Industries Regina Ltd."  }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                                     Weld Hardware, Flanges, and Fasteners      
    |____________________________________________________________________________________________________________________________*/




    MechPurchasedTankAccessories::MechPurchasedTankAccessories()
    {
        name = "Mechanical Tank Parts List";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "SUPPLIER"
        };
        tableData = 
        {
            { "1WF",           4.03, "01/01/2000", "Weld Flange 1\" NPT", "Apex Distribution Inc."         },
            { "SPI-25-3",      1.34, "01/01/2000", "Weld Boss",            "H-J International Inc."         },
            { "14114SST",      0.68, "01/01/2000", "Weld Stud",            "Western Stud Welding Supply"    },
            { "38112SST",      0.80, "01/01/2000", "Weld Stud",            "Western Stud Welding Supply"    },
            { "FW5G12JS",      0.06, "01/01/2000", "Flat Washer",          "Regina Fasteners & Tools Inc."  },
            { "FW316G38SS",    0.16, "01/01/2000", "Flat Washer",          "Regina Fasteners & Tools Inc."  },
            { "FW5G38JS",      0.08, "01/01/2000", "Flat Washer",          "Regina Fasteners & Tools Inc."  },
            { "HN316G38UNCSS", 0.33, "01/01/2000", "Hex Nut",             "Regina Fasteners & Tools Inc."  }
        };
    }



    /*____________________________________________________________________________________________________________________________
    |
    |                                       Structural Steel & Oil Tank Materials Assemblies      
    |____________________________________________________________________________________________________________________________*/




    MechTankSteelOil::MechTankSteelOil()
    {
        name = "Mechanical Tank STEEL & OIL";
        tableHeaders = 
        {
            "SAP_Name",
            "CAD price",
            "Last_price_date",
            "COMPONENT",
            "SUPPLIER"
        };
        tableData = 
        {
            { "STEEL  & OIL - kVA 167",  1267.0, "01/01/2000", "Mild Steel (204 lbs) & Crosstrans 206 (385 liters), TWt (435 kg)", "Hi-Tec & Lubricon" },
            { "STEEL  & OIL - kVA 100",  1126.0, "01/01/2000", "Mild Steel (187 lbs) & Crosstrans 206 (331 liters), TWt (379 kg)", "Hi-Tec & Lubricon" },
            { "STEEL  & OIL - kVA 75",   1038.0, "01/01/2000", "Mild Steel (176 lbs) & Crosstrans 206 (297 liters), TWt (344 kg)", "Hi-Tec & Lubricon" },
            { "STEEL  & OIL - kVA 50",    968.0, "01/01/2000", "Mild Steel (171 lbs) & Crosstrans 206 (263 liters), TWt (312 kg)", "Hi-Tec & Lubricon" },
            { "STEEL  & OIL - kVA 37.5",  858.0, "01/01/2000", "Mild Steel (159 lbs) & Crosstrans 206 (216 liters), TWt (264 kg)", "Hi-Tec & Lubricon" },
            { "STEEL  & OIL - kVA 25",    852.0, "01/01/2000", "Mild Steel (157 lbs) & Crosstrans 206 (217 liters), TWt (264 kg)", "Hi-Tec & Lubricon" }
        };
    }


}