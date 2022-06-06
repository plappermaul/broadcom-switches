/** \file dnx/swstate/auto_generated/diagnostic/dnx_ipq_alloc_mngr_commandline.c
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

#include <bcm_int/dnx/algo/swstate/auto_generated/diagnostic/dnx_ipq_alloc_mngr_diagnostic.h>
#include "dnx_ipq_alloc_mngr_commandline.h"
#include <bcm_int/dnx/algo/swstate/auto_generated/access/dnx_ipq_alloc_mngr_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_cmds[] = {
    {"regions", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_regions_man},
    {"fmq", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_man},
    {"ipq_alloc_mngr", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_man},
    {"tc_voq_flow_map", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_man},
    {"tc_sysport_map", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_man},
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    dnx_ipq_alloc_mngr_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_dump_man = {
    "swstate dnx_ipq_alloc_mngr dump",
    "print the module's content",
    "swstate dnx_ipq_alloc_mngr dump",
    "swstate dnx_ipq_alloc_mngr dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_size_get_man = {
    "swstate dnx_ipq_alloc_mngr size_get",
    "print the module's size",
    "swstate dnx_ipq_alloc_mngr size_get",
    "swstate dnx_ipq_alloc_mngr size_get",
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_desc[] = "swstate dnx_ipq_alloc_mngr commands";



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_cmds[] = {
    {"type", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_regions_type_man},
    {"nof_allocated_queue_quartets", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_man},
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_desc[] = "dnx_ipq_alloc_mngr_db regions commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  regions_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("regions_idx_0", regions_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_regions_dump(unit, regions_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_options[] = {
    {"regions_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db regions dump [,regions_idx_0=<int>]\n",
    "dnx_ipq_alloc_mngr_db regions dump regions_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~REGIONS~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db regions size_get",
    "dnx_ipq_alloc_mngr_db regions size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_type_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_desc[] = "dnx_ipq_alloc_mngr_db regions type commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  regions_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("regions_idx_0", regions_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_regions_type_dump(unit, regions_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_options[] = {
    {"regions_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_type_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db regions type dump [,regions_idx_0=<int>]\n",
    "dnx_ipq_alloc_mngr_db regions type dump regions_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~REGIONS~TYPE~", sizeof(((dnx_ipq_alloc_mngr_db_t*)sw_state_roots_array[unit][DNX_IPQ_ALLOC_MNGR_MODULE_ID])->regions[0].type));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_type_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db regions type size_get",
    "dnx_ipq_alloc_mngr_db regions type size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_desc[] = "dnx_ipq_alloc_mngr_db regions nof_allocated_queue_quartets commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  regions_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("regions_idx_0", regions_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_regions_nof_allocated_queue_quartets_dump(unit, regions_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_options[] = {
    {"regions_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db regions nof_allocated_queue_quartets dump [,regions_idx_0=<int>]\n",
    "dnx_ipq_alloc_mngr_db regions nof_allocated_queue_quartets dump regions_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~REGIONS~NOF_ALLOCATED_QUEUE_QUARTETS~", sizeof(((dnx_ipq_alloc_mngr_db_t*)sw_state_roots_array[unit][DNX_IPQ_ALLOC_MNGR_MODULE_ID])->regions[0].nof_allocated_queue_quartets));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_regions_nof_allocated_queue_quartets_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db regions nof_allocated_queue_quartets size_get",
    "dnx_ipq_alloc_mngr_db regions nof_allocated_queue_quartets size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_cmds[] = {
    {"is_enhance_scheduler_mode", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_man},
    {"max_multicast_queue", NULL, sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_cmds, NULL, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_man},
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_desc[] = "dnx_ipq_alloc_mngr_db fmq commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_fmq_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db fmq dump []\n",
    "dnx_ipq_alloc_mngr_db fmq dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~FMQ~", sizeof(((dnx_ipq_alloc_mngr_db_t*)sw_state_roots_array[unit][DNX_IPQ_ALLOC_MNGR_MODULE_ID])->fmq));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db fmq size_get",
    "dnx_ipq_alloc_mngr_db fmq size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_desc[] = "dnx_ipq_alloc_mngr_db fmq is_enhance_scheduler_mode commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_fmq_is_enhance_scheduler_mode_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db fmq is_enhance_scheduler_mode dump []\n",
    "dnx_ipq_alloc_mngr_db fmq is_enhance_scheduler_mode dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~FMQ~IS_ENHANCE_SCHEDULER_MODE~", sizeof(((dnx_ipq_alloc_mngr_db_t*)sw_state_roots_array[unit][DNX_IPQ_ALLOC_MNGR_MODULE_ID])->fmq.is_enhance_scheduler_mode));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_is_enhance_scheduler_mode_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db fmq is_enhance_scheduler_mode size_get",
    "dnx_ipq_alloc_mngr_db fmq is_enhance_scheduler_mode size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_desc[] = "dnx_ipq_alloc_mngr_db fmq max_multicast_queue commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_fmq_max_multicast_queue_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db fmq max_multicast_queue dump []\n",
    "dnx_ipq_alloc_mngr_db fmq max_multicast_queue dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~FMQ~MAX_MULTICAST_QUEUE~", sizeof(((dnx_ipq_alloc_mngr_db_t*)sw_state_roots_array[unit][DNX_IPQ_ALLOC_MNGR_MODULE_ID])->fmq.max_multicast_queue));
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_fmq_max_multicast_queue_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db fmq max_multicast_queue size_get",
    "dnx_ipq_alloc_mngr_db fmq max_multicast_queue size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_desc[] = "dnx_ipq_alloc_mngr_db ipq_alloc_mngr commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_ipq_alloc_mngr_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db ipq_alloc_mngr dump []\n",
    "dnx_ipq_alloc_mngr_db ipq_alloc_mngr dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~IPQ_ALLOC_MNGR~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_ipq_alloc_mngr_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db ipq_alloc_mngr size_get",
    "dnx_ipq_alloc_mngr_db ipq_alloc_mngr size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_desc[] = "dnx_ipq_alloc_mngr_db tc_voq_flow_map commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    int  tc_voq_flow_map_idx_0 = 0;

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("tc_voq_flow_map_idx_0", tc_voq_flow_map_idx_0);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_tc_voq_flow_map_dump(unit, tc_voq_flow_map_idx_0, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_options[] = {
    {"tc_voq_flow_map_idx_0", SAL_FIELD_TYPE_INT32, "index", NULL},
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db tc_voq_flow_map dump [,tc_voq_flow_map_idx_0=<int>]\n",
    "dnx_ipq_alloc_mngr_db tc_voq_flow_map dump tc_voq_flow_map_idx_0=0 nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~TC_VOQ_FLOW_MAP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_voq_flow_map_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db tc_voq_flow_map size_get",
    "dnx_ipq_alloc_mngr_db tc_voq_flow_map size_get",
};



sh_sand_cmd_t sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_cmds[] = {
    {"dump", sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_options, &dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_man},
    {"size_get", sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_cmd, NULL, dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_options, &dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_man = {
    cmd_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_desc[] = "dnx_ipq_alloc_mngr_db tc_sysport_map commands";


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(dnx_ipq_alloc_mngr_db_tc_sysport_map_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_dump_man = {
    "swstate dump",
    "print the variable value",
    "dnx_ipq_alloc_mngr_db tc_sysport_map dump []\n",
    "dnx_ipq_alloc_mngr_db tc_sysport_map dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(dnx_ipq_alloc_mngr_db_info[unit], dnx_ipq_alloc_mngr_db_layout_str, DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES, "DNX_IPQ_ALLOC_MNGR_DB~TC_SYSPORT_MAP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_dnx_ipq_alloc_mngr_tc_sysport_map_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "dnx_ipq_alloc_mngr_db tc_sysport_map size_get",
    "dnx_ipq_alloc_mngr_db tc_sysport_map size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE