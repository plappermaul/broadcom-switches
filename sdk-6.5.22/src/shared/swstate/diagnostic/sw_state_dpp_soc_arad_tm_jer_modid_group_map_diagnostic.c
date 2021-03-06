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
#include <shared/swstate/sw_state_dump.h>
#include <shared/swstate/diagnostic/sw_state_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_jer_modid_group_map_diagnostic.h>

#ifdef BCM_WARM_BOOT_API_TEST
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dump(int unit, int tm_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map, sizeof(*sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_0.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_0_count.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_1.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_1_count.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_2.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_2_count.dump(unit, tm_idx_0, j0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_0
            , sizeof(*sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_0), SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT)) ? SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_0[i0],
                "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_0[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_0_count,
        "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_0_count: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_1
            , sizeof(*sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_1), SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT)) ? SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_1[i0],
                "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_1[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_1_count,
        "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_1_count: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_2
            , sizeof(*sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_2), SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT)) ? SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(MAX_NOF_FAP_ID_MAPPED_TO_DEST_LIMIT); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_2[i0],
                "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_2[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_dump(int unit, int tm_idx_0, int jer_modid_group_map_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->jer_modid_group_map->dest_2_count,
        "sw_state[%d]->dpp.soc.arad.tm->jer_modid_group_map->dest_2_count: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_soc_arad_tm_jer_modid_group_map_diagnostic_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_0.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_0_count.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_0_count_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_1.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_1_count.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_1_count_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_2.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.jer_modid_group_map.dest_2_count.dump = sw_state_dpp_soc_arad_tm_jer_modid_group_map_dest_2_count_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_API_TEST */
