
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_SAT

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_sat.h>








static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_64B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_64B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 203000000;

    
    define->data = 203000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_128B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_128B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 235000000;

    
    define->data = 235000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_256B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_256B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 310000000;

    
    define->data = 310000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_512B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_512B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 309000000;

    
    define->data = 309000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1024B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_1024B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 369000000;

    
    define->data = 369000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1280B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_1280B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 346000000;

    
    define->data = 346000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1518B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_1518B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 373000000;

    
    define->data = 373000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_bit_rate_in_9000B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_bit_rate_in_9000B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 400000000;

    
    define->data = 400000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_64B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_64B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 450000000;

    
    define->data = 450000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_128B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_128B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 333000000;

    
    define->data = 333000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_256B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_256B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 179000000;

    
    define->data = 179000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_512B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_512B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 93600000;

    
    define->data = 93600000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1024B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_1024B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 47800000;

    
    define->data = 47800000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1280B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_1280B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 38400000;

    
    define->data = 38400000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1518B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_1518B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 32400000;

    
    define->data = 32400000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_packet_rate_in_9000B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_packet_rate_in_9000B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 5540000;

    
    define->data = 5540000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_64B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_64B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 378000000;

    
    define->data = 378000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_128B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_128B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 225000000;

    
    define->data = 225000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_256B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_256B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 150000000;

    
    define->data = 150000000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_512B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_512B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 75000000 ;

    
    define->data = 75000000 ;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1024B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1024B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 45000000 ;

    
    define->data = 45000000 ;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1280B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1280B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 33700000 ;

    
    define->data = 33700000 ;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1518B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1518B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 30600000 ;

    
    define->data = 30600000 ;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_9000B_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_generator;
    int define_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_9000B;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 5540000  ;

    
    define->data = 5540000  ;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}






static shr_error_e
j2c_a0_dnx_data_sat_rx_flow_param_natural_id_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_rx_flow_param;
    int feature_index = dnx_data_sat_rx_flow_param_natural_id;
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
j2c_a0_dnx_data_sat_rx_flow_param_odd_even_id_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_rx_flow_param;
    int feature_index = dnx_data_sat_rx_flow_param_odd_even_id;
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
j2c_a0_dnx_data_sat_rx_flow_param_twamp_mode_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_rx_flow_param;
    int feature_index = dnx_data_sat_rx_flow_param_twamp_mode;
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
j2c_a0_dnx_data_sat_rx_flow_param_twamp_rx_time_stamp_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_rx_flow_param;
    int feature_index = dnx_data_sat_rx_flow_param_twamp_rx_time_stamp;
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
j2c_a0_dnx_data_sat_tx_flow_param_seq_number_wrap_around_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_sat;
    int submodule_index = dnx_data_sat_submodule_tx_flow_param;
    int feature_index = dnx_data_sat_tx_flow_param_seq_number_wrap_around;
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
j2c_a0_data_sat_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_sat;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_sat_submodule_generator;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_64B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_64B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_128B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_128B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_256B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_256B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_512B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_512B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_1024B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1024B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_1280B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1280B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_1518B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_1518B_set;
    data_index = dnx_data_sat_generator_define_max_bit_rate_in_9000B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_bit_rate_in_9000B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_64B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_64B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_128B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_128B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_256B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_256B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_512B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_512B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_1024B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1024B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_1280B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1280B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_1518B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_1518B_set;
    data_index = dnx_data_sat_generator_define_max_packet_rate_in_9000B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_packet_rate_in_9000B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_64B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_64B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_128B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_128B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_256B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_256B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_512B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_512B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1024B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1024B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1280B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1280B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_1518B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_1518B_set;
    data_index = dnx_data_sat_generator_define_max_ctf_recv_packet_rate_in_9000B;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_sat_generator_max_ctf_recv_packet_rate_in_9000B_set;

    

    
    
    submodule_index = dnx_data_sat_submodule_rx_flow_param;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_sat_rx_flow_param_natural_id;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_sat_rx_flow_param_natural_id_set;
    data_index = dnx_data_sat_rx_flow_param_odd_even_id;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_sat_rx_flow_param_odd_even_id_set;
    data_index = dnx_data_sat_rx_flow_param_twamp_mode;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_sat_rx_flow_param_twamp_mode_set;
    data_index = dnx_data_sat_rx_flow_param_twamp_rx_time_stamp;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_sat_rx_flow_param_twamp_rx_time_stamp_set;

    
    
    submodule_index = dnx_data_sat_submodule_tx_flow_param;
    submodule = &module->submodules[submodule_index];

    

    
    data_index = dnx_data_sat_tx_flow_param_seq_number_wrap_around;
    feature = &submodule->features[data_index];
    feature->set = j2c_a0_dnx_data_sat_tx_flow_param_seq_number_wrap_around_set;

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

