/** \file dnx/swstate/auto_generated/diagnostic/dnx_stk_sys_commandline.h
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

#ifndef __DNX_STK_SYS_COMMANDLINE_H__
#define __DNX_STK_SYS_COMMANDLINE_H__

#ifdef BCM_DNX_SUPPORT
#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/swstate/auto_generated/types/dnx_stk_sys_types.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_dnx_stk_sys_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_dump_man;
shr_error_e sh_dnx_swstate_dnx_stk_sys_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_size_get_man;
extern sh_sand_cmd_t sh_dnx_swstate_dnx_stk_sys_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_man;
extern const char cmd_dnx_swstate_dnx_stk_sys_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_dnx_stk_sys_nof_fap_ids_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_nof_fap_ids_man;
extern const char cmd_dnx_swstate_dnx_stk_sys_nof_fap_ids_desc[];
shr_error_e sh_dnx_swstate_dnx_stk_sys_nof_fap_ids_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_nof_fap_ids_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_nof_fap_ids_dump_man;


shr_error_e sh_dnx_swstate_dnx_stk_sys_nof_fap_ids_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_nof_fap_ids_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_nof_fap_ids_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_dnx_stk_sys_module_enable_done_cmds[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_module_enable_done_man;
extern const char cmd_dnx_swstate_dnx_stk_sys_module_enable_done_desc[];
shr_error_e sh_dnx_swstate_dnx_stk_sys_module_enable_done_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_module_enable_done_dump_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_module_enable_done_dump_man;


shr_error_e sh_dnx_swstate_dnx_stk_sys_module_enable_done_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_dnx_stk_sys_module_enable_done_size_get_options[];
extern sh_sand_man_t dnx_swstate_dnx_stk_sys_module_enable_done_size_get_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */
#endif /* BCM_DNX_SUPPORT*/ 

#endif /* __DNX_STK_SYS_COMMANDLINE_H__ */
