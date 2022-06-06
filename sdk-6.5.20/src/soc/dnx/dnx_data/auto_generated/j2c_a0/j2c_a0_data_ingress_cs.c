

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_INGRCS

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_ingress_cs.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_property.h>







static shr_error_e
j2c_a0_dnx_data_ingress_cs_features_disable_ecc_fix_en_before_read_write_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_ingress_cs;
    int submodule_index = dnx_data_ingress_cs_submodule_features;
    int feature_index = dnx_data_ingress_cs_features_disable_ecc_fix_en_before_read_write;
    SHR_FUNC_INIT_VARS(unit);

    feature = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].features[feature_index];
    
    feature->default_data = 0;

    
    feature->data = 0;

    
    feature->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_feature_value_set(unit, feature));

exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
j2c_a0_dnx_data_ingress_cs_parser_parsing_context_map_enable_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_ingress_cs;
    int submodule_index = dnx_data_ingress_cs_submodule_parser;
    int feature_index = dnx_data_ingress_cs_parser_parsing_context_map_enable;
    SHR_FUNC_INIT_VARS(unit);

    feature = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].features[feature_index];
    
    feature->default_data = 1;

    
    feature->data = 1;

    
    feature->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_feature_value_set(unit, feature));

exit:
    SHR_FUNC_EXIT;
}





shr_error_e
j2c_a0_data_ingress_cs_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_ingress_cs;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_ingress_cs_submodule_features;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_ingress_cs_features_disable_ecc_fix_en_before_read_write;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_ingress_cs_features_disable_ecc_fix_en_before_read_write_set;

    
    
    submodule_index = dnx_data_ingress_cs_submodule_parser;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_ingress_cs_parser_parsing_context_map_enable;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_ingress_cs_parser_parsing_context_map_enable_set;

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

