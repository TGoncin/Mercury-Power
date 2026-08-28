//    @date      August 24, 2026
//    @author    Tom Goncin

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "changer.h"
#include "winding.h"

namespace py = pybind11;



void bridge_Changer(py::module_& m)
{
    py::class_<home_Changer>(m, "Changer")



// Constructors

        .def(py::init<home_Winding*>(), py::arg("parent_winding") = nullptr,
            "Initialize Changer with optional parent Winding pointer")

// Fields & Structural Map

        .def_readwrite("changer_fields", &home_Changer::changerFields, "Map of Changer FieldValues")

// Core Operations & Calculations

        .def("get_parent_winding", &home_Changer::getParentWinding, py::return_value_policy::reference, "Get parent winding pointer")
        .def("get_number_steps", &home_Changer::get_NumberSteps, "Get number of tap steps")
        .def("get_per_step_up", &home_Changer::get_PerStepUp, "Get percentage step up")
        .def("get_per_step_down", &home_Changer::get_PerStepDown, "Get percentage step down")
        .def("taps_sup", &home_Changer::taps_Sup, "Calculate high side tap steps")
        .def("taps_inf", &home_Changer::taps_Inf, "Calculate low side tap steps")
        .def("get_type_string", &home_Changer::get_Type, "Get formatted tap type string")

// Action & Execution Methods

        .def("reset_changer_fields", &home_Changer::reset_ChangerFields, "Reset all internal changer fields")
        .def("erase_warnings", &home_Changer::erase_Warnings, "Clear warning states")
        .def("calc_warnings", &home_Changer::calc_Warnings, "Calculate changer warnings")
        .def("calc_changer", &home_Changer::calc_Changer, "Execute full changer calculation pipeline")
        .def("calc_default_capacity", &home_Changer::calc_DefaultCapacity, "Calculate default capacity")
        .def("calc_leads_out_top_coil", &home_Changer::calc_LeadsOutTopCoil, "Calculate lead positions out of top of coil")
        .def("calc_instructions", &home_Changer::calc_Instructions, "Calculate step instructions")
        .def("calc_per_taps", &home_Changer::calc_PerTaps, "Calculate percentage taps sequence")

// Getters

        .def("get_id", &home_Changer::getId)
        .def("get_i_num_steps", &home_Changer::getINumSteps)
        .def("get_t_capacity", &home_Changer::getTCapacity)
        .def("get_t_model_changer", &home_Changer::getTModelChanger)
        .def("get_t_type", &home_Changer::getTType)
        .def("get_t_type_lr", &home_Changer::getTTypeLR)
        .def("get_t_type_cbc", &home_Changer::getTTypeCBC)
        .def("get_b_extra_circuit", &home_Changer::getBExtraCircuit)
        .def("get_i_red_cap", &home_Changer::getIRedCap)
        .def("get_d_per_up", &home_Changer::getDPerUp)
        .def("get_d_per_down", &home_Changer::getDPerDown)
        .def("get_d_per_taps", &home_Changer::getDPerTaps)
        .def("get_d_series_ratio", &home_Changer::getDSeriesRatio)
        .def("get_name", &home_Changer::getName)
        .def("get_s_capacity", &home_Changer::getSCapactity)
        .def("get_s_per_taps", &home_Changer::getSPerTaps)
        .def("get_s_instructions", &home_Changer::getSInstructions)
        .def("get_s_type", &home_Changer::getSType)
        .def("get_t_error_enum", &home_Changer::getTErrorEnum)
        .def("get_s_leads_out_top_coil_heater", &home_Changer::getSLeadsOutTOPCoilHeater)
        .def("get_s_leads_out_top_coil", &home_Changer::getSLeadsOutTOPCoil)
        .def("get_s_error_name", &home_Changer::getSErrorName)
        .def("get_s_error_messages", &home_Changer::getSErrorMessages)

// Setters

        .def("set_id", &home_Changer::setId)
        .def("set_i_num_steps", &home_Changer::setINumSteps)
        .def("set_t_capacity", &home_Changer::setTCapacity)
        .def("set_t_model_changer", &home_Changer::setTModelChanger)
        .def("set_t_type", &home_Changer::setTType)
        .def("set_t_type_lr", &home_Changer::setTTypeLR)
        .def("set_t_type_cbc", &home_Changer::setTTypeCBC)
        .def("set_b_extra_circuit", &home_Changer::setBExtraCircuit)
        .def("set_i_red_cap", &home_Changer::setIRedCap)
        .def("set_d_per_up", &home_Changer::setDPerUp)
        .def("set_d_per_down", &home_Changer::setDPerDown)
        .def("set_d_per_taps", &home_Changer::setDPerTaps)
        .def("set_d_series_ratio", &home_Changer::setDSeriesRatio)
        .def("set_name", &home_Changer::setName)
        .def("set_s_capacity", &home_Changer::setSCapactity)
        .def("set_s_per_taps", &home_Changer::setSPerTaps)
        .def("set_s_instructions", &home_Changer::setSInstructions)
        .def("set_s_type", &home_Changer::setSType)
        .def("set_s_leads_out_top_coil_heater", &home_Changer::setSLeadsOutTOPCoilHeater)
        .def("set_s_leads_out_top_coil", &home_Changer::setSLeadsOutTOPCoil)
        .def("set_s_error_name", &home_Changer::setSErrorName)
        .def("set_t_error_enum", &home_Changer::setTErrorEnum)
        .def("set_s_error_messages", &home_Changer::setSErrorMessages);
}