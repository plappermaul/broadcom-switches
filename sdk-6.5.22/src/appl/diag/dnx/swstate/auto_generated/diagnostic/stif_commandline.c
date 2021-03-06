/** \file dnx/swstate/auto_generated/diagnostic/stif_commandline.c
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

#include <soc/dnx/swstate/auto_generated/diagnostic/stif_diagnostic.h>
#include "stif_commandline.h"
#include <soc/dnx/swstate/auto_generated/access/stif_access.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_stif_cmds[] = {
    {"instance", NULL, sh_dnx_swstate_stif_instance_cmds, NULL, &dnx_swstate_stif_instance_man},
    {"source_mapping", NULL, sh_dnx_swstate_stif_source_mapping_cmds, NULL, &dnx_swstate_stif_source_mapping_man},
    {"dump", sh_dnx_swstate_stif_dump_cmd, NULL, dnx_swstate_stif_dump_options, &dnx_swstate_stif_dump_man},
    {"size_get", sh_dnx_swstate_stif_size_get_cmd, NULL, dnx_swstate_stif_size_get_options, &dnx_swstate_stif_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_diagnostic_operation_counters_options, &dnx_swstate_stif_diagnostic_operation_counters_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_stif_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_stif_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_stif_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_dump_man = {
    "swstate stif dump",
    "print the module's content",
    "swstate stif dump",
    "swstate stif dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_size_get_man = {
    "swstate stif size_get",
    "print the module's size",
    "swstate stif size_get",
    "swstate stif size_get",
};


shr_error_e sh_dnx_swstate_stif_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{


    SHR_FUNC_INIT_VARS(unit);

    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_diagnostic_operation_counters_man = {
    "swstate stif diagnostic_operation_counters",
    "print the module's diagnostic operation counters",
    "swstate stif diagnostic_operation_counters",
    "swstate stif diagnostic_operation_counters",
};


sh_sand_man_t dnx_swstate_stif_man = {
    cmd_dnx_swstate_stif_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_desc[] = "swstate stif commands";



sh_sand_cmd_t sh_dnx_swstate_stif_instance_cmds[] = {
    {"logical_port", NULL, sh_dnx_swstate_stif_instance_logical_port_cmds, NULL, &dnx_swstate_stif_instance_logical_port_man},
    {"dump", sh_dnx_swstate_stif_instance_dump_cmd, NULL, dnx_swstate_stif_instance_dump_options, &dnx_swstate_stif_instance_dump_man},
    {"size_get", sh_dnx_swstate_stif_instance_size_get_cmd, NULL, dnx_swstate_stif_instance_size_get_options, &dnx_swstate_stif_instance_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_instance_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_instance_diagnostic_operation_counters_options, &dnx_swstate_stif_instance_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_man = {
    cmd_dnx_swstate_stif_instance_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_instance_desc[] = "dnx_stif_db instance commands";


shr_error_e sh_dnx_swstate_stif_instance_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  instance_idx_0 = 0;
    int  instance_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("instance_idx_0", instance_idx_0);
    SH_SAND_GET_INT32("instance_idx_1", instance_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_stif_db_instance_dump(unit, instance_idx_0, instance_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_dump_options[] = {
    {"instance_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"instance_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_stif_db instance dump [,instance_idx_0=<int>,instance_idx_1=<int>]\n",
    "dnx_stif_db instance dump instance_idx_0=0 instance_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_instance_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~INSTANCE~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_stif_db instance size_get",
    "dnx_stif_db instance size_get",
};


shr_error_e sh_dnx_swstate_stif_instance_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~INSTANCE~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_stif_db instance diagnostic_operation_counters",
    "dnx_stif_db instance diagnostic_operation_counters",
};



sh_sand_cmd_t sh_dnx_swstate_stif_instance_logical_port_cmds[] = {
    {"dump", sh_dnx_swstate_stif_instance_logical_port_dump_cmd, NULL, dnx_swstate_stif_instance_logical_port_dump_options, &dnx_swstate_stif_instance_logical_port_dump_man},
    {"size_get", sh_dnx_swstate_stif_instance_logical_port_size_get_cmd, NULL, dnx_swstate_stif_instance_logical_port_size_get_options, &dnx_swstate_stif_instance_logical_port_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_options, &dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_logical_port_man = {
    cmd_dnx_swstate_stif_instance_logical_port_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_instance_logical_port_desc[] = "dnx_stif_db instance logical_port commands";


shr_error_e sh_dnx_swstate_stif_instance_logical_port_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  instance_idx_0 = 0;
    int  instance_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("instance_idx_0", instance_idx_0);
    SH_SAND_GET_INT32("instance_idx_1", instance_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_stif_db_instance_logical_port_dump(unit, instance_idx_0, instance_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_logical_port_dump_options[] = {
    {"instance_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"instance_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_logical_port_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_stif_db instance logical_port dump [,instance_idx_0=<int>,instance_idx_1=<int>]\n",
    "dnx_stif_db instance logical_port dump instance_idx_0=0 instance_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_instance_logical_port_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~INSTANCE~LOGICAL_PORT~", sizeof(((dnx_stif_db_t*)sw_state_roots_array[unit][STIF_MODULE_ID])->instance[0][0].logical_port));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_logical_port_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_logical_port_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_stif_db instance logical_port size_get",
    "dnx_stif_db instance logical_port size_get",
};


shr_error_e sh_dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~INSTANCE~LOGICAL_PORT~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_instance_logical_port_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_stif_db instance logical_port diagnostic_operation_counters",
    "dnx_stif_db instance logical_port diagnostic_operation_counters",
};



sh_sand_cmd_t sh_dnx_swstate_stif_source_mapping_cmds[] = {
    {"first_port", NULL, sh_dnx_swstate_stif_source_mapping_first_port_cmds, NULL, &dnx_swstate_stif_source_mapping_first_port_man},
    {"second_port", NULL, sh_dnx_swstate_stif_source_mapping_second_port_cmds, NULL, &dnx_swstate_stif_source_mapping_second_port_man},
    {"dump", sh_dnx_swstate_stif_source_mapping_dump_cmd, NULL, dnx_swstate_stif_source_mapping_dump_options, &dnx_swstate_stif_source_mapping_dump_man},
    {"size_get", sh_dnx_swstate_stif_source_mapping_size_get_cmd, NULL, dnx_swstate_stif_source_mapping_size_get_options, &dnx_swstate_stif_source_mapping_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_source_mapping_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_source_mapping_diagnostic_operation_counters_options, &dnx_swstate_stif_source_mapping_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_man = {
    cmd_dnx_swstate_stif_source_mapping_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_source_mapping_desc[] = "dnx_stif_db source_mapping commands";


shr_error_e sh_dnx_swstate_stif_source_mapping_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  source_mapping_idx_0 = 0;
    int  source_mapping_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("source_mapping_idx_0", source_mapping_idx_0);
    SH_SAND_GET_INT32("source_mapping_idx_1", source_mapping_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_stif_db_source_mapping_dump(unit, source_mapping_idx_0, source_mapping_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_dump_options[] = {
    {"source_mapping_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"source_mapping_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_stif_db source_mapping dump [,source_mapping_idx_0=<int>,source_mapping_idx_1=<int>]\n",
    "dnx_stif_db source_mapping dump source_mapping_idx_0=0 source_mapping_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_stif_db source_mapping size_get",
    "dnx_stif_db source_mapping size_get",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_stif_db source_mapping diagnostic_operation_counters",
    "dnx_stif_db source_mapping diagnostic_operation_counters",
};



sh_sand_cmd_t sh_dnx_swstate_stif_source_mapping_first_port_cmds[] = {
    {"dump", sh_dnx_swstate_stif_source_mapping_first_port_dump_cmd, NULL, dnx_swstate_stif_source_mapping_first_port_dump_options, &dnx_swstate_stif_source_mapping_first_port_dump_man},
    {"size_get", sh_dnx_swstate_stif_source_mapping_first_port_size_get_cmd, NULL, dnx_swstate_stif_source_mapping_first_port_size_get_options, &dnx_swstate_stif_source_mapping_first_port_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_options, &dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_first_port_man = {
    cmd_dnx_swstate_stif_source_mapping_first_port_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_source_mapping_first_port_desc[] = "dnx_stif_db source_mapping first_port commands";


shr_error_e sh_dnx_swstate_stif_source_mapping_first_port_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  source_mapping_idx_0 = 0;
    int  source_mapping_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("source_mapping_idx_0", source_mapping_idx_0);
    SH_SAND_GET_INT32("source_mapping_idx_1", source_mapping_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_stif_db_source_mapping_first_port_dump(unit, source_mapping_idx_0, source_mapping_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_first_port_dump_options[] = {
    {"source_mapping_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"source_mapping_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_first_port_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_stif_db source_mapping first_port dump [,source_mapping_idx_0=<int>,source_mapping_idx_1=<int>]\n",
    "dnx_stif_db source_mapping first_port dump source_mapping_idx_0=0 source_mapping_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_first_port_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~FIRST_PORT~", sizeof(((dnx_stif_db_t*)sw_state_roots_array[unit][STIF_MODULE_ID])->source_mapping[0][0].first_port));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_first_port_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_first_port_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_stif_db source_mapping first_port size_get",
    "dnx_stif_db source_mapping first_port size_get",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~FIRST_PORT~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_first_port_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_stif_db source_mapping first_port diagnostic_operation_counters",
    "dnx_stif_db source_mapping first_port diagnostic_operation_counters",
};



sh_sand_cmd_t sh_dnx_swstate_stif_source_mapping_second_port_cmds[] = {
    {"dump", sh_dnx_swstate_stif_source_mapping_second_port_dump_cmd, NULL, dnx_swstate_stif_source_mapping_second_port_dump_options, &dnx_swstate_stif_source_mapping_second_port_dump_man},
    {"size_get", sh_dnx_swstate_stif_source_mapping_second_port_size_get_cmd, NULL, dnx_swstate_stif_source_mapping_second_port_size_get_options, &dnx_swstate_stif_source_mapping_second_port_size_get_man},
    {"diagnostic_operation_counters", sh_dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_cmd, NULL, dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_options, &dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_second_port_man = {
    cmd_dnx_swstate_stif_source_mapping_second_port_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_stif_source_mapping_second_port_desc[] = "dnx_stif_db source_mapping second_port commands";


shr_error_e sh_dnx_swstate_stif_source_mapping_second_port_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  source_mapping_idx_0 = 0;
    int  source_mapping_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("source_mapping_idx_0", source_mapping_idx_0);
    SH_SAND_GET_INT32("source_mapping_idx_1", source_mapping_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_stif_db_source_mapping_second_port_dump(unit, source_mapping_idx_0, source_mapping_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_second_port_dump_options[] = {
    {"source_mapping_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"source_mapping_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_second_port_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_stif_db source_mapping second_port dump [,source_mapping_idx_0=<int>,source_mapping_idx_1=<int>]\n",
    "dnx_stif_db source_mapping second_port dump source_mapping_idx_0=0 source_mapping_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_second_port_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~SECOND_PORT~", sizeof(((dnx_stif_db_t*)sw_state_roots_array[unit][STIF_MODULE_ID])->source_mapping[0][0].second_port));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_second_port_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_second_port_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_stif_db source_mapping second_port size_get",
    "dnx_stif_db source_mapping second_port size_get",
};


shr_error_e sh_dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    SHR_FUNC_INIT_VARS(unit);


    dnx_sw_state_diagnostic_operation_counters_print(dnx_stif_db_info[unit], dnx_stif_db_layout_str, DNX_STIF_DB_INFO_NOF_ENTRIES, "DNX_STIF_DB~SOURCE_MAPPING~SECOND_PORT~");
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_stif_source_mapping_second_port_diagnostic_operation_counters_man = {
    "swstate diagnostic_operation_counters",
    "print the diagnostic operation counters",
    "dnx_stif_db source_mapping second_port diagnostic_operation_counters",
    "dnx_stif_db source_mapping second_port diagnostic_operation_counters",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
