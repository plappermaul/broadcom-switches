
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_PTP

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_ptp.h>




extern shr_error_e jr2_a0_data_ptp_attach(
    int unit);


extern shr_error_e j2c_a0_data_ptp_attach(
    int unit);


extern shr_error_e q2a_a0_data_ptp_attach(
    int unit);


extern shr_error_e j2p_a0_data_ptp_attach(
    int unit);


extern shr_error_e j2x_a0_data_ptp_attach(
    int unit);




static shr_error_e
dnx_data_ptp_general_init(
    int unit,
    dnxc_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "general";
    submodule_data->doc = "";
    
    submodule_data->nof_features = _dnx_data_ptp_general_feature_nof;
    DNXC_DATA_ALLOC(submodule_data->features, dnxc_data_feature_t,  submodule_data->nof_features, "_dnxc_data ptp general features");

    submodule_data->features[dnx_data_ptp_general_has_trap_code_offset].name = "has_trap_code_offset";
    submodule_data->features[dnx_data_ptp_general_has_trap_code_offset].doc = "";
    submodule_data->features[dnx_data_ptp_general_has_trap_code_offset].flags |= DNXC_DATA_F_FEATURE;

    submodule_data->features[dnx_data_ptp_general_has_external_brcm_mac].name = "has_external_brcm_mac";
    submodule_data->features[dnx_data_ptp_general_has_external_brcm_mac].doc = "";
    submodule_data->features[dnx_data_ptp_general_has_external_brcm_mac].flags |= DNXC_DATA_F_FEATURE;

    submodule_data->features[dnx_data_ptp_general_has_tx_compensation].name = "has_tx_compensation";
    submodule_data->features[dnx_data_ptp_general_has_tx_compensation].doc = "";
    submodule_data->features[dnx_data_ptp_general_has_tx_compensation].flags |= DNXC_DATA_F_FEATURE;

    submodule_data->features[dnx_data_ptp_general_has_f1588].name = "has_f1588";
    submodule_data->features[dnx_data_ptp_general_has_f1588].doc = "";
    submodule_data->features[dnx_data_ptp_general_has_f1588].flags |= DNXC_DATA_F_FEATURE;

    submodule_data->features[dnx_data_ptp_general_has_pp_dsp_ieee_1588_mac_enable].name = "has_pp_dsp_ieee_1588_mac_enable";
    submodule_data->features[dnx_data_ptp_general_has_pp_dsp_ieee_1588_mac_enable].doc = "";
    submodule_data->features[dnx_data_ptp_general_has_pp_dsp_ieee_1588_mac_enable].flags |= DNXC_DATA_F_FEATURE;

    
    submodule_data->nof_defines = _dnx_data_ptp_general_define_nof;
    DNXC_DATA_ALLOC(submodule_data->defines, dnxc_data_define_t, submodule_data->nof_defines, "_dnxc_data ptp general defines");

    submodule_data->defines[dnx_data_ptp_general_define_nof_port_profiles].name = "nof_port_profiles";
    submodule_data->defines[dnx_data_ptp_general_define_nof_port_profiles].doc = "";
    
    submodule_data->defines[dnx_data_ptp_general_define_nof_port_profiles].flags |= DNXC_DATA_F_DEFINE;

    
    submodule_data->nof_tables = _dnx_data_ptp_general_table_nof;
    DNXC_DATA_ALLOC(submodule_data->tables, dnxc_data_table_t, submodule_data->nof_tables, "_dnxc_data ptp general tables");


exit:
    SHR_FUNC_EXIT;
}


int
dnx_data_ptp_general_feature_get(
    int unit,
    dnx_data_ptp_general_feature_e feature)
{
    return dnxc_data_mgmt_feature_data_get(unit, dnx_data_module_ptp, dnx_data_ptp_submodule_general, feature);
}


uint32
dnx_data_ptp_general_nof_port_profiles_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_ptp, dnx_data_ptp_submodule_general, dnx_data_ptp_general_define_nof_port_profiles);
}










static shr_error_e
dnx_data_ptp_ptp_firmware_init(
    int unit,
    dnxc_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "ptp_firmware";
    submodule_data->doc = "";
    
    submodule_data->nof_features = _dnx_data_ptp_ptp_firmware_feature_nof;
    DNXC_DATA_ALLOC(submodule_data->features, dnxc_data_feature_t,  submodule_data->nof_features, "_dnxc_data ptp ptp_firmware features");

    
    submodule_data->nof_defines = _dnx_data_ptp_ptp_firmware_define_nof;
    DNXC_DATA_ALLOC(submodule_data->defines, dnxc_data_define_t, submodule_data->nof_defines, "_dnxc_data ptp ptp_firmware defines");

    submodule_data->defines[dnx_data_ptp_ptp_firmware_define_ptp_cosq_port].name = "ptp_cosq_port";
    submodule_data->defines[dnx_data_ptp_ptp_firmware_define_ptp_cosq_port].doc = "";
    
    submodule_data->defines[dnx_data_ptp_ptp_firmware_define_ptp_cosq_port].flags |= DNXC_DATA_F_NUMERIC;

    
    submodule_data->nof_tables = _dnx_data_ptp_ptp_firmware_table_nof;
    DNXC_DATA_ALLOC(submodule_data->tables, dnxc_data_table_t, submodule_data->nof_tables, "_dnxc_data ptp ptp_firmware tables");


exit:
    SHR_FUNC_EXIT;
}


int
dnx_data_ptp_ptp_firmware_feature_get(
    int unit,
    dnx_data_ptp_ptp_firmware_feature_e feature)
{
    return dnxc_data_mgmt_feature_data_get(unit, dnx_data_module_ptp, dnx_data_ptp_submodule_ptp_firmware, feature);
}


uint32
dnx_data_ptp_ptp_firmware_ptp_cosq_port_get(
    int unit)
{
    return dnxc_data_mgmt_define_data_get(unit, dnx_data_module_ptp, dnx_data_ptp_submodule_ptp_firmware, dnx_data_ptp_ptp_firmware_define_ptp_cosq_port);
}







shr_error_e
dnx_data_ptp_init(
    int unit,
    dnxc_data_module_t *module_data)
{
    SHR_FUNC_INIT_VARS(unit);

    
    module_data->name = "ptp";
    module_data->nof_submodules = _dnx_data_ptp_submodule_nof;
    DNXC_DATA_ALLOC(module_data->submodules, dnxc_data_submodule_t, module_data->nof_submodules, "_dnxc_data ptp submodules");

    
    SHR_IF_ERR_EXIT(dnx_data_ptp_general_init(unit, &module_data->submodules[dnx_data_ptp_submodule_general]));
    SHR_IF_ERR_EXIT(dnx_data_ptp_ptp_firmware_init(unit, &module_data->submodules[dnx_data_ptp_submodule_ptp_firmware]));
    

    if (dnxc_data_mgmt_is_jr2_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_jr2_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_jr2_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2c_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2c_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2c_a1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2c_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_q2a_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_q2a_b0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_q2a_b1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(q2a_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2p_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2p_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2p_a1(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2p_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2p_a2(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2p_a0_data_ptp_attach(unit));
    }
    else


    if (dnxc_data_mgmt_is_j2x_a0(unit))
    {
        SHR_IF_ERR_EXIT(jr2_a0_data_ptp_attach(unit));
        SHR_IF_ERR_EXIT(j2x_a0_data_ptp_attach(unit));
    }
    else

    {
        SHR_EXIT();
    }

exit:
    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

