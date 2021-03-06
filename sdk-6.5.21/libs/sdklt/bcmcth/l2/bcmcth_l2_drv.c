/*! \file bcmcth_l2_drv.c
 *
 * BCMCTH L2 driver APIs.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bsl/bsl.h>
#include <shr/shr_debug.h>
#include <sal/sal_types.h>

#include <bcmcth/bcmcth_l2.h>

/******************************************************************************
* Local definitions
 */
/* Debug log target definition */
#define BSL_LOG_MODULE BSL_LS_BCMCTH_L2

/* L2 driver object */
static bcmcth_l2_drv_t l2_drv[BCMDRD_CONFIG_MAX_UNITS];


/******************************************************************************
* Public functions
 */
bcmcth_l2_drv_t *
bcmcth_l2_drv_get(int unit)
{
    return &l2_drv[unit];
}

int
bcmcth_l2_drv_init(int unit, bool warm)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->init == NULL) {
        return SHR_E_NONE;
    }

    return drv->init(unit, warm);
}

int
bcmcth_l2_drv_cleanup(int unit)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->cleanup == NULL) {
        return SHR_E_NONE;
    }

    return drv->cleanup(unit);
}

int
bcmcth_l2_drv_hw_enable_set(int unit, uint32_t trans_id, bool enable)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->hw_enable == NULL) {
        return SHR_E_NONE;
    }

    return drv->hw_enable(unit, trans_id, enable);
}

int
bcmcth_l2_drv_intr_enable_set(int unit, bool enable, bool in_intr)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->l2_intr_enable_set == NULL) {
        return SHR_E_NONE;
    }

    return drv->l2_intr_enable_set(unit, enable, in_intr);
}

int
bcmcth_l2_drv_intr_handler_set(int unit, bcmcth_l2_drv_intr_handler_f handler)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->intr_handler_set == NULL) {
        return SHR_E_NONE;
    }

    return drv->intr_handler_set(unit, handler);
}

int
bcmcth_l2_drv_entry_delete(int unit, void *data)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->entry_delete == NULL) {
        return SHR_E_NONE;
    }

    return drv->entry_delete(unit, data);
}

int
bcmcth_l2_drv_entry_parse(int unit, const bcmltd_field_t *key,
                          const bcmltd_field_t *data, void *lt_info)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->entry_parse == NULL) {
        return SHR_E_NONE;
    }

    return drv->entry_parse(unit, key, data, lt_info);
}

int
bcmcth_l2_drv_entry_insert(int unit, uint32_t pipe, uint32_t index, void *data)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->entry_insert == NULL) {
        return SHR_E_NONE;
    }

    return drv->entry_insert(unit, pipe, index, data);
}

bool
bcmcth_l2_drv_entry_valid(int unit, void *data)
{
    bcmcth_l2_drv_t *drv;

    drv = &l2_drv[unit];
    if (drv->entry_valid == NULL) {
        return SHR_E_NONE;
    }

    return drv->entry_valid(unit, data);
}


