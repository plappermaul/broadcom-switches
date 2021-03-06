/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/mirror/bcm56880_a0/bcm56880_a0_MIRROR_PORT_ENCAP_SFLOW.map.ltl for
 *      bcm56880_a0
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
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_field_data.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56880_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "bcm56880_a0_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* MIRROR_PORT_ENCAP_SFLOW field init */
static const bcmlrd_field_data_t bcm56880_a0_lrd_mirror_port_encap_sflow_map_field_data_mmd[] = {
    { /* 0 PORT_ID */
      .flags = BCMLTD_FIELD_F_KEY,
      .min = &bcm56880_a0_lrd_ifd_u16_0x0,
      .def = &bcm56880_a0_lrd_ifd_u16_0x0,
      .max = &bcm56880_a0_lrd_ifd_u16_0x9f,
      .depth = 0,
      .width = 8,
      .edata = NULL,
    },
    { /* 1 SAMPLE_ING */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 2 SAMPLE_EGR */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x1,
      .depth = 0,
      .width = 1,
      .edata = NULL,
    },
    { /* 3 SAMPLE_ING_FLEX */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 4 SAMPLE_ING_CPU */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 5 SAMPLE_ING_MIRROR_INSTANCE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 6 SAMPLE_ING_FLEX_CPU */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 7 SAMPLE_ING_FLEX_MIRROR_INSTANCE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .def = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .max = &bcm56880_a0_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 8 SAMPLE_ING_RATE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 9 SAMPLE_EGR_RATE */
      .flags = 0,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0x1fffffff,
      .depth = 0,
      .width = 29,
      .edata = NULL,
    },
    { /* 10 SAMPLE_ING_FLEX_RATE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56880_a0_lrd_ifd_u32_0x0,
      .def = &bcm56880_a0_lrd_ifd_u32_0x0,
      .max = &bcm56880_a0_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56880_a0_lrd_mirror_port_encap_sflow_map_field_data = {
    .fields = 11,
    .field = bcm56880_a0_lrd_mirror_port_encap_sflow_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56880_a0_lrd_mirror_port_encap_sflowt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56880_a0_lrd_mirror_port_encap_sflowt_attr_group = {
    .attributes = 1,
    .attr = bcm56880_a0_lrd_mirror_port_encap_sflowt_attr_entry,
};


const bcmltd_field_desc_t
bcm56880_a0_lrd_bcmltx_id_to_table_sel_src_field_desc_s1[1] = {
    {
        .field_id  = MIRROR_PORT_ENCAP_SFLOWt_PORT_IDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
};

const bcmltd_field_desc_t
bcm56880_a0_lrd_bcmltx_id_to_table_sel_mirror_port_encap_sflow_dst_field_desc_d0[1] = {
    {
        .field_id  = BCMLRD_FIELD_TABLE_SEL,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const uint32_t
bcm56880_a0_lrd_id_to_table_sel_transform_tbl_t1[160] = {
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_0m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_1m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_2m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_3m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_4m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_5m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_6m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_7m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_8m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_9m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_10m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_11m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_12m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_13m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_14m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_15m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_16m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_17m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_18m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_19m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_20m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_21m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_22m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_23m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_24m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_25m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_26m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_27m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_28m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_29m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_30m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_31m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_32m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_33m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_34m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_35m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_36m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_37m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_38m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_39m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_40m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_41m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_42m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_43m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_44m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_45m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_46m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_47m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_48m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_49m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_50m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_51m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_52m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_53m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_54m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_55m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_56m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_57m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_58m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_59m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_60m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_61m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_62m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_63m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_64m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_65m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_66m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_67m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_68m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_69m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_70m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_71m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_72m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_73m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_74m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_75m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_76m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_77m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_78m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_79m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_80m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_81m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_82m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_83m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_84m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_85m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_86m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_87m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_88m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_89m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_90m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_91m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_92m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_93m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_94m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_95m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_96m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_97m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_98m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_99m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_100m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_101m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_102m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_103m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_104m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_105m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_106m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_107m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_108m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_109m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_110m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_111m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_112m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_113m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_114m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_115m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_116m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_117m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_118m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_119m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_120m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_121m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_122m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_123m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_124m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_125m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_126m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_127m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_128m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_129m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_130m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_131m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_132m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_133m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_134m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_135m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_136m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_137m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_138m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_139m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_140m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_141m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_142m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_143m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_144m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_145m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_146m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_147m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_148m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_149m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_150m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_151m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_152m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_153m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_154m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_155m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_156m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_157m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_158m,
    IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_159m,
};

const bcmlrd_field_xfrm_desc_t
bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_fwd_s1_d0_t1_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56880_A0_LTA_BCMLTX_ID_TO_TABLE_SEL_XFRM_HANDLER_FWD_S1_D0_T1_ID,
    .src_fields = 1,
    .src = bcm56880_a0_lrd_bcmltx_id_to_table_sel_src_field_desc_s1,
    .dst_fields = 1,
    .dst = bcm56880_a0_lrd_bcmltx_id_to_table_sel_mirror_port_encap_sflow_dst_field_desc_d0,
    .tables = 160,
    .tbl = bcm56880_a0_lrd_id_to_table_sel_transform_tbl_t1,
};

const bcmlrd_field_xfrm_desc_t
bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_rev_s1_d0_t1_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56880_A0_LTA_BCMLTX_ID_TO_TABLE_SEL_XFRM_HANDLER_REV_S1_D0_T1_ID,
    .src_fields = 1,
    .src = bcm56880_a0_lrd_bcmltx_id_to_table_sel_mirror_port_encap_sflow_dst_field_desc_d0,
    .dst_fields = 1,
    .dst = bcm56880_a0_lrd_bcmltx_id_to_table_sel_src_field_desc_s1,
    .tables = 160,
    .tbl = bcm56880_a0_lrd_id_to_table_sel_transform_tbl_t1,
};


static const bcmlrd_map_entry_t bcm56880_a0_lrd_mirror_port_encap_sflowt_ipost_mirror_sampler_egress_sampler_config_table_0_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = ENABLEf,
            .field_idx = 0,
            .minbit    = 29,
            .maxbit    = 29,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MIRROR_PORT_ENCAP_SFLOWt_SAMPLE_EGRf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 0,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = THRESHOLDf,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 28,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = MIRROR_PORT_ENCAP_SFLOWt_SAMPLE_EGR_RATEf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 28,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_FWD_KEY_FIELD_XFRM_HANDLER,
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
                /* handler: bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_fwd_s1_d0_t1 */
                .desc = &bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_fwd_s1_d0_t1_desc,
            },
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_REV_KEY_FIELD_XFRM_HANDLER,
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
                /* handler: bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_rev_s1_d0_t1 */
                .desc = &bcm56880_a0_lta_bcmltx_id_to_table_sel_xfrm_handler_rev_s1_d0_t1_desc,
            },
        },
    },
};
static const bcmlrd_map_group_t bcm56880_a0_lrd_mirror_port_encap_sflow_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IPOST_MIRROR_SAMPLER_EGRESS_SAMPLER_CONFIG_TABLE_0m,
        },
        .entries = 4,
        .entry = bcm56880_a0_lrd_mirror_port_encap_sflowt_ipost_mirror_sampler_egress_sampler_config_table_0_map_entry
    },
};
const bcmlrd_map_t bcm56880_a0_lrd_mirror_port_encap_sflow_map = {
    .src_id = MIRROR_PORT_ENCAP_SFLOWt,
    .field_data = &bcm56880_a0_lrd_mirror_port_encap_sflow_map_field_data,
    .groups = 1,
    .group  = bcm56880_a0_lrd_mirror_port_encap_sflow_map_group,
    .table_attr = &bcm56880_a0_lrd_mirror_port_encap_sflowt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
