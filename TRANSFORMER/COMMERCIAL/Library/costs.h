/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Costs.h
|    @brief     Material cost submodule for calculations & manufacturing design evaluations.
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initializes localized state containers for component material tracking.
|                   - Calculates structural and bulk costs for the core, windings, insulation, and busbars.
|                   - Evaluates performance efficiency penalization matrices across no-load and load loss parameters.
|               Returns:
|                   - A localized, high-performance data structure representing total component manufacturing costs.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "enums.h"
#include "alias.h"
#include "field_Value.h"
#include "materials.h"

#include "device.h"
#include "winding.h"

using namespace Transformer::Types;



namespace Transformer::Commercial
{
    class Guarantee;

    class Costs
    {
        private:
            static int id_Counter;

        public:
            std::map <std::string, home_FieldValue> costFields;

            home_Device*         device;
            home_ConductorTable  conductorTable;

            explicit Costs(home_Device* parentDevice = nullptr, ConductorTable conductorTable = {});

            home_Device* parent_device() const;

            void calc_Costs
            (
                home_Device*     device,
                home_Winding*    winding_Hv,
                home_Winding*    winding_Lv
            );

            void calc_CostsDefault
            (
                home_Device*    device,
                home_Core*      core,
                home_Losses*    losses, 
                home_Winding*   winding_Hv, 
                home_Winding*   winding_Lv, 
                home_Coil*      coil_Hv, 
                home_Coil*      coil_Lv
            );

            void calc_CostsDesign
            (
                home_Device*    device, 
                home_Core*      core,
                home_Winding*   winding_Hi, 
                home_Winding*   winding_Lv, 
                int             numberPhases
            );

            void calc_EvaluateDesign 
            (
                home_Losses*    losses, 
                home_Guarantee* guarantee
            );

            double get_CoreLeadValue
            (
                home_Device*        device, 
                const std::string&  key
            );

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/

            std::vector<double> getDLamination() const  { return costFields.at("dLamination")    .get<std::vector<double>>(); }
            double getDCore() const                     { return costFields.at("dCore")          .get<double>(); }
            double getDKraft0022() const                { return costFields.at("dKraft0022")     .get<double>(); }
            double getDKraft005() const                 { return costFields.at("dKraft005")      .get<double>(); }
            double getDKraft010() const                 { return costFields.at("dKraft010")      .get<double>(); }
            double getDKraft015() const                 { return costFields.at("dKraft015")      .get<double>(); }
            double getDCrepe() const                    { return costFields.at("dCrepe")         .get<double>(); }
            double getDPB1_16() const                   { return costFields.at("dPB1_16")        .get<double>(); }
            double getDPB1_8() const                    { return costFields.at("dPB1_8")         .get<double>(); }
            double getDDucts() const                    { return costFields.at("dDucts")         .get<double>(); }
            double getDLV() const                       { return costFields.at("dLV")            .get<double>(); }
            double getDHV() const                       { return costFields.at("dHV")            .get<double>(); }
            double getDCoilLeads() const                { return costFields.at("dCoilLeads")     .get<double>(); }
            double getDBusbar() const                   { return costFields.at("dBusbar")        .get<double>(); }
            double getDPBHD_1_4() const                 { return costFields.at("dPBHD_1_4")      .get<double>(); }

            double getDCostCore() const                 { return costFields.at("dCostCore")      .get<double>(); }
            double getDCostKraft0022() const            { return costFields.at("dCostKraft0022") .get<double>(); }
            double getDCostKraft005() const             { return costFields.at("dCostKraft005")  .get<double>(); }
            double getDCostKraft010() const             { return costFields.at("dCostKraft010")  .get<double>(); }
            double getDCostKraft015() const             { return costFields.at("dCostKraft015")  .get<double>(); }
            double getDCostCrepe() const                { return costFields.at("dCostCrepe")     .get<double>(); }
            double getDCostPB1_16() const               { return costFields.at("dCostPB1_16")    .get<double>(); }
            double getDCostPB1_8() const                { return costFields.at("dCostPB1_8")     .get<double>(); }
            double getDCostDucts() const                { return costFields.at("dCostDucts")     .get<double>(); }
            double getDCostLV() const                   { return costFields.at("dCostLV")        .get<double>(); }
            double getDCostHV() const                   { return costFields.at("dCostHV")        .get<double>(); }
            double getDCostCoilLeads() const            { return costFields.at("dCostCoilLeads") .get<double>(); }
            double getDCostBusBar() const               { return costFields.at("dCostBusBar")    .get<double>(); }
            double getDCostPBHD_1_4() const             { return costFields.at("dCostPBHD_1_4")  .get<double>(); }
            double getDTotalCost() const                { return costFields.at("dTotalCost")     .get<double>(); }
            double getDTotalCostRound() const           { return costFields.at("dTotalCostRound").get<double>(); }
            double getDEvalNL() const                   { return costFields.at("dEvalNL")        .get<double>(); }
            double getDEvalLL() const                   { return costFields.at("dEvalLL")        .get<double>(); }
            double getDCCEval() const                   { return costFields.at("dCCEval")        .get<double>(); }

    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/

            void setDLamination(const std::vector<double>& v) { costFields["dLamination"]     = home_FieldValue(v); }
            void setDCore(double v)                           { costFields["dCore"]           = home_FieldValue(v); }
            void setDKraft0022(double v)                      { costFields["dKraft0022"]      = home_FieldValue(v); }
            void setDKraft005(double v)                       { costFields["dKraft005"]       = home_FieldValue(v); }
            void setDKraft010(double v)                       { costFields["dKraft010"]       = home_FieldValue(v); }
            void setDKraft015(double v)                       { costFields["dKraft015"]       = home_FieldValue(v); }
            void setDCrepe(double v)                          { costFields["dCrepe"]          = home_FieldValue(v); }
            void setDPB1_16(double v)                         { costFields["dPB1_16"]         = home_FieldValue(v); }
            void setDPB1_8(double v)                          { costFields["dPB1_8"]          = home_FieldValue(v); }
            void setDDucts(double v)                          { costFields["dDucts"]          = home_FieldValue(v); }
            void setDLV(double v)                             { costFields["dLV"]             = home_FieldValue(v); }
            void setDHV(double v)                             { costFields["dHV"]             = home_FieldValue(v); }
            void setDCoilLeads(double v)                      { costFields["dCoilLeads"]      = home_FieldValue(v); }
            void setDBusbar(double v)                         { costFields["dBusbar"]         = home_FieldValue(v); }
            void setDPBHD_1_4(double v)                       { costFields["dPBHD_1_4"]       = home_FieldValue(v); }

            void setDCostCore(double v)                       { costFields["dCostCore"]       = home_FieldValue(v); }
            void setDCostKraft0022(double v)                  { costFields["dCostKraft0022"]  = home_FieldValue(v); }
            void setDCostKraft005(double v)                   { costFields["dCostKraft005"]   = home_FieldValue(v); }
            void setDCostKraft010(double v)                   { costFields["dCostKraft010"]   = home_FieldValue(v); }
            void setDCostKraft015(double v)                   { costFields["dCostKraft015"]   = home_FieldValue(v); }
            void setDCostCrepe(double v)                      { costFields["dCostCrepe"]      = home_FieldValue(v); }
            void setDCostPB1_16(double v)                     { costFields["dCostPB1_16"]     = home_FieldValue(v); }
            void setDCostPB1_8(double v)                      { costFields["dCostPB1_8"]      = home_FieldValue(v); }
            void setDCostDucts(double v)                      { costFields["dCostDucts"]      = home_FieldValue(v); }
            void setDCostLV(double v)                         { costFields["dCostLV"]         = home_FieldValue(v); }
            void setDCostHV(double v)                         { costFields["dCostHV"]         = home_FieldValue(v); }
            void setDCostCoilLeads(double v)                  { costFields["dCostCoilLeads"]  = home_FieldValue(v); }
            void setDCostBusBar(double v)                     { costFields["dCostBusBar"]     = home_FieldValue(v); }
            void setDCostPBHD_1_4(double v)                   { costFields["dCostPBHD_1_4"]   = home_FieldValue(v); }
            void setDTotalCost(double v)                      { costFields["dTotalCost"]      = home_FieldValue(v); }
            void setDTotalCostRound(double v)                 { costFields["dTotalCostRound"] = home_FieldValue(v); }
            void setDEvalNL(double v)                         { costFields["dEvalNL"]         = home_FieldValue(v); }
            void setDEvalLL(double v)                         { costFields["dEvalLL"]         = home_FieldValue(v); }
            void setDCCEval(double v)                         { costFields["dCCEval"]         = home_FieldValue(v); }
        };

    }