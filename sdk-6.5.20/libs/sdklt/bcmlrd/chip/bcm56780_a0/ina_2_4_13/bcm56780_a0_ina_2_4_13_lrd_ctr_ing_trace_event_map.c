/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/ctr/bcm56780_a0/bcm56780_a0_CTR_ING_TRACE_EVENT.map.ltl for
 *      bcm56780_a0 variant ina_2_4_13
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
#include <bcmlrd/chip/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_lrd_field_data.h>
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "../bcm56780_a0_lrd_enumpool.h"
#include "bcm56780_a0_ina_2_4_13_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* CTR_ING_TRACE_EVENT field init */
static const bcmlrd_field_data_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_field_data_mmd[] = {
    { /* 0 CTR_ING_TRACE_EVENT_ID */
      .flags = BCMLTD_FIELD_F_KEY | BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_ina_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_ina_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_ina_2_4_13_lrd_ifd_u32_0x2f,
      .depth = 0,
      .width = 6,
      .edata = BCM56780_A0_INA_2_4_13_LRD_ING_TRACE_EVENT_T_DATA,
    },
    { /* 1 TRACE_CNT */
      .flags = 0,
      .min = &bcm56780_a0_ina_2_4_13_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_ina_2_4_13_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_ina_2_4_13_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_field_data = {
    .fields = 2,
    .field = bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = TRUE,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_attr_group = {
    .attributes = 1,
    .attr = bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_attr_entry,
};


const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_src_field_desc_s0[1] = {
    {
        .field_id  = CTR_ING_TRACE_EVENTt_CTR_ING_TRACE_EVENT_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 31,
        .entry_idx = 0,
        .reserved  = 0
    },
};

const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_ctr_ing_trace_event_dst_field_desc_d0[1] = {
    {
        .field_id  = BCMLRD_FIELD_TABLE_SEL,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const uint32_t
bcm56780_a0_ina_2_4_13_lrd_trace_event_idx_transform_tbl_t0[48] = {
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_0r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_1r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_2r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_3r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_4r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_5r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_6r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_7r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_8r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_9r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_10r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_11r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_12r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_13r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_14r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_15r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_16r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_17r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_18r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_19r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_20r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_21r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_22r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_23r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_24r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_25r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_26r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_27r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_28r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_29r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_30r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_31r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_32r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_33r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_34r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_35r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_36r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_37r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_38r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_39r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_40r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_41r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_42r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_43r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_44r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_45r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_46r,
    IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_47r,
};

const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_ctr_ing_trace_event_dst_field_desc_d1[1] = {
    {
        .field_id  = BCMLRD_FIELD_TABLE_SEL,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 1,
        .reserved  = 0,
    },
};

const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_bcmltx_ctr_group_stats_src_field_desc_s2[1] = {
    {
        .field_id  = CTR_ING_TRACE_EVENTt_TRACE_CNTf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 0,
        .reserved  = 0
    },
};

static const bcmltd_field_desc_t
bcm56780_a0_ina_2_4_13_lrd_ctr_group_stats_dst_field_desc_d2[2] = {
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 416,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 0,
        .sym      = "IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_0m.COUNTf.ENTRY0",
        .reserved  = 0,
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 417,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 1,
        .sym      = "IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_0m.COUNTf.ENTRY1",
        .reserved  = 0,
    },
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d0_t0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_INA_2_4_13_LTA_BCMLTX_TRACE_EVENT_IDX_XFRM_HANDLER_FWD_S0_D0_T0_ID,
    .src_fields = 1,
    .src = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_src_field_desc_s0,
    .dst_fields = 1,
    .dst = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_ctr_ing_trace_event_dst_field_desc_d0,
    .tables = 48,
    .tbl = bcm56780_a0_ina_2_4_13_lrd_trace_event_idx_transform_tbl_t0,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_rev_s0_d0_t0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_INA_2_4_13_LTA_BCMLTX_TRACE_EVENT_IDX_XFRM_HANDLER_REV_S0_D0_T0_ID,
    .src_fields = 1,
    .src = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_ctr_ing_trace_event_dst_field_desc_d0,
    .dst_fields = 1,
    .dst = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_src_field_desc_s0,
    .tables = 48,
    .tbl = bcm56780_a0_ina_2_4_13_lrd_trace_event_idx_transform_tbl_t0,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d1_t0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_INA_2_4_13_LTA_BCMLTX_TRACE_EVENT_IDX_XFRM_HANDLER_FWD_S0_D1_T0_ID,
    .src_fields = 1,
    .src = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_src_field_desc_s0,
    .dst_fields = 1,
    .dst = bcm56780_a0_ina_2_4_13_lrd_bcmltx_trace_event_idx_ctr_ing_trace_event_dst_field_desc_d1,
    .tables = 48,
    .tbl = bcm56780_a0_ina_2_4_13_lrd_trace_event_idx_transform_tbl_t0,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s2_d2_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_INA_2_4_13_LTA_BCMLTX_CTR_GROUP_STATS_XFRM_HANDLER_FWD_S2_D2_ID,
    .src_fields = 1,
    .src = bcm56780_a0_ina_2_4_13_lrd_bcmltx_ctr_group_stats_src_field_desc_s2,
    .dst_fields = 2,
    .dst = bcm56780_a0_ina_2_4_13_lrd_ctr_group_stats_dst_field_desc_d2,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s2_d2_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_INA_2_4_13_LTA_BCMLTX_CTR_GROUP_STATS_XFRM_HANDLER_REV_S2_D2_ID,
    .src_fields = 2,
    .src = bcm56780_a0_ina_2_4_13_lrd_ctr_group_stats_dst_field_desc_d2,
    .dst_fields = 1,
    .dst = bcm56780_a0_ina_2_4_13_lrd_bcmltx_ctr_group_stats_src_field_desc_s2,
};


static const bcmlrd_map_entry_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_ipost_trace_drop_event_trace_event_counter_0_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 1,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 1,
            .entry_idx = 1,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 1,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_FWD_KEY_FIELD_XFRM_HANDLER,
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
                /* handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d0_t0 */
                .desc = &bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d0_t0_desc,
            },
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_REV_KEY_FIELD_XFRM_HANDLER,
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
                /* handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_rev_s0_d0_t0 */
                .desc = &bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_rev_s0_d0_t0_desc,
            },
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_FWD_KEY_FIELD_XFRM_HANDLER,
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
                /* handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d1_t0 */
                .desc = &bcm56780_a0_ina_2_4_13_lta_bcmltx_trace_event_idx_xfrm_handler_fwd_s0_d1_t0_desc,
            },
        },
    },
    { /* 5 */
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
                /* handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s2_d2 */
                .desc = &bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s2_d2_desc,
            },
        },
    },
    { /* 6 */
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
                /* handler: bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s2_d2 */
                .desc = &bcm56780_a0_ina_2_4_13_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s2_d2_desc,
            },
        },
    },
};
static const bcmlrd_map_group_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IPOST_TRACE_DROP_EVENT_TRACE_EVENT_COUNTER_0r,
        },
        .entries = 7,
        .entry = bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_ipost_trace_drop_event_trace_event_counter_0_map_entry
    },
};
const bcmlrd_map_t bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map = {
    .src_id = CTR_ING_TRACE_EVENTt,
    .field_data = &bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_field_data,
    .groups = 1,
    .group  = bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_event_map_group,
    .table_attr = &bcm56780_a0_ina_2_4_13_lrd_ctr_ing_trace_eventt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
