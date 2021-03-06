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

#include <bcm_int/ltsw/mbcm/rx.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>
#include <bcm_int/ltsw/rx_int.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_RX

/*!
 * \brief The Rx mbcm drivers.
 */
static mbcm_ltsw_rx_drv_t mbcm_ltsw_rx_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_rx_drv_set(
    int unit,
    mbcm_ltsw_rx_drv_t *drv)
{
    mbcm_ltsw_rx_drv_t *local = &mbcm_ltsw_rx_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_cosq_mapping_extended_add(
    int unit,
    uint32 options,
    bcm_rx_cosq_mapping_t *rx_cosq_mapping)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_cosq_mapping_extended_add(unit, options, rx_cosq_mapping));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_cosq_mapping_extended_delete(
    int unit,
    bcm_rx_cosq_mapping_t *rx_cosq_mapping)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_cosq_mapping_extended_delete(unit, rx_cosq_mapping));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_cosq_mapping_extended_delete_all(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_cosq_mapping_extended_delete_all(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_cosq_mapping_extended_get(
    int unit,
    bcm_rx_cosq_mapping_t *rx_cosq_mapping)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_cosq_mapping_extended_get(unit, rx_cosq_mapping));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_queue_channel_get(
    int unit,
    bcm_cos_queue_t queue_id,
    bcm_rx_chan_t *chan_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_queue_channel_get(unit, queue_id, chan_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_queue_channel_set(
    int unit,
    bcm_cos_queue_t queue_id,
    bcm_rx_chan_t chan_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_queue_channel_set(unit, queue_id, chan_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_map_db_init(
    int unit,
    const bcmint_rx_map_db_t ** map_db)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_map_db_init(unit, map_db));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_rx_tm_cos_q_cpu_map_info_get(
    int unit,
    const char *info_id,
    bcmint_tm_cos_q_cpu_map_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_rx_drv[unit].rx_tm_cos_q_cpu_map_info_get(unit, info_id, info));

exit:
    SHR_FUNC_EXIT();
}

