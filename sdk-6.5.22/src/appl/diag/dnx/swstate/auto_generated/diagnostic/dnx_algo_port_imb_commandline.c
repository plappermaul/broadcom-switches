/** \file dnx/swstate/auto_generated/diagnostic/dnx_algo_port_imb_commandline.c
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
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <bcm_int/dnx/algo/swstate/auto_generated/diagnostic/dnx_algo_port_imb_diagnostic.h>
#include "dnx_algo_port_imb_commandline.h"
#include <bcm_int/dnx/algo/swstate/auto_generated/access/dnx_algo_port_imb_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_nif.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_dnx_algo_port_imb_cmds[] = {
    {"rmc_id_res", NULL, sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_cmds, NULL, &dnx_swstate_dnx_algo_port_imb_rmc_id_res_man},
    {"dump", sh_dnx_swstate_dnx_algo_port_imb_dump_cmd, NULL, dnx_swstate_dnx_algo_port_imb_dump_options, &dnx_swstate_dnx_algo_port_imb_dump_man},
    {"size_get", sh_dnx_swstate_dnx_algo_port_imb_size_get_cmd, NULL, dnx_swstate_dnx_algo_port_imb_size_get_options, &dnx_swstate_dnx_algo_port_imb_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_cmd, NULL, dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_options, &dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_algo_port_imb_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_algo_port_imb_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_dump_man = {
    "swstate dnx_algo_port_imb dump",
    "print the module's content",
    "swstate dnx_algo_port_imb dump",
    "swstate dnx_algo_port_imb dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_algo_port_imb_db_info[unit], dnx_algo_port_imb_db_layout_str, DNX_ALGO_PORT_IMB_DB_INFO_NOF_ENTRIES, "DNX_ALGO_PORT_IMB_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_size_get_man = {
    "swstate dnx_algo_port_imb size_get",
    "print the module's size",
    "swstate dnx_algo_port_imb size_get",
    "swstate dnx_algo_port_imb size_get",
};


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{


    SHR_FUNC_INIT_VARS(unit);

    dnx_sw_state_diagnostic_operation_counters_print(dnx_algo_port_imb_db_info[unit], dnx_algo_port_imb_db_layout_str, DNX_ALGO_PORT_IMB_DB_INFO_NOF_ENTRIES, "DNX_ALGO_PORT_IMB_DB~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_diagnostic_operation_counters_man = {
    "swstate dnx_algo_port_imb diagnostic_operation_counters",
    "print the module's diagnostic operation counters",
    "swstate dnx_algo_port_imb diagnostic_operation_counters",
    "swstate dnx_algo_port_imb diagnostic_operation_counters",
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_man = {
    cmd_dnx_swstate_dnx_algo_port_imb_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_algo_port_imb_desc[] = "swstate dnx_algo_port_imb commands";



sh_sand_cmd_t sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_cmd, NULL, dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_options, &dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_man},
    {"size_get", sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_cmd, NULL, dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_options, &dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_cmd, NULL, dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_options, &dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_man = {
    cmd_dnx_swstate_dnx_algo_port_imb_rmc_id_res_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_algo_port_imb_rmc_id_res_desc[] = "dnx_algo_port_imb_db rmc_id_res commands";


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  rmc_id_res_idx_0 = 0;
    int  rmc_id_res_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("rmc_id_res_idx_0", rmc_id_res_idx_0);
    SH_SAND_GET_INT32("rmc_id_res_idx_1", rmc_id_res_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_algo_port_imb_db_rmc_id_res_dump(unit, rmc_id_res_idx_0, rmc_id_res_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_options[] = {
    {"rmc_id_res_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"rmc_id_res_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_algo_port_imb_db rmc_id_res dump [,rmc_id_res_idx_0=<int>,rmc_id_res_idx_1=<int>]\n",
    "dnx_algo_port_imb_db rmc_id_res dump rmc_id_res_idx_0=0 rmc_id_res_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_algo_port_imb_db_info[unit], dnx_algo_port_imb_db_layout_str, DNX_ALGO_PORT_IMB_DB_INFO_NOF_ENTRIES, "DNX_ALGO_PORT_IMB_DB~RMC_ID_RES~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_algo_port_imb_db rmc_id_res size_get",
    "dnx_algo_port_imb_db rmc_id_res size_get",
};


shr_error_e sh_dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_algo_port_imb_db_info[unit], dnx_algo_port_imb_db_layout_str, DNX_ALGO_PORT_IMB_DB_INFO_NOF_ENTRIES, "DNX_ALGO_PORT_IMB_DB~RMC_ID_RES~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_algo_port_imb_rmc_id_res_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_algo_port_imb_db rmc_id_res diagnostic_operation_counters",
    "dnx_algo_port_imb_db rmc_id_res diagnostic_operation_counters",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE