/**___________________________________________________________________________________________________
|    @date      May 20, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Discs.h
|    @brief     Specialized mathematical distribution submodule for disc winding structures.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|                   - Initialization of localized stack arrays for section turn calculations.
|                   - Solves structural turn distribution mapping across physical winding sections.
|                   - Validates configuration feasibility against mechanical wedge parameters.
|               Returns:
|                   - A localized, high-performance data structure (DiscsResult) containing
|                     the calculated mapping arrays, completely decoupled from global state.
|___________________________________________________________________________________________________*/
#pragma once

#include <array>
#include <cmath>
#include <map>
#include <string>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"


namespace Transformer::Components
{


    struct DiscsResult
    {
        bool success                                = false;
        std::array<double, 6>   turns               = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        std::array<int, 6>      sections_2          = {0, 0, 0, 0, 0, 0};
        std::array<int, 6>      turnsPerSection   = {0, 0, 0, 0, 0, 0};
        std::array<int, 6>      fractionalTurn     = {0, 0, 0, 0, 0, 0};
    };

    class Discs
    {
        private:

            static int id_Counter;
            static inline int rdnDelta      (double value, double delta)            { return static_cast<int>(std::round(value / delta)); }
            static inline bool realEquals   (double a, double b, double tol = 1e-6) { return std::abs(a - b) < tol; }

        public:

            static DiscsResult arrangeDiscTurns(double targetTurns,
                                                int wedgeCount,
                                                int sections_1,
                                                int turnsPerSection_init);
            std::map<std::string, home_FieldValue> discFields;
            Discs();

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/

            bool getTSuccess() const                { return discFields.at("tSuccess")            .get<bool>(); }

            int getId() const                       { return discFields.at("id")                  .get<int>(); }
            int getIWedgeCount() const              { return discFields.at("iWedgeCount")         .get<int>(); }
            int getISections1() const               { return discFields.at("iSections1")          .get<int>(); }
            int getITurnsPerSecInit() const         { return discFields.at("iTurnsPerSecInit")    .get<int>(); }

            double getDTargetTurns() const          { return discFields.at("dTargetTurns")        .get<double>(); }

            std::string getName() const             { return discFields.at("name")                .get<std::string>(); }

    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/

            void setId(int value)                   { discFields["id"]                = home_FieldValue(value); }
            void setIWedgeCount(int value)          { discFields["iWedgeCount"]       = home_FieldValue(value); }
            void setISections1(int value)           { discFields["iSections1"]        = home_FieldValue(value); }
            void setITurnsPerSecInit(int value)     { discFields["iTurnsPerSecInit"]  = home_FieldValue(value); }
            void setDTargetTurns(double value)      { discFields["dTargetTurns"]      = home_FieldValue(value); }
            void setTSuccess(bool value)            { discFields["tSuccess"]          = home_FieldValue(value); }
            void setName(const std::string& value)  { discFields["name"]              = home_FieldValue(value); }
    };
}