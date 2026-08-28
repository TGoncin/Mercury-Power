/**___________________________________________________________________________________________________
|    @date      May 19, 2026 ~ EDIT: August 26, 2026
|    @author    Tom Goncin
|    @file      designer.cpp
|    @brief     Coordinator transformer execution, talks to serializer framework
|    @brief    Transformer::Core       is "The Engine Framework"
|               copyright   © PTI Transformers
| 
|               Executes:
|                  - Initialization of transformer object
|                  - Maps entire devices macro-structure
|                  - Evaluates multivariable/global engineering penalty metrics
|
|               Heavily references & instantiates micro frameworks (Dependancies):
|                  - Complex custom data structures [e.g. dev.Device() ]
|                  - Warning managers
|                  - Serializer framework:      Translates complex, in-memory data structures/objects into standardized format 
|___________________________________________________________________________________________________*/
#include <iostream>
#include <cmath>
#include <limits>

#include "alias.h"
using namespace Transformer::Types;

#include "device.h"
#include "core.h"
#include "mechanical.h"
#include "gradient.h"
#include "losses.h"
#include "guarantee.h"
#include "winding.h"

#include "designer.h"


namespace Transformer::Core
{
    Designer::Designer(home_Device* deviceE)
    {
        if (deviceE) 
        { 
            this->device = deviceE; 
        }
        else 
        { 
            this->device = new home_Device(); 
        }

        cost = 0.0;
        penalty = 0.0;
        warning = 0.0;
        validDesign = true;
        penaltyLevel = -1;
    }

    double Designer::designSinglePhase()
    {
        cost = 0.0;
        warning = 0.0;
        penalty = 0.0;
        penaltyLevel = -1;

        mechanicalDevice();
        designDevice();

        if (cost > 0.0) { calculateWarning(); }
        else { warning = 0.0; }

        calculatePenalty();

        listSolutions_LV.clear();
        listSolutions_HV.clear();

        return cost + warning + penalty;
    }

    void Designer::mechanicalDevice()
    {
        if (!device || !device->mechanical) return;

        bool bCalculoWithRad = device->mechanical->mechanicalFields["bCalculoWithRad"].get<bool>();
        if (!bCalculoWithRad) return;

        // Bridge directly to actual mechanical properties
        std::vector<double> dValue = { 0.0, 0.0 };
        device->mechanical->mechanicalFields["iTankIntLarge"].set(10000);
        device->mechanical->mechanicalFields["iTankIntWidth"].set(10000);
        device->mechanical->mechanicalFields["iTankIntHeight"].set(10000);

        device->mechanical->calc_Warnings();
    }

    void Designer::designDevice()
    {
        // Core design loop referencing the actual linked systems
        cost = 0.0;
        validDesign = true;

        if (!device || !device->core) return;

        [[maybe_unused]] int iNoPhases = device->deviceFields["iNoPhases"].get<int>();
        [[maybe_unused]] home_LaminationType tLamType = device->core->coreFields["tLaminationType"].get<home_LaminationType>();
        [[maybe_unused]] bool bAmorphous = (tLamType == home_LaminationType::Am || tLamType == home_LaminationType::AmHB1);

        device->core->calc_AreaCoreOpt();

        // Future hook for evaluateListCoils / chooseCoilsConfigurationLHL 
    }

    void Designer::calculateWarning()
    {
        warning = 0.0;
        double dCostE = cost / 2.0;

        if (!validDesign || !device || !device->core) return;

        // Directly tap into the actual Core subsystem parameters
        double dTotalStackE = device->core->coreFields["dTotalStack"].get<double>();
        double dSteelWidthE = device->core->coreFields["dSteelWidth"].get<double>();

        home_LaminationType tLamType = device->core->coreFields["tLaminationType"].get<home_LaminationType>();
        bool bAmorphous = (tLamType == home_LaminationType::Am || tLamType == home_LaminationType::AmHB1);

        double dLim = (bAmorphous ? 1.95 : 1.8);
        double ratio = (dTotalStackE > 0.0) ? (dSteelWidthE / dTotalStackE) : 0.0;

        if (ratio > dLim) {
            warning += (std::abs(dLim - ratio) / dLim) * dCostE;
        }
        else if (ratio < 1.3 && ratio > 0) {
            warning += (std::abs(1.3 - ratio) / 1.3) * dCostE;
        }
    }

    void Designer::calculatePenalty()
    {
        try {
            if (!validDesign || !device) return;

            penalty = 0.0;
            int numberPhases = device->deviceFields["iNoPhases"].get<int>();
            double dPenaltyAccumulator = 0.0;

            // Trigger the actual warnings inside the mapped subsystems
            device->core->calc_Warnings();
            device->losses->calc_Warnings();
            device->gradients->calc_Warnings();
            device->mechanical->calc_Warnings();

            // (Apply scaling rules as ported from the Python matrix)
            penalty = dPenaltyAccumulator * ((numberPhases != 1) ? 100.0 : 1.0);
        }
        catch (const std::exception& e) {
            std::cerr << "penalty error: " << e.what() << std::endl;
            penalty = std::numeric_limits<double>::infinity();
        }
    }

    void Designer::setPenalty(int iLevel)
    {
        int penal = 25 - iLevel;
        double dPenalBase = std::exp(static_cast<double>(penal));

        penalty = dPenalBase;
        validDesign = false;
        penaltyLevel = iLevel;
    }
}