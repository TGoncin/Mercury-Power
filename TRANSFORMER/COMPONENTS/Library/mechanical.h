/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Mechanical.h
|    @brief     Implementor of architecture layer driven transformer physical & mechanical boundary evaluations.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|  - Initialization of physical bounding dimensions, weights, and oil parameters via FieldValues.
|  - Computes altitudinal ambient adjustments, hot-spot rise limits, and radiator step coefficients.
|  - Resolves localized structural stress variables against maximum manufacturing tolerances.
|               Returns:
|  - High-performance, isolated representation of the transformer mechanical system.
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
#include "field_Value.h"
#include "enums.h"

#include "mechanical_Warnings.h"


namespace Transformer::Components
{
    class Mechanical
    {
        private:
            static int id_Counter;

            home_Device*             device;
            home_Mechanical*         mechanical;
            home_MechanicalWarnings  mechanicalWarnings;

        public:
            std::map <std::string, home_FieldValue>             mechanicalFields;

            explicit                 Mechanical (home_Device* parentDevice = nullptr);

            home_Device*             getParentDevice () const;

            void    reset_Fields ();
            void    erase_Warnings ();
            void    calc_Warnings ();
            void    calc_Mechanical             (home_Cooling* cooling,   home_Gradients* gradient);
            void    calc_MechanicalDefault      (home_Cooling* cooling);
            void    calc_AwrTorHsr              (home_Cooling* cooling,   home_Gradients* gradient);
            void    calc_MaxAwrTorHsr           (home_Cooling* cooling);
            void    logError_Mechanical         (home_Mechanical* mechanical, const std::string& name, home_WarningSeverity warningType, const std::string& msg);

        // _________________________________________________________________________________________
        // |           GETTERS               |
        // |_______________________________________________________________________________________|

            bool getCalculoWithRad       () const { return mechanicalFields.at("bCalculoWithRad")    .get<bool>();          }
            
            double         getH   () const { return mechanicalFields.at("dH")   .get<double>(); }
            double         getL   () const { return mechanicalFields.at("dL")   .get<double>(); }
            double         getD   () const { return mechanicalFields.at("dD")   .get<double>(); }
            double         getA   () const { return mechanicalFields.at("da")   .get<double>(); }
            double         getHin                  () const { return mechanicalFields.at("dHin")               .get<double>(); }
            double         getLin                  () const { return mechanicalFields.at("dLin")               .get<double>(); }
            double         getDin                  () const { return mechanicalFields.at("dDin")               .get<double>(); }
            double         getAin                  () const { return mechanicalFields.at("dain")               .get<double>(); }
            double         getLosses               () const { return mechanicalFields.at("dLosses")            .get<double>(); }
            double         getHotSpotFactor        () const { return mechanicalFields.at("dHotSpotFactor")     .get<double>(); }
            double         getNameplateRise        () const { return mechanicalFields.at("dNameplateRise")     .get<double>(); }
            double         getLVGradient           () const { return mechanicalFields.at("dLVGradient")        .get<double>(); }
            double         getHVGradient           () const { return mechanicalFields.at("dHVGradient")        .get<double>(); }
            double         getMaxAWR               () const { return mechanicalFields.at("dMaxAWR")            .get<double>(); }
            double         getMaxTOR               () const { return mechanicalFields.at("dMaxTOR")            .get<double>(); }
            double         getMaxHSR               () const { return mechanicalFields.at("dMaxHSR")            .get<double>(); }

            std::vector<double>     getCorGradLV0   () const { return mechanicalFields.at("dCorGradLV0")        .get<std::vector<double>>(); }
            std::vector<double>     getCorGradHV0   () const { return mechanicalFields.at("dCorGradHV0")        .get<std::vector<double>>(); }
            std::vector<double>     getTORTOT0      () const { return mechanicalFields.at("dTORTOT0")           .get<std::vector<double>>(); }
            std::vector<double>     getAWRLV0       () const { return mechanicalFields.at("dAWRLV0")            .get<std::vector<double>>(); }
            std::vector<double>     getAWRHV0       () const { return mechanicalFields.at("dAWRHV0")            .get<std::vector<double>>(); }
            std::vector<double>     getHSRLV0       () const { return mechanicalFields.at("dHSRLV0")            .get<std::vector<double>>(); }
            std::vector<double>     getHSRHV0       () const { return mechanicalFields.at("dHSRHV0")            .get<std::vector<double>>(); }
            std::vector<double>     getKW1          () const { return mechanicalFields.at("d_kW1")              .get<std::vector<double>>(); }

        // _________________________________________________________________________________________
        // |           SETTERS               |
        // |_______________________________________________________________________________________|

            void setCalculoWithRad       (bool value)    { mechanicalFields["bCalculoWithRad"]   = home_FieldValue(value); }
            
            void setH   (double value)  { mechanicalFields["dH"]                = home_FieldValue(value); }
            void setL   (double value)  { mechanicalFields["dL"]                = home_FieldValue(value); }
            void setD   (double value)  { mechanicalFields["dD"]                = home_FieldValue(value); }
            void setA   (double value)  { mechanicalFields["da"]                = home_FieldValue(value); }
            void setHin                  (double value)  { mechanicalFields["dHin"]              = home_FieldValue(value); }
            void setLin                  (double value)  { mechanicalFields["dLin"]              = home_FieldValue(value); }
            void setDin                  (double value)  { mechanicalFields["dDin"]              = home_FieldValue(value); }
            void setAin                  (double value)  { mechanicalFields["dain"]              = home_FieldValue(value); }
            void setLosses               (double value)  { mechanicalFields["dLosses"]           = home_FieldValue(value); }
            void setHotSpotFactor        (double value)  { mechanicalFields["dHotSpotFactor"]    = home_FieldValue(value); }
            void setNameplateRise        (double value)  { mechanicalFields["dNameplateRise"]    = home_FieldValue(value); }
            void setLVGradient           (double value)  { mechanicalFields["dLVGradient"]       = home_FieldValue(value); }
            void setHVGradient           (double value)  { mechanicalFields["dHVGradient"]       = home_FieldValue(value); }
            void setMaxAWR               (double value)  { mechanicalFields["dMaxAWR"]           = home_FieldValue(value); }
            void setMaxTOR               (double value)  { mechanicalFields["dMaxTOR"]           = home_FieldValue(value); }
            void setMaxHSR               (double value)  { mechanicalFields["dMaxHSR"]           = home_FieldValue(value); }

            void setCorGradLV0           (const std::vector<double>& value)         { mechanicalFields["dCorGradLV0"]       = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setCorGradHV0           (const std::vector<double>& value)         { mechanicalFields["dCorGradHV0"]       = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setTORTOT0              (const std::vector<double>& value)         { mechanicalFields["dTORTOT0"]          = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setAWRLV0               (const std::vector<double>& value)         { mechanicalFields["dAWRLV0"]           = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setAWRHV0               (const std::vector<double>& value)         { mechanicalFields["dAWRHV0"]           = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setHSRLV0               (const std::vector<double>& value)         { mechanicalFields["dHSRLV0"]           = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setHSRHV0               (const std::vector<double>& value)         { mechanicalFields["dHSRHV0"]           = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
            void setKW1                  (const std::vector<double>& value)         { mechanicalFields["d_kW1"]             = home_FieldValue(home_FieldValue::List(value.begin(), value.end())); }
    };
}
//end of Transformer::Components::Mechanical