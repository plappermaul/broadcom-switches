

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_PORT_PP

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_port_pp.h>



extern shr_error_e jr2_a0_data_port_pp_attach(
    int unit);
extern shr_error_e j2c_a1_data_port_pp_attach(
    int unit);
extern shr_error_e q2a_a0_data_port_pp_attach(
    int unit);
extern shr_error_e j2p_a0_data_port_pp_attach(
    int unit);



static shr_error_e
dnx_data_port_pp_general_init(
    int unit,
    dnxc_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "general";
    submodule_data->doc = "general port pp data";
    
    submodule_data->nof_features = _dnx_data_port_pp_general_feature_nof;
    DNXC_DATA_ALLOC(submodule_data->features, dnxc_data_feature_t,  submodule_data->nof_features, "_dnxc_data port_pp general features");

    
    submodule_data->nof_defines = _dnx_data_port_pp_general_define_nof;
    DNXC_DATA_ALLOC(submodule_data->defines, dnxc_data_define_t, submodule_data->nof_defines, "_dnxc_data port_pp general defines");

    submodule_data->defines[dnx_data_port_pp_general_define_nof_ingress_llvp_profiles].name = "nof_ingress_llvp_profiles";
    submodule_data->defines[dnx_data_port_pp_general_define_nof_ingress_llvp_profiles].doc = "nof ingress llvp profiles";
    
    submodule_data->defines[dnx_data_port_pp_general_define_nof_ingress_llvp_profiles].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_llvp_profiles].name = "nof_egress_llvp_profiles";
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_llvp_profiles].doc = "nof egress llvp profiles";
    
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_llvp_profiles].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_default_ingress_native_ac_llvp_profile_id].name = "default_ingress_native_ac_llvp_profile_id";
    submodule_data->defines[dnx_data_port_pp_general_define_default_ingress_native_ac_llvp_profile_id].doc = "Default Ingress Native AC LLVP profile id";
    
    submodule_data->defines[dnx_data_port_pp_general_define_default_ingress_native_ac_llvp_profile_id].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_native_ac_llvp_profile_id].name = "default_egress_native_ac_llvp_profile_id";
    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_native_ac_llvp_profile_id].doc = "Default Egress Native AC LLVP profile id";
    
    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_native_ac_llvp_profile_id].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_ip_tunnel_llvp_profile_id].name = "default_egress_ip_tunnel_llvp_profile_id";
    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_ip_tunnel_llvp_profile_id].doc = "Default Egress IP Tunnel LLVP profile id";
    
    submodule_data->defines[dnx_data_port_pp_general_define_default_egress_ip_tunnel_llvp_profile_id].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_sit_profile].name = "nof_egress_sit_profile";
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_sit_profile].doc = "nof sit profile";
    
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_sit_profile].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_acceptable_frame_type_profile].name = "nof_egress_acceptable_frame_type_profile";
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_acceptable_frame_type_profile].doc = "nof egress acceptable frame type profile";
    
    submodule_data->defines[dnx_data_port_pp_general_define_nof_egress_acceptable_frame_type_profile].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_sit_inner_eth_additional_tag].name = "sit_inner_eth_additional_tag";
    submodule_data->defines[dnx_data_port_pp_general_define_sit_inner_eth_additional_tag].doc = "";
    
    submodule_data->defines[dnx_data_port_pp_general_define_sit_inner_eth_additional_tag].flags |= DNXC_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_port_pp_general_define_nof_virtual_port_tcam_entries].name = "nof_virtual_port_tcam_entries";
    submodule_data->defines[dnx_data_port_pp_general_define_nof_virtual_port_tcam_entries].doc = "Number of entries per core in PRT_VIRTUAL_PORT_TCAM";
    
    submodule_data->defines[dnx_data_port_pp_general_define_nof_virtual_port_tcam_entries].flags |= DNXC_DATA_F_DEFINE;

    
    submodule_data->nof_tables = _dnx_data_port_pp_general_table_nof;
    DNXC_DATA_ALLOC(submodule_data->tables, dnxc_data_table_t, submodule_data->nof_tables, "_dnxc_data port_pp general tables");


exit:
    SHR_FUNC_EXIT;
}


int
dnx_data_port_pp_general_feature_get(
    int unit,
    dnx_data_port_pp_general_feature_e feature)
{
    return dnxc_data_mgmt_feature_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, feature);
}


uint32
dnx_data_port_pp_general_nof_ingress_llvp_profiles_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_nof_ingress_llvp_profiles);
}

uint32
dnx_data_port_pp_general_nof_egress_llvp_profiles_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_nof_egress_llvp_profiles);
}

uint32
dnx_data_port_pp_general_default_ingress_native_ac_llvp_profile_id_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_default_ingress_native_ac_llvp_profile_id);
}

uint32
dnx_data_port_pp_general_default_egress_native_ac_llvp_profile_id_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_default_egress_native_ac_llvp_profile_id);
}

uint32
dnx_data_port_pp_general_default_egress_ip_tunnel_llvp_profile_id_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_default_egress_ip_tunnel_llvp_profile_id);
}

uint32
dnx_data_port_pp_general_nof_egress_sit_profile_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_nof_egress_sit_profile);
}

uint32
dnx_data_port_pp_general_nof_egress_acceptable_frame_type_profile_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_nof_egress_acceptable_frame_type_profile);
}

uint32
dnx_data_port_pp_general_sit_inner_eth_additional_tag_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_sit_inner_eth_additional_tag);
}

uint32
dnx_data_port_pp_general_nof_virtual_port_tcam_entries_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_port_pp, dnx_data_port_pp_submodule_general, dnx_data_port_pp_general_define_nof_virtual_port_tcam_entries);
}







shr_error_e
dnx_data_port_pp_init(
    int unit,
    dnxc_data_module_t *module_data)
{
    SHR_FUNC_INIT_VARS(unit);

    
    module_data->name = "port_pp";
    module_data->nof_submodules = _dnx_data_port_pp_submodule_nof;
    DNXC_DATA_ALLOC(module_data->submodules, dnxc_data_submodule_t, module_data->nof_submodules, "_dnxc_data port_pp submodules");

    
    SHR_IF_ERR_EXIT(dnx_data_port_pp_general_init(unit, &module_data->submodules[dnx_data_port_pp_submodule_general]));
    
    if (dnxc_data_mgmt_is_jr2_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_jr2_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_jr2_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_j2c_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_j2c_a1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
        SHR_IF_ERR_EXIT(j2c_a1_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_q2a_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_q2a_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_q2a_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_port_pp_attach(unit));
    }
    else
    if (dnxc_data_mgmt_is_j2p_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_port_pp_attach(unit));
        SHR_IF_ERR_EXIT(j2p_a0_data_port_pp_attach(unit));
    }
    else
    {
        SHR_EXIT();
    }

exit:
    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

