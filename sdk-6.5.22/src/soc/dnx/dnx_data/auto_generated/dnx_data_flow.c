
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FLOW

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_flow.h>



#ifdef BCM_DNX2_SUPPORT

extern shr_error_e jr2_a0_data_flow_attach(
    int unit);

#endif 



static shr_error_e
dnx_data_flow_special_fields_init(
    int unit,
    dnxc_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "special_fields";
    submodule_data->doc = "Special fields properties";
    
    submodule_data->nof_features = _dnx_data_flow_special_fields_feature_nof;
    DNXC_DATA_ALLOC(submodule_data->features, dnxc_data_feature_t,  submodule_data->nof_features, "_dnxc_data flow special_fields features");

    
    submodule_data->nof_defines = _dnx_data_flow_special_fields_define_nof;
    DNXC_DATA_ALLOC(submodule_data->defines, dnxc_data_define_t, submodule_data->nof_defines, "_dnxc_data flow special_fields defines");

    
    submodule_data->nof_tables = _dnx_data_flow_special_fields_table_nof;
    DNXC_DATA_ALLOC(submodule_data->tables, dnxc_data_table_t, submodule_data->nof_tables, "_dnxc_data flow special_fields tables");

    
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].name = "name_mapping";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].doc = "";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].flags |= DNXC_DATA_F_TABLE;
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].size_of_values = sizeof(dnx_data_flow_special_fields_name_mapping_t);
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].entry_get = dnx_data_flow_special_fields_name_mapping_entry_str_get;

    
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].nof_keys = 1;
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].keys[0].name = "field_id";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].keys[0].doc = "Dbal field id";

    
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].nof_values = 1;
    DNXC_DATA_ALLOC(submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].values, dnxc_data_value_t, submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].nof_values, "_dnx_data_flow_special_fields_table_name_mapping table values");
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].values[0].name = "field_name";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].values[0].type = "char *";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].values[0].doc = "Field name doesnt have to match dbal field name";
    submodule_data->tables[dnx_data_flow_special_fields_table_name_mapping].values[0].offset = UTILEX_OFFSETOF(dnx_data_flow_special_fields_name_mapping_t, field_name);


exit:
    SHR_FUNC_EXIT;
}


int
dnx_data_flow_special_fields_feature_get(
    int unit,
    dnx_data_flow_special_fields_feature_e feature)
{
    return dnxc_data_mgmt_feature_data_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_special_fields, feature);
}




const dnx_data_flow_special_fields_name_mapping_t *
dnx_data_flow_special_fields_name_mapping_get(
    int unit,
    int field_id)
{
    char *data;
    dnxc_data_table_t *table;

    
    table = dnxc_data_mgmt_table_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_special_fields, dnx_data_flow_special_fields_table_name_mapping);
    
    data = dnxc_data_mgmt_table_data_get(unit, table, field_id, 0);
    return (const dnx_data_flow_special_fields_name_mapping_t *) data;

}


shr_error_e
dnx_data_flow_special_fields_name_mapping_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    dnxc_data_table_t *table;
    const dnx_data_flow_special_fields_name_mapping_t *data;
    SHR_FUNC_INIT_VARS(unit);

    
    table = dnxc_data_mgmt_table_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_special_fields, dnx_data_flow_special_fields_table_name_mapping);
    data = (const dnx_data_flow_special_fields_name_mapping_t *) dnxc_data_mgmt_table_data_diag_get(unit, table, key0, 0);
    switch (value_index)
    {
        case 0:
            sal_snprintf(buffer, DNXC_DATA_MGMT_MAX_TABLE_VALUE_LENGTH, "%s", data->field_name == NULL ? "" : data->field_name);
            break;
    }

    SHR_FUNC_EXIT;
}


const dnxc_data_table_info_t *
dnx_data_flow_special_fields_name_mapping_info_get(
    int unit)
{

    
    return dnxc_data_mgmt_table_info_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_special_fields, dnx_data_flow_special_fields_table_name_mapping);

}






static shr_error_e
dnx_data_flow_ipv4_init(
    int unit,
    dnxc_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "ipv4";
    submodule_data->doc = "IPV4 tunnel data";
    
    submodule_data->nof_features = _dnx_data_flow_ipv4_feature_nof;
    DNXC_DATA_ALLOC(submodule_data->features, dnxc_data_feature_t,  submodule_data->nof_features, "_dnxc_data flow ipv4 features");

    
    submodule_data->nof_defines = _dnx_data_flow_ipv4_define_nof;
    DNXC_DATA_ALLOC(submodule_data->defines, dnxc_data_define_t, submodule_data->nof_defines, "_dnxc_data flow ipv4 defines");

    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_init].name = "use_flow_lif_init";
    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_init].doc = "Use flow lif to configure init tunnels";
    
    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_init].flags |= DNXC_DATA_F_NUMERIC;

    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_term].name = "use_flow_lif_term";
    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_term].doc = "Use flow lif to configure term tunnels";
    
    submodule_data->defines[dnx_data_flow_ipv4_define_use_flow_lif_term].flags |= DNXC_DATA_F_NUMERIC;

    
    submodule_data->nof_tables = _dnx_data_flow_ipv4_table_nof;
    DNXC_DATA_ALLOC(submodule_data->tables, dnxc_data_table_t, submodule_data->nof_tables, "_dnxc_data flow ipv4 tables");


exit:
    SHR_FUNC_EXIT;
}


int
dnx_data_flow_ipv4_feature_get(
    int unit,
    dnx_data_flow_ipv4_feature_e feature)
{
    return dnxc_data_mgmt_feature_data_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_ipv4, feature);
}


uint32
dnx_data_flow_ipv4_use_flow_lif_init_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_ipv4, dnx_data_flow_ipv4_define_use_flow_lif_init);
}

uint32
dnx_data_flow_ipv4_use_flow_lif_term_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_flow, dnx_data_flow_submodule_ipv4, dnx_data_flow_ipv4_define_use_flow_lif_term);
}







shr_error_e
dnx_data_flow_init(
    int unit,
    dnxc_data_module_t *module_data)
{
    SHR_FUNC_INIT_VARS(unit);

    
    module_data->name = "flow";
    module_data->nof_submodules = _dnx_data_flow_submodule_nof;
    DNXC_DATA_ALLOC(module_data->submodules, dnxc_data_submodule_t, module_data->nof_submodules, "_dnxc_data flow submodules");

    
    SHR_IF_ERR_EXIT(dnx_data_flow_special_fields_init(unit, &module_data->submodules[dnx_data_flow_submodule_special_fields]));
    SHR_IF_ERR_EXIT(dnx_data_flow_ipv4_init(unit, &module_data->submodules[dnx_data_flow_submodule_ipv4]));
    
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_jr2_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_jr2_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_jr2_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_j2c_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_j2c_a1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_q2a_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_q2a_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_q2a_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
#ifdef BCM_DNX2_SUPPORT

    if (dnxc_data_mgmt_is_j2p_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_flow_attach(unit));
    }
    else

#endif 
    {
        SHR_EXIT();
    }

exit:
    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

