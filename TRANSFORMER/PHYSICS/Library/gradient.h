/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Gradients.h
|    @brief     Specialized mathematical tracking engine evaluating transformer thermal gradients.
|    @brief     Transformer::Physics    is for "Behavior & Calculations"
|
|               Executes:
|                   - Instantiates and processes thermal fluid dynamics parameters using FieldValues.
|                   - Evaluates localized winding gradients, hot-spot factors, and altitudinal corrections.
|                   - Solves Top-Oil Rise (TOR) constraints against forced and natural cooling limits.
|               Returns:
|                   - High-performance structures tracking electrical transformer thermal physics.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "alias.h"
using namespace Transformer::Types;
#include "definitions.h"
#include "field_Value.h"
#include "enums.h"
#include "materials.h"

#include "gradient_Warnings.h"



namespace Transformer::Physics
{
    class Gradients
    {
        private:
            static int id_Counter;
            home_GradientWarnings gradientWarnings;

        public:
            std::map<std::string, home_FieldValue>  gradientFields;
            home_Device*                            device;
            home_MaterialConstants                  table_MaterialConstants;

            explicit Gradients                  (home_Device* parentDevice = nullptr, home_MaterialConstants materialConstants = {});

            home_Device* getParentDevice() const;
            void reset_Fields();
            void erase_Warnings();
            void calc_Warnings();

            void calc_Gradients        (home_Cooling* cooling);
            void calc_GradientsDef();
            void calc_ThermalRise      (home_Cooling* cooling);

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/

            int getId() const                                               { return gradientFields.at("id").get<int>(); }
            std::string getName() const                                     { return gradientFields.at("name").get<std::string>(); }
            
            std::vector<double> getDn() const                               { return gradientFields.at("dn").get<std::vector<double>>(); }
            std::vector<double> getDW() const                               { return gradientFields.at("dW").get<std::vector<double>>(); }
            std::vector<double> getDN() const                               { return gradientFields.at("dN").get<std::vector<double>>(); }
            std::vector<double> getDL() const                               { return gradientFields.at("dL").get<std::vector<double>>(); }
            std::vector<double> getDH() const                               { return gradientFields.at("dH").get<std::vector<double>>(); }
            std::vector<double> getDb() const                               { return gradientFields.at("db").get<std::vector<double>>(); }
            std::vector<double> getDq1() const                              { return gradientFields.at("dq1").get<std::vector<double>>(); }
            std::vector<double> getDG1() const                              { return gradientFields.at("dG1").get<std::vector<double>>(); }
            std::vector<double> getDq2() const                              { return gradientFields.at("dq2").get<std::vector<double>>(); }
            std::vector<double> getDG2() const                              { return gradientFields.at("dG2").get<std::vector<double>>(); }
            std::vector<double> getDG3() const                              { return gradientFields.at("dG3").get<std::vector<double>>(); }
            std::vector<double> getDLVgradient() const                      { return gradientFields.at("dLVgradient").get<std::vector<double>>(); }
            
            double getDAltCorrLV() const                                    { return gradientFields.at("dAltCorrLV").get<double>(); }
            double getDAltCorrHV() const                                    { return gradientFields.at("dAltCorrHV").get<double>(); }
            
            std::vector<double> getDGradientF() const                       { return gradientFields.at("dGradientF").get<std::vector<double>>(); }
            std::vector<double> getDGradientFinal() const                   { return gradientFields.at("dGradientFinal").get<std::vector<double>>(); }
            
            double getDFCBNLVgradient() const                               { return gradientFields.at("dFCBNLVgradient").get<double>(); }
            double getDTOR() const                                          { return gradientFields.at("dTOR").get<double>(); }
            
            std::vector<double> getDTORFinal() const                        { return gradientFields.at("dTORFinal").get<std::vector<double>>(); }
            
            double getDMaxGrad() const                                      { return gradientFields.at("dMaxGrad").get<double>(); }
            double getDMaxGradF() const                                     { return gradientFields.at("dMaxGradF").get<double>(); }
            double getDFCBNHVGrad() const                                   { return gradientFields.at("dFCBNHVGrad").get<double>(); }
            
            std::vector<std::string> getSErrorName() const                  { return gradientFields.at("sErrorName").get<std::vector<std::string>>(); }
            std::vector<int> getTErrorEnum() const                          { return gradientFields.at("tErrorEnum").get<std::vector<int>>(); }
            std::vector<std::string> getSErrorMessages() const              { return gradientFields.at("sErrorMessages").get<std::vector<std::string>>(); }
    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/

            void setId(int v)                                               { gradientFields["id"]      = home_FieldValue(v); }
            void setName(const std::string& v)                              { gradientFields["name"]    = home_FieldValue(v); }
            
            void setDn(const std::vector<double>& v)                        { gradientFields["dn"]          = home_FieldValue(v); }
            void setDW(const std::vector<double>& v)                        { gradientFields["dW"]          = home_FieldValue(v); }
            void setDN(const std::vector<double>& v)                        { gradientFields["dN"]          = home_FieldValue(v); }
            void setDL(const std::vector<double>& v)                        { gradientFields["dL"]          = home_FieldValue(v); }
            void setDH(const std::vector<double>& v)                        { gradientFields["dH"]          = home_FieldValue(v); }
            void setDb(const std::vector<double>& v)                        { gradientFields["db"]          = home_FieldValue(v); }
            void setDq1(const std::vector<double>& v)                       { gradientFields["dq1"]         = home_FieldValue(v); }
            void setDG1(const std::vector<double>& v)                       { gradientFields["dG1"]         = home_FieldValue(v); }
            void setDq2(const std::vector<double>& v)                       { gradientFields["dq2"]         = home_FieldValue(v); }
            void setDG2(const std::vector<double>& v)                       { gradientFields["dG2"]         = home_FieldValue(v); }
            void setDG3(const std::vector<double>& v)                       { gradientFields["dG3"]         = home_FieldValue(v); }
            void setDLVgradient(const std::vector<double>& v)               { gradientFields["dLVgradient"] = home_FieldValue(v); }
            
            void setDAltCorrLV(double v)                                    { gradientFields["dAltCorrLV"] = home_FieldValue(v); }
            void setDAltCorrHV(double v)                                    { gradientFields["dAltCorrHV"] = home_FieldValue(v); }
            
            void setDGradientF(const std::vector<double>& v)                { gradientFields["dGradientF"]      = home_FieldValue(v); }
            void setDGradientFinal(const std::vector<double>& v)            { gradientFields["dGradientFinal"]  = home_FieldValue(v); }
            
            void setDFCBNLVgradient(double v)                               { gradientFields["dFCBNLVgradient"] = home_FieldValue(v); }
            void setDTOR(double v)                                          { gradientFields["dTOR"]            = home_FieldValue(v); }
            
            void setDTORFinal(const std::vector<double>& v)                 { gradientFields["dTORFinal"] = home_FieldValue(v); }
            
            void setDMaxGrad(double v)                                      { gradientFields["dMaxGrad"]    = home_FieldValue(v); }
            void setDMaxGradF(double v)                                     { gradientFields["dMaxGradF"]   = home_FieldValue(v); }
            void setDFCBNHVGrad(double v)                                   { gradientFields["dFCBNHVGrad"] = home_FieldValue(v); }
            
            void setSErrorName(const std::vector<std::string>& v)           { gradientFields["sErrorName"] = home_FieldValue(v); }
            void setTErrorEnum(const std::vector<int>& v)                   { gradientFields["tErrorEnum"] = home_FieldValue(v); }
            void setSErrorMessages(const std::vector<std::string>& v)       { gradientFields["sErrorMessages"] = home_FieldValue(v); };
    };
// end of Gradients class
}
// end of home_Gradients namespace