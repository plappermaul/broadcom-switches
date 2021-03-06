/********************************************************************************************
 ********************************************************************************************
 *                                                                                          *
 *  Revision      :  $Id: falcon_api_uc_vars_rdwr_defns.h 1187 2015-10-19 20:35:15Z kirand $ *
 *                                                                                          *
 *  Description   :  Read/Write macros to Falcon IP's uC RAM's User variables               *
 *                                                                                          *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.                                                                    *
 *  No portions of this material may be reproduced in any form without                      *
 *  the written permission of:                                                              *
 *      Broadcom Corporation                                                                *
 *      5300 California Avenue                                                              *
 *      Irvine, CA  92617                                                                   *
 *                                                                                          *
 *  All information contained in this document is Broadcom Corporation                      *
 *  company private proprietary, and trade secret.                                          *
 */

/** @file falcon_api_uc_vars_rdwr_defns.h
 * Read/Write macros to Falcon IP's uC RAM's User variables
 */

/* THIS FILE IS GENERATED USING AN AUTOMATED SCRIPT... PLEASE DO NOT EDIT THIS FILE DIRECTLY !!! */


#ifndef FALCON_API_UC_VARS_RDWR_DEFNS_H
#define FALCON_API_UC_VARS_RDWR_DEFNS_H

/************************************/
/*  Serdes IP RAM access functions  */
/************************************/

/* RAM access functions for lane_var_struct variables */
#define falcon2_dino_rdv_config_word()                                            falcon2_dino_rdwl_uc_var(__ERR,0x0)
#define falcon2_dino_wrv_config_word(wr_val)                                      falcon2_dino_wrwl_uc_var(0x0,wr_val)
#define falcon2_dino_rdv_usr_ctrl_retune_after_restart()                          falcon2_dino_rdbl_uc_var(__ERR,0x2)
#define falcon2_dino_wrv_usr_ctrl_retune_after_restart(wr_val)                    falcon2_dino_wrbl_uc_var(0x2,wr_val)
#define falcon2_dino_rdv_usr_ctrl_clk90_offset_adjust()                           falcon2_dino_rdbls_uc_var(__ERR,0x3)
#define falcon2_dino_wrv_usr_ctrl_clk90_offset_adjust(wr_val)                     falcon2_dino_wrbls_uc_var(0x3,wr_val)
#define falcon2_dino_rdv_usr_ctrl_clk90_offset_override()                         falcon2_dino_rdbl_uc_var(__ERR,0x4)
#define falcon2_dino_wrv_usr_ctrl_clk90_offset_override(wr_val)                   falcon2_dino_wrbl_uc_var(0x4,wr_val)
#define falcon2_dino_rdv_usr_ctrl_lane_event_log_level()                          falcon2_dino_rdbl_uc_var(__ERR,0x5)
#define falcon2_dino_wrv_usr_ctrl_lane_event_log_level(wr_val)                    falcon2_dino_wrbl_uc_var(0x5,wr_val)
#define falcon2_dino_rdv_usr_ctrl_vertical_threshold_adjust()                     falcon2_dino_rdbls_uc_var(__ERR,0x6)
#define falcon2_dino_wrv_usr_ctrl_vertical_threshold_adjust(wr_val)               falcon2_dino_wrbls_uc_var(0x6,wr_val)
#define falcon2_dino_rdv_usr_ctrl_cl93n72_frc_byte()                              falcon2_dino_rdbl_uc_var(__ERR,0x7)
#define falcon2_dino_wrv_usr_ctrl_cl93n72_frc_byte(wr_val)                        falcon2_dino_wrbl_uc_var(0x7,wr_val)
#define falcon2_dino_rdv_usr_ctrl_disable_startup_functions_word()                falcon2_dino_rdwl_uc_var(__ERR,0x8)
#define falcon2_dino_wrv_usr_ctrl_disable_startup_functions_word(wr_val)          falcon2_dino_wrwl_uc_var(0x8,wr_val)
#define falcon2_dino_rdv_usr_ctrl_disable_steady_state_functions_word()           falcon2_dino_rdwl_uc_var(__ERR,0xa)
#define falcon2_dino_wrv_usr_ctrl_disable_steady_state_functions_word(wr_val)     falcon2_dino_wrwl_uc_var(0xa,wr_val)
#define falcon2_dino_rdv_usr_ctrl_disable_startup_dfe_functions_byte()            falcon2_dino_rdbl_uc_var(__ERR,0xc)
#define falcon2_dino_wrv_usr_ctrl_disable_startup_dfe_functions_byte(wr_val)      falcon2_dino_wrbl_uc_var(0xc,wr_val)
#define falcon2_dino_rdv_usr_ctrl_disable_steady_state_dfe_functions_byte()       falcon2_dino_rdbl_uc_var(__ERR,0xd)
#define falcon2_dino_wrv_usr_ctrl_disable_steady_state_dfe_functions_byte(wr_val) falcon2_dino_wrbl_uc_var(0xd,wr_val)
#define falcon2_dino_rdv_usr_sts_restart_counter()                                falcon2_dino_rdbl_uc_var(__ERR,0xe)
#define falcon2_dino_wrv_usr_sts_restart_counter(wr_val)                          falcon2_dino_wrbl_uc_var(0xe,wr_val)
#define falcon2_dino_rdv_usr_sts_reset_counter()                                  falcon2_dino_rdbl_uc_var(__ERR,0xf)
#define falcon2_dino_wrv_usr_sts_reset_counter(wr_val)                            falcon2_dino_wrbl_uc_var(0xf,wr_val)
#define falcon2_dino_rdv_usr_sts_pmd_lock_counter()                               falcon2_dino_rdbl_uc_var(__ERR,0x10)
#define falcon2_dino_wrv_usr_sts_pmd_lock_counter(wr_val)                         falcon2_dino_wrbl_uc_var(0x10,wr_val)
#define falcon2_dino_rdv_usr_sts_heye_left()                                      falcon2_dino_rdbl_uc_var(__ERR,0x11)
#define falcon2_dino_wrv_usr_sts_heye_left(wr_val)                                falcon2_dino_wrbl_uc_var(0x11,wr_val)
#define falcon2_dino_rdv_usr_sts_heye_right()                                     falcon2_dino_rdbl_uc_var(__ERR,0x12)
#define falcon2_dino_wrv_usr_sts_heye_right(wr_val)                               falcon2_dino_wrbl_uc_var(0x12,wr_val)
#define falcon2_dino_rdv_usr_sts_veye_upper()                                     falcon2_dino_rdbl_uc_var(__ERR,0x13)
#define falcon2_dino_wrv_usr_sts_veye_upper(wr_val)                               falcon2_dino_wrbl_uc_var(0x13,wr_val)
#define falcon2_dino_rdv_usr_sts_veye_lower()                                     falcon2_dino_rdbl_uc_var(__ERR,0x14)
#define falcon2_dino_wrv_usr_sts_veye_lower(wr_val)                               falcon2_dino_wrbl_uc_var(0x14,wr_val)
#define falcon2_dino_rdv_usr_sts_micro_stopped()                                  falcon2_dino_rdbl_uc_var(__ERR,0x15)
#define falcon2_dino_wrv_usr_sts_micro_stopped(wr_val)                            falcon2_dino_wrbl_uc_var(0x15,wr_val)
#define falcon2_dino_rdv_usr_sts_link_time()                                      falcon2_dino_rdwl_uc_var(__ERR,0x16)
#define falcon2_dino_wrv_usr_sts_link_time(wr_val)                                falcon2_dino_wrwl_uc_var(0x16,wr_val)
#define falcon2_dino_rdv_usr_diag_status()                                        falcon2_dino_rdwl_uc_var(__ERR,0x18)
#define falcon2_dino_wrv_usr_diag_status(wr_val)                                  falcon2_dino_wrwl_uc_var(0x18,wr_val)
#define falcon2_dino_rdv_usr_diag_rd_ptr()                                        falcon2_dino_rdbl_uc_var(__ERR,0x1a)
#define falcon2_dino_wrv_usr_diag_rd_ptr(wr_val)                                  falcon2_dino_wrbl_uc_var(0x1a,wr_val)
#define falcon2_dino_rdv_usr_diag_mode()                                          falcon2_dino_rdbl_uc_var(__ERR,0x1b)
#define falcon2_dino_wrv_usr_diag_mode(wr_val)                                    falcon2_dino_wrbl_uc_var(0x1b,wr_val)
#define falcon2_dino_rdv_usr_main_tap_est()                                       falcon2_dino_rdwls_uc_var(__ERR,0x1c)
#define falcon2_dino_wrv_usr_main_tap_est(wr_val)                                 falcon2_dino_wrwls_uc_var(0x1c,wr_val)
#define falcon2_dino_rdv_usr_sts_phase_hoffset()                                  falcon2_dino_rdbls_uc_var(__ERR,0x1e)
#define falcon2_dino_wrv_usr_sts_phase_hoffset(wr_val)                            falcon2_dino_wrbls_uc_var(0x1e,wr_val)

/* RAM afalcon2_dino_ccess functions for core_var_struct variables */
#define falcon2_dino_rdcv_config_word()                                           falcon2_dino_rdwc_uc_var(__ERR,0x0)
#define falcon2_dino_wrcv_config_word(wr_val)                                     falcon2_dino_wrwc_uc_var(0x0,wr_val)
#define falcon2_dino_rdcv_temp_frc_val()                                          falcon2_dino_rdwc_uc_var(__ERR,0x2)
#define falcon2_dino_wrcv_temp_frc_val(wr_val)                                    falcon2_dino_wrwc_uc_var(0x2,wr_val)
#define falcon2_dino_rdcv_common_ucode_version()                                  falcon2_dino_rdwc_uc_var(__ERR,0x4)
#define falcon2_dino_wrcv_common_ucode_version(wr_val)                            falcon2_dino_wrwc_uc_var(0x4,wr_val)
#define falcon2_dino_rdcv_avg_tmon_reg13bit()                                     falcon2_dino_rdwc_uc_var(__ERR,0x6)
#define falcon2_dino_wrcv_avg_tmon_reg13bit(wr_val)                               falcon2_dino_wrwc_uc_var(0x6,wr_val)
#define falcon2_dino_rdcv_trace_mem_rd_idx()                                      falcon2_dino_rdwc_uc_var(__ERR,0x8)
#define falcon2_dino_wrcv_trace_mem_rd_idx(wr_val)                                falcon2_dino_wrwc_uc_var(0x8,wr_val)
#define falcon2_dino_rdcv_trace_mem_wr_idx()                                      falcon2_dino_rdwc_uc_var(__ERR,0xa)
#define falcon2_dino_wrcv_trace_mem_wr_idx(wr_val)                                falcon2_dino_wrwc_uc_var(0xa,wr_val)
#define falcon2_dino_rdcv_temp_idx()                                              falcon2_dino_rdbc_uc_var(__ERR,0xc)
#define falcon2_dino_wrcv_temp_idx(wr_val)                                        falcon2_dino_wrbc_uc_var(0xc,wr_val)
#define falcon2_dino_rdcv_usr_ctrl_core_event_log_level()                         falcon2_dino_rdbc_uc_var(__ERR,0xd)
#define falcon2_dino_wrcv_usr_ctrl_core_event_log_level(wr_val)                   falcon2_dino_wrbc_uc_var(0xd,wr_val)
#define falcon2_dino_rdcv_common_ucode_minor_version()                            falcon2_dino_rdbc_uc_var(__ERR,0xe)
#define falcon2_dino_wrcv_common_ucode_minor_version(wr_val)                      falcon2_dino_wrbc_uc_var(0xe,wr_val)
#define falcon2_dino_rdcv_afe_hardware_version()                                  falcon2_dino_rdbc_uc_var(__ERR,0xf)
#define falcon2_dino_wrcv_afe_hardware_version(wr_val)                            falcon2_dino_wrbc_uc_var(0xf,wr_val)
#define falcon2_dino_rdcv_status_byte()                                           falcon2_dino_rdbc_uc_var(__ERR,0x10)
#define falcon2_dino_wrcv_status_byte(wr_val)                                     falcon2_dino_wrbc_uc_var(0x10,wr_val)
#define falcon2_dino_rdcv_diag_max_time_control()                                 falcon2_dino_rdbc_uc_var(__ERR,0x11)
#define falcon2_dino_wrcv_diag_max_time_control(wr_val)                           falcon2_dino_wrbc_uc_var(0x11,wr_val)
#define falcon2_dino_rdcv_diag_max_err_control()                                  falcon2_dino_rdbc_uc_var(__ERR,0x12)
#define falcon2_dino_wrcv_diag_max_err_control(wr_val)                            falcon2_dino_wrbc_uc_var(0x12,wr_val)
#define falcon2_dino_rdcv_misc_ctrl_byte()                                        falcon2_dino_rdbc_uc_var(__ERR,0x13)
#define falcon2_dino_wrcv_misc_ctrl_byte(wr_val)                                  falcon2_dino_wrbc_uc_var(0x13,wr_val)
#define falcon2_dino_rdcv_config_pll1_word()                                      falcon2_dino_rdwc_uc_var(__ERR,0x14)
#define falcon2_dino_wrcv_config_pll1_word(wr_val)                                falcon2_dino_wrwc_uc_var(0x14,wr_val)

#endif
