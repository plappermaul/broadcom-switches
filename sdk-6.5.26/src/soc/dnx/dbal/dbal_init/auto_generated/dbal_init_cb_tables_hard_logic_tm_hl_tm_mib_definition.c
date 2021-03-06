
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 */

shr_error_e
fabric_stat_ctrl_lane_select_blockindex_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fmac_id;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FMAC_ID, key_value__fmac_id);
    *offset = key_value__fmac_id;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part1_r64_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit);
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part1_r64_blockindex_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__pm_index;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_PM_INDEX, key_value__pm_index);
    *offset = key_value__pm_index;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part1_r4095_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+1;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part1_rpkt_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+2;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part1_rfcs_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+3;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part2_rxuo_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+4;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part2_rpfc2_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+5;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part2_rpfc6_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+6;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_rx_part2_rbyt_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+12;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_tx_part1_t64_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+7;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_tx_part1_t4095_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+8;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_tx_part1_tpfc2_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+9;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_tx_part2_tpfc6_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+10;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_tx_part2_tmca_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+11;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part1_preempt_r64_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+13;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part1_preempt_r4095_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+14;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part1_preempt_rfcs_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+15;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part2_preempt_rxuo_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+16;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part2_preempt_rmdis_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+17;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_rx_part2_preempt_rbyt_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+24;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_tx_part1_preempt_t64_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+19;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_tx_part1_preempt_t4095_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+20;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_tx_part2_preempt_tpkt_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+21;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_tx_part2_preempt_tmca_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+22;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_preempt_counters_tx_part2_preempt_trpkt_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+23;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_cdmac_mib_counters_row16_18_cdmac_mib_row18_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__lane;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LANE, key_value__lane);
    *offset = key_value__lane*dnx_data_mib.general.nof_rows_per_cdmib_port_get(unit)+18;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_clmac_mib_counters_common_rx_part1_r64_blockindex_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__clu_logical_port;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_CLU_LOGICAL_PORT, key_value__clu_logical_port);
    *offset = key_value__clu_logical_port;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_ilkn_rx_stats_acc_logical_port_type_blockindex_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__logical_port;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_LOGICAL_PORT, key_value__logical_port);
    *offset = key_value__logical_port;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
nif_ilkn_elk_fec_counters_rx_fec_corrected_cnt_blockindex_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fec_unit;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FEC_UNIT, key_value__fec_unit);
    *offset = key_value__fec_unit/4;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
