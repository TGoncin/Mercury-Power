//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      midConnector_COMPONENTS
//    @brief     COMPONENTS main calling file


#include <pybind11/pybind11.h>

namespace py = pybind11;


// Forward declare your helper functions so this file knows they exist

void bridge_Bushing(py::module_& m);
void bridge_Changer(py::module_& m);
void bridge_Coil(py::module_& m);
void bridge_Conductor(py::module_& m);
void bridge_Core(py::module_& m);
void bridge_Discs(py::module_& m);
void bridge_Mechanical(py::module_& m);
void bridge_Winding(py::module_& m);


//              The ONE and ONLY entry point for the PHYSICS directory

PYBIND11_MODULE(bridge_COMPONENTS, m) 
{
    m.doc() = "Mercury 3-Phase Optimization - Physics Module";


//          C A L L   H E L P E R   F U N C T I O N S 
//                                                      Registering all your classes to this one module

    bridge_Bushing(m);
    bridge_Changer(m);
    bridge_Coil(m);
    bridge_Conductor(m);
    bridge_Core(m);
    bridge_Discs(m);
    bridge_Mechanical(m);
    bridge_Winding(m);

}