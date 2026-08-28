#   Author:             Tom Goncin & Sandra Lisset Cruz Rivera
#   File Description:   Use to aid the Python files within /Screen to run the UI with C++ code.
#   Date:               August 27th, 2026


import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

from gui_helpers import create_label, create_entry_focus, create_combobox_focus, create_checkbox_focus, on_select_show, toggle_controls, hide_controls, size, refresh_all_widgets
from gui_helpers import create_label_entry_pair

from Definitions.enums import eLaminationType, eMaterial, eLeads, eBars, eCoating, eTypeConductor, eLogical, eDesignSpec
import subprocess

from Serialize import serialize_device_to_xml, deserialize_device_from_xml
from Definitions.functions import address, ADDRESSZ

import sys
import os

from Screen.form_parameterOptimizer import open_parameters_form
from Resources.resources import resource_path
from PIL import Image, ImageTk

_design_widgets = {}
entries_header = []

def update_reports_button_stateDes(btn_save, device):
    try:
        # Check your actual C++ field objects (e.g., deviceFields or coreFields)
        # Adjust 'bSendOptimize' to match the exact field name in your C++ binding
        fields_obj = getattr(device, 'deviceFields', getattr(device, 'coreFields', None))
        
        enabled = False
        if fields_obj and hasattr(fields_obj, "bSendOptimize"):
            enabled = fields_obj.bSendOptimize.get()

        if enabled:
            btn_save.config(state="normal", bg="#00DE99") 
        else:
            btn_save.config(state="disabled", bg="#DDDDDD") 

    except Exception as e:
        print("update_reports_button_state error:", e)

def open_design_form(root,frame_design_information = None,device = None,rebuild_optimizer_menu=None):
    iIndex = 0
    if frame_design_information is None:
        frame_design_information = tk.Toplevel(root)
        frame_design_information ("")
        frame_design_information ("1200x10")

    #Load logo image
    logo_frame = tk.Frame(frame_design_information)
    logo_frame.grid(row=0, column=0, rowspan=3, sticky="ne", padx=10)

    try:
        img_path = resource_path("Resources/PTI_logo2.gif")
        img = Image.open(img_path)
        img = img.resize((75, 75), Image.Resampling.LANCZOS)
        logo = ImageTk.PhotoImage(img)

        lbl_logo = tk.Label(logo_frame, image=logo)
        lbl_logo.image = logo
        lbl_logo.pack(side="right", padx=0)

    except Exception as e:
        print("Logo not loaded:", e)

        lbl_logo = tk.Label(logo_frame, text="[Logo]")
        lbl_logo.pack(side="right", padx=0)

    inner_frame = ttk.Frame(frame_design_information)
    inner_frame.grid(row=1, column=1, sticky="ew", padx=10, pady=5)
    inner_frame.grid_columnconfigure(0, weight=0)
    inner_frame.grid_columnconfigure(2, weight=0)
    inner_frame.grid_columnconfigure(4, weight=0)
    inner_frame.grid_columnconfigure(6, weight=1)
 
    # Diccionario para widgets
 
    row = 0
    col = 0

    # SO Number
    label, entry = create_label_entry_pair(inner_frame, "SO/Quotation Number", row, col, var=tk.StringVar())
    _design_widgets['so_number'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "Winding structure", row, col, var=tk.StringVar())
    _design_widgets['array_type'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    #Conductor caculates LV
    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "Nominal kVA", row, col, var=tk.StringVar())
    _design_widgets['Nominal_kVA'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "Phases", row, col, var=tk.StringVar())
    _design_widgets['iNoPhases'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1


    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "Transformer family", row, col, var=tk.StringVar())
    _design_widgets['transformer_family'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "Design Standard", row, col, var=tk.StringVar())
    _design_widgets['sDesignSpec'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "Flux Density (T)", row, col, var=tk.StringVar())
    _design_widgets['dFluxDensityVoltT'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "Cost", row, col, var=tk.StringVar())
    _design_widgets['dCCEval'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "HV", row, col, var=tk.StringVar())
    _design_widgets['sVoltage3HV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "LV", row, col, var=tk.StringVar())
    _design_widgets['sVoltage3LV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1


    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "HV-BIL", row, col, var=tk.StringVar())
    _design_widgets['iBilHV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "LV-BIL", row, col, var=tk.StringVar())
    _design_widgets['iBilLV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "HV Amps", row, col, var=tk.StringVar())
    _design_widgets['sLineAmpHV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "LV Amps", row, col, var=tk.StringVar())
    _design_widgets['sLineAmpLV'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "Taps", row, col, var=tk.StringVar())
    _design_widgets['bHaveTaps'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    label, entry = create_label_entry_pair(inner_frame, "", row, col, var=tk.StringVar())
    _design_widgets['sCapactity'] = entry
    entries_header.append(entry)
    entry.config(width=10)
    row += 1

    row = 0
    col += 2
    label, entry = create_label_entry_pair(inner_frame, "MVA", row, col, var=tk.StringVar())
    _design_widgets['skVA'] = entry
    entries_header.append(entry)
    entry.config(width=10)

    row += 1

    _get_device_values(device)

    def on_button_click():
        # Crear ventana secundaria
        window_parameters = tk.Toplevel()
        window_parameters.title("Model Parameters")
        window_parameters.geometry("400x350+200+100")  # ancho x alto + posición

        # Opcional: hacerla modal (bloquea ventana principal)
        window_parameters.transient(root)   # root = tu ventana principal
        window_parameters.grab_set()

        frame_parameters= ttk.Frame(
            window_parameters,
            borderwidth=1,  
            relief="solid"  #solid, ridge, sunken, raised, groove, flat
        )
        frame_parameters.pack(fill="both", expand=True, padx=10, pady=10)

        #window_parameters.wait_window()

        bSend = open_parameters_form(window_parameters,frame_parameters,device,iIndex)

        window_parameters.destroy()

        if bSend==False:
            return

        monitor_path = r"\\PTISK-fileserver.pti.local\DATA\ENGINEERING\13-Enginerring projects\MERCURY\OPTIMIZATION THREE PHASE\Dont touch\Monitor\monitor_gui.exe"
        #monitor_path = r"C:\Users\scruz\OneDrive - PTI Transformers Inc\Documents\Optimizer\Production\Distribution\exe\monitor_gui.exe"
        #monitor_path = r"Resources/monitor_gui.exe"
        # if getattr(sys, 'frozen', False):
        #     BASE_DIR = os.path.dirname(sys.executable)
        # else:
        #     BASE_DIR = os.path.dirname(os.path.abspath(__file__))

        # monitor_path = os.path.join(BASE_DIR, "monitor_gui.exe")

        sAddress = ADDRESSZ


        #so_number = str(device.fields["so_number"].get())
        so_number = str(device.get_so_number())


        sAddress = os.path.join(sAddress, so_number)
        os.makedirs(sAddress, exist_ok=True)

        file_name = so_number + ".opt"
        file_path = os.path.join(sAddress, file_name)

        device.calculate_device()

        success = serialize_device_to_xml(device, file_path, bOptimizer=True)
        if not success:
            messagebox.showinfo("Error", "Cannot send for optimization. Please check the information.")
            return

        # Ejecutar monitor.exe
        result = subprocess.run(
            [monitor_path, file_path],
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            messagebox.showerror("Error", f"Monitor failed:\n{result.stderr}")
            return

        # --------------------------------------------------
        # PARSEO DE RESULTADOS TOP 3
        # --------------------------------------------------
        root.hg_dict = {}

        lines = result.stdout.splitlines()
        current_key = None

        for line in lines:
            line = line.strip()

            # Detecta ranking (#1, #2, #3)
            if line.startswith("#"):
                current_key = line
                root.hg_dict[current_key] = {
                    "path": None,
                    "metrics": {
                        "Cost": None,
                        "Warning": None,
                        "Penalty": None,
                        "TOC": None
                    }
                }

            elif current_key and line.startswith("HG"):
                root.hg_dict[current_key]["path"] = line.split(":", 1)[1].strip()

            elif current_key and line.startswith("Cost"):
                root.hg_dict[current_key]["metrics"]["Cost"] = line.split(":", 1)[1].strip()

            elif current_key and line.startswith("Warning"):
                root.hg_dict[current_key]["metrics"]["Warning"] = line.split(":", 1)[1].strip()

            elif current_key and line.startswith("Penalty"):
                root.hg_dict[current_key]["metrics"]["Penalty"] = line.split(":", 1)[1].strip()

            elif current_key and line.startswith("TOC"):
                root.hg_dict[current_key]["metrics"]["TOC"] = line.split(":", 1)[1].strip()

        if not root.hg_dict:
            messagebox.showerror("Error", "No optimized designs were returned.")
            return

        if rebuild_optimizer_menu:
            rebuild_optimizer_menu()

        # --------------------------------------------------
        # VENTANA DE SELECCION
        # --------------------------------------------------
        select_win = tk.Toplevel()
        select_win.title("Select design")

        tk.Label(
            select_win,
            text="Choose which optimized design to load:"
        ).pack(pady=10)

        def load_design(choice):
            entry = root.hg_dict[choice]
            hg_path = entry["path"]

            bComplete = deserialize_device_from_xml(device, hg_path)
            #device.fields["bBeOptimize"].set(True)
            device.deviceFields["bBeOptimize"].set(True)
            device.calculate_device()
            device.calculate_device()
            refresh_all_widgets(device)

            device.calculate_device()
            if bComplete:
                messagebox.showinfo("Success", f"Design loaded successfully ({choice})")
            else:
                messagebox.showerror("Error", f"Could not load design ({choice})")

            select_win.destroy()
            

        # Crear botones dinamicos
        for key, entry in root.hg_dict.items():
            frame = tk.Frame(select_win)
            frame.pack(pady=5, fill="x", padx=10)

            tk.Button(
                frame,
                text=key,
                width=10,
                command=lambda k=key: load_design(k)
            ).pack(side="left")

            m = entry["metrics"]
            metrics_text = (
                f"Cost: {m['Cost']} | "
                f"Warning: {m['Warning']} | "
                f"Penalty: {m['Penalty']} | "
                f"TOC: {m['TOC']}"
            )

            tk.Label(frame, text=metrics_text).pack(side="left", padx=10)

    # Crear el boton
    btn_save = tk.Button(inner_frame, text="Optimize design", command=on_button_click,bg="#00DE99")
    btn_save.grid(row=row, column=col, columnspan=2, pady=10)  # 


    frame_design_information.btn_save = btn_save
    update_reports_button_stateDes(btn_save, device)

def _get_device_values(device):
    """Get current device values and display them in the interface"""
    try:
        hide_controls(True, entries_header)

        if device is not None:
            # 1. CREATE A SAFE UPDATER: This automatically prevents Checkbutton crashes
            def safe_update(widget_key, value):
                if widget_key not in _design_widgets:
                    return
                widget = _design_widgets[widget_key]
                
                # If it's a text entry or dropdown, delete and insert safely
                if isinstance(widget, (tk.Entry, ttk.Entry, ttk.Combobox)):
                    widget.delete(0, tk.END)
                    widget.insert(0, str(value))
                    
                # If it's a Checkbutton, safely set its boolean variable instead
                elif isinstance(widget, (tk.Checkbutton, ttk.Checkbutton)):
                    if hasattr(widget, 'var') and widget.var is not None:
                        # Convert value to boolean (True/False) and set the checkbox
                        widget.var.set(str(value).lower() in ['true', '1', 'yes'])

            # 2. POPULATE THE UI
            safe_update('so_number', device.get_so_number())
            safe_update('array_type', device.get_customer_name()) 
            safe_update('Nominal_kVA', device.get_quantity())
            safe_update('iNoPhases', device.get_no_phases())
            safe_update('transformer_family', device.get_transformer_family())
            
            # ⚠️ COPY-PASTE WARNING ⚠️
            # You copy-pasted get_so_number() for all of the fields below! 
            # I left them as get_so_number() so it compiles, but you need to 
            # change these to their actual C++ getters (e.g. device.get_iBilHV()) 
            safe_update('sDesignSpec', device.get_so_number())
            safe_update('dFluxDensityVoltT', device.get_so_number())
            safe_update('dCCEval', device.get_so_number())
            safe_update('sVoltage3HV', device.get_so_number())
            safe_update('sVoltage3LV', device.get_so_number())
            safe_update('iBilHV', device.get_so_number())
            safe_update('iBilLV', device.get_so_number())
            safe_update('sLineAmpHV', device.get_so_number())
            safe_update('sLineAmpLV', device.get_so_number())
            safe_update('bHaveTaps', device.get_so_number()) 
            safe_update('sCapactity', device.get_so_number())
            safe_update('skVA', device.get_so_number())

        hide_controls(False, entries_header)

    except Exception as e:
        print(f"Error loading device values: {e}")



def update_design_information(app):
    """Update the General tab with current device values"""
    if app.device:
        _get_device_values(app.device)
    