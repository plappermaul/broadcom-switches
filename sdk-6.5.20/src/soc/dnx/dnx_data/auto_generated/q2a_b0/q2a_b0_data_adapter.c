

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_TX

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_adapter.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_property.h>
#include <src/bcm/dnx/rx/auto_generated/rx_internal_q2a_b0.h>








static shr_error_e
q2a_b0_dnx_data_adapter_rx_packet_header_signal_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_adapter;
    int submodule_index = dnx_data_adapter_submodule_rx;
    int define_index = dnx_data_adapter_rx_define_packet_header_signal_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = PACKET_HEADER_SIGNAL_ID_Q2A_B0;

    
    define->data = PACKET_HEADER_SIGNAL_ID_Q2A_B0;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
q2a_b0_dnx_data_adapter_rx_constant_header_size_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_adapter;
    int submodule_index = dnx_data_adapter_submodule_rx;
    int define_index = dnx_data_adapter_rx_define_constant_header_size;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 28;

    
    define->data = 28;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
q2a_b0_dnx_data_adapter_general_lib_ver_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_adapter;
    int submodule_index = dnx_data_adapter_submodule_general;
    int define_index = dnx_data_adapter_general_define_lib_ver;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 5;

    
    define->data = 5;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}




shr_error_e
q2a_b0_data_adapter_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_adapter;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_adapter_submodule_rx;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_adapter_rx_define_packet_header_signal_id;
    define = &submodule->defines[data_index];
    define->set = q2a_b0_dnx_data_adapter_rx_packet_header_signal_id_set;
    data_index = dnx_data_adapter_rx_define_constant_header_size;
    define = &submodule->defines[data_index];
    define->set = q2a_b0_dnx_data_adapter_rx_constant_header_size_set;

    

    
    
    submodule_index = dnx_data_adapter_submodule_general;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_adapter_general_define_lib_ver;
    define = &submodule->defines[data_index];
    define->set = q2a_b0_dnx_data_adapter_general_lib_ver_set;

    

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

