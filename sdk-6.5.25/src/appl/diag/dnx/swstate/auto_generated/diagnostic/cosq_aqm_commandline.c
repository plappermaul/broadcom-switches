/** \file dnx/swstate/auto_generated/diagnostic/cosq_aqm_commandline.c
 *
 * sw state functions definitions
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
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

#include <bcm_int/dnx/algo/swstate/auto_generated/diagnostic/cosq_aqm_diagnostic.h>
#include "cosq_aqm_commandline.h"
#include <bcm_int/dnx/algo/swstate/auto_generated/access/cosq_aqm_access.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_cosq_aqm_cmds[] = {
    {"aqm_profile_res", NULL, sh_dnx_swstate_cosq_aqm_aqm_profile_res_cmds, NULL, &dnx_swstate_cosq_aqm_aqm_profile_res_man},
    {"aqm_flow_id_res", NULL, sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_cmds, NULL, &dnx_swstate_cosq_aqm_aqm_flow_id_res_man},
    {"dump", sh_dnx_swstate_cosq_aqm_dump_cmd, NULL, dnx_swstate_cosq_aqm_dump_options, &dnx_swstate_cosq_aqm_dump_man},
    {"size_get", sh_dnx_swstate_cosq_aqm_size_get_cmd, NULL, dnx_swstate_cosq_aqm_size_get_options, &dnx_swstate_cosq_aqm_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_cosq_aqm_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_cosq_aqm_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_cosq_aqm_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_dump_man = {
    "swstate cosq_aqm dump",
    "print the module's content",
    "swstate cosq_aqm dump",
    "swstate cosq_aqm dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_cosq_aqm_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_cosq_aqm_db_info[unit], dnx_cosq_aqm_db_layout_str, DNX_COSQ_AQM_DB_INFO_NOF_ENTRIES, "DNX_COSQ_AQM_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_size_get_man = {
    "swstate cosq_aqm size_get",
    "print the module's size",
    "swstate cosq_aqm size_get",
    "swstate cosq_aqm size_get",
};


sh_sand_man_t dnx_swstate_cosq_aqm_man = {
    cmd_dnx_swstate_cosq_aqm_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_cosq_aqm_desc[] = "swstate cosq_aqm commands";



sh_sand_cmd_t sh_dnx_swstate_cosq_aqm_aqm_profile_res_cmds[] = {
    {"dump", sh_dnx_swstate_cosq_aqm_aqm_profile_res_dump_cmd, NULL, dnx_swstate_cosq_aqm_aqm_profile_res_dump_options, &dnx_swstate_cosq_aqm_aqm_profile_res_dump_man},
    {"size_get", sh_dnx_swstate_cosq_aqm_aqm_profile_res_size_get_cmd, NULL, dnx_swstate_cosq_aqm_aqm_profile_res_size_get_options, &dnx_swstate_cosq_aqm_aqm_profile_res_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_profile_res_man = {
    cmd_dnx_swstate_cosq_aqm_aqm_profile_res_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_cosq_aqm_aqm_profile_res_desc[] = "dnx_cosq_aqm_db aqm_profile_res commands";


shr_error_e sh_dnx_swstate_cosq_aqm_aqm_profile_res_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_cosq_aqm_db_aqm_profile_res_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_aqm_profile_res_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_profile_res_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_cosq_aqm_db aqm_profile_res dump []\n",
    "dnx_cosq_aqm_db aqm_profile_res dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_cosq_aqm_aqm_profile_res_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_cosq_aqm_db_info[unit], dnx_cosq_aqm_db_layout_str, DNX_COSQ_AQM_DB_INFO_NOF_ENTRIES, "DNX_COSQ_AQM_DB~AQM_PROFILE_RES~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_aqm_profile_res_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_profile_res_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_cosq_aqm_db aqm_profile_res size_get",
    "dnx_cosq_aqm_db aqm_profile_res size_get",
};



sh_sand_cmd_t sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_cmds[] = {
    {"dump", sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_cmd, NULL, dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_options, &dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_man},
    {"size_get", sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_cmd, NULL, dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_options, &dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_flow_id_res_man = {
    cmd_dnx_swstate_cosq_aqm_aqm_flow_id_res_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_cosq_aqm_aqm_flow_id_res_desc[] = "dnx_cosq_aqm_db aqm_flow_id_res commands";


shr_error_e sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_cosq_aqm_db_aqm_flow_id_res_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_flow_id_res_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_cosq_aqm_db aqm_flow_id_res dump []\n",
    "dnx_cosq_aqm_db aqm_flow_id_res dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_cosq_aqm_db_info[unit], dnx_cosq_aqm_db_layout_str, DNX_COSQ_AQM_DB_INFO_NOF_ENTRIES, "DNX_COSQ_AQM_DB~AQM_FLOW_ID_RES~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_cosq_aqm_aqm_flow_id_res_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_cosq_aqm_db aqm_flow_id_res size_get",
    "dnx_cosq_aqm_db aqm_flow_id_res size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
