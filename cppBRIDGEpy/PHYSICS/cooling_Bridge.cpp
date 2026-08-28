//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      cooling_Bridge.cpp
//    @brief     MidConnector pybind11 wrapper for Transformer Cooling Engine.


#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 

#include "alias.h"
using namespace Transformer::Types;

#include "cooling.h"
#include "winding.h"
#include "device.h"
#include "gradient.h"
#include "mechanical.h"



namespace py = pybind11;



void bridge_Cooling(py::module_& m) 
{
    py::class_<home_Cooling>(m, "Cooling")

//          B I N D    C O N S T R U C T O R 


        .def(py::init<home_Device*>())


//          B I N D    S T A N D A R D   M E T H O D S 


        .def_property_readonly("parent_device", &home_Cooling::getParentDevice)

        .def("reset_fields", &home_Cooling::reset_Fields)
        .def("eraseWarnings", &home_Cooling::erase_Warnings)
        .def("calculate_cooling", &home_Cooling::calc_Cooling)
        .def("calculate_coolingtype", &home_Cooling::calc_CoolingType)
        .def("calculate_tempRise", &home_Cooling::calc_TempRise)
        .def("calculate_ONAF", &home_Cooling::calc_ONAF)

        .def("calculate_MVAOExc", &home_Cooling::calc_MVAOExc,
            py::arg("winding_Hv"),
            py::arg("designSpecCodes"),
            py::arg("NominalKva"))

        .def("add_step", &home_Cooling::add_Step,
            py::arg("step"))

        .def("clear_steps", &home_Cooling::clear_Steps)

        .def("set_type_cooling", &home_Cooling::set_CoolingType_At,
            py::arg("index"),
            py::arg("value"))

        .def("calculate_newCurrentTemp", &home_Cooling::calc_NewCurrentTemp)
        .def("calculate_UseFR3", &home_Cooling::calc_UseFR3)

        .def("calculate_MaxONAF", &home_Cooling::calc_MaxONAF,
            py::arg("mechanical"))

        .def("calculate_TORONAF", &home_Cooling::calc_TORONAF,
            py::arg("gradients"))

        .def("IsKNAN", &home_Cooling::isKNAN)
        .def("calculate_TempRiseNumber", &home_Cooling::calc_TempRiseNumber)


//          B I N D   G E T T E R S   &   S E T T E R S 


        .def_property("cooling_steps_real", &home_Cooling::get_CoolingStepsReal, &home_Cooling::set_CoolingStepsReal)
        .def_property("cooling_steps", &home_Cooling::get_CoolingSteps, &home_Cooling::set_CoolingSteps)
        .def_property("cooling_type", &home_Cooling::get_CoolingType, &home_Cooling::set_CoolingType)
        .def_property("cooling_types", &home_Cooling::get_CoolingTypes, &home_Cooling::set_CoolingTypes)
        .def_property("temperature_rise", &home_Cooling::get_TemperatureRise, &home_Cooling::set_TemperatureRise)
        .def_property("temperature_rises", &home_Cooling::get_TemperatureRises, &home_Cooling::set_TemperatureRises)
        .def_property("temp_cool_rise", &home_Cooling::get_TempCoolRise, &home_Cooling::set_TempCoolRise)
        .def_property("prov_onaf", &home_Cooling::get_ProvONAF, &home_Cooling::set_ProvONAF)
        .def_property("k4_factor", &home_Cooling::get_K4Factor, &home_Cooling::set_K4Factor)
        .def_property("cooling_type_k4", &home_Cooling::get_CoolingTypeK4, &home_Cooling::set_CoolingTypeK4)
        .def_property("temperature_rise_k4", &home_Cooling::get_TemperatureRiseK4, &home_Cooling::set_TemperatureRiseK4)
        .def_property("new_temp_rise", &home_Cooling::get_NewTempRise, &home_Cooling::set_NewTempRise)
        .def_property("current_temp_rise", &home_Cooling::get_CurrentTempRise, &home_Cooling::set_CurrentTempRise)
        .def_property("use_fr3", &home_Cooling::get_UseFR3, &home_Cooling::set_UseFR3)
        .def_property("max_onaf", &home_Cooling::get_MaxONAF, &home_Cooling::set_MaxONAF)
        .def_property("tor_onaf", &home_Cooling::get_TORONAF, &home_Cooling::set_TORONAF)
        .def_property("temp_onaf", &home_Cooling::get_TempONAF, &home_Cooling::set_TempONAF)
        .def_property("over_excitation", &home_Cooling::get_OverExcitation, &home_Cooling::set_OverExcitation)
        .def_property("temp_o_exc", &home_Cooling::get_TempOExc, &home_Cooling::set_TempOExc)
        .def_property("mva_o_exc", &home_Cooling::get_MVAOExc, &home_Cooling::set_MVAOExc)
        .def_property("mva_o_exc_string", &home_Cooling::get_MVAOExc_String, &home_Cooling::set_MVAOExc_String)
        .def_property("error_name", &home_Cooling::get_ErrorName, &home_Cooling::set_ErrorName)
        .def_property("error_enum", &home_Cooling::get_ErrorEnum, &home_Cooling::set_ErrorEnum)
        .def_property("error_messages", &home_Cooling::get_ErrorMessages, &home_Cooling::set_ErrorMessages);
}