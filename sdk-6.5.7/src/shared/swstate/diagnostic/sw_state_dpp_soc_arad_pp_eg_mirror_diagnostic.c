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
#include <shared/swstate/sw_state_dump.h>
#include <shared/swstate/diagnostic/sw_state_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_eg_mirror_diagnostic.h>

#ifdef BCM_WARM_BOOT_API_TEST
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_eg_mirror_dump(int unit, int pp_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.soc.arad.pp->eg_mirror, sizeof(*sw_state[unit]->dpp.soc.arad.pp->eg_mirror), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.eg_mirror.eg_mirror_port_vlan.dump(unit, pp_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.eg_mirror.mirror_profile_multi_set.dump(unit, pp_idx_0, j0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_eg_mirror_eg_mirror_port_vlan_dump(int unit, int pp_idx_0, int eg_mirror_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan), SOC_DPP_DEFS_MAX(NOF_CORES)) ? SOC_DPP_DEFS_MAX(NOF_CORES) - 1 : 0;
        for (; i0 < SOC_DPP_DEFS_MAX(NOF_CORES); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            {
                int i1;
                i1 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan[i0]
                    , sizeof(*sw_state[unit]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan[i0]), ARAD_PORT_NOF_PP_PORTS) ? ARAD_PORT_NOF_PP_PORTS - 1 : 0;
                for (; i1 < ARAD_PORT_NOF_PP_PORTS; i1++) {
                    shr_sw_state_dump_update_current_idx(unit, i1);
                    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan[i0][i1],
                        "sw_state[%d]->dpp.soc.arad.pp->eg_mirror->eg_mirror_port_vlan[%d][%d]: ", unit, i0, i1);
                }
                shr_sw_state_dump_end_of_stride(unit);
            }
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_eg_mirror_mirror_profile_multi_set_dump(int unit, int pp_idx_0, int eg_mirror_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->eg_mirror->mirror_profile_multi_set,
        "sw_state[%d]->dpp.soc.arad.pp->eg_mirror->mirror_profile_multi_set: ", unit);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 


int
sw_state_dpp_soc_arad_pp_eg_mirror_diagnostic_cb_init(int unit){
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.eg_mirror.dump = sw_state_dpp_soc_arad_pp_eg_mirror_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.eg_mirror.eg_mirror_port_vlan.dump = sw_state_dpp_soc_arad_pp_eg_mirror_eg_mirror_port_vlan_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.eg_mirror.mirror_profile_multi_set.dump = sw_state_dpp_soc_arad_pp_eg_mirror_mirror_profile_multi_set_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_API_TEST */
