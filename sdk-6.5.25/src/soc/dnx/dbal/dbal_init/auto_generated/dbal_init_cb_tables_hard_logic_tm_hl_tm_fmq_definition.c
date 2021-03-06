
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * 
 */

shr_error_e
fmq_enhanced_mode_config_is_hr_id_valid_dataoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fmq_enhanced_port_idx;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FMQ_ENHANCED_PORT_IDX, key_value__fmq_enhanced_port_idx);
    *offset = key_value__fmq_enhanced_port_idx*16+12;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
fmq_enhanced_mode_config_is_hr_id_valid_dataoffset_1_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fmq_enhanced_port_idx;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FMQ_ENHANCED_PORT_IDX, key_value__fmq_enhanced_port_idx);
    *offset = key_value__fmq_enhanced_port_idx*16-32+12;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
fmq_enhanced_mode_config_hr_id_dataoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fmq_enhanced_port_idx;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FMQ_ENHANCED_PORT_IDX, key_value__fmq_enhanced_port_idx);
    *offset = key_value__fmq_enhanced_port_idx*16;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
fmq_enhanced_mode_config_hr_id_dataoffset_1_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__fmq_enhanced_port_idx;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FMQ_ENHANCED_PORT_IDX, key_value__fmq_enhanced_port_idx);
    *offset = key_value__fmq_enhanced_port_idx*16-32;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
