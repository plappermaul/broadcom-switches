/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/port/bcm56990_b0/bcm56990_b0_PORT_FP.map.ltl for
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
/* PORT_FP field init */
static const bcmlrd_field_data_t bcm56990_b0_lrd_port_fp_map_field_data_mmd[] = {
    { /* 0 PORT_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0x10f,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 1 FP_VLAN */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 2 FP_ING */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 3 FP_EGR */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 4 USE_TABLE_FP_VLAN_PORT_GRP */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 5 FP_VLAN_PORT_GRP */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0x1ff,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 6 FP_ING_GRP_SEL_CLASS_ID */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 7 FP_EGR_PORT_GRP */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_u16_0x0,
      .def = &bcm56990_b0_lrd_ifd_u16_0x0,
      .max = &bcm56990_b0_lrd_ifd_u16_0x1ff,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 8 FP_EGR_DROP */
      .flags = 0,
      .min = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_b0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_b0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 9 FP_EGR_CLASS_ID_SELECT */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56990_b0_lrd_ifd_u32_0x0,
      .def = &bcm56990_b0_lrd_ifd_u32_0x0,
      .max = &bcm56990_b0_lrd_ifd_u32_0xf,
      .depth = 0,
      .width = 4,
      .edata = BCM56990_B0_LRD_FP_CLASS_ID_SEL_T_DATA,
    },
};
const bcmlrd_map_field_data_t bcm56990_b0_lrd_port_fp_map_field_data = {
    .fields = 10,
    .field = bcm56990_b0_lrd_port_fp_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_b0_lrd_port_fpt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 271,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56990_b0_lrd_port_fpt_attr_group = {
    .attributes = 3,
    .attr = bcm56990_b0_lrd_port_fpt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_b0_lrd_port_fpt_egr_port_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 8,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_PORT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = EFP_FILTER_ENABLEf,
            .field_idx = 0,
            .minbit    = 18,
            .maxbit    = 18,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_EGRf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = EGR_PORT_GROUP_IDf,
            .field_idx = 0,
            .minbit    = 19,
            .maxbit    = 27,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_EGR_PORT_GRPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DEFAULT_DROPf,
            .field_idx = 0,
            .minbit    = 11,
            .maxbit    = 11,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_EGR_DROPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56990_b0_lrd_port_fpt_ifp_i2e_classid_select_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 8,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_PORT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = PORT_SELf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 3,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_EGR_CLASS_ID_SELECTf,
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
static const bcmlrd_map_entry_t bcm56990_b0_lrd_port_fpt_port_tab_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 8,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_PORT_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VFP_ENABLEf,
            .field_idx = 0,
            .minbit    = 153,
            .maxbit    = 153,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_VLANf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = FILTER_ENABLEf,
            .field_idx = 0,
            .minbit    = 45,
            .maxbit    = 45,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_INGf,
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
            .field_id  = VFP_PORT_GROUP_IDf,
            .field_idx = 0,
            .minbit    = 154,
            .maxbit    = 162,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_VLAN_PORT_GRPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 8,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = USE_PORT_TABLE_GROUP_IDf,
            .field_idx = 0,
            .minbit    = 148,
            .maxbit    = 148,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_USE_TABLE_FP_VLAN_PORT_GRPf,
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
            .field_id  = IFP_KEY_SEL_CLASS_IDf,
            .field_idx = 0,
            .minbit    = 54,
            .maxbit    = 61,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = PORT_FPt_FP_ING_GRP_SEL_CLASS_IDf,
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
static const bcmlrd_map_group_t bcm56990_b0_lrd_port_fp_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_PORTm,
        },
        .entries = 4,
        .entry = bcm56990_b0_lrd_port_fpt_egr_port_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IFP_I2E_CLASSID_SELECTm,
        },
        .entries = 2,
        .entry = bcm56990_b0_lrd_port_fpt_ifp_i2e_classid_select_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = PORT_TABm,
        },
        .entries = 6,
        .entry = bcm56990_b0_lrd_port_fpt_port_tab_map_entry
    },
};
const bcmlrd_map_t bcm56990_b0_lrd_port_fp_map = {
    .src_id = PORT_FPt,
    .field_data = &bcm56990_b0_lrd_port_fp_map_field_data,
    .groups = 3,
    .group  = bcm56990_b0_lrd_port_fp_map_group,
    .table_attr = &bcm56990_b0_lrd_port_fpt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
