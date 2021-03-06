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
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_logical_ports_info_diagnostic.h>

#ifdef BCM_WARM_BOOT_API_TEST
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_dump(int unit, int tm_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info, sizeof(*sw_state[unit]->dpp.soc.arad.tm->logical_ports_info), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.valid.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.first_phy_port.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.channel.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.protocol_offset.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.flags.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.core.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.encap_mode.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.tm_port.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.pp_port.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.priority_mode.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.base_q_pair.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.multicast_offset.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.egr_interface.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.outlif_ext_en.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.src_ext_en.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.dst_ext_en.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.first_header_size.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.header_type_out.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.header_type_in.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.pp_flags.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.fc_type.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.mirror_profile.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_tm_src_syst_port_ext_present.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_stag_enabled.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_snoop_enabled.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_tm_ing_shaping_enabled.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.shaper_mode.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.peer_tm_domain.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.timestamp_and_ssp.dump(unit, tm_idx_0, j0);
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
int sw_state_dpp_soc_arad_tm_logical_ports_info_valid_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].valid,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].valid: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_first_phy_port_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].first_phy_port,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].first_phy_port: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_channel_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].channel,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].channel: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_protocol_offset_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].protocol_offset,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].protocol_offset: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_flags_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].flags,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].flags: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_core_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].core,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].core: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_encap_mode_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].encap_mode,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].encap_mode: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_tm_port_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].tm_port,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].tm_port: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_pp_port_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].pp_port,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].pp_port: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_priority_mode_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].priority_mode,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].priority_mode: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_base_q_pair_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].base_q_pair,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].base_q_pair: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_multicast_offset_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].multicast_offset,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].multicast_offset: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_egr_interface_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].egr_interface,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].egr_interface: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_outlif_ext_en_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].outlif_ext_en,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].outlif_ext_en: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_src_ext_en_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].src_ext_en,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].src_ext_en: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_dst_ext_en_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].dst_ext_en,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].dst_ext_en: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_first_header_size_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].first_header_size,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].first_header_size: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_header_type_out_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].header_type_out,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].header_type_out: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_header_type_in_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].header_type_in,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].header_type_in: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_pp_flags_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].pp_flags,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].pp_flags: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_fc_type_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].fc_type,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].fc_type: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_mirror_profile_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].mirror_profile,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].mirror_profile: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_is_tm_src_syst_port_ext_present_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].is_tm_src_syst_port_ext_present,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].is_tm_src_syst_port_ext_present: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_is_stag_enabled_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].is_stag_enabled,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].is_stag_enabled: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_is_snoop_enabled_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].is_snoop_enabled,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].is_snoop_enabled: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_is_tm_ing_shaping_enabled_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].is_tm_ing_shaping_enabled,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].is_tm_ing_shaping_enabled: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_shaper_mode_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].shaper_mode,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].shaper_mode: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_peer_tm_domain_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].peer_tm_domain,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].peer_tm_domain: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_logical_ports_info_timestamp_and_ssp_dump(int unit, int tm_idx_0, int logical_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->logical_ports_info[logical_ports_info_idx_0].timestamp_and_ssp,
        "sw_state[%d]->dpp.soc.arad.tm->logical_ports_info[%d].timestamp_and_ssp: ", unit, logical_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_soc_arad_tm_logical_ports_info_diagnostic_cb_init(int unit){
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.valid.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_valid_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.first_phy_port.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_first_phy_port_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.channel.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_channel_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.protocol_offset.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_protocol_offset_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.flags.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_flags_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.core.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_core_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.encap_mode.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_encap_mode_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.tm_port.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_tm_port_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.pp_port.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_pp_port_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.priority_mode.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_priority_mode_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.base_q_pair.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_base_q_pair_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.multicast_offset.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_multicast_offset_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.egr_interface.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_egr_interface_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.outlif_ext_en.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_outlif_ext_en_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.src_ext_en.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_src_ext_en_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.dst_ext_en.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_dst_ext_en_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.first_header_size.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_first_header_size_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.header_type_out.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_header_type_out_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.header_type_in.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_header_type_in_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.pp_flags.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_pp_flags_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.fc_type.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_fc_type_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.mirror_profile.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_mirror_profile_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_tm_src_syst_port_ext_present.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_is_tm_src_syst_port_ext_present_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_stag_enabled.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_is_stag_enabled_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_snoop_enabled.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_is_snoop_enabled_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.is_tm_ing_shaping_enabled.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_is_tm_ing_shaping_enabled_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.shaper_mode.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_shaper_mode_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.peer_tm_domain.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_peer_tm_domain_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
    sw_state_diagnostic[unit].dpp.soc.arad.tm.logical_ports_info.timestamp_and_ssp.dump = sw_state_dpp_soc_arad_tm_logical_ports_info_timestamp_and_ssp_dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_API_TEST */
