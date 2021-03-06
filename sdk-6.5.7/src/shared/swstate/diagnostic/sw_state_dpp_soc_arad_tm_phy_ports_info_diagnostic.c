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
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_phy_ports_info_diagnostic.h>

#ifdef BCM_WARM_BOOT_API_TEST
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_dump(int unit, int tm_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info, sizeof(*sw_state[unit]->dpp.soc.arad.tm->phy_ports_info), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.initialized.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.phy_ports.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.interface_type.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.speed.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.master_port.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.channels_count.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.is_channelized.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.latch_down.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.runt_pad.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.core.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.is_single_cal_mode.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.high_pirority_cal.dump(unit, tm_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.low_pirority_cal.dump(unit, tm_idx_0, j0);
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
int sw_state_dpp_soc_arad_tm_phy_ports_info_initialized_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].initialized,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].initialized: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_phy_ports_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].phy_ports,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].phy_ports: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_interface_type_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].interface_type,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].interface_type: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_speed_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].speed,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].speed: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_master_port_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].master_port,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].master_port: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_channels_count_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].channels_count,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].channels_count: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_is_channelized_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].is_channelized,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].is_channelized: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_latch_down_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].latch_down,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].latch_down: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_runt_pad_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].runt_pad,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].runt_pad: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_core_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].core,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].core: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_is_single_cal_mode_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].is_single_cal_mode,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].is_single_cal_mode: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_high_pirority_cal_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].high_pirority_cal,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].high_pirority_cal: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_tm_phy_ports_info_low_pirority_cal_dump(int unit, int tm_idx_0, int phy_ports_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.tm->phy_ports_info[phy_ports_info_idx_0].low_pirority_cal,
        "sw_state[%d]->dpp.soc.arad.tm->phy_ports_info[%d].low_pirority_cal: ", unit, phy_ports_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 


int
sw_state_dpp_soc_arad_tm_phy_ports_info_diagnostic_cb_init(int unit){
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.initialized.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_initialized_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.phy_ports.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_phy_ports_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.interface_type.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_interface_type_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.speed.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_speed_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.master_port.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_master_port_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.channels_count.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_channels_count_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.is_channelized.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_is_channelized_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.latch_down.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_latch_down_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.runt_pad.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_runt_pad_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.core.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_core_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.is_single_cal_mode.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_is_single_cal_mode_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.high_pirority_cal.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_high_pirority_cal_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.tm.phy_ports_info.low_pirority_cal.dump = sw_state_dpp_soc_arad_tm_phy_ports_info_low_pirority_cal_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_API_TEST */
