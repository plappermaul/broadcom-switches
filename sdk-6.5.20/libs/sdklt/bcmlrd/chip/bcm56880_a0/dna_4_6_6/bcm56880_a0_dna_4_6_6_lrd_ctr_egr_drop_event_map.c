/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/ctr/bcm56880_a0/bcm56880_a0_CTR_EGR_DROP_EVENT.map.ltl for
 *      bcm56880_a0 variant dna_4_6_6
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
#include <bcmlrd/chip/bcm56880_a0/dna_4_6_6/bcm56880_a0_dna_4_6_6_lrd_field_data.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/dna_4_6_6/bcm56880_a0_dna_4_6_6_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/dna_4_6_6/bcm56880_a0_dna_4_6_6_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56880_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "../bcm56880_a0_lrd_enumpool.h"
#include "bcm56880_a0_dna_4_6_6_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* CTR_EGR_DROP_EVENT field init */
static const bcmlrd_field_data_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_field_data_mmd[] = {
    { /* 0 CTR_EGR_DROP_EVENT_ID */
      .flags = BCMLTD_FIELD_F_KEY | BCMLTD_FIELD_F_ENUM,
      .min = &bcm56880_a0_dna_4_6_6_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_dna_4_6_6_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_dna_4_6_6_lrd_ifd_u32_0x13,
      .depth = 0,
      .width = 5,
      .edata = BCM56880_A0_DNA_4_6_6_LRD_EGR_DROP_EVENT_T_DATA,
    },
    { /* 1 DROP_CNT */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_6_6_lrd_ifd_u64_0x0,
      .def = &bcm56880_a0_dna_4_6_6_lrd_ifd_u64_0x0,
      .max = &bcm56880_a0_dna_4_6_6_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_field_data = {
    .fields = 2,
    .field = bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 20,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = TRUE,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 255,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_attr_group = {
    .attributes = 4,
    .attr = bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_attr_entry,
};


const bcmltd_field_desc_t
bcm56880_a0_dna_4_6_6_lrd_bcmltx_ctr_group_stats_src_field_desc_s1[1] = {
    {
        .field_id  = CTR_EGR_DROP_EVENTt_DROP_CNTf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 0,
        .reserved  = 0
    },
};

static const bcmltd_field_desc_t
bcm56880_a0_dna_4_6_6_lrd_ctr_group_stats_dst_field_desc_d1[4] = {
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 366,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 0,
        .sym      = "EPOST_TRACE_DROP_EVENT_EGR_HIGHEST_DROP_COUNTERm.COUNTf.ENTRY0",
        .reserved  = 0,
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 367,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 1,
        .sym      = "EPOST_TRACE_DROP_EVENT_EGR_HIGHEST_DROP_COUNTERm.COUNTf.ENTRY1",
        .reserved  = 0,
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 368,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 2,
        .sym      = "EPOST_TRACE_DROP_EVENT_EGR_HIGHEST_DROP_COUNTERm.COUNTf.ENTRY2",
        .reserved  = 0,
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 369,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 63,
        .entry_idx = 3,
        .sym      = "EPOST_TRACE_DROP_EVENT_EGR_HIGHEST_DROP_COUNTERm.COUNTf.ENTRY3",
        .reserved  = 0,
    },
};

const bcmlrd_field_xfrm_desc_t
bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s1_d1_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56880_A0_DNA_4_6_6_LTA_BCMLTX_CTR_GROUP_STATS_XFRM_HANDLER_FWD_S1_D1_ID,
    .src_fields = 1,
    .src = bcm56880_a0_dna_4_6_6_lrd_bcmltx_ctr_group_stats_src_field_desc_s1,
    .dst_fields = 4,
    .dst = bcm56880_a0_dna_4_6_6_lrd_ctr_group_stats_dst_field_desc_d1,
};

const bcmlrd_field_xfrm_desc_t
bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s1_d1_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56880_A0_DNA_4_6_6_LTA_BCMLTX_CTR_GROUP_STATS_XFRM_HANDLER_REV_S1_D1_ID,
    .src_fields = 4,
    .src = bcm56880_a0_dna_4_6_6_lrd_ctr_group_stats_dst_field_desc_d1,
    .dst_fields = 1,
    .dst = bcm56880_a0_dna_4_6_6_lrd_bcmltx_ctr_group_stats_src_field_desc_s1,
};


static const bcmlrd_map_entry_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_epost_trace_drop_event_egr_highest_drop_counter_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = CTR_EGR_DROP_EVENTt_CTR_EGR_DROP_EVENT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 1,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = CTR_EGR_DROP_EVENTt_CTR_EGR_DROP_EVENT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 2,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = CTR_EGR_DROP_EVENTt_CTR_EGR_DROP_EVENT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 3,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = CTR_EGR_DROP_EVENTt_CTR_EGR_DROP_EVENT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 1,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 1,
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 2,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 2,
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 3,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 3,
            }
        },
    },
    { /* 8 */
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
                /* handler: bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s1_d1 */
                .desc = &bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_fwd_s1_d1_desc,
            },
        },
    },
    { /* 9 */
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
                /* handler: bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s1_d1 */
                .desc = &bcm56880_a0_dna_4_6_6_lta_bcmltx_ctr_group_stats_xfrm_handler_rev_s1_d1_desc,
            },
        },
    },
};
static const bcmlrd_map_group_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EPOST_TRACE_DROP_EVENT_EGR_HIGHEST_DROP_COUNTERm,
        },
        .entries = 10,
        .entry = bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_epost_trace_drop_event_egr_highest_drop_counter_map_entry
    },
};
const bcmlrd_map_t bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map = {
    .src_id = CTR_EGR_DROP_EVENTt,
    .field_data = &bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_event_map_group,
    .table_attr = &bcm56880_a0_dna_4_6_6_lrd_ctr_egr_drop_eventt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};