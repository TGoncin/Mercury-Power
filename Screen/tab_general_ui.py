# tab_general_ui.py
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus, hide_controls,on_select_show,toggle_controls,size,create_checkbox,make_scrollable,create_entry,create_label_entry_pair,create_label_combobox_pair,create_label_checkbox_pair
from Definitions.enums import eTransformerType,eTransformerFamily,eCoolingType,ePhaseType,eConnectionType,eDesignSpec,eEfficiencySpec,eLogical,eTemperatureRise,eArrayType
_general_widgets ={}
entries_header = []

FAMILY_TYPES = [
    ("None", -1),      # none
    ("MPM", 0),   # Mini Pad Mount
    ("PM", 1),    # Pole Mounted
    ("PLMT", 2),  # platform Mount
    ("PMT", 3),   # Pad Mounted
    ("DPMT", 4),  # DPM
    ("OWP", 5),   # Outdoor WP
    ("SP", 6),   # Submersible Pump
    ("LPMT", 7),  # Light Pad-Mounted)
    ("SP", 8),   # Smal Power
]

FAMILY_DISPLAY = [item[0] for item in FAMILY_TYPES]
FAMILY_TO_VALUE = {item[0]: item[1] for item in FAMILY_TYPES}
FAMILY_TO_STRING = {item[1]: item[0] for item in FAMILY_TYPES}

PHASE_OPTIONS = ["1", "3"]

FREQUENCY_OPTIONS = ["50", "60"]

COOLING_TYPES = [
    ("None", -1),     
    ("ONAN", 0),   
    ("ONAN/ONAF", 1),    
    ("KNAN", 2),  
    ("KNAN/KNAF", 3),   
]

COOLING_DISPLAY = [item[0] for item in COOLING_TYPES]
COOLING_TO_VALUE = {item[0]: item[1] for item in COOLING_TYPES}
COOLING_TO_STRING = {item[1]: item[0] for item in COOLING_TYPES}

TEMPRISE_TYPES = [
    ("None", -1),     
    ("55", 55),   
    ("55/65", 56),    
    ("65", 65),  
]

TEMPRISE_DISPLAY = [item[0] for item in TEMPRISE_TYPES]
TEMPRISE_TO_VALUE = {item[0]: item[1] for item in TEMPRISE_TYPES}
TEMPRISE_TO_STRING = {item[1]: item[0] for item in TEMPRISE_TYPES}

LOGICAL_TYPES = [   
    ("YES", 1),   
    ("NO", 0) 
]

LOGICAL_DISPLAY = [item[0] for item in LOGICAL_TYPES]
LOGICAL_TO_VALUE = {item[0]: item[1] for item in LOGICAL_TYPES}
LOGICAL_TO_STRING = {item[1]: item[0] for item in LOGICAL_TYPES}

DESIGN_TYPES = [   
    ("None", -1),   
    ("ANSI", 0),
    ("C2.1/06", 1),
    ("C2.1/24", 2),
    ("C2.2/06", 3),
    ("C88-16", 4),
    ("C88.M79", 5),
    ("C88.M90", 6),
    ("C199-08", 7),
    ("C227.3/06", 8),
    ("C227.4/06", 9),
    ("C227.4/21", 10),
    ("C227.5/08", 11),
    ("C301.1/06", 12),
    ("C301.2/06", 13)
]

DESIGN_DISPLAY = [item[0] for item in DESIGN_TYPES]
DESIGN_TO_VALUE = {item[0]: item[1] for item in DESIGN_TYPES}
DESIGN_TO_STRING = {item[1]: item[0] for item in DESIGN_TYPES}

def build_general_tab(notebook, app):
    # --- Tab 1: General ---
    tab_general = ttk.Frame(notebook)
    notebook.add(tab_general, text="General Information")

    # Create main frame
    main_frame = ttk.Frame(tab_general)
    main_frame.pack(fill='both', expand=True, padx=5, pady=5)
    
    # Create left and right frames
    left_frame = ttk.Frame(main_frame, width=400, height=500)
    left_frame.pack(side='left', fill='y', padx=5, pady=5)
    #left_frame.pack_propagate(False)

    center_frame = ttk.Frame(main_frame, width=400, height=500)
    center_frame.pack(side='left', fill='y', padx=5, pady=5)
    #center_frame.pack_propagate(False)

    right_frame = ttk.Frame(main_frame, width=400, height=500)
    right_frame.pack(side='left', fill='y', padx=5, pady=5)
    #right_frame.pack_propagate(False)

    style = ttk.Style()
    style.configure('Bold.TLabelframe.Label', font=('TkDefaultFont', 10, 'bold'))

    # ============================================
    # LEFT FRAME - Device Information
    # ============================================
    device_frame = ttk.LabelFrame(left_frame, text="General", padding=10, style='Bold.TLabelframe')
    device_frame.pack(fill='x', padx=5, pady=5)

    row = 1
    label, entry = create_label_entry_pair(device_frame, "SO/Quotation Number", row, 0, var=tk.StringVar())
    _general_widgets['so_number'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Customer", row, 0, var=tk.StringVar())
    _general_widgets['customer_name'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Customer code", row, 0, var=tk.StringVar())
    _general_widgets['customer_code'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Quantity", row, 0, var=tk.StringVar())
    _general_widgets['quantity'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Issued By", row, 0, var=tk.StringVar())
    _general_widgets['issued_by'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Original Issue Date", row, 0, var=tk.StringVar())
    _general_widgets['original_issue_date'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Sales Person", row, 0, var=tk.StringVar())
    _general_widgets['sSalesPerson'] = entry
    row += 1

    label, entry = create_label_entry_pair(device_frame, "Requested Due Date", row, 0, var=tk.StringVar())
    _general_widgets['requested_due_date'] = entry
    row += 1

    # ============================================
    # LEFT FRAME - Electrical Parameters
    # ============================================
    electrical_frame = ttk.LabelFrame(left_frame, text="General Specs", padding=10, style='Bold.TLabelframe')
    electrical_frame.pack(fill='x', padx=5, pady=5)

    label, cmb = create_label_combobox_pair(electrical_frame, "Transformer family (*)", row, 0, FAMILY_DISPLAY, combo_width=27)
    _general_widgets['transformer_family'] = cmb
    row += 1

    label,combo_iNoPhases = create_label_combobox_pair(electrical_frame, "Phases", row, 0, PHASE_OPTIONS, combo_width=27)
    _general_widgets['Phases_combo'] = cmb
    row+=1

    label,entry = create_label_entry_pair(electrical_frame,"Nominal kVA (*)",row,0,var=tk.StringVar())
    _general_widgets['nominal_kva'] = entry
    row+=1

    label,cmb = create_label_combobox_pair(electrical_frame, "Frequency (Hz)", row, 0, FREQUENCY_OPTIONS, combo_width=27)
    _general_widgets['Frequency_Combo'] = cmb
    row+=1

    label,entry = create_label_entry_pair(electrical_frame,"Winding description",row,0,var=tk.StringVar())
    _general_widgets['winding_description'] = entry
    row+=1

    label, cmb = create_label_combobox_pair(electrical_frame, "Cooling Type (*)", row, 0, COOLING_DISPLAY, combo_width=27)
    _general_widgets['tCoolingTypes'] = cmb
    row += 1

    label,entry = create_label_entry_pair(electrical_frame,"If special forced cool",row,0,var=tk.StringVar())
    _general_widgets['dPerSpcFor'] = entry
    row+=1

    label, cmb = create_label_combobox_pair(electrical_frame, "Temperature Rise (*)", row, 0, TEMPRISE_DISPLAY, combo_width=27)
    _general_widgets['tTemperatureRises'] = cmb
    row += 1

    label, cmb = create_label_combobox_pair(electrical_frame, "K4 Factor", row, 0, LOGICAL_DISPLAY, combo_width=27)
    _general_widgets['tK4Factor'] = cmb
    row += 1

    label, cmb = create_label_combobox_pair(electrical_frame, "Prov for ONAF", row, 0, LOGICAL_DISPLAY, combo_width=27)
    _general_widgets['tProvONAF'] = cmb
    row += 1

    # ============================================
    # CENTER FRAME - Optional Information
    # ============================================
    optional_frame = ttk.LabelFrame(center_frame, text="Optionals & Comments", padding=10, style='Bold.TLabelframe')
    optional_frame.pack(fill='x', padx=5, pady=5)

    label,entry = create_label_entry_pair(optional_frame,"Connection Diagrams Items",row,0,var=tk.StringVar())
    _general_widgets['sConnectionDiagramsItems'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"Overall RB factor",row,0,var=tk.StringVar())
    _general_widgets['dOverallRBFactor'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"Altitude (Std<=1000m)",row,0,var=tk.StringVar())
    _general_widgets['dAltitude'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"Winding Spec Comments",row,0,var=tk.StringVar())
    _general_widgets['sWindingSpecComments'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"Purchasing Comments",row,0,var=tk.StringVar())
    _general_widgets['sPurchasingComments'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"Insulation Comments",row,0,var=tk.StringVar())
    _general_widgets['sInsulationComments'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"ONAN Losses",row,0,var=tk.StringVar())
    _general_widgets['dONANLosses'] = entry
    row+=1

    label,entry = create_label_entry_pair(optional_frame,"ONAN TOR\u00B0C",row,0,var=tk.StringVar())
    _general_widgets['dONANTOR'] = entry
    row+=1

    # ============================================
    # CENTER FRAME - Revision Data
    # ============================================
    revision_frame = ttk.LabelFrame(center_frame, text="Revision Data", padding=10, style='Bold.TLabelframe')
    revision_frame.pack(fill='x', padx=5, pady=5)

    label,entry = create_label_entry_pair(revision_frame,"WS, Eng Data, CS & ID Sheets",row,0,var=tk.StringVar())
    _general_widgets['data_description[0]'] = entry
    row+=1

    label,entry = create_label_entry_pair(revision_frame,"Core Drawing",row,0,var=tk.StringVar())
    _general_widgets['data_description[1]'] = entry
    row+=1

    label,entry = create_label_entry_pair(revision_frame,"Purchasing Sumary",row,0,var=tk.StringVar())
    _general_widgets['data_description[2]'] = entry
    row+=1

    label,entry = create_label_entry_pair(revision_frame,"Test Summary",row,0,var=tk.StringVar())
    _general_widgets['data_description[3]'] = entry
    row+=1

    label,entry = create_label_entry_pair(revision_frame,"Test Reports & Ratio Sheets",row,0,var=tk.StringVar())
    _general_widgets['data_description[4]'] = entry
    row+=1

    # ============================================
    # RIGHT FRAME - Specification
    # ============================================
    spec_frame = ttk.LabelFrame(right_frame, text="Specification", padding=10, style='Bold.TLabelframe')
    spec_frame.pack(fill='x', padx=5, pady=5)

    label,entry = create_label_entry_pair(spec_frame,"Customer Spec",row,1,var=tk.StringVar())
    _general_widgets['sCustomerSpec'] = entry
    row+=1

    label,cmb = create_label_combobox_pair(spec_frame, "Design Standard (*)", row, 1, DESIGN_DISPLAY, combo_width=27)
    _general_widgets['tDesignSpec'] = cmb
    row+=1

    label,cmb = create_label_combobox_pair(spec_frame, "Efficiency Standard", row, 1, DESIGN_DISPLAY, combo_width=27)
    _general_widgets['tEfficiencySpec'] = cmb
    row+=1

    # ============================================
    # RIGHT FRAME - Specification
    # ============================================
    guarantees_frame = ttk.LabelFrame(right_frame, text="Guarantees", padding=10, style='Bold.TLabelframe')
    guarantees_frame.pack(fill='x', padx=5, pady=5)
    guarantees_frame.grid_columnconfigure(0, weight=0, minsize=10)   # columna del checkbox
    guarantees_frame.grid_columnconfigure(1, weight=0, minsize=50)  # columna de la etiqueta "No-Load Losses"
    guarantees_frame.grid_columnconfigure(2, weight=1, minsize=50)  # columna del Entry (se expande)
    guarantees_frame.grid_columnconfigure(3, weight=1, minsize=50)  # columna del Entry (se expande)
    guarantees_frame.grid_columnconfigure(4, weight=1, minsize=50)  # columna del Entry (se expande)

    create_label_checkbox_pair
    #label,chk = create_label_checkbox_pair(guarantees_frame, "No-Load Losses", row, 0,var=tk.StringVar())
    chk = create_checkbox(guarantees_frame, row, 0, "",width=5, var=tk.StringVar())
    create_label(guarantees_frame, "No-Load Losses", row, 1)
    _general_widgets['bNLLimited'] = chk

    entry = create_entry(guarantees_frame, row,2, var=tk.StringVar())
    _general_widgets['dNLGuarantee'] = entry
    create_label(guarantees_frame,"W",row,3)
    row+=1

    create_label_checkbox_pair
    chk = create_checkbox(guarantees_frame, row, 0, "",width=5, var=tk.StringVar())
    create_label(guarantees_frame, "Load Losses", row, 1)
    _general_widgets['bLLLimited'] = chk

    entry = create_entry(guarantees_frame, row,2, var=tk.StringVar())
    _general_widgets['dLLGuarantee'] = entry
    create_label(guarantees_frame,"W",row,3)
    row+=1

    create_label_checkbox_pair
    chk = create_checkbox(guarantees_frame, row, 0, "",width=5, var=tk.StringVar())
    create_label(guarantees_frame, "Excitation Current", row, 1)
    _general_widgets['bExcLimited'] = chk

    entry = create_entry(guarantees_frame, row,2, var=tk.StringVar())
    _general_widgets['dExcGuarantee'] = entry
    create_label(guarantees_frame,"%",row,3)
    row+=1

    label,entry = create_label_entry_pair(guarantees_frame,"Impedance",row,1,var=tk.StringVar())
    _general_widgets['dIZGuarantee'] = entry
    create_label(guarantees_frame,"%",row,3)

    entry = create_entry(guarantees_frame, row, 4, var=tk.StringVar())
    _general_widgets['dIZGuaranteeFactor'] = entry
    entries_header.append(entry)
    row+=1

    label,entry = create_label_entry_pair(guarantees_frame,"Special Requeriments",row,1,var=tk.StringVar())
    _general_widgets['sSpecialRequirements'] = entry
    row+=1


    # ============================================
    # RIGHT FRAME - Evaluation
    # ============================================
    evaluation_frame = ttk.LabelFrame(right_frame, text="Evaluation", padding=10, style='Bold.TLabelframe')
    evaluation_frame.pack(fill='x', padx=5, pady=5)

    label,entry = create_label_entry_pair(evaluation_frame,"No-Load Losses",row,1,var=tk.StringVar())
    _general_widgets['dEvaluation_nl'] = entry
    create_label(evaluation_frame,"$/Watt",row,3)
    row+=1

    label,entry = create_label_entry_pair(evaluation_frame,"Load Losses",row,1,var=tk.StringVar())
    _general_widgets['dEvaluation_ll'] = entry
    create_label(evaluation_frame,"$/Watt",row,3)
    row+=1


    # ============================================
    # RIGHT FRAME - Evaluation
    # ============================================
    NL_frame = ttk.LabelFrame(right_frame, text="No-Load Losses", padding=10, style='Bold.TLabelframe')
    NL_frame.pack(fill='x', padx=5, pady=5)

    label,cmb = create_label_combobox_pair(NL_frame, "NL corrected to 85C", row, 1, LOGICAL_DISPLAY, combo_width=27)
    _general_widgets['bNLCorrected_to_85C'] = cmb
    row+=1

    label,entry = create_label_entry_pair(NL_frame,"NL @ 1.05 or 1.00 pu",row,1,var=tk.StringVar())
    _general_widgets['dNL_pu'] = entry
    create_label(NL_frame,"sec",row,3)
    row+=1

    label,entry = create_label_entry_pair(NL_frame,"Momentary grounding",row,1,var=tk.StringVar())
    _general_widgets['dGroundShortTime'] = entry
    row+=1

    label,entry = create_label_entry_pair(NL_frame,"Virtual %Z, for Amp @ Neutral",row,1,var=tk.StringVar())
    _general_widgets['dVirtualZ'] = entry
    row+=1 

    _get_device_values(app)

    return tab_general

def _get_device_values(app):
    device = app.device
    try:
        hide_controls(True, entries_header)

        if device is not None:
            # 1. CREATE THE SAFE UPDATER
            def safe_update(widget_key, value):
                if widget_key not in _general_widgets:
                    return
                widget = _general_widgets[widget_key]
                
                if isinstance(widget, (tk.Entry, ttk.Entry, ttk.Combobox)):
                    widget.delete(0, tk.END)
                    widget.insert(0, str(value) if value is not None else "")
                elif isinstance(widget, (tk.Checkbutton, ttk.Checkbutton)):
                    if hasattr(widget, 'var') and widget.var is not None:
                        widget.var.set(str(value).lower() in ['true', '1', 'yes'])

            # 2. POPULATE UI SAFELY
            safe_update('so_number', device.get_so_number())
            safe_update('customer_name', device.get_customer_name())
            safe_update('customer_code', device.get_customer_code())
            safe_update('quantity', device.get_quantity()) 
            safe_update('issued_by', device.get_issued_by())
            safe_update('original_issue_date', device.get_original_issue_date())
            safe_update('sSalesPerson', device.get_sales_person())
            safe_update('requested_due_date', device.get_requested_due_date())
            safe_update('transformer_family', device.get_transformer_family())  
            safe_update('Phases_combo', device.get_no_phases()) 
            safe_update('nominal_kva', device.get_nominal_kva()) 
            safe_update('Frequency_Combo', device.get_frequency())  
            safe_update('winding_description', device.get_winding_description())

            # Cooling attributes (Assumes cooling proxy handles getters properly)
            safe_update('tCoolingTypes', device.cooling.get_tCoolingTypes())  
            safe_update('tTemperatureRises', device.cooling.get_tTemperatureRises())  
            safe_update('tK4Factor', device.cooling.get_tK4Factor())  
            safe_update('tProvONAF', device.cooling.get_tProvONAF())  

            # Note: This typo (getDONANTOR) will trigger the safe fallback warning in the terminal
            safe_update('dONANTOR', device.getDONANTOR()) 
            
            safe_update('dPerSpcFor', device.get_dPerSpcFor())
            safe_update('sConnectionDiagramsItems', device.get_connection_diagrams_items())
            safe_update('dOverallRBFactor', device.get_overall_rb_factor())
            safe_update('dAltitude', device.get_altitude())
            safe_update('sWindingSpecComments', device.get_winding_spec_comments())
            safe_update('sPurchasingComments', device.get_purchasing_comments())
            safe_update('sInsulationComments', device.get_insulation_comments())

            # ⚠️ YOU NEED TO UPDATE THESE GETTERS ⚠️
            # They are currently all mapped to getSoNumber()
            safe_update('sCustomerSpec', device.getSoNumber())
            safe_update('tDesignSpec', device.getSoNumber())  
            safe_update('tEfficiencySpec', device.getSoNumber()) 
            safe_update('sSpecialRequirements', device.getSoNumber())
            safe_update('dNL_pu', device.getSoNumber())
            safe_update('dGroundShortTime', device.getSoNumber())
            safe_update('dVirtualZ', device.getSoNumber())
            safe_update('dONANLosses', device.getSoNumber())  
            safe_update('bNLLimited', device.getSoNumber())
            safe_update('dNLGuarantee', device.getSoNumber())
            safe_update('bLLLimited', device.getSoNumber())
            safe_update('dLLGuarantee', device.getSoNumber())
            safe_update('bExcLimited', device.getSoNumber())
            safe_update('dExcGuarantee', device.getSoNumber())
            safe_update('dIZGuarantee', device.getSoNumber())
            safe_update('dIZGuaranteeFactor', device.getSoNumber())
            safe_update('dEvaluation_nl', device.getSoNumber())
            safe_update('dEvaluation_ll', device.getSoNumber())
            safe_update('bNLCorrected_to_85C', device.getSoNumber())

        hide_controls(False, entries_header)

    except Exception as e:
        print(f"Error loading device values: {e}")