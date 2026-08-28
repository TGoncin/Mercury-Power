/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Losses.h
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

#include "losses_Warnings.h"




namespace Transformer::Physics
{
    class Cooling;
    class Losses
    {
        private:
            static int id_Counter;

        public:
            std::map<std::string, home_FieldValue> lossFields;
            home_Device* device;
            home_MaterialConstants       table_MaterialConstants;
            home_ResistiveMaterial       table_ResistiveMaterial;
            home_ResistivityTable        table_Resistivity;
            home_ResistanceTable         table_Resistance;
            home_LossWarnings            loss_Warnings;

            explicit Losses
            (
                home_Device*                 getParentDevice    = nullptr, 
                home_MaterialConstants       materialConstants  = {}, 
                home_ResistiveMaterial       resistiveMaterial  = {}, 
                home_ResistivityTable        table_Resistivity  = {}, 
                home_ResistanceTable         table_Resistance   = {}
            );

            home_Device*    getParentDevice() const;



            void            reset_LossFields();
            void            erase_Warnings();
            void            calc_Warnings();

            void    calc_CoilStray          (home_Device*   device,     home_Winding* winding_Lv,    int           numberPhases);
            void    calc_TotalStray         (home_Device*   device);
            void    calc_TotalLoss          (home_Device*   device);
            void    calc_CoreLoss           (home_Core*     core,       int           numberPhases);
            void    calc_TotalLoss          (home_Core*     core);
            void    calc_MaxWatts           (home_Device*   device,     home_Winding* winding_Lv,    Cooling*      cooling);
            void    calc_LossToEvaluate     (home_Guarantee*guarantee,  Losses*       losses);
            void    calc_EvaluateLoss       (home_Guarantee*guarantee);
            void    calc_LoadLossWinding    (home_Device*   device,     home_Winding* winding_Hv,    home_Winding* winding_Lv);



            void    calc_Loss               (home_Device*   device,     home_Winding* winding_Hv,    home_Winding* winding_Lv);
            void    calc_TableResistivity   (home_Coil*     coil_Hv,    home_Coil*    coil_Lo);
            void    calc_TableResistance    (home_Device*   device,     home_Winding* winding_Hv,    home_Winding* Winding_LV,  home_Coil*    coil_Hv,     home_Coil* coil_LV);
            void    calc_LeadsBus           (home_Device*   device,     home_Cooling* cooling,       home_Winding* winding_Hv,  home_Winding* winding_Lv,  home_Coil* coil_HV,  home_Coil* coil_LV);
            double  func_TempFactor         (double         temp,       double        materialConstants);
            void    calc_ResistanceAndLoss  (home_Device*   device,     home_Cooling* cooling,       home_Winding* winding_Hv,  home_Winding* winding_Lv);

    /*_______________________________________________
    |                                               |
    |                G E T T E R S                  |
    |_______________________________________________|*/

            double getDLVWOLeadsCurrentTempR() const                { return lossFields.at("dLVWOLeadsCurrentTempR").get<double>(); }
            double getDLVLeadsCurrentTempR() const                  { return lossFields.at("dLVLeadsCurrentTempR").get<double>(); }
            double getDTotalLVCurrentTempR() const                  { return lossFields.at("dTotalLVCurrentTempR").get<double>(); }
            double getDHVWOLeadsCurrentTempR() const                { return lossFields.at("dHVWOLeadsCurrentTempR").get<double>(); }
            double getDHVLeadsCurrentTempR() const                  { return lossFields.at("dHVLeadsCurrentTempR").get<double>(); }
            double getDTotalHVCurrentTempR() const                  { return lossFields.at("dTotalHVCurrentTempR").get<double>(); }
            double getDLVWOLeadsNewTempR() const                    { return lossFields.at("dLVWOLeadsNewTempR").get<double>(); }
            double getDLVLeadsNewTempR() const                      { return lossFields.at("dLVLeadsNewTempR").get<double>(); }
            double getDTotalLVNewTempR() const                      { return lossFields.at("dTotalLVNewTempR").get<double>(); }
            double getDHVWOLeadsNewTempR() const                    { return lossFields.at("dHVWOLeadsNewTempR").get<double>(); }
            double getDHVLeadsNewTempR() const                      { return lossFields.at("dHVLeadsNewTempR").get<double>(); }
            double getDTotalHVNewTempR() const                      { return lossFields.at("dTotalHVNewTempR").get<double>(); }
            double getDLVWOLeadsCurrentTempW() const                { return lossFields.at("dLVWOLeadsCurrentTempW").get<double>(); }
            double getDLVLeadsCurrentTempW() const                  { return lossFields.at("dLVLeadsCurrentTempW").get<double>(); }
            double getDTotalLVCurrentTempW() const                  { return lossFields.at("dTotalLVCurrentTempW").get<double>(); }
            double getDHVWOLeadsCurrentTempW() const                { return lossFields.at("dHVWOLeadsCurrentTempW").get<double>(); }
            double getDHVLeadsCurrentTempW() const                  { return lossFields.at("dHVLeadsCurrentTempW").get<double>(); }
            double getDTotalHVCurrentTempW() const                  { return lossFields.at("dTotalHVCurrentTempW").get<double>(); }
            double getDLVWOLeadsNewTempW() const                    { return lossFields.at("dLVWOLeadsNewTempW").get<double>(); }
            double getDLVLeadsNewTempW() const                      { return lossFields.at("dLVLeadsNewTempW").get<double>(); }
            double getDTotalLVNewTempW() const                      { return lossFields.at("dTotalLVNewTempW").get<double>(); }
            double getDHVWOLeadsNewTempW() const                    { return lossFields.at("dHVWOLeadsNewTempW").get<double>(); }
            double getHvLeadsNewTempW() const                       { return lossFields.at("hvLeadsNewTempW").get<double>(); }
            double getDTotalHVNewTempW() const                      { return lossFields.at("dTotalHVNewTempW").get<double>(); }
            double getDLVWOLeadsCurrentTempTaps() const             { return lossFields.at("dLVWOLeadsCurrentTempTaps").get<double>(); }
            double getDLVLeadsCurrentTempTaps() const               { return lossFields.at("dLVLeadsCurrentTempTaps").get<double>(); }
            double getDTotalLVCurrentTempTaps() const               { return lossFields.at("dTotalLVCurrentTempTaps").get<double>(); }
            double getDHVWOLeadsCurrentTempTaps() const             { return lossFields.at("dHVWOLeadsCurrentTempTaps").get<double>(); }
            double getDHVLeadsCurrentTempTaps() const               { return lossFields.at("dHVLeadsCurrentTempTaps").get<double>(); }
            double getDTotalHVCurrentTempTaps() const               { return lossFields.at("dTotalHVCurrentTempTaps").get<double>(); }
            double getDLVWOLeadsNewTempTaps() const                 { return lossFields.at("dLVWOLeadsNewTempTaps").get<double>(); }
            double getDLVLeadsNewTempTaps() const                   { return lossFields.at("dLVLeadsNewTempTaps").get<double>(); }
            double getDTotalLVNewTempTaps() const                   { return lossFields.at("dTotalLVNewTempTaps").get<double>(); }
            double getDHVWOLeadsNewTempTaps() const                 { return lossFields.at("dHVWOLeadsNewTempTaps").get<double>(); }
            double getDHVLeadsNewTempTaps() const                   { return lossFields.at("dHVLeadsNewTempTaps").get<double>(); }
            double getDTotalHVNewTempTaps() const                   { return lossFields.at("dTotalHVNewTempTaps").get<double>(); }
            double getDI2RCurrentTemp() const                       { return lossFields.at("dI2RCurrentTemp").get<double>(); }
            double getDI2RNewTemp() const                           { return lossFields.at("dI2RNewTemp").get<double>(); }
            double getDI2RCurrentTempTaps() const                   { return lossFields.at("dI2RCurrentTempTaps").get<double>(); }
            double getDI2RNewTempTaps() const                       { return lossFields.at("dI2RNewTempTaps").get<double>(); }
            double getDLVLeadLoss() const                           { return lossFields.at("dLVLeadLoss").get<double>(); }
            double getDTankStray() const                            { return lossFields.at("dTankStray").get<double>(); }
            double getDCoilSray() const                             { return lossFields.at("dCoilSray").get<double>(); }
            double getTotalStray() const                            { return lossFields.at("totalStray").get<double>(); }
            double getDTotalLLossStrayLead() const                  { return lossFields.at("dTotalLLossStrayLead").get<double>(); }
            double getDCoreLoss() const                             { return lossFields.at("dCoreLoss").get<double>(); }
            double getDTotalLoss() const                            { return lossFields.at("dTotalLoss").get<double>(); }
            double getDNoLoadLossCost() const                       { return lossFields.at("dNoLoadLossCost").get<double>(); }
            double getDLoadLossCost() const                         { return lossFields.at("dLoadLossCost").get<double>(); }
            double getDTotalLossCost() const                        { return lossFields.at("dTotalLossCost").get<double>(); }
            std::string getSNoLoadLossCost() const                  { return lossFields.at("sNoLoadLossCost").get<std::string>(); }
            std::string getSLoadLossCost() const                    { return lossFields.at("sLoadLossCost").get<std::string>(); }
            std::string getSTotalLossCost() const                   { return lossFields.at("sTotalLossCost").get<std::string>(); }
            double getDMaxWatts() const                             { return lossFields.at("dMaxWatts").get<double>(); }
            double getDMaxWattsK4() const                           { return lossFields.at("dMaxWattsK4").get<double>(); }
            double getNoLoadLossLosses() const                      { return lossFields.at("noLoadLossLosses").get<double>(); }
            double getLoadLossosses() const                         { return lossFields.at("loadLossosses").get<double>(); }
            int getICoilLdMat() const                               { return lossFields.at("iCoilLdMat").get<int>(); }
            double getAreaLvBus() const                             { return lossFields.at("areaLvBus").get<double>(); }
            double getDFactorLVBus() const                          { return lossFields.at("dFactorLVBus").get<double>(); }
            double getLengthInLvBus() const                         { return lossFields.at("lengthInLvBus").get<double>(); }
            double getDOhmsLVBus() const                            { return lossFields.at("dOhmsLVBus").get<double>(); }
            double getDLossLVBus() const                            { return lossFields.at("dLossLVBus").get<double>(); }
            double getLeadLengthOutOfCoil() const                   { return lossFields.at("leadLengthOutOfCoil").get<double>(); }
            int getIBusLdMat() const                                { return lossFields.at("iBusLdMat").get<int>(); }
            double getAreaBusWork() const                           { return lossFields.at("areaBusWork").get<double>(); }
            double getDFactorBuswork() const                        { return lossFields.at("dFactorBuswork").get<double>(); }
            double getDOhmsBuswork() const                          { return lossFields.at("dOhmsBuswork").get<double>(); }
            double getDLossBuswork() const                          { return lossFields.at("dLossBuswork").get<double>(); }
            double getDTotLdLoss() const                            { return lossFields.at("dTotLdLoss").get<double>(); }
            double getDHVAmpsRegLds() const                         { return lossFields.at("dHVAmpsRegLds").get<double>(); }
            double getDHVLineAmpsRegLds() const                     { return lossFields.at("dHVLineAmpsRegLds").get<double>(); }
            int getICoilLdMatHV() const                             { return lossFields.at("iCoilLdMatHV").get<int>(); }
            double getDAreaHVBus() const                            { return lossFields.at("dAreaHVBus").get<double>(); }
            double getDFactorHVBus() const                          { return lossFields.at("dFactorHVBus").get<double>(); }
            double getDLengthInHVBus() const                        { return lossFields.at("dLengthInHVBus").get<double>(); }
            double getDOhmsHVBus() const                            { return lossFields.at("dOhmsHVBus").get<double>(); }
            double getDLossHVBus() const                            { return lossFields.at("dLossHVBus").get<double>(); }
            double getAreaBusWorkHV() const                         { return lossFields.at("areaBusWorkHV").get<double>(); }
            double getDFactorBusworkHV() const                      { return lossFields.at("dFactorBusworkHV").get<double>(); }
            double getDOhmsBusworkHV() const                        { return lossFields.at("dOhmsBusworkHV").get<double>(); }
            double getDLossBusworkHV() const                        { return lossFields.at("dLossBusworkHV").get<double>(); }
            double getDTotLdLossHV() const                          { return lossFields.at("dTotLdLossHV").get<double>(); }
            double getD20LLwindingLv() const                        { return lossFields.at("d_20LLwindingLv").get<double>(); }
            double getD55LLwindingLv() const                        { return lossFields.at("d_55LLwindingLv").get<double>(); }
            double getD75LLwindingLv() const                        { return lossFields.at("d_75LLwindingLv").get<double>(); }
            double getD85LLwindingLv() const                        { return lossFields.at("d_85LLwindingLv").get<double>(); }
            double getD20LLwindingHv() const                        { return lossFields.at("d_20LLwindingHv").get<double>(); }
            double getD55LLwindingHv() const                        { return lossFields.at("d_55LLwindingHv").get<double>(); }
            double getD75LLwindingHv() const                        { return lossFields.at("d_75LLwindingHv").get<double>(); }
            double getD85LLwindingHv() const                        { return lossFields.at("d_85LLwindingHv").get<double>(); }
            double getD20Total100Load() const                       { return lossFields.at("d20Total100Load").get<double>(); }
            double getD55Total100Load() const                       { return lossFields.at("d55Total100Load").get<double>(); }
            double getD75Total100Load() const                       { return lossFields.at("d75Total100Load").get<double>(); }
            double getD85Total100Load() const                       { return lossFields.at("d85Total100Load").get<double>(); }
            double getD20TotalLR25() const                          { return lossFields.at("d20TotalLR25").get<double>(); }
            double getD55TotalLR25() const                          { return lossFields.at("d55TotalLR25").get<double>(); }
            double getD75TotalLR25() const                          { return lossFields.at("d75TotalLR25").get<double>(); }
            double getD85TotalLR25() const                          { return lossFields.at("d85TotalLR25").get<double>(); }
            double getD20TotalLR50() const                          { return lossFields.at("d20TotalLR50").get<double>(); }
            double getD55TotalLR50() const                          { return lossFields.at("d55TotalLR50").get<double>(); }
            double getD75TotalLR50() const                          { return lossFields.at("d75TotalLR50").get<double>(); }
            double getD85TotalLR50() const                          { return lossFields.at("d85TotalLR50").get<double>(); }
            double getD20TotalLR75() const                          { return lossFields.at("d20TotalLR75").get<double>(); }
            double getD55TotalLR75() const                          { return lossFields.at("d55TotalLR75").get<double>(); }
            double getD75TotalLR75() const                          { return lossFields.at("d75TotalLR75").get<double>(); }
            double getD85TotalLR75() const                          { return lossFields.at("d85TotalLR75").get<double>(); }
            double getD20TotalLR125() const                         { return lossFields.at("d20TotalLR125").get<double>(); }
            double getD55TotalLR125() const                         { return lossFields.at("d55TotalLR125").get<double>(); }
            double getD75TotalLR125() const                         { return lossFields.at("d75TotalLR125").get<double>(); }
            double getD85TotalLR125() const                         { return lossFields.at("d85TotalLR125").get<double>(); }
            double getD20TotalLR133() const                         { return lossFields.at("d20TotalLR133").get<double>(); }
            double getD55TotalLR133() const                         { return lossFields.at("d55TotalLR133").get<double>(); }
            double getD75TotalLR133() const                         { return lossFields.at("d75TotalLR133").get<double>(); }
            double getD85TotalLR133() const                         { return lossFields.at("d85TotalLR133").get<double>(); }
            std::vector<std::string> getSErrorName() const          { return lossFields.at("sErrorName").get<std::vector<std::string>>(); }
            std::vector<home_WarningSeverity> getTErrorEnum() const      { return lossFields.at("tErrorEnum").get<std::vector<home_WarningSeverity>>(); }
            std::vector<std::string> getSErrorMessages() const      { return lossFields.at("sErrorMessages").get<std::vector<std::string>>(); }

    /*_______________________________________________
    |                                               |
    |                S E T T E R S                  |
    |_______________________________________________|*/

            void setDLVWOLeadsCurrentTempR(double v)                { lossFields["dLVWOLeadsCurrentTempR"] = home_FieldValue(v); }
            void setDLVLeadsCurrentTempR(double v)                  { lossFields["dLVLeadsCurrentTempR"] = home_FieldValue(v); }
            void setDTotalLVCurrentTempR(double v)                  { lossFields["dTotalLVCurrentTempR"] = home_FieldValue(v); }
            void setDHVWOLeadsCurrentTempR(double v)                { lossFields["dHVWOLeadsCurrentTempR"] = home_FieldValue(v); }
            void setDHVLeadsCurrentTempR(double v)                  { lossFields["dHVLeadsCurrentTempR"] = home_FieldValue(v); }
            void setDTotalHVCurrentTempR(double v)                  { lossFields["dTotalHVCurrentTempR"] = home_FieldValue(v); }
            void setDLVWOLeadsNewTempR(double v)                    { lossFields["dLVWOLeadsNewTempR"] = home_FieldValue(v); }
            void setDLVLeadsNewTempR(double v)                      { lossFields["dLVLeadsNewTempR"] = home_FieldValue(v); }
            void setDTotalLVNewTempR(double v)                      { lossFields["dTotalLVNewTempR"] = home_FieldValue(v); }
            void setDHVWOLeadsNewTempR(double v)                    { lossFields["dHVWOLeadsNewTempR"] = home_FieldValue(v); }
            void setDHVLeadsNewTempR(double v)                      { lossFields["dHVLeadsNewTempR"] = home_FieldValue(v); }
            void setDTotalHVNewTempR(double v)                      { lossFields["dTotalHVNewTempR"] = home_FieldValue(v); }
            void setDLVWOLeadsCurrentTempW(double v)                { lossFields["dLVWOLeadsCurrentTempW"] = home_FieldValue(v); }
            void setDLVLeadsCurrentTempW(double v)                  { lossFields["dLVLeadsCurrentTempW"] = home_FieldValue(v); }
            void setDTotalLVCurrentTempW(double v)                  { lossFields["dTotalLVCurrentTempW"] = home_FieldValue(v); }
            void setDHVWOLeadsCurrentTempW(double v)                { lossFields["dHVWOLeadsCurrentTempW"] = home_FieldValue(v); }
            void setDHVLeadsCurrentTempW(double v)                  { lossFields["dHVLeadsCurrentTempW"] = home_FieldValue(v); }
            void setDTotalHVCurrentTempW(double v)                  { lossFields["dTotalHVCurrentTempW"] = home_FieldValue(v); }
            void setDLVWOLeadsNewTempW(double v)                    { lossFields["dLVWOLeadsNewTempW"] = home_FieldValue(v); }
            void setDLVLeadsNewTempW(double v)                      { lossFields["dLVLeadsNewTempW"] = home_FieldValue(v); }
            void setDTotalLVNewTempW(double v)                      { lossFields["dTotalLVNewTempW"] = home_FieldValue(v); }
            void setDHVWOLeadsNewTempW(double v)                    { lossFields["dHVWOLeadsNewTempW"] = home_FieldValue(v); }
            void setHvLeadsNewTempW(double v)                       { lossFields["hvLeadsNewTempW"] = home_FieldValue(v); }
            void setDTotalHVNewTempW(double v)                      { lossFields["dTotalHVNewTempW"] = home_FieldValue(v); }
            void setDLVWOLeadsCurrentTempTaps(double v)             { lossFields["dLVWOLeadsCurrentTempTaps"] = home_FieldValue(v); }
            void setDLVLeadsCurrentTempTaps(double v)               { lossFields["dLVLeadsCurrentTempTaps"] = home_FieldValue(v); }
            void setDTotalLVCurrentTempTaps(double v)               { lossFields["dTotalLVCurrentTempTaps"] = home_FieldValue(v); }
            void setDHVWOLeadsCurrentTempTaps(double v)             { lossFields["dHVWOLeadsCurrentTempTaps"] = home_FieldValue(v); }
            void setDHVLeadsCurrentTempTaps(double v)               { lossFields["dHVLeadsCurrentTempTaps"] = home_FieldValue(v); }
            void setDTotalHVCurrentTempTaps(double v)               { lossFields["dTotalHVCurrentTempTaps"] = home_FieldValue(v); }
            void setDLVWOLeadsNewTempTaps(double v)                 { lossFields["dLVWOLeadsNewTempTaps"] = home_FieldValue(v); }
            void setDLVLeadsNewTempTaps(double v)                   { lossFields["dLVLeadsNewTempTaps"] = home_FieldValue(v); }
            void setDTotalLVNewTempTaps(double v)                   { lossFields["dTotalLVNewTempTaps"] = home_FieldValue(v); }
            void setDHVWOLeadsNewTempTaps(double v)                 { lossFields["dHVWOLeadsNewTempTaps"] = home_FieldValue(v); }
            void setDHVLeadsNewTempTaps(double v)                   { lossFields["dHVLeadsNewTempTaps"] = home_FieldValue(v); }
            void setDTotalHVNewTempTaps(double v)                   { lossFields["dTotalHVNewTempTaps"] = home_FieldValue(v); }
            void setDI2RCurrentTemp(double v)                       { lossFields["dI2RCurrentTemp"] = home_FieldValue(v); }
            void setDI2RNewTemp(double v)                           { lossFields["dI2RNewTemp"] = home_FieldValue(v); }
            void setDI2RCurrentTempTaps(double v)                   { lossFields["dI2RCurrentTempTaps"] = home_FieldValue(v); }
            void setDI2RNewTempTaps(double v)                       { lossFields["dI2RNewTempTaps"] = home_FieldValue(v); }
            void setDLVLeadLoss(double v)                           { lossFields["dLVLeadLoss"] = home_FieldValue(v); }
            void setDTankStray(double v)                            { lossFields["dTankStray"] = home_FieldValue(v); }
            void setDCoilSray(double v)                             { lossFields["dCoilSray"] = home_FieldValue(v); }
            void setTotalStray(double v)                            { lossFields["totalStray"] = home_FieldValue(v); }
            void setDTotalLLossStrayLead(double v)                  { lossFields["dTotalLLossStrayLead"] = home_FieldValue(v); }
            void setDCoreLoss(double v)                             { lossFields["dCoreLoss"] = home_FieldValue(v); }
            void setDTotalLoss(double v)                            { lossFields["dTotalLoss"] = home_FieldValue(v); }
            void setDNoLoadLossCost(double v)                       { lossFields["dNoLoadLossCost"] = home_FieldValue(v); }
            void setDLoadLossCost(double v)                         { lossFields["dLoadLossCost"] = home_FieldValue(v); }
            void setDTotalLossCost(double v)                        { lossFields["dTotalLossCost"] = home_FieldValue(v); }
            void setSNoLoadLossCost(const std::string& v)           { lossFields["sNoLoadLossCost"] = home_FieldValue(v); }
            void setSLoadLossCost(const std::string& v)             { lossFields["sLoadLossCost"] = home_FieldValue(v); }
            void setSTotalLossCost(const std::string& v)            { lossFields["sTotalLossCost"] = home_FieldValue(v); }
            void setDMaxWatts(double v)                             { lossFields["dMaxWatts"] = home_FieldValue(v); }
            void setDMaxWattsK4(double v)                           { lossFields["dMaxWattsK4"] = home_FieldValue(v); }
            void setNoLoadLossLosses(double v)                      { lossFields["noLoadLossLosses"] = home_FieldValue(v); }
            void setLoadLossosses(double v)                         { lossFields["loadLossosses"] = home_FieldValue(v); }
            void setICoilLdMat(int v)                               { lossFields["iCoilLdMat"] = home_FieldValue(v); }
            void setAreaLvBus(double v)                             { lossFields["areaLvBus"] = home_FieldValue(v); }
            void setDFactorLVBus(double v)                          { lossFields["dFactorLVBus"] = home_FieldValue(v); }
            void setLengthInLvBus(double v)                         { lossFields["lengthInLvBus"] = home_FieldValue(v); }
            void setDOhmsLVBus(double v)                            { lossFields["dOhmsLVBus"] = home_FieldValue(v); }
            void setDLossLVBus(double v)                            { lossFields["dLossLVBus"] = home_FieldValue(v); }
            void setLeadLengthOutOfCoil(double v)                   { lossFields["leadLengthOutOfCoil"] = home_FieldValue(v); }
            void setIBusLdMat(int v)                                { lossFields["iBusLdMat"] = home_FieldValue(v); }
            void setAreaBusWork(double v)                           { lossFields["areaBusWork"] = home_FieldValue(v); }
            void setDFactorBuswork(double v)                        { lossFields["dFactorBuswork"] = home_FieldValue(v); }
            void setDOhmsBuswork(double v)                          { lossFields["dOhmsBuswork"] = home_FieldValue(v); }
            void setDLossBuswork(double v)                          { lossFields["dLossBuswork"] = home_FieldValue(v); }
            void setDTotLdLoss(double v)                            { lossFields["dTotLdLoss"] = home_FieldValue(v); }
            void setDHVAmpsRegLds(double v)                         { lossFields["dHVAmpsRegLds"] = home_FieldValue(v); }
            void setDHVLineAmpsRegLds(double v)                     { lossFields["dHVLineAmpsRegLds"] = home_FieldValue(v); }
            void setICoilLdMatHV(int v)                             { lossFields["iCoilLdMatHV"] = home_FieldValue(v); }
            void setDAreaHVBus(double v)                            { lossFields["dAreaHVBus"] = home_FieldValue(v); }
            void setDFactorHVBus(double v)                          { lossFields["dFactorHVBus"] = home_FieldValue(v); }
            void setDLengthInHVBus(double v)                        { lossFields["dLengthInHVBus"] = home_FieldValue(v); }
            void setDOhmsHVBus(double v)                            { lossFields["dOhmsHVBus"] = home_FieldValue(v); }
            void setDLossHVBus(double v)                            { lossFields["dLossHVBus"] = home_FieldValue(v); }
            void setAreaBusWorkHV(double v)                         { lossFields["areaBusWorkHV"] = home_FieldValue(v); }
            void setDFactorBusworkHV(double v)                      { lossFields["dFactorBusworkHV"] = home_FieldValue(v); }
            void setDOhmsBusworkHV(double v)                        { lossFields["dOhmsBusworkHV"] = home_FieldValue(v); }
            void setDLossBusworkHV(double v)                        { lossFields["dLossBusworkHV"] = home_FieldValue(v); }
            void setDTotLdLossHV(double v)                          { lossFields["dTotLdLossHV"] = home_FieldValue(v); }
            void setD20LLwindingLv(double v)                        { lossFields["d_20LLwindingLv"] = home_FieldValue(v); }
            void setD55LLwindingLv(double v)                        { lossFields["d_55LLwindingLv"] = home_FieldValue(v); }
            void setD75LLwindingLv(double v)                        { lossFields["d_75LLwindingLv"] = home_FieldValue(v); }
            void setD85LLwindingLv(double v)                        { lossFields["d_85LLwindingLv"] = home_FieldValue(v); }
            void setD20LLwindingHv(double v)                        { lossFields["d_20LLwindingHv"] = home_FieldValue(v); }
            void setD55LLwindingHv(double v)                        { lossFields["d_55LLwindingHv"] = home_FieldValue(v); }
            void setD75LLwindingHv(double v)                        { lossFields["d_75LLwindingHv"] = home_FieldValue(v); }
            void setD85LLwindingHv(double v)                        { lossFields["d_85LLwindingHv"] = home_FieldValue(v); }
            void setD20Total100Load(double v)                       { lossFields["d20Total100Load"] = home_FieldValue(v); }
            void setD55Total100Load(double v)                       { lossFields["d55Total100Load"] = home_FieldValue(v); }
            void setD75Total100Load(double v)                       { lossFields["d75Total100Load"] = home_FieldValue(v); }
            void setD85Total100Load(double v)                       { lossFields["d85Total100Load"] = home_FieldValue(v); }
            void setD20TotalLR25(double v)                          { lossFields["d20TotalLR25"] = home_FieldValue(v); }
            void setD55TotalLR25(double v)                          { lossFields["d55TotalLR25"] = home_FieldValue(v); }
            void setD75TotalLR25(double v)                          { lossFields["d75TotalLR25"] = home_FieldValue(v); }
            void setD85TotalLR25(double v)                          { lossFields["d85TotalLR25"] = home_FieldValue(v); }
            void setD20TotalLR50(double v)                          { lossFields["d20TotalLR50"] = home_FieldValue(v); }
            void setD55TotalLR50(double v)                          { lossFields["d55TotalLR50"] = home_FieldValue(v); }
            void setD75TotalLR50(double v)                          { lossFields["d75TotalLR50"] = home_FieldValue(v); }
            void setD85TotalLR50(double v)                          { lossFields["d85TotalLR50"] = home_FieldValue(v); }
            void setD20TotalLR75(double v)                          { lossFields["d20TotalLR75"] = home_FieldValue(v); }
            void setD55TotalLR75(double v)                          { lossFields["d55TotalLR75"] = home_FieldValue(v); }
            void setD75TotalLR75(double v)                          { lossFields["d75TotalLR75"] = home_FieldValue(v); }
            void setD85TotalLR75(double v)                          { lossFields["d85TotalLR75"] = home_FieldValue(v); }
            void setD20TotalLR125(double v)                         { lossFields["d20TotalLR125"] = home_FieldValue(v); }
            void setD55TotalLR125(double v)                         { lossFields["d55TotalLR125"] = home_FieldValue(v); }
            void setD75TotalLR125(double v)                         { lossFields["d75TotalLR125"] = home_FieldValue(v); }
            void setD85TotalLR125(double v)                         { lossFields["d85TotalLR125"] = home_FieldValue(v); }
            void setD20TotalLR133(double v)                         { lossFields["d20TotalLR133"] = home_FieldValue(v); }
            void setD55TotalLR133(double v)                         { lossFields["d55TotalLR133"] = home_FieldValue(v); }
            void setD75TotalLR133(double v)                         { lossFields["d75TotalLR133"] = home_FieldValue(v); }
            void setD85TotalLR133(double v)                         { lossFields["d85TotalLR133"] = home_FieldValue(v); }
            void setSErrorName(const std::vector<std::string>& v)       { lossFields["sErrorName"] = home_FieldValue(v); }
            void setTErrorEnum(const std::vector<home_WarningSeverity>& v)   { lossFields["tErrorEnum"] = home_FieldValue(v); }
            void setSErrorMessages(const std::vector<std::string>& v)   { lossFields["sErrorMessages"] = home_FieldValue(v); }
    };
// end of Losses class
}
//end of home_Losses