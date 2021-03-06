/** \file dnx/swstate/auto_generated/diagnostic/algo_rx_commandline.c
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

#include <bcm_int/dnx/algo/swstate/auto_generated/diagnostic/algo_rx_diagnostic.h>
#include "algo_rx_commandline.h"
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_rx_access.h>
#include <bcm_int/dnx/rx/rx.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
sh_sand_cmd_t sh_dnx_swstate_algo_rx_cmds[] = {
    {"trap_ingress_predefined", NULL, sh_dnx_swstate_algo_rx_trap_ingress_predefined_cmds, NULL, &dnx_swstate_algo_rx_trap_ingress_predefined_man},
    {"trap_ingress_user_defined", NULL, sh_dnx_swstate_algo_rx_trap_ingress_user_defined_cmds, NULL, &dnx_swstate_algo_rx_trap_ingress_user_defined_man},
    {"trap_erpp", NULL, sh_dnx_swstate_algo_rx_trap_erpp_cmds, NULL, &dnx_swstate_algo_rx_trap_erpp_man},
    {"trap_etpp", NULL, sh_dnx_swstate_algo_rx_trap_etpp_cmds, NULL, &dnx_swstate_algo_rx_trap_etpp_man},
    {"trap_etpp_oam", NULL, sh_dnx_swstate_algo_rx_trap_etpp_oam_cmds, NULL, &dnx_swstate_algo_rx_trap_etpp_oam_man},
    {"trap_protocol_icmp_type_map", NULL, sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_cmds, NULL, &dnx_swstate_algo_rx_trap_protocol_icmp_type_map_man},
    {"trap_recycle_cmd", NULL, sh_dnx_swstate_algo_rx_trap_recycle_cmd_cmds, NULL, &dnx_swstate_algo_rx_trap_recycle_cmd_man},
    {"trap_lif_ingress", NULL, sh_dnx_swstate_algo_rx_trap_lif_ingress_cmds, NULL, &dnx_swstate_algo_rx_trap_lif_ingress_man},
    {"trap_lif_etpp", NULL, sh_dnx_swstate_algo_rx_trap_lif_etpp_cmds, NULL, &dnx_swstate_algo_rx_trap_lif_etpp_man},
    {"trap_mtu", NULL, sh_dnx_swstate_algo_rx_trap_mtu_cmds, NULL, &dnx_swstate_algo_rx_trap_mtu_man},
    {"dump", sh_dnx_swstate_algo_rx_dump_cmd, NULL, dnx_swstate_algo_rx_dump_options, &dnx_swstate_algo_rx_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_size_get_cmd, NULL, dnx_swstate_algo_rx_size_get_options, &dnx_swstate_algo_rx_size_get_man},
    {NULL}
};


shr_error_e sh_dnx_swstate_algo_rx_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint8 is_init;
    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    algo_rx_db.is_init(unit, &is_init);
    if (is_init) {
        SHR_IF_ERR_EXIT(algo_rx_db_dump(unit, filters));
        if (!filters.nocontent)
        {
            dnx_sw_state_dump_detach_file(unit);
        }
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_dump_man = {
    "swstate algo_rx dump",
    "print the module's content",
    "swstate algo_rx dump",
    "swstate algo_rx dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    uint32 size = 0;
    SHR_FUNC_INIT_VARS(unit);

    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_size_get_man = {
    "swstate algo_rx size_get",
    "print the module's size",
    "swstate algo_rx size_get",
    "swstate algo_rx size_get",
};


sh_sand_man_t dnx_swstate_algo_rx_man = {
    cmd_dnx_swstate_algo_rx_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_desc[] = "swstate algo_rx commands";



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_ingress_predefined_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_ingress_predefined_dump_cmd, NULL, dnx_swstate_algo_rx_trap_ingress_predefined_dump_options, &dnx_swstate_algo_rx_trap_ingress_predefined_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_ingress_predefined_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_ingress_predefined_size_get_options, &dnx_swstate_algo_rx_trap_ingress_predefined_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_predefined_man = {
    cmd_dnx_swstate_algo_rx_trap_ingress_predefined_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_ingress_predefined_desc[] = "algo_rx_db trap_ingress_predefined commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_ingress_predefined_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_ingress_predefined_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_ingress_predefined_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_predefined_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_ingress_predefined dump []\n",
    "algo_rx_db trap_ingress_predefined dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_ingress_predefined_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_INGRESS_PREDEFINED~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_ingress_predefined_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_predefined_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_ingress_predefined size_get",
    "algo_rx_db trap_ingress_predefined size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_ingress_user_defined_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_ingress_user_defined_dump_cmd, NULL, dnx_swstate_algo_rx_trap_ingress_user_defined_dump_options, &dnx_swstate_algo_rx_trap_ingress_user_defined_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_options, &dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_user_defined_man = {
    cmd_dnx_swstate_algo_rx_trap_ingress_user_defined_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_ingress_user_defined_desc[] = "algo_rx_db trap_ingress_user_defined commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_ingress_user_defined_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_ingress_user_defined_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_ingress_user_defined_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_user_defined_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_ingress_user_defined dump []\n",
    "algo_rx_db trap_ingress_user_defined dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_INGRESS_USER_DEFINED~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_ingress_user_defined_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_ingress_user_defined size_get",
    "algo_rx_db trap_ingress_user_defined size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_erpp_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_erpp_dump_cmd, NULL, dnx_swstate_algo_rx_trap_erpp_dump_options, &dnx_swstate_algo_rx_trap_erpp_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_erpp_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_erpp_size_get_options, &dnx_swstate_algo_rx_trap_erpp_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_erpp_man = {
    cmd_dnx_swstate_algo_rx_trap_erpp_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_erpp_desc[] = "algo_rx_db trap_erpp commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_erpp_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_erpp_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_erpp_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_erpp_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_erpp dump []\n",
    "algo_rx_db trap_erpp dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_erpp_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_ERPP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_erpp_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_erpp_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_erpp size_get",
    "algo_rx_db trap_erpp size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_etpp_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_etpp_dump_cmd, NULL, dnx_swstate_algo_rx_trap_etpp_dump_options, &dnx_swstate_algo_rx_trap_etpp_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_etpp_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_etpp_size_get_options, &dnx_swstate_algo_rx_trap_etpp_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_man = {
    cmd_dnx_swstate_algo_rx_trap_etpp_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_etpp_desc[] = "algo_rx_db trap_etpp commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_etpp_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_etpp_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_etpp_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_etpp dump []\n",
    "algo_rx_db trap_etpp dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_etpp_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_ETPP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_etpp_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_etpp size_get",
    "algo_rx_db trap_etpp size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_etpp_oam_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_etpp_oam_dump_cmd, NULL, dnx_swstate_algo_rx_trap_etpp_oam_dump_options, &dnx_swstate_algo_rx_trap_etpp_oam_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_etpp_oam_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_etpp_oam_size_get_options, &dnx_swstate_algo_rx_trap_etpp_oam_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_oam_man = {
    cmd_dnx_swstate_algo_rx_trap_etpp_oam_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_etpp_oam_desc[] = "algo_rx_db trap_etpp_oam commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_etpp_oam_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_etpp_oam_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_etpp_oam_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_oam_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_etpp_oam dump []\n",
    "algo_rx_db trap_etpp_oam dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_etpp_oam_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_ETPP_OAM~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_etpp_oam_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_etpp_oam_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_etpp_oam size_get",
    "algo_rx_db trap_etpp_oam size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_cmd, NULL, dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_options, &dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_options, &dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_protocol_icmp_type_map_man = {
    cmd_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_desc[] = "algo_rx_db trap_protocol_icmp_type_map commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_protocol_icmp_type_map_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_protocol_icmp_type_map_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_protocol_icmp_type_map dump []\n",
    "algo_rx_db trap_protocol_icmp_type_map dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_PROTOCOL_ICMP_TYPE_MAP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_protocol_icmp_type_map_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_protocol_icmp_type_map size_get",
    "algo_rx_db trap_protocol_icmp_type_map size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_recycle_cmd_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_recycle_cmd_dump_cmd, NULL, dnx_swstate_algo_rx_trap_recycle_cmd_dump_options, &dnx_swstate_algo_rx_trap_recycle_cmd_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_recycle_cmd_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_recycle_cmd_size_get_options, &dnx_swstate_algo_rx_trap_recycle_cmd_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_recycle_cmd_man = {
    cmd_dnx_swstate_algo_rx_trap_recycle_cmd_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_recycle_cmd_desc[] = "algo_rx_db trap_recycle_cmd commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_recycle_cmd_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_recycle_cmd_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_recycle_cmd_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_recycle_cmd_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_recycle_cmd dump []\n",
    "algo_rx_db trap_recycle_cmd dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_recycle_cmd_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_RECYCLE_CMD~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_recycle_cmd_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_recycle_cmd_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_recycle_cmd size_get",
    "algo_rx_db trap_recycle_cmd size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_lif_ingress_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_lif_ingress_dump_cmd, NULL, dnx_swstate_algo_rx_trap_lif_ingress_dump_options, &dnx_swstate_algo_rx_trap_lif_ingress_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_lif_ingress_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_lif_ingress_size_get_options, &dnx_swstate_algo_rx_trap_lif_ingress_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_ingress_man = {
    cmd_dnx_swstate_algo_rx_trap_lif_ingress_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_lif_ingress_desc[] = "algo_rx_db trap_lif_ingress commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_lif_ingress_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_lif_ingress_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_lif_ingress_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_ingress_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_lif_ingress dump []\n",
    "algo_rx_db trap_lif_ingress dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_lif_ingress_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_LIF_INGRESS~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_lif_ingress_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_ingress_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_lif_ingress size_get",
    "algo_rx_db trap_lif_ingress size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_lif_etpp_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_lif_etpp_dump_cmd, NULL, dnx_swstate_algo_rx_trap_lif_etpp_dump_options, &dnx_swstate_algo_rx_trap_lif_etpp_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_lif_etpp_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_lif_etpp_size_get_options, &dnx_swstate_algo_rx_trap_lif_etpp_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_etpp_man = {
    cmd_dnx_swstate_algo_rx_trap_lif_etpp_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_lif_etpp_desc[] = "algo_rx_db trap_lif_etpp commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_lif_etpp_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_lif_etpp_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_lif_etpp_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_etpp_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_lif_etpp dump []\n",
    "algo_rx_db trap_lif_etpp dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_lif_etpp_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_LIF_ETPP~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_lif_etpp_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_lif_etpp_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_lif_etpp size_get",
    "algo_rx_db trap_lif_etpp size_get",
};



sh_sand_cmd_t sh_dnx_swstate_algo_rx_trap_mtu_cmds[] = {
    {"dump", sh_dnx_swstate_algo_rx_trap_mtu_dump_cmd, NULL, dnx_swstate_algo_rx_trap_mtu_dump_options, &dnx_swstate_algo_rx_trap_mtu_dump_man},
    {"size_get", sh_dnx_swstate_algo_rx_trap_mtu_size_get_cmd, NULL, dnx_swstate_algo_rx_trap_mtu_size_get_options, &dnx_swstate_algo_rx_trap_mtu_size_get_man},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_mtu_man = {
    cmd_dnx_swstate_algo_rx_trap_mtu_desc,
    NULL,
    NULL,
    NULL,
};


const char cmd_dnx_swstate_algo_rx_trap_mtu_desc[] = "algo_rx_db trap_mtu commands";


shr_error_e sh_dnx_swstate_algo_rx_trap_mtu_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{

    dnx_sw_state_dump_filters_t filters;

    SHR_FUNC_INIT_VARS(unit);


    SH_SAND_GET_BOOL("nocontent", filters.nocontent);
    SH_SAND_GET_STR("typefilter", filters.typefilter);
    SH_SAND_GET_STR("namefilter", filters.namefilter);

    SHR_IF_ERR_EXIT(algo_rx_db_trap_mtu_dump(unit, filters));

    if (!filters.nocontent)
    {
        dnx_sw_state_dump_detach_file(unit);

    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_mtu_dump_options[] = {
    {"nocontent",      SAL_FIELD_TYPE_BOOL, "Decision if to perform content dump or to print the access tree.",  "false"},
    {"typefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable type.",         ""},
    {"namefilter",     SAL_FIELD_TYPE_STR,  "Filter for the variable name.",         ""},
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_mtu_dump_man = {
    "swstate dump",
    "print the variable value",
    "algo_rx_db trap_mtu dump []\n",
    "algo_rx_db trap_mtu dump nocontent=false typefilter="" namefilter="" ",
};


shr_error_e sh_dnx_swstate_algo_rx_trap_mtu_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control)
{
    uint32 size = 0;

    SHR_FUNC_INIT_VARS(unit);


    size = dnx_sw_state_info_size_get(algo_rx_db_info[unit], algo_rx_db_layout_str, ALGO_RX_DB_INFO_NOF_ENTRIES, "ALGO_RX_DB~TRAP_MTU~", 0);
    dnx_sw_state_size_print(size);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}



sh_sand_option_t dnx_swstate_algo_rx_trap_mtu_size_get_options[] = {
    {NULL}
};


sh_sand_man_t dnx_swstate_algo_rx_trap_mtu_size_get_man = {
    "swstate size_get",
    "print the variable size",
    "algo_rx_db trap_mtu size_get",
    "algo_rx_db trap_mtu size_get",
};


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#undef BSL_LOG_MODULE
