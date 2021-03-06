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

#ifndef _SHR_SW_STATE_DPP_BCM_LIF_LINKED_LIST_ACCESS_H_
#define _SHR_SW_STATE_DPP_BCM_LIF_LINKED_LIST_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_alloc */
typedef int (*sw_state_dpp_bcm_lif_linked_list_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_is_allocated */
typedef int (*sw_state_dpp_bcm_lif_linked_list_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_free */
typedef int (*sw_state_dpp_bcm_lif_linked_list_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_set_cb)(
    int unit, uint32 max_nof_lif_linked_lists);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_get_cb)(
    int unit, uint32 *max_nof_lif_linked_lists);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_set_cb)(
    int unit, int lif_linked_lists_array_idx_0, CONST failover_free_lif_linked_list_t *lif_linked_lists_array);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_get_cb)(
    int unit, int lif_linked_lists_array_idx_0, failover_free_lif_linked_list_t *lif_linked_lists_array);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_alloc */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_alloc_cb)(
    int unit, int nof_instances_to_alloc);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_is_allocated */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_free */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_verify_cb)(
    int unit, int lif_linked_lists_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_set_cb)(
    int unit, int lif_linked_lists_array_idx_0, int first_member);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_get_cb)(
    int unit, int lif_linked_lists_array_idx_0, int *first_member);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_verify_cb)(
    int unit, int lif_linked_lists_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_set_cb)(
    int unit, int lif_linked_lists_array_idx_0, uint32 size);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_get_cb)(
    int unit, int lif_linked_lists_array_idx_0, uint32 *size);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_verify_cb)(
    int unit, int lif_linked_lists_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_set_cb)(
    int unit, uint32 max_nof_lif_linked_list_members);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_get_cb)(
    int unit, uint32 *max_nof_lif_linked_list_members);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_set_cb)(
    int unit, int lif_linked_list_members_array_idx_0, CONST failover_free_lif_linked_list_member_t *lif_linked_list_members_array);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_get_cb)(
    int unit, int lif_linked_list_members_array_idx_0, failover_free_lif_linked_list_member_t *lif_linked_list_members_array);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_alloc */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_alloc_cb)(
    int unit, int nof_instances_to_alloc);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_allocated */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_free */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_verify_cb)(
    int unit, int lif_linked_list_members_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_set_cb)(
    int unit, int lif_linked_list_members_array_idx_0, int lif_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_get_cb)(
    int unit, int lif_linked_list_members_array_idx_0, int *lif_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_verify_cb)(
    int unit, int lif_linked_list_members_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_set_cb)(
    int unit, int lif_linked_list_members_array_idx_0, uint8 is_even);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_get_cb)(
    int unit, int lif_linked_list_members_array_idx_0, uint8 *is_even);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_verify_cb)(
    int unit, int lif_linked_list_members_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_set */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_set_cb)(
    int unit, int lif_linked_list_members_array_idx_0, int next);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_get */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_get_cb)(
    int unit, int lif_linked_list_members_array_idx_0, int *next);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_verify */
typedef int (*sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_verify_cb)(
    int unit, int lif_linked_list_members_array_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_alloc_cb alloc;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_is_allocated_cb is_allocated;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_free_cb free;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_verify_cb verify;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_first_member_cbs_t first_member;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_size_cbs_t size;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_verify_cb verify;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_cbs_s {
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_set_cb set;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_get_cb get;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_alloc_cb alloc;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_allocated_cb is_allocated;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_free_cb free;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_verify_cb verify;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_lif_id_cbs_t lif_id;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_is_even_cbs_t is_even;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_next_cbs_t next;
} sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_lif_linked_list_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_lif_linked_list_alloc_cb alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_lif_linked_list_is_allocated_cb is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_lif_linked_list_free_cb free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_lif_linked_list_verify_cb verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_lists_cbs_t max_nof_lif_linked_lists;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_lists_array_cbs_t lif_linked_lists_array;
    sw_state_dpp_bcm_lif_linked_list_max_nof_lif_linked_list_members_cbs_t max_nof_lif_linked_list_members;
    sw_state_dpp_bcm_lif_linked_list_lif_linked_list_members_array_cbs_t lif_linked_list_members_array;
} sw_state_dpp_bcm_lif_linked_list_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_bcm_lif_linked_list_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_BCM_LIF_LINKED_LIST_ACCESS_H_ */
