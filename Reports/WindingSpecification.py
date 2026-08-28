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
import re
from Resources.resources import resource_path
from Definitions.enums import eConnectionType,eArrayType,eLogical,eBIL,eTransformerFamily,eWindingSelection,eTapExtremes,eSequence,eChangerCapacity,eMaterial,eLaminationType,eTypeConductor,eCoating,eDesignSpec,eCoolingType,eSingleCoolingType,eTemperatureRise,eWarnings
from Definitions.functions import address,ADDRESSZ,is_folder_open
from fractions import Fraction
import math

def make_winding_specification(device = None):
    
    tCoolingTypeE = device.cooling.fields["t_CoolingType"].get()
    if tCoolingTypeE[0]==eSingleCoolingType.COOL_ONAN:
        sCooling = "ONAN"
    elif tCoolingTypeE[0]==eSingleCoolingType.COOL_KNAN:
        sCooling = "KNAN"

    iSteps = device.cooling.fields["iCoolingStepsReal"].get()
    if iSteps>1:
        if device.cooling.fields["tProvONAF"].get()==eLogical.TRUE:
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

    iTempRise = device.cooling.fields["tTemperatureRises"].get().value
    sCooling += f" {device.fields['frequency'].get()} Hz "
    sCooling += f" {iTempRise} ° C"
    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    pLCoil = pWindL.coils[0]
    pLCoil1 = None
    if len(pWindL.coils)>1:
        pLCoil1 = pWindL.coils[1]
    pHCoil = pWindH.coils[0]
    pLCond = pLCoil.conductor[0]
    pHCond = pHCoil.conductor[0]
    bHaveTaps = pWindH.have_taps()

    sHVoltage1 = pWindH.fields["sVoltage1"].get()#2 Bushing" if pWindH.fields["iBil_Under"].get()==pWindH.fields["iBIL"].get().value else "Single Bushing"
    sLVoltage1 = pWindL.fields["sVoltage1"].get()#f'{pWindL.fields["dVoltage"].get()} /' if pWindL.fields["iNumBushings"].get() > 2 else None
    sLVoltage2 = pWindL.fields["sVoltage2"].get()#pWindL.fields["dVoltage"].get() / 2 if pWindL.fields["iNumBushings"].get() > 2 else pWindL.fields["dVoltage"].get()

    bIfMPM = device.fields["transformer_family"].get()==eTransformerFamily.MPM# or device.fields["transformer_family"].get()==eTransformerFamily.RUD
    sStartLead = "HV Start Lead:  Begin Away" if bIfMPM else "HV Start Lead:  Begin Towards"

    tLamination = device.core.fields["tLaminationType"].get()
    Core_Dwg_No = device.core.calculate_sPart()
    sDrwg2 = "*** USED Core ***" if str(Core_Dwg_No)[-4:].lower() == "used" else f"Core Dwg: {Core_Dwg_No}"
    sUnderInner = device.fields["sFormMaterial"].get() if device.fields["sFormMaterial"].get() !="" else device.fields["sFormDescription"].get()
    sDP = f"{device.fields['dLatestVersionNo'].get():.2f}" if device.fields['sDPSpecial'].get() != "M" else f"{device.fields['dLatestVersionNo'].get():.2f} M"

    s_CPacking= [0.0]*7
    dCPacking0 = 0.0
    dCPacking0 = round((device.core.fields["dFormSide"].get() + 2 * (pLCoil.fields["dRadialBuiltOutNoses"].get() + pLCoil.fields["dRadialBuiltIntNoses"].get())) * 8) / 8
    s_CPacking[0] = f"{dCPacking0:.3f}" if dCPacking0 != 0.0 else ""
    result = 0.0
    dCPacking1 = round((pLCoil.fields["dOvercoil"].get() + 1/2 + 8/5 * device.fields["dRBIW"].get()) * 8 - 1) / 8
    s_CPacking[1] = f"{dCPacking1:.3f}" if dCPacking1!= 0.0 else ""
    result = 0.0
    s_CPacking[2] = f"{result:.0f}" if result != 0.0 else ""
    result = 2.0
    s_CPacking[3] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    s_CPacking[4] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    s_CPacking[5] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    dBendA = (dCPacking1 -pLCoil.fields["dOvercoil"].get()-1/2)/2
    s_CPacking[6] = f"{dBendA:.3f}" if dBendA != 0.0 else ""

    s_OPacking= [0.0]*6
    s_OPacking[0] = s_CPacking[0]
    result = 0.0
    result = device.core.fields["dWindowHeight"].get()-0.125
    s_OPacking[1] = f"{result:.3f}" if dCPacking1!= 0.0 else ""
    result = 0.0
    s_OPacking[2] = f"{result:.0f}" if result != 0.0 else ""
    result = 2*device.getOPacking(1)
    s_OPacking[3] = f"{result:.0f}" if result != 0.0 else ""
    result = 2*device.getOPacking(2)
    s_OPacking[4] = f"{result:.0f}" if result != 0.0 else ""
    result = 2*device.getOPacking(3)
    s_OPacking[5] = f"{result:.0f}" if result != 0.0 else ""

    s_HDuct = [0.0]*6
    result = 0.0
    result = dBendA - (0.5 if dBendA >= 1.5 else 0.375 if dBendA >= 1.25 else 0.25)
    s_HDuct[0] = f"{result:.3f}" if dCPacking1!= 0.0 else ""
    result = 0.0
    result = dCPacking0 - 0.75
    s_HDuct[1] = f"{result:.3f}" if result != 0.0 else ""
    result = 0.0
    s_HDuct[2] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    s_HDuct[3] = f"{result:.0f}" if result != 0.0 else ""
    result = 4.0
    s_HDuct[4] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    s_HDuct[5] = f"{result:.0f}" if result != 0.0 else ""

    s_VDuct = [0.0]*6
    result = 0.0
    result = dCPacking0
    s_VDuct[0] = f"{result:.3f}" if dCPacking1!= 0.0 else ""
    s_VDuct[1] = s_OPacking[1]
    result = 0.0
    s_VDuct[2] = f"{result:.0f}" if result != 0.0 else ""
    result = 0.0
    s_VDuct[3] = f"{result:.0f}" if result != 0.0 else ""
    iBilHV = pHCoil.fields["iBil"].get()
    result = 2 if (iBilHV >=150 and iBilHV <250) else 0
    s_VDuct[4] = f"{result:.0f}" if result != 0.0 else ""
    result = 2 if (iBilHV >=250) else 0
    result = 0.0
    s_VDuct[5] = f"{result:.0f}" if result != 0.0 else ""

    dBetweenCores = device.core.fields["dFormNose"].get() - 2 * device.core.fields["dStackPerLoop"].get()
    dBetweenCores_frac = Fraction(dBetweenCores).limit_denominator()

    sDwg = f"WS-{device.fields['so_number'].get()}"
    def split_string(text):
        max_len = 91
        words = text.split()
        parts = []
        temp = ""

        for w in words:
            # Calcular longitud si añadimos la palabra
            if temp:
                new_len = len(temp) + 1 + len(w)
            else:
                new_len = len(w)

            if new_len > max_len:
                # Guardar parte y empezar nueva
                parts.append(temp)
                temp = w
            else:
                temp += (" " if temp else "") + w

            # Si ya tenemos 3 partes, agregamos todo lo que quede a la última
            if len(parts) == 2:
                remaining_words = words[words.index(w)+1:]
                if remaining_words:
                    temp += " " + " ".join(remaining_words)
                break

        if temp:
            parts.append(temp)

        # Asegurar que siempre sean 3 elementos
        while len(parts) < 3:
            parts.append("")

        return parts

    # Ejemplo de uso
    sWindingsComments = device.fields["sWindingSpecComments"].get()

    sComments4Eng = split_string(sWindingsComments)

    sOverOuter = "10 - 0.005" if pLCoil.fields['dEndPaper'].get() == 0.005 and pLCoil.fields["iBil"].get() == 60 else pLCoil.fields['s_OverOuterPreliminar'].get()[1] if pLCoil.fields['s_OverOuterPreliminar'].get()[0] is False else pLCoil.fields['s_OverOuterPreliminar'].get()[0]
    sEndFiller = pWindH.fields["sEndFiller"].get()
    sEndFillerLV = pWindL.fields["sEndFiller"].get()    

    sTEW = pWindH.fields["sHVTapLeads"].get()
    date = datetime.strptime(device.fields["original_issue_date"].get(), "%m/%d/%Y")
    sformat_date = date.strftime("%b %#d/%y")

    sDuctsOuter = [""]*2
    sDuctsOuter[0] = f'{pWindL.fields["iNoDuctOuter"].get()}' if pWindL.fields["iNoDuctOuter"].get()>0 else ""
    sDuctsOuter[1] = f'{pLCoil1.fields["sDuctsOut"].get()}' if pWindL.fields["iNoDuctOuter"].get()>0 else ""
    
    sLeadsPads = [""]*3
    sLeadsPads[0] = f'{pWindL.fields["iLeadsUnitPad"].get()}' if pWindL.fields["iLeadsUnitPad"].get() > 0 else "N/A"
    sLeadsPads[1] = f"{pWindL.fields['dLeadsWidthPad'].get()}" if pWindL.fields["dLeadsWidthPad"].get() > 0 else "N/A"
    sLeadsPads[2] = f"{pWindL.fields['dLeadsLenghtPad'].get()}" if pWindL.fields["dLeadsLenghtPad"].get() > 0 else "N/A"
    sHVEndPaper = f"{pHCoil.fields['dEndPaper'].get():.3f}" if pHCoil.fields["sPreInsEnd"].get() != "" else None

    sRev = [""]*3
    sRevDescr = [""]*3
    sDateDescr = [""]*3
    iRev = device.fields["i_RevisionWS"].get()[0]
    sRev[0] = 1 if iRev!=0 else ""
    sRev[1] = 2 if iRev>=2 else ""
    sRev[2] = 3 if iRev==3 else ""
    sRevDescr[0] = device.fields["s_DataDescr0"].get()[0] if iRev!=0 else ""
    sRevDescr[1] = device.fields["s_DataDescr0"].get()[1] if iRev>=2 else ""
    sRevDescr[2] = device.fields["s_DataDescr0"].get()[2] if iRev==3 else ""
    sDateDescr[0] = device.fields["s_DateDescr0"].get()[0] if iRev!=0 else ""
    sDateDescr[1] = device.fields["s_DateDescr0"].get()[1] if iRev>=2 else ""
    sDateDescr[2] = device.fields["s_DateDescr0"].get()[2] if iRev==3 else ""
    sCustomSpec = f"Cust Spec:   {device.guarantee.fields['sCustomerSpec'].get()}"

    iNoPhases = device.fields["iNoPhases"].get()

    if iNoPhases==1:
        
        sTapLeadsOutH = ["-"]*8
        sTapLeadsOut = ["-"]*8
        if pWindH.have_taps():
            for i in range(7):
                sTapLeadsOutH[i] = pWindH.changer.fields["sLeadsOutTOPCoilHeater"].get()[i]
                sTapLeadsOut[i] = pWindH.changer.fields["sLeadsOutTOPCoil"].get()[i]

        sDrwg1 = (
            "Conn. Drwg: 10-A-0002-1 rev 1,  Items:  " +
            (str(device.fields["sConnectionDiagramsItems"].get()) if device.fields["sConnectionDiagramsItems"].get() != "" else
             "D, F, N" if pWindH.have_taps() == True else
             "C,F,O" if device.guarantee.fields["sCustomerSpec"].get() == "DE-310" else
             "C, F, N")
        )

        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Winding Specification", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, "Standard Shell Type, Layer Winding", None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, sCooling, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, "Taps", "HV", "LV", None,None,None],
            [None, None, "SO #", "kVA", "Phases", "Type", pWindH.changer.get_Type() if pWindH.have_taps() else "None", sHVoltage1, sLVoltage1,"HV-BIL", "LV-BIL",None],
            [None, None, device.fields['so_number'].get(), f"{pWindH.fields['d_kVA'].get()[0]:.1f}", iNoPhases, device.fields['transformer_family'].get().name,pWindH.changer.fields['tCapacity'].get().value if pWindH.have_taps() else None, pWindH.fields['dVoltage'].get(), sLVoltage2,  pWindH.fields['iBIL'].get().value, pWindL.fields['iBIL'].get().value, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Winding Structure:  Radial:", None, device.fields['array_type'].get().value, sDrwg1, None, None, None, sDrwg2, None, None],
            [None, None, sStartLead, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Form Dimensions", None, "LV Winding", None, None, "HV Winding", None, None, None, None],
            [None, None, f"{device.core.fields['dFormNose'].get():.3f}", f"{device.core.fields['dFormSide'].get():.3f}", "Under Inner:", sUnderInner, None, f"Under: {pWindH.fields['sHLUnderHV'].get()}", None, None,f"RB: {pWindH.fields['dHLUnderHBRB'].get():.3f}", None],
            [None, None, f"RB: {device.fields['dFormRB'].get()}", None, "Over Outer:", sOverOuter, None, f"Over: {pWindH.fields['sHLOverHV'].get()}", None, None, f"RB: {pWindH.fields['dHLOverHVSideRB'].get():.3f}", None],
            [None, None, "Turns / Leg", None, None, pWindL.fields['dNomTurnsUI'].get(), None,None, pHCoil.fields['dTurns'].get(),  None, None, None],
            [None, None, "Conductor Space", None, None, f"{pLCond.fields['dConductorSpace'].get():.3f}", None,None, f"{pHCond.fields['dConductorSpace'].get():.3f}",  None, None, None],
            [None, None, "Stayback", None, None, f"{pLCond.fields['dStayback'].get():.3f}", None,None, f"{pHCond.fields['dStayback'].get():.3f}",  None, None, None],
            [None, None, "Overcoil", None, None, f"{pLCoil.fields['dOvercoil'].get():.3f}", None,None, f"{pHCoil.fields['dOvercoil'].get():.3f}",  None, sEndFiller,None],
            [None, None, "Sections / Leg", None, None, pLCoil.fields['iSectionsLeg'].get(), None, None,pHCoil.fields['iSectionsLeg'].get(),  None, None, None],
            [None, None, "Turns / Section", None, None, pLCoil.fields['dTurnsSection'].get(), None,None, pHCoil.fields['dTurnsSection'].get(),  None, None, None],
            [None, None, "Turns / Layer", None, None, pLCoil.fields['iTurnsLayers'].get(), None, None,pHCoil.fields['iTurnsLayers'].get(), None, "HV Tot Lyrs",None],
            [None, None, "No. of Layers", None, None,pLCoil.fields['dNoLayersBody'].get(), None, None,pHCoil.fields['dNoLayersBody'].get(),  None, pHCoil.fields['dTotLayers'].get(), None],
            [None, None, "Layer Insulation", None, None, pLCoil.fields['sInsulationEnd'].get(), None, pHCoil.fields['sPreInsEnd'].get(), None, sHVEndPaper, None, None],
            [None, None, None, None, None, None, None, pHCoil.fields['sInsulationInt'].get(), None, f"{pHCoil.fields['dInterPaper'].get():.3f}", None, None],
            [None, None, "Nose Ducts", "-  Inner", f"{pWindL.fields['iNoDuctInner'].get()}",pLCoil.fields['sDuctsIn'].get(),None,  f"{pWindH.fields['iNoDuctInner'].get()}", pHCoil.fields['sDuctsIn'].get(),None, None, None, None],
            [None, None, pLCoil.fields['sWidthDucts'].get(), "-  Outer", sDuctsOuter[0], sDuctsOuter[1], None, None, None, None, None, None],
            [None, None, "Radial Builds", None, f"{pLCoil.fields['dRadialBuiltSides'].get():.2f}", f"{pLCoil.fields['dRadialBuiltIntNoses'].get():.2f}", f"{pLCoil.fields['dRadialBuiltOutNoses'].get():.2f}", f"{pHCoil.fields['dRadialBuiltSides'].get():.2f}", None, f"{pHCoil.fields['dRadialBuiltOutNoses'].get():.2f}", None, None],
            [None, None, "Endfiller", None, None,"Yes" if pWindL.fields['bEndFiller'].get()==eLogical.TRUE else "No",  None, None,"Yes" if pWindH.fields['bEndFiller'].get()==eLogical.TRUE else "No",None,  None, None, None],
            [None, None, "0.010 Lead Pads", None,pWindL.fields['iLeadsBeforeUnit'].get(), f"{pWindL.fields['dLeadsWidth'].get():.3f}", f"{pWindL.fields['dLeadsLenght'].get():.3f}", pWindH.fields['iLeadsBeforeUnit'].get(), f"{pWindH.fields['dLeadsWidth'].get():.3f}", f"{pWindH.fields['dLeadsLenght'].get():.3f}", None, None],
            [None, None, "1/16 PB Lead Pad (LV1 Start Only)", None, sLeadsPads[0], sLeadsPads[1], sLeadsPads[2], None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Taps: Tap Leads Out TOP of Coil" if bHaveTaps else "NoTaps", None, None, None, None, None, None, None, None, None],
            [None, None, sTapLeadsOutH[0], sTapLeadsOutH[1], sTapLeadsOutH[2], sTapLeadsOutH[3], sTapLeadsOutH[4], sTapLeadsOutH[5], sTapLeadsOutH[6], sTapLeadsOutH[7], None, None],
            [None, None, sTapLeadsOut[0], sTapLeadsOut[1], sTapLeadsOut[2], sTapLeadsOut[3], sTapLeadsOut[4], sTapLeadsOut[5], sTapLeadsOut[6], sTapLeadsOut[7], None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pLCond.fields['sSWConductor'].get()}:", pLCond.fields['sMatConductor'].get(), None, None, None, None, None, None, None, None],
            [None, None, pLCond.fields['sDescription'].get(), pLCond.fields['sPapperConductor'].get(), f"{pLCond.fields['dWeight'].get():.0f} lbs", "High", "Wide", "Thick / turn", "Width / turn", None, None, None],
            [None, None, None, None, None,pLCoil.fields['iHigh'].get(), pLCoil.fields['iWide'].get(), f"{pLCoil.fields['dCondThickTurn'].get():.4f}", f"{pLCoil.fields['dCondWdthTurn'].get():.4f}", None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pHCond.fields['sSWConductor'].get()}:", pHCond.fields['sMatConductor'].get(), None, None, None, None, None, None,  "HV Ø  / Tapleads",None],
            [None, None, pHCond.fields['sDescription'].get(), pHCond.fields['sPapperConductor'].get(), f"{pHCond.fields['dWeight'].get():.0f} lbs", "High", "Wide", "Thick / turn", "Width / turn", None,  sTEW,None],
            [None, None, None, None, None, pHCoil.fields['iHigh'].get(), pHCoil.fields['iWide'].get(), f"{pHCoil.fields['dCondThickTurn'].get():.4f}", f"{pHCoil.fields['dCondWdthTurn'].get():.4f}", None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Coil sizes :", "RBIW", "Total RBIW", "LV Nose", "HV Nose", "Sides", "Noses", "LV lead bld", "HV lead bld", None],
            [None, None, None, f"{device.fields['dRBIW'].get():.3f}", f"{device.fields['dTotalRBIW'].get():.3f}", f"{pWindL.fields['dNose'].get():.3f}", f"{pWindH.fields['dNose'].get():.3f}", f"{device.fields['dSides'].get():.3f}", f"{device.fields['dNoses'].get():.3f}", f"{pWindL.fields['dNoses'].get():.3f}", f"{pWindH.fields['dNoses'].get():.3f}", None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, pWindL.fields['sCoilLdMat'].get(), "Coil Leads", None, None, None, None, None, None, None, None],
            [None, None, "LV Start & Finish Leads", None, None, None, None, device.fields['sMaterialBuswork'].get(), "LV Buswork", None, None, None],
            [None, None, "Quantity", "Thick / Lead", "Width", "Density", None, "Qty.    Thick / Lead    Width", None, "Density (ONAN/ONAF)", None, None],
            [None, None, pWindL.fields['iQuantityLeads'].get(), f"{pWindL.fields['dThicknessLeads'].get():.4f}", f"{pWindL.fields['dWidthLeads'].get():.3f}", f"{pWindL.fields['dLeadsDensity'].get():.0f}", None, f"{device.fields['iQuantityBuswork'].get()}           {device.fields['dThicknessBuswork'].get():.2f}             {device.fields['dWidthBuswork'].get():.1f}", None, f"{device.fields['dDensityBuswork'].get():.0f}",  f"{device.fields['dDensityBusworkOnaf'].get():.0f}",None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "See drawing 06-ISD0103 Rev. 1 for reference", None, None, None, None, None, None, None, None, None],
            [None, "Pos", "Item", "Width", "Height", "Pos 1/32\"", "Pos 1/16\"", "Pos 1/8\"", "Pos 1/4\"", "Bend @ \"A\"", None, None],
            [None, "1", "C-Packing", s_CPacking[0], s_CPacking[1], s_CPacking[2], s_CPacking[3], s_CPacking[4], s_CPacking[5], s_CPacking[6], None, None],
            [None, "2", "O-Packing", s_OPacking[0], s_OPacking[1], s_OPacking[2], s_OPacking[3], s_OPacking[4], s_OPacking[5], None, "Packing", None],
            [None, "3", "H-Duct", s_HDuct[0], s_HDuct[1], s_HDuct[2], s_HDuct[3], s_HDuct[4], s_HDuct[5], None, "between cores", None],
            [None, "4", "V-Duct", s_VDuct[0], s_VDuct[1], s_VDuct[2], s_VDuct[3], s_VDuct[4], s_VDuct[5], None, f"{dBetweenCores_frac}", None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Comments From Eng:", None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[0], None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[1], None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[2], None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, "Rev By/Date", "Design By:", device.fields['issued_by'].get(), None, None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, sDateDescr[0], "Design Date:", sformat_date , None, None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, sDateDescr[1], "DP Ver. No.", "Approved By", "Title:         Winding Spec", None,  "Revision No",None],
            [None, sRev[2], sRevDescr[2], None, None, sDateDescr[2], sDP, None, "Drawing No:", sDwg, iRev,None]
        ]
    else:
        sTapLeadsOutH = ["-"]*10
        sTapLeadsOut = ["-"]*10
        bHaveTaps = pWindH.have_taps()
        if bHaveTaps:
            for i in range(7):
                sTapLeadsOutH[i+1] = pWindH.changer.fields["sLeadsOutTOPCoilHeater"].get()[i]
                sTapLeadsOut[i+1] = pWindH.changer.fields["sLeadsOutTOPCoil"].get()[i]

        sTempONAF = [""]*2
        tTempONAF = device.cooling.fields["tTempONAF"].get()
        sTempONAF[0] = tTempONAF[0].value
        sMVA1 = None
        if device.cooling.fields["bOverExcitation"].get()==eLogical.TRUE:
            sTempONAF[1] = device.cooling.fields["tTempOExc"].get().value
            sMVA1 = device.cooling.fields["sMVAOExc"].get()

        sMVA = pWindH.fields["d_kVA"].get()[0] if iNoPhases==1 else pWindH.fields["skVA"].get()
        sTaps = f"Taps ({pWindH.changer.fields['sCapactity'].get() if bHaveTaps else ''})"
        type_connectionHV = pWindH.fields["type_connection"].get()
        type_connectionLV = pWindL.fields["type_connection"].get()
        ground_YHV = pWindH.fields["ground_Y"].get()
        sConnHV = "∆" if type_connectionHV==eConnectionType.DELTA else ("Y" if type_connectionHV==eConnectionType.WYE and ground_YHV==eLogical.FALSE else "")
        sConnLV = "∆" if type_connectionLV==eConnectionType.DELTA else "Y"
        # sGroundHV = f"{pWindH.fields["dLineVoltage"].get():.0f}GY / " if type_connectionHV==eConnectionType.WYE and ground_YHV==eLogical.TRUE else ""
        # sHVoltage1 = f"{sGroundHV}{pWindH.fields["dVoltage"].get():.0f} {sConnHV}"
        sHVoltage1 = (
            f"{pWindH.fields['dLineVoltage'].get():.0f}"
            + (" ∆" if type_connectionHV == eConnectionType.DELTA else (" Y" if ground_YHV == eLogical.FALSE else " GY"))
            + (" " + f"/ {pWindH.fields['dVoltage'].get():.0f}" if type_connectionHV != eConnectionType.DELTA else "")
        )
        sLAux = "" if type_connectionLV == eConnectionType.DELTA else f"/ {pWindL.fields['dVoltage'].get():.0f}"
        sLVoltage1 = f"{pWindL.fields['dLineVoltage'].get():.0f} {sConnLV} {sLAux}"
        dPerUp = pWindH.changer.fields['dPerUp'].get()
        sTypeTaps = pWindH.changer.fields['sPerTaps'].get() if bHaveTaps else None

        dCorrTurnsLyrsHV = pHCond.fields['dCorrTurnsLyrs'].get()
        dCorrLyrsHV = pHCond.fields['dCorrLyrs'].get() 

        if bHaveTaps and pWindH.get_noSteps() > 1:
            if dCorrTurnsLyrsHV == int(dCorrTurnsLyrsHV):
                half_layers = dCorrLyrsHV / 2
                if half_layers > int(half_layers):
                    iCenterLapLay = int(half_layers + 1)
                else:
                    iCenterLapLay = int(half_layers)
            else:
                iCenterLapLay = int(math.ceil(dCorrLyrsHV / 2 / 2) * 2)  # EVEN()
        else:
            iCenterLapLay = 0
        dCenterTap = iCenterLapLay*dCorrTurnsLyrsHV
        dHVReturnLeg = device.core.fields['dSmallWindowWidth'].get() - device.fields['dRBIW'].get() + device.fields['dOverHVRadialBuilt'].get()
        dPhasePhase = device.core.fields['dLargeWindowWidth'].get() - 2 * device.fields['dRBIW'].get() + 2 * device.fields['dOverHVRadialBuilt'].get()
        sLeadsPadsStartFin = ['']*3
        sLeadsPadsStartFin[0] = f"{(pWindL.fields['iLeadsStartFinPadsUnit'].get()/6):.0f}" if pWindL.fields['iLeadsStartFinPadsUnit'].get()>0 else ""
        sLeadsPadsStartFin[1] = f"{pWindL.fields['dLeadsStartFinPadsWidth'].get()}" if pWindL.fields['dLeadsStartFinPadsWidth'].get()>0 else "N/A"
        sLeadsPadsStartFin[2] = f"{pWindL.fields['dLeadsStartFinPadsLenght'].get()}" if pWindL.fields['dLeadsStartFinPadsLenght'].get()>0 else ""

        sDrwg1 = (
            "Connection Drwg:  " +
            (str(device.fields['sConnectionDiagramsItems'].get()) if device.fields['sConnectionDiagramsItems'].get() != "" else
             "10 - A - 0003  rev2,    Item:  " +
             ("A" if type_connectionHV == eConnectionType.WYE and type_connectionLV == eConnectionType.WYE else
              "B" if type_connectionHV == eConnectionType.DELTA and type_connectionLV == eConnectionType.WYE else
              "C" if type_connectionHV == eConnectionType.WYE and type_connectionLV == eConnectionType.DELTA else
              "D"))
        )

        sTapsLeadsTogether = (
            "Group Tap leads 'E, C & A' together and Tap leads 'B, D & f' together and put a 1/2'' spacers between both groups"
            if pHCoil.fields['iBil'].get() > 200 and bHaveTaps
            else ""
        )
        sFormDimensions = "Form from external supplier" if pWindH.fields['d_kVA'].get()[0]>=1500 else "Form Dimensions"
        d_Values = [0,0]
        sLeadsHVTaps = "Warning: Please Define HV Lead Build and Lead Pads Manually" if pWindH.winding_warnings.warningLeadsHVTaps(d_Values)==eWarnings.eWARNING else ""
        sMVAKVA = "MVA" if device.cooling.fields['iCoolingStepsReal'].get()>1 else "kVA"

        data = [
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "For:", None,  device.fields['customer_name'].get() , None, "Winding Specification", None, None, None, "SO #:", device.fields['so_number'].get(),None],
            [None, "Code:", None, device.fields['customer_code'].get(), None, "Standard Shell Type, Layer Winding", None, None, None, "Primary Spec:", device.guarantee.fields['tDesignSpec'].get().value,None],
            [None, None, None, None, None, sCooling, None, None, None, sCustomSpec,None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, sMVAKVA, None, None, None, None, None, None,None,None],
            [None, None, sTempONAF[0], sMVA, "Phases", "Type",sTaps, "HV", "LV","HV-BIL", "LV-BIL",None],
            [None, None, sTempONAF[1], sMVA1, iNoPhases, device.fields['transformer_family'].get().name, sTypeTaps, sHVoltage1, sLVoltage1,  pWindH.fields['iBIL'].get().value, pWindL.fields['iBIL'].get().value, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Winding Structure:  Radial:", None, device.fields['array_type'].get().value, sDrwg1, None, None, None, sDrwg2, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, sFormDimensions, None, "LV Winding", None, None, "HV Winding", None, None, None, None],
            [None, None, f"{device.core.fields['dFormNose'].get():.3f}", f"{device.core.fields['dFormSide'].get():.3f}", "Under Inner:", sUnderInner, None, f"Under: {pWindH.fields['sHLUnderHV'].get()}", None, None,f"RB: {pWindH.fields['dHLUnderHBRB'].get():.3f}", None],
            [None, None, f"RB: {device.fields['dFormRB'].get()}", None, None, None, None,f"Over: {pWindH.fields['sHLOverHV'].get()}", None, None, f"RB: {pWindH.fields['dHLOverHVSideRB'].get():.3f}", None],
            [None, None, "Turns / Leg", None, None, pWindL.fields['dNomTurnsUI'].get(), None,None, pHCoil.fields['dTurns'].get(),  None, None, None],
            [None, None, "Conductor Space", None, None, f"{pLCond.fields['dConductorSpace'].get():.3f}", None,None, f"{pHCond.fields['dConductorSpace'].get():.3f}",  None, None, None],
            [None, None, "Stayback", None, None, f"{pLCond.fields['dStayback'].get():.3f}", None,None, f"{pHCond.fields['dStayback'].get():.3f}",  None, None, None],
            [None, None, "Overcoil", None, None, f"{pLCoil.fields['dOvercoil'].get():.3f}", sEndFillerLV,None, f"{pHCoil.fields['dOvercoil'].get():.3f}",  None, sEndFiller,None],
            [None, None, "Sections / Leg", None, None, pLCoil.fields['iSectionsLeg'].get(), None, None,pHCoil.fields['iSectionsLeg'].get(),  None, None, None],
            [None, None, "Turns / Section", None, None, pLCoil.fields['dTurnsSection'].get(), None,None, pHCoil.fields['dTurnsSection'].get(),  None, None, None],
            [None, None, "Turns / Layer", None, None, pLCoil.fields['iTurnsLayers'].get(), None, None,pHCoil.fields['iTurnsLayers'].get(), None, None, None],
            [None, None, "No. of Layers", None, None,pLCoil.fields['dNoLayersBody'].get(), None, None,pHCoil.fields['dNoLayersBody'].get(),  None, None,  None],
            [None, None, "Layer Insulation", None, None, pLCoil.fields['sInsulationEnd'].get(), None, pHCoil.fields['sPreInsEnd'].get(), None, sHVEndPaper, None, None],
            [None, None, None, None, None, None, None, pHCoil.fields['sInsulationInt'].get(), None, f"{pHCoil.fields['dInterPaper'].get():.3f}", None, None],
            [None, None, "Nose Ducts",pLCoil.fields['sWidthDucts'].get(), f"{pWindL.fields['iNoDuctInner'].get()}",pLCoil.fields['sDuctsIn'].get(),None,  f"{pWindH.fields['iNoDuctInner'].get()}", pHCoil.fields['sDuctsIn'].get(),None, None, None, None],
            [None, None, "All around Ducts", pLCoil.fields['sWidthDucts'].get(), sDuctsOuter[0], sDuctsOuter[1], None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Radial Builds", None, f"{pLCoil.fields['dRadialBuiltSides'].get():.2f}", None, f"{pLCoil.fields['dRadialBuiltIntNoses'].get():.2f}", f"{pHCoil.fields['dRadialBuiltSides'].get():.2f}", None, f"{pHCoil.fields['dRadialBuiltOutNoses'].get():.2f}", None, None],
            [None, None, "Endfiller", None, None,"Yes" if pWindL.fields['bEndFiller'].get()==eLogical.TRUE else "No",  None, None,"Yes" if pWindH.fields['bEndFiller'].get()==eLogical.TRUE else "No",None,  None, None, None],
            [None, None, "0.010 Lead Pads", None,pWindL.fields['iLeadsBeforeUnit'].get(), f"{pWindL.fields['dLeadsWidth'].get():.3f}", f"{pWindL.fields['dLeadsLenght'].get():.3f}", pWindH.fields['iLeadsBeforeUnit'].get(), f"{pWindH.fields['dLeadsWidth'].get():.3f}", f"{pWindH.fields['dLeadsLenght'].get():.3f}", None, None],
            [None, None, "1/16 PB Lead Pad (LV1 Start Only)", None, sLeadsPads[0], sLeadsPads[1], sLeadsPads[2], None, None, None, None, None],
            [None, None, "1/16 PB Lead Pad (After LV St & LV Fin)", None, sLeadsPadsStartFin[0], sLeadsPadsStartFin[1], sLeadsPadsStartFin[2], None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Taps: Tap Leads Out TOP of Coil" if bHaveTaps else "NoTaps", None, None, None, None, None, None, None, None, None, None],
            [None, sTapLeadsOutH[0], sTapLeadsOutH[1], sTapLeadsOutH[2], sTapLeadsOutH[3], sTapLeadsOutH[4], sTapLeadsOutH[5], sTapLeadsOutH[6], sTapLeadsOutH[7], sTapLeadsOutH[8], sTapLeadsOutH[9], None],
            [None, sTapLeadsOut[0], sTapLeadsOut[1], sTapLeadsOut[2], sTapLeadsOut[3], sTapLeadsOut[4], sTapLeadsOut[5], sTapLeadsOut[6], sTapLeadsOut[7], sTapLeadsOut[8], sTapLeadsOut[9], None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, f"{pLCond.fields['sSWConductor'].get()}:", pLCond.fields['sMatConductor'].get(), None, None, None, " Clearances (metal to metal)", None, None, None, None],
            [None, None, pLCond.fields['sDescription'].get(), pLCond.fields['sPapperConductor'].get(), f"{pLCond.fields['dWeight'].get():.0f} lbs", "High x Wide", None, "From-To", "Clearance (in)", None, None, None],
            [None, None, None, None, None,f"{pLCoil.fields['iHigh'].get()} x {pLCoil.fields['iWide'].get()}", None, "LV-Core ", f"{device.fields['dFormRB'].get():.4f}", None, None, None],
            [None, None, "Thick / turn", f"{pLCoil.fields['dCondThickTurn'].get():.4f}", "Width / turn", f"{pLCoil.fields['dCondWdthTurn'].get():.4f}", None, "LV-HV", f"{device.fields['dUnderHVRadialBuilt'].get():.4f}", None, None, None],
            [None, None, None, None, None, None, None, "HV-Return Leg", f"{dHVReturnLeg:.4f}", None, None, None],
            [None, None, f"{pHCond.fields['sSWConductor'].get()}:", pHCond.fields['sMatConductor'].get(), None, None, None, "Phase-Phase", f"{dPhasePhase:.4f}", None,  None,None],
            [None, None, pHCond.fields['sDescription'].get(), pHCond.fields['sPapperConductor'].get(), f"{pHCond.fields['dWeight'].get():.0f} lbs", "High x Wide", None, None, None, None,  None,None],
            [None, None, None, None, None, f"{pHCoil.fields['iHigh'].get()} x {pHCoil.fields['iWide'].get()}", None, None, None, None, None, None],
            [None, None, "Thick / turn", f"{pHCoil.fields['dCondThickTurn'].get():.4f}", "Width / turn", f"{pHCoil.fields['dCondWdthTurn'].get():.4f}", None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, pWindL.fields['sCoilLdMat'].get(), "Coil LV Start & Finish Leads", None, None, None, device.fields['sMaterialBuswork'].get(), "LV Buswork", None, None, None],
            [None, None, "Qty.    Thick / Lead    Width", None,"Density (ONAN/ONAF)", None, None, "Qty.    Thick / Lead    Width", None,"Density (ONAN/ONAF)", None],
            [None, None, f"{pWindL.fields['iQuantityLeads'].get()}           {pWindL.fields['dThicknessLeads'].get():.2f}             {pWindL.fields['dWidthLeads'].get():.1f}",None, f"{pWindL.fields['dLeadsDensity'].get():.0f}",f"{pWindL.fields['dLeadsDensityONAF'].get():.0f}" if pWindL.fields['dLeadsDensityONAF'].get()>0.0 else None, None, f"{device.fields['iQuantityBuswork'].get()}           {device.fields['dThicknessBuswork'].get():.2f}             {device.fields['dWidthBuswork'].get():.1f}", None, f"{device.fields['dDensityBuswork'].get():.0f}",  f"{device.fields['dDensityBusworkOnaf'].get():.0f}" if device.fields['dDensityBusworkOnaf'].get()>0.0 else None,None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "HV Coil Leads - #", None, pWindH.fields['sHVTapLeads'].get(),  None, None, None, None, None, None, None],
            [None, None, "HV Line Leads - #", None, pWindH.fields['sHVTapLineLeads'].get(), sTapsLeadsTogether, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, None, "Coil sizes :", "RBIW", "Total RBIW", "LV Nose", "HV Nose", "Sides", "Noses", "LV lead bld", "HV lead bld", None],
            [None, None, None, f"{device.fields['dRBIW'].get():.3f}", f"{device.fields['dTotalRBIW'].get():.3f}", f"{pWindL.fields['dNose'].get():.3f}", f"{pWindH.fields['dNose'].get():.3f}", f"{device.fields['dSides'].get():.3f}", f"{device.fields['dNoses'].get():.3f}", f"{pWindL.fields['dNoses'].get():.3f}", f"{pWindH.fields['dNoses'].get():.3f}", None],
            [None, None, sLeadsHVTaps,None, None, None, None, None, None, None, None, None],
            [None, None, sLeadsHVTaps,None, None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, "Std. HV T/L", "Centertap Layer", "Centertap", None, None],
            [None, None, None, None, None, None, None, f"{dCorrTurnsLyrsHV:.1f}", iCenterLapLay, f"{dCenterTap:.1f}", None, None],
            [None, None, None, None, None, None, None, None, None, None, None, None],
            [None, "Comments From Engineering:", None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[0], None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[1], None, None, None, None, None, None, None, None, None, None],
            [None, sComments4Eng[2], None, None, None, None, None, None, None, None, None, None],
            [None, "Rev", "Revision Description", None, None, None,"Rev By/Date", "Design By:", device.fields['issued_by'].get(),  None, None, None],
            [None, sRev[0], sRevDescr[0], None, None, sDateDescr[0],None, "Design Date:", sformat_date ,  None, None, None],
            [None, sRev[1], sRevDescr[1], None, None, sDateDescr[1], None,"DP Ver. No.", "Approved By", "Title:Winding Spec        Revision No",None,None],
            [None, sRev[2], sRevDescr[2], None, None, sDateDescr[2],None, sDP, None, f"Drawing No:{sDwg}        {iRev}",None,None]
        ]
    
    return data