# tab_winding_ui.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus, hide_controls,on_select_show,toggle_controls,create_checkbox,size,make_scrollable,create_entry,create_combobox,create_label_entry_pair,create_label_combobox_pair,create_label_checkbox_pair
from Definitions.enums import eTransformerType,eTransformerFamily,eCoolingType,eSingleCoolingType,ePhaseType,eConnectionType,eDesignSpec,eLogical,eTemperatureRise,eArrayType,eChangerCapacity,eChangerCapacity,eBIL,eModelChanger
_general_widgets = {}
entries_header = []

QUANTITY_OPTIONS = ["2","3"]

WINDSTRUCT_TYPES = [
    ("OTHER",-1),
    ("L-H", 0),     
    ("H-L", 1),   
    ("L-H-L", 2),   
    ("L-L-H", 3)
]

WINDSTRUC_DISPLAY = [item[0] for item in WINDSTRUCT_TYPES]
WINDSTRUCT_TO_VALUE = {item[0]: item[1] for item in WINDSTRUCT_TYPES}
WINDSTRUCT_TO_STRING = {item[1]: item[0] for item in WINDSTRUCT_TYPES}

CONN_TYPES = [
    ("DELTA",0),
    ("WYE", 1)
]

CONN_DISPLAY = [item[0] for item in CONN_TYPES]
CONNT_TO_VALUE = {item[0]: item[1] for item in CONN_TYPES}
CONNT_TO_STRING = {item[1]: item[0] for item in CONN_TYPES}

BIL_TYPES = [
    ("30",30),
    ("45", 45),
    ("60", 60),
    ("75", 75),
    ("95", 95),
    ("110", 110),
    ("125", 125),
    ("150", 150),
    ("170", 170),
    ("200", 200),
    ("250", 250)
]

BIL_DISPLAY = [item[0] for item in BIL_TYPES]
BILT_TO_VALUE = {item[0]: item[1] for item in BIL_TYPES}
BILT_TO_STRING = {item[1]: item[0] for item in BIL_TYPES}

CHANGER_TYPES = [
    ("None",-1),
    ("OLTC", 0),
    ("OCTC", 1),
    ("RCBN", 2),
    ("FCBN", 3)
]

CHANGER_DISPLAY = [item[0] for item in CHANGER_TYPES]
CHANGERT_TO_VALUE = {item[0]: item[1] for item in CHANGER_TYPES}
CHANGERT_TO_STRING = {item[1]: item[0] for item in CHANGER_TYPES}

MODEL_TYPES = [
    ("None",-1),
    ("Eaton 100A Crimps", 0),
    ("Orto 0498-9T", 1),
    ("Eaton 300A", 2),
    ("Quality Switch", 3)
]

MODEL_DISPLAY = [item[0] for item in MODEL_TYPES]
MODELT_TO_VALUE = {item[0]: item[1] for item in MODEL_TYPES}
MODELT_TO_STRING = {item[1]: item[0] for item in MODEL_TYPES}

def build_winding_tab(notebook, app):
    # --- Tab 2: Winding ---
    tab_winding = ttk.Frame(notebook)
    notebook.add(tab_winding, text="Winding")

    # Create main frame
    main_frame = ttk.Frame(tab_winding)
    main_frame.pack(fill='both', expand=True, padx=5, pady=5)
    
    # Create left and right frames
    left_frame = ttk.Frame(main_frame, width=400, height=500)
    left_frame.pack(side='left', fill='y', padx=5, pady=5)
    left_frame.pack_propagate(True)

    left_frame.grid_columnconfigure(0, weight=0, minsize=120) 
    left_frame.grid_columnconfigure(1, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(2, weight=1, minsize=120)
    left_frame.grid_columnconfigure(3, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(4, weight=0, minsize=120) 
    left_frame.grid_columnconfigure(5, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(6, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(7, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(8, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(9, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(10, weight=0, minsize=120) 
    left_frame.grid_columnconfigure(11, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(12, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(13, weight=0, minsize=120) 
    left_frame.grid_columnconfigure(14, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(15, weight=1, minsize=120)
    left_frame.grid_columnconfigure(16, weight=0, minsize=120) 
    left_frame.grid_columnconfigure(17, weight=1, minsize=120) 
    left_frame.grid_columnconfigure(18, weight=1, minsize=120) 

    # ============================================
    # LEFT FRAME - Device Information
    # ============================================
    winding_frame = ttk.LabelFrame(left_frame, text="Winding", padding=10, style='Bold.TLabelframe')
    winding_frame.pack(fill='x', padx=5, pady=5)

    row = 0
    col = 0

    label, cmb = create_label_combobox_pair(winding_frame, "Quantity", row, col, QUANTITY_OPTIONS, combo_width=15)
    _general_widgets['number_winding'] = cmb
    entries_header.append(cmb)
    col += 2

    label, cmb = create_label_combobox_pair(winding_frame, "Winding Structure", row, col, WINDSTRUC_DISPLAY, combo_width=15)
    _general_widgets['array_type'] = cmb
    entries_header.append(cmb)
    row += 1

    col = 0
    create_label(winding_frame,"Winding",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Connection",row,col,bold=True)
    col += 1
    create_label(winding_frame,"BIL",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Voltage(*)",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Rated turns",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Number of Bushings(*)",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Ground Wye",row,col,bold=True)
    col += 1
    create_label(winding_frame,"Half turns",row,col,bold=True)
    row += 1

    col = 0
    label, cmb = create_label_combobox_pair(winding_frame, "HV", row, col, CONN_DISPLAY, combo_width=15)
    _general_widgets['type_connectionHV'] = cmb
    col += 2

    cmb = create_combobox(winding_frame, row, col, BIL_DISPLAY, width=15)
    _general_widgets['iBILHV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dVoltageHV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dNomTurnsUIHV'] = entry
    entries_header.append(entry)
    col+=1

    entry = create_entry(winding_frame,row,col,width=20,var=tk.StringVar())
    _general_widgets['iNumBushingsHV'] = entry
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['ground_YHV'] = chk
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHalfTurnsHV'] = chk
    col+=1
    row += 1

    col = 0
    label, cmb = create_label_combobox_pair(winding_frame, "LV", row, col, CONN_DISPLAY, combo_width=15)
    _general_widgets['type_connectionLV'] = cmb
    col += 2

    cmb = create_combobox(winding_frame, row, col, BIL_DISPLAY, width=15)
    _general_widgets['iBILLV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dVoltageLV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dNomTurnsUILV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=20,var=tk.StringVar())
    _general_widgets['iNumBushingsLV'] = entry
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['ground_YLV'] = chk
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHalfTurnsLV'] = chk
    col+=1
    row += 1

    col = 0
    label, cmb = create_label_combobox_pair(winding_frame, "TV", row, col, CONN_DISPLAY, combo_width=15)
    _general_widgets['type_connectionTV'] = cmb
    col += 2

    cmb = create_combobox(winding_frame, row, col, BIL_DISPLAY, width=15)
    _general_widgets['iBILTV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dVoltageTV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dNomTurnsUITV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=20,var=tk.StringVar())
    _general_widgets['iNumBushingsTV'] = entry
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['ground_YTV'] = chk
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHalfTurnsTV'] = chk
    col+=1
    row += 1

    # lbl_dual_voltage = create_label(tab_winding,iIndexRow4,iColumn,"Dual",bold=True,width=8,padx=0,pady=0)
    # iIndexRow4+=1
    # entry_dSerieVoltage_HV = create_entry_focus(tab_winding,iIndexRow4,iColumn,"","dSerieVoltage",pWindH,width=15,padx=0,pady=0)
    # iIndexRow4+=1
    # entry_dSerieVoltage_LV = create_entry_focus(tab_winding,iIndexRow4,iColumn,"","dSerieVoltage",pWindL,width=15,padx=0,pady=0)
    # iIndexRow4+=1
    # entry_dSerieVoltage_TV = None
    # if device.have_tertiary == eLogical.TRUE:
    #     entry_dSerieVoltage_TV = create_entry_focus(tab_winding,iIndexRow4,iColumn,"","dSerieVoltage",pWindT,width=15,padx=0,pady=0)
    #     iIndexRow4+=1

    # if device.dual_voltage == eLogical.TRUE:
    #     iColumn+=1
    col = 0
    create_label(winding_frame,"",row,col,width=8)
    row+=1

    create_label(winding_frame,"Winding",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"Taps",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"%per up",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"%per down",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"Number of steps",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"Capacity type",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"Tap changer",row,col,bold=True)
    col += 1 
    create_label(winding_frame,"Instructions",row,col,bold=True)
    col += 1 

    col = 0
    row+=1

    create_label(winding_frame,"HV",row,col)
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHaveTapsHV'] = chk
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerUpHV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerDownHV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['iNumStepsHV'] = entry
    col+=1

    cmb = create_combobox(winding_frame, row, col, CHANGER_DISPLAY, width=15)
    _general_widgets['tCapacityHV'] = cmb
    col += 1

    cmb = create_combobox(winding_frame, row, col, MODEL_DISPLAY, width=15)
    _general_widgets['tModelChangerHV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['sInstructionsHV'] = entry
    col+=1
    row += 1

    col = 0
    create_label(winding_frame,"LV",row,col)
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHaveTapsLV'] = chk
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerUpLV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerDownLV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['iNumStepsLV'] = entry
    col+=1

    cmb = create_combobox(winding_frame, row, col, CHANGER_DISPLAY, width=15)
    _general_widgets['tCapacityLV'] = cmb
    col += 1

    cmb = create_combobox(winding_frame, row, col, MODEL_DISPLAY, width=15)
    _general_widgets['tModelChangerLV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['sInstructionsLV'] = entry
    col+=1
    row+=1

    col =0
    create_label(winding_frame,"TV",row,col)
    col+=1

    chk = create_checkbox(winding_frame,row,col,"",var=tk.StringVar())
    _general_widgets['bHaveTapsTV'] = chk
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerUpTV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['dPerDownTV'] = entry
    col+=1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['iNumStepsTV'] = entry
    col+=1

    cmb = create_combobox(winding_frame, row, col, CHANGER_DISPLAY, width=15)
    _general_widgets['tCapacityTV'] = cmb
    col += 1

    cmb = create_combobox(winding_frame, row, col, MODEL_DISPLAY, width=15)
    _general_widgets['tModelChangerTV'] = cmb
    col += 1

    entry = create_entry(winding_frame,row,col,width=15,var=tk.StringVar())
    _general_widgets['sInstructionsTV'] = entry
    col+=1


    optional_frame = ttk.LabelFrame(left_frame, text="Optional", padding=10, style='Bold.TLabelframe')
    optional_frame.pack(fill='x', padx=5, pady=5)

    row = 0
    col = 0
    create_label(optional_frame,"Form Data",row,col,bold=True)

    row += 1

    label,entry = create_label_entry_pair(optional_frame,"Form Material",row,col,var=tk.StringVar())
    _general_widgets['sFormMaterial'] = entry
    col += 3

    label,entry = create_label_entry_pair(optional_frame,"Form Nose",row,col,var=tk.StringVar())
    _general_widgets['dFormNose'] = entry
    row+=1

    col = 0
    label,entry = create_label_entry_pair(optional_frame,"Form RB",row,col,var=tk.StringVar())
    _general_widgets['dFormMaterialRB'] = entry
    col += 3

    label,entry = create_label_entry_pair(optional_frame,"Form Side",row,col,var=tk.StringVar())
    _general_widgets['dFormSide'] = entry
    row+=1

    col = 0
    label,entry = create_label_entry_pair(optional_frame,"Form Comments",row,col,var=tk.StringVar())
    _general_widgets['sFormComments'] = entry
    row+=1

    _get_device_values(app)

    return tab_winding

def _get_device_values(app):
    device = app.device
 
    try:
        hide_controls(True, entries_header)

        #if device is not None:
            #_general_widgets['so_number'].delete(0, tk.END)
            #_general_widgets['so_number'].insert(0, device.get_so_number())

        hide_controls(False, entries_header)

    except Exception as e:
        print(f"Error loading device values: {e}")

