/*
|    @date      May 27, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      bushing.cpp
|    @brief     Object creation w/ unique IDs & names (Auto ID incremented)
|___________________________________________________________________________________________________*/
#include <map>
#include <string>

#include "core.h"
#include "bushing.h"




namespace Transformer::Components
{
    int Bushing::id_Counter = 1;

    Bushing::Bushing()
    {
        int id                 = id_Counter++;
        bushingFields["id"]    = home_FieldValue (id);
        bushingFields["name"]  = home_FieldValue ("Bushing " + std::to_string(id));
    }

    void Bushing::reset_BushingFields()
    {
        for (auto& bushingField : bushingFields)
        {
            try {
                bushingField.second.reset();
            }
            catch (...) {} // Silently catch uninitialized variant errors
        }
    }
}  