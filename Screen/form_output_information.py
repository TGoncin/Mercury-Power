#form_output_information.py
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
from gui_helpers import create_label, create_entry,create_label_entry_pair, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size,refresh_all_widgets
from Definitions.enums import eLaminationType,eMaterial,eLeads,eBars,eCoating,eTypeConductor,eLogical,eDesignSpec
import subprocess
from Serialize import serialize_device_to_xml,deserialize_device_from_xml
from Definitions.functions import address,ADDRESSZ
import os
from Screen.form_parameterOptimizer import open_parameters_form
_design_widgets = {}
entries_header = []

def open_output_form(root,frame_design_information = None,device = None):
    iIndex = 0
    if frame_design_information is None:
        frame_design_information = tk.Toplevel(root)
        frame_design_information ("")
        frame_design_information ("1200x10")

    inner_frame = ttk.Frame(frame_design_information)
    inner_frame.grid(row=0, column=1, padx=10, pady=0)
    inner_frame.grid_columnconfigure(0, weight=0, minsize=90) 
    inner_frame.grid_columnconfigure(1, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(2, weight=1, minsize=100)
    inner_frame.grid_columnconfigure(3, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(4, weight=0, minsize=90) 
    inner_frame.grid_columnconfigure(5, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(6, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(7, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(8, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(9, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(10, weight=0, minsize=90) 
    inner_frame.grid_columnconfigure(11, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(12, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(13, weight=0, minsize=90) 
    inner_frame.grid_columnconfigure(14, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(15, weight=1, minsize=50) 
    inner_frame.grid_columnconfigure(16, weight=0, minsize=90) 
    inner_frame.grid_columnconfigure(17, weight=1, minsize=100) 
    inner_frame.grid_columnconfigure(18, weight=1, minsize=50) 

    #Conductor caculates LV
    iIndex = 0
    iColumn = 0

    create_label(inner_frame,"Calc",iIndex,iColumn+1,font_size=7)
    create_label(inner_frame,"       Guar",iIndex,iColumn+2,font_size=7)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, "Core Loss", iIndex, iColumn,var=tk.StringVar())
    _design_widgets['dCoreLoss'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dNLGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, "Load Loss", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dTotalLLossStrayLead'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dLLGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, "Total Loss", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dTotalLoss'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dExcGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    iIndex = 0
    iColumn+=2
    create_label(inner_frame,"     ",iIndex,iColumn)
    iColumn+=1
    create_label(inner_frame,"     ",iIndex,iColumn)
    iColumn+=1

    create_label(inner_frame,"Calc",iIndex,iColumn+1,font_size=7)
    create_label(inner_frame,"Guar",iIndex,iColumn+2,font_size=7)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, "%IZ", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dIZWithLeads'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dIZGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, "%Eff", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['sPerEfficiency'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dPerEffGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, "%Exc", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dPerExc'] = entry
    entries_header.append(entry)
    entry.config(width=15)

    entry = create_entry(inner_frame, iIndex,iColumn+2, var=tk.StringVar())
    _design_widgets['dExcGuarantee'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    iIndex += 1

    iIndex = 0
    iColumn+=4
    create_label(inner_frame,"     ",iIndex,iColumn)
    iColumn+=1
    create_label(inner_frame,"     ",iIndex,iColumn)
    iColumn+=1

    label, entry = create_label_entry_pair(inner_frame, "LV Cond", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightLV'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, "HV Cond", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightHV'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, "Core", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightTwoLoops'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, ".0022 TKr", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightKr0022'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex += 1

    iIndex = 0
    iColumn+=3

    label, entry = create_label_entry_pair(inner_frame, ".005 TKr", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightKr005'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, ".010 TKr", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightKr010'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex += 1

    label, entry = create_label_entry_pair(inner_frame, ".015 TKr", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightKr015'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, "Crepe", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightCrepe'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex += 1

    iIndex = 0
    iColumn+=3

    label, entry = create_label_entry_pair(inner_frame, "Pressboard", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dWeightPressBoard'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex +=1

    label, entry = create_label_entry_pair(inner_frame, "Total C & C", iIndex, iColumn, var=tk.StringVar())
    _design_widgets['dTotalCC'] = entry
    entries_header.append(entry)
    entry.config(width=15)
    create_label(inner_frame,"lbs",iIndex,iColumn+2)
    iIndex += 1

    _get_device_values(device)


def _get_device_values(device):
    """Get current device values and display them in the interface"""
 
    try:
        hide_controls(True, entries_header)

        if device is not None:

            _design_widgets['dCoreLoss'].delete(0, tk.END)
            _design_widgets['dCoreLoss'].insert(0, device.get_so_number())

            _design_widgets['dNLGuarantee'].delete(0, tk.END)
            _design_widgets['dNLGuarantee'].insert(0, device.get_so_number())

            _design_widgets['dTotalLLossStrayLead'].delete(0, tk.END)
            _design_widgets['dTotalLLossStrayLead'].insert(0, device.get_so_number())

            _design_widgets['dLLGuarantee'].delete(0, tk.END)
            _design_widgets['dLLGuarantee'].insert(0, device.get_so_number())

            _design_widgets['dTotalLoss'].delete(0, tk.END)
            _design_widgets['dTotalLoss'].insert(0, device.get_so_number())

            _design_widgets['dIZWithLeads'].delete(0, tk.END)
            _design_widgets['dIZWithLeads'].insert(0, device.get_so_number())

            _design_widgets['dIZGuarantee'].delete(0, tk.END)
            _design_widgets['dIZGuarantee'].insert(0, device.get_so_number())

            _design_widgets['sPerEfficiency'].delete(0, tk.END)
            _design_widgets['sPerEfficiency'].insert(0, device.get_so_number())

            _design_widgets['dPerEffGuarantee'].delete(0, tk.END)
            _design_widgets['dPerEffGuarantee'].insert(0, device.get_so_number())

            _design_widgets['dPerExc'].delete(0, tk.END)
            _design_widgets['dPerExc'].insert(0, device.get_so_number())

            _design_widgets['dExcGuarantee'].delete(0, tk.END)
            _design_widgets['dExcGuarantee'].insert(0, device.get_so_number())

            _design_widgets['dWeightLV'].delete(0, tk.END)
            _design_widgets['dWeightLV'].insert(0, device.get_so_number())

            _design_widgets['dWeightHV'].delete(0, tk.END)
            _design_widgets['dWeightHV'].insert(0, device.get_so_number())

            _design_widgets['dWeightTwoLoops'].delete(0, tk.END)
            _design_widgets['dWeightTwoLoops'].insert(0, device.get_so_number())

            _design_widgets['dWeightKr0022'].delete(0, tk.END)
            _design_widgets['dWeightKr0022'].insert(0, device.get_so_number())

            _design_widgets['dWeightKr005'].delete(0, tk.END)
            _design_widgets['dWeightKr005'].insert(0, device.get_so_number())

            _design_widgets['dWeightKr010'].delete(0, tk.END)
            _design_widgets['dWeightKr010'].insert(0, device.get_so_number())

            _design_widgets['dWeightKr015'].delete(0, tk.END)
            _design_widgets['dWeightKr015'].insert(0, device.get_so_number())

            _design_widgets['dWeightCrepe'].delete(0, tk.END)
            _design_widgets['dWeightCrepe'].insert(0, device.get_so_number())

            _design_widgets['dWeightPressBoard'].delete(0, tk.END)
            _design_widgets['dWeightPressBoard'].insert(0, device.get_so_number())

            _design_widgets['dTotalCC'].delete(0, tk.END)
            _design_widgets['dTotalCC'].insert(0, device.get_so_number())

        hide_controls(False, entries_header)

    except Exception as e:
        print(f"Error loading device values: {e}")


def update_output_information(app):
    """Update the General tab with current device values"""
    if app.device:
        _get_device_values(app.device)