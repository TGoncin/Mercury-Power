#ReportForm.py
import tkinter as tk
from tkinter import ttk
from Reports import Broomfield
from Reports.AllReports import make_excel_reports
from Reports.ForME import make_for_ME_page
from Reports.PurchasingSummary import make_purchasing_summary
from Reports.SAP_purchasing import make_SAP_purchasing
from Reports.Broomfield import make_Broomfield
from tkinter import messagebox
import traceback

report_form_instance = None

def open_report_form(root=None, device=None):
    global report_form_instance

    if report_form_instance is not None and report_form_instance.winfo_exists():
        # Si existe, solo lo ponemos al frente
        report_form_instance.lift()
        return

    # Crear una nueva ventana (form)
    report_form_instance = tk.Toplevel(root)  # <-- asignar a la variable global
    form = report_form_instance
    form.title("Generate Reports")
    form.geometry("400x210")

    # Etiqueta de título
    ttk.Label(form, text="Reports", font=("Segoe UI", 12, "bold")).pack(pady=10)

    # Frame para los checkbuttons y botones
    frame = ttk.Frame(form)
    frame.pack(pady=10, padx=10)

    # Variables para los checkbuttons
    select_all_var = tk.BooleanVar()
    option1 = tk.BooleanVar()
    option2 = tk.BooleanVar()
    option3 = tk.BooleanVar()
    option4 = tk.BooleanVar()

    # Función para seleccionar/desmarcar todos
    def select_all():
        state = select_all_var.get()
        option1.set(state)
        option2.set(state)
        option3.set(state)
        option4.set(state)

    # Función para actualizar "Select All" si algún check individual cambia
    def update_select_all():
        select_all_var.set(option1.get() and option2.get() and option3.get() and option4.get())

    # Checkbutton "Select All"
    ttk.Checkbutton(frame, text="Select All", variable=select_all_var, command=select_all).grid(row=0, column=0, columnspan=2, sticky='w', pady=5)

    # Checkbuttons individuales en 2 columnas
    ttk.Checkbutton(frame, text="All reports", variable=option1, command=update_select_all).grid(row=1, column=0, sticky='w', padx=5)
    ttk.Checkbutton(frame, text="Report for ME", variable=option2, command=update_select_all).grid(row=2, column=0, sticky='w', padx=5)
    ttk.Checkbutton(frame, text="Broomfield", variable=option3, command=update_select_all).grid(row=0, column=1, sticky='w', padx=5)
    ttk.Checkbutton(frame, text="SAP Purchasing", variable=option4, command=update_select_all).grid(row=1, column=1, sticky='w', padx=5)

    # Función Generate
    def generate_report():
        selected = []
        errors = []

        try:
            if option1.get():
                make_excel_reports(device)
                selected.append("All reports")
            if option2.get():
                make_for_ME_page(device)
                selected.append("Report for ME")
            if option3.get():
                make_Broomfield(device)
                selected.append("Broomfield")
            if option4.get():
                make_SAP_purchasing(device)
                selected.append("SAP Purchasing")
        except Exception as e:
            errors.append(str(e))
            traceback.print_exc()

        if errors:
            messagebox.showerror(title="Error Generating Reports", message="An error occurred while generating reports:\n\n" + "\n".join(errors))
        elif selected:
            messagebox.showinfo(title="Reports Generated", message="The following reports were generated:\n\n" + "\n".join(selected))
        else:
            messagebox.showwarning(title="No Reports Selected", message="No reports were selected to generate.")

    # Frame para botones
    button_frame = ttk.Frame(form)
    button_frame.pack(pady=10)

    ttk.Button(button_frame, text="Generate", command=generate_report).pack(side='left', padx=10)

    ttk.Button(button_frame, text="Close", command=form.destroy).pack(side='left', padx=10)
