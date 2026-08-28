#form_parameterOptimizer.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size
from Definitions.enums import eLaminationType,eMaterial,eLeads,eBars,eCoating,eTypeConductor,eLogical
import math

def open_parameters_form(root,frame_parameters = None,device = None, iIndex = 0):

    if frame_parameters is None:
        frame_parameters = tk.Toplevel(root)
        frame_parameters.title("Parameters Optimizer")
        frame_parameters.geometry("850x600")

    container = ttk.Frame(frame_parameters)
    container.pack(fill="both", expand=True)

    canvas = tk.Canvas(container)
    canvas.pack(side="left", fill="both", expand=True)

    scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
    scrollbar.pack(side="right", fill="y")

    canvas.configure(yscrollcommand=scrollbar.set)

    # Permite scrolling con rueda del mouse
    def _on_mousewheel(event):
        canvas.yview_scroll(int(-1*(event.delta/120)), "units")

    canvas.bind_all("<MouseWheel>", _on_mousewheel)

    # Frame interno donde vas a poner todos tus labels/entries
    scroll_frame = ttk.Frame(canvas)
    canvas.create_window((0, 0), window=scroll_frame, anchor="nw")

    # Ajustar tamaño del scroll cuando se agregan widgets
    def configure_scroll_region(event):
        canvas.configure(scrollregion=canvas.bbox("all"))

    scroll_frame.bind("<Configure>", configure_scroll_region)


    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()

    #Conductor caculates LV
    iColumn_design = 1
    iIndexCol21 = iIndex+1
    pLCoil = pLVWind.coils[0]
    pHCoil = pHVWind.coils[0]
    #entries_calculate_coil = []

    create_label(scroll_frame, iIndexCol21, iColumn_design,"Define the limits of the parameters used by the optimizer.",columnspan=3)
    iIndexCol21 += 1

    create_label(scroll_frame, iIndexCol21, iColumn_design + 1, "Minimum")

    create_label(scroll_frame, iIndexCol21, iColumn_design + 2, "Maximum")
    iIndexCol21 += 1
    iWidthE = 16

    entry_dImpedanceMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design,"Impedance","dImpedanceMin",device,width=iWidthE,sticky="ew")

    entry_dImpedanceMax = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","dImpedanceMax",device,width=iWidthE,sticky="ew")
    #hide_controls(False,entry_dImpedanceMin)
    iIndexCol21 +=1

    entry_iFluxDensityMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design,"Flux density","iFluxDensityMin",device,width=iWidthE,sticky="ew")

    entry_iFluxDensityMax = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","iFluxDensityMax",device,width=iWidthE,sticky="ew")

    iIndexCol21 +=1

    iNoPhases = device.fields["iNoPhases"].get()
    tLaminationType = device.core.fields["tLaminationType"].get()
    if tLaminationType == eLaminationType.Am or tLaminationType == eLaminationType.AmHB1:
        if iNoPhases==1:
            listSteel = device.core.fields["d_SteelWidthMap"].current["TypeA"]
        else:
            listSteel = device.core.fields["d_SteelWidthMap"].current["TypeA3"]
    else:
        listSteel = device.core.fields["d_SteelWidthMap"].current["TypeB"]

    combo_dSteelWidthMin = create_combobox_focus(scroll_frame,iIndexCol21,iColumn_design,"Steel width",listSteel,"dSteelWidthMin",device,width=iWidthE)
    
    combo_dSteelWidthMax = create_combobox_focus(scroll_frame,iIndexCol21,iColumn_design+2,"",listSteel,"dSteelWidthMax",device,width=iWidthE)
    # entry_iSteelWidthMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design,"Steel Width","iSteelWidthMin",device,width=iWidthE,sticky="ew",bOutput = True)

    # entry_iSteelWidthMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","iSteelWidthMax",device,width=iWidthE,sticky="ew",bOutput = True)

    iIndexCol21 +=1

    list_totalStack = get_list_total_stack(device)
    combo_dTotalStackMin = create_combobox_focus(scroll_frame,iIndexCol21,iColumn_design,"Total Stack",list_totalStack,"dTotalStackMin",device,width=iWidthE)
    
    combo_dTotalStackMax = create_combobox_focus(scroll_frame,iIndexCol21,iColumn_design+2,"",list_totalStack,"dTotalStackMax",device,width=iWidthE)

    iIndexCol21 +=1

    entry_iCurrentDensityLVMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design,"Current Density LV","iCurrentDensityLVMin",device,width=iWidthE,sticky="ew")

    entry_iCurrentDensityLVMax = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","iCurrentDensityLVMax",device,width=iWidthE,sticky="ew")

    iIndexCol21 +=1

    entry_iCurrentDensityHVMin = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design,"Current Density HV","iCurrentDensityHVMin",device,width=iWidthE,sticky="ew")

    entry_iCurrentDensityHVMax = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","iCurrentDensityHVMax",device,width=iWidthE,sticky="ew")

    iIndexCol21 +=1

    create_label(scroll_frame, iIndexCol21, iColumn_design,"Max LV radial No.") 
    entry_RadialLVMax = create_entry_focus(scroll_frame,iIndexCol21,iColumn_design+2,"","iRadialLVMax",device,width=iWidthE,sticky="ew")
    iIndexCol21 +=1

    def on_change_parameter(event=None):
        device.calculate_parametersOptimizer()
        list_totalStack = get_list_total_stack(device)
        combo_dTotalStackMin["values"] = list_totalStack
        combo_dTotalStackMax["values"] = list_totalStack

    # Para entradas
    entry_iFluxDensityMin.bind("<FocusOut>", on_change_parameter)
    entry_iFluxDensityMax.bind("<FocusOut>", on_change_parameter)
    entry_iCurrentDensityLVMin.bind("<FocusOut>", on_change_parameter)
    entry_iCurrentDensityLVMax.bind("<FocusOut>", on_change_parameter)

    # Para combobox
    combo_dSteelWidthMin.bind("<<ComboboxSelected>>", on_change_parameter)
    combo_dSteelWidthMax.bind("<<ComboboxSelected>>", on_change_parameter)
    combo_dTotalStackMin.bind("<<ComboboxSelected>>", on_change_parameter)
    combo_dTotalStackMax.bind("<<ComboboxSelected>>", on_change_parameter)

    result = False

    def on_send():
        nonlocal result
        result = True
        frame_parameters.destroy()
        

    def on_cancel():
        nonlocal result
        result = False
        frame_parameters.destroy()

    # ===============================
    # BUTTONS
    # ===============================
    button_frame = ttk.Frame(scroll_frame)
    button_frame.grid(row=iIndexCol21, column=1, columnspan=3, pady=20)

    btn_send = ttk.Button(button_frame, text="Send", command=on_send)
    btn_send.pack(side="left", padx=15)

    btn_cancel = ttk.Button(button_frame, text="Cancel", command=on_cancel)
    btn_cancel.pack(side="left", padx=15)

    # Si es ventana independiente → hacerla modal
    if True:#isinstance(frame_parameters, tk.Toplevel):
        #frame_parameters.transient(root)
        #frame_parameters.grab_set()
        #frame_parameters.protocol("WM_DELETE_WINDOW", on_cancel)
        root.wait_window(frame_parameters)

    return result

def get_list_total_stack(device):
    tLaminationTypeE = device.core.fields["tLaminationType"].get()
    bAmorphous = True if tLaminationTypeE == eLaminationType.Am or tLaminationTypeE == eLaminationType.AmHB1 else False
    dLimMax = 2.7 if bAmorphous else 2.3#1.8
    dLimMin = 1.25#1.3
    dSteelWidthMin = device.fields["dSteelWidthMin"].get()
    dSteelWidthMax = device.fields["dSteelWidthMax"].get()

    iLowLim = min(math.floor(dSteelWidthMin/dLimMax),math.floor(dSteelWidthMax/dLimMax))
    iHighLim = max(math.ceil(dSteelWidthMin/dLimMin),math.ceil(dSteelWidthMax/dLimMin))
    if iLowLim<1:
        iLowLim = 1

    if iHighLim>15 or iHighLim==0:
        iHighLim=15
    list_total = []
    for i in range(iLowLim,iHighLim+1):
        if (i>=dSteelWidthMin/dLimMax and i<=dSteelWidthMin/dLimMin) or (i>=dSteelWidthMax/dLimMax and i<=dSteelWidthMax/dLimMin):
            list_total.append(i)
        if ((i+1/8)>=dSteelWidthMin/dLimMax and (i+1/8)<=dSteelWidthMin/dLimMin) or ((i+1/8)>=dSteelWidthMax/dLimMax and (i+1/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 1/8)
        if ((i+2/8)>=dSteelWidthMin/dLimMax and (i+2/8)<=dSteelWidthMin/dLimMin) or ((i+2/8)>=dSteelWidthMax/dLimMax and (i+2/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 2/8)
        if ((i+3/8)>=dSteelWidthMin/dLimMax and (i+3/8)<=dSteelWidthMin/dLimMin) or ((i+3/8)>=dSteelWidthMax/dLimMax and (i+3/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 3/8)
        if ((i+4/8)>=dSteelWidthMin/dLimMax and (i+4/8)<=dSteelWidthMin/dLimMin) or ((i+4/8)>=dSteelWidthMax/dLimMax and (i+4/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 4/8)
        if ((i+5/8)>=dSteelWidthMin/dLimMax and (i+5/8)<=dSteelWidthMin/dLimMin) or ((i+5/8)>=dSteelWidthMax/dLimMax and (i+5/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 5/8)
        if ((i+6/8)>=dSteelWidthMin/dLimMax and (i+6/8)<=dSteelWidthMin/dLimMin) or ((i+6/8)>=dSteelWidthMax/dLimMax and (i+6/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 6/8)
        if ((i+7/8)>=dSteelWidthMin/dLimMax and (i+7/8)<=dSteelWidthMin/dLimMin) or ((i+7/8)>=dSteelWidthMax/dLimMax and (i+7/8)<=dSteelWidthMax/dLimMin):
            list_total.append(i + 7/8)

    list_total.sort()
    device.fields["dTotalStackMin"].assign_default(list_total[0])
    device.fields["dTotalStackMax"].assign_default(list_total[-1])
    return list_total
