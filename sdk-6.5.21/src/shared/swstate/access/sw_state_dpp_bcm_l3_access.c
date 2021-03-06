/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
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
#include <shared/swstate/access/sw_state_dpp_bcm_l3_access.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_alloc(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3,
        sizeof(bcm_dpp_l3_info_t),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.bcm.l3,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.bcm.l3);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_intf_set(int unit, int used_intf){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_intf.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->used_intf,
        used_intf,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_used_intf_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_intf_get(int unit, int *used_intf){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_intf.verify( unit));
    *used_intf = sw_state[unit]->dpp.bcm.l3->used_intf;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_intf_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_vrf_set(int unit, int used_vrf){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_vrf.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->used_vrf,
        used_vrf,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_used_vrf_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_vrf_get(int unit, int *used_vrf){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_vrf.verify( unit));
    *used_vrf = sw_state[unit]->dpp.bcm.l3->used_vrf;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_vrf_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_host_set(int unit, int used_host){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_host.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->used_host,
        used_host,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_used_host_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_host_get(int unit, int *used_host){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_host.verify( unit));
    *used_host = sw_state[unit]->dpp.bcm.l3->used_host;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_host_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_route_set(int unit, int used_route){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_route.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->used_route,
        used_route,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_used_route_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_route_get(int unit, int *used_route){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.used_route.verify( unit));
    *used_route = sw_state[unit]->dpp.bcm.l3->used_route;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_used_route_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_alloc(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state,
        sizeof(bcm_dpp_l3_state_t),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_dpp_l3_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_dpp_l3_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_dpp_l3_state_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.bcm.l3->dpp_l3_state);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_init_set(int unit, int init){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.init.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->init,
        init,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_dpp_l3_state_init_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_init_get(int unit, int *init){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.init.verify( unit));
    *init = sw_state[unit]->dpp.bcm.l3->dpp_l3_state->init;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_init_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_set(int unit, int urpf_mode){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.urpf_mode.verify( unit));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->urpf_mode,
        urpf_mode,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_get(int unit, int *urpf_mode){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.urpf_mode.verify( unit));
    *urpf_mode = sw_state[unit]->dpp.bcm.l3->dpp_l3_state->urpf_mode;
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_set(int unit, int bcm_tunnel_intf_to_eep_idx_0, int bcm_tunnel_intf_to_eep){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.verify( unit, bcm_tunnel_intf_to_eep_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep[bcm_tunnel_intf_to_eep_idx_0],
        bcm_tunnel_intf_to_eep,
        int,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_get(int unit, int bcm_tunnel_intf_to_eep_idx_0, int *bcm_tunnel_intf_to_eep){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.verify( unit, bcm_tunnel_intf_to_eep_idx_0));
    *bcm_tunnel_intf_to_eep = sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep[bcm_tunnel_intf_to_eep_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_alloc(int unit, int nof_instances_to_alloc){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep,
        nof_instances_to_alloc * sizeof(int),
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_is_allocated(int unit, uint8 *is_allocated){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_IS_ALLOC(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep,
        is_allocated,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_free(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_FREE(
        unit,
        sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep,
        SW_STATE_NONE,
        "sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_alloc");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_verify(int unit, int bcm_tunnel_intf_to_eep_idx_0){
    uint32 allocated_size = 0;
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_NULL_ACCESS_CHECK(sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep);
    _SOC_IF_ERR_EXIT(shr_sw_state_allocated_size_get (unit, (uint8*)sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep, &allocated_size));
    SW_STATE_OUT_OF_BOUND_CHECK( bcm_tunnel_intf_to_eep_idx_0, allocated_size /
        sizeof(*(sw_state[unit]->dpp.bcm.l3->dpp_l3_state->bcm_tunnel_intf_to_eep)) - 1, "bcm_tunnel_intf_to_eep");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_types_set(int unit, int vrrp_protocol_set_types_idx_0, uint32 vrrp_protocol_set_types){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_types.verify( unit, vrrp_protocol_set_types_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->vrrp_protocol_set_types[vrrp_protocol_set_types_idx_0],
        vrrp_protocol_set_types,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_vrrp_protocol_set_types_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_types_get(int unit, int vrrp_protocol_set_types_idx_0, uint32 *vrrp_protocol_set_types){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_types.verify( unit, vrrp_protocol_set_types_idx_0));
    *vrrp_protocol_set_types = sw_state[unit]->dpp.bcm.l3->vrrp_protocol_set_types[vrrp_protocol_set_types_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_types_verify(int unit, int vrrp_protocol_set_types_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_OUT_OF_BOUND_CHECK( vrrp_protocol_set_types_idx_0, SOC_DPP_DEFS_MAX(VRRP_NOF_PROTOCOL_GROUPS), "vrrp_protocol_set_types");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_set(int unit, int vrrp_protocol_set_counts_idx_0, uint32 vrrp_protocol_set_counts){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_counts.verify( unit, vrrp_protocol_set_counts_idx_0));
    SHR_SW_STATE_SET(
        unit,
        sw_state[unit]->dpp.bcm.l3->vrrp_protocol_set_counts[vrrp_protocol_set_counts_idx_0],
        vrrp_protocol_set_counts,
        uint32,
        SW_STATE_IS_WB_CHECK | SW_STATE_ALLOW_AUTOSYNC,
        "sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_set");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_get(int unit, int vrrp_protocol_set_counts_idx_0, uint32 *vrrp_protocol_set_counts){
    SW_STATE_INIT_FUNC_DEFS;
    SHR_SW_STATE_VERIFY( sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_counts.verify( unit, vrrp_protocol_set_counts_idx_0));
    *vrrp_protocol_set_counts = sw_state[unit]->dpp.bcm.l3->vrrp_protocol_set_counts[vrrp_protocol_set_counts_idx_0];
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_verify(int unit, int vrrp_protocol_set_counts_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.bcm.l3.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_OUT_OF_BOUND_CHECK( vrrp_protocol_set_counts_idx_0, SOC_DPP_DEFS_MAX(VRRP_NOF_PROTOCOL_GROUPS), "vrrp_protocol_set_counts");
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_bcm_l3_access_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.alloc = sw_state_dpp_bcm_l3_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.is_allocated = sw_state_dpp_bcm_l3_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.free = sw_state_dpp_bcm_l3_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.verify = sw_state_dpp_bcm_l3_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_intf.set = sw_state_dpp_bcm_l3_used_intf_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_intf.get = sw_state_dpp_bcm_l3_used_intf_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_intf.verify = sw_state_dpp_bcm_l3_used_intf_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_vrf.set = sw_state_dpp_bcm_l3_used_vrf_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_vrf.get = sw_state_dpp_bcm_l3_used_vrf_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_vrf.verify = sw_state_dpp_bcm_l3_used_vrf_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_host.set = sw_state_dpp_bcm_l3_used_host_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_host.get = sw_state_dpp_bcm_l3_used_host_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_host.verify = sw_state_dpp_bcm_l3_used_host_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_route.set = sw_state_dpp_bcm_l3_used_route_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_route.get = sw_state_dpp_bcm_l3_used_route_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.used_route.verify = sw_state_dpp_bcm_l3_used_route_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.alloc = sw_state_dpp_bcm_l3_dpp_l3_state_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.is_allocated = sw_state_dpp_bcm_l3_dpp_l3_state_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.free = sw_state_dpp_bcm_l3_dpp_l3_state_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.verify = sw_state_dpp_bcm_l3_dpp_l3_state_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.init.set = sw_state_dpp_bcm_l3_dpp_l3_state_init_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.init.get = sw_state_dpp_bcm_l3_dpp_l3_state_init_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.init.verify = sw_state_dpp_bcm_l3_dpp_l3_state_init_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.urpf_mode.set = sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.urpf_mode.get = sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.urpf_mode.verify = sw_state_dpp_bcm_l3_dpp_l3_state_urpf_mode_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.set = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.get = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.alloc = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.is_allocated = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.free = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.dpp_l3_state.bcm_tunnel_intf_to_eep.verify = sw_state_dpp_bcm_l3_dpp_l3_state_bcm_tunnel_intf_to_eep_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_types.set = sw_state_dpp_bcm_l3_vrrp_protocol_set_types_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_types.get = sw_state_dpp_bcm_l3_vrrp_protocol_set_types_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_types.verify = sw_state_dpp_bcm_l3_vrrp_protocol_set_types_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_counts.set = sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_counts.get = sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_access[unit].dpp.bcm.l3.vrrp_protocol_set_counts.verify = sw_state_dpp_bcm_l3_vrrp_protocol_set_counts_verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
