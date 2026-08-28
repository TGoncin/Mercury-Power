/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Core.cpp
|    @brief     Specializing geometric, electromagnetic, & weight optimization.
|    @brief     Transformer::Components     is "Physical Hardware"
|___________________________________________________________________________________________________*/
/*
|                                    ________________________________________________
|                                    |                                               |
|                                    |                H E A D E R S                  |
|                                    |_______________________________________________|
*/
#define _USE_MATH_DEFINES 
//                                          Standard C++ libraries
#include <iostream>
#include <sstream>
#include <iomanip>
//                                          Alias file for compact type name
#include "alias.h"
using namespace Transformer::Types;
//                                          Extrinsic functions files
#include "enums.h"
#include "definitions.h"
#include "system_Config.h"
#include "table_Search.h"
#include "math_Utilities.h"
#include "conversions.h"
//                                          Forward declarations for extrinsic classes used in this file
#include "field_Value.h"
#include "device.h"
#include "winding.h"
#include "coil.h"
#include "guarantee.h"
#include "core_Warnings.h"
//                                          Intrinsic class file
#include "core.h"



namespace Transformer::Components
{
    int Core::id_Counter = 1;

    Core::Core (home_Device* device, home_MaterialConstants materialConstants) 
        : device(device), table_MaterialConstants(materialConstants), core_Warnings(this) 
    {
        coreFields["id"]                      = home_FieldValue (Core::id_Counter);
        coreFields["name"]                    = home_FieldValue ("Core " + std::to_string(Core::id_Counter));
        coreFields["laminationType"]          = home_FieldValue (static_cast<int>(home_LaminationType::M0H));
        coreFields["totalStack"]              = home_FieldValue (0.00);
        coreFields["dSteelWidth"]             = home_FieldValue (0.00);
        coreFields["dPerOvervoltageMaxFlux"]  = home_FieldValue (0.00);
        coreFields["dTestInductionLevel"]     = home_FieldValue (0.00);
        coreFields["dMaxWattsPound"]          = home_FieldValue (0.00);
        coreFields["lossFactor"]              = home_FieldValue (0.00);
        coreFields["sDrawing"]                = home_FieldValue ("");

        std::map<std::string, std::vector<home_FieldValue::Value>> steelWidthMap;

        steelWidthMap["TypeA"] =
        {
            5.6,    6.7,    8.4, 
            11.2,   12.3,   13.4, 
            14.0,   15.1,   16.8
        };

        steelWidthMap["TypeB"] =
        {
            2.0,    3.0,    4.0, 
            5.0,    6.0,    7.0, 
            8.0,    9.0,    10.0, 
            11.0,   12.0,   13.0, 
            14.0,   15.0,   16.0
        };

        steelWidthMap["TypeA3"] =
        {
            5.6,    6.7,    7.6, 
            8.4,    11.2,   12.3, 
            13.2,   13.4,   14.0, 
            14.3,   15.1,   15.2, 
            16.0,   16.8
        };
/*          _______________________________________________
|          |                                               |
|          |                W R A P P E R                  |
|          |                   L O G I C                   |
|          |_______________________________________________|
*/
        for (const auto& [key, vec] : steelWidthMap)        {   coreFields["d_SteelWidthMap_" + key] = home_FieldValue(home_FieldValue::List(vec.begin(), vec.end())); }


        coreFields["dInductionLevel"]         = home_FieldValue (0.00);
        coreFields["tIsolatedCore"]           = home_FieldValue (false);         
        coreFields["dFluxDensityIni"]         = home_FieldValue (0.00);
        coreFields["fluxDensityLimitMax"]     = home_FieldValue (0.00);
        coreFields["iTotalStack"]             = home_FieldValue (0);
        coreFields["noLoops"]                 = home_FieldValue (1);
        coreFields["sRatedPU"]                = home_FieldValue ("");
        coreFields["sRatedV"]                 = home_FieldValue ("");
        coreFields["dInitialFluxDensity"]     = home_FieldValue (16500.0);
        coreFields["dFluxDensityRatPer"]      = home_FieldValue (0.0);
        coreFields["dFluxDensityVolt"]        = home_FieldValue (0.0);
        coreFields["dFluxDensityRatPerT"]     = home_FieldValue (0.0);
        coreFields["dFluxDensityVoltT"]       = home_FieldValue (0.0);
        coreFields["bLoop1AmCoreStrip"]       = home_FieldValue (0.0);
        coreFields["loop2AmCoreStrip"]        = home_FieldValue (0.0);
        coreFields["dWindowHeight"]           = home_FieldValue (0.00);
        coreFields["dSmallWindowWidth"]       = home_FieldValue (0.00);
        coreFields["dLargeWindowWidth"]       = home_FieldValue (0.00);
        coreFields["dStackPerLoop"]           = home_FieldValue (0.00);
        coreFields["dStripWidth"]             = home_FieldValue (0.00);
        coreFields["dEBottom"]                = home_FieldValue (0.00);
        coreFields["dStackCoreSmLoops"]       = home_FieldValue (0.00);
        coreFields["dStackCoreLgLoops"]       = home_FieldValue (0.00);
        coreFields["dOverallCoreHeight"]      = home_FieldValue (0.00);
        coreFields["dOverallCoreHeightInner"] = home_FieldValue (0.00);
        coreFields["dCoreLengthWithoutPB"]    = home_FieldValue (0.00);
        coreFields["dCoreLengthWithPB"]       = home_FieldValue (0.00);
        coreFields["dArea"]                   = home_FieldValue (0.00);
        coreFields["sArea"]                   = home_FieldValue ("");
        coreFields["dFormNose"]               = home_FieldValue (0.00);
        coreFields["dFormSide"]               = home_FieldValue (0.00);
        coreFields["sForms"]                  = home_FieldValue ("");
        coreFields["dTrueOverCoil"]           = home_FieldValue (0.00);
        coreFields["dLegCenters"]             = home_FieldValue (0.00);
        coreFields["dWeightOneSmLoop"]        = home_FieldValue (0.00);
        coreFields["dWeightOneLgLoop"]        = home_FieldValue (0.00);
        coreFields["dWeightOneLoop"]          = home_FieldValue (0.00);
        coreFields["dWeightOneSmLoopOther"]   = home_FieldValue (0.00);
        coreFields["dWeightOneLgLoopOther"]   = home_FieldValue (0.00);
        coreFields["dWeightOneLoopOther"]     = home_FieldValue (0.00);
        coreFields["dWeightTwoSmLoops"]       = home_FieldValue (0.00);
        coreFields["dWeightTwoLgLoops"]       = home_FieldValue (0.00);
        coreFields["dWeightTwoLoops"]         = home_FieldValue (0.00);
        coreFields["dWeights1_8"]             = home_FieldValue (0.00);
        coreFields["dWeights1_16"]            = home_FieldValue (0.00);
        coreFields["dWeightsPB1_8"]           = home_FieldValue (0.00);
        coreFields["dWeightsPB1_16"]          = home_FieldValue (0.00);
        coreFields["dWeightsPBHD_1_4"]        = home_FieldValue (0.00);
        coreFields["dWeightPressBoard"]       = home_FieldValue (0.00);
        coreFields["dPh_LimbClearance"]       = home_FieldValue (0.00);
        coreFields["dPh_PhClearance"]         = home_FieldValue (0.00);
        coreFields["dLamFactor"]              = home_FieldValue (0.89);
        coreFields["dStackTolPIn"]            = home_FieldValue (0.00);
        coreFields["dStackTolMIn"]            = home_FieldValue (0.0625);
        coreFields["dNLL"]                    = home_FieldValue (0.00);
        coreFields["dFluxDens100V"]           = home_FieldValue (0.00);
        coreFields["dFormDimA"]               = home_FieldValue (0.00);
        coreFields["dFormDimS"]               = home_FieldValue (0.00);
        coreFields["dFormDimN"]               = home_FieldValue (0.00);
        coreFields["dFormDimW"]               = home_FieldValue (0.00);
        coreFields["dTotalFormLength"]        = home_FieldValue (0.00);
        coreFields["destructionFactorAm"]     = home_FieldValue (0.00);
        coreFields["destructionFactorReg"]    = home_FieldValue (0.00);
        coreFields["d_NLLossDataFre"]         = home_FieldValue (home_FieldValue::Value( (6, 0.00)) );
        coreFields["dCommonFact"]             = home_FieldValue (0.00);
        coreFields["d_CoreTolerance"]         = home_FieldValue (home_FieldValue::Value( (4, ""  )) );
        coreFields["dWattsLb"]                = home_FieldValue (0.00);
        coreFields["dWidthClampIns"]          = home_FieldValue (0.00);
        coreFields["dHeightClampIns"]         = home_FieldValue (0.00);
        coreFields["dWeightClampIns"]         = home_FieldValue (0.00);
        coreFields["sSheetComments"]          = home_FieldValue ("");
        coreFields["dWindingHeightAdder"]     = home_FieldValue (0.00);
        coreFields["dWindingWidthAdder"]      = home_FieldValue (0.00);
        coreFields["dCoilCorePBEnd"]          = home_FieldValue (0.00);
        coreFields["dCoilCoilPBSides"]        = home_FieldValue (0.00);
        coreFields["dCoilCorePBSides"]        = home_FieldValue (0.00);
        coreFields["dFixedSmWindowWidth"]     = home_FieldValue (0.00);
        coreFields["dFixedWindowHeight"]      = home_FieldValue (0.00);
        coreFields["dFixedWindowLarge"]       = home_FieldValue (0.00);
        coreFields["dOverCoilAdder"]          = home_FieldValue (0.00);
        coreFields["d_ThickDM"]               = home_FieldValue (home_FieldValue::Value( (2, 0.00)) );
        coreFields["s_LocationDM"]            = home_FieldValue (home_FieldValue::Value( (2, ""  )) );
        coreFields["d_WidthDM"]               = home_FieldValue (home_FieldValue::Value( (2, 0.00)) );
        coreFields["d_RollLengthDM"]          = home_FieldValue (home_FieldValue::Value( (2, 0.00)) );
        coreFields["d_8FtLengthsDM"]          = home_FieldValue (home_FieldValue::Value( (2, 0.00)) );
        coreFields["d_DuctsDM"]               = home_FieldValue (home_FieldValue::Value( (2, 0.00)) );
        coreFields["sDuctMaterial"]           = home_FieldValue (home_FieldValue::Value( (2, ""  )) );
        coreFields["d_IExc"]                  = home_FieldValue (home_FieldValue::Value( (7, 0.00)) );
        coreFields["d_NoLoadLosses20"]        = home_FieldValue (home_FieldValue::Value( (7, 0.00)) );
        coreFields["d_NoLoadLosses75"]        = home_FieldValue (home_FieldValue::Value( (7, 0.00)) );
        coreFields["d_NoLoadLosses85"]        = home_FieldValue (home_FieldValue::Value( (7, 0.00)) );
        coreFields["sCoreSuffix"]             = home_FieldValue ("");
        coreFields["sErrorName"]              = home_FieldValue (home_FieldValue::Value( (Config::ERRORMESSAGES, "")) );
        coreFields["tErrorEnum"]              = home_FieldValue (home_FieldValue::Value( (Config::ERRORMESSAGES, static_cast<int> (home_WarningSeverity::None))) );
        coreFields["sErrorMessages"]          = home_FieldValue (home_FieldValue::Value( (Config::ERRORMESSAGES, "")) );

        pressBoardPacking                     = std::vector<std::vector<double>>( 11, std::vector<double>(8, 0.00) );

        Core::id_Counter++;
    }

    home_LaminationType Core::type_Lamination() const       
    {    
        return coreFields.at("laminationType").get<home_LaminationType>();   
    }

    home_Device* Core::getParentDevice() const
    {
        if (!device)        
            throw std::runtime_error("This core is not attached to any device");
        return device;
    }

    int Core::get_LaminationNumber() const 
    {
        auto laminationType = coreFields.at("laminationType").get<home_LaminationType>();

        if (laminationType == home_LaminationType::Am)     return static_cast<int>(home_LaminationType::Am);
        if (laminationType == home_LaminationType::AmHB1)  return static_cast<int>(home_LaminationType::AmHB1);
        if (laminationType == home_LaminationType::M0H)    return static_cast<int>(home_LaminationType::M0H);
        if (laminationType == home_LaminationType::M2)     return static_cast<int>(home_LaminationType::M2);
        if (laminationType == home_LaminationType::M3)     return static_cast<int>(home_LaminationType::M3);
        if (laminationType == home_LaminationType::M4)     return static_cast<int>(home_LaminationType::M4);

        return static_cast<int>(home_LaminationType::None);
    }

    double Core::calc_FactAmHB1(double fluxDensityVoltTAux, home_Device* device) 
    {
        if (fluxDensityVoltTAux > 1.63)         { return 0.0; }

        int     numberPhases    = device->deviceFields["numberPhases"].get<int>();
        double  vacFactAm       = device->deviceFields["amorphousLossVariance"].get<double>();
        double  factFreq        = device->deviceFields["freqFactorHz"].get<double>();
        double  coreLossFactor  = device->deviceFields["coreLossFactor"].get<double>();

        if (numberPhases == 1) 
        {
            double dAmAssemblyInducedStress = device->deviceFields["dAmAssemblyInducedStress"].get<double>();

            return ((7.4162 * std::pow(fluxDensityVoltTAux, 6) 
                   - 55.712 * std::pow(fluxDensityVoltTAux, 5) 
                   + 174.24 * std::pow(fluxDensityVoltTAux, 4) 
                   - 290.35 * std::pow(fluxDensityVoltTAux, 3) 
                   + 271.9  * std::pow(fluxDensityVoltTAux, 2) 
                   - 135.56 * fluxDensityVoltTAux + 28.126
                   ) 
                   / 1.02 * (1.0 + vacFactAm / 100.0) 
                   * (1.0 + dAmAssemblyInducedStress / 100.0) 
                   * factFreq * (1.0 + coreLossFactor / 100.0)
                  );
        } 
        else 
        {
            double destructionFactor = device->deviceFields["dDestructionFactor"].get<double>();

            return ((3.0983 * std::pow(fluxDensityVoltTAux, 6) 
                   - 23.862 * std::pow(fluxDensityVoltTAux, 5) 
                   + 76.516 * std::pow(fluxDensityVoltTAux, 4) 
                   - 130.51 * std::pow(fluxDensityVoltTAux, 3) 
                   + 124.76 * std::pow(fluxDensityVoltTAux, 2) 
                   - 63.218 * fluxDensityVoltTAux + 13.272
                   ) 
                   * (1.0 + vacFactAm / 100.0) 
                   * (1.0 + destructionFactor / 100.0) 
                   * factFreq 
                   * (1.0 + coreLossFactor / 100.0)
                  );
        }
    }

    double Core::calc_CoreWeight(double CoreSWW, double CoreWinHgt, double CoreStackLoop, double CoreStrip, double KSpaceFactor, home_LaminationType CoreGrade) 
    {
        double factor = (CoreGrade == home_LaminationType::Am || CoreGrade == home_LaminationType::AmHB1) ? 0.26 : 0.276;

        return (((CoreSWW + CoreWinHgt) * 2.0 + M_PI * CoreStackLoop) * CoreStackLoop * CoreStrip * KSpaceFactor * factor);
    }

    double Core::calc_FactAm(double fluxDensityVoltTAux, home_Device* device) 
    {
        if (fluxDensityVoltTAux > 1.56)        return 0.0;

        return ((1.76910872012377 * std::pow(fluxDensityVoltTAux, 6) 
               - 14.2268827328205 * std::pow(fluxDensityVoltTAux, 5) 
               + 47.2335635064786 * std::pow(fluxDensityVoltTAux, 4) 
               - 82.6681478839869 * std::pow(fluxDensityVoltTAux, 3) 
               + 80.447699124127  * std::pow(fluxDensityVoltTAux, 2) 
               - 41.1908340576338 * fluxDensityVoltTAux 
               + 8.69530921343919
               ) 
               * (1.0 + device->deviceFields["amorphousLossVariance"].get<double>() / 100.0) 
               * (1.0 + device->deviceFields["dAmAssemblyInducedStress"].get<double>() / 100.0) 
               * device->deviceFields["freqFactorHz"].get<double>() 
               * (1.0 + device->deviceFields["coreLossFactor"].get<double>() / 100.0));
    }

    void Core::erase_Warnings() 
    {
        for (int i = 0; i < Config::ERRORMESSAGES; ++i) 
        {
            coreFields["sErrorName"]    .set ("", i);
            coreFields["tErrorEnum"]    .set (static_cast<int>(home_WarningSeverity::None), i);
            coreFields["sErrorMessages"].set ("", i);
        }
    }

    void Core::calc_Core() 
    {
        home_Device*  device       = getParentDevice();
        home_Winding* windingHv    = device->get_WindingHV();
        home_Winding* windingLv    = device->get_WindingLV();
        int           numberPhases = coreFields["numberPhases"].get<int>();

        calc_LoopCoreStrip              (numberPhases);
        calc_SteelWidth                 (numberPhases);
        calc_FormNose                   (device, windingHv, numberPhases);
        calc_FormSide                   (device, windingLv, numberPhases);
        calc_Forms();
        calc_CoreDimensions             (device, windingHv, windingLv, numberPhases);
        calc_NoLoadLossDataArray        (device);
        calc_Weights                    (numberPhases);
        calc_ClampInsulationWeight();
        calc_PressboardPacking          (device, windingHv, windingLv, numberPhases);
        calc_FormDimensions();
        calc_WeightPressBoard           (device);
        calc_DuctMaterial               (device, windingHv, windingLv, numberPhases);
        calc_NoLoadLosses               (device, numberPhases);
        calc_CurrExc                    (device, numberPhases);
        calc_StringRated                (numberPhases);
        calc_AmHB1NegativeTolerances    (device, numberPhases);
    }

    void Core::calc_LoopCoreStrip(int numberPhases) 
    {
        if (numberPhases == 1)      { return; }

        double steelWidth       = coreFields["dSteelWidth"].get<double>();
        double result1          = 0.0;
        double result2          = 0.0;
        auto laminationType     = coreFields["laminationType"].get<home_LaminationType>();
        bool amorphous          = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);

        if (amorphous)     
        {
            bool match_error = true;

            for (const auto& row : amHB1CoreStripStandardWidths.tableData)
            {
                if (!row.empty() && row[0] == steelWidth) 
                {
                    match_error = false;
                    break;
                }
            }

            if (match_error)
            {
                for (const auto& row : amHB1CoreStripStandardWidths.tableData) 
                {
                    if (row.size() > 1)
                    {
                        auto it = std::find(row.begin() + 1, row.end(), steelWidth);

                        if (it != row.end())
                        {
                            result1 = row[0];
                            break;
                        }
                    }
                }
            } 
            else    { result1 = 0.0; }
            match_error = true;

            if (amHB1CoreStripStandardWidths.tableData.size() > 1) 
            {
                for (size_t i = 1; i < amHB1CoreStripStandardWidths.tableData.size(); ++i) 
                {
                    if (!amHB1CoreStripStandardWidths.tableData[i].empty() && amHB1CoreStripStandardWidths.tableData[i][0] == steelWidth) 
                    {
                        match_error = false;
                        break;
                    }
                }

                if (match_error) 
                {
                    size_t col_index = 0;
                    bool found = false;
                    for (size_t i = 1; i < amHB1CoreStripStandardWidths.tableData.size(); ++i) 
                    {
                        for (size_t j = 1; j < amHB1CoreStripStandardWidths.tableData[i].size(); ++j) 
                        {
                            if (amHB1CoreStripStandardWidths.tableData[i][j] == steelWidth) 
                            {
                                col_index = j - 1;
                                found = true;
                                break;
                            }
                        }
                        if (found) break;
                    }
                    
                    if (found && col_index + 1 < amHB1CoreStripStandardWidths.tableData[0].size())
                    {
                        result2 = amHB1CoreStripStandardWidths.tableData[0][col_index + 1];
                    }
                }
            }
        }
        coreFields["bLoop1AmCoreStrip"].set(result1);
        coreFields["loop2AmCoreStrip"].set(result2);
    }

    void Core::calc_SteelWidth(int numberPhases) 
    {
        auto laminationType = coreFields["laminationType"].get<home_LaminationType>();
        auto widthMap       = coreFields["d_SteelWidthMap"].get<std::map<std::string, std::vector<home_FieldValue::Value>>>();

        if (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) 
        {
            if (numberPhases == 1)         coreFields["dSteelWidth"].assignDefault(widthMap["TypeA"][0]);
            else                            coreFields["dSteelWidth"].assignDefault(widthMap["TypeA3"][0]);
        } 
        else                                coreFields["dSteelWidth"].assignDefault(widthMap["TypeB"][0]);
    }

    void Core::calc_AreaCore() 
    {
        double total_stack_inch = coreFields["totalStack"].get<double>();
        double steel_width_inch = coreFields["dSteelWidth"].get<double>();
        double kSpaceFactor     = table_MaterialConstants.tableData[6][0];
        double result           = Config::convert_Inch_CM(total_stack_inch, 1) * Config::convert_Inch_CM(steel_width_inch, 1) * kSpaceFactor * 100.0;

        coreFields["dArea"].set(result);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << Config::convert_CM_Inch(result / 100.0, 2);
        coreFields["sArea"].set(ss.str());
    }

    void Core::calc_DensityLimit(home_Device* device) 
    {
        auto laminationType     = coreFields["laminationType"].get<home_LaminationType>();
        bool amorphous          = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);

        int numberPhases        = device->deviceFields["numberPhases"].get<int>();
        double kvaNominal       = device->deviceFields["nominal_kva"].get<double>();
        double fluxDensityLimit = 1.63;

        if (amorphous) 
        {
            if      (numberPhases == 1)            fluxDensityLimit = 1.3; 
            else if (numberPhases == 3) 
            {
                if      (kvaNominal <= 100.0)      fluxDensityLimit = 1.3;
                else if (kvaNominal <= 500.0)      fluxDensityLimit = 1.35;
                else                               fluxDensityLimit = 1.39;
            }
        }
        coreFields["fluxDensityLimitMax"].set(fluxDensityLimit);
    }

    void Core::calc_Density(home_Device* device) 
    {
        calc_DensityLimit(device);

        double vxTVal                 = device->deviceFields["dVxT"].get<double>();
        double frequency              = device->deviceFields["frequency"].get<double>();
        double area                   = Config::convert_CM_Inch(coreFields["dArea"].get<double>() / 100.0, 2);
        double per                    = device->guarantee->guaranteeFields["dNL_pu"].get<double>();
        double perOvervoltageMaxFlux  = coreFields["dPerOvervoltageMaxFlux"].get<double>();

        double result                 = Config::safe_Division(per * (vxTVal * 60.0 / frequency), (0.002664 * area));
        coreFields["dFluxDensityRatPer"].set(result);
        double dFluxDensityVoltVal    = (per != 1.0) ? (1.0 / per) * result : (1.0 + perOvervoltageMaxFlux / 100.0) * result;

        coreFields["dFluxDensityVolt"].set(dFluxDensityVoltVal);
        coreFields["dFluxDensityRatPerT"].set(result / 64.516);
        coreFields["dFluxDensityVoltT"].set(dFluxDensityVoltVal / 64.516);
    }

    void Core::calc_ValidationCore() {}

    void Core::reset_Fields() 
    {
        for (auto& pair : coreFields)
        {
            try
            {
                pair.second.reset();
            }
            catch (...) {}
        }
    }

    void Core::calc_FormNose(home_Device* device, home_Winding* windingHv, int numberPhases) 
    {
        double dCoreStack   = coreFields["totalStack"].get<double>();
        auto tCoreGrade     = coreFields["laminationType"].get<home_LaminationType>();
        double dCoreStrip   = coreFields["dSteelWidth"].get<double>();
        int noLoops         = coreFields["noLoops"].get<int>();
        double result       = 0.0;

        if (numberPhases == 1) 
        {
            double part1 = ((std::floor(dCoreStack / 0.125) - dCoreStack * 8.0) == 0.0) ? dCoreStack : (std::floor(dCoreStack * 8.0) + 1.0) / 8.0;
            result = part1 + 0.25;
        } 
        else 
        {
            double add_core = (tCoreGrade == home_LaminationType::Am || tCoreGrade == home_LaminationType::AmHB1) ? ((dCoreStrip <= 8.4) ? 1.0 : 2.0) : static_cast<double>(noLoops);
            result = std::ceil(dCoreStack * 8.0 + add_core) / 8.0 + 0.125;
        }
        coreFields["dFormNose"].set(result);
    }

    void Core::calc_FormSide(home_Device* device, home_Winding* windingLv, int numberPhases) 
    {
        double steelWidth      = coreFields["dSteelWidth"].get<double>();
        auto laminationType    = coreFields["laminationType"].get<home_LaminationType>();
        double result           = 0.0;

        if (laminationType != home_LaminationType::Am && laminationType != home_LaminationType::AmHB1) 
        {
            if (numberPhases == 1) 
            {
                double kva          = windingLv->windingFields["d_kVA"].get<std::vector<double>>()[0];
                double condWidth    = windingLv->coils[0]->conductor[0].conductorFields.at("dWidth").get<double>();

                result              = steelWidth + ((kva < 750.0) ? 0.25 : 0.375) + ((condWidth == 2.0) ? 0.0625 : 0.0);
                result              = ((static_cast<int>(result / 0.0625)) - result * 16.0 == 0.0) ? result : std::floor(result * 16.0) / 16.0;
            } 
            else 
            {
                int dCompare = coreFields["noLoops"].get<int>();

                if (dCompare == 1 || dCompare == 2)                 result = static_cast<int>((dCompare * 0.125 + steelWidth + 0.25) * 8.0) / 8.0;
            }
        } 
        else 
        {
            if (numberPhases == 1) 
            {
                result              = steelWidth + ((steelWidth > 16.8) ? 0.4725 : ((steelWidth > 8.4) ? 0.315 : 0.1575)) + 0.25;
                result              = std::round(result * 8.0) / 8.0;
            } 
            else 
            {
                double dCompare     = (steelWidth <= 8.4) ? 1.0 : 2.0;
                double value        = (dCompare == 1.0) ? 0.05 + 0.125 + 0.25 + steelWidth : 0.1 + 0.25 + 0.25 + steelWidth;

                result              = ((static_cast<int>(value * 8.0)) - value * 8.0 == 0.0) ? value : std::floor(value * 8.0 + 1.0) / 8.0;
            }
        }
        coreFields["dFormSide"].set(result);
    }

    void Core::calc_Forms() 
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << coreFields["dFormNose"].get<double>() << "  " << coreFields["dFormSide"].get<double>();

        coreFields["sForms"].set(ss.str());
    }

    void Core::calc_TrueOverCoil(home_Device* device, home_Winding* windingLv, int numberPhases) 
    {
        double overCoil                 = windingLv->coils[0]->coilFields["dOvercoil"].get<double>();
        double dOverCoilRadialBuiltE    = device->deviceFields["dOverCoilRadialBuilt"].get<double>();
        double dOverCoilAdder           = coreFields["dOverCoilAdder"].get<double>();
        double result                   = std::ceil((overCoil + 2.0 * ((numberPhases == 1) 
                                                        ? dOverCoilRadialBuiltE 
                                                        : dOverCoilAdder)) / 0.125) * 0.125;

        coreFields["dTrueOverCoil"].set(result);
    }

    void Core::calc_CoreDimensions(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, int numberPhases) 
    {
        calc_TrueOverCoil          (device, windingLv, numberPhases);
        calc_SmallWindowWidth      (device, windingHv, windingLv, numberPhases);
        calc_LargeWindowWidth      (device, windingHv, windingLv, numberPhases);
        calc_StackPerLoop();
        calc_StripWidth            (numberPhases);
        calc_EBottom               (numberPhases);
        calc_OverallCoreHeight     (numberPhases);
        calc_CoreLengthWithoutPB   (numberPhases);
        calc_CoreLengthWithPB();
    }

    void Core::calc_WindowHeight(home_Device* device, int numberPhases) 
    {
        double dOpt_Fixed_WH                = coreFields["dFixedWindowHeight"].get<double>();
        double dOpt_WH_Adder                = coreFields["dWindingHeightAdder"].get<double>();
        double dTrueOverCoilE               = coreFields["dTrueOverCoil"].get<double>();
        double dCoreWHPackingRadialBuiltE   = device->deviceFields["dCoreWHPackingRadialBuilt"].get<double>();
        double result                       = (dOpt_Fixed_WH > 0.0) 
                                                ? dOpt_Fixed_WH 
                                                : std::round((dTrueOverCoilE + 2.0 * dCoreWHPackingRadialBuiltE) / 0.125 + ((numberPhases == 1) ? 1.0 : 2.0)) * 0.125 + dOpt_WH_Adder;
        coreFields["dWindowHeight"].set(result);
    }

    void Core::calc_SmallWindowWidth(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, int numberPhases)  
    {
        double dOpt_Fixed_WW            = coreFields["dFixedSmWindowWidth"].get<double>();
        double dOpt_WW_Adder            = coreFields["dWindingWidthAdder"].get<double>();
        double dRBIWE                   = (numberPhases == 1) 
                                            ? device->calc_RBIWAux(windingHv, windingLv, windingHv->coils[0], windingLv->coils[0], numberPhases) 
                                            : device->deviceFields["dRBIW"].get<double>();
        double dCoilCoreRadialBuiltE    = device->deviceFields["dCoilCoreRadialBuilt"].get<double>();
        double dPh_LimbClearance        = coreFields["dPh_LimbClearance"].get<double>();
        double result                   = (dOpt_Fixed_WW > 0.0) 
                                            ? dOpt_Fixed_WW 
                                            : std::floor((dRBIWE + dOpt_WW_Adder + ((numberPhases == 1) ? dCoilCoreRadialBuiltE : dPh_LimbClearance)) / 0.125 + 1.0) * 0.125;

        coreFields["dSmallWindowWidth"].set(result);
    }

    void Core::calc_LargeWindowWidth(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, int numberPhases) 
    {
        if (numberPhases == 1)      { return; }

        double dFixedWindowLarge    = coreFields["dFixedWindowLarge"].get<double>();
        double dRBIWE               = device->deviceFields["dRBIW"].get<double>();
        double dPh_PhClearance      = coreFields["dPh_PhClearance"].get<double>();
        double result               = std::floor((2.0 * dRBIWE + dFixedWindowLarge + dPh_PhClearance) / 0.125 + 1.0) * 0.125;

        coreFields["dLargeWindowWidth"].set(result);
    }

    void Core::calc_StackPerLoop() 
    {
        double result = coreFields["totalStack"].get<double>() / 2.0;
        coreFields["dStackPerLoop"].set(std::ceil(result / 0.125) * 0.125);
    }

    void Core::calc_StripWidth(int numberPhases) 
    {
        double dCoreStrip = coreFields["dSteelWidth"].get<double>();

        if (numberPhases == 1)      { coreFields["dStripWidth"].set(dCoreStrip); }
        else 
        {
            auto laminationType = coreFields["laminationType"].get<home_LaminationType>();

            if (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1 || coreFields["noLoops"].get<int>() == 1)
            {
                coreFields["dStripWidth"].set(dCoreStrip);
            }
            else                      coreFields["dStripWidth"].set(dCoreStrip / 2.0);
        }
    }

    void Core::calc_EBottom(int numberPhases) 
    {
        double largeWindowWidth   = coreFields["dLargeWindowWidth"].get<double>();
        double smallWindowWidth   = coreFields["dSmallWindowWidth"].get<double>();
        double stackPerLoop       = coreFields["dStackPerLoop"].get<double>();
        auto laminationType       = coreFields["laminationType"].get<home_LaminationType>();
        
        double result   = stackPerLoop;
        double resultL  = stackPerLoop;

        if (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) 
        {
            double factor       = (smallWindowWidth > 7.874) ? 1.11 
                                : ((smallWindowWidth > 6.89) ? 1.125 
                                : ((smallWindowWidth > 5.906) ? 1.1429 
                                : ((smallWindowWidth > 4.921) ? 1.1667 
                                : ((smallWindowWidth > 3.937) ? 1.2 
                                : ((smallWindowWidth > 1.787) ? 1.25 
                                : 1.0)))));
            result = stackPerLoop * factor;

            double factorL      = (largeWindowWidth > 7.874) ? 1.11
                                : ((largeWindowWidth > 6.89) ? 1.125 
                                : ((largeWindowWidth > 5.906) ? 1.1429 
                                : ((largeWindowWidth > 4.921) ? 1.1667 
                                : ((largeWindowWidth > 3.937) ? 1.2 
                                : ((largeWindowWidth > 1.787) ? 1.25 : 1.0)))));
            resultL = stackPerLoop * factorL;
        }

        coreFields["dEBottom"].set(result);

        if (numberPhases != 1) 
        {
            coreFields["dStackCoreSmLoops"].set(result);
            coreFields["dStackCoreLgLoops"].set(resultL);
        } 
        else 
        {
            coreFields["dStackCoreSmLoops"].reset();
            coreFields["dStackCoreLgLoops"].reset();
        }
    }

    void Core::calc_OverallCoreHeight(int numberPhases) 
    {
        double windowHeight   = coreFields["dWindowHeight"].get<double>();
        double stackPerLoop   = coreFields["dStackPerLoop"].get<double>();
        double eBottom        = coreFields["dEBottom"].get<double>();

        coreFields["dOverallCoreHeight"].set(windowHeight + stackPerLoop + eBottom);

        if (numberPhases != 1) 
        {
            auto laminationType   = coreFields["laminationType"].get<home_LaminationType>();

            double innerH         = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) 
                                  ? windowHeight + coreFields["dStackCoreLgLoops"].get<double>() + stackPerLoop 
                                  : windowHeight + coreFields["totalStack"].get<double>();

            coreFields["dOverallCoreHeightInner"].set(innerH);
        } 
        else         coreFields["dOverallCoreHeightInner"].reset();
    }

    void Core::calc_CoreLengthWithoutPB(int numberPhases) 
    {
        double smallWindowWidth   = coreFields["dSmallWindowWidth"].get<double>();
        double largeWindowWidth   = coreFields["dLargeWindowWidth"].get<double>();
        double stackPerLoop       = coreFields["dStackPerLoop"].get<double>();

        double result             = (numberPhases == 1) 
                                  ? 2.0 * smallWindowWidth + 4.0 * stackPerLoop 
                                  : 2.0 * smallWindowWidth + 2.0 * largeWindowWidth + 8.0 * stackPerLoop;

        coreFields["dCoreLengthWithoutPB"].set(result);
    }

    void Core::calc_CoreLengthWithPB() 
    {
        home_Device* device     = getParentDevice();

        double result   = (coreFields["dFormNose"].get<double>() - 2.0 * coreFields["dStackPerLoop"].get<double>()) 
                          * device->deviceFields["numberPhases"].get<int>() 
                          + coreFields["dCoreLengthWithoutPB"].get<double>();

        coreFields["dCoreLengthWithPB"].set(result);
    }

    void Core::calc_NoLoadLossDataArray(home_Device* device) 
    {
        int numberPhases = device->deviceFields["numberPhases"].get<int>();

        calc_DestructionFactors();
        calc_NoLoadLossDataFre(device, numberPhases);
        calc_CoreTolerance();
        calc_WattsLb(device);
        calc_LegCenters(numberPhases);
    }

    void Core::calc_DestructionFactors() 
    {
        double nominalKVA  = getParentDevice()->deviceFields["nominal_kva"].get<double>();

        coreFields["destructionFactorAm"].set(1.155);

        double factor   = (nominalKVA <= 10.0)  ? 1.12 
                        : ((nominalKVA <= 15.0)  ? 1.10 
                        : ((nominalKVA <= 37.0)  ? 1.06 
                        : ((nominalKVA <= 50.0)  ? 1.05 
                        : ((nominalKVA <= 75.0)  ? 1.04 
                        : ((nominalKVA <= 100.0) ? 1.03 
                        : 1.02)))));

        coreFields["destructionFactorReg"].set(factor);
    }

    void Core::calc_CommonFact(home_Device* device, int numberPhases) 
    {
        auto   laminationType            = coreFields["laminationType"].get<home_LaminationType>();

        double fluxDensityRatPerT        = coreFields["dFluxDensityRatPerT"].get<double>();
        double coreLossFactor            = device->deviceFields["coreLossFactor"].get<double>();
        double frequencyFactorHz         = device->deviceFields["freqFactorHz"].get<double>();
        double amorphousLossVariance     = device->deviceFields["amorphousLossVariance"].get<double>();
        double destructionFactor         = device->deviceFields["dDestructionFactor"].get<double>();
        double dAmAssemblyInducedStressE = device->deviceFields["dAmAssemblyInducedStress"].get<double>();
        double result                    = 0.0;

        if (laminationType != home_LaminationType::Am && laminationType != home_LaminationType::AmHB1) 
        {
            result = (std::pow(fluxDensityRatPerT / 2.409, 1.7333) + std::pow(fluxDensityRatPerT / 2.018, 9.728)) * 1.409 * frequencyFactorHz * (1.0 + coreLossFactor / 100.0);
        } 
        else 
        {
            double base_value = coreFields["d_NLLossDataFre"].get<std::vector<double>>()[static_cast<int>((laminationType == home_LaminationType::Am) 
                                ? home_LaminationType::Am 
                                : home_LaminationType::AmHB1)];

            result = base_value * (1.0 + amorphousLossVariance / 100.0) * (1.0 + ((numberPhases == 1) 
                    ? dAmAssemblyInducedStressE 
                    : destructionFactor) / 100.0) * frequencyFactorHz * (1.0 + coreLossFactor / 100.0);
        }

        coreFields["dCommonFact"].set(result);
    }

    void Core::calc_NoLoadLossDataFre(home_Device* device, int numberPhases) 
    {
        auto laminationType = coreFields["laminationType"].get<home_LaminationType>();

        double fluxDensityRatPerT    = coreFields["dFluxDensityRatPerT"].get<double>();
        double destructionFactorReg = coreFields["destructionFactorReg"].get<double>();
        double destructionFactorAmE  = coreFields["destructionFactorAm"].get<double>();

        if (numberPhases == 1) 
        {
            double resultAmHB1  = (0.1939 * std::pow(fluxDensityRatPerT, 5) 
                                - 0.9235 * std::pow(fluxDensityRatPerT, 4) 
                                + 1.8    * std::pow(fluxDensityRatPerT, 3) 
                                - 1.7988 * std::pow(fluxDensityRatPerT, 2) 
                                + 1.0463 * fluxDensityRatPerT - 0.2628) 
                                * destructionFactorAmE;

            coreFields["d_NLLossDataFre"].set(resultAmHB1, static_cast<int>(home_LaminationType::AmHB1));

            double resultAm     = (laminationType == home_LaminationType::Am) 
                                ? (1.76910872012377 * std::pow(fluxDensityRatPerT, 6) 
                                  - 14.2268827328205 * std::pow(fluxDensityRatPerT, 5) 
                                  + 47.2335635064786 * std::pow(fluxDensityRatPerT, 4) 
                                  - 82.6681478839869 * std::pow(fluxDensityRatPerT, 3) 
                                  + 80.447699124127  * std::pow(fluxDensityRatPerT, 2) 
                                  - 41.1908340576338 * fluxDensityRatPerT 
                                  + 8.69530921343919) 
                                : 0.00;
            coreFields["d_NLLossDataFre"].set(resultAm, static_cast<int>(home_LaminationType::Am));

            calc_CommonFact(device, numberPhases);
            double dCommonFactE = coreFields["dCommonFact"].get<double>();

            double resultM0H    = (8.9581 * std::pow(fluxDensityRatPerT, 5) 
                                - 60.345 * std::pow(fluxDensityRatPerT, 4) 
                                + 161.36 * std::pow(fluxDensityRatPerT, 3) 
                                - 213.79 * std::pow(fluxDensityRatPerT, 2) 
                                + 140.6  * fluxDensityRatPerT - 36.62) 
                                * destructionFactorReg;

            coreFields["d_NLLossDataFre"].set(resultM0H, static_cast<int>(home_LaminationType::M0H));
            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M2) ? dCommonFactE * 0.7   : 0.00, static_cast<int>(home_LaminationType::M2));
            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M3) ? dCommonFactE * 0.875 : 0.00, static_cast<int>(home_LaminationType::M3));

            double resultM4     = (1.1916 * std::pow(fluxDensityRatPerT, 5) 
                                - 5.7808  * std::pow(fluxDensityRatPerT, 4) 
                                + 10.759  * std::pow(fluxDensityRatPerT, 3) 
                                - 9.3229  * std::pow(fluxDensityRatPerT, 2) 
                                + 3.9858  * fluxDensityRatPerT - 0.6225) 
                                * destructionFactorReg;

            coreFields["d_NLLossDataFre"].set(resultM4, static_cast<int>(home_LaminationType::M4));
        } 
        else 
        {
            double resultAmHB1  = (laminationType == home_LaminationType::AmHB1) 
                                ? (3.0983 * std::pow(fluxDensityRatPerT, 6) 
                                  - 23.862 * std::pow(fluxDensityRatPerT, 5) 
                                  + 76.516 * std::pow(fluxDensityRatPerT, 4) 
                                  - 130.51 * std::pow(fluxDensityRatPerT, 3) 
                                  + 124.76 * std::pow(fluxDensityRatPerT, 2) 
                                  - 63.218 * fluxDensityRatPerT 
                                  + 13.272) 
                                : 0.0;

            coreFields["d_NLLossDataFre"].set(resultAmHB1, static_cast<int>(home_LaminationType::AmHB1));

            double resultAm     = (laminationType == home_LaminationType::Am) 
                                ? (1.76910872012377 * std::pow(fluxDensityRatPerT, 6) 
                                  - 14.2268827328205 * std::pow(fluxDensityRatPerT, 5) 
                                  + 47.2335635064786 * std::pow(fluxDensityRatPerT, 4) 
                                  - 82.6681478839869 * std::pow(fluxDensityRatPerT, 3) 
                                  + 80.447699124127  * std::pow(fluxDensityRatPerT, 2) 
                                  - 41.1908340576338 * fluxDensityRatPerT 
                                  + 8.69530921343919) 
                                : 0.00;

            coreFields["d_NLLossDataFre"].set(resultAm, static_cast<int>(home_LaminationType::Am));

            calc_CommonFact(device, numberPhases);
            double commonFact = coreFields["dCommonFact"].get<double>();

            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M0H) ? commonFact * 0.76  : 0.0,  static_cast<int>(home_LaminationType::M0H));
            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M2)  ? commonFact * 0.76  : 0.00, static_cast<int>(home_LaminationType::M2));
            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M3)  ? commonFact * 0.875 : 0.00, static_cast<int>(home_LaminationType::M3));
            coreFields["d_NLLossDataFre"].set((laminationType == home_LaminationType::M4)  ? commonFact * 0.97  : 0.00, static_cast<int>(home_LaminationType::M4));
        }
    }

    void Core::calc_CoreTolerance() 
    {
        double stackPerLoop = coreFields["dStackPerLoop"].get<double>();
        auto laminationType = coreFields["laminationType"].get<home_LaminationType>();
        bool boolAm         = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);

        double value        = (stackPerLoop <= 1.75) ? 0.02 
                            : ((stackPerLoop <= 2.25) ? 0.03 
                            : ((stackPerLoop <= 2.75) ? 0.035 
                            : ((stackPerLoop <= 4.5)  ? 0.04 
                            : 0.05)));
        
        coreFields["d_CoreTolerance"].set(std::string("A --> ")     + (boolAm ? "+0.188 / -0.000 in"    : "+0.125 / -0.063 in"),                   static_cast<int>(home_CoreTolerance::A));
        coreFields["d_CoreTolerance"].set(std::string("C --> ")     + (boolAm ? "+0.138 / -0.000 in"    : "+ 0.125, - 0.000 in"),                  static_cast<int>(home_CoreTolerance::C));
        coreFields["d_CoreTolerance"].set(std::string("E --> ")     + (boolAm ? "+0.000 / -0.125 in"    : "+/- ") + std::to_string(value) + " in", static_cast<int>(home_CoreTolerance::E));
        coreFields["d_CoreTolerance"].set(std::string("STRIP --> ") + (boolAm ? "+0.16 / -0.000 in"     : "+/- 0.02 in"),                          static_cast<int>(home_CoreTolerance::Strip));
    }

    void Core::calc_WattsLb(home_Device* device) 
    {
        auto   laminationType      = coreFields["laminationType"].get<home_LaminationType>();
        double commonFact          = coreFields["dCommonFact"].get<double>();
        int    valueLaminationType = get_LaminationNumber();
        auto   noLoadCorrected     = device->guarantee->guaranteeFields["noLoadCorrected_to_85C"].get<bool>();

        double base_value          = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) 
                                   ? commonFact 
                                   : ((valueLaminationType != static_cast<int>(home_LaminationType::None)) 
                                   ? coreFields["d_NLLossDataFre"].get<std::vector<double>>()[valueLaminationType] 
                                   : 0.0);

        double factor              = (noLoadCorrected == true) ? 1.0 - (85.0 - 20.0) * 0.00065 : 1.0;
        double result              = base_value * factor;

        coreFields["dWattsLb"].set((result < 0.0) ? 0.0 : result);
    }

    void Core::calc_LegCenters(int numberPhases) 
    {
        double dWindowE = (numberPhases == 1) ? coreFields["dSmallWindowWidth"].get<double>() : coreFields["dLargeWindowWidth"].get<double>();

        coreFields["dLegCenters"].set(dWindowE + coreFields["dFormNose"].get<double>());
    }

    void Core::calc_Weights(int numberPhases) 
    {
        double smallWindowWidth   = coreFields["dSmallWindowWidth"].get<double>();
        double windowHeight       = coreFields["dWindowHeight"].get<double>();
        double stackPerLoop       = coreFields["dStackPerLoop"].get<double>();
        auto   laminationType     = coreFields["laminationType"].get<home_LaminationType>();
        double stripWidth         = coreFields["dStripWidth"].get<double>();
        double kSpaceFactor       = table_MaterialConstants.tableData[6][0];
        bool   amorphous          = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);

        int    noLoops            = coreFields["noLoops"].get<int>();
        double largeWindowWidth   = coreFields["dLargeWindowWidth"].get<double>();

        double loop1              = 0.0, dloop2 = 0.0;
        bool   isSmallLoop        = !amorphous || (stripWidth != 5.6 && stripWidth != 6.7 && stripWidth != 8.4);

        if (amorphous) 
        {
            auto headers = find_Headers(stripWidth);
            loop1        = headers.first;
            dloop2       = headers.second;
        } 
        else { isSmallLoop = (noLoops == 2); }

        double result1 = 0.0;
        double result2 = 0.0;
        double temp    = 0.0;

        std::vector<double> smallLoop, largeLoop;

        if (isSmallLoop) 
        {
            if (amorphous) 
            {
                temp      = std::ceil((stripWidth * stackPerLoop * kSpaceFactor * 645.16 / 100.0 - stripWidth * 0.13207) / (stripWidth * 0.1829) - 7.0) + 7.0;
                smallLoop = calc_Loop(smallWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor);
                result1   = ((temp * 15.0 * (smallLoop[0] + smallLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0 * loop1 / stripWidth;
            } 
            else        result1 = ((smallWindowWidth + windowHeight) * 2.0 + M_PI * stackPerLoop) * stackPerLoop * stripWidth * kSpaceFactor * 0.276 * 2.0;
        }

        if (numberPhases != 1 && isSmallLoop) 
        {
            if (amorphous) 
            {
                largeLoop = calc_Loop(largeWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor);
                result2   = ((temp * 15.0 * (largeLoop[0] + largeLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0 * loop1 / stripWidth;
            } 
            else        result2 = ((largeWindowWidth + windowHeight) * 2.0 + M_PI * stackPerLoop) * stackPerLoop * stripWidth * kSpaceFactor * 0.276 * 2.0;
        }

        coreFields["dWeightOneSmLoop"].set(result1);
        coreFields["dWeightOneLgLoop"].set(result2);
        coreFields["dWeightOneLoop"]  .set(result1 + result2);

        if (!isSmallLoop) 
        {
            coreFields["dWeightOneSmLoopOther"].reset();
            coreFields["dWeightOneLgLoopOther"].reset();
            coreFields["dWeightOneLoopOther"]  .reset();
        } 
        else if (amorphous) 
        {
            result1 = ((temp * 15.0 * (smallLoop[0] + smallLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0 * dloop2 / stripWidth;
            result2 = (numberPhases != 1) 
                    ? ((temp * 15.0 * (largeLoop[0] + largeLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0 * dloop2 / stripWidth
                    : 0.0;
            
            coreFields["dWeightOneSmLoopOther"].set(result1);
            coreFields["dWeightOneLgLoopOther"].set(result2);
            coreFields["dWeightOneLoopOther"]  .set(result1 + result2);
        }

        if (amorphous) 
        {
            if (!isSmallLoop) 
            {
                temp      = std::ceil((stripWidth * stackPerLoop * kSpaceFactor * 645.16 / 100.0 - stripWidth * 0.13207) / (stripWidth * 0.1829) - 7.0) + 7.0;
                smallLoop = calc_Loop(smallWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor);
                result1   = ((temp * 15.0 * (smallLoop[0] + smallLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0;
            } 
            else        result1 = coreFields["dWeightOneSmLoop"].get<double>() + coreFields["dWeightOneSmLoopOther"].get<double>();
        } 
        else            result1 = (noLoops == 2) 
                                ? 2.0 * coreFields["dWeightOneSmLoop"].get<double>() 
                                : ((smallWindowWidth + windowHeight) * 2.0 + M_PI * stackPerLoop) * stackPerLoop * stripWidth * kSpaceFactor * 0.276 * 2.0;

        if (numberPhases != 1) 
        {
            if (amorphous) 
            {
                if (!isSmallLoop) 
                {
                    largeLoop = calc_Loop(largeWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor);
                    result2   = ((temp * 15.0 * (largeLoop[0] + largeLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733) * 2.20462 * 2.0;
                } 
                else    result2 = coreFields["dWeightOneLgLoop"].get<double>() + coreFields["dWeightOneLgLoopOther"].get<double>();
            } 
            else        result2 = (noLoops == 2) 
                                ? 2.0 * coreFields["dWeightOneLgLoop"].get<double>() 
                                : ((largeWindowWidth + windowHeight) * 2.0 + M_PI * stackPerLoop) * stackPerLoop * stripWidth * kSpaceFactor * 0.276 * 2.0;
        } 
        else            result2 = 0.0;

        coreFields["dWeightTwoSmLoops"].set(result1);
        coreFields["dWeightTwoLgLoops"].set(result2);
        coreFields["dWeightTwoLoops"]  .set(result1 + result2);
    }

    std::pair<double, double> Core::find_Headers(double value) 
    {
        const auto& table = amHB1CoreStripStandardWidths.tableData;

        for (size_t i = 1; i < table.size(); ++i) 
        {
            for (size_t j = 1; j < table[i].size(); ++j) 
            {
                if (table[i][j] == value) { return {table[i][0], table[0][j]}; }
            }
        }
        return {0.0, 0.0};
    }

    std::vector<double> Core::calc_Loop(double smallWindowLargeWidth, double windowHeight, double stripWidth, double stackPerLoop, double kSpaceFactor, double dStackTolIn) 
    {
        int add_val = (smallWindowLargeWidth > 3.55)  ? 18 
                    : ((smallWindowLargeWidth > 3.15)  ? 16 
                    : ((smallWindowLargeWidth > 2.75)  ? 14 
                    : ((smallWindowLargeWidth > 2.56)  ? 13 
                    : ((smallWindowLargeWidth > 2.36)  ? 12 
                    : ((smallWindowLargeWidth > 2.165) ? 11 
                    : ((smallWindowLargeWidth > 1.77)  ? 10 
                    : 0))))));

        double dStartLengthIN = static_cast<double>(static_cast<int>(2.0 * (windowHeight * 25.4 - 12.7) 
                                + 2.0 * (smallWindowLargeWidth * 25.4 - 12.7) 
                                + 41.0 + add_val + 5.0));

        double dEndLengthIN   = dStartLengthIN + (std::ceil((stripWidth * (stackPerLoop - dStackTolIn) * kSpaceFactor * 645.16 / 100.0 - stripWidth * 0.13207) / (stripWidth * 0.1829) - 7.0) + 7.0) * 5.59 - 2.0;
        
        return {dStartLengthIN, dEndLengthIN};
    }

    void Core::calc_ClampInsulationWeight() 
    {
        double stackPerLoop     = coreFields["dStackPerLoop"].get<double>();
        double stripWidth       = coreFields["dStripWidth"].get<double>();
        double coreLengthWithPB = coreFields["dCoreLengthWithPB"].get<double>();
        double result           = 2.0 * (stackPerLoop + 0.375 + 4.0) + stripWidth;

        coreFields["dWidthClampIns"].set(result);
        coreFields["dHeightClampIns"].set(coreLengthWithPB);
        coreFields["dWeightClampIns"].set(result * coreLengthWithPB * 0.02 * 0.035);
    }

    void Core::calc_PressboardPacking(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, int numberPhases) 
    {
        auto   laminationType     = coreFields["laminationType"].get<home_LaminationType>();
        double eBottom            = coreFields["dEBottom"].get<double>();
        double windowHeight       = coreFields["dWindowHeight"].get<double>();
        double stripWidth         = coreFields["dStripWidth"].get<double>();
        double smallWindowWidth   = coreFields["dSmallWindowWidth"].get<double>();
        double largeWindowWidth   = coreFields["dLargeWindowWidth"].get<double>();
        double stackPerLoop       = coreFields["dStackPerLoop"].get<double>();
        double coreLengthWithPB   = coreFields["dCoreLengthWithPB"].get<double>();
        double overallCoreHeight  = coreFields["dOverallCoreHeight"].get<double>();
        bool   amorphous          = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);
        double formNose           = coreFields["dFormNose"].get<double>();
        double formSide           = coreFields["dFormSide"].get<double>();
        double overCoil           = windingLv->coils[0]->coilFields["dOvercoil"].get<double>();
        int    noLoops            = coreFields["noLoops"].get<int>();
        double loop2AmCoreStrip   = coreFields["loop2AmCoreStrip"].get<double>();

        if (numberPhases == 1) 
        {
            pressBoardPacking[0][0] = amorphous ? std::ceil((eBottom + 0.5) * 16.0) / 16.0 + 0.125 : stripWidth;
            pressBoardPacking[1][0] = amorphous ? std::ceil((stripWidth + 0.25) * 16.0) / 16.0 : (formNose - 0.125);
            pressBoardPacking[2][0] = amorphous ? std::ceil(stripWidth * 16.0) / 16.0 : (smallWindowWidth - 0.125);
            pressBoardPacking[3][0] = amorphous ? std::ceil(windowHeight * 16.0) / 16.0 : (stackPerLoop + 0.25);
            pressBoardPacking[4][0] = amorphous ? std::ceil(smallWindowWidth * 16.0) / 16.0 : 0.0;
            pressBoardPacking[5][0] = 0.0;
        } 
        else 
        {
            pressBoardPacking[0][0] = static_cast<int>((amorphous ? stripWidth + 0.2362 : ((noLoops == 1) ? stripWidth : 2.0 * stripWidth + 0.125)) / 0.0625) * 0.0625;
            pressBoardPacking[1][0] = static_cast<int>((amorphous ? ((stripWidth == 5.6 || stripWidth == 6.7 || stripWidth == 8.4) ? stripWidth + 0.2362 : loop2AmCoreStrip + 0.2362) : 0.0) / 0.0625) * 0.0625;
            pressBoardPacking[2][0] = formNose - 0.125;
            pressBoardPacking[3][0] = amorphous ? formNose - 0.125 : 0.0;
            pressBoardPacking[4][0] = smallWindowWidth - 0.125;
            pressBoardPacking[5][0] = largeWindowWidth - 0.125;
        }

        pressBoardPacking[10][0] = amorphous ? formSide : ((noLoops == 1) ? stripWidth : 2.0 * stripWidth + 0.125);

        if (numberPhases == 1) 
        {
            pressBoardPacking[0][1] = amorphous ? std::ceil((2.0 * smallWindowWidth + 3.0 * stackPerLoop) * 16.0) / 16.0 : overallCoreHeight;
            pressBoardPacking[1][1] = amorphous ? std::ceil((coreLengthWithPB - 0.25) * 16.0) / 16.0 : overCoil;
            pressBoardPacking[2][1] = amorphous ? std::ceil(overallCoreHeight * 16.0) / 16.0 : stripWidth + 2.0;
        } 
        else 
        {
            pressBoardPacking[0][1] = static_cast<int>(overallCoreHeight / 0.0625) * 0.0625;
            pressBoardPacking[1][1] = amorphous ? static_cast<int>(overallCoreHeight / 0.0625) * 0.0625 : 0.0;
            pressBoardPacking[2][1] = overCoil;
        }

        double res1_8 = 0.0, res1_16 = 0.0;

        for (int i = 0; i < 11; ++i) 
        {
            res1_8  += pressBoardPacking[i][0] * pressBoardPacking[i][1] * 0.125  * pressBoardPacking[i][4] * 0.035;
            res1_16 += pressBoardPacking[i][0] * pressBoardPacking[i][1] * 0.0625 * pressBoardPacking[i][5] * 0.035;
        }

        coreFields["dWeights1_8"].set(res1_8);
        coreFields["dWeights1_16"].set(res1_16);
    }

    void Core::calc_FormDimensions() 
    {
        double formNose = coreFields["dFormNose"].get<double>();
        double formSide = coreFields["dFormSide"].get<double>();
        double overCoil = getParentDevice()->get_WindingLV()->coils[0]->coilFields["dOvercoil"].get<double>();

        coreFields["dFormDimA"].set((formNose - 0.25) / 2.0);
        coreFields["dFormDimS"].set(formSide);
        coreFields["dFormDimN"].set(formNose);
        coreFields["dFormDimW"].set(overCoil);
        coreFields["dTotalFormLength"].set(2.0 * coreFields["dFormDimA"].get<double>() + 2.0 * coreFields["dFormDimS"].get<double>() + coreFields["dFormDimN"].get<double>());
    }

    void Core::calc_WeightPressBoard(home_Device* device) 
    { 
        double overCoil        = device->get_WindingLV()->coils[0]->coilFields["dOvercoil"].get<double>();
        double formNose        = coreFields["dFormNose"].get<double>();
        double pBWindingForm   = device->deviceFields["dPBWindingForm"].get<double>();
        double totalFormLength = coreFields["dTotalFormLength"].get<double>();
        int    numberPhases    = device->deviceFields["numberPhases"].get<int>();

        double result8  = (numberPhases == 1 && pBWindingForm != 0.0625) 
                        ? ((totalFormLength * overCoil * pBWindingForm) + (overCoil * formNose * pBWindingForm)) * 0.035 
                        : 0.0;

        coreFields["dWeightsPB1_8"].set(result8 + coreFields["dWeights1_8"].get<double>());

        double result16 = (numberPhases == 1 && pBWindingForm == 0.0625) 
                        ? ((totalFormLength * overCoil * pBWindingForm) + (overCoil * formNose * pBWindingForm)) * 0.035 
                        : 0.0;

        coreFields["dWeightsPB1_16"].set(result16 + coreFields["dWeights1_16"].get<double>());

        coreFields["dWeightPressBoard"].set(coreFields["dWeightsPB1_8"].get<double>() + coreFields["dWeightsPB1_16"].get<double>());

        coreFields["dWeightsPBHD_1_4"].set((numberPhases != 1 && pBWindingForm == 0.25) 
                                            ? 3.0 * (pressBoardPacking[8][1] * overCoil * pBWindingForm) * 0.035 
                                            : 0.0);
    }

    void Core::calc_DuctMaterial(home_Device* device, home_Winding* windingHv, home_Winding* windingLv, int numberPhases) 
    {
        double dTotal_HL_MT = device->deviceFields["dTotal_HL_MT"].get<double>();
        double dILH_MT      = device->deviceFields["dILH_MT"].get<double>();
        double dHOL_MT      = device->deviceFields["dHOL_MT"].get<double>();
        double dUOHVDuctHV  = windingHv->coils[0]->coilFields["dUOHVDuct"].get<double>();
        double dOvercoil    = windingLv->coils[0]->coilFields["dOvercoil"].get<double>();

        coreFields["d_ThickDM"].set(windingLv->coils[0]->coilFields["dWidthDucts"].get<double>(), 0);
        coreFields["d_ThickDM"].set(dUOHVDuctHV, 1);
        coreFields["s_LocationDM"].set("Windings", 0);

        if (dUOHVDuctHV != 0.0)         coreFields["s_LocationDM"].set("Under/Over HV", 1);

        coreFields["d_WidthDM"].set(dOvercoil - 0.125, 0);

        if (dUOHVDuctHV != 0.0)         coreFields["d_WidthDM"].set(dOvercoil - 0.125, 1);

        double roll0 = ((dTotal_HL_MT / 5.0) * numberPhases * 2.0 * (windingLv->windingFields["iNoDuctInner"].get<int>() + windingLv->windingFields["iNoDuctOuter"].get<int>() + windingHv->windingFields["iNoDuctInner"].get<int>()) / 12.0);
        coreFields["d_RollLengthDM"].set(roll0, 0);
        
        if (dUOHVDuctHV != 0.0) 
        {
            double roll1 = (dHOL_MT + ((windingHv->windingFields["type_connection"].get<home_ConnectionType>() == home_ConnectionType::Wye) ? 0.0 : dILH_MT)) / 12.0 * numberPhases;
            coreFields["d_RollLengthDM"].set(roll1, 1);
        }

        coreFields["d_8FtLengthsDM"].set(coreFields["d_RollLengthDM"].get<std::vector<double>>()[0] / 8.0, 0);
        coreFields["d_8FtLengthsDM"].set(coreFields["d_RollLengthDM"].get<std::vector<double>>()[1] / 8.0, 1);

        coreFields["d_DuctsDM"].set(Config::safe_Division(coreFields["d_RollLengthDM"].get<std::vector<double>>()[0], static_cast<double>(static_cast<int>(Config::safe_Division(51.0, coreFields["d_WidthDM"].get<std::vector<double>>()[0])))), 0);
        coreFields["sDuctMaterial"].set("\"" + std::to_string(coreFields["d_ThickDM"].get<std::vector<double>>()[0]) + "\" x 52\" Wide Rolls\"", 0);
    }

    void Core::calc_NoLoadLosses(home_Device* device, int numberPhases) 
    {
        auto   laminationType  = coreFields["laminationType"].get<home_LaminationType>();
        bool   amorphous       = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);
        double fluxDensityVolt = (numberPhases != 1 && amorphous) ? coreFields["dFluxDens100V"].get<double>() : coreFields["dFluxDensityVoltT"].get<double>();
        double weight          = coreFields["dWeightTwoLoops"].get<double>();
        double factorCore      = coreFactor.get_Factor(laminationType);

        std::vector<double> steps = {0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20};

        for (size_t i = 0; i < steps.size(); ++i) 
        {
            double aux    = steps[i] * fluxDensityVolt;
            double factor = (laminationType == home_LaminationType::AmHB1) 
                          ? calc_FactAmHB1(aux, device) 
                          : ((laminationType == home_LaminationType::Am) 
                          ? calc_FactAm(aux, device) 
                          : calc_Factor(aux) * factorCore);

            coreFields["d_NoLoadLosses20"].set(weight * factor * Config::NO_LOAD_LOSS_FACTOR, static_cast<int>(i));
            coreFields["d_NoLoadLosses75"].set(coreFields["d_NoLoadLosses20"].get<std::vector<double>>()[i] * (1.0 - (75.0 - 20.0) * 0.00065), static_cast<int>(i));
            coreFields["d_NoLoadLosses85"].set(coreFields["d_NoLoadLosses20"].get<std::vector<double>>()[i] * (1.0 - (85.0 - 20.0) * 0.00065), static_cast<int>(i));
        }
    }

    void Core::calc_CurrExc(home_Device* device, int numberPhases) 
    {
        try 
        {
            auto   laminationType        = coreFields["laminationType"].get<home_LaminationType>();
            bool   amorphous             = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1);
            double kvaNominal            = device->deviceFields["nominal_kva"].get<double>();
            double weight                = coreFields["dWeightTwoLoops"].get<double>();
            double amorphousLossVariance = device->deviceFields["amorphousLossVariance"].get<double>();
            double freqFactorHz          = device->deviceFields["freqFactorHz"].get<double>();
            double lossFactor            = coreFields["lossFactor"].get<double>();
            bool   b3PhasesAmorphous     = (numberPhases != 1 && amorphous);
            double fluxDensityVolt       = b3PhasesAmorphous ? coreFields["dFluxDens100V"].get<double>() : coreFields["dFluxDensityVoltT"].get<double>();
            int    valueLaminationType   = get_LaminationNumber();

            std::vector<double> factors = {0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20};

            const std::vector<std::vector<double>>* tables[] = 
            {
                &table_Ind90.tableData,  &table_Ind95.tableData,      
                &table_Ind100.tableData, &table_Ind105.tableData,     
                &table_Ind110.tableData, &table_Ind115.tableData,     
                &table_Ind120.tableData
            };

            for (size_t i = 0; i < factors.size(); ++i)
            {
                double res = found_Table(*tables[i], 
                                         factors[i] * fluxDensityVolt, 
                                         valueLaminationType + 1, 
                                         amorphousLossVariance, 
                                         freqFactorHz, 
                                         lossFactor, 
                                         b3PhasesAmorphous) 
                             * Config::safe_Division(2.5 * weight / 1000.0, kvaNominal, 0.0);

                coreFields["d_IExc"].set(res, static_cast<int>(i));
                
                if (i == 2) { device->deviceFields["dPerExc"].set(100.0 * res); }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Could not execute excitation loops parsing sequence: " << e.what() << std::endl;      
        }
    }

    double Core::found_Table(const std::vector<std::vector<double>>& table, double fluxDensity, int index, double amorphousLossVariance, double freqFactorHz, double lossFactor, bool b3PhasesAmorphous) 
    {
        if (b3PhasesAmorphous) 
        {
            if (fluxDensity > 1.6) { return 0.0; }

            return (130.76 * std::pow(fluxDensity, 6) 
                  - 836.32 * std::pow(fluxDensity, 5) 
                  + 2191.0 * std::pow(fluxDensity, 4) 
                  - 2994.8 * std::pow(fluxDensity, 3) 
                  + 2236.4 * std::pow(fluxDensity, 2) 
                  - 855.09 * fluxDensity + 128.1) 
                  * (1.0 + amorphousLossVariance / 100.0) 
                  * 1.52 * freqFactorHz 
                  * (1.0 + lossFactor / 100.0);
        }
        
        if (fluxDensity <= 0.0) { return 0.0; }

        int indexAux = -1;

        for (int i = 0; i < 17; ++i) 
        {
            if (table[i][0] == fluxDensity)                   { return table[i][index]; }
            if (table[i][0] > fluxDensity && indexAux == -1) { indexAux = i; }
        }

        if (indexAux != -1 && indexAux > 0) 
        {
            double tGreater   = table[indexAux][0];
            double tLess      = table[indexAux - 1][0];
            double lamGreater = table[indexAux][index];
            double lamLess    = table[indexAux - 1][index];

            return (lamGreater > 0.0) ? lamLess + (fluxDensity - tLess) * (lamGreater - lamLess) / (tGreater - tLess) : 0.0;
        }
        return 0.0;
    }

    double Core::calc_Factor(double fluxDensityVoltTAux) 
    {
        if (fluxDensityVoltTAux > 1.9) { return 0.0; }

        return (std::pow(fluxDensityVoltTAux / 2.409, 1.7333) 
              + std::pow(fluxDensityVoltTAux / 2.018, 9.728)) 
              * 1.409 * getParentDevice()->deviceFields["freqFactorHz"].get<double>() 
              * (1.0 + coreFields["coreLossFactor"].get<double>() / 100.0);
    }

    void Core::calc_StringRated(int numberPhases) 
    {
        double dNlPU = getParentDevice()->guarantee->guaranteeFields["dNL_pu"].get<double>();
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << (dNlPU * 100.0) << "% Rated";
        coreFields["sRatedPU"].set((dNlPU != 1.0) ? ss.str() : "Rated Volts");

        double overVolt = coreFields["dPerOvervoltageMaxFlux"].get<double>() + 100.0;

        std::stringstream ssV;
        ssV << std::fixed << std::setprecision(0) << overVolt;
        coreFields["sRatedV"].set(ssV.str() + ((numberPhases == 1) ? "% Rated Voltage" : "% Volts"));
    }

    std::string Core::calc_sPart() 
    {
        auto         laminationType = coreFields["laminationType"].get<home_LaminationType>();
        home_Device* device         = getParentDevice();

        std::stringstream ss;

        if (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) 
        {
            ss << device->deviceFields["numberPhases"].get<int>() 
               << " - " << Config::to_string(laminationType) 
               << " - " << std::fixed << std::setprecision(1) << coreFields["dSteelWidth"].get<double>() 
               << " - " << std::fixed << std::setprecision(0) << coreFields["dWeightTwoLoops"].get<double>();
        } 
        else 
        {
            ss << device->deviceFields["numberPhases"].get<int>() 
               << " - " << Config::to_string(laminationType) 
               << " - UC - " << device->deviceFields["so_number"].get<std::string>();
        }

        return ss.str();
    }

    void Core::calc_Warnings() 
    {
        std::vector<double> values = {0.0, 0.0};

        core_Warnings.warning_FluxDensity(values);
        core_Warnings.warning_FluxDensityMin(values);
        core_Warnings.warning_TotalStackMax(values);
        core_Warnings.warning_RatioSteelWidthTotalStack(values);
        core_Warnings.warning_MaxLength(values);
        core_Warnings.warning_MaxHeight(values);
    }

    void Core::calc_PadmountLvBusWork(home_Device* device, home_Winding* windingLv, int numberPhases) 
    {
        if (numberPhases == 1) return;
    
        double smallWindowWidth = coreFields["dSmallWindowWidth"].get<double>();
        double stackPerLoop     = coreFields["dStackPerLoop"].get<double>();
        double largeWindowWidth = coreFields["dLargeWindowWidth"].get<double>();
        double lineAmpsLv       = windingLv->windingFields["pdLineAmp"].get<std::vector<double>>()[windingLv->get_NominalPosition()];

        device->deviceFields["dBus1Length"].set(stackPerLoop * 2.0 + smallWindowWidth);
        device->deviceFields["dBus2Length"].set(stackPerLoop * 4.0 + smallWindowWidth + largeWindowWidth);
        device->deviceFields["dBus3Length"].set(stackPerLoop * 6.0 + smallWindowWidth + largeWindowWidth * 2.0);
        device->deviceFields["dBusTotalLength"].set(device->deviceFields["dBus1Length"].get<double>() 
                                                  + device->deviceFields["dBus2Length"].get<double>() 
                                                  + device->deviceFields["dBus3Length"].get<double>());
        device->deviceFields["dBusTotalLengthFeet"].set(device->deviceFields["dBusTotalLength"].get<double>() / 12.0);

        double result = 10.0 * (((lineAmpsLv / 1000.0) < 1.0 ? 7.0 : 1.2) * std::pow(lineAmpsLv / 1000.0, 2.6));

        device->deviceFields["dBusWatts/Feet"].set(result);
        device->deviceFields["dBusTotalWatts"].set(device->deviceFields["dBusTotalLengthFeet"].get<double>() * result);
    }

    void Core::calc_AmHB1NegativeTolerances(home_Device* device, int numberPhases) 
    {
        if (numberPhases == 1) return;

        auto laminationType = coreFields["laminationType"].get<home_LaminationType>();

        if (laminationType != home_LaminationType::Am && laminationType != home_LaminationType::AmHB1) return;

        double stripWidth       = coreFields["dStripWidth"].get<double>();
        double smallWindowWidth = coreFields["dSmallWindowWidth"].get<double>();
        double windowHeight     = coreFields["dWindowHeight"].get<double>();
        double stackPerLoop     = coreFields["dStackPerLoop"].get<double>();
        double totalStack       = coreFields["totalStack"].get<double>();
        double largeWindowWidth = coreFields["dLargeWindowWidth"].get<double>();
        double stackMin         = coreFields["dStackTolMIn"].get<double>();
        double kSpaceFactor     = table_MaterialConstants.tableData[6][0];
        double temp             = std::ceil((stripWidth * (stackPerLoop - stackMin) * kSpaceFactor * 645.16 / 100.0 - stripWidth * 0.13207) / (stripWidth * 0.1829) - 7.0) + 7.0;
        
        auto smallLoop = calc_Loop(smallWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor, stackMin);
        auto largeLoop = calc_Loop(largeWindowWidth, windowHeight, stripWidth, stackPerLoop, kSpaceFactor, stackMin);

        double totalWidth = ((temp * 15.0 * (smallLoop[0] + smallLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733 * 2.20462 * 2.0) 
                          + ((temp * 15.0 * (largeLoop[0] + largeLoop[1]) * stripWidth * 0.60922) / 1000.0 * 0.00733 * 2.20462 * 2.0);

        double cSArea = ((totalStack - 2.0 * stackMin) * stripWidth * kSpaceFactor);

        double fluxDensityRated = device->guarantee->guaranteeFields["dNL_pu"].get<double>() 
                                * (device->deviceFields["dVxT"].get<double>() * 60.0 / device->deviceFields["frequency"].get<double>()) 
                                / (0.002664 * cSArea) 
                                / 64.516;
        
        coreFields["dFluxDens100V"].set((device->guarantee->guaranteeFields["dNL_pu"].get<double>() != 1.0) 
                                       ? (1.0 / device->guarantee->guaranteeFields["dNL_pu"].get<double>()) * fluxDensityRated 
                                       : (1.0 + coreFields["dPerOvervoltageMaxFlux"].get<double>() / 100.0) * fluxDensityRated);

        coreFields["dNLL"].set(((3.0983 * std::pow(fluxDensityRated, 6) 
                               - 23.862 * std::pow(fluxDensityRated, 5) 
                               + 76.516 * std::pow(fluxDensityRated, 4) 
                               - 130.51 * std::pow(fluxDensityRated, 3) 
                               + 124.76 * std::pow(fluxDensityRated, 2) 
                               - 63.218 * fluxDensityRated + 13.272) 
                              * (1.0 + device->deviceFields["amorphousLossVariance"].get<double>() / 100.0) 
                              * (1.0 + device->deviceFields["dDestructionFactor"].get<double>() / 100.0) 
                              * device->deviceFields["freqFactorHz"].get<double>() 
                              * (1.0 + device->deviceFields["lossFactor"].get<double>() / 100.0)) 
                              * totalWidth);
    }

    void Core::calc_AreaCoreOpt() 
    {
        double steelWidthE    = coreFields["dSteelWidth"].get<double>();
        int    iTotalStackE   = coreFields["iTotalStack"].get<int>();

        double dTotalStackCalc = Config::safe_Division(steelWidthE, static_cast<double>(iTotalStackE) / 100.0);
        auto   list_totalstack = get_ListTotalStack(steelWidthE);

        double best            = list_totalstack[0];
        double best_diff       = std::abs(best - dTotalStackCalc);

        for (size_t i = 1; i < list_totalstack.size(); ++i) 
        {
            double diff = std::abs(list_totalstack[i] - dTotalStackCalc);

            if (diff < best_diff) 
            {
                best      = list_totalstack[i];
                best_diff = diff;
            }
        }

        coreFields["totalStack"].set(best);
        double result = Config::convert_Inch_CM(best, 1) * Config::convert_Inch_CM(steelWidthE, 1) * table_MaterialConstants.tableData[6][0] * 100.0;
        coreFields["dArea"].set(result);
    }

    std::vector<double> Core::get_ListTotalStack(double steelWidthE) 
    {
        auto   laminationType = coreFields["laminationType"].get<home_LaminationType>();
        double dLimMax        = (laminationType == home_LaminationType::Am || laminationType == home_LaminationType::AmHB1) ? 2.7 : 2.3;
        double min_val        = steelWidthE / dLimMax;
        double max_val        = steelWidthE / 1.2;

        int iLowLim  = std::max(1, static_cast<int>(std::floor(min_val)));
        int iHighLim = std::min(15, static_cast<int>(std::ceil(max_val)));

        std::vector<double> list_total;
        std::vector<double> steps = {0.0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875};

        for (int i = iLowLim; i <= iHighLim; ++i) 
        {
            for (double s : steps) 
            {
                double v = static_cast<double>(i) + s;
                if (v >= min_val && v <= max_val) list_total.push_back(v);
            }
        }

        return list_total;
    }

    std::vector<double> Core::get_ListTotalStackOpt(home_Device* device, double steelWidth)
    {
        double limitMax  = device->deviceFields["iTotalStackMax"].get<double>() / 100.0;
        double limitMin  = device->deviceFields["iTotalStackMin"].get<double>() / 100.0;
        double minValue  = steelWidth / limitMax;
        double maxValue  = steelWidth / limitMin;

        int lowLimit  = std::max(1, static_cast<int>(std::floor(minValue)));
        int highLimit = std::min(15, static_cast<int>(std::ceil(maxValue)));

        std::vector<double> list_total;
        std::vector<double> steps = {0.0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875};

        for (int i = lowLimit; i <= highLimit; ++i) 
        {
            for (double step : steps) 
            {
                double value = static_cast<double>(i) + step;

                if (value >= minValue && value <= maxValue) { list_total.push_back(value); }
            }
        }

        if (coreFields["laminationType"].get<home_LaminationType>() == home_LaminationType::Am || 
            coreFields["laminationType"].get<home_LaminationType>() == home_LaminationType::AmHB1) 
        {
            list_total.erase(std::remove_if(list_total.begin(), list_total.end(), [](double x) { return x > 9.0; }), list_total.end());
        }

        return list_total;
    }
}