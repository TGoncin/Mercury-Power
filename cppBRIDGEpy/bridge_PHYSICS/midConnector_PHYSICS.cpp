
//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      losses_Bridge.cpp
//    @brief     Main .pyd file for calling PHYSICS bridge/binding/MidConnector files 


#include <pybind11/pybind11.h>

namespace py = pybind11;


// Forward declare your helper functions so this file knows they exist


void bridge_Cooling(py::module_& m);
void bridge_Gradient(py::module_& m);
void bridge_Losses(py::module_& m);
void bridge_Phase_Amperage(py::module_& m);


//              The ONE and ONLY entry point for the physics folder


PYBIND11_MODULE(bridge_PHYSICS, m) {
    m.doc() = "Mercury 3-Phase Optimization - Physics Module";


//          C A L L   H E L P E R   F U N C T I O N S 
//                                                      Registering all your classes to this one module


    bridge_Cooling(m);
    bridge_Gradient(m);
    bridge_Losses(m);
    bridge_Phase_Amperage(m);
}