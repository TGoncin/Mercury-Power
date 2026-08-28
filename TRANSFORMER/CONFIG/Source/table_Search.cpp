/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      table_Loopups.cpp
|    @brief     Mainly for sap_Tables with pricing, material constants or conductor parameters  
|    @brief     Transformer::Data     is for finding
_____________________________________________________________________________________________________*/

#include <vector>

#include "alias.h"
using namespace Transformer::Types;
#include "table_Search.h"




namespace Transformer::Config
{

    std::vector<double> lookup_RowValue (double lookupValue, const std::vector<std::vector<double>>& table)
        {
            for (const auto& row : table)
            {
                if (!row.empty() && row[0] == lookupValue)      return row; 
            }
            return std::vector<double>();
        }



    double lookup_Value(double searchKey, const std::vector<std::vector<double>>& tableData)
    {
        for (const auto& row : tableData)
        {
            if (row.size() >= 2 && row[0] == searchKey)     {   return row[1];  }   // row[0] is Layers column, row[1] is the 'Impulse %' column
        }
        return 0.0;
    }


//                                                      overload for             ConductorStripRow vectors
    double lookup_Value(double searchKey, const std::vector<Commercial::ConductorStripRow>& tableData)
    {
        for (const auto& row : tableData)
        {
            if (row.no_Conductor == static_cast<int>(searchKey))
            {
                if (row.width.has_value())              return row.width.value();
                else if (row.thickness.has_value())     return row.thickness.value();
                else                                    return row.thick_Wire;            
            }
        }
        return 0.0; 
    }


// Overload for ConductorMagnetRow vectors (used by HV windings)

    double lookup_Value(double searchKey, const std::vector<Commercial::ConductorMagnetRow>& tableData)
    {
        for (const auto& row : tableData)
        {
            if (row.no_conductor == static_cast<int>(searchKey))
            {
                // Return the target property for magnet wire (e.g., width_wire, thick_wire, etc.)
                return row.width_wire; 
            }
        }
        return 0.0;
    }
}
// end of TableSearch
