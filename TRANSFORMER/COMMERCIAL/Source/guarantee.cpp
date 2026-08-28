/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Guarantee.cpp
|    @brief     Specialized evaluation engine managing electrical transformer performance guarantees.
|    @brief     Transformer::Commercial     is for "Business & Supply Chain"
|
|               Executes:
|                   - Initialization of baseline assessment metrics using localized home_FieldValues.
|                   - Calculates specified bounds for no-load losses, load losses, and excitation currents.
|                   - Formats performance data strings aligned to international specifications (ANSI/DOE).
|               Returns:
|                   - High-performance structure tracking contractual design compliance boundaries.
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "alias.h"
using namespace Transformer::Types;
#include "system_Config.h"
#include "definitions.h"

#include "guarantee_Warnings.h"
#include "device.h"
#include "losses.h"
#include "core.h"

#include "guarantee.h"


namespace Transformer::Commercial
{
    int Guarantee::id_Counter = 1;

    Guarantee::Guarantee(home_Device* parentDevice)      :   device(parentDevice), guaranteeWarnings(this)
    {
        guaranteeFields["id"]                    = home_FieldValue (Guarantee::id_Counter);
        guaranteeFields["name"]                  = home_FieldValue ("Guarantee " + std::to_string(Guarantee::id_Counter));
        guaranteeFields["dEvaluation_nl"]        = home_FieldValue (0.0);
        guaranteeFields["dEvaluation_ll"]        = home_FieldValue (0.0);
        guaranteeFields["sEvaluation_nl"]        = home_FieldValue ("");
        guaranteeFields["sEvaluation_ll"]        = home_FieldValue ("");
        guaranteeFields["sCustomerSpec"]         = home_FieldValue ("");
        guaranteeFields["tDesignSpec"]           = home_FieldValue (home_DesignSpecCodes::None);
        guaranteeFields["sDesignSpec"]           = home_FieldValue ("");
        guaranteeFields["tEfficiencySpec"]       = home_FieldValue (home_EfficiencySpec::None);
        guaranteeFields["bNLCorrected_to_85C"]   = home_FieldValue (false);
        guaranteeFields["dNL_pu"]                = home_FieldValue (1.00);
        guaranteeFields["dGroundShortTime"]      = home_FieldValue (0);
        guaranteeFields["dVirtualZ"]             = home_FieldValue (0.0);
        guaranteeFields["dNLGuarantee"]          = home_FieldValue (0.0);
        guaranteeFields["dLLGuarantee"]          = home_FieldValue (0.0);
        guaranteeFields["dIZGuarantee"]          = home_FieldValue (0.0);
        guaranteeFields["dExcGuarantee"]         = home_FieldValue (0.0);
        guaranteeFields["sNLGuarantee"]          = home_FieldValue ("");
        guaranteeFields["sLLGuarantee"]          = home_FieldValue ("");
        guaranteeFields["sTotLGuarantee"]        = home_FieldValue ("");
        guaranteeFields["sIZGuarantee"]          = home_FieldValue ("");
        guaranteeFields["sExcGuarantee"]         = home_FieldValue ("");
        guaranteeFields["bNLLimited"]            = home_FieldValue (false);
        guaranteeFields["bLLLimited"]            = home_FieldValue (false);
        guaranteeFields["bExcLimited"]           = home_FieldValue (false);
        guaranteeFields["sErrorName"]            = home_FieldValue (home_FieldValue::List (Config::ERRORMESSAGES, ""));
        guaranteeFields["tErrorEnum"]            = home_FieldValue (home_FieldValue::List (Config::ERRORMESSAGES, home_WarningSeverity::None));
        guaranteeFields["sErrorMessages"]        = home_FieldValue (home_FieldValue::List (Config::ERRORMESSAGES, ""));

        Guarantee::id_Counter++;
    }

    home_Device* Guarantee::getParentDevice() const
    {
        if (device == nullptr)    {        throw std::runtime_error("This submodule is not attached to any Device");    }
        return device;
    }
    void Guarantee::reset_Fields()
    {
        for (auto& pair : guaranteeFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }
    void Guarantee::erase_Warnings()
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i)
        {
            guaranteeFields["sErrorName"]    .set("", i);
            guaranteeFields["tErrorEnum"]    .set(home_WarningSeverity::None, i);
            guaranteeFields["sErrorMessages"].set("", i);
        }
    }
    void Guarantee::calc_Warnings()
    {
        std::vector <double> emptyValues = {0.0, 0.0};

        this->guaranteeWarnings.warning_MaxImpedance (emptyValues);
        this->guaranteeWarnings.warning_Impedance    (emptyValues);
    }
    void Guarantee::calc_Guarantee()
    {
        auto& guaranteedFields = guaranteeFields;
        
        // Evaluation string layouts parsing formatting pipeline
        std::stringstream noLoad, loadLoss;

        noLoad << std::fixed << std::setprecision(2) << guaranteedFields["dEvaluation_nl"].get<double>() << " $/W";
        loadLoss << std::fixed << std::setprecision(2) << guaranteedFields["dEvaluation_ll"].get<double>() << " $/W";

        guaranteedFields["sEvaluation_nl"].set(noLoad.str());
        guaranteedFields["sEvaluation_ll"].set(loadLoss.str());

        auto designSpecValue = guaranteedFields["tDesignSpec"].get<home_DesignSpecStandards>();

        if (designSpecValue != home_DesignSpecStandards::None)
        {
            guaranteedFields["sDesignSpec"].set(std::string(to_string(designSpecValue)));
        }

        calc_LimitedGuarantees();
    }
    void Guarantee::calc_LimitedGuarantees()
    {
        auto&   guaranteedFields    = guaranteeFields;
        auto&   coreFields          = device->core->coreFields;
        auto&   lossesFields        = device->losses->lossFields;

        double noLoadLosses             = lossesFields  ["dNLLosses"].get <double>();
        double loadLosses               = lossesFields  ["dLLosses"] .get <double>();
        double currentExcitationFactor  = coreFields    ["d_IExc"]   .get <std::vector <double>>()[2]; //Nominal 100% excitation index pos

        bool limited_noLoadLoss = (guaranteedFields["bNLLimited"]   .get<bool>() == true);
        bool limited_LoadLoss   = (guaranteedFields["bLLLimited"]   .get<bool>() == true);
        bool limited_Exc        = (guaranteedFields["bExcLimited"]  .get<bool>() == true);

        guaranteedFields["dNLGuarantee"] .assign_Default (noLoadLosses            * Config::NO_LOAD_LOSS_FACTOR);
        guaranteedFields["dLLGuarantee"] .assign_Default (loadLosses              * Config::LOAD_LOSS_FACTOR);
        guaranteedFields["dExcGuarantee"].assign_Default (currentExcitationFactor * currentExcitationFactor);

        if (!limited_noLoadLoss)    {        guaranteedFields["dNLGuarantee"]  .reset();    }
        if (!limited_LoadLoss)      {        guaranteedFields["dLLGuarantee"]  .reset();    }
        if (!limited_Exc)           {        guaranteedFields["dExcGuarantee"] .reset();    }
    }
    void Guarantee::set_DefaultGuarantees()
    {
        calc_LimitedGuarantees();
        auto& guaranteedFields  = guaranteeFields;

        double noLoad               = guaranteedFields["dNLGuarantee"] .get<double>();
        double loadLoss             = guaranteedFields["dLLGuarantee"] .get<double>();
        double impedanceGuaranteed  = guaranteedFields["dIZGuarantee"] .get<double>();
        double dEx                  = guaranteedFields["dExcGuarantee"].get<double>();

        auto format_value = [](double value)            // Lambda utility mirroring python's dynamic float formatting checks
        {
            if (value == 0.0)               return std::string("-");

            std::stringstream sStream;
            sStream << std::fixed << std::setprecision(3) << value;

            return sStream.str();
        };

        guaranteedFields ["sNLGuarantee"]   .set (format_value(noLoad));
        guaranteedFields ["sLLGuarantee"]   .set (format_value(loadLoss));
        guaranteedFields ["sTotLGuarantee"] .set (noLoad != 0.0 ? format_value(noLoad + loadLoss) : "-");

        if (impedanceGuaranteed != 0.0) 
        {
            if (impedanceGuaranteed == 1.5)            { guaranteedFields ["sIZGuarantee"]   .set("Min 1.5%"); } 
            else
            {
                std::stringstream sStream;
                sStream << std::fixed << std::setprecision(3) << impedanceGuaranteed;
                guaranteedFields ["sIZGuarantee"].set(sStream.str());
            }
        }
        else         { guaranteedFields ["sIZGuarantee"].set("-"); }

        guaranteedFields ["sExcGuarantee"].set(format_value(dEx));
    }
}
// end of Guarantee class