#form_mechanical_norads.py
from Resources.resources import resource_path
from PIL import Image, ImageTk
import tkinter as tk
from tkinter import ttk
from gui_helpers import create_label, create_entry_focus, create_combobox_focus,create_checkbox_focus,on_select_show,toggle_controls,hide_controls,size
from Definitions.enums import eLogical, eTemperatureRise,eRadiator,eCooling

def open_mechanical_rads_form(root,frame_mechanical_design = None,device = None, iIndex = 0):

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

    #img_path_tank = resource_path("Resources\\mechanical_tank_single_phase.png")
    #img = Image.open(img_path_tank)
    #img = img.resize((300, 300))
    #icon = ImageTk.PhotoImage(img)

    widthE = 15
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design,"Cooling Calculation",font_size=12,bold = True,sticky="w",columnspan=2)
    iIndexCol21+=1

    temp_rises = [x for x in list(eTemperatureRise) if x!= eTemperatureRise.Temp_55_65]
    tcooling = [x for x in list(eCooling) if x!= eCooling.eNONE]

    create_label(frame_mechanical_design,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1

    combo_tRadiator = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Radiator brand",list(eRadiator),"tRadiator",device.mechanical,width=12)
    entry_dAOR0 = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Proposed AOR Increase-ONAF1","d_AOR[0]",device.mechanical,width=widthE)
    iIndexCol21+=1

    combo_tCooling = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Cooling",list(tcooling),"tCooling",device.mechanical,width=12)
    entry_dAOR1 = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Proposed AOR Increase-ONAF2","d_AOR[1]",device.mechanical,width=widthE)
    iIndexCol21+=1

    combo_tConservator = create_combobox_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Conservator",list(eLogical),"tConservator",device.mechanical,width=12)
    entry_iHCABINF = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Lower radiators header height","iHCABINF",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iAvAmbTemp = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Avg Ambient Temp","iAvAmbTemp",device.mechanical,width=widthE)
    entry_dEEC = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Rads height (between headers)","dEEC",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iFactResistLV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Fact Resist LV","dFactResistLV",device.mechanical,width=widthE)
    entry_iCFM = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Fans Airflow (CFM)","iCFM",device.mechanical,width=widthE)
    iIndexCol21+=1

    entry_iFactResistHV = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Fact Resist HV","dFactResistHV",device.mechanical,width=widthE)
    entry_iTankIntLarge = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Tank's interior large","iTankIntLarge",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iObleas = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Fans per Radiator","iObleas",device.mechanical,width=widthE)
    entry_iTankIntWidth = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Tank's interior width","iTankIntWidth",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iRadiators = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Radiators","iRadiators",device.mechanical,width=widthE)
    entry_iTankIntHeight = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"Tank's oil height","iTankIntHeight",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iGroupRad = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Group Airflow (Fans)","iGroupRad",device.mechanical,width=widthE)
    entry_iCCCenterHeight = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design+3,"C&C center height","iCCCenterHeight",device.mechanical,width=widthE)
    create_label(frame_mechanical_design,iIndexCol21,iColumn_design+4,"mm")
    iIndexCol21+=1

    entry_iQtyFansFA1 = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Fans FA1","iQtyFansFA1",device.mechanical,width=widthE)
    iIndexCol21+=1

    entry_iQtyFansFA2 = create_entry_focus(frame_mechanical_design,iIndexCol21,iColumn_design,"Fans FA2","iQtyFansFA2",device.mechanical,width=widthE)
    iIndexCol21+=1

    for i in range(4):
        create_label(frame_mechanical_design,iIndexCol21,iColumn_design,"")
        iIndexCol21+=1

    frame_rise = tk.Frame(frame_mechanical_design)
    frame_rise.place(x=10, y=420)
    iIndex = 0
    create_label(frame_rise,iIndex,iColumn_design,"Rise")
    create_label(frame_rise,iIndex,iColumn_design+1,"kVA ONAN")
    create_label(frame_rise,iIndex,iColumn_design+2,"kVA ONAF")
    create_label(frame_rise,iIndex,iColumn_design+3,"kW ONAN")
    create_label(frame_rise,iIndex,iColumn_design+4,"kW ONAF")
    create_label(frame_rise,iIndex,iColumn_design+5,"TOR°C")
    iIndex+=1

    entries_cooling0 = []
    combo_tTempONAN = create_entry_focus(frame_rise,iIndex,iColumn_design,"","tTemperatureRise[0]",device.cooling,width=widthE)
    entries_cooling0.append(combo_tTempONAN)
    entry_dkVAONAN1 = create_entry_focus(frame_rise,iIndex,iColumn_design+1,"","d_kVA[0]",pHVWind,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkVAONAN1)
    entry_dkWONAF1 = create_entry_focus(frame_rise,iIndex,iColumn_design+2,"","d_kVA[1]",pHVWind,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkWONAF1)
    entry_dkWONAN1 = create_entry_focus(frame_rise,iIndex,iColumn_design+3,"","d_kW1[0]",device.mechanical,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkWONAN1)
    entry_dkWONAF1 = create_entry_focus(frame_rise,iIndex,iColumn_design+4,"","d_kW1[1]",device.mechanical,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkWONAF1)
    entry_dTORONAF0 = create_entry_focus(frame_rise,iIndex,iColumn_design+5,"","dTORONAF[0]",device.cooling,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dTORONAF0)
    iIndex+=1

    
    combo_tTempONAF = create_entry_focus(frame_rise,iIndex,iColumn_design,"","tTemperatureRise[1]",device.cooling,width=widthE)
    entries_cooling0.append(combo_tTempONAF)
    entry_dkVAONAN2 = create_entry_focus(frame_rise,iIndex,iColumn_design+1,"","dMVAOExc[0]",device.cooling,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkVAONAN2)
    entry_dkVAONAF2 = create_entry_focus(frame_rise,iIndex,iColumn_design+2,"","dMVAOExc[1]",device.cooling,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkVAONAF2)
    entry_dkWONAN2 = create_entry_focus(frame_rise,iIndex,iColumn_design+3,"","d_kW2[0]",device.mechanical,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkWONAN2)
    entry_dkWONAF2 = create_entry_focus(frame_rise,iIndex,iColumn_design+4,"","d_kW2[1]",device.mechanical,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dkWONAF2)
    entry_dTORONAF1 = create_entry_focus(frame_rise,iIndex,iColumn_design+5,"","dTORONAF[1]",device.cooling,bOutput = True,width=widthE)
    entries_cooling0.append(entry_dTORONAF1)
    iIndex+=1

    hide_controls(False,entries_cooling0)

    frame_results = tk.Frame(frame_mechanical_design)
    frame_results.place(x=600, y=10)
    iIndexCol21 = 0

    create_label(frame_results,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1
    create_label(frame_results,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1

    entries_coolingOut = []
    entry_dHth = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"C&C Rads Distance","dHth",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"m")
    entries_coolingOut.append(entry_dHth)
    iIndexCol21+=1
    
    entry_dARad = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Radiators area","dARad",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"m2")
    entries_coolingOut.append(entry_dARad)
    iIndexCol21+=1
    
    entry_dSpareTermBlocks = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Rads Area (Forced Air)","dSpareTermBlocks",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"m2")
    entries_coolingOut.append(entry_dSpareTermBlocks)
    iIndexCol21+=1

    entry_dRingTypeWirConn = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"(N_RAD/GPO_RAD)","dRingTypeWirConn",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dRingTypeWirConn)
    iIndexCol21+=1
    
    entry_dTransCoreGround = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Cover area","dTransCoreGround",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"m2")
    entries_coolingOut.append(entry_dTransCoreGround)
    iIndexCol21+=1
    
    entry_dInsulationNeutralPhase = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Tank area","dInsulationNeutralPhase",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"m2")
    entries_coolingOut.append(entry_dInsulationNeutralPhase)
    iIndexCol21+=1

    entry_iTankExtLarge = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Tank Exterior Large","iTankExtLarge",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"in")
    entries_coolingOut.append(entry_iTankExtLarge)
    iIndexCol21+=1

    entry_iTankExtWidth = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Tank Exterior Large","iTankExtWidth",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"in")
    entries_coolingOut.append(entry_iTankExtWidth)
    iIndexCol21+=1

    entry_iTankExtHeight = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"Tank Exterior Large","iTankExtHeight",device.mechanical,bOutput = True,width=widthE)
    create_label(frame_results,iIndexCol21,iColumn_design+1,"in")
    entries_coolingOut.append(entry_iTankExtHeight)
    iIndexCol21+=1

    entry_dX_A = create_entry_focus(frame_results,iIndexCol21,iColumn_design,"X/A","dX_A",device.mechanical,bOutput = True,width=widthE)#"TC&C ctr–radiator ht. dist. (0.15 ≤ X/A ≤ 0.39)"
    entries_coolingOut.append(entry_dX_A)
    iIndexCol21+=1
    
    iColumn_design += 2
    iIndexCol21 = 0
    create_label(frame_results,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1
    create_label(frame_results,iIndexCol21,iColumn_design,"")
    iIndexCol21+=1

    entry_tTemperatureRiseOut = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","tTemperatureRise[0]",device.cooling,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_tTemperatureRiseOut)
    entry_tTemperatureRise1Out = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","tTemperatureRise[1]",device.cooling,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_tTemperatureRise1Out)
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"")
    create_label(frame_results,iIndexCol21,iColumn_design+1,"Maximum")
    entry_tTemperatureRiseOut = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","t_CoolingType[0]",device.cooling,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_tTemperatureRiseOut)
    entry_tTemperatureRise1Out = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","t_CoolingType[1]",device.cooling,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_tTemperatureRise1Out)
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"Winding")
    create_label(frame_results,iIndexCol21,iColumn_design+2,"LV")
    create_label(frame_results,iIndexCol21,iColumn_design+3,"HV")  
    create_label(frame_results,iIndexCol21,iColumn_design+4,"LV")
    create_label(frame_results,iIndexCol21,iColumn_design+5,"HV")   
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"Corrected Grad")  
    entry_dCorGradLV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","dCorGradLV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dCorGradLV00)
    entry_dCorGradHV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+3,"","dCorGradHV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dCorGradHV00)
    entry_dCorGradLV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","dCorGradLV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dCorGradLV01)
    entry_dCorGradHV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+5,"","dCorGradHV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dCorGradHV01)
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"Hottest Spot Rise")  
    entry_dHSRLMax = create_entry_focus(frame_results,iIndexCol21,iColumn_design+1,"","dHSRMAX",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dHSRLMax)
    entry_dHSRLV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","dHSRLV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dHSRLV00)
    entry_dHSRHV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+3,"","dHSRHV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dHSRHV00)
    entry_dHSRLV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","dHSRLV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dHSRLV01)
    entry_dHSRHV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+5,"","dHSRHV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dHSRHV01)
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"Average Winding Rise")  
    entry_dAWRMAX = create_entry_focus(frame_results,iIndexCol21,iColumn_design+1,"","dAWRMAX",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dAWRMAX)
    entry_dAWRLV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","dAWRLV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dAWRLV00)
    entry_dAWRHV00 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+3,"","dAWRHV0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dAWRHV00)
    entry_dAWRLV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","dAWRLV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dAWRLV01)
    entry_dAWRHV01 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+5,"","dAWRHV0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dAWRHV01)
    iIndexCol21+=1

    create_label(frame_results,iIndexCol21,iColumn_design,"Top Oil Rise")
    entry_dTORMAX = create_entry_focus(frame_results,iIndexCol21,iColumn_design+1,"","dTORMAX",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dTORMAX)
    entry_dTORTOT0 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+2,"","dTORTOT0[0]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dTORTOT0)
    entry_dTORTOT1 = create_entry_focus(frame_results,iIndexCol21,iColumn_design+4,"","dTORTOT0[1]",device.mechanical,bOutput = True,width=widthE)
    entries_coolingOut.append(entry_dTORTOT1)
    iIndexCol21+=1
    hide_controls(False,entries_coolingOut)