/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/stat.h>
#include <bcm_int/ltsw/xgs/stat.h>
#include "sub_dispatch.h"

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_STAT

/******************************************************************************
 * Private functions
 */

static int
bcm56990_a0_ltsw_stat_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_index_init(
    int unit,
    const bcmint_stat_dev_info_t *dev_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_index_init(unit, dev_info));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_lt_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_lt_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_port_attach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_port_attach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_port_detach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_port_detach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_clear(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_clear(unit, port));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_clear_single(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_clear_single(unit, port, stat_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_get(
    int unit,
    bcm_port_t port,
    bool sync_mode,
    bcm_stat_val_t stat_id,
    uint64_t *val)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_get(unit, port, sync_mode, stat_id, val));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_counter_control_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_counter_control_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_counter_control_get(
    int unit,
    bcmi_ltsw_stat_counter_control_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_counter_control_get(unit, info));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_custom_add(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_custom_add(unit, port, stat_id, trigger));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_custom_check(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger,
    int *result)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_custom_check(unit, port, stat_id, trigger, result));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_custom_delete(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id,
    bcm_custom_stat_trigger_t trigger)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_custom_delete(unit, port, stat_id, trigger));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_stat_custom_delete_all(
    int unit,
    bcm_port_t port,
    bcm_stat_val_t stat_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_stat_custom_delete_all(unit, port, stat_id));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Stat driver function variable for bcm56990_a0 device.
 */
static mbcm_ltsw_stat_drv_t bcm56990_a0_ltsw_stat_drv = {
    .stat_init = bcm56990_a0_ltsw_stat_init,
    .stat_detach = bcm56990_a0_ltsw_stat_detach,
    .stat_index_init = bcm56990_a0_ltsw_stat_index_init,
    .stat_lt_detach = bcm56990_a0_ltsw_stat_lt_detach,
    .stat_port_attach = bcm56990_a0_ltsw_stat_port_attach,
    .stat_port_detach = bcm56990_a0_ltsw_stat_port_detach,
    .stat_clear = bcm56990_a0_ltsw_stat_clear,
    .stat_clear_single = bcm56990_a0_ltsw_stat_clear_single,
    .stat_get = bcm56990_a0_ltsw_stat_get,
    .stat_counter_control_init = bcm56990_a0_ltsw_stat_counter_control_init,
    .stat_counter_control_get = bcm56990_a0_ltsw_stat_counter_control_get,
    .stat_custom_add = bcm56990_a0_ltsw_stat_custom_add,
    .stat_custom_check = bcm56990_a0_ltsw_stat_custom_check,
    .stat_custom_delete = bcm56990_a0_ltsw_stat_custom_delete,
    .stat_custom_delete_all = bcm56990_a0_ltsw_stat_custom_delete_all
};

/******************************************************************************
 * Public functions
 */

int
bcm56990_a0_ltsw_stat_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_stat_drv_set(unit, &bcm56990_a0_ltsw_stat_drv));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56990_a0_sub_drv_attach(unit, &bcm56990_a0_ltsw_stat_drv,
                                    BCM56990_A0_SUB_DRV_BCM56990_Ax,
                                    BCM56990_A0_SUB_MOD_STAT));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Attach functions for other members.
 */
#define BCM56990_A0_DRV_ATTACH_ENTRY(_dn,_vn,_pf,_pd,_r0) \
int _vn##_ltsw_stat_drv_attach(int unit) \
{ \
    SHR_FUNC_ENTER(unit); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (mbcm_ltsw_stat_drv_set(unit, &bcm56990_a0_ltsw_stat_drv)); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (bcm56990_a0_sub_drv_attach(unit, &bcm56990_a0_ltsw_stat_drv, \
                                    BCM56990_A0_SUB_DRV_BCM5699x, \
                                    BCM56990_A0_SUB_MOD_STAT)); \
exit: \
    SHR_FUNC_EXIT(); \
}
#include "sub_devlist.h"
