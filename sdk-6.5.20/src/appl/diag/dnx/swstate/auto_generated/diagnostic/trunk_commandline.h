/** \file dnx/swstate/auto_generated/diagnostic/trunk_commandline.h
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
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef __TRUNK_COMMANDLINE_H__
#define __TRUNK_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/swstate/auto_generated/types/trunk_types.h>
#include <include/soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_trunk_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_dump_man;
shr_error_e sh_dnx_swstate_trunk_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_size_get_man;
shr_error_e sh_dnx_swstate_trunk_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_diagnostic_operation_counters_man;
extern sh_sand_cmd_t sh_dnx_swstate_trunk_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_man;
extern const char cmd_dnx_swstate_trunk_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_man;
extern const char cmd_dnx_swstate_trunk_pools_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_psc_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_psc_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_psc_profile_id_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_profile_id_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_psc_profile_id_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_profile_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_profile_id_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_profile_id_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_profile_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_profile_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_profile_id_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_psc_profile_id_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_psc_profile_id_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_psc_profile_id_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_members_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_members_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_members_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_members_system_port_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_system_port_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_members_system_port_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_members_system_port_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_system_port_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_system_port_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_system_port_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_system_port_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_system_port_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_system_port_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_system_port_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_system_port_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_members_flags_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_flags_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_members_flags_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_members_flags_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_flags_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_flags_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_flags_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_flags_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_flags_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_flags_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_flags_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_flags_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_members_index_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_index_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_members_index_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_members_index_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_index_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_index_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_index_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_index_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_index_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_members_index_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_members_index_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_members_index_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_nof_members_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_nof_members_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_nof_members_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_nof_members_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_nof_members_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_nof_members_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_nof_members_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_nof_members_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_nof_members_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_nof_members_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_nof_members_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_nof_members_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_last_member_added_index_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_last_member_added_index_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_last_member_added_index_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_last_member_added_index_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_last_member_added_index_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_last_member_added_index_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_last_member_added_index_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_last_member_added_index_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_last_member_added_index_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_last_member_added_index_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_last_member_added_index_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_last_member_added_index_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_in_use_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_use_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_in_use_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_in_use_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_use_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_use_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_in_use_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_use_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_use_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_in_use_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_use_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_use_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_pp_ports_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_pp_ports_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_pp_ports_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_pp_ports_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_pp_ports_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_pp_ports_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_pp_ports_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_pp_ports_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_pp_ports_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_pp_ports_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_pp_ports_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_pp_ports_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_in_header_type_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_header_type_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_in_header_type_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_in_header_type_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_header_type_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_header_type_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_in_header_type_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_header_type_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_header_type_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_in_header_type_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_in_header_type_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_in_header_type_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_out_header_type_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_out_header_type_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_out_header_type_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_out_header_type_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_out_header_type_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_out_header_type_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_out_header_type_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_out_header_type_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_out_header_type_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_out_header_type_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_out_header_type_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_out_header_type_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_flags_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_flags_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_flags_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_flags_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_flags_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_flags_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_flags_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_flags_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_flags_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_flags_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_flags_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_flags_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_pools_groups_master_trunk_id_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_master_trunk_id_man;
extern const char cmd_dnx_swstate_trunk_pools_groups_master_trunk_id_desc[];
shr_error_e sh_dnx_swstate_trunk_pools_groups_master_trunk_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_master_trunk_id_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_master_trunk_id_dump_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_master_trunk_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_master_trunk_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_master_trunk_id_size_get_man;


shr_error_e sh_dnx_swstate_trunk_pools_groups_master_trunk_id_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_pools_groups_master_trunk_id_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_pools_groups_master_trunk_id_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_trunk_system_port_db_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_man;
extern const char cmd_dnx_swstate_trunk_trunk_system_port_db_desc[];
shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_dump_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_size_get_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_man;
extern const char cmd_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_desc[];
shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_dump_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_size_get_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_man;
extern const char cmd_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_desc[];
shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_dump_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_size_get_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_trunk_id_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_cmds[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_man;
extern const char cmd_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_desc[];
shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_dump_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_dump_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_size_get_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_size_get_man;


shr_error_e sh_dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_trunk_trunk_system_port_db_trunk_system_port_entries_flags_diagnostic_operation_counters_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __TRUNK_COMMANDLINE_H__ */
