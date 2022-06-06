/** \file dnx/swstate/auto_generated/diagnostic/oam_commandline.c
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

#include <soc/dnx/swstate/auto_generated/diagnostic/oam_diagnostic.h>
#include "oam_commandline.h"
#include <soc/dnx/swstate/auto_generated/access/oam_access.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_oam_cmds[] = {
    {"oam_group_sw_db_info", NULL, sh_dnx_swstate_oam_oam_group_sw_db_info_cmds, NULL, &dnx_swstate_oam_oam_group_sw_db_info_man},
    {"oam_endpoint_sw_db_info", NULL, sh_dnx_swstate_oam_oam_endpoint_sw_db_info_cmds, NULL, &dnx_swstate_oam_oam_endpoint_sw_db_info_man},
    {"reflector", NULL, sh_dnx_swstate_oam_reflector_cmds, NULL, &dnx_swstate_oam_reflector_man},
    {"tst_trap_used_cnt", NULL, sh_dnx_swstate_oam_tst_trap_used_cnt_cmds, NULL, &dnx_swstate_oam_tst_trap_used_cnt_man},
    {"dump", sh_dnx_swstate_oam_dump_cmd, NULL, dnx_swstate_oam_dump_options, &dnx_swstate_oam_dump_man},
    {"size_get", sh_dnx_swstate_oam_size_get_cmd, NULL, dnx_swstate_oam_size_get_options, &dnx_swstate_oam_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_oam_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    oam_sw_db_info.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(oam_sw_db_info_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_dump_man = {
    "swstate oam dump",
    "print the module's content",
    "swstate oam dump",
    "swstate oam dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_size_get_man = {
    "swstate oam size_get",
    "print the module's size",
    "swstate oam size_get",
    "swstate oam size_get",
};


sh_sand_man_t dnx_swstate_oam_man = {
    cmd_dnx_swstate_oam_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_desc[] = "swstate oam commands";



sh_sand_cmd_t sh_dnx_swstate_oam_oam_group_sw_db_info_cmds[] = {
    {"oam_group_array_of_linked_lists", NULL, sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_cmds, NULL, &dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_man},
    {"dump", sh_dnx_swstate_oam_oam_group_sw_db_info_dump_cmd, NULL, dnx_swstate_oam_oam_group_sw_db_info_dump_options, &dnx_swstate_oam_oam_group_sw_db_info_dump_man},
    {"size_get", sh_dnx_swstate_oam_oam_group_sw_db_info_size_get_cmd, NULL, dnx_swstate_oam_oam_group_sw_db_info_size_get_options, &dnx_swstate_oam_oam_group_sw_db_info_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_man = {
    cmd_dnx_swstate_oam_oam_group_sw_db_info_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_oam_group_sw_db_info_desc[] = "oam_sw_db_info oam_group_sw_db_info commands";


shr_error_e sh_dnx_swstate_oam_oam_group_sw_db_info_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_oam_group_sw_db_info_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_group_sw_db_info_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info oam_group_sw_db_info dump []\n",
    "oam_sw_db_info oam_group_sw_db_info dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_oam_group_sw_db_info_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~OAM_GROUP_SW_DB_INFO~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->oam_group_sw_db_info));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_group_sw_db_info_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info oam_group_sw_db_info size_get",
    "oam_sw_db_info oam_group_sw_db_info size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_cmds[] = {
    {"dump", sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_cmd, NULL, dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_options, &dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_man},
    {"size_get", sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_cmd, NULL, dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_options, &dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_man = {
    cmd_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_desc[] = "oam_sw_db_info oam_group_sw_db_info oam_group_array_of_linked_lists commands";


shr_error_e sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info oam_group_sw_db_info oam_group_array_of_linked_lists dump []\n",
    "oam_sw_db_info oam_group_sw_db_info oam_group_array_of_linked_lists dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~OAM_GROUP_SW_DB_INFO~OAM_GROUP_ARRAY_OF_LINKED_LISTS~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->oam_group_sw_db_info.oam_group_array_of_linked_lists));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_group_sw_db_info_oam_group_array_of_linked_lists_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info oam_group_sw_db_info oam_group_array_of_linked_lists size_get",
    "oam_sw_db_info oam_group_sw_db_info oam_group_array_of_linked_lists size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_oam_endpoint_sw_db_info_cmds[] = {
    {"oam_endpoint_array_of_rmep_linked_lists", NULL, sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_cmds, NULL, &dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_man},
    {"dump", sh_dnx_swstate_oam_oam_endpoint_sw_db_info_dump_cmd, NULL, dnx_swstate_oam_oam_endpoint_sw_db_info_dump_options, &dnx_swstate_oam_oam_endpoint_sw_db_info_dump_man},
    {"size_get", sh_dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_cmd, NULL, dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_options, &dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_man = {
    cmd_dnx_swstate_oam_oam_endpoint_sw_db_info_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_oam_endpoint_sw_db_info_desc[] = "oam_sw_db_info oam_endpoint_sw_db_info commands";


shr_error_e sh_dnx_swstate_oam_oam_endpoint_sw_db_info_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_oam_endpoint_sw_db_info_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_endpoint_sw_db_info_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info oam_endpoint_sw_db_info dump []\n",
    "oam_sw_db_info oam_endpoint_sw_db_info dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~OAM_ENDPOINT_SW_DB_INFO~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->oam_endpoint_sw_db_info));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info oam_endpoint_sw_db_info size_get",
    "oam_sw_db_info oam_endpoint_sw_db_info size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_cmds[] = {
    {"dump", sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_cmd, NULL, dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_options, &dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_man},
    {"size_get", sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_cmd, NULL, dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_options, &dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_man = {
    cmd_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_desc[] = "oam_sw_db_info oam_endpoint_sw_db_info oam_endpoint_array_of_rmep_linked_lists commands";


shr_error_e sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info oam_endpoint_sw_db_info oam_endpoint_array_of_rmep_linked_lists dump []\n",
    "oam_sw_db_info oam_endpoint_sw_db_info oam_endpoint_array_of_rmep_linked_lists dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~OAM_ENDPOINT_SW_DB_INFO~OAM_ENDPOINT_ARRAY_OF_RMEP_LINKED_LISTS~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->oam_endpoint_sw_db_info.oam_endpoint_array_of_rmep_linked_lists));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_oam_endpoint_sw_db_info_oam_endpoint_array_of_rmep_linked_lists_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info oam_endpoint_sw_db_info oam_endpoint_array_of_rmep_linked_lists size_get",
    "oam_sw_db_info oam_endpoint_sw_db_info oam_endpoint_array_of_rmep_linked_lists size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_reflector_cmds[] = {
    {"encap_id", NULL, sh_dnx_swstate_oam_reflector_encap_id_cmds, NULL, &dnx_swstate_oam_reflector_encap_id_man},
    {"is_allocated", NULL, sh_dnx_swstate_oam_reflector_is_allocated_cmds, NULL, &dnx_swstate_oam_reflector_is_allocated_man},
    {"dump", sh_dnx_swstate_oam_reflector_dump_cmd, NULL, dnx_swstate_oam_reflector_dump_options, &dnx_swstate_oam_reflector_dump_man},
    {"size_get", sh_dnx_swstate_oam_reflector_size_get_cmd, NULL, dnx_swstate_oam_reflector_size_get_options, &dnx_swstate_oam_reflector_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_man = {
    cmd_dnx_swstate_oam_reflector_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_reflector_desc[] = "oam_sw_db_info reflector commands";


shr_error_e sh_dnx_swstate_oam_reflector_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_reflector_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info reflector dump []\n",
    "oam_sw_db_info reflector dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_reflector_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~REFLECTOR~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->reflector));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info reflector size_get",
    "oam_sw_db_info reflector size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_reflector_encap_id_cmds[] = {
    {"dump", sh_dnx_swstate_oam_reflector_encap_id_dump_cmd, NULL, dnx_swstate_oam_reflector_encap_id_dump_options, &dnx_swstate_oam_reflector_encap_id_dump_man},
    {"size_get", sh_dnx_swstate_oam_reflector_encap_id_size_get_cmd, NULL, dnx_swstate_oam_reflector_encap_id_size_get_options, &dnx_swstate_oam_reflector_encap_id_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_encap_id_man = {
    cmd_dnx_swstate_oam_reflector_encap_id_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_reflector_encap_id_desc[] = "oam_sw_db_info reflector encap_id commands";


shr_error_e sh_dnx_swstate_oam_reflector_encap_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_reflector_encap_id_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_encap_id_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_encap_id_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info reflector encap_id dump []\n",
    "oam_sw_db_info reflector encap_id dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_reflector_encap_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~REFLECTOR~ENCAP_ID~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->reflector.encap_id));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_encap_id_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_encap_id_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info reflector encap_id size_get",
    "oam_sw_db_info reflector encap_id size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_reflector_is_allocated_cmds[] = {
    {"dump", sh_dnx_swstate_oam_reflector_is_allocated_dump_cmd, NULL, dnx_swstate_oam_reflector_is_allocated_dump_options, &dnx_swstate_oam_reflector_is_allocated_dump_man},
    {"size_get", sh_dnx_swstate_oam_reflector_is_allocated_size_get_cmd, NULL, dnx_swstate_oam_reflector_is_allocated_size_get_options, &dnx_swstate_oam_reflector_is_allocated_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_is_allocated_man = {
    cmd_dnx_swstate_oam_reflector_is_allocated_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_reflector_is_allocated_desc[] = "oam_sw_db_info reflector is_allocated commands";


shr_error_e sh_dnx_swstate_oam_reflector_is_allocated_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_reflector_is_allocated_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_is_allocated_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_is_allocated_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info reflector is_allocated dump []\n",
    "oam_sw_db_info reflector is_allocated dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_reflector_is_allocated_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~REFLECTOR~IS_ALLOCATED~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->reflector.is_allocated));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_reflector_is_allocated_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_reflector_is_allocated_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info reflector is_allocated size_get",
    "oam_sw_db_info reflector is_allocated size_get",
};



sh_sand_cmd_t sh_dnx_swstate_oam_tst_trap_used_cnt_cmds[] = {
    {"dump", sh_dnx_swstate_oam_tst_trap_used_cnt_dump_cmd, NULL, dnx_swstate_oam_tst_trap_used_cnt_dump_options, &dnx_swstate_oam_tst_trap_used_cnt_dump_man},
    {"size_get", sh_dnx_swstate_oam_tst_trap_used_cnt_size_get_cmd, NULL, dnx_swstate_oam_tst_trap_used_cnt_size_get_options, &dnx_swstate_oam_tst_trap_used_cnt_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_tst_trap_used_cnt_man = {
    cmd_dnx_swstate_oam_tst_trap_used_cnt_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_oam_tst_trap_used_cnt_desc[] = "oam_sw_db_info tst_trap_used_cnt commands";


shr_error_e sh_dnx_swstate_oam_tst_trap_used_cnt_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(oam_sw_db_info_tst_trap_used_cnt_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_tst_trap_used_cnt_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_tst_trap_used_cnt_dump_man = {
    "swstate dump",
    "print the variable value",
    "oam_sw_db_info tst_trap_used_cnt dump []\n",
    "oam_sw_db_info tst_trap_used_cnt dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_oam_tst_trap_used_cnt_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(oam_sw_db_info_info[unit], oam_sw_db_info_layout_str, OAM_SW_DB_INFO_INFO_NOF_ENTRIES, "OAM_SW_DB_INFO~TST_TRAP_USED_CNT~", sizeof(((oam_sw_db_info_t*)sw_state_roots_array[unit][OAM_MODULE_ID])->tst_trap_used_cnt));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_oam_tst_trap_used_cnt_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_oam_tst_trap_used_cnt_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "oam_sw_db_info tst_trap_used_cnt size_get",
    "oam_sw_db_info tst_trap_used_cnt size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE