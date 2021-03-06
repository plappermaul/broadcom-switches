/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from Logical Table mapping files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/* Logical Table Adaptor for component bcmpc */
/* Handler: bcm56996_a0_lta_bcmpc_pc_pm_prop_cth_handler */
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmpc/bcmpc_lth_pm_prop.h>



const bcmltd_generic_arg_t
bcm56996_a0_lta_bcmpc_pc_pm_prop_cth_handler_arg = {
    .sid = PC_PM_PROPt,
    .values = 0,
    .value = NULL,
    .user_data = NULL
};

const bcmltd_table_handler_t
bcm56996_a0_lta_bcmpc_pc_pm_prop_cth_handler = {
    .validate = bcmpc_lth_pm_prop_validate,
    .op_insert       = bcmpc_lth_pm_prop_insert,
    .op_lookup       = bcmpc_lth_pm_prop_lookup,
    .op_delete       = bcmpc_lth_pm_prop_delete,
    .op_update       = bcmpc_lth_pm_prop_update,
    .op_first        = bcmpc_lth_pm_prop_first,
    .op_next         = bcmpc_lth_pm_prop_next,
    .ha       = NULL,
    .arg      = &bcm56996_a0_lta_bcmpc_pc_pm_prop_cth_handler_arg
};

