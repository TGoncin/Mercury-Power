import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
from Resources.resources import resource_path

# ------------------------------------------------------------------
# Public entry: show_sap_tables(root, device, mode="popup")
# ------------------------------------------------------------------
def show_sap_tables(root, device, mode="popup"):
    if mode == "popup":
        return _show_popup(root, device)
    else:
        return _show_footer(root, device)  # opcional, por consistencia

# ------------------------------------------------------------------
# Build SAP table and return populate_grid callable
# ------------------------------------------------------------------
def _build_sap_table(parent_container, *sap_data_objs):
    canvas = tk.Canvas(parent_container)
    v_scroll = ttk.Scrollbar(parent_container, orient="vertical", command=canvas.yview)
    h_scroll = ttk.Scrollbar(parent_container, orient="horizontal", command=canvas.xview)
    scroll_frame = ttk.Frame(canvas)

    scroll_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
    canvas.create_window((0, 0), window=scroll_frame, anchor="nw")
    canvas.configure(yscrollcommand=v_scroll.set, xscrollcommand=h_scroll.set)

    canvas.grid(row=0, column=0, sticky="nsew")
    v_scroll.grid(row=0, column=1, sticky="ns")
    h_scroll.grid(row=1, column=0, sticky="ew")
    parent_container.grid_rowconfigure(0, weight=1)
    parent_container.grid_columnconfigure(0, weight=1)

    def _on_mousewheel(event):
        canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")
    canvas.bind_all("<MouseWheel>", _on_mousewheel)

    def populate_grid():
        for w in scroll_frame.winfo_children():
            try:
                w.destroy()
            except Exception:
                pass

        for idx, sap_obj in enumerate(sap_data_objs):
            if not hasattr(sap_obj, "tb_data") or not sap_obj.tb_data:
                continue

            # Nombre de la tabla
            table_name = getattr(sap_obj, "name", f"SAP Table {idx+1}")

            # Headers y datos
            if hasattr(sap_obj, "tb_headers") and sap_obj.tb_headers:
                headers = sap_obj.tb_headers
                data_rows = sap_obj.tb_data  # ¡Usamos TODAS las filas!
            else:
                headers = sap_obj.tb_data[0]  # primera fila como header
                data_rows = sap_obj.tb_data[1:]  # resto como datos

            frame = ttk.LabelFrame(scroll_frame, text=table_name)
            frame.grid(row=idx, column=0, padx=10, pady=10, sticky="nsew")

            create_table(frame, headers, data_rows)

        scroll_frame.grid_rowconfigure(len(sap_data_objs), weight=1)
        scroll_frame.grid_columnconfigure(0, weight=1)

    try:
        populate_grid()
    except Exception:
        pass

    return populate_grid

def sort_treeview_column(tree, col, reverse):
    def _sort_key(value):
        # Normalizar
        if value is None:
            return (1, "")

        value = str(value).strip()

        # Intentar número
        try:
            return (0, float(value))
        except ValueError:
            return (1, value.lower())

    data = [(tree.set(k, col), k) for k in tree.get_children("")]

    data.sort(key=lambda t: _sort_key(t[0]), reverse=reverse)

    # Reordenar filas
    for index, (_, k) in enumerate(data):
        tree.move(k, "", index)

    # Alternar orden
    tree.heading(
        col,
        command=lambda: sort_treeview_column(tree, col, not reverse)
    )

# ---------------------------------------------------------
def create_table(parent, headers, data):
    """
    Crea un Treeview dentro de parent con headers y data
    """
    frame = ttk.Frame(parent)
    frame.grid(row=0, column=0, sticky="nsew")
    parent.grid_rowconfigure(0, weight=1)
    parent.grid_columnconfigure(0, weight=1)

    tree = ttk.Treeview(frame, columns=headers, show="headings")
    tree.grid(row=0, column=0, sticky="nsew")

    vsb = ttk.Scrollbar(frame, orient="vertical", command=tree.yview)
    hsb = ttk.Scrollbar(frame, orient="horizontal", command=tree.xview)
    tree.configure(yscrollcommand=vsb.set, xscrollcommand=hsb.set)

    vsb.grid(row=0, column=1, sticky="ns")
    hsb.grid(row=1, column=0, sticky="ew")
    frame.grid_rowconfigure(0, weight=1)
    frame.grid_columnconfigure(0, weight=1)

    # Headers
    for col in headers:
        tree.heading(
        col,
        text=col,
        command=lambda c=col: sort_treeview_column(tree, c, False)
        )
        tree.column(col, width=max(120, len(col) * 10), anchor=tk.CENTER)  

    # Data
    for row in data:
        tree.insert("", tk.END, values=row)

    return tree

# ------------------------------------------------------------------
# SAFE PLACE / DESTROY
# ------------------------------------------------------------------
def _safe_place(frame, root, height_frac=0.5, min_height=100):
    if not frame:
        return
    try:
        if not frame.winfo_exists():
            return
    except Exception:
        return
    try:
        rw = root.winfo_width()
        rh = root.winfo_height()
        fh = max(int(rh * height_frac), min_height)
        frame.place(x=0, y=int((rh - fh)/2), width=rw, height=fh)
    except Exception:
        pass

def _safe_destroy(widget, attr_container=None, attr_name=None):
    try:
        if widget and getattr(widget, "winfo_exists", None) and widget.winfo_exists():
            widget.destroy()
    except Exception:
        pass
    if attr_container and attr_name:
        try:
            setattr(attr_container, attr_name, None)
        except Exception:
            pass

# ------------------------------------------------------------------
# POPUP MODE
# ------------------------------------------------------------------
def _show_popup(root, device):
    existing_win = getattr(root, "_sap_tables_win", None)
    if existing_win and existing_win.winfo_exists():
        existing_win.lift()
        return

    win = tk.Toplevel(root)
    win.title("SAP Tables")
    win.geometry("1000x600")
    root._sap_tables_win = win

    # Top bar
    top_bar = tk.Frame(win, bg="#e0e0e0", height=25)
    top_bar.grid(row=0, column=0, sticky="ew")

    def _close_popup():
        _safe_destroy(getattr(root, "_sap_tables_win", None), root, "_sap_tables_win")

    btn_close = ttk.Button(top_bar, text="Close", command=_close_popup)
    btn_close.pack(side="right", padx=5)

    content = tk.Frame(win)
    content.grid(row=1, column=0, sticky="nsew")
    win.grid_rowconfigure(1, weight=1)
    win.grid_columnconfigure(0, weight=1)

    win.populate_grid = _build_sap_table(
        content,
        device.SAP_STD_CONDUCTORS,
        device.SAP_STD_INS_CORE_LEADS,
        device.SAP_STD_BUSHINGS,
        device.SAP_STD_SPADES,
        device.SAP_STD_SWITCHES,
        device.SAP_STD_FUSES,
        device.SAP_STD_GAUGES,
        device.ME_TA_SS_LIST,
        device.ME_COMPONENTS,
        device.ME_TA_PP_LIST,
        device.ME_TANK_STEEL_OIL
    )

# ------------------------------------------------------------------
# FOOTER MODE (opcional)
# ------------------------------------------------------------------
def _show_footer(root, device):
    # Implementa si quieres mostrar las tablas en un footer
    pass
