/** \file dnx/swstate/auto_generated/diagnostic/lif_table_mngr_commandline.h
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

#ifndef __LIF_TABLE_MNGR_COMMANDLINE_H__
#define __LIF_TABLE_MNGR_COMMANDLINE_H__

#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_utils.h>
#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/lif_table_mngr_types.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <soc/dnxc/swstate/callbacks/sw_state_htb_callbacks.h>
#include <soc/dnxc/swstate/types/sw_state_hash_table.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
shr_error_e sh_dnx_swstate_lif_table_mngr_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_dump_man;
shr_error_e sh_dnx_swstate_lif_table_mngr_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_size_get_man;
extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_man;
extern const char cmd_dnx_swstate_lif_table_mngr_desc[];

extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_lif_htb_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_egress_virtual_htb_size_get_man;


#ifdef BCM_DNX2_SUPPORT

extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_size_get_man;

#endif /*BCM_DNX2_SUPPORT*/

#ifdef BCM_DNX2_SUPPORT

extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif1_valid_fields_size_get_man;

#endif /*BCM_DNX2_SUPPORT*/

#ifdef BCM_DNX2_SUPPORT

extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_size_get_man;

#endif /*BCM_DNX2_SUPPORT*/

#ifdef BCM_DNX2_SUPPORT

extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_man;
extern const char cmd_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_valid_fields_per_ingress_lif2_valid_fields_size_get_man;

#endif /*BCM_DNX2_SUPPORT*/




extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_lif_table_info_htb_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_lif_table_info_htb_man;
extern const char cmd_dnx_swstate_lif_table_mngr_lif_table_info_htb_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_lif_table_info_htb_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_lif_table_info_htb_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_lif_table_info_htb_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_lif_table_info_htb_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_lif_table_info_htb_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_lif_table_info_htb_size_get_man;



extern sh_sand_cmd_t sh_dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_cmds[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_man;
extern const char cmd_dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_desc[];
shr_error_e sh_dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_dump_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_dump_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_dump_man;


shr_error_e sh_dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_size_get_cmd(int unit, args_t *args, sh_sand_control_t *sand_control);
extern sh_sand_option_t dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_size_get_options[];
extern sh_sand_man_t dnx_swstate_lif_table_mngr_flow_egress_virtual_gport_id_size_get_man;


#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __LIF_TABLE_MNGR_COMMANDLINE_H__ */