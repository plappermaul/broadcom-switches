/** \file dnx/swstate/auto_generated/diagnostic/dnx_ipq_commandline.c
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

#include <soc/dnx/swstate/auto_generated/diagnostic/dnx_ipq_diagnostic.h>
#include "dnx_ipq_commandline.h"
#include <soc/dnx/swstate/auto_generated/access/dnx_ipq_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_cmds[] = {
    {"base_queues", NULL, sh_dnx_swstate_dnx_ipq_base_queues_cmds, NULL, &dnx_swstate_dnx_ipq_base_queues_man},
    {"base_queue_is_asymm", NULL, sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_cmds, NULL, &dnx_swstate_dnx_ipq_base_queue_is_asymm_man},
    {"dump", sh_dnx_swstate_dnx_ipq_dump_cmd, NULL, dnx_swstate_dnx_ipq_dump_options, &dnx_swstate_dnx_ipq_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_size_get_cmd, NULL, dnx_swstate_dnx_ipq_size_get_options, &dnx_swstate_dnx_ipq_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_dnx_ipq_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_ipq_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_ipq_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_dump_man = {
    "swstate dnx_ipq dump",
    "print the module's content",
    "swstate dnx_ipq dump",
    "swstate dnx_ipq dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_ipq_db_info[unit], dnx_ipq_db_layout_str, DNX_IPQ_DB_INFO_NOF_ENTRIES, "DNX_IPQ_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_size_get_man = {
    "swstate dnx_ipq size_get",
    "print the module's size",
    "swstate dnx_ipq size_get",
    "swstate dnx_ipq size_get",
};


sh_sand_man_t dnx_swstate_dnx_ipq_man = {
    cmd_dnx_swstate_dnx_ipq_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_desc[] = "swstate dnx_ipq commands";



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_base_queues_cmds[] = {
    {"num_cos", NULL, sh_dnx_swstate_dnx_ipq_base_queues_num_cos_cmds, NULL, &dnx_swstate_dnx_ipq_base_queues_num_cos_man},
    {"sys_port_ref_counter", NULL, sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_cmds, NULL, &dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_man},
    {"dump", sh_dnx_swstate_dnx_ipq_base_queues_dump_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_dump_options, &dnx_swstate_dnx_ipq_base_queues_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_base_queues_size_get_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_size_get_options, &dnx_swstate_dnx_ipq_base_queues_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_man = {
    cmd_dnx_swstate_dnx_ipq_base_queues_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_base_queues_desc[] = "dnx_ipq_db base_queues commands";


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  base_queues_idx_0 = 0;
    int  base_queues_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("base_queues_idx_0", base_queues_idx_0);
    SH_SAND_GET_INT32("base_queues_idx_1", base_queues_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_db_base_queues_dump(unit, base_queues_idx_0, base_queues_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_dump_options[] = {
    {"base_queues_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"base_queues_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_db base_queues dump [,base_queues_idx_0=<int>,base_queues_idx_1=<int>]\n",
    "dnx_ipq_db base_queues dump base_queues_idx_0=0 base_queues_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_db_info[unit], dnx_ipq_db_layout_str, DNX_IPQ_DB_INFO_NOF_ENTRIES, "DNX_IPQ_DB~BASE_QUEUES~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_db base_queues size_get",
    "dnx_ipq_db base_queues size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_base_queues_num_cos_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_base_queues_num_cos_dump_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_num_cos_dump_options, &dnx_swstate_dnx_ipq_base_queues_num_cos_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_options, &dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_num_cos_man = {
    cmd_dnx_swstate_dnx_ipq_base_queues_num_cos_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_base_queues_num_cos_desc[] = "dnx_ipq_db base_queues num_cos commands";


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_num_cos_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  base_queues_idx_0 = 0;
    int  base_queues_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("base_queues_idx_0", base_queues_idx_0);
    SH_SAND_GET_INT32("base_queues_idx_1", base_queues_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_db_base_queues_num_cos_dump(unit, base_queues_idx_0, base_queues_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_num_cos_dump_options[] = {
    {"base_queues_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"base_queues_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_num_cos_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_db base_queues num_cos dump [,base_queues_idx_0=<int>,base_queues_idx_1=<int>]\n",
    "dnx_ipq_db base_queues num_cos dump base_queues_idx_0=0 base_queues_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_db_info[unit], dnx_ipq_db_layout_str, DNX_IPQ_DB_INFO_NOF_ENTRIES, "DNX_IPQ_DB~BASE_QUEUES~NUM_COS~", sizeof(((dnx_ipq_db_t*)sw_state_roots_array[unit][DNX_IPQ_MODULE_ID])->base_queues[0][0].num_cos));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_num_cos_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_db base_queues num_cos size_get",
    "dnx_ipq_db base_queues num_cos size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_options, &dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_cmd, NULL, dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_options, &dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_man = {
    cmd_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_desc[] = "dnx_ipq_db base_queues sys_port_ref_counter commands";


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  base_queues_idx_0 = 0;
    int  base_queues_idx_1 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("base_queues_idx_0", base_queues_idx_0);
    SH_SAND_GET_INT32("base_queues_idx_1", base_queues_idx_1);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_db_base_queues_sys_port_ref_counter_dump(unit, base_queues_idx_0, base_queues_idx_1, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_options[] = {
    {"base_queues_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"base_queues_idx_1", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_db base_queues sys_port_ref_counter dump [,base_queues_idx_0=<int>,base_queues_idx_1=<int>]\n",
    "dnx_ipq_db base_queues sys_port_ref_counter dump base_queues_idx_0=0 base_queues_idx_1=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_db_info[unit], dnx_ipq_db_layout_str, DNX_IPQ_DB_INFO_NOF_ENTRIES, "DNX_IPQ_DB~BASE_QUEUES~SYS_PORT_REF_COUNTER~", sizeof(((dnx_ipq_db_t*)sw_state_roots_array[unit][DNX_IPQ_MODULE_ID])->base_queues[0][0].sys_port_ref_counter));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queues_sys_port_ref_counter_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_db base_queues sys_port_ref_counter size_get",
    "dnx_ipq_db base_queues sys_port_ref_counter size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_cmd, NULL, dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_options, &dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_cmd, NULL, dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_options, &dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queue_is_asymm_man = {
    cmd_dnx_swstate_dnx_ipq_base_queue_is_asymm_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_base_queue_is_asymm_desc[] = "dnx_ipq_db base_queue_is_asymm commands";


shr_error_e sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  base_queue_is_asymm_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("base_queue_is_asymm_idx_0", base_queue_is_asymm_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_db_base_queue_is_asymm_dump(unit, base_queue_is_asymm_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_options[] = {
    {"base_queue_is_asymm_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queue_is_asymm_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_db base_queue_is_asymm dump [,base_queue_is_asymm_idx_0=<int>]\n",
    "dnx_ipq_db base_queue_is_asymm dump base_queue_is_asymm_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_db_info[unit], dnx_ipq_db_layout_str, DNX_IPQ_DB_INFO_NOF_ENTRIES, "DNX_IPQ_DB~BASE_QUEUE_IS_ASYMM~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_base_queue_is_asymm_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_db base_queue_is_asymm size_get",
    "dnx_ipq_db base_queue_is_asymm size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE