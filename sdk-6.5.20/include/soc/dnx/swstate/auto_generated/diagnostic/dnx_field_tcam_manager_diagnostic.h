
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef __DNX_FIELD_TCAM_MANAGER_DIAGNOSTIC_H__
#define __DNX_FIELD_TCAM_MANAGER_DIAGNOSTIC_H__

#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/dnx_sw_state_size.h>
#include <soc/dnxc/swstate/dnx_sw_state_diagnostic_operation_counters.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_field_tcam_manager_types.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_field_types.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum {
    DNX_FIELD_TCAM_MANAGER_SW_INFO,
    DNX_FIELD_TCAM_MANAGER_SW_TCAM_HANDLERS_INFO,
    DNX_FIELD_TCAM_MANAGER_SW_TCAM_HANDLERS_MODE_INFO,
    DNX_FIELD_TCAM_MANAGER_SW_TCAM_HANDLERS_STATE_INFO,
    DNX_FIELD_TCAM_MANAGER_SW_INFO_NOF_ENTRIES
} sw_state_dnx_field_tcam_manager_sw_layout_e;


extern dnx_sw_state_diagnostic_info_t dnx_field_tcam_manager_sw_info[SOC_MAX_NUM_DEVICES][DNX_FIELD_TCAM_MANAGER_SW_INFO_NOF_ENTRIES];

extern const char* dnx_field_tcam_manager_sw_layout_str[DNX_FIELD_TCAM_MANAGER_SW_INFO_NOF_ENTRIES];
int dnx_field_tcam_manager_sw_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_field_tcam_manager_sw_tcam_handlers_dump(
    int unit, int tcam_handlers_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_tcam_manager_sw_tcam_handlers_mode_dump(
    int unit, int tcam_handlers_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_tcam_manager_sw_tcam_handlers_state_dump(
    int unit, int tcam_handlers_idx_0, dnx_sw_state_dump_filters_t filters);

#endif 

#endif 
