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

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_FWD_MACT_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_FWD_MACT_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_set_cb)(
    int unit, CONST ARAD_PP_FWD_MACT *fwd_mact);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_get_cb)(
    int unit, ARAD_PP_FWD_MACT *fwd_mact);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_alloc */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_is_allocated */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_free */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_set_cb)(
    int unit, SOC_PPC_FRWRD_MACT_LEARNING_MODE learning_mode);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_get_cb)(
    int unit, SOC_PPC_FRWRD_MACT_LEARNING_MODE *learning_mode);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_set_cb)(
    int unit, uint8 is_petra_a_compatible);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_get_cb)(
    int unit, uint8 *is_petra_a_compatible);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_set_cb)(
    int unit, int flush_db_data_arr_idx_0, CONST ARAD_PP_FRWRD_MACT_FLUSH_DB_DATA_ARR *flush_db_data_arr);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_get_cb)(
    int unit, int flush_db_data_arr_idx_0, ARAD_PP_FRWRD_MACT_FLUSH_DB_DATA_ARR *flush_db_data_arr);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_verify_cb)(
    int unit, int flush_db_data_arr_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_set_cb)(
    int unit, int flush_db_data_arr_idx_0, int flush_db_data_idx_0, uint32 flush_db_data);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_get_cb)(
    int unit, int flush_db_data_arr_idx_0, int flush_db_data_idx_0, uint32 *flush_db_data);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_verify_cb)(
    int unit, int flush_db_data_arr_idx_0, int flush_db_data_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_set_cb)(
    int unit, uint32 flush_entry_use);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_get_cb)(
    int unit, uint32 *flush_entry_use);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_set_cb)(
    int unit, uint8 traverse_mode);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_get_cb)(
    int unit, uint8 *traverse_mode);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_set_cb)(
    int unit, uint32 num_entries_in_dma_host_memory);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_get_cb)(
    int unit, uint32 *num_entries_in_dma_host_memory);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_set_cb)(
    int unit, uint32 size_of_learn_event);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_get_cb)(
    int unit, uint32 *size_of_learn_event);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_set */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_set_cb)(
    int unit, uint32 host_memory_allocated_bytes);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_get */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_get_cb)(
    int unit, uint32 *host_memory_allocated_bytes);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_verify */
typedef int (*sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_verify_cb)(
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
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_verify_cb verify;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_flush_db_data_cbs_t flush_db_data;
} sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_verify_cb verify;
} sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_fwd_mact_cbs_s {
    sw_state_dpp_soc_arad_pp_fwd_mact_set_cb set;
    sw_state_dpp_soc_arad_pp_fwd_mact_get_cb get;
    sw_state_dpp_soc_arad_pp_fwd_mact_alloc_cb alloc;
    sw_state_dpp_soc_arad_pp_fwd_mact_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_pp_fwd_mact_free_cb free;
    sw_state_dpp_soc_arad_pp_fwd_mact_verify_cb verify;
    sw_state_dpp_soc_arad_pp_fwd_mact_learning_mode_cbs_t learning_mode;
    sw_state_dpp_soc_arad_pp_fwd_mact_is_petra_a_compatible_cbs_t is_petra_a_compatible;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_db_data_arr_cbs_t flush_db_data_arr;
    sw_state_dpp_soc_arad_pp_fwd_mact_flush_entry_use_cbs_t flush_entry_use;
    sw_state_dpp_soc_arad_pp_fwd_mact_traverse_mode_cbs_t traverse_mode;
    sw_state_dpp_soc_arad_pp_fwd_mact_num_entries_in_dma_host_memory_cbs_t num_entries_in_dma_host_memory;
    sw_state_dpp_soc_arad_pp_fwd_mact_size_of_learn_event_cbs_t size_of_learn_event;
    sw_state_dpp_soc_arad_pp_fwd_mact_host_memory_allocated_bytes_cbs_t host_memory_allocated_bytes;
} sw_state_dpp_soc_arad_pp_fwd_mact_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_pp_fwd_mact_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_FWD_MACT_ACCESS_H_ */
