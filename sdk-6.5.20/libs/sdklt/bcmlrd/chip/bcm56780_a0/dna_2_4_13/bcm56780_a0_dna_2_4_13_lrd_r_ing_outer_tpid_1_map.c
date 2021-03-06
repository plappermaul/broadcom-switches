/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/npl/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_R_ING_OUTER_TPID_1.map.ltl for
 *      bcm56780_a0 variant dna_2_4_13
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
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_field_data.h>
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "../bcm56780_a0_lrd_enumpool.h"
#include "bcm56780_a0_dna_2_4_13_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* R_ING_OUTER_TPID_1 field init */
static const bcmlrd_field_data_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_field_data_mmd[] = {
    { /* 0 TPID */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_field_data = {
    .fields = 1,
    .field = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_attr_group = {
    .attributes = 1,
    .attr = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_attr_entry,
};


const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_src_field_desc_s0[18] = {
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
};

static const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_dst_field_desc[18] = {
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_field_desc_s1[18] = {
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 8,
        .maxbit    = 15,
        .entry_idx = 0,
        .reserved  = 0
    },
    {
        .field_id  = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
        .field_idx = 0,
        .minbit    = 0,
        .maxbit    = 7,
        .entry_idx = 0,
        .reserved  = 0
    },
};

static const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_field_desc[18] = {
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 0,
        .reserved  = 0,
    },
    {
        .field_id  = KEYf,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 0,
        .reserved  = 0,
    },
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_fwd_s0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_DNA_2_4_13_LTA_BCMLTX_FIELD_DEMUX_R_ING_OUTER_TPID_1T_TPIDF_0_XFRM_HANDLER_FWD_S0_D0_ID,
    .src_fields = 18,
    .src = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_src_field_desc_s0,
    .dst_fields = 18,
    .dst = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_dst_field_desc,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_rev_s0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_DNA_2_4_13_LTA_BCMLTX_FIELD_DEMUX_R_ING_OUTER_TPID_1T_TPIDF_0_XFRM_HANDLER_REV_S0_D0_ID,
    .src_fields = 18,
    .src = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_dst_field_desc,
    .dst_fields = 18,
    .dst = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_0_src_field_desc_s0,
};

const bcmlrd_field_xfrm_desc_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_s0_d0_desc = {
    .handler_id = BCMLTD_TRANSFORM_BCM56780_A0_DNA_2_4_13_LTA_BCMLTX_FIELD_DEMUX_R_ING_OUTER_TPID_1T_TPIDF_1_XFRM_HANDLER_FWD_S0_D0_ID,
    .src_fields = 18,
    .src = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_field_desc_s1,
    .dst_fields = 18,
    .dst = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_field_desc,
};


static const bcmlrd_map_entry_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_flex_hve_iparser1_scc_profile2_5_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_MAPPED_VALUE,
        .desc = {
            .field_id  = DATAf,
            .field_idx = 0,
            .minbit    = 32,
            .maxbit    = 47,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .mapped = {
                .src = {
                    .field_id = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
                    .field_idx = 0,
                    .minbit    = 0,
                    .maxbit    = 15,
                    .entry_idx = 0,
                    .reserved  = 0
                }
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
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
};
static const bcmlrd_map_entry_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_iparser1_hme_stage0_tcam_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 15,
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 1,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 16,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 2,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 17,
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 3,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 3,
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 4,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 33,
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 5,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 34,
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 6,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 35,
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 7,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 4,
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 8,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 5,
            }
        },
    },
    { /* 9 */
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
                /* handler: bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_fwd_s0_d0 */
                .desc = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_fwd_s0_d0_desc,
            },
        },
    },
    { /* 10 */
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
                /* handler: bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_rev_s0_d0 */
                .desc = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_0_xfrm_handler_rev_s0_d0_desc,
            },
        },
    },
};
static const bcmlrd_map_entry_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_iparser2_hme_stage0_tcam_map_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 15,
            }
        },
    },
    { /* 1 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 1,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 16,
            }
        },
    },
    { /* 2 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 2,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 17,
            }
        },
    },
    { /* 3 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 3,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 3,
            }
        },
    },
    { /* 4 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 4,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 33,
            }
        },
    },
    { /* 5 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 5,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 34,
            }
        },
    },
    { /* 6 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 6,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 35,
            }
        },
    },
    { /* 7 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 7,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 4,
            }
        },
    },
    { /* 8 */
        .entry_type = BCMLRD_MAP_ENTRY_FIXED_KEY,
        .desc = {
            .field_id  = BCMLRD_FIELD_INDEX,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 5,
            .entry_idx = 8,
            .reserved  = 0
        },
        .u = {
            .fixed = {
                .value = 5,
            }
        },
    },
    { /* 9 */
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
                /* handler: bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_s0_d0 */
                .desc = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_s0_d0_desc,
            },
        },
    },
};
static const bcmlrd_map_group_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = FLEX_HVE_IPARSER1_SCC_PROFILE2_5m,
        },
        .entries = 2,
        .entry = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_flex_hve_iparser1_scc_profile2_5_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IPARSER1_HME_STAGE0_TCAMm,
        },
        .entries = 11,
        .entry = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_iparser1_hme_stage0_tcam_map_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_PHYSICAL,
            .id = IPARSER2_HME_STAGE0_TCAMm,
        },
        .entries = 10,
        .entry = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_iparser2_hme_stage0_tcam_map_entry
    },
};
const bcmlrd_map_t bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map = {
    .src_id = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t,
    .field_data = &bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_field_data,
    .groups = 3,
    .group  = bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1_map_group,
    .table_attr = &bcm56780_a0_dna_2_4_13_lrd_r_ing_outer_tpid_1t_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_INSERT | BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_DELETE
};
