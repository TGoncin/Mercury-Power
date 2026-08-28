# form_warnings.py
import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
from Resources.resources import resource_path
from Definitions.enums import eWarnings

# Optional: Tooltip class (ASCII-safe)
class ToolTip:
    def __init__(self, widget, text):
        self.widget = widget
        self.text = text
        self.tipwindow = None
        self.widget.bind("<Enter>", self.show)
        self.widget.bind("<Leave>", self.hide)

    def show(self, event=None):
        if self.tipwindow or not self.text:
            return
        try:
            x, y, cx, cy = self.widget.bbox("insert") if self.widget.winfo_ismapped() else (0, 0, 0, 0)
        except Exception:
            x, y, cx, cy = (0, 0, 0, 0)
        x += self.widget.winfo_rootx() + 25
        y += self.widget.winfo_rooty() + 20
        self.tipwindow = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(True)
        tw.wm_geometry(f"+{x}+{y}")
        label = tk.Label(
            tw,
            text=self.text,
            justify="left",
            background="#ffffe0",
            relief="solid",
            borderwidth=1,
            font=("tahoma", "8", "normal")
        )
        label.pack(ipadx=1)

    def hide(self, event=None):
        if self.tipwindow:
            try:
                self.tipwindow.destroy()
            except Exception:
                pass
        self.tipwindow = None


# ------------------------------------------------------------------
# Public entry: show_warnings(root, device, mode="footer"|"popup")
# ------------------------------------------------------------------
def show_warnings(root, device, mode="footer"):
    """
    mode = 'footer' or 'popup'
    safe wrapper that ensures exclusivity between footer and popup
    """
    # close popup if requested footer
    if mode == "footer" and getattr(root, "_warnings_win", None):
        try:
            if root._warnings_win.winfo_exists():
                root._warnings_win.destroy()
        except Exception:
            pass
        root._warnings_win = None

    # close footer if requested popup
    if mode == "popup" and getattr(root, "_warnings_frame", None):
        try:
            if root._warnings_frame.winfo_exists():
                root._warnings_frame.destroy()
        except Exception:
            pass
        root._warnings_frame = None

    if mode == "footer":
        return _show_footer(root, device)
    else:
        return _show_popup(root, device)


# ------------------------------------------------------------------
# collect objects
# ------------------------------------------------------------------
def _collect_objects(device):
    objects_with_names = []

    pWindH = device.getHVWind()
    pWindL = device.getLVWind()

    objects_with_names.append((device, "General Information"))
    if hasattr(device, "gradients"):
        objects_with_names.append((device.gradients, "Coils Design"))
    if hasattr(device, "losses"):
        objects_with_names.append((device.losses, "Coils Design"))
    if hasattr(device, "core"):
        objects_with_names.append((device.core, "Core Design"))
    if hasattr(device, "guarantee"):
        objects_with_names.append((device.guarantee, "General Information"))
    if hasattr(device, "mechanical"):
        objects_with_names.append((device.mechanical, "Mechanical"))
    if hasattr(device, "optimizer"):
        objects_with_names.append((device.optimizer, "Optimizer"))
    if hasattr(device, "windings"):
        objects_with_names.append((pWindL, "Winding"))
    if hasattr(device, "windings"):
        objects_with_names.append((pWindH, "Winding"))
    # safe checks for coils lists
    try:
        if hasattr(pWindL, "coils") and pWindL.coils:
            if hasattr(pWindL.coils[0], "conductor"):
                objects_with_names.append((pWindL.coils[0].conductor[0], "Coils Design"))
    except Exception:
        pass
    try:
        if hasattr(pWindH, "coils") and pWindH.coils:
            if hasattr(pWindH.coils[0], "conductor"):
                objects_with_names.append((pWindH.coils[0].conductor[0], "Coils Design"))
    except Exception:
        pass

    return objects_with_names


# ------------------------------------------------------------------
# Build warning table and return populate_grid callable
# ------------------------------------------------------------------
def _build_warning_table(parent_container, objects_with_names):

    icon_size = (16, 16)
    enum_icon_files = {
        "eWARNING": ("warning-icon2.png", "Warning"),
        "eERROR": ("error-icon.png", "Error"),
        "eOPTIMIZER": ("optimizer-icon.png", "Optimizer")
    }

    icon_dict = {}
    for enum_name, (filename, label_text) in enum_icon_files.items():
        try:
            img = Image.open(resource_path(f"Resources/{filename}"))
            img = img.resize(icon_size)
            icon_dict[enum_name] = ImageTk.PhotoImage(img)
        except Exception:
            icon_dict[enum_name] = None

    filter_frame = ttk.Frame(parent_container)
    # use grid to keep similar layout as before
    filter_frame.grid(row=0, column=0, sticky="ew", pady=3, padx=3)

    icon_subframe = ttk.Frame(filter_frame)
    icon_subframe.grid(row=0, column=0, sticky="w")

    canvas = tk.Canvas(parent_container)
    v_scroll = ttk.Scrollbar(parent_container, orient="vertical", command=canvas.yview)
    h_scroll = ttk.Scrollbar(parent_container, orient="horizontal", command=canvas.xview)
    scroll_frame = ttk.Frame(canvas)

    scroll_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
    canvas.create_window((0, 0), window=scroll_frame, anchor="nw")
    canvas.configure(yscrollcommand=v_scroll.set, xscrollcommand=h_scroll.set)

    canvas.grid(row=1, column=0, sticky="nsew")
    v_scroll.grid(row=1, column=1, sticky="ns")
    h_scroll.grid(row=2, column=0, sticky="ew")

    parent_container.grid_rowconfigure(1, weight=1)
    parent_container.grid_columnconfigure(0, weight=1)

    def populate_grid(filter_enum=None):
        # safety: parent_container might have been destroyed
        if not getattr(parent_container, "winfo_exists", None):
            return
        try:
            if not parent_container.winfo_exists():
                return
        except Exception:
            return

        for w in scroll_frame.winfo_children():
            try:
                w.destroy()
            except Exception:
                pass

        headers = ["Severity", "Section", "Message"]
        for col, header in enumerate(headers):
            lbl = ttk.Label(scroll_frame, text=header, font=("Helvetica", 10, "bold"))
            lbl.grid(row=0, column=col, padx=5, pady=5, sticky="nsew")

        row_id = 1
        for obj, parent_name in objects_with_names:
            # safe access to fields dict
            try:
                tErrorEnum = obj.fields.get("tErrorEnum")
                sErrorMessages = obj.fields.get("sErrorMessages")
            except Exception:
                continue

            if not tErrorEnum or not sErrorMessages:
                continue

            # iterate safely by length of messages if available
            try:
                n = len(sErrorMessages.current)
            except Exception:
                # fallback: try to iterate directly
                try:
                    iter_msgs = list(sErrorMessages.current)
                    n = len(iter_msgs)
                except Exception:
                    n = 0

            for i in range(n):
                try:
                    enum_val = tErrorEnum.get(i) if hasattr(tErrorEnum, "get") else tErrorEnum.current[i]
                except Exception:
                    try:
                        enum_val = tErrorEnum.current[i]
                    except Exception:
                        continue
                try:
                    msg_val = sErrorMessages.get(i) if hasattr(sErrorMessages, "get") else sErrorMessages.current[i]
                except Exception:
                    try:
                        msg_val = sErrorMessages.current[i]
                    except Exception:
                        continue

                if msg_val == "" or msg_val is None:
                    continue
                if filter_enum and enum_val != filter_enum:
                    continue

                enum_name = getattr(enum_val, "name", str(enum_val))
                icon = icon_dict.get(enum_name)
                if icon:
                    lbl_enum = ttk.Label(scroll_frame, image=icon)
                    lbl_enum.image = icon
                else:
                    lbl_enum = ttk.Label(scroll_frame, text=str(enum_name))

                lbl_parent = ttk.Label(scroll_frame, text=parent_name)
                lbl_msg = ttk.Label(scroll_frame, text=str(msg_val))

                lbl_enum.grid(row=row_id, column=0, padx=5, pady=2, sticky="nsew")
                lbl_parent.grid(row=row_id, column=1, padx=5, pady=2, sticky="nsew")
                lbl_msg.grid(row=row_id, column=2, padx=5, pady=2, sticky="nsew")

                row_id += 1

        for col in range(3):
            try:
                scroll_frame.grid_columnconfigure(col, weight=1)
            except Exception:
                pass

    # build filter icons and bindings
    for idx, (enum_name, (filename, label_text)) in enumerate(enum_icon_files.items()):
        item = ttk.Frame(icon_subframe)
        item.grid(row=0, column=idx, padx=5)

        icon_img = icon_dict.get(enum_name)
        lbl_icon = ttk.Label(item, image=icon_img, cursor="hand2") if icon_img else ttk.Label(item, text=label_text, cursor="hand2")
        if icon_img:
            lbl_icon.image = icon_img
        lbl_icon.grid(row=0, column=0)

        lbl_text = ttk.Label(item, text=label_text)
        lbl_text.grid(row=0, column=1, padx=(2, 0))

        # safe binding - use lambda that calls populate_grid with eWarnings[en]
        lbl_icon.bind("<Button-1>", lambda e, en=enum_name: populate_grid(filter_enum=eWarnings[en]))
        lbl_text.bind("<Button-1>", lambda e, en=enum_name: populate_grid(filter_enum=eWarnings[en]))

    btn_all = ttk.Button(icon_subframe, text="All", command=lambda: populate_grid(None))
    btn_all.grid(row=0, column=len(enum_icon_files), padx=5)

    # initial populate
    try:
        populate_grid(None)
    except Exception:
        pass

    return populate_grid


# ------------------------------------------------------------------
# Helpers: safe place and safe destroy
# ------------------------------------------------------------------
def _safe_place(frame, root, height_frac=0.30, min_height=60, small_height=28):
    """
    Place frame at bottom of root if frame exists.
    If frame does not exist or was destroyed, do nothing.
    """
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
        frame.place(x=0, y=rh - fh, width=rw, height=fh)
    except Exception:
        # final guard: if place fails, ignore
        pass


def _safe_destroy(widget, attr_container, attr_name):
    """
    Destroy widget if exists and set attribute to None on attr_container
    """
    try:
        if widget and getattr(widget, "winfo_exists", None) and widget.winfo_exists():
            widget.destroy()
    except Exception:
        pass
    try:
        setattr(attr_container, attr_name, None)
    except Exception:
        pass


# ------------------------------------------------------------------
# FOOTER MODE (safe)
# ------------------------------------------------------------------
def _show_footer(root, device):
    """
    Create or restore footer. Safe against double-destroy and invalid paths.
    """

    # if popup exists, destroy it to keep exclusivity
    if getattr(root, "_warnings_win", None):
        _safe_destroy(root._warnings_win, root, "_warnings_win")

    # if footer already there and valid, refresh grid
    existing = getattr(root, "_warnings_frame", None)
    try:
        if existing and existing.winfo_exists():
            # ensure we refresh grid if available
            try:
                if hasattr(existing, "populate_grid"):
                    existing.populate_grid(None)
            except Exception:
                pass
            # ensure place in case it was hidden
            _safe_place(existing, root)
            return
    except Exception:
        pass

    # build new footer
    objects = _collect_objects(device)

    frame = tk.Frame(root, bg="white", bd=1, relief="raised")
    root._warnings_frame = frame

    # initial place
    _safe_place(frame, root)

    # top bar with buttons
    top_bar = tk.Frame(frame, bg="#e0e0e0", height=25)
    top_bar.pack(fill="x")

    # minimize toggles a small bar instead of destroy
    def _destroy_footer():
        _safe_destroy(frame, root, "_warnings_frame")

    # btn_min = ttk.Button(top_bar, text="Minimize", width=10, command=_toggle_minimize)
    # btn_min.pack(side="right", padx=5)

    img_minimize = Image.open(resource_path("Resources/minimize.png"))
    img_minimize = img_minimize.resize((20, 20))  # ajusta tamaño si quieres
    popup_minimize = ImageTk.PhotoImage(img_minimize)

    # crear boton con imagen
    btn_min = ttk.Button(top_bar, image=popup_minimize, width=10, command=_destroy_footer)
    btn_min.image = popup_minimize  # mantener referencia para que no se borre
    btn_min.pack(side="right", padx=5)

    def _footer_to_popup():
        # destroy footer safely and open popup
        _safe_destroy(root._warnings_frame, root, "_warnings_frame")
        _show_popup(root, device)

    img_popup = Image.open(resource_path("Resources/popup.png"))
    img_popup = img_popup.resize((20, 20))  # ajusta tamaño si quieres
    popup_icon = ImageTk.PhotoImage(img_popup)

    # crear boton con imagen
    btn_popup = ttk.Button(top_bar, image=popup_icon, width=30, command=_footer_to_popup)
    btn_popup.image = popup_icon  # mantener referencia para que no se borre
    btn_popup.pack(side="right", padx=5)

    # content area
    content = tk.Frame(frame)
    content.pack(fill="both", expand=True)

    # attach populate function
    try:
        frame.populate_grid = _build_warning_table(content, objects)
    except Exception:
        frame.populate_grid = lambda *a, **k: None

    # bind resize safely: handler checks existence before placing
    def _on_resize(event):
        f = getattr(root, "_warnings_frame", None)
        if not f:
            return
        try:
            if not f.winfo_exists():
                return
        except Exception:
            return
        _safe_place(f, root)

    # remove previous binding if present to avoid duplicates (best effort)
    try:
        root.unbind("<Configure>")
    except Exception:
        # if unbind not desired globally, we still bind but it may duplicate; keep robust handler
        pass
    try:
        root.bind("<Configure>", _on_resize)
    except Exception:
        pass

    # initial populate
    try:
        if hasattr(frame, "populate_grid"):
            frame.populate_grid(None)
    except Exception:
        pass


# ------------------------------------------------------------------
# SWITCH FOOTER -> POPUP (safe)
# ------------------------------------------------------------------
def _switch_footer_to_popup(root, device):
    _safe_destroy(getattr(root, "_warnings_frame", None), root, "_warnings_frame")
    _show_popup(root, device)


# ------------------------------------------------------------------
# POPUP MODE (safe)
# ------------------------------------------------------------------
def _show_popup(root, device):
    """
    Show popup window. If footer exists, hide/destroy it first.
    """

    # if popup already exists, refresh and lift
    existing_win = getattr(root, "_warnings_win", None)
    try:
        if existing_win and existing_win.winfo_exists():
            try:
                if hasattr(existing_win, "populate_grid"):
                    existing_win.populate_grid(None)
            except Exception:
                pass
            try:
                existing_win.lift()
            except Exception:
                pass
            return
    except Exception:
        pass

    # hide/destroy footer if present
    _safe_destroy(getattr(root, "_warnings_frame", None), root, "_warnings_frame")

    objects = _collect_objects(device)

    win = tk.Toplevel(root)
    win.title("Warnings")
    win.geometry("800x400")
    root._warnings_win = win

    top_bar = tk.Frame(win, bg="#e0e0e0", height=30)
    top_bar.grid(row=0, column=0, sticky="ew")

    def _to_footer():
        # close popup and show footer
        _safe_destroy(getattr(root, "_warnings_win", None), root, "_warnings_win")
        _show_footer(root, device)

    # btn_to_footer = ttk.Button(top_bar, text="To Footer", command=_to_footer)
    # btn_to_footer.pack(side="right", padx=5)

    img_collapsed = Image.open(resource_path("Resources/collapsed.png"))
    img_collapsed = img_collapsed.resize((20, 20))  # ajusta tamaño si quieres
    collapsed_icon = ImageTk.PhotoImage(img_collapsed)

    # crear boton con imagen
    btn_to_footer = ttk.Button(top_bar, image=collapsed_icon, width=30, command=_to_footer)
    btn_to_footer.image = collapsed_icon  # mantener referencia para que no se borre
    btn_to_footer.pack(side="right", padx=5)
    
    content = tk.Frame(win)
    content.grid(row=1, column=0, sticky="nsew")

    # attach populate function
    try:
        win.populate_grid = _build_warning_table(content, objects)
    except Exception:
        win.populate_grid = lambda *a, **k: None

    win.grid_rowconfigure(1, weight=1)
    win.grid_columnconfigure(0, weight=1)

    # when user closes popup, ensure references cleared and footer restored
    def _on_close():
        _safe_destroy(getattr(root, "_warnings_win", None), root, "_warnings_win")
        # decide to restore footer automatically to preserve previous behavior
        _show_footer(root, device)

    try:
        win.protocol("WM_DELETE_WINDOW", _on_close)
    except Exception:
        pass


# ------------------------------------------------------------------
# SWITCH POPUP -> FOOTER (safe)
# ------------------------------------------------------------------
def _switch_popup_to_footer(root, device):
    _safe_destroy(getattr(root, "_warnings_win", None), root, "_warnings_win")
    _show_footer(root, device)
