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
from Definitions.enums import eConnectionType,eArrayType,eLogical,eBIL,eTransformerFamily,eWarnings,eWindingSelection,eTapExtremes,eSequence,eChangerCapacity,eMaterial,eLaminationType,eTypeConductor,eCoating,eDesignSpec,eCoolingType,eSingleCoolingType,eTemperatureRise
from Definitions.functions import address,ADDRESSZ,is_folder_open

def make_quote_page(device = None):
    device_f = device.fields

    iNoPhasesE = device_f["iNoPhases"].get()
    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    pLCoil = pWindL.coils[0]
    pHCoil = pWindH.coils[0]
    pLCond = pLCoil.conductor[0]
    pHCond = pHCoil.conductor[0]
    coreE = device.core
    costsE = device.costs
    lossesE = device.losses

    pWindH_f = pWindH.fields
    pWindL_f = pWindL.fields
    pHCoil_f = pHCoil.fields
    pLCoil_f = pLCoil.fields
    pHCond_f = pHCond.fields
    pLCond_f = pLCond.fields
    coreE_f = coreE.fields
    costsE_f = costsE.fields
    lossesE_f = lossesE.fields

    sVoltage1LV = pWindL_f["sVoltage1"].get()
    sVoltage2LV = pWindL_f["sVoltage2"].get()
    dNL = device.guarantee.fields["dNLGuarantee"].get()
    dLL = device.guarantee.fields["dLLGuarantee"].get()
    sNL = f"{dNL:.1f}" if dNL!=0.0 else "NA"
    sLL = f"{dLL:.1f}" if dLL!=0.0 else "NA"
    sTotalL = f"{(dNL + dLL):.1f}" if (dNL!=0.0  and dLL!=0.0) else "NA"

    dONANLosses = device_f["dONANLosses"].get()
    if dONANLosses > 0:
        result = dONANLosses
    else:
        dTotalLoss = lossesE_f["dTotalLoss"].get()
        if pWindH.have_taps() and pWindH.changer.fields["tCapacity"].get() == eChangerCapacity.FCBN:
            result = round((dTotalLoss + (lossesE_f["dI2RCurrentTempTaps"].get() - lossesE_f["dI2RCurrentTemp"].get())) * 1, -1)
        else:
            result = round(dTotalLoss * 1, -1)
    sTotalLoss = f"{result:.0f}"
    dEvalNL = costsE_f["dEvalNL"].get()
    dEvalLL = costsE_f["dEvalLL"].get()
    sEvaNl = f"{dEvalNL:.0f}" if dEvalNL!=0.0 else ""
    sEvaLl = f"{dEvalLL:.0f}" if dEvalLL!=0.0 else ""
    sEvaTotal = f"{(dEvalNL+dEvalLL):.0f}" if (dEvalNL+dEvalLL)!=0.0 else ""
    frequency = device_f["frequency"].get()
    sFreq  = f"@ {frequency}Hz & % Voltage"
    sIEXC = f"%I exc. @ {frequency}Hz"
    dCurrentTempRise = device.cooling.fields["dCurrentTempRise"].get()
    sEfficiency = f"Rated Voltage, NLL @ 20ºC, {frequency}Hz & LL @ {dCurrentTempRise:.0f} ºC, % Load"
    sRegulation = f"Rated Voltage, LL @ {dCurrentTempRise:.0f} ºC, % Load"

    s_Weight = [""]*13
    dAux = coreE_f["dWeightTwoLoops"].get()
    s_Weight[0] = f"{dAux:.0f}" if dAux else ""
    dAux = device_f["dWeightKr0022"].get()
    s_Weight[1] = f"{dAux:.1f}" if dAux else ""
    dAux = device_f["dWeightKr005"].get()
    s_Weight[2] = f"{dAux:.1f}" if dAux else ""
    dAux = device_f["dWeightKr010"].get()
    s_Weight[3] = f"{dAux:.0f}" if dAux else ""
    dAux = device_f["dWeightCrepe"].get()
    s_Weight[4] = f"{dAux:.0f}" if dAux else ""
    dAux = coreE_f["dWeightsPB1_16"].get()
    s_Weight[5] = f"{dAux:.0f}" if dAux else ""
    dAux = coreE_f["dWeightsPB1_8"].get()
    s_Weight[6] = f"{dAux:.0f}" if dAux else ""
    dAux = coreE_f["d_DuctsDM"].get()[0]
    s_Weight[7] = f"{dAux:.0f}" if dAux else ""
    dAux = pWindL_f["dWeight"].get()
    s_Weight[8] = f"{dAux:.0f}" if dAux else ""
    dAux = pWindH_f["dWeight"].get()
    s_Weight[9] = f"{dAux:.0f}" if dAux else ""
    dAux = pWindL_f["dWeightCoilLeads"].get()
    s_Weight[10] = f"{dAux:.0f}" if dAux else ""
    dAux = pWindL_f["dWeightBusBar"].get()
    s_Weight[11] = f"{dAux:.0f}" if dAux else ""
    dAux = coreE_f["dWeightsPBHD_1_4"].get()
    s_Weight[12] = f"{dAux:.0f}" if dAux else ""

    s_Cost4Item = [""]*13
    dAux = costsE_f["dCostCore"].get()
    s_Cost4Item[0] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostKraft0022"].get()
    s_Cost4Item[1] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostKraft005"].get()
    s_Cost4Item[2] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostKraft010"].get()
    s_Cost4Item[3] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostCrepe"].get()
    s_Cost4Item[4] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostPB1_16"].get()
    s_Cost4Item[5] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostPB1_8"].get()
    s_Cost4Item[6] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostDucts"].get()
    s_Cost4Item[7] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostLV"].get()
    s_Cost4Item[8] = f"{dAux:.0f}" if dAux > 0 else ""
    dAux = costsE_f["dCostHV"].get()
    s_Cost4Item[9] = f"{dAux:.0f}" if dAux > 0 else ""
    dAux = costsE_f["dCostCoilLeads"].get()
    s_Cost4Item[10] = f"{dAux:.1f}" if dAux > 0 else ""
    dAux = costsE_f["dCostBusBar"].get()
    s_Cost4Item[11] = f"{dAux:.0f}" if dAux > 0 else ""
    dAux = costsE_f["dCostPBHD_1_4"].get()
    s_Cost4Item[12] = f"{dAux:.0f}" if dAux > 0 else ""
    
    dAux = device_f["dPerEffDoE2016"].get()
    sDoe2016 = f"{100*dAux:.3f}%" if dAux!=0.0 else ""
    dAux = device_f["dPerEff8021"].get()
    sC8021 = f"{100*dAux:.3f}%" if dAux!=0.0 else ""
    dAux = device_f["dPerEff8023"].get()
    sC8023 = f"{100*dAux:.3f}%" if dAux!=0.0 else ""
    sStandard = [""]*3
    sStandard[0] = "Minimum efficiency value per DoE 2016" if (device.guarantee.fields["tDesignSpec"].get()==eDesignSpec.ANSI) else ""
    sStandard[1] = "Minimum efficiency value per C802.1-13" if (device_f["dPerEff8021"].get()!=0.0) else ""
    sStandard[2] = "Minimum efficiency value per C802.3-15" if (device_f["dPerEff8023"].get()!=0.0) else ""

    sMinIZ = f"Min {device.guarantee.fields['dIZGuarantee'].get():.1f}%"

    s_NoLoadLosses20 = [""]*7
    dAux = coreE_f["d_NoLoadLosses20"].get()
    s_NoLoadLosses20[0] = f"{dAux[0]:.0f}" if dAux[0]>0.0 else ""
    s_NoLoadLosses20[1] = f"{dAux[1]:.0f}" if dAux[1] > 0.0 else ""
    s_NoLoadLosses20[2] = f"{dAux[2]:.0f}" if dAux[2] > 0.0 else ""
    s_NoLoadLosses20[3] = f"{dAux[3]:.0f}" if dAux[3] > 0.0 else ""
    s_NoLoadLosses20[4] = f"{dAux[4]:.0f}" if dAux[4] > 0.0 else ""
    s_NoLoadLosses20[5] = f"{dAux[5]:.0f}" if dAux[5] > 0.0 else ""
    s_NoLoadLosses20[6] = f"{dAux[6]:.0f}" if dAux[6] > 0.0 else ""

    s_NoLoadLosses75 = [""]*7
    dAux = coreE_f["d_NoLoadLosses75"].get()
    s_NoLoadLosses75[0] = f"{dAux[0]:.0f}" if dAux[0]>0.0 else ""
    s_NoLoadLosses75[1] = f"{dAux[1]:.0f}" if dAux[1]>0.0 else ""
    s_NoLoadLosses75[2] = f"{dAux[2]:.0f}" if dAux[2]>0.0 else ""
    s_NoLoadLosses75[3] = f"{dAux[3]:.0f}" if dAux[3]>0.0 else ""
    s_NoLoadLosses75[4] = f"{dAux[4]:.0f}" if dAux[4]>0.0 else ""
    s_NoLoadLosses75[5] = f"{dAux[5]:.0f}" if dAux[5]>0.0 else ""
    s_NoLoadLosses75[6] = f"{dAux[6]:.0f}" if dAux[6]>0.0 else ""

    s_NoLoadLosses85 = [""]*7
    dAux = coreE_f["d_NoLoadLosses85"].get()
    s_NoLoadLosses85[0] = f"{dAux[0]:.0f}" if dAux[0]>0.0 else ""
    s_NoLoadLosses85[1] = f"{dAux[1]:.0f}" if dAux[1]>0.0 else ""
    s_NoLoadLosses85[2] = f"{dAux[2]:.0f}" if dAux[2]>0.0 else ""
    s_NoLoadLosses85[3] = f"{dAux[3]:.0f}" if dAux[3]>0.0 else ""
    s_NoLoadLosses85[4] = f"{dAux[4]:.0f}" if dAux[4]>0.0 else ""
    s_NoLoadLosses85[5] = f"{dAux[5]:.0f}" if dAux[5]>0.0 else ""
    s_NoLoadLosses85[6] = f"{dAux[6]:.0f}" if dAux[6]>0.0 else ""

    s_IExcV = [""]*7
    dAux = coreE_f["d_IExc"].get()
    s_IExcV[0] = f"{100*dAux[0]:.2f}%" if dAux[0]>0.0 else ""
    s_IExcV[1] = f"{100*dAux[1]:.2f}%" if dAux[1]>0.0 else ""
    s_IExcV[2] = f"{100*dAux[2]:.2f}%" if dAux[2]>0.0 else ""
    s_IExcV[3] = f"{100*dAux[3]:.2f}%" if dAux[3]>0.0 else ""
    s_IExcV[4] = f"{100*dAux[4]:.2f}%" if dAux[4]>0.0 else ""
    s_IExcV[5] = f"{100*dAux[5]:.2f}%" if dAux[5]>0.0 else ""
    s_IExcV[6] = f"{100*dAux[6]:.2f}%" if dAux[6]>0.0 else ""

    s_PerEff80 = [""]*8
    dAux = device_f["d_PerEfficiency80"].get()
    s_PerEff80[0] = f"{100*dAux[0]:.2f}%" if dAux[0]>0.0 else ""
    s_PerEff80[1] = f"{100*dAux[1]:.2f}%" if dAux[1]>0.0 else ""
    s_PerEff80[2] = f"{100*dAux[2]:.2f}%" if dAux[2]>0.0 else ""
    s_PerEff80[3] = f"{100*dAux[3]:.2f}%" if dAux[3]>0.0 else ""
    s_PerEff80[4] = f"{100*dAux[4]:.2f}%" if dAux[4]>0.0 else ""
    s_PerEff80[5] = f"{100*dAux[5]:.2f}%" if dAux[5]>0.0 else ""
    s_PerEff80[6] = f"{100*dAux[6]:.2f}%" if dAux[6]>0.0 else ""
    s_PerEff80[7] = f"{100*dAux[7]:.2f}%" if dAux[7]>0.0 else ""
    
    s_PerEff90 = [""]*8
    dAux = device_f["d_PerEfficiency90"].get()
    s_PerEff90[0] = f"{100*dAux[0]:.2f}%" if dAux[0]>0.0 else ""
    s_PerEff90[1] = f"{100*dAux[1]:.2f}%" if dAux[1]>0.0 else ""
    s_PerEff90[2] = f"{100*dAux[2]:.2f}%" if dAux[2]>0.0 else ""
    s_PerEff90[3] = f"{100*dAux[3]:.2f}%" if dAux[3]>0.0 else ""
    s_PerEff90[4] = f"{100*dAux[4]:.2f}%" if dAux[4]>0.0 else ""
    s_PerEff90[5] = f"{100*dAux[5]:.2f}%" if dAux[5]>0.0 else ""
    s_PerEff90[6] = f"{100*dAux[6]:.2f}%" if dAux[6]>0.0 else ""
    s_PerEff90[7] = f"{100*dAux[7]:.2f}%" if dAux[7]>0.0 else ""

    s_PerEff100 = [""]*8
    dAux = device_f["d_PerEfficiency100"].get()
    s_PerEff100[0] = f"{100*dAux[0]:.2f}%" if dAux[0]>0.0 else ""
    s_PerEff100[1] = f"{100*dAux[1]:.2f}%" if dAux[1]>0.0 else ""
    s_PerEff100[2] = f"{100*dAux[2]:.2f}%" if dAux[2]>0.0 else ""
    s_PerEff100[3] = f"{100*dAux[3]:.2f}%" if dAux[3]>0.0 else ""
    s_PerEff100[4] = f"{100*dAux[4]:.2f}%" if dAux[4]>0.0 else ""
    s_PerEff100[5] = f"{100*dAux[5]:.2f}%" if dAux[5]>0.0 else ""
    s_PerEff100[6] = f"{100*dAux[6]:.2f}%" if dAux[6]>0.0 else ""
    s_PerEff100[7] = f"{100*dAux[7]:.2f}%" if dAux[7]>0.0 else ""

    s_PerReg80 = [""]*8
    dAux = device_f["d_PerRegulation80"].get()
    s_PerReg80[0] = f"{dAux[0]:.2f}" if dAux[0]>0.0 else ""
    s_PerReg80[1] = f"{dAux[1]:.2f}" if dAux[1]>0.0 else ""
    s_PerReg80[2] = f"{dAux[2]:.2f}" if dAux[2]>0.0 else ""
    s_PerReg80[3] = f"{dAux[3]:.2f}" if dAux[3]>0.0 else ""
    s_PerReg80[4] = f"{dAux[4]:.2f}" if dAux[4]>0.0 else ""
    s_PerReg80[5] = f"{dAux[5]:.2f}" if dAux[5]>0.0 else ""
    s_PerReg80[6] = f"{dAux[6]:.2f}" if dAux[6]>0.0 else ""
    s_PerReg80[7] = f"{dAux[7]:.2f}" if dAux[7]>0.0 else ""

    s_PerReg90 = [""]*8
    dAux = device_f["d_PerRegulation90"].get()
    s_PerReg90[0] = f"{dAux[0]:.2f}" if dAux[0]>0.0 else ""
    s_PerReg90[1] = f"{dAux[1]:.2f}" if dAux[1]>0.0 else ""
    s_PerReg90[2] = f"{dAux[2]:.2f}" if dAux[2]>0.0 else ""
    s_PerReg90[3] = f"{dAux[3]:.2f}" if dAux[3]>0.0 else ""
    s_PerReg90[4] = f"{dAux[4]:.2f}" if dAux[4]>0.0 else ""
    s_PerReg90[5] = f"{dAux[5]:.2f}" if dAux[5]>0.0 else ""
    s_PerReg90[6] = f"{dAux[6]:.2f}" if dAux[6]>0.0 else ""
    s_PerReg90[7] = f"{dAux[7]:.2f}" if dAux[7]>0.0 else ""

    s_PerReg100 = [""]*8
    dAux = device_f["d_PerRegulation100"].get()
    s_PerReg100[0] = f"{dAux[0]:.2f}" if dAux[0]>0.0 else ""
    s_PerReg100[1] = f"{dAux[1]:.2f}" if dAux[1]>0.0 else ""
    s_PerReg100[2] = f"{dAux[2]:.2f}" if dAux[2]>0.0 else ""
    s_PerReg100[3] = f"{dAux[3]:.2f}" if dAux[3]>0.0 else ""
    s_PerReg100[4] = f"{dAux[4]:.2f}" if dAux[4]>0.0 else ""
    s_PerReg100[5] = f"{dAux[5]:.2f}" if dAux[5]>0.0 else ""
    s_PerReg100[6] = f"{dAux[6]:.2f}" if dAux[6]>0.0 else ""
    s_PerReg100[7] = f"{dAux[7]:.2f}" if dAux[7]>0.0 else ""

    MVA = pWindH_f["d_kVA"].get()[0] if iNoPhasesE==1 else pWindH_f["skVA"].get()

    sMVAKVA = "kVA" if iNoPhasesE==1 else ("MVA" if device.cooling.fields["iCoolingStepsReal"].get()>1 else "kVA")

    data = [
        [None, None, None, None, None, None, "Quote Page", None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "SO #", "kVA" if iNoPhasesE==1 else sMVAKVA, "Type","Core","Flux (kL)", "LV Cond 1","LV Dens.","HV Cond 1","HV Dens.",None, None],
        [None, None, device_f["so_number"].get(), MVA, device_f["transformer_family"].get().name, coreE_f["tLaminationType"].get().value, f'{coreE_f["dFluxDensityRatPer"].get():.0f}', pLCond_f["sDescription"].get(), f'{pLCond_f["dDensity"].get():.0f}', pHCond_f["sDescription"].get(), f'{pHCond_f["dDensity"].get():.0f}', None, None],
        [None, None, None, "HV", "LV", "Core Strip", "Tesla", "LV-BIL", "LV Turns", "HV-BIL", "HV Turns", None, None],
        [None, None, None, pWindH_f["dVoltage"].get(), sVoltage1LV+sVoltage2LV, f'{coreE_f["dSteelWidth"].get():.3f}', f'{coreE_f["dFluxDensityRatPerT"].get():.2f}', pWindL_f["iBIL"].get().value, f'{pWindL_f["dNomTurnsUI"].get():.1f}', pWindH_f["iBIL"].get().value,f'{pHCoil_f["dTurns"].get():.1f}',None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "Quotes Entry", None, "$ Costs $", "Updated", None,  None, "Losses & %IZ",None, None, None, None],
        [None, None, f"Base {sMVAKVA}", MVA, "Item", "$ Each", "Weight (lb)", "$ Design", None, "Design", "Guarantees", None, None],
        [None,None,"Coil Nose",f"{device_f['dNoses'].get():.2f}","Core (lb)", f"{costsE_f['dCore'].get():.2f}", s_Weight[0],s_Cost4Item[0], "Core loss", f"{lossesE_f['dCoreLoss'].get():.0f}", sNL, None, None],
        [None, None, "Window Ht", f"{coreE_f['dWindowHeight'].get():.3f}", "Kraft-.002 (lb)", f"{costsE_f['dKraft0022'].get():.2f}", s_Weight[1],  s_Cost4Item[1], "Load loss", f"{lossesE_f['dTotalLLossStrayLead'].get():.0f}", sLL, None, None],
        [None, None, "Core Stack", f"{coreE_f['dTotalStack'].get():.3f}", "Kraft-.005 (lb)", f"{costsE_f['dKraft005'].get():.2f}", s_Weight[2], s_Cost4Item[2], "Total loss", f"{(lossesE_f['dCoreLoss'].get()+lossesE_f['dTotalLLossStrayLead'].get()):.0f}", sTotalL, None, None],
        [None, None, "Fe Wt Lbs", f"{coreE_f['dWeightTwoLoops'].get():.0f}", "Kraft-.010 (lb)", f"{costsE_f['dKraft010'].get():.2f}", s_Weight[3], s_Cost4Item[3], "% IZ", f"{device_f['dIZWithLeads'].get():.2f}", sMinIZ, None, None],
        [None, None, "Core Length",  f"{coreE_f['dCoreLengthWithPB'].get():.3f}", "Crepe (lb)", f"{costsE_f['dCrepe'].get():.2f}", s_Weight[4], s_Cost4Item[4], None, None, None, None, None],
        [None, None, "Total Loss", sTotalLoss, "PB 1/16 (lb)", f"{costsE_f['dPB1_16'].get():.2f}", s_Weight[5], s_Cost4Item[5], None, "Loss Evaluation", None, None, None],
        [None, None, "TOR 'C", f"{device.gradients.fields['dTORFinal'].get():.0f}", "PB 1/8 (lb)", f"{costsE_f['dPB1_8'].get():.2f}", s_Weight[6], s_Cost4Item[6], None, device.guarantee.fields['sEvaluation_nl'].get(), device.guarantee.fields['sEvaluation_ll'].get(), None, None],
        [None, None, "Notes:", None, "Duct (sq.ft)",  f"{costsE_f['dDucts'].get():.2f}", s_Weight[7], s_Cost4Item[7], None, "Guarantees or design", None,  None, None],
        [None, None, None, None, pWindL_f['sTypeConductor'].get(), f"{costsE_f['dLV'].get():.2f}", s_Weight[8], s_Cost4Item[8], None, "Eval NL", sEvaNl, None, None],
        [None, None, None, None, pWindH_f['sTypeConductor'].get(), f"{costsE_f['dHV'].get():.2f}", s_Weight[9], s_Cost4Item[9], None, "Eval LL", sEvaLl, None, None],
        [None, None, None, None, "Coil leads (lb)", f"{costsE_f['dCoilLeads'].get():.2f}", s_Weight[10], s_Cost4Item[10], None, "Eval Total", sEvaTotal, None, None],
        [None, None, None, None, "Busbar (lb)", f"{costsE_f['dBusbar'].get():.2f}",  s_Weight[11], s_Cost4Item[11], None, "C & C + Eval", f"{costsE_f['dCCEval'].get():.0f}", None, None],
        [None, None, None, None, "PB HD 1/4 (lb)",  f"{costsE_f['dPBHD_1_4'].get():.2f}", s_Weight[12], s_Cost4Item[12], None, None, None, None, None],
        [None, None, None, None, None, None, None, f"${costsE_f['dTotalCostRound'].get():.0f}", None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        # [None, None, None, None, None, None, None, None, None, None, None, None, None],
        # [None, None, None, None, None, None, None, None, None, None, None, None, None],
        # [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "Load Losses", "Winding", None, None, "Total Load Losses @ Rated Load other than Full Load", None, None, None, None, None, None],
        [None, None, "Temp (ºC)", "Total LV", "Total HV", "Total @ 100% Load", "25%", "50%", "75%", "125%", "133%", None, None],
        [None, None, "20", f"{lossesE_f['d_20LLWindLV'].get():.0f}", f"{lossesE_f['d_20LLWindHV'].get():.0f}", f"{lossesE_f['d20Total100Load'].get():.0f}", f"{lossesE_f['d20TotalLR25'].get():.0f}", f"{lossesE_f['d20TotalLR50'].get():.0f}", f"{lossesE_f['d20TotalLR75'].get():.0f}", f"{lossesE_f['d20TotalLR125'].get():.0f}", f"{lossesE_f['d20TotalLR133'].get():.0f}", None, None],
        [None, None, "55", f"{lossesE_f['d_55LLWindLV'].get():.0f}", f"{lossesE_f['d_55LLWindHV'].get():.0f}", f"{lossesE_f['d55Total100Load'].get():.0f}", f"{lossesE_f['d55TotalLR25'].get():.0f}", f"{lossesE_f['d55TotalLR50'].get():.0f}", f"{lossesE_f['d55TotalLR75'].get():.0f}", f"{lossesE_f['d55TotalLR125'].get():.0f}", f"{lossesE_f['d55TotalLR133'].get():.0f}", None, None],
        [None, None, "75", f"{lossesE_f['d_75LLWindLV'].get():.0f}", f"{lossesE_f['d_75LLWindHV'].get():.0f}", f"{lossesE_f['d75Total100Load'].get():.0f}", f"{lossesE_f['d75TotalLR25'].get():.0f}", f"{lossesE_f['d75TotalLR50'].get():.0f}", f"{lossesE_f['d75TotalLR75'].get():.0f}", f"{lossesE_f['d75TotalLR125'].get():.0f}", f"{lossesE_f['d75TotalLR133'].get():.0f}", None, None],
        [None, None, "85", f"{lossesE_f['d_85LLWindLV'].get():.0f}", f"{lossesE_f['d_85LLWindHV'].get():.0f}", f"{lossesE_f['d85Total100Load'].get():.0f}", f"{lossesE_f['d85TotalLR25'].get():.0f}", f"{lossesE_f['d85TotalLR50'].get():.0f}", f"{lossesE_f['d85TotalLR75'].get():.0f}", f"{lossesE_f['d85TotalLR125'].get():.0f}", f"{lossesE_f['d85TotalLR133'].get():.0f}", None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "No-Load Losses", sFreq, None, None, None, None, None, None, None, None, None],
        [None, None, "Temp (ºC)", "90%", "95%", "100%", "105%", "110%", "115%", "120%", None, None, None],
        [None, None, "20", s_NoLoadLosses20[0], s_NoLoadLosses20[1], s_NoLoadLosses20[2], s_NoLoadLosses20[3], s_NoLoadLosses20[4], s_NoLoadLosses20[5], s_NoLoadLosses20[6], None, None, None],
        [None, None, "75", s_NoLoadLosses75[0], s_NoLoadLosses75[1], s_NoLoadLosses75[2], s_NoLoadLosses75[3], s_NoLoadLosses75[4], s_NoLoadLosses75[5], s_NoLoadLosses75[6], None, None, None],
        [None, None, "85", s_NoLoadLosses85[0], s_NoLoadLosses85[1], s_NoLoadLosses85[2], s_NoLoadLosses85[3], s_NoLoadLosses85[4], s_NoLoadLosses85[5], s_NoLoadLosses85[6], None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "Voltage =>", "90% Volts", "95% Volts", "100% Volts", "105% Volts", "110% Volts", "115% Volts", "120% Volts", None, None, None],
        [None, None, sIEXC, s_IExcV[0], s_IExcV[1], s_IExcV[2], s_IExcV[3], s_IExcV[4], s_IExcV[5], s_IExcV[6], None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "%Efficiency", sEfficiency, None, None, None, None, None, None, None, None, None],
        [None, None, "Power Factor", "25%", "50%", "75%", "100%", "125%", "133%", "150%", "175%", None, None],
        [None, None, "0.8", s_PerEff80[0], s_PerEff80[1], s_PerEff80[2], s_PerEff80[3], s_PerEff80[4], s_PerEff80[5], s_PerEff80[6], s_PerEff80[7], None, None],
        [None, None, "0.9", s_PerEff90[0], s_PerEff90[1], s_PerEff90[2], s_PerEff90[3], s_PerEff90[4], s_PerEff90[5], s_PerEff90[6], s_PerEff90[7], None, None],
        [None, None, "1.0", s_PerEff100[0], s_PerEff100[1], s_PerEff100[2], s_PerEff100[3], s_PerEff100[4], s_PerEff100[5], s_PerEff100[6], s_PerEff100[7], None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "%Regulation", sRegulation, None, None, None, None, None, None, None, None, None],
        [None, None, "Power Factor", "25%", "50%", "75%", "100%", "125%", "133%", "150%", "175%", None, None],
        [None, None, "0.8", s_PerReg80[0], s_PerReg80[1], s_PerReg80[2], s_PerReg80[3], s_PerReg80[4], s_PerReg80[5], s_PerReg80[6], s_PerReg80[7], None, None],
        [None, None, "0.9", s_PerReg90[0], s_PerReg90[1], s_PerReg90[2], s_PerReg90[3], s_PerReg90[4], s_PerReg90[5], s_PerReg90[6], s_PerReg90[7], None, None],
        [None, None, "1.0", s_PerReg100[0], s_PerReg100[1], s_PerReg100[2], s_PerReg100[3], s_PerReg100[4], s_PerReg100[5], s_PerReg100[6], s_PerReg100[7], None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
        [None, None, "Standard", "Efficiency", "Requirements", None, None, "Req'd Eff.", None, None, None, None, None],
        [None, None, "DoE (2016)", f"{100*device_f['dPerEfficiency'].get():.3f}%", "50% Load; NL @20ºC & 100%V; LL@55ºC", None,None,sDoe2016, sStandard[0], None, None, None, None],
        [None, None, "C802.1", f"{100*device_f['dPerEfficiency'].get():.3f}%", "50% Load; NL @20ºC & 100%V; LL@55ºC", None,None,sC8021, sStandard[1], None, None, None, None, None, None],
        [None, None, "C802.3", f"{100*device_f['dPerEfficiency'].get():.3f}%", "50% Load; NL @20ºC & 100%V; LL@55ºC", None,None,sC8023, sStandard[2], None, None, None, None, None, None],
        [None, None, None, None, None, None, None, None, None, None, None, None, None],
    ]
    
    return data