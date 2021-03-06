
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

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_ecgm.h>
#include <bcm_int/dnx/cosq/egress/ecgm.h>








static shr_error_e
jr2_a0_dnx_data_ecgm_general_ecgm_is_supported_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_general;
    int define_index = dnx_data_ecgm_general_define_ecgm_is_supported;
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
jr2_a0_dnx_data_ecgm_core_resources_nof_sp_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_nof_sp;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 2;

    
    define->data = 2;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_nof_interface_profiles_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_nof_interface_profiles;
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
jr2_a0_dnx_data_ecgm_core_resources_nof_port_profiles_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_nof_port_profiles;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 16;

    
    define->data = 16;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_total_pds_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_total_pds;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 64000;

    
    define->data = 64000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_total_pds_nof_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_total_pds_nof_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 16;

    
    define->data = 16;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_total_dbs_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_total_dbs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 24000;

    
    define->data = 24000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_total_dbs_nof_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_total_dbs_nof_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 15;

    
    define->data = 15;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_reserved_dbs_per_sp_nof_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_reserved_dbs_per_sp_nof_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 15;

    
    define->data = 15;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_max_alpha_value_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_max_alpha_value;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 7;

    
    define->data = 7;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_min_alpha_value_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_min_alpha_value;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = -7;

    
    define->data = -7;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_core_resources_max_core_bandwidth_Mbps_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_core_resources;
    int define_index = dnx_data_ecgm_core_resources_define_max_core_bandwidth_Mbps;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 2400000;

    
    define->data = 2400000;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}






static shr_error_e
jr2_a0_dnx_data_ecgm_port_resources_profile_management_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_port_resources;
    int feature_index = dnx_data_ecgm_port_resources_profile_management;
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
jr2_a0_dnx_data_ecgm_port_resources_max_nof_ports_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_port_resources;
    int define_index = dnx_data_ecgm_port_resources_define_max_nof_ports;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 48;

    
    define->data = 48;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}








static shr_error_e
jr2_a0_dnx_data_ecgm_delete_fifo_thresholds_set(
    int unit)
{
    int fifo_type_index;
    dnx_data_ecgm_delete_fifo_thresholds_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_delete_fifo;
    int table_index = dnx_data_ecgm_delete_fifo_table_thresholds;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 3;
    table->info_get.key_size[0] = 3;

    
    table->values[0].default_val = "0";
    table->values[1].default_val = "0";
    table->values[2].default_val = "0";
    table->values[3].default_val = "0";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_ecgm_delete_fifo_thresholds_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_ecgm_delete_fifo_table_thresholds");

    
    default_data = (dnx_data_ecgm_delete_fifo_thresholds_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->fqp_high_priority = 0;
    default_data->stop_mc_low = 0;
    default_data->stop_mc_high = 0;
    default_data->stop_all = 0;
    
    for (fifo_type_index = 0; fifo_type_index < table->keys[0].size; fifo_type_index++)
    {
        data = (dnx_data_ecgm_delete_fifo_thresholds_t *) dnxc_data_mgmt_table_data_get(unit, table, fifo_type_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (ECGM_LONG_PACKETS_FIFO < table->keys[0].size)
    {
        data = (dnx_data_ecgm_delete_fifo_thresholds_t *) dnxc_data_mgmt_table_data_get(unit, table, ECGM_LONG_PACKETS_FIFO, 0);
        data->fqp_high_priority = 500;
        data->stop_mc_low = 500;
        data->stop_mc_high = 400;
        data->stop_all = 300;
    }
    if (ECGM_LCD_FIFO < table->keys[0].size)
    {
        data = (dnx_data_ecgm_delete_fifo_thresholds_t *) dnxc_data_mgmt_table_data_get(unit, table, ECGM_LCD_FIFO, 0);
        data->fqp_high_priority = 500;
        data->stop_mc_low = 500;
        data->stop_mc_high = 400;
        data->stop_all = 300;
    }
    if (ECGM_TDM_AND_SHORT_PACKETS < table->keys[0].size)
    {
        data = (dnx_data_ecgm_delete_fifo_thresholds_t *) dnxc_data_mgmt_table_data_get(unit, table, ECGM_TDM_AND_SHORT_PACKETS, 0);
        data->fqp_high_priority = 400;
        data->stop_mc_low = 400;
        data->stop_mc_high = 350;
        data->stop_all = 300;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}





static shr_error_e
jr2_a0_dnx_data_ecgm_info_fc_egr_if_vector_select_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int feature_index = dnx_data_ecgm_info_fc_egr_if_vector_select;
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
jr2_a0_dnx_data_ecgm_info_fc_q_pair_vector_select_nof_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_fc_q_pair_vector_select_nof_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 2;

    
    define->data = 2;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_fc_q_pair_vector_nof_presented_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_fc_q_pair_vector_nof_presented;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 128;

    
    define->data = 128;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_fc_egr_if_vector_nof_presented_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_fc_egr_if_vector_nof_presented;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 128;

    
    define->data = 128;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_nof_dropped_reasons_rqp_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_nof_dropped_reasons_rqp;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 11;

    
    define->data = 11;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_nof_dropped_reasons_pqp_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_nof_dropped_reasons_pqp;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 16;

    
    define->data = 16;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_nof_bits_in_pd_count_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int define_index = dnx_data_ecgm_info_define_nof_bits_in_pd_count;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 16;

    
    define->data = 16;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}



static shr_error_e
jr2_a0_dnx_data_ecgm_info_dropped_reason_rqp_set(
    int unit)
{
    int index_index;
    dnx_data_ecgm_info_dropped_reason_rqp_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int table_index = dnx_data_ecgm_info_table_dropped_reason_rqp;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 11;
    table->info_get.key_size[0] = 11;

    
    table->values[0].default_val = "NULL";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_ecgm_info_dropped_reason_rqp_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_ecgm_info_table_dropped_reason_rqp");

    
    default_data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->reason = NULL;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->reason = "Ttl DBs thld violated";
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->reason = "Ttl UC DBs pool thld violated";
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->reason = "UC pkt discarded, UC FIFO is full";
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->reason = "MC HP discarded, MC FIFO is full";
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->reason = "MC LP discarded, MC FIFO is full";
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->reason = "Ttl MC DBs pool thld violated";
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->reason = "Packet-DP not eligbl for shared DB";
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->reason = "SP DBs threshold violated";
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->reason = "DP mthd: MC-TC DB thld violated";
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->reason = "SP mthd: MC-TC to SP0 DB thld vilt";
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_rqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->reason = "SP mthd: MC-TC to SP1 DB thld vilt";
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
jr2_a0_dnx_data_ecgm_info_dropped_reason_pqp_set(
    int unit)
{
    int index_index;
    dnx_data_ecgm_info_dropped_reason_pqp_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = dnx_data_ecgm_submodule_info;
    int table_index = dnx_data_ecgm_info_table_dropped_reason_pqp;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 16;
    table->info_get.key_size[0] = 16;

    
    table->values[0].default_val = "NULL";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_ecgm_info_dropped_reason_pqp_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_ecgm_info_table_dropped_reason_pqp");

    
    default_data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->reason = NULL;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->reason = "Total PDs threshold violated";
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->reason = "Ttl UC PDs pool thld violt";
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->reason = "Per port UC PDs threshold";
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->reason = "Per queue UC PDs thresholds";
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->reason = "Per port UC DBs threshold";
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->reason = "Per queue UC DBs threshold";
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->reason = "Per queue disable bit";
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->reason = "no description in reg";
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->reason = "Total PDs MC pool threshold";
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->reason = "Per interface PDs threhold";
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->reason = "MC SP threshold";
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->reason = "per MC-TC threshold";
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->reason = "MC PDs per port threshold";
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->reason = "MC PDs per queue threshold";
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->reason = "MC per port size threshold";
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_ecgm_info_dropped_reason_pqp_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->reason = "MC per queue size threshold";
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}



shr_error_e
jr2_a0_data_ecgm_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_ecgm;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_ecgm_submodule_general;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_ecgm_general_define_ecgm_is_supported;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_general_ecgm_is_supported_set;

    

    
    
    submodule_index = dnx_data_ecgm_submodule_core_resources;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_ecgm_core_resources_define_nof_sp;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_nof_sp_set;
    data_index = dnx_data_ecgm_core_resources_define_nof_interface_profiles;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_nof_interface_profiles_set;
    data_index = dnx_data_ecgm_core_resources_define_nof_port_profiles;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_nof_port_profiles_set;
    data_index = dnx_data_ecgm_core_resources_define_total_pds;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_total_pds_set;
    data_index = dnx_data_ecgm_core_resources_define_total_pds_nof_bits;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_total_pds_nof_bits_set;
    data_index = dnx_data_ecgm_core_resources_define_total_dbs;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_total_dbs_set;
    data_index = dnx_data_ecgm_core_resources_define_total_dbs_nof_bits;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_total_dbs_nof_bits_set;
    data_index = dnx_data_ecgm_core_resources_define_reserved_dbs_per_sp_nof_bits;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_reserved_dbs_per_sp_nof_bits_set;
    data_index = dnx_data_ecgm_core_resources_define_max_alpha_value;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_max_alpha_value_set;
    data_index = dnx_data_ecgm_core_resources_define_min_alpha_value;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_min_alpha_value_set;
    data_index = dnx_data_ecgm_core_resources_define_max_core_bandwidth_Mbps;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_core_resources_max_core_bandwidth_Mbps_set;

    

    
    
    submodule_index = dnx_data_ecgm_submodule_port_resources;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_ecgm_port_resources_define_max_nof_ports;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_port_resources_max_nof_ports_set;

    
    data_index = dnx_data_ecgm_port_resources_profile_management;
    feature = &submodule->features[data_index];
    feature->set = jr2_a0_dnx_data_ecgm_port_resources_profile_management_set;

    
    
    submodule_index = dnx_data_ecgm_submodule_delete_fifo;
    submodule = &module->submodules[submodule_index];

    

    

    
    data_index = dnx_data_ecgm_delete_fifo_table_thresholds;
    table = &submodule->tables[data_index];
    table->set = jr2_a0_dnx_data_ecgm_delete_fifo_thresholds_set;
    
    submodule_index = dnx_data_ecgm_submodule_info;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_ecgm_info_define_fc_q_pair_vector_select_nof_bits;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_fc_q_pair_vector_select_nof_bits_set;
    data_index = dnx_data_ecgm_info_define_fc_q_pair_vector_nof_presented;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_fc_q_pair_vector_nof_presented_set;
    data_index = dnx_data_ecgm_info_define_fc_egr_if_vector_nof_presented;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_fc_egr_if_vector_nof_presented_set;
    data_index = dnx_data_ecgm_info_define_nof_dropped_reasons_rqp;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_nof_dropped_reasons_rqp_set;
    data_index = dnx_data_ecgm_info_define_nof_dropped_reasons_pqp;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_nof_dropped_reasons_pqp_set;
    data_index = dnx_data_ecgm_info_define_nof_bits_in_pd_count;
    define = &submodule->defines[data_index];
    define->set = jr2_a0_dnx_data_ecgm_info_nof_bits_in_pd_count_set;

    
    data_index = dnx_data_ecgm_info_fc_egr_if_vector_select;
    feature = &submodule->features[data_index];
    feature->set = jr2_a0_dnx_data_ecgm_info_fc_egr_if_vector_select_set;

    
    data_index = dnx_data_ecgm_info_table_dropped_reason_rqp;
    table = &submodule->tables[data_index];
    table->set = jr2_a0_dnx_data_ecgm_info_dropped_reason_rqp_set;
    data_index = dnx_data_ecgm_info_table_dropped_reason_pqp;
    table = &submodule->tables[data_index];
    table->set = jr2_a0_dnx_data_ecgm_info_dropped_reason_pqp_set;

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

