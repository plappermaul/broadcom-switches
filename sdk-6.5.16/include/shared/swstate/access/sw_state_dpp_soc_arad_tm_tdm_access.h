/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_TM_TDM_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_TM_TDM_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_set */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_set_cb)(
    int unit, CONST ARAD_TDM *tdm);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_get */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_get_cb)(
    int unit, ARAD_TDM *tdm);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_alloc */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_is_allocated */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_free */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_verify */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_context_map_set */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_context_map_set_cb)(
    int unit, int context_map_idx_0, ARAD_INTERFACE_ID context_map);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_context_map_get */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_context_map_get_cb)(
    int unit, int context_map_idx_0, ARAD_INTERFACE_ID *context_map);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_tdm_context_map_verify */
typedef int (*sw_state_dpp_soc_arad_tm_tdm_context_map_verify_cb)(
    int unit, int context_map_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_tdm_context_map_cbs_s {
    sw_state_dpp_soc_arad_tm_tdm_context_map_set_cb set;
    sw_state_dpp_soc_arad_tm_tdm_context_map_get_cb get;
    sw_state_dpp_soc_arad_tm_tdm_context_map_verify_cb verify;
} sw_state_dpp_soc_arad_tm_tdm_context_map_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_tdm_cbs_s {
    sw_state_dpp_soc_arad_tm_tdm_set_cb set;
    sw_state_dpp_soc_arad_tm_tdm_get_cb get;
    sw_state_dpp_soc_arad_tm_tdm_alloc_cb alloc;
    sw_state_dpp_soc_arad_tm_tdm_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_tm_tdm_free_cb free;
    sw_state_dpp_soc_arad_tm_tdm_verify_cb verify;
    sw_state_dpp_soc_arad_tm_tdm_context_map_cbs_t context_map;
} sw_state_dpp_soc_arad_tm_tdm_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_tm_tdm_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_TM_TDM_ACCESS_H_ */
