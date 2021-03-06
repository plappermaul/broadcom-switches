/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/l2/bcm56996_a0/bcm56996_a0_L2_VFI_FDB.map.ltl for
 *      bcm56996_a0
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
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56996_a0/bcm56996_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56996_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56996_a0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* L2_VFI_FDB field init */
static const bcmlrd_field_data_t bcm56996_a0_lrd_l2_vfi_fdb_map_field_data_mmd[] = {
    { /* 0 VFI_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x1fff,
      .depth = 0,
      .width = 13,
      .edata = NULL,
    },
    { /* 1 MAC_ADDR */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56996_a0_lrd_ifd_u64_0x0,
      .def = &bcm56996_a0_lrd_ifd_u64_0x0,
      .max = &bcm56996_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 2 DEST_TYPE */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56996_a0_lrd_ifd_u32_0x0,
      .def = &bcm56996_a0_lrd_ifd_u32_0x0,
      .max = &bcm56996_a0_lrd_ifd_u32_0x5,
      .depth = 0,
      .width = 3,
      .edata = BCM56996_A0_LRD_L2_DEST_T_DATA,
    },
    { /* 3 MODID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 4 MODPORT */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x10f,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 5 TRUNK_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 6 L2_MC_GROUP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x1ff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 7 TM_MC_GROUP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x1,
      .def = &bcm56996_a0_lrd_ifd_u16_0x1,
      .max = &bcm56996_a0_lrd_ifd_u16_0x1ff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 8 NHOP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x1,
      .def = &bcm56996_a0_lrd_ifd_u16_0x1,
      .max = &bcm56996_a0_lrd_ifd_u16_0x7fff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 9 ECMP_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 15,
      .edata = NULL,
    },
    { /* 10 CLASS_ID */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x3f,
      .depth = 0,
      .width = 6,
      .edata = NULL,
    },
    { /* 11 ASSIGN_INT_PRI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 12 INT_PRI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 13 DST_DROP */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 14 COPY_TO_CPU */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 15 SRC_DROP */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 16 TRUST_SRC_INT_PRI */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 17 STATIC */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 18 PENDING */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 19 DVP */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u16_0x0,
      .def = &bcm56996_a0_lrd_ifd_u16_0x0,
      .max = &bcm56996_a0_lrd_ifd_u16_0x1fff,
      .depth = 0,
      .width = 13,
      .edata = NULL,
    },
    { /* 20 HIT_SRC_MAC */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 21 HIT_DST_MAC */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 22 HIT_LOCAL_SRC_MAC */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56996_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 23 L2_DST_BLOCK_ID */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56996_a0_lrd_l2_vfi_fdb_map_field_data = {
    .fields = 24,
    .field = bcm56996_a0_lrd_l2_vfi_fdb_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56996_a0_lrd_l2_vfi_fdbt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_SIZE,
        .value = 8192,
    },
};

static const bcmlrd_map_attr_t bcm56996_a0_lrd_l2_vfi_fdbt_attr_group = {
    .attributes = 2,
    .attr = bcm56996_a0_lrd_l2_vfi_fdbt_attr_entry,
};


const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_src_field_desc_s0[1] = {
    {
        .field_id  = L2_VFI_FDBt_DEST_TYPEf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 31,
        .entry_idx = 0,
        .reserved  = 0
    },
};

const bcmltd_field_desc_t
bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_l2_vfi_fdb_dst_field_desc_d0[1] = {
    {
        .field_id  = L2v_DEST_TYPEf,
        .field_idx = 0,
        .minbit    = 71,
        .maxbit    = 73,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_fwd_s0_k0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMLTX_L2_VFI_FDB_DEST_XFRM_HANDLER_FWD_S0_K0_D0_ID,
    .src_fields = 1,
    .src = bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_src_field_desc_s0,
    .dst_fields = 1,
    .dst = bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_l2_vfi_fdb_dst_field_desc_d0,
};

const bcmlrd_field_xfrm_desc_t
bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_rev_s0_k0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56996_A0_LTA_BCMLTX_L2_VFI_FDB_DEST_XFRM_HANDLER_REV_S0_K0_D0_ID,
    .src_fields = 1,
    .src = bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_l2_vfi_fdb_dst_field_desc_d0,
    .dst_fields = 1,
    .dst = bcm56996_a0_lrd_bcmltx_l2_vfi_fdb_dest_src_field_desc_s0,
};


static const bcmlrd_map_entry_t bcm56996_a0_lrd_l2_vfi_fdbt_l2_entry_single_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BASE_VALIDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 2,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 1,
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = KEY_TYPEf,
            .field_idx = 0,
            .minbit    = 3,
            .maxbit    = 3,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = L2v_VFIf,
            .field_idx = 0,
            .minbit    = 4,
            .maxbit    = 16,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_VFI_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 12,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = L2v_MAC_ADDRf,
            .field_idx = 0,
            .minbit    = 17,
            .maxbit    = 64,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_MAC_ADDRf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 47,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_MODPORTf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_TRUNK_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_L2_MC_GROUP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_TM_MC_GROUP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_NHOP_IDf,
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
            .field_id  = L2v_DESTINATIONf,
            .field_idx = 0,
            .minbit    = 74,
            .maxbit    = 88,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_ECMP_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 14,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 10 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_CLASS_IDf,
            .field_idx = 0,
            .minbit    = 103,
            .maxbit    = 108,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_CLASS_IDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 5,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 11 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_RPEf,
            .field_idx = 0,
            .minbit    = 109,
            .maxbit    = 109,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_ASSIGN_INT_PRIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 12 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_PRIf,
            .field_idx = 0,
            .minbit    = 110,
            .maxbit    = 113,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_INT_PRIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 3,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 13 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DST_DISCARDf,
            .field_idx = 0,
            .minbit    = 114,
            .maxbit    = 114,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_DST_DROPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 14 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_CPUf,
            .field_idx = 0,
            .minbit    = 115,
            .maxbit    = 115,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_COPY_TO_CPUf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 15 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_SRC_DISCARDf,
            .field_idx = 0,
            .minbit    = 116,
            .maxbit    = 116,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_SRC_DROPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 16 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_SCPf,
            .field_idx = 0,
            .minbit    = 117,
            .maxbit    = 117,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_TRUST_SRC_INT_PRIf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 17 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_STATIC_BITf,
            .field_idx = 0,
            .minbit    = 119,
            .maxbit    = 119,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_STATICf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 18 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_PENDINGf,
            .field_idx = 0,
            .minbit    = 118,
            .maxbit    = 118,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_PENDINGf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 19 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_DVPf,
            .field_idx = 0,
            .minbit    = 90,
            .maxbit    = 102,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_DVPf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 12,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 20 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_RESERVED_1f,
            .field_idx = 0,
            .minbit    = 67,
            .maxbit    = 67,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_HIT_SRC_MACf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 21 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = L2v_RESERVED_1f,
            .field_idx = 0,
            .minbit    = 68,
            .maxbit    = 68,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_VFI_FDBt_HIT_DST_MACf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 22 */
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
                /* handler: bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_fwd_s0_k0_d0 */
                .desc = &bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_fwd_s0_k0_d0_desc,
            },
        },
    },
    { /* 23 */
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
                /* handler: bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_rev_s0_k0_d0 */
                .desc = &bcm56996_a0_lta_bcmltx_l2_vfi_fdb_dest_xfrm_handler_rev_s0_k0_d0_desc,
            },
        },
    },
};
const bcmlrd_field_selector_t bcm56996_a0_lrd_l2_vfi_fdb_map_select[] = {
    { /* Node:0, Type:ROOT, DEST_TYPE */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_ROOT,
        .field_id = L2_VFI_FDBt_DEST_TYPEf,
        .group_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .entry_index = BCMLRD_INVALID_SELECTOR_INDEX,
        .selection_parent = BCMLRD_INVALID_SELECTOR_INDEX,
        .group = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_id = BCMLRD_INVALID_SELECTOR_INDEX,
        .selector_value = BCMLRD_INVALID_SELECTOR_INDEX
    },
    { /* Node:1, Type:Directmap, MODPORT:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_MODPORTf,
        .group_index = 0,
        .entry_index = 4,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 0 /* PORT */
    },
    { /* Node:2, Type:Directmap, TRUNK_ID:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_TRUNK_IDf,
        .group_index = 0,
        .entry_index = 5,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 1 /* TRUNK */
    },
    { /* Node:3, Type:Directmap, L2_MC_GROUP_ID:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_L2_MC_GROUP_IDf,
        .group_index = 0,
        .entry_index = 6,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 2 /* L2_MC_GRP */
    },
    { /* Node:4, Type:Directmap, TM_MC_GROUP_ID:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_TM_MC_GROUP_IDf,
        .group_index = 0,
        .entry_index = 7,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 3 /* L3_MC_GRP */
    },
    { /* Node:5, Type:Directmap, NHOP_ID:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_NHOP_IDf,
        .group_index = 0,
        .entry_index = 8,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 4 /* NHOP */
    },
    { /* Node:6, Type:Directmap, ECMP_ID:L2_ENTRY_SINGLE.L2::DESTINATION */
        .selector_type = BCMLRD_FIELD_SELECTOR_TYPE_MAP_ENTRY,
        .field_id = L2_VFI_FDBt_ECMP_IDf,
        .group_index = 0,
        .entry_index = 9,
        .selection_parent = 0,
        .group = 0,
        .selector_id = L2_VFI_FDBt_DEST_TYPEf,
        .selector_value = 5 /* ECMP_GRP */
    },
};

const bcmlrd_field_selector_data_t bcm56996_a0_lrd_l2_vfi_fdb_map_select_data = {
    .num_field_selector = 7,
    .field_selector = bcm56996_a0_lrd_l2_vfi_fdb_map_select,
};

static const bcmlrd_map_group_t bcm56996_a0_lrd_l2_vfi_fdb_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = L2_ENTRY_SINGLEm,
        },
        .entries = 24,
        .entry = bcm56996_a0_lrd_l2_vfi_fdbt_l2_entry_single_map_entry
    },
};
const bcmlrd_map_t bcm56996_a0_lrd_l2_vfi_fdb_map = {
    .src_id = L2_VFI_FDBt,
    .field_data = &bcm56996_a0_lrd_l2_vfi_fdb_map_field_data,
    .groups = 1,
    .group  = bcm56996_a0_lrd_l2_vfi_fdb_map_group,
    .table_attr = &bcm56996_a0_lrd_l2_vfi_fdbt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE,
    .sel = &bcm56996_a0_lrd_l2_vfi_fdb_map_select_data,
};
