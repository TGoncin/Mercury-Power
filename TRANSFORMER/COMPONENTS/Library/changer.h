/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Changer.h
|    @brief     Manageer of Tap changer voltage configurations, step evaluations, & structural lead calculations.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|                   - Initialization and tracking of tap changer fields utilizing localized FieldValues.
|                   - Calculations for tap capacities, percentage steps, and nominal step ratios.
|                   - Resolves geometric electrical lead placements out of the top of the physical transformer coil.
|               Returns:
|                   - A localized, high-performance representation of the tap changer system.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "alias.h"
using namespace Transformer::Types;
#include "field_Value.h"
#include "enums.h"



namespace Transformer::Components
{
    class Winding;
    class Changer
    {
        private:
            static int id_Counter;
            Winding* winding = nullptr;

        public:
            std::map <std::string, home_FieldValue> changerFields;

            explicit Changer (home_Winding* getParentWinding = nullptr);
/*      _______________________________________________
|      |                                               |
|      |                F U N C T I O N S              |
|      |_______________________________________________|*/
            home_Winding* getParentWinding() const;

            int    get_NumberSteps() const;

            double get_PerStepUp() const;
            double get_PerStepDown() const;
            double taps_Sup() const;
            double taps_Inf() const;

            std::string get_Type() const;

            void reset_ChangerFields();
            void erase_Warnings();
            void calc_Warnings();
            void calc_Changer();
            void calc_DefaultCapacity();
            void calc_LeadsOutTopCoil();
            void calc_Instructions();
            void calc_PerTaps();
/*      _______________________________________________
|      |                                               |
|      |                G E T T E R S                  |
|      |_______________________________________________|*/
            int getId() const                       { return changerFields.at("id")           .get<int>(); }
            int getINumSteps() const                { return changerFields.at("iNumSteps")     .get<int>(); }
            int getTCapacity() const                { return changerFields.at("tCapacity")     .get<int>(); }
            int getTModelChanger() const            { return changerFields.at("tModelChanger") .get<int>(); }
            int getTType() const                    { return changerFields.at("tType")         .get<int>(); }
            int getTTypeLR() const                  { return changerFields.at("tTypeLR")       .get<int>(); }
            int getTTypeCBC() const                 { return changerFields.at("tTypeCBC")      .get<int>(); }

            bool getBExtraCircuit() const           { return changerFields.at("bExtraCircuit") .get<bool>(); }
            bool getIRedCap() const                 { return changerFields.at("iRedCap")       .get<bool>(); }

            double getDPerUp() const                { return changerFields.at("dPerUp")        .get<double>(); }
            double getDPerDown() const              { return changerFields.at("dPerDown")      .get<double>(); }
            double getDPerTaps() const              { return changerFields.at("dPerTaps")      .get<double>(); }
            double getDSeriesRatio() const          { return changerFields.at("dSeriesRatio")  .get<double>(); }

            std::string getName() const             { return changerFields.at("name")          .get<std::string>(); }
            std::string getSCapactity() const       { return changerFields.at("sCapactity")    .get<std::string>(); }
            std::string getSPerTaps() const         { return changerFields.at("sPerTaps")      .get<std::string>(); }
            std::string getSInstructions() const    { return changerFields.at("sInstructions") .get<std::string>(); }

            std::string getSType() const            { return changerFields.at("sType")         .get<std::string>(); }

            std::vector<int> getTErrorEnum() const                      { return changerFields.at("tErrorEnum").get<std::vector<int>>(); }
            std::vector<std::string> getSLeadsOutTOPCoilHeater() const  { return changerFields.at("sLeadsOutTOPCoilHeater").get<std::vector<std::string>>(); }
            std::vector<std::string> getSLeadsOutTOPCoil() const        { return changerFields.at("sLeadsOutTOPCoil")      .get<std::vector<std::string>>(); }
            std::vector<std::string> getSErrorName() const              { return changerFields.at("sErrorName").get<std::vector<std::string>>(); }
            std::vector<std::string> getSErrorMessages() const          { return changerFields.at("sErrorMessages").get<std::vector<std::string>>(); }
/*      _______________________________________________
|      |                                               |
|      |                S E T T E R S                  |
|      |_______________________________________________|  */

            void setId(int value)                   { changerFields["id"]            = home_FieldValue(value); }
            void setINumSteps(int value)            { changerFields["iNumSteps"]      = home_FieldValue(value); }
            void setTCapacity(int value)            { changerFields["tCapacity"]      = home_FieldValue(value); }
            void setTModelChanger(int value)        { changerFields["tModelChanger"]  = home_FieldValue(value); }
            void setTType(int value)                { changerFields["tType"]          = home_FieldValue(value); }
            void setTTypeLR(int value)              { changerFields["tTypeLR"]        = home_FieldValue(value); }
            void setTTypeCBC(int value)             { changerFields["tTypeCBC"]       = home_FieldValue(value); }

            void setBExtraCircuit(bool value)       { changerFields["bExtraCircuit"]  = home_FieldValue(value); }
            void setIRedCap(bool value)             { changerFields["iRedCap"]        = home_FieldValue(value); }

            void setDPerUp(double value)            { changerFields["dPerUp"]         = home_FieldValue(value); }
            void setDPerDown(double value)          { changerFields["dPerDown"]       = home_FieldValue(value); }
            void setDPerTaps(double value)          { changerFields["dPerTaps"]       = home_FieldValue(value); }
            void setDSeriesRatio(double value)      { changerFields["dSeriesRatio"]   = home_FieldValue(value); }

            void setName(const std::string& value)          { changerFields["name"]          = home_FieldValue(value); }
            void setSCapactity(const std::string& value)    { changerFields["sCapactity"]    = home_FieldValue(value); }
            void setSPerTaps(const std::string& value)      { changerFields["sPerTaps"]      = home_FieldValue(value); }
            void setSInstructions(const std::string& value) { changerFields["sInstructions"] = home_FieldValue(value); }
            void setSType(const std::string& value)         { changerFields["sType"]         = home_FieldValue(value); }

            void setSLeadsOutTOPCoilHeater(const std::vector<std::string>& value) { changerFields["sLeadsOutTOPCoilHeater"] = home_FieldValue(value); }
            void setSLeadsOutTOPCoil(const std::vector<std::string>& value)       { changerFields["sLeadsOutTOPCoil"]       = home_FieldValue(value); }

            void setSErrorName(const std::vector<std::string>& value)       { changerFields["sErrorName"]       = home_FieldValue(value); }
            void setTErrorEnum(const std::vector<int>& value)               { changerFields["tErrorEnum"]       = home_FieldValue(value); }
            void setSErrorMessages(const std::vector<std::string>& value)   { changerFields["sErrorMessages"]   = home_FieldValue(value); }
    };  
// end of Changer class
}      
 //  end of Transformer::Components