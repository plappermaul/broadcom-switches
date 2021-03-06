/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by c-struct-gen from PTRM data files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/c-struct-gen
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

static const bcmcth_trunk_vp_member_field_info_t bcm56780_a0_trunk_vp_flds_mem0[4];
static const bcmcth_trunk_vp_member_field_info_t bcm56780_a0_trunk_vp_wflds_mem0[4];
static const bcmcth_trunk_vp_member_info_t bcm56780_a0_trunk_vp_member0_info;
static const bcmcth_trunk_vp_member_info_t bcm56780_a0_trunk_vp_wmember0_info;
static const uint32_t bcm56780_a0_trunk_vp_mem0[1];

static const bcmcth_trunk_vp_member_field_info_t bcm56780_a0_trunk_vp_flds_mem0[4] = {
    {
        .end_bit = 15,
        .grp_fld = TRUNK_VPt_DVPf,
        .mbr_fld = DATAf,
        .start_bit = 0,
    },
    {
        .end_bit = 31,
        .grp_fld = TRUNK_VPt_NHOP_INDEX_1f,
        .mbr_fld = DATAf,
        .start_bit = 16,
    },
    {
        .end_bit = 47,
        .grp_fld = TRUNK_VPt_NHOP_2_OR_ECMP_GROUP_INDEX_1f,
        .mbr_fld = DATAf,
        .start_bit = 32,
    },
    {
        .end_bit = 51,
        .grp_fld = TRUNK_VPt_ECMP_AND_PROT_SWT_SFC_PROFILE_INDEXf,
        .mbr_fld = DATAf,
        .start_bit = 48,
    },
};
static const bcmcth_trunk_vp_member_field_info_t bcm56780_a0_trunk_vp_wflds_mem0[4] = {
    {
        .end_bit = 15,
        .grp_fld = TRUNK_VP_WEIGHTEDt_DVPf,
        .mbr_fld = DATAf,
        .start_bit = 0,
    },
    {
        .end_bit = 31,
        .grp_fld = TRUNK_VP_WEIGHTEDt_NHOP_INDEX_1f,
        .mbr_fld = DATAf,
        .start_bit = 16,
    },
    {
        .end_bit = 47,
        .grp_fld = TRUNK_VP_WEIGHTEDt_NHOP_2_OR_ECMP_GROUP_INDEX_1f,
        .mbr_fld = DATAf,
        .start_bit = 32,
    },
    {
        .end_bit = 51,
        .grp_fld = TRUNK_VP_WEIGHTEDt_ECMP_AND_PROT_SWT_SFC_PROFILE_INDEXf,
        .mbr_fld = DATAf,
        .start_bit = 48,
    },
};
static const bcmcth_trunk_vp_member_info_t bcm56780_a0_trunk_vp_member0_info = {
    .flds =     bcm56780_a0_trunk_vp_flds_mem0,
    .flds_count = 4,/* implicit length assignment */
    .grp_tbl = TRUNK_VPt,
    .mbr_tbl =     bcm56780_a0_trunk_vp_mem0,
    .mbr_tbl_count = 1,/* implicit length assignment */
};
static const bcmcth_trunk_vp_member_info_t bcm56780_a0_trunk_vp_wmember0_info = {
    .flds =     bcm56780_a0_trunk_vp_wflds_mem0,
    .flds_count = 4,/* implicit length assignment */
    .grp_tbl = TRUNK_VP_WEIGHTEDt,
    .mbr_tbl =     bcm56780_a0_trunk_vp_mem0,
    .mbr_tbl_count = 1,/* implicit length assignment */
};
static const uint32_t bcm56780_a0_trunk_vp_mem0[1] = {
    MEMDB_ECMP_LEVEL0_MEM0m,
};

