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
/* Handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmltx/bcmltx_field_demux_wide.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmlrd/chip/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_lrd_xfrm_field_desc.h>


extern const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_src_field_desc_s0[];

static const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_dst_field_desc[8] = {
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 153,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 151,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_0_ONLYf.ENTRY0[151:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 154,
        .field_idx = 0,
        .minbit    = 124,
        .maxbit    = 135,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_0_ONLYf.ENTRY0[135:124]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 155,
        .field_idx = 0,
        .minbit    = 186,
        .maxbit    = 201,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_0_ONLYf.ENTRY0[201:186]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 156,
        .field_idx = 0,
        .minbit    = 162,
        .maxbit    = 185,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_0_ONLYf.ENTRY0[185:162]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 165,
        .field_idx = 0,
        .minbit    = 346,
        .maxbit    = 361,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_1_ONLYf.ENTRY0[151:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 166,
        .field_idx = 0,
        .minbit    = 322,
        .maxbit    = 345,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_1_ONLYf.ENTRY0[135:112]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 167,
        .field_idx = 0,
        .minbit    = 396,
        .maxbit    = 411,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_1_ONLYf.ENTRY0[201:186]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 168,
        .field_idx = 0,
        .minbit    = 372,
        .maxbit    = 375,
        .entry_idx = 0,
        .sym       = "MEMDB_TCAM_IFTA80_MEM0_0_MEM0_1m.TCAM_1_ONLYf.ENTRY0[165:162]",
        .reserved  = 0
    },
};

static const
bcmltd_field_list_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_src_list_s0 = {
    .field_num = 8,
    .field_array = bcm56780_a0_ina_2_4_13_lrd_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_src_field_desc_s0
};

static const bcmltd_field_list_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_dst_list_d0 = {
    .field_num = 8,
    .field_array = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_dst_field_desc
};

static const uint32_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_src_s0[1] = {
    BCM56780_A0_INA_2_4_13_L3_IPV6_UNICAST_DEFIP_128_TABLEt_IPV6_ADDR_MASKf,
};

static const uint32_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_dst_d0[8] = {
    BCMLTD_INTERNAL_FIELD_BASE + 153,
    BCMLTD_INTERNAL_FIELD_BASE + 154,
    BCMLTD_INTERNAL_FIELD_BASE + 155,
    BCMLTD_INTERNAL_FIELD_BASE + 156,
    BCMLTD_INTERNAL_FIELD_BASE + 165,
    BCMLTD_INTERNAL_FIELD_BASE + 166,
    BCMLTD_INTERNAL_FIELD_BASE + 167,
    BCMLTD_INTERNAL_FIELD_BASE + 168,
};

static const bcmltd_generic_arg_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_comp_data = {
    .sid       = BCM56780_A0_INA_2_4_13_L3_IPV6_UNICAST_DEFIP_128_TABLEt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_fwd_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_src_s0,
    .field_list  = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_src_list_s0,
    .rfields     = 8,
    .rfield      = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_dst_d0,
    .rfield_list = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_dst_list_d0,
    .comp_data   = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_comp_data
};

static const bcmltd_transform_arg_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_rev_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 8,
    .field       = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_dst_d0,
    .field_list  = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_dst_list_d0,
    .rfields     = 1,
    .rfield      = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_transform_src_s0,
    .rfield_list = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_src_list_s0,
    .comp_data   = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_comp_data
};

const bcmltd_xfrm_handler_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_fwd_s0_d0 = {
    .transform     = bcmltx_field_demux_wide_transform,
    .arg           = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_fwd_arg_s0_d0
};

const bcmltd_xfrm_handler_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_rev_s0_d0 = {
    .transform     = bcmltx_field_mux_wide_transform,
    .arg           = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_wide_l3_ipv6_unicast_defip_128_tablet_ipv6_addr_maskf_0_xfrm_handler_rev_arg_s0_d0
};


