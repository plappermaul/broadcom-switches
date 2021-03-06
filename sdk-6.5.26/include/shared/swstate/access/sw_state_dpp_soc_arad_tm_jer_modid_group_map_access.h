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

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_TM_JER_MODID_GROUP_MAP_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_TM_JER_MODID_GROUP_MAP_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_set_cb)(
    int unit, CONST JER_MODID_GROUP_MAP *jer_modid_group_map);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_get_cb)(
    int unit, JER_MODID_GROUP_MAP *jer_modid_group_map);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_alloc */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_is_allocated */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_free */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_set_cb)(
    int unit, int dest_0_idx_0, int dest_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_get_cb)(
    int unit, int dest_0_idx_0, int *dest_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_verify_cb)(
    int unit, int dest_0_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_set_cb)(
    int unit, int dest_0_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_get_cb)(
    int unit, int *dest_0_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_set_cb)(
    int unit, int dest_1_idx_0, int dest_1);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_get_cb)(
    int unit, int dest_1_idx_0, int *dest_1);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_verify_cb)(
    int unit, int dest_1_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_set_cb)(
    int unit, int dest_1_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_get_cb)(
    int unit, int *dest_1_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_set_cb)(
    int unit, int dest_2_idx_0, int dest_2);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_get_cb)(
    int unit, int dest_2_idx_0, int *dest_2);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_verify_cb)(
    int unit, int dest_2_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_set */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_set_cb)(
    int unit, int dest_2_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_get */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_get_cb)(
    int unit, int *dest_2_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_verify */
typedef int (*sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_verify_cb)(
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
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_verify_cb verify;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_jer_modid_group_map_cbs_s {
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_set_cb set;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_get_cb get;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_alloc_cb alloc;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_free_cb free;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_verify_cb verify;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_cbs_t dest_0;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_cbs_t dest_0_count;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_cbs_t dest_1;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_cbs_t dest_1_count;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_cbs_t dest_2;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_cbs_t dest_2_count;
} sw_state_dpp_soc_arad_tm_jer_modid_group_map_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_tm_jer_modid_group_map_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_TM_JER_MODID_GROUP_MAP_ACCESS_H_ */
