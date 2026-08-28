//    @date      August 24, 2026
//    @author    Tom Goncin


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "bushing.h"

#include "alias.h"
using namespace Transformer::Types;

namespace py = pybind11;



void bridge_Bushing(py::module_& m)
{
    py::class_<home_Bushing>(m, "Bushing")


// Constructors

        .def(py::init<>(), "Initialize Bushing with auto-incremented ID and name")

// Fields & State Methods

        .def_readwrite("bushing_fields", &home_Bushing::bushingFields, "Map of Bushing FieldValues")
        .def("reset_bushing_fields", &home_Bushing::reset_BushingFields, "Reset all internal field values")

// Getters

        .def("get_id", &home_Bushing::getId, "Get bushing integer ID")
        .def("get_name", &home_Bushing::getName, "Get bushing string name")

// Setters

        .def("set_id", &home_Bushing::setId, py::arg("value"), "Set bushing integer ID")
        .def("set_name", &home_Bushing::setName, py::arg("value"), "Set bushing string name");
}
