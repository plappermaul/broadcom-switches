/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/npl/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_VLAN_XLATE_3_STRENGTH_PROFILE.map.ltl for
 *      bcm56880_a0 variant dna_4_7_11
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
#include <bcmlrd/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_lrd_field_data.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56880_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "../bcm56880_a0_lrd_enumpool.h"
#include "bcm56880_a0_dna_4_7_11_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* VLAN_XLATE_3_STRENGTH_PROFILE field init */
static const bcmlrd_field_data_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_field_data_mmd[] = {
    { /* 0 VLAN_XLATE_3_STRENGTH_PROFILE_INDEX */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 1 SVP_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 2 VFI_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 3 L2_IIF_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 4 L3_IIF_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 5 OPAQUE_OBJ0_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 6 OPAQUE_CTRL_ID_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 7 OPAQUE_CTRL_ID_1_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
    { /* 8 VLAN_TAG_PRESERVE_CTRL_STRENGTH */
      .flags = 0,
      .min = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .def = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x0,
      .max = &bcm56880_a0_dna_4_7_11_lrd_ifd_u8_0x7,
      .depth = 0,
      .width = 3,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_field_data = {
    .fields = 9,
    .field = bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profilet_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 16,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_RESOURCE_INFO,
        .value = BCM56880_A0_DNA_4_7_11_VLAN_XLATE_3_STRENGTH_PROFILE_RESOURCE_INFOt,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profilet_attr_group = {
    .attributes = 3,
    .attr = bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profilet_attr_entry,
};

static const bcmlrd_map_entry_t bcm56880_a0_dna_4_7_11_lrd_bcmimm_vlan_xlate_3_strength_profile_entry[] = {
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
            /* handler: bcm56880_a0_dna_4_7_11_lta_bcmimm_vlan_xlate_3_strength_profile_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56880_A0_DNA_4_7_11_LTA_BCMIMM_VLAN_XLATE_3_STRENGTH_PROFILE_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56880_a0_dna_4_7_11_lrd_bcmimm_vlan_xlate_3_strength_profile_entry
    },
};
const bcmlrd_map_t bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map = {
    .src_id = BCM56880_A0_DNA_4_7_11_VLAN_XLATE_3_STRENGTH_PROFILEt,
    .field_data = &bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profile_map_group,
    .table_attr = &bcm56880_a0_dna_4_7_11_lrd_vlan_xlate_3_strength_profilet_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
