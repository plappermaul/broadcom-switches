/*****************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by xfc_map_parser
 * from the NPL output file(s) header.yml
 * for device bcm56780_a0 and variant dna_2_4_13.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * All Rights Reserved.$
 *
 * Tool Path: $SDK/INTERNAL/fltg/xfc_map_parser
 */

#include <sal/sal_types.h>
#include <shr/shr_error.h>
#include <bsl/bsl.h>
#include <bcmpkt/flexhdr/bcmpkt_svtag_t.h>
#include <bcmpkt/flexhdr/bcmpkt_svtag_t_internal.h>

#define MASK(_bn) (((uint32_t)0x1<<(_bn))-1)
#define WORD_FIELD_GET(_d,_s,_l) (((_d) >> (_s)) & MASK(_l))
#define WORD_FIELD_SET(_d,_s,_l,_v) (_d)=(((_d) & ~(MASK(_l) << (_s))) | (((_v) & MASK(_l)) << (_s)))
#define WORD_FIELD_MASK(_d,_s,_l) (_d)=((_d) | (MASK(_l) << (_s)))

static uint32_t bcmpkt_svtag_t_data_upr_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 16, 16);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_SVTAG_T_DATA_UPR with value %u on device BCM56780_A0_DNA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_svtag_t_data_upr_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_SVTAG_T_DATA_UPR with value %u on device BCM56780_A0_DNA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 16, 16, val);
}

static uint32_t bcmpkt_svtag_t_data_lwr_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 0, 16);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_SVTAG_T_DATA_LWR with value %u on device BCM56780_A0_DNA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_svtag_t_data_lwr_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_SVTAG_T_DATA_LWR with value %u on device BCM56780_A0_DNA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 0, 16, val);
}

static uint32_t bcmpkt_svtag_t_i_support_get(uint32_t *data, uint32_t **addr) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_SVTAG_T_I_SUPPORT on device BCM56780_A0_DNA_2_4_13\n")));
    return true;
}

const bcmpkt_svtag_t_fget_t bcm56780_a0_dna_2_4_13_svtag_t_fget = {
    {
        bcmpkt_svtag_t_data_upr_get,
        bcmpkt_svtag_t_data_lwr_get,
    }
};

const bcmpkt_svtag_t_fset_t bcm56780_a0_dna_2_4_13_svtag_t_fset = {
    {
        bcmpkt_svtag_t_data_upr_set,
        bcmpkt_svtag_t_data_lwr_set,
    }
};


const bcmpkt_svtag_t_figet_t bcm56780_a0_dna_2_4_13_svtag_t_figet = {
    {
        bcmpkt_svtag_t_i_support_get
    }
};

static shr_enum_map_t bcm56780_a0_dna_2_4_13_svtag_t_view_types[] = {
    {NULL, -1},
};

/* -2: unsupported, -1: global, others: view's value */
static int bcm56780_a0_dna_2_4_13_svtag_t_view_infos[BCMPKT_SVTAG_T_FID_COUNT] = {
    -1,
    -1,
};

void bcm56780_a0_dna_2_4_13_svtag_t_view_info_get(bcmpkt_pmd_view_info_t *info)
{
    info->view_infos = bcm56780_a0_dna_2_4_13_svtag_t_view_infos;
    info->view_types = bcm56780_a0_dna_2_4_13_svtag_t_view_types;
    info->view_type_get = NULL;
}
