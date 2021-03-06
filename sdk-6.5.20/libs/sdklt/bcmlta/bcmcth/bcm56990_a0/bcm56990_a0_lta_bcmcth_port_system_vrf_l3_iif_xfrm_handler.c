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
/* Logical Table Adaptor for component bcmcth */
/* Handler: bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmcth/bcmcth_port_system_vrf_l3_iif.h>
#include <bcmdrd/chip/bcm56990_a0_enum.h>
#include <bcmlrd/chip/bcm56990_a0/bcm56990_a0_lrd_xfrm_field_desc.h>

extern const bcmltd_field_desc_t
bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_src_field_desc_s0[];

static const bcmltd_field_desc_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_field_desc_k0[1] = {
    {
        .field_id  = PORT_SYSTEMt_PORT_SYSTEM_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
};

extern const bcmltd_field_desc_t
bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_dst_field_desc_d0[];

static const
bcmltd_field_list_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_s0 = {
    .field_num = 4,
    .field_array = bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_src_field_desc_s0
};

static const
bcmltd_field_list_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_k0 = {
    .field_num = 1,
    .field_array = bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_field_desc_k0
};

static const bcmltd_field_list_t
bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_dst_list_d0 = {
    .field_num = 1,
    .field_array = bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_dst_field_desc_d0
};

static const uint32_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_s0[4] = {
    PORT_SYSTEMt_OPERATING_MODEf,
    PORT_SYSTEMt_L3_IIF_IDf,
    PORT_SYSTEMt_VRF_IDf,
    PORT_SYSTEMt_VLAN_IDf,
};

static const uint32_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_k0[1] = {
    PORT_SYSTEMt_PORT_SYSTEM_IDf,
};

static const uint32_t
bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_transform_dst_d0[1] = {
    BASE_VRF_L3IIF_OVIDf,
};

static const bcmltd_generic_arg_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_comp_data = {
    .sid       = PORT_SYSTEMt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_fwd_arg_s0_k0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 4,
    .field       = bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_s0,
    .field_list  = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_s0,
    .kfields     = 1,
    .kfield      = bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_k0,
    .kfield_list = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_k0,
    .rfields     = 1,
    .rfield      = bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_transform_dst_d0,
    .rfield_list = &bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_dst_list_d0,
    .comp_data   = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_comp_data
};

static const bcmltd_transform_arg_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_rev_arg_s0_k0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_transform_dst_d0,
    .field_list  = &bcm56990_a0_lrd_bcmcth_port_system_vrf_l3_iif_port_system_dst_list_d0,
    .kfields     = 1,
    .kfield      = bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_k0,
    .kfield_list = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_k0,
    .rfields     = 4,
    .rfield      = bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_transform_src_s0,
    .rfield_list = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_src_list_s0,
    .comp_data   = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_comp_data
};

const bcmltd_xfrm_handler_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_fwd_s0_k0_d0 = {
    .ext_transform = bcmcth_port_system_vrf_l3_iif_transform,
    .arg           = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_fwd_arg_s0_k0_d0
};

const bcmltd_xfrm_handler_t
bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_rev_s0_k0_d0 = {
    .ext_transform = bcmcth_port_system_vrf_l3_iif_rev_transform,
    .arg           = &bcm56990_a0_lta_bcmcth_port_system_vrf_l3_iif_xfrm_handler_rev_arg_s0_k0_d0
};


