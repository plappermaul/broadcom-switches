/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/policer.h>
#include <bcm_int/ltsw/xfs/policer.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcmltd/chip/bcmltd_str.h>
#include <bcm_int/ltsw/generated/policer_ha.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_POLICER

/******************************************************************************
 * Private functions
 */

static int
bcm56880_a0_ltsw_policer_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_deinit(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_create(
    int unit,
    bcm_policer_config_t *pol_cfg,
    bcm_policer_t *policer_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_create(unit, pol_cfg, policer_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_destroy(
    int unit,
    bcm_policer_t policer_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_destroy(unit, policer_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_destroy_all(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_destroy_all(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_set(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_set(unit, policer_id, pol_cfg));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_get(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_get(unit, policer_id, pol_cfg));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_traverse(
    int unit,
    bcm_policer_traverse_cb cb,
    void *user_data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_traverse(unit, cb, user_data));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_capacity_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *num_meters)
{
    SHR_FUNC_ENTER(unit);

    if (stage == bcmiPolicerStageIngress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_capacity_get(unit, METER_ING_FP_TEMPLATEs, num_meters));
    } else if (stage == bcmiPolicerStageEgress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_capacity_get(unit, METER_EGR_FP_TEMPLATEs, num_meters));
    } else {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_max_pool_idx_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *max_idx)
{
    int rv = 0;
    uint32_t ltid = 0, fid = 0;
    uint64_t min = 0, max = 0;

    SHR_FUNC_ENTER(unit);

    if (stage == bcmiPolicerStageIngress) {
        rv = bcmi_lt_field_value_range_get(unit,
                                           METER_ING_FP_TEMPLATEs,
                                           POOL_INSTANCEs,
                                           &min,
                                           &max);
        if (rv == SHR_E_NONE) {
            rv = bcmlt_table_field_id_get_by_name(
                unit,
                "DT_IFP_ACTION_TEMPLATE",
                "ING_CMD_BUS_SFC_IFP_METER_PROFILE_INDEX",
                &ltid,
                &fid);
            if (rv != SHR_E_NONE) {
                rv = SHR_E_NONE;
                max = 1;
            }
        } else {
            rv = SHR_E_NONE;
            max = 0;
        }
    } else if (stage == bcmiPolicerStageEgress) {
        rv = bcmi_lt_field_value_range_get(unit,
                                           METER_EGR_FP_TEMPLATEs,
                                           POOL_INSTANCEs,
                                           &min,
                                           &max);
        if (rv == SHR_E_UNAVAIL) {
            rv = SHR_E_NONE;
            max = 0;
        }
    } else {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    if (rv != SHR_E_NOT_FOUND) {
        SHR_IF_ERR_VERBOSE_EXIT(rv);
    }

    *max_idx = max;

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_pool_init(
    int unit,
    bcmint_policer_pool_state_t *pool_state,
    int num_pipes,
    int max_pools)
{
    bcmint_policer_pool_state_t *ptr = NULL;
    int pool_idx = 0, pipe_idx = 0;

    SHR_FUNC_ENTER(unit);

    ptr = pool_state;

    for (pipe_idx = 0; pipe_idx < num_pipes; pipe_idx++) {
        for (pool_idx = 0; pool_idx < max_pools; pool_idx++) {
            if ((pool_idx == 2) ||
                 (pool_idx == 3)) {
                 ptr->mode = BCMI_LTSW_POLICER_POOL_MODE_WITH_ID;
            }
            ptr++;
        }
    }

    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_install(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg,
    int pipe,
    int pool)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_policer_install(unit, policer_id, pol_cfg, pipe, pool));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_supported(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    bool *support)
{
    SHR_FUNC_ENTER(unit);

    if ((stage == bcmiPolicerStageIngress) ||
        (stage == bcmiPolicerStageEgress)) {
        *support = TRUE;
    } else {
        *support = FALSE;
    }

    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_policer_stage_flags_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *flags)
{
    SHR_FUNC_ENTER(unit);

    *flags = 0;

    SHR_FUNC_EXIT();
}

/*!
 * \brief Policer driver function variable for bcm56880_a0 device.
 */
static mbcm_ltsw_policer_drv_t bcm56880_a0_ltsw_policer_drv = {
    .policer_deinit = bcm56880_a0_ltsw_policer_deinit,
    .policer_init = bcm56880_a0_ltsw_policer_init,
    .policer_create = bcm56880_a0_ltsw_policer_create,
    .policer_destroy = bcm56880_a0_ltsw_policer_destroy,
    .policer_destroy_all = bcm56880_a0_ltsw_policer_destroy_all,
    .policer_set = bcm56880_a0_ltsw_policer_set,
    .policer_get = bcm56880_a0_ltsw_policer_get,
    .policer_traverse = bcm56880_a0_ltsw_policer_traverse,
    .policer_capacity_get = bcm56880_a0_ltsw_policer_capacity_get,
    .policer_max_pool_idx_get = bcm56880_a0_ltsw_policer_max_pool_idx_get,
    .policer_install = bcm56880_a0_ltsw_policer_install,
    .policer_supported = bcm56880_a0_ltsw_policer_supported,
    .policer_stage_flags_get = bcm56880_a0_ltsw_policer_stage_flags_get,
    .policer_pool_init = bcm56880_a0_ltsw_policer_pool_init,
};

/******************************************************************************
 * Public functions
 */

int
bcm56880_a0_ltsw_policer_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_policer_drv_set(unit, &bcm56880_a0_ltsw_policer_drv));

exit:
    SHR_FUNC_EXIT();
}

