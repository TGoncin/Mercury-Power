/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Mechanical.cpp
|    @brief     Implementor of architecture layer driven transformer physical & mechanical boundary evaluations.
|    @brief     Transformer::Components     is "Physical Hardware"
|
|               Executes:
|                   - Initialization of physical bounding dimensions, weights, and oil parameters via home_FieldValues.
|                   - Computes altitudinal ambient adjustments, hot-spot rise limits, and radiator step coefficients.
|                   - Resolves localized structural stress variables against maximum manufacturing tolerances.
|               Returns:
|                   - High-performance, isolated representation of the transformer mechanical system.
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "system_Config.h"


#include "device.h"
#include "cooling.h"
#include "gradient.h"
#include "cooling.h"

#include "mechanical.h"



namespace Transformer::Components
{
    int Mechanical::id_Counter = 1;

    Mechanical::Mechanical(home_Device* parentDevice) : device(parentDevice), mechanicalWarnings(this)
    {
        mechanicalFields["bCalculoWithRad"]   = home_FieldValue (false);
        mechanicalFields["dH"]                = home_FieldValue (0.0);
        mechanicalFields["dL"]                = home_FieldValue (0.0);
        mechanicalFields["dD"]                = home_FieldValue (0.0);
        mechanicalFields["da"]                = home_FieldValue (0.0);
        mechanicalFields["dHin"]              = home_FieldValue (0.0);
        mechanicalFields["dLin"]              = home_FieldValue (0.0);
        mechanicalFields["dDin"]              = home_FieldValue (0.0);
        mechanicalFields["dain"]              = home_FieldValue (0.0);
        mechanicalFields["tSegment"]          = home_FieldValue (home_FieldValue::List(4, home_Segment::Free));
        mechanicalFields["dSegment"]          = home_FieldValue (home_FieldValue::List(4, 0.0));
        mechanicalFields["dSegmentin"]        = home_FieldValue (home_FieldValue::List(4, 0.0));
        mechanicalFields["dLosses"]           = home_FieldValue (0.0);
        mechanicalFields["dHotSpotFactor"]    = home_FieldValue (1.3);
        mechanicalFields["dNameplateRise"]    = home_FieldValue (0.0);
        mechanicalFields["dLVGradient"]       = home_FieldValue (0.0);
        mechanicalFields["dHVGradient"]       = home_FieldValue (0.0);
        mechanicalFields["dCorGradLV0"]       = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dCorGradHV0"]       = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dTORTOT0"]          = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dAWRLV0"]           = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dAWRHV0"]           = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dHSRLV0"]           = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dHSRHV0"]           = home_FieldValue (home_FieldValue::List(2, 0.0));
        mechanicalFields["dMaxAWR"]           = home_FieldValue (0.0);
        mechanicalFields["dMaxTOR"]           = home_FieldValue (0.0);
        mechanicalFields["dMaxHSR"]           = home_FieldValue (0.0);
        mechanicalFields["d_kW1"]             = home_FieldValue (home_FieldValue::List(5, 0.0));
        mechanicalFields["sErrorName"]        = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, ""));
        mechanicalFields["tErrorEnum"]        = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, home_WarningSeverity::None));
        mechanicalFields["sErrorMessages"]    = home_FieldValue (home_FieldValue::List(Config::ERRORMESSAGES, ""));

        Mechanical::id_Counter++;
    }

    home_Device* Mechanical::getParentDevice() const
    {
        if (device == nullptr)  throw std::runtime_error("This module is not attached to any Device");
        else                    return device;
    }
    
    void Mechanical::reset_Fields()
    {
        for (auto& pair : mechanicalFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }
    
    void Mechanical::erase_Warnings()
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i)
        {
            mechanicalFields["sErrorName"]    .set("", i);
            mechanicalFields["tErrorEnum"]    .set(home_WarningSeverity::None, i);
            mechanicalFields["sErrorMessages"].set("", i);
        }
    }
    
    void Mechanical::calc_Warnings()    // Specifically tank boundaries 
    {
        std::vector<double> emptyValues = {0.0, 0.0};

        mechanicalWarnings.warning_IntTankMaxLength (emptyValues);
        mechanicalWarnings.warning_IntTankMaxDepth  (emptyValues);
        mechanicalWarnings.warning_IntTankMaxHeight (emptyValues);
    }
    
    void Mechanical::calc_Mechanical(home_Cooling* cooling,   home_Gradients* gradient)
    {
        calc_MechanicalDefault  (cooling);
        calc_AwrTorHsr          (cooling, gradient);
        calc_MaxAwrTorHsr       (cooling);
    }
    
    void Mechanical::calc_MechanicalDefault(home_Cooling* cooling)
    {
        auto&   object          = mechanicalFields;
        int     coolingSteps    = cooling->coolingFields["iCoolingSteps"].get<int>();
        bool    k4Factor        = (cooling->coolingFields["tK4Factor"].get<bool>() == true);
        
        int indexOffset         = k4Factor ? 1 : 0;
        auto& kw1               = object["d_kW1"];

        // Processes structural thermal dissipation limits sequentially across steps
        for (int i = 0; i < coolingSteps; ++i)
        {
            if (i == 0 && !k4Factor)       continue;

            double calculatedLossLimit = object["dLosses"].get<double>() * std::pow(1.1, i - indexOffset);
            kw1.set(calculatedLossLimit, i);
        }
    }
    
    void Mechanical::calc_AwrTorHsr(home_Cooling* cooling, home_Gradients* gradient)
    {
        home_Device* device = getParentDevice();

        int positionLV = device->get_PositionBT();
        int positionHV = device->get_PositionAT();

        auto gradientLV    = gradient->gradientFields["dLVgradient"]   .get<std::vector<double>>();
        auto gradientFinal = gradient->gradientFields["dGradientFinal"].get<std::vector<double>>();
        auto finalTOR      = gradient->gradientFields["dTORFinal"]     .get<std::vector<double>>();

        auto& corGradLV0 = mechanicalFields["dCorGradLV0"];
        auto& corGradHV0 = mechanicalFields["dCorGradHV0"];
        auto& torTOR0    = mechanicalFields["dTORTOT0"];
        auto& awrLV0     = mechanicalFields["dAWRLV0"];
        auto& awrHV0     = mechanicalFields["dAWRHV0"];
        auto& hsrLV0     = mechanicalFields["dHSRLV0"];
        auto& hsrHV0     = mechanicalFields["dHSRHV0"];

        corGradLV0  .set(gradientLV[positionLV], 0);
        corGradHV0  .set(gradientLV[positionHV], 0);
        torTOR0     .set(finalTOR[0], 0);

        bool overExcitation = (cooling->coolingFields["overExcitation"].get<bool>() == true);

        if (overExcitation)
        {
            corGradLV0  .set(gradient->gradientFields["dFCBNLVgradient"].get<double>(), 1);
            corGradHV0  .set(gradient->gradientFields["dFCBNHVGrad"]    .get<double>(), 1);
            torTOR0     .set(finalTOR[1], 1);
        }

// Step 0 primary rise calculation loops
        double result = torTOR0.get<std::vector<double>>()[0] + corGradLV0.get<std::vector<double>>()[0];
        awrLV0.set(result, 0);

        if (overExcitation)    result = torTOR0.get<std::vector<double>>()[1] + corGradLV0.get<std::vector<double>>()[1];
        else                   result = torTOR0.get<std::vector<double>>()[0] + corGradLV0.get<std::vector<double>>()[1]; // Safe fallback match
        
        awrLV0.set(result, 1);

// Step 1 primary rise calculation loops
        result = torTOR0.get<std::vector<double>>()[0] + corGradHV0.get<std::vector<double>>()[0];
        awrHV0.set(result, 0);

        if (overExcitation)        result = torTOR0.get<std::vector<double>>()[1] + corGradHV0.get<std::vector<double>>()[1];
        else                       result = torTOR0.get<std::vector<double>>()[0] + corGradHV0.get<std::vector<double>>()[1];
        awrHV0.set(result, 1);

// Hot-spot rise factor derivations (1.1x scaling constraint)
        result = torTOR0.get<std::vector<double>>()[0] + (corGradLV0.get<std::vector<double>>()[0] * 1.1);
        hsrLV0.set(result, 0);

        result = torTOR0.get<std::vector<double>>()[1] + (corGradLV0.get<std::vector<double>>()[1] * 1.1);
        hsrLV0.set(result, 1);

        result = torTOR0.get<std::vector<double>>()[0] + (corGradHV0.get<std::vector<double>>()[0] * 1.1);
        hsrHV0.set(result, 0);

        result = torTOR0.get<std::vector<double>>()[1] + (corGradHV0.get<std::vector<double>>()[1] * 1.1);
        hsrHV0.set(result, 1);
    }
    
    void Mechanical::calc_MaxAwrTorHsr(home_Cooling* cooling)
    {
        auto& object = mechanicalFields; 
        
        int steps = cooling->coolingFields["iCoolingSteps"].get<int>();

        auto awrLV0_vec  = object["dAWRLV0"] .get<std::vector<double>>();
        auto awrHV0_vec  = object["dAWRHV0"] .get<std::vector<double>>();
        auto torTOT0_vec = object["dTORTOT0"].get<std::vector<double>>();
        auto hsrLV0_vec  = object["dHSRLV0"] .get<std::vector<double>>();
        auto hsrHV0_vec  = object["dHSRHV0"] .get<std::vector<double>>();

        double maxAWR = 0.0;
        double maxTOR = 0.0;
        double maxHSR = 0.0;

        for (int i = 0; i < steps; ++i)
        {
            maxAWR = std::max( {maxAWR, awrLV0_vec[i], awrHV0_vec[i]} );
            maxTOR = std::max( maxTOR, torTOT0_vec[i] );
            maxHSR = std::max( {maxHSR, hsrLV0_vec[i], hsrHV0_vec[i]} );
        }

        object["dMaxAWR"].set(maxAWR);
        object["dMaxTOR"].set(maxTOR);
        object["dMaxHSR"].set(maxHSR);
    }
}
//end of class Mechanical