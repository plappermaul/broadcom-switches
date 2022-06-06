
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SOCDNX_GENERAL

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_fc.h>
#include <bcm_int/dnx/cosq/flow_control/cosq_fc.h>
#include <bcm_int/dnx/cosq/flow_control/flow_control_imp.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_nif.h>
#include <soc/dnx/intr/auto_generated/j2x/j2x_intr.h>
#include <shared/utilex/utilex_integer_arithmetic.h>







static shr_error_e
j2x_a0_dnx_data_fc_general_fc_to_nif_status_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_fc_to_nif_status;
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
j2x_a0_dnx_data_fc_general_ilu_fc_status_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilu_fc_status;
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
j2x_a0_dnx_data_fc_general_ilu_fc_status_per_tdm_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilu_fc_status_per_tdm;
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
j2x_a0_dnx_data_fc_general_inb_ilkn_rx_status_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_inb_ilkn_rx_status;
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
j2x_a0_dnx_data_fc_general_glb_res_lp_to_llfc_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_glb_res_lp_to_llfc;
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
j2x_a0_dnx_data_fc_general_vsqd_to_llfc_pfc_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_vsqd_to_llfc_pfc;
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
j2x_a0_dnx_data_fc_general_vsq_thr_en_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_vsq_thr_en;
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
j2x_a0_dnx_data_fc_general_oob_tx_reset_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_oob_tx_reset;
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
j2x_a0_dnx_data_fc_general_additional_enablers_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_additional_enablers;
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
j2x_a0_dnx_data_fc_general_ilkn_inb_llfc_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilkn_inb_llfc;
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
j2x_a0_dnx_data_fc_general_flexe_flow_control_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_flexe_flow_control;
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
j2x_a0_dnx_data_fc_general_glb_rsc_oob_en_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_glb_rsc_oob_en;
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
j2x_a0_dnx_data_fc_general_cat2_oob_en_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_cat2_oob_en;
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
j2x_a0_dnx_data_fc_general_pfc_rec_cdu_shift_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_pfc_rec_cdu_shift;
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
j2x_a0_dnx_data_fc_general_pfc_status_regs_double_read_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_pfc_status_regs_double_read;
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
j2x_a0_dnx_data_fc_general_ilkn_oob_fast_llfc_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilkn_oob_fast_llfc;
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
j2x_a0_dnx_data_fc_general_pp_inb_enable_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_pp_inb_enable;
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
j2x_a0_dnx_data_fc_general_fc_from_llfc_vsq_status_sel_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_fc_from_llfc_vsq_status_sel;
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
j2x_a0_dnx_data_fc_general_if_2_cfc_enablers_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_if_2_cfc_enablers;
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
j2x_a0_dnx_data_fc_general_ilkn_oob_en_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilkn_oob_en;
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
j2x_a0_dnx_data_fc_general_ilkn_polarity_set_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilkn_polarity_set;
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
j2x_a0_dnx_data_fc_general_additional_rx_pfc_bmp_types_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_additional_rx_pfc_bmp_types;
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
j2x_a0_dnx_data_fc_general_disable_pfc_mask_on_link_fail_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_disable_pfc_mask_on_link_fail;
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
j2x_a0_dnx_data_fc_general_gen_pfc_from_llfc_bitmap_per_type_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_gen_pfc_from_llfc_bitmap_per_type;
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
j2x_a0_dnx_data_fc_general_llfc_handled_in_cfc_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_llfc_handled_in_cfc;
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
j2x_a0_dnx_data_fc_general_coe_vid_mapping_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_coe_vid_mapping;
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
j2x_a0_dnx_data_fc_general_ilkn_inband_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_ilkn_inband_support;
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
j2x_a0_dnx_data_fc_general_oob_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_oob_support;
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
j2x_a0_dnx_data_fc_general_coe_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_coe_support;
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
j2x_a0_dnx_data_fc_general_nif_pfc_bitmaps_e_f_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_nif_pfc_bitmaps_e_f_support;
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
j2x_a0_dnx_data_fc_general_qmlf_fc_high_and_low_rmcs_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_qmlf_fc_high_and_low_rmcs_support;
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
j2x_a0_dnx_data_fc_general_qmlf_rmcs_thresholds_per_profile_support_set(
    int unit)
{
    dnxc_data_feature_t *feature;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int feature_index = dnx_data_fc_general_qmlf_rmcs_thresholds_per_profile_support;
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
j2x_a0_dnx_data_fc_general_nof_glb_rsc_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_glb_rsc;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 14;

    
    define->data = 14;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_gen_bmps_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_gen_bmps;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 32;

    
    define->data = 32;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_gen_bmp_first_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_gen_bmp_first_id;
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
j2x_a0_dnx_data_fc_general_nof_af_nif_indications_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_af_nif_indications;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = dnx_data_nif.phys.nof_phys_get(unit);

    
    define->data = dnx_data_nif.phys.nof_phys_get(unit);

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_cal_llfcs_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_cal_llfcs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = dnx_data_nif.phys.nof_phys_get(unit) + 4;

    
    define->data = dnx_data_nif.phys.nof_phys_get(unit) + 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_entries_qmlf_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_entries_qmlf;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 65535;

    
    define->data = 65535;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_bits_in_pfc_status_select_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_bits_in_pfc_status_select;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4;

    
    define->data = 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_rmc_thresholds_profiles_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_rmc_thresholds_profiles;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 6;

    
    define->data = 6;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_rmc_pfc_set_threshold_default_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_rmc_pfc_set_threshold_default;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 300;

    
    define->data = 300;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_rmc_pfc_clear_threshold_default_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_rmc_pfc_clear_threshold_default;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 292;

    
    define->data = 292;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_rmc_llfc_set_threshold_default_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_rmc_llfc_set_threshold_default;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 512;

    
    define->data = 512;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_rmc_llfc_clear_threshold_default_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_rmc_llfc_clear_threshold_default;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 504;

    
    define->data = 504;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_general_nof_bits_in_nif_eth_rmc_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_general;
    int define_index = dnx_data_fc_general_define_nof_bits_in_nif_eth_rmc_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = utilex_log2_round_up(dnx_data_nif.phys.nof_phys_get(unit));

    
    define->data = utilex_log2_round_up(dnx_data_nif.phys.nof_phys_get(unit));

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_rx_dest_pfc_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_pfc;
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
j2x_a0_dnx_data_fc_calendar_common_rx_dest_nif_llfc_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_nif_llfc;
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
j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_2_prio_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_port_2_prio;
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
j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_4_prio_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_port_4_prio;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 3;

    
    define->data = 3;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_8_prio_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_port_8_prio;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4;

    
    define->data = 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_rx_dest_pfc_generic_bmp_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_pfc_generic_bmp;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 5;

    
    define->data = 5;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_rx_dest_empty_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_rx_dest_empty;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 6;

    
    define->data = 6;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_tx_src_vsq_a_b_c_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_vsq_a_b_c;
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
j2x_a0_dnx_data_fc_calendar_common_tx_src_vsq_d_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_vsq_d;
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
j2x_a0_dnx_data_fc_calendar_common_tx_src_llfc_vsq_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_llfc_vsq;
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
j2x_a0_dnx_data_fc_calendar_common_tx_src_pfc_vsq_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_pfc_vsq;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 3;

    
    define->data = 3;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_tx_src_glb_rsc_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_glb_rsc;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4;

    
    define->data = 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_tx_src_nif_llfc_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_nif_llfc;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 6;

    
    define->data = 6;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_calendar_common_tx_src_empty_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_calendar_common;
    int define_index = dnx_data_fc_calendar_common_define_tx_src_empty;
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
j2x_a0_dnx_data_fc_coe_nof_coe_cal_instances_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_coe_cal_instances;
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
j2x_a0_dnx_data_fc_coe_nof_supported_chan_ids_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_supported_chan_ids;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4096;

    
    define->data = 4096;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_nof_coe_vid_mask_width_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_coe_vid_mask_width;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 10;

    
    define->data = 10;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_nof_coe_vid_right_shift_width_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_coe_vid_right_shift_width;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4;

    
    define->data = 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_nof_coe_ptc_mask_width_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_coe_ptc_mask_width;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 12;

    
    define->data = 12;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_nof_coe_ptc_left_shift_width_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_nof_coe_ptc_left_shift_width;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 4;

    
    define->data = 4;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_max_coe_ticks_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_max_coe_ticks;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 1048575;

    
    define->data = 1048575;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_coe_fc_support_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_fc_support;
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
j2x_a0_dnx_data_fc_coe_map_value_size_in_bits_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_coe;
    int define_index = dnx_data_fc_coe_define_map_value_size_in_bits;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 10;

    
    define->data = 10;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}







static shr_error_e
j2x_a0_dnx_data_fc_inband_nof_inband_intlkn_cal_instances_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_inband;
    int define_index = dnx_data_fc_inband_define_nof_inband_intlkn_cal_instances;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = dnx_data_nif.ilkn.ilkn_if_nof_get(unit);

    
    define->data = dnx_data_nif.ilkn.ilkn_if_nof_get(unit);

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}



static shr_error_e
j2x_a0_dnx_data_fc_inband_calender_length_set(
    int unit)
{
    int ilkn_id_index;
    dnx_data_fc_inband_calender_length_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_inband;
    int table_index = dnx_data_fc_inband_table_calender_length;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 2;
    table->info_get.key_size[0] = 2;

    
    table->values[0].default_val = "256";
    table->values[1].default_val = "256";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_fc_inband_calender_length_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_fc_inband_table_calender_length");

    
    default_data = (dnx_data_fc_inband_calender_length_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->tx = 256;
    default_data->rx = 256;
    
    for (ilkn_id_index = 0; ilkn_id_index < table->keys[0].size; ilkn_id_index++)
    {
        data = (dnx_data_fc_inband_calender_length_t *) dnxc_data_mgmt_table_data_get(unit, table, ilkn_id_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    table->property.name = "fc_inband_intlkn_calender_length";
    table->property.doc =
        "\n"
    ;
    table->property.method = dnxc_data_property_method_custom;
    table->property.method_str = "custom";

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));

    for (ilkn_id_index = 0; ilkn_id_index < table->keys[0].size; ilkn_id_index++)
    {
        data = (dnx_data_fc_inband_calender_length_t *) dnxc_data_mgmt_table_data_get(unit, table, ilkn_id_index, 0);
        SHR_IF_ERR_EXIT(dnx_data_property_fc_inband_calender_length_read(unit, ilkn_id_index, data));
    }
    
    table->flags |= (DNXC_DATA_F_PROPERTY);

exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_inband_port_set(
    int unit)
{
    int ilkn_id_index;
    dnx_data_fc_inband_port_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_inband;
    int table_index = dnx_data_fc_inband_table_port;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 2;
    table->info_get.key_size[0] = 2;

    
    table->values[0].default_val = "DNX_FC_CAL_MODE_DISABLE";
    table->values[1].default_val = "DNX_FC_CAL_MODE_DISABLE";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_fc_inband_port_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_fc_inband_table_port");

    
    default_data = (dnx_data_fc_inband_port_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->mode = DNX_FC_CAL_MODE_DISABLE;
    default_data->llfc_mode = DNX_FC_CAL_MODE_DISABLE;
    
    for (ilkn_id_index = 0; ilkn_id_index < table->keys[0].size; ilkn_id_index++)
    {
        data = (dnx_data_fc_inband_port_t *) dnxc_data_mgmt_table_data_get(unit, table, ilkn_id_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    table->values[0].property.name = spn_FC_INBAND_INTLKN_MODE;
    table->values[0].property.doc =
        "\n"
    ;
    table->values[0].property.method = dnxc_data_property_method_port_direct_map;
    table->values[0].property.method_str = "port_direct_map";
    table->values[0].property.nof_mapping = 4;
    DNXC_DATA_ALLOC(table->values[0].property.mapping, dnxc_data_property_map_t, table->values[0].property.nof_mapping, "dnx_data_fc_inband_port_t property mapping");

    table->values[0].property.mapping[0].name = "0";
    table->values[0].property.mapping[0].val = DNX_FC_CAL_MODE_DISABLE;
    table->values[0].property.mapping[0].is_default = 1 ;
    table->values[0].property.mapping[1].name = "1";
    table->values[0].property.mapping[1].val = DNX_FC_CAL_MODE_RX_ENABLE;
    table->values[0].property.mapping[2].name = "2";
    table->values[0].property.mapping[2].val = DNX_FC_CAL_MODE_TX_ENABLE;
    table->values[0].property.mapping[3].name = "3";
    table->values[0].property.mapping[3].val = DNX_FC_CAL_MODE_TX_RX_ENABLE;
    
    table->values[1].property.name = spn_FC_INBAND_INTLKN_CALENDER_LLFC_MODE;
    table->values[1].property.doc =
        "\n"
    ;
    table->values[1].property.method = dnxc_data_property_method_port_direct_map;
    table->values[1].property.method_str = "port_direct_map";
    table->values[1].property.nof_mapping = 3;
    DNXC_DATA_ALLOC(table->values[1].property.mapping, dnxc_data_property_map_t, table->values[1].property.nof_mapping, "dnx_data_fc_inband_port_t property mapping");

    table->values[1].property.mapping[0].name = "0";
    table->values[1].property.mapping[0].val = DNX_FC_INB_ILKN_CAL_LLFC_MODE_DISABLE;
    table->values[1].property.mapping[1].name = "1";
    table->values[1].property.mapping[1].val = DNX_FC_INB_ILKN_CAL_LLFC_MODE_CH_0_ONLY;
    table->values[1].property.mapping[2].name = "2";
    table->values[1].property.mapping[2].val = DNX_FC_INB_ILKN_CAL_LLFC_MODE_EVERY_16_CH;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));

    for (ilkn_id_index = 0; ilkn_id_index < table->keys[0].size; ilkn_id_index++)
    {
        data = (dnx_data_fc_inband_port_t *) dnxc_data_mgmt_table_data_get(unit, table, ilkn_id_index, 0);
        table->values[0].property.skip_is_active_check = TRUE;
        SHR_IF_ERR_EXIT(dnxc_data_mgmt_property_read(unit, &table->values[0].property, ilkn_id_index, &data->mode));
    }
    for (ilkn_id_index = 0; ilkn_id_index < table->keys[0].size; ilkn_id_index++)
    {
        data = (dnx_data_fc_inband_port_t *) dnxc_data_mgmt_table_data_get(unit, table, ilkn_id_index, 0);
        table->values[1].property.skip_is_active_check = TRUE;
        SHR_IF_ERR_EXIT(dnxc_data_mgmt_property_read(unit, &table->values[1].property, ilkn_id_index, &data->llfc_mode));
    }
    
    table->flags |= (DNXC_DATA_F_PROPERTY);

exit:
    SHR_FUNC_EXIT;
}






static shr_error_e
j2x_a0_dnx_data_fc_pfc_deadlock_cdum_intr_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_pfc_deadlock;
    int define_index = dnx_data_fc_pfc_deadlock_define_cdum_intr_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = J2X_INT_LAST;

    
    define->data = J2X_INT_LAST;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_pfc_deadlock_cdu_intr_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_pfc_deadlock;
    int define_index = dnx_data_fc_pfc_deadlock_define_cdu_intr_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = J2X_INT_LAST;

    
    define->data = J2X_INT_LAST;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_pfc_deadlock_clu_intr_id_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_pfc_deadlock;
    int define_index = dnx_data_fc_pfc_deadlock_define_clu_intr_id;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = J2X_INT_LAST;

    
    define->data = J2X_INT_LAST;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_fc_pfc_deadlock_nof_bits_in_port_num_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_fc;
    int submodule_index = dnx_data_fc_submodule_pfc_deadlock;
    int define_index = dnx_data_fc_pfc_deadlock_define_nof_bits_in_port_num;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 6;

    
    define->data = 6;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}




shr_error_e
j2x_a0_data_fc_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_fc;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_fc_submodule_general;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_fc_general_define_nof_glb_rsc;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_glb_rsc_set;
    data_index = dnx_data_fc_general_define_nof_gen_bmps;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_gen_bmps_set;
    data_index = dnx_data_fc_general_define_gen_bmp_first_id;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_gen_bmp_first_id_set;
    data_index = dnx_data_fc_general_define_nof_af_nif_indications;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_af_nif_indications_set;
    data_index = dnx_data_fc_general_define_nof_cal_llfcs;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_cal_llfcs_set;
    data_index = dnx_data_fc_general_define_nof_entries_qmlf;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_entries_qmlf_set;
    data_index = dnx_data_fc_general_define_nof_bits_in_pfc_status_select;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_bits_in_pfc_status_select_set;
    data_index = dnx_data_fc_general_define_nof_rmc_thresholds_profiles;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_rmc_thresholds_profiles_set;
    data_index = dnx_data_fc_general_define_rmc_pfc_set_threshold_default;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_rmc_pfc_set_threshold_default_set;
    data_index = dnx_data_fc_general_define_rmc_pfc_clear_threshold_default;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_rmc_pfc_clear_threshold_default_set;
    data_index = dnx_data_fc_general_define_rmc_llfc_set_threshold_default;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_rmc_llfc_set_threshold_default_set;
    data_index = dnx_data_fc_general_define_rmc_llfc_clear_threshold_default;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_rmc_llfc_clear_threshold_default_set;
    data_index = dnx_data_fc_general_define_nof_bits_in_nif_eth_rmc_id;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_general_nof_bits_in_nif_eth_rmc_id_set;

    
    data_index = dnx_data_fc_general_fc_to_nif_status;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_fc_to_nif_status_set;
    data_index = dnx_data_fc_general_ilu_fc_status;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilu_fc_status_set;
    data_index = dnx_data_fc_general_ilu_fc_status_per_tdm;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilu_fc_status_per_tdm_set;
    data_index = dnx_data_fc_general_inb_ilkn_rx_status;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_inb_ilkn_rx_status_set;
    data_index = dnx_data_fc_general_glb_res_lp_to_llfc;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_glb_res_lp_to_llfc_set;
    data_index = dnx_data_fc_general_vsqd_to_llfc_pfc;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_vsqd_to_llfc_pfc_set;
    data_index = dnx_data_fc_general_vsq_thr_en;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_vsq_thr_en_set;
    data_index = dnx_data_fc_general_oob_tx_reset;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_oob_tx_reset_set;
    data_index = dnx_data_fc_general_additional_enablers;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_additional_enablers_set;
    data_index = dnx_data_fc_general_ilkn_inb_llfc;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilkn_inb_llfc_set;
    data_index = dnx_data_fc_general_flexe_flow_control;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_flexe_flow_control_set;
    data_index = dnx_data_fc_general_glb_rsc_oob_en;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_glb_rsc_oob_en_set;
    data_index = dnx_data_fc_general_cat2_oob_en;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_cat2_oob_en_set;
    data_index = dnx_data_fc_general_pfc_rec_cdu_shift;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_pfc_rec_cdu_shift_set;
    data_index = dnx_data_fc_general_pfc_status_regs_double_read;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_pfc_status_regs_double_read_set;
    data_index = dnx_data_fc_general_ilkn_oob_fast_llfc;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilkn_oob_fast_llfc_set;
    data_index = dnx_data_fc_general_pp_inb_enable;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_pp_inb_enable_set;
    data_index = dnx_data_fc_general_fc_from_llfc_vsq_status_sel;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_fc_from_llfc_vsq_status_sel_set;
    data_index = dnx_data_fc_general_if_2_cfc_enablers;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_if_2_cfc_enablers_set;
    data_index = dnx_data_fc_general_ilkn_oob_en;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilkn_oob_en_set;
    data_index = dnx_data_fc_general_ilkn_polarity_set;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilkn_polarity_set_set;
    data_index = dnx_data_fc_general_additional_rx_pfc_bmp_types;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_additional_rx_pfc_bmp_types_set;
    data_index = dnx_data_fc_general_disable_pfc_mask_on_link_fail;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_disable_pfc_mask_on_link_fail_set;
    data_index = dnx_data_fc_general_gen_pfc_from_llfc_bitmap_per_type;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_gen_pfc_from_llfc_bitmap_per_type_set;
    data_index = dnx_data_fc_general_llfc_handled_in_cfc;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_llfc_handled_in_cfc_set;
    data_index = dnx_data_fc_general_coe_vid_mapping;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_coe_vid_mapping_set;
    data_index = dnx_data_fc_general_ilkn_inband_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_ilkn_inband_support_set;
    data_index = dnx_data_fc_general_oob_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_oob_support_set;
    data_index = dnx_data_fc_general_coe_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_coe_support_set;
    data_index = dnx_data_fc_general_nif_pfc_bitmaps_e_f_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_nif_pfc_bitmaps_e_f_support_set;
    data_index = dnx_data_fc_general_qmlf_fc_high_and_low_rmcs_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_qmlf_fc_high_and_low_rmcs_support_set;
    data_index = dnx_data_fc_general_qmlf_rmcs_thresholds_per_profile_support;
    feature = &submodule->features[data_index];
    feature->set = j2x_a0_dnx_data_fc_general_qmlf_rmcs_thresholds_per_profile_support_set;

    
    
    submodule_index = dnx_data_fc_submodule_calendar_common;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_fc_calendar_common_define_rx_dest_pfc;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_pfc_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_nif_llfc;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_nif_llfc_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_port_2_prio;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_2_prio_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_port_4_prio;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_4_prio_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_port_8_prio;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_port_8_prio_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_pfc_generic_bmp;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_pfc_generic_bmp_set;
    data_index = dnx_data_fc_calendar_common_define_rx_dest_empty;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_rx_dest_empty_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_vsq_a_b_c;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_vsq_a_b_c_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_vsq_d;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_vsq_d_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_llfc_vsq;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_llfc_vsq_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_pfc_vsq;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_pfc_vsq_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_glb_rsc;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_glb_rsc_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_nif_llfc;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_nif_llfc_set;
    data_index = dnx_data_fc_calendar_common_define_tx_src_empty;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_calendar_common_tx_src_empty_set;

    

    
    
    submodule_index = dnx_data_fc_submodule_coe;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_fc_coe_define_nof_coe_cal_instances;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_coe_cal_instances_set;
    data_index = dnx_data_fc_coe_define_nof_supported_chan_ids;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_supported_chan_ids_set;
    data_index = dnx_data_fc_coe_define_nof_coe_vid_mask_width;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_coe_vid_mask_width_set;
    data_index = dnx_data_fc_coe_define_nof_coe_vid_right_shift_width;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_coe_vid_right_shift_width_set;
    data_index = dnx_data_fc_coe_define_nof_coe_ptc_mask_width;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_coe_ptc_mask_width_set;
    data_index = dnx_data_fc_coe_define_nof_coe_ptc_left_shift_width;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_nof_coe_ptc_left_shift_width_set;
    data_index = dnx_data_fc_coe_define_max_coe_ticks;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_max_coe_ticks_set;
    data_index = dnx_data_fc_coe_define_fc_support;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_fc_support_set;
    data_index = dnx_data_fc_coe_define_map_value_size_in_bits;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_coe_map_value_size_in_bits_set;

    

    
    
    submodule_index = dnx_data_fc_submodule_inband;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_fc_inband_define_nof_inband_intlkn_cal_instances;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_inband_nof_inband_intlkn_cal_instances_set;

    

    
    data_index = dnx_data_fc_inband_table_calender_length;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_fc_inband_calender_length_set;
    data_index = dnx_data_fc_inband_table_port;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_fc_inband_port_set;
    
    submodule_index = dnx_data_fc_submodule_pfc_deadlock;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_fc_pfc_deadlock_define_cdum_intr_id;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_pfc_deadlock_cdum_intr_id_set;
    data_index = dnx_data_fc_pfc_deadlock_define_cdu_intr_id;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_pfc_deadlock_cdu_intr_id_set;
    data_index = dnx_data_fc_pfc_deadlock_define_clu_intr_id;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_pfc_deadlock_clu_intr_id_set;
    data_index = dnx_data_fc_pfc_deadlock_define_nof_bits_in_port_num;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_fc_pfc_deadlock_nof_bits_in_port_num_set;

    

    

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

