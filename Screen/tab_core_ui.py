# tab_core_ui.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size,add_dynamic_restore
from Definitions.enums import eLaminationType,eMaterial,eLeads,eBars,eCoating,eTypeConductor,eLogical
import math
MAXTAPS = 33

def build_core_tab(notebook, device):
    # --- Tab 4: CoreDesign---
    tab_core = ttk.Frame(notebook)
    notebook.add(tab_core, text="Core Design")

    size(tab_core)

    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()

    iIndex = 0
    iColumn = 0
    create_label(tab_core,iIndex,iColumn+1,"Core",font_size=12,bold = True)
    iIndex+=1

    iIndexType = 1

    # entry_dTotalStack = create_entry_focus(tab_core,iIndexType+2,iColumn,"Total Stack",device.core.fields["d_TotalStackMap"].get(),"dTotalStack",device.core,width=10,sticky="nsew")
    list_totalStack = get_list_total_stack(device.core)
    combo_dTotalStack = create_combobox_focus(tab_core,iIndexType+2,iColumn,"Total Stack",list_totalStack,"dTotalStack",device.core,width=27,warning_func=[device.core.core_warnings.warningTotalStackMax,device.core.core_warnings.warningRatioSteelWidthTotalStack])
    controls_list1 = [combo_dTotalStack]

    combo_dSteelWidth = create_combobox_focus(tab_core,iIndexType+1,iColumn,"Steel width",device.core.fields["d_SteelWidthMap"].current["TypeA"],"dSteelWidth",device.core,width=27, combos=controls_list1,on_select_func=on_steelwidth_change)

    controls_list = [combo_dSteelWidth]
    lam_types = [x for x in list(eLaminationType) if x!= eLaminationType.TYPE_NONE]
    combo_tLaminationType = create_combobox_focus(tab_core,iIndexType,iColumn,"Steel Grade (*)",lam_types,"tLaminationType",device.core,width=27, combos=controls_list,on_select_func=on_lamination_change)

    on_lamination_change(None,device.core.fields["tLaminationType"].get(),device.core, controls_list)
    iIndex += 3

    combo_tIsolatedCore = create_combobox_focus(tab_core,iIndex,iColumn,"Isolated Core",list(eLogical),"tIsolatedCore",device.core,width=27)
    iIndex +=1

    entry_dInductionLevel = create_entry_focus(tab_core,iIndex,iColumn,"Induction level","dInductionLevel",device.core,width=10,sticky="nsew")
    iIndex +=1

    entry_iNoLoops = create_combobox_focus(tab_core,iIndex,iColumn,"No Loops",[1,2],"iNoLoops",device.core,width=27)
    iIndex +=1

    entry_dLossFact = create_entry_focus(tab_core,iIndex,iColumn,"Loss Fact (%)","dLossFact",device.core,width=10,sticky="nsew")
    iIndex +=1

    entry_dLamFactor = create_entry_focus(tab_core,iIndex,iColumn,"Lam. Factor","dLamFactor",device.core,sticky="nsew")
    iIndex+=1

    entry_dStackTolPIn = create_entry_focus(tab_core,iIndex,iColumn,"Stack tol. + (in)","dStackTolPIn",device.core,sticky="nsew")
    iIndex+=1

    entry_dStackTolMIn = create_entry_focus(tab_core,iIndex,iColumn,"Stack tol - (in)","dStackTolMIn",device.core,sticky="nsew")
    iIndex+=1

    create_label(tab_core,iIndex,iColumn+1,"")
    iIndex+=1
    #Core Data Optional
    create_label(tab_core,iIndex,iColumn+1,"Optional Data",bold = True)
    iIndex+=1

    entry_sSheetComments = create_entry_focus(tab_core,iIndex,iColumn,"Sheet Comments","sSheetComments",device.core,sticky="nsew")
    iIndex+=1

    entry_dWindingHeightAdder = create_entry_focus(tab_core,iIndex,iColumn,"Window Height Adder","dWindingHeightAdder",device.core,sticky="nsew")
    iIndex+=1

    entry_dWindingWidhtAdder = create_entry_focus(tab_core,iIndex,iColumn,"Window Width Adder","dWindingWidthAdder",device.core,sticky="nsew")
    iIndex+=1

    entry_dCoilCorePBEnd = create_entry_focus(tab_core,iIndex,iColumn,"Coil to Core PB / End","dCoilCorePBEnd",device.core,sticky="nsew")
    iIndex+=1

    entry_dCoilCoilPBSides = create_entry_focus(tab_core,iIndex,iColumn,"Coil to Coil PB Sides","dCoilCoilPBSides",device.core,sticky="nsew")
    iIndex+=1

    entry_dCoilCorePBSides = create_entry_focus(tab_core,iIndex,iColumn,"Coil to Core PB Sides","dCoilCorePBSides",device.core,sticky="nsew")
    iIndex+=1

    entry_dFixedSmWindowWidth = create_entry_focus(tab_core,iIndex,iColumn,"Fixed Sm Window Width","dFixedSmWindowWidth",device.core,sticky="nsew")
    iIndex+=1

    entry_dFixedWindowHeight = create_entry_focus(tab_core,iIndex,iColumn,"Fixed Window Height","dFixedWindowHeight",device.core,sticky="nsew")
    iIndex+=1

    iColumn += 2
    iIndexCalcCore = 1
    entries_core_output=[]

    #NLper = device.guarantee.fields["dNL_pu"].get()
    create_label(tab_core,iIndexCalcCore,iColumn+1,"NL pu Rated",width=15)
    create_label(tab_core,iIndexCalcCore,iColumn+2,"Rated Voltage",width=15)
    iIndexCalcCore +=1

    entry_dFluxDensityRatPer = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"kL","dFluxDensityRatPer",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dFluxDensityRatPer)

    entry_dFluxDensityVolt = create_entry_focus(tab_core,iIndexCalcCore,iColumn+2,"","dFluxDensityVolt",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dFluxDensityVolt)

    iIndexCalcCore +=1

    entry_dFluxDensityRatPerT = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"T","dFluxDensityRatPerT",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dFluxDensityRatPerT)

    entry_dFluxDensityVoltT = create_entry_focus(tab_core,iIndexCalcCore,iColumn+2,"","dFluxDensityVoltT",device.core,width=20,sticky="nsew",bOutput=True,warning_func=device.core.core_warnings.warningFluxDensity)
    entries_core_output.append(entry_dFluxDensityVoltT)
    iIndexCalcCore +=1

    create_label(tab_core,iIndexCalcCore,iColumn,"",width=15)
    iIndexCalcCore +=1

    iIndexWeights = iIndexCalcCore
    entry_sVxT = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"V/T","sVxT",device,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_sVxT)
    iIndexCalcCore +=1

    entry_sArea = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Area in2","sArea",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_sArea)
    iIndexCalcCore +=1

    entry_dWindowHeight = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Window Height","dWindowHeight",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWindowHeight)
    iIndexCalcCore +=1

    entry_dSmallWindowWidth = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Window Width","dSmallWindowWidth",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dSmallWindowWidth)
    iIndexCalcCore +=1

    entry_dSmallWindowWidth = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Window Large","dLargeWindowWidth",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dSmallWindowWidth)
    iIndexCalcCore +=1

    entry_dStackPerLoop = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Stack per Loop","dStackPerLoop",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dStackPerLoop)
    iIndexCalcCore +=1

    entry_dStackCoreSmLoops = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Stack in core joints(Sm loops)","dStackCoreSmLoops",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dStackCoreSmLoops)
    iIndexCalcCore +=1

    entry_dStackCoreLgLoops = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Stack in core joints(Lg loops)","dStackCoreLgLoops",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dStackCoreLgLoops)
    iIndexCalcCore +=1

    entry_dStripWidth = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Strip Width","dStripWidth",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dStripWidth)
    iIndexCalcCore +=1

    entry_dOverallCoreHeight = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Overall Core Height outer","dOverallCoreHeight",device.core,width=20,sticky="nsew",bOutput=True,warning_func=device.core.core_warnings.warningMaxHeight)
    entries_core_output.append(entry_dOverallCoreHeight)
    iIndexCalcCore +=1

    entry_dOverallCoreHeightInner = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Overall Core Height inner","dOverallCoreHeightInner",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dOverallCoreHeightInner)
    iIndexCalcCore +=1

    entry_dCoreLengthWithoutPB = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Core Length Without PB","dCoreLengthWithoutPB",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dCoreLengthWithoutPB)
    iIndexCalcCore +=1

    entry_dCoreLengthWithPB = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Core Length Width PB","dCoreLengthWithPB",device.core,width=20,sticky="nsew",bOutput=True,warning_func=device.core.core_warnings.warningMaxLength)
    entries_core_output.append(entry_dCoreLengthWithPB)
    iIndexCalcCore +=1

    entry_dWattsLb = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Watts/Lb","dWattsLb",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWattsLb)
    iIndexCalcCore +=1

    entry_dLegCenters = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"Leg Centers","dLegCenters",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dLegCenters)
    iIndexCalcCore +=1

    entry_dTrueOverCoil = create_entry_focus(tab_core,iIndexCalcCore,iColumn,"True Overcoil","dTrueOverCoil",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dTrueOverCoil)
    iIndexCalcCore +=1

    iColumn+=2

    create_label(tab_core,iIndexWeights,iColumn,"Weights (lbs.)/set",columnspan = 2)
    iIndexWeights +=1
    entry_dWeightOneSmLoopSet = create_entry_focus(tab_core,iIndexWeights,iColumn,"Sm loops","dWeightOneSmLoop",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneSmLoopSet)
    iIndexWeights +=1

    entry_dWeightOneLgLoopSet = create_entry_focus(tab_core,iIndexWeights,iColumn,"Lg loops","dWeightOneLgLoop",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLgLoopSet)
    iIndexWeights +=1

    entry_dWeightOneLoopSet = create_entry_focus(tab_core,iIndexWeights,iColumn,"Total","dWeightOneLoop",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLoopSet)
    iIndexWeights +=1

    create_label(tab_core,iIndexWeights,iColumn,"Weights (lbs.)/Other set",columnspan = 2)
    iIndexWeights +=1
    entry_dWeightOneSmLoopOther = create_entry_focus(tab_core,iIndexWeights,iColumn,"Sm loops","dWeightOneSmLoopOther",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneSmLoopOther)
    iIndexWeights +=1

    entry_dWeightOneLgLoopSetOther = create_entry_focus(tab_core,iIndexWeights,iColumn,"Lg loops","dWeightOneLgLoopOther",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLgLoopSetOther)
    iIndexWeights +=1

    entry_dWeightOneLoopSetOther = create_entry_focus(tab_core,iIndexWeights,iColumn,"Total","dWeightOneLoopOther",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLoopSetOther)
    iIndexWeights +=1

    create_label(tab_core,iIndexWeights,iColumn,"Weights (lbs.)/Complete Unit",columnspan = 2)
    iIndexWeights +=1
    entry_dWeightOneSmLoopUnit = create_entry_focus(tab_core,iIndexWeights,iColumn,"Sm loops","dWeightTwoSmLoops",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneSmLoopUnit)
    iIndexWeights +=1

    entry_dWeightOneLgLoopUnit = create_entry_focus(tab_core,iIndexWeights,iColumn,"Lg loops","dWeightTwoLgLoops",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLgLoopUnit)
    iIndexWeights +=1

    entry_dWeightOneLoopUnit = create_entry_focus(tab_core,iIndexWeights,iColumn,"Total","dWeightTwoLoops",device.core,width=20,sticky="nsew",bOutput=True)
    entries_core_output.append(entry_dWeightOneLoopUnit)
    iIndexWeights +=1


    hide_controls(False,entries_core_output)


    iColumn +=4
    iIndexColTaps = 0
    lbltaps = create_label(tab_core,iIndexColTaps,iColumn+3,"Taps",bold = True,columnspan=2)
    iIndexColTaps+=1
    entries_taps = []

    entry_sLeadsOutTOPCoilHeaterl0 = create_entry_focus(tab_core,iIndexColTaps,iColumn,"","sLeadsOutTOPCoilHeater[0]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl0)
    entry_sLeadsOutTOPCoilHeaterl1 = create_entry_focus(tab_core,iIndexColTaps,iColumn+1,"","sLeadsOutTOPCoilHeater[1]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl1)
    entry_sLeadsOutTOPCoilHeaterl2 = create_entry_focus(tab_core,iIndexColTaps,iColumn+2,"","sLeadsOutTOPCoilHeater[2]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl2)
    entry_sLeadsOutTOPCoilHeaterl3 = create_entry_focus(tab_core,iIndexColTaps,iColumn+3,"","sLeadsOutTOPCoilHeater[3]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl3)
    entry_sLeadsOutTOPCoilHeaterl4 = create_entry_focus(tab_core,iIndexColTaps,iColumn+4,"","sLeadsOutTOPCoilHeater[4]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl4)
    entry_sLeadsOutTOPCoilHeaterl5 = create_entry_focus(tab_core,iIndexColTaps,iColumn+5,"","sLeadsOutTOPCoilHeater[5]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl5)
    entry_sLeadsOutTOPCoilHeaterl6 = create_entry_focus(tab_core,iIndexColTaps,iColumn+6,"","sLeadsOutTOPCoilHeater[6]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl6)
    entry_sLeadsOutTOPCoilHeaterl7 = create_entry_focus(tab_core,iIndexColTaps,iColumn+7,"","sLeadsOutTOPCoilHeater[7]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoilHeaterl7)
    iIndexColTaps +=1

    entry_sLeadsOutTOPCoil0 = create_entry_focus(tab_core,iIndexColTaps,iColumn,"","sLeadsOutTOPCoil[0]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil0)
    entry_sLeadsOutTOPCoil1 = create_entry_focus(tab_core,iIndexColTaps,iColumn+1,"","sLeadsOutTOPCoil[1]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil1)
    entry_sLeadsOutTOPCoil2 = create_entry_focus(tab_core,iIndexColTaps,iColumn+2,"","sLeadsOutTOPCoil[2]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil2)
    entry_sLeadsOutTOPCoil3 = create_entry_focus(tab_core,iIndexColTaps,iColumn+3,"","sLeadsOutTOPCoil[3]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil3)
    entry_sLeadsOutTOPCoil4 = create_entry_focus(tab_core,iIndexColTaps,iColumn+4,"","sLeadsOutTOPCoil[4]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil4)
    entry_sLeadsOutTOPCoil5 = create_entry_focus(tab_core,iIndexColTaps,iColumn+5,"","sLeadsOutTOPCoil[5]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil5)
    entry_sLeadsOutTOPCoil6 = create_entry_focus(tab_core,iIndexColTaps,iColumn+6,"","sLeadsOutTOPCoil[6]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil6)
    entry_sLeadsOutTOPCoil7 = create_entry_focus(tab_core,iIndexColTaps,iColumn+7,"","sLeadsOutTOPCoil[7]",pHVWind.changer,width=10,sticky="nsew",bOutput = True)
    entries_taps.append(entry_sLeadsOutTOPCoil7)
    iIndexColTaps +=1


    #Conductor information LV
    iIndexCol2 = iIndexColTaps
    pCoilLV = pLVWind.coils[0]

    lbl_LV_W =create_label(tab_core,iIndexCol2,iColumn,"LV Winding",font_size=12,bold = True)
    iIndexCol2+=1

    lbl_LV_RTurns =create_label(tab_core,iIndexCol2,iColumn,"Rated Turns",width = 12)
    entry_dRTurns_LV = create_entry_focus(tab_core,iIndexCol2,iColumn," ","dTurnsNom",pLVWind,width=10,sticky="ew", bOutput = True)
    iIndexCol2+=1

    iIndexCol21 = iIndexCol2
    iIndexCol22 = iIndexCol2
    iIndexCol23 = iIndexCol2
    iIndexCol24 = iIndexCol2

    entries_dkVPhaseDef_LV = []
    lbl_LV_VTap = create_label(tab_core,iIndexCol21,iColumn,"Voltage Tap",bold=True)
    for i in range(MAXTAPS):
        iIndexCol21 += 1
        entry_dkVPhaseDef_LV = create_entry_focus(
            tab_core,
            iIndexCol21,
            iColumn,
            "",  # label vacío
            f"d_kVPhaseDef[{i}]",
            pLVWind,
            width=10,
            bOutput = True
        )
        entries_dkVPhaseDef_LV.append(entry_dkVPhaseDef_LV)

    entries_dTurns_LV = []
    lbl_LV_Turns = create_label(tab_core,iIndexCol22,iColumn+1,"Turns",bold=True)
    for i in range(MAXTAPS):
        iIndexCol22 += 1
        entry_dTurnsC_LV = create_entry_focus(
            tab_core,
            iIndexCol22,
            iColumn+1,
            "",  # label vacío
            f"d_Turns[{i}]",
            pLVWind,
            width=10,
            bOutput = True
        )
        entries_dTurns_LV.append(entry_dTurnsC_LV)   

    entries_dCircTurns_LV = []
    lbl_LV_TurnsTap = create_label(tab_core,iIndexCol23,iColumn+2,"Turns/Tap",bold=True)
    for i in range(MAXTAPS):
        iIndexCol23 += 1
        entry_dCircTurns_LV = create_entry_focus(
            tab_core,
            iIndexCol23,
            iColumn+2,
            "",  # label vacío
            f"d_CircTurns[{i}]",
            pLVWind,
            width=10,
            bOutput = True
        )
        entries_dCircTurns_LV.append(entry_dCircTurns_LV) 
    
    entries_dError_LV = []
    lbl_LV_Error = create_label(tab_core,iIndexCol24,iColumn+3,"%Error",bold=True)
    for i in range(MAXTAPS):
        iIndexCol24 += 1
        entry_dError_LV = create_entry_focus(
            tab_core,
            iIndexCol24,
            iColumn+3,
            "",  # label vacío
            f"d_Error[{i}]",
            pLVWind,
            width=10,
            bOutput = True
        )
        entries_dError_LV.append(entry_dError_LV)   

    #Conductor information HV
    iColumn+=5
    iIndexCol3 = iIndexColTaps
    pCoilHV = pHVWind.coils[0]

    lbl_HV_W = create_label(tab_core,iIndexCol3,iColumn,"HV Winding",font_size=12,bold = True)
    iIndexCol3+=1

    lbl_HV_RTurns =create_label(tab_core,iIndexCol3,iColumn,"Rated Turns",width=12)
    entry_dTurns_HV = create_entry_focus(tab_core,iIndexCol3,iColumn," ","dTurnsNom",pHVWind,width=10,sticky="ew", bOutput = True)
    iIndexCol3+=1

    iIndexCol31 = iIndexCol3
    iIndexCol32 = iIndexCol3
    iIndexCol33 = iIndexCol3
    iIndexCol34 = iIndexCol3

    entries_dkVPhaseDef_HV = []
    lbl_HV_VTap = create_label(tab_core,iIndexCol31,iColumn,"Voltage Tap",bold=True)
    for i in range(MAXTAPS):
        iIndexCol31 += 1
        entry_dkVPhaseDef_HV = create_entry_focus(
            tab_core,
            iIndexCol31,
            iColumn,
            "",  # label vacío
            f"d_kVPhaseDef[{i}]",
            pHVWind,
            width=10,
            bOutput = True
        )
        entries_dkVPhaseDef_HV.append(entry_dkVPhaseDef_HV) 

    entries_dTurns_HV = []
    lbl_HV_Turns = create_label(tab_core,iIndexCol32,iColumn+1,"Turns",bold=True)
    for i in range(MAXTAPS):
        iIndexCol32 += 1
        entry_dTurnsC_HV = create_entry_focus(
            tab_core,
            iIndexCol32,
            iColumn+1,
            "",  # label vacío
            f"d_Turns[{i}]",
            pHVWind,
            width=10,
            bOutput = True
        )
        entries_dTurns_HV.append(entry_dTurnsC_HV) 

    entries_dCircTurns_HV = []
    lbl_HV_TurnsTap = create_label(tab_core,iIndexCol33,iColumn+2,"Turns/Tap",bold=True)
    for i in range(MAXTAPS):
        iIndexCol33 += 1
        entry_dCircTurns_HV = create_entry_focus(
            tab_core,
            iIndexCol33,
            iColumn+2,
            "",  # label vacío
            f"d_CircTurns[{i}]",
            pHVWind,
            width=10, 
            bOutput = True
        )
        entries_dCircTurns_HV.append(entry_dCircTurns_HV) 
    
    entries_dError_HV = []
    lbl_HV_Error = create_label(tab_core,iIndexCol34,iColumn+3,"%Error",bold=True)
    for i in range(MAXTAPS):
        iIndexCol34 += 1
        entry_dError_HV = create_entry_focus(
            tab_core,
            iIndexCol34,
            iColumn+3,
            "",  # label vacío
            f"d_Error[{i}]",
            pHVWind,
            width=10,
            bOutput = True
        )
        entries_dError_HV.append(entry_dError_HV) 

    

    def update_controls_visibility():
        iNoSteps = pLVWind.get_noSteps()
        iNomPos = pLVWind.fields["iNomPos"].get()

        bTaps = pLVWind.have_taps() or pHVWind.have_taps()
        bTapsHV = pHVWind.have_taps()
        bTapsLV = pLVWind.have_taps()

        toggle_controls(bTaps,lbl_LV_W)
        toggle_controls(bTaps,lbl_LV_RTurns)
        toggle_controls(bTaps,entry_dRTurns_LV)
        toggle_controls(bTapsLV,lbl_LV_VTap)
        toggle_controls(bTapsLV,lbl_LV_Turns)
        toggle_controls(bTapsLV,lbl_LV_TurnsTap)
        toggle_controls(bTapsLV,lbl_LV_Error)

        toggle_controls(bTaps,lbltaps)
        hide_controls(False,entries_taps)
        toggle_controls(bTaps,entries_taps)

        hide_controls(False,entry_dRTurns_LV)
        hide_controls(False,entries_dkVPhaseDef_LV)
        hide_controls(False,entries_dTurns_LV)
        hide_controls(True,entries_dTurns_LV[iNomPos])
        hide_controls(False,entries_dCircTurns_LV)
        hide_controls(False,entries_dError_LV)
        if iNoSteps==1:
            toggle_controls(False, entries_dkVPhaseDef_LV)
            toggle_controls(False, entries_dTurns_LV)
            toggle_controls(False, entries_dCircTurns_LV)
            toggle_controls(False, entries_dError_LV)
        else:
            toggle_controls(True, entries_dkVPhaseDef_LV[:iNoSteps])
            toggle_controls(False, entries_dkVPhaseDef_LV[iNoSteps:])
            toggle_controls(True, entries_dTurns_LV[:iNoSteps])
            toggle_controls(False, entries_dTurns_LV[iNoSteps:])
            toggle_controls(True, entries_dCircTurns_LV[:iNoSteps])
            toggle_controls(False, entries_dCircTurns_LV[0])
            toggle_controls(False, entries_dCircTurns_LV[iNoSteps:])
            toggle_controls(True, entries_dError_LV[:iNoSteps])
            toggle_controls(False, entries_dError_LV[iNoSteps:])

        iNoStepsH = pHVWind.get_noSteps()
        iNomPosH = pHVWind.fields["iNomPos"].get()

        toggle_controls(bTaps,lbl_HV_W)
        toggle_controls(bTaps,lbl_HV_RTurns)
        toggle_controls(bTaps,entry_dTurns_HV)
        toggle_controls(bTapsHV,lbl_HV_VTap)
        toggle_controls(bTapsHV,lbl_HV_Turns)
        toggle_controls(bTapsHV,lbl_HV_TurnsTap)
        toggle_controls(bTapsHV,lbl_HV_Error)

        hide_controls(False,entry_dTurns_HV)
        hide_controls(False,entries_dkVPhaseDef_HV)
        hide_controls(False,entries_dTurns_HV)
        #hide_controls(True,entries_dTurns_HV[iNomPosH])
        hide_controls(False,entries_dCircTurns_HV)
        hide_controls(False,entries_dError_HV)
        if iNoStepsH==1:
            toggle_controls(False, entries_dkVPhaseDef_HV)
            toggle_controls(False, entries_dTurns_HV)
            toggle_controls(False, entries_dCircTurns_HV)
            toggle_controls(False, entries_dError_HV)
        else:
            toggle_controls(True, entries_dkVPhaseDef_HV[:iNoStepsH])
            toggle_controls(False, entries_dkVPhaseDef_HV[iNoStepsH:])
            toggle_controls(True, entries_dTurns_HV[:iNoStepsH])
            toggle_controls(False, entries_dTurns_HV[iNoStepsH:])
            toggle_controls(True, entries_dCircTurns_HV[:iNoStepsH])
            toggle_controls(False, entries_dCircTurns_HV[0])
            toggle_controls(False, entries_dCircTurns_HV[iNoStepsH:])
            toggle_controls(True, entries_dError_HV[:iNoStepsH])
            toggle_controls(False, entries_dError_HV[iNoStepsH:])

    tab_core.update_controls_visibility = update_controls_visibility
    tab_core.update_controls_visibility()

    return tab_core

def on_lamination_change(event, selected,device,controls):
    combo_dSteelWidth = controls[0]
    selected_enum = selected#eLaminationType(selected)

    deviceE = device.parent_device
    iNoPhases = deviceE.fields["iNoPhases"].get()

    if selected_enum == eLaminationType.Am.value or selected_enum == eLaminationType.AmHB1.value:
        if iNoPhases==1:
            combo_dSteelWidth["values"] = device.fields["d_SteelWidthMap"].current["TypeA"]
        else:
            combo_dSteelWidth["values"] = device.fields["d_SteelWidthMap"].current["TypeA3"]
    else:
        combo_dSteelWidth["values"] = device.fields["d_SteelWidthMap"].current["TypeB"]

    combo_dSteelWidth.current(0)  # Select the first value automatically

    hide_controls(selected_enum!=eLaminationType.Am,controls[1:]) # Show/hide controls based on selection

def on_steelwidth_change(event, selected,device,controls):
    combo_dTotalStack = controls[0]
    selected_enum = selected

    list_totalStack = get_list_total_stack(device)

    combo_dTotalStack["values"] = list_totalStack


def get_list_total_stack(device):
    tLaminationTypeE = device.fields["tLaminationType"].get()
    bAmorphous = True if tLaminationTypeE == eLaminationType.Am or tLaminationTypeE == eLaminationType.AmHB1 else False
    dLimMax = 2.7 if bAmorphous else 2.3#1.8
    dLimMin = 1.22#1.3
    dSteelWidth = device.fields["dSteelWidth"].get()
    iLowLim = math.floor(dSteelWidth/dLimMax)
    iHighLim = math.ceil(dSteelWidth/dLimMin)
    if iLowLim<1:
        iLowLim = 1

    if iHighLim>15 or iHighLim==0:
        iHighLim=15
    list_total = []
    for i in range(iLowLim,iHighLim+1):
        if i>=dSteelWidth/dLimMax and i<=dSteelWidth/dLimMin:
            list_total.append(i)
        if (i+1/8)>=dSteelWidth/dLimMax and (i+1/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 1/8)
        if (i+2/8)>=dSteelWidth/dLimMax and (i+2/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 2/8)
        if (i+3/8)>=dSteelWidth/dLimMax and (i+3/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 3/8)
        if (i+4/8)>=dSteelWidth/dLimMax and (i+4/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 4/8)
        if (i+5/8)>=dSteelWidth/dLimMax and (i+5/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 5/8)
        if (i+6/8)>=dSteelWidth/dLimMax and (i+6/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 6/8)
        if (i+7/8)>=dSteelWidth/dLimMax and (i+7/8)<=dSteelWidth/dLimMin:
            list_total.append(i + 7/8)

    device.fields["dTotalStack"].assign_default(list_total[0])
    return list_total
