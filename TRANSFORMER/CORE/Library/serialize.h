/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Serialize.h
|    @brief     XML Serialization & Deserialization engine for deep transformer hardware structures.
|    @brielf    Transformer::Core       is "The Engine Framework"
|
|               Executes:
|                   - Full recursive extraction of FieldValue attributes into an XML Element Tree.
|                   - Automatic inspection and mapping of child objects (Cooling, Guarantee, Losses, etc.).
|                   - Dynamic element deserialization tracking indexing boundaries across object arrays.
|               Returns:
|                   - High-performance serialization utility methods decoupled from global framework states.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <iostream>
#include <map>

#include "alias.h"

using namespace Transformer::Types;



namespace Transformer::Core
{
    class Device;

    struct XmlElement
    {
        std::string name;
        std::string text;

        std::map    <std::string, std::string>      attributes;
        std::vector <std::shared_ptr<XmlElement>>   children;

        std::shared_ptr <XmlElement>                   find         (const std::string& query)                                     { return nullptr; }
        std::vector     <std::shared_ptr<XmlElement>>  find_all     (const std::string& query)                                     { return {}; }
        std::string                                    get_attribute(const std::string& attr, const std::string& default_val = "") { return default_val; }
    };
// end of XmlElement class
    class Serialize
    {
        public:
            static void serialize_Fields            (void* obj,      std::shared_ptr<XmlElement> parent_element, const std::string& class_type);
            static void load_Fields                 (void* obj,      std::shared_ptr<XmlElement> xml_element,    const std::string& class_type);

            static bool deserialize_device_from_xml (home_Device* device, const std::string& file_path);
    };  
// end of Serialize class
}
//end of serialize.h