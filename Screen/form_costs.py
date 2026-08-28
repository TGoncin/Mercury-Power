# form_costs.py
import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
from Resources.resources import resource_path
from gui_helpers import create_label, create_entry_focus, hide_controls
from Definitions.enums import eLaminationType, eMaterial, eLeads, eBars, eCoating, eTypeConductor, eLogical, eWindingDesc2
import sys, os

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
def open_costs_form(root, device, mode="footer"):
    """
    mode = 'footer' or 'popup'
    safe wrapper that ensures exclusivity between footer and popup
    """
    # close popup if requested footer
    if mode == "footer" and getattr(root, "_costs_win", None):
        try:
            if root._costs_win.winfo_exists():
                root._costs_win.destroy()
        except Exception:
            pass
        root._costs_win = None

    # close footer if requested popup
    if mode == "popup" and getattr(root, "_costs_frame", None):
        try:
            if root._costs_frame.winfo_exists():
                root._costs_frame.destroy()
        except Exception:
            pass
        root._costs_frame = None

    if mode == "footer":
        return _show_footer(root, device)
    else:
        return _show_popup(root, device)


# ------------------------------------------------------------------
# collect objects
# ------------------------------------------------------------------
def _collect_objects(device,content):
    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()
    pLCoil = pLVWind.coils[0]
    pHCoil = pHVWind.coils[0]

    #sCosts
    iColumn_design = 0
    iIndex = 0

    entries_costs = []

    iWidthE = 15

    create_label(content,iIndex ,iColumn_design,"Costs",bold = True,sticky="nsew",columnspan = 4)
    create_label(content,iIndex ,iColumn_design+5,"Guarantees and Total",bold = True,sticky="nsew",columnspan = 2)
    iIndex +=1

    create_label(content,iIndex ,iColumn_design,"Item")
    create_label(content,iIndex ,iColumn_design+1,"$ Each")
    create_label(content,iIndex ,iColumn_design+2,"Weight (lb)")
    create_label(content,iIndex ,iColumn_design+3,"$ Design")

    create_label(content,iIndex ,iColumn_design+5,"$ Eval")
    create_label(content,iIndex ,iColumn_design+6,"Losses (W)")
    create_label(content,iIndex ,iColumn_design+7,"$ Design")
    iIndex +=1

    create_label(content,iIndex ,iColumn_design,"Core (lb)")
    iColumn_design += 1

    entry_dCore = create_entry_focus(content,iIndex,iColumn_design,"","dCore",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCore)
    iColumn_design += 1

    entry_dWeightTwoLoops = create_entry_focus(content,iIndex,iColumn_design,"","dWeightTwoLoops",device.core,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightTwoLoops)
    iColumn_design += 1

    entry_dCostCore = create_entry_focus(content,iIndex,iColumn_design,"","dCostCore",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostCore)
    iColumn_design += 1

    create_label(content,iIndex ,iColumn_design,"No-Load Losses")
    iColumn_design += 1

    entry_dEvaluation_nl = create_entry_focus(content,iIndex,iColumn_design,"","dEvaluation_nl",device.guarantee,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dEvaluation_nl)
    iColumn_design += 1

    entry_dNLLosses = create_entry_focus(content,iIndex,iColumn_design,"","dNLLosses",device.losses,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dNLLosses)
    iColumn_design += 1

    entry_dEvalNL = create_entry_focus(content,iIndex,iColumn_design,"","dEvalNL",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dEvalNL)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Kraft-.002 (lb)")
    iColumn_design += 1

    entry_dKraft0022 = create_entry_focus(content,iIndex,iColumn_design,"","dKraft0022",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dKraft0022)
    iColumn_design += 1

    entry_dWeightKr0022 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightKr0022",device,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightKr0022)
    iColumn_design += 1

    entry_dCostKraft0022 = create_entry_focus(content,iIndex,iColumn_design,"","dCostKraft0022",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostKraft0022)
    iColumn_design += 1

    create_label(content,iIndex ,iColumn_design,"Load Losses")
    iColumn_design += 1

    entry_dEvaluation_ll = create_entry_focus(content,iIndex,iColumn_design,"","dEvaluation_ll",device.guarantee,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dEvaluation_ll)
    iColumn_design += 1

    entry_dLLosses = create_entry_focus(content,iIndex,iColumn_design,"","dLLosses",device.losses,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dLLosses)
    iColumn_design += 1

    entry_dEvalLL = create_entry_focus(content,iIndex,iColumn_design,"","dEvalLL",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dEvalLL)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Kraft-.005 (lb)")
    iColumn_design += 1

    entry_dKraft005 = create_entry_focus(content,iIndex,iColumn_design,"","dKraft005",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dKraft005)
    iColumn_design += 1

    entry_dWeightKr005 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightKr005",device,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightKr005)
    iColumn_design += 1

    entry_dCostKraft005 = create_entry_focus(content,iIndex,iColumn_design,"","dCostKraft005",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostKraft005)
    iColumn_design += 1

    entry_dCCEval = create_entry_focus(content,iIndex,iColumn_design+2,"C & C + Eval","dCCEval",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCCEval)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Kraft-.010 (lb)")
    iColumn_design += 1

    entry_dKraft010 = create_entry_focus(content,iIndex,iColumn_design,"","dKraft010",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dKraft010)
    iColumn_design += 1

    entry_dWeightKr010 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightKr010",device,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightKr010)
    iColumn_design += 1

    entry_dCostKraft010 = create_entry_focus(content,iIndex,iColumn_design,"","dCostKraft010",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostKraft010)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Crepe (lb)")
    iColumn_design += 1

    entry_dCrepe = create_entry_focus(content,iIndex,iColumn_design,"","dCrepe",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCrepe)
    iColumn_design += 1

    entry_dWeightCrepe = create_entry_focus(content,iIndex,iColumn_design,"","dWeightCrepe",device,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightCrepe)
    iColumn_design += 1

    entry_dCostCrepe = create_entry_focus(content,iIndex,iColumn_design,"","dCostCrepe",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostCrepe)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"PB 1/16 (lb)")
    iColumn_design += 1

    entry_dPB1_16 = create_entry_focus(content,iIndex,iColumn_design,"","dPB1_16",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dPB1_16)
    iColumn_design += 1

    entry_dWeightsPB1_16 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightsPB1_16",device.core,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightsPB1_16)
    iColumn_design += 1

    entry_dCostPB1_16 = create_entry_focus(content,iIndex,iColumn_design,"","dCostPB1_16",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostPB1_16)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"PB 1/8 (lb)")
    iColumn_design += 1

    entry_dPB1_8 = create_entry_focus(content,iIndex,iColumn_design,"","dPB1_8",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dPB1_8)
    iColumn_design += 1

    entry_dWeightsPB1_8 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightsPB1_8",device.core,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightsPB1_8)
    iColumn_design += 1

    entry_dCostPB1_8 = create_entry_focus(content,iIndex,iColumn_design,"","dCostPB1_8",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostPB1_8)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Duct (sq.ft)")
    iColumn_design += 1

    entry_dDucts = create_entry_focus(content,iIndex,iColumn_design,"","dDucts",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dDucts)
    iColumn_design += 1

    entry_d_DuctsDM = create_entry_focus(content,iIndex,iColumn_design,"","d_DuctsDM[0]",device.core,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_d_DuctsDM)
    iColumn_design += 1

    entry_dCostDucts = create_entry_focus(content,iIndex,iColumn_design,"","dCostDucts",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostDucts)

    iIndex +=1
    iColumn_design = 0

    #create_label(content,iIndex ,iColumn_design,field_name="sTypeConductor",parent_obj=pLVWind)
    entry_sTypeConductorLV = create_entry_focus(content,iIndex,iColumn_design,"","sTypeConductor",pLVWind,width=iWidthE,sticky="ew")
    entries_costs.append(entry_sTypeConductorLV)
    iColumn_design += 1

    entry_dLV = create_entry_focus(content,iIndex,iColumn_design,"","dLV",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dLV)
    iColumn_design += 1

    entry_dWeightLV = create_entry_focus(content,iIndex,iColumn_design,"","dWeight",pLVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightLV)
    iColumn_design += 1

    entry_dCostLV = create_entry_focus(content,iIndex,iColumn_design,"","dCostLV",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostLV)

    iIndex +=1
    iColumn_design = 0

    #create_label(content,iIndex ,iColumn_design,field_name="sTypeConductor",parent_obj=pHVWind)
    entry_sTypeConductorHV = create_entry_focus(content,iIndex,iColumn_design,"","sTypeConductor",pHVWind,width=iWidthE,sticky="ew")
    entries_costs.append(entry_sTypeConductorHV)
    iColumn_design += 1

    entry_dHV = create_entry_focus(content,iIndex,iColumn_design,"","dHV",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dHV)
    iColumn_design += 1

    entry_dWeightHV = create_entry_focus(content,iIndex,iColumn_design,"","dWeight",pHVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightHV)
    iColumn_design += 1

    entry_dCostHV = create_entry_focus(content,iIndex,iColumn_design,"","dCostHV",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostHV)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Coil leads (lb)")
    iColumn_design += 1

    entry_dCoilLeads = create_entry_focus(content,iIndex,iColumn_design,"","dCoilLeads",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCoilLeads)
    iColumn_design += 1

    entry_dWeightCoilLeads = create_entry_focus(content,iIndex,iColumn_design,"","dWeightCoilLeads",pLVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightCoilLeads)
    iColumn_design += 1

    entry_dCostCoilLeads = create_entry_focus(content,iIndex,iColumn_design,"","dCostCoilLeads",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostCoilLeads)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"Busbar (lb)")
    iColumn_design += 1

    entry_dBusbar = create_entry_focus(content,iIndex,iColumn_design,"","dBusbar",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dBusbar)
    iColumn_design += 1

    entry_dWeightBusBar = create_entry_focus(content,iIndex,iColumn_design,"","dWeightBusBar",pLVWind,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightBusBar)
    iColumn_design += 1

    entry_dCostBusBar = create_entry_focus(content,iIndex,iColumn_design,"","dCostBusBar",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostBusBar)

    iIndex +=1
    iColumn_design = 0

    create_label(content,iIndex ,iColumn_design,"PB HB 1/4 (lb)")
    iColumn_design += 1

    entry_dPBHD_1_4 = create_entry_focus(content,iIndex,iColumn_design,"","dPBHD_1_4",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dPBHD_1_4)
    iColumn_design += 1

    entry_dWeightsPBHD_1_4 = create_entry_focus(content,iIndex,iColumn_design,"","dWeightsPBHD_1_4",device.core,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dWeightsPBHD_1_4)
    iColumn_design += 1

    entry_dCostPBHD_1_4 = create_entry_focus(content,iIndex,iColumn_design,"","dCostPBHD_1_4",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dCostPBHD_1_4)


    iIndex +=1

    entry_dTotalCostRound = create_entry_focus(content,iIndex,iColumn_design-1,"Cost","dTotalCostRound",device.costs,width=iWidthE,sticky="ew",bOutput = True)
    entries_costs.append(entry_dTotalCostRound)

    iIndex +=1
    iColumn_design = 0

    hide_controls(False,entries_costs)


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
    if getattr(root, "_costs_win", None):
        _safe_destroy(root._costs_win, root, "_costs_win")

    # if footer already there and valid, refresh grid
    existing = getattr(root, "_costs_frame", None)
    try:
        if existing and existing.winfo_exists():
            # ensure place in case it was hidden
            _safe_place(existing, root)
            return
    except Exception:
        pass

    frame = tk.Frame(root, bg="white", bd=1, relief="raised")
    root._costs_frame = frame

    # initial place
    _safe_place(frame, root)

    # top bar with buttons
    top_bar = tk.Frame(frame, bg="#e0e0e0", height=25)
    top_bar.pack(fill="x")

    # minimize toggles a small bar instead of destroy
    def _destroy_footer():
        _safe_destroy(frame, root, "_costs_frame")

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
        _safe_destroy(root._costs_frame, root, "_costs_frame")
        _show_popup(root, device)

    img_popup = Image.open(resource_path("Resources/popup.png"))
    img_popup = img_popup.resize((20, 20))  # ajusta tamaño si quieres
    popup_icon = ImageTk.PhotoImage(img_popup)

    # crear boton con imagen
    btn_popup = ttk.Button(top_bar, image=popup_icon, width=30, command=_footer_to_popup)
    btn_popup.image = popup_icon  # mantener referencia para que no se borre
    btn_popup.pack(side="right", padx=5)

    # content area
    canvas = tk.Canvas(frame, highlightthickness=0)#, bg="white", highlightthickness=0)
    scrollbar = ttk.Scrollbar(frame, orient="vertical", command=canvas.yview)
    canvas.configure(yscrollcommand=scrollbar.set)

    scrollbar.pack(side="right", fill="y")
    canvas.pack(side="left", fill="both", expand=True)

    # Frame interno para contenido
    content = tk.Frame(canvas)#, bg="white")
    canvas.create_window((0, 0), window=content, anchor="nw")

    _collect_objects(device, content)

    def on_frame_configure(event):
        canvas.configure(scrollregion=canvas.bbox("all"))

    content.bind("<Configure>", on_frame_configure)

    def _on_mousewheel(event):
        widget = event.widget
        if widget.winfo_exists():
            widget.yview_scroll(int(-1 * (event.delta / 120)), "units")

    canvas.bind_all("<MouseWheel>", _on_mousewheel)  # Windows / Mac
    canvas.bind_all("<Button-4>", lambda e: canvas.yview_scroll(-1, "units"))  # Linux
    canvas.bind_all("<Button-5>", lambda e: canvas.yview_scroll(1, "units"))   # Linux

    # bind resize safely: handler checks existence before placing
    def _on_resize(event):
        f = getattr(root, "_costs_frame", None)
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


# ------------------------------------------------------------------
# SWITCH FOOTER -> POPUP (safe)
# ------------------------------------------------------------------
def _switch_footer_to_popup(root, device):
    _safe_destroy(getattr(root, "_costs_frame", None), root, "_costs_frame")
    _show_popup(root, device)


# ------------------------------------------------------------------
# POPUP MODE (safe)
# ------------------------------------------------------------------
def _show_popup(root, device):
    """
    Show popup window. If footer exists, hide/destroy it first.
    """

    # if popup already exists, refresh and lift
    existing_win = getattr(root, "_costs_win", None)
    try:
        if existing_win and existing_win.winfo_exists():
            try:
                existing_win.lift()
            except Exception:
                pass
            return
    except Exception:
        pass

    # hide/destroy footer if present
    _safe_destroy(getattr(root, "_costs_frame", None), root, "_costs_frame")

    win = tk.Toplevel(root)
    win.title("Costs")
    win.geometry("850x550")
    root._costs_win = win

    top_bar = tk.Frame(win, bg="#e0e0e0", height=30)
    top_bar.grid(row=0, column=0, sticky="ew")

    def _to_footer():
        # close popup and show footer
        _safe_destroy(getattr(root, "_costs_win", None), root, "_costs_win")
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

    _collect_objects(device,content)

    win.grid_rowconfigure(1, weight=1)
    win.grid_columnconfigure(0, weight=1)

    # when user closes popup, ensure references cleared and footer restored
    def _on_close():
        _safe_destroy(getattr(root, "_costs_win", None), root, "_costs_win")
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
    _safe_destroy(getattr(root, "_costs_win", None), root, "_costs_win")
    _show_footer(root, device)
