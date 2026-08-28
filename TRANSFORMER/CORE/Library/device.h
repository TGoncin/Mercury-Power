/**___________________________________________________________________________________________________
|    @date      May 26, 2026 ~ EDIT: August 17 2026
|    @author    Tom Goncin
|    @file      Device.h
|    @brief     Top-level orchestration layer driving the transformer structural validation framework.
|    @brielf    Transformer::Core       is "The Engine Framework"
|
|               Executes:
|                   - Global configuration mappings utilizing flexible multi-variant FieldValues.
|                   - Orchestrates sub-module pipelines across core, windings, thermal cooling, and losses.
|                   - Handles electrical simulation loops resolving performance efficiency and load metrics.
|               Returns:
|                   - A central unified simulation structure decoupled completely from global thread spaces.
|___________________________________________________________________________________________________*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "enums.h"
#include "alias.h"
#include "field_Value.h"
#include "materials.h"
#include "sap_Tables.h"

#include "costs.h"
#include "gradient.h"

#include "tests.h"

using namespace Transformer::Types;



namespace Transformer::Core
{
    class Device
    {
        public:
            home_WarningSeverity                device_Warnings;

            std::map <std::string, FieldValue>  deviceFields;
            std::map <std::string, double>      initial_FieldValues;

            std::vector <std::unique_ptr <home_Winding>>   windings;
            std::vector <std::unique_ptr <home_Coil>>      coil;

            std::unique_ptr <home_Cooling>      cooling;
            std::unique_ptr <home_Guarantee>    guarantee;
            std::unique_ptr <home_Losses>       losses;
            std::unique_ptr <home_Core>         core;
            std::unique_ptr <home_Tests>        tests;
            std::unique_ptr <home_Gradients>    gradients;
            std::unique_ptr <home_Costs>        costs;
            std::unique_ptr <home_Mechanical>   mechanical;

            Commercial::ItemsSap                            itemsSap;
            Commercial::SapStandardConductors               sapStandardConductors;
            Commercial::SapStandardCoreLeads                sapStandardCoreLeads;
            Commercial::SapStandardBushings                 sapStandardBushings;
            Commercial::SapStandardSpades                   sapStandardSpades;
            Commercial::SapStandardSwitches                 sapStandardSwitches;
            Commercial::SapStandardFuses                    sapStandardFuses;
            Commercial::SapStandardGauges                   sapStandardGauges;
            Commercial::MechTankAccessoriesStructuralSteel  mechTankAccessoriesStructuralSteel;
            Commercial::MechComponents                      mechComponents;
            Commercial::MechPurchasedTankAccessories        mechPurchasedTankAccessories;
            Commercial::MechTankSteelOil                    mechTankSteelOil;

            Commercial::MaterialConstants                   table_MaterialConstants;
            Commercial::ConductorTable                      table_Conductor;
            Commercial::ResistiveMaterial                   table_ResistiveMaterial;
            Commercial::CoilBil1Ph                          table_CoilBil1Ph;
            Commercial::CoilBil3Ph                          table_CoilBil3Ph;
            Commercial::ResistanceTable                     table_Resistance;
            Commercial::ResistivityTable                    table_Resistivity;
            Commercial::EfficiencyTable_Doe216              table_EfficiencyDOE216;
            Commercial::EfficiencyTable_8021                table_Efficiency_8021;
            Commercial::EfficiencyTable_8023                table_Efficiency_8023;
            Commercial::PackingTable                        table_Packing;
            Commercial::DuctThicknessTable                  table_DuctThickness;

            Device();
            ~Device();

            const std::vector <std::unique_ptr <home_Winding>>& list_Windings() const;

            bool          have_Tertiary()       const;
            bool          dual_Voltage()        const;

            FieldValue  get_Current (const std::string& key) const;
            FieldValue  get_Default (const std::string& key) const;

            home_Winding* get_Winding (home_WindingDesc windingDesc) const;
            home_Winding* get_WindingTV() const;
            home_Winding* get_WindingHV() const;
            home_Winding* get_WindingLV() const;

            home_Coil*    get_Coil              (int index) const;
            home_Coil*    get_LastCoil()        const;
            home_Coil*    get_CoilByPosition    (int position) const;
            home_Coil*    create_Coil           (home_CoilType type,    home_RegulationType regulation);

            home_CoilType        default_CoilType           (const std::string& coil);
            home_WindingDesc     default_WindingDescription (const std::string& coil);

            home_RegulationType  default_RegulationType     (const std::string& coil);

            int get_WindingLastCoil() const;
            int get_PositionAT()      const;
            int get_PositionBT()      const;

            double get_Frequency()    const;
            double calc_RBIWAux       (home_Winding* winding_Hv, home_Winding* winding_Lv, home_Coil* coil_Hv, home_Coil* coil_Lv, int numberPhases);
            double calc_MinEffDoe2016 (home_Winding* winding_Hv, home_Coil*    coil_Hv,    home_DesignSpecStandards  designSpec, int numberPhases);
            double calc_MinEff8021    (home_DesignSpecStandards designSpecStandards,   home_Winding* winding_Hv, int numberPhases);
            double calc_MinEff8023    (home_DesignSpecStandards designSpecStandards,   home_Winding* winding_Hv, home_Winding* winding_Lv, int numberPhases);
            double calc_NumdemReg     (double powerFactor, double per, double loadLos, double kVANom, double IZ);
            double calc_NumdemEf      (double powerFactor, double per, double loadLos, double kVANom, double NoLoadLosses);
            double get_Packing        (int index);

            void load_ItemsSap();
            void define_PricesSapTables();
            void update_PriceConductors();
            void add_Winding                  (home_WindingDesc enums);
            void reset_Windings();
            void set_Field                    (const std::string& key, const FieldValue& value);
            void reset_Device();
            void set_DualVoltage              (bool value);     //setter
            void assign_CoilTypes             (home_ArrayType array);
            void create_CoilArray             (home_ArrayType array);
            void associate_CoilsWithWindings();
            void reset_Fields();
            void erase_Warnings();
            void calc_Warnings();
            void calc_PreviousDevice          (home_Winding* winding_Hv,      home_Winding* winding_Lv, home_Cooling* coolingModule);
            void calc_Device                  (bool          warnings = true);
            void calc_DeviceLosses();
            void ruleConductorSpace           (home_Winding* winding_Hv,      home_Winding* winding_Lv);
            void looking_kWSteps              (home_Cooling* coolingModule,        home_Winding* winding_Hv);
            void calc_DeviceOptimization      (home_Winding* winding_Hv,      home_Winding* winding_Lv);
            void calc_VxTi                    (double        previousCoreArea);
            void calc_Vxt                     (home_Winding* winding_Lv,      home_Winding* winding_Tertiary);
            void calc_TurnsRatio();
            void calc_PhaseAmp();
            void calc_CoreCoil();
            void calc_OverCoil();
            void calc_CoreWH                  (home_Winding* winding_Hv,      home_Winding* winding_Lv, int numberPhases);
            void calc_Form                    (int numberPhases);
            void calc_OverHV                  (int numberPhases);
            void calc_UnderHV                 (int numberPhases);
            void calc_OverOLV();
            void calc_CoilCore();
            void calc_CoilCoil();
            void calc_FormRB();
            void calc                         (home_Winding* winding_Hv,    home_Winding* winding_Lv);
            void calc_MaterialBuswork         (home_Coil*    coil_Lv);
            void calc_WindingForm             (home_Winding* winding_Lv,    home_Coil*    coil_Lv,        int          numberPhases);
            void calc_MeanTurn                (home_Core*    coreModule,    home_Winding* winding_Hv,     home_Winding* winding_Lv,    home_Coil* coil_Hv, home_Coil* coil_Lv,     int numberPhases);
            void calc_ResistiveMaterial();
            void calc_TabMaterialConstants();
            void calc_tableDataCoils();
            void calc_BufferFactor            (home_Core*    coreModule,    home_Winding* winding_Hv,     home_Winding* winding_Lv,   home_Coil* coil_Hv, home_Coil* coil_Lv,     int numberPhases);
            void calc_PressboardPacking       (home_Core*    coreModule);
            void calc_RBIW                    (home_Core*    coreModule,    home_Winding* winding_Hv,     home_Winding* winding_Lv,   home_Coil* coil_Hv, home_Coil* coil_Lv,     int numberPhases);
            void calc_DensityBuswork          (home_Winding* winding_Lv);
            void calc_InsulationLenghts       (home_Core*    coreModule,    home_Winding* winding_Hv,     home_Coil*    coil_Hv,      home_Coil* coil_Lv);
            void calc_TotalCC                 (home_Core*    coreModule,    home_Coil*    coil_Hv,        home_Coil*    coil_Lv);
            void calc_Impedance               (home_Core*    coreModule,    home_Winding* winding_Hv,     home_Winding* winding_Lv,   home_Coil* coil_Hv, home_Coil* coil_Lv,     home_Losses* lossesModule,  home_Guarantee* guaranteeModule, int numberPhases );
            void calc_PerEfficiency           (home_Core*   coreModule,     home_Cooling*      coolingModule,  home_Losses*       lossesModule);
            void calc_PerRegulation           (home_Cooling*     coolingModule,  home_Losses*       lossesModule);
            void calc_PercEfficiency          (home_Core*   coreModule,     home_Losses*       lossesModule);
            void calc_MinEfficiencytables     (home_Guarantee*   guaranteeModule,home_Winding* winding_Hv,     home_Winding* winding_Lv,   home_Coil* coil_Hv);
            void calc_TFMax                   (home_Winding*winding_Hv,     home_Winding* winding_Lv,     home_Coil*    coil_Hv,      home_Coil* coil_Lv,  home_Losses*   lossesModule, home_Cooling*coolingModule, home_Guarantee* guaranteeModule, int numberPhases);
            void calc_RevisionWS              (int index);
            void calc_RevisionsDesc           (int index);
            void calc_UnderInner();
            void save_MinInfoReports          (home_Winding* winding_Hv,     home_Winding*   winding_Lv);
            void send_Optimize();
            void calc_VxvOpt                  (home_Core*    coreModule,     home_Guarantee* guaranteeModule);
            void calc_NomTurnsOpt             (home_Winding* winding_Lv,     home_Winding*   windingTertiary, int            numberPhases);
            void adjust_Core                  (Device*       device,         home_Core*      coreModule,      home_Winding*  winding_Lv,         home_Winding* windingTertiary, bool amorphous);
            void calc_DeviceIni();
            void verify_Efficiency();
            void calc_BusworkDensityOptimizer (int quantityLVLeads, double densityLVLeads,  home_Winding* winding_Lv);
            void calc_ParametersOptimizer();
            void calc_DuctsWind               (home_Winding* winding_Hv, home_Winding* winding_Lv, int numberPhases, bool LHL);



//               S T R I N G   G E T T E R S


            std::string getSoNumber() const                         { return deviceFields.count("so_number") ? deviceFields.at("so_number").get<std::string>() : "";}
            std::string getCustomerName() const                     { return deviceFields.at("customer_name").get<std::string>(); }
            std::string getCustomerCode() const                     { return deviceFields.at("customer_code").get<std::string>(); }
            std::string getIssuedBy() const                         { return deviceFields.at("issued_by").get<std::string>(); }
            std::string getOriginalIssueDate() const                { return deviceFields.at("original_issue_date").get<std::string>(); }
            std::string getSSalesPerson() const                     { return deviceFields.at("sSalesPerson").get<std::string>(); }
            std::string getRequestedDueDate() const                 { return deviceFields.at("requested_due_date").get<std::string>(); }
            std::string getWindingDescription() const               { return deviceFields.at("winding_description").get<std::string>(); }
            std::string getSDensityBuswork() const                  { return deviceFields.at("s_DensityBuswork").get<std::string>(); }
            std::string getSDensityBusworkOnaf() const              { return deviceFields.at("s_DensityBusworkOnaf").get<std::string>(); }
            std::string getSConnectionDiagramsItems() const         { return deviceFields.at("sConnectionDiagramsItems").get<std::string>(); }
            std::string getSWindingSpecComments() const             { return deviceFields.at("sWindingSpecComments").get<std::string>(); }
            std::string getSPurchasingComments() const              { return deviceFields.at("sPurchasingComments").get<std::string>(); }
            std::string getSInsulationComments() const              { return deviceFields.at("sInsulationComments").get<std::string>(); }
            std::string getSFormMaterial() const                    { return deviceFields.at("sFormMaterial").get<std::string>(); }
            std::string getSFormComments() const                    { return deviceFields.at("sFormComments").get<std::string>(); }
            std::string getSVxT() const                             { return deviceFields.at("sVxT").get<std::string>(); }
            std::string getSOverCoilDescription() const             { return deviceFields.at("sOverCoilDescription").get<std::string>(); }
            std::string getSCoreWHPackingDescription() const        { return deviceFields.at("sCoreWHPackingDescription").get<std::string>(); }
            std::string getSFormDescription() const                 { return deviceFields.at("sFormDescription").get<std::string>(); }
            std::string getSUnderHVDescription() const              { return deviceFields.at("sUnderHVDescription").get<std::string>(); }
            std::string getSOverHVDescription() const               { return deviceFields.at("sOverHVDescription").get<std::string>(); }
            std::string getSOverOLVDescription() const              { return deviceFields.at("sOverOLVDescription").get<std::string>(); }
            std::string getSCoilCoreDescription() const             { return deviceFields.at("sCoilCoreDescription").get<std::string>(); }
            std::string getSCoilCoilDescription() const             { return deviceFields.at("sCoilCoilDescription").get<std::string>(); }
            std::string getSFormRBWS() const                        { return deviceFields.at("sFormRBWS").get<std::string>(); }
            std::string getSUnderInner() const                      { return deviceFields.at("sUnderInner").get<std::string>(); }
            std::string getSWindingFormMaterial() const             { return deviceFields.at("sWindingFormMaterial").get<std::string>(); }

    /*_______________________________________________
    |                                               |
    |       N U M E R I C A L   G E T T E R S       |
    |_______________________________________________|*/
            int getQuantity() const                                 { return deviceFields.at("quantity").get<int>(); }
            int getINoPhases() const                                { return deviceFields.at("iNoPhases").get<int>(); }
            int getNumCoils() const                                 { return deviceFields.at("num_Coils").get<int>(); }
            int getNumberWinding() const                            { return deviceFields.at("number_winding").get<int>(); }
            int getIQuantityBuswork() const                         { return deviceFields.at("i_QuantityBuswork").get<int>(); }
            int getDAltitude() const                                { return deviceFields.at("dAltitude").get<int>(); }
            int getIBILUnder() const                                { return deviceFields.at("iBILUnder").get<int>(); }
            int getIQtyCrepe() const                                { return deviceFields.at("iQtyCrepe").get<int>(); }
            int getIFluxDensityMax() const                          { return deviceFields.at("iFluxDensityMax").get<int>(); }
            int getIFluxDensityMin() const                          { return deviceFields.at("iFluxDensityMin").get<int>(); }
            int getITotalStackMax() const                           { return deviceFields.at("iTotalStackMax").get<int>(); }
            int getITotalStackMin() const                           { return deviceFields.at("iTotalStackMin").get<int>(); }

            double getDLatestVersion() const                        { return deviceFields.at("dLatestVersion").get<double>(); }
            double getNominalKva() const                            { return deviceFields.at("nominal_kva").get<double>(); }
            double getFrequency() const                             { return deviceFields.at("frequency").get<double>(); }
            double getDThicknessBuswork() const                     { return deviceFields.at("d_ThicknessBuswork").get<double>(); }
            double getDWidthBuswork() const                         { return deviceFields.at("d_WidthBuswork").get<double>(); }
            double getDDensityBuswork() const                       { return deviceFields.at("d_DensityBuswork").get<double>(); }
            double getDDensityBusworkOnaf() const                   { return deviceFields.at("d_DensityBusworkOnaf").get<double>(); }
            double getDBus1Length() const                           { return deviceFields.at("dBus1Length").get<double>(); }
            double getDBus2Length() const                           { return deviceFields.at("dBus2Length").get<double>(); }
            double getDBus3Length() const                           { return deviceFields.at("dBus3Length").get<double>(); }
            double getDBusTotalLength() const                       { return deviceFields.at("dBusTotalLength").get<double>(); }
            double getDBusTotalLengthFeet() const                   { return deviceFields.at("dBusTotalLengthFeet").get<double>(); }
            double getDBusWattsPerFeet() const                      { return deviceFields.at("dBusWatts/Feet").get<double>(); }
            double getDBusTotalWatts() const                        { return deviceFields.at("dBusTotalWatts").get<double>(); }
            double getDOverallRBFactor() const                      { return deviceFields.at("dOverallRBFactor").get<double>(); }
            double getDONANLosses() const                           { return deviceFields.at("dONANLosses").get<double>(); }
            double getDONANTOR() const                              { return deviceFields.at("dONANTOR").get<double>(); }
            double getDFormMaterialRB() const                       { return deviceFields.at("dFormMaterialRB").get<double>(); }
            double getDVxTi() const                                 { return deviceFields.at("dVxTi").get<double>(); }
            double getDPreviousLVNomTurns() const                   { return deviceFields.at("dPreviousLVNomTurns").get<double>(); }
            double getDVxT() const                                  { return deviceFields.at("dVxT").get<double>(); }
            double getDOverCoilkvmm() const                         { return deviceFields.at("dOverCoilkvmm").get<double>(); }
            double getDOverCoilRadialBuilt() const                  { return deviceFields.at("dOverCoilRadialBuilt").get<double>(); }
            double getDCoreWHPackingkvmm() const                    { return deviceFields.at("dCoreWHPackingkvmm").get<double>(); }
            double getDCoreWHPackingRadialBuilt() const             { return deviceFields.at("dCoreWHPackingRadialBuilt").get<double>(); }
            double getDFormkvmm() const                             { return deviceFields.at("dFormkvmm").get<double>(); }
            double getDFormRadialBuilt() const                      { return deviceFields.at("dFormRadialBuilt").get<double>(); }
            double getDUnderHVkvmm() const                          { return deviceFields.at("dUnderHVkvmm").get<double>(); }
            double getDUnderHVRadialBuilt() const                   { return deviceFields.at("dUnderHVRadialBuilt").get<double>(); }
            double getDOverHVkvmm() const                           { return deviceFields.at("dOverHVkvmm").get<double>(); }
            double getDOverHVRadialBuilt() const                    { return deviceFields.at("dOverHVRadialBuilt").get<double>(); }
            double getDOverOLVkvmm() const                          { return deviceFields.at("dOverOLVkvmm").get<double>(); }
            double getDOverOLVRadialBuilt() const                   { return deviceFields.at("dOverOLVRadialBuilt").get<double>(); }
            double getDCoilCorekvmm() const                         { return deviceFields.at("dCoilCorekvmm").get<double>(); }
            double getDCoilCoreRadialBuilt() const                  { return deviceFields.at("dCoilCoreRadialBuilt").get<double>(); }
            double getDCoilCoilkvmm() const                         { return deviceFields.at("dCoilCoilkvmm").get<double>(); }
            double getDCoilCoilRadialBuilt() const                  { return deviceFields.at("dCoilCoilRadialBuilt").get<double>(); }
            double getDFormRB() const                               { return deviceFields.at("dFormRB").get<double>(); }
            double getDPBWindingForm() const                        { return deviceFields.at("dPBWindingForm").get<double>(); }
            double getDRBWindingForm() const                        { return deviceFields.at("dRBWindingForm").get<double>(); }
            double getDRBOverOLV() const                            { return deviceFields.at("dRBOverOLV").get<double>(); }

            bool getBDesingSpecial() const                          { return deviceFields.at("bDesingSpecial").get<bool>(); }
            bool getHaveTertiary() const                            { return deviceFields.at("have_tertiary").get<bool>(); }
            bool getDualVoltage() const                             { return deviceFields.at("dual_voltage").get<bool>(); }

    /*_______________________________________________
    |                                               |
    |          E N U M S   &   V E C T O R S        |
    |_______________________________________________|*/
            Config::Family getTransformerFamily() const             { return deviceFields.at("transformer_family").get<Config::Family>(); }
            Config::ArrayType getPreviousarrayType() const          { return deviceFields.at("previousarray_type").get<Config::ArrayType>(); }
            Config::ArrayType getArrayType() const                  { return deviceFields.at("array_type").get<Config::ArrayType>(); }
            Config::Material getTMaterialBuswork() const            { return deviceFields.at("t_MaterialBuswork").get<Config::Material>(); }
            Config::Material getSMaterialBuswork() const            { return deviceFields.at("s_MaterialBuswork").get<Config::Material>(); }
            Config::Bars getTTypeBuswork() const                    { return deviceFields.at("t_TypeBuswork").get<Config::Bars>(); }

            std::vector<int> getIRevisionWS() const                 { return deviceFields.at("i_RevisionWS").get<std::vector<int>>(); }
            std::vector<std::string> getDataDescription() const     { return deviceFields.at("data_description").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDateDescr0() const         { return deviceFields.at("s_DateDescr0").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDateDescr1() const         { return deviceFields.at("s_DateDescr1").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDateDescr2() const         { return deviceFields.at("s_DateDescr2").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDateDescr3() const         { return deviceFields.at("s_DateDescr3").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDateDescr4() const         { return deviceFields.at("s_DateDescr4").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDataDescr0() const         { return deviceFields.at("s_DataDescr0").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDataDescr1() const         { return deviceFields.at("s_DataDescr1").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDataDescr2() const         { return deviceFields.at("s_DataDescr2").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDataDescr3() const         { return deviceFields.at("s_DataDescr3").get<std::vector<std::string>>(); }
            std::vector<std::string> getSDataDescr4() const         { return deviceFields.at("s_DataDescr4").get<std::vector<std::string>>(); }
            std::vector<std::string> getSErrorName() const          { return deviceFields.at("sErrorName").get<std::vector<std::string>>(); }
            std::vector<home_WarningSeverity> getTErrorEnum() const { return deviceFields.at("tErrorEnum").get<std::vector<home_WarningSeverity>>(); }
            std::vector<std::string> getSErrorMessages() const      { return deviceFields.at("sErrorMessages").get<std::vector<std::string>>(); }

    /*_______________________________________________
    |                                               |
    |          S T R I N G   S E T T E R S          |
    |_______________________________________________|*/
            void setSoNumber(const std::string& v)                  { deviceFields["so_number"] = FieldValue(v); }
            void setCustomerName(const std::string& v)              { deviceFields["customer_name"] = FieldValue(v); }
            void setCustomerCode(const std::string& v)              { deviceFields["customer_code"] = FieldValue(v); }
            void setIssuedBy(const std::string& v)                  { deviceFields["issued_by"] = FieldValue(v); }
            void setOriginalIssueDate(const std::string& v)         { deviceFields["original_issue_date"] = FieldValue(v); }
            void setSSalesPerson(const std::string& v)              { deviceFields["sSalesPerson"] = FieldValue(v); }
            void setRequestedDueDate(const std::string& v)          { deviceFields["requested_due_date"] = FieldValue(v); }
            void setWindingDescription(const std::string& v)        { deviceFields["winding_description"] = FieldValue(v); }
            void setSDensityBuswork(const std::string& v)           { deviceFields["s_DensityBuswork"] = FieldValue(v); }
            void setSDensityBusworkOnaf(const std::string& v)       { deviceFields["s_DensityBusworkOnaf"] = FieldValue(v); }
            void setSConnectionDiagramsItems(const std::string& v)  { deviceFields["sConnectionDiagramsItems"] = FieldValue(v); }
            void setSWindingSpecComments(const std::string& v)      { deviceFields["sWindingSpecComments"] = FieldValue(v); }
            void setSPurchasingComments(const std::string& v)       { deviceFields["sPurchasingComments"] = FieldValue(v); }
            void setSInsulationComments(const std::string& v)       { deviceFields["sInsulationComments"] = FieldValue(v); }
            void setSFormMaterial(const std::string& v)             { deviceFields["sFormMaterial"] = FieldValue(v); }
            void setSFormComments(const std::string& v)             { deviceFields["sFormComments"] = FieldValue(v); }
            void setSVxT(const std::string& v)                      { deviceFields["sVxT"] = FieldValue(v); }
            void setSOverCoilDescription(const std::string& v)      { deviceFields["sOverCoilDescription"] = FieldValue(v); }
            void setSCoreWHPackingDescription(const std::string& v) { deviceFields["sCoreWHPackingDescription"] = FieldValue(v); }
            void setSFormDescription(const std::string& v)          { deviceFields["sFormDescription"] = FieldValue(v); }
            void setSUnderHVDescription(const std::string& v)       { deviceFields["sUnderHVDescription"] = FieldValue(v); }
            void setSOverHVDescription(const std::string& v)        { deviceFields["sOverHVDescription"] = FieldValue(v); }
            void setSOverOLVDescription(const std::string& v)       { deviceFields["sOverOLVDescription"] = FieldValue(v); }
            void setSCoilCoreDescription(const std::string& v)      { deviceFields["sCoilCoreDescription"] = FieldValue(v); }
            void setSCoilCoilDescription(const std::string& v)      { deviceFields["sCoilCoilDescription"] = FieldValue(v); }
            void setSFormRBWS(const std::string& v)                 { deviceFields["sFormRBWS"] = FieldValue(v); }
            void setSUnderInner(const std::string& v)               { deviceFields["sUnderInner"] = FieldValue(v); }
            void setSWindingFormMaterial(const std::string& v)      { deviceFields["sWindingFormMaterial"] = FieldValue(v); }

    /*_______________________________________________
    |                                               |
    |       N U M E R I C A L   S E T T E R S       |
    |_______________________________________________|*/
            void setQuantity(int v)                                 { deviceFields["quantity"]                  = FieldValue(v); }
            void setINoPhases(int v)                                { deviceFields["iNoPhases"]                 = FieldValue(v); }
            void setNumCoils(int v)                                 { deviceFields["num_Coils"]                 = FieldValue(v); }
            void setNumberWinding(int v)                            { deviceFields["number_winding"]            = FieldValue(v); }
            void setIQuantityBuswork(int v)                         { deviceFields["i_QuantityBuswork"]         = FieldValue(v); }
            void setDAltitude(int v)                                { deviceFields["dAltitude"]                 = FieldValue(v); }
            void setIBILUnder(int v)                                { deviceFields["iBILUnder"]                 = FieldValue(v); }
            void setIQtyCrepe(int v)                                { deviceFields["iQtyCrepe"]                 = FieldValue(v); }
            void setIFluxDensityMax(int v)                          { deviceFields["iFluxDensityMax"]           = FieldValue(v); }
            void setIFluxDensityMin(int v)                          { deviceFields["iFluxDensityMin"]           = FieldValue(v); }
            void setITotalStackMax(int v)                           { deviceFields["iTotalStackMax"]            = FieldValue(v); }
            void setITotalStackMin(int v)                           { deviceFields["iTotalStackMin"]            = FieldValue(v); }
            void setDLatestVersion(double v)                        { deviceFields["dLatestVersion"]            = FieldValue(v); }
            void setNominalKva(double v)                            { deviceFields["nominal_kva"]               = FieldValue(v); }
            void setFrequency(double v)                             { deviceFields["frequency"]                 = FieldValue(v); }
            void setDThicknessBuswork(double v)                     { deviceFields["d_ThicknessBuswork"]        = FieldValue(v); }
            void setDWidthBuswork(double v)                         { deviceFields["d_WidthBuswork"]            = FieldValue(v); }
            void setDDensityBuswork(double v)                       { deviceFields["d_DensityBuswork"]          = FieldValue(v); }
            void setDDensityBusworkOnaf(double v)                   { deviceFields["d_DensityBusworkOnaf"]      = FieldValue(v); }
            void setDBus1Length(double v)                           { deviceFields["dBus1Length"]               = FieldValue(v); }
            void setDBus2Length(double v)                           { deviceFields["dBus2Length"]               = FieldValue(v); }
            void setDBus3Length(double v)                           { deviceFields["dBus3Length"]               = FieldValue(v); }
            void setDBusTotalLength(double v)                       { deviceFields["dBusTotalLength"]           = FieldValue(v); }
            void setDBusTotalLengthFeet(double v)                   { deviceFields["dBusTotalLengthFeet"]       = FieldValue(v); }
            void setDBusWattsPerFeet(double v)                      { deviceFields["dBusWatts/Feet"]            = FieldValue(v); }
            void setDBusTotalWatts(double v)                        { deviceFields["dBusTotalWatts"]            = FieldValue(v); }
            void setDOverallRBFactor(double v)                      { deviceFields["dOverallRBFactor"]          = FieldValue(v); }
            void setDONANLosses(double v)                           { deviceFields["dONANLosses"]               = FieldValue(v); }
            void setDONANTOR(double v)                              { deviceFields["dONANTOR"]                  = FieldValue(v); }
            void setDFormMaterialRB(double v)                       { deviceFields["dFormMaterialRB"]           = FieldValue(v); }
            void setDVxTi(double v)                                 { deviceFields["dVxTi"]                     = FieldValue(v); }
            void setDPreviousLVNomTurns(double v)                   { deviceFields["dPreviousLVNomTurns"]       = FieldValue(v); }
            void setDVxT(double v)                                  { deviceFields["dVxT"]                      = FieldValue(v); }
            void setDOverCoilkvmm(double v)                         { deviceFields["dOverCoilkvmm"]             = FieldValue(v); }
            void setDOverCoilRadialBuilt(double v)                  { deviceFields["dOverCoilRadialBuilt"]      = FieldValue(v); }
            void setDCoreWHPackingkvmm(double v)                    { deviceFields["dCoreWHPackingkvmm"]        = FieldValue(v); }
            void setDCoreWHPackingRadialBuilt(double v)             { deviceFields["dCoreWHPackingRadialBuilt"] = FieldValue(v); }
            void setDFormkvmm(double v)                             { deviceFields["dFormkvmm"]                 = FieldValue(v); }
            void setDFormRadialBuilt(double v)                      { deviceFields["dFormRadialBuilt"]          = FieldValue(v); }
            void setDUnderHVkvmm(double v)                          { deviceFields["dUnderHVkvmm"]              = FieldValue(v); }
            void setDUnderHVRadialBuilt(double v)                   { deviceFields["dUnderHVRadialBuilt"]       = FieldValue(v); }
            void setDOverHVkvmm(double v)                           { deviceFields["dOverHVkvmm"]               = FieldValue(v); }
            void setDOverHVRadialBuilt(double v)                    { deviceFields["dOverHVRadialBuilt"]        = FieldValue(v); }
            void setDOverOLVkvmm(double v)                          { deviceFields["dOverOLVkvmm"]              = FieldValue(v); }
            void setDOverOLVRadialBuilt(double v)                   { deviceFields["dOverOLVRadialBuilt"]       = FieldValue(v); }
            void setDCoilCorekvmm(double v)                         { deviceFields["dCoilCorekvmm"]             = FieldValue(v); }
            void setDCoilCoreRadialBuilt(double v)                  { deviceFields["dCoilCoreRadialBuilt"]      = FieldValue(v); }
            void setDCoilCoilkvmm(double v)                         { deviceFields["dCoilCoilkvmm"]             = FieldValue(v); }
            void setDCoilCoilRadialBuilt(double v)                  { deviceFields["dCoilCoilRadialBuilt"]      = FieldValue(v); }
            void setDFormRB(double v)                               { deviceFields["dFormRB"]                   = FieldValue(v); }
            void setDPBWindingForm(double v)                        { deviceFields["dPBWindingForm"]            = FieldValue(v); }
            void setDRBWindingForm(double v)                        { deviceFields["dRBWindingForm"]            = FieldValue(v); }
            void setDRBOverOLV(double v)                            { deviceFields["dRBOverOLV"]                = FieldValue(v); }

            void setBDesingSpecial(bool v)                          { deviceFields["bDesingSpecial"]            = FieldValue(v); }
            void setHaveTertiary(bool v)                            { deviceFields["have_tertiary"]             = FieldValue(v); }
    /*_______________________________________________
    |                                               |
    |          E N U M S   &   V E C T O R S        |
    |_______________________________________________|*/
            void setTransformerFamily(Config::Family v)             { deviceFields["transformer_family"]        = FieldValue(v); }
            void setPreviousarrayType(Config::ArrayType v)          { deviceFields["previousarray_type"]        = FieldValue(v); }
            void setArrayType(Config::ArrayType v)                  { deviceFields["array_type"]                = FieldValue(v); }
            void setTMaterialBuswork(Config::Material v)            { deviceFields["t_MaterialBuswork"]         = FieldValue(v); }
            void setSMaterialBuswork(Config::Material v)            { deviceFields["s_MaterialBuswork"]         = FieldValue(v); }
            void setTTypeBuswork(Config::Bars v)                    { deviceFields["t_TypeBuswork"]             = FieldValue(v); }

            void setIRevisionWS(const std::vector<int>& v)              { deviceFields["i_RevisionWS"]      = FieldValue(v); }
            void setDataDescription(const std::vector<std::string>& v)  { deviceFields["data_description"]  = FieldValue(v); }
            void setSDateDescr0(const std::vector<std::string>& v)      { deviceFields["s_DateDescr0"]      = FieldValue(v); }
            void setSDateDescr1(const std::vector<std::string>& v)      { deviceFields["s_DateDescr1"]      = FieldValue(v); }
            void setSDateDescr2(const std::vector<std::string>& v)      { deviceFields["s_DateDescr2"]      = FieldValue(v); }
            void setSDateDescr3(const std::vector<std::string>& v)      { deviceFields["s_DateDescr3"]      = FieldValue(v); }
            void setSDateDescr4(const std::vector<std::string>& v)      { deviceFields["s_DateDescr4"]      = FieldValue(v); }
            void setSDataDescr0(const std::vector<std::string>& v)      { deviceFields["s_DataDescr0"]      = FieldValue(v); }
            void setSDataDescr1(const std::vector<std::string>& v)      { deviceFields["s_DataDescr1"]      = FieldValue(v); }
            void setSDataDescr2(const std::vector<std::string>& v)      { deviceFields["s_DataDescr2"]      = FieldValue(v); }
            void setSDataDescr3(const std::vector<std::string>& v)      { deviceFields["s_DataDescr3"]      = FieldValue(v); }
            void setSDataDescr4(const std::vector<std::string>& v)      { deviceFields["s_DataDescr4"]      = FieldValue(v); }
            void setSErrorName(const std::vector<std::string>& v)       { deviceFields["sErrorName"]        = FieldValue(v); }
            void setTErrorEnum(const std::vector<home_WarningSeverity>& v)   { deviceFields["tErrorEnum"]        = FieldValue(v); }
            void setSErrorMessages(const std::vector<std::string>& v)   { deviceFields["sErrorMessages"]    = FieldValue(v); }

    };
    //end of device class
}
// end of namespace Transformer::Core::Device