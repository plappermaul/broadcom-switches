
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_COSQ

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_iqs.h>












static shr_error_e
q2a_a0_dnx_data_iqs_deq_default_dqcq_fc_to_dram_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_iqs;
    int submodule_index = dnx_data_iqs_submodule_deq_default;
    int feature_index = dnx_data_iqs_deq_default_dqcq_fc_to_dram;
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
q2a_a0_dnx_data_iqs_dqcq_8_priorities_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_iqs;
    int submodule_index = dnx_data_iqs_submodule_dqcq;
    int feature_index = dnx_data_iqs_dqcq_8_priorities;
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
q2a_a0_dnx_data_iqs_dqcq_max_nof_contexts_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_iqs;
    int submodule_index = dnx_data_iqs_submodule_dqcq;
    int define_index = dnx_data_iqs_dqcq_define_max_nof_contexts;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 1;

    
    define->data = 1;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
q2a_a0_dnx_data_iqs_dqcq_nof_priorities_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_iqs;
    int submodule_index = dnx_data_iqs_submodule_dqcq;
    int define_index = dnx_data_iqs_dqcq_define_nof_priorities;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 8;

    
    define->data = 8;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
q2a_a0_dnx_data_iqs_dbal_ipt_counters_nof_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_iqs;
    int submodule_index = dnx_data_iqs_submodule_dbal;
    int define_index = dnx_data_iqs_dbal_define_ipt_counters_nof_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 32;

    
    define->data = 32;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}




shr_error_e
q2a_a0_data_iqs_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_iqs;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_iqs_submodule_credit;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_iqs_credit_define_fmq_credit_fc_on_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_credit_fc_off_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_byte_fc_on_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_byte_fc_off_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_eir_credit_fc_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_eir_byte_fc_th;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_nof_be_classes;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_max_be_weight;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_max_burst_max;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_shp_crdt_rate_mltp;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_shp_crdt_rate_delta;
    define = &submodule->defines[data_index];
    define->set = NULL;
    data_index = dnx_data_iqs_credit_define_fmq_shp_rate_max;
    define = &submodule->defines[data_index];
    define->set = NULL;

    

    
    
    submodule_index = dnx_data_iqs_submodule_deq_default;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_iqs_deq_default_dqcq_fc_to_dram;
    feature = &submodule->features[data_index];
    feature->set = q2a_a0_dnx_data_iqs_deq_default_dqcq_fc_to_dram_set;

    
    
    submodule_index = dnx_data_iqs_submodule_dqcq;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_iqs_dqcq_define_max_nof_contexts;
    define = &submodule->defines[data_index];
    define->set = q2a_a0_dnx_data_iqs_dqcq_max_nof_contexts_set;
    data_index = dnx_data_iqs_dqcq_define_nof_priorities;
    define = &submodule->defines[data_index];
    define->set = q2a_a0_dnx_data_iqs_dqcq_nof_priorities_set;

    
    data_index = dnx_data_iqs_dqcq_8_priorities;
    feature = &submodule->features[data_index];
    feature->set = q2a_a0_dnx_data_iqs_dqcq_8_priorities_set;

    
    
    submodule_index = dnx_data_iqs_submodule_dbal;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_iqs_dbal_define_ipt_counters_nof_bits;
    define = &submodule->defines[data_index];
    define->set = q2a_a0_dnx_data_iqs_dbal_ipt_counters_nof_bits_set;

    

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

