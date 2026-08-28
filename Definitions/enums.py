# Author:               TOM GONCIN
# File Description:     File used to aid UI in handling necessary enumerations. No data is stored yet, just placeholders

from enum import Enum
from pyclbr import Class

#from statistics import LinearRegression
from tkinter import commondialog

class eTransformerType(Enum):
    SINGLE_PHASE = "Single Phase"
    THREE_PHASE = "Three Phase"
    AUTO_TRANSFORMER = "Auto Transformer"

class eTransformerFamily(Enum):
    NONE = "None"
    MPM = "Mini Pad Mount"
    PM = "Pole Mounted"
    PLMT = "Platform Mount"
    PMT = "Pad-Mounted"
    DPM = "DPM"
    OWP = "Outdoor WP"
    SP = "Submersible"
    LPMT = "Light Pad-Mounted"
    #Pdmt = "Pad-Mount Transformer"

class eWindingDesc(Enum):
    NONE = "None"
    HV = "High Voltage"
    LV = "Low Voltage"
    TV = "Tertiary Voltage"
    REG_HV = "Regulation High Voltage"
    REG_LV = "Regulation High Voltage"
    SERIES = "Series Voltage"
    COMMON = "Common Voltage"

class eWindingDesc2(Enum):
    NONE = -1
    HV = 0
    LV = 1
    TV = 2
    REG_HV = 3
    REG_LV = 4
    SERIES = 0
    COMMON = 1

class eWidthDucts(Enum):
    DNone = "0.0"
    D1_8 = "1/8"
    D1_4 = "1/4"
    D3_16 = "3/16"

class eCoolingType(Enum):
    COOL_NONE = "None"
    COOL_ONAN = "ONAN"
    COOL_ONAN_ONAF = "ONAN/ONAF"
    COOL_ONAN_ONAF_ONAF = "ONAN/ONAF/ONAF"
    COOL_KNAN= "KNAN"
    COOL_KNAN_KNAF = "KNAN/KNAF"
    COOL_KNAN_KNAF_KNAF = "KNAN/KNAF/KNAF"
    #COOL_LNAN= "LNAN"
    #COOL_LNAN_LNAF = "LNAN/LNAF"
    #COOL_LNAN_LNAF_ONAF = "LNAN/LNAF/LNAF"
    

class eSingleCoolingType(Enum):
    COOL_NONE = "None"
    COOL_ONAN = "ONAN"
    COOL_ONAF = "ONAF"
    COOL_KNAN = "KNAN"
    COOL_KNAF = "KNAF"
    COOL_LNAN = "LNAN"
    COOL_LNAF = "LNAF"

class ePhaseType(Enum):
    SINGLE = "Single"
    THREE = "Three"

class eConnectionType(Enum):
    NONE = "None"
    DELTA = "Delta"
    WYE = "Wye"

class eDesignSpec(Enum):
    NONE = "None"
    ANSI = "ANSI"
    C2106 = "C2.1/06"
    C2124 = "C2.1/24" 
    C2206 = "C2.2/06" 
    C8816 = "C88-16"
    C88M79 = "C88.M79"
    C88M90 = "C88.M90"
    C19908 = "C199-08"
    C227306 = "C227.3/06"
    C227406 = "C227.4/06"
    C227421 = "C227.4/21"
    C227508 = "C227.5/08"
    C301106 = "C301.1/06"
    C301206 = "C301.2/06"   

class eEfficiencySpec(Enum):
    NONE = "None"
    ANSI = "ANSI"
    C2106 = "C2.1/06"
    C2124 = "C2.1/24"
    C2206 = "C2.2/06"
    C8816 = "C88-16"
    C88M79 = "C88.M79"
    C88M90 = "C88.M90"
    C19908 = "C199-08"
    C227306 = "C227.3/06"
    C227406 = "C227.4/06"
    C227421 = "C227.4/21"
    C227508 = "C227.5/08"   
    C301106 = "C301.1/06"
    C301206 = "C301.2/06" 

class eLogical(Enum):
    TRUE = "Yes"
    FALSE = "No"
 
class eLogical2(Enum):
    TRUE = 1
    FALSE = 0

class eTemperatureRise(Enum):
    Temp_NONE = "None"
    #Temp_45 = "45"
    #Temp_45_55 = "45/55"
    #Temp_50 = "50"
    Temp_55 = "55"
    Temp_55_65 = "55/65"
    #Temp_60 = "60"
    Temp_65 = "65"
    #Temp_65_75 = "65/75"
    #Temp_70 = "70"
    #Temp_75 = "75"

class eArrayType(Enum):
    H_L = "H-L"
    L_H = "L-H"
    L_H_L = "L-H-L"
    L_L_H = "L-L-H"
    OTHER = "Other"
    # L_HT = "L-HT"
    # RL_L_HT = "RL-L-HT"
    # L_H_RH = "L-H-RH"
    # T_L_HT = "T-L-HT"
    # T_L_H_RH = "T-L-H-RH"
    # L_H = "L-H"
    # RL_L_H = "RL-L-H"
    # T_RL_L_HT = "T-RL-L-HT"
    # T_RL_L_H = "T-RL-L-H"
    # RL_L_L_HT = "RL-L-L-HT"
    # H_L = "H-L"
    # RL_L_L_H = "RL-L-L-H"
    # T_RL_L_H_RH = "T-RL-L-H-RH"
    # H_L_H_RH = "H-L-H-RH"
    # L_L_HT = "L-L-HT"
    # RH_H_L_H = "RH-H-L-H"
    # T_RH_H_L_H = "T-RH-H-L-H"
    # RL_L_H_RH = "RL-L-H-RH"
    # L_H_RH_T = "L-H-RH-T"
    # L_L_H_RH = "L-L-H-RH"
    # L_RH_H_T = "L-RH-H-T"
    # T_RH_H_L = "T-RH-H-L"
    # L_H_RH_H_T = "L-H-RH-H-T"
    # T_H_RH_L = "T-H-RH-L"
    # L_L_H = "L-L-H"
    # L_RH_H = "L-RH-H"
    # L_RL_HT = "L-RL-HT"
    # T_RH_L_H = "T-RH-L-H"
    # T_H_L_RH = "T-H-L-RH"
    # RH_L_H = "RH-L-H"
    # H_L_H = "H-L-H"
    # RL_L_T_RH_H = "RL-L-T-RH-H"
    # L_RL_T_HT = "L-RL-T-HT"
    # T_L_RH_H = "T-L-RH-H"
    # L_T_L_H_RH = "L-T-L-H-RH"
    # T_RL_L_L_H = "T-RL-L-L-H"
    # T_H_RH_H_L = "T-H-RH-H-L"
    # T_H_L_HT = "T-H-L-HT"
    # L_H_RH_L = "L-H-RH-L"
    # H_L_HT = "H-L-HT"
    # T_RL_L_L_HT = "T-RL-L-L-HT"
    # T_H_T_L_RL = "T-H-T-L-RL"
    # T_L_T_HT = "T-L-T-HT"
    # RH_L_L_H = "RH-L-L-H"
    # L_RL_H_T = "L-RL-H-T"
    # T_L_L_HT = "T-L-L-HT"
    # T_L_H_RH_H = "T-L-H-RH-H"
    # L_HT_RL = "L-HT-RL"
    # T_RL_L_RH_H = "T-RL-L-RH-H"
    # RL_T_L_H_RH = "RL-T-L-H-RH"
    # T_T_L_HT = "T-T-L-HT"
    # H_L_RH_H = "H-L-RH-H"
    # T_L_H_H_RH = "T-L-H-H-RH"
    # T_L_H = "T-L-H"
    # T_L_L_H_RH = "T-L-L-H-RH"
    # HT_L_RL = "HT-L-RL"
    # T_H_L_H = "T-H-L-H"
    # RL_T_L_HT = "RL-T-L-HT"
    # L_RL_L_HT = "L-RL-L-HT"
    # T_L_H_H = "T-L-H-H"
    # L_H_H_RH = "L-H-H-RH"
    # H_L_L = "H-L-L"
    # RL_L_H_T_RH = "RL-L-H-T-RH"
    # T_L_RL_HT = "T-L-RL-HT"
    # RL_T_L_L_HT = "RL-T-L-L-HT"
    # L_H_L_HT = "L-H-L-HT"
    # RL_L_T_H = "RL-L-T-H"
    # H_L_L_H_RH = "H-L-L-H-RH"
    # T_RH_L_HT = "T-RH-L-HT"
    # L_H_HT = "L-H-HT"
    # L_H_RL = "L-H-RL"
    # T_L_RL_H_RH = "T-L-RL-H-RH"
    # T_H_RL_L = "T-H-RL-L"
    # RH_H_L = "RH-H-L"
    # T_L_RH_H_L = "T-L-RH-H-L"
    # L_T_HT = "L-T-HT"
    # L_H_T_RH = "L-H-T-RH"
    # T_RH_RL_H_L_H = "T-RH-RL-H-L-H"
    # H_RH_L = "H-RH-L"
    # H_RH_H_L_H = "H-RH-H-L-H"
    # RL_RH_L_H_T = "RL-RH-L-H-T"
    # T_C_ST = "T-C-ST"
    # T_C_RS_S = "T-C-RS-S"
    # T_RC_C_ST = "T-RC-C-ST"
    # T_C_S_RS = "T-C-S-RS"
    # T_RC_C_S = "T-RC-C-S"
    # T_RS_C_S = "T-RS-C-S"
    # C_S = "C-S"
    # T_RC_C_S_RS = "T-RC-C-S-RS"
    # C_ST = "C-ST"
    # T_C_RC_S = "T-C-RC-S"
    # T_C_S = "T-C-S"
    # T_S_C_RS_S = "T-S-C-RS-S"
    # C_C_RS_S = "C-C-RS-S"
    # T_RC_C_RS_S = "T-RC-C-RS-S"
    # T_RC_C_C_ST = "T-RC-C-C-ST"
    # T_C_S_RC = "T-C-S-RC"
    # C_RS_S = "C-RS-S"
    # T_C_C_RS_S = "T-C-C-RS-S"
    # C_C_ST = "C-C-ST"
    # C_RS_C_S = "C-RS-C-S"
    # RC_T_C_S = "RC-T-C-S"
    # C_S_RS = "C-S-RS"
    # RC_C_S = "RC-C-S"
    # T_C_RC_ST = "T-C-RC-ST"
    # RC_C_ST = "RC-C-ST"
    # T_RC_S_C_ST = "T-RC-S-C-ST"
    # T_RC_C_S_S = "T-RC-C-S-S"
    # T_C_RS_C_S = "T-C-RS-C-S"
    # T_C_S_RS_S = "T-C-S-RS-S"
    # C_C_S_RS = "C-C-S-RS"
    # T_C_C_ST = "T-C-C-ST"
    # T_C_RS_S_T = "T-C-RS-S-T"
    # T_C_C_S_RS = "T-C-C-S-RS"
    # T_S_C_RC = "T-S-C-RC"
    # T_RS_S_C_S = "T-RS-S-C-S"
    # T_S_RC_C_RS_RS = "T-S-RC-C-RS-RS"
    # C_T_S_RS_S = "C-T-S-RS-S"
    # T_S_C_ST = "T-S-C-ST"
    # T_RS_S_C = "T-RS-S-C"
    # RC_C_RS_S = "RC-C-RS-S"
    # T_S_C_S_RS = "T-S-C-S-RS"
    # C_C_S = "C-C-S"
    # T_C_S_S_RS = "T-C-S-S-RS"
    # T_RC_RC_C_ST = "T-RC-RC-C-ST"
    # T_RS_C_ST = "T-RS-C-ST"
    # T_S_RS_C_T = "T-S-RS-C-T"
    # T_S_RC_C_S_RS = "T-S-RC-C-S-RS"
    # C_S_S = "C-S-S"
    # S_C_RS = "S-C-RS"
    # RS_C_C_S = "RS-C-C-S"
    # T_RC_C_S_ST = "T-RC-C-S-ST"
    # T_RC_S_C_S = "T-RC-S-C-S"
    # T_RC_RS_C_S = "T-RC-RS-C-S"
    # C_S_T_ST = "C-S-T-ST"
    # T_T_C_RS_S = "T-T-C-RS-S"
    # S_C_RC_T = "S-C-RC-T"
    # C_S_RS_T = "C-S-RS-T"
    # T_C_ST_C_RC = "T-C-ST-C-RC"
    # S_C = "S-C"
    # RL_L_L_H_RH = "RL-L-L-H-RH"
    # T_C_ST_RS = "T-C-ST-RS"
    # T_L_HT_RH = "T-L-HT-RH"
    # T_C_RS_ST = "T-C-RS-ST"

class eCoilType(Enum):
    DEFAULT = -1,
    HELICOIDAL = 0,
    DISCOS = 1,	
    MULTICIRCUITOS = 2

class eRegulationType(Enum):
    REGULATION = 1
    TAPS = 2
    NONE = 3

class eChangerCapacity(Enum):
    NONE = "None"
    OLTC = "OLTC"
    OCTC = "OCTC"
    RCBN = "RCBN"
    FCBN = "FCBN"

class eModelChanger(Enum):
    NONE = "None"
    EATON100ACRIMPS = "Eaton 100A Crimps"
    ORTON0498_9T = "Orto 0498-9T"
    EATON300A = "Eaton 300A"
    QUALITYSWITCH = "Quality Switch"

class eBIL(Enum):
    BIL_0 = "0"
    BIL_30 = "30"
    BIL_45 = "45"
    BIL_60 = "60"
    BIL_75 = "75"
    BIL_95 = "95"
    BIL_110 = "110"
    BIL_125 = "125"
    BIL_150 = "150"
    BIL_170 = "170"
    BIL_200 = "200"
    BIL_250 = "250"
    #BIL_325 = "325"
    #BIL_350 = "350"
    #BIL_450 = "450"
    #BIL_550 = "550"
    #BIL_650 = "650"
    #BIL_750 = "750"
    #BIL_825 = "825"
    #BIL_850 = "850"
    #BIL_900 = "900"
    #BIL_950 = "950"
    #BIL_1050 = "1050"
    #BIL_1175 = "1175"
    #BIL_1300 = "1300"
    #BIL_1425 = "1425"
    #BIL_1550 = "1550"
    #BIL_1675 = "1675"

# class eBILUnder(Enum):
#     BIL_0 = "0"
#     BIL_30 = "30"
#     BIL_60 = "60"
#     BIL_75 = "75"
#     BIL_95 = "95"
#     BIL_110 = "110"
#     BIL_125 = "125"
#     BIL_150 = "150"
#     BIL_170 = "170"
#     BIL_200 = "200"
#     BIL_250 = "250"
#     #BIL_325 = "325"
#     #BIL_350 = "350"
#     #BIL_450 = "450"
#     #BIL_550 = "550"
#     #BIL_650 = "650"
#     #BIL_750 = "750"
#     #BIL_825 = "825"
#     #BIL_850 = "850"
#     #BIL_900 = "900"
#     #BIL_950 = "950"
#     #BIL_1050 = "1050"
#     #BIL_1175 = "1175"
#     #BIL_1300 = "1300"
#     #BIL_1425 = "1425"
#     #BIL_1550 = "1550"
#     #BIL_1675 = "1675"

class eLaminationType(Enum):
    TYPE_NONE = "None"
    M0H = "MOH"
    M2 = "M2"
    M3 = "M3"
    M4 = "M4"
    Am = "Am"
    AmHB1 = "Am HB1"

class eLaminationTypeNum(Enum):
    TYPE_NONE = -1
    M0H = 0
    M2 = 1
    M3 = 2
    M4 = 3
    Am = 4
    AmHB1 = 5

class eCoreTolerance(Enum):
    NONE = -1
    A = 0
    C = 1
    E = 2
    STRIP = 3

class eMaterial(Enum):
    MAT_NONE = "None"
    CU = "Copper"
    AL = "Aluminum"

class eMaterialVal(Enum):
    MAT_NONE = "None"
    CU = 1
    AL = 2

class eLeads(Enum):
    LEADS_NONE = "None"
    CU011 = "011CU"         #0.010x1"
    CU0931 = "0931CU"       #0.093x1"
    CU052 = "052CU"         #0.050x2"
    CU050x4S = "050x4SCU"   #0.050x4"
    AL047X2 = "047X2AL"     #0.047x2"
    AL090X2 = "090X2AL"     #0.090x2"
    AL063X3 = "063X3AL"     #0.063x3"
    AL079X4 = "079X4AL"     #0.0787x4"

class eBars(Enum):
    BARS_NONE = "None"
    CU15 = "15CUBAR"        #0.25x1.5"
    CU2 = "2CUBAR"          #0.25x2"
    CU3 = "3CUBAR"          #0.25x3"
    CU4 = "4CUBAR"          #0.25x4"
    ALFRE2 = "2ALFRE"       #0.25x2"
    ALFRE3 = "3ALFRE"       #0.25x3"
    ALFRE4 = "4ALFRE"       #0.25x4"
    CULEADS011 = "011CU"         #0.010x1"
    CULEADS0931 = "0931CU"       #0.093x1"
    CULEADS052 = "052CU"         #0.050x2"
    CULEADS050x4S = "050X4SCU"   #0.050x4"
    ALLEADS047X2 = "047X2AL"     #0.047x2"
    ALLEADS090X2 = "090X2AL"     #0.090x2"
    ALLEADS063X3 = "063X3AL"     #0.063x3"
    ALLEADS079X4 = "079X4AL"     #0.0787x4"

class eTypeConductor(Enum):
    NONE = "None"
    STRIP = "Strip"
    MAGNET = "Magnet"

class eCoating(Enum):
    NONE = "None"
    ENAMEL = "Enamel"
    PAPER = "Paper"

class eWindingSelection(Enum):
    NONE = "None"
    HV = "HV"
    LV = "LV"
    BOTH = "Both"
    
class eTapExtremes(Enum):
    NONE = "None"
    E = "E"
    A = "A"

class eSequence(Enum):
    NONE = "None"
    R1 = "1R"
    C2 = "2C"
    F1 = "1F"
    R1C2 = "1R,2C"
    R1F1 = "1R,1F"
    C2F1 = "2C,1F"
    R1C2F1 = "1R,2C,1F"
   # type: ignore

class eChangerType(Enum):
	NONE = -1,
	CSC = 0,
	CBC = 1

class eLRType(Enum):
    NONE = "None"
    L = "Linear"
    R = "Reversing"

class eCBCType(Enum):
	NONE = "None"
	RESISTIVE = "Resistive"			
	REACTIVE = "Reactive"			

class eStandardTemperatureRise(Enum):
    NONE = "None"
    IEEEC571290 = "IEEE C57.12.90"
    IEC600762 = "IEC 60076-2"
    CSAC88 = "CSA C88"

class eStandardLightningImpulse(Enum):
    NONE = "None"
    IEEEC571290 = "IEEE C57.12.90"
    IEC600763 = "IEC 60076-3"
    CSAC88 = "CSA C88"

class eStandardRIV(Enum):
    NONE = "None"
    IEEEC571290 = "IEEE C57.12.90"
    CISPR182 = "CISPR 18-2"
    IEC60437 = "IEC 60437"
    CSAC88 = "CSA C88"
    NEMA1071987 = "NEMA 107-1987"

class eStandardSoundLevel(Enum):
    NONE = "None"
    IEEEC571290 = "IEEE C57.12.90"
    IEEC571291 = "IEEE C57.12.91"
    IEC6007610 = "IEC 60076-10"
    CSAC88 = "CSA C88"

class eStandardPartialDischarge(Enum):
    NONE = "None"
    IEEC571290 = "IEEE C57.12.90"
    IEEC57113 = "IEEE C57.113"
    IEC60270 = "IEC 60270"
    CSAC88 = "CSA C88"

class eWarnings(Enum):
    eNONE = -1
    eWARNING = 0
    eERROR = 1

class eLiquid(Enum):
    eNONE = "None"
    eOil = "Oil"
    eFR3 = "FR3"

class eSegment(Enum):
    eFREE = "Free"
    eBLOCK = "Block"

class eRadiator(Enum):
    eNONE = "None"
    eMENK = "Menk"
    eTRANTER = "Tranter"

class eCooling(Enum):
    eNONE = "None"
    eNATURAL = "Natural"
    e1STAGEFORCED = "1-stage Forced"
    e2STAGEFORCED = "2-stage Forced"

class eErrorDiseniar(Enum):
	ERR_NINGUNO = 0
	ERR_ENF		= 400000000000000
	ERR_DISMEC	= 500000000000000
	ERR_DISELEC	= 600000000000000
	ERR_DIMCOND	= 700000000000000
	ERR_COND	= 800000000000000
	ERR_57PC	= 900000000000000
