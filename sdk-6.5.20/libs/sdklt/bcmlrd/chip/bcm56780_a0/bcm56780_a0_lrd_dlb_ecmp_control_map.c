/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/dlb/bcm56780_a0/bcm56780_a0_DLB_ECMP_CONTROL.map.ltl for
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
/* DLB_ECMP_CONTROL field init */
static const bcmlrd_field_data_t bcm56780_a0_lrd_dlb_ecmp_control_map_field_data_mmd[] = {
    { /* 0 BASE_ECMP_ID */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 1 SEED */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffff,
      .depth = 0,
      .width = 24,
      .edata = NULL,
    },
    { /* 2 MONITOR_SEED */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x1,
      .def = &bcm56780_a0_lrd_ifd_u64_0x1,
      .max = &bcm56780_a0_lrd_ifd_u64_0x1ffffffe,
      .depth = 0,
      .width = 29,
      .edata = NULL,
    },
    { /* 3 FP_ING_SEED */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x1,
      .def = &bcm56780_a0_lrd_ifd_u64_0x1,
      .max = &bcm56780_a0_lrd_ifd_u64_0x1ffffffe,
      .depth = 0,
      .width = 29,
      .edata = NULL,
    },
    { /* 4 MIRROR */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 5 MIRROR_INSTANCE_ID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_lrd_ifd_u8_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_lrd_dlb_ecmp_control_map_field_data = {
    .fields = 6,
    .field = bcm56780_a0_lrd_dlb_ecmp_control_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_lrd_dlb_ecmp_controlt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_lrd_dlb_ecmp_controlt_attr_group = {
    .attributes = 1,
    .attr = bcm56780_a0_lrd_dlb_ecmp_controlt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_dlb_id_offset_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DLB_ID_OFFSETf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 11,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_ECMP_CONTROLt_BASE_ECMP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_monitor_control_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = SEEDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 28,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_ECMP_CONTROLt_MONITOR_SEEDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 28,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_monitor_ifp_control_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = SEEDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 28,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_ECMP_CONTROLt_FP_ING_SEEDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 28,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_random_selection_control_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = SEEDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 23,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_ECMP_CONTROLt_SEEDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 23,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_group_t bcm56780_a0_lrd_dlb_ecmp_control_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = DLB_ECMP_DLB_ID_OFFSETr,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_dlb_id_offset_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = DLB_ECMP_MONITOR_CONTROLr,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_monitor_control_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = DLB_ECMP_MONITOR_IFP_CONTROLr,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_monitor_ifp_control_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = DLB_ECMP_RANDOM_SELECTION_CONTROLr,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_dlb_ecmp_controlt_dlb_ecmp_random_selection_control_map_entry
    },
};
const bcmlrd_map_t bcm56780_a0_lrd_dlb_ecmp_control_map = {
    .src_id = DLB_ECMP_CONTROLt,
    .field_data = &bcm56780_a0_lrd_dlb_ecmp_control_map_field_data,
    .groups = 4,
    .group  = bcm56780_a0_lrd_dlb_ecmp_control_map_group,
    .table_attr = &bcm56780_a0_lrd_dlb_ecmp_controlt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
