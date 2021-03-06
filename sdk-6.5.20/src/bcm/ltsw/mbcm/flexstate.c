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

#include <bcm_int/ltsw/mbcm/flexstate.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_FLEXSTATE

/*!
 * \brief The Flexstate mbcm drivers.
 */
static mbcm_ltsw_flexstate_drv_t mbcm_ltsw_flexstate_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_flexstate_drv_set(
    int unit,
    mbcm_ltsw_flexstate_drv_t *drv)
{
    mbcm_ltsw_flexstate_drv_t *local = &mbcm_ltsw_flexstate_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_flexstate_source_info_get(
    int unit,
    int source,
    bcmint_flexstate_source_info_t **source_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_flexstate_drv[unit].flexstate_source_info_get(unit, source, source_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_flexstate_object_info_get(
    int unit,
    int object,
    bcmint_flexstate_object_info_t **object_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_flexstate_drv[unit].flexstate_object_info_get(unit, object, object_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_flexstate_stage_info_get(
    int unit,
    bcmint_flexstate_stage_info_t **stage_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_flexstate_drv[unit].flexstate_stage_info_get(unit, stage_info));

exit:
    SHR_FUNC_EXIT();
}

