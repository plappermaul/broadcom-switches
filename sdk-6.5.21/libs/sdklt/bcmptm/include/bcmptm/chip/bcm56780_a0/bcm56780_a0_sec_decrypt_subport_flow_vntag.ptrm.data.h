/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by c-struct-gen from PTRM data files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/c-struct-gen
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_sid_data_only[1][1];
static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_tcam_shared_lt_array[14];
static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_tcam_sid[1][1];
static const bcmdrd_sid_t * bcm56780_a0_subport_flow_vntag_sid_data_only_array[1];
static const bcmdrd_sid_t * bcm56780_a0_subport_flow_vntag_tcam_sid_array[1];
static const bcmptm_rm_tcam_hw_entry_info_t bcm56780_a0_subport_flow_vntag_rm_tcam_hw_entry_info[1];
static const bcmptm_rm_tcam_more_info_t bcm56780_a0_subport_flow_vntag_rm_tcam_more_info[1][1];
static const lt_mreq_info_t bcm56780_a0_subport_flow_vntag_lt_mreq_info;
static const uint16_t bcm56780_a0_subport_flow_vntag_key_type_val[1][1];
static const uint16_t bcm56780_a0_subport_flow_vntag_tcam_data_end_bit_array[1];
static const uint16_t * bcm56780_a0_subport_flow_vntag_key_type_val_array[1];

static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_sid_data_only[1][1] = {
    {
    SUB_PORT_MAP_TABLEm,
    },
};
static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_tcam_shared_lt_array[14] = {
    SEC_DECRYPT_SUBPORT_FLOW_ETHERNET_IIt,
    SEC_DECRYPT_SUBPORT_FLOW_SNAPt,
    SEC_DECRYPT_SUBPORT_FLOW_LLCt,
    SEC_DECRYPT_SUBPORT_FLOW_MPLSt,
    SEC_DECRYPT_SUBPORT_FLOW_PBBt,
    SEC_DECRYPT_SUBPORT_FLOW_VNTAGt,
    SEC_DECRYPT_SUBPORT_FLOW_ETAGt,
    SEC_DECRYPT_SUBPORT_FLOW_IPV4t,
    SEC_DECRYPT_SUBPORT_FLOW_UDP_OVER_IPV4t,
    SEC_DECRYPT_SUBPORT_FLOW_TCP_OVER_IPV4t,
    SEC_DECRYPT_SUBPORT_FLOW_IPV6t,
    SEC_DECRYPT_SUBPORT_FLOW_UDP_OVER_IPV6t,
    SEC_DECRYPT_SUBPORT_FLOW_TCP_OVER_IPV6t,
    SEC_DECRYPT_SUBPORT_FLOWt,
};
static const bcmdrd_sid_t bcm56780_a0_subport_flow_vntag_tcam_sid[1][1] = {
    {
    ISEC_SP_TCAMm,
    },
};
static const bcmdrd_sid_t * bcm56780_a0_subport_flow_vntag_sid_data_only_array[1] = {
    bcm56780_a0_subport_flow_vntag_sid_data_only[0],
};
static const bcmdrd_sid_t * bcm56780_a0_subport_flow_vntag_tcam_sid_array[1] = {
    bcm56780_a0_subport_flow_vntag_tcam_sid[0],
};
static const bcmptm_rm_tcam_hw_entry_info_t bcm56780_a0_subport_flow_vntag_rm_tcam_hw_entry_info[1] = {
    {
        .data_end_bit =     bcm56780_a0_subport_flow_vntag_tcam_data_end_bit_array,
        /* implicit length assignment suppressed */
        .data_start_bit = 0,
        .group_mode = 0,
        .hw_dtype_field_start_bit = 0,
        .hw_dtype_field_width = 0,
        .hw_ktype_field_info = 0,
        .key_size = 472,
        .key_type_val =     bcm56780_a0_subport_flow_vntag_key_type_val_array,
        /* implicit length assignment suppressed */
        .num_data_rows = 1,
        .num_depth_inst = 1,
        .num_key_rows = 1,
        .sid =     bcm56780_a0_subport_flow_vntag_tcam_sid_array,
        /* implicit length assignment suppressed */
        .sid_data_only =     bcm56780_a0_subport_flow_vntag_sid_data_only_array,
        /* implicit length assignment suppressed */
    },
};
static const bcmptm_rm_tcam_more_info_t bcm56780_a0_subport_flow_vntag_rm_tcam_more_info[1][1] = {
    {
        {
            .flags = 0,
            .hw_atomicity_support = 1,
            .is_per_block = 1,
            .pipe_count = 6,
            .shared_lt_count = 14,
            .shared_lt_info =     bcm56780_a0_subport_flow_vntag_tcam_shared_lt_array,
            /* implicit length assignment suppressed */
        },
    },
};
static const lt_mreq_info_t bcm56780_a0_subport_flow_vntag_lt_mreq_info = {
    .aggr_ekw_edw = 0,
    .hw_entry_info =     bcm56780_a0_subport_flow_vntag_rm_tcam_hw_entry_info,
    .hw_entry_info_count = 1,/* implicit length assignment */
    .num_edw = 1,
    .num_ekw = 1,
    .pt_type = LT_PT_TYPE_TCAM_PRI_ONLY,
    .rm_more_info =     bcm56780_a0_subport_flow_vntag_rm_tcam_more_info,
    .rm_more_info_count = 1,/* implicit length assignment */
};
static const uint16_t bcm56780_a0_subport_flow_vntag_key_type_val[1][1] = {
    {
    0,
    },
};
static const uint16_t bcm56780_a0_subport_flow_vntag_tcam_data_end_bit_array[1] = {
    11,
};
static const uint16_t * bcm56780_a0_subport_flow_vntag_key_type_val_array[1] = {
    bcm56780_a0_subport_flow_vntag_key_type_val[0],
};

