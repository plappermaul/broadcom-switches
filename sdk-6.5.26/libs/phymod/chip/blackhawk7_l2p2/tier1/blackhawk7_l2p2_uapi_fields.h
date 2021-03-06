
/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2021 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 *                                                                                 *
 **********************************************************************************/

/***********************************************************************************
 ***********************************************************************************
 *                                                                                 *
 *  Revision      :                                                            *
 *                                                                                 *
 *  Description   :  This file contains Unified API jump tables                    *
 *                                                                                 *
 ***********************************************************************************
 **********************************************************************************/

/* THIS FILE IS GENERATED USING AN AUTOMATED SCRIPT.DO NOT EDIT THIS FILE DIRECTLY! */

#ifdef __cplusplus
extern "C" { /* __cplusplus */
#endif

#ifndef BLACKHAWK7_L2P2_UAPI_FIELDS_H
#define BLACKHAWK7_L2P2_UAPI_FIELDS_H

#include "common/srds_api_err_code.h"
#include "common/srds_api_types.h"

err_code_t blackhawk7_l2p2_wr_uapi_ams_rx_ibias_pibuf(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ams_rx_ibias_pibuf(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ams_rx_ibias_pibuf(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_dac_test_mem_tm(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_dac_test_mem_tm(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_dac_test_mem_tm(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_readaddr(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_readaddr(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_readaddr(srds_access_t *sa__, uint16_t reg_value);
uint16_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_readdata_hi(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_readdata_hi(srds_access_t *sa__, uint16_t reg_value);
uint16_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_readdata_med(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_readdata_med(srds_access_t *sa__, uint16_t reg_value);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_readdata_lo(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_readdata_lo(srds_access_t *sa__, uint16_t reg_value);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_readback_available(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_readback_available(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_wclk_switch(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_wclk_switch(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_wclk_switch(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_reset_mem_data(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_reset_mem_data(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_reset_mem_data(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_counter_more_than_15_en(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_counter_more_than_15_en(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_counter_more_than_15_en(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_en(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_en(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_en(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_tlb_err_analyze_counter_en(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_rd_uapi_tlb_err_analyze_counter_en(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_tlb_err_analyze_counter_en(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ams_rx_eq2_ctrl4(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ams_rx_eq2_ctrl4(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ams_rx_eq2_ctrl4(srds_access_t *sa__, uint16_t reg_value);
uint8_t blackhawk7_l2p2_rd_uapi_ring_done(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_done(srds_access_t *sa__, uint16_t reg_value);
uint8_t blackhawk7_l2p2_rd_uapi_ring_count_overflow(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_count_overflow(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_threshold_msb(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ring_threshold_msb(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_threshold_msb(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_sel(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ring_sel(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_sel(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_resetb_frc(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ring_resetb_frc(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_resetb_frc(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_resetb_frc_val(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ring_resetb_frc_val(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_resetb_frc_val(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_en(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_ring_en(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_en(srds_access_t *sa__, uint16_t reg_value);
uint16_t blackhawk7_l2p2_rd_uapi_ring_count(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_count(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_ring_threshold_lsb(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_rd_uapi_ring_threshold_lsb(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_ring_threshold_lsb(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_wr_uapi_prbs_chk_new_timer_mode(srds_access_t *sa__, uint8_t wr_val);
uint8_t blackhawk7_l2p2_rd_uapi_prbs_chk_new_timer_mode(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_prbs_chk_new_timer_mode(srds_access_t *sa__, uint16_t reg_value);
uint8_t blackhawk7_l2p2_rd_uapi_prbs_chk_timer_done_lh(srds_access_t *sa__, err_code_t *err_code_p);
uint16_t blackhawk7_l2p2_ex_uapi_prbs_chk_timer_done_lh(srds_access_t *sa__, uint16_t reg_value);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DAC_TEST_COM_DAC_TEST_MEM_TEST(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DAC_TEST_COM_DAC_TEST_MEM_TEST(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_TLB_ERR_ANALYZE_CONFIG_2(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_TLB_ERR_ANALYZE_CONFIG_2(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_2(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_2(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_1(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_1(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_0(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_TLB_ERR_ANALYZE_STATUS_0(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_B_TLB_ERR_ANALYZE_CONFIG_0(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_B_TLB_ERR_ANALYZE_CONFIG_0(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_B_TLB_ERR_ANALYZE_CONFIG_1(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_B_TLB_ERR_ANALYZE_CONFIG_1(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_DIG_COM_RING_OSC_STATUS_0(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_DIG_COM_RING_OSC_STATUS_0(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_0(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_0(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_1(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_1(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_2(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_CONFIG_2(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_2(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_2(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_1(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_1(srds_access_t *sa__, err_code_t *err_code_p);
err_code_t blackhawk7_l2p2_reg_wr_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_0(srds_access_t *sa__, uint16_t wr_val);
uint16_t blackhawk7_l2p2_reg_rd_uapi_TLB_ERR_AGGR_COM_TLB_ERR_ANALYZE_STATUS_0(srds_access_t *sa__, err_code_t *err_code_p);

#endif /* BLACKHAWK7_L2P2_UAPI_FIELDS_H */
#ifdef __cplusplus
}
#endif /* __cplusplus */
