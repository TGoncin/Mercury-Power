/**______________________________________________________________
|
|    @date      May 21, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      file_System.cpp
|    @brief     File system utilities  
|_______________________________________________________________*/
#ifdef _WIN32
#endif

#include "file_System.h"

#include "table_Search.h"



namespace Transformer::Config
{
    std::string get_so_folder_path (const std::string &so_number)
    {
        return "C:/Users/Default/Documents/Quotes/" + so_number;
    }
}
// end of file_System.cpp