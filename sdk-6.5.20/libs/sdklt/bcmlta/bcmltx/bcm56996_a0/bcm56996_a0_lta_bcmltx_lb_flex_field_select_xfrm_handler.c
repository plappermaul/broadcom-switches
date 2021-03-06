/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from Logical Table mapping files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/* Logical Table Adaptor for component bcmltx */
/* Handler: bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmltx/bcmlb/bcmltx_lb_flex_field_select.h>
#include <bcmdrd/chip/bcm56996_a0_enum.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_xfrm_field_desc.h>

static const uint32_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_arg0[1] = {
    11,
};

extern const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_lb_flex_field_select_src_field_desc_s0[];

extern const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_dst_field_desc_d0[];

static const
bcmltd_field_list_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_src_list_s0 = {
    .field_num = 2,
    .field_array = bcm56996_a0_lrd_bcmltx_lb_flex_field_select_src_field_desc_s0
};

static const bcmltd_field_list_t
bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_dst_list_d0 = {
    .field_num = 22,
    .field_array = bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_dst_field_desc_d0
};

static const uint32_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_src_s0[2] = {
    LB_HASH_BINS_ASSIGNMENTt_HASH0_BIN_FLEX_FIELD_SELECTf,
    LB_HASH_BINS_ASSIGNMENTt_HASH1_BIN_FLEX_FIELD_SELECTf,
};

static const uint32_t
bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_transform_dst_d0[22] = {
    ENABLE_FLEX_FIELD_3_Af,
    ENABLE_FLEX_FIELD_4_Af,
    ENABLE_FLEX_FIELD_5_Af,
    ENABLE_FLEX_FIELD_6_Af,
    ENABLE_FLEX_FIELD_7_Af,
    ENABLE_FLEX_FIELD_8_Af,
    ENABLE_FLEX_FIELD_9_Af,
    ENABLE_FLEX_FIELD_10_Af,
    ENABLE_FLEX_FIELD_11_Af,
    ENABLE_FLEX_FIELD_12_Af,
    ENABLE_FLEX_FIELD_13_Af,
    ENABLE_FLEX_FIELD_3_Bf,
    ENABLE_FLEX_FIELD_4_Bf,
    ENABLE_FLEX_FIELD_5_Bf,
    ENABLE_FLEX_FIELD_6_Bf,
    ENABLE_FLEX_FIELD_7_Bf,
    ENABLE_FLEX_FIELD_8_Bf,
    ENABLE_FLEX_FIELD_9_Bf,
    ENABLE_FLEX_FIELD_10_Bf,
    ENABLE_FLEX_FIELD_11_Bf,
    ENABLE_FLEX_FIELD_12_Bf,
    ENABLE_FLEX_FIELD_13_Bf,
};

static const bcmltd_generic_arg_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_comp_data = {
    .sid       = LB_HASH_BINS_ASSIGNMENTt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_fwd_arg_s0_d0_x0 = {
    .values      = 1,
    .value       = bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_arg0,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_src_s0,
    .field_list  = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_src_list_s0,
    .rfields     = 22,
    .rfield      = bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_transform_dst_d0,
    .rfield_list = &bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_dst_list_d0,
    .comp_data   = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_comp_data
};

static const bcmltd_transform_arg_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_rev_arg_s0_d0_x0 = {
    .values      = 1,
    .value       = bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_arg0,
    .tables      = 0,
    .table       = NULL,
    .fields      = 22,
    .field       = bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_transform_dst_d0,
    .field_list  = &bcm56996_a0_lrd_bcmltx_lb_flex_field_select_lb_hash_bins_assignment_dst_list_d0,
    .rfields     = 2,
    .rfield      = bcm56996_a0_lta_bcmltx_lb_flex_field_select_transform_src_s0,
    .rfield_list = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_src_list_s0,
    .comp_data   = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_comp_data
};

const bcmltd_xfrm_handler_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_fwd_s0_d0_x0 = {
    .transform     = bcmltx_lb_flex_field_select_transform,
    .arg           = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_fwd_arg_s0_d0_x0
};

const bcmltd_xfrm_handler_t
bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_rev_s0_d0_x0 = {
    .transform     = bcmltx_lb_flex_field_select_rev_transform,
    .arg           = &bcm56996_a0_lta_bcmltx_lb_flex_field_select_xfrm_handler_rev_arg_s0_d0_x0
};


