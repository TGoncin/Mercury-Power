/**___________________________________________________________________________________________________
|    @date      May 26, 2026
|    @author    Tom Goncin
|    @file      Gradients.cpp
|    @brief     Specialized mathematical tracking engine evaluating transformer thermal gradients.
|    @brief     Transformer::Physics    is for "Behavior & Calculations"
|
|               Executes:
|                   - Instantiates and processes thermal fluid dynamics parameters using home_FieldValues.
|                   - Evaluates localized winding gradients, hot-spot factors, and altitudinal corrections.
|                   - Solves Top-Oil Rise (TOR) constraints against forced and natural cooling limits.
|               Returns:
|                   - High-performance structures tracking electrical transformer thermal physics.
|___________________________________________________________________________________________________*/

#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
#include "math_Utilities.h"
#include "system_Config.h"

#include "cooling.h"
#include "gradient.h"
#include "device.h"

using namespace Transformer::Types;



namespace Transformer::Physics
{
    int Gradients::id_Counter = 1;

    Gradients::Gradients(home_Device* parentDevice, home_MaterialConstants materialConstants)    
        : device(parentDevice), table_MaterialConstants(materialConstants), gradientWarnings(this)
    {

        gradientFields["id"]                = home_FieldValue(id_Counter);
        gradientFields["name"]              = home_FieldValue("Gradients " + std::to_string(id_Counter));
        gradientFields["dn"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dW"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dN"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dL"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dH"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["db"]                = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dq1"]               = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dG1"]               = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dq2"]               = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dG2"]               = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dG3"]               = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dLVgradient"]       = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dAltCorrLV"]        = home_FieldValue(0.0);
        gradientFields["dAltCorrHV"]        = home_FieldValue(0.0);
        gradientFields["dGradientF"]        = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dGradientFinal"]    = home_FieldValue(home_FieldValue::List(Config::MAX_WINDINGS, 0.0));
        gradientFields["dFCBNLVgradient"]   = home_FieldValue(0.0);
        gradientFields["dTOR"]              = home_FieldValue(0.0);
        gradientFields["dTORFinal"]         = home_FieldValue(home_FieldValue::List(3, 0.0));
        gradientFields["dMaxGrad"]          = home_FieldValue(0.0);
        gradientFields["dMaxGradF"]         = home_FieldValue(0.0);
        gradientFields["dFCBNHVGrad"]       = home_FieldValue(0.0);
        gradientFields["sErrorName"]        = home_FieldValue(std::vector<std::string>(Config::ERRORMESSAGES, ""));
        gradientFields["tErrorEnum"]        = home_FieldValue(home_FieldValue::List(Config::ERRORMESSAGES, static_cast<int>(home_WarningSeverity::None)));
        gradientFields["sConfig::ERRORMESSAGES"] = home_FieldValue(std::vector<std::string>(Config::ERRORMESSAGES, ""));

        Gradients::id_Counter++;
    }

    home_Device* Gradients::getParentDevice() const
    {
        if (device == nullptr)      {   throw std::runtime_error("This sub-module is not attached to any Device");  }
        return device;
    }

    void Gradients::reset_Fields()
    {
        for (auto& pair : gradientFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }

    void Gradients::erase_Warnings()
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i)
        {
            gradientFields["sErrorName"].set("", i);
            gradientFields["tErrorEnum"].set(static_cast<int>(home_WarningSeverity::None), i);
            gradientFields["sConfig::ERRORMESSAGES"].set("", i);
        }
    }

    void Gradients::calc_Warnings()
    {
        int numberOfCoils = getParentDevice()->coil.size();

        for (int i = 0; i < numberOfCoils; ++i)
        {
            std::vector<double> emptyValues = {0.0, 0.0};
            gradientWarnings.warning_ValueGradient(i, emptyValues);
        }
    }

    void Gradients::calc_Gradients(home_Cooling* cooling)
    {
        calc_GradientsDef();
        calc_ThermalRise(cooling);
    }

    void Gradients::calc_GradientsDef()
    {
        auto&           gradientField   = gradientFields;
        home_Device*    device          = getParentDevice();
        int             coilCount       = device->deviceFields["num_Coils"].get<int>();

        auto dn = gradientField["dn"].get<std::vector<double>>();
        auto dW = gradientField["dW"].get<std::vector<double>>();
        auto dN = gradientField["dN"].get<std::vector<double>>();
        auto dL = gradientField["dL"].get<std::vector<double>>();
        auto dH = gradientField["dH"].get<std::vector<double>>();
        auto db = gradientField["db"].get<std::vector<double>>();

        for (int i = 0; i < coilCount; ++i) 
        {
            double dq1Val = Config::safe_Division(dW[i], 2.0 * dN[i] * dL[i] * dH[i]);
            gradientField["dq1"].set(dq1Val, i);

            double dK_Oil = table_MaterialConstants.tableData[5][0]; 
            double dG1Val = dK_Oil * std::pow(dq1Val, 0.8) * std::pow(db[i], 0.4);
            gradientField["dG1"].set(dG1Val, i);

            double dq2Val = Config::safe_Division(dW[i], 2.0 * dn[i] * dL[i] * dH[i]);
            gradientField["dq2"].set(dq2Val, i);

            double dG2Val = dK_Oil * std::pow(dq2Val, 0.8) * std::pow(db[i], 0.4);
            gradientField["dG2"].set(dG2Val, i);

            double dG3Val = 1.35 * (dG1Val + dG2Val);
            gradientField["dG3"].set(dG3Val, i);

            double dLVgradVal = dG1Val + dG2Val + dG3Val;
            gradientField["dLVgradient"].set(dLVgradVal, i);
        }
    }

    void Gradients::calc_ThermalRise(home_Cooling* cooling)
    {
        auto&        gradientField  = gradientFields;
        home_Device* device         = getParentDevice();
        
        int     numberPhases= device->deviceFields["iNoPhases"].get<int>();
        double  nominalKva = device->deviceFields["nominal_kva"].get<double>();
        int     coilCount   = device->deviceFields["num_Coils"].get<int>();

        int positionLv = device->get_PositionBT();
        int positionHv = device->get_PositionAT();

        double  altCorrectLvValue   = gradientField["dAltCorrLV"].get<double>();
        double  altCorrectHvValue   = gradientField["dAltCorrHV"].get<double>();
        auto    gradientLv          = gradientField["dLVgradient"].get<std::vector<double>>();

        auto& gradientF     = gradientField["dGradientF"];
        auto& gradientFinal = gradientField["dGradientFinal"];

        double maxGradF = 0.0;
        double maxGrad  = 0.0;

        for (int i = 0; i < coilCount; ++i) 
        {
            double corr     = (i == positionLv) ? altCorrectLvValue : altCorrectHvValue;
            double gradF    = gradientLv[i] + corr;
            gradientF.set(gradF, i);

            double finalGrad = gradF * 1.15; 
            gradientFinal.set(finalGrad, i);

            maxGradF = std::max(maxGradF, gradF);
            maxGrad  = std::max(maxGrad, finalGrad);
        }

        gradientField["dMaxGradF"].set(maxGradF);
        gradientField["dMaxGrad"].set(maxGrad);

        double fcbnLvGradient = gradientLv[positionLv] * 1.15 + altCorrectLvValue;
        gradientField["dFCBNLVgradient"].set(fcbnLvGradient);

        auto temperatureRiseVector = cooling->coolingFields["tTemperatureRise"].get<std::vector<home_TemperatureRise>>();
        double tempRise = (temperatureRiseVector[0] == home_TemperatureRise::Temp_65) ? 65.0 : 55.0;

        double result = 0.0;

        if (numberPhases == 1)
        {
            result = (tempRise - maxGrad) * 1.1 - 1.0;
        }
        else
        {
            double subFactor = (nominalKva <= 1000.0) ? 2.0 : ( (nominalKva < 4000.0) ? 3.0 : 4.0 );
            result           = (tempRise - maxGrad) * 1.1 - subFactor;
        }
        gradientField["dTOR"].set(result);

        double fcbn_HvGradientValue = gradientField["dG1"].get<std::vector<double>>()[positionLv] 
                                    + gradientField["dG2"].get<std::vector<double>>()[positionLv] 
                                    + gradientField["dG3"].get<std::vector<double>>()[positionLv];
        
        if (cooling->isKNAN())      {   fcbn_HvGradientValue += 5.0;  }
        gradientField["dFCBNHVGrad"].set(fcbn_HvGradientValue);

        maxGradF = std::max(gradientLv[positionHv], fcbnLvGradient);
        maxGrad  = std::max(maxGradF, fcbn_HvGradientValue);

        if (numberPhases == 1)        { result = (tempRise - maxGrad) * 1.1 - 1.0;  } 
        else
        {
            double subFactor    = (nominalKva <= 1000.0) ? 2.0 : ( (nominalKva < 4000.0) ? 3.0 : 4.0 );
            result              = (tempRise - maxGrad) * 1.1 - subFactor;
        }

        int steps = cooling->coolingFields["iCoolingSteps"].get<int>();

        for (int i = 0; i < steps; ++i)     {   gradientField["dTORFinal"].set(result, i);  }
    }

}
//end of home_Gradient