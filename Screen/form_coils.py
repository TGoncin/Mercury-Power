#form_coils.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size
from Definitions.enums import eLaminationType,eMaterial,eLeads,eBars,eCoating,eTypeConductor,eLogical

def open_coils_form(root,frame_coils_design = None,device = None, iIndex = 0):

    if frame_coils_design is None:
        frame_coils_design = tk.Toplevel(root)
        frame_coils_design.title("Coils Design")
        frame_coils_design.geometry("800x600")

    # container = ttk.Frame(frame_coils_design)
    # container.pack(fill="both", expand=True)

    # canvas = tk.Canvas(container)
    # canvas.pack(side="left", fill="both", expand=True)

    # scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
    # scrollbar.pack(side="right", fill="y")

    # canvas.configure(yscrollcommand=scrollbar.set)

    # # Permite scrolling con rueda del mouse
    # def _on_mousewheel(event):
    #     canvas.yview_scroll(int(-1*(event.delta/120)), "units")

    # canvas.bind_all("<MouseWheel>", _on_mousewheel)

    # # Frame interno donde vas a poner todos tus labels/entries
    # frame_coils_design = ttk.Frame(canvas)
    # canvas.create_window((0, 0), window=frame_coils_design, anchor="nw")

    # # Ajustar tamaño del scroll cuando se agregan widgets
    # def configure_scroll_region(event):
    #     canvas.configure(scrollregion=canvas.bbox("all"))

    # frame_coils_design.bind("<Configure>", configure_scroll_region)


    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()
    coreE = device.core
    pLCoil = pLVWind.coils[0]
    pHCoil = pHVWind.coils[0]
    pCondL0 = pLCoil.conductor[0]
    pCondH0 = pHCoil.conductor[0]
    coolingE = device.cooling
    gradientsE = device.gradients
    lossesE = device.losses

    #Conductor caculates LV
    iColumn_design = 1
    iIndexCol21 = iIndex+1
    
    entries_calculate_coil = []

    create_label(frame_coils_design, iIndexCol21, iColumn_design, "Form Dimensions", bold=True)

    create_label(frame_coils_design, iIndexCol21, iColumn_design + 1, "Low Voltage", bold=True)
    iIndexCol21 += 1
    iWidthE = 16

    entry_sForms = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"","sForms",coreE,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sForms)

    entry_UnderInnerLV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design+1,"","sUnderInner",device,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_UnderInnerLV)
    iIndexCol21 +=1

    entry_sFormRBWS = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"","sFormRBWS",device,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sFormRBWS)

    entry_sOverLV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design+1,"","sOver",pLCoil,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sOverLV)

    iIndexCol21 +=1

    entry_dTurns_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Turns","dNomTurnsUI",pLVWind,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dTurns_LV)
    iIndexCol21 +=1

    entry_dConductorSpace_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Conductor Space","dConductorSpace",pCondL0,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dConductorSpace_LV)
    iIndexCol21 +=1

    entry_dStayback_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Stayback","dStayback",pCondL0,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dStayback_LV)
    iIndexCol21 +=1

    entry_dOvercoil_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Overcoil","dOvercoil",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dOvercoil_LV)
    iIndexCol21 +=1

    entry_iSectionsLeg_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Sections/Leg","iSectionsLeg",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_iSectionsLeg_LV)
    iIndexCol21 +=1

    entry_dTurnsSection_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Turns/Section","dTurnsSection",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dTurnsSection_LV)
    iIndexCol21 +=1

    entry_iTurnsLayers_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Turns/Layer","iTurnsLayers",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_iTurnsLayers_LV)
    iIndexCol21 +=1

    entry_dNoLayers_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"No of Layers","dNoLayersBody",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dNoLayers_LV)
    iIndexCol21 +=1

    entry_sInsulationEnd_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Layer Insulation","sInsulationEnd",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sInsulationEnd_LV)
    iIndexCol21 +=1

    entry_sInsulationInt_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design," ","sInsulationIntComplete",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sInsulationInt_LV)
    iIndexCol21 +=1

    create_label(frame_coils_design,iIndexCol21,iColumn_design,"Nose Ducts",)
    entry_sNoseDuctsInner_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"-Inner","sNoseDuctsInner",pLVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sNoseDuctsInner_LV)
    iIndexCol21 +=1

    entry_sWidthDucts_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"","sWidthDucts",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sWidthDucts_LV)
    entry_sNoseDuctsOuter_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"-Outer","sNoseDuctsOuter",pLVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sNoseDuctsOuter_LV)
    iIndexCol21 +=1

    entry_sRadialBuilds_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"Radial Builds","sRadialBuilds",pLCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sRadialBuilds_LV)
    iIndexCol21 +=1

    combo_bEndFiller_LV = create_combobox_focus(frame_coils_design,iIndexCol21,iColumn_design,"Endfiller",list(eLogical),"bEndFiller",pLVWind,width=iWidthE)
    entries_calculate_coil.append(combo_bEndFiller_LV)
    iIndexCol21 +=1

    entry_sLeadsOutput_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"0.010 Lead Pads","sLeadsOutput",pLVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sLeadsOutput_LV)
    iIndexCol21 +=1

    entry_sLeadsOutputPad_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"1/16 PB Lead Pad","sLeadsOutputPad",pLVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sLeadsOutputPad_LV)
    iIndexCol21 +=1

    entry_sLeadsOutputPad_LV = create_entry_focus(frame_coils_design,iIndexCol21,iColumn_design,"1/16 PB Lead Pad (LV St&Fin)","sLeadsStartFinPads",pLVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sLeadsOutputPad_LV)
    iIndexCol21 +=1

    #Conductor caculates HV
    iColumn_design+=3
    iIndexCol31 = iIndex+1
    pHCoil = pHVWind.coils[0]

    create_label(frame_coils_design,iIndexCol31,iColumn_design,"High Voltage",bold = True)
    iIndexCol31+=1

    entry_sHLUnderHVWS = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sHLUnderHVWS",pHVWind,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sHLUnderHVWS)

    entry_sRBHL = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design+2,"","sHLUnderHBRBWS",pHVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_calculate_coil.append(entry_sRBHL)

    iIndexCol31 +=1

    entry_sOverHV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sOver",pHCoil,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sOverHV)

    entry_sRBHO = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design+2,"","sHLOverHVSideRBWS",pHVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_calculate_coil.append(entry_sRBHO)

    iIndexCol31 +=1

    entry_dTurns_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dTurns",pHCoil,width=iWidthE,sticky="ew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dTurns_HV)
    iIndexCol31 +=1

    entry_dConductorSpace_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dConductorSpace",pCondH0,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dConductorSpace_HV)
    iIndexCol31 +=1

    entry_dStayback_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dStayback",pCondH0,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dStayback_HV)
    iIndexCol31 +=1

    entry_dOvercoil_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dOvercoil",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dOvercoil_HV)
    entry_sEndFiller_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design+2,"","sEndFiller",pHVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sEndFiller_HV)
    iIndexCol31 +=1

    entry_iSectionsLeg_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","iSectionsLeg",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_iSectionsLeg_HV)
    iIndexCol31 +=1

    entry_dTurnsSection_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dTurnsSection",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dTurnsSection_HV)
    iIndexCol31 +=1

    entry_iTurnsLayers_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","iTurnsLayers",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_iTurnsLayers_HV)

    create_label(frame_coils_design,iIndexCol31,iColumn_design+2,"HV Tot Lyrs")
    iIndexCol31 +=1

    entry_dNoLayers_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","dNoLayersBody",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dNoLayers_HV)
    entry_dTotLayers_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design+2,"","dTotLayers",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_dTotLayers_HV)
    iIndexCol31 +=1

    entry_sInsulationEnd_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sInsulationEnd",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sInsulationEnd_HV)
    iIndexCol31 +=1

    entry_sInsulationInt_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sInsulationIntComplete",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sInsulationInt_HV)
    iIndexCol31 +=1

    #create_label(frame_coils_design_coils,iIndexCol31,iColumn-1,"Nose Ducts",)
    entry_sNoseDuctsInner_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sNoseDuctsInner",pHVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sNoseDuctsInner_HV)
    iIndexCol31 +=1

    # entry_sWidthDucts_HV = create_entry_focus(frame_coils_design_coils,iIndexCol31,iColumn-1,"","sWidthDucts",pHCoil,width=10,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sWidthDucts_HV)

    entry_sNoseDuctsOuter_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sNoseDuctsOuter",pHVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sNoseDuctsOuter_HV)
    iIndexCol31 +=1

    entry_sRadialBuilds_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sRadialBuilds",pHCoil,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sRadialBuilds_HV)
    iIndexCol31 +=1

    combo_bEndFiller_HV = create_combobox_focus(frame_coils_design,iIndexCol31,iColumn_design,"",list(eLogical),"bEndFiller",pHVWind,width=iWidthE)
    entries_calculate_coil.append(combo_bEndFiller_HV)
    iIndexCol31 +=1

    entry_sLeadsOutput_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_design,"","sLeadsOutput",pHVWind,width=iWidthE,sticky="nsew",bOutput = True,columnspan=2)
    entries_calculate_coil.append(entry_sLeadsOutput_HV)
    iIndexCol31 +=1

    create_label(frame_coils_design,iIndexCol31,iColumn_design,"")
    iIndexCol31 +=1

    create_label(frame_coils_design,iIndexCol31,iColumn_design,"")
    iIndexCol31 +=1

    iColumn_conductor=1

    # #conductor LV
    # iIndexCol31 +=1
    # iColumn_conductor = 0
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    # iIndexCol31+=1
    # pLConductor = pLVWind.coils[0].conductor[0]

    # entry_sSWConductor_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sSWConductor",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sSWConductor_LV)
    # iColumn_conductor+=1

    # entry_sMatConductor_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sMatConductor",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sMatConductor_LV)
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"High")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Wide")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Thick / turn")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Width / turn")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Area")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Density")
    # iColumn_conductor+=1

    # iIndexCol31+=1

    # iColumn_conductor=0
    # entry_sDescription_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sDescription",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sDescription_LV)
    # iColumn_conductor+=1

    # entry_sPapperConductor_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sPapperConductor",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sPapperConductor_LV)
    # iColumn_conductor+=1

    # # entry_sWeight_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sWeight",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # # entries_calculate_coil.append(entry_sWeight_LV)
    # # iColumn_conductor+=1

    # entry_iHigh_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","iHigh",pLVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_iHigh_LV)
    # iColumn_conductor+=1

    # entry_iWide_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","iWide",pLVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_iWide_LV)
    # iColumn_conductor+=1

    # entry_dCondThickTurn_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dCondThickTurn",pLVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dCondThickTurn_LV)
    # iColumn_conductor+=1

    # entry_dCondWdthTurn_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dCondWdthTurn",pLVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dCondWdthTurn_LV)
    # iColumn_conductor+=1

    # entry_dAreaCond_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dAreaCond",pLVWind.coils[0].conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dAreaCond_LV)
    # iColumn_conductor+=1

    # entry_dDensity_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sDensity",pLVWind.coils[0].conductor[0],width=iWidthE,sticky="nsew",bOutput = True,warning_func=pLVWind.coils[0].conductor[0].conductor_warnings.warningCurrentDensityMax)
    # entries_calculate_coil.append(entry_dDensity_LV)
    # iColumn_conductor+=1

    # iIndexCol31 +=1

    # #conductor HV
    # iColumn_conductor = 0
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    # iIndexCol31+=1
    # pHConductor = pHVWind.coils[0].conductor[0]

    # entry_sSWConductor_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sSWConductor",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sSWConductor_HV)
    # iColumn_conductor+=1

    # entry_sMatConductor_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sMatConductor",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sMatConductor_HV)
    # iColumn_conductor+=1
    
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"High")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Wide")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Thick / turn")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Width / turn")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Area")
    # iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Density")
    # iColumn_conductor+=1

    # #create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    # #iColumn_conductor+=1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"HV / Tapleads")
    # iIndexCol31+=1

    # iColumn_conductor=0
    # entry_sDescription_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sDescription",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sDescription_HV)
    # iColumn_conductor+=1

    # entry_sPapperConductor_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sPapperConductor",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sPapperConductor_HV)
    # iColumn_conductor+=1

    # # entry_sWeight_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sWeight",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # # entries_calculate_coil.append(entry_sWeight_HV)
    # # iColumn_conductor+=1

    # entry_iHigh_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","iHigh",pHVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_iHigh_HV)
    # iColumn_conductor+=1

    # entry_iWide_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","iWide",pHVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_iWide_HV)
    # iColumn_conductor+=1

    # entry_dCondThickTurn_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dCondThickTurn",pHVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dCondThickTurn_HV)
    # iColumn_conductor+=1

    # entry_dCondWdthTurn_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dCondWdthTurn",pHVWind.coils[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dCondWdthTurn_HV)
    # iColumn_conductor+=1

    # entry_dAreaCond_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dAreaCond",pHVWind.coils[0].conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dAreaCond_HV)
    # iColumn_conductor+=1

    # entry_dDensity_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sDensity",pHVWind.coils[0].conductor[0],width=iWidthE,sticky="nsew",bOutput = True,warning_func=pHVWind.coils[0].conductor[0].conductor_warnings.warningCurrentDensityMax)
    # entries_calculate_coil.append(entry_dDensity_HV)
    # iColumn_conductor+=1

    # entry_sHVTapLeads = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","sHVTapLeads",pHVWind,width=iWidthE,sticky="nsew",bOutput = True)
    # #entries_calculate_coil.append(entry_dDensity_HV)
    # iColumn_conductor+=1

    iIndexCol31+=1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    iIndexCol31+=1

    iColumn_conductor=1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+3,"Coil sizes")
    iIndexCol31+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"RBIW")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Total RBIW")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"LV Nose")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"HV Nose")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Sides")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Noses")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"LV lead bld")
    iColumn_conductor+=1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"HV lead bld")
    iColumn_conductor+=1
    iIndexCol31 += 1

    iColumn_conductor = 1
    entry_dRBIW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dRBIW",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dRBIW)
    iColumn_conductor+=1

    entry_dTotalRBIW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dTotalRBIW",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalRBIW)
    iColumn_conductor+=1

    entry_dNose_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dNose",pLVWind,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNose_LV)
    iColumn_conductor+=1

    entry_dNose_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dNose",pHVWind,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNose_HV)
    iColumn_conductor+=1

    entry_dSides = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dSides",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dSides)
    iColumn_conductor+=1

    entry_dNoses = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dNoses",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNoses)
    iColumn_conductor+=1

    entry_dNoses_LV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dNoses",pLVWind,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNoses_LV)
    iColumn_conductor+=1

    entry_dNoses_HV = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","dNoses",pHVWind,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNoses_HV)
    iColumn_conductor+=1

    iIndexCol31 +=1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    iIndexCol31 +=1
    iColumn_conductor -= 2
    indexWeights = iIndexCol31

    # #weights
    # create_label(frame_coils_design,indexWeights,iColumn_conductor,"Weights (lbs / Unit)", sticky = "nsew",bold=True, columnspan=2)
    # indexWeights += 1

    # entry_dWeight_LV = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"LV Cond","dWeight",pLConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dWeight_LV)
    # indexWeights+=1

    # entry_dWeight_HV = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"HV Cond","dWeight",pHConductor,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dWeight_HV)
    # indexWeights+=1

    # entry_dWeightTwoLoops = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"Core","dWeightTwoLoops",coreE,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightTwoLoops)
    # indexWeights +=1

    # entry_dWeightKr0022 = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,".0022 TKr","dWeightKr0022",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightKr0022)
    # indexWeights +=1

    # entry_dWeightKr005 = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,".005 TKr","dWeightKr005",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightKr005)
    # indexWeights +=1

    # entry_dWeightKr010 = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,".010 TKr","dWeightKr010",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightKr010)
    # indexWeights +=1

    # entry_dWeightKr015 = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,".015 TKr","dWeightKr015",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightKr015)
    # indexWeights +=1

    # entry_dWeightCrepe = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"Crepe","dWeightCrepe",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightCrepe)
    # indexWeights +=1

    # entry_dWeightPressBoard = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"Pressboard","dWeightPressBoard",coreE,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dWeightPressBoard)
    # indexWeights +=1

    # entry_dTotalCC = create_entry_focus(frame_coils_design,indexWeights,iColumn_conductor,"Total C & C","dTotalCC",device,width=20,sticky="nsew",bOutput=True)
    # entries_calculate_coil.append(entry_dTotalCC)
    # indexWeights +=1

    # iColumn_conductor = 0
    # #guaranteed values
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"Calculated & Guaranteed Values", sticky = "nsew",bold=True, columnspan=5)
    # iIndexCol31 += 1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"NL @ 85C, Rated Volts", sticky = "nsew", columnspan=3)

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+4,"LL 85C Rated kVA and Tap", sticky = "nsew", columnspan=2)
    # iIndexCol31 += 1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"Core")
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+2,"Load Loss")
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+3,"Total Loss")
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+4,"%IZ")
    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor+5,"%Ex")
    # iIndexCol31 += 1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Calculated:")
    # entry_dCoreLoss = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dCoreLoss",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dCoreLoss)

    # entry_dTotalLLossStrayLead1 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dTotalLLossStrayLead",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dTotalLLossStrayLead1)

    # entry_dTotalLoss = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dTotalLoss",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dTotalLoss)

    # entry_dIZWithLeads = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dIZWithLeads",device,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dIZWithLeads)

    # entry_dPerExc = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dPerExc",device,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_dPerExc)

    # iIndexCol31 += 1

    # create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Guarantee:")
    # entry_sNLGuarantee = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","sNLGuarantee",device.guarantee,width=iWidthE,sticky="nsew",bOutput = True,warning_func=lossesE.losses_warnings.warningValueCoreLosses)
    # entries_calculate_coil.append(entry_sNLGuarantee)

    # entry_sLLGuarantee = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","sLLGuarantee",device.guarantee,width=iWidthE,sticky="nsew",bOutput = True,warning_func=lossesE.losses_warnings.warningValueLoadLosses)
    # entries_calculate_coil.append(entry_sLLGuarantee)

    # entry_sTotLGuarantee = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","sTotLGuarantee",device.guarantee,width=iWidthE,sticky="nsew",bOutput = True,warning_func=lossesE.losses_warnings.warningValueTotalLosses)
    # entries_calculate_coil.append(entry_sTotLGuarantee)

    # entry_sIZGuarantee = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","sIZGuarantee",device.guarantee,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sIZGuarantee)

    # entry_sExcGuarantee = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","sExcGuarantee",device.guarantee,width=iWidthE,sticky="nsew",bOutput = True)
    # entries_calculate_coil.append(entry_sExcGuarantee)

    # iIndexCol31 += 1
    # iIndexCol31 += 1

    iColumn_conductor = 0
    #temp rise cooling data
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Temp Rise Data (Rated kVA & Tap)", sticky = "nsew",bold=True, columnspan=4)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+4,"Cooling Data 65 ONAN", sticky = "nsew",bold=True, columnspan=2)
    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Rise C")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"ILV Grad C")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+2,"HV Grad C")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+3,"OLV Grad C")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+4,"Cooling Data")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+5,"Max Watts")
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+6,"TOR C")
    iIndexCol31 += 1

    entry_tTemperatureRises = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"","tTemperatureRises",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_tTemperatureRises)

    entry_dGradientFinal0 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dGradientFinal[0]",gradientsE,width=iWidthE,sticky="nsew",bOutput = True,warning_func=gradientsE.gradient_warnings.warningValueGradientCoil)
    entries_calculate_coil.append(entry_dGradientFinal0)

    entry_dGradientFinal1 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dGradientFinal[1]",gradientsE,width=iWidthE,sticky="nsew",bOutput = True,warning_func=gradientsE.gradient_warnings.warningValueGradientCoil)
    entries_calculate_coil.append(entry_dGradientFinal1)

    entry_dGradientFinal2 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dGradientFinal[2]",gradientsE,width=iWidthE,sticky="nsew",bOutput = True,warning_func=gradientsE.gradient_warnings.warningValueGradientCoil)
    entries_calculate_coil.append(entry_dGradientFinal2)

    entry_sTempCoolRise = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","sTempCoolRise[0]",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_sTempCoolRise)

    entry_dMaxWatts = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dMaxWatts",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dMaxWatts)

    entry_dTORFinal = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dTORFinal",gradientsE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTORFinal)
    iIndexCol31 += 1

    entry_sTempCoolRise1 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","sTempCoolRise[1]",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_sTempCoolRise1)

    entry_dMaxONAF = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dMaxONAF[1]",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dMaxONAF)

    entry_dTORONAF = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dTORONAF[1]",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTORONAF)

    iIndexCol31 += 1
    entry_sUseFR3 = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","sUseFR3",coolingE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_sUseFR3)
    iIndexCol31 += 1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    iIndexCol31 += 1

    iColumn_conductor = 0
    #Resistance and Loss Summary
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Resistance and Loss Summary",bold=True, columnspan=6)
    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"Resistances (per Phase)", sticky = "nsew", columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+3,"Watts @ Rated Current", sticky = "nsew", columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+5,"Watts @ FCBN", sticky = "nsew" ,columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"Stray Loss", sticky = "nsew", columnspan=2)
    iIndexCol31 += 1

    entry_dCurrentTempRiseR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dCurrentTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dCurrentTempRiseR)

    entry_dNewTempRiseR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dNewTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dNewTempRiseR)

    entry_dCurrentTempRiseL = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dCurrentTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dCurrentTempRiseL)

    entry_dNewTempRiseL = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dNewTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dNewTempRiseL)

    entry_dCurrentTempRiseT = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dCurrentTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dCurrentTempRiseT)

    entry_dNewTempRiseT = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dNewTempRise",coolingE,width=iWidthE,sticky="nsew",bInfo = True)
    entries_calculate_coil.append(entry_dNewTempRiseT)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"Coil Stray")
    entry_dCoilSray = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+8,"","dCoilSray",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dCoilSray)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"LV w/o Lds:")

    entry_dLVWOLeadsCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dLVWOLeadsCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWOLeadsCurrentTempR)

    entry_dLVWOLeadsNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dLVWOLeadsNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWOLeadsNewTempR)

    entry_dLVWOLeadsCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dLVWOLeadsCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWOLeadsCurrentTempW)

    entry_dLVWOLeadsNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dLVWOLeadsNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWOLeadsNewTempW)

    entry_dLVWLeadsCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dLVWOLeadsCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWLeadsCurrentTempTaps)

    entry_dLVWLeadsNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dLVWOLeadsNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVWLeadsNewTempTaps)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"Tank Stray")
    entry_dTankStray = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+8,"","dTankStray",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTankStray)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"LV Leads:")

    entry_dLVLeadsCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dLVLeadsCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsCurrentTempR)

    entry_dLVLeadsNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dLVLeadsNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsNewTempR)

    entry_dLVLeadsCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dLVLeadsCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsCurrentTempW)

    entry_dLVLeadsNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dLVLeadsNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsNewTempW)

    entry_dLVLeadsCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dLVLeadsCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsCurrentTempTaps)

    entry_dLVLeadsNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dLVLeadsNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadsNewTempTaps)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"Total Stray")
    entry_dTotalStray = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+8,"","dTotalStray",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalStray)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Total LV:")

    entry_dTotalLVCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dTotalLVCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVCurrentTempR)

    entry_dTotalLVNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dTotalLVNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVNewTempR)

    entry_dTotalLVCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dTotalLVCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVCurrentTempW)

    entry_dTotalLVNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dTotalLVNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVNewTempW)

    entry_dTotalLVCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dTotalLVCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVCurrentTempTaps)

    entry_dTotalLVNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dTotalLVNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLVNewTempTaps)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"LV Lead Loss")
    entry_dLVLeadLoss = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+8,"","dLVLeadLoss",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVLeadLoss)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"HV w/o Lds:")

    entry_dTotalHVCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dHVWOLeadsCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempR)

    entry_dTotalHVNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dHVWOLeadsNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempR)

    entry_dTotalHVCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dHVWOLeadsCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempW)

    entry_dTotalHVNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dHVWOLeadsNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempW)

    entry_dTotalHVCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dHVWOLeadsCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempTaps)

    entry_dTotalHVNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dHVWOLeadsNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempTaps)

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+7,"Tot. L.Loss(S+L)")
    entry_dTotalLLossStrayLead = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+8,"","dTotalLLossStrayLead",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalLLossStrayLead)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"HV Leads")

    entry_dTotalHVCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dHVLeadsCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempR)

    entry_dTotalHVNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dHVLeadsNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempR)

    entry_dTotalHVCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dHVLeadsCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempW)

    entry_dTotalHVNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dHVLeadsNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempW)

    entry_dTotalHVCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dHVLeadsCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempTaps)

    entry_dTotalHVNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dHVLeadsNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempTaps)

    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"Total HV:")

    entry_dTotalHVCurrentTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+1,"","dTotalHVCurrentTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempR)

    entry_dTotalHVNewTempR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+2,"","dTotalHVNewTempR",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempR)

    entry_dTotalHVCurrentTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dTotalHVCurrentTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempW)

    entry_dTotalHVNewTempW = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dTotalHVNewTempW",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempW)

    entry_dTotalHVCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dTotalHVCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVCurrentTempTaps)

    entry_dTotalHVNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dTotalHVNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dTotalHVNewTempTaps)

    iIndexCol31 += 1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+1,"Total I2R Loss:", bold = True, columnspan=2)

    entry_dI2RCurrentTemp = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+3,"","dI2RCurrentTemp",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dI2RCurrentTemp)

    entry_dI2RNewTemp = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+4,"","dI2RNewTemp",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dI2RNewTemp)

    entry_dI2RCurrentTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+5,"","dI2RCurrentTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dI2RCurrentTempTaps)

    entry_dI2RNewTempTaps = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor+6,"","dI2RNewTempTaps",lossesE,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dI2RNewTempTaps)

    iIndexCol31 += 1
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"")
    iIndexCol31 += 1

    create_label(frame_coils_design,iIndexCol31,iColumn_conductor,"%IZ Summary", bold = True, columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+2,"Mean Turn Data", bold = True, columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+4,"Build Factors (%)", bold = True, columnspan=2)
    create_label(frame_coils_design,iIndexCol31,iColumn_conductor+6,"Efficiency", bold = True, columnspan=2)
    iIndexCol31 += 1
    iIndexCol32 = iIndexCol31
    iIndexCol33 = iIndexCol31
    iIndexCol34 = iIndexCol31

        #impedance
    entry_dLineLeadFactor = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"Lead Factor","dLineLeadFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLineLeadFactor)
    iIndexCol31 += 1

    entry_dIXCoil = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"IX % - Coil","dIXCoil",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dIXCoil)
    iIndexCol31 += 1

    entry_dIR = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"IR %","dIR",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dIR)
    iIndexCol31 += 1

    entry_dIZWithoutLeads = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"IZ % -  w/o Ld","dIZWithoutLeads",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dIZWithoutLeads)
    iIndexCol31 += 1

    entry_dIZWithLeads = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"IZ % - w/Ld","dIZWithLeads",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dIZWithLeads)
    iIndexCol31 += 1

    entry_dOWPIZ = create_entry_focus(frame_coils_design,iIndexCol31,iColumn_conductor,"OWP %IZ","dOWPIZ",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dOWPIZ)
    iIndexCol31 += 1

        #mean turn data
    iColumn_conductor += 2
    entry_dFormMT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"Form Circ.","dFormMT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dFormMT)
    iIndexCol32 += 1

    entry_dILV_MT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"ILV-MT","dILV_MT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dILV_MT)
    iIndexCol32 += 1

    entry_dILV_OT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"LV-OT","dILV_OT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dILV_OT)
    iIndexCol32 += 1

    entry_dILH_OT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"LH-OT","dILH_OT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dILH_OT)
    iIndexCol32 += 1

    entry_dHV_MT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"HV-MT","dHV_MT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dHV_MT)
    iIndexCol32 += 1

    entry_dHV_OT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"HV-OT","dHV_OT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dHV_OT)
    iIndexCol32 += 1

    entry_dOLV_MT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"OLV-MT","dOLV_MT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dOLV_MT)
    iIndexCol32 += 1

    entry_dHOL_OT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"HV-Outer-OT","dHOL_OT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dHOL_OT)
    iIndexCol32 += 1

    entry_dLV_MT = create_entry_focus(frame_coils_design,iIndexCol32,iColumn_conductor,"Avg LV-MT","dLV_MT",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLV_MT)
    iIndexCol32 += 1

        #build factor
    iColumn_conductor += 2
    entry_dLVBuildFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"LV RB","dLVBuildFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLVBuildFactor)
    iIndexCol33 += 1

    entry_dHVBuildFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"HV RB","dHVBuildFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dHVBuildFactor)
    iIndexCol33 += 1

    entry_dCoilRB = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"Coil RB","dCoilRB",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dCoilRB)
    iIndexCol33 += 1

    entry_dCoreLossFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"NL","dCoreLossFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dCoreLossFactor)
    iIndexColEval = iIndexCol33
    iIndexCol33 += 1

    entry_dLoadLossFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"LL","dLoadLossFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dLoadLossFactor)
    iIndexCol33 += 1

    entry_dNoseBuildFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"Nose Build","dNoseBuildFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dNoseBuildFactor)
    iIndexCol33 += 1

    entry_dInsulationLengthFactor = create_entry_focus(frame_coils_design,iIndexCol33,iColumn_conductor,"Ins Length","dInsulationLengthFactor",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dInsulationLengthFactor)
    iIndexCol33 += 1

            #efficiency
    iColumn_conductor += 2
    create_label(frame_coils_design,iIndexCol34,iColumn_conductor+1,"Calculated")
    create_label(frame_coils_design,iIndexCol34,iColumn_conductor+2,"Standard")
    iIndexCol34 += 1

    entry_dEffDoe2016eff = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor,"Doe (2016)","sPerEfficiency",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dEffDoe2016eff)
    entry_dEffDoe2016 = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor+2,"","sPerEffDoE2016",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dEffDoe2016)
    iIndexCol34 += 1

    entry_dC8021eff = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor,"C802.1","sPerEfficiency",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dC8021eff)
    entry_dC8021 = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor+2,"","sPerEff8021",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dC8021)
    iIndexCol34 += 1

    entry_dC8023eff = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor,"C802.3","sPerEfficiency",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dC8023eff)
    entry_dC8023 = create_entry_focus(frame_coils_design,iIndexCol34,iColumn_conductor+2,"","sPerEff8023",device,width=iWidthE,sticky="nsew",bOutput = True)
    entries_calculate_coil.append(entry_dC8023)
    iIndexCol34 += 1

    hide_controls(False,entries_calculate_coil)


    