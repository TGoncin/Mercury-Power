//    @date      August 24, 2026
//    @author    Tom Goncin

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "conductor.h"
#include "coil.h"
#include "winding.h"
#include "device.h"

namespace py = pybind11;



void bridge_Conductor(py::module_& m)
{

// Expose ConductorSolution struct

    py::class_<home_ConductorSolution>(m, "ConductorSolution")
        .def(py::init<>())
        .def_readwrite("number_of_conductors", &home_ConductorSolution::numberOfConductors)
        .def_readwrite("radial_dimensions", &home_ConductorSolution::radialDimensions)
        .def_readwrite("axial_dimensions", &home_ConductorSolution::axialDimensions);

// Expose Conductor class

    py::class_<home_Conductor>(m, "Conductor")

// Constructors

        .def(py::init<home_Coil*,
            home_MaterialConstants,
            home_CoilBil3Ph,
            home_ConductorTable>(),
            py::arg("coil") = nullptr,
            py::arg("material_constants"),
            py::arg("table_coil_bil_3ph"),
            py::arg("table_conductor_tables"),
            "Initialize Conductor with coil and table parameters")

// Fields & Maps

        .def_readwrite("conductor_fields", &home_Conductor::conductorFields, "Map of Conductor FieldValues")

// Navigation Methods

        .def("get_parent_device", &home_Conductor::getParentDevice, py::return_value_policy::reference, "Get parent device pointer")
        .def("get_parent_winding", &home_Conductor::getParentWinding, py::return_value_policy::reference, "Get parent winding pointer")
        .def("get_parent_coil", &home_Conductor::getParentCoil, py::return_value_policy::reference, "Get parent coil pointer")

// Optimization & Search Methods

        .def("get_nominal_conductor", &home_Conductor::get_NominalConductor, py::arg("winding_type"), "Get nominal conductor numbers")
        .def("find_possible_conductors", &home_Conductor::find_PossibleConductors, py::arg("winding"), "Find possible conductors for winding")
        .def("get_nominal_conductor_opt", &home_Conductor::get_NominalConductorOpt, py::arg("winding_type"), "Get optimized nominal conductor pairs")
        .def("look_conductor_device", &home_Conductor::look_ConductorDevice, py::arg("number_phases"), py::arg("winding"), py::arg("winding_type"), "Look up conductor in device")
        .def("look_no_conductor", &home_Conductor::look_NoConductor, py::arg("conductor_type"), py::arg("number_phases"), py::arg("conductor"), py::arg("winding_type"), "Look up conductor number")
        .def("get_max_density_conductor_opt", &home_Conductor::get_MaxDensityConductorOpt, "Get max density conductor optimization limit")

// Pipeline & Calculation Actions

        .def("reset_fields", &home_Conductor::reset_Fields, "Reset all internal conductor fields")
        .def("copy", &home_Conductor::copy, py::arg("other_conductor"), "Copy conductor parameters")
        .def("erase_warnings", &home_Conductor::erase_Warnings, "Clear warning states")
        .def("assign_winding", &home_Conductor::assign_Winding, py::arg("winding"), "Assign parent winding pointer")
        .def("assign_defaults", &home_Conductor::assign_Defaults, py::arg("device"), py::arg("winding"), py::arg("coil"), py::arg("winding_type"), "Assign default parameters")
        .def("calc_conductor", &home_Conductor::calc_Conductor, py::arg("device"), py::arg("winding"), py::arg("coil"), py::arg("winding_type"), "Execute full conductor calculation pipeline")

// Getters

        .def("get_id", &home_Conductor::getId)
        .def("get_i_type", &home_Conductor::getIType)
        .def("get_i_no_conductor", &home_Conductor::getINoConductor)
        .def("get_i_no_axial", &home_Conductor::getINoAxial)
        .def("get_i_no_radial", &home_Conductor::getINoRadial)
        .def("get_i_radial_level", &home_Conductor::getIRadialLevel)
        .def("get_t_mat_aluminum", &home_Conductor::getTMatAluminum)
        .def("get_d_width", &home_Conductor::getDWidth)
        .def("get_d_weight", &home_Conductor::getDWeight)
        .def("get_d_width_strand", &home_Conductor::getDWidthStrand)
        .def("get_d_thick_strand", &home_Conductor::getDThickStrand)
        .def("get_area_strand", &home_Conductor::getAreaStrand)
        .def("get_d_total_width_turn", &home_Conductor::getDTotalWidthTurn)
        .def("get_d_total_thick_turn", &home_Conductor::getDTotalThickTurn)
        .def("get_d_magnet_area", &home_Conductor::getDMagnetArea)
        .def("get_d_area_cond", &home_Conductor::getDAreaCond)
        .def("get_d_density", &home_Conductor::getDDensity)
        .def("get_d_density_onaf", &home_Conductor::getDDensityONAF)
        .def("get_d_density_fcbn", &home_Conductor::getDDensityFCBN)
        .def("get_d_density_fcbn_onaf", &home_Conductor::getDDensityFCBNONAF)
        .def("get_d_conductor_space", &home_Conductor::getDConductorSpace)
        .def("get_d_stayback", &home_Conductor::getDStayback)
        .def("get_dt", &home_Conductor::getDT)
        .def("get_dt2", &home_Conductor::getDT2)
        .def("get_d_sb_min", &home_Conductor::getDSBMin)
        .def("get_d_uncorr_sb", &home_Conductor::getDUncorrSB)
        .def("get_d_uncorr_cond_spc", &home_Conductor::getDUncorrCondSpc)
        .def("get_d_max_turns_lyr", &home_Conductor::getDMaxTurnsLyr)
        .def("get_d_uncorr_lyrs", &home_Conductor::getDUncorrLyrs)
        .def("get_d_corr_turns_lyrs", &home_Conductor::getDCorrTurnsLyrs)
        .def("get_d_corr_lyrs", &home_Conductor::getDCorrLyrs)
        .def("get_name", &home_Conductor::getName)
        .def("get_s_weight", &home_Conductor::getSWeight)
        .def("get_s_density", &home_Conductor::getSDensity)
        .def("get_s_description", &home_Conductor::getSDescription)
        .def("get_s_sw_conductor", &home_Conductor::getSSWConductor)
        .def("get_s_mat_conductor", &home_Conductor::getSMatConductor)
        .def("get_s_papper_conductor", &home_Conductor::getSPapperConductor)

// Setters

        .def("set_id", &home_Conductor::setId)
        .def("set_i_type", &home_Conductor::setIType)
        .def("set_i_no_conductor", &home_Conductor::setINoConductor)
        .def("set_i_no_axial", &home_Conductor::setINoAxial)
        .def("set_i_no_radial", &home_Conductor::setINoRadial)
        .def("set_i_radial_level", &home_Conductor::setIRadialLevel)
        .def("set_t_mat_aluminum", &home_Conductor::setTMatAluminum)
        .def("set_d_width", &home_Conductor::setDWidth)
        .def("set_d_weight", &home_Conductor::setDWeight)
        .def("set_d_width_strand", &home_Conductor::setDWidthStrand)
        .def("set_d_thick_strand", &home_Conductor::setDThickStrand)
        .def("set_area_strand", &home_Conductor::setAreaStrand)
        .def("set_d_total_width_turn", &home_Conductor::setDTotalWidthTurn)
        .def("set_d_total_thick_turn", &home_Conductor::setDTotalThickTurn)
        .def("set_d_magnet_area", &home_Conductor::setDMagnetArea)
        .def("set_d_area_cond", &home_Conductor::setDAreaCond)
        .def("set_d_density", &home_Conductor::setDDensity)
        .def("set_d_density_onaf", &home_Conductor::setDDensityONAF)
        .def("set_d_density_fcbn", &home_Conductor::setDDensityFCBN)
        .def("set_d_density_fcbn_onaf", &home_Conductor::setDDensityFCBNONAF)
        .def("set_d_conductor_space", &home_Conductor::setDConductorSpace)
        .def("set_d_stayback", &home_Conductor::setDStayback)
        .def("set_dt", &home_Conductor::setDT)
        .def("set_dt2", &home_Conductor::setDT2)
        .def("set_d_sb_min", &home_Conductor::setDSBMin)
        .def("set_d_uncorr_sb", &home_Conductor::setDUncorrSB)
        .def("set_d_uncorr_cond_spc", &home_Conductor::setDUncorrCondSpc)
        .def("set_d_max_turns_lyr", &home_Conductor::setDMaxTurnsLyr)
        .def("set_d_uncorr_lyrs", &home_Conductor::setDUncorrLyrs)
        .def("set_d_corr_turns_lyrs", &home_Conductor::setDCorrTurnsLyrs)
        .def("set_d_corr_lyrs", &home_Conductor::setDCorrLyrs)
        .def("set_name", &home_Conductor::setName)
        .def("set_s_weight", &home_Conductor::setSWeight)
        .def("set_s_density", &home_Conductor::setSDensity)
        .def("set_s_description", &home_Conductor::setSDescription)
        .def("set_s_sw_conductor", &home_Conductor::setSSWConductor)
        .def("set_s_mat_conductor", &home_Conductor::setSMatConductor)
        .def("set_s_papper_conductor", &home_Conductor::setSPapperConductor);
}