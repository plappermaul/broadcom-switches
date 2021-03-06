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

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_PON_DOUBLE_LOOKUP_ACCESS_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_PON_DOUBLE_LOOKUP_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_set */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_set_cb)(
    int unit, CONST ARAD_PON_DOUBLE_LOOKUP *pon_double_lookup);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_get */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_get_cb)(
    int unit, ARAD_PON_DOUBLE_LOOKUP *pon_double_lookup);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_alloc */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_is_allocated */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_free */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_verify */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_set */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_set_cb)(
    int unit, int _bit_num);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_clear */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_clear_cb)(
    int unit, int _bit_num);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_get */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_get_cb)(
    int unit, int _bit_num, uint8* result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_read */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_read_cb)(
    int unit, int sw_state_bmp_first, int result_first, int _range, SHR_BITDCL *result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_write */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_write_cb)(
    int unit, int sw_state_bmp_first, int input_bmp_first, int _range, SHR_BITDCL *input_bmp);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_and */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_and_cb)(
    int unit, int _first, int _count, SHR_BITDCL *input_bmp);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_or */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_or_cb)(
    int unit, int _first, int _count, SHR_BITDCL *input_bmp);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_xor */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_xor_cb)(
    int unit, int _first, int _count, SHR_BITDCL *input_bmp);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_remove */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_remove_cb)(
    int unit, int _first, int _count, SHR_BITDCL *input_bmp);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_negate */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_negate_cb)(
    int unit, int _first, int _count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_clear */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_clear_cb)(
    int unit, int _first, int _count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_set */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_set_cb)(
    int unit, int _first, int _count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_null */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_null_cb)(
    int unit, int _first, int _count, uint8 *result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_test */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_test_cb)(
    int unit, int _first, int _count, uint8 *result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_eq */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_eq_cb)(
    int unit, SHR_BITDCL *input_bmp, int _first, int _count, uint8 *result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_count */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_count_cb)(
    int unit, int _first, int _range, int *result);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_verify */
typedef int (*sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_verify_cb)(
    int unit, int pon_double_lookup_enable_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_cbs_s {
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_set_cb bit_set;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_clear_cb bit_clear;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_get_cb bit_get;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_read_cb bit_range_read;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_write_cb bit_range_write;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_and_cb bit_range_and;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_or_cb bit_range_or;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_xor_cb bit_range_xor;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_remove_cb bit_range_remove;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_negate_cb bit_range_negate;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_clear_cb bit_range_clear;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_set_cb bit_range_set;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_null_cb bit_range_null;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_test_cb bit_range_test;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_eq_cb bit_range_eq;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_bit_range_count_cb bit_range_count;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_verify_cb verify;
} sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_pon_double_lookup_cbs_s {
    sw_state_dpp_soc_arad_pp_pon_double_lookup_set_cb set;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_get_cb get;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_alloc_cb alloc;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_is_allocated_cb is_allocated;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_free_cb free;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_verify_cb verify;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_pon_double_lookup_enable_cbs_t pon_double_lookup_enable;
} sw_state_dpp_soc_arad_pp_pon_double_lookup_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_pp_pon_double_lookup_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_PON_DOUBLE_LOOKUP_ACCESS_H_ */
