# tab_tests_ui.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size,get_enum_safe
from Definitions.enums import eLogical,eWindingSelection,eTapExtremes,eSequence,eStandardTemperatureRise,eStandardLightningImpulse,eDesignSpec,eStandardRIV,eStandardSoundLevel,eStandardPartialDischarge

def add_restore_default(widget, default_value, widget_var=None):
    menu = tk.Menu(widget, tearoff=0)
    def restore():
        if isinstance(widget, tk.Entry):
            widget.delete(0, tk.END)
            widget.insert(0, default_value)
        elif isinstance(widget, ttk.Combobox):
            widget.set(default_value)
        elif isinstance(widget, tk.Checkbutton) and widget_var is not None:
            widget_var.set(default_value)
    menu.add_command(label="Restore Default", command=restore)
    
    def show_menu(event):
        menu.tk_popup(event.x_root, event.y_root)
    
    widget.bind("<Button-3>", show_menu)  # Windows/Linux
    widget.bind("<Button-2>", show_menu)  # Mac

def build_tests_tab(notebook, device):

    # --- Tab 5: Tests---
    tab_tests = ttk.Frame(notebook)
    notebook.add(tab_tests, text="Tests")

    size(tab_tests)

    iColumn = 0
    iIndexType = 0
    iIndexType1 = 0
    iIndexType2 = 0
    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()

    #Routine tests
    create_label(tab_tests,iIndexType,iColumn,"Routine Tests",font_size=12,bold = True)
    iIndexType+=1

    entry_dHipotHV = create_entry_focus(tab_tests,iIndexType+1,iColumn,"Hipot kV HV","dHipotHV",device.tests,sticky="nsew")
    add_restore_default(entry_dHipotHV, default_value=device.tests.fields["dHipotHV"].getdefault())

    entry_dHipotLV = create_entry_focus(tab_tests,iIndexType+2,iColumn,"Hipot kV LV","dHipotLV",device.tests,sticky="nsew")

    controls_Hipot = [entry_dHipotHV,entry_dHipotLV]
    combo_tHipot = create_combobox_focus(tab_tests,iIndexType,iColumn,"Hipot",list(eWindingSelection),"tHipot",device.tests,width=27,combos=controls_Hipot,on_select_func=on_Hipot_change)
    add_restore_default(combo_tHipot, default_value=device.tests.fields["tHipot"].getdefault().value)
    on_Hipot_change(None, combo_tHipot.var.get(), device, controls_Hipot)
    iIndexType+=3

    entry_dXRatedVoltage = create_entry_focus(tab_tests,iIndexType+1,iColumn,"X Rated Voltage","dXRatedVoltage",device.tests,sticky="nsew")
    controls_induced = [entry_dXRatedVoltage]
    combo_bInduced = create_combobox_focus(tab_tests,iIndexType,iColumn,"Induced",list(eLogical),"bInduced",device.tests,width=27,combos=controls_induced,on_select_func=on_bool_change)
    on_bool_change(None, combo_bInduced.var.get(), device, controls_induced)
    iIndexType+=2

    entries_resistance = []
    combo_bOtherResistance = create_combobox_focus(tab_tests,iIndexType+1,iColumn,"Other resistance",list(eLogical),"bOtherResistance",device.tests,width=27)
    entries_resistance.append(combo_bOtherResistance)

    combo_tTapsExtremesORAII = create_combobox_focus(tab_tests,iIndexType+2,iColumn,"Taps Extremes OR AII",list(eTapExtremes),"tTapsExtremesORAII",device.tests,width=27)
    entries_resistance.append(combo_tTapsExtremesORAII)

    entry_sSampleRate = create_entry_focus(tab_tests,iIndexType+3,iColumn,"Sample Rate","sSampleRate",device.tests,sticky="nsew")
    entries_resistance.append(entry_sSampleRate)

    combo_bResistance = create_combobox_focus(tab_tests,iIndexType,iColumn,"Resistance test",list(eLogical),"bResistance",device.tests,width=27,combos=entries_resistance,on_select_func=on_bool_change)
    on_bool_change(None, combo_bResistance.var.get(), device, entries_resistance)

    iIndexType+=4

    iColumn+=2
    iIndexType1 =1
    
    entries_coreload = []

    entry_dExtraNoLoad = create_entry_focus(tab_tests,iIndexType1+1,iColumn,"Extra No Load (%Nom)","dExtraNoLoad",device.tests,sticky="nsew")
    entries_coreload.append(entry_dExtraNoLoad)

    entry_dExtraNoLoad2 = create_entry_focus(tab_tests,iIndexType1+2,iColumn,"2 Extra No Load (%Nom)","dExtraNoLoad2",device.tests,sticky="nsew")
    entries_coreload.append(entry_dExtraNoLoad2)

    combo_bCoreLossTest = create_combobox_focus(tab_tests,iIndexType1,iColumn,"Core Loss Test",list(eLogical),"bCoreLossTest",device.tests,width=27,combos=entries_coreload,on_select_func=on_bool_change)
    on_bool_change(None, combo_bCoreLossTest.var.get(), device, entries_coreload)
    iIndexType1+=3

    entries_losskVA = []
    entry_d3LoadLosskVA = create_entry_focus(tab_tests,iIndexType1+1,iColumn,"3rd Load Loss %kVA","d3LoadLosskVA",device.tests,sticky="nsew")
    entries_losskVA.append(entry_d3LoadLosskVA)

    entry_d4LoadLosskVA = create_entry_focus(tab_tests,iIndexType1+2,iColumn,"4th Load Loss %kVA","d4LoadLosskVA",device.tests,sticky="nsew")
    entries_losskVA.append(entry_d4LoadLosskVA)

    entry_d5LoadLosskVA = create_entry_focus(tab_tests,iIndexType1+2,iColumn,"5th Load Loss %kVA","d5LoadLosskVA",device.tests,sticky="nsew")
    entries_losskVA.append(entry_d5LoadLosskVA)

    combo_bLoadLossTest = create_combobox_focus(tab_tests,iIndexType1,iColumn,"Load Loss Test",list(eLogical),"bLoadLossTest",device.tests,width=27,combos=entries_losskVA,on_select_func=on_bool_change)
    on_bool_change(None, combo_bLoadLossTest.var.get(), device, entries_losskVA)
    iIndexType1+=3

    entries_lossTap = []
    entry_d3LoadLossTap = create_entry_focus(tab_tests,iIndexType1+1,iColumn,"3rd Load Loss %Tap","d3LoadLossTap",device.tests,sticky="nsew")
    entries_lossTap.append(entry_d3LoadLossTap)

    entry_d4LoadLossTap = create_entry_focus(tab_tests,iIndexType1+2,iColumn,"4th Load Loss %Tap","d4LoadLossTap",device.tests,sticky="nsew")
    entries_lossTap.append(entry_d4LoadLossTap)

    entry_d5LoadLossTap = create_entry_focus(tab_tests,iIndexType1+2,iColumn,"5th Load Loss %Tap","d4LoadLossTap",device.tests,sticky="nsew")
    entries_lossTap.append(entry_d5LoadLossTap)

    combo_bLoadLoss95Tap = create_combobox_focus(tab_tests,iIndexType1,iColumn,"Load Loss % Tap",list(eLogical),"bLoadLossTap",device.tests,width=27,combos=entries_lossTap,on_select_func=on_bool_change)
    on_bool_change(None, combo_bLoadLoss95Tap.var.get(), device, entries_lossTap)

    iIndexType1+=3

    entries_otherRoutine = []
    entry_sOtherRoutineComments = create_entry_focus(tab_tests,iIndexType1+1,iColumn,"Comments","sOtherRoutineComments",device.tests,sticky="nsew")
    entries_otherRoutine.append(entry_sOtherRoutineComments)

    combo_OtherRoutine = create_combobox_focus(tab_tests,iIndexType1,iColumn,"Other Routine Tests",list(eLogical),"bOtherRoutine",device.tests,width=27,combos=entries_otherRoutine,on_select_func=on_bool_change)
    on_bool_change(None, combo_OtherRoutine.var.get(), device, entries_otherRoutine)
    iIndexType1+=2

    iColumn+=2
    iIndexType2 =0

    create_label(tab_tests,iIndexType2,iColumn,"Type Tests",font_size=12,bold = True)
    iIndexType2+=1

    entries_TR =[]

    combo_sSpecificationsTR = create_combobox_focus(tab_tests,iIndexType2+1,iColumn,"To Which Specifications",list(eStandardTemperatureRise),"tSpecificationsTR",device.tests,width=27,sticky="nsew")
    entries_TR.append(combo_sSpecificationsTR)

    entry_sCommentsTR = create_entry_focus(tab_tests,iIndexType2+2,iColumn,"Comments","sCommentsTR",device.tests,sticky="nsew")
    entries_TR.append(entry_sCommentsTR)

    combo_bTemperatureRise = create_combobox_focus(tab_tests,iIndexType2,iColumn,"Temperature Rise",list(eLogical),"bTemperatureRise",device.tests,width=27,combos=entries_TR,on_select_func=on_bool_change)
    on_bool_change(None, combo_bTemperatureRise.var.get(), device, entries_TR)
    iIndexType2+=3

    entries_LI = []
    
    combo_sSpecificationsLI = create_combobox_focus(tab_tests,iIndexType2+1,iColumn,"To Which Specifications",list(eStandardLightningImpulse),"tSpecificationsLI",device.tests,width=27,sticky="nsew")
    entries_LI.append(combo_sSpecificationsLI)

    combo_tSequenceLI = create_combobox_focus(tab_tests,iIndexType2+2,iColumn,"Sequence",list(eSequence),"tSequenceLI",device.tests,width=27)
    entries_LI.append(combo_tSequenceLI)

    combo_tImpulse = create_combobox_focus(tab_tests,iIndexType2+3,iColumn,"Impulse",list(eWindingSelection),"tImpulse",device.tests,width=27)
    entries_LI.append(combo_tImpulse)

    entry_sCommentsLI = create_entry_focus(tab_tests,iIndexType2+4,iColumn,"Comments","sCommentsLI",device.tests,sticky="nsew")
    entries_LI.append(entry_sCommentsLI)

    combo_bLightningImpulse = create_combobox_focus(tab_tests,iIndexType2,iColumn,"Lightning Impulse",list(eLogical),"bLightningImpulse",device.tests,width=27,combos=entries_LI,on_select_func=on_bool_change)
    on_bool_change(None, combo_bLightningImpulse.var.get(), device, entries_LI)
    iIndexType2+=5

    entries_RIV = []

    combo_sSpecificationsRIV = create_combobox_focus(tab_tests,iIndexType2+1,iColumn,"To Which Specifications",list(eStandardRIV),"tSpecificationsRIV",device.tests,width=27,sticky="nsew")
    entries_RIV.append(combo_sSpecificationsRIV)

    entry_sCommentsRIV = create_entry_focus(tab_tests,iIndexType2+2,iColumn,"Comments","sCommentsRIV",device.tests,sticky="nsew")
    entries_RIV.append(entry_sCommentsRIV)

    combo_bRIV = create_combobox_focus(tab_tests,iIndexType2,iColumn,"Radio Influence Voltage",list(eLogical),"bRIV",device.tests,width=27,combos=entries_RIV,on_select_func=on_bool_change)
    on_bool_change(None, combo_bRIV.var.get(), device, entries_RIV)
    iIndexType2+=3

    iColumn+=2
    entries_SL = []
    iIndexType3 = 1

    combo_sSpecificationsSL = create_combobox_focus(tab_tests,iIndexType3+1,iColumn,"To Which Specifications",list(eStandardSoundLevel),"tSpecificationsSL",device.tests,width=27,sticky="nsew")
    entries_SL.append(combo_sSpecificationsSL)

    entry_sCommentsSL = create_entry_focus(tab_tests,iIndexType3+2,iColumn,"Comments","sCommentsSL",device.tests,sticky="nsew")
    entries_SL.append(entry_sCommentsSL)

    combo_bSoundLevel = create_combobox_focus(tab_tests,iIndexType3,iColumn,"Sound Level",list(eLogical),"bSoundLevel",device.tests,width=27,combos=entries_SL,on_select_func=on_bool_change)
    on_bool_change(None, combo_bSoundLevel.var.get(), device, entries_SL)
    iIndexType3+=3

    entries_PD = []

    combo_sSpecificationsPD = create_combobox_focus(tab_tests,iIndexType3+1,iColumn,"To Which Specifications",list(eStandardPartialDischarge),"tSpecificationsPD",device.tests,width=27,sticky="nsew")
    entries_PD.append(combo_sSpecificationsPD)

    entry_sCommentsPD = create_entry_focus(tab_tests,iIndexType3+2,iColumn,"Comments","sCommentsPD",device.tests,sticky="nsew")
    entries_PD.append(entry_sCommentsPD)

    combo_bPartialDischarge = create_combobox_focus(tab_tests,iIndexType3,iColumn,"Partial Discharge",list(eLogical),"bPartialDischarge",device.tests,width=27,combos=entries_PD,on_select_func=on_bool_change)
    on_bool_change(None, combo_bPartialDischarge.var.get(), device, entries_PD)
    iIndexType3+=3

    entries_OT = []

    entry_sOtherRoutineCommentsType = create_entry_focus(tab_tests,iIndexType3+1,iColumn,"Comments","sOtherRoutineCommentsType",device.tests,sticky="nsew")
    entries_OT.append(entry_sOtherRoutineCommentsType)

    combo_OtherTypeTest = create_combobox_focus(tab_tests,iIndexType3,iColumn,"Other Type Tests",list(eLogical),"bOtherTypeTest",device.tests,width=27,combos=entries_OT,on_select_func=on_bool_change)
    on_bool_change(None, combo_OtherTypeTest.var.get(), device, entries_OT)
    iIndexType3+=1

    entry_dDissipationFactor = create_entry_focus(tab_tests,iIndexType3+1,iColumn,"Dissipation Factor (%)","dDissipationFactor",device.tests,sticky="nsew")
    entries_OT.append(entry_dDissipationFactor)

    iIndexType3+=2
    return tab_tests


def on_Hipot_change(event, selected, device, controls):
        selected_enum = get_enum_safe(eWindingSelection, selected)

        hide_controls(True,controls) # Show/hide controls based on selection
        controlsE = []

        if selected_enum == eWindingSelection.NONE:
            controlsE = controls
        elif selected_enum == eWindingSelection.HV:
            controlsE = [controls[1]]
        elif selected_enum == eWindingSelection.LV:
            controlsE = [controls[0]]

        hide_controls(False,controlsE) # Show/hide controls based on selection

def on_bool_change(event, selected, device, controls):
        selected_enum = get_enum_safe(eLogical, selected)
        hide_controls(selected_enum==eLogical.TRUE,controls) # Show/hide controls based on selection


