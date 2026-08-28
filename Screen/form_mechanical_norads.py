#form_mechanical_norads.py
from Resources.resources import resource_path
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size
from Definitions.enums import eSegment,eLiquid,eSegment

def open_mechanical_norads_form(root,frame_mechanical_design = None,device = None, iIndex = 0):

    if frame_mechanical_design is None:
        frame_mechanical_design = tk.Toplevel(root)
        frame_mechanical_design.title("Coils Design")
        frame_mechanical_design.geometry("800x600")


    pHVWind = device.getHVWind()
    pLVWind = device.getLVWind()

    #Conductor caculates LV
    iColumn_design = 1
    iIndexCol21 = iIndex+1
    pLCoil = pLVWind.coils[0]
    pHCoil = pHVWind.coils[0]

    img_path_tank = resource_path("Resources\\mechanical_tank_single_phase.png")
    img = Image.open(img_path_tank)
    img = img.resize((300, 300))
    icon = ImageTk.PhotoImage(img)

    widthE = 15
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design,"Cooling Calculation",font_size=12,bold = True,sticky="w",columnspan=2)
    iIndexCol21+=1

    entry_dHin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"H","dHin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in")

    entry_dHmm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dH",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"mm")
    hide_controls(False,entry_dHmm)

    entry_tSegment1mm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+8,"","dSegment[0]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+8,"m2")
    hide_controls(False,entry_tSegment1mm)

    entry_tSegment1in = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+6,"","dSegmentin[0]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+6,"in2")
    hide_controls(False,entry_tSegment1in)

    combo_tSegment1 = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design+4,"S1",list(eSegment),"tSegment[0]",device.mechanical,width=widthE)#,combos=[entry_tSegment1mm,entry_tSegment1in],on_select_func=segment_changed)
    iIndexCol21+=1

    entry_dLin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"L","dLin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in")

    entry_dLmm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dL",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"mm")
    hide_controls(False,entry_dLmm)

    entry_tSegment2mm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+8,"","dSegment[1]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+8,"m2")
    hide_controls(False,entry_tSegment2mm)

    entry_tSegment2in = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+6,"","dSegmentin[1]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+6,"in2")
    hide_controls(False,entry_tSegment2in)

    combo_tSegment2 = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design+4,"S2",list(eSegment),"tSegment[1]",device.mechanical,width=widthE)#,combos=[entry_tSegment2mm,entry_tSegment2in],on_select_func=segment_changed)
    iIndexCol21+=1

    entry_dDin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"D","dDin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in")

    entry_dDmm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dD",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"mm")
    hide_controls(False,entry_dDmm)

    entry_tSegment3mm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+8,"","dSegment[2]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+8,"m2")
    hide_controls(False,entry_tSegment3mm)

    entry_tSegment3in = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+6,"","dSegmentin[2]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+6,"in2")
    hide_controls(False,entry_tSegment3in)

    combo_tSegment3 = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design+4,"S3",list(eSegment),"tSegment[2]",device.mechanical,width=widthE)#,combos=[entry_tSegment3mm,entry_tSegment3in],on_select_func=segment_changed)

    iIndexCol21+=1

    entry_dain = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"a","dain",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in")

    entry_damm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","da",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"mm")
    hide_controls(False,entry_damm)

    entry_tSegment4mm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+8,"","dSegment[3]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+8,"m2")
    hide_controls(False,entry_tSegment4mm)

    entry_tSegment4in = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+6,"","dSegmentin[3]",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+6,"in2")
    hide_controls(False,entry_tSegment4in)

    combo_tSegment4 = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design+4,"S4",list(eSegment),"tSegment[3]",device.mechanical,width=widthE)#,combos=[entry_tSegment4mm,entry_tSegment4in],on_select_func=segment_changed)
    iIndexCol21+=1

    create_label(frame_mechanical_design,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1

    entry_dLosses = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Losses","dLosses",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"W")

    entry_dLosses = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Elevation","dElevation",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"m")

    entry_dExtraDegrees = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+5," ","dExtraDegrees",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+6,"C")
    iIndexCol21+=1

    entry_dHotSpotFactor = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Host spot factor","dHotSpotFactor",device.mechanical,bOutput = True,width=widthE)
    
    combo_tLiquid = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Liquid",list(eLiquid),"tLiquid",device.mechanical,width=widthE)
    iIndexCol21+=1

    entry_dNamePlateRise = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Nameplate Rise","dNameplateRise",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C")    
    iIndexCol21+=1

    entry_dLVGradient = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"LV Gradient","dLVGradient",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C")    

    iIndexCol21+=1

    frame_icon = tk.Frame(frame_mechanical_design)
    frame_icon.place(x=450, y=320)  # ajusta la posicion segun tu tab

    label_icon = tk.Label(frame_icon, image=icon)
    label_icon.image = icon
    label_icon.pack()

    entry_dHVGradient = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"HV Gradient","dHVGradient",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    iIndexCol21+=1

    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"") 
    iIndexCol21+=1

    # entry_dOverrideAin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Override A","dOverrideAin",device.mechanical,bOutput = True,width=widthE,warning_func=device.mechanical.mechanical_warnings.warningOverrideA)
    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in2") 
    
    # entry_dOverrideA = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dOverrideA",device.mechanical,bOutput = True,width=widthE)
    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"m2") 
    # hide_controls(False,entry_dOverrideA)

    # iIndexCol21+=1

    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"") 
    # iIndexCol21+=1

    entry_dAin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Area","dAin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in2") 
    #hide_controls(False,entry_dAin)

    entry_dAm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dA",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"m2") 
    hide_controls(False,entry_dAm)
    iIndexCol21+=1

    entry_dqin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"q","dqin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"kW/in2",font_size = 9)
    hide_controls(False,entry_dqin) 

    entry_dqn = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dq",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"kW/m2",font_size = 9) 
    hide_controls(False,entry_dqn)
    iIndexCol21+=1

    entry_dhin = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"h'","dhin",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"in2") 
    hide_controls(False,entry_dhin)

    entry_dhm = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"","dh",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+3,"m2") 
    hide_controls(False,entry_dhm)
    iIndexCol21+=1

    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"") 
    # iIndexCol21+=1

    entry_dAOR = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"AOR","dAOR",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dAOR)
    iIndexCol21+=1

    entry_dTOR = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"TOR","dTOR",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dTOR)
    iIndexCol21+=1

    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"") 
    # iIndexCol21+=1

    entry_dAWR_LV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"AWR LV","dAWR_LV",device.mechanical,bOutput = True,width=widthE,warning_func=device.mechanical.mechanical_warnings.warningAWRLV)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dAWR_LV)
    iIndexCol21+=1

    entry_dAWR_HV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"AWR HV","dAWR_HV",device.mechanical,bOutput = True,width=widthE,warning_func=device.mechanical.mechanical_warnings.warningAWRHV)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dAWR_HV)
    iIndexCol21+=1

    # create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"") 
    # iIndexCol21+=1

    entry_dHSR_LV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"HSR LV","dHSR_LV",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dHSR_LV)
    iIndexCol21+=1

    entry_dHSR_HV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"HSR HV","dHSR_HV",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+1,"C") 
    hide_controls(False,entry_dHSR_HV)
    iIndexCol21+=1

   

    def segment_changed(event, selected,device,controls):
        value = parse_enum(eSegment,selected)

        if controls:
            for w in controls:
                if value == eSegment.eBLOCK:
                    hide_controls(False,w)
                else:
                    hide_controls(True,w)

    def parse_enum(enum_class, selected):
        # Ya es un enum
        if isinstance(selected, enum_class):
            return selected

        # Caso: coincide con un name del enum
        if isinstance(selected, str) and selected in enum_class.__members__:
            return enum_class[selected]

        # Caso: coincide con un value del enum
        for member in enum_class:
            if member.value == selected:
                return member

        # Nada coincide
        return None

    


    