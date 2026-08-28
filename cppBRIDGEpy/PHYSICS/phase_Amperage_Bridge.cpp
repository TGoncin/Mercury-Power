//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      phase_Amperage_Bridge.cpp
//    @brief     MidConnector pybind11 wrapper for Transformer Phase Amperage calculations.


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
#include "enums.h"
#include "transformer_Config.h"

#include "phase_Amperage.h"
#include "cooling.h"
#include "winding.h"
#include "device.h"
#include "gradient.h"
#include "mechanical.h"

using namespace Transformer::Types;
namespace py = pybind11;
using namespace Transformer::Physics;




//                  M A S T E R   M O D U L E   P A T T E R M
//                                                              NOTICE:     void bind_... instead of PYBIND11_MODULE


void bridge_Phase_Amperage(py::module_& m) 
{
    py::class_<PhaseAmperage>(m, "PhaseAmperage")


//                  B I N D   C O N S T R U C T O R  
//                                                  Default constructor (since no parameters are required]

    .def(py::init<>())


//          B I N D    S T A N D A R D   M E T H O D S 
//                                                     Standard mappings MATCHING legacy Py names --> new C++ names


    .def ("calculate_phase_amperageX",      &PhaseAmperage::calculate_phase_amperageX)
    .def ("calculate_amp_fase_h",           &PhaseAmperage::calculate_phase_amperage_h)
    .def ("calculate_phase_amp_sin_rel_h",  &PhaseAmperage::calculate_phase_amp_without_ratio_h)
    .def ("calculate_phase_amperage",       &PhaseAmperage::calculate_phase_amperage)


//          B I N D   G E T T E R S   &   S E T T E R S 


//                           integer values

        .def_property ("reduced_capacity",   &PhaseAmperage::get_ReducedCapacity, &PhaseAmperage::set_ReducedCapacity)
        .def_property ("pos_nom",            &PhaseAmperage::get_PosNom, &PhaseAmperage::set_PosNom)
        .def_property ("connection",         &PhaseAmperage::get_Connection, &PhaseAmperage::set_Connection)
        .def_property ("phases",             &PhaseAmperage::get_Phases, &PhaseAmperage::set_Phases)
        .def_property ("len_kv",             &PhaseAmperage::get_LenKv, &PhaseAmperage::set_LenKv)
        .def_property ("len_kvX",            &PhaseAmperage::get_LenKvX, &PhaseAmperage::set_LenKvX)
        .def_property ("len_kvH",            &PhaseAmperage::get_LenKvH, &PhaseAmperage::set_LenKvH)
        .def_property ("len_amp_phaseX",     &PhaseAmperage::get_LenAmpPhaseX, &PhaseAmperage::set_LenAmpPhaseX)
        .def_property ("len_amp_phaseH",     &PhaseAmperage::get_LenAmpPhaseH, &PhaseAmperage::set_LenAmpPhaseH)
        .def_property ("auto_connection",    &PhaseAmperage::get_AutoConnection, &PhaseAmperage::set_AutoConnection)
        .def_property ("lr_type",            &PhaseAmperage::get_LrType, &PhaseAmperage::set_LrType)
        .def_property ("winding_dev",        &PhaseAmperage::get_WindingDev, &PhaseAmperage::set_WindingDev)
        .def_property ("regulation",         &PhaseAmperage::get_Regulation, &PhaseAmperage::set_Regulation)
        .def_property ("reg_connection_x",   &PhaseAmperage::get_RegConnectionX, &PhaseAmperage::set_RegConnectionX)

//                           boolean values

        .def_property ("meets_vfvv_range",  &PhaseAmperage::get_MeetsVfvvRange, &PhaseAmperage::set_MeetsVfvvRange)

//                           double values

        .def_property ("mva",           &PhaseAmperage::get_Mva, &PhaseAmperage::set_Mva)
        .def_property ("series_ratio",  &PhaseAmperage::get_SeriesRatio, &PhaseAmperage::set_SeriesRatio)

//                           vectors --> arrays

        .def_property ("kv_values",                 &PhaseAmperage::get_KvValues, &PhaseAmperage::set_KvValues)
        .def_property ("kv_x_values",               &PhaseAmperage::get_KvXValues, &PhaseAmperage::set_KvXValues)
        .def_property ("kv_h_values",               &PhaseAmperage::get_KvHValues, &PhaseAmperage::set_KvHValues)
        .def_property ("phase_amp_x_values",        &PhaseAmperage::get_PhaseAmpXValues, &PhaseAmperage::set_PhaseAmpXValues)
        .def_property ("phase_amp_h_values",        &PhaseAmperage::get_PhaseAmpHValues, &PhaseAmperage::set_PhaseAmpHValues)
        .def_property ("phase_amp_without_ratio",   &PhaseAmperage::get_PhaseAmpWithoutRatio, &PhaseAmperage::set_PhaseAmpWithoutRatio)
        .def_property ("line_amp_h_values",         &PhaseAmperage::get_LineAmpHValues, &PhaseAmperage::set_LineAmpHValues);
}