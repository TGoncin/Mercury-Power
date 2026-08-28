//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      losses_Bridge.cpp
//    @brief     CORE main calling file


#include <pybind11/pybind11.h>

namespace py = pybind11;


// Forward declare your helper functions so this file knows they exist


void bridge_Designer(py::module_& m);
void bridge_Device(py::module_& m);
void bridge_Field_Value(py::module_& m);
void bridge_Serialize(py::module_& m);




//              The ONE and ONLY entry point for the physics folder


PYBIND11_MODULE(bridge_CORE, m) {
    m.doc() = "Mercury 3-Phase Optimization - Physics Module";


//          C A L L   H E L P E R   F U N C T I O N S 
//                                                      Registering all your classes to this one module


    bridge_Designer(m);
    bridge_Device(m);
    bridge_Field_Value(m);
    bridge_Serialize(m);
}