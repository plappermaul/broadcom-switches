/*
 * 
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * Purpose:    Implements application interrupt lists for JERICHO.
 */

/*************
 * INCLUDES  *
 *************/
#include <shared/bsl.h>

#include <soc/dpp/JER/jer_intr.h>

#include <bcm/error.h>
#include <bcm/debug.h>
#include <bcm_int/common/debug.h>

#include <appl/diag/system.h>
#include <appl/dcmn/interrupts/dcmn_intr.h>

/*************
 * DEFINES   *
 *************/
#ifdef _ERR_MSG_MODULE_NAME
#error "_ERR_MSG_MODULE_NAME redefined"
#endif
#define _ERR_MSG_MODULE_NAME BSL_APPL_INTR

/*************
 * TYPE DEFS *
 *************/
jer_interrupt_type jer_int_disable_on_init[] = {
    INVALIDr
};



jer_interrupt_type jer_int_active_on_init[] = {
/*
 * List of interrupts that are vectors pointing to other interrupt registers
 */
    JER_INT_DRCE_ERROR_ECC,
    JER_INT_DRCD_ERROR_ECC,
    JER_INT_MMU_ERROR_ECC,
    JER_INT_IRE_ERROR_ECC,
    JER_INT_IRE_EXTERNAL_IF_ERROR,
    JER_INT_NBIH_ERROR_ECC,
    JER_INT_NBIH_NBI_PKT_DROP_COUNTERS_0_75P_INTERRUPT,
    JER_INT_NBIH_LINK_STATUS_CHANGE_INT,
    JER_INT_ILKN_PML_ERROR_ECC,
    JER_INT_OLP_ERROR_ECC,
    JER_INT_IPS_ERROR_ECC,
    JER_INT_IHP_ERROR_ECC,
    JER_INT_EDB_ERROR_ECC,
    JER_INT_EDB_ESEM_INTERRUPT_ONE,
    JER_INT_EDB_GLEM_INTERRUPT_ONE,
    JER_INT_DRCC_ERROR_ECC,
    JER_INT_IDR_ERROR_ECC,
    JER_INT_IDR_ERROR_REASSEMBLY,
    JER_INT_DRCH_ERROR_ECC,
    JER_INT_EPNI_ERROR_ECC,
    JER_INT_EPNI_PP_INT_VEC,
    JER_INT_DRCG_ERROR_ECC,
    JER_INT_FDR_INT_REG_1,
    JER_INT_FDR_INT_REG_2,
    JER_INT_FDR_INT_REG_3,
    JER_INT_FDR_INT_REG_4,
    JER_INT_FMAC_INT_REG_1,
    JER_INT_FMAC_INT_REG_2,
    JER_INT_FMAC_INT_REG_3,
    JER_INT_FMAC_INT_REG_4,
    JER_INT_FMAC_INT_REG_5,
    JER_INT_FMAC_INT_REG_6,
    JER_INT_FMAC_INT_REG_7,
    JER_INT_FMAC_INT_REG_8,
    JER_INT_CRPS_ERROR_ECC,
    JER_INT_CRPS_ENGINE,
    JER_INT_CRPS_SRC_INVLID_ACCESS,
    JER_INT_CRPS_OFFSET_WAS_FILTERED,
    JER_INT_PPDB_A_ERROR_ECC,
    JER_INT_PPDB_A_OEMA_INTERRUPT_ONE,
    JER_INT_PPDB_A_OEMB_INTERRUPT_ONE,
    JER_INT_CFC_ERROR_ECC,
    JER_INT_EGQ_ERROR_ECC,
    JER_INT_EGQ_ERPP_DISCARD_INT_VEC,
    JER_INT_EGQ_PKT_REAS_INT_VEC,
    JER_INT_EGQ_ERPP_DISCARD_INT_VEC_2,
    JER_INT_IRR_ERROR_ECC,
    JER_INT_MRPS_ERROR_ECC,
    JER_INT_ILKN_PMH_ERROR_ECC,
    JER_INT_DRCA_ERROR_ECC,
    JER_INT_CGM_CGM_REP_AROUND_INT_VEC,
    JER_INT_OAMP_ERROR_ECC,
    JER_INT_OAMP_RMAPEM_INTERRUPT_ONE,
    JER_INT_IHB_ERROR_ECC,
    JER_INT_IHB_ISEM_INTERRUPT_ONE,
    JER_INT_DRCF_ERROR_ECC,
    JER_INT_SCH_ERROR_ECC,
    JER_INT_IPST_ERROR_ECC,
    JER_INT_IQM_ERROR_ECC,
    JER_INT_NBIL_ERROR_ECC,
    JER_INT_NBIL_NBI_PKT_DROP_COUNTERS_0_75P_INTERRUPT,
    JER_INT_NBIL_LINK_STATUS_CHANGE_INT,
    JER_INT_DRCB_ERROR_ECC,
    JER_INT_IQMT_ERROR_ECC,
    JER_INT_PPDB_B_ERROR_ECC,
    JER_INT_PPDB_B_LARGE_EM_INTERRUPT_ONE,
    JER_INT_PPDB_B_LARGE_EM_INTERRUPT_TWO,
    JER_INT_PPDB_A_TCAM_PROTECTION_ERROR,
    JER_INT_ECI_ERROR_ECC,
    JER_INT_FCR_ERROR_ECC,
    JER_INT_FDA_ERROR_ECC,
    JER_INT_FDR_ERROR_ECC,
    JER_INT_FDT_ERROR_ECC,
    JER_INT_FMAC_ERROR_ECC,
    JER_INT_FSRD_ERROR_ECC,
    JER_INT_FSRD_INT_REG_0,
    JER_INT_FSRD_INT_REG_1,
    JER_INT_FSRD_INT_REG_2,
    JER_INT_IPT_ERROR_ECC,
    JER_INT_MTRPS_EM_ERROR_ECC,
    JER_INT_OCB_ERROR_ECC,
    JER_INT_RTP_ERROR_ECC,

/*
 * Parity & ECC error interrupts
 */
    JER_INT_DRCE_ECC_PARITY_ERR_INT,
    JER_INT_DRCE_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCE_ECC_ECC_2B_ERR_INT,
    JER_INT_OCB_CRC_ERR_INT,
    JER_INT_OCB_ECC_PARITY_ERR_INT,
    JER_INT_OCB_ECC_ECC_1B_ERR_INT,
    JER_INT_OCB_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCD_ECC_PARITY_ERR_INT,
    JER_INT_DRCD_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCD_ECC_ECC_2B_ERR_INT,
    JER_INT_MMU_DRAM_OPP_CRC_ERR_INT,
    JER_INT_MMU_ECC_PARITY_ERR_INT,
    JER_INT_MMU_ECC_ECC_1B_ERR_INT,
    JER_INT_MMU_ECC_ECC_2B_ERR_INT,
    JER_INT_IRE_ECC_PARITY_ERR_INT,
    JER_INT_IRE_ECC_ECC_1B_ERR_INT,
    JER_INT_IRE_ECC_ECC_2B_ERR_INT,
    JER_INT_NBIH_ECC_PARITY_ERR_INT,
    JER_INT_NBIH_ECC_ECC_1B_ERR_INT,
    JER_INT_NBIH_ECC_ECC_2B_ERR_INT,
    JER_INT_ILKN_PML_ECC_PARITY_ERR_INT,
    JER_INT_ILKN_PML_ECC_ECC_1B_ERR_INT,
    JER_INT_ILKN_PML_ECC_ECC_2B_ERR_INT,
    JER_INT_OLP_ECC_PARITY_ERR_INT,
    JER_INT_OLP_ECC_ECC_1B_ERR_INT,
    JER_INT_OLP_ECC_ECC_2B_ERR_INT,
    JER_INT_IPS_CR_FLW_ID_ERR_INT,
    JER_INT_IPS_ECC_PARITY_ERR_INT,
    JER_INT_IPS_ECC_ECC_1B_ERR_INT,
    JER_INT_IPS_ECC_ECC_2B_ERR_INT,
    JER_INT_IHP_ECC_PARITY_ERR_INT,
    JER_INT_IHP_ECC_ECC_1B_ERR_INT,
    JER_INT_IHP_ECC_ECC_2B_ERR_INT,
    JER_INT_FDA_ECC_PARITY_ERR_INT,
    JER_INT_FDA_ECC_ECC_1B_ERR_INT,
    JER_INT_FDA_ECC_ECC_2B_ERR_INT,
    JER_INT_EDB_ECC_PARITY_ERR_INT,
    JER_INT_EDB_ECC_ECC_1B_ERR_INT,
    JER_INT_EDB_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCC_ECC_PARITY_ERR_INT,
    JER_INT_DRCC_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCC_ECC_ECC_2B_ERR_INT,
    JER_INT_IDR_MMU_ECC_1B_ERR_INT,
    JER_INT_IDR_MMU_ECC_2B_ERR_INT,
    JER_INT_IDR_ECC_PARITY_ERR_INT,
    JER_INT_IDR_ECC_ECC_1B_ERR_INT,
    JER_INT_IDR_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCH_ECC_PARITY_ERR_INT,
    JER_INT_DRCH_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCH_ECC_ECC_2B_ERR_INT,
    JER_INT_EPNI_ECC_PARITY_ERR_INT,
    JER_INT_EPNI_ECC_ECC_1B_ERR_INT,
    JER_INT_EPNI_ECC_ECC_2B_ERR_INT,
    JER_INT_IPT_ECC_PARITY_ERR_INT,
    JER_INT_IPT_ECC_ECC_1B_ERR_INT,
    JER_INT_IPT_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCG_ECC_PARITY_ERR_INT,
    JER_INT_DRCG_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCG_ECC_ECC_2B_ERR_INT,
    JER_INT_FDR_ECC_PARITY_ERR_INT,
    JER_INT_FDR_ECC_ECC_1B_ERR_INT,
    JER_INT_FDR_ECC_ECC_2B_ERR_INT,
    JER_INT_FDR_FDR_4_P_1_ECC_1B_ERR_INT,
    JER_INT_FDR_FDR_4_P_1_ECC_2B_ERR_INT,
    JER_INT_FDR_FDR_4_P_2_ECC_1B_ERR_INT,
    JER_INT_FDR_FDR_4_P_2_ECC_2B_ERR_INT,
    JER_INT_FDR_FDR_4_P_3_ECC_1B_ERR_INT,
    JER_INT_FDR_FDR_4_P_3_ECC_2B_ERR_INT,
    JER_INT_FMAC_ECC_PARITY_ERR_INT,
    JER_INT_FMAC_ECC_ECC_1B_ERR_INT,
    JER_INT_FMAC_ECC_ECC_2B_ERR_INT,
    JER_INT_CRPS_ECC_PARITY_ERR_INT,
    JER_INT_CRPS_ECC_ECC_1B_ERR_INT,
    JER_INT_CRPS_ECC_ECC_2B_ERR_INT,
    JER_INT_FDT_ECC_PARITY_ERR_INT,
    JER_INT_FDT_ECC_ECC_1B_ERR_INT,
    JER_INT_FDT_ECC_ECC_2B_ERR_INT,
    JER_INT_PPDB_A_ECC_PARITY_ERR_INT,
    JER_INT_PPDB_A_ECC_ECC_1B_ERR_INT,
    JER_INT_PPDB_A_ECC_ECC_2B_ERR_INT,
    JER_INT_CFC_ECC_PARITY_ERR_INT,
    JER_INT_CFC_ECC_ECC_1B_ERR_INT,
    JER_INT_CFC_ECC_ECC_2B_ERR_INT,
    JER_INT_EGQ_ECC_PARITY_ERR_INT,
    JER_INT_EGQ_ECC_ECC_1B_ERR_INT,
    JER_INT_EGQ_ECC_ECC_2B_ERR_INT,
    JER_INT_RTP_ECC_PARITY_ERR_INT,
    JER_INT_RTP_ECC_ECC_1B_ERR_INT,
    JER_INT_RTP_ECC_ECC_2B_ERR_INT,
    JER_INT_IRR_ECC_PARITY_ERR_INT,
    JER_INT_IRR_ECC_ECC_1B_ERR_INT,
    JER_INT_IRR_ECC_ECC_2B_ERR_INT,
    JER_INT_MRPS_ECC_PARITY_ERR_INT,
    JER_INT_MRPS_ECC_ECC_1B_ERR_INT,
    JER_INT_MRPS_ECC_ECC_2B_ERR_INT,
    JER_INT_ILKN_PMH_ECC_PARITY_ERR_INT,
    JER_INT_ILKN_PMH_ECC_ECC_1B_ERR_INT,
    JER_INT_ILKN_PMH_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCA_ECC_PARITY_ERR_INT,
    JER_INT_DRCA_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCA_ECC_ECC_2B_ERR_INT,
    JER_INT_OAMP_ECC_PARITY_ERR_INT,
    JER_INT_OAMP_ECC_ECC_1B_ERR_INT,
    JER_INT_OAMP_ECC_ECC_2B_ERR_INT,
    JER_INT_IHB_ECC_PARITY_ERR_INT,
    JER_INT_IHB_ECC_ECC_1B_ERR_INT,
    JER_INT_IHB_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCF_ECC_PARITY_ERR_INT,
    JER_INT_DRCF_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCF_ECC_ECC_2B_ERR_INT,
    JER_INT_MTRPS_EM_ECC_PARITY_ERR_INT,
    JER_INT_MTRPS_EM_ECC_ECC_1B_ERR_INT,
    JER_INT_MTRPS_EM_ECC_ECC_2B_ERR_INT,
    JER_INT_FCR_ECC_PARITY_ERR_INT,
    JER_INT_FCR_ECC_ECC_1B_ERR_INT,
    JER_INT_FCR_ECC_ECC_2B_ERR_INT,
    JER_INT_SCH_ECC_PARITY_ERR_INT,
    JER_INT_SCH_ECC_ECC_1B_ERR_INT,
    JER_INT_SCH_ECC_ECC_2B_ERR_INT,
    JER_INT_IPST_ECC_PARITY_ERR_INT,
    JER_INT_IPST_ECC_ECC_1B_ERR_INT,
    JER_INT_IPST_ECC_ECC_2B_ERR_INT,
    JER_INT_IQM_ECC_PARITY_ERR_INT,
    JER_INT_IQM_ECC_ECC_1B_ERR_INT,
    JER_INT_IQM_ECC_ECC_2B_ERR_INT,
    JER_INT_NBIL_ECC_PARITY_ERR_INT,
    JER_INT_NBIL_ECC_ECC_1B_ERR_INT,
    JER_INT_NBIL_ECC_ECC_2B_ERR_INT,
    JER_INT_DRCB_ECC_PARITY_ERR_INT,
    JER_INT_DRCB_ECC_ECC_1B_ERR_INT,
    JER_INT_DRCB_ECC_ECC_2B_ERR_INT,
    JER_INT_IQMT_ECC_PARITY_ERR_INT,
    JER_INT_IQMT_ECC_ECC_1B_ERR_INT,
    JER_INT_IQMT_ECC_ECC_2B_ERR_INT,
    JER_INT_FSRD_ECC_PARITY_ERR_INT,
    JER_INT_FSRD_ECC_ECC_1B_ERR_INT,
    JER_INT_FSRD_ECC_ECC_2B_ERR_INT,
    JER_INT_ECI_ECC_PARITY_ERR_INT,
    JER_INT_ECI_ECC_ECC_1B_ERR_INT,
    JER_INT_ECI_ECC_ECC_2B_ERR_INT,
    JER_INT_PPDB_B_ECC_PARITY_ERR_INT,
    JER_INT_PPDB_B_ECC_ECC_1B_ERR_INT,
    JER_INT_PPDB_B_ECC_ECC_2B_ERR_INT,
/* 
 * DRC interrupts
 */ 
    JER_INT_DRCA_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCB_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCC_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCD_PHY_CDR_ABOVE_TH,
    JER_INT_DRCE_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCF_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCG_PHY_CDR_ABOVE_TH, 
    JER_INT_DRCH_PHY_CDR_ABOVE_TH,
    JER_INT_DRCA_DRAM_RD_CRC_ERR,
    JER_INT_DRCB_DRAM_RD_CRC_ERR,
    JER_INT_DRCC_DRAM_RD_CRC_ERR,
    JER_INT_DRCD_DRAM_RD_CRC_ERR,
    JER_INT_DRCE_DRAM_RD_CRC_ERR,
    JER_INT_DRCF_DRAM_RD_CRC_ERR,
    JER_INT_DRCG_DRAM_RD_CRC_ERR,
    JER_INT_DRCH_DRAM_RD_CRC_ERR,
    JER_INT_DRCA_DRAM_WR_CRC_ERR,
    JER_INT_DRCB_DRAM_WR_CRC_ERR,
    JER_INT_DRCC_DRAM_WR_CRC_ERR,
    JER_INT_DRCD_DRAM_WR_CRC_ERR,
    JER_INT_DRCE_DRAM_WR_CRC_ERR,
    JER_INT_DRCF_DRAM_WR_CRC_ERR,
    JER_INT_DRCG_DRAM_WR_CRC_ERR,
    JER_INT_DRCH_DRAM_WR_CRC_ERR,
    INVALIDr
};

jer_interrupt_type jer_int_disable_print_on_init[] = {
    INVALIDr
};

/*************
 * FUNCTIONS *
 *************/
int
jer_interrupt_cmn_param_init(int unit, intr_common_params_t* common_params)
{
    BCMDNX_INIT_FUNC_DEFS;

    BCMDNX_NULL_CHECK(common_params);

    common_params->int_disable_on_init = jer_int_disable_on_init;
    common_params->int_disable_print_on_init = jer_int_disable_print_on_init;
    common_params->int_active_on_init = jer_int_active_on_init;

exit:
    BCMDNX_FUNC_RETURN;
}

#undef _ERR_MSG_MODULE_NAME

