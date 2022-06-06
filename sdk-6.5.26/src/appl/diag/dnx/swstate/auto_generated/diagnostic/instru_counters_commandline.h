/** \file dnx/swstate/auto_generated/diagnostic/instru_counters_commandline.h
 *
 * sw state functions declarations
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

#ifndef __DNX_INSTRU_COUNTERS_COMMANDLINE_H__
#define __DNX_INSTRU_COUNTERS_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/swstate/auto_generated/types/instru_counters_types.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_instru_counters_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_dump_man;
shr_error_e sh_dnx_swstate_instru_counters_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_size_get_man;
extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_man;
extern const char cmd_dnx_swstate_instru_counters_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_icgm_countes_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_man;
extern const char cmd_dnx_swstate_instru_counters_icgm_countes_desc[];
shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_icgm_countes_sync_counters_started_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_sync_counters_started_man;
extern const char cmd_dnx_swstate_instru_counters_icgm_countes_sync_counters_started_desc[];
shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_sync_counters_started_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_sync_counters_started_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_sync_counters_started_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_sync_counters_started_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_sync_counters_started_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_sync_counters_started_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_man;
extern const char cmd_dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_desc[];
shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_tod_1588_timestamp_start_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_man;
extern const char cmd_dnx_swstate_instru_counters_icgm_countes_per_core_info_desc[];
shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_per_core_info_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_per_core_info_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_man;
extern const char cmd_dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_desc[];
shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_icgm_countes_per_core_info_record_last_read_nof_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_nif_countes_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_man;
extern const char cmd_dnx_swstate_instru_counters_nif_countes_desc[];
shr_error_e sh_dnx_swstate_instru_counters_nif_countes_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_nif_countes_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_nif_countes_sync_counters_started_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_sync_counters_started_man;
extern const char cmd_dnx_swstate_instru_counters_nif_countes_sync_counters_started_desc[];
shr_error_e sh_dnx_swstate_instru_counters_nif_countes_sync_counters_started_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_sync_counters_started_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_sync_counters_started_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_nif_countes_sync_counters_started_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_sync_counters_started_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_sync_counters_started_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_man;
extern const char cmd_dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_desc[];
shr_error_e sh_dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_tod_1588_timestamp_start_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_man;
extern const char cmd_dnx_swstate_instru_counters_nif_countes_per_ethu_info_desc[];
shr_error_e sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_cmds[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_man;
extern const char cmd_dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_desc[];
shr_error_e sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_dump_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_dump_man;


shr_error_e sh_dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_size_get_options[];
extern sh_sand_man_t dnx_swstate_instru_counters_nif_countes_per_ethu_info_record_last_read_nof_size_get_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __DNX_INSTRU_COUNTERS_COMMANDLINE_H__ */