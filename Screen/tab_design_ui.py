# tab_design_ui.py
from gui_helpers import create_label, create_entry_focus, create_combobox_focus, create_checkbox_focus, on_select_show, toggle_controls, hide_controls, size,get_enum_safe
from Definitions.enums import eLaminationType, eTransformerFamily,eMaterial, eLeads, eBars, eCoating, eTypeConductor, eLogical,eBIL,eWidthDucts
from Screen.form_coils import open_coils_form
import tkinter as tk
from tkinter import ttk
import sys

def build_design_tab(notebook, device):
    # --- Tab 4: Design---
    tab_design = ttk.Frame(notebook)
    notebook.add(tab_design, text="Coils Design")

    size(tab_design)

    # ===== Scroll Container =====
    container = ttk.Frame(tab_design)
    container.pack(fill="both", expand=True)

    canvas = tk.Canvas(container)
    scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=scrollbar.set)

    scrollbar.pack(side="right", fill="y")
    canvas.pack(side="left", fill="both", expand=True)

    # Frame dentro del Canvas donde irán tus controles
    scrollable_frame = ttk.Frame(canvas)
    scroll_window = canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")

    # Ajuste automático del scroll al tamaño del contenido
    def on_frame_configure(event):
        canvas.configure(scrollregion=canvas.bbox("all"))
    scrollable_frame.bind("<Configure>", on_frame_configure)

    # Ajustar ancho del frame al canvas
    def on_canvas_configure(event):
        canvas.itemconfig(scroll_window, width=event.width)
    canvas.bind("<Configure>", on_canvas_configure)

    # Permitir scroll con la rueda del mouse
    def _on_mousewheel(event):
        if sys.platform.startswith("win"):
            canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")
        elif sys.platform == "darwin":
            canvas.yview_scroll(int(-1 * event.delta), "units")
        else:  # Linux
            if event.num == 4:
                canvas.yview_scroll(-1, "units")
            elif event.num == 5:
                canvas.yview_scroll(1, "units")

    def _bind_to_mousewheel(event):
        if sys.platform.startswith("linux"):
            canvas.bind_all("<Button-4>", _on_mousewheel)
            canvas.bind_all("<Button-5>", _on_mousewheel)
        else:
            canvas.bind_all("<MouseWheel>", _on_mousewheel)

    def _unbind_from_mousewheel(event):
        if sys.platform.startswith("linux"):
            canvas.unbind_all("<Button-4>")
            canvas.unbind_all("<Button-5>")
        else:
            canvas.unbind_all("<MouseWheel>")

    canvas.bind("<Enter>", _bind_to_mousewheel)
    canvas.bind("<Leave>", _unbind_from_mousewheel)

    # A partir de aquí va **todo tu contenido igualito**
    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()
    
    # iIndex+=1

    iIndexType = 1
    iIndex = iIndexType
    #Conductor information LV
    iWidthE = 30
    frame_coils = ttk.Frame(scrollable_frame)
    frame_coils.pack(fill="x", padx=10, pady=5)
    iColumn=0
    iIndexCol2 = iIndex
    pCoilLV = pLVWind.coils[0]

    create_label(frame_coils,iIndexCol2,iColumn,"Winding",font_size=12,bold = True)
    iIndexCol2+=1

    create_label(frame_coils,iIndexCol2+1,iColumn+1,"Low Voltage",bold = True,width=15)
    iIndexCol2+=1

    create_label(frame_coils,iIndexCol2,iColumn,"",bold=True)
    iIndexCol2+=1

    controls_LV = []

    combo_bConstantStayback_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Force constant Stayback",list(eLogical),"bConstantStayback",pCoilLV,width=iWidthE,sticky="nsew")
    controls_LV.append(combo_bConstantStayback_LV)
    iIndexCol2 +=1

    combo_iLayers_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"No. Layers",["1","2"],"iLayers",pCoilLV,width=iWidthE,sticky="nsew")
    iIndexCol2 +=1

    combo_iSectionsLeg_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Sections/Leg",["1","2"],"iSectionsLeg",pCoilLV,width=iWidthE,sticky="nsew")
    iIndexCol2 +=1
    controls_LV.append(combo_iSectionsLeg_LV)

    create_label(frame_coils,iIndexCol2,iColumn,"Conductor",bold=True)
    iIndexCol2+=1

    nocon_listLV = get_listnoconductor(pCoilLV.conductor[0])

    combo_iNoConductor_LV = create_combobox_focus(frame_coils,iIndexCol2+2,iColumn,"No Conductor",nocon_listLV,"iNoConductor",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")
    controls_list_conductorLV = [combo_iNoConductor_LV]

    mat_types = [x for x in list(eMaterial) if x!= eMaterial.MAT_NONE]
    combo_iMatAluminum_LV = create_combobox_focus(frame_coils,iIndexCol2+1,iColumn,"Material (*)",mat_types,"tMatAluminum",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")

    cond_types = [x for x in list(eTypeConductor) if x!= eTypeConductor.NONE]
    combo_iType_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Type conductor (*)",cond_types,"iType",pCoilLV.conductor[0],width=iWidthE,sticky="nsew", combos=controls_list_conductorLV,on_select_func=on_type_change)
    iIndexCol2 +=2
    
    #entry_iNoConductor_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"No Conductor","iNoConductor",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol2 +=1

    
    entry_dWidth_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Width","dWidth",pCoilLV.conductor[0],width=iWidthE,sticky="nsew",warning_func = [pCoilLV.conductor[0].conductor_warnings.warningWidthTable, pCoilLV.conductor[0].conductor_warnings.warningWidth])
    iIndexCol2 +=1

    entry_sDesc_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Description","sDescription",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")
    controls_LV.append(entry_sDesc_LV)
    iIndexCol2 +=1

    make_iType_toggle(pCoilLV.conductor[0].fields["iType"], entry_dWidth_LV)

    entry_iNoAxial_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"No Axial strands","iNoAxial",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol2 +=1

    entry_iNoRadial_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"No Radial strands","iNoRadial",pCoilLV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol2 +=1

    entry_dCondThickTurn_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Thick / turn","dCondThickTurn",pCoilLV,width=iWidthE,sticky="nsew",bOutput = True)
    controls_LV.append(entry_dCondThickTurn_LV)
    iIndexCol2 +=1

    entry_dCondWdthTurn_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Width / turn","dCondWdthTurn",pCoilLV,width=iWidthE,sticky="nsew",bOutput = True)
    controls_LV.append(entry_dCondWdthTurn_LV)
    iIndexCol2 +=1

    entry_dAreaCond_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Area","dAreaCond",pCoilLV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    controls_LV.append(entry_dAreaCond_LV)
    iIndexCol2 +=1

    entry_sDensity_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Density","sDensity",pCoilLV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True,warning_func=pCoilLV.conductor[0].conductor_warnings.warningCurrentDensityMax)
    controls_LV.append(entry_sDensity_LV)
    iIndexCol2 +=1

    # entry_dDensityONAF_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Density ONAF","dDensityONAF",pCoilLV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    # controls_LV.append(entry_dDensityONAF_LV)
    # iIndexCol2 +=1

    entry_iNoDuctInner_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"No of Ducts inner","iNoDuctInner",pLVWind,width=iWidthE,sticky="nsew",warning_func=pLVWind.winding_warnings.warningDuctsInner)
    iIndexCol2 +=1

    entry_iNoDuctOuter_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"No of Ducts outer","iNoDuctOuter",pLVWind,width=iWidthE,sticky="nsew",warning_func=pLVWind.winding_warnings.warningDuctsOuter)
    iIndexCol2 +=1

    # entry_dDuctSize_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Duct size","dDuctSize",pLVWind,width=iWidthE,sticky="nsew")
    # iIndexCol2 +=1

    combo_dDuctSize_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Duct size",list(eWidthDucts),"tDuctSize",pLVWind,width=iWidthE)
    iIndexCol2 +=1

    eCoating_options = list(eCoating)#[x for x in eCoating if x != eCoating.NONE]
    combo_tCoating_LV = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Coating for rectangular",eCoating_options,"tCoating",pLVWind,width=iWidthE)
    iIndexCol2 +=1


    create_label(frame_coils,iIndexCol2,iColumn,"Lead",bold=True)
    iIndexCol2+=1

    entry_iLeadBuild_LV = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Lead build","dLeadBuild",pLVWind,width=iWidthE,sticky="nsew")
    iIndexCol2 +=1

    entry_iQuantityLeads = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Quantity","iQuantityLeads",pLVWind,width=10,sticky="ew")
    iIndexCol2 +=1

    entry_dThicknessLeads = create_entry_focus(frame_coils,iIndexCol2+2,iColumn,"Thickness","dThicknessLeads",pLVWind,width=10,sticky="nsew",bOutput=True)

    entry_dWidthLeads = create_entry_focus(frame_coils,iIndexCol2+3,iColumn,"Width","dWidthLeads",pLVWind,width=10,sticky="nsew",bOutput=True)

    entry_sDensityLeads = create_entry_focus(frame_coils,iIndexCol2+4,iColumn,"Density ONAN","sLeadsDensity",pLVWind,width=10,sticky="nsew",bOutput=True)

    entry_sDensityLeadsONAF = create_entry_focus(frame_coils,iIndexCol2+5,iColumn,"Density ONAF","sLeadsDensityONAF",pLVWind,width=10,sticky="nsew",bOutput=True)

    controls_Leads = [entry_dThicknessLeads,entry_dWidthLeads,entry_sDensityLeads,entry_sDensityLeadsONAF]

    leads_list = get_leads_by_material(pLVWind.fields["tMaterialLeads"].get())

    combo_tTypeLeads = create_combobox_focus(frame_coils,iIndexCol2+1,iColumn,"Type",leads_list,"tTypeLeads",pLVWind,width=iWidthE, combos=controls_Leads,on_select_func=on_leads_change)

    controls_list_leads = [combo_tTypeLeads]
    combo_tMaterialLeads = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Material",list(eMaterial),"tMaterialLeads",pLVWind,width=iWidthE, combos=controls_list_leads,on_select_func = on_material_leads_change,bOutput = True)
    iIndexCol2 +=6

    hide_controls(False,combo_tMaterialLeads) 
    
    hide_controls(False,controls_Leads) 


    create_label(frame_coils,iIndexCol2,iColumn,"Buswork",bold = True)
    iIndexCol2+=1

    entry_iQuantityBusworks = create_entry_focus(frame_coils,iIndexCol2,iColumn,"Quantity","iQuantityBuswork",device,width=iWidthE,sticky="ew")
    iIndexCol2 +=1

    entry_dThicknessBusworks = create_entry_focus(frame_coils,iIndexCol2+2,iColumn,"Thickness","dThicknessBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_dWidthBusworks = create_entry_focus(frame_coils,iIndexCol2+3,iColumn,"Width","dWidthBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_sDensityBuswork = create_entry_focus(frame_coils,iIndexCol2+4,iColumn,"Density ONAN","sDensityBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_sDensityBusworkOnaf = create_entry_focus(frame_coils,iIndexCol2+5,iColumn,"Density ONAF","sDensityBusworkOnaf",device,width=iWidthE,sticky="nsew",bOutput=True)

    controls_Busworks = [entry_dThicknessBusworks,entry_dWidthBusworks,entry_sDensityBuswork,entry_sDensityBusworkOnaf]

    buswork_list = get_buswork_by_material(device.fields["tMaterialBuswork"].get(),device)

    combo_tTypeBusworks = create_combobox_focus(frame_coils,iIndexCol2+1,iColumn,"Type",buswork_list,"tTypeBuswork",device,width=iWidthE, combos=controls_Busworks,on_select_func=on_bars_change)

    controls_list_Busworks = [combo_tTypeBusworks]

    combo_tMaterialBusworks = create_combobox_focus(frame_coils,iIndexCol2,iColumn,"Material",list(eMaterial),"tMaterialBuswork",device,width=iWidthE, combos=controls_list_Busworks,on_select_func=on_material_bars_change,bOutput = True)
    hide_controls(False,combo_tMaterialBusworks)
    iIndexCol2 +=6
    
    hide_controls(False,controls_Busworks) 

    hide_controls(False,controls_LV)

     #LV Data Option
    iIndexType2 = iIndexCol2
    #iColumn +=2
    create_label(frame_coils,iIndexType2,iColumn,"Optional Data",bold = True)
    iIndexType2+=1

    entry_sDuctPlacement = create_entry_focus(frame_coils,iIndexType2,iColumn,"Duct placement","sDuctPlacement",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dStayback = create_entry_focus(frame_coils,iIndexType2,iColumn,"Stayback","dOptStayback",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dLayerInsulation = create_entry_focus(frame_coils,iIndexType2,iColumn,"Layer insulation","dLayerInsulation",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dEndInsulation = create_entry_focus(frame_coils,iIndexType2,iColumn,"End insulation","dEndInsulation",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dLeadPads = create_entry_focus(frame_coils,iIndexType2,iColumn,"No Lead Pads","iLeadPads",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dNoseAdder = create_entry_focus(frame_coils,iIndexType2,iColumn,"Nose Adder","dNoseAdder",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dCoilLineLeads = create_entry_focus(frame_coils,iIndexType2,iColumn,"Coil & Line Leads","dCoilLineLeads",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dLeadPadWitdh = create_entry_focus(frame_coils,iIndexType2,iColumn,"Lead Pad With","dLeadPadWitdh",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    bil_options = list(eBIL)
    combo_iBilUnder = create_combobox_focus(frame_coils,iIndexType2,iColumn,"BIL Under",bil_options,"iBILUnder",pLVWind,width=iWidthE)
    iIndexType2+=1

    entry_sHLUnder = create_entry_focus(frame_coils,iIndexType2,iColumn,"HL under","sHLUnder",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dHLUnderRB = create_entry_focus(frame_coils,iIndexType2,iColumn,"HL under (RB)","dHLUnderRB",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_sHLOver = create_entry_focus(frame_coils,iIndexType2,iColumn,"HL over","sHLOver",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dHLOverRB = create_entry_focus(frame_coils,iIndexType2,iColumn,"HL over (RB)","dHLOverRB",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    entry_dBuildFactor = create_entry_focus(frame_coils,iIndexType2,iColumn,"Build factor","dBuildFactor",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1

    combo_bEndfiller = create_combobox_focus(frame_coils,iIndexType2,iColumn,"Endfiller",list(eLogical),"bEndFiller",pLVWind,width=iWidthE)
    iIndexType2+=1

    entry_sPHTapLeads = create_entry_focus(frame_coils,iIndexType2,iColumn,"Ph./Tap Leads","sPHTapLeads",pLVWind,width = iWidthE,sticky="nsew")
    iIndexType2+=1


    control_LV = [entry_dEndInsulation,entry_dLeadPadWitdh,entry_dCoilLineLeads,entry_dLeadPads,
                  combo_iBilUnder,entry_sHLUnder,entry_dHLUnderRB,entry_sHLOver,
                  entry_dHLOverRB,entry_sPHTapLeads]

    hide_controls(False,control_LV)

    #Conductor information HV
    iColumn+=2
    iIndexCol3 = iIndex+1
    pCoilHV = pHVWind.coils[0]
    controls_HV = []

    create_label(frame_coils,iIndexCol3+1,iColumn,"High Voltage",bold = True)
    iIndexCol3+=1

    create_label(frame_coils,iIndexCol3,iColumn-1,"",bold=True)
    iIndexCol3+=1

    combo_bConstantStayback_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",list(eLogical),"bConstantStayback",pCoilHV,width=iWidthE,sticky="nsew")
    iIndexCol3 +=1

    combo_iLayers_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",[1,2],"iLayers",pCoilHV,width=iWidthE,sticky="nsew")
    controls_HV.append(combo_iLayers_HV)
    iIndexCol3 +=1

    combo_iSectionsLeg_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",[1,2],"iSectionsLeg",pCoilHV,width=iWidthE,sticky="nsew")
    iIndexCol3 +=1
    controls_HV.append(combo_iSectionsLeg_HV)

    create_label(frame_coils,iIndexCol3,iColumn-1,"",bold=True)
    iIndexCol3+=1

    

    nocon_listHV = get_listnoconductor(pCoilHV.conductor[0])
    combo_iNoConductor_HV = create_combobox_focus(frame_coils,iIndexCol3+2,iColumn,"",nocon_listHV,"iNoConductor",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    controls_list_conductorHV = [combo_iNoConductor_HV]

    combo_iMatAluminum_HV = create_combobox_focus(frame_coils,iIndexCol3+1,iColumn,"",mat_types,"tMatAluminum",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    #hide_controls(False,combo_iMatAluminum_HV)
    combo_iType_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",cond_types,"iType",pCoilHV.conductor[0],width=22,sticky="nsew", combos=controls_list_conductorHV,on_select_func=on_type_change)
    iIndexCol3 +=2

    iIndexCol3 +=1

    entry_dWidth_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dWidth",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol3 +=1

    entry_sDesc_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","sDescription",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    controls_HV.append(entry_sDesc_HV)
    iIndexCol3 +=1

    make_iType_toggle(pCoilHV.conductor[0].fields["iType"], entry_dWidth_HV)

    entry_iNoAxial_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iNoAxial",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol3 +=1

    entry_iNoRadial_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iNoRadial",pCoilHV.conductor[0],width=iWidthE,sticky="nsew")
    iIndexCol3 +=1

    entry_dCondThickTurn_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dCondThickTurn",pCoilHV,width=iWidthE,sticky="nsew",bOutput = True)
    controls_HV.append(entry_dCondThickTurn_HV)
    iIndexCol3 +=1

    entry_dCondWdthTurn_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dCondWdthTurn",pCoilHV,width=iWidthE,sticky="nsew",bOutput = True)
    controls_HV.append(entry_dCondWdthTurn_HV)
    iIndexCol3 +=1

    entry_dAreaCond_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dAreaCond",pCoilHV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    controls_HV.append(entry_dAreaCond_HV)
    iIndexCol3 +=1

    entry_sDensity_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","sDensity",pCoilHV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True,warning_func=pCoilHV.conductor[0].conductor_warnings.warningCurrentDensityMax)
    controls_HV.append(entry_sDensity_HV)
    iIndexCol3 +=1

    # entry_dDensityONAF_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dDensityONAF",pCoilHV.conductor[0],width=iWidthE,sticky="nsew",bOutput = True)
    # controls_HV.append(entry_dDensityONAF_HV)
    # iIndexCol3 +=1

    entry_iNoDuctInner_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iNoDuctInner",pHVWind,width=iWidthE,sticky="nsew",warning_func=pHVWind.winding_warnings.warningDuctsInner)
    iIndexCol3 +=1

    entry_iNoDuctOuter_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iNoDuctOuter",pHVWind,width=iWidthE,sticky="nsew")
    controls_HV.append(entry_iNoDuctOuter_HV)
    iIndexCol3 +=1

    # entry_dDuctSize_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dDuctSize",pHVWind,width=iWidthE,sticky="nsew")
    # controls_HV.append(entry_dDuctSize_HV)
    # iIndexCol3 +=1

    combo_dDuctSize_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",list(eWidthDucts),"tDuctSize",pHVWind,width=iWidthE)
    controls_HV.append(combo_dDuctSize_HV)
    iIndexCol3 +=1

    combo_tCoating_HV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",eCoating_options,"tCoating",pHVWind,width=22,sticky="nsew")
    #controls_HV.append(combo_tCoating_HV)
    iIndexCol3 +=1

    create_label(frame_coils,iIndexCol3,iColumn,"",bold = True)
    iIndexCol3+=1

    entry_iLeadBuild_HV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","dLeadBuild",pHVWind,width=iWidthE,sticky="nsew")
    iIndexCol3 +=1

    entry_iQuantityLeadsHV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iQuantityLeads",pHVWind,width=10,sticky="ew")
    iIndexCol3 +=1

    entry_dThicknessLeadsHV = create_entry_focus(frame_coils,iIndexCol3+2,iColumn,"","dThicknessLeads",pHVWind,width=10,sticky="nsew",bOutput=True)

    entry_dWidthLeadsHV = create_entry_focus(frame_coils,iIndexCol3+3,iColumn,"","dWidthLeads",pHVWind,width=10,sticky="nsew",bOutput=True)

    entry_sDensityLeadsHV = create_entry_focus(frame_coils,iIndexCol3+4,iColumn,"","sLeadsDensity",pHVWind,width=10,sticky="nsew",bOutput=True)

    entry_sDensityLeadsONAFHV = create_entry_focus(frame_coils,iIndexCol3+5,iColumn,"","sLeadsDensityONAF",pHVWind,width=10,sticky="nsew",bOutput=True)

    controls_LeadsHV = [entry_dThicknessLeadsHV,entry_dWidthLeadsHV,entry_sDensityLeadsHV,entry_sDensityLeadsONAFHV]

    leads_listHV = get_leads_by_material(pHVWind.fields["tMaterialLeads"].get())

    combo_tTypeLeadsHV = create_combobox_focus(frame_coils,iIndexCol3+1,iColumn,"",leads_listHV,"tTypeLeads",pHVWind,width=iWidthE, combos=controls_LeadsHV,on_select_func=on_leads_change)

    controls_list_leadsHV = [combo_tTypeLeadsHV]
    combo_tMaterialLeadsHV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",list(eMaterial),"tMaterialLeads",pHVWind,width=iWidthE, combos=controls_list_leadsHV)
    iIndexCol3 +=6

    hide_controls(False,controls_LeadsHV) 

    controls_LeadsHV1 = [entry_iQuantityLeadsHV,entry_dThicknessLeadsHV,entry_dWidthLeadsHV,entry_sDensityLeadsHV,entry_sDensityLeadsONAFHV,combo_tTypeLeadsHV,combo_tMaterialLeadsHV]
    toggle_controls(False,controls_LeadsHV1)
    hide_controls(False,controls_HV)

    create_label(frame_coils,iIndexCol3,iColumn,"",bold = True)
    iIndexCol3+=1

    entry_iQuantityBusworksHV = create_entry_focus(frame_coils,iIndexCol3,iColumn,"","iQuantityBuswork",device,width=iWidthE,sticky="ew")
    iIndexCol3 +=1

    entry_dThicknessBusworksHV = create_entry_focus(frame_coils,iIndexCol3+2,iColumn,"","dThicknessBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_dWidthBusworksHV = create_entry_focus(frame_coils,iIndexCol3+3,iColumn,"","dWidthBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_sDensityBusworkHV = create_entry_focus(frame_coils,iIndexCol3+4,iColumn,"","sDensityBuswork",device,width=iWidthE,sticky="nsew",bOutput=True)

    entry_sDensityBusworkOnafHV = create_entry_focus(frame_coils,iIndexCol3+5,iColumn,"","sDensityBusworkOnaf",device,width=iWidthE,sticky="nsew",bOutput=True)

    controls_BusworksHV = [entry_dThicknessBusworksHV,entry_dWidthBusworksHV,entry_sDensityBusworkHV,entry_sDensityBusworkOnafHV]

    buswork_list = get_buswork_by_material(device.fields["tMaterialBuswork"].get(),device)
    combo_tTypeBusworksHV = create_combobox_focus(frame_coils,iIndexCol3+1,iColumn,"",buswork_list,"tTypeBuswork",device,width=iWidthE, combos=controls_BusworksHV,on_select_func=on_bars_change)

    controls_list_BusworksHV = [combo_tTypeBusworksHV]

    combo_tMaterialBusworksHV = create_combobox_focus(frame_coils,iIndexCol3,iColumn,"",list(eMaterial),"tMaterialBuswork",device,width=iWidthE, combos=controls_list_BusworksHV)
    iIndexCol3 +=6

    hide_controls(False,controls_BusworksHV) 

    controls_BusworksHV1 = [combo_tMaterialBusworksHV,entry_iQuantityBusworksHV,entry_dThicknessBusworksHV,entry_dWidthBusworksHV,entry_sDensityBusworkHV,entry_sDensityBusworkOnafHV,controls_BusworksHV,combo_tTypeBusworksHV]
    toggle_controls(False,controls_BusworksHV1)

    create_label(frame_coils,iIndexCol3,iColumn-1,"",bold=True)


     #HV Data Optional
    #iColumn +=2
    iIndexType3 = iIndexCol3
    create_label(frame_coils,iIndexType3,iColumn,"",bold = True)
    iIndexType3+=1

    entry_sDuctPlacement_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","sDuctPlacement",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dStayback_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dOptStayback",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dLayerInsulation_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dLayerInsulation",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dEndInsulation_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dEndInsulation",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dLeadPads_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","iLeadPads",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dNoseAdder_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dNoseAdder",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dCoilLineLeads_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dCoilLineLeads",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dLeadPadWitdh_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dLeadPadWitdh",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    combo_iBilUnder_HV = create_combobox_focus(frame_coils,iIndexType3,iColumn,"",bil_options,"iBILUnder",pHVWind,width=22,sticky="nsew",warning_func=pHVWind.winding_warnings.warningBILUnder)
    iIndexType3+=1

    entry_sHLUnder_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","sHLUnder",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dHLUnderRB_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dHLUnderRB",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_sHLOver_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","sHLOver",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dHLOverRB_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dHLOverRB",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    entry_dBuildFactor_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","dBuildFactor",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    combo_bEndfiller_HV = create_combobox_focus(frame_coils,iIndexType3,iColumn,"",list(eLogical),"bEndFiller",pHVWind,width=22,sticky="nsew")
    iIndexType3+=1

    entry_sPHTapLeads_HV = create_entry_focus(frame_coils,iIndexType3,iColumn,"","sPHTapLeads",pHVWind,width = iWidthE,sticky="nsew")
    iIndexType3+=1

    frame_coils_design = ttk.Frame(
        scrollable_frame,
        borderwidth=2,  
        relief="solid"  #solid, ridge, sunken, raised, groove, flat
    )
    h = frame_coils.winfo_height()-50
    frame_coils_design.place(x=600, y=40,width=1150, height=h)

    open_coils_form(None,frame_coils_design,device,iIndex)
    return tab_design

def make_iType_toggle(field_iType, entry_to_toggle, type_to_show=eTypeConductor.STRIP):

    def on_change(*args):
        entry_to_toggle.after_idle(check_type)

    def check_type():
        current_type = field_iType.get()
        value_str = current_type.name if hasattr(current_type, "name") else str(current_type)
        toggle_controls(value_str == type_to_show.name, entry_to_toggle)

    var_iType = field_iType.var
    if isinstance(var_iType, list):
        var_iType = var_iType[0]

    # Quitar trazas previas
    try:
        var_iType.trace_remove("write", field_iType._trace_id)
    except Exception:
        pass

    # Agregar traza nueva
    field_iType._trace_id = var_iType.trace_add("write", on_change)

    # Evaluar estado inicial
    check_type()

def get_leads_by_material(material: str):
    """Devuelve lista de leads filtrados por tipo de material"""
    if material.name == "CU":
        return [e for e in eLeads if (e.name.startswith("CU") or e.value=="None")]
    elif material.name == "AL":
        return [e for e in eLeads if (e.name.startswith("AL") or e.value=="None")]
    else:
        return eLeads.LEADS_NONE

def on_material_leads_change(event, selected, device, controls):
    combo_type = controls[0]

    new_material = device.fields["tMaterialLeads"].get()
    tTypeLeads = device.fields["tTypeLeads"].get()

    # Get available lead types (as enum list)
    new_values = get_leads_by_material(new_material)

    # Update combo values with enum string values
    if isinstance(new_values, list):
        str_values = [v.value for v in new_values]
    else:
        str_values = new_values.value
    combo_type["values"] = str_values

    # Automatically select the first option
    #combo_type.current(0)  

    # Keep enum mapping for later retrieval
    if isinstance(new_values, list):
        combo_type.enum_map = {v.value: v for v in new_values}
        tValue = new_values[1]
    else:
        combo_type.enum_map = new_values.value
        tValue = new_values

    current_value = tTypeLeads.value if hasattr(tTypeLeads, 'value') else str(tTypeLeads)
    current_in_values = current_value in str_values
    
    if current_in_values:
        # Si está, seleccionarlo
        combo_type.set(current_value)
    else:
        # Si no está, tomar el primer valor del combo
        if str_values:
            combo_type.set(tValue)
            device.fields["tTypeLeads"].set(tValue)



def on_leads_change(event, selected, device, controls):
    # Convert selected string to enum (Material)
    selected_enum = get_enum_safe(eLeads, selected)

    if selected_enum == eLeads.CU011:
        device.fields["dThicknessLeads"].set(0.010)
        device.fields["dWidthLeads"].set(1)
        # device.fields["dThicknessLeads"].var.set("0.010")
        # device.fields["dWidthLeads"].var.set("1")
    elif selected_enum == eLeads.CU0931:
        device.fields["dThicknessLeads"].set(0.093)
        device.fields["dWidthLeads"].set(1)
        # device.fields["dThicknessLeads"].var.set("0.093")
        # device.fields["dWidthLeads"].var.set("1")
    elif selected_enum == eLeads.CU052:
        device.fields["dThicknessLeads"].set(0.050)
        device.fields["dWidthLeads"].set(2)
    elif selected_enum == eLeads.CU050x4S:
        device.fields["dThicknessLeads"].set(0.050)
        device.fields["dWidthLeads"].set(4)
    elif selected_enum == eLeads.AL047X2:
        device.fields["dThicknessLeads"].set(0.047)
        device.fields["dWidthLeads"].set(2)
    elif selected_enum == eLeads.AL090X2:
        device.fields["dThicknessLeads"].set(0.090)
        device.fields["dWidthLeads"].set(2)
    elif selected_enum == eLeads.AL063X3:
        device.fields["dThicknessLeads"].set(0.063)
        device.fields["dWidthLeads"].set(3)
    elif selected_enum == eLeads.AL079X4:
        device.fields["dThicknessLeads"].set(0.0787)
        device.fields["dWidthLeads"].set(4)
    else:
        device.fields["dThicknessLeads"].set(0.0)
        device.fields["dWidthLeads"].set(0.0)

def get_buswork_by_material(material: str,device=None):
    # if material.name == "CU":
    #     return [e for e in eBars if (e.name.startswith("CU") and not e.name.startswith("CULEADS")) or e.value=="None")]
    # elif material.name == "AL":
    #     return [e for e in eBars if ((e.name.startswith("AL") and not e.name.startswith("ALLEADS")) or e.value=="None")]
    # else:
    #     return eBars.BARS_NONE
    if material.name == "CU":
        return [e for e in eBars if (e.name.startswith("CU") or e.value=="None")]
    elif material.name == "AL":
        return [e for e in eBars if (e.name.startswith("AL") or e.value=="None")]
    else:
        return eBars.BARS_NONE

# def on_material_change(event, selected = None, device =None, combo_tTypeBusworks = None):
#     new_material = device.fields["tMaterialBuswork"].get()
#     new_buswork_list = get_buswork_by_material(new_material)

#     str_values = [v.value for v in new_buswork_list]
#     combo_tTypeBusworks["values"] = str_values
#     combo_tTypeBusworks.set(new_buswork_list[0] if new_buswork_list else "")

def on_material_bars_change(event, selected, device, controls):
    combo_type = controls[0]

    # Convert selected string to enum (Material)
    new_material = device.fields["tMaterialBuswork"].get()
    tTypeBuswork = device.fields["tTypeBuswork"].get()
    #if selected_enum != eMaterial.MAT_NONE:
    #    device.windings[1].fields["tMaterialLeads"].set(selected)

    # Get available lead types (as enum list)
    new_values = get_buswork_by_material(new_material,device)

    # Update combo values with enum string values
    if isinstance(new_values, list):
        str_values = [v.value for v in new_values]
    else:
        str_values = new_values.value
        
    combo_type["values"] = str_values

    # Automatically select the first option
    #combo_type.current(0)  

    # Keep enum mapping for later retrieval
    if isinstance(new_values, list):
        combo_type.enum_map = {v.value: v for v in new_values}
        tValue = new_values[1]
    else:
        combo_type.enum_map = new_values.value
        tValue = new_values

    current_value = tTypeBuswork.value if hasattr(tTypeBuswork, 'value') else str(tTypeBuswork)
    current_in_values = current_value in str_values
    
    if current_in_values:
        # Si está, seleccionarlo
        combo_type.set(current_value)
    else:
        # Si no está, tomar el primer valor del combo
        if str_values:
            combo_type.set(tValue)
            device.fields["tTypeBuswork"].set(tValue)

def on_bars_change(event, selected, device, controls):
    # Convert selected string to enum (Material)
    selected_enum = get_enum_safe(eBars, selected)

    if selected_enum == eBars.CU15:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(1.5)
    elif selected_enum == eBars.CU2:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(2)
    elif selected_enum == eBars.CU3:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(3)
    elif selected_enum == eBars.CU4:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(4)
    elif selected_enum == eBars.ALFRE2:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(2)
    elif selected_enum == eBars.ALFRE3:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(3)
    elif selected_enum == eBars.ALFRE4:
        device.fields["dThicknessBuswork"].set(0.25)
        device.fields["dWidthBuswork"].set(4)
    elif selected_enum == eBars.CULEADS011:
        device.fields["dThicknessBuswork"].set(0.010)
        device.fields["dWidthBuswork"].set(1)
    elif selected_enum == eBars.CULEADS0931:
        device.fields["dThicknessBuswork"].set(0.093)
        device.fields["dWidthBuswork"].set(1)
    elif selected_enum == eBars.CULEADS052:
        device.fields["dThicknessBuswork"].set(0.050)
        device.fields["dWidthBuswork"].set(2)
    elif selected_enum == eBars.CULEADS050x4S:
        device.fields["dThicknessBuswork"].set(0.050)
        device.fields["dWidthBuswork"].set(4)
    elif selected_enum == eBars.ALLEADS047X2:
        device.fields["dThicknessBuswork"].set(0.047)
        device.fields["dWidthBuswork"].set(2)
    elif selected_enum == eBars.ALLEADS090X2:
        device.fields["dThicknessBuswork"].set(0.090)
        device.fields["dWidthBuswork"].set(2)
    elif selected_enum == eBars.ALLEADS063X3:
        device.fields["dThicknessBuswork"].set(0.063)
        device.fields["dWidthBuswork"].set(3)
    elif selected_enum == eBars.ALLEADS079X4:
        device.fields["dThicknessBuswork"].set(0.0787)
        device.fields["dWidthBuswork"].set(4)
    else:   
        device.fields["dThicknessBuswork"].set(0.0)
        device.fields["dWidthBuswork"].set(0.0)

def get_listnoconductor(device = None):
    iWindingTypeE = device.parent_winding().getWindingType()  
    return device.getNoConductor(iWindingTypeE)

def on_type_change(event, selected, device, controls):
    combo_type = controls[0]

    new_values = get_listnoconductor(device)

    # Update combo values with enum string values
    if isinstance(new_values, list):
        str_values = [str(v) for v in new_values]  # convertir cada número a string
        combo_type.enum_map = {str(v): v for v in new_values}  # mapeo string -> valor original
    else:
        str_values = [str(new_values)]  # combobox espera lista
        combo_type.enum_map = {str(new_values): new_values}

    # Actualizar valores del combobox
    combo_type["values"] = str_values
