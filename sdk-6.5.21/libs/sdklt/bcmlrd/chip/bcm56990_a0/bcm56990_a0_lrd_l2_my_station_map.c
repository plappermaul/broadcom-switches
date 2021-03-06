/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/l2/bcm56990_a0/bcm56990_a0_L2_MY_STATION.map.ltl for
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
/* L2_MY_STATION field init */
static const bcmlrd_field_data_t bcm56990_a0_lrd_l2_my_station_map_field_data_mmd[] = {
    { /* 0 VLAN_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 1 VLAN_ID_MASK */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u16_0x0,
      .def = &bcm56990_a0_lrd_ifd_u16_0x0,
      .max = &bcm56990_a0_lrd_ifd_u16_0xfff,
      .depth = 0,
      .width = 12,
      .edata = NULL,
    },
    { /* 2 MAC_ADDR */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u64_0x0,
      .def = &bcm56990_a0_lrd_ifd_u64_0x0,
      .max = &bcm56990_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 3 MAC_ADDR_MASK */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56990_a0_lrd_ifd_u64_0x0,
      .def = &bcm56990_a0_lrd_ifd_u64_0x0,
      .max = &bcm56990_a0_lrd_ifd_u64_0xffffffffffff,
      .depth = 0,
      .width = 48,
      .edata = NULL,
    },
    { /* 4 ENTRY_PRIORITY */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_u32_0x0,
      .def = &bcm56990_a0_lrd_ifd_u32_0x0,
      .max = &bcm56990_a0_lrd_ifd_u32_0x7fffffff,
      .depth = 0,
      .width = 31,
      .edata = NULL,
    },
    { /* 5 IPV6_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 6 IPV6_MC_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 7 IPV4_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 8 IPV4_MC_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 9 ARP_RARP_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 10 MPLS_TERMINATION */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 11 DROP */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 12 COPY_TO_CPU */
      .flags = 0,
      .min = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56990_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56990_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56990_a0_lrd_l2_my_station_map_field_data = {
    .fields = 13,
    .field = bcm56990_a0_lrd_l2_my_station_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56990_a0_lrd_l2_my_stationt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_SIZE,
        .value = 512,
    },
};

static const bcmlrd_map_attr_t bcm56990_a0_lrd_l2_my_stationt_attr_group = {
    .attributes = 2,
    .attr = bcm56990_a0_lrd_l2_my_stationt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56990_a0_lrd_l2_my_stationt_my_station_tcam_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = VALIDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 1,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 3,
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_ENTRY_PRIORITY,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 30,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_ENTRY_PRIORITYf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 30,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = VLAN_IDf,
            .field_idx = 0,
            .minbit    = 50,
            .maxbit    = 61,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_VLAN_IDf,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = VLAN_ID_MASKf,
            .field_idx = 0,
            .minbit    = 130,
            .maxbit    = 141,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_VLAN_ID_MASKf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 11,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = MAC_ADDRf,
            .field_idx = 0,
            .minbit    = 2,
            .maxbit    = 49,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_MAC_ADDRf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 47,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_KEY,
        .desc = {
            .field_id  = MAC_ADDR_MASKf,
            .field_idx = 0,
            .minbit    = 82,
            .maxbit    = 129,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_MAC_ADDR_MASKf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 47,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = Tf,
            .field_idx = 0,
            .minbit    = 71,
            .maxbit    = 71,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = T_MASKf,
            .field_idx = 0,
            .minbit    = 151,
            .maxbit    = 151,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = TGIDf,
            .field_idx = 0,
            .minbit    = 62,
            .maxbit    = 70,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 9 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = TGID_MASKf,
            .field_idx = 0,
            .minbit    = 142,
            .maxbit    = 150,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 0,
            }
        },
    },
    { /* 10 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IPV6_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 184,
            .maxbit    = 184,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_IPV6_TERMINATIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 11 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IPV6_MULTICAST_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 183,
            .maxbit    = 183,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_IPV6_MC_TERMINATIONf,
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
            .field_id  = IPV4_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 182,
            .maxbit    = 182,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_IPV4_TERMINATIONf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 13 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IPV4_MULTICAST_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 181,
            .maxbit    = 181,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_IPV4_MC_TERMINATIONf,
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
            .field_id  = ARP_RARP_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 178,
            .maxbit    = 178,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_ARP_RARP_TERMINATIONf,
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
            .field_id  = MPLS_TERMINATION_ALLOWEDf,
            .field_idx = 0,
            .minbit    = 185,
            .maxbit    = 185,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_MPLS_TERMINATIONf,
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
            .field_id  = DISCARDf,
            .field_idx = 0,
            .minbit    = 180,
            .maxbit    = 180,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_DROPf,
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
            .field_id  = COPY_TO_CPUf,
            .field_idx = 0,
            .minbit    = 179,
            .maxbit    = 179,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = L2_MY_STATIONt_COPY_TO_CPUf,
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
static const bcmlrd_map_group_t bcm56990_a0_lrd_l2_my_station_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = MY_STATION_TCAMm,
        },
        .entries = 18,
        .entry = bcm56990_a0_lrd_l2_my_stationt_my_station_tcam_map_entry
    },
};
const bcmlrd_map_t bcm56990_a0_lrd_l2_my_station_map = {
    .src_id = L2_MY_STATIONt,
    .field_data = &bcm56990_a0_lrd_l2_my_station_map_field_data,
    .groups = 1,
    .group  = bcm56990_a0_lrd_l2_my_station_map_group,
    .table_attr = &bcm56990_a0_lrd_l2_my_stationt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
