# tab_optional_ui.py
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size
from Definitions.enums import eLogical,eBIL,eBILUnder

def build_optional_tab(notebook, device):
    import tkinter as tk
    from tkinter import ttk

    # --- Tab 5: Optional---
    tab_optional = ttk.Frame(notebook)
    notebook.add(tab_optional, text="Optional")

    size(tab_optional)

    iIndex = 0
    iColumn = 0
    iIndexType = 0
    iIndexType1 = 0
    iIndexType2 = 0
    iIndexType3 = 0
    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()

    

    
    return tab_optional
