//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      gradient_Bridge.cpp
//    @brief     MidConnector pybind11 wrapper for Transformer Thermal Gradients engine.


#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 


#include "alias.h"
using namespace Transformer::Types;

#include "math_Utilities.h"
#include "system_Config.h"


#include "cooling.h"
#include "gradient.h"
#include "device.h"



namespace py = pybind11;




void bridge_Gradient(py::module_& m) 
{
    py::class_<home_Gradients>(m, "Gradients")


//          B I N D   C O N S T R U C T O R


        .def(py::init([](home_Device* parentDevice) {
        return new home_Gradients(parentDevice);
            }))

//          B I N D    S T A N D A R D   M E T H O D S 


        .def_property_readonly("parent_device", &home_Gradients::getParentDevice)

        .def("reset_fields", &home_Gradients::reset_Fields)
        .def("eraseWarnings", &home_Gradients::erase_Warnings)
        .def("calculateWarnings", &home_Gradients::calc_Warnings)

        .def("calculate_Gradients", &home_Gradients::calc_Gradients,
            py::arg("cooling"))

        .def("calc_GradientsDef", &home_Gradients::calc_GradientsDef)
        .def("calc_ThermalRise", &home_Gradients::calc_ThermalRise,
            py::arg("cooling"))


//          B I N D   G E T T E R S   &   S E T T E R S 


        .def_property("id", &home_Gradients::getId, &home_Gradients::setId)
        .def_property("name", &home_Gradients::getName, &home_Gradients::setName)

        .def_property("dn", &home_Gradients::getDn, &home_Gradients::setDn)
        .def_property("d_w", &home_Gradients::getDW, &home_Gradients::setDW)
        .def_property("d_N", &home_Gradients::getDN, &home_Gradients::setDN)
        .def_property("d_l", &home_Gradients::getDL, &home_Gradients::setDL)
        .def_property("d_h", &home_Gradients::getDH, &home_Gradients::setDH)
        .def_property("d_b", &home_Gradients::getDb, &home_Gradients::setDb)

        .def_property("dq1", &home_Gradients::getDq1, &home_Gradients::setDq1)
        .def_property("dg1", &home_Gradients::getDG1, &home_Gradients::setDG1)
        .def_property("dq2", &home_Gradients::getDq2, &home_Gradients::setDq2)
        .def_property("dg2", &home_Gradients::getDG2, &home_Gradients::setDG2)
        .def_property("dg3", &home_Gradients::getDG3, &home_Gradients::setDG3)

        .def_property("dlv_gradient", &home_Gradients::getDLVgradient, &home_Gradients::setDLVgradient)

        .def_property("d_alt_corr_lv", &home_Gradients::getDAltCorrLV, &home_Gradients::setDAltCorrLV)
        .def_property("d_alt_corr_hv", &home_Gradients::getDAltCorrHV, &home_Gradients::setDAltCorrHV)

        .def_property("d_gradient_f", &home_Gradients::getDGradientF, &home_Gradients::setDGradientF)
        .def_property("d_gradient_final", &home_Gradients::getDGradientFinal, &home_Gradients::setDGradientFinal)

        .def_property("d_fcbn_lv_gradient", &home_Gradients::getDFCBNLVgradient, &home_Gradients::setDFCBNLVgradient)
        .def_property("d_tor", &home_Gradients::getDTOR, &home_Gradients::setDTOR)

        .def_property("d_tor_final", &home_Gradients::getDTORFinal, &home_Gradients::setDTORFinal)

        .def_property("d_max_grad", &home_Gradients::getDMaxGrad, &home_Gradients::setDMaxGrad)
        .def_property("d_max_grad_f", &home_Gradients::getDMaxGradF, &home_Gradients::setDMaxGradF)
        .def_property("d_fcbn_hv_grad", &home_Gradients::getDFCBNHVGrad, &home_Gradients::setDFCBNHVGrad)

        .def_property("error_name", &home_Gradients::getSErrorName, &home_Gradients::setSErrorName)
        .def_property("error_enum", &home_Gradients::getTErrorEnum, &home_Gradients::setTErrorEnum)
        .def_property("error_messages", &home_Gradients::getSErrorMessages, &home_Gradients::setSErrorMessages);
}