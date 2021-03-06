/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/tnl/bcm56990_b0/bcm56990_b0_TNL_MPLS_ENCAP.map.ltl for
 *      bcm56990_b0
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
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56990_b0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56990_b0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* TNL_MPLS_ENCAP field init */
static const bcmlrd_field_data_t bcm56990_b0_lrd_tnl_mpls_encap_map_field_data_mmd[] = {
    { /* 0 TNL_MPLS_ENCAP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_b0_lrd_ifd_u16_0x1,
      .def = &bcm56990_b0_lrd_ifd_u16_0x1,
      .max = &bcm56990_b0_lrd_ifd_u16_0x7ff8,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 1 ENCAP_INDEX */
      .flags = BCMLRD_FIELD_F_READ_ONLY,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 2 NUM_LABELS */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x8,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 3 MAX_LABELS */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x1,
      .def = &bcm56990_b0_lrd_ifd_u8_0x1,
      .max = &bcm56990_b0_lrd_ifd_u8_0x8,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 4 LABEL */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u32_0x0,
      .def = &bcm56990_b0_lrd_ifd_u32_0x0,
      .max = &bcm56990_b0_lrd_ifd_u32_0xfffff,
      .depth = 8,
      .width = 20,
      .edata = NULL,
    },
    { /* 5 LABEL_TTL */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0xff,
      .depth = 8,
      .width = 8,
      .edata = NULL,
    },
    { /* 6 MPLS_TNL_EXP_MODE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56990_b0_lrd_ifd_u32_0x0,
      .def = &bcm56990_b0_lrd_ifd_u32_0x0,
      .max = &bcm56990_b0_lrd_ifd_u32_0x2,
      .depth = 8,
      .width = 2,
      .edata = BCM56990_B0_LRD_MPLS_EXP_MODE_T_DATA,
    },
    { /* 7 PHB_EGR_MPLS_ID */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0xf,
      .depth = 8,
      .width = 4,
      .edata = NULL,
    },
    { /* 8 LABEL_EXP */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x7,
      .depth = 8,
      .width = 3,
      .edata = NULL,
    },
    { /* 9 PRI */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x7,
      .depth = 8,
      .width = 3,
      .edata = NULL,
    },
    { /* 10 CFI */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 8,
      .width = 1,
      .edata = NULL,
    },
    { /* 11 ECN_CNG_TO_MPLS_EXP_ID */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x3,
      .depth = 8,
      .width = 2,
      .edata = NULL,
    },
    { /* 12 ECN_CNG_TO_MPLS_EXP_PRIORITY */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x1,
      .depth = 8,
      .width = 1,
      .edata = NULL,
    },
    { /* 13 ECN_IP_TO_MPLS_EXP_ID */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x3,
      .depth = 8,
      .width = 2,
      .edata = NULL,
    },
    { /* 14 ECN_IP_TO_MPLS_EXP_PRIORITY */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u8_0x0,
      .def = &bcm56990_b0_lrd_ifd_u8_0x0,
      .max = &bcm56990_b0_lrd_ifd_u8_0x1,
      .depth = 8,
      .width = 1,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56990_b0_lrd_tnl_mpls_encap_map_field_data = {
    .fields = 15,
    .field = bcm56990_b0_lrd_tnl_mpls_encap_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_b0_lrd_tnl_mpls_encapt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 32760,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_HARDWARE,
        .value = EGR_IP_TUNNEL_MPLSm,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_SHARED_INDEX,
        .value = TNL_IPV4_ENCAPt,
    },
    { /* 4 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 32760,
    },
    { /* 5 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 1,
    },
};

static const bcmlrd_map_attr_t bcm56990_b0_lrd_tnl_mpls_encapt_attr_group = {
    .attributes = 6,
    .attr = bcm56990_b0_lrd_tnl_mpls_encapt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_b0_lrd_bcmimm_tnl_mpls_encap_entry[] = {
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
            /* handler: bcm56990_b0_lta_bcmimm_tnl_mpls_encap_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56990_B0_LTA_BCMIMM_TNL_MPLS_ENCAP_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_entry_t
bcm56990_b0_lrd_bcmltx_tnl_mpls_encap_validate_entry[] = {
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
            /* handler: bcm56990_b0_lta_bcmltx_tnl_mpls_encap_std_val_fv_handler */
            .handler_id = BCMLTD_VALIDATE_BCM56990_B0_LTA_BCMLTX_TNL_MPLS_ENCAP_STD_VAL_FV_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56990_b0_lrd_tnl_mpls_encap_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56990_b0_lrd_bcmimm_tnl_mpls_encap_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_VALIDATION,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56990_b0_lrd_bcmltx_tnl_mpls_encap_validate_entry
    },
};
const bcmlrd_map_t bcm56990_b0_lrd_tnl_mpls_encap_map = {
    .src_id = TNL_MPLS_ENCAPt,
    .field_data = &bcm56990_b0_lrd_tnl_mpls_encap_map_field_data,
    .groups = 2,
    .group  = bcm56990_b0_lrd_tnl_mpls_encap_map_group,
    .table_attr = &bcm56990_b0_lrd_tnl_mpls_encapt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
