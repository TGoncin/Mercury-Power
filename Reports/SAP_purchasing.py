import shutil
from pathlib import Path
import win32com.client as win32
from win32com.client import constants
from Resources.resources import resource_path
from Definitions.functions import vlookup,safe_div,is_folder_open,ROUNDECIM
from Definitions.enums import eLaminationType,eLaminationTypeNum,eTypeConductor,eMaterial
import math
import os

def make_SAP_purchasing(device=None):
    """
    Copy the Mercury BOM file to the user's Documents, unblock it, update SAP tables,
    add or refresh 'CC-Mercury' sheet, and then open the modified file.
    """

    # 1️⃣ Original file
    original_file = resource_path("Resources/Mercury Single Phase - BOM - R1.xlsm")
    if not Path(original_file).exists():
        raise FileNotFoundError(f"Original file not found: {original_file}")

    # 2️⃣ Destination folder: Documents\<SO_NUMBER>
    documents_folder = Path.home() / "Documents"
    so_number = str(device.fields["so_number"].get())
    dest_folder = documents_folder / so_number
    dest_folder.mkdir(parents=True, exist_ok=True)

    # 3️⃣ New file path
    new_file = dest_folder / "Mercury Single Phase - BOM - R1.xlsm"

    # 4️⃣ Copy exact file
    shutil.copy2(original_file, new_file)

    # 5️⃣ Remove Windows Mark of the Web (unblock)
    zone_identifier = str(new_file) + ":Zone.Identifier"
    try:
        if Path(zone_identifier).exists():
            Path(zone_identifier).unlink()
    except Exception:
        pass

    print(f"File copied and unblocked successfully to: {new_file}")

    # 6️⃣ Open Excel via COM Automation
    excel = win32.gencache.EnsureDispatch('Excel.Application')
    excel.Visible = False
    excel.DisplayAlerts = False  # avoid prompts
    wb = excel.Workbooks.Open(str(new_file))

    # 7️⃣ Fixed SAP sheet names and corresponding tables
    sheet_table_map = {
        "SAP_STD_CONDUCTORS": device.SAP_STD_CONDUCTORS,
        "SAP_STD_INS_CORE_LEADS": device.SAP_STD_INS_CORE_LEADS,
        "SAP_STD_BUSHINGS": device.SAP_STD_BUSHINGS,
        "SAP_STD_SPADES": device.SAP_STD_SPADES,
        "SAP_STD_SWITCHES": device.SAP_STD_SWITCHES,
        "SAP_STD_FUSES": device.SAP_STD_FUSES,
        "SAP_STD_GAUGES": device.SAP_STD_GAUGES,
        "ME TA-SS_LIST": device.ME_TA_SS_LIST,
        "ME Components": device.ME_COMPONENTS,
        "ME TA-PP_LIST": device.ME_TA_PP_LIST,
        "ME_TANK STEEL & OIL": device.ME_TANK_STEEL_OIL
    }

    # Colores (RGB → BGR para Excel)
    BLUE_HEADER   = 0xF8E9DA   # Azul claro
    ORANGE_HEADER = 0xACC7F7   # Naranja claro
    PINK_HEADER   = 0xE49EDD   # Rosa claro
    BLACK         = 0x000000

    # 8️⃣ Update SAP sheets
    for sheet_name, table in sheet_table_map.items():
        if sheet_name.upper() == "BOM":
            continue

        try:
            ws = wb.Sheets(sheet_name)
        except Exception:
            ws = wb.Sheets.Add()
            ws.Name = sheet_name

        if table.name=="ME Components":
            l = 5
        # Clear existing data except row 1
        used_range = ws.UsedRange
        if used_range.Rows.Count > 1 or used_range.Columns.Count > 1:
            ws.Range(ws.Cells(2, 1), ws.Cells(used_range.Rows.Count, used_range.Columns.Count)).ClearContents()

        # Write headers
        for col, header in enumerate(table.tb_headers, start=1):
            ws.Cells(1, col).Value = header

        # Write data
        for row_idx, row_data in enumerate(table.tb_data, start=2):
            for col_idx, value in enumerate(row_data, start=1):
                ws.Cells(row_idx, col_idx).Value = value

    # 9️⃣ Add or refresh "CC-Mercury" sheet after BOM
    try:
        bom_sheet = wb.Sheets("BOM")
    except Exception:
        bom_sheet = wb.Sheets.Add()
        bom_sheet.Name = "BOM"

    # Check if CC-Mercury exists
    try:
        cc_sheet = wb.Sheets("CC-Mercury")
        # Clear previous content
        used_range = cc_sheet.UsedRange
        if used_range.Rows.Count > 1 or used_range.Columns.Count > 1:
            cc_sheet.Range(cc_sheet.Cells(2, 1),
                           cc_sheet.Cells(used_range.Rows.Count, used_range.Columns.Count)).ClearContents()
    except Exception:
        # Add new sheet after BOM
        cc_sheet = wb.Sheets.Add(After=bom_sheet)
        cc_sheet.Name = "CC-Mercury"

    # Headers for CC-Mercury
    cc_headers = ["SAP Name","Price (CAD)","Last Purchase Date","Component","Supplier","Type",
                  "Quantity","Total Item Price","Total Price"]

    header_row = 1
    last_col = len(cc_headers)

    for col, header in enumerate(cc_headers, start=1):
        cc_sheet.Cells(1, col).Value = header

    cc_data =[]
    pWindH = device.getHVWind()
    pWindL = device.getLVWind()
    iBusLdMatE = device.losses.fields["iBusLdMat"].get()
    iTypeLVE = pWindL.coils[0].conductor[0].fields["iType"].get()
    tTypeMatAlLVE = pWindL.coils[0].conductor[0].fields["tMatAluminum"].get()
    iNoConductorLVE = pWindL.coils[0].conductor[0].fields["iNoConductor"].get()
    iTypeHVE = pWindH.coils[0].conductor[0].fields["iType"].get()
    tTypeMatAlHVE = pWindH.coils[0].conductor[0].fields["tMatAluminum"].get()
    iNoConductorHVE = pWindH.coils[0].conductor[0].fields["iNoConductor"].get()
    tLaminationTypeE = device.core.fields["tLaminationType"].get()

    dWeightCore = ROUNDECIM(device.core.fields["dWeightTwoLoops"].get(),3)
    dLVWeight = ROUNDECIM(pWindL.fields["dWeight"].get(),3)
    dHVWeight = ROUNDECIM(pWindH.fields["dWeight"].get(),3)
    dWeightKr0022 = ROUNDECIM(device.fields["dWeightKr0022"].get(),3)
    dWeightKr005 = ROUNDECIM(device.fields["dWeightKr005"].get(),3)
    dWeightKr010 = ROUNDECIM(device.fields["dWeightKr010"].get(),3)
    dWeightCrepe = ROUNDECIM(device.fields["dWeightCrepe"].get(),3)
    dWeightPB1_16 = ROUNDECIM(device.core.fields["dWeightsPB1_16"].get(),3)
    dWeightPB1_8 = ROUNDECIM(device.core.fields["dWeightsPB1_8"].get(),3)
    dWeightDucts = ROUNDECIM(device.core.fields["d_DuctsDM"].get()[0],3)
    dWeightCoilLeads = ROUNDECIM(pWindL.fields["dWeightCoilLeads"].get(),3)
    dWeightBusBar = ROUNDECIM(pWindL.fields["dWeightBusBar"].get(),3)

    sCore = device.core.fields["tLaminationType"].get().value
    sTypeCore = "Core"
    sLastTimeCore = "Optimizer"

    tab = device.SAP_STD_INS_CORE_LEADS.tb_data
    sSAPKr0022 = "2KP"
    dPriceKr0022 = vlookup(sSAPKr0022, tab, 1)
    sLastTimeKr0022 = vlookup(sSAPKr0022, tab, 2)
    sTypeKr0022 = vlookup(sSAPKr0022, tab, 3)

    sSAPKr005 = "5KP"
    dPriceKr005 = vlookup(sSAPKr005, tab, 1)
    sLastTimeKr005 = vlookup(sSAPKr005, tab, 2)
    sTypeKr005 = vlookup(sSAPKr005, tab, 3)

    sSAPKr010 = "10KP"
    dPriceKr010 = vlookup(sSAPKr010, tab, 1)
    sLastTimeKr010 = vlookup(sSAPKr010, tab, 2)
    sTypeKr010 = vlookup(sSAPKr010, tab, 3)

    sSAPCrepe = "510HC"
    dPriceCrepe = vlookup(sSAPCrepe, tab, 1)
    sLastTimeCrepe = vlookup(sSAPCrepe, tab, 2)
    sTypeCrepe = vlookup(sSAPCrepe, tab, 3)

    sSAPPB1_16 = "16PB"
    dPricePB1_16 = vlookup(sSAPPB1_16, tab, 1)
    sLastTimePB1_16 = vlookup(sSAPPB1_16, tab, 2)
    sTypePB1_16 = vlookup(sSAPPB1_16, tab, 3)

    sSAPPB1_8 = "8PB"
    dPricePB1_8 = vlookup(sSAPPB1_8, tab, 1)
    sLastTimePB1_8 = vlookup(sSAPPB1_8, tab, 2)
    sTypePB1_8 = vlookup(sSAPPB1_8, tab, 3)

    sSAPDucts = "8DUCT"
    dPriceDucts = vlookup(sSAPDucts, tab, 1)
    sLastTimeDucts = vlookup(sSAPDucts, tab, 2)
    sTypeDucts = vlookup(sSAPDucts, tab, 3)

    sSAPLeads = pWindL.fields["tTypeLeads"].get().value
    dPricLeads = vlookup(sSAPLeads, tab, 1)
    sLastTimeLeads = vlookup(sSAPLeads, tab, 2)
    sTypeLeads = vlookup(sSAPLeads, tab, 3)

    sSAPBusBar = device.fields["tTypeBuswork"].get().value
    dPricBusBar = vlookup(sSAPBusBar, tab, 1)
    sLastTimeBusBar = vlookup(sSAPBusBar, tab, 2)
    sTypeBusBar = vlookup(sSAPBusBar, tab, 3)

    dPriceCore = device.costs.fields["dCore"].get()
    sSAPLV = 0.0
    dPriceLV = 0.0
    if iTypeLVE == eTypeConductor.STRIP:
        if tTypeMatAlLVE == eMaterial.CU:
            sSAPLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_COPPER_strip, 7)  
            dPriceLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_COPPER_strip, 6)    
        else:
            sSAPLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_ALUMINUM_strip, 7)  
            dPriceLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_ALUMINUM_strip, 6)  
    else:
        if tTypeMatAlLVE == eMaterial.CU:
            sSAPLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_COPPER_magnet, 7) 
            dPriceLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_COPPER_magnet, 6)
        else:
            sSAPLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_ALUMINUM_magnet, 7)
            dPriceLV = vlookup(iNoConductorLVE, device.tbConductor.tb_data_ALUMINUM_magnet, 6)
    sLastTimeLV = vlookup(sSAPLV, device.SAP_STD_CONDUCTORS.tb_data, 2)  
    sTypeLV = vlookup(sSAPLV, device.SAP_STD_CONDUCTORS.tb_data, 4)

    sSAPHV = 0.0
    dPriceHV = 0.0
    if iTypeHVE == eTypeConductor.STRIP:
        if tTypeMatAlLVE == eMaterial.CU:
            sSAPHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_COPPER_strip, 7)  
            dPriceHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_COPPER_strip, 6)    
        else:
            sSAPHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_ALUMINUM_strip, 7)  
            dPriceHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_ALUMINUM_strip, 6)  
    else:
        if tTypeMatAlLVE == eMaterial.CU:
            sSAPHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_COPPER_magnet, 7) 
            dPriceHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_COPPER_magnet, 6)
        else:
            sSAPHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_ALUMINUM_magnet, 7)
            dPriceHV = vlookup(iNoConductorHVE, device.tbConductor.tb_data_ALUMINUM_magnet, 6)
    sLastTimeHV = vlookup(sSAPHV, device.SAP_STD_CONDUCTORS.tb_data, 2)  
    sTypeHV = vlookup(sSAPHV, device.SAP_STD_CONDUCTORS.tb_data, 4)
    
    
    # Example matrix data to populate A2 (replace with real data as needed)
    if dWeightCore>0.0:
        cc_data.append([sCore, dPriceCore, sLastTimeCore, None, None, sTypeCore, dWeightCore, dWeightCore*dPriceCore, None])
    if dWeightKr0022>0.0:
        cc_data.append([sSAPKr0022, dPriceKr0022, sLastTimeKr0022, None, None, sTypeKr0022, dWeightKr0022, dWeightKr0022*dPriceKr0022, None])
    if dWeightKr005  >0.0:
        cc_data.append([sSAPKr005, dPriceKr005, sLastTimeKr005, None, None, sTypeKr005, dWeightKr005, dWeightKr005 *dPriceKr005, None])
    if dWeightKr010>0.0:
        cc_data.append([sSAPKr010, dPriceKr010, sLastTimeKr010, None, None, sTypeKr010, dWeightKr010, dWeightKr010*dPriceKr010, None])
    if dWeightCrepe>0.0:
        cc_data.append([sSAPCrepe, dPriceCrepe, sLastTimeCrepe, None, None, sTypeCrepe, dWeightCrepe, dWeightCrepe*dPriceCrepe, None])
    if dWeightPB1_16 >0.0:
        cc_data.append([sSAPPB1_16, dPricePB1_16, sLastTimePB1_16, None, None, sTypePB1_16, dWeightPB1_16, dWeightPB1_16 *dPricePB1_16, None])
    if dWeightPB1_8  >0.0:
        cc_data.append([sSAPPB1_8, dPricePB1_8, sLastTimePB1_8, None, None, sTypePB1_8, dWeightPB1_8, dWeightPB1_8  *dPricePB1_8, None])
    if dWeightDucts  >0.0:
        cc_data.append([sSAPDucts, dPriceDucts, sLastTimeDucts, None, None, sTypeDucts, dWeightDucts, dWeightDucts  *dPriceDucts, None])
    if dLVWeight>0.0:
        cc_data.append([sSAPLV, dPriceLV, sLastTimeLV, None, None, sTypeLV, dLVWeight, dLVWeight*dPriceLV, None])
    if dHVWeight>0.0:
        cc_data.append([sSAPHV, dPriceHV, sLastTimeHV, None, None, sTypeHV, dHVWeight, dHVWeight*dPriceHV, None])
    if dWeightCoilLeads   >0.0:
        cc_data.append([sSAPLeads, dPricLeads, sLastTimeLeads, None, None, sTypeLeads, dWeightCoilLeads, dWeightCoilLeads*dPricLeads, None])
    if dWeightBusBar   >0.0:
        cc_data.append([sSAPBusBar, dPricBusBar, sLastTimeBusBar, None, None, sTypeBusBar, dWeightDucts, dWeightBusBar*dPricBusBar, None])
    
    # Write matrix to CC-Mercury starting at A2
    for row_idx, row in enumerate(cc_data, start=2):
        for col_idx, value in enumerate(row, start=1):
            cc_sheet.Cells(row_idx, col_idx).Value = value

    header_range = cc_sheet.Range(
        cc_sheet.Cells(header_row, 1),
        cc_sheet.Cells(header_row, last_col)
    )

    # Formato general encabezados
    header_range.Font.Bold = True
    header_range.Font.Color = BLACK
    header_range.HorizontalAlignment = constants.xlCenter
    header_range.VerticalAlignment = constants.xlCenter
    header_range.WrapText = True

    # Aplicar colores por columna
    for col in range(1, last_col + 1):
        cell = cc_sheet.Cells(1, col)

        # Quantity & Total Item Price
        if col in [7, 8]:
            cell.Interior.Color = ORANGE_HEADER

        # Total Price
        elif col == 9:
            cell.Interior.Color = PINK_HEADER

        # Resto
        else:
            cell.Interior.Color = BLUE_HEADER

    # Bordes para encabezados
    header_range.Borders.LineStyle = constants.xlContinuous
    header_range.Borders.Weight = constants.xlThin

    # Autoajustar columnas
    cc_sheet.Columns("A:I").AutoFit()

    # Congelar encabezado
    cc_sheet.Application.ActiveWindow.SplitRow = 1
    cc_sheet.Application.ActiveWindow.FreezePanes = True

    # 10️⃣ Save workbook
    wb.Save()
    # Close the workbook and quit Excel (release resources)
    wb.Close(SaveChanges=True)
    excel.Quit()
    del excel  # release COM object

    if not is_folder_open(dest_folder):
        os.startfile(dest_folder)
    # 11️⃣ Open the modified file for the user in a new instance
    # excel_user = win32.gencache.EnsureDispatch('Excel.Application')
    # excel_user.Visible = True
    # wb_user = excel_user.Workbooks.Open(str(new_file))

    # print(f"SAP tables updated and 'CC-Mercury' sheet added in: {new_file}")
