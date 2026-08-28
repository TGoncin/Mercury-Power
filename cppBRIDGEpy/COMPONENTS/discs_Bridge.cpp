//    @date      August 24, 2026
//    @author    Tom Goncin


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "discs.h"

namespace py = pybind11;



void bridge_Discs(py::module_& m)
{

// Expose DiscsResult structure

    py::class_<home_DiscsResult>(m, "DiscsResult")
        .def(py::init<>())
        .def_readwrite("success", &home_DiscsResult::success)
        .def_readwrite("turns", &home_DiscsResult::turns)
        .def_readwrite("sections_2", &home_DiscsResult::sections_2)
        .def_readwrite("turns_per_section", &home_DiscsResult::turnsPerSection)
        .def_readwrite("fractional_turn", &home_DiscsResult::fractionalTurn);

// Expose Discs class

    py::class_<home_Discs>(m, "Discs")

// Constructors

        .def(py::init<>(), "Initialize Discs container framework")

// Fields Map

        .def_readwrite("disc_fields", &home_Discs::discFields, "Map of Discs FieldValues")

// Core Static Calculation Method

        .def_static("arrange_disc_turns", &home_Discs::arrangeDiscTurns,
            py::arg("target_turns"), py::arg("wedge_count"), py::arg("sections_1"), py::arg("turns_per_section_init"),
            "Execute asymmetrical disc turn distribution strategy")

// Getters

        .def("get_t_success", &home_Discs::getTSuccess)
        .def("get_id", &home_Discs::getId)
        .def("get_i_wedge_count", &home_Discs::getIWedgeCount)
        .def("get_i_sections1", &home_Discs::getISections1)
        .def("get_i_turns_per_sec_init", &home_Discs::getITurnsPerSecInit)
        .def("get_d_target_turns", &home_Discs::getDTargetTurns)
        .def("get_name", &home_Discs::getName)

// Setters

        .def("set_id", &home_Discs::setId)
        .def("set_i_wedge_count", &home_Discs::setIWedgeCount)
        .def("set_i_sections1", &home_Discs::setISections1)
        .def("set_i_turns_per_sec_init", &home_Discs::setITurnsPerSecInit)
        .def("set_d_target_turns", &home_Discs::setDTargetTurns)
        .def("set_t_success", &home_Discs::setTSuccess)
        .def("set_name", &home_Discs::setName);
}