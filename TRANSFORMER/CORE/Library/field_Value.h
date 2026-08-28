//    @date      May 27, 2026 ~ EDIT: August 17 2026
//    @author    Tom Goncin
//    @file      fieldValue.h
//    @brief     Polymorphism used to create a flexible data container for variants of field values
//    @brief     Transformer::Core       is "The Engine Framework"
#pragma once

//                                              H E A D E R S
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <cmath>
#include <algorithm>
#include <type_traits> // <--- CRITICAL: Required for the compile-time shield!

#include "alias.h"
#include "enums.h"
using namespace Transformer::Types;

//                                              C L A S S
namespace Transformer::Core
{
    class FieldValue
    {
    public:

        using Value = std::variant< int,
            double,
            std::string,
            bool,
            home_WarningSeverity,
            home_Segment>;

        using List = std::vector<Value>;

        using Data = std::variant<  Value,
            List, std::vector<std::string>,
            std::vector<home_WarningSeverity>,
            std::vector<double>,
            std::vector<int>>;

    private:


//                  C O M P I L E - T I M E   S H I E L D               Safely locked inside the class


        template <typename T, typename Variant>
        struct is_in_variant;

        template <typename T, typename... Types>
        struct is_in_variant<T, std::variant<Types...>> : std::disjunction<std::is_same<T, Types>...> {};


//                 E N D   O F    S H I E L D      



        Data default_Data;          // MUST be after alias!!
        Data current_Data;          // MUST be after alias!!

        bool currentEqualsDefault;
        bool updatingVariable;

        std::vector<std::function<void()>> refreshCallbacks;

        void trigger_Callbacks();

    public:                     // I have "public" explicitly in this order; with 2 publics

        FieldValue();
        explicit FieldValue(Data defaultValue);

        Data    get_Current(int index = -1) const;
        Data    get_Default(int index = -1) const;
        size_t  size()         const;

        void    reset();
        void    set(Data value, int index = -1, int decimals = 6);
        void    set(const FieldValue& otherField);
        void    set_Field(Data value, int index = -1, int decimals = 6);
        void    assignDefault(Data value, int index = -1, int decimals = 6);

        void    assign_Default(Data value, int index = -1, int decimals = 6) { assignDefault(value, index, decimals); }
        void    bind(std::function<void()> callback);
        void    set_FromString(const std::string& str, int index = -1);

        double  get_Double(double value)  const;
        int     get_Int(int value)     const;

        bool    has_Changed()  const;
        bool    is_Default()   const;
        bool    is_Array()     const;

        std::string              get_AsString()       const;
        std::vector<std::string> get_AsStringVector() const;

        FieldValue(const std::vector<double>& vec) : FieldValue(List(vec.begin(), vec.end())) {}
        FieldValue(const std::vector<int>& vec) : FieldValue(List(vec.begin(), vec.end())) {}



        //                                  G E T T E R S



        Data getDefaultData() const { return default_Data; }
        Data getCurrentData() const { return current_Data; }
        bool getCurrentEqualsDefault() const { return currentEqualsDefault; }
        bool getUpdatingVariable() const { return updatingVariable; }
        std::vector<std::function<void()>> getRefreshCallbacks() const { return refreshCallbacks; }



        //                                  S E T T E R S



        void setDefaultData(const Data& v) { default_Data = v; }
        void setCurrentData(const Data& v) { current_Data = v; }
        void setCurrentEqualsDefault(bool v) { currentEqualsDefault = v; }
        void setUpdatingVariable(bool v) { updatingVariable = v; }
        void setRefreshCallbacks(const std::vector<std::function<void()>>& v) { refreshCallbacks = v; }



        //                                  T Y P E     C H E C K   &    C O N V E R S I O N



        template <typename T>
        T get() const
        {

//  1.  Direct scalar value stored in Value 

            if (std::holds_alternative<Value>(current_Data))
            {
                const Value& val = std::get<Value>(current_Data);

                // COMPILE-TIME SHIELD 1: Call the struct we defined in 'private'
                if constexpr (is_in_variant<T, Value>::value)
                {
                    if (std::holds_alternative<T>(val)) { return std::get<T>(val); }
                }

                // Enum requested, but stored as underlying int 
                if constexpr (std::is_enum_v<T>)
                {
                    if (std::holds_alternative<int>(val)) { return static_cast<T>(std::get<int>(val)); }
                }

                // Implicit DOUBLE extraction if stored as int 
                if constexpr (std::is_same_v<T, double>)
                {
                    if (std::holds_alternative<int>(val)) { return static_cast<double>(std::get<int>(val)); }
                }

                // Implicit int extraction if stored as double 
                if constexpr (std::is_same_v<T, int>)
                {
                    if (std::holds_alternative<double>(val)) { return static_cast<int>(std::get<double>(val)); }
                }

                throw std::runtime_error("FieldValue::get<T>() - Type mismatch for stored scalar Value.");
            }


//  2.  Direct match for vector containers stored in Data           (e.g., std::vector<double>)


//              C O M P I L E - T I M E    S H I E L D   2              Call the struct we defined in 'private'



            if constexpr (is_in_variant<T, Data>::value)
            {
                if (std::holds_alternative<T>(current_Data)) { return std::get<T>(current_Data); }
            }

            throw std::runtime_error("FieldValue::get<T>() - Attempted to access incompatible variant storage type.");
        }


//                                  S T O R E    A N Y     E N U M



        template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
        FieldValue(T enum_value) : FieldValue(static_cast<int>(enum_value)) {}



//                                  S E T    V A L U E



        template<typename T>
        void set_Value(T&& givenValue) { current_Data = std::forward<T>(givenValue); }

        void set_Value(Data value, int index = -1, int decimals = 6) { set(value, index, decimals); }
        void set_Value(const FieldValue& other) { current_Data = other.current_Data; }
    };
}
//  end of FieldValue class
// end of Transformer::Core::FieldValue