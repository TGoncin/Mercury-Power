/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Guarantee.h
|    @brief     Specialized evaluation engine managing electrical transformer performance guarantees.
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initialization of baseline assessment metrics using localized FieldValues.
|                   - Calculates specified bounds for no-load losses, load losses, and excitation currents.
|                   - Formats performance data strings aligned to international specifications (ANSI/DOE).
|               Returns:
|                   - High-performance structure tracking contractual design compliance boundaries.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <algorithm>

#include "alias.h"
#include "enums.h"
#include "field_Value.h"

#include "guarantee_Warnings.h"
#include "guarantee.h"

using namespace Transformer::Types;



namespace Transformer::Commercial
{
    class Guarantee
    {
        private:
            static int id_Counter;

        public:
            std::map <std::string, home_FieldValue>             guaranteeFields;

            home_Device*                                        device;
            home_GuaranteeWarnings                              guaranteeWarnings;

            explicit Guarantee (home_Device* parentDevice);

            home_Device* getParentDevice() const;

            void    reset_Fields();
            void    erase_Warnings();
            void    calc_Warnings();
            void    calc_Guarantee();
            void    set_DefaultGuarantees();
            void    calc_LimitedGuarantees();

        // _________________________________________________________________________________________
        // |                                      GETTERS                                          |
        // |_______________________________________________________________________________________|

            int                             getId                   () const { return guaranteeFields.at("id")                  .get<int>(); }
            std::string                     getName                 () const { return guaranteeFields.at("name")                .get<std::string>(); }
            
            double                          getDEvaluation_nl       () const { return guaranteeFields.at("dEvaluation_nl")      .get<double>(); }
            double                          getDEvaluation_ll       () const { return guaranteeFields.at("dEvaluation_ll")      .get<double>(); }
            
            std::string                     getSEvaluation_nl       () const { return guaranteeFields.at("sEvaluation_nl")      .get<std::string>(); }
            std::string                     getSEvaluation_ll       () const { return guaranteeFields.at("sEvaluation_ll")      .get<std::string>(); }
            std::string                     getSCustomerSpec        () const { return guaranteeFields.at("sCustomerSpec")       .get<std::string>(); }
            
            home_DesignSpecCodes            getTDesignSpec          () const { return guaranteeFields.at("tDesignSpec")         .get<home_DesignSpecCodes>(); }
            std::string                     getSDesignSpec          () const { return guaranteeFields.at("sDesignSpec")         .get<std::string>(); }
            
            home_EfficiencySpec             getTEfficiencySpec      () const { return guaranteeFields.at("tEfficiencySpec")     .get<home_EfficiencySpec>(); }
            
            bool                            getBNLCorrected_to_85C  () const { return guaranteeFields.at("bNLCorrected_to_85C") .get<bool>(); }
            
            double                          getDNL_pu               () const { return guaranteeFields.at("dNL_pu")              .get<double>(); }
            double                          getDGroundShortTime     () const { return guaranteeFields.at("dGroundShortTime")    .get<double>(); }
            double                          getDVirtualZ            () const { return guaranteeFields.at("dVirtualZ")           .get<double>(); }
            double                          getDNLGuarantee         () const { return guaranteeFields.at("dNLGuarantee")        .get<double>(); }
            double                          getDLLGuarantee         () const { return guaranteeFields.at("dLLGuarantee")        .get<double>(); }
            double                          getDIZGuarantee         () const { return guaranteeFields.at("dIZGuarantee")        .get<double>(); }
            double                          getDExcGuarantee        () const { return guaranteeFields.at("dExcGuarantee")       .get<double>(); }
            
            std::string                     getSNLGuarantee         () const { return guaranteeFields.at("sNLGuarantee")        .get<std::string>(); }
            std::string                     getSLLGuarantee         () const { return guaranteeFields.at("sLLGuarantee")        .get<std::string>(); }
            std::string                     getSTotLGuarantee       () const { return guaranteeFields.at("sTotLGuarantee")      .get<std::string>(); }
            std::string                     getSIZGuarantee         () const { return guaranteeFields.at("sIZGuarantee")        .get<std::string>(); }
            std::string                     getSExcGuarantee        () const { return guaranteeFields.at("sExcGuarantee")       .get<std::string>(); }
            
            bool                            getBNLLimited           () const { return guaranteeFields.at("bNLLimited")          .get<bool>(); }
            bool                            getBLLLimited           () const { return guaranteeFields.at("bLLLimited")          .get<bool>(); }
            bool                            getBExcLimited          () const { return guaranteeFields.at("bExcLimited")         .get<bool>(); }
            
            std::vector<std::string>        getSErrorName           () const { return guaranteeFields.at("sErrorName")          .get<std::vector<std::string>>(); }
            std::vector<home_WarningSeverity> getTErrorEnum         () const { return guaranteeFields.at("tErrorEnum")          .get<std::vector<home_WarningSeverity>>(); }
            std::vector<std::string>        getSErrorMessages       () const { return guaranteeFields.at("sErrorMessages")      .get<std::vector<std::string>>(); }

        // _________________________________________________________________________________________
        // |                                      SETTERS                                          |
        // |_______________________________________________________________________________________|

            void setId                      (int v)                                     { guaranteeFields["id"]                  = home_FieldValue(v); }
            void setName                    (const std::string& v)                      { guaranteeFields["name"]                = home_FieldValue(v); }
            
            void setDEvaluation_nl          (double v)                                  { guaranteeFields["dEvaluation_nl"]      = home_FieldValue(v); }
            void setDEvaluation_ll          (double v)                                  { guaranteeFields["dEvaluation_ll"]      = home_FieldValue(v); }
            
            void setSEvaluation_nl          (const std::string& v)                      { guaranteeFields["sEvaluation_nl"]      = home_FieldValue(v); }
            void setSEvaluation_ll          (const std::string& v)                      { guaranteeFields["sEvaluation_ll"]      = home_FieldValue(v); }
            void setSCustomerSpec           (const std::string& v)                      { guaranteeFields["sCustomerSpec"]       = home_FieldValue(v); }
            
            void setTDesignSpec             (home_DesignSpecCodes v)                    { guaranteeFields["tDesignSpec"]         = home_FieldValue(v); }
            void setSDesignSpec             (const std::string& v)                      { guaranteeFields["sDesignSpec"]         = home_FieldValue(v); }
            
            void setTEfficiencySpec         (home_EfficiencySpec v)                     { guaranteeFields["tEfficiencySpec"]     = home_FieldValue(v); }
            
            void setBNLCorrected_to_85C     (bool v)                                    { guaranteeFields["bNLCorrected_to_85C"] = home_FieldValue(v); }
            
            void setDNL_pu                  (double v)                                  { guaranteeFields["dNL_pu"]              = home_FieldValue(v); }
            void setDGroundShortTime        (double v)                                  { guaranteeFields["dGroundShortTime"]    = home_FieldValue(v); }
            void setDVirtualZ               (double v)                                  { guaranteeFields["dVirtualZ"]           = home_FieldValue(v); }
            void setDNLGuarantee            (double v)                                  { guaranteeFields["dNLGuarantee"]        = home_FieldValue(v); }
            void setDLLGuarantee            (double v)                                  { guaranteeFields["dLLGuarantee"]        = home_FieldValue(v); }
            void setDIZGuarantee            (double v)                                  { guaranteeFields["dIZGuarantee"]        = home_FieldValue(v); }
            void setDExcGuarantee           (double v)                                  { guaranteeFields["dExcGuarantee"]       = home_FieldValue(v); }
            
            void setSNLGuarantee            (const std::string& v)                      { guaranteeFields["sNLGuarantee"]        = home_FieldValue(v); }
            void setSLLGuarantee            (const std::string& v)                      { guaranteeFields["sLLGuarantee"]        = home_FieldValue(v); }
            void setSTotLGuarantee          (const std::string& v)                      { guaranteeFields["sTotLGuarantee"]      = home_FieldValue(v); }
            void setSIZGuarantee            (const std::string& v)                      { guaranteeFields["sIZGuarantee"]        = home_FieldValue(v); }
            void setSExcGuarantee           (const std::string& v)                      { guaranteeFields["sExcGuarantee"]       = home_FieldValue(v); }
            
            void setBNLLimited              (bool v)                                    { guaranteeFields["bNLLimited"]          = home_FieldValue(v); }
            void setBLLLimited              (bool v)                                    { guaranteeFields["bLLLimited"]          = home_FieldValue(v); }
            void setBExcLimited             (bool v)                                    { guaranteeFields["bExcLimited"]         = home_FieldValue(v); }
            
            void setSErrorName              (const std::vector<std::string>& v)         { guaranteeFields["sErrorName"]          = home_FieldValue(v); }
            void setTErrorEnum              (const std::vector<home_WarningSeverity>& v){ guaranteeFields["tErrorEnum"]          = home_FieldValue(v); }
            void setSErrorMessages          (const std::vector<std::string>& v)         { guaranteeFields["sErrorMessages"]      = home_FieldValue(v); }
 };
}