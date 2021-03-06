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

#include <bcm_int/ltsw/mbcm/stat.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_STAT

/*!
 * \brief The Stat mbcm drivers.
 */
static mbcm_ltsw_stat_drv_t mbcm_ltsw_stat_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_stat_drv_set(
    int unit,
    mbcm_ltsw_stat_drv_t *drv)
{
    mbcm_ltsw_stat_drv_t *local = &mbcm_ltsw_stat_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_db_get(
    int unit,
    bcmint_stat_db_t *stat_db)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_db_get(unit, stat_db));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_dev_info_get(
    int unit,
    const bcmint_stat_dev_info_t **devinfo)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_dev_info_get(unit, devinfo));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_index_init(
    int unit,
    const bcmint_stat_dev_info_t *dev_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_index_init(unit, dev_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_lt_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_lt_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_port_attach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_port_attach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_port_detach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_port_detach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_clear(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_clear(unit, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_clear_single(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_clear_single(unit, port, stat_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_get(
    int unit,
    bcm_port_t port,
    bool sync_mode,
    bcm_stat_val_t stat_id,
    uint64_t *val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_get(unit, port, sync_mode, stat_id, val));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_drop_event_counter_get(
    int unit,
    bool sync_mode,
    bcm_pkt_drop_event_t type,
    uint64_t *val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_drop_event_counter_get(unit, sync_mode, type, val));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_drop_event_counter_set(
    int unit,
    bcm_pkt_drop_event_t type,
    uint64_t val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_drop_event_counter_set(unit, type, val));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_trace_event_counter_get(
    int unit,
    bool sync_mode,
    bcm_pkt_trace_event_t type,
    uint64_t *val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_trace_event_counter_get(unit, sync_mode, type, val));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_trace_event_counter_set(
    int unit,
    bcm_pkt_trace_event_t type,
    uint64_t val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_trace_event_counter_set(unit, type, val));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_counter_control_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_counter_control_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_counter_control_get(
    int unit,
    bcmi_ltsw_stat_counter_control_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_counter_control_get(unit, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_custom_add(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_custom_add(unit, port, stat_id, trigger));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_custom_check(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger,
    int *result)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_custom_check(unit, port, stat_id, trigger, result));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_custom_delete(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_custom_delete(unit, port, stat_id, trigger));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_stat_custom_delete_all(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv[unit].stat_custom_delete_all(unit, port, stat_id));

exit:
    SHR_FUNC_EXIT();
}

