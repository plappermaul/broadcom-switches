/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/dlb/bcm56880_a0/bcm56880_a0_DLB_CONTROL.map.ltl for
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
/* DLB_CONTROL field init */
static const bcmlrd_field_data_t bcm56880_a0_lrd_dlb_control_map_field_data_mmd[] = {
    { /* 0 SAMPLING_PERIOD */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 1 PORT_LOAD_WEIGHT */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 2 PORT_QUEUE_SIZE_WEIGHT */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 3 TM_QUEUE_SIZE_WEIGHT */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 4 CAP_PORT_LOAD */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 5 CAP_PORT_QUEUE_SIZE */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 6 CAP_TM_QUEUE_SIZE */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 7 ETHERTYPE_ELIGIBILITY */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 8 LB_HASH_USE_FLOW_DLB_ECMP */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_lrd_dlb_control_map_field_data = {
    .fields = 9,
    .field = bcm56880_a0_lrd_dlb_control_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_lrd_dlb_controlt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_lrd_dlb_controlt_attr_group = {
    .attributes = 1,
    .attr = bcm56880_a0_lrd_dlb_controlt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56880_a0_lrd_dlb_controlt_dlb_ecmp_quality_measure_control_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = PORT_LOADING_WEIGHTf,
            .field_idx = 0,
            .minbit    = 8,
            .maxbit    = 11,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_PORT_LOAD_WEIGHTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = TOTAL_QSIZE_WEIGHTf,
            .field_idx = 0,
            .minbit    = 12,
            .maxbit    = 15,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_PORT_QUEUE_SIZE_WEIGHTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = ITM_PORT_QSIZE_WEIGHTf,
            .field_idx = 0,
            .minbit    = 16,
            .maxbit    = 19,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_TM_QUEUE_SIZE_WEIGHTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = CAP_PORT_LOADING_AVERAGEf,
            .field_idx = 0,
            .minbit    = 20,
            .maxbit    = 20,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_CAP_PORT_LOADf,
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
            .field_id  = CAP_TOTAL_PORT_QSIZE_AVERAGEf,
            .field_idx = 0,
            .minbit    = 21,
            .maxbit    = 21,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_CAP_PORT_QUEUE_SIZEf,
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
            .field_id  = CAP_ITM_PORT_QSIZE_AVERAGEf,
            .field_idx = 0,
            .minbit    = 22,
            .maxbit    = 22,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_CAP_TM_QUEUE_SIZEf,
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
            .field_id  = SAMPLING_PERIODf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = DLB_CONTROLt_SAMPLING_PERIODf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_group_t bcm56880_a0_lrd_dlb_control_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = DLB_ECMP_QUALITY_MEASURE_CONTROLr,
        },
        .entries = 7,
        .entry = bcm56880_a0_lrd_dlb_controlt_dlb_ecmp_quality_measure_control_map_entry
    },
};
const bcmlrd_map_t bcm56880_a0_lrd_dlb_control_map = {
    .src_id = DLB_CONTROLt,
    .field_data = &bcm56880_a0_lrd_dlb_control_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_lrd_dlb_control_map_group,
    .table_attr = &bcm56880_a0_lrd_dlb_controlt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};