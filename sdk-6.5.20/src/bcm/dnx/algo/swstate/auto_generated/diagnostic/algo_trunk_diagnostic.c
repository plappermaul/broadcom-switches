/** \file algo/swstate/auto_generated/diagnostic/algo_trunk_diagnostic.c
 *
 * sw state functions definitions
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <soc/dnxc/swstate/dnxc_sw_state_c_includes.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/diagnostic/algo_trunk_diagnostic.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
/*
 * Global Variables
 */

extern algo_trunk_sw_state_t * algo_trunk_db_data[SOC_MAX_NUM_DEVICES];
/*
 * FUNCTIONs
 */

/*
 *
 * dump function for the variable algo_trunk_db
 * AUTO-GENERATED - DO NOT MODIFY
 */
int
algo_trunk_db_dump(int  unit,  dnx_sw_state_dump_filters_t  filters)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;
    SHR_IF_ERR_EXIT(algo_trunk_db_egress_trunk_dump(unit, filters));
    DNXC_SW_STATE_FUNC_RETURN;
}



/*
 *
 * dump function for the variable egress_trunk
 * AUTO-GENERATED - DO NOT MODIFY
 */
int
algo_trunk_db_egress_trunk_dump(int  unit,  dnx_sw_state_dump_filters_t  filters)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    if (dnx_sw_state_compare(filters.typefilter, "dnx_algo_template_t") != TRUE)
    {
         SHR_EXIT();
    }

    if (dnx_sw_state_compare(filters.namefilter, "algo_trunk egress_trunk") != TRUE)
    {
         SHR_EXIT();
    }

    if (filters.nocontent)
    {
         DNX_SW_STATE_PRINT(unit, "swstate algo_trunk egress_trunk\n");
    }
    else
    {
        dnx_sw_state_dump_attach_file(
            unit,
            "algo_trunk_db/egress_trunk.txt",
            "algo_trunk_db[%d]->","((algo_trunk_sw_state_t*)sw_state_roots_array[%d][ALGO_TRUNK_MODULE_ID])->","egress_trunk: ");

        DNX_SW_STATE_DUMP_PTR_NULL_CHECK(
            unit,
            ((algo_trunk_sw_state_t*)sw_state_roots_array[unit][ALGO_TRUNK_MODULE_ID]));

        DNX_SW_STATE_PRINT_MONITOR(
            unit,
            "algo_trunk_db[%d]->","((algo_trunk_sw_state_t*)sw_state_roots_array[%d][ALGO_TRUNK_MODULE_ID])->","egress_trunk: ");

        DNX_SW_STATE_PRINT_FILE(
            unit,
            " ");

        DNX_ALGO_TEMP_MNGR_PRINT(
            unit,
            ALGO_TRUNK_MODULE_ID,
            &((algo_trunk_sw_state_t*)sw_state_roots_array[unit][ALGO_TRUNK_MODULE_ID])->egress_trunk,
            dnx_algo_trunk_egresstrunk_profile_print_cb);

        dnx_sw_state_dump_detach_file(
            unit);

    }

    DNXC_SW_STATE_FUNC_RETURN;
}



/*
 * Global Variables
 */

dnx_sw_state_diagnostic_info_t algo_trunk_db_info[SOC_MAX_NUM_DEVICES][ALGO_TRUNK_DB_INFO_NOF_ENTRIES];
const char* algo_trunk_db_layout_str[ALGO_TRUNK_DB_INFO_NOF_ENTRIES] = {
    "ALGO_TRUNK_DB~",
    "ALGO_TRUNK_DB~EGRESS_TRUNK~",
};
#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
