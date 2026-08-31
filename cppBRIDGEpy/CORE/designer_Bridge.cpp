//   @date      August 21, 2026
//   @author    Tom Goncin
//   @file      designer_Bridge.cpp
//   @brief     Pybind11 bridge module exposing the Transformer::Core::Designer engine to Python.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"

#include "designer.h"
#include "device.h"

namespace py = pybind11;
using namespace Transformer::Types;


void bridge_Designer(py::module_& m)
{
    py::class_<home_Designer>(m, "Designer")


//      C O N S T R U C T O R S

        .def(py::init<home_Device*>(),
            py::arg("deviceE") = nullptr,
            "Initialize Designer with an optional shared pointer to a Device object")

//      C O R E   E X E C U T I O N   M E T H O D S 

        .def("designSinglePhase", &home_Designer::designSinglePhase,
            "Execute single-phase structural mechanical allocations and electrical optimization sweep")
        .def("calculatePenalty", &home_Designer::calculatePenalty,
            "Evaluate absolute compliance bounds and compute total design penalty")
        .def("calculateWarning", &home_Designer::calculateWarning,
            "Calculate warning metrics and penalty adjustments")


//      P R O P E R T I E S    &    G E T T E R S   &  S E T T E R S    M A P P I N G


        .def_property("cost",
            &home_Designer::getCost,
            &home_Designer::setCost,
            "Design evaluation cost")

        .def_property("penalty",
            &home_Designer::getPenalty,
            &home_Designer::setPenalty,
            "Accumulated design penalty score")

        .def_property("warning",
            &home_Designer::getWarning,
            &home_Designer::setWarning,
            "Calculated design warning value")

        .def_property("validDesign",
            &home_Designer::getValidDesign,
            &home_Designer::setValidDesign,
            "Boolean flag indicating design validity")

        .def_property("penaltyLevel",
            &home_Designer::getPenaltyLevel,
            &home_Designer::setPenaltyLevel,
            "Current penalty tracking tier level")

        .def_property("listSolutions_LV",
            &home_Designer::getListSolutions_LV,
            &home_Designer::setListSolutions_LV,
            "Low-voltage solution vector options")

        .def_property("listSolutions_HV",
            &home_Designer::getListSolutions_HV,
            &home_Designer::setListSolutions_HV,
            "High-voltage solution vector options")

        .def_property("device",
            &home_Designer::getDevice,
            &home_Designer::setDevice,
            "Associated transformer Device shared pointer object");
}