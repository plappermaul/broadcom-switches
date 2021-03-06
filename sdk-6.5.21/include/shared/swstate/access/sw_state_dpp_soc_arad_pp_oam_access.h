/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_set_cb)(
    int unit, CONST ARAD_PP_OAM_SW_STATE *oam);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_get_cb)(
    int unit, ARAD_PP_OAM_SW_STATE *oam);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_alloc */
typedef int (*sw_state_dpp_soc_arad_pp_oam_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_is_allocated */
typedef int (*sw_state_dpp_soc_arad_pp_oam_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_free */
typedef int (*sw_state_dpp_soc_arad_pp_oam_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_set_cb)(
    int unit, uint32 trap_code_recycle);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_get_cb)(
    int unit, uint32 *trap_code_recycle);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_set_cb)(
    int unit, uint32 trap_code_snoop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_get_cb)(
    int unit, uint32 *trap_code_snoop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_set_cb)(
    int unit, uint32 trap_code_frwrd);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_get_cb)(
    int unit, uint32 *trap_code_frwrd);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_drop_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_drop_set_cb)(
    int unit, uint32 trap_code_drop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_drop_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_drop_get_cb)(
    int unit, uint32 *trap_code_drop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_drop_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_drop_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_set_cb)(
    int unit, uint32 trap_code_trap_to_cpu_level);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_get_cb)(
    int unit, uint32 *trap_code_trap_to_cpu_level);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_set_cb)(
    int unit, uint32 trap_code_trap_to_cpu_passive);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_get_cb)(
    int unit, uint32 *trap_code_trap_to_cpu_passive);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_set_cb)(
    int unit, uint32 mirror_profile_snoop_to_cpu);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_get_cb)(
    int unit, uint32 *mirror_profile_snoop_to_cpu);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_set_cb)(
    int unit, uint32 mirror_profile_recycle);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_get_cb)(
    int unit, uint32 *mirror_profile_recycle);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_set_cb)(
    int unit, uint32 mirror_profile_err_level);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_get_cb)(
    int unit, uint32 *mirror_profile_err_level);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_set_cb)(
    int unit, uint32 mirror_profile_err_passive);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_get_cb)(
    int unit, uint32 *mirror_profile_err_passive);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_set_cb)(
    int unit, uint32 tcam_last_entry_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_get_cb)(
    int unit, uint32 *tcam_last_entry_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_set_cb)(
    int unit, uint32 lb_trap_used_cnt);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_get_cb)(
    int unit, uint32 *lb_trap_used_cnt);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_set */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_set_cb)(
    int unit, uint32 tst_trap_used_cnt);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_get */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_get_cb)(
    int unit, uint32 *tst_trap_used_cnt);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_verify */
typedef int (*sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_drop_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_drop_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_verify_cb verify;
} sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_oam_cbs_s {
    sw_state_dpp_soc_arad_pp_oam_set_cb set;
    sw_state_dpp_soc_arad_pp_oam_get_cb get;
    sw_state_dpp_soc_arad_pp_oam_alloc_cb alloc;
    sw_state_dpp_soc_arad_pp_oam_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_pp_oam_free_cb free;
    sw_state_dpp_soc_arad_pp_oam_verify_cb verify;
    sw_state_dpp_soc_arad_pp_oam_trap_code_recycle_cbs_t trap_code_recycle;
    sw_state_dpp_soc_arad_pp_oam_trap_code_snoop_cbs_t trap_code_snoop;
    sw_state_dpp_soc_arad_pp_oam_trap_code_frwrd_cbs_t trap_code_frwrd;
    sw_state_dpp_soc_arad_pp_oam_trap_code_drop_cbs_t trap_code_drop;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_level_cbs_t trap_code_trap_to_cpu_level;
    sw_state_dpp_soc_arad_pp_oam_trap_code_trap_to_cpu_passive_cbs_t trap_code_trap_to_cpu_passive;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_snoop_to_cpu_cbs_t mirror_profile_snoop_to_cpu;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_recycle_cbs_t mirror_profile_recycle;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_level_cbs_t mirror_profile_err_level;
    sw_state_dpp_soc_arad_pp_oam_mirror_profile_err_passive_cbs_t mirror_profile_err_passive;
    sw_state_dpp_soc_arad_pp_oam_tcam_last_entry_id_cbs_t tcam_last_entry_id;
    sw_state_dpp_soc_arad_pp_oam_lb_trap_used_cnt_cbs_t lb_trap_used_cnt;
    sw_state_dpp_soc_arad_pp_oam_tst_trap_used_cnt_cbs_t tst_trap_used_cnt;
} sw_state_dpp_soc_arad_pp_oam_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_pp_oam_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_OAM_ACCESS_H_ */
