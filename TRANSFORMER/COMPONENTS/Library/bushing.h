/*
|    @date      May 27, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      bushing.h
|    @brief     Object creation w/ unique IDs & names (Auto ID incremented)
|___________________________________________________________________________________________________*/
#pragma once

#include <map>
#include <string>

#include "alias.h"
#include "field_Value.h"

using namespace Transformer::Types;



namespace Transformer::Components
{
    class Bushing
    {
        private:
            static int id_Counter;
            
        public:
            std::map<std::string, home_FieldValue> bushingFields;
            Bushing();
            void reset_BushingFields();

//  G E T T E R S

            int         getId   () const { return bushingFields.at("id")    .get<int>(); }
            std::string getName () const { return bushingFields.at("name")  .get<std::string>(); }

//  S E T T E R S

            void    setId   (int value)                 { bushingFields["id"]   = home_FieldValue(value); }
            void    setName (const std::string& value)  { bushingFields["name"] = home_FieldValue(value); }
    };  
}
