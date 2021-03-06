/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
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
#include <shared/swstate/access/sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_access.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_set(int unit, int dram_wr_crc_interrupt_state_idx_0, int dram_wr_crc_interrupt_state){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.verify( unit, dram_wr_crc_interrupt_state_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state[dram_wr_crc_interrupt_state_idx_0],
        dram_wr_crc_interrupt_state,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_get(int unit, int dram_wr_crc_interrupt_state_idx_0, int *dram_wr_crc_interrupt_state){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.verify( unit, dram_wr_crc_interrupt_state_idx_0));
    *dram_wr_crc_interrupt_state = sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state[dram_wr_crc_interrupt_state_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_alloc(int unit, int nof_instances_to_alloc){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state,
        nof_instances_to_alloc * sizeof(int),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state,
        SW_STATE_NONE,
        "sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_verify(int unit, int dram_wr_crc_interrupt_state_idx_0){
    uint32 allocated_size = 0;
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.jericho.tm.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state);
    _SOC_IF_ERR_EXIT(shr_sw_state_allocated_size_get (unit, (uint8*)sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state, &allocated_size));
    SW_STATE_OUT_OF_BOUND_CHECK( dram_wr_crc_interrupt_state_idx_0, allocated_size /
        sizeof(*(sw_state[unit]->dpp.soc.jericho.tm.dram_wr_crc_interrupt_state)) - 1, "dram_wr_crc_interrupt_state");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_access_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.set = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.get = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.alloc = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.is_allocated = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.free = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.jericho.tm.dram_wr_crc_interrupt_state.verify = sw_state_dpp_soc_jericho_tm_dram_wr_crc_interrupt_state_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
