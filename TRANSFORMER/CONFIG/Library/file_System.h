/**______________________________________________________________
|
|    @date      May 21, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      file_System.h
|    @brief     File system utilities  
|_______________________________________________________________*/
#pragma once

#include <string>


namespace Transformer::Config
{
    const std::string ADDRESSZ  =  R"(\\PTISK-fileserver.pti.local\DATA\ENGINEERING\13-Enginerring projects\MERCURY\OPTIMIZATION THREE PHASE\Designs)";

    std::string get_so_folder_path          (const std::string& so_number);

    bool        is_folder_open_windows      (const std::string& folder_path);
}
// end of file_System.h