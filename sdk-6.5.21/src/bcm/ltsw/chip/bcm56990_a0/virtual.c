/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/virtual.h>
#include "sub_dispatch.h"

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COMMON

/******************************************************************************
 * Private functions
 */

/*!
 * \brief Virtual driver function variable for bcm56990_a0 device.
 */
static mbcm_ltsw_virtual_drv_t bcm56990_a0_ltsw_virtual_drv = {
    NULL
};

/******************************************************************************
 * Public functions
 */

int
bcm56990_a0_ltsw_virtual_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT
        (mbcm_ltsw_virtual_drv_set(unit, &bcm56990_a0_ltsw_virtual_drv));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Attach functions for other members.
 */
#define BCM56990_A0_DRV_ATTACH_ENTRY(_dn,_vn,_pf,_pd,_r0) \
int _vn##_ltsw_virtual_drv_attach(int unit) \
{ \
    SHR_FUNC_ENTER(unit); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (bcm56990_a0_sub_drv_attach(unit, &bcm56990_a0_ltsw_virtual_drv, \
                                    BCM56990_A0_SUB_DRV_BCM5699x, \
                                    BCM56990_A0_SUB_MOD_VIRTUAL)); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (mbcm_ltsw_virtual_drv_set(unit, &bcm56990_a0_ltsw_virtual_drv)); \
exit: \
    SHR_FUNC_EXIT(); \
}
#include "sub_devlist.h"
