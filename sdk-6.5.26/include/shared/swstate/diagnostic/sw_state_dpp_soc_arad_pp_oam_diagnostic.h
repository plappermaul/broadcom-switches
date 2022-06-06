/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_SUPPORT
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_dump_cb)(
    int unit, int pp_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_drop_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_drop_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_dump */
typedef int (*sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_dump_cb)(
    int unit, int pp_idx_0, int oam_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_drop_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_drop_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_dump_cb dump;
} sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_dump_cb dump;
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_diagnostic_cbs_t trap_code_recycle;
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_diagnostic_cbs_t trap_code_snoop;
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_diagnostic_cbs_t trap_code_frwrd;
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_diagnostic_cbs_t trap_code_drop;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_diagnostic_cbs_t trap_code_trap_to_cpu_level;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_diagnostic_cbs_t trap_code_trap_to_cpu_passive;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_diagnostic_cbs_t mirror_profile_snoop_to_cpu;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_diagnostic_cbs_t mirror_profile_recycle;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_diagnostic_cbs_t mirror_profile_err_level;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_diagnostic_cbs_t mirror_profile_err_passive;
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_diagnostic_cbs_t tcam_last_entry_id;
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_diagnostic_cbs_t lb_trap_used_cnt;
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_diagnostic_cbs_t tst_trap_used_cnt;
    sw_state_dpp_soc_arad_pp_oam_lb_tst_up_trap_used_cnt_diagnostic_cbs_t lb_tst_up_trap_used_cnt;
} sw_state_dpp_soc_arad_pp_oam_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_pp_oam_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_SUPPORT */

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_DIAGNOSTIC_H_ */