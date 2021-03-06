/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/types.h>

#include <bcm_int/ltsw/mbcm/pktio.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_PKTIO

/*!
 * \brief The Pktio mbcm drivers.
 */
static mbcm_ltsw_pktio_drv_t mbcm_ltsw_pktio_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_pktio_drv_set(
    int unit,
    mbcm_ltsw_pktio_drv_t *drv)
{
    mbcm_ltsw_pktio_drv_t *local = &mbcm_ltsw_pktio_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_pktio_fid_support_get(
    int unit,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcm_pktio_bitmap_t *support)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_pktio_drv[unit].pktio_fid_support_get(unit, hdr_info, support));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_pktio_fid_info_get(
    int unit,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcmint_pktio_fid_info_t **fid_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_pktio_drv[unit].pktio_fid_info_get(unit, hdr_info, fid_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_pktio_fid_info_lookup(
    int unit,
    int opcode,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcmint_pktio_fid_info_t **fid_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_pktio_drv[unit].pktio_fid_info_lookup(unit, opcode, hdr_info, fid_info));

exit:
    SHR_FUNC_EXIT();
}

