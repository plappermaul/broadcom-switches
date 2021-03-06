/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
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
#include <shared/swstate/access/sw_state_dpp_soc_arad_tm_interrupts_access.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_alloc(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts,
        sizeof(ARAD_INTERRUPTS),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_soc_arad_tm_interrupts_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts,
        SW_STATE_NONE,
        "sw_state_dpp_soc_arad_tm_interrupts_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.soc.arad.tm->interrupts);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_set(int unit, int cmc_irq2_mask_idx_0, uint32 cmc_irq2_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq2_mask.verify( unit, cmc_irq2_mask_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq2_mask[cmc_irq2_mask_idx_0],
        cmc_irq2_mask,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_get(int unit, int cmc_irq2_mask_idx_0, uint32 *cmc_irq2_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq2_mask.verify( unit, cmc_irq2_mask_idx_0));
    *cmc_irq2_mask = sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq2_mask[cmc_irq2_mask_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_verify(int unit, int cmc_irq2_mask_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_OUT_OF_BOUND_CHECK( cmc_irq2_mask_idx_0, SOC_CMCS_NUM_MAX, "cmc_irq2_mask");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_set(int unit, int cmc_irq3_mask_idx_0, uint32 cmc_irq3_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq3_mask.verify( unit, cmc_irq3_mask_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq3_mask[cmc_irq3_mask_idx_0],
        cmc_irq3_mask,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_get(int unit, int cmc_irq3_mask_idx_0, uint32 *cmc_irq3_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq3_mask.verify( unit, cmc_irq3_mask_idx_0));
    *cmc_irq3_mask = sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq3_mask[cmc_irq3_mask_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_verify(int unit, int cmc_irq3_mask_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_OUT_OF_BOUND_CHECK( cmc_irq3_mask_idx_0, SOC_CMCS_NUM_MAX, "cmc_irq3_mask");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_set(int unit, int cmc_irq4_mask_idx_0, uint32 cmc_irq4_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq4_mask.verify( unit, cmc_irq4_mask_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq4_mask[cmc_irq4_mask_idx_0],
        cmc_irq4_mask,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_get(int unit, int cmc_irq4_mask_idx_0, uint32 *cmc_irq4_mask){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq4_mask.verify( unit, cmc_irq4_mask_idx_0));
    *cmc_irq4_mask = sw_state[unit]->dpp.soc.arad.tm->interrupts->cmc_irq4_mask[cmc_irq4_mask_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_verify(int unit, int cmc_irq4_mask_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_OUT_OF_BOUND_CHECK( cmc_irq4_mask_idx_0, SOC_CMCS_NUM_MAX, "cmc_irq4_mask");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_set(int unit, int interrupt_data_idx_0, CONST ARAD_INTERRUPT_DATA *interrupt_data){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify( unit, interrupt_data_idx_0));
    SHR_SW_STATE_MEMSET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0],
        interrupt_data,
        ARAD_INTERRUPT_DATA,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_get(int unit, int interrupt_data_idx_0, ARAD_INTERRUPT_DATA *interrupt_data){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify( unit, interrupt_data_idx_0));
    *interrupt_data = sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_alloc(int unit, int nof_instances_to_alloc){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data,
        nof_instances_to_alloc * sizeof(ARAD_INTERRUPT_DATA),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data,
        SW_STATE_NONE,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_verify(int unit, int interrupt_data_idx_0){
    uint32 allocated_size = 0;
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data);
    _SOC_IF_ERR_EXIT(shr_sw_state_allocated_size_get (unit, (uint8*)sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data, &allocated_size));
    SW_STATE_OUT_OF_BOUND_CHECK( interrupt_data_idx_0, allocated_size /
        sizeof(*(sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data)) - 1, "interrupt_data");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_set(int unit, int interrupt_data_idx_0, uint32 flags){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.flags.verify( unit, interrupt_data_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].flags,
        flags,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_get(int unit, int interrupt_data_idx_0, uint32 *flags){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.flags.verify( unit, interrupt_data_idx_0));
    *flags = sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].flags;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_verify(int unit, int interrupt_data_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify( unit, interrupt_data_idx_0);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_set(int unit, int interrupt_data_idx_0, uint32 storm_timed_count){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_count.verify( unit, interrupt_data_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].storm_timed_count,
        storm_timed_count,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_get(int unit, int interrupt_data_idx_0, uint32 *storm_timed_count){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_count.verify( unit, interrupt_data_idx_0));
    *storm_timed_count = sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].storm_timed_count;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_verify(int unit, int interrupt_data_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify( unit, interrupt_data_idx_0);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_set(int unit, int interrupt_data_idx_0, uint32 storm_timed_period){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_period.verify( unit, interrupt_data_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].storm_timed_period,
        storm_timed_period,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_get(int unit, int interrupt_data_idx_0, uint32 *storm_timed_period){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_period.verify( unit, interrupt_data_idx_0));
    *storm_timed_period = sw_state[unit]->dpp.soc.arad.tm->interrupts->interrupt_data[interrupt_data_idx_0].storm_timed_period;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_verify(int unit, int interrupt_data_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify( unit, interrupt_data_idx_0);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_soc_arad_tm_interrupts_access_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.alloc = sw_state_dpp_soc_arad_tm_interrupts_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.is_allocated = sw_state_dpp_soc_arad_tm_interrupts_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.free = sw_state_dpp_soc_arad_tm_interrupts_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.verify = sw_state_dpp_soc_arad_tm_interrupts_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq2_mask.set = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq2_mask.get = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq2_mask.verify = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq2_mask_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq3_mask.set = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq3_mask.get = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq3_mask.verify = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq3_mask_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq4_mask.set = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq4_mask.get = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.cmc_irq4_mask.verify = sw_state_dpp_soc_arad_tm_interrupts_cmc_irq4_mask_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.set = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.get = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.alloc = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.is_allocated = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.free = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.verify = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.flags.set = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.flags.get = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.flags.verify = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_flags_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_count.set = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_count.get = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_count.verify = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_count_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_period.set = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_period.get = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.soc.arad.tm.interrupts.interrupt_data.storm_timed_period.verify = sw_state_dpp_soc_arad_tm_interrupts_interrupt_data_storm_timed_period_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
