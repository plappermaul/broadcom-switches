/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.  
 */
#ifndef _SW_STATE_DIAGNOSTICS_H
#define _SW_STATE_DIAGNOSTICS_H

#include <soc/dnxc/swstate/dnxc_sw_state_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_common.h>
#include <soc/dnxc/swstate/dnxc_sw_state_dispatcher.h>

#ifdef BCM_DNX_SUPPORT
#include <bcm_int/dnx/init/init_time_analyzer.h>
#endif

#ifdef BCM_DNX_SUPPORT
#define SW_STATE_DIAG_DNX_ONLY(x) x
#else
#define SW_STATE_DIAG_DNX_ONLY(x)
#endif

int dnxc_sw_state_diag_init(
    int unit);

int dnxc_sw_state_diag_is_on(
    int unit);

#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum
{
    DNX_SW_STATE_DIAG_ALLOC_BITMAP,
    DNX_SW_STATE_DIAG_ALLOC,
    DNX_SW_STATE_DIAG_FREE,
    DNX_SW_STATE_DIAG_PTR_ALLOC,
    DNX_SW_STATE_DIAG_PTR_FREE,
    DNX_SW_STATE_DIAG_MODIFY,
    DNX_SW_STATE_DIAG_READ,
    DNX_SW_STATE_DIAG_BITMAP,
    DNX_SW_STATE_DIAG_PBMP,
    DNX_SW_STATE_DIAG_MUTEX,
    DNX_SW_STATE_DIAG_SEM,
    DNX_SW_STATE_DIAG_OCC_BM,
    DNX_SW_STATE_DIAG_HTB,
    DNX_SW_STATE_DIAG_LL,
    DNX_SW_STATE_DIAG_BT,
    DNX_SW_STATE_DIAG_CB_DB,
    DNX_SW_STATE_DIAG_MULTI_SET,
    DNX_SW_STATE_DIAG_DEFRAGMENTED_CHUNK,
    DNX_SW_STATE_DIAG_RESOURCE_TAG_BITMAP,
    DNX_SW_STATE_DIAG_RES_MNGR,
    DNX_SW_STATE_DIAG_TEMP_MNGR,
    DNX_SW_STATE_DIAG_STRING
} dnx_sw_state_func_type_diag_info;

typedef struct dnx_sw_state_diagnostic_info_s
{
    uint32 size;

} dnx_sw_state_diagnostic_info_t;

#define DNX_SW_STATE_DIAG_INFO_PRE(unit, module_id) \
    SW_STATE_DIAG_DNX_ONLY(DNX_INIT_TIME_ANALYZER_SW_STATE_START(unit, DNX_INIT_TIME_ANALYZER_SW_STATE)); \
    if (dnxc_sw_state_diag_is_on(unit) && !sw_state_is_done_init(unit)) {\
       SHR_IF_ERR_EXIT(dnxc_sw_state_dispatcher[unit][module_id].start_measurement_point(unit)); \
    }

#define DNX_SW_STATE_DIAG_INFO_POST(unit, module_id, info_struct, node_idx, function, ptr) \
    DNX_SW_STATE_DIAG_INFO_POST_WITH_FLAGS(unit, module_id, info_struct, node_idx, function, ptr, DNXC_SW_STATE_NO_FLAGS)

#define DNX_SW_STATE_DIAG_INFO_POST_WITH_FLAGS(unit, module_id, info_struct, node_idx, function, ptr, flags) \
    do { \
        if (dnxc_sw_state_diag_is_on(unit) && !sw_state_is_done_init(unit)) { \
            uint32 _allowed_size = DNXC_SW_STATE_ALLOC_DS_MAX_BYTES; \
            long unsigned int _actual_size =(long unsigned int) dnxc_sw_state_dispatcher[unit][module_id].size_measurement_get(unit); \
            if(!(sw_state_is_flag_on(flags, DNXC_SW_STATE_DNX_DATA_ALLOC)) && \
               !(sw_state_is_flag_on(flags, DNXC_SW_STATE_DNX_DATA_ALLOC_EXCEPTION)) && \
                (sw_state_is_flag_on(flags, DNXC_SW_STATE_AUTO_GENERATED_ALLOCATION)) && \
                (_actual_size > _allowed_size)) \
            { \
                SHR_ERR_EXIT(_SHR_E_RESOURCE, "The size of the allocated sw state mem was too big: %lu", _actual_size ); \
            } \
            if (function == DNX_SW_STATE_DIAG_FREE || function == DNX_SW_STATE_DIAG_PTR_FREE) { \
                uint32 allocated_size = 0; \
                dnxc_sw_state_dynamic_allocated_size_get(unit, module_id, (uint8*)ptr, &allocated_size); \
                info_struct[unit][node_idx].size -= allocated_size; \
                dnxc_sw_state_dispatcher[unit][module_id].module_size_update(unit, module_id, allocated_size, 0);\
            } else { \
                info_struct[unit][node_idx].size += _actual_size; \
                dnxc_sw_state_dispatcher[unit][module_id].module_size_update(unit, module_id, _actual_size, 1);\
            } \
            SW_STATE_DIAG_DNX_ONLY(DNX_INIT_TIME_ANALYZER_SW_STATE_STOP(unit, DNX_INIT_TIME_ANALYZER_SW_STATE)); \
        } \
    } while(0)

#else

#define DNX_SW_STATE_DIAG_INFO_UPDATE(unit, module_id, info_struct, node_idx, function, alloc_size, ptr)

#define DNX_SW_STATE_DIAG_INFO_PRE(unit, module_id)

#define DNX_SW_STATE_DIAG_INFO_POST(unit, module_id, info_struct, node_idx, function, ptr)

#define DNX_SW_STATE_DIAG_INFO_POST_WITH_FLAGS(unit, module_id, info_struct, node_idx, function, ptr, flags)

#endif

#endif
