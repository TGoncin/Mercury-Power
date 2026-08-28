/**___________________________________________________________________________________________________
|    @date      August 24, 2026
|    @author    Tom Goncin
|    @file      coil_Bridge.cpp
|    @brief     Pybind11 bridge module exposing the home_Coil class.
|___________________________________________________________________________________________________*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "coil.h"
#include "device.h"
#include "winding.h"

namespace py = pybind11;


void bridge_Coil(py::module_& m)
{
    py::class_<home_Coil>(m, "Coil")

// Constructors

        .def(py::init< home_Device*,
            home_CoilType,
            home_RegulationType,
            home_MaterialConstants,
            home_CoilBil1Ph,
            home_CoilBil3Ph,
            home_ConductorTable>(),
            py::arg("device") = nullptr,
            py::arg("coil_type"),
            py::arg("regulation_type"),
            py::arg("material_constants"),
            py::arg("table_coil_bil_1ph"),
            py::arg("table_coil_bil_3ph"),
            py::arg("table_conductor"),
            "Initialize Coil with device and configuration tables")

// Fields & Vectors

        .def_readwrite("coil_fields", &home_Coil::coilFields, "Map of Coil FieldValues")
        .def_readwrite("conductor", &home_Coil::conductor, "Vector of Conductor hardware elements")

// Navigation & Structural Methods

        .def("parent_device", &home_Coil::parentDevice, py::return_value_policy::reference, "Get parent device pointer")
        .def("get_parent_winding", &home_Coil::getParentWinding, py::return_value_policy::reference, "Get parent winding pointer")
        .def("number_conductors", &home_Coil::numberConductors, "Get conductor count")
        .def("get_position", &home_Coil::get_Position, "Get coil position in device list")

// Action & Pipeline Calculations

        .def("assign_index", &home_Coil::assign_Index, py::arg("index"), "Assign coil index ID")
        .def("assign_winding_desc", &home_Coil::assign_WindingDesc, py::arg("winding_desc"), "Assign winding descriptor")
        .def("assign_winding", &home_Coil::assign_Winding, py::arg("winding"), "Assign parent winding pointer")
        .def("remove_winding", &home_Coil::remove_Winding, "Remove coil from current winding")
        .def("assign_parents", &home_Coil::assign_Parents, "Assign parent links down to conductors")
        .def("assign_defaults", &home_Coil::assign_Defaults, "Assign default parametric values")
        .def("reset_fields", &home_Coil::reset_Fields, "Reset all internal coil fields")
        .def("erase_warnings", &home_Coil::erase_Warnings, "Clear warning states")
        .def("calc_warnings", &home_Coil::calc_Warnings, "Calculate coil warnings")
        .def("calc_coil", &home_Coil::calc_Coil, "Execute full coil calculation pipeline")

// Getters

        .def("get_id", &home_Coil::getId)
        .def("get_name", &home_Coil::getName)
        .def("get_i_bil", &home_Coil::getIBil)
        .def("get_t_desc_devanado", &home_Coil::getTDescDevanado)
        .def("get_i_biles", &home_Coil::getIBiles)
        .def("get_d_turns", &home_Coil::getDTurns)
        .def("get_i_layers", &home_Coil::getILayers)
        .def("get_i_sections_leg", &home_Coil::getISectionsLeg)
        .def("get_b_constant_stayback", &home_Coil::getBConstantStayback)
        .def("get_t_regulation_type", &home_Coil::getTRegulationType)
        .def("get_t_coil_type", &home_Coil::getTCoilType)
        .def("get_d_staybak", &home_Coil::getDStaybak)
        .def("get_d_cooling_duct", &home_Coil::getDCoolingDuct)
        .def("get_d_core_protection", &home_Coil::getDCoreProtection)
        .def("get_d_form_details", &home_Coil::getDFormDetails)
        .def("get_d_form_duct", &home_Coil::getDFormDuct)
        .def("get_d_form_001_paper", &home_Coil::getDForm001Paper)
        .def("get_d_under_hv_001_paper", &home_Coil::getDUnderHV001Paper)
        .def("get_d_uohv_001_paper", &home_Coil::getDUOHV001Paper)
        .def("get_d_uohv_duct", &home_Coil::getDUOHVDuct)
        .def("get_d_uohv_crepe_paper", &home_Coil::getDUOHVCrepePaper)
        .def("get_d_uohv_001_paper2", &home_Coil::getDUOHV001Paper2)
        .def("get_d_coil_core_duct", &home_Coil::getDCoilCoreDuct)
        .def("get_d_coil_core_packing", &home_Coil::getDCoilCorePacking)
        .def("get_d_coil_coil_duct", &home_Coil::getDCoilCoilDuct)
        .def("get_d_coil_coil_packing", &home_Coil::getDCoilCoilPacking)
        .def("get_d_coil_coil_duct2", &home_Coil::getDCoilCoilDuct2)
        .def("get_pd_phase_amp", &home_Coil::getPdPhaseAmp)
        .def("get_pd_phase_amp_sin_rel", &home_Coil::getPdPhaseAmpSinRel)
        .def("get_i_long_phase_amp", &home_Coil::getILongPhaseAmp)
        .def("get_pd_phase_amp_dif_pos", &home_Coil::getPdPhaseAmpDifPos)
        .def("get_d_overcoil", &home_Coil::getDOvercoil)
        .def("get_d_turns_section", &home_Coil::getDTurnsSection)
        .def("get_i_turns_layers", &home_Coil::getITurnsLayers)
        .def("get_d_no_layers_under", &home_Coil::getDNoLayersUnder)
        .def("get_d_no_layers_body", &home_Coil::getDNoLayersBody)
        .def("get_d_no_layers_over", &home_Coil::getDNoLayersOver)
        .def("get_d_tot_layers", &home_Coil::getDTotLayers)
        .def("get_i_ducts", &home_Coil::getIDucts)
        .def("get_ducts_in", &home_Coil::getDuctsIn)
        .def("get_ducts_out", &home_Coil::getDuctsOut)
        .def("get_d_width_ducts", &home_Coil::getDWidthDucts)
        .def("get_s_width_ducts", &home_Coil::getSWidthDucts)
        .def("get_d_inter_paper", &home_Coil::getDInterPaper)
        .def("get_d_end_paper", &home_Coil::getDEndPaper)
        .def("get_i_tot_no_end_layers", &home_Coil::getITotNoEndLayers)
        .def("get_s_insulation_end", &home_Coil::getSInsulationEnd)
        .def("get_s_pre_ins_end", &home_Coil::getSPreInsEnd)
        .def("get_s_insulation_int", &home_Coil::getSInsulationInt)
        .def("get_s_insulation_int_complete", &home_Coil::getSInsulationIntComplete)
        .def("get_i_required_num_ducts", &home_Coil::getIRequiredNumDucts)
        .def("get_s_over_outer_preliminar", &home_Coil::getSOverOuterPreliminar)
        .def("get_s_over", &home_Coil::getSOver)
        .def("get_dk_vmm", &home_Coil::getDkVmm)
        .def("get_d_radial_built", &home_Coil::getDRadialBuilt)
        .def("get_s_description_tb", &home_Coil::getSDescriptionTb)
        .def("get_d_radial_built_sides", &home_Coil::getDRadialBuiltSides)
        .def("get_d_radial_built_int_noses", &home_Coil::getDRadialBuiltIntNoses)
        .def("get_d_radial_built_out_noses", &home_Coil::getDRadialBuiltOutNoses)
        .def("get_s_radial_builds", &home_Coil::getSRadialBuilds)
        .def("get_i_high", &home_Coil::getIHigh)
        .def("get_i_wide", &home_Coil::getIWide)
        .def("get_d_cond_thick_turn", &home_Coil::getDCondThickTurn)
        .def("get_d_cond_wdth_turn", &home_Coil::getDCondWdthTurn)
        .def("get_s_error_name", &home_Coil::getSErrorName)
        .def("get_t_error_enum", &home_Coil::getTErrorEnum)
        .def("get_s_error_messages", &home_Coil::getSErrorMessages)

// Setters

        .def("set_id", &home_Coil::setId)
        .def("set_name", &home_Coil::setName)
        .def("set_i_bil", &home_Coil::setIBil)
        .def("set_t_desc_devanado", &home_Coil::setTDescDevanado)
        .def("set_i_biles", &home_Coil::setIBiles)
        .def("set_d_turns", &home_Coil::setDTurns)
        .def("set_i_layers", &home_Coil::setILayers)
        .def("set_i_sections_leg", &home_Coil::setISectionsLeg)
        .def("set_b_constant_stayback", &home_Coil::setBConstantStayback)
        .def("set_t_regulation_type", &home_Coil::setTRegulationType)
        .def("set_t_coil_type", &home_Coil::setTCoilType)
        .def("set_d_staybak", &home_Coil::setDStaybak)
        .def("set_d_cooling_duct", &home_Coil::setDCoolingDuct)
        .def("set_d_core_protection", &home_Coil::setDCoreProtection)
        .def("set_d_form_details", &home_Coil::setDFormDetails)
        .def("set_d_form_duct", &home_Coil::setDFormDuct)
        .def("set_d_form_001_paper", &home_Coil::setDForm001Paper)
        .def("set_d_under_hv_001_paper", &home_Coil::setDUnderHV001Paper)
        .def("set_d_uohv_001_paper", &home_Coil::setDUOHV001Paper)
        .def("set_d_uohv_duct", &home_Coil::setDUOHVDuct)
        .def("set_d_uohv_crepe_paper", &home_Coil::setDUOHVCrepePaper)
        .def("set_d_uohv_001_paper2", &home_Coil::setDUOHV001Paper2)
        .def("set_d_coil_core_duct", &home_Coil::setDCoilCoreDuct)
        .def("set_d_coil_core_packing", &home_Coil::setDCoilCorePacking)
        .def("set_d_coil_coil_duct", &home_Coil::setDCoilCoilDuct)
        .def("set_d_coil_coil_packing", &home_Coil::setDCoilCoilPacking)
        .def("set_d_coil_coil_duct2", &home_Coil::setDCoilCoilDuct2)
        .def("set_pd_phase_amp", &home_Coil::setPdPhaseAmp)
        .def("set_pd_phase_amp_sin_rel", &home_Coil::setPdPhaseAmpSinRel)
        .def("set_i_long_phase_amp", &home_Coil::setILongPhaseAmp)
        .def("set_pd_phase_amp_dif_pos", &home_Coil::setPdPhaseAmpDifPos)
        .def("set_d_overcoil", &home_Coil::setDOvercoil)
        .def("set_d_turns_section", &home_Coil::setDTurnsSection)
        .def("set_i_turns_layers", &home_Coil::setITurnsLayers)
        .def("set_d_no_layers_under", &home_Coil::setDNoLayersUnder)
        .def("set_d_no_layers_body", &home_Coil::setDNoLayersBody)
        .def("set_d_no_layers_over", &home_Coil::setDNoLayersOver)
        .def("set_d_tot_layers", &home_Coil::setDTotLayers)
        .def("set_i_ducts", &home_Coil::setIDucts)
        .def("set_ducts_in", &home_Coil::setDuctsIn)
        .def("set_ducts_out", &home_Coil::setDuctsOut)
        .def("set_d_width_ducts", &home_Coil::setDWidthDucts)
        .def("set_s_width_ducts", &home_Coil::setSWidthDucts)
        .def("set_d_inter_paper", &home_Coil::setDInterPaper)
        .def("set_d_end_paper", &home_Coil::setDEndPaper)
        .def("set_i_tot_no_end_layers", &home_Coil::setITotNoEndLayers)
        .def("set_s_insulation_end", &home_Coil::setSInsulationEnd)
        .def("set_s_pre_ins_end", &home_Coil::setSPreInsEnd)
        .def("set_s_insulation_int", &home_Coil::setSInsulationInt)
        .def("set_s_insulation_int_complete", &home_Coil::setSInsulationIntComplete)
        .def("set_i_required_num_ducts", &home_Coil::setIRequiredNumDucts)
        .def("set_s_over_outer_preliminar", &home_Coil::setSOverOuterPreliminar)
        .def("set_s_over", &home_Coil::setSOver)
        .def("set_dk_vmm", &home_Coil::setDkVmm)
        .def("set_d_radial_built", &home_Coil::setDRadialBuilt)
        .def("set_s_description_tb", &home_Coil::setSDescriptionTb)
        .def("set_d_radial_built_sides", &home_Coil::setDRadialBuiltSides)
        .def("set_d_radial_built_int_noses", &home_Coil::setDRadialBuiltIntNoses)
        .def("set_d_radial_built_out_noses", &home_Coil::setDRadialBuiltOutNoses)
        .def("set_s_radial_builds", &home_Coil::setSRadialBuilds)
        .def("set_i_high", &home_Coil::setIHigh)
        .def("set_i_wide", &home_Coil::setIWide)
        .def("set_d_cond_thick_turn", &home_Coil::setDCondThickTurn)
        .def("set_d_cond_wdth_turn", &home_Coil::setDCondWdthTurn)
        .def("set_s_error_name", &home_Coil::setSErrorName)
        .def("set_t_error_enum", &home_Coil::setTErrorEnum)
        .def("set_s_error_messages", &home_Coil::setSErrorMessages);
}