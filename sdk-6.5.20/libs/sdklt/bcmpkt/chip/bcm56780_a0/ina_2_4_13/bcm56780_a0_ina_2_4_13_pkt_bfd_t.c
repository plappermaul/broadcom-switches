/*****************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by xfc_map_parser
 * from the NPL output file(s) header.yml
 * for device bcm56780_a0 and variant ina_2_4_13.
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
#include <bcmpkt/flexhdr/bcmpkt_bfd_t.h>
#include <bcmpkt/flexhdr/bcmpkt_bfd_t_internal.h>

#define MASK(_bn) (((uint32_t)0x1<<(_bn))-1)
#define WORD_FIELD_GET(_d,_s,_l) (((_d) >> (_s)) & MASK(_l))
#define WORD_FIELD_SET(_d,_s,_l,_v) (_d)=(((_d) & ~(MASK(_l) << (_s))) | (((_v) & MASK(_l)) << (_s)))
#define WORD_FIELD_MASK(_d,_s,_l) (_d)=((_d) | (MASK(_l) << (_s)))

static uint32_t bcmpkt_bfd_t_detectmult_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 8, 8);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_DETECTMULT with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_detectmult_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_DETECTMULT with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 8, 8, val);
}

static uint32_t bcmpkt_bfd_t_cpi_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 19, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_CPI with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_cpi_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_CPI with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 19, 1, val);
}

static uint32_t bcmpkt_bfd_t_ap_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 18, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_AP with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_ap_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_AP with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 18, 1, val);
}

static uint32_t bcmpkt_bfd_t_mpt_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 16, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_MPT with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_mpt_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_MPT with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 16, 1, val);
}

static uint32_t bcmpkt_bfd_t_desmintxintv_get(uint32_t *data) {
    uint32_t val;

    val = SHR_E_PARAM;
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_DESMINTXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_desmintxintv_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_DESMINTXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    LOG_ERROR(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Invalid param\n")));
}

static uint32_t bcmpkt_bfd_t_reqminrxintv_get(uint32_t *data) {
    uint32_t val;

    val = SHR_E_PARAM;
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_REQMINRXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_reqminrxintv_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_REQMINRXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    LOG_ERROR(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Invalid param\n")));
}

static uint32_t bcmpkt_bfd_t_mydiscrim_get(uint32_t *data) {
    uint32_t val;

    val = SHR_E_PARAM;
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_MYDISCRIM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_mydiscrim_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_MYDISCRIM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    LOG_ERROR(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Invalid param\n")));
}

static uint32_t bcmpkt_bfd_t_poll_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 21, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_POLL with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_poll_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_POLL with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 21, 1, val);
}

static uint32_t bcmpkt_bfd_t_bfd_length_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 0, 8);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_BFD_LENGTH with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_bfd_length_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_BFD_LENGTH with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 0, 8, val);
}

static uint32_t bcmpkt_bfd_t_minechorxintv_get(uint32_t *data) {
    uint32_t val;

    val = SHR_E_PARAM;
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_MINECHORXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_minechorxintv_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_MINECHORXINTV with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    LOG_ERROR(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Invalid param\n")));
}

static uint32_t bcmpkt_bfd_t_sta_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 22, 2);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_STA with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_sta_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_STA with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 22, 2, val);
}

static uint32_t bcmpkt_bfd_t_diag_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 24, 5);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_DIAG with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_diag_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_DIAG with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 24, 5, val);
}

static uint32_t bcmpkt_bfd_t_version_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 29, 3);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_VERSION with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_version_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_VERSION with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 29, 3, val);
}

static uint32_t bcmpkt_bfd_t_dem_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 17, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_DEM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_dem_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_DEM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 17, 1, val);
}

static uint32_t bcmpkt_bfd_t_urdiscrim_get(uint32_t *data) {
    uint32_t val;

    val = SHR_E_PARAM;
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_URDISCRIM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_urdiscrim_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_URDISCRIM with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    LOG_ERROR(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Invalid param\n")));
}

static uint32_t bcmpkt_bfd_t_fin_get(uint32_t *data) {
    uint32_t val;

    val = WORD_FIELD_GET(data[0], 20, 1);
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_FIN with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    return val;
}

static void bcmpkt_bfd_t_fin_set(uint32_t *data, uint32_t val) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Set field BCMPKT_BFD_T_FIN with value %u on device BCM56780_A0_INA_2_4_13\n"), val));

    WORD_FIELD_SET(data[0], 20, 1, val);
}

static uint32_t bcmpkt_bfd_t_i_support_get(uint32_t *data, uint32_t **addr) {
    LOG_DEBUG(BSL_LS_BCMPKT_FLEX_HDR,
        (BSL_META("Get field BCMPKT_BFD_T_I_SUPPORT on device BCM56780_A0_INA_2_4_13\n")));
    return true;
}

const bcmpkt_bfd_t_fget_t bcm56780_a0_ina_2_4_13_bfd_t_fget = {
    {
        bcmpkt_bfd_t_detectmult_get,
        bcmpkt_bfd_t_cpi_get,
        bcmpkt_bfd_t_ap_get,
        bcmpkt_bfd_t_mpt_get,
        bcmpkt_bfd_t_desmintxintv_get,
        bcmpkt_bfd_t_reqminrxintv_get,
        bcmpkt_bfd_t_mydiscrim_get,
        bcmpkt_bfd_t_poll_get,
        bcmpkt_bfd_t_bfd_length_get,
        bcmpkt_bfd_t_minechorxintv_get,
        bcmpkt_bfd_t_sta_get,
        bcmpkt_bfd_t_diag_get,
        bcmpkt_bfd_t_version_get,
        bcmpkt_bfd_t_dem_get,
        bcmpkt_bfd_t_urdiscrim_get,
        bcmpkt_bfd_t_fin_get,
    }
};

const bcmpkt_bfd_t_fset_t bcm56780_a0_ina_2_4_13_bfd_t_fset = {
    {
        bcmpkt_bfd_t_detectmult_set,
        bcmpkt_bfd_t_cpi_set,
        bcmpkt_bfd_t_ap_set,
        bcmpkt_bfd_t_mpt_set,
        bcmpkt_bfd_t_desmintxintv_set,
        bcmpkt_bfd_t_reqminrxintv_set,
        bcmpkt_bfd_t_mydiscrim_set,
        bcmpkt_bfd_t_poll_set,
        bcmpkt_bfd_t_bfd_length_set,
        bcmpkt_bfd_t_minechorxintv_set,
        bcmpkt_bfd_t_sta_set,
        bcmpkt_bfd_t_diag_set,
        bcmpkt_bfd_t_version_set,
        bcmpkt_bfd_t_dem_set,
        bcmpkt_bfd_t_urdiscrim_set,
        bcmpkt_bfd_t_fin_set,
    }
};


const bcmpkt_bfd_t_figet_t bcm56780_a0_ina_2_4_13_bfd_t_figet = {
    {
        bcmpkt_bfd_t_i_support_get
    }
};

static shr_enum_map_t bcm56780_a0_ina_2_4_13_bfd_t_view_types[] = {
    {NULL, -1},
};

/* -2: unsupported, -1: global, others: view's value */
static int bcm56780_a0_ina_2_4_13_bfd_t_view_infos[BCMPKT_BFD_T_FID_COUNT] = {
    -2,
    -1,
    -1,
    -1,
    -2,
    -2,
    -2,
    -1,
    -1,
    -2,
    -1,
    -1,
    -1,
    -1,
    -2,
    -1,
};

void bcm56780_a0_ina_2_4_13_bfd_t_view_info_get(bcmpkt_pmd_view_info_t *info)
{
    info->view_infos = bcm56780_a0_ina_2_4_13_bfd_t_view_infos;
    info->view_types = bcm56780_a0_ina_2_4_13_bfd_t_view_types;
    info->view_type_get = NULL;
}
