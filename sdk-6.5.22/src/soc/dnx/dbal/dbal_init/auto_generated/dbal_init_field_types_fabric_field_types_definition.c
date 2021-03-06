
/*
 * 
 This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 
 Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_pipe_id_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PIPE_ID];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PIPE_ID" , 2 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.pipes.max_nof_pipes_get(unit)-1;
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_context_id_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_CONTEXT_ID];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_CONTEXT_ID" , 2 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.pipes.max_nof_contexts_get(unit)-1;
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_subcontext_id_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_SUBCONTEXT_ID];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_SUBCONTEXT_ID" , 3 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.pipes.max_nof_subcontexts_get(unit)-1;
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_link_id_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_LINK_ID];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_LINK_ID" , dnx_data_fabric.dbal.nof_links_bits_nof_get(unit) , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.links.max_link_id_get(unit);
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_priority_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PRIORITY];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PRIORITY" , dnx_data_fabric.dbal.priority_bits_nof_get(unit) , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.cell.nof_priorities_get(unit)-1;
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_fc_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_DTQ_FC];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_DTQ_FC" , 2 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_DTQ_FC_RDF + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_SHAPER].name_from_interface, "SHAPER", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_SHAPER].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_LP].name_from_interface, "LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_LP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_HP].name_from_interface, "HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_RDF].name_from_interface, "RDF", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_RDF].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_SHAPER].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_LP].value_from_mapping = 1;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_HP].value_from_mapping = 2;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_FC_RDF].value_from_mapping = 3;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_ocb_fc_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_DTQ_OCB_FC];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_DTQ_OCB_FC" , 1 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_PDQ + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_SHAPER].name_from_interface, "SHAPER", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_SHAPER].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_PDQ].name_from_interface, "PDQ", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_PDQ].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_SHAPER].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_OCB_FC_PDQ].value_from_mapping = 1;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_mode_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_DTQ_MODE];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_DTQ_MODE" , 2 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_HPMC_LPMC + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_SINGLE_QUEUE].name_from_interface, "SINGLE_QUEUE", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_SINGLE_QUEUE].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_MC].name_from_interface, "UC_MC", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_MC].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_HPMC_LPMC].name_from_interface, "UC_HPMC_LPMC", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_HPMC_LPMC].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_SINGLE_QUEUE].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_MC].value_from_mapping = 1;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_MODE_UC_HPMC_LPMC].value_from_mapping = 2;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_global_shaper_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_DTQ_GLOBAL_SHAPER];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_DTQ_GLOBAL_SHAPER" , 3 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ0 + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FMC_DEV3].name_from_interface, "FMC_DEV3", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FMC_DEV3].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FUC_DEV2].name_from_interface, "FUC_DEV2", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FUC_DEV2].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_TOTAL_DEV1].name_from_interface, "TOTAL_DEV1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_TOTAL_DEV1].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ1].name_from_interface, "EGQ1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ1].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ0].name_from_interface, "EGQ0", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ0].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FMC_DEV3].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_FUC_DEV2].value_from_mapping = 1;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_TOTAL_DEV1].value_from_mapping = 2;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ1].value_from_mapping = 3;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_DTQ_GLOBAL_SHAPER_EGQ0].value_from_mapping = 4;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_context_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PDQ_CONTEXT];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PDQ_CONTEXT" , 4 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_HP].name_from_interface, "LOCAL_0_HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_LP].name_from_interface, "LOCAL_0_LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_LP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_HP].name_from_interface, "LOCAL_1_HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_LP].name_from_interface, "LOCAL_1_LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_LP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_HP].name_from_interface, "FUC_DEV1_HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_LP].name_from_interface, "FUC_DEV1_LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_LP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_HP].name_from_interface, "DEV2_HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_LP].name_from_interface, "DEV2_LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_LP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_HP].name_from_interface, "FMC_DEV3_HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP].name_from_interface, "FMC_DEV3_LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP].name_from_interface));
    }
    if (DBAL_IS_J2C_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_HP].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_LP].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_HP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_LP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_HP].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_LP].value_from_mapping = 3;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_HP].value_from_mapping = 4;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_LP].value_from_mapping = 5;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_HP].value_from_mapping = 6;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP].value_from_mapping = 7;
        }
    }
    else if (DBAL_IS_JR2_A0 || DBAL_IS_JR2_B0 || DBAL_IS_J2P_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_HP].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_LP].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_HP].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_LP].value_from_mapping = 3;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_HP].value_from_mapping = 4;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_LP].value_from_mapping = 5;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_HP].value_from_mapping = 6;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_LP].value_from_mapping = 7;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_HP].value_from_mapping = 8;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP].value_from_mapping = 9;
        }
    }
    else if (DBAL_IS_Q2A_A0 || DBAL_IS_Q2A_B0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_HP].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_0_LP].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_HP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_LOCAL_1_LP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_HP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FUC_DEV1_LP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_HP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_DEV2_LP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_HP].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_CONTEXT_FMC_DEV3_LP].is_invalid_value_from_mapping = TRUE;
        }
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_ocb_context_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PDQ_OCB_CONTEXT];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PDQ_OCB_CONTEXT" , 3 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3 + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_0].name_from_interface, "LOCAL_0", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_0].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_1].name_from_interface, "LOCAL_1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_1].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FUC_DEV1].name_from_interface, "FUC_DEV1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FUC_DEV1].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_DEV2].name_from_interface, "DEV2", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_DEV2].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3].name_from_interface, "FMC_DEV3", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3].name_from_interface));
    }
    if (DBAL_IS_J2C_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_0].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_1].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FUC_DEV1].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_DEV2].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3].value_from_mapping = 3;
        }
    }
    else if (DBAL_IS_JR2_A0 || DBAL_IS_JR2_B0 || DBAL_IS_J2P_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_0].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_1].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FUC_DEV1].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_DEV2].value_from_mapping = 3;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3].value_from_mapping = 4;
        }
    }
    else if (DBAL_IS_Q2A_A0 || DBAL_IS_Q2A_B0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_0].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_LOCAL_1].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FUC_DEV1].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_DEV2].is_invalid_value_from_mapping = TRUE;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_OCB_CONTEXT_FMC_DEV3].is_invalid_value_from_mapping = TRUE;
        }
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_s2d_context_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PDQ_S2D_CONTEXT];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PDQ_S2D_CONTEXT" , 1 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_LP + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_HP].name_from_interface, "HP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_HP].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_LP].name_from_interface, "LP", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_LP].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_HP].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_PDQ_S2D_CONTEXT_LP].value_from_mapping = 1;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_pcp_mode_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_PCP_MODE];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_PCP_MODE" , 2 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_PCP_MODE_CONTINUOUS + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_NO_PACKING].name_from_interface, "NO_PACKING", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_NO_PACKING].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_SIMPLE].name_from_interface, "SIMPLE", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_SIMPLE].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_CONTINUOUS].name_from_interface, "CONTINUOUS", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_CONTINUOUS].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_NO_PACKING].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_SIMPLE].value_from_mapping = 1;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_PCP_MODE_CONTINUOUS].value_from_mapping = 2;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_wfq_fifo_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_WFQ_FIFO];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_WFQ_FIFO" , 3 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_MESH_MC + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_0].name_from_interface, "FABRIC_PIPE_0", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_0].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_1].name_from_interface, "FABRIC_PIPE_1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_1].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_2].name_from_interface, "FABRIC_PIPE_2", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_2].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_LOCAL_UC].name_from_interface, "LOCAL_UC", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_LOCAL_UC].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_MESH_MC].name_from_interface, "MESH_MC", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_MESH_MC].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_0].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_1].value_from_mapping = 1;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_FABRIC_PIPE_2].value_from_mapping = 2;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_LOCAL_UC].value_from_mapping = 3;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_WFQ_FIFO_MESH_MC].value_from_mapping = 4;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_load_balancing_count_mode_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_LOAD_BALANCING_COUNT_MODE];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_LOAD_BALANCING_COUNT_MODE" , 1 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_CLOCKS + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_TRANSACTIONS].name_from_interface, "TRANSACTIONS", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_TRANSACTIONS].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_CLOCKS].name_from_interface, "CLOCKS", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_CLOCKS].name_from_interface));
    }
    
    
    
    
    cur_field_type_param->nof_illegal_value = 0;
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    cur_field_type_param->max_value = DBAL_DB_INVALID;
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = 0;
    cur_field_type_param->default_val_valid = FALSE;
    
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_TRANSACTIONS].value_from_mapping = 0;
        
        enum_info[DBAL_ENUM_FVAL_FABRIC_LOAD_BALANCING_COUNT_MODE_CLOCKS].value_from_mapping = 1;
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_load_balancing_context_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_LOAD_BALANCING_CONTEXT];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_LOAD_BALANCING_CONTEXT" , 4 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.links.load_balancing_profile_id_max_get(unit);
    
    cur_field_type_param->min_value = dnx_data_fabric.links.load_balancing_profile_id_min_get(unit);
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_force_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_FORCE];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_FORCE" , 3 , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_ENUM , 0 , 1 , FALSE ));
    
    
    
    
    cur_field_type_param->nof_enum_vals = DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_1 + 1;
    
    sal_memset(cur_field_type_param->enums, 0x0, cur_field_type_param->nof_enum_vals * sizeof(dbal_db_enum_info_struct_t));
    {
        dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_DISABLE].name_from_interface, "DISABLE", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_DISABLE].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_FABRIC].name_from_interface, "FABRIC", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_FABRIC].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_0].name_from_interface, "CORE_0", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_0].name_from_interface));
        sal_strncpy(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_1].name_from_interface, "CORE_1", sizeof(enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_1].name_from_interface));
    }
    if (DBAL_IS_J2C_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_DISABLE].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_FABRIC].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_0].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_1].is_invalid_value_from_mapping = TRUE;
        }
    }
    else if (DBAL_IS_JR2_A0 || DBAL_IS_JR2_B0 || DBAL_IS_J2P_A0)
    {
        
        
        
        
        cur_field_type_param->nof_illegal_value = 0;
        cur_field_type_param->min_value = DBAL_DB_INVALID;
        cur_field_type_param->max_value = DBAL_DB_INVALID;
        cur_field_type_param->const_value = DBAL_DB_INVALID;
        cur_field_type_param->default_val = 0;
        cur_field_type_param->default_val_valid = FALSE;
        
        {
            dbal_db_enum_info_struct_t * enum_info = cur_field_type_param->enums;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_DISABLE].value_from_mapping = 0;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_FABRIC].value_from_mapping = 1;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_0].value_from_mapping = 2;
            
            enum_info[DBAL_ENUM_FVAL_FABRIC_FORCE_CORE_1].value_from_mapping = 4;
        }
    }
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
_dbal_init_field_types_fabric_field_types_definition_fabric_sr_cell_instance_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    dbal_field_types_basic_info_t * cur_field_types_info = field_types_info[DBAL_FIELD_TYPE_DEF_FABRIC_SR_CELL_INSTANCE];
    int is_valid = TRUE;
    SHR_FUNC_INIT_VARS(unit);
    dbal_init_field_type_db_struct_clear(unit, cur_field_type_param);
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_general_info_set(unit, cur_field_type_param, cur_field_types_info, "FABRIC_SR_CELL_INSTANCE" , utilex_log2_round_up(dnx_data_fabric.cell.sr_cell_nof_instances_get(unit)*dnx_data_device.general.nof_cores_get(unit)) , is_valid , FALSE , DBAL_FIELD_PRINT_TYPE_UINT32 , 0 , 1 , FALSE ));
    
    
    
    
    
    
    
    
    cur_field_type_param->max_value = dnx_data_fabric.cell.sr_cell_nof_instances_get(unit)*dnx_data_device.general.nof_cores_get(unit)-1;
    
    cur_field_type_param->min_value = DBAL_DB_INVALID;
    
    cur_field_type_param->const_value = DBAL_DB_INVALID;
    cur_field_type_param->default_val = DBAL_DB_INVALID;
    
    cur_field_type_param->nof_illegal_value = 0;
    
    SHR_IF_ERR_EXIT(dbal_db_init_field_type_add(unit, cur_field_type_param, cur_field_types_info));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dbal_init_field_types_fabric_field_types_definition_init(
int unit,
field_type_db_struct_t * cur_field_type_param,
dbal_field_types_basic_info_t ** field_types_info)
{
    SHR_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_pipe_id_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_context_id_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_subcontext_id_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_link_id_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_priority_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_fc_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_ocb_fc_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_mode_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_dtq_global_shaper_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_context_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_ocb_context_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_pdq_s2d_context_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_pcp_mode_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_wfq_fifo_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_load_balancing_count_mode_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_load_balancing_context_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_force_init(unit, cur_field_type_param, field_types_info));
    SHR_IF_ERR_EXIT(_dbal_init_field_types_fabric_field_types_definition_fabric_sr_cell_instance_init(unit, cur_field_type_param, field_types_info));
exit:
    SHR_FUNC_EXIT;
}
