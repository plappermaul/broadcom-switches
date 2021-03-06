/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from Logical Table mapping files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/* Logical Table Adaptor for component bcmltx */
/* Handler: bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmltx/bcmltx_field_demux.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_xfrm_field_desc.h>


extern const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_field_desc_s1[];

static const bcmltd_field_desc_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_field_desc[18] = {
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 364,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 0,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY0[143:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 365,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 0,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY0[135:128]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 366,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 1,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY1[143:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 367,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 1,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY1[135:128]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 336,
        .field_idx = 0,
        .minbit    = 136,
        .maxbit    = 143,
        .entry_idx = 2,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY2[143:136]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 337,
        .field_idx = 0,
        .minbit    = 128,
        .maxbit    = 135,
        .entry_idx = 2,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY2[135:128]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 368,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 3,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY3[191:184]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 369,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 3,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY3[183:176]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 370,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 4,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY4[127:120]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 371,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 4,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY4[119:112]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 372,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 5,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY5[127:120]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 373,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 5,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY5[119:112]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 344,
        .field_idx = 0,
        .minbit    = 120,
        .maxbit    = 127,
        .entry_idx = 6,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY6[127:120]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 345,
        .field_idx = 0,
        .minbit    = 112,
        .maxbit    = 119,
        .entry_idx = 6,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY6[119:112]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 374,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 7,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY7[191:184]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 375,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 7,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY7[183:176]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 376,
        .field_idx = 0,
        .minbit    = 184,
        .maxbit    = 191,
        .entry_idx = 8,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY8[191:184]",
        .reserved  = 0
    },
    {
        .field_id  = BCMLTD_INTERNAL_FIELD_BASE + 377,
        .field_idx = 0,
        .minbit    = 176,
        .maxbit    = 183,
        .entry_idx = 8,
        .sym       = "IPARSER2_HME_STAGE0_TCAMm.KEYf.ENTRY8[183:176]",
        .reserved  = 0
    },
};

static const
bcmltd_field_list_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_list_s0 = {
    .field_num = 18,
    .field_array = bcm56780_a0_dna_2_4_13_lrd_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_field_desc_s1
};

static const bcmltd_field_list_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_list_d0 = {
    .field_num = 18,
    .field_array = bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_field_desc
};

static const uint32_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_transform_src_s0[1] = {
    BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t_TPIDf,
};

static const uint32_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_transform_dst_d0[18] = {
    BCMLTD_INTERNAL_FIELD_BASE + 364,
    BCMLTD_INTERNAL_FIELD_BASE + 365,
    BCMLTD_INTERNAL_FIELD_BASE + 366,
    BCMLTD_INTERNAL_FIELD_BASE + 367,
    BCMLTD_INTERNAL_FIELD_BASE + 336,
    BCMLTD_INTERNAL_FIELD_BASE + 337,
    BCMLTD_INTERNAL_FIELD_BASE + 368,
    BCMLTD_INTERNAL_FIELD_BASE + 369,
    BCMLTD_INTERNAL_FIELD_BASE + 370,
    BCMLTD_INTERNAL_FIELD_BASE + 371,
    BCMLTD_INTERNAL_FIELD_BASE + 372,
    BCMLTD_INTERNAL_FIELD_BASE + 373,
    BCMLTD_INTERNAL_FIELD_BASE + 344,
    BCMLTD_INTERNAL_FIELD_BASE + 345,
    BCMLTD_INTERNAL_FIELD_BASE + 374,
    BCMLTD_INTERNAL_FIELD_BASE + 375,
    BCMLTD_INTERNAL_FIELD_BASE + 376,
    BCMLTD_INTERNAL_FIELD_BASE + 377,
};

static const bcmltd_generic_arg_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_comp_data = {
    .sid       = BCM56780_A0_DNA_2_4_13_R_ING_OUTER_TPID_1t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_transform_src_s0,
    .field_list  = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_src_list_s0,
    .rfields     = 18,
    .rfield      = bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_transform_dst_d0,
    .rfield_list = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_dst_list_d0,
    .comp_data   = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_comp_data
};

const bcmltd_xfrm_handler_t
bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_s0_d0 = {
    .transform     = bcmltx_field_demux_transform,
    .arg           = &bcm56780_a0_dna_2_4_13_lta_bcmltx_field_demux_r_ing_outer_tpid_1t_tpidf_1_xfrm_handler_fwd_arg_s0_d0
};


