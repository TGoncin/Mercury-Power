//    @date      August 31, 2026
//    @author    Tom Goncin
//    @file      enums_Bridge.cpp
//    @brief     Pybind11 bridge module exposing shared Transformer enums to Python.

#include <pybind11/pybind11.h>

#include "alias.h"
#include "enums.h"

using namespace Transformer::Types;

namespace py = pybind11;



void bridge_Enums(py::module_& m)
{
    py::enum_<home_WarningSeverity>(m, "WarningSeverity")
        .value("None_", home_WarningSeverity::None)
        .value("Warning", home_WarningSeverity::Warning)
        .value("Error", home_WarningSeverity::Error)
        .export_values();

    py::enum_<home_Segment>(m, "Segment")
        .value("Free", home_Segment::Free)
        .value("Block", home_Segment::Block)
        .export_values();
}