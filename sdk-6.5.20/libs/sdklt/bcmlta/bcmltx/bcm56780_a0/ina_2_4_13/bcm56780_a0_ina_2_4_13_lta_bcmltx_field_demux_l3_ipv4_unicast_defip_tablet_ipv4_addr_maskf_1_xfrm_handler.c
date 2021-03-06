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
/* Handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmltx/bcmltx_field_demux.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmlrd/chip/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_lrd_xfrm_field_desc.h>


extern const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_src_field_desc_s1[];

static const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_dst_field_desc[4] = {
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 95,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 151,
        .entry_idx = 1,
        .sym       = "MEMDB_TCAM_IFTA80_MEM6_0m.TCAM_ONLYf.ENTRY1[151:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 96,
        .field_idx = 0,
        .minbit    = 124,
        .maxbit    = 135,
        .entry_idx = 1,
        .sym       = "MEMDB_TCAM_IFTA80_MEM6_0m.TCAM_ONLYf.ENTRY1[135:124]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 97,
        .field_idx = 0,
        .minbit    = 110,
        .maxbit    = 111,
        .entry_idx = 1,
        .sym       = "MEMDB_TCAM_IFTA80_MEM6_0m.TCAM_ONLYf.ENTRY1[111:110]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 98,
        .field_idx = 0,
        .minbit    = 106,
        .maxbit    = 107,
        .entry_idx = 1,
        .sym       = "MEMDB_TCAM_IFTA80_MEM6_0m.TCAM_ONLYf.ENTRY1[107:106]",
        .reserved  = 0
    },
};

static const
bcmltd_field_list_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_src_list_s0 = {
    .field_num = 4,
    .field_array = bcm56780_a0_ina_2_4_13_lrd_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_src_field_desc_s1
};

static const bcmltd_field_list_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_dst_list_d0 = {
    .field_num = 4,
    .field_array = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_dst_field_desc
};

static const uint32_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_transform_src_s0[1] = {
    BCM56780_A0_INA_2_4_13_L3_IPV4_UNICAST_DEFIP_TABLEt_IPV4_ADDR_MASKf,
};

static const uint32_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_transform_dst_d0[4] = {
    BCMLTD_INTERNAL_FIELD_BASE + 95,
    BCMLTD_INTERNAL_FIELD_BASE + 96,
    BCMLTD_INTERNAL_FIELD_BASE + 97,
    BCMLTD_INTERNAL_FIELD_BASE + 98,
};

static const bcmltd_generic_arg_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_comp_data = {
    .sid       = BCM56780_A0_INA_2_4_13_L3_IPV4_UNICAST_DEFIP_TABLEt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_xfrm_handler_fwd_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_transform_src_s0,
    .field_list  = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_src_list_s0,
    .rfields     = 4,
    .rfield      = bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_transform_dst_d0,
    .rfield_list = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_dst_list_d0,
    .comp_data   = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_comp_data
};

const bcmltd_xfrm_handler_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_xfrm_handler_fwd_s0_d0 = {
    .transform     = bcmltx_field_demux_transform,
    .arg           = &bcm56780_a0_ina_2_4_13_lta_bcmltx_field_demux_l3_ipv4_unicast_defip_tablet_ipv4_addr_maskf_1_xfrm_handler_fwd_arg_s0_d0
};


