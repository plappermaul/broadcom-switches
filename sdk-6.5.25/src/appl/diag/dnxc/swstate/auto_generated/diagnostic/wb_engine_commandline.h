/** \file dnxc/swstate/auto_generated/diagnostic/wb_engine_commandline.h
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

#ifndef __DNXC_WB_ENGINE_COMMANDLINE_H__
#define __DNXC_WB_ENGINE_COMMANDLINE_H__

#if defined(BCM_DNX_SUPPORT) || defined(BCM_DNXF_SUPPORT)
#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/auto_generated/types/wb_engine_types.h>
#include <soc/wb_engine.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_wb_engine_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_dump_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_dump_man;
shr_error_e sh_dnx_swstate_wb_engine_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_size_get_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_size_get_man;
extern sh_sand_cmd_t sh_dnx_swstate_wb_engine_cmds[];
extern sh_sand_man_t dnx_swstate_wb_engine_man;
extern const char cmd_dnx_swstate_wb_engine_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_wb_engine_buffer_cmds[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_man;
extern const char cmd_dnx_swstate_wb_engine_buffer_desc[];
shr_error_e sh_dnx_swstate_wb_engine_buffer_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_buffer_dump_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_dump_man;


shr_error_e sh_dnx_swstate_wb_engine_buffer_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_buffer_size_get_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_wb_engine_buffer_instance_cmds[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_instance_man;
extern const char cmd_dnx_swstate_wb_engine_buffer_instance_desc[];
shr_error_e sh_dnx_swstate_wb_engine_buffer_instance_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_buffer_instance_dump_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_instance_dump_man;


shr_error_e sh_dnx_swstate_wb_engine_buffer_instance_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_wb_engine_buffer_instance_size_get_options[];
extern sh_sand_man_t dnx_swstate_wb_engine_buffer_instance_size_get_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#endif /* defined(BCM_DNX_SUPPORT) || defined(BCM_DNXF_SUPPORT)*/ 

#endif /* __DNXC_WB_ENGINE_COMMANDLINE_H__ */