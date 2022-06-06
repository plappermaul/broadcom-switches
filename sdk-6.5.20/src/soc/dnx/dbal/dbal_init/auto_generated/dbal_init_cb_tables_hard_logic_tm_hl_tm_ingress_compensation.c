
/*
 * 
 This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 
 Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <src/soc/dnx/dbal/dbal_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data.h>

shr_error_e
ingress_compensation_voq_to_credit_class_profile_map_compensation_profile_entryoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__voq;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_VOQ, key_value__voq);
    *offset = key_value__voq/4;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
ingress_compensation_voq_to_credit_class_profile_map_compensation_profile_dataoffset_0_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__voq;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_VOQ, key_value__voq);
    *offset = key_value__voq%4*13+6;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
shr_error_e
ingress_compensation_voq_to_credit_class_profile_map_compensation_profile_dataoffset_1_cb(
int unit,
void * entry_handle,
dbal_fields_e current_mapped_field_id,
uint32 * offset)
{
    dbal_entry_handle_t *  eh = (dbal_entry_handle_t *) entry_handle;
    uint32 key_value__voq;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FORMULA_CB_GET_KEY_VALUE(unit, eh, DBAL_FIELD_VOQ, key_value__voq);
    *offset = key_value__voq%4*11+6;
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}
