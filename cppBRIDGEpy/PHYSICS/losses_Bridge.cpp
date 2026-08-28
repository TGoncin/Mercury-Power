
//    @date      August 20, 2026
//    @author    Tom Goncin & Sandra
//    @file      home_Losses_Bridge.cpp
//    @brief     MidConnector pybind11 wrapper for Transformer Electrical Resistance and home_Losses engine.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 

#include "alias.h"
#include "field_Value.h"
#include "materials.h"
#include "math_Utilities.h"
#include "definitions.h"

#include "mechanical.h"
#include "losses.h"
#include "device.h"
#include "winding.h"
#include "coil.h"
#include "cooling.h"
#include "core.h"
#include "guarantee.h"
#include "losses_Warnings.h"


using namespace Transformer::Types;
namespace py = pybind11;





void bridge_Losses(py::module_& m) 
{
    py::class_<home_Losses>(m, "home_Losses")


//          B I N D   C O N S T R U C T O R


    .def(py::init([](home_Device* parentDevice) { return new home_Losses(parentDevice); }))


//          B I N D    S T A N D A R D   M E T H O D S 


    .def_property_readonly("parent_device", &home_Losses::getParentDevice)

    .def("reset_fields", &home_Losses::reset_LossFields)
    .def("eraseWarnings", &home_Losses::erase_Warnings)
    .def("calculateWarnings", &home_Losses::calc_Warnings)

//                                  MUST Map Overloaded C++ functions to legacy distinct Python names

    .def("calculate_TotalLLoss", static_cast<void (home_Losses::*)(home_Device*)>(&home_Losses::calc_TotalLoss), py::arg("device"))
    .def("calculate_TotalLoss", static_cast<void (home_Losses::*)(home_Core*)>(&home_Losses::calc_TotalLoss), py::arg("core"))

//                                  Standard mappings

    .def("calculate_home_Losses", &home_Losses::calc_Loss)
    .def("calculate_tbResistivity", &home_Losses::calc_TableResistivity)
    .def("calculate_tbResistance", &home_Losses::calc_TableResistance)
    .def("calculate_LeadsBus", &home_Losses::calc_LeadsBus)
    .def("function_Temp_factor", &home_Losses::func_TempFactor)
    .def("calculate_ResistanceAndLoss", &home_Losses::calc_ResistanceAndLoss)
    .def("calculate_coilStray", &home_Losses::calc_CoilStray)
    .def("calculate_TotalStray", &home_Losses::calc_TotalStray)
    .def("calculate_CoreLoss", &home_Losses::calc_CoreLoss)
    .def("calculate_MaxWatts", &home_Losses::calc_MaxWatts)
    .def("calculate_home_LossesForEvaluation", &home_Losses::calc_LossToEvaluate)
    .def("calculate_evaluationLoss", &home_Losses::calc_EvaluateLoss)
    .def("calculate_Loadhome_LossesWinding", &home_Losses::calc_LoadLossWinding)


//          B I N D   G E T T E R S   &   S E T T E R S 


    .def_property("d_lv_wo_leads_current_temp_r", &home_Losses::getDLVWOLeadsCurrentTempR, &home_Losses::setDLVWOLeadsCurrentTempR)
    .def_property("d_lv_leads_current_temp_r", &home_Losses::getDLVLeadsCurrentTempR, &home_Losses::setDLVLeadsCurrentTempR)
    .def_property("d_total_lv_current_temp_r", &home_Losses::getDTotalLVCurrentTempR, &home_Losses::setDTotalLVCurrentTempR)
    .def_property("d_hv_wo_leads_current_temp_r", &home_Losses::getDHVWOLeadsCurrentTempR, &home_Losses::setDHVWOLeadsCurrentTempR)
    .def_property("d_hv_leads_current_temp_r", &home_Losses::getDHVLeadsCurrentTempR, &home_Losses::setDHVLeadsCurrentTempR)
    .def_property("d_total_hv_current_temp_r", &home_Losses::getDTotalHVCurrentTempR, &home_Losses::setDTotalHVCurrentTempR)

    .def_property("d_lv_wo_leads_new_temp_r", &home_Losses::getDLVWOLeadsNewTempR, &home_Losses::setDLVWOLeadsNewTempR)
    .def_property("d_lv_leads_new_temp_r", &home_Losses::getDLVLeadsNewTempR, &home_Losses::setDLVLeadsNewTempR)
    .def_property("d_total_lv_new_temp_r", &home_Losses::getDTotalLVNewTempR, &home_Losses::setDTotalLVNewTempR)
    .def_property("d_hv_wo_leads_new_temp_r", &home_Losses::getDHVWOLeadsNewTempR, &home_Losses::setDHVWOLeadsNewTempR)
    .def_property("d_hv_leads_new_temp_r", &home_Losses::getDHVLeadsNewTempR, &home_Losses::setDHVLeadsNewTempR)
    .def_property("d_total_hv_new_temp_r", &home_Losses::getDTotalHVNewTempR, &home_Losses::setDTotalHVNewTempR)

    .def_property("d_lv_wo_leads_current_temp_w", &home_Losses::getDLVWOLeadsCurrentTempW, &home_Losses::setDLVWOLeadsCurrentTempW)
    .def_property("d_lv_leads_current_temp_w", &home_Losses::getDLVLeadsCurrentTempW, &home_Losses::setDLVLeadsCurrentTempW)
    .def_property("d_total_lv_current_temp_w", &home_Losses::getDTotalLVCurrentTempW, &home_Losses::setDTotalLVCurrentTempW)
    .def_property("d_hv_wo_leads_current_temp_w", &home_Losses::getDHVWOLeadsCurrentTempW, &home_Losses::setDHVWOLeadsCurrentTempW)
    .def_property("d_hv_leads_current_temp_w", &home_Losses::getDHVLeadsCurrentTempW, &home_Losses::setDHVLeadsCurrentTempW)
    .def_property("d_total_hv_current_temp_w", &home_Losses::getDTotalHVCurrentTempW, &home_Losses::setDTotalHVCurrentTempW)

    .def_property("d_lv_wo_leads_new_temp_w", &home_Losses::getDLVWOLeadsNewTempW, &home_Losses::setDLVWOLeadsNewTempW)
    .def_property("d_lv_leads_new_temp_w", &home_Losses::getDLVLeadsNewTempW, &home_Losses::setDLVLeadsNewTempW)
    .def_property("d_total_lv_new_temp_w", &home_Losses::getDTotalLVNewTempW, &home_Losses::setDTotalLVNewTempW)
    .def_property("d_hv_wo_leads_new_temp_w", &home_Losses::getDHVWOLeadsNewTempW, &home_Losses::setDHVWOLeadsNewTempW)
    .def_property("hv_leads_new_temp_w", &home_Losses::getHvLeadsNewTempW, &home_Losses::setHvLeadsNewTempW)
    .def_property("d_total_hv_new_temp_w", &home_Losses::getDTotalHVNewTempW, &home_Losses::setDTotalHVNewTempW)

    .def_property("d_lv_wo_leads_current_temp_taps", &home_Losses::getDLVWOLeadsCurrentTempTaps, &home_Losses::setDLVWOLeadsCurrentTempTaps)
    .def_property("d_lv_leads_current_temp_taps", &home_Losses::getDLVLeadsCurrentTempTaps, &home_Losses::setDLVLeadsCurrentTempTaps)
    .def_property("d_total_lv_current_temp_taps", &home_Losses::getDTotalLVCurrentTempTaps, &home_Losses::setDTotalLVCurrentTempTaps)
    .def_property("d_hv_wo_leads_current_temp_taps", &home_Losses::getDHVWOLeadsCurrentTempTaps, &home_Losses::setDHVWOLeadsCurrentTempTaps)
    .def_property("d_hv_leads_current_temp_taps", &home_Losses::getDHVLeadsCurrentTempTaps, &home_Losses::setDHVLeadsCurrentTempTaps)
    .def_property("d_total_hv_current_temp_taps", &home_Losses::getDTotalHVCurrentTempTaps, &home_Losses::setDTotalHVCurrentTempTaps)

    .def_property("d_lv_wo_leads_new_temp_taps", &home_Losses::getDLVWOLeadsNewTempTaps, &home_Losses::setDLVWOLeadsNewTempTaps)
    .def_property("d_lv_leads_new_temp_taps", &home_Losses::getDLVLeadsNewTempTaps, &home_Losses::setDLVLeadsNewTempTaps)
    .def_property("d_total_lv_new_temp_taps", &home_Losses::getDTotalLVNewTempTaps, &home_Losses::setDTotalLVNewTempTaps)
    .def_property("d_hv_wo_leads_new_temp_taps", &home_Losses::getDHVWOLeadsNewTempTaps, &home_Losses::setDHVWOLeadsNewTempTaps)
    .def_property("d_hv_leads_new_temp_taps", &home_Losses::getDHVLeadsNewTempTaps, &home_Losses::setDHVLeadsNewTempTaps)
    .def_property("d_total_hv_new_temp_taps", &home_Losses::getDTotalHVNewTempTaps, &home_Losses::setDTotalHVNewTempTaps)

    .def_property("d_i2r_current_temp", &home_Losses::getDI2RCurrentTemp, &home_Losses::setDI2RCurrentTemp)
    .def_property("d_i2r_new_temp", &home_Losses::getDI2RNewTemp, &home_Losses::setDI2RNewTemp)
    .def_property("d_i2r_current_temp_taps", &home_Losses::getDI2RCurrentTempTaps, &home_Losses::setDI2RCurrentTempTaps)
    .def_property("d_i2r_new_temp_taps", &home_Losses::getDI2RNewTempTaps, &home_Losses::setDI2RNewTempTaps)
    .def_property("d_lv_lead_loss", &home_Losses::getDLVLeadLoss, &home_Losses::setDLVLeadLoss)

    .def_property("d_tank_stray", &home_Losses::getDTankStray, &home_Losses::setDTankStray)
    .def_property("d_coil_sray", &home_Losses::getDCoilSray, &home_Losses::setDCoilSray)
    .def_property("total_stray", &home_Losses::getTotalStray, &home_Losses::setTotalStray)
    .def_property("d_total_lloss_stray_lead", &home_Losses::getDTotalLLossStrayLead, &home_Losses::setDTotalLLossStrayLead)
    .def_property("d_core_loss", &home_Losses::getDCoreLoss, &home_Losses::setDCoreLoss)
    .def_property("d_total_loss", &home_Losses::getDTotalLoss, &home_Losses::setDTotalLoss)

    .def_property("d_no_load_loss_cost", &home_Losses::getDNoLoadLossCost, &home_Losses::setDNoLoadLossCost)
    .def_property("d_load_loss_cost", &home_Losses::getDLoadLossCost, &home_Losses::setDLoadLossCost)
    .def_property("d_total_loss_cost", &home_Losses::getDTotalLossCost, &home_Losses::setDTotalLossCost)
    .def_property("s_no_load_loss_cost", &home_Losses::getSNoLoadLossCost, &home_Losses::setSNoLoadLossCost)
    .def_property("s_load_loss_cost", &home_Losses::getSLoadLossCost, &home_Losses::setSLoadLossCost)
    .def_property("s_total_loss_cost", &home_Losses::getSTotalLossCost, &home_Losses::setSTotalLossCost)

    .def_property("d_max_watts", &home_Losses::getDMaxWatts, &home_Losses::setDMaxWatts)
    .def_property("d_max_watts_k4", &home_Losses::getDMaxWattsK4, &home_Losses::setDMaxWattsK4)
    .def_property("noLoadLossLosses", &home_Losses::getNoLoadLossLosses, &home_Losses::setNoLoadLossLosses)
    .def_property("load_loss_osses", &home_Losses::getLoadLossosses, &home_Losses::setLoadLossosses)

    .def_property("i_coil_ld_mat", &home_Losses::getICoilLdMat, &home_Losses::setICoilLdMat)
    .def_property("area_lv_bus", &home_Losses::getAreaLvBus, &home_Losses::setAreaLvBus)
    .def_property("d_factor_lv_bus", &home_Losses::getDFactorLVBus, &home_Losses::setDFactorLVBus)
    .def_property("length_in_lv_bus", &home_Losses::getLengthInLvBus, &home_Losses::setLengthInLvBus)
    .def_property("d_ohms_lv_bus", &home_Losses::getDOhmsLVBus, &home_Losses::setDOhmsLVBus)
    .def_property("d_loss_lv_bus", &home_Losses::getDLossLVBus, &home_Losses::setDLossLVBus)
    .def_property("lead_length_out_of_coil", &home_Losses::getLeadLengthOutOfCoil, &home_Losses::setLeadLengthOutOfCoil)

    .def_property("i_bus_ld_mat", &home_Losses::getIBusLdMat, &home_Losses::setIBusLdMat)
    .def_property("area_bus_work", &home_Losses::getAreaBusWork, &home_Losses::setAreaBusWork)
    .def_property("d_factor_buswork", &home_Losses::getDFactorBuswork, &home_Losses::setDFactorBuswork)
    .def_property("d_ohms_buswork", &home_Losses::getDOhmsBuswork, &home_Losses::setDOhmsBuswork)
    .def_property("d_loss_buswork", &home_Losses::getDLossBuswork, &home_Losses::setDLossBuswork)
    .def_property("d_tot_ld_loss", &home_Losses::getDTotLdLoss, &home_Losses::setDTotLdLoss)
    .def_property("d_hv_amps_reg_lds", &home_Losses::getDHVAmpsRegLds, &home_Losses::setDHVAmpsRegLds)
    .def_property("d_hv_line_amps_reg_lds", &home_Losses::getDHVLineAmpsRegLds, &home_Losses::setDHVLineAmpsRegLds)

    .def_property("i_coil_ld_mat_hv", &home_Losses::getICoilLdMatHV, &home_Losses::setICoilLdMatHV)
    .def_property("d_area_hv_bus", &home_Losses::getDAreaHVBus, &home_Losses::setDAreaHVBus)
    .def_property("d_factor_hv_bus", &home_Losses::getDFactorHVBus, &home_Losses::setDFactorHVBus)
    .def_property("d_length_in_hv_bus", &home_Losses::getDLengthInHVBus, &home_Losses::setDLengthInHVBus)
    .def_property("d_ohms_hv_bus", &home_Losses::getDOhmsHVBus, &home_Losses::setDOhmsHVBus)
    .def_property("d_loss_hv_bus", &home_Losses::getDLossHVBus, &home_Losses::setDLossHVBus)
    .def_property("area_bus_work_hv", &home_Losses::getAreaBusWorkHV, &home_Losses::setAreaBusWorkHV)
    .def_property("d_factor_buswork_hv", &home_Losses::getDFactorBusworkHV, &home_Losses::setDFactorBusworkHV)
    .def_property("d_ohms_buswork_hv", &home_Losses::getDOhmsBusworkHV, &home_Losses::setDOhmsBusworkHV)
    .def_property("d_loss_buswork_hv", &home_Losses::getDLossBusworkHV, &home_Losses::setDLossBusworkHV)
    .def_property("d_tot_ld_loss_hv", &home_Losses::getDTotLdLossHV, &home_Losses::setDTotLdLossHV)

    .def_property("d_20_ll_winding_lv", &home_Losses::getD20LLwindingLv, &home_Losses::setD20LLwindingLv)
    .def_property("d_55_ll_winding_lv", &home_Losses::getD55LLwindingLv, &home_Losses::setD55LLwindingLv)
    .def_property("d_75_ll_winding_lv", &home_Losses::getD75LLwindingLv, &home_Losses::setD75LLwindingLv)
    .def_property("d_85_ll_winding_lv", &home_Losses::getD85LLwindingLv, &home_Losses::setD85LLwindingLv)
    .def_property("d_20_ll_winding_hv", &home_Losses::getD20LLwindingHv, &home_Losses::setD20LLwindingHv)
    .def_property("d_55_ll_winding_hv", &home_Losses::getD55LLwindingHv, &home_Losses::setD55LLwindingHv)
    .def_property("d_75_ll_winding_hv", &home_Losses::getD75LLwindingHv, &home_Losses::setD75LLwindingHv)
    .def_property("d_85_ll_winding_hv", &home_Losses::getD85LLwindingHv, &home_Losses::setD85LLwindingHv)

    .def_property("d_20_total_100_load", &home_Losses::getD20Total100Load, &home_Losses::setD20Total100Load)
    .def_property("d_55_total_100_load", &home_Losses::getD55Total100Load, &home_Losses::setD55Total100Load)
    .def_property("d_75_total_100_load", &home_Losses::getD75Total100Load, &home_Losses::setD75Total100Load)
    .def_property("d_85_total_100_load", &home_Losses::getD85Total100Load, &home_Losses::setD85Total100Load)

    .def_property("d_20_total_lr25", &home_Losses::getD20TotalLR25, &home_Losses::setD20TotalLR25)
    .def_property("d_55_total_lr25", &home_Losses::getD55TotalLR25, &home_Losses::setD55TotalLR25)
    .def_property("d_75_total_lr25", &home_Losses::getD75TotalLR25, &home_Losses::setD75TotalLR25)
    .def_property("d_85_total_lr25", &home_Losses::getD85TotalLR25, &home_Losses::setD85TotalLR25)
    .def_property("d_20_total_lr50", &home_Losses::getD20TotalLR50, &home_Losses::setD20TotalLR50)
    .def_property("d_55_total_lr50", &home_Losses::getD55TotalLR50, &home_Losses::setD55TotalLR50)
    .def_property("d_75_total_lr50", &home_Losses::getD75TotalLR50, &home_Losses::setD75TotalLR50)
    .def_property("d_85_total_lr50", &home_Losses::getD85TotalLR50, &home_Losses::setD85TotalLR50)
    .def_property("d_20_total_lr75", &home_Losses::getD20TotalLR75, &home_Losses::setD20TotalLR75)
    .def_property("d_55_total_lr75", &home_Losses::getD55TotalLR75, &home_Losses::setD55TotalLR75)
    .def_property("d_75_total_lr75", &home_Losses::getD75TotalLR75, &home_Losses::setD75TotalLR75)
    .def_property("d_85_total_lr75", &home_Losses::getD85TotalLR75, &home_Losses::setD85TotalLR75)
    .def_property("d_20_total_lr125", &home_Losses::getD20TotalLR125, &home_Losses::setD20TotalLR125)
    .def_property("d_55_total_lr125", &home_Losses::getD55TotalLR125, &home_Losses::setD55TotalLR125)
    .def_property("d_75_total_lr125", &home_Losses::getD75TotalLR125, &home_Losses::setD75TotalLR125)
    .def_property("d_85_total_lr125", &home_Losses::getD85TotalLR125, &home_Losses::setD85TotalLR125)
    .def_property("d_20_total_lr133", &home_Losses::getD20TotalLR133, &home_Losses::setD20TotalLR133)
    .def_property("d_55_total_lr133", &home_Losses::getD55TotalLR133, &home_Losses::setD55TotalLR133)
    .def_property("d_75_total_lr133", &home_Losses::getD75TotalLR133, &home_Losses::setD75TotalLR133)
    .def_property("d_85_total_lr133", &home_Losses::getD85TotalLR133, &home_Losses::setD85TotalLR133)

    .def_property("error_name", &home_Losses::getSErrorName, &home_Losses::setSErrorName)
    .def_property("error_enum", &home_Losses::getTErrorEnum, &home_Losses::setTErrorEnum)
    .def_property("error_messages", &home_Losses::getSErrorMessages, &home_Losses::setSErrorMessages);
}