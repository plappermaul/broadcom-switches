/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/mirror/bcm56780_a0/bcm56780_a0_MIRROR_ENCAP_MIRROR_ON_DROP.map.ltl for
 *      bcm56780_a0
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
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56780_a0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* MIRROR_ENCAP_MIRROR_ON_DROP field init */
static const bcmlrd_field_data_t bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_field_data_mmd[] = {
    { /* 0 MIRROR_ENCAP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 1 UNTAG */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 2 SRC_MAC */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 3 DST_MAC */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 4 SRC_IPV4 */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 5 DST_IPV4 */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 6 TOS */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 7 TTL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 8 PROTO */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 9 DO_NOT_FRAGMENT */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 10 L4_DST_PORT */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 11 L4_SRC_PORT */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 12 ETHERTYPE */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 13 OUTER_VLAN */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 14 PRI */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 15 CFI */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 16 VLAN_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 17 TPID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 18 INITIAL_SEQ_NUM */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 19 OBSERVATION_DOMAIN */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 20 TEMPLATE_IDENTIFIER */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 21 METADATA_TYPE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x1,
      .depth = 0,
      .width = 1,
      .edata = BCM56780_A0_LRD_MIRROR_METADATA_T_DATA,
    },
    { /* 22 METADATA */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 23 IPFIX_VERSION */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 24 SWITCH_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 25 VARIABLE_FLAG */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0xff,
      .max = &bcm56780_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 26 HIGIG3 */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 27 HIGIG3_ETHERTYPE */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 28 HIGIG3_BASE_HDR */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 29 MODE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x1,
      .depth = 0,
      .width = 1,
      .edata = BCM56780_A0_LRD_MIRROR_ON_DROP_MODE_T_DATA,
    },
    { /* 30 TRUNCATE */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 31 TRUNCATE_ACTION */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_MIRROR_TRUNCATE_ACTION_T_DATA,
    },
    { /* 32 MIRROR_TRUNCATE_LENGTH_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x1,
      .def = &bcm56780_a0_lrd_ifd_u32_0x1,
      .max = &bcm56780_a0_lrd_ifd_u32_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_field_data = {
    .fields = 33,
    .field = bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_lrd_mirror_encap_mirror_on_dropt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 16,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 15,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_lrd_mirror_encap_mirror_on_dropt_attr_group = {
    .attributes = 4,
    .attr = bcm56780_a0_lrd_mirror_encap_mirror_on_dropt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_lrd_bcmimm_mirror_encap_mirror_on_drop_entry[] = {
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
            /* handler: bcm56780_a0_lta_bcmimm_mirror_encap_mirror_on_drop_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56780_A0_LTA_BCMIMM_MIRROR_ENCAP_MIRROR_ON_DROP_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_bcmimm_mirror_encap_mirror_on_drop_entry
    },
};
const bcmlrd_map_t bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map = {
    .src_id = MIRROR_ENCAP_MIRROR_ON_DROPt,
    .field_data = &bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_field_data,
    .groups = 1,
    .group  = bcm56780_a0_lrd_mirror_encap_mirror_on_drop_map_group,
    .table_attr = &bcm56780_a0_lrd_mirror_encap_mirror_on_dropt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
