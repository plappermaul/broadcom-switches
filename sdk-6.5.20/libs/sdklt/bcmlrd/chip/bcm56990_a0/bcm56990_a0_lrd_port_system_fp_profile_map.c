/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/port/bcm56990_a0/bcm56990_a0_PORT_SYSTEM_FP_PROFILE.map.ltl for
 *      bcm56990_a0
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
#include <bcmlrd/chip/bcm56990_a0/bcm56990_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56990_a0/bcm56990_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56990_a0/bcm56990_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56990_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56990_a0_lrd_enumpool.h"
/* PORT_SYSTEM_FP_PROFILE field init */
static const bcmlrd_field_data_t bcm56990_a0_lrd_port_system_fp_profile_map_field_data_mmd[] = {
    { /* 0 PIPE */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u8_0x0,
      .def = &bcm56990_a0_lrd_ifd_u8_0x0,
      .max = &bcm56990_a0_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 1 PORT_SYSTEM_PROFILE_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x43,
      .depth = 0,
      .width = 7,
      .edata = NULL,
    },
    { /* 2 FP_VLAN */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 3 FP_ING */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 4 USE_TABLE_FP_VLAN_PORT_GRP */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 5 FP_VLAN_PORT_GRP */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x1ff,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 6 FP_ING_GRP_SEL_CLASS_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56990_a0_lrd_port_system_fp_profile_map_field_data = {
    .fields = 7,
    .field = bcm56990_a0_lrd_port_system_fp_profile_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_a0_lrd_port_system_fp_profilet_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INSTANCE_MAX_INDEX,
        .value = 15,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INSTANCE_MIN_INDEX,
        .value = 0,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 67,
    },
    { /* 4 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56990_a0_lrd_port_system_fp_profilet_attr_group = {
    .attributes = 5,
    .attr = bcm56990_a0_lrd_port_system_fp_profilet_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_a0_lrd_port_system_fp_profilet_lport_tab_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INSTANCE,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 3,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_PIPEf,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 6,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_PORT_SYSTEM_PROFILE_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 6,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VFP_ENABLEf,
            .field_idx = 0,
            .minbit    = 152,
            .maxbit    = 152,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_FP_VLANf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = FILTER_ENABLEf,
            .field_idx = 0,
            .minbit    = 47,
            .maxbit    = 47,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_FP_INGf,
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
            .field_id  = VFP_PORT_GROUP_IDf,
            .field_idx = 0,
            .minbit    = 153,
            .maxbit    = 161,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_FP_VLAN_PORT_GRPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = USE_PORT_TABLE_GROUP_IDf,
            .field_idx = 0,
            .minbit    = 147,
            .maxbit    = 147,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_USE_TABLE_FP_VLAN_PORT_GRPf,
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
            .field_id  = IFP_KEY_SEL_CLASS_IDf,
            .field_idx = 0,
            .minbit    = 56,
            .maxbit    = 63,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_SYSTEM_FP_PROFILEt_FP_ING_GRP_SEL_CLASS_IDf,
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
/* Map conditions */
static const uint32_t
bcm56990_a0_lrd_port_system_fp_profile_map_cond_edit[] = {
    1, /* Number of groups. */
    0, /* Group ID. */
    1, /* Number of entries. */
    0, /* Entry ID. */
    0, /* Number of multi-entry transforms. */
};

static const bcmlrd_condition_t
bcm56990_a0_lrd_port_system_fp_profile_map_condition[] = {
    {
        .op    = BCMLRD_CONDITION_AND,
        .table = PORT_CONFIGt,
        .field = PORT_CONFIGt_PORT_SYSTEM_PROFILE_OPERMODE_PIPEUNIQUEf,
        .value = 1
    },
};

static const bcmlrd_cond_expr_t
bcm56990_a0_lrd_port_system_fp_profile_map_cond_expr[] = {
    {
        .num_cond = 1,
        .cond     = bcm56990_a0_lrd_port_system_fp_profile_map_condition + 0,
    },
};

static const bcmlrd_cond_field_t bcm56990_a0_lrd_port_system_fp_profile_map_cond_field[] = {
    {
        .field = PORT_SYSTEM_FP_PROFILEt_PIPEf,
        .expr  = bcm56990_a0_lrd_port_system_fp_profile_map_cond_expr + 0,
        .edit  = bcm56990_a0_lrd_port_system_fp_profile_map_cond_edit + 0,
    },
};
static const bcmlrd_cond_data_t
bcm56990_a0_lrd_port_system_fp_profile_map_cond = {
    .num_field_cond = 1,
    .field_cond = bcm56990_a0_lrd_port_system_fp_profile_map_cond_field
};
static const bcmlrd_map_group_t bcm56990_a0_lrd_port_system_fp_profile_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = LPORT_TABm,
        },
        .entries = 7,
        .entry = bcm56990_a0_lrd_port_system_fp_profilet_lport_tab_map_entry
    },
};
const bcmlrd_map_t bcm56990_a0_lrd_port_system_fp_profile_map = {
    .src_id = PORT_SYSTEM_FP_PROFILEt,
    .field_data = &bcm56990_a0_lrd_port_system_fp_profile_map_field_data,
    .groups = 1,
    .group  = bcm56990_a0_lrd_port_system_fp_profile_map_group,
    .table_attr = &bcm56990_a0_lrd_port_system_fp_profilet_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE,
    .cond = &bcm56990_a0_lrd_port_system_fp_profile_map_cond,
};
