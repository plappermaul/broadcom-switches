/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/flex_digest/bcm56880_a0/bcm56880_a0_FLEX_DIGEST_LKUP_MASK_PROFILE.map.ltl for
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
/* FLEX_DIGEST_LKUP_MASK_PROFILE field init */
static const bcmlrd_field_data_t bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_field_data_mmd[] = {
    { /* 0 FLEX_DIGEST_LKUP_MASK_PROFILE_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0x1f,
      .depth = 0,
      .width = 5,
      .edata = NULL,
    },
    { /* 1 MASK */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_field_data = {
    .fields = 2,
    .field = bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 31,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_attr_group = {
    .attributes = 3,
    .attr = bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_attr_entry,
};

static const bcmlrd_map_entry_t bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_flex_digest_lkup_mask_profile_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 4,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = FLEX_DIGEST_LKUP_MASK_PROFILEt_FLEX_DIGEST_LKUP_MASK_PROFILE_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 4,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VALUEf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 15,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = FLEX_DIGEST_LKUP_MASK_PROFILEt_MASKf,
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
static const bcmlrd_map_group_t bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = FLEX_DIGEST_LKUP_MASK_PROFILEm,
        },
        .entries = 2,
        .entry = bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_flex_digest_lkup_mask_profile_map_entry
    },
};
const bcmlrd_map_t bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map = {
    .src_id = FLEX_DIGEST_LKUP_MASK_PROFILEt,
    .field_data = &bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_lrd_flex_digest_lkup_mask_profile_map_group,
    .table_attr = &bcm56880_a0_lrd_flex_digest_lkup_mask_profilet_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
