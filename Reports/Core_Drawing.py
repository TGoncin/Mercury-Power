from http.client import LOOP_DETECTED
import openpyxl
from openpyxl import Workbook
from openpyxl.styles import Font, Alignment, Border, Side, PatternFill
from openpyxl.utils import get_column_letter
from openpyxl.worksheet.page import PageMargins
from openpyxl.drawing.image import Image
from PIL import Image as PILImage
from io import BytesIO
from datetime import datetime
import win32com.client as win32
import os
from Resources.resources import resource_path
from Definitions.enums import eConnectionType,eArrayType,eLogical,eBIL,eTransformerFamily,eWindingSelection,eTapExtremes,eSequence
from Definitions.enums import eCoreTolerance,eChangerCapacity,eMaterial,eLaminationType,eTypeConductor,eCoating,eDesignSpec,eCoolingType,eSingleCoolingType,eTemperatureRise
from Definitions.functions import address,ADDRESSZ,PI,is_folder_open
from datetime import datetime

def make_core_drawing(device = None):
    tLamination = device.core.fields['tLaminationType'].get()
    bAmorphous = True if tLamination == eLaminationType.Am or tLamination == eLaminationType.AmHB1 else False 
    iNoPhases = device.fields['iNoPhases'].get()

    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    pLCoil = pWindL.coils[0]
    pHCoil = pWindH.coils[0]
    pLCond = pLCoil.conductor[0]
    pHCond = pHCoil.conductor[0]
    bHaveTaps = pWindH.have_taps()

    sCore = device.core.calculate_sPart()
    sOneLoop = ("One Loop" if (device.core.fields['dSteelWidth'].get()==5.6 or device.core.fields['dSteelWidth'].get()==6.7 or device.core.fields['dSteelWidth'].get()==8.4) else "Two Loops") if bAmorphous else "One Loop"
    sTwoLoops = ("Two Loops" if (device.core.fields['dSteelWidth'].get()==5.6 or device.core.fields['dSteelWidth'].get()==6.7 or device.core.fields['dSteelWidth'].get()==8.4) else "Four Loops") if bAmorphous else "Two Loops"

    s_Notes = ['']*5
    s_Notes[0] = "Gaps on the bottom" if bAmorphous else "Core Gaps On Sides"
    s_Notes[1] = "*****  This is a USED Core in surplus stock,  DO NOT ORDER !  *****" if device.core.fields['sCoreSuffix'].get()=="USED" else ""
    if device.core.fields['sSheetComments'].get()!="" and len(device.core.fields['sSheetComments'].get()) > 0:  
        if len(device.core.fields['sSheetComments'].get()) < 72:
            s_Notes[2] = device.core.fields['sSheetComments'].get()
        else:
            espacio_index = device.core.fields['sSheetComments'].get().find(" ", 68)
            if espacio_index != -1:
                s_Notes[2] = device.core.fields['sSheetComments'].get()[:espacio_index]
            else:
                s_Notes[2] = device.core.fields['sSheetComments'].get() 
    else:
        s_Notes[2] = ""
    
    if len(device.core.fields['sSheetComments'].get()) <= 72:
        s_Notes[3] = ""
    elif len(device.core.fields['sSheetComments'].get()) <= 144:
        s_Notes[3] = device.core.fields['sSheetComments'].get()[len(s_Notes[2]):] 
    else:
        espacio_index = device.core.fields['sSheetComments'].get().find(" ", len(s_Notes[2]) + 67)
        if espacio_index != -1:
            s_Notes[3] = device.core.fields['sSheetComments'].get()[len(s_Notes[2]):espacio_index]
        else:
            s_Notes[3] = device.core.fields['sSheetComments'].get()[len(s_Notes[2]):] 

    if s_Notes[3] == " ":
        s_Notes[4] = " "
    elif len(s_Notes[2]) + len(s_Notes[3]) == len(device.core.fields['sSheetComments'].get()):
        s_Notes[4] = ""
    else:
        s_Notes[4] = device.core.fields['sSheetComments'].get()[len(s_Notes[2]) + len(s_Notes[3]):] 

    if tLamination == eLaminationType.AmHB1:
        sWLbTHz = "0.122 W/Lb @ 1.4T, 60Hz"
    elif tLamination == eLaminationType.M4:
        sWLbTHz = "0.74 W/Lb @ 1.7T, 60Hz"
    else:
        sWLbTHz = "0.6 W/Lb @ 1.7T, 60Hz"

    if tLamination == eLaminationType.AmHB1:
        value = device.fields['dVxT'].get() / 2 / device.core.fields['dFluxDensityVoltT'].get() * 1.4
    else:
        value = device.fields['dVxT'].get() / 2 / device.core.fields['dFluxDensityVoltT'].get() * 1.7
    value_str = f"{value:.3f}"
    sTestLoop = f"Test Loop =    {value_str}  V/T"

    date = datetime.strptime(device.fields['original_issue_date'].get(), "%m/%d/%Y")
    sformat_date = date.strftime("%b %#d/%y")
    SDrawNo = f"CD-{device.fields['so_number'].get()}"

    sDPNo = f"{device.fields['dLatestVersionNo'].get():.2f}" if device.fields['bDesingSpecial'].get()==eLogical.FALSE else f"{device.fields['dLatestVersionNo'].get():.2f} M"
    
    sPTI = None
    sCoreManufacturer =None
    sInnerPer = None
    sOuterPer = None
    sInnerPTI = None
    sOuterPTI = None
    sInnerCoreManufacturer = None
    sOuterCoreManufacturer = None
    if bAmorphous:
        sPTI = "PTI's"
        sCoreManufacturer = "Core Manufacturer's"
        sInnerPer = "Inner perimeter"
        sOuterPer = "Outer perimeter"
        dInnerPTI = (2*(device.core.fields['dSmallWindowWidth'].get()+device.core.fields['dWindowHeight'].get()))
        sInnerPTI = f"{(dInnerPTI):.3f}''"
        sOuterPTI = f"{((dInnerPTI/PI+2*device.core.fields['dStackPerLoop'].get())*PI):.3f}''"
        sInnerCoreManufacturer = "18.0'' Min."
        sOuterCoreManufacturer = "132.0'' Max."

    sRev = ['']*3
    sRevDescr = ['']*3
    sDateDescr = ['']*3
    iRev = device.fields['i_RevisionWS'].get()[1]
    sRev[0] = 1 if iRev!=0 else ""
    sRev[1] = 2 if iRev>=2 else ""
    sRev[2] = 3 if iRev==3 else ""
    sRevDescr[0] = device.fields['s_DataDescr1'].get()[0] if iRev!=0 else ""
    sRevDescr[1] = device.fields['s_DataDescr1'].get()[1] if iRev>=2 else ""
    sRevDescr[2] = device.fields['s_DataDescr1'].get()[2] if iRev==3 else ""
    sDateDescr[0] = device.fields['s_DateDescr1'].get()[0] if iRev!=0 else ""
    sDateDescr[1] = device.fields['s_DateDescr1'].get()[1] if iRev>=2 else ""
    sDateDescr[2] = device.fields['s_DateDescr1'].get()[2] if iRev==3 else ""
    sCustomSpec = f"Cust Spec:   {device.guarantee.fields['sCustomerSpec'].get()}"
    data = []

    if iNoPhases==1:
        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Core Drawing", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, None, None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, None, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, "Part No:", sCore, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Core Dimensiones (Inches)", None, None, None, None, None, None, None, None, None],
            [None, None, "A", "C", "E", "-", "B", "D", "Clamp 'L'", "Steel type", None, None],
            [None, None, "Window", "Small Win", "Stack per", "Strip Width", "Overall Core", "Core Length", "Core Length", "'M'", None, None],
            [None, None, "Height", "Width", "Loop", None, "Height", "Withouth PB", "With PB", None, None, None],
            [None, None, f"{device.core.fields['dWindowHeight'].get():.3f}", f"{device.core.fields['dSmallWindowWidth'].get():.3f}", f"{device.core.fields['dStackPerLoop'].get():.3f}", f"{device.core.fields['dStripWidth'].get():.3f}", f"{device.core.fields['dOverallCoreHeight'].get():.3f}", f"{device.core.fields['dCoreLengthWithoutPB'].get():.3f}", f"{device.core.fields['dCoreLengthWithPB'].get():.3f}", device.core.fields['tLaminationType'].get().value, None, None],
            [None, None, None, None, "E bottom", None, None, "Tolerance On Dimensions", None, None, None, None],
            [None, None, None, None, "Stack per", None, None, device.core.fields['d_CoreTolerance'].get()[eCoreTolerance.A.value], None, None, None, None],
            [None, None, None, None, "Loop", None, None, device.core.fields['d_CoreTolerance'].get()[eCoreTolerance.C.value], None, None, None, None],
            [None, None, None, None, f"{device.core.fields['dEBottom'].get():.3f}", None, None, device.core.fields['d_CoreTolerance'].get()[eCoreTolerance.E.value], None, None, None, None],
            [None, None, None, None, None, None, None, device.core.fields['d_CoreTolerance'].get()[eCoreTolerance.STRIP.value], None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, "Weights (Lbs.) / Complete Unit", None, None, None],
            [None, None, None, None, None, None, None, None, sOneLoop, sTwoLoops, None, None],
            [None, None, None, None, None, None, None, None, f"{device.core.fields['dWeightOneLoop'].get():.0f}", f"{device.core.fields['dWeightTwoLoops'].get():.0f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, sPTI, sCoreManufacturer, None, None, None, None, None, None, None],
            [None, None, sInnerPer, sInnerPTI, sInnerCoreManufacturer, None, None, None, None, None, None, None],
            [None, None, sOuterPer, sOuterPTI, sOuterCoreManufacturer, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Notes:", None, None, None, None, None, None, None, None, None],
            [None, None, s_Notes[0], None, None, None, None, None, None, None, None, None],
            [None, None, s_Notes[1], None, None, None, None, None, None, None, None, None],
            [None, None, s_Notes[2], None, None, None, None, None, None, None, None, None],
            [None, None, s_Notes[3], None, None, None, None, None, None, None, None, None],
            [None, None, s_Notes[4], None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Max. Core Losses: ", None, sWLbTHz, None, None, sTestLoop, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, "Rev By/Date", "Design By:", device.fields['issued_by'].get(), None, None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, sDateDescr[0], "Design Date:", sformat_date, None, None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, sDateDescr[1], "DP Ver. No.", "Approved By", "Title:", "Core Drawing", "Revision No", None],
            [None, sRev[2], sRevDescr[2], None, None, sDateDescr[2], sDPNo, None, "Drawing No:", SDrawNo, iRev,None]
        ]
    else:
        
        tCoolingTypes = device.cooling.fields['tCoolingTypes'].get()
        tCoolingTypeE = device.cooling.fields['t_CoolingType'].get()
        if tCoolingTypeE[0]==eSingleCoolingType.COOL_ONAN:
            sCooling = "ONAN"
        elif tCoolingTypeE[0]==eSingleCoolingType.COOL_KNAN:
            sCooling = "KNAN"

        iSteps = device.cooling.fields['iCoolingStepsReal'].get()
        if iSteps>1:
            if device.cooling.fields['tProvONAF'].get()==eLogical.TRUE:
                sCooling += "/prov "
            else:
                sCooling += "/"

        if iSteps>1:
            if tCoolingTypeE[1]==eSingleCoolingType.COOL_ONAF:
                sCooling += "ONAF"
            elif tCoolingTypeE[1]==eSingleCoolingType.COOL_KNAF:
                sCooling += "KNAF"

        if iSteps>2:
            if tCoolingTypeE[2]==eSingleCoolingType.COOL_ONAF:
                sCooling += "/ONAF"
            elif tCoolingTypeE[2]==eSingleCoolingType.COOL_KNAF:
                sCooling += "/KNAF"
        iTempRise = device.cooling.fields['tTemperatureRises'].get().value
        sCooling += f" {device.fields['frequency'].get()} Hz "
        sCooling += f" {iTempRise} ° C"

        sSplitCore = (
            "Require 2 complete sets of small loops and large loops for complete core, (see notes below)"
            if bAmorphous and device.core.fields['dSteelWidth'].get() > 8.4
            else ""
            if bAmorphous
            else ""
            if device.core.fields['iNoLoops'].get()  == 1
            else "* SPLIT CORE - Require 2 complete sets of small loops and large loops for complete core *"
        )

        sCeroTolerance = ['']*4
        sCeroTolerance[0] = (
            '+0.138", -0" on Dimensions F, C '
            if tLamination == eLaminationType.AmHB1
            else ' +/- 0.02" on Dimensions A, C, E, F'
        )
        sCeroTolerance[1] = (
            '+0.188" (3/16"), -0" on Dimension A '
            if tLamination == eLaminationType.AmHB1
            else ' +/- 0.00315" on Strip Width (per loop)'
        )
        sCeroTolerance[2] = (
            "+ 0.0625'' / -0.0625'' on Dimension E "
            if tLamination == eLaminationType.AmHB1
            else ""
        )
        sCeroTolerance[3] = (
            '+0.160", -0" on Strip Width (per loop) '
            if tLamination == eLaminationType.AmHB1
            else ""
        )

        dStripWidth = device.core.fields['dStripWidth'].get()
        dloop1 = 0.0
        dloop2 = 0.0
        if tLamination in (eLaminationType.Am, eLaminationType.AmHB1):
            if dStripWidth in (5.6, 6.7, 8.4):
                sTitleWeight = ""
                sTitleWeightAm = ""
                sDescWeightAm = ""
                sSmLoopsAmT = ""
                sLgLoopsAmT = ""
                sTotalAmT = ""
                sSmLoopsAm = ""
                sLgLoopsAm = ""
                sTotalAm = ""
            else:
                dloop1, dloop2 = device.core.find_headers(dStripWidth)
                sTitleWeight = f"One set, core strip width {dloop1}" if dloop1!=0.0 else 0.0
                sTitleWeightAm =  f"Other set, core strip width {dloop2}" if dloop2!=0.0 else 0.0
                sDescWeightAm = "Weights (Lbs.) / set"
                sSmLoopsAmT = "Sm loops"
                sLgLoopsAmT = "Lg loops"
                sTotalAmT ="Total/Set"
                sSmLoopsAm = f"{device.core.fields['dWeightOneSmLoopOther'].get():.0f}"
                sLgLoopsAm = f"{device.core.fields['dWeightOneLgLoopOther'].get():.0f}"
                sTotalAm = f"{device.core.fields['dWeightOneLoopOther'].get():.0f}"
        else:
            sTitleWeight = ""
            sTitleWeightAm = ""
            sDescWeightAm = ""
            sSmLoopsAmT = ""
            sLgLoopsAmT = ""
            sTotalAmT = ""
            sSmLoopsAm = ""
            sLgLoopsAm = ""
            sTotalAm = ""

        sloop = ""
        if dloop1!=0.0:
            sloop = f"2 loops: 1 loop strip width {dloop1:.1f}"
        if dloop2!=0.0:
            sloop = f"{sloop} and another loop {dloop2:.1f}"

        sSm = ['']*2
        sLg = ['']*2
        sTotal = ['']*2
        if tLamination in (eLaminationType.Am, eLaminationType.AmHB1):
            dAux = 2*(device.core.fields['dSmallWindowWidth'].get()+device.core.fields['dWindowHeight'].get())
            sSm[0] = f"{dAux:.3f}''"
            sSm[1] = f"{((dAux/PI+2*device.core.fields['dStackPerLoop'].get())*PI):.3f}''"

            dAux = 2*(device.core.fields['dLargeWindowWidth'].get()+device.core.fields['dWindowHeight'].get())
            sLg[0] = f"{dAux:.3f}''"
            sLg[1] = f"{((dAux/PI+2*device.core.fields['dStackPerLoop'].get())*PI):.3f}''"

            sTotal[0] = "18.0'' Min."
            sTotal[1] = "132.0'' Max."

        sNotes = ['']*7
        sNotes[0] = sloop
        sNotes[1] = (
            "Gaps on the bottom"
            if bAmorphous
            else "Inner loops - gaps on top"
        )
        sNotes[2] = "" if bAmorphous else "CORE SHALL BE UNICORE"
        sNotes[3] = (
            ""
            if bAmorphous
            else 'Use PTI material specification "MS-05" for general characteristics of this core'
        )
        sNotes[4] = s_Notes[2]
        sNotes[5] = s_Notes[3]
        sNotes[6] = s_Notes[4]
        sNotes1 = "" if bAmorphous else "Outer loops - gaps on sides"

        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Core Drawing", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, "Standard Shell Type, Layer Winding", None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, sCooling, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, "L= D + PACKING BETWEEN LOOPS", None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, sSplitCore, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, "Part No:", sCore, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Core Dimensiones (Inches)", None, None, None, None, None, None, None, None, None],
            [None, None, "A", "F", "C", "E", "Core", "B outer loops", "D", "Clamp 'L'", "Steel type",  None],
            [None, None, "Window", "Small Win", "Large Win","Stack per", "Strip Width", "Overall Core", "Core Length", "Core Length", "'M'",  None],
            [None, None, "Height", "Width", "Width","Loop", None, "Height", "Withouth PB", "With PB", None,  None],
            [None, None, f"{device.core.fields['dWindowHeight'].get():.3f}", f"{device.core.fields['dSmallWindowWidth'].get():.3f}",f"{device.core.fields['dLargeWindowWidth'].get():.3f}", f"{device.core.fields['dStackPerLoop'].get():.3f}", f"{dStripWidth:.3f}", f"{device.core.fields['dOverallCoreHeight'].get():.3f}", f"{device.core.fields['dCoreLengthWithoutPB'].get():.3f}", f"{device.core.fields['dCoreLengthWithPB'].get():.3f}", device.core.fields['tLaminationType'].get().value,  None],
            [None, None, None, None, None, "E2", "E3", "B2 inner loops", "Tolerance On Dimensions", None, None, None],
            [None, None, None, None, None, "Stack in core", "Stack in core", "Overall Core", sCeroTolerance[0], None, None, None],
            [None, None, None, None, None, "joints (Sm loops)", "joints (Lg loops)", "Height", sCeroTolerance[1], None, None, None],
            [None, None, None, None, None, f"{device.core.fields['dStackCoreSmLoops'].get():.3f}", f"{device.core.fields['dStackCoreLgLoops'].get():.3f}", f"{device.core.fields['dOverallCoreHeightInner'].get():.3f}", sCeroTolerance[2], None, None, None],
            [None, None, None, None, None, None, None, None, sCeroTolerance[3], None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, sTitleWeight, None, None, None, sTitleWeightAm, None, None, None, None],
            [None, None, None, "Weights (Lbs.) / set", None, None, None, sDescWeightAm, None, None, None, None],
            [None, None, None, "Sm loops", "Lg loops", "Total/sets", None, sSmLoopsAmT, sLgLoopsAmT, sTotalAmT, None, None],
            [None, None, None, f"{device.core.fields['dWeightOneSmLoop'].get():.0f}", f"{device.core.fields['dWeightOneLgLoop'].get():.0f}", f"{device.core.fields['dWeightOneLoop'].get():.0f}", None, sSmLoopsAm, sLgLoopsAm, sTotalAm, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, "Weights (Lbs.) / Complete Unit", None, None, None, None, None, None, None, None],
            [None, None, None, "Sm loops", "Lg loops", "Total/Unit", None, None, None, None, None, None],
            [None, None, None, f"{device.core.fields['dWeightTwoSmLoops'].get():.0f}", f"{device.core.fields['dWeightTwoLgLoops'].get():.0f}", f"{device.core.fields['dWeightTwoLoops'].get():.0f}", None, None, None, None, None, None],
            [None, None, None, sSm[0], sLg[0], sTotal[0], None, None, None, None, None, None],
            [None, None, None, sSm[1], sLg[1], sTotal[1], None, None, None, None, None, None],
            [None, None, "Notes:", None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[0], None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[1], None, None, None, sNotes1, None, None, None, None, None],
            [None, None, sNotes[2], None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[3], None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[4], None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[5], None, None, None, None, None, None, None, None, None],
            [None, None, sNotes[6], None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, "CORE MANUFACTURER'S TESTING INSTRUCTIONS", None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Max. Core Losses: ", None, sWLbTHz, None, sTestLoop, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, None, "Rev By/Date", "Design By:", device.fields['issued_by'].get(),  None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, None, sDateDescr[0], "Design Date:", sformat_date,  None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, None, sDateDescr[1], "DP Ver. No.", "Approved By", "Title:  Core Drawing     Rev. #", None],
            [None, sRev[2], sRevDescr[2], None, None, None, sDateDescr[2], sDPNo, None, f"Drawing No:{SDrawNo}         {iRev}", None]
        ]
    
    return data

