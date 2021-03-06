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

#include <bcm_int/ltsw/mbcm/dev.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COMMON

/*!
 * \brief The Dev mbcm drivers.
 */
static mbcm_ltsw_dev_drv_t mbcm_ltsw_dev_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_dev_drv_set(
    int unit,
    mbcm_ltsw_dev_drv_t *drv)
{
    mbcm_ltsw_dev_drv_t *local = &mbcm_ltsw_dev_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_dev_info_init(
    int unit,
    bcmint_dev_info_t *dev_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_dev_drv[unit].dev_info_init(unit, dev_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_dev_logic_port_pp_pipe(
    int unit,
    bcm_port_t port,
    int *pipe)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_dev_drv[unit].dev_logic_port_pp_pipe(unit, port, pipe));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_dev_logic_port_pp_pipe_port(
    int unit,
    bcm_port_t port,
    int *pp_port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_dev_drv[unit].dev_logic_port_pp_pipe_port(unit, port, pp_port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_dev_pp_pipe_port_logic_port(
    int unit,
    int pipe,
    int pp_port,
    bcm_port_t *port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_dev_drv[unit].dev_pp_pipe_port_logic_port(unit, pipe, pp_port, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_dev_pp_pipe_logic_pbmp(
    int unit,
    int pipe,
    bcm_pbmp_t *pbmp)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_dev_drv[unit].dev_pp_pipe_logic_pbmp(unit, pipe, pbmp));

exit:
    SHR_FUNC_EXIT();
}

