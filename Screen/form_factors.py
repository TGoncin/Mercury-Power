# form_factors.py
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,size,make_scrollable
from Definitions.enums import eTransformerType,eTransformerFamily,eCoolingType,ePhaseType,eConnectionType,eDesignSpec,eEfficiencySpec,eLogical,eTemperatureRise,eArrayType
import tkinter as tk
from tkinter import ttk
report_form_factors = None

def open_factors_form(root=None, device=None):
    global report_form_factors

    if report_form_factors is not None and report_form_factors.winfo_exists():
        # Si existe, solo lo ponemos al frente
        report_form_factors.lift()
        return

    # Crear una nueva ventana (form)
    report_form_factors = tk.Toplevel(root)  # <-- asignar a la variable global
    form = report_form_factors
    form.title("Modify Factors")
    form.geometry("1000x300")
    
    form.transient(root)        # Asociada al root
    form.grab_set()             # Bloquea otras ventanas
    form.focus_force()          # La pone al frente
    form.resizable(False, False)  # Opcional: no redimensionable

    # Evitar minimizar
    form.attributes("-toolwindow", True)  # En Windows quita botón minimizar

    # Controlar boton X
    def on_close():
        form.grab_release()
        form.destroy()

    form.protocol("WM_DELETE_WINDOW", on_close)

    # # Etiqueta de título
    # ttk.Label(form, text="Advanced factors", font=("Segoe UI", 12, "bold")).pack(pady=10)

    # Frame para los checkbuttons y botones
    frame = ttk.Frame(form)
    frame.pack(pady=10, padx=10)

    #create_label(scrollable, 0, 0, "Transformer Type")
    iColumn = 0
    iIndex = 0
    create_label(frame,iIndex,iColumn,"Core Related",font_size=12,bold = True,sticky="w",columnspan=2)
    iIndex+=1

    entry_dCoreLossFactor = create_entry_focus(frame,iIndex,iColumn,"Core Loss Factor (%)","dCoreLossFactor",device)
    iIndex+=1

    entry_dFreqFactorHz = create_entry_focus(frame,iIndex,iColumn,"Freq Factor @60 Hz","dFreqFactorHz",device)
    iIndex+=1

    entry_dAmorphousLossVariance = create_entry_focus(frame,iIndex,iColumn,"Amorphous Loss Variance","dAmorphousLossVariance",device)
    iIndex+=1

    entry_dAmAssemblyInducedStress = create_entry_focus(frame,iIndex,iColumn,"Am. Assembly Induced Stress","dAmAssemblyInducedStress",device)#,validate_func=only_numbers)
    iIndex+=1

    entry_dResistance = create_entry_focus(frame,iIndex,iColumn,"Resistance (%)","dResistance",device)
    iIndex+=1

    entry_dLoadLossFactor = create_entry_focus(frame,iIndex,iColumn,"Load Loss Factor (%)","dLoadLossFactor",device)
    iIndex+=1

    entry_dDestructionFactor = create_entry_focus(frame,iIndex,iColumn,"Destruction Factor","dDestructionFactor",device)
    iIndex+=1

    iColumn += 2
    iIndex = 0
    create_label(frame,iIndex,iColumn,"Conductor & Weight",font_size=12,bold = True,sticky="w",columnspan=2)
    iIndex+=1

    entry_dLVCondWghtFactor = create_entry_focus(frame,iIndex,iColumn,"LV Cond Weight Factor (%)","dLVCondWghtFactor",device)
    iIndex+=1

    entry_dHVCondWghtFactor = create_entry_focus(frame,iIndex,iColumn,"HV Cond Weight Factor (%)","dHVCondWghtFactor",device)
    iIndex+=1

    entry_dPaperWghtFactor = create_entry_focus(frame,iIndex,iColumn,"Paper Weight Factor (%)","dPaperWghtFactor",device)
    iIndex+=1

    iColumn += 2
    iIndex = 0
    create_label(frame,iIndex,iColumn,"Build & Geometry",font_size=12,bold = True,sticky="w",columnspan=2)
    iIndex+=1

    entry_dLVBuildFactor = create_entry_focus(frame,iIndex,iColumn,"LV Build Factor (%)","dLVBuildFactor",device)
    iIndex+=1

    entry_dHVBuildFactor = create_entry_focus(frame,iIndex,iColumn,"HV Build Factor (%)","dHVBuildFactor",device)
    iIndex+=1

    entry_dHLBuildFactor = create_entry_focus(frame,iIndex,iColumn,"H-L Build Factor (%)","dHLBuildFactor",device)
    iIndex+=1

    entry_dCoilRB = create_entry_focus(frame,iIndex,iColumn,"Coil RB (%)","dCoilRB",device)
    iIndex+=1

    entry_dInsulationLengthFactor = create_entry_focus(frame,iIndex,iColumn,"Insulation Length Factor (%)","dInsulationLengthFactor",device)
    iIndex+=1

    entry_dNoseBuildFactor = create_entry_focus(frame,iIndex,iColumn,"Nose Build Factor (%)","dNoseBuildFactor",device)
    iIndex+=1

    def on_change_parameter(event=None):
        device.calculate_device()
        device.calculate_device()

    entry_dCoreLossFactor.bind("<FocusOut>", on_change_parameter)
    entry_dFreqFactorHz.bind("<FocusOut>", on_change_parameter)
    entry_dAmorphousLossVariance.bind("<FocusOut>", on_change_parameter)
    entry_dAmAssemblyInducedStress.bind("<FocusOut>", on_change_parameter)
    entry_dResistance.bind("<FocusOut>", on_change_parameter)
    entry_dLoadLossFactor.bind("<FocusOut>", on_change_parameter)
    entry_dLVCondWghtFactor.bind("<FocusOut>", on_change_parameter)
    entry_dHVCondWghtFactor.bind("<FocusOut>", on_change_parameter)
    entry_dPaperWghtFactor.bind("<FocusOut>", on_change_parameter)
    entry_dLVBuildFactor.bind("<FocusOut>", on_change_parameter)
    entry_dHVBuildFactor.bind("<FocusOut>", on_change_parameter)
    entry_dCoilRB.bind("<FocusOut>", on_change_parameter)
    entry_dInsulationLengthFactor.bind("<FocusOut>", on_change_parameter)
    entry_dNoseBuildFactor.bind("<FocusOut>", on_change_parameter)

    root.wait_window(form)