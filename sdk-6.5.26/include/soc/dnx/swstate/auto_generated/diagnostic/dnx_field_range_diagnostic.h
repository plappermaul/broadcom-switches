
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef __DNX_FIELD_RANGE_DIAGNOSTIC_H__
#define __DNX_FIELD_RANGE_DIAGNOSTIC_H__

#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/dnx_sw_state_size.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_field_range_types.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_field_types.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum {
    DNX_FIELD_RANGE_SW_DB_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_RANGE_TYPES_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_DNX_QUAL_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_QUAL_INFO_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_QUAL_INFO_INPUT_TYPE_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_QUAL_INFO_INPUT_ARG_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_QUAL_INFO_OFFSET_INFO,
    DNX_FIELD_RANGE_SW_DB_EXT_L4_OPS_FFC_QUALS_QUAL_INFO_BASE_QUAL_INFO,
    DNX_FIELD_RANGE_SW_DB_INFO_NOF_ENTRIES
} sw_state_dnx_field_range_sw_db_layout_e;


extern dnx_sw_state_diagnostic_info_t dnx_field_range_sw_db_info[SOC_MAX_NUM_DEVICES][DNX_FIELD_RANGE_SW_DB_INFO_NOF_ENTRIES];

extern const char* dnx_field_range_sw_db_layout_str[DNX_FIELD_RANGE_SW_DB_INFO_NOF_ENTRIES];
int dnx_field_range_sw_db_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_range_types_dump(
    int unit, int ext_l4_ops_range_types_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_dnx_qual_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_qual_info_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_qual_info_input_type_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_qual_info_input_arg_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_qual_info_offset_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_field_range_sw_db_ext_l4_ops_ffc_quals_qual_info_base_qual_dump(
    int unit, int ext_l4_ops_ffc_quals_idx_0, dnx_sw_state_dump_filters_t filters);

#endif 

#endif 
