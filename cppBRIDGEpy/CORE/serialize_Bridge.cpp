
//  @date      August 24, 2026
//  @author    Tom Goncin
//  @file      serialize_Bridge.cpp
//  @brief     Pybind11 bridge module exposing the home_Serialize XML engine.


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "serialize.h"
#include "device.h"

namespace py = pybind11;



void bridge_Serialize(py::module_& m)
{
    py::class_<Serialize>(m, "Serialize")
        .def_static("deserialize_device_from_xml",
            &Serialize::deserialize_device_from_xml,
            py::arg("device"), py::arg("file_path"),
            "Deserialize transformer device parameters from a designated XML file path");
}