/** \file dnx/swstate/auto_generated/diagnostic/algo_tdm_commandline.h
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

#ifndef __ALGO_TDM_COMMANDLINE_H__
#define __ALGO_TDM_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/algo_tdm_types.h>
#include <bcm_int/dnx/algo/algo_tdm.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_algo_tdm_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_dump_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_dump_man;
shr_error_e sh_dnx_swstate_algo_tdm_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_size_get_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_size_get_man;
shr_error_e sh_dnx_swstate_algo_tdm_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_diagnostic_operation_counters_man;
extern sh_sand_cmd_t sh_dnx_swstate_algo_tdm_cmds[];
extern sh_sand_man_t dnx_swstate_algo_tdm_man;
extern const char cmd_dnx_swstate_algo_tdm_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_algo_tdm_ingress_context_cmds[];
extern sh_sand_man_t dnx_swstate_algo_tdm_ingress_context_man;
extern const char cmd_dnx_swstate_algo_tdm_ingress_context_desc[];
shr_error_e sh_dnx_swstate_algo_tdm_ingress_context_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_ingress_context_dump_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_ingress_context_dump_man;


shr_error_e sh_dnx_swstate_algo_tdm_ingress_context_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_ingress_context_size_get_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_ingress_context_size_get_man;


shr_error_e sh_dnx_swstate_algo_tdm_ingress_context_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_ingress_context_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_ingress_context_diagnostic_operation_counters_man;



extern sh_sand_cmd_t sh_dnx_swstate_algo_tdm_link_mask_cmds[];
extern sh_sand_man_t dnx_swstate_algo_tdm_link_mask_man;
extern const char cmd_dnx_swstate_algo_tdm_link_mask_desc[];
shr_error_e sh_dnx_swstate_algo_tdm_link_mask_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_link_mask_dump_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_link_mask_dump_man;


shr_error_e sh_dnx_swstate_algo_tdm_link_mask_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_link_mask_size_get_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_link_mask_size_get_man;


shr_error_e sh_dnx_swstate_algo_tdm_link_mask_diagnostic_operation_counters_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_algo_tdm_link_mask_diagnostic_operation_counters_options[];
extern sh_sand_man_t dnx_swstate_algo_tdm_link_mask_diagnostic_operation_counters_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __ALGO_TDM_COMMANDLINE_H__ */
