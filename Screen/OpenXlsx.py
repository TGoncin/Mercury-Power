#OpenXlsx.py
from datetime import datetime,date
from openpyxl import load_workbook
from tkinter import Tk, filedialog
from enum import Enum
from Definitions.enums import eConnectionType,eArrayType,eLogical,eBIL,eTransformerFamily,eWindingSelection,eTapExtremes,eSequence,eChangerCapacity
from Definitions.enums import eMaterial,eLaminationType,eTypeConductor,eCoating,eDesignSpec,eCoolingType,eSingleCoolingType,eTemperatureRise,eWindingDesc2
from Definitions.enums import eStandardTemperatureRise,eStandardLightningImpulse,eStandardRIV,eStandardSoundLevel,eStandardPartialDischarge,eLeads,eBars,eModelChanger
import re
import os

def get_cell_value(field,sheet, cell_ref):
    """
    Retorna el valor de la celda si existe, de lo contrario retorna None.
    Convierte automáticamente fórmulas como '=TODAY()' a fecha de hoy.
    """

    value = field.get()  # o .default si lo tiene
    cell = sheet[cell_ref]

    if cell.value is None:
        return 

    if isinstance(value, int):
        try:
            field.set(int(cell.value))
        except:
            field.set(0)  # fallback
    elif isinstance(value, float):
        try:
            field.set(float(cell.value))
        except:
            field.set(0.0)
    elif isinstance(value, str):
        field.set(str(cell.value))
    elif isinstance(value, Enum):
        # Para enums, intenta asignar usando el nombre o valor
        enum_type = type(value)
        try:
            # primero intenta por valor
            field.set(enum_type(cell.value))
        except:
            try:
                # luego intenta por nombre si es string
                field.set(enum_type[cell.value])
            except:
                field.set(value)  # fallback al default
    else:
        field.set(cell.value)  # cualquier otro tipo

def extract_number(filename):
    # Tomar los primeros 10 caracteres para revisar la R
    first10 = filename[:10]

    # Buscar número después de R si existe en los primeros 10 caracteres
    match_R = re.search(r'R(\d+)', first10)
    if match_R:
        return int(match_R.group(1))
    
    # Si no se encontró R, buscar número después de -
    match_dash = re.search(r'-(\d+)', filename)
    if match_dash:
        return int(match_dash.group(1))
    
    # Si no hay coincidencia
    return None

def open_excel(device):
    # Hide the main tkinter window
    root = Tk()
    root.withdraw()

    # Open a file dialog to select an Excel file
    file_path = filedialog.askopenfilename(
        title="Select an Excel file",
        filetypes=[("Excel files", "*.xlsx *.xlsm *.xltx *.xltm")]
    )

    if file_path:
        try:
            print("Selected file:", file_path)

            # Load the selected Excel workbook
            wb = load_workbook(file_path, data_only=True)

            # iRev = extract_number(os.path.basename(file_path))
            # if iRev != None:
            #     device.fields["iRevision"].set(iRev)

            sheet = wb.active  # You can use wb["SheetName"] if you know the name

            pWindH = device.getHVWind()
            pWindL = device.getLVWind()

            if (sheet["O80"].value=="GENERAL" or sheet["O80"].value=="General") and (sheet["O95"].value=="GENERAL SPECS" or sheet["O95"].value=="General Specs"):
                single_phase(sheet,device,pWindH,pWindL)
            elif (sheet["O89"].value=="GENERAL" or sheet["O89"].value=="General") and (sheet["O104"].value=="General Specs" or sheet["O104"].value=="GENERAL SPECS"):
                three_phases(sheet,device,pWindH,pWindL)
        
            device.calculate_device()
            device.calculate_device()
            return True
        except Exception as e:
            return False
        
def single_phase(sheet,device,pWindH,pWindL):
    iNoPhases = 1
    device.fields["iNoPhases"].set(iNoPhases)
    sArray = sheet["E81"].value#winding array
    if sArray=="H - L" or sArray=="H-L":
        device.fields["array_type"].set(eArrayType.H_L)
    elif sArray=="L - H" or sArray=="L-H":
        device.fields["array_type"].set(eArrayType.L_H)
    elif sArray=="L - H - L" or sArray=="L-H-L":
        device.fields["array_type"].set(eArrayType.L_H_L)
    elif sArray=="L - L - H" or sArray=="L-L-H":
        device.fields["array_type"].set(eArrayType.L_L_H)
    # Read specific cell values
    #General 
    cell = sheet["P81"].value
    def normalize_value(value):
        value = str(value).strip()  # ensure it's a string and remove leading/trailing spaces
        parts = value.split("-", 1)  # split into two parts using the first dash

        if len(parts) != 2:
        # No dash found, return the value as is
            return value

        # Determine which part is numeric and which is the code
        if parts[0].isdigit() and parts[1].startswith("Q"):
            # Case "1-Q12345" → invert to correct format
            return f"{parts[1]}-{parts[0]}"
        else:
            # Already in correct format "Q12345-1" or similar
            return value

    sVal = normalize_value(cell)
    device.fields["so_number"].set(sVal)
    #get_cell_value(device.fields["so_number"],sheet,"P81") #SO #
    get_cell_value(device.fields["customer_name"],sheet,"P82")#Customer
    get_cell_value(device.fields["customer_code"],sheet,"P83")#Custumer Code #
    get_cell_value(device.fields["quantity"],sheet,"P84")#Quantity 
    get_cell_value(device.guarantee.fields["dEvaluation_nl"],sheet,"P85")#Evaluation - NL
    get_cell_value(device.guarantee.fields["dEvaluation_ll"],sheet,"P86")#$/Watt          - LL
    get_cell_value(device.fields["issued_by"],sheet,"P87")#Issued By
    cell = sheet["P88"]#Original Issue Date
    if cell.value == "=TODAY()":
        dt = date.today()
    else:
        if isinstance(cell.value, (datetime, date)):
            dt = cell.value if isinstance(cell.value, date) else cell.value.date()
        else:
            dt = datetime.strptime(cell.value, "%Y-%m-%d").date()

    formatted = dt.strftime("%m/%d/%Y")
    device.fields["original_issue_date"].set(formatted)
    get_cell_value(device.guarantee.fields["sCustomerSpec"],sheet,"P89")#Customer Spec
    get_cell_value(device.guarantee.fields["tDesignSpec"],sheet,"P90")#Design Spec
    if sheet["P92"].value=="YES" or sheet["P92"].value=="Yes":#NL corrected to 85C
        device.guarantee.fields["bNLCorrected_to_85C"].set(eLogical.TRUE)
    get_cell_value(device.guarantee.fields["dNL_pu"],sheet,"P93")#NL @ 1.05 or 1.00 pu

    #General Specs
    dkVAE = sheet["P96"].value #Nominal kVA
    pWindH.fields["d_kVA"].set(dkVAE,0)      
    pWindL.fields["d_kVA"].set(dkVAE,0)
    device.fields["nominal_kva"].set(dkVAE)

    if sheet["P97"].value=="ONAN":#ONAN or ONAN/ONAF ?
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN) 
    elif sheet["P97"].value=="ONAN/ONAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN_ONAF) 
    elif sheet["P97"].value=="ONAN/ONAF/ONAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN_ONAF_ONAF) 
    elif sheet["P97"].value=="KNAN":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN) 
    elif sheet["P97"].value=="KNAN/KNAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN_KNAF)
    elif sheet["P97"].value=="KNAN/KNAF/KNAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN_KNAF_KNAF)

    get_cell_value(device.fields["frequency"],sheet,"P98") #Frequency
    if sheet["P99"].value==65 or sheet["P99"].value=="65":#Temperature rise - C
        device.cooling.fields["tTemperatureRises"].set(eTemperatureRise.Temp_65)  
    elif sheet["P99"].value==55 or sheet["P99"].value=="55":#Temperature rise - C
        device.cooling.fields["tTemperatureRises"].set(eTemperatureRise.Temp_55)  

    if sheet["P100"].value=="MPM" or sheet["P100"].value=="Mini Pad Mount":#Type (MPM, DPM, Pdmt)
        device.fields["transformer_family"].set(eTransformerFamily.MPM)  
    elif sheet["P100"].value=="PM" or sheet["P100"].value=="Pole Mounted" or sheet["P100"].value=="Polemount":
        device.fields["transformer_family"].set(eTransformerFamily.PM)  
    elif sheet["P100"].value=="PLMT" or sheet["P100"].value=="Platform Mount":
        device.fields["transformer_family"].set(eTransformerFamily.PLMT)  
    elif sheet["P100"].value=="PMT" or sheet["P100"].value=="Pad-Mounted":
        device.fields["transformer_family"].set(eTransformerFamily.PMT) 
    elif sheet["P100"].value=="DPM" or sheet["P100"].value=="Dead-Front Pad-Mounted":
        device.fields["transformer_family"].set(eTransformerFamily.DPM) 
    elif sheet["P100"].value=="OWP" or sheet["P100"].value=="Outdoor Weatherproof":
        device.fields["transformer_family"].set(eTransformerFamily.OWP)
    elif sheet["P100"].value=="SP" or sheet["P100"].value=="Submersible PMT":
        device.fields["transformer_family"].set(eTransformerFamily.SP)
    elif sheet["P100"].value=="LPMT":
        device.fields["transformer_family"].set(eTransformerFamily.LPMT)
    get_cell_value(device.fields["winding_description"],sheet,"P101")#Winding Description

    #Taps
    iNumberPositions = sheet["P107"].value
    iPositionsAboveRated = sheet["P106"].value
    if iNumberPositions>0 and iPositionsAboveRated>0:
        iPasos = (iNumberPositions-1)/iPositionsAboveRated 
    if sheet["P104"].value=="Yes" or sheet["P104"].value=="YES": #Taps ? (Yes/No)   
        pWindH.fields["bHaveTaps"].set(eLogical.TRUE)
    if pWindH.have_taps():
        pWindH.changer.fields["dPerUp"].set(iPasos*sheet["P105"].value)   #% per step up
        pWindH.changer.fields["dPerDown"].set(iPasos*sheet["P105"].value)   #% per step down
        pWindH.changer.fields["iNumSteps"].set(iNumberPositions-1)   #Number of Positions 
        #get_cell_value(pWindH.changer.fields["sType"],sheet,"P108")#Type
        get_cell_value(pWindH.changer.fields["tCapacity"],sheet,"P109")#Capacity  
        get_cell_value(pWindH.changer.fields["sInstructions"],sheet,"P110")#Instructions To Winding  

        # sPorcentagsTaps = sheet["P108"].value
        # matches = re.findall(r"[+-]?\d+\.?\d*", sPorcentagsTaps)
        # numbers = [float(x) for x in matches]

    #Voltages, Connections, BIL
    get_cell_value(pWindH.fields["dVoltage"],sheet,"P113")#High Voltage Ø
    dConE = sheet["P114"].value #HV Conn (Wye = 1, ∆ = 1.73)
    if dConE==1.73:
        pWindH.fields["type_connection"].set(eConnectionType.DELTA) 
    else: 
        pWindH.fields["type_connection"].set(eConnectionType.WYE) 
    if sheet["P115"].value==30 or sheet["P115"].value=="30":#HV BIL
        pWindH.fields["iBIL"].set(eBIL.BIL_30) 
    elif sheet["P115"].value==45 or sheet["P115"].value=="45":#HV BIL
        pWindH.fields["iBIL"].set(eBIL.BIL_45)     
    elif sheet["P115"].value==60 or sheet["P115"].value=="60":
        pWindH.fields["iBIL"].set(eBIL.BIL_60)
    elif sheet["P115"].value==75 or sheet["P115"].value=="75":
        pWindH.fields["iBIL"].set(eBIL.BIL_75)
    elif sheet["P115"].value==95 or sheet["P115"].value=="95":
        pWindH.fields["iBIL"].set(eBIL.BIL_95)
    elif sheet["P115"].value==110 or sheet["P115"].value=="110":
        pWindH.fields["iBIL"].set(eBIL.BIL_110)
    elif sheet["P115"].value==125 or sheet["P115"].value=="125":
        pWindH.fields["iBIL"].set(eBIL.BIL_125)
    elif sheet["P115"].value==150 or sheet["P115"].value=="150":
        pWindH.fields["iBIL"].set(eBIL.BIL_150)
    elif sheet["P115"].value==170 or sheet["P115"].value=="170":
        pWindH.fields["iBIL"].set(eBIL.BIL_170)
    elif sheet["P115"].value==200 or sheet["P115"].value=="200":
        pWindH.fields["iBIL"].set(eBIL.BIL_200)
    elif sheet["P115"].value==250 or sheet["P115"].value=="250":
        pWindH.fields["iBIL"].set(eBIL.BIL_250)
    else:
        pWindH.fields["iBIL"].set(eBIL.BIL_30)

    get_cell_value(pWindH.fields["ground_Y"],sheet,"P116")#If Wye Conn, GrdY (Yes/No)
    get_cell_value(pWindL.fields["dVoltage"],sheet,"P118")#Low Voltage Ø
    dConE = sheet["P119"].value #LV Conn (Wye = 1, ∆ = 1.73)
    if dConE==1.73:
        pWindL.fields["type_connection"].set(eConnectionType.DELTA) 
    else: 
        pWindL.fields["type_connection"].set(eConnectionType.WYE) 
    if sheet["P120"].value==30 or sheet["P120"].value=="30":#LV BIL
        pWindL.fields["iBIL"].set(eBIL.BIL_30) 
    elif sheet["P120"].value==45 or sheet["P120"].value=="45":#LV BIL
        pWindL.fields["iBIL"].set(eBIL.BIL_45) 
    elif sheet["P120"].value==60 or sheet["P120"].value=="60":
        pWindL.fields["iBIL"].set(eBIL.BIL_60)
    elif sheet["P120"].value==75 or sheet["P120"].value=="75":
        pWindL.fields["iBIL"].set(eBIL.BIL_75)
    elif sheet["P120"].value==95 or sheet["P120"].value=="95":
        pWindL.fields["iBIL"].set(eBIL.BIL_95)
    elif sheet["P120"].value==110 or sheet["P120"].value=="110":
        pWindL.fields["iBIL"].set(eBIL.BIL_110)
    elif sheet["P120"].value==125 or sheet["P120"].value=="125":
        pWindL.fields["iBIL"].set(eBIL.BIL_125)
    elif sheet["P120"].value==150 or sheet["P120"].value=="150":
        pWindL.fields["iBIL"].set(eBIL.BIL_150)
    elif sheet["P120"].value==170 or sheet["P120"].value=="170":
        pWindL.fields["iBIL"].set(eBIL.BIL_170)
    elif sheet["P120"].value==200 or sheet["P120"].value=="200":
        pWindL.fields["iBIL"].set(eBIL.BIL_200)
    elif sheet["P120"].value==250 or sheet["P120"].value=="250":
        pWindL.fields["iBIL"].set(eBIL.BIL_250)
    else:
        pWindL.fields["iBIL"].set(eBIL.BIL_30)
    get_cell_value(pWindL.fields["iNumBushings"],sheet,"P121")#Number of LV Bushings

        #LV Leads & Bus
        
    get_cell_value(pWindL.fields["iQuantityLeads"],sheet,"P123")#Coil leads - Quantity
    #get_cell_value(pWindL.fields["dThicknessLeads"],sheet,"P124")#Thickness
    #get_cell_value(pWindL.fields["dWidthLeads"],sheet,"P125")#- Width
    if sheet["P126"].value=="AL" or sheet["P126"].value=="Al": #Opt. LV Coil Ld Mat (Cu/Al)
       pWindL.fields["tMaterialLeads"].set(eMaterial.AL)
       if sheet["P125"].value==2.0 and sheet["P124"].value==0.047:
           pWindL.fields["tTypeLeads"].set(eLeads.AL047X2)
       elif sheet["P125"].value==2.0 and sheet["P124"].value==0.090:
           pWindL.fields["tTypeLeads"].set(eLeads.AL090X2)
       elif sheet["P125"].value==3.0 and sheet["P124"].value==0.063:
           pWindL.fields["tTypeLeads"].set(eLeads.AL063X3)
       elif sheet["P125"].value==4.0 and sheet["P124"].value==0.0787:
           pWindL.fields["tTypeLeads"].set(eLeads.AL079X4)
            
    else:
       pWindL.fields["tMaterialLeads"].set(eMaterial.CU)
       if sheet["P125"].value==1.0 and sheet["P124"].value==0.010:
           pWindL.fields["tTypeLeads"].set(eLeads.CU011)
       elif sheet["P125"].value==1.0 and sheet["P124"].value==0.093:
           pWindL.fields["tTypeLeads"].set(eLeads.CU0931)
       elif sheet["P125"].value==2.0 and sheet["P124"].value==0.050:
           pWindL.fields["tTypeLeads"].set(eLeads.CU052)
       elif sheet["P125"].value==4.0 and sheet["P124"].value==0.050:
           pWindL.fields["tTypeLeads"].set(eLeads.CU050x4S)   

    get_cell_value(device.fields["iQuantityBuswork"],sheet,"P127")#Buswork - Quantity
    #get_cell_value(device.fields["dThicknessBuswork"],sheet,"P128")#Thickness
    #get_cell_value(device.fields["dWidthBuswork"],sheet,"P129")#- Width
    if sheet["P130"].value=="AL" or sheet["P130"].value=="Al": #Opt. LV Bus Mat. (Cu/Al)
        device.fields["tMaterialBuswork"].set(eMaterial.AL)
        if sheet["P129"].value==2.0 and sheet["P128"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE2)
        elif sheet["P129"].value==3.0 and sheet["P128"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE3)
        elif sheet["P129"].value==4.0 and sheet["P128"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE4)
        elif sheet["P129"].value==2.0 and sheet["P128"].value==0.047:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS047X2)
        elif sheet["P129"].value==2.0 and sheet["P128"].value==0.090:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS090X2)
        elif sheet["P129"].value==3.0 and sheet["P128"].value==0.063:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS063X3)
        elif sheet["P129"].value==4.0 and sheet["P128"].value==0.0787:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS079X4)
    else:
        device.fields["tMaterialBuswork"].set(eMaterial.CU)
        if sheet["P129"].value==1.5 and sheet["P128"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU15)
        elif sheet["P129"].value==2.0 and sheet["P128"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU2)
        elif sheet["P129"].value==3.0 and sheet["P128"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU3)
        elif sheet["P129"].value==4.0 and sheet["P128"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU4)
        elif sheet["P129"].value==1.0 and sheet["P128"].value==0.010:
            device.fields["tTypeBuswork"].set(eBars.CULEADS011)
        elif sheet["P129"].value==1.0 and sheet["P128"].value==0.093:
            device.fields["tTypeBuswork"].set(eBars.CULEADS0931)
        elif sheet["P129"].value==2.0 and sheet["P128"].value==0.050:
            device.fields["tTypeBuswork"].set(eBars.CULEADS052)
        elif sheet["P129"].value==4.0 and sheet["P128"].value==0.050:
            device.fields["tTypeBuswork"].set(eBars.CULEADS050x4S)
    #device.fields["dThicknessBuswork"].set(sheet["P131"].value) #2" flex leads - No. high or '0'
    #device.fields["dWidthBuswork"].set(sheet["P132"].value) #- No. wide
    #device.fields["tTypeBuswork"].set(sheet["P133"].value) #Inches / phase

    #Core
    if sheet["P136"].value=="MOH":#Coil leads - Quantity
        device.core.fields["tLaminationType"].set(eLaminationType.M0H) 
    elif sheet["P136"].value=="M4":
        device.core.fields["tLaminationType"].set(eLaminationType.M4) 
    elif sheet["P136"].value=="M3":
        device.core.fields["tLaminationType"].set(eLaminationType.M3) 
    elif sheet["P136"].value=="M2":
        device.core.fields["tLaminationType"].set(eLaminationType.M2) 
    elif sheet["P136"].value=="Am":
        device.core.fields["tLaminationType"].set(eLaminationType.Am) 
    elif sheet["P136"].value=="Am HB1":
        device.core.fields["tLaminationType"].set(eLaminationType.AmHB1) 

    get_cell_value(device.core.fields["dTotalStack"],sheet,"P137")#Total stack
    get_cell_value(device.core.fields["dSteelWidth"],sheet,"P138")#Steel width,if Am5.6,6.7,8.4
    get_cell_value(device.core.fields["dPerOvervoltageMaxFlux"],sheet,"P139")#% Overvoltage re Max Flux
    get_cell_value(device.core.fields["dTestInductionLevel"],sheet,"P140")#Test Induction Level
    get_cell_value(device.core.fields["dMaxWattsPound"],sheet,"P141")#MAX watts / pound  ( w/ lb)
    get_cell_value(device.core.fields["dLossFact"],sheet,"P142")#Core Loss Fact (%) (Zero for Am)

    #LV winding
    get_cell_value(pWindL.fields["dTurnsNom"],sheet,"P145")#Turns
    get_cell_value(pWindL.fields["dNomTurnsUI"],sheet,"P145")#Turns
    get_cell_value(pWindL.coils[0].fields["iLayers"],sheet,"P146")#No. Lyrs (Wire wound LV)
    if sheet["P147"].value==1 or sheet["P147"].value=="1":#1-Strip, 2-magnet
       pWindL.coils[0].conductor[0].fields["iType"].set(eTypeConductor.STRIP) 
    else:
       pWindL.coils[0].conductor[0].fields["iType"].set(eTypeConductor.MAGNET)

    get_cell_value(pWindL.coils[0].conductor[0].fields["dWidth"],sheet,"P149")
    pWindL.coils[0].conductor[0].calculate_tables(pWindL,eWindingDesc2.LV.value)

    sCond = sheet["C109"].value
    tTypeCond  = eMaterial.AL if sheet["D108"].value=="Aluminum" else eMaterial.CU
    iNoCondE = pWindL.coils[0].conductor[0].lookNoConductor(tTypeCond,iNoPhases,sCond,eWindingDesc2.LV.value)
    pWindL.coils[0].conductor[0].fields["iNoConductor"].set(iNoCondE)
    pWindL.coils[0].conductor[0].calculate_Material()
    #Strip Width ( If Strip Cond. ) 
    get_cell_value(pWindL.coils[0].conductor[0].fields["iNoAxial"],sheet,"P150")#No axial strands (Wide)
    get_cell_value(pWindL.coils[0].conductor[0].fields["iNoRadial"],sheet,"P151")#No radial strands (High)
    get_cell_value(pWindL.fields["dLeadBuild"],sheet,"P155")#No radial strands (High)
    get_cell_value(pWindL.fields["iNoDuctInner"],sheet,"P156")#No. of ducts inner
    get_cell_value(pWindL.fields["iNoDuctOuter"],sheet,"P157")#No. of ducts outer
    get_cell_value(pWindL.fields["dDuctSize"],sheet,"P158")#Duct Size
    if sheet["P159"].value=="Enamel":#Coating for rectangular (Enamel/Paper)?
        pWindL.fields["tCoating"].set(eCoating.ENAMEL) 
    elif sheet["P159"].value=="Paper":
        pWindL.fields["tCoating"].set(eCoating.PAPER) 
    # else:
    #     pWindL.fields["tCoating"].set(eCoating.NONE) 

    #HV winding
    if sheet["P162"].value=="Yes": #Force Constant Stayback?
        pWindH.coils[0].fields["bConstantStayback"].set(eLogical.TRUE)
    else:
        pWindH.coils[0].fields["bConstantStayback"].set(eLogical.FALSE)

    pWindH.coils[0].conductor[0].calculate_tables(pWindH,eWindingDesc2.HV.value)

    sCond = sheet["C113"].value
    tTypeCond  = eMaterial.AL if sheet["D112"].value=="Aluminum" else eMaterial.CU
    iNoCondE = pWindH.coils[0].conductor[0].lookNoConductor(tTypeCond,iNoPhases,sCond,eWindingDesc2.HV.value)
    pWindH.coils[0].conductor[0].fields["iNoConductor"].set(iNoCondE)
    pWindH.coils[0].conductor[0].calculate_Material()
    get_cell_value(pWindH.coils[0].conductor[0].fields["iNoAxial"],sheet,"P164")#No axial strands (Wide)
    get_cell_value(pWindH.coils[0].conductor[0].fields["iNoRadial"],sheet,"P165")#No radial strands (High)
    get_cell_value(pWindH.fields["dLeadBuild"],sheet,"P170")#No axial strands (Wide)
    get_cell_value(pWindH.fields["iNoDuctInner"],sheet,"P171")#No radial strands (High)

    #Revision data
    device.fields["data_description"].set(sheet["S81"].value,0) 
    device.fields["data_description"].set(sheet["S82"].value,1) 
    device.fields["data_description"].set(sheet["S83"].value,2) 
    device.fields["data_description"].set(sheet["S84"].value,3) 
    device.fields["data_description"].set(sheet["S85"].value,4) 

    #Guarantees
    get_cell_value(device.guarantee.fields["dNLGuarantee"],sheet,"S88")#NL Guarantee
    get_cell_value(device.guarantee.fields["dLLGuarantee"],sheet,"S89")#LL Guarantee
    #IZ Guarantee
    cell1 = sheet["S90"].value
    match = re.search(r'(\d+(\.\d+)?)\s*%', cell1)
    if match:
        numero = float(match.group(1))
  
        device.guarantee.fields["dIZGuarantee"].set(numero)
    get_cell_value(device.guarantee.fields["dExcGuarantee"],sheet,"S91")#Excitation Guarantee

    #General Optionals & Comments
    get_cell_value(device.fields["sConnectionDiagramsItems"],sheet,"S93")#Connection Diagram Items
    get_cell_value(device.fields["dOverallRBFactor"],sheet,"S94")#Opt Overall RB factor (%)
    get_cell_value(device.fields["dAltitude"],sheet,"S95")#Opt. Altitude (Std <=1000 m)
    get_cell_value(device.fields["sWindingSpecComments"],sheet,"S96")#Opt. Winding Spec Comments
    get_cell_value(device.core.fields["sSheetComments"],sheet,"S97")#Opt. Core Sheet Comments
    get_cell_value(device.fields["sPurchasingComments"],sheet,"S98")#Opt. Purchasing Comments
    get_cell_value(device.fields["sInsulationComments"],sheet,"S99")#Opt. Insulation Comments
    get_cell_value(device.fields["dONANLosses"],sheet,"S100")#Opt. ONAN Losses
    get_cell_value(device.fields["dONANTOR"],sheet,"S101")#Opt. ONAN TOR°C

    #Optional Core Data
    get_cell_value(device.core.fields["dWindingHeightAdder"],sheet,"S103")#Opt Win Height Adder
    get_cell_value(device.core.fields["dWindingWidthAdder"],sheet,"S104")#Opt Win Width Adder
    get_cell_value(device.core.fields["dCoilCorePBEnd"],sheet,"S105")#Opt Coil to Core PB / End
    get_cell_value(device.core.fields["dCoilCoilPBSides"],sheet,"S106")#Coil to Coil PB Sides
    get_cell_value(device.core.fields["dCoilCorePBSides"],sheet,"S107")#Coil to Core PB Sides
    get_cell_value(device.core.fields["dFixedSmWindowWidth"],sheet,"S108")#Fixed Sm Window Width
    get_cell_value(device.core.fields["dFixedWindowHeight"],sheet,"S109")#Fixed Window Height

    #Optional LV Data
    get_cell_value(pWindL.fields["sDuctPlacement"],sheet,"S112")#Opt LV Duct placement
    get_cell_value(pWindL.fields["dLayerInsulation"],sheet,"S113")#Opt LV Layer insulation 
    get_cell_value(pWindL.fields["dOptStayback"],sheet,"S114")#Opt. LV Stayback
    get_cell_value(pWindL.fields["dBuildFactor"],sheet,"S115")#Opt LV Build factor (%)
    if sheet["S116"].value=="No" or sheet["S116"].value=="NO":
        pWindL.fields["bEndfillerOpt"].set(eLogical.FALSE) #Opt. LV Endfiller ? (Yes/No)
    else:
        pWindL.fields["bEndfillerOpt"].set(eLogical.TRUE)

    #Optional HV Data
    get_cell_value(pWindH.fields["sDuctPlacement"],sheet,"S120")#Opt LV Duct placement
    get_cell_value(pWindH.fields["dOptStayback"],sheet,"S121")#Opt. LV Stayback
    get_cell_value(pWindH.fields["dLayerInsulation"],sheet,"S122")#Opt LV Layer insulation 
    get_cell_value(pWindH.fields["dEndInsulation"],sheet,"S123")#Optional HV End Insulation 
    get_cell_value(pWindH.fields["dLeadPadWitdh"],sheet,"S124")#Opt. HV Lead Pad Width
    get_cell_value(pWindH.fields["iLeadPads"],sheet,"S125")#Opt. # of HV lead pads
    #Opt BIL Under HV
    if sheet["S126"].value==30 or sheet["S126"].value=="30":#LV BIL
        pWindH.fields["iBILUnder"].set(eBIL.BIL_30) 
    elif sheet["S126"].value==45 or sheet["S126"].value=="45":#LV BIL
        pWindH.fields["iBILUnder"].set(eBIL.BIL_45)
    elif sheet["S126"].value==60 or sheet["S126"].value=="60":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_60)
    elif sheet["S126"].value==75 or sheet["S126"].value=="75":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_75)
    elif sheet["S126"].value==95 or sheet["S126"].value=="95":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_95)
    elif sheet["S126"].value==110 or sheet["S126"].value=="110":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_110)
    elif sheet["S126"].value==125 or sheet["S126"].value=="125":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_125)
    elif sheet["S126"].value==150 or sheet["S126"].value=="150":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_150)
    elif sheet["S126"].value==170 or sheet["S126"].value=="170":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_170)
    elif sheet["S126"].value==200 or sheet["S126"].value=="200":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_200)
    elif sheet["S126"].value==250 or sheet["S126"].value=="250":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_250)
    else:
        pWindH.fields["iBILUnder"].set(eBIL.BIL_0)
    get_cell_value(pWindH.fields["sHLUnder"],sheet,"S127")#Optional HL under
    get_cell_value(pWindH.fields["dHLUnderRB"],sheet,"S128")#Optional HL under (RB)
    get_cell_value(pWindH.fields["sHLOver"],sheet,"S129")#Optional HL over
    get_cell_value(pWindH.fields["dHLOverRB"],sheet,"S130")#Optional HL over (RB)
    get_cell_value(pWindH.fields["dBuildFactor"],sheet,"S131")#Optional HV build factor (%)
    if sheet["S132"].value=="Yes" or sheet["S132"].value=="YES":#Opt. HV Endfiller ? (Yes/No)
        pWindH.fields["bEndfillerOpt"].set(eLogical.TRUE) 
    else:
        pWindH.fields["bEndfillerOpt"].set(eLogical.FALSE) 
    get_cell_value(pWindH.fields["sPHTapLeads"],sheet,"S133")#Optional HV Ph. / Tap Leads

    #Routine Tests
    if sheet["S136"].value=="LV":
        device.tests.fields["tHipot"].set(eWindingSelection.LV) #Hipot (HV,LV,Both,None)
    elif sheet["S136"].value=="HV":
        device.tests.fields["tHipot"].set(eWindingSelection.HV) 
    elif sheet["S136"].value=="Both":
        device.tests.fields["tHipot"].set(eWindingSelection.BOTH) 
    else:
        device.tests.fields["tHipot"].set(eWindingSelection.NONE) 
    get_cell_value(device.tests.fields["dHipotHV"],sheet,"S137")#Hipot HV
    get_cell_value(device.tests.fields["dHipotLV"],sheet,"S138")#Hipot LV

    if sheet["S139"].value=="Yes":#Induced (Yes/No)
        device.tests.fields["bInduced"].set(eLogical.TRUE)
    else:
        device.tests.fields["bInduced"].set(eLogical.FALSE)
    device.tests.fields["dXRatedVoltage"].set(sheet["S140"].value) #X Rated Voltage

    if sheet["S141"].value=="Yes":#Resistance Test (Yes/No)
        device.tests.fields["bResistance"].set(eLogical.TRUE)
    else:
        device.tests.fields["bResistance"].set(eLogical.FALSE)
        
    if sheet["S142"].value=="Yes":#Other Resistance (Yes/No)
        device.tests.fields["bOtherResistance"].set(eLogical.TRUE)
    else:
        device.tests.fields["bOtherResistance"].set(eLogical.FALSE)

    sS143 = sheet["S143"].value
    if sS143=="E": #Tap Extremes OR All (E/A)
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.E)
    elif sS143=="A":
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.A)
    else:
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.NONE)
 
    get_cell_value(device.tests.fields["sSampleRate"],sheet,"S144")#Opt. Sample Rate (description)
    if sheet["S145"].value=="Yes":  #Core Loss Test? (Yes/No)
        device.tests.fields["bCoreLossTest"].set(eLogical.TRUE)
    else:
        device.tests.fields["bCoreLossTest"].set(eLogical.FALSE)
    get_cell_value(device.tests.fields["dExtraNoLoad"],sheet,"S146")#Opt. Extra No Load (% Nom.)
    if sheet["S147"].value=="Yes":  #Opt. Extra No Load (% Nom.)
        device.tests.fields["bLoadLossTest"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bLoadLossTest"].set(eLogical.FALSE) 

    if sheet["S148"].value=="Yes":  #Ld Loss @ "&100
        device.tests.fields["bLoadLossTap"].set(eLogical.TRUE)
    else:
        device.tests.fields["bLoadLossTap"].set(eLogical.FALSE)

    get_cell_value(device.tests.fields["d3LoadLosskVA"],sheet,"S149")#Opt. 3rd Load Loss % kVA
    get_cell_value(device.tests.fields["d3LoadLossTap"],sheet,"S150")#Opt. 3rd Load Loss % Tap 
    get_cell_value(device.tests.fields["d4LoadLosskVA"],sheet,"S151")#Opt. 4th Load Loss % kVA
    get_cell_value(device.tests.fields["d4LoadLossTap"],sheet,"S152")#Opt. 4th Load Loss % Tap 
    get_cell_value(device.tests.fields["sOtherRoutineComments"],sheet,"S153")#Other Routine Tests or Comm.

    #Type Tests
    if sheet["S156"].value=="Yes":  #Temperature Rise ? (Yes/No)
        device.tests.fields["bTemperatureRise"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bTemperatureRise"].set(eLogical.FALSE) 
    sSpecification = sheet["S157"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.IEEEC571290)
    elif sSpecification=="IEC 60076-2":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.IEC600762)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.CSAC88)

    get_cell_value(device.tests.fields["sCommentsTR"],sheet,"S158")#Comments:
    if sheet["S159"].value=="Yes":  #Lightning Impulse ? (Yes/No)
        device.tests.fields["bLightningImpulse"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bLightningImpulse"].set(eLogical.FALSE) 
    sSpecification = sheet["S160"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.IEEEC571290)
    elif sSpecification=="IEC 60076-3":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.IEC600763)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.CSAC88)
    sSequence = sheet["S161"].value
    if sSequence=="1R":#Sequence (1R, 2C, 1F)
        device.tests.fields["tSequenceLI"].set(eSequence.R1) 
    elif sSequence=="2C":
        device.tests.fields["tSequenceLI"].set(eSequence.C2) 
    elif sSequence=="1F":
        device.tests.fields["tSequenceLI"].set(eSequence.F1) 
    elif sSequence=="1R, 2C":
        device.tests.fields["tSequenceLI"].set(eSequence.R1C2) 
    elif sSequence=="1R, 1F":
        device.tests.fields["tSequenceLI"].set(eSequence.R1F1)
    elif sSequence=="2C, 1F":
        device.tests.fields["tSequenceLI"].set(eSequence.C2F1) 
    elif sSequence=="1R, 2C, 1F":
        device.tests.fields["tSequenceLI"].set(eSequence.R1C2F1) 
    else:
        device.tests.fields["tSequenceLI"].set(eSequence.NONE) 

    if sheet["S162"].value=="HV":   #Impulse (HV,LV,Both,None)
        device.tests.fields["tImpulse"].set(eWindingSelection.HV) 
    elif sheet["S162"].value=="LV":  
        device.tests.fields["tImpulse"].set(eWindingSelection.LV) 
    elif sheet["S162"].value=="Both":  
        device.tests.fields["tImpulse"].set(eWindingSelection.BOTH) 
    else:
        device.tests.fields["tImpulse"].set(eWindingSelection.NONE)
    get_cell_value(device.tests.fields["sCommentsLI"],sheet,"S163")#Comments:
    if sheet["S164"].value=="Yes":  #RIV ? (Yes/No)
        device.tests.fields["bRIV"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bRIV"].set(eLogical.FALSE) 
    sSpecification = sheet["S165"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.IEEEC571290)
    elif sSpecification=="CISPR 18-2":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.CISPR182)
    elif sSpecification=="IEC 60437":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.IEC60437)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.CSAC88)
    get_cell_value(device.tests.fields["sCommentsRIV"],sheet,"S166")#Comments:
    if sheet["S167"].value=="Yes":  #Sound Level ? (Yes/No)
        device.tests.fields["bSoundLevel"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bSoundLevel"].set(eLogical.FALSE) 
    sSpecification = sheet["S168"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEEEC571290)
    elif sSpecification=="IEE C57.12.91":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEEC571291)
    elif sSpecification=="IEC 60076-10":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEC6007610)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.CSAC88)
    get_cell_value(device.tests.fields["sCommentsSL"],sheet,"S169")#Comments:
    if sheet["S170"].value!="":
        device.tests.fields["bOtherTypeTest"].set(eLogical.TRUE)
        get_cell_value(device.tests.fields["sOtherRoutineCommentsType"],sheet,"S170")#Other Type Tests or Comm.

    texto = str(sheet["F189"].value)
    resultado = "per k4" in texto.lower()
    if resultado:
        device.cooling.fields["tK4Factor"].set(eLogical.TRUE)

    #Optional Form Data
    get_cell_value(device.fields["sFormMaterial"],sheet,"S172")#Other Type Tests or Comm.
    get_cell_value(device.fields["dFormMaterialRB"],sheet,"S173")#Other Type Tests or Comm.

    get_cell_value(pWindL.coils[0].fields["iSectionsLeg"],sheet,"F91")#Sections / Leg
    get_cell_value(pWindH.coils[0].fields["iSectionsLeg"],sheet,"I91")#Sections / Leg

def three_phases(sheet,device,pWindH,pWindL):
    iNoPhases = 3
    device.fields["iNoPhases"].set(iNoPhases)
    sArrayOr = sheet["C81"].value#winding array
    match = re.search(r'Radial:\s*([^"]+)', sArrayOr)

    if match:
        sArray = match.group(1).strip()

    if sArray=="H - L" or sArray=="H-L" or sArray == "High - Low":
        device.fields["array_type"].set(eArrayType.H_L)
    elif sArray=="L - H" or sArray=="L-H" or sArray == "Low - High":
        device.fields["array_type"].set(eArrayType.L_H)
    elif sArray=="L - H - L" or sArray=="L-H-L" or sArray == "Low - High - Low":
        device.fields["array_type"].set(eArrayType.L_H_L)
    elif sArray=="L - L - H" or sArray=="L-L-H" or sArray == "Low - Low - High":
        device.fields["array_type"].set(eArrayType.L_L_H)
    # Read specific cell values
    #General 
    cell = sheet["P90"].value
    def normalize_value(value):
        value = str(value).strip()  # ensure it's a string and remove leading/trailing spaces
        parts = value.split("-", 1)  # split into two parts using the first dash

        if len(parts) != 2:
        # No dash found, return the value as is
            return value

        # Determine which part is numeric and which is the code
        if parts[0].isdigit() and parts[1].startswith("Q"):
            # Case "1-Q12345" → invert to correct format
            return f"{parts[1]}-{parts[0]}"
        else:
            # Already in correct format "Q12345-1" or similar
            return value

    sVal = normalize_value(cell)
    device.fields["so_number"].set(sVal)

    get_cell_value(device.fields["customer_name"],sheet,"P91")#Customer
    get_cell_value(device.fields["customer_code"],sheet,"P92")#Custumer Code #
    #get_cell_value(device.fields["quantity"],sheet,"P84")#Quantity 
    get_cell_value(device.guarantee.fields["dEvaluation_nl"],sheet,"P93")#Evaluation - NL
    get_cell_value(device.guarantee.fields["dEvaluation_ll"],sheet,"P94")#$/Watt          - LL
    get_cell_value(device.fields["issued_by"],sheet,"P95")#Issued By
    cell = sheet["P96"]#Original Issue Date
    if cell.value == "=TODAY()":
        dt = date.today()
    else:
        if isinstance(cell.value, (datetime, date)):
            dt = cell.value if isinstance(cell.value, date) else cell.value.date()
        else:
            dt = datetime.strptime(cell.value, "%Y-%m-%d").date()

    formatted = dt.strftime("%m/%d/%Y")
    device.fields["original_issue_date"].set(formatted)
    get_cell_value(device.guarantee.fields["sCustomerSpec"],sheet,"P97")#Customer Spec
    get_cell_value(device.guarantee.fields["tDesignSpec"],sheet,"P98")#Design Spec
    get_cell_value(device.guarantee.fields["tEfficiencySpec"],sheet,"P99")
    if sheet["P100"].value=="YES" or sheet["P100"].value=="Yes":#NL corrected to 85C
        device.guarantee.fields["bNLCorrected_to_85C"].set(eLogical.TRUE)
    get_cell_value(device.guarantee.fields["dNL_pu"],sheet,"P101")#NL @ 1.05 or 1.00 pu
    get_cell_value(device.guarantee.fields["dGroundShortTime"],sheet,"P102")#If for Grounding, short time duration (sec.)
    get_cell_value(device.guarantee.fields["dVirtualZ"],sheet,"P103")#Virtual %Z, for Amp @ Neutral, if not limited by %Z

    #General Specs
    dkVAE = sheet["P105"].value #Nominal kVA
    pWindH.fields["d_kVA"].set(dkVAE,0)      
    pWindL.fields["d_kVA"].set(dkVAE,0)
    device.fields["nominal_kva"].set(dkVAE)

    if sheet["P106"].value=="ONAN":#ONAN or ONAN/ONAF ?
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN) 
    elif sheet["P106"].value=="ONAN/ONAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN_ONAF) 
    elif sheet["P106"].value=="ONAN/ONAF/ONAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_ONAN_ONAF_ONAF) 
    elif sheet["P106"].value=="KNAN":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN) 
    elif sheet["P106"].value=="KNAN/KNAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN_KNAF)
    elif sheet["P106"].value=="KNAN/KNAF/KNAF":
        device.cooling.fields["tCoolingTypes"].set(eCoolingType.COOL_KNAN_KNAF_KNAF)
    
    dValue = sheet["P107"].value
    if dValue>0.0:
        device.fields["dPerSpcFor"].set(dValue)

    #P108 falta
    if sheet["P108"].value=="YES" or sheet["P108"].value=="Yes":
        device.cooling.fields["tProvONAF"].set(eLogical.TRUE)  
    else:
        device.cooling.fields["tProvONAF"].set(eLogical.FALSE)
    get_cell_value(device.fields["frequency"],sheet,"P109") #Frequency
    if sheet["P111"].value=="YES" or sheet["P111"].value=="Yes" or sheet["P111"].value=="yes":
        device.cooling.fields["tTemperatureRises"].set(eTemperatureRise.Temp_55_65)  
    else:
        if sheet["P110"].value==65 or sheet["P110"].value=="65":#Temperature rise - C
            device.cooling.fields["tTemperatureRises"].set(eTemperatureRise.Temp_65)  
        elif sheet["P110"].value==55 or sheet["P110"].value=="55":#Temperature rise - C
            device.cooling.fields["tTemperatureRises"].set(eTemperatureRise.Temp_55)  

    if sheet["P112"].value=="MPM" or sheet["P112"].value=="Mini Pad Mount":#Type (MPM, DPM, Pdmt)
        device.fields["transformer_family"].set(eTransformerFamily.MPM)  
    elif sheet["P112"].value=="PM" or sheet["P112"].value=="Pole Mounted" or sheet["P112"].value=="Polemount":
        device.fields["transformer_family"].set(eTransformerFamily.PM)  
    elif sheet["P112"].value=="PLMT" or sheet["P112"].value=="Platform Mount":
        device.fields["transformer_family"].set(eTransformerFamily.PLMT)  
    elif sheet["P112"].value=="PMT" or sheet["P112"].value=="Pad-Mounted":
        device.fields["transformer_family"].set(eTransformerFamily.PMT) 
    elif sheet["P112"].value=="DPM" or sheet["P112"].value=="Dead-Front Pad-Mounted":
        device.fields["transformer_family"].set(eTransformerFamily.DPM) 
    elif sheet["P112"].value=="OWP" or sheet["P112"].value=="Outdoor Weatherproof":
        device.fields["transformer_family"].set(eTransformerFamily.OWP)
    elif sheet["P112"].value=="SP" or sheet["P112"].value=="Submersible PMT":
        device.fields["transformer_family"].set(eTransformerFamily.SP)
    elif sheet["P112"].value=="LPMT":
        device.fields["transformer_family"].set(eTransformerFamily.LPMT)
    get_cell_value(device.fields["winding_description"],sheet,"P113")#Winding Description

    #Taps
    iNumberPositions = sheet["P118"].value
    iPositionsAboveRated = sheet["P117"].value
    if iNumberPositions>0 and iPositionsAboveRated>0:
        iPasos = (iNumberPositions-1)/iPositionsAboveRated 
    if sheet["P115"].value=="Yes" or sheet["P115"].value=="YES": #Taps ? (Yes/No)   
        pWindH.fields["bHaveTaps"].set(eLogical.TRUE)
    if pWindH.have_taps():
        pWindH.changer.fields["dPerUp"].set(iPasos*sheet["P116"].value)   #% per step up
        pWindH.changer.fields["dPerDown"].set(iPasos*sheet["P116"].value)   #% per step down
        pWindH.changer.fields["iNumSteps"].set(iNumberPositions-1)   #Number of Positions 
        #get_cell_value(pWindH.changer.fields["sType"],sheet,"P108")#Type
        get_cell_value(pWindH.changer.fields["tCapacity"],sheet,"P120")#Capacity  
        #get_cell_value(pWindH.changer.fields["sInstructions"],sheet,"P110")#Instructions To Winding  

        sPorcentagsTaps = sheet["P119"].value
        matches = re.findall(r"[+-]?\d+\.?\d*", sPorcentagsTaps)
        numbers = [float(x) for x in matches]

        if "±" in sPorcentagsTaps:
            # Encontrar todos los números después de ±
            plus_numbers = re.findall(r"±(\d+\.?\d*)", sPorcentagsTaps)
            for x in plus_numbers:
                numbers.append(-float(x))
                numbers.append(float(x))

        min_val = abs(min(numbers))
        max_val = abs(max(numbers))

        pWindH.changer.fields["dPerUp"].set(max_val)   
        pWindH.changer.fields["dPerDown"].set(min_val)

    #Voltages, Connections, BIL
    get_cell_value(pWindH.fields["dVoltage"],sheet,"P122")#High Voltage Ø
    dConE = sheet["P123"].value #HV Conn (Wye = 1, ∆ = 1.73)
    if dConE==1.73:
        pWindH.fields["type_connection"].set(eConnectionType.DELTA) 
    else: 
        pWindH.fields["type_connection"].set(eConnectionType.WYE) 
    if sheet["P124"].value==30 or sheet["P124"].value=="30":#HV BIL
        pWindH.fields["iBIL"].set(eBIL.BIL_30) 
    elif sheet["P124"].value==45 or sheet["P124"].value=="45":#HV BIL
        pWindH.fields["iBIL"].set(eBIL.BIL_45) 
    elif sheet["P124"].value==60 or sheet["PP124115"].value=="60":
        pWindH.fields["iBIL"].set(eBIL.BIL_60)
    elif sheet["P124"].value==75 or sheet["P124"].value=="75":
        pWindH.fields["iBIL"].set(eBIL.BIL_75)
    elif sheet["P124"].value==95 or sheet["P124"].value=="95":
        pWindH.fields["iBIL"].set(eBIL.BIL_95)
    elif sheet["P124"].value==110 or sheet["P124"].value=="110":
        pWindH.fields["iBIL"].set(eBIL.BIL_110)
    elif sheet["P124"].value==125 or sheet["P124"].value=="125":
        pWindH.fields["iBIL"].set(eBIL.BIL_125)
    elif sheet["P124"].value==150 or sheet["P124"].value=="150":
        pWindH.fields["iBIL"].set(eBIL.BIL_150)
    elif sheet["P124"].value==170 or sheet["P124"].value=="170":
        pWindH.fields["iBIL"].set(eBIL.BIL_170)
    elif sheet["P124"].value==200 or sheet["P124"].value=="200":
        pWindH.fields["iBIL"].set(eBIL.BIL_200)
    elif sheet["P124"].value==250 or sheet["P124"].value=="250":
        pWindH.fields["iBIL"].set(eBIL.BIL_250)
    else:
        pWindH.fields["iBIL"].set(eBIL.BIL_30)

    value = sheet["P125"].value
    if value=="Yes" or value=="yes" or value=="YES":
        pWindH.fields["ground_Y"].set(eLogical.TRUE)
    else:
        pWindH.fields["ground_Y"].set(eLogical.FALSE)
    #get_cell_value(pWindH.fields["ground_Y"],sheet,"P125")#If Wye Conn, GrdY (Yes/No)
    get_cell_value(pWindL.fields["dVoltage"],sheet,"P129")#Low Voltage Ø
    dConE = sheet["P130"].value #LV Conn (Wye = 1, ∆ = 1.73)
    if dConE==1.73:
        pWindL.fields["type_connection"].set(eConnectionType.DELTA) 
    else: 
        pWindL.fields["type_connection"].set(eConnectionType.WYE) 
    if sheet["P131"].value==30 or sheet["P131"].value=="30":#LV BIL
        pWindL.fields["iBIL"].set(eBIL.BIL_30) 
    elif sheet["P131"].value==45 or sheet["P131"].value=="45":#LV BIL
        pWindL.fields["iBIL"].set(eBIL.BIL_45) 
    elif sheet["P131"].value==60 or sheet["P131"].value=="60":
        pWindL.fields["iBIL"].set(eBIL.BIL_60)
    elif sheet["P131"].value==75 or sheet["P131"].value=="75":
        pWindL.fields["iBIL"].set(eBIL.BIL_75)
    elif sheet["P131"].value==95 or sheet["P131"].value=="95":
        pWindL.fields["iBIL"].set(eBIL.BIL_95)
    elif sheet["P131"].value==110 or sheet["P131"].value=="110":
        pWindL.fields["iBIL"].set(eBIL.BIL_110)
    elif sheet["P131"].value==125 or sheet["P131"].value=="125":
        pWindL.fields["iBIL"].set(eBIL.BIL_125)
    elif sheet["P131"].value==150 or sheet["P131"].value=="150":
        pWindL.fields["iBIL"].set(eBIL.BIL_150)
    elif sheet["P131"].value==170 or sheet["P131"].value=="170":
        pWindL.fields["iBIL"].set(eBIL.BIL_170)
    elif sheet["P131"].value==200 or sheet["P131"].value=="200":
        pWindL.fields["iBIL"].set(eBIL.BIL_200)
    elif sheet["P131"].value==250 or sheet["P131"].value=="250":
        pWindL.fields["iBIL"].set(eBIL.BIL_250)
    else:
        pWindL.fields["iBIL"].set(eBIL.BIL_30)
    #get_cell_value(pWindL.fields["iNumBushings"],sheet,"P121")#Number of LV Bushings

    #LV Leads & Bus  
    get_cell_value(pWindL.fields["iQuantityLeads"],sheet,"P133")#Coil leads - Quantity
    #get_cell_value(pWindL.fields["dThicknessLeads"],sheet,"P124")#Thickness
    #get_cell_value(pWindL.fields["dWidthLeads"],sheet,"P125")#- Width
    if sheet["P136"].value=="AL" or sheet["P136"].value=="Al": #Opt. LV Coil Ld Mat (Cu/Al)
       pWindL.fields["tMaterialLeads"].set(eMaterial.AL)
       if sheet["P135"].value==2.0 and sheet["P134"].value==0.047:
           pWindL.fields["tTypeLeads"].set(eLeads.AL047X2)
       elif sheet["P135"].value==2.0 and sheet["P134"].value==0.090:
           pWindL.fields["tTypeLeads"].set(eLeads.AL090X2)
       elif sheet["P135"].value==3.0 and sheet["P134"].value==0.063:
           pWindL.fields["tTypeLeads"].set(eLeads.AL063X3)
       elif sheet["P135"].value==4.0 and sheet["P134"].value==0.0787:
           pWindL.fields["tTypeLeads"].set(eLeads.AL079X4)
            
    else:
       pWindL.fields["tMaterialLeads"].set(eMaterial.CU)
       if sheet["P135"].value==1.0 and sheet["P134"].value==0.010:
           pWindL.fields["tTypeLeads"].set(eLeads.CU011)
       elif sheet["P135"].value==1.0 and sheet["P134"].value==0.093:
           pWindL.fields["tTypeLeads"].set(eLeads.CU0931)
       elif sheet["P135"].value==2.0 and sheet["P134"].value==0.050:
           pWindL.fields["tTypeLeads"].set(eLeads.CU052)
       elif sheet["P135"].value==4.0 and sheet["P134"].value==0.050:
           pWindL.fields["tTypeLeads"].set(eLeads.CU050x4S)   

    get_cell_value(device.fields["iQuantityBuswork"],sheet,"P137")#Buswork - Quantity
    #get_cell_value(device.fields["dThicknessBuswork"],sheet,"P128")#Thickness
    #get_cell_value(device.fields["dWidthBuswork"],sheet,"P129")#- Width
    if sheet["P140"].value=="AL" or sheet["P140"].value=="Al": #Opt. LV Bus Mat. (Cu/Al)
        device.fields["tMaterialBuswork"].set(eMaterial.AL)
        if sheet["P139"].value==2.0 and sheet["P138"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE2)
        elif sheet["P139"].value==3.0 and sheet["P138"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE3)
        elif sheet["P139"].value==4.0 and sheet["P138"].value==0.25:
           device.fields["tTypeBuswork"].set(eBars.ALFRE4)
        elif sheet["P139"].value==2.0 and sheet["P138"].value==0.047:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS047X2)
        elif sheet["P139"].value==2.0 and sheet["P138"].value==0.090:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS090X2)
        elif sheet["P139"].value==3.0 and sheet["P138"].value==0.063:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS063X3)
        elif sheet["P139"].value==4.0 and sheet["P138"].value==0.0787:
           device.fields["tTypeBuswork"].set(eBars.ALLEADS079X4)
    else:
        device.fields["tMaterialBuswork"].set(eMaterial.CU)
        if sheet["P139"].value==1.5 and sheet["P138"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU15)
        elif sheet["P139"].value==2.0 and sheet["P138"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU2)
        elif sheet["P139"].value==3.0 and sheet["P138"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU3)
        elif sheet["P139"].value==4.0 and sheet["P138"].value==0.25:
            device.fields["tTypeBuswork"].set(eBars.CU4)
        elif sheet["P139"].value==1.0 and sheet["P138"].value==0.010:
            device.fields["tTypeBuswork"].set(eBars.CULEADS011)
        elif sheet["P139"].value==1.0 and sheet["P138"].value==0.093:
            device.fields["tTypeBuswork"].set(eBars.CULEADS0931)
        elif sheet["P139"].value==2.0 and sheet["P138"].value==0.050:
            device.fields["tTypeBuswork"].set(eBars.CULEADS052)
        elif sheet["P139"].value==4.0 and sheet["P138"].value==0.050:
            device.fields["tTypeBuswork"].set(eBars.CULEADS050x4S)
    # if sheet["P141"].value!="" and sheet["P141"].value!=None:    
    #     device.cooling.fields["dMaxONAF"].set(sheet["P141"].value,1) #Max ONAF1 // LNAF1 Losses
    # if sheet["P142"].value!="" and sheet["P142"].value!=None:  
    #     device.cooling.fields["dTORONAF"].set(sheet["P142"].value,1) #TOR°C @ ONAF1 // LNAF1
    # #P143 falta
    # if sheet["P144"].value!="" and sheet["P144"].value!=None: 
    #     device.cooling.fields["dMaxONAF"].set(sheet["P144"].value,2) #Max ONAF2 // LNAF2 Losses
    # if sheet["P145"].value!="" and sheet["P145"].value!=None: 
    #     device.cooling.fields["dTORONAF"].set(sheet["P145"].value,2) #TOR°C @ ONAF2 // LNAF2       
    #P146 falta

    #Core
    if sheet["P148"].value=="YES" or sheet["P148"].value=="Yes" or sheet["P148"].value=="yes":#Isolated Core (Y/N)
       device.core.fields["tIsolatedCore"].set(eLogical.TRUE) 
    else:
        device.core.fields["tIsolatedCore"].set(eLogical.FALSE) 
    if sheet["P149"].value=="MOH":#Coil leads - Quantity
        device.core.fields["tLaminationType"].set(eLaminationType.M0H) 
    elif sheet["P149"].value=="M4":
        device.core.fields["tLaminationType"].set(eLaminationType.M4) 
    elif sheet["P149"].value=="M3":
        device.core.fields["tLaminationType"].set(eLaminationType.M3) 
    elif sheet["P149"].value=="M2":
        device.core.fields["tLaminationType"].set(eLaminationType.M2) 
    elif sheet["P149"].value=="Am":
        device.core.fields["tLaminationType"].set(eLaminationType.Am) 
    elif sheet["P149"].value=="Am HB1":
        device.core.fields["tLaminationType"].set(eLaminationType.AmHB1) 

    get_cell_value(device.core.fields["dTotalStack"],sheet,"P150")#Total stack
    get_cell_value(device.core.fields["dSteelWidth"],sheet,"P151")#Steel width,if Am5.6,6.7,8.4
    get_cell_value(device.core.fields["dPerOvervoltageMaxFlux"],sheet,"P152")#% Overvoltage re Max Flux
    #get_cell_value(device.core.fields["dTestInductionLevel"],sheet,"P140")#Test Induction Level
    get_cell_value(device.core.fields["dMaxWattsPound"],sheet,"P153")#MAX watts / pound  ( w/ lb)
    get_cell_value(device.core.fields["dInductionLevel"],sheet,"P154")
    get_cell_value(device.core.fields["iNoLoops"],sheet,"P155")
    get_cell_value(device.core.fields["dLossFact"],sheet,"P156")#Core Loss Fact (%) (Zero for Am)

    #LV winding
    get_cell_value(pWindL.fields["dTurnsNom"],sheet,"P158")#Turns
    get_cell_value(pWindL.fields["dNomTurnsUI"],sheet,"P158")#Turns
    get_cell_value(pWindL.coils[0].fields["iLayers"],sheet,"P159")#No. Lyrs (Wire wound LV)
    if sheet["P160"].value==1 or sheet["P160"].value=="1":#1-Strip, 2-magnet
       pWindL.coils[0].conductor[0].fields["iType"].set(eTypeConductor.STRIP) 
    else:
       pWindL.coils[0].conductor[0].fields["iType"].set(eTypeConductor.MAGNET)

    get_cell_value(pWindL.coils[0].conductor[0].fields["dWidth"],sheet,"P162")#Strip Width ( If Strip Cond. ) 
    pWindL.coils[0].conductor[0].calculate_tables(pWindL,eWindingDesc2.LV.value)

    sCond = sheet["C169"].value
    tTypeCond  = eMaterial.AL if sheet["D168"].value=="Aluminum" else eMaterial.CU
    iNoCondE = pWindL.coils[0].conductor[0].lookNoConductor(tTypeCond,iNoPhases,sCond,eWindingDesc2.LV.value)
    pWindL.coils[0].conductor[0].fields["iNoConductor"].set(iNoCondE)##1 LV Cond No. 
    pWindL.coils[0].conductor[0].calculate_Material()
    
    get_cell_value(pWindL.coils[0].conductor[0].fields["iNoAxial"],sheet,"P163")#No axial strands (Wide)
    get_cell_value(pWindL.coils[0].conductor[0].fields["iNoRadial"],sheet,"P164")#No radial strands (High)
    #P167 falta
    get_cell_value(pWindL.fields["dLeadBuild"],sheet,"P168")#No radial strands (High)
    get_cell_value(pWindL.fields["iNoDuctInner"],sheet,"P169")#No. of ducts inner
    get_cell_value(pWindL.fields["iNoDuctOuter"],sheet,"P170")#No. of ducts outer
    get_cell_value(pWindL.fields["dDuctSize"],sheet,"P171")#Duct Size
    if sheet["P172"].value=="Enamel":#Coating for rectangular (Enamel/Paper)?
        pWindL.fields["tCoating"].set(eCoating.ENAMEL) 
    elif sheet["P172"].value=="Paper":
        pWindL.fields["tCoating"].set(eCoating.PAPER) 
    # else:
    #     pWindL.fields["tCoating"].set(eCoating.NONE) 

    #HV winding
    if sheet["P174"].value=="Yes": #Force Constant Stayback?
        pWindH.coils[0].fields["bConstantStayback"].set(eLogical.TRUE)
    else:
        pWindH.coils[0].fields["bConstantStayback"].set(eLogical.FALSE)
    if sheet["P175"].value==1 or sheet["P175"].value=="1":#1-Strip, 2-magnet
       pWindH.coils[0].conductor[0].fields["iType"].set(eTypeConductor.STRIP) 
    else:
       pWindH.coils[0].conductor[0].fields["iType"].set(eTypeConductor.MAGNET)

    get_cell_value(pWindH.coils[0].conductor[0].fields["dWidth"],sheet,"P177")
    pWindH.coils[0].conductor[0].calculate_tables(pWindH,eWindingDesc2.HV.value)

    sCond = sheet["C173"].value
    tTypeCond  = eMaterial.AL if sheet["D172"].value=="Aluminum" else eMaterial.CU
    iNoCondE = pWindL.coils[0].conductor[0].lookNoConductor(tTypeCond,iNoPhases,sCond,eWindingDesc2.HV.value)
    pWindH.coils[0].conductor[0].fields["iNoConductor"].set(iNoCondE)##1 LV Cond No. 
    pWindH.coils[0].conductor[0].calculate_Material()
    
    get_cell_value(pWindH.coils[0].conductor[0].fields["iNoAxial"],sheet,"P178")#No axial strands (Wide)
    get_cell_value(pWindH.coils[0].conductor[0].fields["iNoRadial"],sheet,"P179")#No radial strands (High)
    get_cell_value(pWindH.coils[0].fields["iLayers"],sheet,"P183")
    get_cell_value(pWindH.fields["bHalfTurns"],sheet,"P184")
    get_cell_value(pWindH.fields["iNoDuctInner"],sheet,"P185")#No radial strands (High)
    #P186 falta
    get_cell_value(pWindH.fields["sPHTapLeads"],sheet,"P187")
    #get_cell_value(pWindH.fields["dCoilLineLeads"],sheet,"P187")
    get_cell_value(pWindH.fields["dLeadBuild"],sheet,"P188")#No axial strands (Wide)
    if pWindH.have_taps():
        if sheet["P189"].value=="Eaton 100A Crimps": #Opt. Tapchanger
            pWindH.changer.fields["tModelChanger"].set(eModelChanger.EATON100ACRIMPS)
        elif sheet["P189"].value=="Orto 0498-9T":
            pWindH.changer.fields["tModelChanger"].set(eModelChanger.ORTON0498_9T)
        elif sheet["P189"].value=="Eaton 300A":
            pWindH.changer.fields["tModelChanger"].set(eModelChanger.EATON300A)
        elif sheet["P189"].value=="Quality Switch":
            pWindH.changer.fields["tModelChanger"].set(eModelChanger.QUALITYSWITCH)
        else:
            pWindH.changer.fields["tModelChanger"].set(eModelChanger.NONE)

    #Revision data
    device.fields["data_description"].set(sheet["S91"].value,0) 
    device.fields["data_description"].set(sheet["S92"].value,1) 
    device.fields["data_description"].set(sheet["S93"].value,2) 
    device.fields["data_description"].set(sheet["S94"].value,3) 
    device.fields["data_description"].set(sheet["S95"].value,4) 

    #Guarantees
    get_cell_value(device.guarantee.fields["dNLGuarantee"],sheet,"S98")#NL Guarantee
    get_cell_value(device.guarantee.fields["dLLGuarantee"],sheet,"S99")#LL Guarantee
    #IZ Guarantee
    cell1 = sheet["S100"].value
    if cell1 is not None:
        match = re.search(r'(\d+(\.\d+)?)\s*%', cell1)
        if match:
            numero = float(match.group(1))
  
            device.guarantee.fields["dIZGuarantee"].set(numero)
    get_cell_value(device.guarantee.fields["dExcGuarantee"],sheet,"S101")#Excitation Guarantee

    #General Optionals & Comments
    get_cell_value(device.fields["sConnectionDiagramsItems"],sheet,"S103")#Connection Diagram Items
    get_cell_value(device.fields["dOverallRBFactor"],sheet,"S104")#Opt Overall RB factor (%)
    get_cell_value(device.fields["dAltitude"],sheet,"S105")#Opt. Altitude (Std <=1000 m)
    get_cell_value(device.fields["sWindingSpecComments"],sheet,"S106")#Opt. Winding Spec Comments
    get_cell_value(device.core.fields["sSheetComments"],sheet,"S107")#Opt. Core Sheet Comments
    get_cell_value(device.fields["sPurchasingComments"],sheet,"S108")#Opt. Purchasing Comments
    get_cell_value(device.fields["sInsulationComments"],sheet,"S109")#Opt. Insulation Comments
    get_cell_value(device.fields["dONANLosses"],sheet,"S110")#Opt. ONAN Losses
    get_cell_value(device.fields["dONANTOR"],sheet,"S111")#Opt. ONAN TOR°C

    #Optional Core Data
    get_cell_value(device.core.fields["dWindingHeightAdder"],sheet,"S113")#Opt Win Height Adder
    get_cell_value(device.core.fields["dWindingWidthAdder"],sheet,"S114")#Opt Win Width Adder
    get_cell_value(device.core.fields["dCoilCorePBEnd"],sheet,"S115")#Opt Coil to Core PB / End
    get_cell_value(device.core.fields["dCoilCoilPBSides"],sheet,"S116")#Coil to Coil PB Sides
    get_cell_value(device.core.fields["dCoilCorePBSides"],sheet,"S117")#Coil to Core PB Sides
    get_cell_value(device.core.fields["dFixedSmWindowWidth"],sheet,"S118")#Fixed Sm Window Width
    get_cell_value(device.core.fields["dFixedWindowHeight"],sheet,"S119")#Fixed Window Height
    get_cell_value(device.core.fields["dFixedWindowLarge"],sheet,"S120")#Fixed Window Height

     #Optional Form Data
    get_cell_value(device.fields["sFormMaterial"],sheet,"S122")#Other Type Tests or Comm.
    get_cell_value(device.fields["dFormMaterialRB"],sheet,"S123")#Other Type Tests or Comm.
    get_cell_value(device.fields["sFormComments"],sheet,"S124")#

    #Optional LV Data
    get_cell_value(pWindL.fields["sDuctPlacement"],sheet,"S126")#Opt LV Duct placement
    get_cell_value(pWindL.fields["dLayerInsulation"],sheet,"S127")#Opt LV Layer insulation 
    get_cell_value(pWindL.fields["dOptStayback"],sheet,"S128")#Opt. LV Stayback
    get_cell_value(pWindL.fields["dBuildFactor"],sheet,"S129")#Opt LV Build factor (%)
    if sheet["S130"].value!=None:
        if sheet["S130"].value=="Yes" or sheet["S130"].value=="YES" or sheet["S130"].value=="yes":
            pWindL.fields["bEndfillerOpt"].set(eLogical.TRUE) #Opt. LV Endfiller ? (Yes/No)
        else:
            pWindL.fields["bEndfillerOpt"].set(eLogical.FALSE)
    get_cell_value(pWindL.fields["iLeadPads"],sheet,"S131")
    get_cell_value(pWindL.fields["dNoseAdder"],sheet,"S132")

    #Optional HV Data
    get_cell_value(pWindH.fields["sDuctPlacement"],sheet,"S134")#Opt LV Duct placement
    get_cell_value(pWindH.fields["dOptStayback"],sheet,"S135")#Opt. LV Stayback
    get_cell_value(pWindH.fields["dLayerInsulation"],sheet,"S136")#Opt LV Layer insulation 
    get_cell_value(pWindH.fields["dEndInsulation"],sheet,"S137")#Optional HV End Insulation 
    get_cell_value(pWindH.fields["dLeadPadWitdh"],sheet,"S138")#Opt. HV Lead Pad Width
    get_cell_value(pWindH.fields["iLeadPads"],sheet,"S139")#Opt. # of HV lead pads
    #Opt BIL Under HV
    if sheet["S140"].value==30 or sheet["S140"].value=="30":#LV BIL
        pWindH.fields["iBILUnder"].set(eBIL.BIL_30) 
    elif sheet["S140"].value==45 or sheet["S140"].value=="45":#LV BIL
        pWindH.fields["iBILUnder"].set(eBIL.BIL_45) 
    elif sheet["S140"].value==60 or sheet["S140"].value=="60":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_60)
    elif sheet["S140"].value==75 or sheet["S140"].value=="75":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_75)
    elif sheet["S140"].value==95 or sheet["S140"].value=="95":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_95)
    elif sheet["S140"].value==110 or sheet["S140"].value=="110":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_110)
    elif sheet["S140"].value==125 or sheet["S140"].value=="125":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_125)
    elif sheet["S140"].value==150 or sheet["S140"].value=="150":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_150)
    elif sheet["S140"].value==170 or sheet["S140"].value=="170":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_170)
    elif sheet["S140"].value==200 or sheet["S140"].value=="200":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_200)
    elif sheet["S140"].value==250 or sheet["S140"].value=="250":
        pWindH.fields["iBILUnder"].set(eBIL.BIL_250)
    else:
        pWindH.fields["iBILUnder"].set(eBIL.BIL_0)
    get_cell_value(pWindH.fields["sHLUnder"],sheet,"S141")#Optional HL under
    get_cell_value(pWindH.fields["dHLUnderRB"],sheet,"S142")#Optional HL under (RB)
    get_cell_value(pWindH.fields["sHLOver"],sheet,"S143")#Optional HL over
    get_cell_value(pWindH.fields["dHLOverRB"],sheet,"S144")#Optional HL over (RB)
    get_cell_value(pWindH.fields["dBuildFactor"],sheet,"S145")#Optional HV build factor (%)
    if sheet["S146"].value!=None:
        if sheet["S146"].value=="Yes" or sheet["S146"].value=="YES" or sheet["S146"].value=="yes":#Opt. HV Endfiller ? (Yes/No)
            pWindH.fields["bEndfillerOpt"].set(eLogical.TRUE) 
        else:
            pWindH.fields["bEndfillerOpt"].set(eLogical.FALSE) 
    get_cell_value(pWindH.fields["dNoseAdder"],sheet,"S147")
    #get_cell_value(pWindH.fields["sPHTapLeads"],sheet,"S133")#Optional HV Ph. / Tap Leads

    #Routine Tests
    if sheet["S149"].value=="LV":
        device.tests.fields["tHipot"].set(eWindingSelection.LV) #Hipot (HV,LV,Both,None)
    elif sheet["S149"].value=="HV":
        device.tests.fields["tHipot"].set(eWindingSelection.HV) 
    elif sheet["S149"].value=="Both":
        device.tests.fields["tHipot"].set(eWindingSelection.BOTH) 
    else:
        device.tests.fields["tHipot"].set(eWindingSelection.NONE) 
    get_cell_value(device.tests.fields["dHipotHV"],sheet,"S150")#Hipot HV
    get_cell_value(device.tests.fields["dHipotLV"],sheet,"S151")#Hipot LV

    if sheet["S152"].value=="Yes":#Induced (Yes/No)
        device.tests.fields["bInduced"].set(eLogical.TRUE)
    else:
        device.tests.fields["bInduced"].set(eLogical.FALSE)
    if sheet["S153"].value is not None:
        device.tests.fields["dXRatedVoltage"].set(sheet["S153"].value) #X Rated Voltage

    if sheet["S154"].value=="Yes":#Resistance Test (Yes/No)
        device.tests.fields["bResistance"].set(eLogical.TRUE)
    else:
        device.tests.fields["bResistance"].set(eLogical.FALSE)
        
    if sheet["S155"].value=="Yes":#Other Resistance (Yes/No)
        device.tests.fields["bOtherResistance"].set(eLogical.TRUE)
    else:
        device.tests.fields["bOtherResistance"].set(eLogical.FALSE)

    sS143 = sheet["S156"].value
    if sS143=="E": #Tap Extremes OR All (E/A)
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.E)
    elif sS143=="A":
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.A)
    else:
        device.tests.fields["tTapsExtremesORAII"].set(eTapExtremes.NONE)
 
    get_cell_value(device.tests.fields["sSampleRate"],sheet,"S157")#Opt. Sample Rate (description)
    if sheet["S158"].value=="Yes" or sheet["S158"].value=="yes" or sheet["S158"].value=="YES":  #Core Loss Test? (Yes/No)
        device.tests.fields["bCoreLossTest"].set(eLogical.TRUE)
    else:
        device.tests.fields["bCoreLossTest"].set(eLogical.FALSE)
    if sheet["S159"].value=="":#Opt. Extra No Load (% Nom.)
        device.tests.fields["dExtraNoLoad"].set(0.0)
    else:
        device.tests.fields["dExtraNoLoad"].set(sheet["S159"].value)
    if sheet["S159"].value=="":#Opt.2 Extra No Load (% Nom.)
        device.tests.fields["dExtraNoLoad2"].set(0.0)
    else:
        device.tests.fields["dExtraNoLoad2"].set(sheet["S159"].value)
    if sheet["S161"].value=="Yes" or sheet["S161"].value=="yes" or sheet["S161"].value=="YES":  #Opt. Extra No Load (% Nom.)
        device.tests.fields["bLoadLossTest"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bLoadLossTest"].set(eLogical.FALSE) 

    if sheet["S162"].value=="Yes" or sheet["S162"].value=="yes" or sheet["S162"].value=="YES":  #Ld Loss @ "&100
        device.tests.fields["bLoadLossTap"].set(eLogical.TRUE)
    else:
        device.tests.fields["bLoadLossTap"].set(eLogical.FALSE)

    get_cell_value(device.tests.fields["d3LoadLosskVA"],sheet,"S163")#Opt. 3rd Load Loss % kVA
    get_cell_value(device.tests.fields["d3LoadLossTap"],sheet,"S164")#Opt. 3rd Load Loss % Tap 
    get_cell_value(device.tests.fields["d4LoadLosskVA"],sheet,"S165")#Opt. 4th Load Loss % kVA
    get_cell_value(device.tests.fields["d4LoadLossTap"],sheet,"S166")#Opt. 4th Load Loss % Tap 
    get_cell_value(device.tests.fields["d5LoadLosskVA"],sheet,"S167")#Opt. 5th Load Loss % kVA
    get_cell_value(device.tests.fields["d5LoadLossTap"],sheet,"S168")#Opt. 5th Load Loss % Tap 
    get_cell_value(device.tests.fields["dDissipationFactor"],sheet,"S169")
    #P170 falta
    if sheet["S171"].value!="":
        device.tests.fields["bOtherRoutine"].set(eLogical.TRUE)
        get_cell_value(device.tests.fields["sOtherRoutineComments"],sheet,"S171")#Other Routine Tests or Comm.

    #Type Tests
    if sheet["S173"].value=="Yes":  #Temperature Rise ? (Yes/No)
        device.tests.fields["bTemperatureRise"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bTemperatureRise"].set(eLogical.FALSE) 
    sSpecification = sheet["S174"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.IEEEC571290)
    elif sSpecification=="IEC 60076-2":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.IEC600762)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsTR"].set(eStandardTemperatureRise.CSAC88)

    get_cell_value(device.tests.fields["sCommentsTR"],sheet,"S175")#Comments:
    if sheet["S176"].value=="Yes":  #Lightning Impulse ? (Yes/No)
        device.tests.fields["bLightningImpulse"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bLightningImpulse"].set(eLogical.FALSE) 
    sSpecification = sheet["S177"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.IEEEC571290)
    elif sSpecification=="IEC 60076-3":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.IEC600763)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsLI"].set(eStandardLightningImpulse.CSAC88)
    sSequence = sheet["S178"].value
    if sSequence=="1R":#Sequence (1R, 2C, 1F)
        device.tests.fields["tSequenceLI"].set(eSequence.R1) 
    elif sSequence=="2C":
        device.tests.fields["tSequenceLI"].set(eSequence.C2) 
    elif sSequence=="1F":
        device.tests.fields["tSequenceLI"].set(eSequence.F1) 
    elif sSequence=="1R, 2C":
        device.tests.fields["tSequenceLI"].set(eSequence.R1C2) 
    elif sSequence=="1R, 1F":
        device.tests.fields["tSequenceLI"].set(eSequence.R1F1)
    elif sSequence=="2C, 1F":
        device.tests.fields["tSequenceLI"].set(eSequence.C2F1) 
    elif sSequence=="1R, 2C, 1F" or sSequence=='1R, 2C,1F':
        device.tests.fields["tSequenceLI"].set(eSequence.R1C2F1) 
    else:
        device.tests.fields["tSequenceLI"].set(eSequence.NONE) 

    if sheet["S179"].value=="HV":   #Impulse (HV,LV,Both,None)
        device.tests.fields["tImpulse"].set(eWindingSelection.HV) 
    elif sheet["S179"].value=="LV":  
        device.tests.fields["tImpulse"].set(eWindingSelection.LV) 
    elif sheet["S179"].value=="Both":  
        device.tests.fields["tImpulse"].set(eWindingSelection.BOTH) 
    else:
        device.tests.fields["tImpulse"].set(eWindingSelection.NONE)
    get_cell_value(device.tests.fields["sCommentsLI"],sheet,"S180")#Comments:
    if sheet["S181"].value=="Yes":  #RIV ? (Yes/No)
        device.tests.fields["bRIV"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bRIV"].set(eLogical.FALSE) 
    sSpecification = sheet["S182"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.IEEEC571290)
    elif sSpecification=="CISPR 18-2":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.CISPR182)
    elif sSpecification=="IEC 60437":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.IEC60437)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.CSAC88)
    elif sSpecification=="NEMA 107-1987":
        device.tests.fields["tSpecificationsRIV"].set(eStandardRIV.NEMA1071987)

    get_cell_value(device.tests.fields["sCommentsRIV"],sheet,"S183")#Comments:
    if sheet["S184"].value=="Yes":  #Sound Level ? (Yes/No)
        device.tests.fields["bSoundLevel"].set(eLogical.TRUE) 
    else:
        device.tests.fields["bSoundLevel"].set(eLogical.FALSE) 
    sSpecification = sheet["S185"].value #To which Specifications ? 
    if sSpecification=="ANSI C57.12.90" or sSpecification=="ANSI C57 12.90":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEEEC571290)
    elif sSpecification=="IEE C57.12.91":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEEC571291)
    elif sSpecification=="IEC 60076-10":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.IEC6007610)
    elif sSpecification=="CSA C88":
        device.tests.fields["tSpecificationsSL"].set(eStandardSoundLevel.CSAC88)
    get_cell_value(device.tests.fields["sCommentsSL"],sheet,"S186")#Comments:
    if sheet["S187"].value!="":
        device.tests.fields["bOtherTypeTest"].set(eLogical.TRUE)
        get_cell_value(device.tests.fields["sOtherRoutineCommentsType"],sheet,"S170")#Other Type Tests or Comm.

    texto = sheet["F189"].value
    resultado = False
    if texto is not None:
        resultado = "per k4" in texto.lower()
    if resultado:
        device.cooling.fields["tK4Factor"].set(eLogical.TRUE)
    
    get_cell_value(pWindL.coils[0].fields["iSectionsLeg"],sheet,"F90")#Sections / Leg
    get_cell_value(pWindH.coils[0].fields["iSectionsLeg"],sheet,"I90")#Sections / Leg

    get_cell_value(device.core.fields["dLamFactor"],sheet,"Z21")
    get_cell_value(device.core.fields["dStackTolPIn"],sheet,"Z22")
    get_cell_value(device.core.fields["dStackTolMIn"],sheet,"Z23")