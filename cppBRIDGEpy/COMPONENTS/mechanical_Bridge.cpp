//    @date      August 24, 2026
//    @author    Tom Goncin


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "mechanical.h"
#include "device.h"
#include "cooling.h"
#include "gradient.h"

namespace py = pybind11;



void bridge_Mechanical(py::module_& m)
{
    py::class_<home_Mechanical>(m, "Mechanical")



// Constructors

        .def(py::init<home_Device*>(),
            py::arg("parent_device") = nullptr,
            "Initialize Mechanical framework with optional parent Device pointer")

// Fields Map

        .def_readwrite("mechanical_fields", &home_Mechanical::mechanicalFields, "Map of Mechanical FieldValues")

// Navigation Methods

        .def("get_parent_device", &home_Mechanical::getParentDevice, py::return_value_policy::reference, "Get parent device pointer")

// Action & Calculation Pipeline Methods

        .def("reset_fields", &home_Mechanical::reset_Fields, "Reset all internal mechanical fields")
        .def("erase_warnings", &home_Mechanical::erase_Warnings, "Clear warning states")
        .def("calc_warnings", &home_Mechanical::calc_Warnings, "Calculate tank boundary warnings")
        .def("calc_mechanical", &home_Mechanical::calc_Mechanical, py::arg("cooling"), py::arg("gradient"), "Execute full mechanical evaluation pipeline")
        .def("calc_mechanical_default", &home_Mechanical::calc_MechanicalDefault, py::arg("cooling"), "Calculate thermal dissipation limits across cooling steps")
        .def("calc_awr_tor_hsr", &home_Mechanical::calc_AwrTorHsr, py::arg("cooling"), py::arg("gradient"), "Calculate AWR, TOR, and HSR rise components")
        .def("calc_max_awr_tor_hsr", &home_Mechanical::calc_MaxAwrTorHsr, py::arg("cooling"), "Calculate maximum limit values for AWR, TOR, and HSR")

// Getters

        .def("get_calculo_with_rad", &home_Mechanical::getCalculoWithRad)
        .def("get_h", &home_Mechanical::getH)
        .def("get_l", &home_Mechanical::getL)
        .def("get_d", &home_Mechanical::getD)
        .def("get_a", &home_Mechanical::getA)
        .def("get_hin", &home_Mechanical::getHin)
        .def("get_lin", &home_Mechanical::getLin)
        .def("get_din", &home_Mechanical::getDin)
        .def("get_ain", &home_Mechanical::getAin)
        .def("get_losses", &home_Mechanical::getLosses)
        .def("get_hot_spot_factor", &home_Mechanical::getHotSpotFactor)
        .def("get_nameplate_rise", &home_Mechanical::getNameplateRise)
        .def("get_lv_gradient", &home_Mechanical::getLVGradient)
        .def("get_hv_gradient", &home_Mechanical::getHVGradient)
        .def("get_max_awr", &home_Mechanical::getMaxAWR)
        .def("get_max_tor", &home_Mechanical::getMaxTOR)
        .def("get_max_hsr", &home_Mechanical::getMaxHSR)
        .def("get_cor_grad_lv0", &home_Mechanical::getCorGradLV0)
        .def("get_cor_grad_hv0", &home_Mechanical::getCorGradHV0)
        .def("get_tortot0", &home_Mechanical::getTORTOT0)
        .def("get_awrlv0", &home_Mechanical::getAWRLV0)
        .def("get_awrhv0", &home_Mechanical::getAWRHV0)
        .def("get_hsrlv0", &home_Mechanical::getHSRLV0)
        .def("get_hsrhv0", &home_Mechanical::getHSRHV0)
        .def("get_kw1", &home_Mechanical::getKW1)

// Setters

        .def("set_calculo_with_rad", &home_Mechanical::setCalculoWithRad)
        .def("set_h", &home_Mechanical::setH)
        .def("set_l", &home_Mechanical::setL)
        .def("set_d", &home_Mechanical::setD)
        .def("set_a", &home_Mechanical::setA)
        .def("set_hin", &home_Mechanical::setHin)
        .def("set_lin", &home_Mechanical::setLin)
        .def("set_din", &home_Mechanical::setDin)
        .def("set_ain", &home_Mechanical::setAin)
        .def("set_losses", &home_Mechanical::setLosses)
        .def("set_hot_spot_factor", &home_Mechanical::setHotSpotFactor)
        .def("set_nameplate_rise", &home_Mechanical::setNameplateRise)
        .def("set_lv_gradient", &home_Mechanical::setLVGradient)
        .def("set_hv_gradient", &home_Mechanical::setHVGradient)
        .def("set_max_awr", &home_Mechanical::setMaxAWR)
        .def("set_max_tor", &home_Mechanical::setMaxTOR)
        .def("set_max_hsr", &home_Mechanical::setMaxHSR)
        .def("set_cor_grad_lv0", &home_Mechanical::setCorGradLV0)
        .def("set_cor_grad_hv0", &home_Mechanical::setCorGradHV0)
        .def("set_tortot0", &home_Mechanical::setTORTOT0)
        .def("set_awrlv0", &home_Mechanical::setAWRLV0)
        .def("set_awrhv0", &home_Mechanical::setAWRHV0)
        .def("set_hsrlv0", &home_Mechanical::setHSRLV0)
        .def("set_hsrhv0", &home_Mechanical::setHSRHV0)
        .def("set_kw1", &home_Mechanical::setKW1);
}