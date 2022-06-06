/** \file dnx/swstate/auto_generated/diagnostic/dnx_algo_lane_map_commandline.h
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
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifndef __DNX_ALGO_LANE_MAP_COMMANDLINE_H__
#define __DNX_ALGO_LANE_MAP_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_algo_lane_map_types.h>
#include <soc/dnxc/swstate/auto_generated/types/dnxc_types.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_dump_man;
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_size_get_man;
extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_map_size_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_map_size_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_map_size_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_map_size_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_map_size_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_map_size_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_map_size_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_map_size_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_map_size_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_rx_id_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_lane2serdes_tx_id_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_rx_id_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_man;
extern const char cmd_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_desc[];
shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_dump_man;


shr_error_e sh_dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_algo_lane_map_lane_map_serdes2lane_tx_id_size_get_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __DNX_ALGO_LANE_MAP_COMMANDLINE_H__ */