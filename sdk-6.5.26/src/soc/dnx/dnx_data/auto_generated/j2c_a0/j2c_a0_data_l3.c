
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_L3

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_l3.h>








static shr_error_e
j2c_a0_dnx_data_l3_fec_supported_fec_flags_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_fec;
    int define_index = dnx_data_l3_fec_define_supported_fec_flags;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = (BCM_L3_2ND_HIERARCHY | BCM_L3_3RD_HIERARCHY | BCM_L3_MC_NO_RPF | BCM_L3_MC_RPF_EXPLICIT | BCM_L3_MC_RPF_EXPLICIT_ECMP | BCM_L3_MC_RPF_SIP_BASE | BCM_L3_ENCAP_SPACE_OPTIMIZED | BCM_L3_HIT | BCM_L3_HIT_CLEAR);

    
    define->data = (BCM_L3_2ND_HIERARCHY | BCM_L3_3RD_HIERARCHY | BCM_L3_MC_NO_RPF | BCM_L3_MC_RPF_EXPLICIT | BCM_L3_MC_RPF_EXPLICIT_ECMP | BCM_L3_MC_RPF_SIP_BASE | BCM_L3_ENCAP_SPACE_OPTIMIZED | BCM_L3_HIT | BCM_L3_HIT_CLEAR);

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_l3_fec_supported_fec_flags2_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_fec;
    int define_index = dnx_data_l3_fec_define_supported_fec_flags2;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = (BCM_L3_FLAGS2_ECMP_RANGE_OVERLAP | BCM_L3_FLAGS2_SKIP_HIT_CLEAR);

    
    define->data = (BCM_L3_FLAGS2_ECMP_RANGE_OVERLAP | BCM_L3_FLAGS2_SKIP_HIT_CLEAR);

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
j2c_a0_dnx_data_l3_source_address_custom_sa_use_dual_homing_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_source_address;
    int define_index = dnx_data_l3_source_address_define_custom_sa_use_dual_homing;
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
j2c_a0_dnx_data_l3_feature_fec_hit_bit_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_feature;
    int feature_index = dnx_data_l3_feature_fec_hit_bit;
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
j2c_a0_dnx_data_l3_feature_nat_on_a_stick_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_feature;
    int feature_index = dnx_data_l3_feature_nat_on_a_stick;
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
j2c_a0_dnx_data_l3_feature_ingress_frag_not_first_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_l3;
    int submodule_index = dnx_data_l3_submodule_feature;
    int feature_index = dnx_data_l3_feature_ingress_frag_not_first;
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
j2c_a0_data_l3_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_l3;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_l3_submodule_fec;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_l3_fec_define_supported_fec_flags;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_l3_fec_supported_fec_flags_set;
    data_index = dnx_data_l3_fec_define_supported_fec_flags2;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_l3_fec_supported_fec_flags2_set;

    

    
    
    submodule_index = dnx_data_l3_submodule_source_address;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_l3_source_address_define_custom_sa_use_dual_homing;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_l3_source_address_custom_sa_use_dual_homing_set;

    

    
    
    submodule_index = dnx_data_l3_submodule_feature;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_l3_feature_fec_hit_bit;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_l3_feature_fec_hit_bit_set;
    data_index = dnx_data_l3_feature_nat_on_a_stick;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_l3_feature_nat_on_a_stick_set;
    data_index = dnx_data_l3_feature_ingress_frag_not_first;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_l3_feature_ingress_frag_not_first_set;

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

