/*
|       @date      June 16, 2026 ~ EDIT: August 17 2026
|       @author    Tom Goncin
|       @file      systemConfig.h
|       @brief     Global constant Variables,           Transformer::Config     for 'System & Setup'
*/
#pragma once


#include <string>
#include <string_view>



namespace Transformer::Config
{
  inline const std::string_view COST_TABLE_PATH = "...";
  inline const int ERRORMESSAGES = 10;

  constexpr std::string_view OPTIMIZATION_EXTENSION           = ".dbs";
  constexpr std::string_view SERIES_OPTIMIZATION_EXTENSION    = "-S.dbs";
  constexpr std::string_view FAILED_OPTIMIZATION_EXTENSION    = ".dbf";


}
// end of Transformer::Config