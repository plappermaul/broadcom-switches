/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/l3/bcm56990_a0/bcm56990_a0_L3_UC_NHOP.map.ltl for
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
/* L3_UC_NHOP field init */
static const bcmlrd_field_data_t bcm56990_a0_lrd_l3_uc_nhop_map_field_data_mmd[] = {
    { /* 0 NHOP_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u16_0x1,
      .def = &bcm56990_a0_lrd_ifd_u16_0x1,
      .max = &bcm56990_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 1 MODID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u8_0x0,
      .def = &bcm56990_a0_lrd_ifd_u8_0x0,
      .max = &bcm56990_a0_lrd_ifd_u8_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 2 MODPORT */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x10f,
      .depth = 0,
      .width = 9,
      .edata = NULL,
    },
    { /* 3 IS_TRUNK */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 4 TRUNK_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 5 VLAN_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 6 DROP */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 7 COPY_TO_CPU */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 8 L3_EIF_VALID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 9 L3_EIF_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u32_0x0,
      .def = &bcm56990_a0_lrd_ifd_u32_0x0,
      .max = &bcm56990_a0_lrd_ifd_u32_0x1fff,
      .depth = 0,
      .width = 13,
      .edata = NULL,
    },
    { /* 10 DST_MAC */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u64_0x0,
      .def = &bcm56990_a0_lrd_ifd_u64_0x0,
      .max = &bcm56990_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 11 KEEP_DST_MAC */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 12 KEEP_SRC_MAC */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 13 KEEP_TTL */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 14 KEEP_VLAN_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 15 CTR_EGR_FLEX_POOL_NUMBER */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u32_0x0,
      .def = &bcm56990_a0_lrd_ifd_u32_0x0,
      .max = &bcm56990_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 16 CTR_EGR_FLEX_BASE_INDEX */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u32_0x0,
      .def = &bcm56990_a0_lrd_ifd_u32_0x0,
      .max = &bcm56990_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 17 CTR_EGR_FLEX_OFFSET_MODE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u32_0x0,
      .def = &bcm56990_a0_lrd_ifd_u32_0x0,
      .max = &bcm56990_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 18 CLASS_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x7f,
      .depth = 0,
      .width = 7,
      .edata = NULL,
    },
    { /* 19 CTR_EGR_EFLEX_OBJECT */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 20 L3_UC_NHOP_CTR_EGR_EFLEX_ACTION_ID */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u8_0x0,
      .def = &bcm56990_a0_lrd_ifd_u8_0x0,
      .max = &bcm56990_a0_lrd_ifd_u8_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 21 DO_NOT_CUT_THROUGH */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 22 DVP */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 23 ING_UNDERLAY_NHOP_VALID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 24 ING_UNDERLAY_NHOP_ID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 25 EGR_UNDERLAY_NHOP_VALID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 26 EGR_UNDERLAY_NHOP_ID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56990_a0_lrd_l3_uc_nhop_map_field_data = {
    .fields = 27,
    .field = bcm56990_a0_lrd_l3_uc_nhop_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_a0_lrd_l3_uc_nhopt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_SHARED_INDEX,
        .value = L3_UC_NHOPt,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 32767,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 1,
    },
};

static const bcmlrd_map_attr_t bcm56990_a0_lrd_l3_uc_nhopt_attr_group = {
    .attributes = 4,
    .attr = bcm56990_a0_lrd_l3_uc_nhopt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_a0_lrd_l3_uc_nhopt_egr_l3_next_hop_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = ENTRY_TYPEf,
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
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L3v_MAC_ADDRESSf,
            .field_idx = 0,
            .minbit    = 37,
            .maxbit    = 84,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_DST_MACf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 47,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L3v_L3_UC_DA_DISABLEf,
            .field_idx = 0,
            .minbit    = 31,
            .maxbit    = 31,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_KEEP_DST_MACf,
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
            .field_id  = L3v_L3_UC_TTL_DISABLEf,
            .field_idx = 0,
            .minbit    = 33,
            .maxbit    = 33,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_KEEP_TTLf,
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
            .field_id  = L3v_L3_UC_VLAN_DISABLEf,
            .field_idx = 0,
            .minbit    = 34,
            .maxbit    = 34,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_KEEP_VLAN_IDf,
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
            .field_id  = L3v_L3_UC_SA_DISABLEf,
            .field_idx = 0,
            .minbit    = 32,
            .maxbit    = 32,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_KEEP_SRC_MACf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L3v_CLASS_IDf,
            .field_idx = 0,
            .minbit    = 8,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_CLASS_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 6,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L3v_FLEX_CTR_OBJECTf,
            .field_idx = 0,
            .minbit    = 16,
            .maxbit    = 30,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_CTR_EGR_EFLEX_OBJECTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 9 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L3v_FLEX_CTR_ACTION_SELf,
            .field_idx = 0,
            .minbit    = 15,
            .maxbit    = 15,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_L3_UC_NHOP_CTR_EGR_EFLEX_ACTION_IDf,
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
static const bcmlrd_map_entry_t bcm56990_a0_lrd_l3_uc_nhopt_egr_l3_next_hop_2_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = INTF_NUMf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 12,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_L3_EIF_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 12,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
};
static const bcmlrd_map_entry_t bcm56990_a0_lrd_l3_uc_nhopt_ing_l3_next_hop_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 14,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_NHOP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = ENTRY_TYPEf,
            .field_idx = 0,
            .minbit    = 25,
            .maxbit    = 26,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VLAN_IDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 11,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_VLAN_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = PORT_NUMf,
            .field_idx = 0,
            .minbit    = 14,
            .maxbit    = 22,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_MODPORTf,
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
            .field_id  = TGIDf,
            .field_idx = 0,
            .minbit    = 14,
            .maxbit    = 21,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_TRUNK_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 7,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = Tf,
            .field_idx = 0,
            .minbit    = 27,
            .maxbit    = 27,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_IS_TRUNKf,
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
            .field_id  = COPY_TO_CPUf,
            .field_idx = 0,
            .minbit    = 23,
            .maxbit    = 23,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_COPY_TO_CPUf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DROPf,
            .field_idx = 0,
            .minbit    = 24,
            .maxbit    = 24,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L3_UC_NHOPt_DROPf,
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
const bcmlrd_field_selector_t bcm56990_a0_lrd_l3_uc_nhop_map_select[] = {
    { /* Node:0, Type:ROOT, IS_TRUNK */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_ROOT,
        .field_id = L3_UC_NHOPt_IS_TRUNKf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = BCMLRD_INVALID_SELECTOR_INDEX,
        .group = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_id = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_value = BCMLRD_INVALID_SELECTOR_INDEX
    },
    { /* Node:1, Type:FIELD, MODPORT */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L3_UC_NHOPt_MODPORTf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L3_UC_NHOPt_IS_TRUNKf,
        .selector_value = FALSE
    },
    { /* Node:2, Type:FIELD, TRUNK_ID */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_FIELD,
        .field_id = L3_UC_NHOPt_TRUNK_IDf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L3_UC_NHOPt_IS_TRUNKf,
        .selector_value = TRUE
    },
};

const bcmlrd_field_selector_data_t bcm56990_a0_lrd_l3_uc_nhop_map_select_data = {
    .num_field_selector = 3,
    .field_selector = bcm56990_a0_lrd_l3_uc_nhop_map_select,
};

static const bcmlrd_map_group_t bcm56990_a0_lrd_l3_uc_nhop_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_L3_NEXT_HOPm,
        },
        .entries = 10,
        .entry = bcm56990_a0_lrd_l3_uc_nhopt_egr_l3_next_hop_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_L3_NEXT_HOP_2m,
        },
        .entries = 2,
        .entry = bcm56990_a0_lrd_l3_uc_nhopt_egr_l3_next_hop_2_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = ING_L3_NEXT_HOPm,
        },
        .entries = 8,
        .entry = bcm56990_a0_lrd_l3_uc_nhopt_ing_l3_next_hop_map_entry
    },
};
const bcmlrd_map_t bcm56990_a0_lrd_l3_uc_nhop_map = {
    .src_id = L3_UC_NHOPt,
    .field_data = &bcm56990_a0_lrd_l3_uc_nhop_map_field_data,
    .groups = 3,
    .group  = bcm56990_a0_lrd_l3_uc_nhop_map_group,
    .table_attr = &bcm56990_a0_lrd_l3_uc_nhopt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE,
    .sel = &bcm56990_a0_lrd_l3_uc_nhop_map_select_data,
};
