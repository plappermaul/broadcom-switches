
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 */

shr_error_e
kleap_e_pmf_ffc_instruction_epmf_ffc_instruction_dataoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__ffc_idx;
    uint32 result_size__epmf_ffc_instruction = 0;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_FFC_IDX, key_value__ffc_idx);
    result_size__epmf_ffc_instruction += 29;
    *offset = key_value__ffc_idx*result_size__epmf_ffc_instruction;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
