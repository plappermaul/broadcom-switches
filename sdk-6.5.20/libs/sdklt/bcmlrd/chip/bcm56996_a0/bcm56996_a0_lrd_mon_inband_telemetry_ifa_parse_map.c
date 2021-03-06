/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/mon/bcm56996_a0/bcm56996_a0_MON_INBAND_TELEMETRY_IFA_PARSE.map.ltl for
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
/* MON_INBAND_TELEMETRY_IFA_PARSE field init */
static const bcmlrd_field_data_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_field_data_mmd[] = {
    { /* 0 VERSION */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 1 IP_PROTO */
      .flags = 0,
      .min = &bcm56996_a0_lrd_ifd_u8_0x0,
      .def = &bcm56996_a0_lrd_ifd_u8_0x0,
      .max = &bcm56996_a0_lrd_ifd_u8_0xff,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_field_data = {
    .fields = 2,
    .field = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_attr_group = {
    .attributes = 1,
    .attr = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_attr_entry,
};

static const bcmlrd_map_entry_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_egr_ifa_hdr_config_0_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFA_VALIDf,
            .field_idx = 0,
            .minbit    = 8,
            .maxbit    = 8,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFAf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MON_INBAND_TELEMETRY_IFA_PARSEt_IP_PROTOf,
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
static const bcmlrd_map_entry_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_egr_ifa_hdr_config_1_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFA_VALIDf,
            .field_idx = 0,
            .minbit    = 8,
            .maxbit    = 8,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFAf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MON_INBAND_TELEMETRY_IFA_PARSEt_IP_PROTOf,
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
static const bcmlrd_map_entry_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_ifa_detect_config_0_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = VERSIONf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 3,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MON_INBAND_TELEMETRY_IFA_PARSEt_VERSIONf,
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
static const bcmlrd_map_entry_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_ifa_hdr_config_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFA_VALIDf,
            .field_idx = 0,
            .minbit    = 8,
            .maxbit    = 8,
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
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = IP_PROTO_IFAf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 7,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MON_INBAND_TELEMETRY_IFA_PARSEt_IP_PROTOf,
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
static const bcmlrd_map_group_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_IFA_HDR_CONFIG_0r,
        },
        .entries = 2,
        .entry = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_egr_ifa_hdr_config_0_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = EGR_IFA_HDR_CONFIG_1r,
        },
        .entries = 2,
        .entry = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_egr_ifa_hdr_config_1_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IFA_DETECT_CONFIG_0r,
        },
        .entries = 1,
        .entry = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_ifa_detect_config_0_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IFA_HDR_CONFIGr,
        },
        .entries = 2,
        .entry = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_ifa_hdr_config_map_entry
    },
};
const bcmlrd_map_t bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map = {
    .src_id = MON_INBAND_TELEMETRY_IFA_PARSEt,
    .field_data = &bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_field_data,
    .groups = 4,
    .group  = bcm56996_a0_lrd_mon_inband_telemetry_ifa_parse_map_group,
    .table_attr = &bcm56996_a0_lrd_mon_inband_telemetry_ifa_parset_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
