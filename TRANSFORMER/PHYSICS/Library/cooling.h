/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Cooling.h
|    @brief     Specialized mathematical tracking engine evaluating transformer cooling states.
|    @brief     Transformer::Physics    is for "Behavior & Calculations"
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
#include "materials.h"
#include "enums.h"
#include "system_Config.h"

using namespace Transformer::Types;
namespace Transformer::Physics
{
    class Gradients;
    class Cooling
    {
        public:
            home_Device* device;
            home_Device* getParentDevice() const;

            std::map<std::string, home_FieldValue>  coolingFields;                                   
            std::map<std::string, double>           initialFieldValues;        
  
            explicit Cooling            (home_Device* parentDevice);

            bool isKNAN() const;

            int  calc_TempRiseNumber()   const;

            void reset_Fields();
            void erase_Warnings();
            void calc_Cooling();
            void calc_CoolingType();
            void calc_TempRise();
            void calc_ONAF();
            void calc_MVAOExc           (home_Winding* winding_Hv, home_DesignSpecCodes designSpecCodes, double NominalKva);
            void add_Step               (const std::string& step);
            void clear_Steps();
            void set_CoolingType_At     (int index, const std::string& value);
            void calc_NewCurrentTemp();
            void calc_UseFR3();
            void calc_MaxONAF           (home_Mechanical* mechanical);
            void calc_TORONAF           (home_Gradients*  gradients);



//                  G E T T E R S


        int get_CoolingStepsReal() const                            { return coolingFields.at("iCoolingStepsReal")  .get<int>(); }
        int get_CoolingSteps() const                                { return coolingFields.at("iCoolingSteps")      .get<int>(); }
        
        std::vector<int> get_CoolingType() const                    { return coolingFields.at("t_CoolingType")      .get<std::vector<int>>(); }
        int get_CoolingTypes() const                                { return coolingFields.at("tCoolingTypes")      .get<int>(); }
        
        std::vector<int> get_TemperatureRise() const                { return coolingFields.at("tTemperatureRise")   .get<std::vector<int>>(); }
        int get_TemperatureRises() const                            { return coolingFields.at("tTemperatureRises")  .get<int>(); }
        
        std::vector<std::string> get_TempCoolRise() const           { return coolingFields.at("sTempCoolRise")      .get<std::vector<std::string>>(); }
        
        bool get_ProvONAF() const                                   { return coolingFields.at("tProvONAF")          .get<bool>(); }
        bool get_K4Factor() const                                   { return coolingFields.at("tK4Factor")          .get<bool>(); }
        
        std::vector<int> get_CoolingTypeK4() const                  { return coolingFields.at("t_CoolingTypeK4")    .get<std::vector<int>>(); }
        std::vector<int> get_TemperatureRiseK4() const              { return coolingFields.at("tTemperatureRiseK4") .get<std::vector<int>>(); }
        
        double get_NewTempRise() const                              { return coolingFields.at("dNewTempRise")       .get<double>(); }
        double get_CurrentTempRise() const                          { return coolingFields.at("dCurrentempRise")    .get<double>(); }
        
        std::string get_UseFR3() const                              { return coolingFields.at("sUseFR3")            .get<std::string>(); }
        
        std::vector<double> get_MaxONAF() const                     { return coolingFields.at("dMaxONAF")           .get<std::vector<double>>(); }
        std::vector<double> get_TORONAF() const                     { return coolingFields.at("dTORONAF")           .get<std::vector<double>>(); }
        std::vector<int>    get_TempONAF() const                    { return coolingFields.at("tTempONAF")          .get<std::vector<int>>(); }
        
        bool get_OverExcitation() const                             { return coolingFields.at("overExcitation")     .get<bool>(); }
        int  get_TempOExc() const                                   { return coolingFields.at("tTempOExc")          .get<int>(); }

        std::vector<double> get_MVAOExc() const                     { return coolingFields.at("dMVAOExc")           .get<std::vector<double>>(); }
        std::string get_MVAOExc_String() const                      { return coolingFields.at("sMVAOExc")           .get<std::string>(); } // Renamed to avoid collision
        
        std::vector<std::string> get_ErrorName() const              { return coolingFields.at("sErrorName")         .get<std::vector<std::string>>(); }
        std::vector<int> get_ErrorEnum() const                      { return coolingFields.at("tErrorEnum")         .get<std::vector<int>>(); }
        std::vector<std::string> get_ErrorMessages() const          { return coolingFields.at("sErrorMessages")     .get<std::vector<std::string>>(); }


//                S E T T E R S                  


        void set_CoolingStepsReal(int v)                            { coolingFields["iCoolingStepsReal"] = home_FieldValue(v); }
        void set_CoolingSteps(int v)                                { coolingFields["iCoolingSteps"]     = home_FieldValue(v); }
        
        void set_CoolingType(const std::vector<int>& v)             { coolingFields["t_CoolingType"]     = home_FieldValue(v); }
        void set_CoolingTypes(int v)                                { coolingFields["tCoolingTypes"]     = home_FieldValue(v); }

        void set_TemperatureRise(const std::vector<int>& v)         { coolingFields["tTemperatureRise"]  = home_FieldValue(v); }
        void set_TemperatureRises(int v)                            { coolingFields["tTemperatureRises"] = home_FieldValue(v); }
        
        void set_TempCoolRise(const std::vector<std::string>& v)    { coolingFields["sTempCoolRise"]     = home_FieldValue(v); }
        
        void set_ProvONAF(bool v)                                   { coolingFields["tProvONAF"]         = home_FieldValue(static_cast<int>(v)); }
        void set_K4Factor(bool v)                                   { coolingFields["tK4Factor"]         = home_FieldValue(static_cast<int>(v)); }
        
        void set_CoolingTypeK4(const std::vector<int>& v)           { coolingFields["t_CoolingTypeK4"]   = home_FieldValue(v); }
        void set_TemperatureRiseK4(const std::vector<int>& v)       { coolingFields["tTemperatureRiseK4"]= home_FieldValue(v); }
        
        void set_NewTempRise(double v)                              { coolingFields["dNewTempRise"]      = home_FieldValue(v); }
        void set_CurrentTempRise(double v)                          { coolingFields["dCurrentempRise"]   = home_FieldValue(v); }
        
        void set_UseFR3(const std::string& v)                       { coolingFields["sUseFR3"]           = home_FieldValue(v); }
        
        void set_MaxONAF(const std::vector<double>& v)              { coolingFields["dMaxONAF"]          = home_FieldValue(v); }
        void set_TORONAF(const std::vector<double>& v)              { coolingFields["dTORONAF"]          = home_FieldValue(v); }
        void set_TempONAF(const std::vector<int>& v)                { coolingFields["tTempONAF"]         = home_FieldValue(v); }
        
        void set_OverExcitation(bool v)                             { coolingFields["overExcitation"]    = home_FieldValue(static_cast<int>(v)); }
        void set_TempOExc(int v)                                    { coolingFields["tTempOExc"]         = home_FieldValue(v); }
         
        void set_MVAOExc(const std::vector<double>& v)              { coolingFields["dMVAOExc"]          = home_FieldValue(v); }
        void set_MVAOExc_String(const std::string& v)               { coolingFields["sMVAOExc"]          = home_FieldValue(v); }
        
        void set_ErrorName(const std::vector<std::string>& v)       { coolingFields["sErrorName"]        = home_FieldValue(v); }
        void set_ErrorEnum(const std::vector<int>& v)               { coolingFields["tErrorEnum"]        = home_FieldValue(v); }
        void set_ErrorMessages(const std::vector<std::string>& v)   { coolingFields["sErrorMessages"]    = home_FieldValue(v); }
    };
   //end of Cooling class
}