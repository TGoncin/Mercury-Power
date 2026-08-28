/**___________________________________________________________________________________________________
|    @date      August 24, 2026
|    @author    Tom Goncin
|    @file      core_Bridge.cpp
|    @brief     Pybind11 bridge module exposing the home_Core class.
|___________________________________________________________________________________________________*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alias.h"
using namespace Transformer::Types;

#include "core.h"
#include "device.h"
#include "winding.h"

namespace py = pybind11;

void bridge_Core(py::module_& m)
{
    py::class_<home_Core>(m, "Core")


// Constructors

        .def(py::init<Transformer::Core::Device*, Transformer::Commercial::MaterialConstants>(),
            py::arg("device"),
            py::arg("material_constants"),
            "Initialize Core with device and material constants")

// Fields & Vectors

        .def_readwrite("core_fields", &home_Core::coreFields, "Map of Core FieldValues")
        .def_readwrite("press_board_packing", &home_Core::pressBoardPacking, "Pressboard packing 2D array matrix")

// Navigation & Property Methods

        .def("type_lamination", &home_Core::type_Lamination, "Get lamination type enum")
        .def("get_parent_device", &home_Core::getParentDevice, py::return_value_policy::reference, "Get parent device pointer")
        .def("get_lamination_number", &home_Core::get_LaminationNumber, "Get lamination numeric mapping value")

// Action & Pipeline Calculation Methods

        .def("reset_fields", &home_Core::reset_Fields, "Reset all internal core fields")
        .def("erase_warnings", &home_Core::erase_Warnings, "Clear warning states")
        .def("calc_core", &home_Core::calc_Core, "Execute full core calculation pipeline")
        .def("calc_loop_core_strip", &home_Core::calc_LoopCoreStrip, py::arg("number_phases"), "Calculate loop core strip specifications")
        .def("calc_steel_width", &home_Core::calc_SteelWidth, py::arg("number_phases"), "Calculate steel width")
        .def("calc_area_core", &home_Core::calc_AreaCore, "Calculate cross-sectional core area")
        .def("calc_density_limit", &home_Core::calc_DensityLimit, py::arg("device"), "Calculate flux density limit")
        .def("calc_density", &home_Core::calc_Density, py::arg("device"), "Calculate core flux densities")
        .def("calc_validation_core", &home_Core::calc_ValidationCore, "Validate core constraints")
        .def("calc_forms", &home_Core::calc_Forms, "Calculate core form string mappings")
        .def("calc_stack_per_loop", &home_Core::calc_StackPerLoop, "Calculate stack height per loop")
        .def("calc_strip_width", &home_Core::calc_StripWidth, py::arg("number_phases"), "Calculate strip width")
        .def("calc_overall_core_height", &home_Core::calc_OverallCoreHeight, py::arg("number_phases"), "Calculate overall core height")
        .def("calc_core_length_without_pb", &home_Core::calc_CoreLengthWithoutPB, py::arg("number_phases"), "Calculate core length excluding pressboard")
        .def("calc_core_length_with_pb", &home_Core::calc_CoreLengthWithPB, "Calculate core length including pressboard")
        .def("calc_no_load_loss_data_array", &home_Core::calc_NoLoadLossDataArray, py::arg("device"), "Initialize no load loss calculations")
        .def("calc_destruction_factors", &home_Core::calc_DestructionFactors, "Calculate core destruction factors")
        .def("calc_weights", &home_Core::calc_Weights, py::arg("number_phases"), "Calculate core and loop weights")
        .def("calc_clamp_insulation_weight", &home_Core::calc_ClampInsulationWeight, "Calculate clamp insulation dimensions and weight")
        .def("calc_form_dimensions", &home_Core::calc_FormDimensions, "Calculate physical form dimensions")
        .def("calc_weight_press_board", &home_Core::calc_WeightPressBoard, py::arg("device"), "Calculate pressboard insulation weights")
        .def("calc_no_load_losses", &home_Core::calc_NoLoadLosses, py::arg("device"), py::arg("number_phases"), "Calculate core no load loss metrics")
        .def("calc_curr_exc", &home_Core::calc_CurrExc, py::arg("device"), py::arg("number_phases"), "Calculate exciting current ratios")
        .def("calc_string_rated", &home_Core::calc_StringRated, py::arg("number_phases"), "Calculate rated voltage text strings")
        .def("calc_warnings", &home_Core::calc_Warnings, "Calculate core design warnings")
        .def("calc_area_core_opt", &home_Core::calc_AreaCoreOpt, "Optimize core cross-sectional area stack")
        .def("get_list_total_stack", &home_Core::get_ListTotalStack, py::arg("steel_width"), "Get valid total stack options for steel width")
        .def("get_list_total_stack_opt", &home_Core::get_ListTotalStackOpt, py::arg("device"), py::arg("steel_width"), "Get optimized total stack options")
        .def("calc_s_part", &home_Core::calc_sPart, "Generate part identification string")

// Overloaded helper functions

        .def("calc_loop", py::overload_cast<double, double, double, double, double, double>(&home_Core::calc_Loop),
            py::arg("small_window_large_width"), py::arg("window_height"), py::arg("strip_width"), py::arg("stack_per_loop"), py::arg("k_space_factor"), py::arg("d_stack_tol_in") = 0.0,
            "Calculate core loop starting and ending lengths")

// Getters

        .def("get_t_isolated_core", &home_Core::getTIsolatedCore)
        .def("get_id", &home_Core::getId)
        .def("get_t_lamination_type", &home_Core::getTLaminationType)
        .def("get_i_total_stack", &home_Core::getITotalStack)
        .def("get_i_no_loops", &home_Core::getINoLoops)
        .def("get_d_total_stack", &home_Core::getDTotalStack)
        .def("get_d_steel_width", &home_Core::getDSteelWidth)
        .def("get_d_per_overvoltage_max_flux", &home_Core::getDPerOvervoltageMaxFlux)
        .def("get_d_test_induction_level", &home_Core::getDTestInductionLevel)
        .def("get_d_max_watts_pound", &home_Core::getDMaxWattsPound)
        .def("get_d_loss_fact", &home_Core::getDLossFact)
        .def("get_d_initial_flux_density", &home_Core::getDInitialFluxDensity)
        .def("get_d_flux_density_rat_per", &home_Core::getDFluxDensityRatPer)
        .def("get_d_flux_density_volt", &home_Core::getDFluxDensityVolt)
        .def("get_d_flux_density_rat_per_t", &home_Core::getDFluxDensityRatPerT)
        .def("get_d_flux_density_volt_t", &home_Core::getDFluxDensityVoltT)
        .def("get_b_loop1_am_core_strip", &home_Core::getBLoop1AmCoreStrip)
        .def("get_b_loop2_am_core_strip", &home_Core::getBLoop2AmCoreStrip)
        .def("get_d_window_height", &home_Core::getDWindowHeight)
        .def("get_d_small_window_width", &home_Core::getDSmallWindowWidth)
        .def("get_d_large_window_width", &home_Core::getDLargeWindowWidth)
        .def("get_d_stack_per_loop", &home_Core::getDStackPerLoop)
        .def("get_d_strip_width", &home_Core::getDStripWidth)
        .def("get_d_e_bottom", &home_Core::getDEBottom)
        .def("get_d_stack_core_sm_loops", &home_Core::getDStackCoreSmLoops)
        .def("get_d_stack_core_lg_loops", &home_Core::getDStackCoreLgLoops)
        .def("get_d_overall_core_height", &home_Core::getDOverallCoreHeight)
        .def("get_d_overall_core_height_inner", &home_Core::getDOverallCoreHeightInner)
        .def("get_d_core_length_without_pb", &home_Core::getDCoreLengthWithoutPB)
        .def("get_d_core_length_with_pb", &home_Core::getDCoreLengthWithPB)
        .def("get_d_area", &home_Core::getDArea)
        .def("get_d_induction_level", &home_Core::getDInductionLevel)
        .def("get_d_flux_density_ini", &home_Core::getDFluxDensityIni)
        .def("get_d_flux_density_limit_max", &home_Core::getDFluxDensityLimitMax)
        .def("get_d_form_nose", &home_Core::getDFormNose)
        .def("get_d_form_side", &home_Core::getDFormSide)
        .def("get_d_true_over_coil", &home_Core::getDTrueOverCoil)
        .def("get_d_leg_centers", &home_Core::getDLegCenters)
        .def("get_d_weight_one_sm_loop", &home_Core::getDWeightOneSmLoop)
        .def("get_d_weight_one_lg_loop", &home_Core::getDWeightOneLgLoop)
        .def("get_d_weight_one_loop", &home_Core::getDWeightOneLoop)
        .def("get_d_weight_two_sm_loops", &home_Core::getDWeightTwoSmLoops)
        .def("get_d_weight_two_lg_loops", &home_Core::getDWeightTwoLgLoops)
        .def("get_d_weight_two_loops", &home_Core::getDWeightTwoLoops)
        .def("get_d_weights1_8", &home_Core::getDWeights1_8)
        .def("get_d_weights1_16", &home_Core::getDWeights1_16)
        .def("get_d_weights_pb1_8", &home_Core::getDWeightsPB1_8)
        .def("get_d_weights_pb1_16", &home_Core::getDWeightsPB1_16)
        .def("get_d_weights_pb_hd_1_4", &home_Core::getDWeightsPBHD_1_4)
        .def("get_d_weight_press_board", &home_Core::getDWeightPressBoard)
        .def("get_d_nll", &home_Core::getDNLL)
        .def("get_d_flux_dens100v", &home_Core::getDFluxDens100V)
        .def("get_s_rated_pu", &home_Core::getSRatedPU)
        .def("get_s_rated_v", &home_Core::getSRatedV)
        .def("get_name", &home_Core::getName)
        .def("get_s_drawing", &home_Core::getSDrawing)
        .def("get_s_area", &home_Core::getSArea)
        .def("get_s_forms", &home_Core::getSForms)
        .def("get_s_sheet_comments", &home_Core::getSSheetComments)
        .def("get_s_core_suffix", &home_Core::getSCoreSuffix)
        .def("get_t_error_enum", &home_Core::getTErrorEnum)
        .def("get_s_error_name", &home_Core::getSErrorName)
        .def("get_s_error_messages", &home_Core::getSErrorMessages)

// Setters

        .def("set_id", &home_Core::setId)
        .def("set_t_lamination_type", &home_Core::setTLaminationType)
        .def("set_i_total_stack", &home_Core::setITotalStack)
        .def("set_i_no_loops", &home_Core::setINoLoops)
        .def("set_d_total_stack", &home_Core::setDTotalStack)
        .def("set_d_steel_width", &home_Core::setDSteelWidth)
        .def("set_d_per_overvoltage_max_flux", &home_Core::setDPerOvervoltageMaxFlux)
        .def("set_d_test_induction_level", &home_Core::setDTestInductionLevel)
        .def("set_d_max_watts_pound", &home_Core::setDMaxWattsPound)
        .def("set_d_loss_fact", &home_Core::setDLossFact)
        .def("set_d_induction_level", &home_Core::setDInductionLevel)
        .def("set_t_isolated_core", &home_Core::setTIsolatedCore)
        .def("set_d_flux_density_ini", &home_Core::setDFluxDensityIni)
        .def("set_d_flux_density_limit_max", &home_Core::setDFluxDensityLimitMax)
        .def("set_name", &home_Core::setName)
        .def("set_s_drawing", &home_Core::setSDrawing)
        .def("set_s_rated_pu", &home_Core::setSRatedPU)
        .def("set_s_rated_v", &home_Core::setSRatedV)
        .def("set_s_area", &home_Core::setSArea)
        .def("set_s_forms", &home_Core::setSForms)
        .def("set_s_sheet_comments", &home_Core::setSSheetComments)
        .def("set_s_core_suffix", &home_Core::setSCoreSuffix);
}