
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <soc/dnx/swstate/auto_generated/types/dnx_algo_synce_types.h>
#include <soc/dnx/swstate/auto_generated/layout/dnx_algo_synce_layout.h>

dnxc_sw_state_layout_t layout_array_dnx_algo_synce[DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB_NOF_PARAMS)];

shr_error_e
dnx_algo_synce_init_layout_structure(int unit)
{

    int idx;

    DNXC_SW_STATE_INIT_FUNC_DEFS;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db";
    layout_array_dnx_algo_synce[idx].type = "dnx_algo_synce_db_t";
    layout_array_dnx_algo_synce[idx].doc = "SyncE Data Base";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(dnx_algo_synce_db_t);
    layout_array_dnx_algo_synce[idx].parent  = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FIRST);
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__LAST);

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__fabric_synce_status";
    layout_array_dnx_algo_synce[idx].type = "soc_dnxc_synce_status_t";
    layout_array_dnx_algo_synce[idx].doc = "Fabric SyncE status Data Base";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(soc_dnxc_synce_status_t);
    layout_array_dnx_algo_synce[idx].parent  = 0;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__FIRST);
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__LAST);

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__nif_synce_status";
    layout_array_dnx_algo_synce[idx].type = "soc_dnxc_synce_status_t";
    layout_array_dnx_algo_synce[idx].doc = "NIF SyncE status Data Base";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(soc_dnxc_synce_status_t);
    layout_array_dnx_algo_synce[idx].parent  = 0;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__FIRST);
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__LAST);

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__MASTER_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__fabric_synce_status__master_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "master synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 1;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__SLAVE_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__fabric_synce_status__slave_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "slave synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 1;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__MASTER_SET1_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__fabric_synce_status__master_set1_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "master to set1 synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 1;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__FABRIC_SYNCE_STATUS__SLAVE_SET1_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__fabric_synce_status__slave_set1_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "slave to set1 synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 1;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__MASTER_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__nif_synce_status__master_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "master synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 2;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__SLAVE_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__nif_synce_status__slave_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "slave synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 2;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__MASTER_SET1_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__nif_synce_status__master_set1_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "master to set1 synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 2;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_ALGO_SYNCE_DB__NIF_SYNCE_STATUS__SLAVE_SET1_SYNCE_ENABLED);
    layout_array_dnx_algo_synce[idx].name = "dnx_algo_synce_db__nif_synce_status__slave_set1_synce_enabled";
    layout_array_dnx_algo_synce[idx].type = "int";
    layout_array_dnx_algo_synce[idx].doc = "slave to set1 synce is enabled";
    layout_array_dnx_algo_synce[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].size_of = sizeof(int);
    layout_array_dnx_algo_synce[idx].parent  = 2;
    layout_array_dnx_algo_synce[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_algo_synce[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;


    dnxc_sw_state_layout_init_structure(unit, DNX_SW_STATE_DNX_ALGO_SYNCE_DB, layout_array_dnx_algo_synce, sw_state_layout_array[unit][DNX_ALGO_SYNCE_MODULE_ID], DNX_SW_STATE_DNX_ALGO_SYNCE_DB_NOF_PARAMS);

    DNXC_SW_STATE_FUNC_RETURN;
}

#undef BSL_LOG_MODULE
