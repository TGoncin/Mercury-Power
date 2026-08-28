# Serialize.py
import xml.etree.ElementTree as ET
from xml.dom import minidom
from enum import Enum
from Definitions.enums import eArrayType
import ast


IGNORE_FIELDS = {"previousarray_type"}

def serialize_fields(obj, parent_element):
    """
    Serializes all fields and child objects of an object to XML.
    """
    # Serialize the fields of the object
    if hasattr(obj, "fields"):
        for field_name, field in obj.fields.items():

            if hasattr(field, "isDefault") and field.isDefault():
                continue

            field_elem = ET.SubElement(parent_element, "Field", name=field_name)
            value = getattr(field, "current", "")
            
            # Handle lists or tuples
            if isinstance(value, (list, tuple)):
                for i, val in enumerate(value):
                    val_str = val.name if isinstance(val, Enum) else str(val)
                    ET.SubElement(field_elem, "Value", index=str(i)).text = val_str
            else:
                val_str = value.name if isinstance(value, Enum) else str(value)
                ET.SubElement(field_elem, "Value").text = val_str

    # Serialize unique child objects
    children_attrs = ["cooling", "guarantee", "losses", "core", "tests", "gradients","costs","mechanical"]
    for attr_name in children_attrs:
        child_obj = getattr(obj, attr_name, None)
        if child_obj is not None:
            child_elem = ET.SubElement(parent_element, "Child", name=getattr(child_obj, "object_name", attr_name))
            serialize_fields(child_obj, child_elem)

    if hasattr(obj, "windings"):
        windings_list = getattr(obj, "windings")
        windings_elem = ET.SubElement(parent_element, "List", name="windings")
        for i, winding in enumerate(windings_list):
            winding_item_elem = ET.SubElement(windings_elem, "Item", index=str(i), name=getattr(winding, "object_name", "winding"))
            serialize_fields(winding, winding_item_elem)

            # changer dentro de cada winding (objeto único)
            if hasattr(winding, "changer"):
                changer = getattr(winding, "changer")
                if changer:
                    changer_elem = ET.SubElement(winding_item_elem, "Child", name=getattr(changer, "object_name", "changer"))
                    serialize_fields(changer, changer_elem)

            # coils dentro de cada winding
            if hasattr(winding, "coils"):
                coils_list = getattr(winding, "coils")
                if coils_list:
                    coils_elem = ET.SubElement(winding_item_elem, "List", name="coils")
                    for j, coil in enumerate(coils_list):
                        coil_item_elem = ET.SubElement(coils_elem, "Item", index=str(j), name=getattr(coil, "object_name", "coil"))
                        serialize_fields(coil, coil_item_elem)

                        # conductors dentro de cada coil
                        if hasattr(coil, "conductor"):
                            cond_list = getattr(coil, "conductor")
                            if cond_list:
                                cond_elem = ET.SubElement(coil_item_elem, "List", name="conductor")
                                for k, cond in enumerate(cond_list):
                                    cond_item_elem = ET.SubElement(cond_elem, "Item", index=str(k), name=getattr(cond, "object_name", "Item"))
                                    serialize_fields(cond, cond_item_elem)

def serialize_fields_opt(obj, parent_element):
    """
    Serializes selected fields and child objects of an object to XML.
    allowed_fields: list of field names that should be serialized.
    If None, serialize all fields (default behavior).
    """
    allowed_fields = [
        # Generales
        "array_type",
        "so_number",
        "customer_name",
        "customer_code",
        "quantity",
        "issued_by",
        "original_issue_date",
        "sSalesPerson",
        "requested_due_date",
        "nominal_kva", 
        "frequency",
        "transformer_family",
        "winding_description",
        "iNoPhases",
        "dLVBuildFactor",
        "dHVBuildFactor",
        "dCoilRB",
        "dLVCondWghtFactor",
        "dHVCondWghtFactor",
        "dPaperWghtFactor",
        "dCoreLossFactor",
        "dResistance",
        "dLoadLossFactor",
        "dInsulationLengthFactor",
        "dNoseBuildFactor",
        "dFreqFactorHz",
        "dAmorphousLossVariance",
        "dAmAssemblyInducedStress",
        "iFluxDensityMin",
        "iFluxDensityMax",
        "iSteelWidthMin",
        "iSteelWidthMax",
        "iTotalStackMin",
        "iTotalStackMax",
        "iCurrentDensityLVMin",
        "iCurrentDensityLVMax",
        "iCurrentDensityHVMin",
        "iCurrentDensityHVMax",
        "iRadialLVMax",
        "dImpedanceMin",
        "dImpedanceMax",

        # Guarantee
        "dEvaluation_nl",
        "dEvaluation_ll",
        "sCustomerSpec",
        "tDesignSpec",
        "bNLCorrected_to_85C",
        "dNL_pu",
        
        # Cooling
        "tCoolingTypes", 
        "tTemperatureRises",
        "tK4Factor",

        # Taps (changer)
        "bHaveTaps",
        "dPerUp",
        "dPerDown",
        "iNumSteps",
        "tCapacity",
        "sInstructions",
        "ground_Y",
        "iNumBushings",

        #winding   
        "tDescription",
        "dVoltage",
        "dSerieVoltage",
        "type_connection",
        "iBIL",
        "iQuantityLeads",
        "tMaterialLeads",
        "tTypeLeads",
        "dThicknessLeads",
        "dWidthLeads",
        "iQuantityBuswork",
        "tMaterialBuswork",
        "tTypeBuswork",
        "dThicknessBuswork",
        "dWidthBuswork",
        "sHVTapLeads",
        "dBuildFactor",
        "ground_Y",
        "bHalfTurns",

        #core
        "tLaminationType",
        "dTotalStack",
        "dSteelWidth",
        "dPerOvervoltageMaxFlux",
        "dTestInductionLevel",
        "dMaxWattsPound",
        "dLossFact",
        "dFormNose",
        "dFormSide",
        "dLamFactor",
        "dStackTolPIn",
        "dStackTolMIn",
        "iNoLoops",

        #coil
        "dTurnsNom",
        "dNomTurnsUI",
        "iLayers",
        "iType",
        "tMatAluminum",
        "iNoConductor",
        "dWidth",
        "iNoAxial",
        "iNoRadial",
        "sDescription",
        "dLeadBuild",
        "iNoDuctInner",
        "iNoDuctOuter",
        "dDuctSize",
        "tCoating",
        "bConstantStayback",
        "dBuildFactorUsed",

        #Revision data
        "data_description",
        
        #Guarantees
        "dNLGuarantee",
        "dLLGuarantee",
        "dIZGuarantee",
        "dExcGuarantee",

        #General Optionals & Comments
        "sConnectionDiagramsItems",
        "dOverallRBFactor",
        "dAltitude",
        "sWindingSpecComments",
        "sSheetComments",
        "sPurchasingComments",
        "sInsulationComments",
        "dONANLosses",
        "dONANTOR",

        #Optional Core Data
        "dWindingHeightAdder",
        "dWindingWidthAdder",
        "dCoilCorePBEnd",
        "dCoilCoilPBSides",
        "dCoilCorePBSides",
        "dFixedSmWindowWidth",
        "dFixedWindowHeight",
        
        #Optional LV Data
        "sDuctPlacement",
        "dOptStayback",
        "dLayerInsulation",
        "dBuildFactor",
        "bEndfiller",
        "dEndInsulation",
        "dLeadPadWitdh",
        "iLeadPads",

        #Opt BIL Under HV
        "iBILUnder",
        "sHLUnder",
        "dHLUnderRB",
        "sHLOver",
        "dHLOverRB",
        "sPHTapLeads",

        #Routine Tests
        "tHipot",
        "dHipotHV",
        "dHipotLV",
        "bInduced",
        "dXRatedVoltage",
        "bResistance",
        "bOtherResistance",
        "tTapsExtremesORAII",
        "sSampleRate",
        "bCoreLossTest",
        "dExtraNoLoad",
        "bLoadLossTest",
        "bLoadLossTap",
        "d3LoadLosskVA",
        "d3LoadLossTap",
        "d4LoadLosskVA",
        "d4LoadLossTap",
        "d5LoadLosskVA",
        "d5LoadLossTap",
        "dDissipationFactor",
        "bOtherRoutine",
        "sOtherRoutineComments",

        #Type Tests
        "bTemperatureRise",
        "tSpecificationsTR",
        "sCommentsTR",
        "bLightningImpulse",
        "tSpecificationsLI",
        "tSequenceLI",
        "tImpulse",
        "sCommentsLI",
        "bRIV",
        "tSpecificationsRIV",
        "sCommentsRIV",
        "bSoundLevel",
        "tSpecificationsSL",
        "sCommentsSL",
        "bPartialDischarge",
        "tSpecificationsPD",
        "sCommentsPD",
        "bOtherTypeTest",
        "sOtherRoutineCommentsType",
        "sFormMaterial",
        "dFormMaterialRB",
        "iSectionsLeg",

        #mechanical
        "tSegment"
    ]

    optimizer_fields = [
        "iFluxDensityMin",
        "iFluxDensityMax",
        "iSteelWidthMin",
        "iSteelWidthMax",
        "iTotalStackMin",
        "iTotalStackMax",
        "iCurrentDensityLVMin",
        "iCurrentDensityLVMax",
        "iCurrentDensityHVMin",
        "iCurrentDensityHVMax",
        "iRadialLVMax",
        "dImpedanceMin",
        "dImpedanceMax"
    ]

    # Serialize only allowed fields
    if hasattr(obj, "fields"):
        for field_name, field in obj.fields.items():

            # if allowed_fields is not None and field_name not in allowed_fields:
            #     continue
            if allowed_fields is not None and field_name not in allowed_fields:
                if hasattr(field, "isDefault") and field.isDefault():
                    continue

            if hasattr(field, "isDefault") and field.isDefault() and field_name not in optimizer_fields:
                continue

            field_elem = ET.SubElement(parent_element, "Field", name=field_name)
            value = getattr(field, "current", "")

            # Lists or tuples
            if isinstance(value, (list, tuple)):
                for i, val in enumerate(value):
                    val_str = val.name if isinstance(val, Enum) else str(val)
                    ET.SubElement(field_elem, "Value", index=str(i)).text = val_str
            else:
                val_str = value.name if isinstance(value, Enum) else str(value)
                ET.SubElement(field_elem, "Value").text = val_str

    # Unique child objects
    children_attrs = ["cooling", "guarantee", "losses", "core", "tests", "gradients", "costs", "mechanical"]
    for attr_name in children_attrs:
        child_obj = getattr(obj, attr_name, None)
        if child_obj is not None:
            child_elem = ET.SubElement(parent_element, "Child", name=getattr(child_obj, "object_name", attr_name))
            serialize_fields_opt(child_obj, child_elem)

    # Windings list
    if hasattr(obj, "windings"):
        windings_list = getattr(obj, "windings")
        windings_elem = ET.SubElement(parent_element, "List", name="windings")
        for i, winding in enumerate(windings_list):
            winding_item_elem = ET.SubElement(windings_elem, "Item", index=str(i), name=getattr(winding, "object_name", "winding"))
            serialize_fields_opt(winding, winding_item_elem)

            # changer
            if hasattr(winding, "changer"):
                changer = getattr(winding, "changer")
                if changer:
                    changer_elem = ET.SubElement(winding_item_elem, "Child", name=getattr(changer, "object_name", "changer"))
                    serialize_fields_opt(changer, changer_elem)

            # coils
            if hasattr(winding, "coils"):
                coils_list = getattr(winding, "coils")
                if coils_list:
                    coils_elem = ET.SubElement(winding_item_elem, "List", name="coils")
                    for j, coil in enumerate(coils_list):
                        coil_item_elem = ET.SubElement(coils_elem, "Item", index=str(j), name=getattr(coil, "object_name", "coil"))
                        serialize_fields_opt(coil, coil_item_elem)

                        # conductors
                        if hasattr(coil, "conductor"):
                            cond_list = getattr(coil, "conductor")
                            if cond_list:
                                cond_elem = ET.SubElement(coil_item_elem, "List", name="conductor")
                                for k, cond in enumerate(cond_list):
                                    cond_item_elem = ET.SubElement(cond_elem, "Item", index=str(k), name=getattr(cond, "object_name", "Item"))
                                    serialize_fields_opt(cond, cond_item_elem)

def serialize_device_to_xml(device, filename,bOptimizer = False):
    """
    Serializes a complete Device object to a human-readable XML file.
    """
    try:
        root = ET.Element("Device")
        if bOptimizer==True:
            serialize_fields_opt(device, root)
        else:
            serialize_fields(device, root)

        # Prettify the XML for readability
        rough_string = ET.tostring(root, 'utf-8')
        reparsed = minidom.parseString(rough_string)
        pretty_xml = reparsed.toprettyxml(indent="  ")

        # Save to file
        with open(filename, "w", encoding="utf-8") as f:
            f.write(pretty_xml)

        return True
    except Exception as e:
        print(f"Error saving XML: {e}")
        return False  # Failed


def convert_like_default(default_value, xml_value):
    """
    Convierte un valor del XML al mismo tipo que el default del FieldValue.
    """
    if isinstance(default_value, dict):
        try:
            parsed = ast.literal_eval(xml_value)
            result = {}
            for k, arr in parsed.items():
                if k in default_value:
                    sub_default = default_value[k]
                    result[k] = [convert_like_default(sub_default[0], v) for v in arr]
                else:
                    result[k] = arr
            return result
        except:
            return default_value

    if isinstance(default_value, list):
        if isinstance(xml_value, str):
            try:
                xml_value = ast.literal_eval(xml_value)
            except:
                return default_value
        if len(default_value) == 0:
            return xml_value
        return [convert_like_default(default_value[0], x) for x in xml_value]

    if isinstance(default_value, float):
        try:
            return float(xml_value)
        except:
            return default_value

    if isinstance(default_value, int):
        try:
            return int(xml_value)
        except:
            return default_value

    if hasattr(default_value, "name"):
        enum_type = type(default_value)
        try:
            return enum_type[xml_value]
        except:
            pass
        try:
            return enum_type(int(xml_value))
        except:
            return default_value

    if isinstance(default_value, str):
        return str(xml_value) if xml_value!=None else ""

    return xml_value


def load_fields(obj, xml_element):
    """
    Recursively loads XML values into objects, including:
    - fields
    - child objects
    - lists of child objects (windings, coils, conductors)
    Works automatically without list_classes or _item_class.
    """
    def convert_list_like_default(default_value, xml_value):
        import ast

        if isinstance(default_value, list):
            # Si es string, intentar evaluarlo
            if isinstance(xml_value, str):
                try:
                    xml_value = ast.literal_eval(xml_value)
                except:
                    return default_value
        
            # Si no es iterable, envolver en lista
            if not isinstance(xml_value, (list, tuple)):
                xml_value = [xml_value]
        
            if len(default_value) == 0:
                return xml_value
        
            # Recorre xml_value y convierte cada elemento
            return [convert_like_default(default_value[0], x) for x in xml_value]
    
        else:
            return convert_like_default(default_value, xml_value)

    # --------- Load fields ---------
    if hasattr(obj, "fields"):
        for field_elem in xml_element.findall("Field"):
            name = field_elem.get("name")
            if name in IGNORE_FIELDS:
                continue

            field_obj = obj.fields.get(name)
            if not field_obj:
                continue

            if name=="iNoConductor":
                l = 15
            if name=="iNomPos":
                continue

            if name == "dVoltage":
                values = [int(float(v.text)) for v in field_elem.findall("Value")]
            else:
                values = [v.text for v in field_elem.findall("Value")]

            if values:
                if len(values) == 1:
                    field_obj.set(convert_like_default(field_obj.default, values[0]))
                else:
                    field_obj.set([convert_like_default(field_obj.default[i], v) for i, v in enumerate(values)])
            
            if name == "array_type" and hasattr(obj, "assignCoilTypes"):
                obj.assignCoilTypes(field_obj.get())

    # --------- Load child objects ---------
    for child_elem in xml_element.findall("Child"):
        name = child_elem.get("name")
        child_obj = getattr(obj, name, None)
        if child_obj:
            load_fields(child_obj, child_elem)

    # --------- Load lists of objects ---------
    # detectar automáticamente listas en el objeto
    for attr_name in dir(obj):
        if attr_name.startswith("_") or callable(getattr(obj, attr_name)):
            continue

        target_list = getattr(obj, attr_name)
        if isinstance(target_list, list):
            # Encontrar la lista correspondiente en XML
            list_elem = xml_element.find(f"./List[@name='{attr_name}']")
            if list_elem is None:
                continue

            for item_elem in list_elem.findall("Item"):
                index = int(item_elem.get("index", 0))
                if index >= len(target_list):
                    # crear objeto según tipo del primer elemento
                    item_class = type(target_list[0]) if len(target_list) > 0 else None
                    if item_class is None:
                        continue
                    target_list.append(item_class())

                # Cargar campos del item
                load_fields(target_list[index], item_elem)

                # **Recursivamente cargar listas internas del item**
                # Esto permite que coil.conductors se llene
                load_fields(target_list[index], item_elem)

def deserialize_device_from_xml(device, file_path):
    try:
        device.reset_fields()
        tree = ET.parse(file_path)
        xmlroot = tree.getroot()
        load_fields(device, xmlroot)
        print(f"Device loaded from {file_path}")
        return True
    except Exception as e:
        print(f"Error loading device from {file_path}: {e}")
        return False
