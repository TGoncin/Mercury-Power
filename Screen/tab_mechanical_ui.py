# tab_mechanical_ui.py
from Screen.form_mechanical_norads import open_mechanical_norads_form
from Screen.form_mechanical_rads import open_mechanical_rads_form
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,size,make_scrollable,hide_controls
from Definitions.enums import eLogical
import tkinter as tk
from tkinter import ttk

def build_mechanical_tab(notebook, device):
    # --- Tab : Mechanical---
    tab_mechanical = ttk.Frame(notebook)
    notebook.add(tab_mechanical, text="Mechanical")

    #scrollable = make_scrollable(tab_mechanical)
    size(tab_mechanical)

    # ===== Scroll Container =====
    container = ttk.Frame(tab_mechanical)
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
        canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")
    canvas.bind_all("<MouseWheel>", _on_mousewheel)

    frame_mechanical_norads_design = ttk.Frame(
        scrollable_frame,
        borderwidth=2,  
        relief="solid"  #solid, ridge, sunken, raised, groove, flat
    )

    frame_mechanical_norads_design.pack(
        padx=0,
        pady=0,
        fill="both",
        expand=True
    )

    frame_mechanical_rads_design = ttk.Frame(
        scrollable_frame,  
        relief="solid"  #solid, ridge, sunken, raised, groove, flat
    )

    frame_mechanical_rads_design.pack(
        padx=0,
        pady=0,
        fill="both",
        expand=True
    )

    _last_value = {"v": None} 

    def refresh_mechanical_form(*_): 
        v = device.mechanical.fields["bCalculoWithRad"].get() 
        if v == _last_value["v"]: 
            return 
        _last_value["v"] = v

        # ocultar ambos frames
        frame_mechanical_norads_design.pack_forget()
        frame_mechanical_rads_design.pack_forget()

        # mostrar solo el que corresponda
        if v == eLogical.FALSE: 
            frame_mechanical_norads_design.pack(fill="both", expand=True)
        else: 
            frame_mechanical_rads_design.pack(fill="both", expand=True)

    open_mechanical_norads_form(None, frame_mechanical_norads_design, device, 0)
    open_mechanical_rads_form(None, frame_mechanical_rads_design, device, 0)

    # Luego sincronizar visibilidad
    refresh_mechanical_form()

    device.mechanical.fields["bCalculoWithRad"].bind(
        refresh_mechanical_form
    )

    return tab_mechanical


