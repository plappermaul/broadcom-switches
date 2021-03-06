/** \file dnx/swstate/auto_generated/diagnostic/dnx_field_tcam_bank_commandline.c
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

#include <soc/dnx/swstate/auto_generated/diagnostic/dnx_field_tcam_bank_diagnostic.h>
#include "dnx_field_tcam_bank_commandline.h"
#include <soc/dnx/swstate/auto_generated/access/dnx_field_tcam_bank_access.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_cmds[] = {
    {"tcam_banks", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_man},
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_dump_options, &dnx_swstate_dnx_field_tcam_bank_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_size_get_options, &dnx_swstate_dnx_field_tcam_bank_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_field_tcam_bank_sw.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_dump_man = {
    "swstate dnx_field_tcam_bank dump",
    "print the module's content",
    "swstate dnx_field_tcam_bank dump",
    "swstate dnx_field_tcam_bank dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_size_get_man = {
    "swstate dnx_field_tcam_bank size_get",
    "print the module's size",
    "swstate dnx_field_tcam_bank size_get",
    "swstate dnx_field_tcam_bank size_get",
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_desc[] = "swstate dnx_field_tcam_bank commands";



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_cmds[] = {
    {"owner_stage", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_man},
    {"active_handlers_id", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_man},
    {"nof_free_entries", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_man},
    {"bank_mode", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_man},
    {"key_size", NULL, sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_cmds, NULL, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_man},
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_desc[] = "dnx_field_tcam_bank_sw tcam_banks commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_dump(unit, tcam_banks_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks dump [,tcam_banks_idx_0=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks dump tcam_banks_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks size_get",
    "dnx_field_tcam_bank_sw tcam_banks size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_desc[] = "dnx_field_tcam_bank_sw tcam_banks owner_stage commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;
    int  owner_stage_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);
    SH_SAND_GET_INT32("owner_stage_idx_0", owner_stage_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_owner_stage_dump(unit, tcam_banks_idx_0, owner_stage_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"owner_stage_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks owner_stage dump [,tcam_banks_idx_0=<int>,owner_stage_idx_0=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks owner_stage dump tcam_banks_idx_0=0 owner_stage_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~OWNER_STAGE~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_owner_stage_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks owner_stage size_get",
    "dnx_field_tcam_bank_sw tcam_banks owner_stage size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_desc[] = "dnx_field_tcam_bank_sw tcam_banks active_handlers_id commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;
    int  active_handlers_id_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);
    SH_SAND_GET_INT32("active_handlers_id_idx_0", active_handlers_id_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_active_handlers_id_dump(unit, tcam_banks_idx_0, active_handlers_id_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"active_handlers_id_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks active_handlers_id dump [,tcam_banks_idx_0=<int>,active_handlers_id_idx_0=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks active_handlers_id dump tcam_banks_idx_0=0 active_handlers_id_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~ACTIVE_HANDLERS_ID~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_active_handlers_id_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks active_handlers_id size_get",
    "dnx_field_tcam_bank_sw tcam_banks active_handlers_id size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_desc[] = "dnx_field_tcam_bank_sw tcam_banks nof_free_entries commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;
    int  nof_free_entries_idx_0 = 0;
    int  nof_free_entries_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);
    SH_SAND_GET_INT32("nof_free_entries_idx_0", nof_free_entries_idx_0);
    SH_SAND_GET_INT32("nof_free_entries_idx_1", nof_free_entries_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_nof_free_entries_dump(unit, tcam_banks_idx_0, nof_free_entries_idx_0, nof_free_entries_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nof_free_entries_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nof_free_entries_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks nof_free_entries dump [,tcam_banks_idx_0=<int>,nof_free_entries_idx_0=<int>,nof_free_entries_idx_1=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks nof_free_entries dump tcam_banks_idx_0=0 nof_free_entries_idx_0=0 nof_free_entries_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~NOF_FREE_ENTRIES~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_nof_free_entries_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks nof_free_entries size_get",
    "dnx_field_tcam_bank_sw tcam_banks nof_free_entries size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_desc[] = "dnx_field_tcam_bank_sw tcam_banks bank_mode commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_bank_mode_dump(unit, tcam_banks_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks bank_mode dump [,tcam_banks_idx_0=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks bank_mode dump tcam_banks_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~BANK_MODE~", sizeof(((dnx_field_tcam_bank_sw_t*)sw_state_roots_array[unit][DNX_FIELD_TCAM_BANK_MODULE_ID])->tcam_banks[0].bank_mode));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_bank_mode_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks bank_mode size_get",
    "dnx_field_tcam_bank_sw tcam_banks bank_mode size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_man},
    {"size_get", sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_cmd, NULL, dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_options, &dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_man = {
    cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_desc[] = "dnx_field_tcam_bank_sw tcam_banks key_size commands";


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tcam_banks_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tcam_banks_idx_0", tcam_banks_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_field_tcam_bank_sw_tcam_banks_key_size_dump(unit, tcam_banks_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_options[] = {
    {"tcam_banks_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_field_tcam_bank_sw tcam_banks key_size dump [,tcam_banks_idx_0=<int>]\n",
    "dnx_field_tcam_bank_sw tcam_banks key_size dump tcam_banks_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_field_tcam_bank_sw_info[unit], dnx_field_tcam_bank_sw_layout_str, DNX_FIELD_TCAM_BANK_SW_INFO_NOF_ENTRIES, "DNX_FIELD_TCAM_BANK_SW~TCAM_BANKS~KEY_SIZE~", sizeof(((dnx_field_tcam_bank_sw_t*)sw_state_roots_array[unit][DNX_FIELD_TCAM_BANK_MODULE_ID])->tcam_banks[0].key_size));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_field_tcam_bank_tcam_banks_key_size_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_field_tcam_bank_sw tcam_banks key_size size_get",
    "dnx_field_tcam_bank_sw tcam_banks key_size size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
