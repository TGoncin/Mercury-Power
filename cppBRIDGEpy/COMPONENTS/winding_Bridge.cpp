//    @date      August 24, 2026
//    @author    Tom Goncin


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "winding.h"
#include "device.h"
#include "coil.h"
#include "bushing.h"

namespace py = pybind11;




void bridge_Winding(py::module_& m)
{
    py::class_<home_Winding>(m, "Winding")
 

// Constructors

        .def(py::init<>(), "Initialize Winding framework")

// Fields Map & Vectors

        .def_readwrite("winding_fields", &home_Winding::windingFields, "Map of Winding FieldValues")
        .def_readwrite("changer", &home_Winding::changer, "Associated Tap Changer subsystem")
        .def_readwrite("bushings", &home_Winding::bushings, "Vector of Bushing objects")
        .def_readwrite("coils", &home_Winding::coils, "Vector of Coil pointers")

// Navigation Methods

        .def("get_parent_device", &home_Winding::getParentDevice, py::return_value_policy::reference, "Get parent device pointer")

// State & Pipeline Calculation Methods

        .def("have_taps", &home_Winding::have_Taps, "Check if winding has active tap steps")
        .def("get_nominal_position", &home_Winding::get_NominalPosition, "Get nominal tap position")
        .def("get_number_steps", &home_Winding::get_NumberSteps, "Get number of steps")
        .def("get_winding_type", &home_Winding::get_WindingType, "Get winding category descriptor type")
        .def("add_coils", &home_Winding::add_Coils, py::arg("coil"), "Add coil to winding collection")
        .def("reset_winding_fields", &home_Winding::reset_WindingFields, "Reset all internal winding and changer fields")
        .def("erase_warnings", &home_Winding::erase_Warnings, "Clear warning states")
        .def("use_ducts", &home_Winding::use_Ducts, "Evaluate duct usage criteria")
        .def("calc_warnings", &home_Winding::calc_Warnings, "Calculate winding and lead warnings")
        .def("calc_winding", &home_Winding::calc_Winding, "Execute basic winding calculation pipeline")
        .def("calc_default_kva", &home_Winding::calc_DefaultKVA, py::arg("device"), "Calculate default KVA steps")
        .def("calc_default_line_kv", &home_Winding::calc_DefaultLineKV, "Calculate default line kilovolts")
        .def("calc_default_phase_kv", &home_Winding::calc_DefaultPhaseKV, py::arg("device"), py::arg("winding_hv"), py::arg("winding_lv"), py::arg("winding_tertiary"), "Calculate phase kilovolts definition")
        .def("calc_amperage", &home_Winding::calc_Amperage, "Calculate line and phase amperages")
        .def("calc_leads", &home_Winding::calc_Leads, "Calculate busbar lead dimensions")

// Getters

        .def("get_id", &home_Winding::getId)
        .def("get_name", &home_Winding::getName)
        .def("get_description", &home_Winding::getDescription)
        .def("get_voltage", &home_Winding::getVoltage)
        .def("get_type_connection", &home_Winding::getTypeConnection)
        .def("get_bil", &home_Winding::getBIL)
        .def("get_bil_under", &home_Winding::getBILUnder)
        .def("get_half_turns", &home_Winding::getHalfTurns)
        .def("get_coating", &home_Winding::getCoating)
        .def("get_nom_turns_ui", &home_Winding::getNomTurnsUI)
        .def("get_type_leads", &home_Winding::getTypeLeads)
        .def("get_quantity_leads", &home_Winding::getQuantityLeads)
        .def("get_thickness_leads", &home_Winding::getThicknessLeads)
        .def("get_width_leads", &home_Winding::getWidthLeads)
        .def("get_leads_density", &home_Winding::getLeadsDensity)
        .def("get_s_leads_density", &home_Winding::getSLeadsDensity)
        .def("get_no_duct_inner", &home_Winding::getNoDuctInner)
        .def("get_no_duct_outer", &home_Winding::getNoDuctOuter)
        .def("get_duct_size", &home_Winding::getDuctSize)
        .def("get_duct_placement", &home_Winding::getDuctPlacement)
        .def("get_opt_stayback", &home_Winding::getOptStayback)
        .def("get_layer_insulation", &home_Winding::getLayerInsulation)
        .def("get_end_insulation", &home_Winding::getEndInsulation)
        .def("get_build_factor_used", &home_Winding::getBuildFactorUsed)
        .def("get_hl_over_nose_reduction", &home_Winding::getHLOverNoseReduction)
        .def("get_kva", &home_Winding::getKVA)
        .def("get_line_k_voltage", &home_Winding::getLineKVoltage)
        .def("get_kv_phase_def", &home_Winding::getKVPhaseDef)
        .def("get_turns", &home_Winding::getTurns)
        .def("get_pd_phase_amp", &home_Winding::getPdPhaseAmp)
        .def("get_pd_phase_amp1", &home_Winding::getPdPhaseAmp1)
        .def("get_pd_line_amp", &home_Winding::getPdLineAmp)
        .def("get_pd_line_amp1", &home_Winding::getPdLineAmp1)
        .def("get_s_error_name", &home_Winding::getSErrorName)
        .def("get_t_error_enum", &home_Winding::getTErrorEnum)
        .def("get_s_error_messages", &home_Winding::getSErrorMessages)

// Setters

        .def("set_id", &home_Winding::setId)
        .def("set_name", &home_Winding::setName)
        .def("set_description", &home_Winding::setDescription)
        .def("set_voltage", &home_Winding::setVoltage)
        .def("set_type_connection", &home_Winding::setTypeConnection)
        .def("set_bil", &home_Winding::setBIL)
        .def("set_bil_under", &home_Winding::setBILUnder)
        .def("set_half_turns", &home_Winding::setHalfTurns)
        .def("set_coating", &home_Winding::setCoating)
        .def("set_nom_turns_ui", &home_Winding::setNomTurnsUI)
        .def("set_type_leads", &home_Winding::setTypeLeads)
        .def("set_quantity_leads", &home_Winding::setQuantityLeads)
        .def("set_thickness_leads", &home_Winding::setThicknessLeads)
        .def("set_width_leads", &home_Winding::setWidthLeads)
        .def("set_leads_density", &home_Winding::setLeadsDensity)
        .def("set_s_leads_density", &home_Winding::setSLeadsDensity)
        .def("set_no_duct_inner", &home_Winding::setNoDuctInner)
        .def("set_no_duct_outer", &home_Winding::setNoDuctOuter)
        .def("set_duct_size", &home_Winding::setDuctSize)
        .def("set_duct_placement", &home_Winding::setDuctPlacement)
        .def("set_opt_stayback", &home_Winding::setOptStayback)
        .def("set_layer_insulation", &home_Winding::setLayerInsulation)
        .def("set_end_insulation", &home_Winding::setEndInsulation)
        .def("set_build_factor_used", &home_Winding::setBuildFactorUsed)
        .def("set_hl_over_nose_reduction", &home_Winding::setHLOverNoseReduction)
        .def("set_kva", &home_Winding::setKVA)
        .def("set_line_k_voltage", &home_Winding::setLineKVoltage)
        .def("set_kv_phase_def", &home_Winding::setKVPhaseDef)
        .def("set_turns", &home_Winding::setTurns)
        .def("set_pd_phase_amp", &home_Winding::setPdPhaseAmp)
        .def("set_pd_phase_amp1", &home_Winding::setPdPhaseAmp1)
        .def("set_pd_line_amp", &home_Winding::setPdLineAmp)
        .def("set_pd_line_amp1", &home_Winding::setPdLineAmp1)
        .def("set_s_error_name", &home_Winding::setSErrorName)
        .def("set_t_error_enum", &home_Winding::setTErrorEnum)
        .def("set_s_error_messages", &home_Winding::setSErrorMessages);
}