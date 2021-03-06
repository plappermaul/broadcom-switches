/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
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
#include <shared/swstate/diagnostic/sw_state_dpp_bcm_alloc_mngr_diagnostic.h>

#ifdef BCM_WARM_BOOT_SUPPORT
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_dump(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.fec_bank_groups.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.resource_to_pool_map.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.template_to_pool_map.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_thresh_map.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_interface_unicast_thresh_map.dump(unit, j0);
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_interface_multicast_thresh_map.dump(unit, j0);
#ifdef BCM_88470_A0
            sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.fec_is_cascaded.dump(unit, j0);
#endif /* BCM_88470_A0*/ 
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
int sw_state_dpp_bcm_alloc_mngr_fec_bank_groups_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.bcm.alloc_mngr->fec_bank_groups
            , sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->fec_bank_groups), SOC_DPP_DEFS_MAX(NOF_FEC_BANKS)) ? SOC_DPP_DEFS_MAX(NOF_FEC_BANKS) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(NOF_FEC_BANKS); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->fec_bank_groups[i0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->fec_bank_groups[%d]: ", unit, i0);
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
int sw_state_dpp_bcm_alloc_mngr_resource_to_pool_map_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr->resource_to_pool_map, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->resource_to_pool_map), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->resource_to_pool_map[j0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->resource_to_pool_map[%d]: ", unit, j0);
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
int sw_state_dpp_bcm_alloc_mngr_template_to_pool_map_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr->template_to_pool_map, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->template_to_pool_map), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->template_to_pool_map[j0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->template_to_pool_map[%d]: ", unit, j0);
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
int sw_state_dpp_bcm_alloc_mngr_egress_encap_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.egress_encap_banks.dump(unit, alloc_mngr_idx_0);
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.egress_encap_count.dump(unit, alloc_mngr_idx_0);
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.init.dump(unit, alloc_mngr_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_egress_encap_egress_encap_banks_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_banks, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_banks), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_banks[j0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_banks[%d]: ", unit, j0);
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
int sw_state_dpp_bcm_alloc_mngr_egress_encap_egress_encap_count_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_count,
        "sw_state[%d]->dpp.bcm.alloc_mngr->egress_encap.egress_encap_count: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_egress_encap_init_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egress_encap.init,
        "sw_state[%d]->dpp.bcm.alloc_mngr->egress_encap.init: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_ingress_lif_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.ingress_lif_banks.dump(unit, alloc_mngr_idx_0);
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.ingress_lif_count.dump(unit, alloc_mngr_idx_0);
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.init.dump(unit, alloc_mngr_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_ingress_lif_ingress_lif_banks_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_banks, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_banks), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_banks[j0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_banks[%d]: ", unit, j0);
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
int sw_state_dpp_bcm_alloc_mngr_ingress_lif_ingress_lif_count_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_count,
        "sw_state[%d]->dpp.bcm.alloc_mngr->ingress_lif.ingress_lif_count: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_ingress_lif_init_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->ingress_lif.init,
        "sw_state[%d]->dpp.bcm.alloc_mngr->ingress_lif.init: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_sync_lif_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.sync_lif_banks.dump(unit, alloc_mngr_idx_0);
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.init.dump(unit, alloc_mngr_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_sync_lif_sync_lif_banks_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.bcm.alloc_mngr->sync_lif.sync_lif_banks, sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->sync_lif.sync_lif_banks), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->sync_lif.sync_lif_banks[j0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->sync_lif.sync_lif_banks[%d]: ", unit, j0);
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
int sw_state_dpp_bcm_alloc_mngr_sync_lif_init_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->sync_lif.init,
        "sw_state[%d]->dpp.bcm.alloc_mngr->sync_lif.init: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_egr_thresh_map_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_thresh_map
            , sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->egr_thresh_map), SOC_DPP_DEFS_MAX(NOF_LOGICAL_PORTS)) ? SOC_DPP_DEFS_MAX(NOF_LOGICAL_PORTS) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(NOF_LOGICAL_PORTS); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_thresh_map[i0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->egr_thresh_map[%d]: ", unit, i0);
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
int sw_state_dpp_bcm_alloc_mngr_egr_interface_unicast_thresh_map_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_unicast_thresh_map
            , sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_unicast_thresh_map), SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES)) ? SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES) - 1 : 0;
        for (; i0 < SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_unicast_thresh_map[i0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->egr_interface_unicast_thresh_map[%d]: ", unit, i0);
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
int sw_state_dpp_bcm_alloc_mngr_egr_interface_multicast_thresh_map_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_multicast_thresh_map
            , sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_multicast_thresh_map), SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES)) ? SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES) - 1 : 0;
        for (; i0 < SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)*SOC_DPP_DEFS_MAX(NOF_CORES); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->egr_interface_multicast_thresh_map[i0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->egr_interface_multicast_thresh_map[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#if defined(BCM_88470_A0) && defined(BCM_PETRA_SUPPORT) 
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_alloc_mngr_fec_is_cascaded_dump(int unit, int alloc_mngr_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.bcm.alloc_mngr->fec_is_cascaded
            , sizeof(*sw_state[unit]->dpp.bcm.alloc_mngr->fec_is_cascaded), _SHR_BITDCLSIZE(SOC_DPP_DEFS_MAX(NOF_FECS))) ? _SHR_BITDCLSIZE(SOC_DPP_DEFS_MAX(NOF_FECS)) - 1 : 0;
        for (; i0 < _SHR_BITDCLSIZE(SOC_DPP_DEFS_MAX(NOF_FECS)); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.bcm.alloc_mngr->fec_is_cascaded[i0],
                "sw_state[%d]->dpp.bcm.alloc_mngr->fec_is_cascaded[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* defined(BCM_88470_A0) && defined(BCM_PETRA_SUPPORT) */ 


int
sw_state_dpp_bcm_alloc_mngr_diagnostic_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.dump = sw_state_dpp_bcm_alloc_mngr_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.fec_bank_groups.dump = sw_state_dpp_bcm_alloc_mngr_fec_bank_groups_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.resource_to_pool_map.dump = sw_state_dpp_bcm_alloc_mngr_resource_to_pool_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.template_to_pool_map.dump = sw_state_dpp_bcm_alloc_mngr_template_to_pool_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.dump = sw_state_dpp_bcm_alloc_mngr_egress_encap_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.egress_encap_banks.dump = sw_state_dpp_bcm_alloc_mngr_egress_encap_egress_encap_banks_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.egress_encap_count.dump = sw_state_dpp_bcm_alloc_mngr_egress_encap_egress_encap_count_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egress_encap.init.dump = sw_state_dpp_bcm_alloc_mngr_egress_encap_init_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.dump = sw_state_dpp_bcm_alloc_mngr_ingress_lif_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.ingress_lif_banks.dump = sw_state_dpp_bcm_alloc_mngr_ingress_lif_ingress_lif_banks_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.ingress_lif_count.dump = sw_state_dpp_bcm_alloc_mngr_ingress_lif_ingress_lif_count_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.ingress_lif.init.dump = sw_state_dpp_bcm_alloc_mngr_ingress_lif_init_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.dump = sw_state_dpp_bcm_alloc_mngr_sync_lif_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.sync_lif_banks.dump = sw_state_dpp_bcm_alloc_mngr_sync_lif_sync_lif_banks_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.sync_lif.init.dump = sw_state_dpp_bcm_alloc_mngr_sync_lif_init_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_thresh_map.dump = sw_state_dpp_bcm_alloc_mngr_egr_thresh_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_interface_unicast_thresh_map.dump = sw_state_dpp_bcm_alloc_mngr_egr_interface_unicast_thresh_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.egr_interface_multicast_thresh_map.dump = sw_state_dpp_bcm_alloc_mngr_egr_interface_multicast_thresh_map_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#if defined(BCM_88470_A0) && defined(BCM_PETRA_SUPPORT) 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.bcm.alloc_mngr.fec_is_cascaded.dump = sw_state_dpp_bcm_alloc_mngr_fec_is_cascaded_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* defined(BCM_88470_A0) && defined(BCM_PETRA_SUPPORT) */ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_SUPPORT */
