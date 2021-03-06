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
/* Logical Table Adaptor for component bcmcth */
/* Handler: bcm56880_a0_lta_bcmcth_mon_flowtracker_flow_static_cth_handler */
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmcth/bcmcth_mon_flowtracker_static.h>



const bcmltd_generic_arg_t
bcm56880_a0_lta_bcmcth_mon_flowtracker_flow_static_cth_handler_arg = {
    .sid = MON_FLOWTRACKER_FLOW_STATICt,
    .values = 0,
    .value = NULL,
    .user_data = NULL
};

const bcmltd_table_handler_t
bcm56880_a0_lta_bcmcth_mon_flowtracker_flow_static_cth_handler = {
    .validate = bcmcth_mon_flowtracker_static_validate,
    .op_insert       = bcmcth_mon_flowtracker_static_insert,
    .op_lookup       = bcmcth_mon_flowtracker_static_lookup,
    .op_delete       = bcmcth_mon_flowtracker_static_delete,
    .op_update       = bcmcth_mon_flowtracker_static_update,
    .op_first        = bcmcth_mon_flowtracker_static_first,
    .op_next         = bcmcth_mon_flowtracker_static_next,
    .ha       = NULL,
    .arg      = &bcm56880_a0_lta_bcmcth_mon_flowtracker_flow_static_cth_handler_arg
};

