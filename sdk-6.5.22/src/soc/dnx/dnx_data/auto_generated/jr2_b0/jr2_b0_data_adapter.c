
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_TX

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_adapter.h>
#include <src/bcm/dnx/rx/auto_generated/rx_internal_jr2_b0.h>








static shr_error_e
jr2_b0_dnx_data_adapter_rx_packet_header_signal_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_adapter;
    int submodule_index = dnx_data_adapter_submodule_rx;
    int define_index = dnx_data_adapter_rx_define_packet_header_signal_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = PACKET_HEADER_SIGNAL_ID_JR2_B0;

    
    define->data = PACKET_HEADER_SIGNAL_ID_JR2_B0;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}




shr_error_e
jr2_b0_data_adapter_attach(
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
    define->set = jr2_b0_dnx_data_adapter_rx_packet_header_signal_id_set;

    

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE
