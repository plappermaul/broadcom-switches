/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/lb_hash/bcm56990_b0/bcm56990_b0_LB_HASH_GTP_L4_PORT_MATCH.map.ltl for
 *      bcm56990_b0
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
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56990_b0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56990_b0_lrd_enumpool.h"
/* LB_HASH_GTP_L4_PORT_MATCH field init */
static const bcmlrd_field_data_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_field_data_mmd[] = {
    { /* 0 LB_HASH_GTP_L4_PORT_MATCH_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 1 MATCH */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56990_b0_lrd_ifd_u32_0x0,
      .def = &bcm56990_b0_lrd_ifd_u32_0x0,
      .max = &bcm56990_b0_lrd_ifd_u32_0x4,
      .depth = 0,
      .width = 3,
      .edata = BCMLTD_COMMON_LB_HASH_GTP_L4_PORT_MATCH_T_DATA,
    },
    { /* 2 SRC_L4_PORT */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 3 DST_L4_PORT */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_field_data = {
    .fields = 4,
    .field = bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 3,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_attr_group = {
    .attributes = 3,
    .attr = bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_gtp_port_table_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 1,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = LB_HASH_GTP_L4_PORT_MATCHt_LB_HASH_GTP_L4_PORT_MATCH_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 1,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = MATCH_ENABLEf,
            .field_idx = 0,
            .minbit    = 16,
            .maxbit    = 18,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = LB_HASH_GTP_L4_PORT_MATCHt_MATCHf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 2,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = SRC_PORTf,
            .field_idx = 0,
            .minbit    = 19,
            .maxbit    = 34,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = LB_HASH_GTP_L4_PORT_MATCHt_SRC_L4_PORTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 15,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DST_PORTf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 15,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = LB_HASH_GTP_L4_PORT_MATCHt_DST_L4_PORTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 15,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_group_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = GTP_PORT_TABLEm,
        },
        .entries = 4,
        .entry = bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_gtp_port_table_map_entry
    },
};
const bcmlrd_map_t bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map = {
    .src_id = LB_HASH_GTP_L4_PORT_MATCHt,
    .field_data = &bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_field_data,
    .groups = 1,
    .group  = bcm56990_b0_lrd_lb_hash_gtp_l4_port_match_map_group,
    .table_attr = &bcm56990_b0_lrd_lb_hash_gtp_l4_port_matcht_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
