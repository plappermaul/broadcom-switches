
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#include <src/soc/dnx/dbal/dbal_init/auto_generated/dbal_init_internal_tables_hard_logic_pp.h>

shr_error_e
dbal_init_tables_hard_logic_pp_init(
int unit,
table_db_struct_t * cur_table_param,
dbal_logical_table_t * table_info)
{
    SHR_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_arr_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_bfd_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_context_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_failover_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_global_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_in_lif_profile_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_init_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_instru_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_kbp_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_kleap_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_l2_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_l3_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_mdb_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_mpls_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_oam_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_out_lif_profile_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_port_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_ptp_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_qos_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_sit_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_srv6_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_stat_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_stg_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_switch_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_trap_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_tunnel_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(dbal_init_tables_hard_logic_pp_vlan_init(unit, cur_table_param, table_info));
exit:
    SHR_FUNC_EXIT;
}
