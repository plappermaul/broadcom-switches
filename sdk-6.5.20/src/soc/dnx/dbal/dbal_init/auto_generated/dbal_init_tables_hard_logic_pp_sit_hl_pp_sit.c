
/*
 * 
 This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 
 Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <src/soc/dnx/dbal/dbal_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data.h>

shr_error_e
_dbal_init_tables_hard_logic_pp_sit_hl_pp_sit_sit_egress_profile_table_init(
int unit,
table_db_struct_t * cur_table_param,
dbal_logical_table_t * table_info)
{
    dbal_logical_table_t * table_entry = table_info + DBAL_TABLE_SIT_EGRESS_PROFILE_TABLE;
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_table_db_struct_clear(cur_table_param);
    SHR_IF_ERR_EXIT(dbal_db_init_general_info_set(unit, table_entry, DBAL_TABLE_SIT_EGRESS_PROFILE_TABLE , is_valid , "SIT_EGRESS_PROFILE_TABLE" , DBAL_ACCESS_METHOD_HARD_LOGIC ));
    
    
    table_entry->maturity_level = DBAL_MATURITY_HIGH;
    
    
    
    
    table_entry->table_type = DBAL_TABLE_TYPE_DIRECT;
    
    
    table_entry->nof_labels = 1;
    SHR_ALLOC_SET_ZERO(table_entry->table_labels, table_entry->nof_labels * sizeof(dbal_labels_e), "table labels allocation", "%s%s%s\r\n", table_entry->table_name, EMPTY, EMPTY);
    table_entry->table_labels[0] = DBAL_LABEL_SIT;
    
    {
        int field_index = 0;
        dbal_table_field_info_t * dbal_db_field;
        table_db_field_params_struct_t db_field;
        SHR_ALLOC_SET_ZERO(table_entry->keys_info, 1 * sizeof(dbal_table_field_info_t), "key fields info allocation", "%s%s%s\r\n", "SIT_EGRESS_PROFILE_TABLE" , EMPTY, EMPTY);
        
        {
            dbal_db_field = &table_entry->keys_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SIT_PROFILE , DBAL_FIELD_TYPE_DEF_SIT_PROFILE , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 3;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, TRUE));
            field_index += dbal_db_field->nof_instances;
        }
        table_entry->nof_key_fields = field_index;
        dbal_db_init_table_calculate_key_size(table_entry);
        dbal_db_init_table_and_db_fields_set_allocator_fields(unit, table_entry, 1);
    }
    
    {
        int field_index = 0;
        int result_type_counter = 0;
        dbal_table_field_info_t * dbal_db_field;
        table_db_field_params_struct_t db_field;
        DBAL_DB_INIT_TABLE_NOF_RESULT_TYPES_SET(table_entry, 1);
        SHR_IF_ERR_EXIT(dbal_db_init_table_db_interface_results_alloc(unit, table_entry));
        
        DBAL_DB_INIT_TABLE_NOF_RESULT_FIELDS_SET(table_entry, (table_entry->multi_res_info[result_type_counter]), 4);
        DBAL_DB_INIT_TABLE_INTERFACE_RESULT_FIELDS_ALLOC(table_entry, table_entry->multi_res_info[result_type_counter]);
        
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SIT_TAG_TYPE , DBAL_FIELD_TYPE_DEF_SIT_TAG_TYPE , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SIT_TPID , DBAL_FIELD_TYPE_DEF_TPID , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 16;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SIT_PCP_DEI_SRC , DBAL_FIELD_TYPE_DEF_SIT_PCP_DEI_SRC , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SIT_VID_SRC , DBAL_FIELD_TYPE_DEF_SIT_VID_SRC , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
    }
    
    
    
    
    dbal_db_init_table_db_struct_access_clear(cur_table_param, table_entry);
    
    {
        
        uint8 is_standard_1 = FALSE;
        
        uint8 is_compatible_with_all_images = TRUE;
        DBAL_DB_INIT_TABLE_IMAGE_SPECIFIC_TABLE_INDICATION_SET(table_entry, is_standard_1, is_compatible_with_all_images);
        SHR_IF_ERR_EXIT(dbal_db_init_table_set_table_incompatible_image_sw_state_indication(unit, DBAL_TABLE_SIT_EGRESS_PROFILE_TABLE, is_standard_1, is_compatible_with_all_images));
    }
    
    
    table_entry->core_mode = DBAL_CORE_MODE_SBC;
    
    dbal_db_init_table_add_result_type_physical_values_by_result_type_index(table_entry);
    
    {
        int map_idx = 0;
        int access_counter;
        access_counter = 0;
        sal_strncpy(cur_table_param->hl_access[map_idx].mapping_result_name, EMPTY , sizeof(cur_table_param->hl_access[map_idx].mapping_result_name));
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, FALSE , TRUE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SIT_TAG_TYPE + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDm;
                    int fieldHwEntityId = INVALIDf;
                    
                    
                    fieldHwEntityId = TAG_TYPEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_ETPP_SIT_PROFILE_MAP_TABLE , regMemHwEntityId , fieldHwEntityId , INVALIDm ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, FALSE , TRUE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SIT_TPID + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDm;
                    int fieldHwEntityId = INVALIDf;
                    
                    
                    fieldHwEntityId = TPIDf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_ETPP_SIT_PROFILE_MAP_TABLE , regMemHwEntityId , fieldHwEntityId , INVALIDm ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, FALSE , TRUE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SIT_PCP_DEI_SRC + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDm;
                    int fieldHwEntityId = INVALIDf;
                    
                    
                    fieldHwEntityId = PCP_DEI_SOURCEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_ETPP_SIT_PROFILE_MAP_TABLE , regMemHwEntityId , fieldHwEntityId , INVALIDm ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, FALSE , TRUE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SIT_VID_SRC + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDm;
                    int fieldHwEntityId = INVALIDf;
                    
                    
                    fieldHwEntityId = VID_SOURCEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_ETPP_SIT_PROFILE_MAP_TABLE , regMemHwEntityId , fieldHwEntityId , INVALIDm ));
                }
            }
        }
        
        cur_table_param->hl_access[map_idx].nof_access = access_counter;
        
        map_idx++;
    }
    SHR_IF_ERR_EXIT(dbal_db_init_table_add(unit, cur_table_param, DBAL_TABLE_SIT_EGRESS_PROFILE_TABLE, table_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_tables_hard_logic_pp_sit_hl_pp_sit_sit_ingress_general_configuration_init(
int unit,
table_db_struct_t * cur_table_param,
dbal_logical_table_t * table_info)
{
    dbal_logical_table_t * table_entry = table_info + DBAL_TABLE_SIT_INGRESS_GENERAL_CONFIGURATION;
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_table_db_struct_clear(cur_table_param);
    SHR_IF_ERR_EXIT(dbal_db_init_general_info_set(unit, table_entry, DBAL_TABLE_SIT_INGRESS_GENERAL_CONFIGURATION , is_valid , "SIT_INGRESS_GENERAL_CONFIGURATION" , DBAL_ACCESS_METHOD_HARD_LOGIC ));
    
    
    table_entry->maturity_level = DBAL_MATURITY_HIGH;
    
    
    
    
    table_entry->table_type = DBAL_TABLE_TYPE_DIRECT;
    
    
    table_entry->nof_labels = 1;
    SHR_ALLOC_SET_ZERO(table_entry->table_labels, table_entry->nof_labels * sizeof(dbal_labels_e), "table labels allocation", "%s%s%s\r\n", table_entry->table_name, EMPTY, EMPTY);
    table_entry->table_labels[0] = DBAL_LABEL_SIT;
    
    
    {
        int field_index = 0;
        int result_type_counter = 0;
        dbal_table_field_info_t * dbal_db_field;
        table_db_field_params_struct_t db_field;
        DBAL_DB_INIT_TABLE_NOF_RESULT_TYPES_SET(table_entry, 1);
        SHR_IF_ERR_EXIT(dbal_db_init_table_db_interface_results_alloc(unit, table_entry));
        
        DBAL_DB_INIT_TABLE_NOF_RESULT_FIELDS_SET(table_entry, (table_entry->multi_res_info[result_type_counter]), 2 + 2 + 2 + 2 + 2);
        DBAL_DB_INIT_TABLE_INTERFACE_RESULT_FIELDS_ALLOC(table_entry, table_entry->multi_res_info[result_type_counter]);
        
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SWAP_TPID_OFFSET , DBAL_FIELD_TYPE_DEF_UINT , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 5;
            db_field.nof_instances = 2;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SWAP_TPID , DBAL_FIELD_TYPE_DEF_TPID , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 16;
            db_field.nof_instances = 2;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SWAP_TPID_MASK , DBAL_FIELD_TYPE_DEF_UINT , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 16;
            db_field.nof_instances = 2;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_GLOBAL_SWAP_TPID_OFFSET , DBAL_FIELD_TYPE_DEF_UINT , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 5;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_GLOBAL_SWAP_TPID_SIZE , DBAL_FIELD_TYPE_DEF_SIT_TAG_SWAP_SIZE , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 2;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
        {
            dbal_db_field = &table_entry->multi_res_info[result_type_counter].results_info[field_index];
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_mandatory_values(unit, DBAL_FIELD_SWAP_TPID_SIZE , DBAL_FIELD_TYPE_DEF_SIT_TAG_SWAP_SIZE , dbal_db_field));
            dbal_db_init_table_field_params_init(&db_field);
            db_field.size = 2;
            db_field.nof_instances = 2;
            SHR_IF_ERR_EXIT(dbal_db_init_table_field_set_optional_or_default_values(unit, table_entry, dbal_db_field, &db_field, FALSE));
            field_index += dbal_db_field->nof_instances;
        }
    }
    
    
    
    
    dbal_db_init_table_db_struct_access_clear(cur_table_param, table_entry);
    
    {
        
        uint8 is_standard_1 = FALSE;
        
        uint8 is_compatible_with_all_images = TRUE;
        DBAL_DB_INIT_TABLE_IMAGE_SPECIFIC_TABLE_INDICATION_SET(table_entry, is_standard_1, is_compatible_with_all_images);
        SHR_IF_ERR_EXIT(dbal_db_init_table_set_table_incompatible_image_sw_state_indication(unit, DBAL_TABLE_SIT_INGRESS_GENERAL_CONFIGURATION, is_standard_1, is_compatible_with_all_images));
    }
    
    
    table_entry->core_mode = DBAL_CORE_MODE_SBC;
    
    dbal_db_init_table_add_result_type_physical_values_by_result_type_index(table_entry);
    
    {
        int map_idx = 0;
        int access_counter;
        access_counter = 0;
        sal_strncpy(cur_table_param->hl_access[map_idx].mapping_result_name, EMPTY , sizeof(cur_table_param->hl_access[map_idx].mapping_result_name));
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 0;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_OFFSET + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TAG_SWAP_OFFSET_0f;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 1;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_OFFSET + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TAG_SWAP_OFFSET_1f;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 0;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TAG_SWAP_TPID_0f;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 1;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TAG_SWAP_TPID_1f;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 0;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_MASK + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TPID_0_TAG_SWAP_MASKf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 1;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_MASK + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TPID_1_TAG_SWAP_MASKf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 0;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_SIZE + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TPID_0_TAG_SWAP_SIZEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                instance_index = 1;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_SWAP_TPID_SIZE + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = TPID_1_TAG_SWAP_SIZEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_GLOBAL_SWAP_TPID_OFFSET + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = GLOBAL_TAG_SWAP_OFFSETf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        {
            table_db_access_params_struct_t * access_params;
            
            {
                int instance_index = 0;
                access_params = &cur_table_param->hl_access[map_idx].access[access_counter];
                dbal_db_init_table_db_struct_hl_access_params_clear(access_params);
                access_counter++;
                
                SHR_IF_ERR_EXIT(dbal_db_init_hard_logic_direct_set_access_type(unit, access_params, TRUE , FALSE ););
                
                
                access_params->access_field_id = DBAL_FIELD_GLOBAL_SWAP_TPID_SIZE + instance_index;
                SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_field(unit, access_params, 0 , 0 , DBAL_VALUE_FIELD_ENCODE_NONE , 0 ));
                
                {
                    int regMemHwEntityId = INVALIDr;
                    int fieldHwEntityId = INVALIDf;
                    
                    regMemHwEntityId = IRE_TAG_SWAP_CONFIGURATIONr;
                    
                    fieldHwEntityId = GLOBAL_TAG_SWAP_SIZEf;
                    SHR_IF_ERR_EXIT(dbal_db_init_table_set_access_register_memory(unit, access_params, DBAL_HW_ENTITY_GROUP_EMPTY , regMemHwEntityId , fieldHwEntityId , INVALIDr ));
                }
            }
        }
        
        cur_table_param->hl_access[map_idx].nof_access = access_counter;
        
        map_idx++;
    }
    SHR_IF_ERR_EXIT(dbal_db_init_table_add(unit, cur_table_param, DBAL_TABLE_SIT_INGRESS_GENERAL_CONFIGURATION, table_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dbal_init_tables_hard_logic_pp_sit_hl_pp_sit_init(
int unit,
table_db_struct_t * cur_table_param,
dbal_logical_table_t * table_info)
{
    SHR_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(_dbal_init_tables_hard_logic_pp_sit_hl_pp_sit_sit_egress_profile_table_init(unit, cur_table_param, table_info));
    SHR_IF_ERR_EXIT(_dbal_init_tables_hard_logic_pp_sit_hl_pp_sit_sit_ingress_general_configuration_init(unit, cur_table_param, table_info));
exit:
    SHR_FUNC_EXIT;
}
