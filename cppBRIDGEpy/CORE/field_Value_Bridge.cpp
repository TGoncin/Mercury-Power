//  @date      August 24, 2026
//  @author    Tom Goncin
//  @file      field_Value_Bridge.cpp
//  @brief     Comprehensive Pybind11 bridge module exposing the home_FieldValue class.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "alias.h"

#include "field_Value.h"

namespace py = pybind11;
using namespace Transformer::Types;




void bridge_Field_Value(py::module_& m)
{
    py::class_<home_FieldValue>(m, "FieldValue")



        //      C O N S T R U C T O R S


        .def(py::init<>(), "Initialize FieldValue with default empty state")
        .def(py::init<home_FieldValue::Data>(), py::arg("defaultValue"), "Initialize with default variant Data")


        //      S T A T E    &    V A L I D A T I O N    M E T H O D S


        .def("reset", &home_FieldValue::reset)
        .def("has_changed", &home_FieldValue::has_Changed)
        .def("is_default", &home_FieldValue::is_Default)
        .def("is_array", &home_FieldValue::is_Array)
        .def("size", &home_FieldValue::size)


        //      V A L U E   S E T T E R S   &   M U T A T O R S 
        //                                                      using overload_cast for safety


        .def("set", py::overload_cast<home_FieldValue::Data, int, int>(&home_FieldValue::set),
            py::arg("value"), py::arg("index") = -1, py::arg("decimals") = 6)

        .def("set_field", py::overload_cast<home_FieldValue::Data, int, int>(&home_FieldValue::set_Field),
            py::arg("value"), py::arg("index") = -1, py::arg("decimals") = 6)

        .def("assign_default", py::overload_cast<home_FieldValue::Data, int, int>(&home_FieldValue::assign_Default),
            py::arg("value"), py::arg("index") = -1, py::arg("decimals") = 6)

        .def("set_from_string", &home_FieldValue::set_FromString,
            py::arg("str"), py::arg("index") = -1)


        //      V A L U E    G E T T E R S


        .def("get_current", &home_FieldValue::get_Current, py::arg("index") = -1)
        .def("get_default", &home_FieldValue::get_Default, py::arg("index") = -1)


        //      F O R M A T T I N G   &   S T R I N G    C O N V E R T E R S


        .def("get_as_string", &home_FieldValue::get_AsString)
        .def("get_as_string_vector", &home_FieldValue::get_AsStringVector)


        //      E X P L I C I T   T Y P E    E X T R A C T O R S 
        //                                                      FieldValue::get<T>()

        .def("get_int", &home_FieldValue::get<int>)
        .def("get_double", &home_FieldValue::get<double>)
        .def("get_string", &home_FieldValue::get<std::string>)
        .def("get_bool", &home_FieldValue::get<bool>)

        //      G E N E R I C   G E T   ( P y t h o N   A l i a s )
        //                                                      Aliases Python's generic .get() calls directly to your C++ variant getter

        .def("get", &home_FieldValue::get_Current, py::arg("index") = -1, "Alias for get_current() to satisfy Python .get() calls");
}

