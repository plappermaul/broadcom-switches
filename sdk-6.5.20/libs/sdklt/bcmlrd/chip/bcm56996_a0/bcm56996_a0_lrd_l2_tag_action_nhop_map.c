/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/l2/bcm56996_a0/bcm56996_a0_L2_TAG_ACTION_NHOP.map.ltl for
 *      bcm56996_a0
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <bcmlrd/bcmlrd_internal.h>
#include <bcmlrd/chip/bcmlrd_id.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56996_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56996_a0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* L2_TAG_ACTION_NHOP field init */
static const bcmlrd_field_data_t bcm56996_a0_lrd_l2_tag_action_nhop_map_field_data_mmd[] = {
    { /* 0 NHOP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56996_a0_lrd_ifd_u16_0x1,
      .def = &bcm56996_a0_lrd_ifd_u16_0x1,
      .max = &bcm56996_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 1 IS_TRUNK */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 2 MODPORT */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x10f,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 3 TRUNK_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 4 VLAN_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 5 ING_UNDERLAY_NHOP_VALID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 6 ING_UNDERLAY_NHOP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 7 DROP */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 8 COPY_TO_CPU */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 9 DO_NOT_CUT_THROUGH */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 10 L3_EIF_VALID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 11 L3_EIF_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x1fff,
      .depth = 0,
      .width = 13,
      .edata = NULL,
    },
    { /* 12 DVP */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x1fff,
      .depth = 0,
      .width = 13,
      .edata = NULL,
    },
    { /* 13 EGR_UNDERLAY_NHOP_VALID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 14 EGR_UNDERLAY_NHOP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 15 VLAN_TAG_PRESENT_ACTION */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x7,
      .depth = 0,
      .width = 3,
      .edata = BCM56996_A0_LRD_VLAN_TAG_PRESENT_ACTION_T_DATA,
    },
    { /* 16 VLAN_TAG_NOT_PRESENT_ACTION */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x1,
      .depth = 0,
      .width = 1,
      .edata = BCM56996_A0_LRD_VLAN_TAG_NOT_PRESENT_ACTION_T_DATA,
    },
    { /* 17 L2_OPAQUE_TAG_PRESENT_ACTION */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x2,
      .depth = 0,
      .width = 2,
      .edata = BCM56996_A0_LRD_L2_OPAQUE_TAG_PRESENT_ACTION_T_DATA,
    },
    { /* 18 L2_OPAQUE_TAG_NOT_PRESENT_ACTION */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x1,
      .depth = 0,
      .width = 1,
      .edata = BCM56996_A0_LRD_L2_OPAQUE_TAG_NOT_PRESENT_ACTION_T_DATA,
    },
    { /* 19 IS_PHB_MAP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 20 REMARK_CFI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 21 OCFI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 22 OPRI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 23 PHB_EGR_L2_INT_PRI_TO_OTAG_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 24 OVID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 25 VLAN_OUTER_TPID_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 26 CTR_EGR_EFLEX_OBJECT */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 27 CTR_EGR_EFLEX_ACTION */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 28 CLASS_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 29 ES_IDENTIFIER */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0x3f,
      .depth = 0,
      .width = 6,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56996_a0_lrd_l2_tag_action_nhop_map_field_data = {
    .fields = 30,
    .field = bcm56996_a0_lrd_l2_tag_action_nhop_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56996_a0_lrd_l2_tag_action_nhopt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_SHARED_INDEX,
        .value = L3_UC_NHOPt,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 32767,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 1,
    },
};

static const bcmlrd_map_attr_t bcm56996_a0_lrd_l2_tag_action_nhopt_attr_group = {
    .attributes = 4,
    .attr = bcm56996_a0_lrd_l2_tag_action_nhopt_attr_entry,
};


const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_src_field_desc_s1[2] = {
    {
        .field_id  = L2_TAG_ACTION_NHOPt_L3_EIF_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 31,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = L2_TAG_ACTION_NHOPt_L3_EIF_VALIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 0,
        .entry_idx = 0,
        .reserved  = 0
    },
};

const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_l2_tag_action_nhop_dst_field_desc_d0[4] = {
    {
        .field_id  = U_INTF_NUM_VALIDf,
        .field_idx = 0,
        .minbit    = 40,
        .maxbit    = 40,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = U_INTF_NUMf,
        .field_idx = 0,
        .minbit    = 27,
        .maxbit    = 39,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = O_INTF_NUM_VALIDf,
        .field_idx = 0,
        .minbit    = 26,
        .maxbit    = 26,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = O_INTF_NUMf,
        .field_idx = 0,
        .minbit    = 13,
        .maxbit    = 25,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_src_field_desc_s0[5] = {
    {
        .field_id  = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_VALIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 0,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = L2_TAG_ACTION_NHOPt_IS_TRUNKf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 0,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = L2_TAG_ACTION_NHOPt_MODPORTf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = L2_TAG_ACTION_NHOPt_TRUNK_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
};

const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_l2_tag_action_nhop_dst_field_desc_d0[5] = {
    {
        .field_id  = U_NHI_VALIDf,
        .field_idx = 0,
        .minbit    = 34,
        .maxbit    = 34,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = U_NHIf,
        .field_idx = 0,
        .minbit    = 14,
        .maxbit    = 28,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = Tf,
        .field_idx = 0,
        .minbit    = 23,
        .maxbit    = 23,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = PORT_NUMf,
        .field_idx = 0,
        .minbit    = 14,
        .maxbit    = 22,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = TGIDf,
        .field_idx = 0,
        .minbit    = 14,
        .maxbit    = 21,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_fwd_s1_k1_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMCTH_L3_UC_NHOP_MAP_XFRM_HANDLER_FWD_S1_K1_D0_ID,
    .src_fields = 2,
    .src = bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_src_field_desc_s1,
    .dst_fields = 4,
    .dst = bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_l2_tag_action_nhop_dst_field_desc_d0,
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_rev_s1_k1_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMCTH_L3_UC_NHOP_MAP_XFRM_HANDLER_REV_S1_K1_D0_ID,
    .src_fields = 4,
    .src = bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_l2_tag_action_nhop_dst_field_desc_d0,
    .dst_fields = 2,
    .dst = bcm56996_a0_lrd_bcmcth_l3_uc_nhop_map_src_field_desc_s1,
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_fwd_s0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMLTX_L3UC_NHOP_OR_PORT_XFRM_HANDLER_FWD_S0_D0_ID,
    .src_fields = 5,
    .src = bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_src_field_desc_s0,
    .dst_fields = 5,
    .dst = bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_l2_tag_action_nhop_dst_field_desc_d0,
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_rev_s0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMLTX_L3UC_NHOP_OR_PORT_XFRM_HANDLER_REV_S0_D0_ID,
    .src_fields = 5,
    .src = bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_l2_tag_action_nhop_dst_field_desc_d0,
    .dst_fields = 5,
    .dst = bcm56996_a0_lrd_bcmltx_l3uc_nhop_or_port_src_field_desc_s0,
};


static const bcmlrd_map_entry_t bcm56996_a0_lrd_l2_tag_action_nhopt_egr_l3_next_hop_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = ENTRY_TYPEf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 4,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_L2_TAG_ACTION_IF_PRESENTf,
            .field_idx = 0,
            .minbit    = 55,
            .maxbit    = 57,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_VLAN_TAG_PRESENT_ACTIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 2,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_L2_TAG_ACTION_IF_NOT_PRESENTf,
            .field_idx = 0,
            .minbit    = 53,
            .maxbit    = 54,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_VLAN_TAG_NOT_PRESENT_ACTIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 1,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_L2_OPAQUE_TAG_ACTION_IF_PRESENTf,
            .field_idx = 0,
            .minbit    = 51,
            .maxbit    = 52,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_L2_OPAQUE_TAG_PRESENT_ACTIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 1,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_L2_OPAQUE_TAG_ACTION_IF_NOT_PRESENTf,
            .field_idx = 0,
            .minbit    = 50,
            .maxbit    = 50,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_L2_OPAQUE_TAG_NOT_PRESENT_ACTIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_OPRI_CFI_SELf,
            .field_idx = 0,
            .minbit    = 59,
            .maxbit    = 59,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_IS_PHB_MAP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_OPRI_MAPPING_PTRf,
            .field_idx = 0,
            .minbit    = 46,
            .maxbit    = 49,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_PHB_EGR_L2_INT_PRI_TO_OTAG_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_OCFIf,
            .field_idx = 0,
            .minbit    = 49,
            .maxbit    = 49,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_OCFIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 9 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_OPRIf,
            .field_idx = 0,
            .minbit    = 46,
            .maxbit    = 48,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_OPRIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 2,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 10 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_L2_TAG_REMARK_CFIf,
            .field_idx = 0,
            .minbit    = 58,
            .maxbit    = 58,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_REMARK_CFIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 11 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_OVIDf,
            .field_idx = 0,
            .minbit    = 60,
            .maxbit    = 71,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_OVIDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 12 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_L2_TAG_SET_TPID_INDEXf,
            .field_idx = 0,
            .minbit    = 72,
            .maxbit    = 73,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_VLAN_OUTER_TPID_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 1,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 13 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_FLEX_CTR_OBJECTf,
            .field_idx = 0,
            .minbit    = 31,
            .maxbit    = 45,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_CTR_EGR_EFLEX_OBJECTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 14 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_FLEX_CTR_ACTIONf,
            .field_idx = 0,
            .minbit    = 27,
            .maxbit    = 30,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_CTR_EGR_EFLEX_ACTIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 15 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_CLASS_IDf,
            .field_idx = 0,
            .minbit    = 9,
            .maxbit    = 20,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_CLASS_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 16 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2_TAGv_ES_IDf,
            .field_idx = 0,
            .minbit    = 21,
            .maxbit    = 26,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_ES_IDENTIFIERf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 5,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56996_a0_lrd_l2_tag_action_nhopt_egr_l3_next_hop_object_map_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = U_NHOP_INDEX_VALIDf,
            .field_idx = 0,
            .minbit    = 56,
            .maxbit    = 56,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_EGR_UNDERLAY_NHOP_VALIDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = U_NHOP_INDEXf,
            .field_idx = 0,
            .minbit    = 41,
            .maxbit    = 55,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_EGR_UNDERLAY_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DVPf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 12,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_DVPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 12,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_FWD_VALUE_FIELD_XFRM_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .xfrm = {
                /* handler: bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_fwd_s1_k1_d0 */
                .desc = &bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_fwd_s1_k1_d0_desc,
            },
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_REV_VALUE_FIELD_XFRM_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .xfrm = {
                /* handler: bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_rev_s1_k1_d0 */
                .desc = &bcm56996_a0_lta_bcmcth_l3_uc_nhop_map_xfrm_handler_rev_s1_k1_d0_desc,
            },
        },
    },
};
static const bcmlrd_map_entry_t bcm56996_a0_lrd_l2_tag_action_nhopt_ing_l3_next_hop_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = ENTRY_TYPEf,
            .field_idx = 0,
            .minbit    = 32,
            .maxbit    = 33,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VLAN_IDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 11,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_VLAN_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DO_NOT_CUT_THROUGHf,
            .field_idx = 0,
            .minbit    = 30,
            .maxbit    = 30,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_DO_NOT_CUT_THROUGHf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = COPY_TO_CPUf,
            .field_idx = 0,
            .minbit    = 29,
            .maxbit    = 29,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_COPY_TO_CPUf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DROPf,
            .field_idx = 0,
            .minbit    = 31,
            .maxbit    = 31,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_TAG_ACTION_NHOPt_DROPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_FWD_VALUE_FIELD_XFRM_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .xfrm = {
                /* handler: bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_fwd_s0_d0 */
                .desc = &bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_fwd_s0_d0_desc,
            },
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_REV_VALUE_FIELD_XFRM_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .xfrm = {
                /* handler: bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_rev_s0_d0 */
                .desc = &bcm56996_a0_lta_bcmltx_l3uc_nhop_or_port_xfrm_handler_rev_s0_d0_desc,
            },
        },
    },
};
const bcmlrd_field_selector_t bcm56996_a0_lrd_l2_tag_action_nhop_map_select[] = {
    { /* Node:0, Type:ROOT, ING_UNDERLAY_NHOP_VALID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_ROOT,
        .field_id = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_VALIDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = BCMLRD_INVALID_SELECTOR_INDEX,
        .group = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_id = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_value = BCMLRD_INVALID_SELECTOR_INDEX
    },
    { /* Node:1, Type:FIELD, IS_TRUNK */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_IS_TRUNKf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_VALIDf,
        .selector_value = FALSE
    },
    { /* Node:2, Type:FIELD, MODPORT */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_MODPORTf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 1,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_IS_TRUNKf,
        .selector_value = FALSE
    },
    { /* Node:3, Type:FIELD, TRUNK_ID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_TRUNK_IDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 1,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_IS_TRUNKf,
        .selector_value = TRUE
    },
    { /* Node:4, Type:FIELD, ING_UNDERLAY_NHOP_ID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_IDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_ING_UNDERLAY_NHOP_VALIDf,
        .selector_value = TRUE
    },
    { /* Node:5, Type:ROOT, IS_PHB_MAP_ID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_ROOT,
        .field_id = L2_TAG_ACTION_NHOPt_IS_PHB_MAP_IDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = BCMLRD_INVALID_SELECTOR_INDEX,
        .group = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_id = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_value = BCMLRD_INVALID_SELECTOR_INDEX
    },
    { /* Node:6, Type:FIELD, PHB_EGR_L2_INT_PRI_TO_OTAG_ID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_PHB_EGR_L2_INT_PRI_TO_OTAG_IDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 5,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_IS_PHB_MAP_IDf,
        .selector_value = TRUE
    },
    { /* Node:7, Type:FIELD, OCFI */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_OCFIf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 5,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_IS_PHB_MAP_IDf,
        .selector_value = FALSE
    },
    { /* Node:8, Type:FIELD, OPRI */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L2_TAG_ACTION_NHOPt_OPRIf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 5,
        .group = 0,
        .selector_id = L2_TAG_ACTION_NHOPt_IS_PHB_MAP_IDf,
        .selector_value = FALSE
    },
};

const bcmlrd_field_selector_data_t bcm56996_a0_lrd_l2_tag_action_nhop_map_select_data = {
    .num_field_selector = 9,
    .field_selector = bcm56996_a0_lrd_l2_tag_action_nhop_map_select,
};

static const bcmlrd_map_group_t bcm56996_a0_lrd_l2_tag_action_nhop_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_L3_NEXT_HOPm,
        },
        .entries = 17,
        .entry = bcm56996_a0_lrd_l2_tag_action_nhopt_egr_l3_next_hop_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_L3_NEXT_HOP_OBJECT_MAPm,
        },
        .entries = 6,
        .entry = bcm56996_a0_lrd_l2_tag_action_nhopt_egr_l3_next_hop_object_map_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = ING_L3_NEXT_HOPm,
        },
        .entries = 8,
        .entry = bcm56996_a0_lrd_l2_tag_action_nhopt_ing_l3_next_hop_map_entry
    },
};
const bcmlrd_map_t bcm56996_a0_lrd_l2_tag_action_nhop_map = {
    .src_id = L2_TAG_ACTION_NHOPt,
    .field_data = &bcm56996_a0_lrd_l2_tag_action_nhop_map_field_data,
    .groups = 3,
    .group  = bcm56996_a0_lrd_l2_tag_action_nhop_map_group,
    .table_attr = &bcm56996_a0_lrd_l2_tag_action_nhopt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE,
    .sel = &bcm56996_a0_lrd_l2_tag_action_nhop_map_select_data,
};
