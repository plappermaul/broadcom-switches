/**
 * \file diag_sand_framework_autocomplete.c
 *
 * Autocomplete sand shell commands.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#include <sal/appl/sal.h>

#include <shared/shrextend/shrextend_debug.h>
#include <soc/drv.h>
#include <appl/diag/system.h>
#include <appl/diag/shell.h>
#include "diag_sand_framework_autocomplete.h"

#ifdef DNX_SW_STATE_LOGGING
#include <soc/dnxc/swstate/auto_generated/access/dnxc_module_ids_access.h>
#include <soc/dnxc/swstate/dnx_sw_state_logging.h>
#endif /* DNX_SW_STATE_LOGGING */

#define BSL_LOG_MODULE BSL_LS_APPL_SHELL

#ifdef INCLUDE_AUTOCOMPLETE

/* fill the shell autocomplete tree with the commands, sub commands and options */

void
sh_sand_cmd_autocomplete_enum_init(
    int unit,
    autocomplete_node_t * ac_node,
    sh_sand_enum_t * enum_struct)
{
    for (; enum_struct != NULL && enum_struct->string != NULL; enum_struct++)
    {
        char enum_text[AUTOCOMPLETE_MAX_STRING_LEN] = { 0 };
        sal_strncat_s(enum_text, enum_struct->string, AUTOCOMPLETE_MAX_STRING_LEN);
        autocomplete_node_add(unit, ac_node, enum_text, /* is_option */ 1, /* invoke_cb */ NULL,        /* ac_option_cb 
                                                                                                         */ NULL);
    }
}

void
sh_sand_cmd_autocomplete_options_init_recursive(
    int unit,
    sh_sand_option_t * option,
    autocomplete_node_t * ac_node,
    sh_sand_invoke_cb_t invoke_cb)
{
    for (; option != NULL && (option->keyword != NULL || option->inherited_options != NULL); option++)
    {
        if (option->inherited_options)
        {
            /*
             * recursively process all inherited options as siblings of current option's siblings 
             */
            sh_sand_cmd_autocomplete_options_init_recursive(unit, option->inherited_options, ac_node, invoke_cb);
        }
        if (option->keyword)
        {
            char option_text[AUTOCOMPLETE_MAX_STRING_LEN] = { 0 };
            sal_strncat_s(option_text, option->keyword, AUTOCOMPLETE_MAX_STRING_LEN);
            if (option->type != SAL_FIELD_TYPE_BOOL && option->type != SAL_FIELD_TYPE_NONE)
            {
                sal_strncat_s(option_text, "=", AUTOCOMPLETE_MAX_STRING_LEN);
            }
            if (option->type == SAL_FIELD_TYPE_ENUM && option->ext_ptr)
            {
                /*
                 * add parent node for enum values 
                 */
                autocomplete_node_t *enum_ac_node = autocomplete_node_add(unit, ac_node, option_text, /* is_option */ 1,        /* invoke_cb 
                                                                           */ NULL, /* ac_option_cb */ NULL);
                sh_sand_cmd_autocomplete_enum_init(unit, enum_ac_node, (sh_sand_enum_t *) option->ext_ptr);
            }
            else
            {
                autocomplete_node_add(unit, ac_node, option_text, /* is_option */ 1, invoke_cb, option->ac_option_cb);
            }
        }
    }
}

void
sh_sand_cmd_autocomplete_init(
    int unit,
    autocomplete_node_t * parent,
    sh_sand_cmd_t * sh_sand_cmd_a)
{
    int flags = 0;
    sh_sand_cmd_t *sh_sand_cmd;

    /*
     * For auto-compeltion we ignore commands that are not available in current HW/SW configuration
     */
    SH_SAND_CMD_ITERATOR_COND(sh_sand_cmd, sh_sand_cmd_a, flags)
    {
        /*
         * add a node to the autocomplete tree with the keyword as the string and the upper command as a parent
         */
        autocomplete_node_t *ac_node =
            autocomplete_node_add(unit, parent, sh_sand_cmd->keyword, 0, sh_sand_cmd->invoke_cb, NULL);
        if (sh_sand_cmd->child_cmd_a == NULL)
        {
            sh_sand_cmd_autocomplete_options_init_recursive(unit, sh_sand_cmd->options, ac_node,
                                                            sh_sand_cmd->invoke_cb);
        }
        else
        {
            sh_sand_cmd_autocomplete_init(unit, ac_node, sh_sand_cmd->child_cmd_a);
        }
    }

    if (parent != NULL && sh_sand_cmd_a != NULL && sh_sand_cmd_a->keyword != NULL)
    {
        autocomplete_node_add(unit, parent, "usage", 0, NULL, NULL);
    }
}

void
sh_sand_cmd_autocomplete_deinit(
    int unit,
    sh_sand_cmd_t * sh_sand_cmd_a)
{
    int flags = 0;
    sh_sand_cmd_t *sh_sand_cmd;

    SH_SAND_CMD_ITERATOR_COND(sh_sand_cmd, sh_sand_cmd_a, flags)
    {
        autocomplete_node_t *ac_node = autocomplete_find_root(unit, sh_sand_cmd->keyword);
        autocomplete_node_delete(unit, ac_node);
    }
}

#if defined(DNX_SW_STATE_LOGGING)
void
sh_sand_sw_state_cmd_autocomplete_init(
    int unit)
{
    int module_id;
    int func_type;
    char *log_start_keywords[5] = { "swstate", "logger", "start", "module_id=", NULL };
    char *log_stop_keywords[5] = { "swstate", "logger", "stop", "module_id=", NULL };
    char *log_start_by_function_keywords[5] = { "swstate", "logger", "start", "function_type=", NULL };
    char *log_stop_by_function_keywords[5] = { "swstate", "logger", "stop", "function_type=", NULL };

    char *log_add_keywords[5] = { "swstate", "logger", "add", "module_id=", NULL };
    char *log_remove_keywords[5] = { "swstate", "logger", "remove", "module_id=", NULL };
    char *log_add_by_function_keywords[5] = { "swstate", "logger", "add", "function_type=", NULL };
    char *log_remove_by_function_keywords[5] = { "swstate", "logger", "remove", "function_type=", NULL };
    char *log_print_keywords[5] = { "swstate", "logger", "print", "module_id=", NULL };
    char *log_print_by_function_keywords[5] = { "swstate", "logger", "print", "function_type=", NULL };

    char opt_text[AUTOCOMPLETE_MAX_STRING_LEN] = { 0 };
    char log_by_func_text[AUTOCOMPLETE_MAX_STRING_LEN] = { 0 };

    autocomplete_node_t *log_start_node = autocomplete_find_node(unit, log_start_keywords);
    autocomplete_node_t *log_stop_node = autocomplete_find_node(unit, log_stop_keywords);
    autocomplete_node_t *log_func_start_node = autocomplete_find_node(unit, log_start_by_function_keywords);
    autocomplete_node_t *log_func_stop_node = autocomplete_find_node(unit, log_stop_by_function_keywords);

    autocomplete_node_t *log_add_node = autocomplete_find_node(unit, log_add_keywords);
    autocomplete_node_t *log_remove_node = autocomplete_find_node(unit, log_remove_keywords);
    autocomplete_node_t *log_func_add_node = autocomplete_find_node(unit, log_add_by_function_keywords);
    autocomplete_node_t *log_func_remove_node = autocomplete_find_node(unit, log_remove_by_function_keywords);
    autocomplete_node_t *log_print_node = autocomplete_find_node(unit, log_print_keywords);
    autocomplete_node_t *log_func_print_node = autocomplete_find_node(unit, log_print_by_function_keywords);

    if (log_start_node == NULL)
    {
        cli_out("swstate logger start module_id= command's autocomplete node not found\n");
        goto exit;
    }

    if (log_stop_node == NULL)
    {
        cli_out("swstate logger stop module_id= command's autocomplete node not found\n");
        goto exit;
    }

    for (module_id = 0; module_id < NOF_MODULE_ID; module_id++)
    {
        if (dnxc_module_id_e_get_name(module_id) != NULL)
        {
            sal_strncpy
                (opt_text, dnxc_module_id_e_get_name(module_id),
                 AUTOCOMPLETE_MAX_STRING_LEN - sal_strlen(opt_text) - 1);
            autocomplete_node_add(unit, log_start_node, opt_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_stop_node, opt_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_add_node, opt_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_remove_node, opt_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_print_node, opt_text, 0, NULL, NULL);
        }
    }

    if (log_func_start_node == NULL)
    {
        cli_out("swstate logger start function_type= command's autocomplete node not found\n");
        goto exit;
    }

    if (log_func_stop_node == NULL)
    {
        cli_out("swstate logger stop function_type= command's autocomplete node not found\n");
        goto exit;
    }

    for (func_type = 0; func_type < DNX_SW_STATE_COUNT_LOGGING; func_type++)
    {
        if (dnxc_fuction_type_e_get_name(func_type) != NULL)
        {
            sal_strncpy(log_by_func_text, dnxc_fuction_type_e_get_name(func_type),
                        AUTOCOMPLETE_MAX_STRING_LEN - sal_strlen(log_by_func_text) - 1);
            autocomplete_node_add(unit, log_func_start_node, log_by_func_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_func_stop_node, log_by_func_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_func_add_node, log_by_func_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_func_remove_node, log_by_func_text, 0, NULL, NULL);
            autocomplete_node_add(unit, log_func_print_node, log_by_func_text, 0, NULL, NULL);
        }
    }
exit:
    return;
}
#endif /* DNX_SW_STATE_LOGGING */

#else
/*
 * {
 */

/**
 * This is DUMMY code. It is only for compilers that do not accept empty files
 * and is never to be used.
 */
void
sh_sand_autocomplete_dummy_empty_function_to_make_compiler_happy(
    int unit)
{
}

/*
 * }
 */
#endif /* INCLUDE_AUTOCOMPLETE */
