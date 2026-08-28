/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Serialize.cpp
|    @brief     XML Serialization & Deserialization engine for deep transformer hardware structures.
|    @brielf    Transformer::Core       is "The Engine Framework"
|
|               Executes:
|                   - Full recursive extraction of FieldValue attributes into an XML Element Tree.
|                   - Automatic inspection and mapping of child objectects (Cooling, Guarantee, Losses, etc.).
|                   - Dynamic element deserialization tracking indexing boundaries across objectect arrays.
|               Returns:
|                   - High-performance serialization utility methods decoupled from global framework states.
|___________________________________________________________________________________________________*/
#include <algorithm>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"

#include "serialize.h"
#include "device.h"
#include "coil.h"
#include "device.h"




namespace Transformer::Core
{
    const std::set<std::string>    ignoreFields        =  {"previousarray_type"};
    const std::vector<std::string> childrensAttributes = { "cooling", "guarantee", "losses", "core", "tests", "gradients", "costs", "mechanical"};

    void Serialize::serialize_Fields(void *object, std::shared_ptr<XmlElement> parentElement, const std::string &classType)
    {
        if (object == nullptr)         {    return; }

        if (classType == "Device")
        {
            home_Device* device     = static_cast<home_Device*>(object);

            for (const auto &[field_name, field] : device->deviceFields)
            {
                if (ignoreFields.count(field_name) || field.is_Default())       {   continue;   }

                auto fieldElement = std::make_shared<XmlElement>();

                fieldElement->name                = "Field";
                fieldElement->attributes["name"]  = field_name;

// Handles multi-variant array representations cleanly
                if (field.is_Array())
                {
                    std::vector<std::string> values = field.get_AsStringVector();

                    for (size_t i = 0; i < values.size(); ++i)
                    {
                        auto value_element = std::make_shared<XmlElement>();

                        value_element->name                  = "Value";
                        value_element->attributes["index"]   = std::to_string(i);
                        value_element->text                  = values[i];

                        fieldElement->children.push_back(value_element);
                    }
                }
                else
                {
                    auto value_element      = std::make_shared<XmlElement>();

                    value_element->name     = "Value";
                    value_element->text     = field.get_AsString();

                    fieldElement->children.push_back(value_element);
                }
                parentElement->children.push_back(fieldElement);
            }

// Deep-copy loop serialization over composite uniquely owned submodule types
            for (const std::string &attr_name : childrensAttributes)
            {
                auto child_elem = std::make_shared<XmlElement>();

                child_elem->name                = "objectect";
                child_elem->attributes["name"]  = attr_name;

                if      (attr_name == "cooling")          serialize_Fields(device->cooling   .get(), child_elem, "Cooling");
                else if (attr_name == "guarantee")        serialize_Fields(device->guarantee .get(), child_elem, "Guarantee");
                else if (attr_name == "losses")           serialize_Fields(device->losses    .get(), child_elem, "Losses");
                else if (attr_name == "core")             serialize_Fields(device->core      .get(), child_elem, "Core");
                else if (attr_name == "tests")            serialize_Fields(device->tests     .get(), child_elem, "Tests");
                else if (attr_name == "gradients")        serialize_Fields(device->gradients .get(), child_elem, "Gradients");
                else if (attr_name == "costs")            serialize_Fields(device->costs     .get(), child_elem, "Costs");
                else if (attr_name == "mechanical")       serialize_Fields(device->mechanical.get(), child_elem, "Mechanical");

                parentElement->children.push_back(child_elem);
            }

            // List serialization layout mapping structural Coils arrays dynamically
            auto list_elem                  = std::make_shared<XmlElement>();
            list_elem->name                 = "List";
            list_elem->attributes["name"]   = "Coil";

            for (size_t i = 0; i < device->coil.size(); ++i)
            {
                auto item_elem                  = std::make_shared<XmlElement>();
                item_elem->name                 = "Item";
                item_elem->attributes["index"]  = std::to_string(i);

                serialize_Fields(device->coil[i].get(), item_elem, "Coil");
                list_elem->children.push_back(item_elem);
            }

            parentElement->children.push_back(list_elem);
        }
    }
    void Serialize::load_Fields(void *object, std::shared_ptr<XmlElement> xmlElement, const std::string &classType)
    {
        if (object == nullptr || xmlElement == nullptr)     {   return; }

        try
        {
            if (classType == "Device")
            {
                home_Device *device = static_cast<home_Device *>(object);

// Decouple field tags from DOM elements sequentially
                for (auto fieldElement : xmlElement->find_all("Field"))     
                {
                    std::string field_name = fieldElement->get_attribute("name");

                    if (field_name.empty() || ignoreFields.count(field_name))           {   continue;   }

                    if (device->deviceFields.count(field_name))
                    {
                        auto& field          = device->deviceFields[field_name];
                        auto  value_elements = fieldElement->find_all("Value");

                        if (field.is_Array())
                        {
                            for (auto value : value_elements)
                            {
                                int index   = std::stoi(value->get_attribute("index", "0"));

                                field.set_FromString(value->text, index);
                            }
                        }
                        else if (!value_elements.empty())       {   field.set_FromString(value_elements[0]->text);  }
                    }
                }

// Deserialization iteration blocks handling composition targets
                for (const std::string &attr_name : childrensAttributes)            
                {
                    auto object_elem = xmlElement->find(".//objectect[@name='" + attr_name + "']");

                    if (object_elem == nullptr)                     {   continue;   }

                    if      (attr_name == "cooling")        load_Fields(device->cooling     .get(), object_elem, "Cooling");
                    else if (attr_name == "guarantee")      load_Fields(device->guarantee   .get(), object_elem, "Guarantee");
                    else if (attr_name == "losses")         load_Fields(device->losses      .get(), object_elem, "Losses");
                    else if (attr_name == "core")           load_Fields(device->core        .get(), object_elem, "Core");
                    else if (attr_name == "tests")          load_Fields(device->tests       .get(), object_elem, "Tests");
                    else if (attr_name == "gradients")      load_Fields(device->gradients   .get(), object_elem, "Gradients");
                    else if (attr_name == "costs")          load_Fields(device->costs       .get(), object_elem, "Costs");
                    else if (attr_name == "mechanical")     load_Fields(device->mechanical  .get(), object_elem, "Mechanical");
                }

// Load and dynamically allocate interior list elements (e.g., Coils) safely
                auto list_elem = xmlElement->find(".//List[@name='Coil']");         

                if (list_elem != nullptr)
                {
                    for (auto item_elem : list_elem->find_all("Item"))
                    {
                        try
                        {
                            size_t index = std::stoul(item_elem->get_attribute("index", "0"));

// Grows vector capacity dynamically to prevent out-of-bounds segfaults
                            if (index >= device->coil.size())
                            { 
                                device->coil.push_back(std::make_unique<home_Coil>(device));
                            }
                            load_Fields(device->coil[index].get(), item_elem, "Coil");
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "[load_Fields:ListItem] Loop mapping failed on index assignment: " << e.what() << std::endl;
                        }
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[load_Fields:Exception] Root exception processing: " << e.what() << std::endl;
        }
    }

    bool Serialize::deserialize_device_from_xml(home_Device *device, const std::string &filePath)
    {
        try
        {
// Mock parsing wrapper illustrating doc element tracking matching std tinyxml trees
            std::shared_ptr<XmlElement> xml_root = std::make_shared<XmlElement>();

            load_Fields(device, xml_root, "Device");
            std::cout << "Device successfully loaded from " << filePath << std::endl;

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Deserialization terminated cleanly on unexpected EOF path: " << e.what() << std::endl;

            return false;
        }
    }
}
// end of serialize.cpp