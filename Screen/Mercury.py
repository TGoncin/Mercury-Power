# /////////////////////////////////////////////////////////////////////////////////
# /// \file        MercuryPower.py
# /// \author      Sandra Lisset Cruz Rivera & Tom Goncin
# /// \brief       Main application module for Mercury Power Transformer Designer
# /// \date        Mar 31, 2026
# ///
# /// \copyright   © PTI Transformers
# ///
# /// \details
# /// This is the main application module for the Mercury Power Transformer
# /// Designer. It provides a graphical user interface for managing transformer
# /// design parameters including core, windings, guarantees, mechanical
# /// calculations, and testing specifications.
# /////////////////////////////////////////////////////////////////////////////////



import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from PIL import Image, ImageTk
import sys
import os
import importlib.util

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))


# The exact folder where Visual Studio builds your .pyd files
bridge_bin_path = r"C:\Users\TGonc\source\repos\Mercury Optimizer 3Phase\cppBRIDGEpy\bin\Release"


# 1. Bypass Windows Security: Explicitly tell Python to trust this folder for DLLs
if hasattr(os, 'add_dll_directory'):
    os.add_dll_directory(bridge_bin_path)


# 2. Tell Python to look in this folder for your custom modules
if bridge_bin_path not in sys.path:
    sys.path.insert(0, bridge_bin_path)

import bridge_CORE
import bridge_PHYSICS
import bridge_COMPONENTS

MODULE_LOADED = True
mc = None  # Keep this as None just in case any old UI code checks for it

from gui_helpers import (
    create_label, create_entry, create_combobox, create_checkbox,
    create_label_entry_pair, create_label_combobox_pair, create_button_row,
    set_entry_value, get_entry_value, create_label_frame, hide_controls
)
from form_design_information import open_design_form
from form_output_information import open_output_form
from OpenXlsx import open_excel
from Definitions.enums import *

# ============================================
# Route configuration
# ============================================

# Get current directory
current_dir = os.path.dirname(os.path.abspath(__file__))

# Add SourceFiles to the path to import UI modules
source_path = os.path.join(current_dir, "SourceFiles")
if source_path not in sys.path:
    sys.path.insert(0, source_path)

# C++ Module Path Configuration
bin_path = os.path.join(current_dir, "..", "bin", "Release")
bin_path = os.path.abspath(bin_path)

python32_dir = r"C:\Users\scruz\AppData\Local\Programs\Python\Python39-32"

# Import UI Modules
from tab_core_ui import build_core_tab
from tab_mechanical_ui import build_mechanical_tab
from tab_tests_ui import build_tests_tab
from tab_general_ui import build_general_tab
from tab_winding_ui import build_winding_tab
from tab_design_ui import build_design_tab



# ============================================
# Helper Functions
# ============================================

def resource_path(relative_path):
    """Get the absolute path of a resource"""
    try:
        base_path = sys._MEIPASS
    except Exception:
        # Anchor the path to the directory containing Mercury.py
        base_path = os.path.dirname(os.path.abspath(__file__))
    
    full_path = os.path.join(base_path, relative_path)
    return full_path


def load_icon(path, size=(24, 24)):
    """Load an icon from the specified path with better error handling"""
    try:
        full_path = resource_path(path)
        
        if not os.path.exists(full_path):
            print(f"⚠️ Icon file not found: {full_path}")
            return None
        
        img = Image.open(full_path)
        img = img.resize(size, Image.Resampling.LANCZOS)
        return ImageTk.PhotoImage(img)
        
    except Exception as e:
        print(f"❌ Error loading icon {path}: {e}")
        print(f"   Full path: {full_path if 'full_path' in locals() else 'unknown'}")
        return None

def verify_icons():
    """Verify that all required icons exist"""
    icons = [
        "Resources\\new_icon.png",
        "Resources\\open_file_icon.png", 
        "Resources\\save_icon.png",
        "Resources\\import_excel_icon.png",
        "Resources\\export_excel_icon.png",
        "Resources\\reports_icon.png",
        "Resources\\exit_icon.png",
        "Resources\\dollar-icon.png"
    ]
    
    missing_icons = []
    for icon in icons:
        full_path = resource_path(icon)
        if not os.path.exists(full_path):
            missing_icons.append(icon)
    
    if missing_icons:
        print("\n⚠️ Missing icon files:")
        for icon in missing_icons:
            print(f"   - {icon}")
        print(f"\nCurrent working directory: {os.getcwd()}")
        print(f"Resource base path: {resource_path('')}")
        return False
    else:
        print("✓ All icon files found")
        return True

class ToolTip:
    """Class to display tooltips on buttons"""
    def __init__(self, widget, text):
        self.widget = widget
        self.text = text
        self.tooltip_window = None
        widget.bind("<Enter>", self.show_tooltip)
        widget.bind("<Leave>", self.hide_tooltip)

    def show_tooltip(self, event=None):
        x = self.widget.winfo_rootx() + 20
        y = self.widget.winfo_rooty() + 20
        self.tooltip_window = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(True)
        tw.wm_geometry(f"+{x}+{y}")
        label = tk.Label(tw, text=self.text, background="#ffffe0",
                        relief="solid", borderwidth=1, font=("Segoe UI", 9))
        label.pack()

    def hide_tooltip(self, event=None):
        if self.tooltip_window:
            self.tooltip_window.destroy()
            self.tooltip_window = None



def refresh_callbacks_recursive(obj):
    """Refresh callbacks recursively"""
    if hasattr(obj, "_refresh_callbacks"):
        for refresh_func in obj._refresh_callbacks:
            refresh_func()
    
    for attr_name in dir(obj):
        if attr_name.startswith("_"):
            continue
        attr = getattr(obj, attr_name)
        if isinstance(attr, list):
            for item in attr:
                refresh_callbacks_recursive(item)
        elif isinstance(attr, dict):
            for item in attr.values():
                refresh_callbacks_recursive(item)
        elif hasattr(attr, "_refresh_callbacks"):
            refresh_callbacks_recursive(attr)


# ============================================
# Main Class
# ============================================


class TransformerApp:
    def _on_windy_available(self):
        """Callback cuando windY está disponible"""
        if hasattr(self, 'tab_winding') and hasattr(self.tab_winding, 'windY'):
            self.tab_winding.windY = self.windY
            print("  ✓ Updated tab_winding.windY reference")
        
        if hasattr(self, 'windings') and 'TER' in self.windings:
            self.windings['TER'] = self.windY
            print("  ✓ Updated windings['TER'] reference")

    def __init__(self, root):
        self.root = root
        self.root.title("PTI Transformers - Mercury Power Transformer Designer")
        
        # Set the window icon
        ico_path = resource_path("..\\Resources\\Mercury.ico")
        if os.path.exists(ico_path):
            self.root.iconbitmap(ico_path)
        
        self.root.state("zoomed")
        self.root.minsize(1200, 800)
        self.root.configure(bg='#f0f0f0')
        
        self.MODULE_LOADED = MODULE_LOADED
        
        # C++ Data Structure
        self._connector = None
        self.device = None
        self.core = None
        self.guarantee = None
        self.mechanical = None
        self.tests = None
        self.costs = None
        self.cooling = None
        self.losses = None
        self.coils = None
        
        # Widgets references
        self.winding_widgets = {}
        self.core_widgets = {}
        self.guarantee_widgets = {}
        self.mechanical_widgets = {}
        self.tests_widgets = {}
        
        # Dictionary for optimizer designs
        self.root.hg_dict = {}

        # Create C++ instances if module is loaded (Removed mc check)
        if MODULE_LOADED:
            self._create_cpp_instances()
        
        # Create UI
        self._create_toolbar()
        frame_design_information = tk.Frame(root,width=1400, height=100)
        frame_design_information.pack(side="top", fill="x", pady=(0, 0))

        open_design_form(root, frame_design_information, self.device, None)

        self._create_footer()
        frame_output_information = tk.Frame(root, width=1400, height=100)
        frame_output_information.pack(side="bottom", fill="x", pady=(0, 0))
        open_output_form(root, frame_output_information, self.device)

        self._create_notebook()
        self._create_menu()

    def _get_coil_by_desc_and_regulation(self, desc: int, is_regulation: bool):
        """Search for a coil by description and regulation flag"""
        if not self.coils:
            return None

        if isinstance(self.coils, (list, tuple)):
            coils_iter = self.coils
        else:
            coils_iter = self.coils.values()

        for coil in coils_iter:
            try:
                if coil.getDescDevanado() == desc and coil.getIsRegulation() == is_regulation:
                    return coil
            except AttributeError:
                continue
        return None


    def _create_cpp_instances(self):
        try:
            print("\n=== Creating Modular C++ instances ===")
            self.device = bridge_CORE.Device()
            print("✓ bridge_CORE.Device instance created")

            class _DeviceProxy:
                def __init__(self, real_device):
                    object.__setattr__(self, '_real_device', real_device)

                def __getattribute__(self, name):
                    if name == '_real_device':
                        return object.__getattribute__(self, name)
                    real_dev = object.__getattribute__(self, '_real_device')
                    if name.startswith('get_'):
                        base_name = name[4:]
                        camel_parts = base_name.split('_')
                        camel_name = 'get' + camel_parts[0].capitalize() + ''.join(p.capitalize() for p in camel_parts[1:])
                        if real_dev is not None and hasattr(real_dev, camel_name):
                            return getattr(real_dev, camel_name)
                        if real_dev is not None and hasattr(real_dev, 'deviceFields') and base_name in real_dev.deviceFields:
                            return lambda: real_dev.deviceFields[base_name].get()
                        return lambda: ""
                    if real_dev is not None and hasattr(real_dev, name):
                        attr = getattr(real_dev, name)
                        if callable(attr): return attr
                        if attr is not None and not isinstance(attr, (str, int, float, list, dict, tuple)):
                            return _DeviceProxy(attr)
                        return attr
                    #raise AttributeError(f"'_DeviceProxy' object has no attribute '{name}'")
                    print(f"⚠️ MISSING ATTRIBUTE: '{name}'. Providing safe fallback.")
                    return lambda *args, **kwargs: ""

                def __setattr__(self, name, value):
                    real_dev = object.__getattribute__(self, '_real_device')
                    setattr(real_dev, name, value)

            self.device = _DeviceProxy(self.device)
            print("✓ Proxy wrapper applied")

            #print("-> Attempting reassignDefaults...")
            #if hasattr(self.device, 'reassignDefaults'):
            #    self.device.reassignDefaults()
            #print("✓ reassignDefaults passed")

            print("-> Attempting core access...")
            self.core = _DeviceProxy(self.device.core) if hasattr(self.device, 'core') and self.device.core else None
            
            print("-> Attempting WindingHV access...")
            self.windH = _DeviceProxy(self.device.get_WindingHV()) if hasattr(self.device, 'get_WindingHV') else None
            
            print("-> Attempting WindingLV access...")
            self.windX = _DeviceProxy(self.device.get_WindingLV()) if hasattr(self.device, 'get_WindingLV') else None
            
            print("-> Attempting WindingTV access...")
            self.windY = _DeviceProxy(self.device.get_WindingTV()) if hasattr(self.device, 'get_WindingTV') else None
            
            print("-> Attempting sub-module proxy wrapping...")
            #self.guarantee = _DeviceProxy(self.device.guarantee) if hasattr(self.device, 'guarantee') and self.device.guarantee else None
            #self.costs = _DeviceProxy(self.device.costs) if hasattr(self.device, 'costs') and self.device.costs else None
            self.cooling = _DeviceProxy(self.device.cooling) if hasattr(self.device, 'cooling') and self.device.cooling else None
            
            print("-> Attempting coil list access...")
            self.coils = list(self.device.coil) if hasattr(self.device, 'coil') else []

            print("=== C++ instances initialization complete ===\n")
            self.MODULE_LOADED = True

        except Exception as e:
            print(f"❌ Error creating C++ instances: {e}")
            self.MODULE_LOADED = False

    def _update_component_references(self, force_clear=False):
        """Update references directly from the Device"""
        if not self.device:
            print("No device available")
            return

        if not hasattr(self, 'windings') or self.windings is None:
            self.windings = {'HV': None, 'LV': None, 'TER': None}

        print("Updating component references...")

        try:
            # Call calculations directly on self.device now!
            self.device.reassignDefaults()
            self.device.calculateDevice()
            self.device.calculateDevice()
        except Exception as e:
            print(f"⚠️ Error recalculating device: {e}")

        # Core
        if hasattr(self.device, 'core'):
            self.core = self.device.core
            print("  ✓ Core reference updated")
        elif force_clear:
            self.core = None

        # Windings (using your direct C++ getters)
        if hasattr(self.device, 'get_WindingHV'):
            self.windings['HV'] = self.device.get_WindingHV()
            self.windings['LV'] = self.device.get_WindingLV()
            self.windings['TER'] = self.device.get_WindingTV()
            print("  ✓ Windings references updated")

        # Guarantee
        if hasattr(self.device, 'guarantee'):
            self.guarantee = self.device.guarantee
            print("  ✓ Guarantee reference updated")

        # Costs
        if hasattr(self.device, 'costs'):
            self.costs = self.device.costs
            print("  ✓ Costs reference updated")

        # Cooling
        if hasattr(self.device, 'cooling'):
            self.cooling = self.device.cooling
            print("  ✓ Cooling reference updated")

        print("Component references update complete")

    def create_new_design(self):
        """Create a new transformer design"""
        if not self.MODULE_LOADED:
            messagebox.showerror("Error", "C++ Module not loaded")
            return False
        
        try:
            print("\n=== Creating new design ===")
            
            # Create a brand new device directly from your C++ bridge!
            self.device = bridge_CORE.Device()
            print("✓ New device created directly via bridge_CORE")
            
            if self.device:
                self._update_component_references()
                self._update_status("New design created")
                messagebox.showinfo("Success", "New design created successfully")
                return True
            else:
                return False
                
        except Exception as e:
            print(f"Error creating new design: {e}")
            import traceback
            traceback.print_exc()
            messagebox.showerror("Error", f"Failed to create new design: {e}")
            return False


    def save_file(self, file_path=None):
        """Save transformer design to XML file"""
        if not self.MODULE_LOADED or not self.device:
            messagebox.showerror("Error", "C++ module not loaded or object not initialized")
            return False

        if file_path is None:
            from pathlib import Path
            documents_dir = Path.home() / "Documents"
    
            so_num = None
            if hasattr(self.device, 'getSoNumber'):
                try:
                    so_num = self.device.getSoNumber()
                except Exception:
                    so_num = None
            
            default_filename = f"{so_num}.xml" if so_num else "transformer.xml"

            file_path = filedialog.asksaveasfilename(
                title="Save Transformer Design",
                defaultextension=".xml",
                initialdir=str(documents_dir),
                initialfile=default_filename,
                filetypes=[("XML files", "*.xml"), ("All files", "*.*")]
            )

        if not file_path:
            return False

        try:
            print(f"\n=== Saving file with saveToXML: {file_path} ===")
            success = self.device.saveToXML(file_path)
        
            if success:
                self._update_status(f"Saved: {os.path.basename(file_path)}")
                messagebox.showinfo("Success", f"File saved successfully:\n{file_path}")
                return True
            else:
                messagebox.showerror("Error", f"Failed to save file:\n{file_path}")
                return False
            
        except Exception as e:
            print(f"Error saving file: {e}")
            import traceback
            traceback.print_exc()
            messagebox.showerror("Error", f"Exception while saving:\n{str(e)}")
            return False

    def get_device_type(self):
        """Get the device type"""
        if not self.MODULE_LOADED or not self._connector:
            return None
        try:
            if hasattr(self._connector, 'get_device_type'):
                return self._connector.get_device_type()
            return -1
        except Exception as e:
            print(f"Error getting device type: {e}")
            return None

    def _update_status(self, message):
        """Update status bar messages"""
        if hasattr(self, 'status_label'):
            self.status_label.config(text=message)
        print(f"Status: {message}")

    def _create_toolbar(self):
        """Create the toolbar"""
        toolbar = tk.Frame(self.root, bd=1, relief="raised")
        toolbar.pack(side="top", fill="x")
        
        verify_icons()
        self.toolbar_icons = {}

        self.toolbar_icons['new'] = load_icon("Resources\\new_icon.png")
        self.toolbar_icons['open'] = load_icon("Resources\\open_file_icon.png")
        self.toolbar_icons['save'] = load_icon("Resources\\save_icon.png")
        self.toolbar_icons['import_excel'] = load_icon("Resources\\import_excel_icon.png")
        self.toolbar_icons['export_excel'] = load_icon("Resources\\export_excel_icon.png")
        self.toolbar_icons['reports'] = load_icon("Resources\\reports_icon.png")
        self.toolbar_icons['exit'] = load_icon("Resources\\exit_icon.png")
        
        if self.toolbar_icons['new']:
            btn_new = ttk.Button(toolbar, image=self.toolbar_icons['new'], command=self.new_quotation)
            btn_new.pack(side="left", padx=2, pady=2)
            ToolTip(btn_new, "New quotation or order")
        
        if self.toolbar_icons['open']:
            btn_open = ttk.Button(toolbar, image=self.toolbar_icons['open'], command=self.open_file)
            btn_open.pack(side="left", padx=2, pady=2)
            ToolTip(btn_open, "Open quotation or order")
        
        if self.toolbar_icons['save']:
            btn_save = ttk.Button(toolbar, image=self.toolbar_icons['save'], command=self.save_file)
            btn_save.pack(side="left", padx=2, pady=2)
            ToolTip(btn_save, "Save quotation or order")

        if self.toolbar_icons['import_excel']:
            btn_imp_excel = ttk.Button(toolbar, image=self.toolbar_icons['import_excel'], command=self.import_excel)
            btn_imp_excel.pack(side="left", padx=2, pady=2)
            ToolTip(btn_imp_excel, "Import from Excel")
        
        if self.toolbar_icons['export_excel']:
            btn_exp_excel = ttk.Button(toolbar, image=self.toolbar_icons['export_excel'], command=self.export_excel)
            btn_exp_excel.pack(side="left", padx=2, pady=2)
            ToolTip(btn_exp_excel, "Export to Excel")

        if self.toolbar_icons['reports']:
            btn_reports = ttk.Button(toolbar, image=self.toolbar_icons['reports'], command=self.generate_reports)
            btn_reports.pack(side="left", padx=2, pady=2)
            ToolTip(btn_reports, "Generate reports")

        if self.toolbar_icons['exit']:
            btn_exit = ttk.Button(toolbar, image=self.toolbar_icons['exit'], command=self.root.quit)
            btn_exit.pack(side="left", padx=2, pady=2)
            ToolTip(btn_exit, "Exit")

    def _create_notebook(self):
        """Create the notebook with all tabs"""
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(expand=True, fill="both", padx=10, pady=(0, 10))
        
        self.tab_general = build_general_tab(self.notebook, self)
        self.tab_winding = build_winding_tab(self.notebook, self)
        # self.tab_tests = build_tests_tab(self.notebook, self)
        # self.tab_core = build_core_tab(self.notebook, self)
        # self.tab_design = build_design_tab(self.notebook, self)
        # self.tab_mechanical = build_mechanical_tab(self.notebook, self)
        
        self.tabs = [self.tab_general, self.tab_winding]#, self.tab_core, self.tab_design, self.tab_mechanical, self.tab_tests]

    def _create_menu(self):
        """Create the main menu"""
        menubar = tk.Menu(self.root)
        
        file_menu = tk.Menu(menubar, tearoff=0)
        file_menu.add_command(label="New", command=self.new_quotation)
        file_menu.add_command(label="Open", command=self.open_file)
        file_menu.add_command(label="Save", command=self.save_file)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.root.quit)
        menubar.add_cascade(label="File", menu=file_menu)
        
        tools_menu = tk.Menu(menubar, tearoff=0)
        tools_menu.add_command(label="Test Module", command=self.test_module)
        tools_menu.add_command(label="Module Info", command=self.show_module_info)
        menubar.add_cascade(label="Tools", menu=tools_menu)
        
        help_menu = tk.Menu(menubar, tearoff=0)
        help_menu.add_command(label="User Manual", command=self.open_pdf)
        help_menu.add_command(label="About", command=self.about)
        menubar.add_cascade(label="Help", menu=help_menu)
        
        self.root.config(menu=menubar)
        self.menu_bar = menubar

    def _create_footer(self):
        """Create the footer with additional icons"""
        footer = tk.Frame(self.root, bd=1, relief="raised", height=35)
        footer.pack(side="bottom", fill="x")
        footer.pack_propagate(False)
        footer.lift()

        verify_icons()
        self.footer_icons = {}
        self.footer_icons['info'] = load_icon("Resources\\dollar-icon.png", (20, 20))
        self.footer_icons['warning'] = load_icon("Resources\\warning-icon2.png", (20, 20))
        self.footer_icons['help'] = load_icon("Resources\\help_icon.png", (20, 20))

        if self.footer_icons['info']:
            btn_info = ttk.Button(footer, image=self.footer_icons['info'], command=self.show_module_info)
            btn_info.pack(side="left", padx=2, pady=2)
            ToolTip(btn_info, "Module Information")

        separator = ttk.Separator(footer, orient='vertical')
        separator.pack(side="left", fill="y", padx=5, pady=2)

        self.status_label = ttk.Label(footer, text="Ready", anchor="w")
        self.status_label.pack(side="left", padx=5, fill="x", expand=True)

    # ============================================
    # Action Functions
    # ============================================

    def new_quotation(self):
        """Create a new quote"""
        if messagebox.askyesno("New", "Create a new quotation? Current data will be lost."):
            self.create_new_design()

    def open_file(self):
        """Open an existing transformer design from an XML file"""
        if not self.MODULE_LOADED or not self.device:
            messagebox.showerror("Error", "C++ module not loaded or object not initialized")
            return False

        from pathlib import Path
        documents_dir = Path.home() / "Documents"

        file_path = filedialog.askopenfilename(
            title="Open Transformer Design",
            initialdir=str(documents_dir),
            filetypes=[("XML files", "*.xml"), ("All files", "*.*")]
        )

        if not file_path:
            return False

        try:
            print(f"\n=== Opening file with openXML: {file_path} ===")
            success = self.device.openXML(file_path)
        
            if success:
                self._update_component_references()
                self._update_status(f"Opened: {os.path.basename(file_path)}")
                messagebox.showinfo("Success", f"File loaded successfully:\n{file_path}")
                return True
            else:
                messagebox.showerror("Error", f"Failed to load file:\n{file_path}")
                return False
            
        except Exception as e:
            print(f"Error opening file: {e}")
            import traceback
            traceback.print_exc()
            messagebox.showerror("Error", f"Exception while opening:\n{str(e)}")
            return False

    def import_excel(self):
        """Import transformer design from an Excel file"""
        if not self.MODULE_LOADED or not self.device:
            messagebox.showerror("Error", "C++ module not loaded or object not initialized")
            return False

        try:
            success = open_excel(self)
            if success:
                self._update_component_references()
                self._update_status("Excel import completed successfully")
                messagebox.showinfo("Success", "Transformer design imported from Excel successfully.")
                return True
            else:
                self._update_status("Excel import failed")
                messagebox.showerror("Error", "Failed to import from Excel.")
                return False
        except Exception as e:
            print(f"Unexpected error during Excel import: {e}")
            import traceback
            traceback.print_exc()
            messagebox.showerror("Error", f"Exception during Excel import:\n{str(e)}")
            return False

    def export_excel(self):
        """Export to Excel"""
        file_path = filedialog.asksaveasfilename(
            title="Export to Excel",
            defaultextension=".xlsx",
            filetypes=[("Excel files", "*.xlsx")]
        )
        if file_path:
            self._update_status(f"Exporting to Excel: {os.path.basename(file_path)}")
            messagebox.showinfo("Info", "Excel export feature coming soon")

    def generate_reports(self):
        """Generate reports"""
        self._update_status("Generating reports...")
        messagebox.showinfo("Info", "Report generation feature coming soon")

    def open_pdf(self):
        """Open the user manual"""
        ruta_pdf = r"\\PTISK-fileserver.pti.local\DATA\ENGINEERING\13-Enginerring projects\MERCURY\OPTIMIZATION THREE PHASE\Dont touch\MERCURY SYSTEM USER MANUAL.pdf"  
        if os.path.exists(ruta_pdf):
            os.startfile(ruta_pdf)
        else:
            messagebox.showinfo("Info", "User manual not found")

    def test_module(self):
        """Test the C++ module"""
        if not self.MODULE_LOADED:
            messagebox.showerror("Error", "C++ Module not loaded")
            return
        
        info = "=== MidConnector Module Test ===\n\n"
        info += f"Module loaded from: {bin_path}\n"
        info += "Available components:\n"
        info += f"  • Connector: {self._connector is not None}\n"
        info += f"  • Device: {self.device is not None}\n"
        info += f"  • Core: {self.core is not None}\n"
        info += f"  • Guarantee: {self.guarantee is not None}\n"
        info += f"  • Costs: {self.costs is not None}\n"
        info += f"  • Cooling: {self.cooling is not None}\n"
        info += f"  • Coils: {len(self.coils) if self.coils else 0}\n"
        
        messagebox.showinfo("Module Test", info)

    def show_module_info(self):
        """Show module information"""
        info = "Mercury Power Transformer Designer\n"
        info += "=" * 40 + "\n\n"
        info += "C++ Integration Status:\n"
        info += f"  • Module Loaded: {self.MODULE_LOADED}\n"
        if self.MODULE_LOADED:
            info += f"  • Module Path: {bin_path}\n"
            info += f"  • Connector: {'✓' if self._connector else '✗'}\n"
            info += f"  • Device: {'✓' if self.device else '✗'}\n"
        
        info += "\nFeatures:\n"
        info += "  • Core Management\n"
        info += "  • Winding Management (HV, LV, Tertiary)\n"
        info += "  • Guarantee Management\n"
        info += "  • Mechanical Calculations\n"
        info += "  • Test Management\n"
        info += "  • Cost Analysis\n"
        info += "  • Cooling System\n"
        info += "  • Loss Calculations\n"
        
        messagebox.showinfo("Module Information", info)

    def about(self):
        """Show application information"""
        info = "PTI Transformers - Mercury Power Transformer Designer\n"
        info += "Version 1.0\n\n"
        info += "Power Transformer Design Software with C++ Integration\n\n"
        info += f"C++ Module: {'Loaded' if self.MODULE_LOADED else 'Not Loaded'}\n"
        info += f"Module Path: {bin_path}\n\n"
        info += "© PTI Transformers 2026"
        
        messagebox.showinfo("About", info)


# ============================================
# Main Entry Point
# ============================================

if __name__ == "__main__":
    root = tk.Tk()
    app = TransformerApp(root)
    
    def on_close():
        root.destroy()
        sys.exit()
    
    root.protocol("WM_DELETE_WINDOW", on_close)
    root.mainloop()