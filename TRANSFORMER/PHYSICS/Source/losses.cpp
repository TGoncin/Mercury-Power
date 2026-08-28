/**___________________________________________________________________________________________________
|    @date      May 26, 2026
|    @author    Tom Goncin
|    @file      Losses.cpp
|    @brief     Specialized mathematical tracking engine evaluating transformer electrical resistance and losses.
|    @brief     Transformer::Physics    is for "Behavior & Calculations" 
|
|               Executes:
|                   - Instantiates and tracks baseline transformer winding resistance and core losses via FieldValues.
|                   - Computes sequential adjustments for resistivity across different thermal states.
|                   - Resolves magnetic leakage loss vectors across active transformer loads and stray matrices.
|               Returns:
|                   - A high-performance, decoupled structure tracking unified transformer efficiency loss models.
|___________________________________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include <iomanip>

#include "math_Utilities.h"
#include "definitions.h"

#include "losses.h"
#include "device.h"
#include "winding.h"
#include "coil.h"
#include "cooling.h"
#include "core.h"
#include "guarantee.h"




namespace Transformer::Commercial
{
    class MaterialConstants;
    class ResistiveMaterial;
    class ResistivityTable;
    class ResistanceTable; 
}



namespace Transformer::Physics
{
    int Losses::id_Counter = 1;

    Losses::Losses
    (
        home_Device*           parentDevice, 
        home_MaterialConstants materialConstants,
        home_ResistiveMaterial resistiveMaterial,
        home_ResistivityTable  table_Resistivity,
        home_ResistanceTable   table_Resistance )
    : 
        device                  (parentDevice), 
        table_MaterialConstants (materialConstants), 
        table_ResistiveMaterial (resistiveMaterial), 
        table_Resistivity       (table_Resistivity),
        table_Resistance        (table_Resistance),
        loss_Warnings           (loss_Warnings)
    {
        lossFields["dLVWOLeadsCurrentTempR"]    = FieldValue(0.0);
        lossFields["dLVLeadsCurrentTempR"]      = FieldValue(0.0);
        lossFields["dTotalLVCurrentTempR"]      = FieldValue(0.0);
        lossFields["dHVWOLeadsCurrentTempR"]    = FieldValue(0.0);
        lossFields["dHVLeadsCurrentTempR"]      = FieldValue(0.0);
        lossFields["dTotalHVCurrentTempR"]      = FieldValue(0.0);
        lossFields["dLVWOLeadsNewTempR"]        = FieldValue(0.0);
        lossFields["dLVLeadsNewTempR"]          = FieldValue(0.0);
        lossFields["dTotalLVNewTempR"]          = FieldValue(0.0);
        lossFields["dHVWOLeadsNewTempR"]        = FieldValue(0.0);
        lossFields["dHVLeadsNewTempR"]          = FieldValue(0.0);
        lossFields["dTotalHVNewTempR"]          = FieldValue(0.0);
        lossFields["dLVWOLeadsCurrentTempW"]    = FieldValue(0.0);
        lossFields["dLVLeadsCurrentTempW"]      = FieldValue(0.0);
        lossFields["dTotalLVCurrentTempW"]      = FieldValue(0.0);
        lossFields["dHVWOLeadsCurrentTempW"]    = FieldValue(0.0);
        lossFields["dHVLeadsCurrentTempW"]      = FieldValue(0.0);
        lossFields["dTotalHVCurrentTempW"]      = FieldValue(0.0);
        lossFields["dLVWOLeadsNewTempW"]        = FieldValue(0.0);
        lossFields["dLVLeadsNewTempW"]          = FieldValue(0.0);
        lossFields["dTotalLVNewTempW"]          = FieldValue(0.0);
        lossFields["dHVWOLeadsNewTempW"]        = FieldValue(0.0);
        lossFields["hvLeadsNewTempW"]          = FieldValue(0.0);
        lossFields["dTotalHVNewTempW"]          = FieldValue(0.0);
        lossFields["dLVWOLeadsCurrentTempTaps"] = FieldValue(0.0);
        lossFields["dLVLeadsCurrentTempTaps"]   = FieldValue(0.0);
        lossFields["dTotalLVCurrentTempTaps"]   = FieldValue(0.0);
        lossFields["dHVWOLeadsCurrentTempTaps"] = FieldValue(0.0);
        lossFields["dHVLeadsCurrentTempTaps"]   = FieldValue(0.0);
        lossFields["dTotalHVCurrentTempTaps"]   = FieldValue(0.0);
        lossFields["dLVWOLeadsNewTempTaps"]     = FieldValue(0.0);
        lossFields["dLVLeadsNewTempTaps"]       = FieldValue(0.0);
        lossFields["dTotalLVNewTempTaps"]       = FieldValue(0.0);
        lossFields["dHVWOLeadsNewTempTaps"]     = FieldValue(0.0);
        lossFields["dHVLeadsNewTempTaps"] = FieldValue(0.0);
        lossFields["dTotalHVNewTempTaps"] = FieldValue(0.0);
        lossFields["dI2RCurrentTemp"]     = FieldValue(0.0);
        lossFields["dI2RNewTemp"]         = FieldValue(0.0);
        lossFields["dI2RCurrentTempTaps"] = FieldValue(0.0);
        lossFields["dI2RNewTempTaps"]     = FieldValue(0.0);
        lossFields["dLVLeadLoss"]         = FieldValue(0.0);
        lossFields["dTankStray"]          = FieldValue(0.0);
        lossFields["dCoilSray"]           = FieldValue(0.0);
        lossFields["totalStray"]         = FieldValue(0.0);
        lossFields["dTotalLLossStrayLead"]= FieldValue(0.0);
        lossFields["dCoreLoss"]       = FieldValue(0.0);
        lossFields["dTotalLoss"]      = FieldValue(0.0);
    
        lossFields["dNoLoadLossCost"] = FieldValue(0.0);
        lossFields["dLoadLossCost"]   = FieldValue(0.0);
        lossFields["dTotalLossCost"]  = FieldValue(0.0);
        lossFields["sNoLoadLossCost"] = FieldValue("");
        lossFields["sLoadLossCost"]   = FieldValue("");
        lossFields["sTotalLossCost"]  = FieldValue("");
    
        lossFields["dMaxWatts"]       = FieldValue(0.0);
        lossFields["dMaxWattsK4"]     = FieldValue(0.0);
        lossFields["noLoadLossLosses"]       = FieldValue(0.0);
        lossFields["loadLossosses"]        = FieldValue(0.0);
        lossFields["iCoilLdMat"]            = FieldValue(0);
        lossFields["areaLvBus"]            = FieldValue(0.0);
        lossFields["dFactorLVBus"]          = FieldValue(0.0);
        lossFields["lengthInLvBus"]        = FieldValue(0.0);
        lossFields["dOhmsLVBus"]            = FieldValue(0.0);
        lossFields["dLossLVBus"]            = FieldValue(0.0);
        lossFields["leadLengthOutOfCoil"]  = FieldValue(0.0);
        lossFields["iBusLdMat"]             = FieldValue(0);
        lossFields["areaBusWork"]          = FieldValue(0.0);
        lossFields["dFactorBuswork"]        = FieldValue(0.0);
        lossFields["dOhmsBuswork"]          = FieldValue(0.0);
        lossFields["dLossBuswork"]          = FieldValue(0.0);
        lossFields["dTotLdLoss"]            = FieldValue(0.0);
        lossFields["dHVAmpsRegLds"]         = FieldValue(0.0);
        lossFields["dHVLineAmpsRegLds"]     = FieldValue(0.0);
        lossFields["iCoilLdMatHV"]          = FieldValue(0);
        lossFields["dAreaHVBus"]            = FieldValue(0.0);
        lossFields["dFactorHVBus"]          = FieldValue(0.0);
        lossFields["dLengthInHVBus"]        = FieldValue(0.0);
        lossFields["dOhmsHVBus"]            = FieldValue(0.0);
        lossFields["dLossHVBus"]            = FieldValue(0.0);
        lossFields["areaBusWorkHV"]        = FieldValue(0.0);
        lossFields["dFactorBusworkHV"]      = FieldValue(0.0);
        lossFields["dOhmsBusworkHV"]        = FieldValue(0.0);
        lossFields["dLossBusworkHV"]        = FieldValue(0.0);
        lossFields["dTotLdLossHV"] = FieldValue(0.0);
        lossFields["d_20LLwindingLv"] = FieldValue(0.0);
        lossFields["d_55LLwindingLv"] = FieldValue(0.0);
        lossFields["d_75LLwindingLv"] = FieldValue(0.0);
        lossFields["d_85LLwindingLv"] = FieldValue(0.0);
        lossFields["d_20LLwindingHv"] = FieldValue(0.0);
        lossFields["d_55LLwindingHv"] = FieldValue(0.0);
        lossFields["d_75LLwindingHv"] = FieldValue(0.0);
        lossFields["d_85LLwindingHv"] = FieldValue(0.0);
        lossFields["d20Total100Load"] = FieldValue(0.0);
        lossFields["d55Total100Load"] = FieldValue(0.0);
        lossFields["d75Total100Load"] = FieldValue(0.0);
        lossFields["d85Total100Load"] = FieldValue(0.0);
        lossFields["d20TotalLR25"] = FieldValue(0.0);
        lossFields["d55TotalLR25"] = FieldValue(0.0);
        lossFields["d75TotalLR25"] = FieldValue(0.0);
        lossFields["d85TotalLR25"] = FieldValue(0.0);
        lossFields["d20TotalLR50"] = FieldValue(0.0);
        lossFields["d55TotalLR50"] = FieldValue(0.0);
        lossFields["d75TotalLR50"] = FieldValue(0.0);
        lossFields["d85TotalLR50"] = FieldValue(0.0);
        lossFields["d20TotalLR75"] = FieldValue(0.0);
        lossFields["d55TotalLR75"] = FieldValue(0.0);
        lossFields["d75TotalLR75"] = FieldValue(0.0);
        lossFields["d85TotalLR75"] = FieldValue(0.0);
        lossFields["d20TotalLR125"] = FieldValue(0.0);
        lossFields["d55TotalLR125"] = FieldValue(0.0);
        lossFields["d75TotalLR125"] = FieldValue(0.0);
        lossFields["d85TotalLR125"] = FieldValue(0.0);
        lossFields["d20TotalLR133"] = FieldValue(0.0);
        lossFields["d55TotalLR133"] = FieldValue(0.0);
        lossFields["d75TotalLR133"] = FieldValue(0.0);
        lossFields["d85TotalLR133"] = FieldValue(0.0);
        lossFields["sErrorName"]        = FieldValue(std::vector<std::string>(ERRORMESSAGES, ""));
        lossFields["tErrorEnum"]        = FieldValue(std::vector<WarningSeverity>(ERRORMESSAGES, WarningSeverity::None));
        lossFields["sErrorMessages"]    = FieldValue(std::vector<std::string>(ERRORMESSAGES, ""));

        Losses::id_Counter++;
    }

    home_Device* Losses::getParentDevice() const
    {
        if (!device) throw std::runtime_error("This module is not attached to any Device");
        return device;
    }

    void Losses::reset_LossFields() 
    {
        for (auto& pair : lossFields) 
        {
            try 
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }

    void Losses::erase_Warnings()
    {
        for (int i = 0; i < ERRORMESSAGES; ++i)
        {
            lossFields["sErrorName"]      .set("", i);
            lossFields["tErrorEnum"]      .set(WarningSeverity::None, i);
            lossFields["sErrorMessages"]  .set("", i);
        }
    }

    void Losses::calc_Warnings()
    {
        std::vector<double> values = {0.0, 0.0};
        loss_Warnings.warning_ValueCoreLosses(values);
        loss_Warnings.warning_ValueLoadLosses(values);
    }

    void Losses::calc_Loss(home_Device* device, home_Winding* windingHv, home_Winding* windingLv)
    {
        home_Coil*      coilLv          = windingLv->coils[0];
        home_Coil*      coilHv          = windingHv->coils[0];
        home_Cooling*   cooling         = device->cooling.get();
        home_Core*      core            = device->core.get();
        home_Guarantee* guarantee       = device->guarantee.get();
        int             numberPhases    = lossFields["numberPhases"].get<int>();

        calc_TableResistivity   (coilHv, coilLv);
        calc_TableResistance    (device, windingHv, windingLv, coilHv, coilLv);
        calc_LeadsBus           (device, cooling, windingHv, windingLv, coilHv, coilLv);
        calc_ResistanceAndLoss  (device, cooling, windingHv, windingLv);
        calc_CoilStray          (device, windingLv, numberPhases);
        calc_TotalStray         (device);
        calc_TotalLoss          (device);
        calc_CoreLoss           (core, numberPhases);
        calc_TotalLoss          (core);
        calc_MaxWatts           (device, windingHv, cooling);
        calc_LossToEvaluate     (guarantee, this);
        calc_EvaluateLoss       (guarantee);
        calc_LoadLossWinding    (device, windingHv, windingLv);
    }

    void Losses::calc_TableResistivity(home_Coil* coilHv, home_Coil* coilLv)
    {
        home_Material materialAluminumLv = coilLv->coilFields["tMatAluminum"].get<home_Material>();
        home_Material materialAluminumHv = coilHv->coilFields["tMatAluminum"].get<home_Material>();
        
        auto& resistiveMaterial = table_ResistiveMaterial.tableData;
        auto& resistivity       = table_Resistivity.tableData;

        double result = (materialAluminumLv == Material::Aluminum) ? resistiveMaterial[4][1] : resistiveMaterial[4][0];
        resistivity[0][1]  = result;

        double alpha  = (materialAluminumLv == Material::Aluminum) ? resistiveMaterial[0][1] : resistiveMaterial[0][0];

        resistivity[1][1]  = (1.0 + alpha * (55.0 - 20.0)) * resistivity[0][1];
        resistivity[2][1]  = (1.0 + alpha * (75.0 - 20.0)) * resistivity[0][1];
        resistivity[3][1]  = (1.0 + alpha * (85.0 - 20.0)) * resistivity[0][1];

        result = (materialAluminumHv == Material::Aluminum) ? resistiveMaterial[4][1] : resistiveMaterial[4][0];
        resistivity[0][2] = result;

        alpha = (materialAluminumHv == Material::Aluminum) ? resistiveMaterial[0][1] : resistiveMaterial[0][0];
        resistivity[1][2] = (1.0 + alpha * (55.0 - 20.0)) * resistivity[0][2];
        resistivity[2][2] = (1.0 + alpha * (75.0 - 20.0)) * resistivity[0][2];
        resistivity[3][2] = (1.0 + alpha * (85.0 - 20.0)) * resistivity[0][2];
    }

    void Losses::calc_TableResistance(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, home_Coil* coilHv, home_Coil* coilLv)
    {
        home_Device* losses = getParentDevice();
        auto&   lossField   = lossFields;

        int     numberPhases    = device->deviceFields ["numberPhases"]  .get<int>();
        double  lv_MT           = device->deviceFields ["dLV_MT"]        .get<double>();
        double  ilv_MT          = device->deviceFields ["dILV_MT"]       .get<double>();
        double AreaCondLv       = coilLv->coilFields["dAreaCond"]        .get<double>();
        double factResistance   = device->deviceFields["dResistance"]    .get<double>();
        double nominalTurnsUiLv = windingLv->windingFields["dNomTurnsUI"].get<double>();
        double areaLvBus        = lossField["areaLvBus"]                 .get<double>();
        double lengthInLvBus    = lossField["lengthInLvBus"]             .get<double>();
        
        auto& resistivityData    = table_Resistivity.tableData;
        auto& resistanceData     = table_Resistance.tableData;

        double mtFactor    = (numberPhases == 1) ? lv_MT : ilv_MT;

        for (int i = 0; i < 4; ++i)
        {
            resistanceData[i][1] = ((mtFactor / 12.0 * nominalTurnsUiLv * resistivityData[i][1] / AreaCondLv) * (1.0 + factResistance / 100.0));
        }

        for (int i = 0; i < 4; ++i)
        {
            resistanceData[i][2] = 2.0 * (resistivityData[i][1] * safe_Division((lengthInLvBus / 12.0), areaLvBus)) * (1.0 + factResistance / 100.0);
        }
    }

    void Losses::calc_LeadsBus (home_Device* device, home_Cooling* cooling, home_Winding* winding_Hv, home_Winding* winding_Lv,  home_Coil* coil_HV,  home_Coil* coil_LV){}

    double Losses::func_TempFactor(double temp, double materialConstants)
    {
        return (materialConstants + 20.0) / (materialConstants + temp);
    }

    void Losses::calc_ResistanceAndLoss(home_Device* device, home_Cooling* cooling, home_Winding* windingHv, home_Winding* windingLv) {}

    void Losses::calc_CoilStray(home_Device* device, home_Winding* windingLv, int numberPhases)
    {
        double  iXCoil      = device->deviceFields["dIXCoil"].get<double>();
        double  nominalKva  = windingLv->windingFields["d_kVA"].get<std::vector<double>>()[0];
        double  ampLV       = windingLv->windingFields["pdPhaseAmp"].get<std::vector<double>>()[windingLv->get_NominalPosition()];
        double result       = ((numberPhases == 1 ? 15.0 : 12.0) * nominalKva * iXCoil / 100.0) + (15.0 * 0.001 * ampLV * (std::pow(nominalKva, 0.4)));

        lossFields["dCoilSray"].set(result);
    }

    void Losses::calc_TotalStray(home_Device* device)
    {
        auto& lossesField = lossFields;

        double frequency   = lossesField["frequency"].get<double>();
        double coilSray    = lossesField["dCoilSray"].get<double>();
        double tankStray   = lossesField["dTankStray"].get<double>();
        
        double result     = (frequency == 60.0) ? (coilSray + tankStray) : 0.83 * (coilSray + tankStray);

        lossesField["totalStray"].set(result);
    }

    void Losses::calc_TotalLoss(home_Device* device)
    {
        auto& lossField = lossFields;

        double frequencyE       = lossFields["frequency"].get<double>();
        double dLoadLossFactorE = lossFields["dLoadLossFactor"].get<double>();
        double result           = (lossField["dTotalLVCurrentTempW"].get<double>() + lossField["dTotalHVCurrentTempW"].get<double>() + lossField["totalStray"].get<double>() * frequencyE / 60.0) * (dLoadLossFactorE / 100.0 + 1.0);

        lossField["dTotalLLossStrayLead"].set(result);
    }

    void Losses::calc_CoreLoss(home_Core* core, int numberPhases)
    {
        auto coreField = core->coreFields;
        LaminationType laminationType = coreField["tLaminationType"].get<LaminationType>();

        double weightTwoLoops  = coreField["dWeightTwoLoops"].get<double>();
        double fluxDensityVolt = coreField["dFluxDensityVolt"].get<double>();
        double NLL             = coreField["noLoadLossL"].get<double>();
        double wattsLb         = coreField["dWattsLb"].get<double>();
        double result          = 0.0;

        if      (laminationType == LaminationType::Am)      {   result = (fluxDensityVolt < (numberPhases == 1 ? 87.16   : 87.32))   ? weightTwoLoops * wattsLb : 0.0;   }
        else if (laminationType == LaminationType::AmHB1)   {   result = (fluxDensityVolt < (numberPhases == 1 ? 91.6578 : 91.6579)) ? ((numberPhases == 1) ? weightTwoLoops * wattsLb : NLL) : 0.0;    }
        else                                                {   result = (fluxDensityVolt < (numberPhases == 1 ? 113.0   : 116.0))   ? weightTwoLoops * wattsLb : 0.0;  }

        lossFields["dCoreLoss"].set(result);
    }

    void Losses::calc_TotalLoss(home_Core* core)
    {
        auto& lossField         = lossFields;
        double fluxDensityRatPer= core->coreFields["dFluxDensityRatPer"].get<double>();
        double result           = (fluxDensityRatPer < 113.0) ? lossField["dCoreLoss"].get<double>() + lossField["dTotalLLossStrayLead"].get<double>() : 0.0;

        lossField["dTotalLoss"].set(result);
    }

    void Losses::calc_MaxWatts(home_Device* device, home_Winding* windingHv, home_Cooling* cooling)
    {
        auto&  lossField   = lossFields;
        double LossesONAN  = device->deviceFields["dONANLosses"].get<double>();
        double totalLoss   = lossField["dTotalLoss"].get<double>();
        double result      = 0.0;

        if (LossesONAN > 0.0)         result = LossesONAN;
        else
        {
            if (windingHv->have_Taps() && windingHv->changer.changerFields["tCapacity"].get<ChangerCapacity>() == ChangerCapacity::FCBN)
            {
                result = std::round((totalLoss + (lossField["dI2RCurrentTempTaps"].get<double>() - lossField["dI2RCurrentTemp"].get<double>() * 1.0)));
            }
            else      result = std::round(totalLoss * 1.0);
        }

        lossField["dMaxWatts"].set(result);

        if (cooling->coolingFields["tK4Factor"].get<bool>() == true)
        {
            double coilSray            = lossField["dCoilSray"].get<double>();
            double tankStray           = lossField["dTankStray"].get<double>();
            double coreLoss            = lossField["dCoreLoss"].get<double>();
            double totalLvCurrentTempW = lossField["dTotalLVCurrentTempW"].get<double>();
            double totalHvCurrentTempW = lossField["dTotalHVCurrentTempW"].get<double>();
            
            result  = std::round(totalLvCurrentTempW + totalHvCurrentTempW + (4.0 * coilSray) + tankStray + coreLoss);

            lossField["dMaxWattsK4"].set(result);
        } 
        else         lossField["dMaxWattsK4"].reset();
    }

    void Losses::calc_LossToEvaluate(home_Guarantee* guarantee, home_Losses* losses)
    {
        auto& lossField     = lossFields;
        auto guaranteeMap   = guarantee->guaranteeFields; 
        auto lossMap        = losses->lossFields;

        double noLoadLoss   = guaranteeMap["noLoadLossGuarantee"].get<double>();
        double loadLoss     = guaranteeMap["loadLossGuarantee"].get<double>();

        lossField["noLoadLossLosses"].set((noLoadLoss > 0.0 && loadLoss > 0.0) ? noLoadLoss : lossMap["dCoreLoss"]           .get<double>());
        lossField["loadLossosses"]   .set((noLoadLoss > 0.0 && loadLoss > 0.0) ? loadLoss :   lossMap["dTotalLLossStrayLead"].get<double>());
    }

    void Losses::calc_EvaluateLoss(home_Guarantee* guarantee)
    {
        auto& lossField       = lossFields;
        auto  guaranteeField  = guarantee->guaranteeFields;
    
        double evaluationNL = guaranteeField["dEvaluation_nl"].get<double>();
        double evaluationLL = guaranteeField["dEvaluation_ll"].get<double>();

        double costNL = lossField["dCoreLoss"]  .get<double>() * evaluationNL;
        double costLL = lossField["dTotalLoss"] .get<double>() * evaluationLL;
        
        lossField["dNoLoadLossCost"].set(costNL);
        lossField["dLoadLossCost"]  .set(costLL);
        lossField["dTotalLossCost"] .set(costNL + costLL);

        std::stringstream sStreamNL, sStreamLL, sStreamTotal;
        sStreamNL   << "$"          << std::fixed << std::setprecision(0) << costNL;
        sStreamLL   << "$"          << std::fixed << std::setprecision(0) << costLL;
        sStreamTotal<< "Total: $"   << std::fixed << std::setprecision(0) << (costNL + costLL);

        lossField["sNoLoadLossCost"].set(sStreamNL.str());
        lossField["sLoadLossCost"]  .set(sStreamLL.str());
        lossField["sTotalLossCost"] .set(sStreamTotal.str());
    }

    void Losses::calc_LoadLossWinding(home_Device* device, home_Winding* winding_Hv, home_Winding* winding_Lv)
    {
        auto&   lossField   = lossFields;
        auto    deviceMap   = device->deviceFields;
        auto    windingLvMap= winding_Lv->windingFields;
        auto    windingHvMap= winding_Hv->windingFields;

        int     nominalPosition = winding_Lv->get_NominalPosition();;
        double  frequency       = deviceMap["frequency"].get<double>();
        int     numberPhases    = deviceMap["numberPhases"].get<int>();

        double areaBusWork         = lossField["areaBusWork"].get<double>();
        double leadLengthOutOfCoil = (numberPhases == 1) ? lossField["leadLengthOutOfCoil"].get<double>() : deviceMap["dBusTotalLength"].get<double>();
        double areaLvBus            = lossField["areaLvBus"].get<double>();
        double lengthInLvBus        = lossField["lengthInLvBus"].get<double>();
        double factResistance       = deviceMap["dResistance"].get<double>();
        double dkVANom              = deviceMap["nominal_kva"].get<double>();

        double operatingAmpsLv  = windingLvMap["pdPhaseAmp"].get<std::vector<double>>()[nominalPosition];
        double lineAmpsLv       = windingLvMap["pdLineAmp"].get<std::vector<double>>()[nominalPosition];
        double operatingAmpsHv  = windingHvMap["pdPhaseAmp"].get<std::vector<double>>()[winding_Hv->get_NominalPosition()];

        double factor      = (dkVANom > 650.0) ? 1.25 : 1.0;
        auto&  resistance  = table_Resistance.tableData;
        auto&  resistivity = table_Resistivity.tableData;

        double lastFact = (numberPhases == 1) ? 4.0 : 2.0;

        // sequence matrix processing multiple thermal states indexes mapping safely
        for (int i = 0; i < 4; ++i)\
        {
            double term1 = (operatingAmpsLv * operatingAmpsLv) * resistivity[i][1] * numberPhases;
            double term2 = factor * ( ( resistivity[i][1] * safe_Division((leadLengthOutOfCoil / 12.0), areaBusWork) ) 
                                    * ( 1.0 + factResistance / 100.0 ) 
                                    * ( lineAmpsLv * lineAmpsLv )
                                    + ( resistivity[i][1] * safe_Division((lengthInLvBus / 12.0), areaLvBus)) 
                                    * ( 1.0 + factResistance / 100.0) 
                                    * (operatingAmpsLv * operatingAmpsLv) 
                                    * numberPhases 
                                    * lastFact
                                    );
            
            if      (i == 0) lossField["d_20LLwindingLv"].set(term1 + term2);
            else if (i == 1) lossField["d_55LLwindingLv"].set(term1 + term2);
            else if (i == 2) lossField["d_75LLwindingLv"].set(term1 + term2);
            else if (i == 3) lossField["d_85LLwindingLv"].set(term1 + term2);
        }

        double hvLeadsNewTempW  = lossField["hvLeadsNewTempW"].get<double>();
        double material         = table_MaterialConstants.tableData[2][0];

// TOTAL HV CALCULATION STATE TRACKING

        lastFact = (numberPhases == 1) ? 0.0 : hvLeadsNewTempW;
        lossField["d_20LLwindingHv"].set((operatingAmpsHv * operatingAmpsHv) * resistivity[0][4] * numberPhases + lastFact);

        lastFact = (numberPhases == 1) ? 0.0 : hvLeadsNewTempW / (material + 20.0) * (material + 55.0);
        lossField["d_55LLwindingHv"].set((operatingAmpsHv * operatingAmpsHv) * resistivity[1][4] * numberPhases + lastFact);

        lastFact = (numberPhases == 1) ? 0.0 : hvLeadsNewTempW / (material + 20.0) * (material + 75.0);
        lossField["d_75LLwindingHv"].set((operatingAmpsHv * operatingAmpsHv) * resistivity[2][4] * numberPhases + lastFact);

        lastFact = (numberPhases == 1) ? 0.0 : hvLeadsNewTempW / (material + 20.0) * (material + 80.0);
        lossField["d_85LLwindingHv"].set((operatingAmpsHv * operatingAmpsHv) * resistivity[3][4] * numberPhases + lastFact);

// TOTAL 100% LOAD CALCULATIONS

        double totalStray   = lossField["totalStray"].get<double>();

        lossField["d20Total100Load"].set((lossField["d_20LLwindingLv"].get<double>() + lossField["d_20LLwindingHv"].get<double>() + (totalStray * frequency / 60.0)) * LOAD_LOSS_FACTOR);
        lossField["d55Total100Load"].set((lossField["d_55LLwindingLv"].get<double>() + lossField["d_55LLwindingHv"].get<double>() + (totalStray * frequency / 60.0)) * LOAD_LOSS_FACTOR);
        lossField["d75Total100Load"].set((lossField["d_75LLwindingLv"].get<double>() + lossField["d_75LLwindingHv"].get<double>() + (totalStray * frequency / 60.0)) * LOAD_LOSS_FACTOR);
        lossField["d85Total100Load"].set((lossField["d_85LLwindingLv"].get<double>() + lossField["d_85LLwindingHv"].get<double>() + (totalStray * frequency / 60.0)) * LOAD_LOSS_FACTOR);

// DYNAMIC MULTI-TIER LOAD SCALING FACTOR MAPPING

        std::vector<double>         steps    = {0.25, 0.50, 0.75, 1.25, 1.33};
        std::vector<std::string>    prefixes = {"25", "50", "75", "125", "133"};

        for (size_t s = 0; s < steps.size(); ++s)
        {
            double por  = steps[s] * steps[s];

            lossField["d20TotalLR" + prefixes[s]].set(lossField["d20Total100Load"].get<double>() * por);
            lossField["d55TotalLR" + prefixes[s]].set(lossField["d55Total100Load"].get<double>() * por);
            lossField["d75TotalLR" + prefixes[s]].set(lossField["d75Total100Load"].get<double>() * por);
            lossField["d85TotalLR" + prefixes[s]].set(lossField["d85Total100Load"].get<double>() * por);
        }
    }

}
// end of Losses class