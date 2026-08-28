/**___________________________________________________________________________________________________
|    @date      June 16, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      conversions.h
|    @brief     Conversion utilities  
|    @brief     Transformer::Core::Math     is for basic rounding of the entire engine
_____________________________________________________________________________________________________*/
#pragma once

#include <vector>

#include "materials.h"
#include "alias.h"

using namespace Transformer::Types;

namespace Transformer::Config
{
    std::vector<double> lookup_RowValue (double lookupValue, const std::vector<std::vector<double>>& table);

    double lookup_Value(double searchKey, const std::vector<std::vector<double>>& tableData);
    double lookup_Value(double searchKey, const std::vector<std::vector<double>>& tableData);
    double lookup_Value(double searchKey, const std::vector<home_ConductorStripRow>& tableData);
    double lookup_Value(double searchKey, const std::vector<Commercial::ConductorMagnetRow>& tableData);
}