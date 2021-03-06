/*! \file bcm56990_b0_tm_imm_init.c
 *
 * BCMTM routines for initializing the IMM interface.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_debug.h>
#include "bcm56990_b0_tm_imm_internal.h"

#include <bcmtm/chip/bcm56990_b0_tm.h>

#define BSL_LOG_MODULE  BSL_LS_BCMTM_IMM_CHIP

/*******************************************************************************
 * Public functions
 */
int
bcm56990_b0_tm_imm_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    /* Register all IMM callback handlers here. */
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_pri_grp_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_headroom_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_uc_q_to_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_mc_q_to_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_pri_grp_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_uc_q_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_mc_q_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_uc_port_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_mc_port_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_mc_egr_service_pool_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_q_grp_register(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_dynamic_margin_register(unit));

exit:
    SHR_FUNC_EXIT();
}

int
bcm56990_b0_tm_imm_populate(int unit)
{
    SHR_FUNC_ENTER(unit);

    /* Pre-populate LTs (Qualified LTs only). Check guidelines. */
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_pri_grp_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_headroom_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_uc_q_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_mc_q_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_uc_port_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_mc_port_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_mc_egr_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_q_grp_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_uc_q_to_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_mc_q_to_service_pool_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_pri_grp_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_pfc_deadlock_recovery_status_populate(unit));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_dynamic_margin_populate(unit));
exit:
    SHR_FUNC_EXIT();
}

int
bcm56990_b0_tm_imm_update(int unit, int pport, int up)
{
    SHR_FUNC_ENTER(unit);

    /* All port related LTs should be updated during flexport operation. */
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_pri_grp_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_pri_grp_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_mc_port_service_pool_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_egr_uc_port_service_pool_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_ing_port_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_ing_port_service_pool_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_mc_q_to_service_pool_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_port_uc_q_to_service_pool_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_mc_q_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_uc_q_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_thd_q_grp_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_pfc_deadlock_recovery_status_update(unit, pport, up));
    SHR_IF_ERR_EXIT
        (bcm56990_b0_tm_pfc_deadlock_recovery_update(unit, pport, up));
exit:
    SHR_FUNC_EXIT();
}
