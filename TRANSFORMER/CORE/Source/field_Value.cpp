/*___________________________________________________________________________________________________
|       @date      May 27, 2026 ~ EDIT: August 17 2026
|       @author    Tom Goncin
|       @file      fieldValue.h
|       @brief     Polymorphism used to create a flexible data container for variants of field values
|       @brief     Transformer::Core       is "The Engine Framework"
|___________________________________________________________________________________________________*/


#include "alias.h"
using namespace Transformer::Types;
#include "math_Utilities.h"

#include "field_Value.h"



/*                                    ________________________________________________
|                                    |                                               |
|                                    |                C L A S S                      |
|                                    |_______________________________________________|
*/
namespace Transformer::Core
{
    FieldValue::FieldValue () : currentEqualsDefault (true), updatingVariable (false)
    {
        default_Data = std::string (""); 
        current_Data = std::string ("");
    }
    FieldValue::FieldValue (Data val) : default_Data (val), current_Data (val), currentEqualsDefault (true), updatingVariable (false) {}

    void FieldValue::bind        (std::function<void ()> callback)
    {
        refreshCallbacks.push_back (callback);    
    }
    void FieldValue::set (Data value, int index, int decimals)
    {
        if (index >= 0 && std::holds_alternative<List>(current_Data))
        {
            auto& list = std::get<List>(current_Data);
            Value newValue;

            if (std::holds_alternative<List>(value))                 newValue = std::get<List>(value)[index];
            else                                                     newValue = std::get<Value>(value);

            if (std::holds_alternative<double>(newValue))            list[index] = Config::round_Decimal(std::get<double>(newValue), decimals);
            else                                                     list[index] = newValue;
        } 
        else if (std::holds_alternative<Value>(value))
        {
            Value val = std::get<Value>(value);

            if (std::holds_alternative<double>(val))                current_Data = Value(Config::round_Decimal(std::get<double>(val), decimals));
            else                                                    current_Data = val;
        }
        currentEqualsDefault = false;
        trigger_Callbacks();
    }
    void FieldValue::set (const FieldValue& otherField)
    {
        current_Data            = otherField.current_Data;
        currentEqualsDefault  = (current_Data == default_Data);
        trigger_Callbacks();
    }
    void FieldValue::set_Field (Data value, int index, int decimals)
    {
        set (value, index, decimals);
        if  (current_Data == default_Data)          { currentEqualsDefault = true; }
    }
    void FieldValue::reset ()
    {
        current_Data            = default_Data;
        currentEqualsDefault  = true;
        trigger_Callbacks ();
    }
    void FieldValue::trigger_Callbacks ()                // RUN bound function
    {
        if  (!updatingVariable)
        for  (auto& cb : refreshCallbacks)
        if  (cb) cb ();
    }
    void FieldValue::set_FromString(const std::string& str, int index)
    {
        try 
        {
            Value targetType;
            // what data type we are trying to parse the string into
            if      (   index >= 0 && std::holds_alternative<List>(default_Data)    )      {   targetType = std::get<List>(default_Data)[0];   }
            else if (   std::holds_alternative<Value>(default_Data)    )                   {   targetType = std::get<Value>(default_Data);     }
            else    {   return; }

            // parse string into the correct variant type and set it
            if      (   std::holds_alternative<int>(targetType)     )               {   set(std::stoi(str), index);     }
            else if (   std::holds_alternative<double>(targetType)  )               {   set(std::stod(str), index);     }
            else if (   std::holds_alternative<bool>(targetType)    )   
            {
                bool isTrue = (str == "true" || str == "1" || str == "True" || str == "TRUE");
                set(isTrue, index);
            }
            else if (   std::holds_alternative<std::string>(targetType)    )        {   set(str, index);    }
            else if (std::holds_alternative<home_WarningSeverity>(targetType))      {   set(static_cast<home_WarningSeverity>(std::stoi(str)), index);  }
            else if (std::holds_alternative<home_Segment>(targetType))              {   set(static_cast<home_Segment>(std::stoi(str)), index);  }
        }
        catch (const std::exception& e)
        {
            std::cerr << "FieldValue Error: Failed to parse string '" << str << "' into required type. Reason: " << e.what() << std::endl;
        }
    }
    void FieldValue::assignDefault(Data value, int index, int decimals)
    {
        if (index >= 0 && std::holds_alternative<List>(default_Data))
        {
            auto& list = std::get<List>(default_Data);
            Value newValue = std::holds_alternative<List>(value) ? std::get<List>(value)[index] : std::get<Value>(value);

            if (std::holds_alternative<double>(newValue))
                list[index] = Config::round_Decimal(std::get<double>(newValue), decimals);
            else
                list[index] = newValue;
        }
        else if (std::holds_alternative<Value>(value))
        {
            Value val = std::get<Value>(value);
            if (std::holds_alternative<double>(val))
                default_Data = Value(Config::round_Decimal(std::get<double>(val), decimals));
            else
                default_Data = val;
        }
        else
        {
            default_Data = value;
        }
        currentEqualsDefault = (current_Data == default_Data);
    }

    bool    FieldValue::has_Changed () const                            {    return current_Data != default_Data;  }
    bool    FieldValue::is_Default  () const                            {    return currentEqualsDefault;  }
    bool    FieldValue::is_Array() const
    {
        return      std::holds_alternative<List>(current_Data) 
                ||  std::holds_alternative<std::vector<std::string>>(current_Data) 
                ||  std::holds_alternative<std::vector<home_WarningSeverity>>(current_Data);
    }

    double  FieldValue::get_Double  (double value ) const               {   return value;   }

    int     FieldValue::get_Int     (int value)     const               {   return value;   }

    FieldValue::Data FieldValue::get_Current(int index) const
    {
        if (index >= 0)
        {
            if (std::holds_alternative<List>(current_Data))
            {
                const auto& list = std::get<List>(current_Data);

                if (static_cast<size_t>(index) < list.size()) 
                {
                    return list[index]; 
                }
            }
            else if (std::holds_alternative<std::vector<std::string>>(current_Data))
            {
                const auto& list = std::get<std::vector<std::string>>(current_Data);
                if (static_cast<size_t>(index) < list.size())
                {
                    return Value(list[index]); 
                }
            }
            else if (std::holds_alternative<std::vector<home_WarningSeverity>>(current_Data))
            {
                const auto& list = std::get<std::vector<home_WarningSeverity>>(current_Data);

                if (static_cast<size_t>(index) < list.size()) 
                {
                    return Value(list[index]); 
                }
            }
            throw std::out_of_range("FieldValue::get_Current - Index out of bounds or data is not an array.");
        }
        

        return current_Data;        // If index is -1, return the entire variant
    }
    FieldValue::Data FieldValue::get_Default(int index) const
    {
        if (index >= 0)
        {
            if (std::holds_alternative<List>(default_Data))
            {
                const auto& list = std::get<List>(default_Data);

                if (static_cast<size_t>(index) < list.size()) 
                {
                    return list[index];
                }
            }
            else if (std::holds_alternative<std::vector<std::string>>(default_Data))
            {
                const auto& list = std::get<std::vector<std::string>>(default_Data);

                if (static_cast<size_t>(index) < list.size()) 
                {
                    return Value(list[index]);
                }
            }
            else if (std::holds_alternative<std::vector<home_WarningSeverity>>(default_Data))
            {
                const auto& list = std::get<std::vector<home_WarningSeverity>>(default_Data);
                
                if (static_cast<size_t>(index) < list.size()) 
                {
                    return Value(list[index]);
                } 
           }
            throw std::out_of_range("FieldValue::get_Default - Index out of bounds or data is not an array.");
        }
        return default_Data;        // If index is -1, return the entire variant
    }

    size_t FieldValue::size() const
    {
        if      (std::holds_alternative<List>                             (current_Data))   {   return std::get<List>                             (current_Data).size();    }
        else if (std::holds_alternative<std::vector<std::string>>         (current_Data))   {   return std::get<std::vector<std::string>>         (current_Data).size();    }
        else if (std::holds_alternative<std::vector<home_WarningSeverity>>(current_Data))   {   return std::get<std::vector<home_WarningSeverity>>(current_Data).size();    }

        return 1;           // Scalar values have a size of 1
    }

    std::string FieldValue::get_AsString() const
    {
        if (std::holds_alternative<Value>(current_Data))
        {
            const Value& value = std::get<Value>(current_Data);
            
            if (std::holds_alternative<int>         (value))      return std::to_string (std::get<int> (value));
            if (std::holds_alternative<double>      (value))      return std::to_string (std::get<double> (value));
            if (std::holds_alternative<std::string> (value))      return std::get<std::string> (value);
            if (std::holds_alternative<bool>        (value))      return std::get<bool> (value) ? "true" : "false";
        // enum casts
            if (std::holds_alternative<home_WarningSeverity>(value)) return std::to_string(static_cast<int>(std::get<home_WarningSeverity>(value)));
            if (std::holds_alternative<home_Segment>(value))         return std::to_string(static_cast<int>(std::get<home_Segment>(value)));
        }
        return "";
    }

    std::vector<std::string> FieldValue::get_AsStringVector() const
    {
        std::vector<std::string> result;

        if (std::holds_alternative<List>(current_Data))
        {
            const auto& list = std::get<List>(current_Data);
            for (const auto& value : list)
            {
                if (std::holds_alternative<int>(value))                    result.push_back(std::to_string(std::get<int>(value)));
                else if (std::holds_alternative<double>(value))                 result.push_back(std::to_string(std::get<double>(value)));
                else if (std::holds_alternative<std::string>(value))            result.push_back(std::get<std::string>(value));
                else if (std::holds_alternative<bool>(value))                   result.push_back(std::get<bool>(value) ? "true" : "false");
                else if (std::holds_alternative<home_WarningSeverity>(value))   result.push_back(std::to_string(static_cast<int>(std::get<home_WarningSeverity>(value))));
                else if (std::holds_alternative<home_Segment>(value))           result.push_back(std::to_string(static_cast<int>(std::get<home_Segment>(value))));
            }
        }
        else if (std::holds_alternative<std::vector<std::string>>(current_Data))
        {
            // REPLACED: Now it assigns to result instead of returning early
            result = std::get<std::vector<std::string>>(current_Data);
        }
        else if (std::holds_alternative<std::vector<home_WarningSeverity>>(current_Data))
        {
            const auto& list = std::get<std::vector<home_WarningSeverity>>(current_Data);
            for (const auto& value : list)
            {
                result.push_back(std::to_string(static_cast<int>(value)));
            }
        }
        else if (std::holds_alternative<Value>(current_Data))
        {
            result.push_back(get_AsString());
        }

        return result;
    }
}
//end of FieldValue class