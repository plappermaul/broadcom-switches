/** \file dnx/swstate/auto_generated/diagnostic/l2_commandline.h
 *
 * sw state functions declarations
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

#ifndef __L2_COMMANDLINE_H__
#define __L2_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/swstate/auto_generated/types/l2_types.h>
#include <bcm/switch.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_l2_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_dump_man;
shr_error_e sh_dnx_swstate_l2_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_size_get_man;
shr_error_e sh_dnx_swstate_l2_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_diagnostic_operation_counters_man;
extern sh_sand_cmd_t sh_dnx_swstate_l2_cmds[];
extern sh_sand_man_t dnx_swstate_l2_man;
extern const char cmd_dnx_swstate_l2_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_l2_fid_profiles_changed_cmds[];
extern sh_sand_man_t dnx_swstate_l2_fid_profiles_changed_man;
extern const char cmd_dnx_swstate_l2_fid_profiles_changed_desc[];
shr_error_e sh_dnx_swstate_l2_fid_profiles_changed_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_fid_profiles_changed_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_fid_profiles_changed_dump_man;


shr_error_e sh_dnx_swstate_l2_fid_profiles_changed_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_fid_profiles_changed_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_fid_profiles_changed_size_get_man;


shr_error_e sh_dnx_swstate_l2_fid_profiles_changed_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_fid_profiles_changed_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_fid_profiles_changed_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_counter_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_counter_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_counter_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_counter_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_counter_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4uc_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv4mc_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6uc_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_ipv6mc_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_cmds[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_man;
extern const char cmd_dnx_swstate_l2_opportunistic_learning_ctx_mpls_desc[];
shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_dump_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_size_get_man;


shr_error_e sh_dnx_swstate_l2_opportunistic_learning_ctx_mpls_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_opportunistic_learning_ctx_mpls_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_l2_routing_learning_cmds[];
extern sh_sand_man_t dnx_swstate_l2_routing_learning_man;
extern const char cmd_dnx_swstate_l2_routing_learning_desc[];
shr_error_e sh_dnx_swstate_l2_routing_learning_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_routing_learning_dump_options[];
extern sh_sand_man_t dnx_swstate_l2_routing_learning_dump_man;


shr_error_e sh_dnx_swstate_l2_routing_learning_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_routing_learning_size_get_options[];
extern sh_sand_man_t dnx_swstate_l2_routing_learning_size_get_man;


shr_error_e sh_dnx_swstate_l2_routing_learning_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_l2_routing_learning_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_l2_routing_learning_diagnostic_operation_counters_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __L2_COMMANDLINE_H__ */
