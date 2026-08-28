/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Core.h
|    @brief     Specializing geometric, electromagnetic, & weight optimization.
|    @brief     Transformer::Components     is "Physical Hardware"
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
#include "materials.h"
#include "core_Warnings.h"
#include "sap_Tables.h"


namespace Transformer::Components
{
    class Winding;
    class Core
    {
        private:
            static int id_Counter;

        public:
            std::map<std::string, home_FieldValue> coreFields;
            
            home_Device*                                device;
            home_MaterialConstants                      table_MaterialConstants;
            Commercial::CoreFactor                      coreFactor;
            Commercial::AmHB1CoreStripStandardWidths    amHB1CoreStripStandardWidths;
            home_CoreWarnings                           core_Warnings;

            Commercial::InductionTable table_Ind90{0.90};
            Commercial::InductionTable table_Ind95{0.95};
            Commercial::InductionTable table_Ind100{1.00};
            Commercial::InductionTable table_Ind105{1.05};
            Commercial::InductionTable table_Ind110{1.10};
            Commercial::InductionTable table_Ind115{1.15};
            Commercial::InductionTable table_Ind120{1.20};

            std::map    <std::string, double>     initialFieldValues;
            std::vector <std::vector <double>>    pressBoardPacking;

            explicit Core (home_Device* device = nullptr, home_MaterialConstants materialConstants = {});

/*                                    _______________________________________________
|                                    |                                               |
|                                    |                F U N C T I O N S              |
|                                    |_______________________________________________|*/

            home_LaminationType type_Lamination() const;
            home_Device*        getParentDevice() const;

            void reset_Fields();
            int  get_LaminationNumber() const;
            void erase_Warnings();
            void calc_Core();
            void calc_LoopCoreStrip            (int numberPhases);
            void calc_SteelWidth               (int numberPhases);
            void calc_AreaCore();
            void calc_DensityLimit             (home_Device* device);
            void calc_Density                  (home_Device* device);
            void calc_ValidationCore();
            void calc_FormNose                 (home_Device* device, home_Winding* windingHv,     int numberPhases);
            void calc_FormSide                 (home_Device* device, home_Winding* windingLv,     int numberPhases);
            void calc_Forms();
            void calc_TrueOverCoil             (home_Device* device, home_Winding* windingLv,     int numberPhases);
            void calc_CoreDimensions           (home_Device* device, home_Winding* windingHv,     home_Winding* windingLv, int numberPhases);
            void calc_WindowHeight             (home_Device* device, int numberPhases);
            void calc_SmallWindowWidth         (home_Device* device, home_Winding* windingHv,     home_Winding* windingLv, int numberPhases);
            void calc_LargeWindowWidth         (home_Device* device, home_Winding* windingHv,     home_Winding* windingLv, int numberPhases);
            void calc_StackPerLoop();
            void calc_StripWidth               (int numberPhases);
            void calc_EBottom                  (int numberPhases);
            void calc_OverallCoreHeight        (int numberPhases);
            void calc_CoreLengthWithoutPB      (int numberPhases);
            void calc_CoreLengthWithPB();
            void calc_NoLoadLossDataArray      (home_Device* device);
            void calc_DestructionFactors();
            void calc_CommonFact               (home_Device* device, int numberPhases);
            void calc_NoLoadLossDataFre        (home_Device* device, int numberPhases);
            void calc_CoreTolerance();
            void calc_WattsLb                  (home_Device* device);
            void calc_LegCenters               (int numberPhases);    
            void calc_Weights                  (int numberPhases);
            void calc_ClampInsulationWeight();
            void calc_PressboardPacking        (home_Device* device, home_Winding* windingHv,      home_Winding* windingLv, int numberPhases);
            void calc_FormDimensions();
            void calc_WeightPressBoard         (home_Device* device);
            void calc_DuctMaterial             (home_Device* device, home_Winding* windingHv,      home_Winding* windingLv, int numberPhases);
            void calc_NoLoadLosses             (home_Device* device, int numberPhases);
            void calc_CurrExc                  (home_Device* device, int numberPhases);
            void calc_StringRated              (int numberPhases);
            void calc_Warnings();
            void calc_PadmountLvBusWork        (home_Device* device, home_Winding* windingLv,      int numberPhases);
            void calc_AmHB1NegativeTolerances  (home_Device* device, int numberPhases);
            void calc_AreaCoreOpt();

            double  calc_FactAmHB1             (double fluxDensityVoltTAux, home_Device* device);
            double  calc_FactAm                (double fluxDensityVoltTAux, home_Device* device);
            double  calc_Factor                (double fluxDensityVoltTAux);
            double  found_Table
            (
                const std::vector<std::vector<double>>& table, 
                double  fluxDensity, 
                int     index, 
                double  amorphousLossVariance   = 0.0, 
                double  freqFactorHz            = 0.0, 
                double  lossFact                = 0.0, 
                bool    b3PhasesAmorphous       = false
            );
            static double calc_CoreWeight
            (
                double              coreSWW, 
                double              coreWinHgt, 
                double              coreStackLoop, 
                double              coreStrip, 
                double              kSpaceFactor, 
                home_LaminationType coreGrade 
            );

            std::vector<double> get_ListTotalStack      (double steelWidth);
            std::vector<double> get_ListTotalStackOpt   (home_Device* device, double steelWidth);
            std::vector<double> calc_Loop
            (
                double smallWindowLargeWidth, 
                double windowHeight, 
                double stripWidth, 
                double stackPerLoop, 
                double kSpaceFactor, 
                double dStackTolIn = 0.0
            );

            std::string calc_sPart();
            std::pair<double, double> find_Headers      (double value); 

/*                                    _______________________________________________
|                                    |                                               |
|                                    |                G E T T E R S                  |
|                                    |_______________________________________________|*/

            bool getTIsolatedCore() const           { return coreFields.at("tIsolatedCore")         .get<bool>(); }
            
            int getId() const                       { return coreFields.at("id")                    .get<int>(); }
            int getTLaminationType() const          { return coreFields.at("tLaminationType")       .get<int>(); }
            int getITotalStack() const              { return coreFields.at("iTotalStack")           .get<int>(); }
            int getINoLoops() const                 { return coreFields.at("iNoLoops")              .get<int>(); }
            
            double getDTotalStack() const           { return coreFields.at("dTotalStack")           .get<double>(); }
            double getDSteelWidth() const           { return coreFields.at("dSteelWidth")           .get<double>(); }
            double getDPerOvervoltageMaxFlux() const{ return coreFields.at("dPerOvervoltageMaxFlux").get<double>(); }
            double getDTestInductionLevel() const   { return coreFields.at("dTestInductionLevel")   .get<double>(); }
            double getDMaxWattsPound() const        { return coreFields.at("dMaxWattsPound")        .get<double>(); }
            double getDLossFact() const             { return coreFields.at("dLossFact")             .get<double>(); }
            double getDInitialFluxDensity() const   { return coreFields.at("dInitialFluxDensity")   .get<double>(); }
            double getDFluxDensityRatPer() const    { return coreFields.at("dFluxDensityRatPer")    .get<double>(); }
            double getDFluxDensityVolt() const      { return coreFields.at("dFluxDensityVolt")      .get<double>(); }
            double getDFluxDensityRatPerT() const   { return coreFields.at("dFluxDensityRatPerT")   .get<double>(); }
            double getDFluxDensityVoltT() const     { return coreFields.at("dFluxDensityVoltT")     .get<double>(); }
            double getBLoop1AmCoreStrip() const     { return coreFields.at("bLoop1AmCoreStrip")     .get<double>(); }
            double getBLoop2AmCoreStrip() const     { return coreFields.at("bLoop2AmCoreStrip")     .get<double>(); }
            double getDWindowHeight() const         { return coreFields.at("dWindowHeight")         .get<double>(); }
            double getDSmallWindowWidth() const     { return coreFields.at("dSmallWindowWidth")     .get<double>(); }
            double getDLargeWindowWidth() const     { return coreFields.at("dLargeWindowWidth")     .get<double>(); }
            double getDStackPerLoop() const         { return coreFields.at("dStackPerLoop")         .get<double>(); }
            double getDStripWidth() const           { return coreFields.at("dStripWidth")           .get<double>(); }
            double getDEBottom() const              { return coreFields.at("dEBottom")              .get<double>(); }
            double getDStackCoreSmLoops() const     { return coreFields.at("dStackCoreSmLoops")     .get<double>(); }
            double getDStackCoreLgLoops() const     { return coreFields.at("dStackCoreLgLoops")     .get<double>(); }
            double getDOverallCoreHeight() const    { return coreFields.at("dOverallCoreHeight")    .get<double>(); }
            double getDOverallCoreHeightInner()const{ return coreFields.at("dOverallCoreHeightInner").get<double>(); }
            double getDCoreLengthWithoutPB() const  { return coreFields.at("dCoreLengthWithoutPB")  .get<double>(); }
            double getDCoreLengthWithPB() const     { return coreFields.at("dCoreLengthWithPB")     .get<double>(); }
            double getDArea() const                 { return coreFields.at("dArea")                 .get<double>(); }
            double getDInductionLevel() const       { return coreFields.at("dInductionLevel")       .get<double>(); }
            double getDFluxDensityIni() const       { return coreFields.at("dFluxDensityIni")       .get<double>(); }
            double getDFluxDensityLimitMax() const  { return coreFields.at("dFluxDensityLimitMax")  .get<double>(); }
            double getDFormNose() const             { return coreFields.at("dFormNose")             .get<double>(); }
            double getDFormSide() const             { return coreFields.at("dFormSide")             .get<double>(); }
            double getDTrueOverCoil() const         { return coreFields.at("dTrueOverCoil")         .get<double>(); }
            double getDLegCenters() const           { return coreFields.at("dLegCenters")           .get<double>(); }
            double getDWeightOneSmLoop() const      { return coreFields.at("dWeightOneSmLoop")      .get<double>(); }
            double getDWeightOneLgLoop() const      { return coreFields.at("dWeightOneLgLoop")      .get<double>(); }
            double getDWeightOneLoop() const        { return coreFields.at("dWeightOneLoop")        .get<double>(); }
            double getDWeightOneSmLoopOther() const { return coreFields.at("dWeightOneSmLoopOther") .get<double>(); }
            double getDWeightOneLgLoopOther() const { return coreFields.at("dWeightOneLgLoopOther") .get<double>(); }
            double getDWeightOneLoopOther() const   { return coreFields.at("dWeightOneLoopOther")   .get<double>(); }
            double getDWeightTwoSmLoops() const     { return coreFields.at("dWeightTwoSmLoops")     .get<double>(); }
            double getDWeightTwoLgLoops() const     { return coreFields.at("dWeightTwoLgLoops")     .get<double>(); }
            double getDWeightTwoLoops() const       { return coreFields.at("dWeightTwoLoops")       .get<double>(); }
            double getDWeights1_8() const           { return coreFields.at("dWeights1_8")           .get<double>(); }
            double getDWeights1_16() const          { return coreFields.at("dWeights1_16")          .get<double>(); }
            double getDWeightsPB1_8() const         { return coreFields.at("dWeightsPB1_8")         .get<double>(); }
            double getDWeightsPB1_16() const        { return coreFields.at("dWeightsPB1_16")        .get<double>(); }
            double getDWeightsPBHD_1_4() const      { return coreFields.at("dWeightsPBHD_1_4")      .get<double>(); }
            double getDWeightPressBoard() const     { return coreFields.at("dWeightPressBoard")     .get<double>(); }
            double getDPh_LimbClearance() const     { return coreFields.at("dPh_LimbClearance")     .get<double>(); }
            double getDPh_PhClearance() const       { return coreFields.at("dPh_PhClearance")       .get<double>(); }
            double getDLamFactor() const            { return coreFields.at("dLamFactor")            .get<double>(); }
            double getDStackTolPIn() const          { return coreFields.at("dStackTolPIn")          .get<double>(); }
            double getDStackTolMIn() const          { return coreFields.at("dStackTolMIn")          .get<double>(); }
            double getDNLL() const                  { return coreFields.at("dNLL")                  .get<double>(); }
            double getDFluxDens100V() const         { return coreFields.at("dFluxDens100V")         .get<double>(); }
            double getDFormDimA() const             { return coreFields.at("dFormDimA")             .get<double>(); }
            double getDFormDimS() const             { return coreFields.at("dFormDimS")             .get<double>(); }
            double getDFormDimN() const             { return coreFields.at("dFormDimN")             .get<double>(); }
            double getDFormDimW() const             { return coreFields.at("dFormDimW")             .get<double>(); }
            double getDTotalFormLength() const      { return coreFields.at("dTotalFormLength")      .get<double>(); }
            double getDDestructionFactorAm() const  { return coreFields.at("dDestructionFactorAm")  .get<double>(); }
            double getDDestructionFactorReg() const { return coreFields.at("dDestructionFactorReg") .get<double>(); }
            double getDCommonFact() const           { return coreFields.at("dCommonFact")           .get<double>(); }
            double getDWattsLb() const              { return coreFields.at("dWattsLb")              .get<double>(); }
            double getDWidthClampIns() const        { return coreFields.at("dWidthClampIns")        .get<double>(); }
            double getDHeightClampIns() const       { return coreFields.at("dHeightClampIns")       .get<double>(); }
            double getDWeightClampIns() const       { return coreFields.at("dWeightClampIns")       .get<double>(); }
            double getDWindingHeightAdder() const   { return coreFields.at("dWindingHeightAdder")   .get<double>(); }
            double getDWindingWidthAdder() const    { return coreFields.at("dWindingWidthAdder")    .get<double>(); }
            double getDCoilCorePBEnd() const        { return coreFields.at("dCoilCorePBEnd")        .get<double>(); }
            double getDCoilCoilPBSides() const      { return coreFields.at("dCoilCoilPBSides")      .get<double>(); }
            double getDCoilCorePBSides() const      { return coreFields.at("dCoilCorePBSides")      .get<double>(); }
            double getDFixedSmWindowWidth() const   { return coreFields.at("dFixedSmWindowWidth")   .get<double>(); }
            double getDFixedWindowHeight() const    { return coreFields.at("dFixedWindowHeight")    .get<double>(); }
            double getDFixedWindowLarge() const     { return coreFields.at("dFixedWindowLarge")     .get<double>(); }
            double getDOverCoilAdder() const        { return coreFields.at("dOverCoilAdder")        .get<double>(); }
            
            std::string getSRatedPU() const         { return coreFields.at("sRatedPU")              .get<std::string>(); }
            std::string getSRatedV() const          { return coreFields.at("sRatedV")               .get<std::string>(); }
            std::string getName() const             { return coreFields.at("name")                  .get<std::string>(); }
            std::string getSDrawing() const         { return coreFields.at("sDrawing")              .get<std::string>(); }
            std::string getSArea() const            { return coreFields.at("sArea")                 .get<std::string>(); }
            std::string getSForms() const           { return coreFields.at("sForms")                .get<std::string>(); }
            std::string getSSheetComments() const   { return coreFields.at("sSheetComments")        .get<std::string>(); }
            std::string getSCoreSuffix() const      { return coreFields.at("sCoreSuffix")           .get<std::string>(); }
            
            std::vector<int> getTErrorEnum() const  { return coreFields.at("tErrorEnum").get<std::vector<int>>(); }
            
            std::vector<double> getD_NLLossDataFre() const { return coreFields.at("d_NLLossDataFre").get<std::vector<double>>(); }
            std::vector<double> getD_ThickDM() const       { return coreFields.at("d_ThickDM")      .get<std::vector<double>>(); }
            std::vector<double> getD_WidthDM() const       { return coreFields.at("d_WidthDM")      .get<std::vector<double>>(); }
            std::vector<double> getD_RollLengthDM() const  { return coreFields.at("d_RollLengthDM") .get<std::vector<double>>(); }
            std::vector<double> getD_8FtLengthsDM() const  { return coreFields.at("d_8FtLengthsDM") .get<std::vector<double>>(); }
            std::vector<double> getD_DuctsDM() const       { return coreFields.at("d_DuctsDM")      .get<std::vector<double>>(); }
            std::vector<double> getD_IExc() const          { return coreFields.at("d_IExc")         .get<std::vector<double>>(); }
            std::vector<double> getD_NoLoadLosses20() const{ return coreFields.at("d_NoLoadLosses20").get<std::vector<double>>(); }
            std::vector<double> getD_NoLoadLosses75() const{ return coreFields.at("d_NoLoadLosses75").get<std::vector<double>>(); }
            std::vector<double> getD_NoLoadLosses85() const{ return coreFields.at("d_NoLoadLosses85").get<std::vector<double>>(); }

            std::vector<std::string> getD_CoreTolerance() const { return coreFields.at("d_CoreTolerance").get<std::vector<std::string>>(); }
            std::vector<std::string> getS_LocationDM() const    { return coreFields.at("s_LocationDM")   .get<std::vector<std::string>>(); }
            std::vector<std::string> getSDuctMaterial() const   { return coreFields.at("sDuctMaterial")  .get<std::vector<std::string>>(); }
            std::vector<std::string> getSErrorName() const      { return coreFields.at("sErrorName").get<std::vector<std::string>>(); }
            std::vector<std::string> getSErrorMessages() const  { return coreFields.at("sErrorMessages").get<std::vector<std::string>>(); }

/*                                    _______________________________________________
|                                    |                                               |
|                                    |                S E T T E R S                  |
|                                    |_______________________________________________|*/

            void setId(int value)                           { coreFields["id"]                      = home_FieldValue(value); }
            void setTLaminationType(int value)              { coreFields["tLaminationType"]         = home_FieldValue(value); }
            void setITotalStack(int value)                  { coreFields["iTotalStack"]             = home_FieldValue(value); }
            void setINoLoops(int value)                     { coreFields["iNoLoops"]                = home_FieldValue(value); }

            void setDTotalStack(double value)               { coreFields["dTotalStack"]             = home_FieldValue(value); }
            void setDSteelWidth(double value)               { coreFields["dSteelWidth"]             = home_FieldValue(value); }
            void setDPerOvervoltageMaxFlux(double value)    { coreFields["dPerOvervoltageMaxFlux"]  = home_FieldValue(value); }
            void setDTestInductionLevel(double value)       { coreFields["dTestInductionLevel"]     = home_FieldValue(value); }
            void setDMaxWattsPound(double value)            { coreFields["dMaxWattsPound"]          = home_FieldValue(value); }
            void setDLossFact(double value)                 { coreFields["dLossFact"]               = home_FieldValue(value); }
            void setDInductionLevel(double value)           { coreFields["dInductionLevel"]         = home_FieldValue(value); }
            void setTIsolatedCore(bool value)               { coreFields["tIsolatedCore"]           = home_FieldValue(value); }
            void setDFluxDensityIni(double value)           { coreFields["dFluxDensityIni"]         = home_FieldValue(value); }
            void setDFluxDensityLimitMax(double value)      { coreFields["dFluxDensityLimitMax"]    = home_FieldValue(value); }
            void setDInitialFluxDensity(double value)       { coreFields["dInitialFluxDensity"]     = home_FieldValue(value); }
            void setDFluxDensityRatPer(double value)        { coreFields["dFluxDensityRatPer"]      = home_FieldValue(value); }
            void setDFluxDensityVolt(double value)          { coreFields["dFluxDensityVolt"]        = home_FieldValue(value); }
            void setDFluxDensityRatPerT(double value)       { coreFields["dFluxDensityRatPerT"]     = home_FieldValue(value); }
            void setDFluxDensityVoltT(double value)         { coreFields["dFluxDensityVoltT"]       = home_FieldValue(value); }
            void setBLoop1AmCoreStrip(double value)         { coreFields["bLoop1AmCoreStrip"]       = home_FieldValue(value); }
            void setBLoop2AmCoreStrip(double value)         { coreFields["bLoop2AmCoreStrip"]       = home_FieldValue(value); }
            void setDWindowHeight(double value)             { coreFields["dWindowHeight"]           = home_FieldValue(value); }
            void setDSmallWindowWidth(double value)         { coreFields["dSmallWindowWidth"]       = home_FieldValue(value); }
            void setDLargeWindowWidth(double value)         { coreFields["dLargeWindowWidth"]       = home_FieldValue(value); }
            void setDStackPerLoop(double value)             { coreFields["dStackPerLoop"]           = home_FieldValue(value); }
            void setDStripWidth(double value)               { coreFields["dStripWidth"]             = home_FieldValue(value); }
            void setDEBottom(double value)                  { coreFields["dEBottom"]                = home_FieldValue(value); }
            void setDStackCoreSmLoops(double value)         { coreFields["dStackCoreSmLoops"]       = home_FieldValue(value); }
            void setDStackCoreLgLoops(double value)         { coreFields["dStackCoreLgLoops"]       = home_FieldValue(value); }
            void setDOverallCoreHeight(double value)        { coreFields["dOverallCoreHeight"]      = home_FieldValue(value); }
            void setDOverallCoreHeightInner(double value)   { coreFields["dOverallCoreHeightInner"] = home_FieldValue(value); }
            void setDCoreLengthWithoutPB(double value)      { coreFields["dCoreLengthWithoutPB"]    = home_FieldValue(value); }
            void setDCoreLengthWithPB(double value)         { coreFields["dCoreLengthWithPB"]       = home_FieldValue(value); }
            void setDArea(double value)                     { coreFields["dArea"]                   = home_FieldValue(value); }
            void setDFormNose(double value)                 { coreFields["dFormNose"]               = home_FieldValue(value); }
            void setDFormSide(double value)                 { coreFields["dFormSide"]               = home_FieldValue(value); }
            void setDTrueOverCoil(double value)             { coreFields["dTrueOverCoil"]           = home_FieldValue(value); }
            void setDLegCenters(double value)               { coreFields["dLegCenters"]             = home_FieldValue(value); }
            void setDWeightOneSmLoop(double value)          { coreFields["dWeightOneSmLoop"]        = home_FieldValue(value); }
            void setDWeightOneLgLoop(double value)          { coreFields["dWeightOneLgLoop"]        = home_FieldValue(value); }
            void setDWeightOneLoop(double value)            { coreFields["dWeightOneLoop"]          = home_FieldValue(value); }
            void setDWeightOneSmLoopOther(double value)     { coreFields["dWeightOneSmLoopOther"]   = home_FieldValue(value); }
            void setDWeightOneLgLoopOther(double value)     { coreFields["dWeightOneLgLoopOther"]   = home_FieldValue(value); }
            void setDWeightOneLoopOther(double value)       { coreFields["dWeightOneLoopOther"]     = home_FieldValue(value); }
            void setDWeightTwoSmLoops(double value)         { coreFields["dWeightTwoSmLoops"]       = home_FieldValue(value); }
            void setDWeightTwoLgLoops(double value)         { coreFields["dWeightTwoLgLoops"]       = home_FieldValue(value); }
            void setDWeightTwoLoops(double value)           { coreFields["dWeightTwoLoops"]         = home_FieldValue(value); }
            void setDWeights1_8(double value)               { coreFields["dWeights1_8"]             = home_FieldValue(value); }
            void setDWeights1_16(double value)              { coreFields["dWeights1_16"]            = home_FieldValue(value); }
            void setDWeightsPB1_8(double value)             { coreFields["dWeightsPB1_8"]           = home_FieldValue(value); }
            void setDWeightsPB1_16(double value)            { coreFields["dWeightsPB1_16"]          = home_FieldValue(value); }
            void setDWeightsPBHD_1_4(double value)          { coreFields["dWeightsPBHD_1_4"]        = home_FieldValue(value); }
            void setDWeightPressBoard(double value)         { coreFields["dWeightPressBoard"]       = home_FieldValue(value); }
            void setDPh_LimbClearance(double value)         { coreFields["dPh_LimbClearance"]       = home_FieldValue(value); }
            void setDPh_PhClearance(double value)           { coreFields["dPh_PhClearance"]         = home_FieldValue(value); }
            void setDLamFactor(double value)                { coreFields["dLamFactor"]              = home_FieldValue(value); }
            void setDStackTolPIn(double value)              { coreFields["dStackTolPIn"]            = home_FieldValue(value); }
            void setDStackTolMIn(double value)              { coreFields["dStackTolMIn"]            = home_FieldValue(value); }
            void setDNLL(double value)                      { coreFields["dNLL"]                    = home_FieldValue(value); }
            void setDFluxDens100V(double value)             { coreFields["dFluxDens100V"]           = home_FieldValue(value); }
            void setDFormDimA(double value)                 { coreFields["dFormDimA"]               = home_FieldValue(value); }
            void setDFormDimS(double value)                 { coreFields["dFormDimS"]               = home_FieldValue(value); }
            void setDFormDimN(double value)                 { coreFields["dFormDimN"]               = home_FieldValue(value); }
            void setDFormDimW(double value)                 { coreFields["dFormDimW"]               = home_FieldValue(value); }
            void setDTotalFormLength(double value)          { coreFields["dTotalFormLength"]        = home_FieldValue(value); }
            void setDDestructionFactorAm(double value)      { coreFields["dDestructionFactorAm"]    = home_FieldValue(value); }
            void setDDestructionFactorReg(double value)     { coreFields["dDestructionFactorReg"]   = home_FieldValue(value); }
            void setDCommonFact(double value)               { coreFields["dCommonFact"]             = home_FieldValue(value); }
            void setDWattsLb(double value)                  { coreFields["dWattsLb"]                = home_FieldValue(value); }
            void setDWidthClampIns(double value)            { coreFields["dWidthClampIns"]          = home_FieldValue(value); }
            void setDHeightClampIns(double value)           { coreFields["dHeightClampIns"]         = home_FieldValue(value); }
            void setDWeightClampIns(double value)           { coreFields["dWeightClampIns"]         = home_FieldValue(value); }
            void setDWindingHeightAdder(double value)       { coreFields["dWindingHeightAdder"]     = home_FieldValue(value); }
            void setDWindingWidthAdder(double value)        { coreFields["dWindingWidthAdder"]      = home_FieldValue(value); }
            void setDCoilCorePBEnd(double value)            { coreFields["dCoilCorePBEnd"]          = home_FieldValue(value); }
            void setDCoilCoilPBSides(double value)          { coreFields["dCoilCoilPBSides"]        = home_FieldValue(value); }
            void setDCoilCorePBSides(double value)          { coreFields["dCoilCorePBSides"]        = home_FieldValue(value); }
            void setDFixedSmWindowWidth(double value)       { coreFields["dFixedSmWindowWidth"]     = home_FieldValue(value); }
            void setDFixedWindowHeight(double value)        { coreFields["dFixedWindowHeight"]      = home_FieldValue(value); }
            void setDFixedWindowLarge(double value)         { coreFields["dFixedWindowLarge"]       = home_FieldValue(value); }
            void setDOverCoilAdder(double value)            { coreFields["dOverCoilAdder"]          = home_FieldValue(value); }

            void setName(const std::string& value)          { coreFields["name"]                    = home_FieldValue(value); }
            void setSDrawing(const std::string& value)      { coreFields["sDrawing"]                = home_FieldValue(value); }
            void setSRatedPU(const std::string& value)      { coreFields["sRatedPU"]                = home_FieldValue(value); }
            void setSRatedV(const std::string& value)       { coreFields["sRatedV"]                 = home_FieldValue(value); }
            void setSArea(const std::string& value)         { coreFields["sArea"]                   = home_FieldValue(value); }
            void setSForms(const std::string& value)        { coreFields["sForms"]                  = home_FieldValue(value); }
            void setSSheetComments(const std::string& value){ coreFields["sSheetComments"]          = home_FieldValue(value); }
            void setSCoreSuffix(const std::string& value)   { coreFields["sCoreSuffix"]             = home_FieldValue(value); }

            void setD_CoreTolerance(const std::vector<std::string>& value) { coreFields["d_CoreTolerance"] = home_FieldValue(value); }
            void setS_LocationDM(const std::vector<std::string>& value)    { coreFields["s_LocationDM"]    = home_FieldValue(value); }
            void setSDuctMaterial(const std::vector<std::string>& value)   { coreFields["sDuctMaterial"]   = home_FieldValue(value); }

            void setSErrorName(const std::vector<std::string>& v)   { coreFields["sErrorName"] = home_FieldValue(v); }
            void setTErrorEnum(const std::vector<int>& v)           { coreFields["tErrorEnum"] = home_FieldValue(v); }
            void setSErrorMessages(const std::vector<std::string>& v){ coreFields["sErrorMessages"] = home_FieldValue(v); }
    };
}
// end of Core class