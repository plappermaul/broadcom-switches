/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/ipmc.h>
#include <bcm_int/ltsw/xgs/ipmc.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_IPMC

/******************************************************************************
 * Private functions
 */

static int
bcm56990_a0_ltsw_ipmc_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_pub_info_get(
    int unit,
    bcmint_ipmc_pub_info_t *mc_pub)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_pub_info_get(unit, mc_pub));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_l2_add(
    int unit,
    bcm_ipmc_addr_t *data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_l2_add(unit, data));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_l2_remove(
    int unit,
    bcm_ipmc_addr_t *data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_l2_remove(unit, data));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_l2_find(
    int unit,
    bcm_ipmc_addr_t *data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_l2_find(unit, data));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_l2_traverse(
    int unit,
    uint32_t flags,
    bcm_ipmc_traverse_cb cb,
    void *user_data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_l2_traverse(unit, flags, cb, user_data));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_l2_delete_all(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_l2_delete_all(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_enable(
    int unit,
    int enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_enable(unit, enable));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_egress_port_set(
    int unit,
    bcm_port_t port,
    const bcm_mac_t mac,
    int untag,
    bcm_vlan_t vid,
    int ttl_thresh)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_egress_port_set(unit, port, mac, untag, vid, ttl_thresh));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_ltsw_ipmc_egress_port_get(
    int unit,
    bcm_port_t port,
    bcm_mac_t mac,
    int *untag,
    bcm_vlan_t *vid,
    int *ttl_thresh)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_ipmc_egress_port_get(unit, port, mac, untag, vid, ttl_thresh));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Ipmc driver function variable for bcm56990_a0 device.
 */
static mbcm_ltsw_ipmc_drv_t bcm56990_a0_ltsw_ipmc_drv = {
    .ipmc_init = bcm56990_a0_ltsw_ipmc_init,
    .ipmc_detach = bcm56990_a0_ltsw_ipmc_detach,
    .ipmc_pub_info_get = bcm56990_a0_ltsw_ipmc_pub_info_get,
    .ipmc_l2_add = bcm56990_a0_ltsw_ipmc_l2_add,
    .ipmc_l2_remove = bcm56990_a0_ltsw_ipmc_l2_remove,
    .ipmc_l2_find = bcm56990_a0_ltsw_ipmc_l2_find,
    .ipmc_l2_traverse = bcm56990_a0_ltsw_ipmc_l2_traverse,
    .ipmc_l2_delete_all = bcm56990_a0_ltsw_ipmc_l2_delete_all,
    .ipmc_enable = bcm56990_a0_ltsw_ipmc_enable,
    .ipmc_egress_port_set = bcm56990_a0_ltsw_ipmc_egress_port_set,
    .ipmc_egress_port_get = bcm56990_a0_ltsw_ipmc_egress_port_get,
};

/******************************************************************************
 * Public functions
 */

int
bcm56990_a0_ltsw_ipmc_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_ipmc_drv_set(unit, &bcm56990_a0_ltsw_ipmc_drv));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Attach functions for other members.
 */
#define BCM56990_A0_DRV_ATTACH_ENTRY(_dn,_vn,_pf,_pd,_r0) \
int _vn##_ltsw_ipmc_drv_attach(int unit) \
{ \
    SHR_FUNC_ENTER(unit); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (mbcm_ltsw_ipmc_drv_set(unit, &bcm56990_a0_ltsw_ipmc_drv)); \
exit: \
    SHR_FUNC_EXIT(); \
}
#include "sub_devlist.h"
