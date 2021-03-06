/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/npl/bcm56780_a0/ina_2_4_13/bcm56780_a0_ina_2_4_13_R_IFA_EDITOR_PARAMS.map.ltl for
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
/* R_IFA_EDITOR_PARAMS field init */
static const bcmlrd_field_data_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_field_data_mmd[] = {
    { /* 0 LOOPBACK_CTRL */
      .flags = 0,
      .min = &bcm56780_a0_ina_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_ina_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_ina_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 1 SYS_SOURCE_IFA_PROBE_COPY */
      .flags = 0,
      .min = &bcm56780_a0_ina_2_4_13_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_ina_2_4_13_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_ina_2_4_13_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_field_data = {
    .fields = 2,
    .field = bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_attr_group = {
    .attributes = 1,
    .attr = bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_flex_editor_small_flex_hdr_0_user_field_0_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DATAf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 3,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = BCM56780_A0_INA_2_4_13_R_IFA_EDITOR_PARAMSt_LOOPBACK_CTRLf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_flex_editor_small_flex_hdr_0_user_field_2_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DATAf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 15,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = BCM56780_A0_INA_2_4_13_R_IFA_EDITOR_PARAMSt_SYS_SOURCE_IFA_PROBE_COPYf,
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
static const bcmlrd_map_group_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = FLEX_EDITOR_SMALL_FLEX_HDR_0_USER_FIELD_0r,
        },
        .entries = 1,
        .entry = bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_flex_editor_small_flex_hdr_0_user_field_0_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = FLEX_EDITOR_SMALL_FLEX_HDR_0_USER_FIELD_2r,
        },
        .entries = 1,
        .entry = bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_flex_editor_small_flex_hdr_0_user_field_2_map_entry
    },
};
const bcmlrd_map_t bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map = {
    .src_id = BCM56780_A0_INA_2_4_13_R_IFA_EDITOR_PARAMSt,
    .field_data = &bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_field_data,
    .groups = 2,
    .group  = bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_params_map_group,
    .table_attr = &bcm56780_a0_ina_2_4_13_lrd_r_ifa_editor_paramst_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
