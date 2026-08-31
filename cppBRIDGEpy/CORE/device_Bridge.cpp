//  @date      August 21, 2026
//  @author    Tom Goncin
//  @file      device_Bridge.cpp
//  @brief     Pybind11 bridge module exposing the home_Device engine to Python.


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"

#include "device.h"
#include "winding.h"
#include "coil.h"
#include "cooling.h"
#include "guarantee.h"
#include "core.h"
#include "mechanical.h"
#include "costs.h"

#include "tests.h"

namespace py = pybind11;
using namespace Transformer::Types;




void bridge_Device(py::module_& m)
{
    py::class_<home_Device>(m, "Device")



//      C O N S T R U C T O R S


        .def(py::init<>(), "Initialize Device with default configuration maps and field values")


//      Expose the raw configuration map directly to Python


        .def_readwrite("deviceFields", &home_Device::deviceFields)


//      Expose unique_ptr submodules safely via reference getters


        .def("get_core", [](Device& d) -> home_Core* { return d.core.get(); },
            py::return_value_policy::reference_internal, "Get core sub-module")
        .def("get_cooling", [](Device& d) -> home_Cooling* { return d.cooling.get(); },
            py::return_value_policy::reference_internal, "Get cooling sub-module")
        .def("get_guarantee", [](Device& d) -> home_Guarantee* { return d.guarantee.get(); },
            py::return_value_policy::reference_internal, "Get guarantee sub-module")
        .def("get_costs", [](Device& d) -> home_Costs* { return d.costs.get(); },
            py::return_value_policy::reference_internal, "Get costs sub-module")
        .def("get_tests", [](Device& d) -> home_Tests* { return d.tests.get(); },
            py::return_value_policy::reference_internal, "Get tests sub-module")
        .def("get_mechanical", [](Device& d) -> home_Mechanical* { return d.mechanical.get(); },
            py::return_value_policy::reference_internal, "Get mechanical sub-module")

        // Sub-modules exposed as read-only properties so Python can call device.cooling, device.core, etc.
        .def_property_readonly("core", [](home_Device& d) { return d.core.get(); }, py::return_value_policy::reference_internal)
        .def_property_readonly("cooling", [](home_Device& d) { return d.cooling.get(); }, py::return_value_policy::reference_internal)
        .def_property_readonly("guarantee", [](home_Device& d) { return d.guarantee.get(); }, py::return_value_policy::reference_internal)
        .def_property_readonly("costs", [](home_Device& d) { return d.costs.get(); }, py::return_value_policy::reference_internal)
        .def_property_readonly("tests", [](home_Device& d) { return d.tests.get(); }, py::return_value_policy::reference_internal)
        .def_property_readonly("mechanical", [](home_Device& d) { return d.mechanical.get(); }, py::return_value_policy::reference_internal)

// Pipeline and reset methods


        .def("reset_Fields", &home_Device::reset_Fields)
        .def("calculateDevice", &home_Device::calc_Device, py::arg("warnings") = true)
        .def("reassignDefaults", &home_Device::reset_Fields)

// Windings


        .def("get_WindingHV", &home_Device::get_WindingHV, py::return_value_policy::reference_internal)
        .def("get_WindingLV", &home_Device::get_WindingLV, py::return_value_policy::reference_internal)
        .def("get_WindingTV", &home_Device::get_WindingTV, py::return_value_policy::reference_internal)


        //      S I M U L A T I O N    &    C A L C    P I P E L I N E S


        .def("calc_Device", &home_Device::calc_Device,
            py::arg("warnings") = true,
            "Execute full device simulation calculation pipeline")
        .def("reset_Fields", &home_Device::reset_Fields,
            "Reset all underlying sub-modules and field defaults")
        .def("reset_Device", &home_Device::reset_Device,
            "Clear device fields, windings, and coils")
        .def("erase_Warnings", &home_Device::erase_Warnings,
            "Clear all active engine error and warning messages")


        //      W I N D I N G   &    C O I L    A C C E S S O R S


        .def("get_WindingHV", &home_Device::get_WindingHV,
            py::return_value_policy::reference_internal,
            "Retrieve High Voltage winding pointer")
        .def("get_WindingLV", &home_Device::get_WindingLV,
            py::return_value_policy::reference_internal,
            "Retrieve Low Voltage winding pointer")
        .def("get_WindingTV", &home_Device::get_WindingTV,
            py::return_value_policy::reference_internal,
            "Retrieve Tertiary winding pointer if present")
        .def("get_LastCoil", &home_Device::get_LastCoil,
            py::return_value_policy::reference_internal,
            "Retrieve the last coil in the assembly")
        .def("get_Coil", &home_Device::get_Coil,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Retrieve coil by index")
        .def("get_CoilByPosition", &home_Device::get_CoilByPosition,
            py::arg("position"),
            py::return_value_policy::reference_internal,
            "Retrieve coil by position")
        .def("get_WindingLastCoil", &home_Device::get_WindingLastCoil,
            "Get count/index of last coil for iteration bounds")


        //      C O R E   P A R A M E T E R S   G E T T E R S   &  S E T T E R S 


        .def("getNominalKva", &home_Device::getNominalKva, "Get nominal kVA rating")
        .def("setNominalKva", &home_Device::setNominalKva, py::arg("v"), "Set nominal kVA rating")
        .def("getFrequency", &home_Device::get_Frequency, "Get operating frequency")
        .def("setFrequency", &home_Device::setFrequency, py::arg("v"), "Set operating frequency")
        .def("getINoPhases", &home_Device::getINoPhases, "Get number of operating phases")
        .def("setINoPhases", &home_Device::setINoPhases, py::arg("v"), "Set number of operating phases")


        //      S T R I N G    G E T T E R S   &   S E T T E R S


        .def("getSoNumber", &home_Device::getSoNumber)
        .def("setSoNumber", &home_Device::setSoNumber, py::arg("v"))
        .def("getCustomerName", &home_Device::getCustomerName)
        .def("setCustomerName", &home_Device::setCustomerName, py::arg("v"))
        .def("getCustomerCode", &home_Device::getCustomerCode)
        .def("setCustomerCode", &home_Device::setCustomerCode, py::arg("v"))


        //      N U M E R I C A L   G E T T E R S    &   S E T T E R S 


        .def("getQuantity", &home_Device::getQuantity)
        .def("setQuantity", &home_Device::setQuantity, py::arg("v"))


        //      P Y - F R I E N D L Y   S N A K E _ C A S E    A L I A S E S


        .def("get_so_number", &home_Device::getSoNumber)
        .def("get_customer_name", &home_Device::getCustomerName)
        .def("get_customer_code", &home_Device::getCustomerCode)
        .def("get_quantity", &home_Device::getQuantity);

}