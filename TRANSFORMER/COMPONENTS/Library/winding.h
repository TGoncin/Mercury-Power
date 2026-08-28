/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Winding.h
|    @brief     Implementation manager for transformer winding parameter calculation loops.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|                   - INITIALIZATION:   winding config states (V, I, BIL, Taps)
|                   - EVALUATE:         line, phase, tap voltages to standards
|                   - COMPUTES:         current loads, automated cable/lead layout dimensions
|                   - RETURNS:          high performance data structure
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "alias.h"
#include "field_Value.h"
#include "enums.h"
#include "winding_Warnings.h"

#include "bushing.h"
#include "changer.h"


using namespace Transformer::Types;


namespace Transformer::Components
{
class Winding
{
    private:
        static int      id_Counter;
    public:
        std::map <std::string, home_FieldValue> windingFields;

        home_Device*            device;
        home_Device*            getParentDevice() const;

        Changer                 changer;

        std::vector <Bushing>   bushings;
        std::vector <Coil*>     coils;

        home_WindingWarnings    winding_Warnings;

        Winding();
        ~Winding();

        bool have_Taps() const;

        int  get_NominalPosition() const;
        int  get_NumberSteps() const;
        int  get_WindingType() const;

        void add_Coils                (home_Coil* coil);
        void reset_WindingFields();
        void erase_Warnings();
        void use_Ducts();
        void calc_Warnings();
        void calc_Winding();
        void calc_DefaultKVA         (home_Device* device);
        void calc_DefaultLineKV();
        void calc_DefaultPhaseKV     (home_Device* device, Winding* winding_Hv, Winding* winding_Lv, Winding* winding_Tertiary);
        void calc_Amperage();
        void calc_Leads();

/*______________________________________________
|                                               |
|                G E T T E R S                  |
|_______________________________________________|*/

        int getId() const                                    { return windingFields.at("id")                  .get<int>(); }
        std::string getName() const                          { return windingFields.at("name")                .get<std::string>(); }
        home_WindingDesc getDescription() const              { return windingFields.at("tDescription")        .get<home_WindingDesc>(); }
        double getVoltage() const                            { return windingFields.at("dVoltage")            .get<double>(); }
        home_ConnectionType getTypeConnection() const        { return windingFields.at("type_connection")     .get<home_ConnectionType>(); }
        int getBIL() const                                   { return windingFields.at("iBIL")                .get<int>(); }
        int getBILUnder() const                              { return windingFields.at("iBILUnder")           .get<int>(); }
        bool getHalfTurns() const                            { return windingFields.at("bHalfTurns")          .get<bool>(); }
        home_Coating getCoating() const                      { return windingFields.at("tCoating")            .get<home_Coating>(); }
        double getNomTurnsUI() const                         { return windingFields.at("dNomTurnsUI")         .get<double>(); }
        std::string getTypeLeads() const                     { return windingFields.at("tTypeLeads")          .get<std::string>(); }
        int getQuantityLeads() const                         { return windingFields.at("quantityLeads")       .get<int>(); }
        double getThicknessLeads() const                     { return windingFields.at("thicknessLeads")      .get<double>(); }
        double getWidthLeads() const                         { return windingFields.at("widthLeads")          .get<double>(); }
        double getLeadsDensity() const                       { return windingFields.at("dLeadsDensity")       .get<double>(); }
        std::string getSLeadsDensity() const                 { return windingFields.at("sLeadsDensity")       .get<std::string>(); }
        int getNoDuctInner() const                           { return windingFields.at("iNoDuctInner")        .get<int>(); }
        int getNoDuctOuter() const                           { return windingFields.at("iNoDuctOuter")        .get<int>(); }
        double getDuctSize() const                           { return windingFields.at("dDuctSize")           .get<double>(); }
        std::string getDuctPlacement() const                 { return windingFields.at("sDuctPlacement")      .get<std::string>(); }
        double getOptStayback() const                        { return windingFields.at("dOptStayback")        .get<double>(); }
        double getLayerInsulation() const                    { return windingFields.at("dLayerInsulation")    .get<double>(); }
        double getEndInsulation() const                      { return windingFields.at("dEndInsulation")      .get<double>(); }
        double getBuildFactorUsed() const                    { return windingFields.at("dBuildFactorUsed")    .get<double>(); }
        double getHLOverNoseReduction() const                { return windingFields.at("dHLOverNoseReduction").get<double>(); }

        std::vector<double> getKVA() const                   { return windingFields.at("d_kVA")               .get<std::vector<double>>(); }
        std::vector<double> getLineKVoltage() const          { return windingFields.at("d_line_kVoltage")     .get<std::vector<double>>(); }
        std::vector<double> getKVPhaseDef() const            { return windingFields.at("d_kVPhaseDef")        .get<std::vector<double>>(); }
        std::vector<double> getTurns() const                 { return windingFields.at("d_Turns")             .get<std::vector<double>>(); }
        std::vector<double> getPdPhaseAmp() const            { return windingFields.at("pdPhaseAmp")          .get<std::vector<double>>(); }
        std::vector<double> getPdPhaseAmp1() const           { return windingFields.at("pdPhaseAmp1")         .get<std::vector<double>>(); }
        std::vector<double> getPdLineAmp() const             { return windingFields.at("pdLineAmp")           .get<std::vector<double>>(); }
        std::vector<double> getPdLineAmp1() const            { return windingFields.at("pdLineAmp1")          .get<std::vector<double>>(); }

        std::vector<std::string> getSErrorName() const       { return windingFields.at("sErrorName")          .get<std::vector<std::string>>(); }
        std::vector<int> getTErrorEnum() const               { return windingFields.at("tErrorEnum")          .get<std::vector<int>>(); }
        std::vector<std::string> getSErrorMessages() const   { return windingFields.at("sErrorMessages")      .get<std::vector<std::string>>(); }
/*______________________________________________
|                                               |
|                G E T T E R S                  |
|_______________________________________________*/

        void setId(int value)                                { windingFields["id"]                  = FieldValue(value); }
        void setName(const std::string& value)               { windingFields["name"]                = FieldValue(value); }
        void setDescription(home_WindingDesc value)          { windingFields["tDescription"]        = FieldValue(value); }
        void setVoltage(double value)                        { windingFields["dVoltage"]            = FieldValue(value); }
        void setTypeConnection(home_ConnectionType value)    { windingFields["type_connection"]     = FieldValue(value); }
        void setBIL(int value)                               { windingFields["iBIL"]                = FieldValue(value); }
        void setBILUnder(int value)                          { windingFields["iBILUnder"]           = FieldValue(value); }
        void setHalfTurns(bool value)                        { windingFields["bHalfTurns"]          = FieldValue(value); }
        void setCoating(home_Coating value)                  { windingFields["tCoating"]            = FieldValue(value); }
        void setNomTurnsUI(double value)                     { windingFields["dNomTurnsUI"]         = FieldValue(value); }
        void setTypeLeads(const std::string& value)          { windingFields["tTypeLeads"]          = FieldValue(value); }
        void setQuantityLeads(int value)                     { windingFields["quantityLeads"]       = FieldValue(value); }
        void setThicknessLeads(double value)                 { windingFields["thicknessLeads"]      = FieldValue(value); }
        void setWidthLeads(double value)                     { windingFields["widthLeads"]          = FieldValue(value); }
        void setLeadsDensity(double value)                   { windingFields["dLeadsDensity"]       = FieldValue(value); }
        void setSLeadsDensity(const std::string& value)      { windingFields["sLeadsDensity"]       = FieldValue(value); }
        void setNoDuctInner(int value)                       { windingFields["iNoDuctInner"]        = FieldValue(value); }
        void setNoDuctOuter(int value)                       { windingFields["iNoDuctOuter"]        = FieldValue(value); }
        void setDuctSize(double value)                       { windingFields["dDuctSize"]           = FieldValue(value); }
        void setDuctPlacement(const std::string& value)      { windingFields["sDuctPlacement"]      = FieldValue(value); }
        void setOptStayback(double value)                    { windingFields["dOptStayback"]        = FieldValue(value); }
        void setLayerInsulation(double value)                { windingFields["dLayerInsulation"]    = FieldValue(value); }
        void setEndInsulation(double value)                  { windingFields["dEndInsulation"]      = FieldValue(value); }
        void setBuildFactorUsed(double value)                { windingFields["dBuildFactorUsed"]    = FieldValue(value); }
        void setHLOverNoseReduction(double value)            { windingFields["dHLOverNoseReduction"]= FieldValue(value); }

        void setKVA(const std::vector<double>& value)        { windingFields["d_kVA"]            = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setLineKVoltage(const std::vector<double>& value){ windingFields["d_line_kVoltage"] = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setKVPhaseDef(const std::vector<double>& value) { windingFields["d_kVPhaseDef"]     = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setTurns(const std::vector<double>& value)      { windingFields["d_Turns"]          = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setPdPhaseAmp(const std::vector<double>& value) { windingFields["pdPhaseAmp"]       = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setPdPhaseAmp1(const std::vector<double>& value){ windingFields["pdPhaseAmp1"]      = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setPdLineAmp(const std::vector<double>& value)  { windingFields["pdLineAmp"]        = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setPdLineAmp1(const std::vector<double>& value) { windingFields["pdLineAmp1"]       = FieldValue(FieldValue::List(value.begin(), value.end())); }



        void setSErrorName(const std::vector<std::string>& value) { windingFields["sErrorName"]         = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setTErrorEnum(const std::vector<int>& value)         { windingFields["tErrorEnum"]         = FieldValue(FieldValue::List(value.begin(), value.end())); }
        void setSErrorMessages(const std::vector<std::string>& value) { windingFields["sErrorMessages"] = FieldValue(FieldValue::List(value.begin(), value.end())); }
    };
// end of Winding class

}
//end of Transformer::Components::Winding