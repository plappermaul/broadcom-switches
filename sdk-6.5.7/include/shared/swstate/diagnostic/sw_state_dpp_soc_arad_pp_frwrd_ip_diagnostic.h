/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_FRWRD_IP_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_FRWRD_IP_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_API_TEST
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_dump_cb)(
    int unit, int pp_idx_0);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0);
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0, int location_tbl_idx_0);
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0);
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0);
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
/* implemented by: sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_dump */
typedef int (*sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_dump_cb)(
    int unit, int pp_idx_0, int frwrd_ip_idx_0);
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_dump_cb dump;
} sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_diagnostic_cbs_t;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_diagnostic_cbs_s {
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_dump_cb dump;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_diagnostic_cbs_t;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_diagnostic_cbs_s {
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_dump_cb dump;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_db_entry_diagnostic_cbs_t db_entry;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_diagnostic_cbs_t;

#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_diagnostic_cbs_s {
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_dump_cb dump;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_diagnostic_cbs_t;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_diagnostic_cbs_s {
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_dump_cb dump;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_diagnostic_cbs_t;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_diagnostic_cbs_s {
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_dump_cb dump;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_entry_flag_diagnostic_cbs_t entry_flag;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_diagnostic_cbs_t;

#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
typedef struct sw_state_dpp_soc_arad_pp_frwrd_ip_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_frwrd_ip_dump_cb dump;
    sw_state_dpp_soc_arad_pp_frwrd_ip_route_key_to_entry_id_diagnostic_cbs_t route_key_to_entry_id;
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_location_tbl_diagnostic_cbs_t location_tbl;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_cache_mode_diagnostic_cbs_t kbp_cache_mode;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
#if defined(INCLUDE_KBP)&&!defined(BCM_88030)
    sw_state_dpp_soc_arad_pp_frwrd_ip_kbp_diag_flag_diagnostic_cbs_t kbp_diag_flag;
#endif /* defined(INCLUDE_KBP)&&!defined(BCM_88030)*/ 
} sw_state_dpp_soc_arad_pp_frwrd_ip_diagnostic_cbs_t;

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

int sw_state_dpp_soc_arad_pp_frwrd_ip_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_API_TEST */

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_FRWRD_IP_DIAGNOSTIC_H_ */
