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
from Definitions.enums import eConnectionType,eArrayType,eLogical,eBIL,eWarnings,eTransformerFamily,eWindingSelection,eTapExtremes,eSequence,eChangerCapacity,eMaterial,eLaminationType,eTypeConductor,eCoating,eDesignSpec,eCoolingType,eSingleCoolingType,eTemperatureRise
from Definitions.functions import address,ADDRESSZ,is_folder_open
from datetime import datetime

def make_engineering_data(device = None):

    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    pLCoil = pWindL.coils[0]
    pHCoil = pWindH.coils[0]
    pLCond = pLCoil.conductor[0]
    pHCond = pHCoil.conductor[0]

    bHaveTaps = pWindH.have_taps()
    tCoolingTypes = device.cooling.fields['tCoolingTypes'].get()
    tCoolingTypeE = device.cooling.fields['t_CoolingType'].get()
    iSteps = device.cooling.fields['iCoolingStepsReal'].get()
    if tCoolingTypeE[0]==eSingleCoolingType.COOL_ONAN:
        sCooling = "ONAN"
    elif tCoolingTypeE[0]==eSingleCoolingType.COOL_KNAN:
        sCooling = "KNAN"

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

    d_kVA = pWindH.fields['d_kVA'].get()
    bTapsFCBN = True if bHaveTaps and pWindH.changer.fields['tCapacity'].get() == eChangerCapacity.FCBN else False
    sCooling2 = f"Cooling Data {iTempRise}° ONAN"
    sTOR = "FCBN TOR °C" if bTapsFCBN else "TOR °C"
    sCurrentTemp = f"@ {device.cooling.fields['dCurrentTempRise'].get()}°C"
    sNewTemp = f"@ {device.cooling.fields['dNewTempRise'].get()}°C"
    sCurrentTempTap = sCurrentTemp if bTapsFCBN else ""
    sNewTempTap = sNewTemp if bTapsFCBN else ""
    sI2R = f"%IZ Summary @{iTempRise}°C"
    sTaps = "Taps Desciption" if bHaveTaps else "No Taps"
    sHVoltage1 = pWindH.fields['sVoltage1'].get()#2 Bushing" if pWindH.fields['iBil_Under'].get()==pWindH.fields['iBIL'].get().value else "Single Bushing"
    sLVoltage1 = pWindL.fields['sVoltage1'].get()#f'{pWindL.fields['dVoltage'].get()} /' if pWindL.fields['iNumBushings'].get() > 2 else None
    sLVoltage2 = pWindL.fields['sVoltage2'].get()#pWindL.fields['dVoltage'].get() / 2 if pWindL.fields['iNumBushings'].get() > 2 else pWindL.fields['dVoltage'].get()
    sUseFR3 = device.cooling.fields['sUseFR3'].get()

    s_BuildFactor = ['']*7
    s_BuildFactor[0] = f"{device.fields['dLVBuildFactor'].get():.1f}" if device.fields['dLVBuildFactor'].get()>0.0 else ""
    s_BuildFactor[1] = f"{device.fields['dHVBuildFactor'].get():.1f}" if device.fields['dHVBuildFactor'].get()>0.0 else ""
    s_BuildFactor[2] = f"{device.fields['dCoilRB'].get():.1f}" if device.fields['dCoilRB'].get()>0.0 else ""
    s_BuildFactor[3] = f"{device.fields['dCoreLossFactor'].get():.1f}" if device.fields['dCoreLossFactor'].get()>0.0 else ""
    s_BuildFactor[4] = f"{device.fields['dLoadLossFactor'].get():.1f}" if device.fields['dLoadLossFactor'].get()>0.0 else ""
    s_BuildFactor[5] = f"{device.fields['dNoseBuildFactor'].get():.1f}" if device.fields['dNoseBuildFactor'].get()>0.0 else ""
    s_BuildFactor[6] = f"{device.fields['dInsulationLengthFactor'].get():.1f}" if device.fields['dInsulationLengthFactor'].get()>0.0 else ""

    s_WeightsE = ['']*9
    s_WeightsE[0] = f"{pWindL.fields['dWeight'].get():.0f}" if pWindL.fields['dWeight'].get()>0.0 else ""
    s_WeightsE[1] = f"{pWindH.fields['dWeight'].get():.0f}" if pWindH.fields['dWeight'].get()>0.0 else ""
    s_WeightsE[2] = f"{device.core.fields['dWeightTwoLoops'].get():.0f}" if device.core.fields['dWeightTwoLoops'].get()>0.0 else ""
    s_WeightsE[3] = f"{device.fields['dWeightKr0022'].get():.1f}" if device.fields['dWeightKr0022'].get()>0.0 else ""
    s_WeightsE[4] = f"{device.fields['dWeightKr005'].get():.1f}" if device.fields['dWeightKr005'].get()>0.0 else ""
    s_WeightsE[5] = f"{device.fields['dWeightKr010'].get():.1f}" if device.fields['dWeightKr010'].get()>0.0 else ""
    s_WeightsE[6] = f"{device.fields['dWeightKr015'].get():.1f}" if device.fields['dWeightKr015'].get()>0.0 else ""
    s_WeightsE[7] = f"{device.fields['dWeightCrepe'].get():.1f}" if device.fields['dWeightCrepe'].get()>0.0 else ""
    s_WeightsE[8] = f"{device.core.fields['dWeightPressBoard'].get():.1f}" if device.core.fields['dWeightPressBoard'].get()>0.0 else ""
    s_CurrentTaps = ['']*6
    s_CurrentTaps[0] =f"{device.losses.fields['dLVWOLeadsCurrentTempTaps'].get():.0f}" if device.losses.fields['dLVWOLeadsCurrentTempTaps'].get()!=0 else ""
    s_CurrentTaps[1] =f"{device.losses.fields['dLVLeadsCurrentTempTaps'].get():.0f}" if device.losses.fields['dLVLeadsCurrentTempTaps'].get()!=0 else ""
    s_CurrentTaps[2] =f"{device.losses.fields['dTotalLVCurrentTempTaps'].get():.0f}" if device.losses.fields['dTotalLVCurrentTempTaps'].get()!=0 else ""
    s_CurrentTaps[3] =f"{device.losses.fields['dHVWOLeadsCurrentTempTaps'].get():.0f}" if device.losses.fields['dHVWOLeadsCurrentTempTaps'].get()!=0 else ""
    s_CurrentTaps[4] =f"{device.losses.fields['dHVLeadsCurrentTempTaps'].get():.0f}" if device.losses.fields['dHVLeadsCurrentTempTaps'].get()!=0 else ""
    s_CurrentTaps[5] =f"{device.losses.fields['dTotalHVCurrentTempTaps'].get():.0f}" if device.losses.fields['dTotalHVCurrentTempTaps'].get()!=0 else ""

    sI2RCurrentTaps = f"{device.losses.fields['dI2RCurrentTempTaps'].get():.0f}" if device.losses.fields['dI2RCurrentTempTaps'].get()!=0 else ""
    sI2RNewTaps = f"{device.losses.fields['dI2RNewTempTaps'].get():.0f}" if device.losses.fields['dI2RNewTempTaps'].get()!=0 else ""
    s_NewTaps = ['']*6
    s_NewTaps[0] =f"{device.losses.fields['dLVWOLeadsNewTempTaps'].get():.0f}" if device.losses.fields['dLVWOLeadsNewTempTaps'].get()!=0 else ""
    s_NewTaps[1] =f"{device.losses.fields['dLVLeadsNewTempTaps'].get():.0f}" if device.losses.fields['dLVLeadsNewTempTaps'].get()!=0 else ""
    s_NewTaps[2] =f"{device.losses.fields['dTotalLVNewTempTaps'].get():.0f}" if device.losses.fields['dTotalLVNewTempTaps'].get()!=0 else ""
    s_NewTaps[3] =f"{device.losses.fields['dHVWOLeadsNewTempTaps'].get():.0f}" if device.losses.fields['dHVWOLeadsNewTempTaps'].get()!=0 else ""
    s_NewTaps[4] =f"{device.losses.fields['dHVLeadsNewTempTaps'].get():.0f}" if device.losses.fields['dHVLeadsNewTempTaps'].get()!=0 else ""
    s_NewTaps[5] =f"{device.losses.fields['dTotalHVNewTempTaps'].get():.0f}" if device.losses.fields['dTotalHVNewTempTaps'].get()!=0 else ""

    dNl85C = 85 if device.guarantee.fields['bNLCorrected_to_85C'].get()==True else 20
    sNL85C = f"NL @ {dNl85C:.0f}°C,{device.core.fields['sRatedV'].get()}"
    sLL85C = f"LL @ {device.cooling.fields['dCurrentTempRise'].get():.0f}°C, Rated kVA and Tap"
    sResistanceTaps = "@ 20°C" if bTapsFCBN else ""
    
    sDoe2016 = device.fields['sPerEffDoE2016'].get()
    sC8021 = device.fields['sPerEff8021'].get()
    sC8023 = device.fields['sPerEff8023'].get()

    sDP = f"{device.fields['dLatestVersionNo'].get():.2f}" if device.fields['sDPSpecial'].get() != "M" else f"{device.fields['dLatestVersionNo'].get():.2f} M"
    sDwg = f"ED-{device.fields['so_number'].get()}"
    date = datetime.strptime(device.fields['original_issue_date'].get(), "%m/%d/%Y")
    sformat_date = date.strftime("%b %#d/%y")
    sAmpsParal = pWindL.fields['sAmpParall'].get() if pWindL.fields['sAmpParall'].get()!="" else "N/A"
    sEvaTotal = f"Total: $ {(device.costs.fields['dEvalNL'].get()+device.costs.fields['dEvalLL'].get()):.0f}" 
    sMinIZ = f"Min {device.guarantee.fields['dIZGuarantee'].get():.1f}%"
    sNLGuarantee = f"{device.guarantee.fields['dNLGuarantee'].get():.0f}" if device.guarantee.fields['dNLGuarantee'].get()>0 else "-"
    sLLGuarantee = f"{device.guarantee.fields['dLLGuarantee'].get():.0f}" if device.guarantee.fields['dLLGuarantee'].get()>0 else "-"
    sTotL = f"{(device.guarantee.fields['dNLGuarantee'].get()+device.guarantee.fields['dLLGuarantee'].get()):.0f}" if (device.guarantee.fields['dNLGuarantee'].get()+device.guarantee.fields['dLLGuarantee'].get())>0 else "-"

    sRev = ['']*3
    sRevDescr = ['']*3
    sDateDescr = ['']*3
    iRev = device.fields['i_RevisionWS'].get()[0]
    sRev[0] = 1 if iRev!=0 else ""
    sRev[1] = 2 if iRev>=2 else ""
    sRev[2] = 3 if iRev==3 else ""
    sRevDescr[0] = device.fields['s_DataDescr0'].get()[0] if iRev!=0 else ""
    sRevDescr[1] = device.fields['s_DataDescr0'].get()[1] if iRev>=2 else ""
    sRevDescr[2] = device.fields['s_DataDescr0'].get()[2] if iRev==3 else ""
    sDateDescr[0] = device.fields['s_DateDescr0'].get()[0] if iRev!=0 else ""
    sDateDescr[1] = device.fields['s_DateDescr0'].get()[1] if iRev>=2 else ""
    sDateDescr[2] = device.fields['s_DateDescr0'].get()[2] if iRev==3 else ""

    s_Taps = ['']*2
    if bHaveTaps:
        s_Taps[0] = f"{pWindH.changer.fields['iNumSteps'].get()} @ {(pWindH.changer.fields['dPerUp'].get()/pWindH.changer.taps_sup())}% ({pWindH.changer.fields['tCapacity'].get().value})"
        s_Taps[1] = f"{pWindH.changer.taps_sup()} - Up, {pWindH.changer.taps_inf()} - Down"
    sCustomSpec = f"Cust Spec:   {device.guarantee.fields['sCustomerSpec'].get()}"
    iNoPhases = device.fields['iNoPhases'].get()
    data = []

    if iNoPhases==1:
        
        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Engineering Data", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, "Standard Shell Type, Layer Winding", None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, sCooling, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, "Taps", "HV", "LV", None,None,None],
            [None, None, "SO #", "kVA", "Phases", "Type", pWindH.changer.get_Type() if bHaveTaps else "None", sHVoltage1, sLVoltage1,"HV-BIL", "LV-BIL",None],
            [None, None, device.fields['so_number'].get(), d_kVA[0], iNoPhases, device.fields['transformer_family'].get().name,pWindH.changer.fields['tCapacity'].get().value if bHaveTaps else None, pWindH.fields['dVoltage'].get(), sLVoltage2,  pWindH.fields['iBIL'].get().value, pWindL.fields['iBIL'].get().value, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "DoE (2016)", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sDoe2016, None, None, None, None, None, None, None],
            [None, None, "C802.1", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sC8021, None, None, None, None, None, None, None],
            [None, None, "C802.3", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sC8023, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Turns Data", None, None, None, None, "HV Amps", "LV Current Ratings", None, None, None],
            [None, None, "V/T", "LV Total", "HV Nom", "HV Total", None, "Ph Amps", "Amps Series", "Amps Parall", None, None],
            [None, None, device.fields['sVxT'].get(), pLCoil.fields['dTurns'].get(), pWindH.fields['dNomTurnsUI'].get(), pHCoil.fields['dTurns'].get(), None, pWindH.fields['sLineAmp'].get(), pWindL.fields['sLineAmp'].get(), sAmpsParal, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Core Data", None, None, None, None, None, None, None, None, None],
            [None, None, "Flux Density", None, None, "Stack", "Strip", "Steel Grade", "Net Area", "True Overcoil", None, None],
            [None, None, device.core.fields['sRatedPU'].get(), device.core.fields['sRatedV'].get(), None,f"{device.core.fields['dTotalStack'].get():.3f}", f"{device.core.fields['dSteelWidth'].get():.3f}", device.core.fields['tLaminationType'].get().value, device.core.fields['sArea'].get(), f"{device.core.fields['dTrueOverCoil'].get():.3f}", None, None],
            [None, "kL", f"{device.core.fields['dFluxDensityRatPer'].get():.3f}", f"{device.core.fields['dFluxDensityVolt'].get():.3f}", None, "Win Width", "Window Hgt", "Watts / Lb", "L", "Leg Centers", None, None],
            [None, "T",  f"{device.core.fields['dFluxDensityRatPerT'].get():.3f}",  f"{device.core.fields['dFluxDensityVoltT'].get():.3f}", None, f"{device.core.fields['dSmallWindowWidth'].get():.3f}", f"{device.core.fields['dWindowHeight'].get():.3f}",  f"{device.core.fields['dWattsLb'].get():.6f}", f"{device.core.fields['dCoreLengthWithPB'].get():.3f}", f"{device.core.fields['dLegCenters'].get():.3f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pLCond.fields['sSWConductor'].get()}:", pLCond.fields['sMatConductor'].get(), "High", "Wide", "Thick / turn", "Width / turn", "Area", "Density", None, None],
            [None, None, pLCond.fields['sDescription'].get(), pLCond.fields['sPapperConductor'].get(), pLCoil.fields['iHigh'].get(), pLCoil.fields['iWide'].get(), f"{pLCoil.fields['dCondThickTurn'].get():.4f}", f"{pLCoil.fields['dCondWdthTurn'].get():.3f}", f"{pLCond.fields['dAreaCond'].get():.6f}", f"{pLCond.fields['dDensity'].get():.0f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pHCond.fields['sSWConductor'].get()}:", pHCond.fields['sMatConductor'].get(), "High", "Wide", "Thick / turn", "Width / turn", "Area", "Density", None, None],
            [None, None, pHCond.fields['sDescription'].get(), pHCond.fields['sPapperConductor'].get(), pHCoil.fields['iHigh'].get(), pHCoil.fields['iWide'].get(), f"{pHCoil.fields['dCondThickTurn'].get():.4f}", f"{pHCoil.fields['dCondWdthTurn'].get():.3f}", f"{pHCond.fields['dAreaCond'].get():.6f}", f"{pHCond.fields['dDensity'].get():.0f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, "Calculated & Guaranteed Values", None, None, None, None, None, "Weights (lbs / Unit)", None, None],
            [None, None, None, sNL85C, None, None, sLL85C, None, None, "LV Cond",s_WeightsE[0], None],
            [None, None, None, "Core", "Load Loss", "Total Loss", "% IZ", "% Ex", None, "HV Cond", s_WeightsE[1], None],
            [None, None, "Calculated:", f"{device.losses.fields['dCoreLoss'].get():.0f}", f"{device.losses.fields['dTotalLLossStrayLead'].get():.0f}",  f"{device.losses.fields['dTotalLoss'].get():.0f}", f"{device.fields['dIZWithLeads'].get():.2f}", "-", None, "Core",  s_WeightsE[2], None],
            [None, None, "Guarantee:", sNLGuarantee, sLLGuarantee, sTotL, sMinIZ, device.guarantee.fields['sExcGuarantee'].get(), None, ".0022 TKr", s_WeightsE[3], None],
            [None, None, "Tested:", None, None, None, None, None, None, ".005 Kr", s_WeightsE[4], None],
            [None, None, "% Diff (Calc):", None, None, None, None, None, None, ".010 Kr", s_WeightsE[5], None],
            [None, None, None, None, None, None, None, None, None, ".015 Kr", s_WeightsE[6], None],
            [None, None, "Temp Rise Data (Rated kVA & Tap)", None, None, None, sCooling2, None, None, s_WeightsE[7], None],
            [None, None, "Rise °C", "ILV Grad °C", "HV Grad °C", "OLV Grad °C", "Max Watts", sTOR, None, "Pressboard", s_WeightsE[8], None],
            [None, None, iTempRise, f"{device.gradients.fields['dGradientFinal'].get()[0]:.1f}", f"{device.gradients.fields['dGradientFinal'].get()[1]:.1f}",  f"{device.gradients.fields['dGradientFinal'].get()[2]:.1f}", f"{device.losses.fields['dMaxWatts'].get():.0f}", f"{device.gradients.fields['dTORFinal'].get():.1f}", None, "Total C & C:",  f"{device.fields['dTotalCC'].get():.0f}", None],
            [None, None, None, sUseFR3, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Resistance and Loss Summary", None, None, None, None, None, None, None, None, None],
            [None, None, None, "Resistances (per Phase)", None, "Watts @ Rated Current", None, sResistanceTaps, None, "Stray Loss", None, None],
            [None, None, None, sCurrentTemp, sNewTemp, sCurrentTemp, sNewTemp, sCurrentTempTap, sNewTempTap, "Coil stray", f"{device.losses.fields['dCoilSray'].get():.0f}", None],
            [None, None, "LV w/o Lds:", f"{device.losses.fields['dLVWOLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dLVWOLeadsNewTempR'].get():.6f}", f"{device.losses.fields['dLVWOLeadsCurrentTempW'].get():.0f}", f"{device.losses.fields['dLVWOLeadsNewTempW'].get():.0f}", s_CurrentTaps[0], s_NewTaps[0], "Tank stray", f"{device.losses.fields['dTankStray'].get():.0f}", None],
            [None, None, "LV Leads:", f"{device.losses.fields['dLVLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dLVLeadsNewTempR'].get():.6f}",  f"{device.losses.fields['dLVLeadsCurrentTempW'].get():.0f}",  f"{device.losses.fields['dLVLeadsNewTempW'].get():.0f}", s_CurrentTaps[1], s_NewTaps[1], "Total stray",  f"{device.losses.fields['dTotalStray'].get():.0f}", None],
            [None, None, "Total LV:", f"{device.losses.fields['dTotalLVCurrentTempR'].get():.6f}", f"{device.losses.fields['dTotalLVNewTempR'].get():.6f}", f"{device.losses.fields['dTotalLVCurrentTempW'].get():.0f}", f"{device.losses.fields['dTotalLVNewTempW'].get():.0f}", s_CurrentTaps[2], s_NewTaps[2], "LV Lead Loss", f"{device.losses.fields['dLVLeadLoss'].get():.0f}", None],
            [None, None, "Total HV:", f"{device.losses.fields['dTotalHVCurrentTempR'].get():.6f}", f"{device.losses.fields['dTotalHVNewTempR'].get():.6f}", f"{device.losses.fields['dTotalHVCurrentTempW'].get():.0f}", f"{device.losses.fields['dTotalHVNewTempW'].get():.0f}", s_CurrentTaps[5], s_NewTaps[5], "Tot. L.Loss w/Stray & Lead",  None, None],
            [None, None, None, None, "Total I2R Loss:", f"{device.losses.fields['dI2RCurrentTemp'].get():.0f}", f"{device.losses.fields['dI2RNewTemp'].get():.0f}", sI2RCurrentTaps, sI2RNewTaps, f"{device.losses.fields['dTotalLLossStrayLead'].get():.0f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, sI2R, None, "Mean Turn Data", None, "Build Factors (%)", None, None, sTaps, None, None],
            [None, None, "Lead Factor", f"{device.fields['dLineLeadFactor'].get():.2f}", "Form Circ.", f"{device.fields['dFormMT'].get():.2f}", "LV RB",  s_BuildFactor[0], None, s_Taps[0], None, None],
            [None, None, "IX % - Coil ", f"{device.fields['dIXCoil'].get():.2f}", "ILV-MT", f"{device.fields['dILV_MT'].get():.2f}", "HV RB", s_BuildFactor[1], None, s_Taps[1], None, None],
            [None, None, "IR %", f"{device.fields['dIR'].get():.2f}", "LH-OT", f"{device.fields['dILH_OT'].get():.2f}", "Coil RB", s_BuildFactor[2], None, None, None, None],
            [None, None, "IZ % -  w/o Ld", f"{device.fields['dIZWithoutLeads'].get():.2f}", "HV-MT", f"{device.fields['dHV_MT'].get():.2f}", "NL", s_BuildFactor[3], None, "Calculated Evaluation ", None, None],
            [None, None, "IZ % - w/Ld", f"{device.fields['dIZWithLeads'].get():.2f}", "HV-OT", f"{device.fields['dHV_OT'].get():.2f}", "LL", s_BuildFactor[4], None, f"NL: ${device.guarantee.fields['dEvaluation_nl'].get():.2f}", f"$ {device.costs.fields['dEvalNL'].get():.0f}", None],
            [None, None, None, None, "OLV-MT", f"{device.fields['dOLV_MT'].get():.2f}", "Nose Build", s_BuildFactor[5], None, f"LL: ${device.guarantee.fields['dEvaluation_ll'].get():.2f}", f"$ {device.costs.fields['dEvalLL'].get():.0f}", None],
            [None, None, None, None, "Avg LV-MT", f"{device.fields['dLV_MT'].get():.2f}", "Ins Length", s_BuildFactor[6], None, sEvaTotal, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, "Rev By/Date", "Design By:", device.fields['issued_by'].get(), None, None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, sDateDescr[0], "Design Date:", sformat_date, None, None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, sDateDescr[1], "DP Ver. No.", "Approved By", "Title:",     "Engineering Data", "Revision No", None],
            [None, sRev[2], sRevDescr[2], None, None, sDateDescr[2], sDP, None, "Drawing No:", sDwg, iRev, None]
        ]
    else:
       
        sTempONAF = ['']*2
        tTempONAF = device.cooling.fields['tTempONAF'].get()
        steps = device.cooling.fields['iCoolingSteps'].get()
        sTempONAF[0] = tTempONAF[0].value
        sMVA1 = None
        bOverExc = device.cooling.fields['bOverExcitation'].get()==eLogical.TRUE
        if bOverExc:
            sTempONAF[1] = device.cooling.fields['tTempOExc'].get().value
            sMVA1 = device.cooling.fields['sMVAOExc'].get()

        sMVA = d_kVA[0] if iNoPhases==1 else pWindH.fields['skVA'].get()
        sTaps = f"Taps ({pWindH.changer.fields['sCapactity'].get() if bHaveTaps else ''})"
        type_connectionHV = pWindH.fields['type_connection'].get()
        type_connectionLV = pWindL.fields['type_connection'].get()
        ground_YHV = pWindH.fields['ground_Y'].get()
        sConnHV = "∆" if type_connectionHV==eConnectionType.DELTA else ("Y" if type_connectionHV==eConnectionType.WYE and ground_YHV==eLogical.FALSE else "")
        sConnLV = "∆" if type_connectionLV==eConnectionType.DELTA else "Y"
        # sGroundHV = f"{pWindH.fields['dLineVoltage'].get():.0f}GY / " if type_connectionHV==eConnectionType.WYE and ground_YHV==eLogical.TRUE else ""
        # sHVoltage1 = f"{sGroundHV}{pWindH.fields['dVoltage'].get():.0f} {sConnHV}"
        sHVoltage1 = (
            f"{pWindH.fields['dLineVoltage'].get():.0f}"
            + (" ∆" if type_connectionHV==eConnectionType.DELTA else (" Y" if ground_YHV==eLogical.FALSE else " GY"))
            + (" " + f"/ {pWindH.fields['dVoltage'].get():.0f}" if type_connectionHV!=eConnectionType.DELTA else "")
        )
        sLAux = "" if type_connectionLV==eConnectionType.DELTA else f"/ {pWindL.fields['dVoltage'].get():.0f}"
        sLVoltage1 = f"{pWindL.fields['dLineVoltage'].get():.0f} {sConnLV} {sLAux}"
        dPerUp = pWindH.changer.fields['dPerUp'].get()
        sTypeTaps = pWindH.changer.fields['sPerTaps'].get() if bHaveTaps else None
    
        s_KVA = ['']*3
        s_KVA[0] = f"{d_kVA[0]:.1f}"if d_kVA[0]!=0.0 else ""
        s_KVA[1] = f"{d_kVA[1]:.1f}"if steps>=2 and d_kVA[1]!=0.0 else ""
        s_KVA[2] = f"{d_kVA[2]:.1f}"if steps==3 and d_kVA[2]!=0.0 else ""
    
        s_KVAONAF = ['']*3
        if bOverExc:
            d_KVAONAF = device.cooling.fields['dMVAOExc'].get()
            s_KVAONAF[0] = f"{(d_KVAONAF[0]):.1f}"if d_KVAONAF[0]!=0.0 else ""
            s_KVAONAF[1] = f"{(d_KVAONAF[1]):.1f}"if steps>=2 and d_KVAONAF[1]!=0.0 else ""
            s_KVAONAF[2] = f"{(d_KVAONAF[2]):.1f}"if steps==3 and d_KVAONAF[2]!=0.0 else ""

        iStepsHV = pWindH.get_noSteps()-1
        iPosNomHV = pWindH.fields['iNomPos'].get()
        pdPhaseAmpHV = pWindH.fields['pdPhaseAmp'].get()
        pdPhaseAmpHV1 = pWindH.fields['pdPhaseAmp1'].get()
        pdPhaseAmpHV2 = pWindH.fields['pdPhaseAmp2'].get()
        pdLineAmpHV = pWindH.fields['pdLineAmp'].get()
        pdLineAmpHV1 = pWindH.fields['pdLineAmp1'].get()
        pdLineAmpHV2 = pWindH.fields['pdLineAmp2'].get()
        pdPhaseAmpOExcHV = pWindH.fields['pdPhaseAmpOExc'].get()
        pdPhaseAmpOExcHV1 = pWindH.fields['pdPhaseAmpOExc1'].get()
        pdPhaseAmpOExcHV2 = pWindH.fields['pdPhaseAmpOExc2'].get()
        pdLineAmpOExcHV = pWindH.fields['pdLineAmpOExc'].get()
        pdLineAmpOExcHV1 = pWindH.fields['pdLineAmpOExc1'].get()
        pdLineAmpOExcHV2 = pWindH.fields['pdLineAmpOExc2'].get()
        iStepsLV = pWindL.get_noSteps()-1
        iPosNomLV = pWindL.fields['iNomPos'].get()
        pdPhaseAmpLV = pWindL.fields['pdPhaseAmp'].get()
        pdPhaseAmpLV1 = pWindL.fields['pdPhaseAmp1'].get()
        pdPhaseAmpLV2 = pWindL.fields['pdPhaseAmp2'].get()
        pdLineAmpLV = pWindL.fields['pdLineAmp'].get()
        pdLineAmpLV1 = pWindL.fields['pdLineAmp1'].get()
        pdLineAmpLV2 = pWindL.fields['pdLineAmp2'].get()
        pdPhaseAmpOExcLV = pWindL.fields['pdPhaseAmpOExc'].get()
        pdPhaseAmpOExcLV1 = pWindL.fields['pdPhaseAmpOExc1'].get()
        pdPhaseAmpOExcLV2 = pWindL.fields['pdPhaseAmpOExc2'].get()
        pdLineAmpOExcLV = pWindL.fields['pdLineAmpOExc'].get()
        pdLineAmpOExcLV1 = pWindL.fields['pdLineAmpOExc1'].get()
        pdLineAmpOExcLV2 = pWindL.fields['pdLineAmpOExc2'].get()

        s_Rise00 = ['']*6
        s_Rise00[0] = f"{pdLineAmpHV[iPosNomHV]:.2f}" if pdLineAmpHV[iPosNomHV]>0.0 else ""
        s_Rise00[1] = f"{pdPhaseAmpHV[iPosNomHV]:.2f}" if pdPhaseAmpHV[iPosNomHV]>0.0 else ""
        s_Rise00[2] = f"{pdLineAmpHV[iStepsHV]:.2f}" if bTapsFCBN and pdLineAmpHV[iStepsHV]>0.0 else ""
        s_Rise00[3] = f"{pdPhaseAmpHV[iStepsHV]:.2f}" if bTapsFCBN and pdPhaseAmpHV[iStepsHV]>0.0 else ""
        s_Rise00[4] = f"{pdLineAmpLV[iPosNomLV]:.2f}" if pdLineAmpLV[iPosNomLV]>0.0 else ""
        s_Rise00[5] = f"{pdPhaseAmpLV[iPosNomLV]:.2f}" if pdPhaseAmpLV[iPosNomLV]>0.0 else ""

        s_Rise01 = ['']*6
        s_Rise01[0] = f"{pdLineAmpHV1[iPosNomHV]:.2f}" if steps >= 2 and pdLineAmpHV1[iPosNomHV]>0.0 else ""
        s_Rise01[1] = f"{pdPhaseAmpHV1[iPosNomHV]:.2f}" if steps >= 2 and pdPhaseAmpHV1[iPosNomHV]>0.0 else ""
        s_Rise01[2] = f"{pdLineAmpHV1[iStepsHV]:.2f}" if steps >= 2 and bTapsFCBN and pdLineAmpHV1[iStepsHV]>0.0 else ""
        s_Rise01[3] = f"{pdPhaseAmpHV1[iStepsHV]:.2f}" if steps >= 2 and bTapsFCBN and pdPhaseAmpHV1[iStepsHV]>0.0 else ""
        s_Rise01[4] = f"{pdLineAmpLV1[iPosNomLV]:.2f}" if steps >= 2 and pdLineAmpLV1[iPosNomLV]>0.0 else ""
        s_Rise01[5] = f"{pdPhaseAmpLV1[iPosNomLV]:.2f}" if steps >= 2 and pdPhaseAmpLV1[iPosNomLV]>0.0 else ""

        s_Rise02 = ['']*6
        s_Rise02[0] = f"{pdLineAmpHV2[iPosNomHV]:.2f}" if steps >= 3 and pdLineAmpHV2[iPosNomHV]>0.0 else ""
        s_Rise02[1] = f"{pdPhaseAmpHV2[iPosNomHV]:.2f}" if steps >= 3 and pdPhaseAmpHV2[iPosNomHV]>0.0 else ""
        s_Rise02[2] = f"{pdLineAmpHV2[iStepsHV]:.2f}" if steps >= 3 and bTapsFCBN and pdLineAmpHV2[iStepsHV]>0.0 else ""
        s_Rise02[3] = f"{pdPhaseAmpHV2[iStepsHV]:.2f}" if steps >= 3 and bTapsFCBN and pdPhaseAmpHV2[iStepsHV]>0.0 else ""
        s_Rise02[4] = f"{pdLineAmpLV2[iPosNomLV]:.2f}" if steps >= 3 and pdLineAmpLV2[iPosNomLV]>0.0 else ""
        s_Rise02[5] = f"{pdPhaseAmpLV2[iPosNomLV]:.2f}" if steps >= 3 and pdPhaseAmpLV2[iPosNomLV]>0.0 else ""

        s_Rise10 = ['']*6
        if bOverExc:
            s_Rise10[0] = f"{pdLineAmpOExcHV[iPosNomHV]:.2f}" if pdLineAmpOExcHV[iPosNomHV]>0.0 else ""
            s_Rise10[1] = f"{pdPhaseAmpOExcHV[iPosNomHV]:.2f}" if pdPhaseAmpOExcHV[iPosNomHV]>0.0 else ""
            s_Rise10[2] = f"{pdLineAmpOExcHV[iStepsHV]:.2f}" if bTapsFCBN and pdLineAmpOExcHV[iStepsHV]>0.0 else ""
            s_Rise10[3] = f"{pdPhaseAmpOExcHV[iStepsHV]:.2f}" if bTapsFCBN and pdPhaseAmpOExcHV[iStepsHV]>0.0 else ""
            s_Rise10[4] = f"{pdLineAmpOExcLV[iPosNomLV]:.2f}" if pdLineAmpOExcLV[iPosNomLV]>0.0 else ""
            s_Rise10[5] = f"{pdPhaseAmpOExcLV[iPosNomLV]:.2f}" if pdPhaseAmpOExcLV[iPosNomLV]>0.0 else ""

        s_Rise11 = ['']*6
        if bOverExc:
            s_Rise11[0] = f"{pdLineAmpOExcHV1[iPosNomHV]:.2f}" if steps >= 2 and pdLineAmpOExcHV1[iPosNomHV]>0.0 else ""
            s_Rise11[1] = f"{pdPhaseAmpOExcHV1[iPosNomHV]:.2f}" if steps >= 2 and pdPhaseAmpOExcHV1[iPosNomHV]>0.0 else ""
            s_Rise11[2] = f"{pdLineAmpOExcHV1[iStepsHV]:.2f}" if steps >= 2 and bTapsFCBN and pdLineAmpOExcHV1[iStepsHV]>0.0 else ""
            s_Rise11[3] = f"{pdPhaseAmpOExcHV1[iStepsHV]:.2f}" if steps >= 2 and bTapsFCBN and pdPhaseAmpOExcHV1[iStepsHV]>0.0 else ""
            s_Rise11[4] = f"{pdLineAmpOExcLV1[iPosNomLV]:.2f}" if steps >= 2 and pdLineAmpOExcLV1[iPosNomLV]>0.0 else ""
            s_Rise11[5] = f"{pdPhaseAmpOExcLV1[iPosNomLV]:.2f}" if steps >= 2 and pdPhaseAmpOExcLV1[iPosNomLV]>0.0 else ""

        s_Rise12 = ['']*6
        if bOverExc:
            s_Rise12[0] = f"{pdLineAmpOExcHV2[iPosNomHV]:.2f}" if steps >= 3 and pdLineAmpOExcHV2[iPosNomHV]>0.0 else ""
            s_Rise12[1] = f"{pdPhaseAmpOExcHV2[iPosNomHV]:.2f}" if steps >= 3 and pdPhaseAmpOExcHV2[iPosNomHV]>0.0 else ""
            s_Rise12[2] = f"{pdLineAmpOExcHV2[iStepsHV]:.2f}" if steps >= 3 and bTapsFCBN and pdLineAmpOExcHV2[iStepsHV]>0.0 else ""
            s_Rise12[3] = f"{pdPhaseAmpOExcHV2[iStepsHV]:.2f}" if steps >= 3 and bTapsFCBN and pdPhaseAmpOExcHV2[iStepsHV]>0.0 else ""
            s_Rise12[4] = f"{pdLineAmpOExcLV2[iPosNomLV]:.2f}" if steps >= 3 and pdLineAmpOExcLV2[iPosNomLV]>0.0 else ""
            s_Rise12[5] = f"{pdPhaseAmpOExcLV2[iPosNomLV]:.2f}" if steps >= 3 and pdPhaseAmpOExcLV2[iPosNomLV]>0.0 else ""

        bFocedCooling = True if tCoolingTypes in (eCoolingType.COOL_ONAN_ONAF,eCoolingType.COOL_ONAN_ONAF_ONAF,eCoolingType.COOL_KNAN_KNAF,eCoolingType.COOL_KNAN_KNAF_KNAF) else False
        sDensityONAFLV = f"{pLCond.fields['dDensityONAF'].get():.0f}" if bFocedCooling else ""
        sDensityONAFHV = f"{pHCond.fields['dDensityONAF'].get():.0f}" if bFocedCooling else ""

        sDensityONANTaps = ""
        sDensityONAFTaps = ""
        if bTapsFCBN:
            sDensityONANTaps= f"{pHCond.fields['dDensityFCBN'].get():.0f}" 
            sDensityONAFTaps= f"{pHCond.fields['dDensityFCBNONAF'].get():.0f}" if bFocedCooling else ""

        tTemperatureRise = device.cooling.fields['tTemperatureRise'].get()
        tTemperatureRiseK4 = device.cooling.fields['tTemperatureRiseK4'].get()
        sTempRiseCool = ['']*3
        sTempRiseCool[0] = f"{tTemperatureRise[0].value} ° C {tCoolingTypeE[0].value}"
        if device.cooling.fields['tK4Factor'].get()!=eLogical.TRUE:
            if steps>=2:
                sTempRiseCool[1] = f"{tTemperatureRise[1].value} ° C {tCoolingTypeE[1].value}{steps-1}"
            if steps>=3:
                sTempRiseCool[2] = f"{tTemperatureRise[2].value} ° C {tCoolingTypeE[2].value}{steps-1}"
        else:
            if steps>=1:
                sTempRiseCool[1] = f"{tTemperatureRiseK4[0].value} ° C PER K4 FACTOR"
            if steps>=2:
                sTempRiseCool[2] = f"{tTemperatureRiseK4[1].value} ° C PER K4 FACTOR"
    
        dLossMaxWatts = [0.0]*3
        sLossMaxWatts = ['']*3
        # dLossMaxWatts[0] = (
        #     device.fields['dONANLosses'].get()
        #     if device.fields['dONANLosses'].get() > 0.0
        #     else round((device.losses.fields['dTotalLoss'].get() + (device.losses.fields['dI2RCurrentTempTaps'].get() - device.losses.fields['dI2RCurrentTemp'].get())), -1)
        #     if bTapsFCBN
        #     else round(device.losses.fields['dTotalLoss'].get(), -1)
        # )
        if device.cooling.fields['tK4Factor'].get()==eLogical.TRUE:
            dLossMaxWatts[0] = device.losses.fields['dMaxWatts'].get()
            dLossMaxWatts[1] = device.mechanical.fields['d_kW1'].get()[0]*1000
            dLossMaxWatts[2] = device.mechanical.fields['d_kW2'].get()[0]*1000
        else:
            dLossMaxWatts[0] = device.mechanical.fields['d_kW1'].get()[0]*1000
            dLossMaxWatts[1] = device.mechanical.fields['d_kW2'].get()[1]*1000

        sLossMaxWatts[0] = f"{dLossMaxWatts[0]:.0f}" if dLossMaxWatts[0]!=0.0 else ""
        sLossMaxWatts[1] = f"{dLossMaxWatts[1]:.0f}" if dLossMaxWatts[1]!=0.0 else ""
        sLossMaxWatts[2] = f"{dLossMaxWatts[2]:.0f}" if dLossMaxWatts[2]!=0.0 else ""

        dFCBNTOR = [0.0]*3
        sFCBNTOR = ['']*3
        dFCBNTOR[0] = device.gradients.fields['dTORFinal'].get()
        if device.cooling.fields['tK4Factor'].get()==eLogical.TRUE:
            dFCBNTOR[1] = device.cooling.fields['dTORONAF'].get()[0]
            dFCBNTOR[2] = device.cooling.fields['dTORONAF'].get()[1]
        else:
            dFCBNTOR[1] = device.cooling.fields['dTORONAF'].get()[1]
            dFCBNTOR[2] = device.cooling.fields['dTORONAF'].get()[2]
        sFCBNTOR[0] = f"{dFCBNTOR[0]:.1f} °" if dFCBNTOR[0]!=0.0 else ""
        sFCBNTOR[1] = f"{dFCBNTOR[1]:.1f} °" if dFCBNTOR[1]!=0.0 else ""
        sFCBNTOR[2] = f"{dFCBNTOR[2]:.1f} °" if dFCBNTOR[2]!=0.0 else ""

        sPS = ['']*3
        sPS[0] = "-"
        sPS[1] = (
            f"{(dLossMaxWatts[1] / dLossMaxWatts[0] * dFCBNTOR[0] / dFCBNTOR[1]):.2f}"
            if dLossMaxWatts[1] > 0
               and dLossMaxWatts[0] != 0
               and dFCBNTOR[1] != 0
            else ""
        )
        sPS[2] = (
            f"{(dLossMaxWatts[2] / dLossMaxWatts[0] * dFCBNTOR[0] / dFCBNTOR[2]):.2f}"
            if dLossMaxWatts[2] > 0
               and dLossMaxWatts[0] != 0
               and dFCBNTOR[2] != 0
            else ""
        )

        sShortCircuit = ['']*2
        sShortCircuit[0] = f"{device.fields['dShortCircLV'].get():.2f}" if device.fields['dShortCircLV'].get()!=0.0 else "N/A"
        sShortCircuit[1] = f"{device.fields['dShortCircHV'].get():.2f}" if device.fields['dShortCircHV'].get()!=0.0 else "N/A"
        sNeutral = ['']*2
        sNeutral[0] = f"{device.fields['dNeutralLV'].get():.2f}" if device.fields['dNeutralLV'].get()!=0.0 else "N/A"
        sNeutral[1] = f"{device.fields['dNeutralHV'].get():.2f}" if device.fields['dNeutralHV'].get()!=0.0 else "N/A"

        sGr = f"Grd {device.fields['dTFMaxC88M90212_GrAL'].get()}°C / {device.fields['dTFMaxC88M90212_GrCU'].get()}°C"
        sGround = ['']*2
        sGround[0] = f"{device.fields['dFinalGroundLV'].get():.2f}" if device.fields['dFinalGroundLV'].get()!=0.0 else "N/A"
        sGround[1] = f"{device.fields['dFinalGroundHV'].get():.2f}" if device.fields['dFinalGroundHV'].get()!=0.0 else "N/A"
        sTr = f"X'fmr {device.fields['dTFMaxC88M901041_TraAL'].get()}°C / {device.fields['dTFMaxC88M901041_TraCU'].get()}°C"
        sTrans = ['']*2
        sTrans[0] = f"{device.fields['dFinalTransLV'].get():.2f}" if device.fields['dFinalTransLV'].get()!=0.0 else "N/A"
        sTrans[1] = f"{device.fields['dFinalTransHV'].get():.2f}" if device.fields['dFinalTransHV'].get()!=0.0 else "N/A"
        sMVAKVA = "MVA" if device.cooling.fields['iCoolingStepsReal'].get()>1 else "kVA"

        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Engineering Data", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, "Standard Shell Type, Layer Winding", None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, sCooling, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, sMVAKVA, None, None, None, None, None, None,None,None],
            [None, None, sTempONAF[0], sMVA, "Phases", "Type",sTaps, "HV", "LV","HV-BIL", "LV-BIL",None],
            [None, None, sTempONAF[1], sMVA1, iNoPhases, device.fields['transformer_family'].get().name, sTypeTaps, sHVoltage1, sLVoltage1,  pWindH.fields['iBIL'].get().value, pWindL.fields['iBIL'].get().value, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Rise", "kVA", "HV Line Amps", "HV Ph Amps", None, "HV Line Amps", "HV Ph Amps", "LV Line Amps", "LV Ph Amps", None],
            [None, None, sTempONAF[0], s_KVA[0], s_Rise00[0], s_Rise00[1],"FCBN", s_Rise00[2], s_Rise00[3], s_Rise00[4], s_Rise00[5], None],
            [None, None, None, s_KVA[1], s_Rise01[0], s_Rise01[1], None, s_Rise01[2], s_Rise01[3], s_Rise01[4], s_Rise01[5], None],
            [None, None, None, s_KVA[2], s_Rise02[0], s_Rise02[1], None, s_Rise02[2], s_Rise02[3], s_Rise02[4], s_Rise02[5], None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, sTempONAF[1], s_KVAONAF[0], s_Rise10[0], s_Rise10[1], "FCBN", s_Rise10[2], s_Rise10[3], s_Rise10[4], s_Rise10[5], None],
            [None, None, None, s_KVAONAF[1], s_Rise11[0], s_Rise11[1], None, s_Rise11[2], s_Rise11[3], s_Rise11[4], s_Rise11[5], None],
            [None, None, None, s_KVAONAF[2], s_Rise12[0], s_Rise12[1], None, s_Rise12[2], s_Rise12[3], s_Rise12[4], s_Rise12[5], None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "V/T", "Core Data", None, None, None, None, None, None, None, None],
            [None, None, device.fields['sVxT'].get(), "Flux Density", None, "Stack", "Strip", "Steel Grade", "Net Area", "L", None, None],
            [None, None, None, device.core.fields['sRatedPU'].get(), device.core.fields['sRatedV'].get(), f"{device.core.fields['dTotalStack'].get():.3f}", f"{device.core.fields['dSteelWidth'].get():.3f}", device.core.fields['tLaminationType'].get().value, device.core.fields['sArea'].get(), f"{device.core.fields['dCoreLengthWithPB'].get():.3f}", None, None],
            [None, None, "kL", f"{device.core.fields['dFluxDensityRatPer'].get():.3f}", f"{device.core.fields['dFluxDensityVolt'].get():.3f}",  "Win Width",None, "Window Hgt", "Watts / Lb", "Leg Centers", "True Overcoil",  None],
            [None, None, "T",  f"{device.core.fields['dFluxDensityRatPerT'].get():.3f}",  f"{device.core.fields['dFluxDensityVoltT'].get():.3f}", f"{device.core.fields['dSmallWindowWidth'].get():.3f}",f"{device.core.fields['dLargeWindowWidth'].get():.3f}" , f"{device.core.fields['dWindowHeight'].get():.3f}",  f"{device.core.fields['dWattsLb'].get():.6f}", f"{device.core.fields['dLegCenters'].get():.3f}",f"{device.core.fields['dTrueOverCoil'].get():.3f}" ,  None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pLCond.fields['sSWConductor'].get()}:", pLCond.fields['sMatConductor'].get(), "High x Wide", "Thick / turn", "Width / turn", "Area", "ONAN Density", "ONAF Density", None, None],
            [None, None, pLCond.fields['sDescription'].get(), pLCond.fields['sPapperConductor'].get(), f"{pLCoil.fields['iHigh'].get()} x {pLCoil.fields['iWide'].get()}", f"{pLCoil.fields['dCondThickTurn'].get():.4f}", f"{pLCoil.fields['dCondWdthTurn'].get():.3f}", f"{pLCond.fields['dAreaCond'].get():.6f}", f"{pLCond.fields['dDensity'].get():.0f}", sDensityONAFLV, None],
            [None, None, None, None, None, None, None, None, f"@ {sTempONAF[0]} °C Rise", None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pHCond.fields['sSWConductor'].get()}:", pHCond.fields['sMatConductor'].get(), "High x Wide", "Thick / turn", "Width / turn", "Area", "ONAN Density", "ONAF Density", None, None],
            [None, None, pHCond.fields['sDescription'].get(), pHCond.fields['sPapperConductor'].get(), f"{pHCoil.fields['iHigh'].get()} x {pHCoil.fields['iWide'].get()}", f"{pHCoil.fields['dCondThickTurn'].get():.4f}", f"{pHCoil.fields['dCondWdthTurn'].get():.3f}", f"{pHCond.fields['dAreaCond'].get():.6f}", f"{pHCond.fields['dDensity'].get():.0f}", sDensityONAFHV, None],
            [None, None, None, None, None, None, None,  "@FCBN:" if bTapsFCBN else None, sDensityONANTaps, sDensityONAFTaps, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, "Calculated & Guaranteed Values", None, None, None, None, None, None, None, None],
            [None, None, None, sNL85C, None, None, sLL85C, None, None, "Weights (lbs / Unit)", None,None],
            [None, None, None, "Core", "Load Loss", "Total Loss", "% IZ", "% Ex", None, "LV Cond",s_WeightsE[0], None],
            [None, None, "Calculated:", f"{device.losses.fields['dCoreLoss'].get():.0f}", f"{device.losses.fields['dTotalLLossStrayLead'].get():.0f}",  f"{device.losses.fields['dTotalLoss'].get():.0f}", f"{device.fields['dIZWithLeads'].get():.2f}", f"{device.fields['dPerExc'].get():.3f}", None,  "HV Cond", s_WeightsE[1], None],
            [None, None, "Guarantee:", sNLGuarantee, sLLGuarantee, sTotL, sMinIZ, device.guarantee.fields['sExcGuarantee'].get(), None, "Core",  s_WeightsE[2], None],
            [None, None, None, None, None, None, None, None, None, ".0022 TKr", s_WeightsE[3], None],
            [None, None, "Standard", "Calc. Eff.", "Req'd Eff.", None, None, None, None, ".005 Kr", s_WeightsE[4], None],
            [None, None, "Doe (2016)", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sDoe2016, None, None, None, None, ".010 Kr", s_WeightsE[5], None],
            [None, None, "C802.1", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sC8021, None, None, None, None,"Crepe", s_WeightsE[7], None],
            [None, None, "C802.3", f"{100*device.fields['dPerEfficiency'].get():.3f}%", sC8023, None, None, None, None, "Pressboard", s_WeightsE[8], None],
            [None, None, None, None, None, None, None, None, None, "Total C & C:",  f"{device.fields['dTotalCC'].get():.0f}", None],
            [None, None, "Temp Rise Data (Rated kVA & Tap)", None, None, "Cooling Data", None, "Max Watts", sTOR, "P/S (2.8 max)", None, None],
            [None, None, "Rise °C", "LV Grad °C", "HV Grad °C",  sTempRiseCool[0],None,  sLossMaxWatts[0], sFCBNTOR[0], sPS[0], None, None],
            [None, None, sTempONAF[0], f"{device.gradients.fields['dGradientFinal'].get()[0]:.2f}", f"{device.gradients.fields['dGradientFinal'].get()[1]:.2f}", sTempRiseCool[1],None, sLossMaxWatts[1],  sFCBNTOR[1], sPS[1], None, None],
            [None, None, None, sUseFR3, None, sTempRiseCool[2],None,  sLossMaxWatts[2],  sFCBNTOR[2], sPS[2], None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Resistance and Loss Summary", None, None, None, None, None, None, None, None, None],
            [None, None, None, "Resistances (per Phase)", None, "Watts @ Rated Current", None, sResistanceTaps, None, "Stray Loss", None, None],
            [None, None, None, sCurrentTemp, sNewTemp, sCurrentTemp, sNewTemp, sCurrentTempTap, sNewTempTap, "Coil stray", f"{device.losses.fields['dCoilSray'].get():.0f}", None],
            [None, None, "LV w/o Lds:", f"{device.losses.fields['dLVWOLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dLVWOLeadsNewTempR'].get():.6f}", f"{device.losses.fields['dLVWOLeadsCurrentTempW'].get():.0f}", f"{device.losses.fields['dLVWOLeadsNewTempW'].get():.0f}", s_CurrentTaps[0], s_NewTaps[0], "Tank stray", f"{device.losses.fields['dTankStray'].get():.0f}", None],
            [None, None, "LV Leads:", f"{device.losses.fields['dLVLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dLVLeadsNewTempR'].get():.6f}",  f"{device.losses.fields['dLVLeadsCurrentTempW'].get():.0f}",  f"{device.losses.fields['dLVLeadsNewTempW'].get():.0f}", s_CurrentTaps[1], s_NewTaps[1], "Total stray",  f"{device.losses.fields['dTotalStray'].get():.0f}", None],
            [None, None, "Total LV:", f"{device.losses.fields['dTotalLVCurrentTempR'].get():.6f}", f"{device.losses.fields['dTotalLVNewTempR'].get():.6f}", f"{device.losses.fields['dTotalLVCurrentTempW'].get():.0f}", f"{device.losses.fields['dTotalLVNewTempW'].get():.0f}", s_CurrentTaps[2], s_NewTaps[2], "LV Lead Loss", f"{device.losses.fields['dLVLeadLoss'].get():.0f}", None],
            [None, None, "HV w/o Lds:", f"{device.losses.fields['dHVWOLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dHVWOLeadsNewTempR'].get():.6f}", f"{device.losses.fields['dHVWOLeadsCurrentTempW'].get():.0f}", f"{device.losses.fields['dHVWOLeadsNewTempW'].get():.0f}", s_CurrentTaps[3], s_NewTaps[0], None, None, None],
            [None, None, "HV Leads:", f"{device.losses.fields['dHVLeadsCurrentTempR'].get():.6f}", f"{device.losses.fields['dHVLeadsNewTempR'].get():.6f}",  f"{device.losses.fields['dHVLeadsCurrentTempW'].get():.0f}",  f"{device.losses.fields['dHVLeadsNewTempW'].get():.0f}", s_CurrentTaps[4], s_NewTaps[4], "Tot. L.Loss w/Stray & Lead", None, None],
            [None, None, "Total HV:", f"{device.losses.fields['dTotalHVCurrentTempR'].get():.6f}", f"{device.losses.fields['dTotalHVNewTempR'].get():.6f}", f"{device.losses.fields['dTotalHVCurrentTempW'].get():.0f}", f"{device.losses.fields['dTotalHVNewTempW'].get():.0f}", s_CurrentTaps[5], s_NewTaps[5], f"{device.losses.fields['dTotalLLossStrayLead'].get():.0f}",  None, None],
            [None, None, None, None, "Total I2R Loss:", f"{device.losses.fields['dI2RCurrentTemp'].get():.0f}", f"{device.losses.fields['dI2RNewTemp'].get():.0f}", sI2RCurrentTaps, sI2RNewTaps, None, None, None],
            [None, None, None, None, None, None, None, None, None, "Tap Description" if bHaveTaps else "NoTaps", None, None],
            [None, None, sI2R, None, "Mean Turn Data", None, "Build Factors (%)", None, None, s_Taps[0], None, None],
            [None, None, "Lead Factor", f"{device.fields['dLineLeadFactor'].get():.2f}", "Form Circ.", f"{device.fields['dFormMT'].get():.2f}", "LV RB",  s_BuildFactor[0], None, s_Taps[1], None, None],
            [None, None, "IX % - Coil ", f"{device.fields['dIXCoil'].get():.2f}", "LV-MT", f"{device.fields['dILV_MT'].get():.2f}", "HV RB", s_BuildFactor[1], None, "Temp. Rise after a short-circuit", None, None],
            [None, None, "IR %", f"{device.fields['dIR'].get():.2f}", "LV-OT", f"{device.fields['dILV_OT'].get():.2f}", "Coil RB", s_BuildFactor[2], None, "LV", "HV", None],
            [None, None, "IZ % -  w/o Ld", f"{device.fields['dIZWithoutLeads'].get():.2f}", "HL-OT", f"{device.fields['dILH_OT'].get():.2f}", "NL", s_BuildFactor[3], "Φ Short circuit Amp", sShortCircuit[0], sShortCircuit[1], None],
            [None, None, "IZ % - w/Ld", f"{device.fields['dIZWithLeads'].get():.2f}", "HV-MT", f"{device.fields['dHV_MT'].get():.2f}", "Resistance", s_BuildFactor[4], "@ Neutral",sNeutral[0], sNeutral[1], None],
            [None, None, "OWP %IZ", f"{device.fields['dOWPIZ'].get():.2f}", "HV-OT", f"{device.fields['dHV_OT'].get():.2f}", "Nose Build", s_BuildFactor[5], sGr, sGround[0], sGround[1],None],
            [None, None, None, None, "HV-Outer-OT", f"{device.fields['dHOL_OT'].get():.2f}", "Ins Length", s_BuildFactor[6], sTr, sTrans[0], sTrans[1], None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, None, "Rev By/Date", "Design By:", device.fields['issued_by'].get(),  None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, None, sDateDescr[0], "Design Date:", sformat_date,  None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, None, sDateDescr[1], "DP Ver. No.", "Approved By", "Title:  Engineering Data     Rev. #", None],
            [None, sRev[2], sRevDescr[2], None, None, None, sDateDescr[2], sDP, None, f"Drawing No:{sDwg}         {iRev}", None]
        ]
    
    return data

