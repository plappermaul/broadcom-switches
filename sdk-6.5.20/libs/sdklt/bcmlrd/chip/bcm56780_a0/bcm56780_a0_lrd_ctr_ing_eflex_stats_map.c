/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/ctr/bcm56780_a0/bcm56780_a0_CTR_ING_EFLEX_STATS.map.ltl for
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
/* CTR_ING_EFLEX_STATS field init */
static const bcmlrd_field_data_t bcm56780_a0_lrd_ctr_ing_eflex_stats_map_field_data_mmd[] = {
    { /* 0 CTR_ING_EFLEX_ACTION_PROFILE_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56780_a0_lrd_ifd_u16_0x1,
      .def = &bcm56780_a0_lrd_ifd_u16_0x1,
      .max = &bcm56780_a0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 1 CTR_EFLEX_INDEX */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56780_a0_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_lrd_ifd_u32_0xeffff,
      .depth = 0,
      .width = 20,
      .edata = NULL,
    },
    { /* 2 CTR_A_UPPER */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 3 CTR_A_LOWER */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
    { /* 4 CTR_B */
      .flags = 0,
      .min = &bcm56780_a0_lrd_ifd_u64_0x0,
      .def = &bcm56780_a0_lrd_ifd_u64_0x0,
      .max = &bcm56780_a0_lrd_ifd_u64_0xffffffffffffffff,
      .depth = 0,
      .width = 64,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_lrd_ctr_ing_eflex_stats_map_field_data = {
    .fields = 5,
    .field = bcm56780_a0_lrd_ctr_ing_eflex_stats_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_lrd_ctr_ing_eflex_statst_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 983040,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = true,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_lrd_ctr_ing_eflex_statst_attr_group = {
    .attributes = 2,
    .attr = bcm56780_a0_lrd_ctr_ing_eflex_statst_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_lrd_bcmcth_ctr_ing_eflex_stats_entry[] = {
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
            /* handler: bcm56780_a0_lta_bcmcth_ctr_ing_eflex_stats_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56780_A0_LTA_BCMCTH_CTR_ING_EFLEX_STATS_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56780_a0_lrd_ctr_ing_eflex_stats_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_lrd_bcmcth_ctr_ing_eflex_stats_entry
    },
};
const bcmlrd_map_t bcm56780_a0_lrd_ctr_ing_eflex_stats_map = {
    .src_id = CTR_ING_EFLEX_STATSt,
    .field_data = &bcm56780_a0_lrd_ctr_ing_eflex_stats_map_field_data,
    .groups = 1,
    .group  = bcm56780_a0_lrd_ctr_ing_eflex_stats_map_group,
    .table_attr = &bcm56780_a0_lrd_ctr_ing_eflex_statst_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE
};
