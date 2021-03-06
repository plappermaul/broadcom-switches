/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/mon/bcm56780_a0/bcm56780_a0_MON_FLOWTRACKER_GROUP.map.ltl for
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
/* MON_FLOWTRACKER_GROUP field init */
static const bcmlrd_field_data_t bcm56780_a0_lrd_mon_flowtracker_group_map_field_data_mmd[] = {
    { /* 0 MON_FLOWTRACKER_GROUP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56780_a0_lrd_ifd_u32_0x1,
      .def = &bcm56780_a0_lrd_ifd_u32_0x1,
      .max = &bcm56780_a0_lrd_ifd_u32_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 1 LEARN */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 2 DT_EM_GRP_TEMPLATE_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 3 NUM_TRACKING_PARAMETERS */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x8,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 4 TRACKING_PARAMETERS_TYPE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x10,
      .depth = 8,
      .width = 5,
      .edata = BCMLTD_COMMON_FLOWTRACKER_TRACKING_PARAM_TYPE_T_DATA,
    },
    { /* 5 TRACKING_PARAMETERS_UDF_POLICY_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0x11cf,
      .depth = 8,
      .width = 13,
      .edata = NULL,
    },
    { /* 6 FLOW_LIMIT */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 7 AGING_INTERVAL_MS */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 8 NUM_EXPORT_TRIGGERS */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0x4,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 9 EXPORT_TRIGGERS */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x3,
      .depth = 4,
      .width = 2,
      .edata = BCMLTD_COMMON_FLOWTRACKER_EXPORT_TRIGGER_TYPE_T_DATA,
    },
    { /* 10 NUM_ACTIONS */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x8,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 11 ACTIONS_TYPE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xa,
      .depth = 11,
      .width = 4,
      .edata = BCMLTD_COMMON_FLOWTRACKER_ACTION_TYPE_T_DATA,
    },
    { /* 12 ACTIONS_EM_FT_OPAQUE_OBJ0_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 11,
      .width = 16,
      .edata = NULL,
    },
    { /* 13 ACTIONS_EM_FT_IOAM_GBP_ACTION_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xf,
      .depth = 11,
      .width = 4,
      .edata = NULL,
    },
    { /* 14 ACTIONS_EM_FT_COPY_TO_CPU_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 11,
      .width = 1,
      .edata = NULL,
    },
    { /* 15 ACTIONS_EM_FT_DROP_ACTION_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x1,
      .depth = 11,
      .width = 1,
      .edata = NULL,
    },
    { /* 16 ACTIONS_DESTINATION_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 11,
      .width = 16,
      .edata = NULL,
    },
    { /* 17 ACTIONS_DESTINATION_TYPE_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xf,
      .depth = 11,
      .width = 4,
      .edata = NULL,
    },
    { /* 18 ACTIONS_EM_FT_FLEX_STATE_ACTION_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xf,
      .depth = 11,
      .width = 4,
      .edata = NULL,
    },
    { /* 19 ACTIONS_FLEX_CTR_ACTION_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0x3f,
      .depth = 11,
      .width = 6,
      .edata = NULL,
    },
    { /* 20 ACTIONS_PKT_FLOW_ELIGIBILITY_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0x3f,
      .depth = 11,
      .width = 6,
      .edata = NULL,
    },
    { /* 21 ACTIONS_L2_IIF_SVP_MIRROR_INDEX_0_VAL */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0xf,
      .depth = 11,
      .width = 4,
      .edata = NULL,
    },
    { /* 22 MON_FLOWTRACKER_ELEPHANT_PROFILE_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xffffffff,
      .depth = 0,
      .width = 32,
      .edata = NULL,
    },
    { /* 23 OPERATIONAL_STATE */
      .flags = BCMLRD_FIELD_F_READ_ONLY | BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0x5,
      .depth = 0,
      .width = 3,
      .edata = BCMLTD_COMMON_MON_FLOWTRACKER_GROUP_STATE_T_DATA,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_lrd_mon_flowtracker_group_map_field_data = {
    .fields = 24,
    .field = bcm56780_a0_lrd_mon_flowtracker_group_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_lrd_mon_flowtracker_groupt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = TRUE,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_lrd_mon_flowtracker_groupt_attr_group = {
    .attributes = 1,
    .attr = bcm56780_a0_lrd_mon_flowtracker_groupt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_lrd_bcmimm_mon_flowtracker_group_entry[] = {
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
            /* handler: bcm56780_a0_lta_bcmimm_mon_flowtracker_group_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56780_A0_LTA_BCMIMM_MON_FLOWTRACKER_GROUP_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_entry_t
bcm56780_a0_lrd_bcmltx_mon_flowtracker_group_validate_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_VALUE_FIELD_VALIDATION,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            /* handler: bcm56780_a0_lta_bcmltx_mon_flowtracker_group_std_val_fv_handler */
            .handler_id = BCMLTD_VALIDATE_BCM56780_A0_LTA_BCMLTX_MON_FLOWTRACKER_GROUP_STD_VAL_FV_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56780_a0_lrd_mon_flowtracker_group_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_bcmimm_mon_flowtracker_group_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_VALIDATION,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_bcmltx_mon_flowtracker_group_validate_entry
    },
};
const bcmlrd_map_t bcm56780_a0_lrd_mon_flowtracker_group_map = {
    .src_id = MON_FLOWTRACKER_GROUPt,
    .field_data = &bcm56780_a0_lrd_mon_flowtracker_group_map_field_data,
    .groups = 2,
    .group  = bcm56780_a0_lrd_mon_flowtracker_group_map_group,
    .table_attr = &bcm56780_a0_lrd_mon_flowtracker_groupt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
