//   @date      August 21, 2026
//   @author    Tom Goncin
//   @file      designer_Bridge.cpp
//   @brief     Pybind11 bridge module exposing the Transformer::Core::Designer engine to Python.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "designer.h"
#include "device.h"

namespace py = pybind11;



void bridge_Designer(py::module_& m)
{
    py::class_<Designer>(m, "Designer")


//      C O N S T R U C T O R S

        .def(py::init<home_Device*>(),
            py::arg("deviceE") = nullptr,
            "Initialize Designer with an optional shared pointer to a Device object")

//      C O R E   E X E C U T I O N   M E T H O D S 

        .def("designSinglePhase", &Transformer::Core::Designer::designSinglePhase,
            "Execute single-phase structural mechanical allocations and electrical optimization sweep")
        .def("calculatePenalty", &Transformer::Core::Designer::calculatePenalty,
            "Evaluate absolute compliance bounds and compute total design penalty")
        .def("calculateWarning", &Transformer::Core::Designer::calculateWarning,
            "Calculate warning metrics and penalty adjustments")


//      P R O P E R T I E S    &    G E T T E R S   &  S E T T E R S    M A P P I N G


        .def_property("cost",
            &Transformer::Core::Designer::getCost,
            &Transformer::Core::Designer::setCost,
            "Design evaluation cost")

        .def_property("penalty",
            &Transformer::Core::Designer::getPenalty,
            &Transformer::Core::Designer::setPenalty,
            "Accumulated design penalty score")

        .def_property("warning",
            &Transformer::Core::Designer::getWarning,
            &Transformer::Core::Designer::setWarning,
            "Calculated design warning value")

        .def_property("validDesign",
            &Transformer::Core::Designer::getValidDesign,
            &Transformer::Core::Designer::setValidDesign,
            "Boolean flag indicating design validity")

        .def_property("penaltyLevel",
            &Transformer::Core::Designer::getPenaltyLevel,
            &Transformer::Core::Designer::setPenaltyLevel,
            "Current penalty tracking tier level")

        .def_property("listSolutions_LV",
            &Transformer::Core::Designer::getListSolutions_LV,
            &Transformer::Core::Designer::setListSolutions_LV,
            "Low-voltage solution vector options")

        .def_property("listSolutions_HV",
            &Transformer::Core::Designer::getListSolutions_HV,
            &Transformer::Core::Designer::setListSolutions_HV,
            "High-voltage solution vector options")

        .def_property("device",
            &Transformer::Core::Designer::getDevice,
            &Transformer::Core::Designer::setDevice,
            "Associated transformer Device shared pointer object");
}