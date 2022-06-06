

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_COSQ

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_ingr_congestion.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_property.h>
#include <bcm_int/dnx/cosq/ingress/ingress_congestion.h>







static shr_error_e
q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_ingr_congestion;
    int submodule_index = dnx_data_ingr_congestion_submodule_info;
    int feature_index = dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable;
    SHR_FUNC_INIT_VARS(unit);

    feature = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].features[feature_index];
    
    feature->default_data = 1;

    
    feature->data = 1;

    
    feature->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_feature_value_set(unit, feature));

exit:
    SHR_FUNC_EXIT;
}



static shr_error_e
q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_reject_flow_profile_value_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ingr_congestion;
    int submodule_index = dnx_data_ingr_congestion_submodule_info;
    int define_index = dnx_data_ingr_congestion_info_define_latency_based_admission_reject_flow_profile_value;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 0;

    
    define->data = 0;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable_value_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ingr_congestion;
    int submodule_index = dnx_data_ingr_congestion_submodule_info;
    int define_index = dnx_data_ingr_congestion_info_define_latency_based_admission_cgm_extended_profiles_enable_value;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 0;

    
    define->data = 0;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_max_supported_profile_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ingr_congestion;
    int submodule_index = dnx_data_ingr_congestion_submodule_info;
    int define_index = dnx_data_ingr_congestion_info_define_latency_based_admission_max_supported_profile;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 31;

    
    define->data = 31;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}




shr_error_e
q2a_b1_data_ingr_congestion_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_ingr_congestion;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_ingr_congestion_submodule_info;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_ingr_congestion_info_define_latency_based_admission_reject_flow_profile_value;
    define = &submodule->defines[data_index];
    define->set = q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_reject_flow_profile_value_set;
    data_index = dnx_data_ingr_congestion_info_define_latency_based_admission_cgm_extended_profiles_enable_value;
    define = &submodule->defines[data_index];
    define->set = q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable_value_set;
    data_index = dnx_data_ingr_congestion_info_define_latency_based_admission_max_supported_profile;
    define = &submodule->defines[data_index];
    define->set = q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_max_supported_profile_set;

    
    data_index = dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable;
    feature = &submodule->features[data_index];
    feature->set = q2a_b1_dnx_data_ingr_congestion_info_latency_based_admission_cgm_extended_profiles_enable_set;

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

