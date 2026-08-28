# -*- coding: utf-8 -*-
import tkinter as tk
from tkinter import PhotoImage
from Resources.resources import resource_path
from Structure.device import Device  # import the Device class
from tkinter import ttk, messagebox
from PIL import Image, ImageTk  # Pillow required: py -3.13 -m pip install pillow
from Definitions.enums import eTransformerType, eTransformerFamily, eCoolingType, ePhaseType, eConnectionType, eDesignSpec, eLogical, eTemperatureRise,eMaterial,eWindingDesc2
from gui_helpers import create_label, create_entry_focus, create_combobox_focus, create_checkbox_focus, on_select_show, toggle_controls, make_scrollable, refresh_all_widgets
from enum import Enum
from tab_general_ui import build_general_tab
from tab_winding_ui import build_winding_tab
from tab_core_ui import build_core_tab
from tab_design_ui import build_design_tab
#from tab_optional_ui import build_optional_tab
from tab_tests_ui import build_tests_tab
from tab_mechanical_ui import build_mechanical_tab
from Reports.ReportForm import open_report_form
from OpenXlsx import open_excel
from SaveXlsx import save_excel
from Structure.Serialize import serialize_device_to_xml,deserialize_device_from_xml
from Screen.form_coils import open_coils_form
from Screen.form_design_information import open_design_form,update_reports_button_stateDes
from Screen.form_output_information import open_output_form
from Screen.form_costs import open_costs_form
from Screen.form_factors import open_factors_form
from Screen.form_warnings import show_warnings
from Screen.form_SAPTables import show_sap_tables
from Definitions.functions import address,ADDRESSZ
from tkinter import Tk, filedialog
import os
import sys

# --------------------------
# Main Window
# --------------------------
root = tk.Tk()
root.title("PTI Transformers - Mercury")
root.hg_dict = {} #designs from optimizer

ico_path = os.path.join(os.path.dirname(__file__), "..", "Resources", "Mercury_title_bar.ico")
if os.path.exists(ico_path):
    root.iconbitmap(ico_path)  # ICO only
else:
    print("Mercury.ico not found")

#icon = ImageTk.PhotoImage(file=ico_path)
#root.iconphoto(True, icon)

#root.geometry("1560x720")
#root.geometry("1920x1080")
root.state("zoomed")

root.minsize(1200, 800)
# Usage
device = Device()

# Create the menu bar
menu_bar = tk.Menu(root)

# --------------------------
# FILE menu
# --------------------------
def refresh_callbacks_recursive(obj):
    # Refresca los callbacks de este objeto
    if hasattr(obj, "_refresh_callbacks"):
        for refresh_func in obj._refresh_callbacks:
            refresh_func()
    
    # Busca atributos hijos que puedan tener callbacks
    for attr_name in dir(obj):
        if attr_name.startswith("_"):
            continue  # ignorar atributos privados
        attr = getattr(obj, attr_name)
        
        # Si es lista o dict, recorrer elementos
        if isinstance(attr, list):
            for item in attr:
                refresh_callbacks_recursive(item)
        elif isinstance(attr, dict):
            for item in attr.values():
                refresh_callbacks_recursive(item)
        else:
            # Si es un objeto con _refresh_callbacks, refrescar recursivamente
            if hasattr(attr, "_refresh_callbacks"):
                refresh_callbacks_recursive(attr)

def new_quotation(parent,notebook = None):
    device.reset_fields()

    refresh_all_widgets(device)
    refresh_callbacks_recursive(device)


def open_excel_script(parent,notebook = None):
    device.reset_fields()
    bSave = open_excel(device)

    if bSave:
        messagebox.showinfo(
            title="Excel Opened",
            message="The Excel file was opened successfully."
        )
        refresh_all_widgets(device)
        refresh_callbacks_recursive(device)
    else:
        messagebox.showerror(
            title="Open Error",
            message="The Excel file could not be opened."
        )


def save_excel_script(parent,notebook = None):
    bSave = save_excel(device)
    if bSave:
        messagebox.showinfo(
            title="Excel Saved",
            message="The Excel file was saved successfully."
        )
    else:
        messagebox.showerror(
            title="Save Error",
            message="The Excel file could not be saved."
        )

def serialize(parent):
    #sAddress = address(device.fields["so_number"].get())
    sAddress = ADDRESSZ
    so_number = str(device.fields["so_number"].get())  # ensure it's a string
    sAddress = os.path.join(sAddress, so_number)
    os.makedirs(sAddress, exist_ok=True)

    file_name = so_number + ".xml"
    file_path = os.path.join(sAddress, file_name)

    if os.path.exists(file_path):
        overwrite = messagebox.askyesno(
            "Confirm",
            f"The file already exists.\nDo you want to replace it?"
        )
        if not overwrite:
            messagebox.showinfo("Cancelled", "File not saved.")
            return  

    success = serialize_device_to_xml(device,file_path)
    if success:
        messagebox.showinfo("Success", f"Design saved successfully.")
    else:
        messagebox.showinfo("Error", f"Design could not be saved. Please check the information..")


def deserialize(parent):
    root = Tk()
    root.withdraw()
    
    sAddress = ADDRESSZ

    file_path = filedialog.askopenfilename(
        title="Select an file",
        initialdir=sAddress,
        filetypes=[("XML files", "*.xml"),
                   ("MERCURY files", "*.hg")]
    )

    if not file_path:
        return 

    bSuccess = deserialize_device_from_xml(device, file_path)
    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    pCondHV = pWindH.coils[0].conductor[0]
    pCondLV = pWindL.coils[0].conductor[0]
    iNoPhases = device.fields["iNoPhases"].get()

    iNoCondHV = pCondHV.lookConductorDevice(iNoPhases,pWindL,eWindingDesc2.HV.value)
    pCondHV.fields["iNoConductor"].set(iNoCondHV)

    iNoCondLV = pCondLV.lookConductorDevice(iNoPhases,pWindH,eWindingDesc2.LV.value)
    pCondLV.fields["iNoConductor"].set(iNoCondLV)

    device.calculate_device()
    device.calculate_device()

    refresh_all_widgets(device)
    refresh_callbacks_recursive(device)

    if bSuccess:
        messagebox.showinfo("Success", f"Design loaded successfully.")
    else:
        messagebox.showinfo("Error", f"Could not load the design. Please check the file or try again.")

def open_pdf():
    ruta_pdf =  r"\\PTISK-fileserver.pti.local\DATA\ENGINEERING\13-Enginerring projects\MERCURY\OPTIMIZATION THREE PHASE\Dont touch\MERCURY SYSTEM USER MANUAL.pdf"  
    if os.path.exists(ruta_pdf):
        os.startfile(ruta_pdf)  
    else:
        print("Archivo no encontrado:", ruta_pdf)


def iterate_fieldvalues(obj, visited=None):
    """Iterate over all FieldValues of an object and its dependencies, avoiding cycles."""
    if visited is None:
        visited = set()

    # Avoid processing the same object more than once
    if id(obj) in visited:
        return
    visited.add(id(obj))

    # First, the FieldValues of the current object
    if hasattr(obj, "fields"):
        for field in obj.fields.values():
            yield field

    # Traverse all attributes
    for attr_name in dir(obj):
        if attr_name.startswith("_"):
            continue
        attr = getattr(obj, attr_name)
        if attr is None:
            continue
        if isinstance(attr, list):
            for item in attr:
                yield from iterate_fieldvalues(item, visited)
        elif hasattr(attr, "fields"):
            yield from iterate_fieldvalues(attr, visited)

#excel = make_quote_page_excel(device)

def find_menu_index(menu, text):
    """Busca un item por nombre y lo deshabilita (funciona siempre)."""
    count = menu.index("end")
    for i in range(count + 1):
        try:
            d = menu.entryconfigure(i)
            if "label" in d and d["label"][-1] == text:
                return i
        except:
            pass
    return None

def update_reports_button_state():
    try:
        enabled = device.fields["bMinInformationSaveReports"].get()

        idx = find_menu_index(menu_bar, "Reports")
        if idx is not None:
            state = "normal" if enabled else "disabled"
            menu_bar.entryconfig(idx, state=state)

        # Bottom
        btn_reports.config(state="normal" if enabled else "disabled")

    except Exception as e:
        print("update_reports_button_state error:", e)

def update_Optimizer_button_state():
    try:
        enabled = device.fields["bBeOptimize"].get()

        idx = find_menu_index(menu_bar, "Optimizer")
        if idx is not None:
            state = "normal" if enabled else "disabled"
            menu_bar.entryconfig(idx, state=state)

        # Bottom
        #btn_reports.config(state="normal" if enabled else "disabled")

    except Exception as e:
        print("update_optimizer_button_state error:", e)

def rebuild_optimizer_menu():
    reports_menu.delete(0, tk.END)  # limpia todo el menu

    if not root.hg_dict:
        reports_menu.add_command(
            label="No optimizer results yet",
            state="disabled"
        )
        return

    for key, entry in root.hg_dict.items():
        m = entry["metrics"]

        label = (
            f"{key}  "
            f"Cost:{m.get('Cost')}  "
            f"Warning:{m.get('Warning')}  "
            f"Penalty:{m.get('Penalty')}  "
            f"TOC:{m.get('TOC')}"
        )

        reports_menu.add_command(
            label=label,
            command=lambda k=key: load_design_optimizer(k)
        )

def load_design_optimizer(choice):
    entry = root.hg_dict[choice]
    hg_path = entry["path"]
    bComplete = deserialize_device_from_xml(device, hg_path)
    device.fields["bBeOptimize"].set(True)
    device.calculate_device()
    device.calculate_device()

    refresh_all_widgets(device)
    refresh_callbacks_recursive(device)

    if bComplete:
         messagebox.showinfo("Success", f"Design loaded successfully ({choice})")
    else:
        messagebox.showerror("Error", f"Could not load design ({choice})")

def attach_device_callback(widget):

    def refresh_widget(widget):

        device.calculate_device()
        refresh_all_widgets(device)

        update_reports_button_state()
        update_reports_button_stateDes(frame_design_information.btn_save,device)
        update_Optimizer_button_state()

        if isinstance(widget, ttk.Combobox) and hasattr(widget, "var"):
            for callback in getattr(widget, "_refresh_callbacks", []):
                callback()
        elif isinstance(widget, tk.Entry) and hasattr(widget, "var"):
            for callback in getattr(widget, "_refresh_callbacks", []):
                callback()
        elif isinstance(widget, tk.Checkbutton) and hasattr(widget, "var"):
            for callback in getattr(widget, "_refresh_callbacks", []):
                callback()

        device.calculate_device()
        device.calculate_device()
        for tab in [tab_general, tab_winding,  tab_core, tab_design,  tab_tests, tab_mechanical]:
            if hasattr(tab, "update_controls_visibility"):
                try:
                    tab.update_controls_visibility()
                except Exception as e:
                    print("Error updating tab visibility:", e)

        if hasattr(root, "_warnings_frame") and root._warnings_frame is not None and root._warnings_frame.winfo_exists():
            root._warnings_frame.populate_grid(None)

        if hasattr(root, "_warnings_win") and root._warnings_win is not None and root._warnings_win.winfo_exists():
            root._warnings_win.populate_grid(None)

    if isinstance(widget, ttk.Combobox):
        widget.bind("<<ComboboxSelected>>", lambda e: (refresh_widget(widget)))
    elif isinstance(widget, tk.Entry) and hasattr(widget, "var"):
        widget.var.trace_add("write", lambda *args: (refresh_widget(widget)))
    elif isinstance(widget, tk.Entry):
        widget.bind("<KeyRelease>", lambda e: (refresh_widget(widget)))
    elif isinstance(widget, tk.Checkbutton) and hasattr(widget, "var"):
        widget.var.trace_add("write", lambda *args: (refresh_widget(widget)))
    elif isinstance(widget, tk.Checkbutton):
        widget.bind("<ButtonRelease-1>", lambda e: (refresh_widget(widget)))
    elif isinstance(widget, tk.Scale):
        old_cmd = widget.cget("command")
        device.calculate_device()
        device.calculate_device()
        def new_cmd(val):
            if old_cmd:
                old_cmd(val)
        widget.config(command=new_cmd)

    for child in widget.winfo_children():
        attach_device_callback(child)

toolbar = tk.Frame(root, bd=1, relief="raised")
toolbar.pack(side="top", fill="x")

def load_icon(path, size=(24, 24)):
    try:
        img = Image.open(resource_path(path))
        img = img.resize(size, Image.Resampling.LANCZOS)
        return ImageTk.PhotoImage(img)
    except Exception as e:
        print(f"Error loading icon {path}: {e}")
        return None

icon_new = load_icon("Resources\\new_icon.png")
icon_open = load_icon("Resources\\open_file_icon.png")
icon_save = load_icon("Resources\\save_icon.png")
icon_exit = load_icon("Resources\\exit_icon.png")
icon_excel_open = load_icon("Resources\\import_excel_icon.png")
icon_excel_save = load_icon("Resources\\export_excel_icon.png")
icon_reports = load_icon("Resources\\reports_icon.png")

if icon_new:
    btn_new = ttk.Button(toolbar, image=icon_new, command=lambda: new_quotation(device, notebook))
    btn_new.pack(side="left", padx=2, pady=2)

if icon_open:
    btn_open = ttk.Button(toolbar, image=icon_open, command=lambda: deserialize(device))
    btn_open.pack(side="left", padx=2, pady=2)

if icon_save:
    btn_save = ttk.Button(toolbar, image=icon_save, command=lambda: serialize(device))
    btn_save.pack(side="left", padx=2, pady=2)

if icon_excel_open:
    btn_excel_open = ttk.Button(toolbar, image=icon_excel_open, command=lambda: open_excel_script(device, notebook))
    btn_excel_open.pack(side="left", padx=2, pady=2)

if icon_excel_save:
    btn_excel_save = ttk.Button(toolbar, image=icon_excel_save, command=lambda: save_excel_script(device, notebook))
    btn_excel_save.pack(side="left", padx=2, pady=2)

if icon_reports:
    btn_reports = ttk.Button(toolbar, image=icon_reports, command=lambda: open_report_form(root, device))
    btn_reports.pack(side="left", padx=2, pady=2)

if icon_exit:
    btn_exit = ttk.Button(toolbar, image=icon_exit, command=root.quit)
    btn_exit.pack(side="left", padx=2, pady=2)
# --------------------------
# Top frame: Logo + Title
# --------------------------
#frame_top = tk.Frame(root, width=1400, height=60)
#frame_top.pack(side="top", fill="x", pady=(5, 0))  # No expand
#frame_top.pack_propagate(False)  

# Load logo image
# try:
#     img_path = resource_path("Resources/PTI_logo.jpg")
#     img = Image.open(img_path)
#     img = img.resize((50, 50), Image.Resampling.LANCZOS)
#     logo = ImageTk.PhotoImage(img)
#     lbl_logo = tk.Label(frame_top, image=logo)
#     lbl_logo.pack(side="left", padx=10)
# except Exception as e:
#     print("Logo not loaded:", e)
#     lbl_logo = tk.Label(frame_top, text="[Logo]")
#     lbl_logo.pack(side="left", padx=10)

# Title next to logo
# lbl_title = tk.Label(frame_top, text="Single Phase", font=("Arial", 16))
# lbl_title.pack(side="left", padx=10)

#Design information
frame_design_information = tk.Frame(root,width=1400, height=100)#,borderwidth=1,  relief="solid")
frame_design_information.pack(side="top", fill="x", pady=(0, 0))

open_design_form(root,frame_design_information,device,rebuild_optimizer_menu)

frame_output_information = tk.Frame(root,width=1400, height=100)#,borderwidth=1,  relief="solid")

open_output_form(root,frame_output_information,device)

# --------------------------
# Notebook (Tabs)
# --------------------------
notebook = ttk.Notebook(root)
notebook.pack(expand=True, fill="both", padx=10, pady=(0, 10))

device.calculate_device()
device.calculate_device()
refresh_all_widgets(device)

tab_general = build_general_tab(notebook, device)

tab_winding = build_winding_tab(notebook, device)
#tab_winding_scroll = make_scrollable(tab_winding)

tab_tests = build_tests_tab(notebook, device)

tab_core = build_core_tab(notebook, device)

tab_design = build_design_tab(notebook, device)
#tab_design_scroll = make_scrollable(tab_design)

tab_mechanical = build_mechanical_tab(notebook, device)
#tab_optional = build_optional_tab(notebook, device)
#tab_optional_scroll = make_scrollable(tab_optional)


file_menu = tk.Menu(menu_bar, tearoff=0)
file_menu.add_command(label="New", command=lambda: new_quotation(device,notebook))
file_menu.add_command(label="Open", command=lambda: deserialize(device))
file_menu.add_command(label="Save", command=lambda: serialize(device))
file_menu.add_separator()
file_menu.add_command(label="Exit", command=root.quit)
menu_bar.add_cascade(label="File", menu=file_menu)

excel_menu = tk.Menu(menu_bar, tearoff=0)
excel_menu.add_command(label="Open file Excel", command=lambda: open_excel_script(device,notebook))
excel_menu.add_command(label="Export to file Excel", command=lambda: save_excel_script(device,notebook))
menu_bar.add_cascade(label="Excel", menu=excel_menu)

reports_menu = tk.Menu(menu_bar, tearoff=0)
reports_menu.add_command(label="Generate Reports", command=lambda: open_report_form(root,device))
menu_bar.add_cascade(label="Reports", menu=reports_menu)
update_reports_button_state()

reports_menu = tk.Menu(menu_bar, tearoff=0)
for key, entry in root.hg_dict.items():
    m = entry["metrics"]

    label = (
        f"{key}  "
        f"Cost:{m.get('Cost')}  "
        f"Warning:{m.get('Warning')}  "
        f"Penalty:{m.get('Penalty')}  "
        f"TOC:{m.get('TOC')}"
    )

    reports_menu.add_command(
        label=label,
        command=lambda k=key: load_design_optimizer(k)
    )
menu_bar.add_cascade(label="Optimizer", menu=reports_menu)
update_Optimizer_button_state()

factors_menu = tk.Menu(menu_bar, tearoff=0)
factors_menu.add_command(label="Modify factors", command=lambda: open_factors_form(root,device))
menu_bar.add_cascade(label="Factors", menu=factors_menu)  

help_menu = tk.Menu(menu_bar, tearoff=0)
help_menu.add_command(label="User Manual", command=open_pdf)
menu_bar.add_cascade(label="Help", menu=help_menu)  

# Footer bar
footer = ttk.Frame(root, height=30)
footer.pack(side="bottom", fill="x")
frame_output_information.pack(side="bottom", fill="x", pady=(0, 0))
footer.configure(relief="raised", padding=5)

#Icon for costs added in footer
# Load an image (PNG or GIF)
img_path_coil = resource_path("Resources\\dollar-icon.png")
img = Image.open(img_path_coil)
img = img.resize((30, 30))  
icon = ImageTk.PhotoImage(img)

# Label with image
lbl_footer = ttk.Label(footer, image=icon, cursor="hand2")
lbl_footer.pack(side="left", padx=5)
lbl_footer.bind("<Button-1>", lambda e: open_costs_form(root,device))

#Icon for warnings added in footer
# Load an image (PNG or GIF)
img_path_warning = resource_path("Resources\\warning-icon.png")
img_warning = Image.open(img_path_warning)
img_warning = img_warning.resize((30, 30))  
icon_warning = ImageTk.PhotoImage(img_warning)

# Label with image
lbl_footer = ttk.Label(footer, image=icon_warning, cursor="hand2")
lbl_footer.pack(side="left", padx=15)
lbl_footer.bind("<Button-1>", lambda e: show_warnings(root,device))

img_path_SAP = resource_path("Resources\\SAP.png")
img_path_SAP = Image.open(img_path_SAP)
img_path_SAP = img_path_SAP.resize((50, 30))  
icon_SAP = ImageTk.PhotoImage(img_path_SAP)

lbl_footer = ttk.Label(footer, image=icon_SAP, cursor="hand2")
lbl_footer.pack(side="left", padx=20)
lbl_footer.bind("<Button-1>", lambda e: show_sap_tables(root,device))

# #Icon for warnings added in footer
# # Load an image (PNG or GIF)
# img_path_coil = resource_path("Resources\\coil.png")
# img_coil = Image.open(img_path_coil)
# img_coil = img_coil.resize((30, 30))  
# icon_coil = ImageTk.PhotoImage(img_coil)

# # Label with image
# lbl_footer = ttk.Label(footer, image=icon_coil, cursor="hand2")
# lbl_footer.pack(side="left", padx=15)
# lbl_footer.bind("<Button-1>", lambda e: open_coils_form(root,None,device,0))
# Attach the menu bar to the main window
root.config(menu=menu_bar)
device.calculate_device()
device.calculate_device()

# <-- This is where the tracking is activated
for tab in [tab_general,  tab_winding,  tab_core, tab_design,  tab_tests, tab_mechanical]:
    attach_device_callback(tab)

#tab_tests_scroll = make_scrollable(tab_tests)

# --------------------------
# Start main loop
# --------------------------
def on_close():
    root.destroy() 
    sys.exit()

root.protocol("WM_DELETE_WINDOW", on_close)

root.mainloop()
