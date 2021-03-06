/** \file dnx/swstate/auto_generated/diagnostic/pp_commandline.c
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
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <soc/dnx/swstate/auto_generated/diagnostic/pp_diagnostic.h>
#include "pp_commandline.h"
#include <soc/dnx/swstate/auto_generated/access/pp_access.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_pp_cmds[] = {
    {"pp_stage_kbr_select", NULL, sh_dnx_swstate_pp_pp_stage_kbr_select_cmds, NULL, &dnx_swstate_pp_pp_stage_kbr_select_man},
    {"dump", sh_dnx_swstate_pp_dump_cmd, NULL, dnx_swstate_pp_dump_options, &dnx_swstate_pp_dump_man},
    {"size_get", sh_dnx_swstate_pp_size_get_cmd, NULL, dnx_swstate_pp_size_get_options, &dnx_swstate_pp_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_pp_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    pp_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(pp_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_pp_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_pp_dump_man = {
    "swstate pp dump",
    "print the module's content",
    "swstate pp dump",
    "swstate pp dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_pp_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(pp_db_info[unit], pp_db_layout_str, PP_DB_INFO_NOF_ENTRIES, "PP_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_pp_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_pp_size_get_man = {
    "swstate pp size_get",
    "print the module's size",
    "swstate pp size_get",
    "swstate pp size_get",
};


sh_sand_man_t dnx_swstate_pp_man = {
    cmd_dnx_swstate_pp_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_pp_desc[] = "swstate pp commands";



sh_sand_cmd_t sh_dnx_swstate_pp_pp_stage_kbr_select_cmds[] = {
    {"dump", sh_dnx_swstate_pp_pp_stage_kbr_select_dump_cmd, NULL, dnx_swstate_pp_pp_stage_kbr_select_dump_options, &dnx_swstate_pp_pp_stage_kbr_select_dump_man},
    {"size_get", sh_dnx_swstate_pp_pp_stage_kbr_select_size_get_cmd, NULL, dnx_swstate_pp_pp_stage_kbr_select_size_get_options, &dnx_swstate_pp_pp_stage_kbr_select_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_pp_pp_stage_kbr_select_man = {
    cmd_dnx_swstate_pp_pp_stage_kbr_select_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_pp_pp_stage_kbr_select_desc[] = "pp_db pp_stage_kbr_select commands";


shr_error_e sh_dnx_swstate_pp_pp_stage_kbr_select_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  pp_stage_kbr_select_idx_0 = 0;
    int  pp_stage_kbr_select_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("pp_stage_kbr_select_idx_0", pp_stage_kbr_select_idx_0);
    SH_SAND_GET_INT32("pp_stage_kbr_select_idx_1", pp_stage_kbr_select_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(pp_db_pp_stage_kbr_select_dump(unit, pp_stage_kbr_select_idx_0, pp_stage_kbr_select_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_pp_pp_stage_kbr_select_dump_options[] = {
    {"pp_stage_kbr_select_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"pp_stage_kbr_select_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_pp_pp_stage_kbr_select_dump_man = {
    "swstate dump",
    "print the variable value",
    "pp_db pp_stage_kbr_select dump [,pp_stage_kbr_select_idx_0=<int>,pp_stage_kbr_select_idx_1=<int>]\n",
    "pp_db pp_stage_kbr_select dump pp_stage_kbr_select_idx_0=0 pp_stage_kbr_select_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_pp_pp_stage_kbr_select_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(pp_db_info[unit], pp_db_layout_str, PP_DB_INFO_NOF_ENTRIES, "PP_DB~PP_STAGE_KBR_SELECT~", sizeof(((dnx_pp_sw_state_t*)sw_state_roots_array[unit][PP_MODULE_ID])->pp_stage_kbr_select));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_pp_pp_stage_kbr_select_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_pp_pp_stage_kbr_select_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "pp_db pp_stage_kbr_select size_get",
    "pp_db pp_stage_kbr_select size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
