/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/mon/bcm56880_a0/bcm56880_a0_MON_FLOWTRACKER_FLOW_DATA.map.ltl for
 *      bcm56880_a0
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
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56880_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56880_a0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* MON_FLOWTRACKER_FLOW_DATA field init */
static const bcmlrd_field_data_t bcm56880_a0_lrd_mon_flowtracker_flow_data_map_field_data_mmd[] = {
    { /* 0 MON_FLOWTRACKER_GROUP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 1 FLOW_KEY_SRC_IPV4 */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 2 FLOW_KEY_DST_IPV4 */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 3 FLOW_KEY_SRC_L4_PORT */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 4 FLOW_KEY_DST_L4_PORT */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 5 FLOW_KEY_IP_PROTO */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 6 FLOW_KEY_INNER_SRC_IPV4 */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 7 FLOW_KEY_INNER_DST_IPV4 */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 8 FLOW_KEY_INNER_SRC_L4_PORT */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 9 FLOW_KEY_INNER_DST_L4_PORT */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 10 FLOW_KEY_INNER_IP_PROTO */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 11 FLOW_KEY_VNID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 12 FLOW_KEY_CUSTOM_KEY_UPPER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 13 FLOW_KEY_CUSTOM_KEY_LOWER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 14 FLOW_KEY_ING_PORT_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 15 FLOW_KEY_INNER_SRC_IPV6_UPPER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 16 FLOW_KEY_INNER_SRC_IPV6_LOWER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 17 FLOW_KEY_INNER_DST_IPV6_UPPER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 18 FLOW_KEY_INNER_DST_IPV6_LOWER */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 19 DATA_PKT_COUNT */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 20 DATA_BYTE_COUNT */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 21 DATA_FLOW_START_TIMESTAMP_MSECS */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 22 DATA_OBSERVATION_TIMESTAMP_MSECS */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56880_a0_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 23 STATIC */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_lrd_mon_flowtracker_flow_data_map_field_data = {
    .fields = 24,
    .field = bcm56880_a0_lrd_mon_flowtracker_flow_data_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_lrd_mon_flowtracker_flow_datat_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 32768,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = TRUE,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_lrd_mon_flowtracker_flow_datat_attr_group = {
    .attributes = 2,
    .attr = bcm56880_a0_lrd_mon_flowtracker_flow_datat_attr_entry,
};

static const bcmlrd_map_entry_t bcm56880_a0_lrd_bcmcth_mon_flowtracker_flow_data_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_TABLE_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            /* handler: bcm56880_a0_lta_bcmcth_mon_flowtracker_flow_data_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56880_A0_LTA_BCMCTH_MON_FLOWTRACKER_FLOW_DATA_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56880_a0_lrd_mon_flowtracker_flow_data_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56880_a0_lrd_bcmcth_mon_flowtracker_flow_data_entry
    },
};
const bcmlrd_map_t bcm56880_a0_lrd_mon_flowtracker_flow_data_map = {
    .src_id = MON_FLOWTRACKER_FLOW_DATAt,
    .field_data = &bcm56880_a0_lrd_mon_flowtracker_flow_data_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_lrd_mon_flowtracker_flow_data_map_group,
    .table_attr = &bcm56880_a0_lrd_mon_flowtracker_flow_datat_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE
};
