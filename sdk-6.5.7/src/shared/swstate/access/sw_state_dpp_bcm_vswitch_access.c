/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#include <sal/core/libc.h>
#include <shared/swstate/sw_state.h>
#include <shared/error.h>
#include <shared/swstate/sw_state_defs.h>
#include <shared/bsl.h>
#include <shared/swstate/access/sw_state_access.h>
#include <shared/swstate/access/sw_state_dpp_bcm_vswitch_access.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_alloc(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.vswitch,
        sizeof(_bcm_dpp_vswitch_bookkeeping_t),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_vswitch_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.vswitch,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_vswitch_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.bcm.vswitch,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_vswitch_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.bcm.vswitch);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_initialized_set(int unit, int initialized){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.vswitch.initialized.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.vswitch->initialized,
        initialized,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_vswitch_initialized_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_initialized_get(int unit, int *initialized){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.vswitch.initialized.verify( unit));
    *initialized = sw_state[unit]->dpp.bcm.vswitch->initialized;
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_initialized_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.vswitch.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_alloc_bitmap(int unit, int _nof_bits_to_alloc){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC_BITMAP(
        unit,
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
        _nof_bits_to_alloc,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_vswitch_vsi_usage_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_vswitch_vsi_usage_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_set(int unit, int _bit_num){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _bit_num, _bit_num, "vsi_usage");
        SHR_SW_STATE_BIT_SET(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _bit_num);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_clear(int unit, int _bit_num){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _bit_num, _bit_num, "vsi_usage");
        SHR_SW_STATE_BIT_CLEAR(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _bit_num);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_get(int unit, int _bit_num, uint8* result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _bit_num, _bit_num, "vsi_usage");
        SHR_SW_STATE_BIT_GET(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _bit_num,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_read(int unit, int sw_state_bmp_first, int result_first, int _range, SHR_BITDCL *result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, sw_state_bmp_first, sw_state_bmp_first + _range - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_READ(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         sw_state_bmp_first,
         result_first,
         _range,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_write(int unit, int sw_state_bmp_first, int input_bmp_first, int _range, SHR_BITDCL *input_bmp){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, sw_state_bmp_first, sw_state_bmp_first + _range - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_WRITE(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         sw_state_bmp_first,
         input_bmp_first,
          _range,
         input_bmp);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_and(int unit, int _first, int _count, SHR_BITDCL *input_bmp){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_AND(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         input_bmp,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_or(int unit, int _first, int _count, SHR_BITDCL *input_bmp){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_OR(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         input_bmp,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_xor(int unit, int _first, int _count, SHR_BITDCL *input_bmp){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_XOR(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         input_bmp,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_remove(int unit, int _first, int _count, SHR_BITDCL *input_bmp){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_REMOVE(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         input_bmp,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_negate(int unit, int _first, int _count){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_NEGATE(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_clear(int unit, int _first, int _count){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_CLEAR(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_set(int unit, int _first, int _count){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_SET(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_null(int unit, int _first, int _count, uint8 *result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_NULL(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_test(int unit, int _first, int _count, uint8 *result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_TEST(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_eq(int unit, SHR_BITDCL *input_bmp, int _first, int _count, uint8 *result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _count - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_EQ(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _count,
         input_bmp,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_count(int unit, int _first, int _range, int *result){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_BITDCL_VERIFY(sw_state_access[unit].dpp.bcm.vswitch.verify(unit),
        sw_state[unit]->dpp.bcm.vswitch->vsi_usage, _first, _first + _range - 1, "vsi_usage");
        SHR_SW_STATE_BIT_RANGE_COUNT(
        unit,
         SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
         sw_state[unit]->dpp.bcm.vswitch->vsi_usage,
         _first,
         _range,
         result);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vpn_detach_ports_set(int unit, int vpn_detach_ports){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.vswitch.vpn_detach_ports.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.vswitch->vpn_detach_ports,
        vpn_detach_ports,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_vswitch_vpn_detach_ports_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vpn_detach_ports_get(int unit, int *vpn_detach_ports){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.vswitch.vpn_detach_ports.verify( unit));
    *vpn_detach_ports = sw_state[unit]->dpp.bcm.vswitch->vpn_detach_ports;
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_vswitch_vpn_detach_ports_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.vswitch.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 


int
sw_state_dpp_bcm_vswitch_access_cb_init(int unit){
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.alloc = sw_state_dpp_bcm_vswitch_alloc;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.is_allocated = sw_state_dpp_bcm_vswitch_is_allocated;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.free = sw_state_dpp_bcm_vswitch_free;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.verify = sw_state_dpp_bcm_vswitch_verify;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.initialized.set = sw_state_dpp_bcm_vswitch_initialized_set;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.initialized.get = sw_state_dpp_bcm_vswitch_initialized_get;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.initialized.verify = sw_state_dpp_bcm_vswitch_initialized_verify;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.alloc_bitmap = sw_state_dpp_bcm_vswitch_vsi_usage_alloc_bitmap;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.free = sw_state_dpp_bcm_vswitch_vsi_usage_free;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_set = sw_state_dpp_bcm_vswitch_vsi_usage_bit_set;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_clear = sw_state_dpp_bcm_vswitch_vsi_usage_bit_clear;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_get = sw_state_dpp_bcm_vswitch_vsi_usage_bit_get;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_read = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_read;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_write = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_write;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_and = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_and;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_or = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_or;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_xor = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_xor;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_remove = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_remove;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_negate = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_negate;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_clear = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_clear;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_set = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_set;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_null = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_null;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_test = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_test;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_eq = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_eq;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vsi_usage.bit_range_count = sw_state_dpp_bcm_vswitch_vsi_usage_bit_range_count;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vpn_detach_ports.set = sw_state_dpp_bcm_vswitch_vpn_detach_ports_set;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vpn_detach_ports.get = sw_state_dpp_bcm_vswitch_vpn_detach_ports_get;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.bcm.vswitch.vpn_detach_ports.verify = sw_state_dpp_bcm_vswitch_vpn_detach_ports_verify;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
