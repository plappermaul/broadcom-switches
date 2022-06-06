/*! \file bcmpc_cmicx_pmac.c
 *
 * Shared PMAC access driver for CMICx-based devices.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcmbd/bcmbd_cmicx_pmac.h>

#include <bcmpc/bcmpc_cmicx_pmac.h>

static int
cmicx_pmac_read(void *user_acc, bcmpmac_pport_t port, uint32_t addr,
                uint32_t idx, size_t size, uint32_t *data)
{
    bcmpc_pmac_access_data_t *pmac_data = (bcmpc_pmac_access_data_t *)user_acc;

    if (!pmac_data) {
        return SHR_E_INTERNAL;
    }

    return bcmbd_cmicx_pmac_read(pmac_data->unit, pmac_data->blk,
                                 port, addr, idx, size, data);
}

static int
cmicx_pmac_write(void *user_acc, bcmpmac_pport_t port, uint32_t addr,
                 uint32_t idx, size_t size, uint32_t *data)
{
    bcmpc_pmac_access_data_t *pmac_data = (bcmpc_pmac_access_data_t *)user_acc;

    if (!pmac_data) {
        return SHR_E_INTERNAL;
    }

    return bcmbd_cmicx_pmac_write(pmac_data->unit, pmac_data->blk,
                                  port, addr, idx, size, data);
}

static bcmpmac_reg_access_t pmac_acc =
{
    .name = "cmicx_pmac",
    .read = cmicx_pmac_read,
    .write = cmicx_pmac_write
};

bcmpmac_reg_access_t *
bcmpc_cmicx_pmac_bus_get(int unit, bcmpc_pport_t pport)
{
    return &pmac_acc;
}
