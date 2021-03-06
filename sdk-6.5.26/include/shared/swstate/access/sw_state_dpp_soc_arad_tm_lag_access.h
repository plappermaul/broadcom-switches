/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_TM_LAG_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_TM_LAG_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_set */
typedef int (*sw_state_dpp_soc_arad_tm_lag_set_cb)(
    int unit, CONST ARAD_LAGS_INFO *lag);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_get */
typedef int (*sw_state_dpp_soc_arad_tm_lag_get_cb)(
    int unit, ARAD_LAGS_INFO *lag);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_alloc */
typedef int (*sw_state_dpp_soc_arad_tm_lag_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_is_allocated */
typedef int (*sw_state_dpp_soc_arad_tm_lag_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_free */
typedef int (*sw_state_dpp_soc_arad_tm_lag_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_verify */
typedef int (*sw_state_dpp_soc_arad_tm_lag_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_in_use_set */
typedef int (*sw_state_dpp_soc_arad_tm_lag_in_use_set_cb)(
    int unit, int in_use_idx_0, uint8 in_use);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_in_use_get */
typedef int (*sw_state_dpp_soc_arad_tm_lag_in_use_get_cb)(
    int unit, int in_use_idx_0, uint8 *in_use);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_in_use_verify */
typedef int (*sw_state_dpp_soc_arad_tm_lag_in_use_verify_cb)(
    int unit, int in_use_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_set */
typedef int (*sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_set_cb)(
    int unit, int local_to_reassembly_context_idx_0, uint32 local_to_reassembly_context);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_get */
typedef int (*sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_get_cb)(
    int unit, int local_to_reassembly_context_idx_0, uint32 *local_to_reassembly_context);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_verify */
typedef int (*sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_verify_cb)(
    int unit, int local_to_reassembly_context_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_lag_in_use_cbs_s {
    sw_state_dpp_soc_arad_tm_lag_in_use_set_cb set;
    sw_state_dpp_soc_arad_tm_lag_in_use_get_cb get;
    sw_state_dpp_soc_arad_tm_lag_in_use_verify_cb verify;
} sw_state_dpp_soc_arad_tm_lag_in_use_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_cbs_s {
    sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_set_cb set;
    sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_get_cb get;
    sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_verify_cb verify;
} sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_lag_cbs_s {
    sw_state_dpp_soc_arad_tm_lag_set_cb set;
    sw_state_dpp_soc_arad_tm_lag_get_cb get;
    sw_state_dpp_soc_arad_tm_lag_alloc_cb alloc;
    sw_state_dpp_soc_arad_tm_lag_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_tm_lag_free_cb free;
    sw_state_dpp_soc_arad_tm_lag_verify_cb verify;
    sw_state_dpp_soc_arad_tm_lag_in_use_cbs_t in_use;
    sw_state_dpp_soc_arad_tm_lag_local_to_reassembly_context_cbs_t local_to_reassembly_context;
} sw_state_dpp_soc_arad_tm_lag_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_tm_lag_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_TM_LAG_ACCESS_H_ */
