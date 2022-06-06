
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_INTR

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_intr.h>
#include <soc/dnx/intr/auto_generated/j2x/j2x_intr.h>
#include <soc/mcm/allenum.h>
#include <soc/dnxc/dnxc_intr_corr_act_func.h>








static shr_error_e
j2x_a0_dnx_data_intr_general_nof_interrupts_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int define_index = dnx_data_intr_general_define_nof_interrupts;
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
j2x_a0_dnx_data_intr_general_epni_ebd_interrupt_assert_enable_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int define_index = dnx_data_intr_general_define_epni_ebd_interrupt_assert_enable;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = 1;

    
    define->data = 1;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;
    
    define->property.name = spn_CUSTOM_FEATURE;
    define->property.doc = 
        "\n"
    ;
    define->property.method = dnxc_data_property_method_suffix_enable;
    define->property.method_str = "suffix_enable";
    define->property.suffix = "epni_ebd_interrupt_assert_enable";

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));

    SHR_IF_ERR_EXIT(dnxc_data_mgmt_property_read(unit, &define->property, -1, &define->data));
    
    define->flags |= (DNXC_DATA_F_PROPERTY);

exit:
    SHR_FUNC_EXIT;
}



static shr_error_e
j2x_a0_dnx_data_intr_general_active_on_init_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_active_on_init_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_active_on_init;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 477;
    table->info_get.key_size[0] = 477;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_active_on_init_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_active_on_init");

    
    default_data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = J2X_INT_ARB_ERROR_ECC;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2X_INT_BDM_ERROR_ECC;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->intr = J2X_INT_CDPM_LINK_STATUS_CHANGE_INT;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->intr = J2X_INT_CDPM_FRMR_W_40_DEBUG_INT;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->intr = J2X_INT_CFC_ERROR_ECC;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->intr = J2X_INT_CGM_ERROR_ECC;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->intr = J2X_INT_CGM_VOQ_OCCUPANCY_ERR_INT;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->intr = J2X_INT_CGM_VSQ_OCCUPANCY_ERR_INT;
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->intr = J2X_INT_CGM_VOQ_TOTAL_OCCUPANCY_ERR_INT;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->intr = J2X_INT_CGM_VSQ_TOTAL_OCCUPANCY_ERR_INT;
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->intr = J2X_INT_CGM_CONGESTION_INT;
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->intr = J2X_INT_CGM_LOW_PRIORITY_INT;
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->intr = J2X_INT_CGM_FIFO_ERR_INT;
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->intr = J2X_INT_CRPS_ERROR_ECC;
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->intr = J2X_INT_CRPS_ERROR_LM_COLLIDE;
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->intr = J2X_INT_CRPS_ERROR_COUNTER_OVF;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->intr = J2X_INT_CRPS_DMA_0_FULLNESS_EVENT;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->intr = J2X_INT_CRPS_DMA_1_FULLNESS_EVENT;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->intr = J2X_INT_CRPS_CONDITIONAL_TRIGGER_OCCURED;
    }
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->intr = J2X_INT_CRPS_CONDITIONAL_TRIGGER_WHILE_FIFO_IS_FULL;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->intr = J2X_INT_CRPS_FIFOS_OVERFLOW;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->intr = J2X_INT_CRPS_FIFOS_UNDERFLOW;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->intr = J2X_INT_DDHA_ERROR_ECC;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->intr = J2X_INT_DDHB_ERROR_ECC;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->intr = J2X_INT_DDP_ERROR_ECC;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->intr = J2X_INT_DDP_ERROR_EXTERNAL_MEM;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->intr = J2X_INT_DDP_FIFO_ERROR;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->intr = J2X_INT_DDP_ERROR_ITE;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->intr = J2X_INT_DDP_ERROR_ITE_FIFO;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->intr = J2X_INT_DDP_ERROR_BDBC;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->intr = J2X_INT_DQM_ERROR_ECC;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->intr = J2X_INT_DQM_TX_FIFOS_ERR_INT;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->intr = J2X_INT_DQM_FIFO_ERR_INT;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->intr = J2X_INT_ECGM_ERROR_ECC;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->intr = J2X_INT_ECGM_CGM_REP_AROUND_INT_VEC;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->intr = J2X_INT_ECI_ERROR_ECC;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->intr = J2X_INT_EDB_ERROR_ECC;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->intr = J2X_INT_EPNI_ERROR_ECC;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->intr = J2X_INT_EPRE_ERROR_ECC;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->intr = J2X_INT_EPS_ERROR_ECC;
    }
    if (40 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 40, 0);
        data->intr = J2X_INT_EPS_FIFO_STATUS_INTERRUPT_REG;
    }
    if (41 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 41, 0);
        data->intr = J2X_INT_ERPP_ERROR_ECC;
    }
    if (42 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 42, 0);
        data->intr = J2X_INT_ERPP_ERPP_DISCARD_INT_VEC;
    }
    if (43 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 43, 0);
        data->intr = J2X_INT_ERPP_ERPP_DISCARD_INT_VEC_2;
    }
    if (44 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 44, 0);
        data->intr = J2X_INT_ESB_ERROR_ECC;
    }
    if (45 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 45, 0);
        data->intr = J2X_INT_ETPPA_ERROR_ECC;
    }
    if (46 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 46, 0);
        data->intr = J2X_INT_ETPPA_ETPP_PRP_INT_VEC;
    }
    if (47 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 47, 0);
        data->intr = J2X_INT_ETPPA_ETPPA_INT_VEC;
    }
    if (48 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 48, 0);
        data->intr = J2X_INT_ETPPB_ERROR_ECC;
    }
    if (49 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 49, 0);
        data->intr = J2X_INT_ETPPB_BTC_INT_VEC;
    }
    if (50 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 50, 0);
        data->intr = J2X_INT_ETPPB_ETPPB_INT_VEC;
    }
    if (51 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 51, 0);
        data->intr = J2X_INT_ETPPC_ERROR_ECC;
    }
    if (52 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 52, 0);
        data->intr = J2X_INT_ETPPC_ETPP_TERM_INT_VEC;
    }
    if (53 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 53, 0);
        data->intr = J2X_INT_EVNT_ERROR_ECC;
    }
    if (54 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 54, 0);
        data->intr = J2X_INT_EVNT_EVENTOR_INTERRUPT;
    }
    if (55 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 55, 0);
        data->intr = J2X_INT_FAWR_ERROR_ECC;
    }
    if (56 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 56, 0);
        data->intr = J2X_INT_FAWT_ERROR_ECC;
    }
    if (57 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 57, 0);
        data->intr = J2X_INT_FBSW_ERROR_ECC;
    }
    if (58 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 58, 0);
        data->intr = J2X_INT_FCR_ERROR_ECC;
    }
    if (59 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 59, 0);
        data->intr = J2X_INT_FCT_ERROR_ECC;
    }
    if (60 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 60, 0);
        data->intr = J2X_INT_FDA_ERROR_ECC;
    }
    if (61 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 61, 0);
        data->intr = J2X_INT_FDCRL_ERROR_ECC;
    }
    if (62 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 62, 0);
        data->intr = J2X_INT_FDCRS_ERROR_ECC;
    }
    if (63 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 63, 0);
        data->intr = J2X_INT_FDMP_ERROR_ECC;
    }
    if (64 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 64, 0);
        data->intr = J2X_INT_FDMX_ERROR_ECC;
    }
    if (65 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 65, 0);
        data->intr = J2X_INT_FDMXA_ERROR_ECC;
    }
    if (66 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 66, 0);
        data->intr = J2X_INT_FDMXBI_ERROR_ECC;
    }
    if (67 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 67, 0);
        data->intr = J2X_INT_FDMXBJ_ERROR_ECC;
    }
    if (68 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 68, 0);
        data->intr = J2X_INT_FDMXC_ERROR_ECC;
    }
    if (69 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 69, 0);
        data->intr = J2X_INT_FDMXT_ERROR_ECC;
    }
    if (70 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 70, 0);
        data->intr = J2X_INT_FDR_ERROR_ECC;
    }
    if (71 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 71, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_0;
    }
    if (72 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 72, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_1;
    }
    if (73 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 73, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_2;
    }
    if (74 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 74, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_3;
    }
    if (75 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 75, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_4;
    }
    if (76 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 76, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_5;
    }
    if (77 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 77, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_6;
    }
    if (78 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 78, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_1_MAC_7;
    }
    if (79 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 79, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_0;
    }
    if (80 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 80, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_1;
    }
    if (81 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 81, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_2;
    }
    if (82 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 82, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_3;
    }
    if (83 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 83, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_4;
    }
    if (84 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 84, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_5;
    }
    if (85 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 85, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_6;
    }
    if (86 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 86, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_2_MAC_7;
    }
    if (87 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 87, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_0;
    }
    if (88 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 88, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_1;
    }
    if (89 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 89, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_2;
    }
    if (90 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 90, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_3;
    }
    if (91 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 91, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_4;
    }
    if (92 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 92, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_5;
    }
    if (93 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 93, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_6;
    }
    if (94 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 94, 0);
        data->intr = J2X_INT_FDR_INT_REG_P_3_MAC_7;
    }
    if (95 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 95, 0);
        data->intr = J2X_INT_FDR_INT_REG_GENERAL;
    }
    if (96 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 96, 0);
        data->intr = J2X_INT_FDT_ERROR_ECC;
    }
    if (97 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 97, 0);
        data->intr = J2X_INT_FDTL_ERROR_ECC;
    }
    if (98 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 98, 0);
        data->intr = J2X_INT_FECPB_ERROR_ECC;
    }
    if (99 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 99, 0);
        data->intr = J2X_INT_FECRL_ERROR_ECC;
    }
    if (100 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 100, 0);
        data->intr = J2X_INT_FECRS_ERROR_ECC;
    }
    if (101 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 101, 0);
        data->intr = J2X_INT_FFLXO_ERROR_ECC;
    }
    if (102 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 102, 0);
        data->intr = J2X_INT_FFOA_ERROR_ECC;
    }
    if (103 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 103, 0);
        data->intr = J2X_INT_FFOB_ERROR_ECC;
    }
    if (104 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 104, 0);
        data->intr = J2X_INT_FGDMP_ERROR_ECC;
    }
    if (105 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 105, 0);
        data->intr = J2X_INT_FGMAP_ERROR_ECC;
    }
    if (106 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 106, 0);
        data->intr = J2X_INT_FICPB_ERROR_ECC;
    }
    if (107 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 107, 0);
        data->intr = J2X_INT_FLEXMAC_ERROR_ECC;
    }
    if (108 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 108, 0);
        data->intr = J2X_INT_FLFR_ERROR_ECC;
    }
    if (109 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 109, 0);
        data->intr = J2X_INT_FLFT_ERROR_ECC;
    }
    if (110 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 110, 0);
        data->intr = J2X_INT_FLOTN_ERROR_ECC;
    }
    if (111 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 111, 0);
        data->intr = J2X_INT_FMAC_ERROR_ECC;
    }
    if (112 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 112, 0);
        data->intr = J2X_INT_FMAC_INT_REG_1;
    }
    if (113 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 113, 0);
        data->intr = J2X_INT_FMAC_INT_REG_2;
    }
    if (114 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 114, 0);
        data->intr = J2X_INT_FMAC_INT_REG_3;
    }
    if (115 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 115, 0);
        data->intr = J2X_INT_FMAC_INT_REG_4;
    }
    if (116 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 116, 0);
        data->intr = J2X_INT_FMAC_INT_REG_5;
    }
    if (117 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 117, 0);
        data->intr = J2X_INT_FMAC_INT_REG_6;
    }
    if (118 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 118, 0);
        data->intr = J2X_INT_FMAC_INT_REG_7;
    }
    if (119 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 119, 0);
        data->intr = J2X_INT_FMAC_INT_REG_8;
    }
    if (120 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 120, 0);
        data->intr = J2X_INT_FMACR_ERROR_ECC;
    }
    if (121 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 121, 0);
        data->intr = J2X_INT_FMACT_ERROR_ECC;
    }
    if (122 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 122, 0);
        data->intr = J2X_INT_FMAP_ERROR_ECC;
    }
    if (123 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 123, 0);
        data->intr = J2X_INT_FMX_ERROR_ECC;
    }
    if (124 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 124, 0);
        data->intr = J2X_INT_FMXAI_ERROR_ECC;
    }
    if (125 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 125, 0);
        data->intr = J2X_INT_FMXAJ_ERROR_ECC;
    }
    if (126 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 126, 0);
        data->intr = J2X_INT_FMXBI_ERROR_ECC;
    }
    if (127 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 127, 0);
        data->intr = J2X_INT_FMXBJ_ERROR_ECC;
    }
    if (128 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 128, 0);
        data->intr = J2X_INT_FMXBK_ERROR_ECC;
    }
    if (129 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 129, 0);
        data->intr = J2X_INT_FMXC_ERROR_ECC;
    }
    if (130 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 130, 0);
        data->intr = J2X_INT_FMXT_ERROR_ECC;
    }
    if (131 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 131, 0);
        data->intr = J2X_INT_FOAM_ERROR_ECC;
    }
    if (132 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 132, 0);
        data->intr = J2X_INT_FODUO_ERROR_ECC;
    }
    if (133 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 133, 0);
        data->intr = J2X_INT_FOSW_ERROR_ECC;
    }
    if (134 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 134, 0);
        data->intr = J2X_INT_FPMR_ERROR_ECC;
    }
    if (135 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 135, 0);
        data->intr = J2X_INT_FPMT_ERROR_ECC;
    }
    if (136 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 136, 0);
        data->intr = J2X_INT_FPTPR_ERROR_ECC;
    }
    if (137 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 137, 0);
        data->intr = J2X_INT_FPTPT_ERROR_ECC;
    }
    if (138 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 138, 0);
        data->intr = J2X_INT_FQP_ERROR_ECC;
    }
    if (139 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 139, 0);
        data->intr = J2X_INT_FRA_ERROR_ECC;
    }
    if (140 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 140, 0);
        data->intr = J2X_INT_FRWA_ERROR_ECC;
    }
    if (141 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 141, 0);
        data->intr = J2X_INT_FRWBC_ERROR_ECC;
    }
    if (142 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 142, 0);
        data->intr = J2X_INT_FSAR_ERROR_ECC;
    }
    if (143 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 143, 0);
        data->intr = J2X_INT_FSRD_ERROR_ECC;
    }
    if (144 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 144, 0);
        data->intr = J2X_INT_FSRD_INT_REG_0;
    }
    if (145 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 145, 0);
        data->intr = J2X_INT_HBC_ERROR_ECC;
    }
    if (146 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 146, 0);
        data->intr = J2X_INT_HBC_DSI_ERROR;
    }
    if (147 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 147, 0);
        data->intr = J2X_INT_HBC_RDR_ERROR;
    }
    if (148 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 148, 0);
        data->intr = J2X_INT_HBC_PIPELINES_ERROR;
    }
    if (149 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 149, 0);
        data->intr = J2X_INT_HBMC_ERROR_ECC;
    }
    if (150 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 150, 0);
        data->intr = J2X_INT_ILE_ERROR_ECC;
    }
    if (151 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 151, 0);
        data->intr = J2X_INT_ILU_ERROR_ECC;
    }
    if (152 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 152, 0);
        data->intr = J2X_INT_IPPA_ERROR_ECC;
    }
    if (153 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 153, 0);
        data->intr = J2X_INT_IPPB_ERROR_ECC;
    }
    if (154 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 154, 0);
        data->intr = J2X_INT_IPPB_LEL_INTERRUPT_REGISTER_ONE;
    }
    if (155 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 155, 0);
        data->intr = J2X_INT_IPPC_ERROR_ECC;
    }
    if (156 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 156, 0);
        data->intr = J2X_INT_IPPD_ERROR_ECC;
    }
    if (157 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 157, 0);
        data->intr = J2X_INT_IPPE_ERROR_ECC;
    }
    if (158 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 158, 0);
        data->intr = J2X_INT_IPPF_ERROR_ECC;
    }
    if (159 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 159, 0);
        data->intr = J2X_INT_IPS_ERROR_ECC;
    }
    if (160 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 160, 0);
        data->intr = J2X_INT_IPS_ACTIVE_QUEUE_COUNT_ERR_INT;
    }
    if (161 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 161, 0);
        data->intr = J2X_INT_IPS_FIFO_ERR_INT;
    }
    if (162 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 162, 0);
        data->intr = J2X_INT_IPT_ERROR_ECC;
    }
    if (163 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 163, 0);
        data->intr = J2X_INT_IPT_ERROR_FIFOS;
    }
    if (164 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 164, 0);
        data->intr = J2X_INT_IPT_ERROR_ITE;
    }
    if (165 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 165, 0);
        data->intr = J2X_INT_IPT_ERROR_ITE_FIFO;
    }
    if (166 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 166, 0);
        data->intr = J2X_INT_IQM_ERROR_ECC;
    }
    if (167 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 167, 0);
        data->intr = J2X_INT_IQM_FIFO_ERR_INT;
    }
    if (168 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 168, 0);
        data->intr = J2X_INT_IRE_ERROR_ECC;
    }
    if (169 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 169, 0);
        data->intr = J2X_INT_IRE_RCY_INTERFACE_ERROR;
    }
    if (170 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 170, 0);
        data->intr = J2X_INT_IRE_INTERNAL_INTERFACE_ERROR;
    }
    if (171 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 171, 0);
        data->intr = J2X_INT_IRE_NIF_ERROR;
    }
    if (172 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 172, 0);
        data->intr = J2X_INT_IRE_FIFO_ERROR;
    }
    if (173 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 173, 0);
        data->intr = J2X_INT_ITPP_ERROR_ECC;
    }
    if (174 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 174, 0);
        data->intr = J2X_INT_ITPPD_ERROR_ECC;
    }
    if (175 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 175, 0);
        data->intr = J2X_INT_KAPS_ERROR_ECC;
    }
    if (176 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 176, 0);
        data->intr = J2X_INT_MACT_ERROR_ECC;
    }
    if (177 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 177, 0);
        data->intr = J2X_INT_MACT_LOCAL_MACT_INT;
    }
    if (178 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 178, 0);
        data->intr = J2X_INT_MCP_ERROR_ECC;
    }
    if (179 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 179, 0);
        data->intr = J2X_INT_MDB_ERROR_ECC;
    }
    if (180 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 180, 0);
        data->intr = J2X_INT_MRPS_ERROR_ECC;
    }
    if (181 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 181, 0);
        data->intr = J2X_INT_MRPS_INGRESS_ENG_WRAP_ERR_INT;
    }
    if (182 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 182, 0);
        data->intr = J2X_INT_MRPS_EGRESS_ENG_WRAP_ERR_INT;
    }
    if (183 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 183, 0);
        data->intr = J2X_INT_MRPS_INGRESS_ENG_OOP_FIFO_NOT_EMPTY_INT;
    }
    if (184 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 184, 0);
        data->intr = J2X_INT_MRPS_EGRESS_ENG_OOP_FIFO_NOT_EMPTY_INT;
    }
    if (185 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 185, 0);
        data->intr = J2X_INT_MSW_ERROR_ECC;
    }
    if (186 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 186, 0);
        data->intr = J2X_INT_MSW_MACSEC_INT;
    }
    if (187 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 187, 0);
        data->intr = J2X_INT_MTM_ERROR_ECC;
    }
    if (188 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 188, 0);
        data->intr = J2X_INT_OAMP_ERROR_ECC;
    }
    if (189 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 189, 0);
        data->intr = J2X_INT_OCB_ERROR_ECC;
    }
    if (190 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 190, 0);
        data->intr = J2X_INT_OCBM_ERROR_ECC;
    }
    if (191 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 191, 0);
        data->intr = J2X_INT_OCBM_ERROR_FBC_BANK;
    }
    if (192 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 192, 0);
        data->intr = J2X_INT_OCBM_ERROR_FIFO_INT;
    }
    if (193 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 193, 0);
        data->intr = J2X_INT_OFR_ERROR_ECC;
    }
    if (194 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 194, 0);
        data->intr = J2X_INT_OFT_ERROR_ECC;
    }
    if (195 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 195, 0);
        data->intr = J2X_INT_OLP_ERROR_ECC;
    }
    if (196 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 196, 0);
        data->intr = J2X_INT_PDM_ERROR_ECC;
    }
    if (197 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 197, 0);
        data->intr = J2X_INT_PEM_ERROR_ECC;
    }
    if (198 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 198, 0);
        data->intr = J2X_INT_PQP_ERROR_ECC;
    }
    if (199 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 199, 0);
        data->intr = J2X_INT_PQP_FIFO_STATUS_INTERRUPT_REG;
    }
    if (200 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 200, 0);
        data->intr = J2X_INT_RQP_ERROR_ECC;
    }
    if (201 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 201, 0);
        data->intr = J2X_INT_RQP_PKT_REAS_INT_VEC;
    }
    if (202 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 202, 0);
        data->intr = J2X_INT_RQP_PRS_INT_VEC;
    }
    if (203 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 203, 0);
        data->intr = J2X_INT_RQP_ETB_INT_VEC;
    }
    if (204 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 204, 0);
        data->intr = J2X_INT_RTP_ERROR_ECC;
    }
    if (205 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 205, 0);
        data->intr = J2X_INT_SCH_ERROR_ECC;
    }
    if (206 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 206, 0);
        data->intr = J2X_INT_SIF_ERROR_ECC;
    }
    if (207 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 207, 0);
        data->intr = J2X_INT_SIF_STATISTICS_BILLING_FIFOS_ERR_INT;
    }
    if (208 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 208, 0);
        data->intr = J2X_INT_SIF_STATISTICS_BILLING_OPCODE_ERR_INT;
    }
    if (209 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 209, 0);
        data->intr = J2X_INT_SPB_ERROR_ECC;
    }
    if (210 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 210, 0);
        data->intr = J2X_INT_SPB_ERROR_REASSEMBLY;
    }
    if (211 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 211, 0);
        data->intr = J2X_INT_SPB_ERROR_FIFO;
    }
    if (212 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 212, 0);
        data->intr = J2X_INT_SPB_FIFO_ERROR;
    }
    if (213 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 213, 0);
        data->intr = J2X_INT_SQM_ERROR_ECC;
    }
    if (214 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 214, 0);
        data->intr = J2X_INT_SQM_TX_FIFOS_ERR_INT;
    }
    if (215 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 215, 0);
        data->intr = J2X_INT_SQM_FIFO_ERR_INT;
    }
    if (216 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 216, 0);
        data->intr = J2X_INT_TCAM_ERROR_ECC;
    }
    if (217 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 217, 0);
        data->intr = J2X_INT_TDU_ERROR_ECC;
    }
    if (218 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 218, 0);
        data->intr = J2X_INT_TSN_ERROR_ECC;
    }
    if (219 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 219, 0);
        data->intr = J2X_INT_W40_W_40_FLR;
    }
    if (220 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 220, 0);
        data->intr = J2X_INT_WB_ERROR_ECC;
    }
    if (221 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 221, 0);
        data->intr = J2X_INT_ERPP_ECC_PARITY_ERR_INT;
    }
    if (222 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 222, 0);
        data->intr = J2X_INT_ETPPA_ECC_PARITY_ERR_INT;
    }
    if (223 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 223, 0);
        data->intr = J2X_INT_ETPPB_ECC_PARITY_ERR_INT;
    }
    if (224 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 224, 0);
        data->intr = J2X_INT_ETPPC_ECC_PARITY_ERR_INT;
    }
    if (225 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 225, 0);
        data->intr = J2X_INT_IPPA_ECC_PARITY_ERR_INT;
    }
    if (226 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 226, 0);
        data->intr = J2X_INT_IPPB_ECC_PARITY_ERR_INT;
    }
    if (227 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 227, 0);
        data->intr = J2X_INT_IPPC_ECC_PARITY_ERR_INT;
    }
    if (228 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 228, 0);
        data->intr = J2X_INT_IPPD_ECC_PARITY_ERR_INT;
    }
    if (229 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 229, 0);
        data->intr = J2X_INT_IPPE_ECC_PARITY_ERR_INT;
    }
    if (230 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 230, 0);
        data->intr = J2X_INT_IPPF_ECC_PARITY_ERR_INT;
    }
    if (231 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 231, 0);
        data->intr = J2X_INT_IRE_ECC_PARITY_ERR_INT;
    }
    if (232 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 232, 0);
        data->intr = J2X_INT_MDB_ECC_PARITY_ERR_INT;
    }
    if (233 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 233, 0);
        data->intr = J2X_INT_OFR_ECC_PARITY_ERR_INT;
    }
    if (234 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 234, 0);
        data->intr = J2X_INT_OFT_ECC_PARITY_ERR_INT;
    }
    if (235 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 235, 0);
        data->intr = J2X_INT_RQP_ECC_PARITY_ERR_INT;
    }
    if (236 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 236, 0);
        data->intr = J2X_INT_SCH_ECC_PARITY_ERR_INT;
    }
    if (237 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 237, 0);
        data->intr = J2X_INT_SPB_ECC_PARITY_ERR_INT;
    }
    if (238 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 238, 0);
        data->intr = J2X_INT_ARB_ECC_ECC_1B_ERR_INT;
    }
    if (239 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 239, 0);
        data->intr = J2X_INT_BDM_ECC_ECC_1B_ERR_INT;
    }
    if (240 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 240, 0);
        data->intr = J2X_INT_CFC_ECC_ECC_1B_ERR_INT;
    }
    if (241 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 241, 0);
        data->intr = J2X_INT_CGM_ECC_ECC_1B_ERR_INT;
    }
    if (242 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 242, 0);
        data->intr = J2X_INT_CRPS_ECC_ECC_1B_ERR_INT;
    }
    if (243 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 243, 0);
        data->intr = J2X_INT_DDHA_ECC_ECC_1B_ERR_INT;
    }
    if (244 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 244, 0);
        data->intr = J2X_INT_DDHB_ECC_ECC_1B_ERR_INT;
    }
    if (245 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 245, 0);
        data->intr = J2X_INT_DDP_ECC_ECC_1B_ERR_INT;
    }
    if (246 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 246, 0);
        data->intr = J2X_INT_DQM_ECC_ECC_1B_ERR_INT;
    }
    if (247 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 247, 0);
        data->intr = J2X_INT_ECGM_ECC_ECC_1B_ERR_INT;
    }
    if (248 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 248, 0);
        data->intr = J2X_INT_ECI_ECC_ECC_1B_ERR_INT;
    }
    if (249 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 249, 0);
        data->intr = J2X_INT_EDB_ECC_ECC_1B_ERR_INT;
    }
    if (250 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 250, 0);
        data->intr = J2X_INT_EPNI_ECC_ECC_1B_ERR_INT;
    }
    if (251 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 251, 0);
        data->intr = J2X_INT_EPRE_ECC_ECC_1B_ERR_INT;
    }
    if (252 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 252, 0);
        data->intr = J2X_INT_EPS_ECC_ECC_1B_ERR_INT;
    }
    if (253 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 253, 0);
        data->intr = J2X_INT_ERPP_ECC_ECC_1B_ERR_INT;
    }
    if (254 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 254, 0);
        data->intr = J2X_INT_ESB_ECC_ECC_1B_ERR_INT;
    }
    if (255 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 255, 0);
        data->intr = J2X_INT_ETPPA_ECC_ECC_1B_ERR_INT;
    }
    if (256 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 256, 0);
        data->intr = J2X_INT_ETPPB_ECC_ECC_1B_ERR_INT;
    }
    if (257 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 257, 0);
        data->intr = J2X_INT_ETPPC_ECC_ECC_1B_ERR_INT;
    }
    if (258 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 258, 0);
        data->intr = J2X_INT_EVNT_ECC_ECC_1B_ERR_INT;
    }
    if (259 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 259, 0);
        data->intr = J2X_INT_FAWR_ECC_ECC_1B_ERR_INT;
    }
    if (260 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 260, 0);
        data->intr = J2X_INT_FAWT_ECC_ECC_1B_ERR_INT;
    }
    if (261 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 261, 0);
        data->intr = J2X_INT_FBSW_ECC_ECC_1B_ERR_INT;
    }
    if (262 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 262, 0);
        data->intr = J2X_INT_FCR_ECC_ECC_1B_ERR_INT;
    }
    if (263 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 263, 0);
        data->intr = J2X_INT_FCT_ECC_ECC_1B_ERR_INT;
    }
    if (264 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 264, 0);
        data->intr = J2X_INT_FDA_ECC_ECC_1B_ERR_INT;
    }
    if (265 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 265, 0);
        data->intr = J2X_INT_FDCRL_ECC_ECC_1B_ERR_INT;
    }
    if (266 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 266, 0);
        data->intr = J2X_INT_FDCRS_ECC_ECC_1B_ERR_INT;
    }
    if (267 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 267, 0);
        data->intr = J2X_INT_FDMP_ECC_ECC_1B_ERR_INT;
    }
    if (268 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 268, 0);
        data->intr = J2X_INT_FDMX_ECC_ECC_1B_ERR_INT;
    }
    if (269 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 269, 0);
        data->intr = J2X_INT_FDMXA_ECC_ECC_1B_ERR_INT;
    }
    if (270 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 270, 0);
        data->intr = J2X_INT_FDMXBI_ECC_ECC_1B_ERR_INT;
    }
    if (271 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 271, 0);
        data->intr = J2X_INT_FDMXBJ_ECC_ECC_1B_ERR_INT;
    }
    if (272 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 272, 0);
        data->intr = J2X_INT_FDMXC_ECC_ECC_1B_ERR_INT;
    }
    if (273 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 273, 0);
        data->intr = J2X_INT_FDMXT_ECC_ECC_1B_ERR_INT;
    }
    if (274 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 274, 0);
        data->intr = J2X_INT_FDR_ECC_ECC_1B_ERR_INT;
    }
    if (275 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 275, 0);
        data->intr = J2X_INT_FDT_ECC_ECC_1B_ERR_INT;
    }
    if (276 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 276, 0);
        data->intr = J2X_INT_FDTL_ECC_ECC_1B_ERR_INT;
    }
    if (277 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 277, 0);
        data->intr = J2X_INT_FECPB_ECC_ECC_1B_ERR_INT;
    }
    if (278 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 278, 0);
        data->intr = J2X_INT_FECRL_ECC_ECC_1B_ERR_INT;
    }
    if (279 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 279, 0);
        data->intr = J2X_INT_FECRS_ECC_ECC_1B_ERR_INT;
    }
    if (280 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 280, 0);
        data->intr = J2X_INT_FFLXO_ECC_ECC_1B_ERR_INT;
    }
    if (281 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 281, 0);
        data->intr = J2X_INT_FFOA_ECC_ECC_1B_ERR_INT;
    }
    if (282 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 282, 0);
        data->intr = J2X_INT_FFOB_ECC_ECC_1B_ERR_INT;
    }
    if (283 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 283, 0);
        data->intr = J2X_INT_FGDMP_ECC_ECC_1B_ERR_INT;
    }
    if (284 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 284, 0);
        data->intr = J2X_INT_FGMAP_ECC_ECC_1B_ERR_INT;
    }
    if (285 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 285, 0);
        data->intr = J2X_INT_FICPB_ECC_ECC_1B_ERR_INT;
    }
    if (286 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 286, 0);
        data->intr = J2X_INT_FLFR_ECC_ECC_1B_ERR_INT;
    }
    if (287 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 287, 0);
        data->intr = J2X_INT_FLFT_ECC_ECC_1B_ERR_INT;
    }
    if (288 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 288, 0);
        data->intr = J2X_INT_FLOTN_ECC_ECC_1B_ERR_INT;
    }
    if (289 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 289, 0);
        data->intr = J2X_INT_FMAC_ECC_ECC_1B_ERR_INT;
    }
    if (290 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 290, 0);
        data->intr = J2X_INT_FMACR_ECC_ECC_1B_ERR_INT;
    }
    if (291 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 291, 0);
        data->intr = J2X_INT_FMACT_ECC_ECC_1B_ERR_INT;
    }
    if (292 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 292, 0);
        data->intr = J2X_INT_FMAP_ECC_ECC_1B_ERR_INT;
    }
    if (293 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 293, 0);
        data->intr = J2X_INT_FMX_ECC_ECC_1B_ERR_INT;
    }
    if (294 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 294, 0);
        data->intr = J2X_INT_FMXAI_ECC_ECC_1B_ERR_INT;
    }
    if (295 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 295, 0);
        data->intr = J2X_INT_FMXAJ_ECC_ECC_1B_ERR_INT;
    }
    if (296 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 296, 0);
        data->intr = J2X_INT_FMXBI_ECC_ECC_1B_ERR_INT;
    }
    if (297 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 297, 0);
        data->intr = J2X_INT_FMXBJ_ECC_ECC_1B_ERR_INT;
    }
    if (298 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 298, 0);
        data->intr = J2X_INT_FMXBK_ECC_ECC_1B_ERR_INT;
    }
    if (299 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 299, 0);
        data->intr = J2X_INT_FMXC_ECC_ECC_1B_ERR_INT;
    }
    if (300 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 300, 0);
        data->intr = J2X_INT_FMXT_ECC_ECC_1B_ERR_INT;
    }
    if (301 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 301, 0);
        data->intr = J2X_INT_FOAM_ECC_ECC_1B_ERR_INT;
    }
    if (302 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 302, 0);
        data->intr = J2X_INT_FODUO_ECC_ECC_1B_ERR_INT;
    }
    if (303 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 303, 0);
        data->intr = J2X_INT_FOSW_ECC_ECC_1B_ERR_INT;
    }
    if (304 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 304, 0);
        data->intr = J2X_INT_FPMR_ECC_ECC_1B_ERR_INT;
    }
    if (305 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 305, 0);
        data->intr = J2X_INT_FPMT_ECC_ECC_1B_ERR_INT;
    }
    if (306 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 306, 0);
        data->intr = J2X_INT_FPTPR_ECC_ECC_1B_ERR_INT;
    }
    if (307 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 307, 0);
        data->intr = J2X_INT_FPTPT_ECC_ECC_1B_ERR_INT;
    }
    if (308 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 308, 0);
        data->intr = J2X_INT_FQP_ECC_ECC_1B_ERR_INT;
    }
    if (309 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 309, 0);
        data->intr = J2X_INT_FRA_ECC_ECC_1B_ERR_INT;
    }
    if (310 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 310, 0);
        data->intr = J2X_INT_FRWA_ECC_ECC_1B_ERR_INT;
    }
    if (311 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 311, 0);
        data->intr = J2X_INT_FRWBC_ECC_ECC_1B_ERR_INT;
    }
    if (312 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 312, 0);
        data->intr = J2X_INT_FSAR_ECC_ECC_1B_ERR_INT;
    }
    if (313 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 313, 0);
        data->intr = J2X_INT_HBC_ECC_ECC_1B_ERR_INT;
    }
    if (314 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 314, 0);
        data->intr = J2X_INT_ILE_ECC_ECC_1B_ERR_INT;
    }
    if (315 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 315, 0);
        data->intr = J2X_INT_ILU_ECC_ECC_1B_ERR_INT;
    }
    if (316 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 316, 0);
        data->intr = J2X_INT_IPPA_ECC_ECC_1B_ERR_INT;
    }
    if (317 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 317, 0);
        data->intr = J2X_INT_IPPB_ECC_ECC_1B_ERR_INT;
    }
    if (318 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 318, 0);
        data->intr = J2X_INT_IPPC_ECC_ECC_1B_ERR_INT;
    }
    if (319 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 319, 0);
        data->intr = J2X_INT_IPPD_ECC_ECC_1B_ERR_INT;
    }
    if (320 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 320, 0);
        data->intr = J2X_INT_IPPE_ECC_ECC_1B_ERR_INT;
    }
    if (321 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 321, 0);
        data->intr = J2X_INT_IPPF_ECC_ECC_1B_ERR_INT;
    }
    if (322 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 322, 0);
        data->intr = J2X_INT_IPS_ECC_ECC_1B_ERR_INT;
    }
    if (323 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 323, 0);
        data->intr = J2X_INT_IPT_ECC_ECC_1B_ERR_INT;
    }
    if (324 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 324, 0);
        data->intr = J2X_INT_IQM_ECC_ECC_1B_ERR_INT;
    }
    if (325 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 325, 0);
        data->intr = J2X_INT_IRE_ECC_ECC_1B_ERR_INT;
    }
    if (326 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 326, 0);
        data->intr = J2X_INT_ITPP_ECC_ECC_1B_ERR_INT;
    }
    if (327 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 327, 0);
        data->intr = J2X_INT_ITPPD_ECC_ECC_1B_ERR_INT;
    }
    if (328 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 328, 0);
        data->intr = J2X_INT_KAPS_ECC_ECC_1B_ERR_INT;
    }
    if (329 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 329, 0);
        data->intr = J2X_INT_MACT_ECC_ECC_1B_ERR_INT;
    }
    if (330 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 330, 0);
        data->intr = J2X_INT_MCP_ECC_ECC_1B_ERR_INT;
    }
    if (331 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 331, 0);
        data->intr = J2X_INT_MDB_ECC_ECC_1B_ERR_INT;
    }
    if (332 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 332, 0);
        data->intr = J2X_INT_MRPS_ECC_ECC_1B_ERR_INT;
    }
    if (333 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 333, 0);
        data->intr = J2X_INT_MTM_ECC_ECC_1B_ERR_INT;
    }
    if (334 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 334, 0);
        data->intr = J2X_INT_OAMP_ECC_ECC_1B_ERR_INT;
    }
    if (335 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 335, 0);
        data->intr = J2X_INT_OCB_ECC_ECC_1B_ERR_INT;
    }
    if (336 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 336, 0);
        data->intr = J2X_INT_OCBM_ECC_ECC_1B_ERR_INT;
    }
    if (337 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 337, 0);
        data->intr = J2X_INT_OFR_ECC_ECC_1B_ERR_INT;
    }
    if (338 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 338, 0);
        data->intr = J2X_INT_OFT_ECC_ECC_1B_ERR_INT;
    }
    if (339 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 339, 0);
        data->intr = J2X_INT_OLP_ECC_ECC_1B_ERR_INT;
    }
    if (340 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 340, 0);
        data->intr = J2X_INT_PDM_ECC_ECC_1B_ERR_INT;
    }
    if (341 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 341, 0);
        data->intr = J2X_INT_PEM_ECC_ECC_1B_ERR_INT;
    }
    if (342 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 342, 0);
        data->intr = J2X_INT_PQP_ECC_ECC_1B_ERR_INT;
    }
    if (343 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 343, 0);
        data->intr = J2X_INT_RQP_ECC_ECC_1B_ERR_INT;
    }
    if (344 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 344, 0);
        data->intr = J2X_INT_RTP_ECC_ECC_1B_ERR_INT;
    }
    if (345 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 345, 0);
        data->intr = J2X_INT_SCH_ECC_ECC_1B_ERR_INT;
    }
    if (346 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 346, 0);
        data->intr = J2X_INT_SIF_ECC_ECC_1B_ERR_INT;
    }
    if (347 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 347, 0);
        data->intr = J2X_INT_SPB_ECC_ECC_1B_ERR_INT;
    }
    if (348 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 348, 0);
        data->intr = J2X_INT_SQM_ECC_ECC_1B_ERR_INT;
    }
    if (349 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 349, 0);
        data->intr = J2X_INT_TCAM_ECC_ECC_1B_ERR_INT;
    }
    if (350 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 350, 0);
        data->intr = J2X_INT_TDU_ECC_ECC_1B_ERR_INT;
    }
    if (351 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 351, 0);
        data->intr = J2X_INT_TSN_ECC_ECC_1B_ERR_INT;
    }
    if (352 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 352, 0);
        data->intr = J2X_INT_WB_ECC_ECC_1B_ERR_INT;
    }
    if (353 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 353, 0);
        data->intr = J2X_INT_ARB_ECC_ECC_2B_ERR_INT;
    }
    if (354 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 354, 0);
        data->intr = J2X_INT_BDM_ECC_ECC_2B_ERR_INT;
    }
    if (355 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 355, 0);
        data->intr = J2X_INT_CFC_ECC_ECC_2B_ERR_INT;
    }
    if (356 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 356, 0);
        data->intr = J2X_INT_CGM_ECC_ECC_2B_ERR_INT;
    }
    if (357 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 357, 0);
        data->intr = J2X_INT_CRPS_ECC_ECC_2B_ERR_INT;
    }
    if (358 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 358, 0);
        data->intr = J2X_INT_DDHA_ECC_ECC_2B_ERR_INT;
    }
    if (359 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 359, 0);
        data->intr = J2X_INT_DDHB_ECC_ECC_2B_ERR_INT;
    }
    if (360 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 360, 0);
        data->intr = J2X_INT_DDP_ECC_ECC_2B_ERR_INT;
    }
    if (361 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 361, 0);
        data->intr = J2X_INT_DQM_ECC_ECC_2B_ERR_INT;
    }
    if (362 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 362, 0);
        data->intr = J2X_INT_ECGM_ECC_ECC_2B_ERR_INT;
    }
    if (363 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 363, 0);
        data->intr = J2X_INT_ECI_ECC_ECC_2B_ERR_INT;
    }
    if (364 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 364, 0);
        data->intr = J2X_INT_EDB_ECC_ECC_2B_ERR_INT;
    }
    if (365 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 365, 0);
        data->intr = J2X_INT_EPNI_ECC_ECC_2B_ERR_INT;
    }
    if (366 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 366, 0);
        data->intr = J2X_INT_EPRE_ECC_ECC_2B_ERR_INT;
    }
    if (367 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 367, 0);
        data->intr = J2X_INT_EPS_ECC_ECC_2B_ERR_INT;
    }
    if (368 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 368, 0);
        data->intr = J2X_INT_ERPP_ECC_ECC_2B_ERR_INT;
    }
    if (369 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 369, 0);
        data->intr = J2X_INT_ESB_ECC_ECC_2B_ERR_INT;
    }
    if (370 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 370, 0);
        data->intr = J2X_INT_ETPPA_ECC_ECC_2B_ERR_INT;
    }
    if (371 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 371, 0);
        data->intr = J2X_INT_ETPPB_ECC_ECC_2B_ERR_INT;
    }
    if (372 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 372, 0);
        data->intr = J2X_INT_ETPPC_ECC_ECC_2B_ERR_INT;
    }
    if (373 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 373, 0);
        data->intr = J2X_INT_EVNT_ECC_ECC_2B_ERR_INT;
    }
    if (374 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 374, 0);
        data->intr = J2X_INT_FAWR_ECC_ECC_2B_ERR_INT;
    }
    if (375 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 375, 0);
        data->intr = J2X_INT_FAWT_ECC_ECC_2B_ERR_INT;
    }
    if (376 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 376, 0);
        data->intr = J2X_INT_FBSW_ECC_ECC_2B_ERR_INT;
    }
    if (377 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 377, 0);
        data->intr = J2X_INT_FCR_ECC_ECC_2B_ERR_INT;
    }
    if (378 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 378, 0);
        data->intr = J2X_INT_FCT_ECC_ECC_2B_ERR_INT;
    }
    if (379 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 379, 0);
        data->intr = J2X_INT_FDA_ECC_ECC_2B_ERR_INT;
    }
    if (380 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 380, 0);
        data->intr = J2X_INT_FDCRL_ECC_ECC_2B_ERR_INT;
    }
    if (381 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 381, 0);
        data->intr = J2X_INT_FDCRS_ECC_ECC_2B_ERR_INT;
    }
    if (382 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 382, 0);
        data->intr = J2X_INT_FDMP_ECC_ECC_2B_ERR_INT;
    }
    if (383 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 383, 0);
        data->intr = J2X_INT_FDMX_ECC_ECC_2B_ERR_INT;
    }
    if (384 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 384, 0);
        data->intr = J2X_INT_FDMXA_ECC_ECC_2B_ERR_INT;
    }
    if (385 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 385, 0);
        data->intr = J2X_INT_FDMXBI_ECC_ECC_2B_ERR_INT;
    }
    if (386 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 386, 0);
        data->intr = J2X_INT_FDMXBJ_ECC_ECC_2B_ERR_INT;
    }
    if (387 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 387, 0);
        data->intr = J2X_INT_FDMXC_ECC_ECC_2B_ERR_INT;
    }
    if (388 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 388, 0);
        data->intr = J2X_INT_FDMXT_ECC_ECC_2B_ERR_INT;
    }
    if (389 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 389, 0);
        data->intr = J2X_INT_FDR_ECC_ECC_2B_ERR_INT;
    }
    if (390 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 390, 0);
        data->intr = J2X_INT_FDT_ECC_ECC_2B_ERR_INT;
    }
    if (391 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 391, 0);
        data->intr = J2X_INT_FDTL_ECC_ECC_2B_ERR_INT;
    }
    if (392 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 392, 0);
        data->intr = J2X_INT_FECPB_ECC_ECC_2B_ERR_INT;
    }
    if (393 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 393, 0);
        data->intr = J2X_INT_FECRL_ECC_ECC_2B_ERR_INT;
    }
    if (394 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 394, 0);
        data->intr = J2X_INT_FECRS_ECC_ECC_2B_ERR_INT;
    }
    if (395 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 395, 0);
        data->intr = J2X_INT_FFLXO_ECC_ECC_2B_ERR_INT;
    }
    if (396 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 396, 0);
        data->intr = J2X_INT_FFOA_ECC_ECC_2B_ERR_INT;
    }
    if (397 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 397, 0);
        data->intr = J2X_INT_FFOB_ECC_ECC_2B_ERR_INT;
    }
    if (398 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 398, 0);
        data->intr = J2X_INT_FGDMP_ECC_ECC_2B_ERR_INT;
    }
    if (399 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 399, 0);
        data->intr = J2X_INT_FGMAP_ECC_ECC_2B_ERR_INT;
    }
    if (400 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 400, 0);
        data->intr = J2X_INT_FICPB_ECC_ECC_2B_ERR_INT;
    }
    if (401 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 401, 0);
        data->intr = J2X_INT_FLFR_ECC_ECC_2B_ERR_INT;
    }
    if (402 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 402, 0);
        data->intr = J2X_INT_FLFT_ECC_ECC_2B_ERR_INT;
    }
    if (403 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 403, 0);
        data->intr = J2X_INT_FLOTN_ECC_ECC_2B_ERR_INT;
    }
    if (404 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 404, 0);
        data->intr = J2X_INT_FMAC_ECC_ECC_2B_ERR_INT;
    }
    if (405 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 405, 0);
        data->intr = J2X_INT_FMACR_ECC_ECC_2B_ERR_INT;
    }
    if (406 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 406, 0);
        data->intr = J2X_INT_FMACT_ECC_ECC_2B_ERR_INT;
    }
    if (407 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 407, 0);
        data->intr = J2X_INT_FMAP_ECC_ECC_2B_ERR_INT;
    }
    if (408 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 408, 0);
        data->intr = J2X_INT_FMX_ECC_ECC_2B_ERR_INT;
    }
    if (409 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 409, 0);
        data->intr = J2X_INT_FMXAI_ECC_ECC_2B_ERR_INT;
    }
    if (410 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 410, 0);
        data->intr = J2X_INT_FMXAJ_ECC_ECC_2B_ERR_INT;
    }
    if (411 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 411, 0);
        data->intr = J2X_INT_FMXBI_ECC_ECC_2B_ERR_INT;
    }
    if (412 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 412, 0);
        data->intr = J2X_INT_FMXBJ_ECC_ECC_2B_ERR_INT;
    }
    if (413 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 413, 0);
        data->intr = J2X_INT_FMXBK_ECC_ECC_2B_ERR_INT;
    }
    if (414 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 414, 0);
        data->intr = J2X_INT_FMXC_ECC_ECC_2B_ERR_INT;
    }
    if (415 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 415, 0);
        data->intr = J2X_INT_FMXT_ECC_ECC_2B_ERR_INT;
    }
    if (416 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 416, 0);
        data->intr = J2X_INT_FOAM_ECC_ECC_2B_ERR_INT;
    }
    if (417 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 417, 0);
        data->intr = J2X_INT_FODUO_ECC_ECC_2B_ERR_INT;
    }
    if (418 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 418, 0);
        data->intr = J2X_INT_FOSW_ECC_ECC_2B_ERR_INT;
    }
    if (419 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 419, 0);
        data->intr = J2X_INT_FPMR_ECC_ECC_2B_ERR_INT;
    }
    if (420 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 420, 0);
        data->intr = J2X_INT_FPMT_ECC_ECC_2B_ERR_INT;
    }
    if (421 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 421, 0);
        data->intr = J2X_INT_FPTPR_ECC_ECC_2B_ERR_INT;
    }
    if (422 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 422, 0);
        data->intr = J2X_INT_FPTPT_ECC_ECC_2B_ERR_INT;
    }
    if (423 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 423, 0);
        data->intr = J2X_INT_FQP_ECC_ECC_2B_ERR_INT;
    }
    if (424 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 424, 0);
        data->intr = J2X_INT_FRA_ECC_ECC_2B_ERR_INT;
    }
    if (425 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 425, 0);
        data->intr = J2X_INT_FRWA_ECC_ECC_2B_ERR_INT;
    }
    if (426 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 426, 0);
        data->intr = J2X_INT_FRWBC_ECC_ECC_2B_ERR_INT;
    }
    if (427 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 427, 0);
        data->intr = J2X_INT_FSAR_ECC_ECC_2B_ERR_INT;
    }
    if (428 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 428, 0);
        data->intr = J2X_INT_HBC_ECC_ECC_2B_ERR_INT;
    }
    if (429 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 429, 0);
        data->intr = J2X_INT_ILE_ECC_ECC_2B_ERR_INT;
    }
    if (430 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 430, 0);
        data->intr = J2X_INT_ILU_ECC_ECC_2B_ERR_INT;
    }
    if (431 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 431, 0);
        data->intr = J2X_INT_IPPA_ECC_ECC_2B_ERR_INT;
    }
    if (432 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 432, 0);
        data->intr = J2X_INT_IPPB_ECC_ECC_2B_ERR_INT;
    }
    if (433 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 433, 0);
        data->intr = J2X_INT_IPPC_ECC_ECC_2B_ERR_INT;
    }
    if (434 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 434, 0);
        data->intr = J2X_INT_IPPD_ECC_ECC_2B_ERR_INT;
    }
    if (435 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 435, 0);
        data->intr = J2X_INT_IPPE_ECC_ECC_2B_ERR_INT;
    }
    if (436 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 436, 0);
        data->intr = J2X_INT_IPPF_ECC_ECC_2B_ERR_INT;
    }
    if (437 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 437, 0);
        data->intr = J2X_INT_IPS_ECC_ECC_2B_ERR_INT;
    }
    if (438 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 438, 0);
        data->intr = J2X_INT_IPT_ECC_ECC_2B_ERR_INT;
    }
    if (439 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 439, 0);
        data->intr = J2X_INT_IQM_ECC_ECC_2B_ERR_INT;
    }
    if (440 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 440, 0);
        data->intr = J2X_INT_IRE_ECC_ECC_2B_ERR_INT;
    }
    if (441 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 441, 0);
        data->intr = J2X_INT_ITPP_ECC_ECC_2B_ERR_INT;
    }
    if (442 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 442, 0);
        data->intr = J2X_INT_ITPPD_ECC_ECC_2B_ERR_INT;
    }
    if (443 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 443, 0);
        data->intr = J2X_INT_KAPS_ECC_ECC_2B_ERR_INT;
    }
    if (444 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 444, 0);
        data->intr = J2X_INT_MACT_ECC_ECC_2B_ERR_INT;
    }
    if (445 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 445, 0);
        data->intr = J2X_INT_MCP_ECC_ECC_2B_ERR_INT;
    }
    if (446 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 446, 0);
        data->intr = J2X_INT_MDB_ECC_ECC_2B_ERR_INT;
    }
    if (447 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 447, 0);
        data->intr = J2X_INT_MRPS_ECC_ECC_2B_ERR_INT;
    }
    if (448 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 448, 0);
        data->intr = J2X_INT_MTM_ECC_ECC_2B_ERR_INT;
    }
    if (449 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 449, 0);
        data->intr = J2X_INT_OAMP_ECC_ECC_2B_ERR_INT;
    }
    if (450 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 450, 0);
        data->intr = J2X_INT_OCB_ECC_ECC_2B_ERR_INT;
    }
    if (451 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 451, 0);
        data->intr = J2X_INT_OCBM_ECC_ECC_2B_ERR_INT;
    }
    if (452 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 452, 0);
        data->intr = J2X_INT_OFR_ECC_ECC_2B_ERR_INT;
    }
    if (453 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 453, 0);
        data->intr = J2X_INT_OFT_ECC_ECC_2B_ERR_INT;
    }
    if (454 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 454, 0);
        data->intr = J2X_INT_OLP_ECC_ECC_2B_ERR_INT;
    }
    if (455 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 455, 0);
        data->intr = J2X_INT_PDM_ECC_ECC_2B_ERR_INT;
    }
    if (456 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 456, 0);
        data->intr = J2X_INT_PEM_ECC_ECC_2B_ERR_INT;
    }
    if (457 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 457, 0);
        data->intr = J2X_INT_PQP_ECC_ECC_2B_ERR_INT;
    }
    if (458 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 458, 0);
        data->intr = J2X_INT_RQP_ECC_ECC_2B_ERR_INT;
    }
    if (459 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 459, 0);
        data->intr = J2X_INT_RTP_ECC_ECC_2B_ERR_INT;
    }
    if (460 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 460, 0);
        data->intr = J2X_INT_SCH_ECC_ECC_2B_ERR_INT;
    }
    if (461 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 461, 0);
        data->intr = J2X_INT_SIF_ECC_ECC_2B_ERR_INT;
    }
    if (462 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 462, 0);
        data->intr = J2X_INT_SPB_ECC_ECC_2B_ERR_INT;
    }
    if (463 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 463, 0);
        data->intr = J2X_INT_SQM_ECC_ECC_2B_ERR_INT;
    }
    if (464 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 464, 0);
        data->intr = J2X_INT_TCAM_ECC_ECC_2B_ERR_INT;
    }
    if (465 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 465, 0);
        data->intr = J2X_INT_TDU_ECC_ECC_2B_ERR_INT;
    }
    if (466 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 466, 0);
        data->intr = J2X_INT_TSN_ECC_ECC_2B_ERR_INT;
    }
    if (467 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 467, 0);
        data->intr = J2X_INT_WB_ECC_ECC_2B_ERR_INT;
    }
    if (468 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 468, 0);
        data->intr = J2X_INT_TCAM_TCAM_PROTECTION_ERROR_1_BIT_ECC;
    }
    if (469 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 469, 0);
        data->intr = J2X_INT_TCAM_TCAM_PROTECTION_ERROR_2_BIT_ECC;
    }
    if (470 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 470, 0);
        data->intr = J2X_INT_DDP_DELETE_BDB_FIFO_NOT_EMPTY;
    }
    if (471 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 471, 0);
        data->intr = J2X_INT_DDP_DELETE_BDB_FIFO_FULL;
    }
    if (472 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 472, 0);
        data->intr = J2X_INT_RQP_PKT_REAS_INT_VEC;
    }
    if (473 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 473, 0);
        data->intr = J2X_INT_EPNI_ETPP_TOTAL_INC_ABOVE_TH_WITH_EBD_INT;
    }
    if (474 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 474, 0);
        data->intr = J2X_INT_EPNI_ETPP_EBD_ERR_INT;
    }
    if (475 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 475, 0);
        data->intr = J2X_INT_MSW_MACSEC_MACSEC_INTERRUPT_REGISTER;
    }
    if (476 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 476, 0);
        data->intr = INVALIDr;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_disable_on_init_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_disable_on_init_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_disable_on_init;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 1;
    table->info_get.key_size[0] = 1;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_disable_on_init_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_disable_on_init");

    
    default_data = (dnx_data_intr_general_disable_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_disable_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = INVALIDr;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_disable_print_on_init_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_disable_print_on_init_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_disable_print_on_init;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 3;
    table->info_get.key_size[0] = 3;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_disable_print_on_init_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_disable_print_on_init");

    
    default_data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = J2X_INT_OAMP_PENDING_EVENT;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2X_INT_OAMP_STAT_PENDING_EVENT;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->intr = INVALIDr;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_block_intr_field_mapping_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_block_intr_field_mapping_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_block_intr_field_mapping;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 2048;
    table->info_get.key_size[0] = 2048;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_block_intr_field_mapping_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_block_intr_field_mapping");

    
    default_data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (63 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 63, 0);
        data->intr = ARB_INTERRUPTf;
    }
    if (82 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 82, 0);
        data->intr = BDM_INTERRUPTf;
    }
    if (241 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 241, 0);
        data->intr = CDPM_0_INTERRUPTf;
    }
    if (242 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 242, 0);
        data->intr = CDPM_1_INTERRUPTf;
    }
    if (243 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 243, 0);
        data->intr = CDPM_2_INTERRUPTf;
    }
    if (244 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 244, 0);
        data->intr = CDPM_3_INTERRUPTf;
    }
    if (245 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 245, 0);
        data->intr = CDPM_4_INTERRUPTf;
    }
    if (246 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 246, 0);
        data->intr = CDPM_5_INTERRUPTf;
    }
    if (247 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 247, 0);
        data->intr = CDPM_6_INTERRUPTf;
    }
    if (248 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 248, 0);
        data->intr = CDPM_7_INTERRUPTf;
    }
    if (1520 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1520, 0);
        data->intr = CDPM_0_PM_INTERRUPTf;
    }
    if (1536 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1536, 0);
        data->intr = CDPM_1_PM_INTERRUPTf;
    }
    if (1552 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1552, 0);
        data->intr = CDPM_2_PM_INTERRUPTf;
    }
    if (1568 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1568, 0);
        data->intr = CDPM_3_PM_INTERRUPTf;
    }
    if (1584 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1584, 0);
        data->intr = CDPM_4_PM_INTERRUPTf;
    }
    if (1600 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1600, 0);
        data->intr = CDPM_5_PM_INTERRUPTf;
    }
    if (1616 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1616, 0);
        data->intr = CDPM_6_PM_INTERRUPTf;
    }
    if (1632 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1632, 0);
        data->intr = CDPM_7_PM_INTERRUPTf;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->intr = CFC_INTERRUPTf;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->intr = CGM_INTERRUPTf;
    }
    if (85 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 85, 0);
        data->intr = CRPS_INTERRUPTf;
    }
    if (300 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 300, 0);
        data->intr = DDHA_INTERRUPTf;
    }
    if (301 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 301, 0);
        data->intr = DDHB_0_INTERRUPTf;
    }
    if (302 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 302, 0);
        data->intr = DDHB_1_INTERRUPTf;
    }
    if (303 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 303, 0);
        data->intr = DDHB_2_INTERRUPTf;
    }
    if (304 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 304, 0);
        data->intr = DDHB_3_INTERRUPTf;
    }
    if (156 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 156, 0);
        data->intr = DDP_INTERRUPTf;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->intr = DQM_INTERRUPTf;
    }
    if (141 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 141, 0);
        data->intr = ECGM_INTERRUPTf;
    }
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = ECI_INTERRUPTf;
    }
    if (212 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 212, 0);
        data->intr = EDB_INTERRUPTf;
    }
    if (66 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 66, 0);
        data->intr = EPNI_INTERRUPTf;
    }
    if (65 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 65, 0);
        data->intr = EPRE_INTERRUPTf;
    }
    if (139 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 139, 0);
        data->intr = EPS_SPR_INTERRUPTf;
    }
    if (144 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 144, 0);
        data->intr = ERPP_INTERRUPTf;
    }
    if (88 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 88, 0);
        data->intr = ESB_INTERRUPTf;
    }
    if (90 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 90, 0);
        data->intr = ETPPA_INTERRUPTf;
    }
    if (145 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 145, 0);
        data->intr = ETPPB_INTERRUPTf;
    }
    if (68 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 68, 0);
        data->intr = ETPPC_INTERRUPTf;
    }
    if (309 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 309, 0);
        data->intr = EVNT_INTERRUPTf;
    }
    if (407 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 407, 0);
        data->intr = FAWR_INTERRUPTf;
    }
    if (437 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 437, 0);
        data->intr = FAWT_INTERRUPTf;
    }
    if (400 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 400, 0);
        data->intr = FBSW_INTERRUPTf;
    }
    if (78 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 78, 0);
        data->intr = FCR_INTERRUPTf;
    }
    if (80 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 80, 0);
        data->intr = FCT_INTERRUPTf;
    }
    if (79 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 79, 0);
        data->intr = FDA_INTERRUPTf;
    }
    if (401 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 401, 0);
        data->intr = FDCRL_INTERRUPTf;
    }
    if (402 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 402, 0);
        data->intr = FDCRS_INTERRUPTf;
    }
    if (436 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 436, 0);
        data->intr = FDMP_INTERRUPTf;
    }
    if (408 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 408, 0);
        data->intr = FDMX_0_INTERRUPTf;
    }
    if (409 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 409, 0);
        data->intr = FDMX_1_INTERRUPTf;
    }
    if (429 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 429, 0);
        data->intr = FDMXA_INTERRUPTf;
    }
    if (450 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 450, 0);
        data->intr = FDMXBI_INTERRUPTf;
    }
    if (438 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 438, 0);
        data->intr = FDMXBJ_INTERRUPTf;
    }
    if (431 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 431, 0);
        data->intr = FDMXC_INTERRUPTf;
    }
    if (439 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 439, 0);
        data->intr = FDMXT_INTERRUPTf;
    }
    if (74 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 74, 0);
        data->intr = FDR_INTERRUPTf;
    }
    if (75 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 75, 0);
        data->intr = FDT_INTERRUPTf;
    }
    if (76 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 76, 0);
        data->intr = FDTL_INTERRUPTf;
    }
    if (453 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 453, 0);
        data->intr = FECPB_INTERRUPTf;
    }
    if (403 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 403, 0);
        data->intr = FECRL_INTERRUPTf;
    }
    if (404 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 404, 0);
        data->intr = FECRS_INTERRUPTf;
    }
    if (440 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 440, 0);
        data->intr = FFLXO_INTERRUPTf;
    }
    if (405 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 405, 0);
        data->intr = FFOA_INTERRUPTf;
    }
    if (406 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 406, 0);
        data->intr = FFOB_INTERRUPTf;
    }
    if (421 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 421, 0);
        data->intr = FGDMP_INTERRUPTf;
    }
    if (422 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 422, 0);
        data->intr = FGMAP_INTERRUPTf;
    }
    if (454 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 454, 0);
        data->intr = FICPB_INTERRUPTf;
    }
    if (266 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 266, 0);
        data->intr = FLEXMAC_INTERRUPTf;
    }
    if (414 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 414, 0);
        data->intr = FLFR_0_INTERRUPTf;
    }
    if (415 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 415, 0);
        data->intr = FLFR_1_INTERRUPTf;
    }
    if (416 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 416, 0);
        data->intr = FLFR_2_INTERRUPTf;
    }
    if (417 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 417, 0);
        data->intr = FLFR_3_INTERRUPTf;
    }
    if (418 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 418, 0);
        data->intr = FLFT_0_INTERRUPTf;
    }
    if (419 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 419, 0);
        data->intr = FLFT_1_INTERRUPTf;
    }
    if (420 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 420, 0);
        data->intr = FLOTN_INTERRUPTf;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->intr = FMAC_0_INTERRUPTf;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->intr = FMAC_1_INTERRUPTf;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->intr = FMAC_2_INTERRUPTf;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->intr = FMAC_3_INTERRUPTf;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->intr = FMAC_4_INTERRUPTf;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->intr = FMAC_5_INTERRUPTf;
    }
    if (41 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 41, 0);
        data->intr = FMAC_6_INTERRUPTf;
    }
    if (42 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 42, 0);
        data->intr = FMAC_7_INTERRUPTf;
    }
    if (44 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 44, 0);
        data->intr = FMAC_8_INTERRUPTf;
    }
    if (45 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 45, 0);
        data->intr = FMAC_9_INTERRUPTf;
    }
    if (47 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 47, 0);
        data->intr = FMAC_10_INTERRUPTf;
    }
    if (48 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 48, 0);
        data->intr = FMAC_11_INTERRUPTf;
    }
    if (50 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 50, 0);
        data->intr = FMAC_12_INTERRUPTf;
    }
    if (51 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 51, 0);
        data->intr = FMAC_13_INTERRUPTf;
    }
    if (53 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 53, 0);
        data->intr = FMAC_14_INTERRUPTf;
    }
    if (54 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 54, 0);
        data->intr = FMAC_15_INTERRUPTf;
    }
    if (441 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 441, 0);
        data->intr = FMACR_INTERRUPTf;
    }
    if (442 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 442, 0);
        data->intr = FMACT_INTERRUPTf;
    }
    if (423 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 423, 0);
        data->intr = FMAP_INTERRUPTf;
    }
    if (443 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 443, 0);
        data->intr = FMX_0_INTERRUPTf;
    }
    if (444 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 444, 0);
        data->intr = FMX_1_INTERRUPTf;
    }
    if (430 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 430, 0);
        data->intr = FMXAI_INTERRUPTf;
    }
    if (451 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 451, 0);
        data->intr = FMXAJ_INTERRUPTf;
    }
    if (445 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 445, 0);
        data->intr = FMXBI_INTERRUPTf;
    }
    if (446 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 446, 0);
        data->intr = FMXBJ_INTERRUPTf;
    }
    if (452 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 452, 0);
        data->intr = FMXBK_INTERRUPTf;
    }
    if (432 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 432, 0);
        data->intr = FMXC_INTERRUPTf;
    }
    if (447 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 447, 0);
        data->intr = FMXT_INTERRUPTf;
    }
    if (424 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 424, 0);
        data->intr = FOAM_0_INTERRUPTf;
    }
    if (425 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 425, 0);
        data->intr = FOAM_1_INTERRUPTf;
    }
    if (426 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 426, 0);
        data->intr = FODUO_0_INTERRUPTf;
    }
    if (427 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 427, 0);
        data->intr = FODUO_1_INTERRUPTf;
    }
    if (428 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 428, 0);
        data->intr = FOSW_INTERRUPTf;
    }
    if (433 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 433, 0);
        data->intr = FPMR_INTERRUPTf;
    }
    if (434 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 434, 0);
        data->intr = FPMT_INTERRUPTf;
    }
    if (412 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 412, 0);
        data->intr = FPTPR_INTERRUPTf;
    }
    if (413 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 413, 0);
        data->intr = FPTPT_INTERRUPTf;
    }
    if (138 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 138, 0);
        data->intr = FQP_INTERRUPTf;
    }
    if (448 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 448, 0);
        data->intr = FRA_0_INTERRUPTf;
    }
    if (449 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 449, 0);
        data->intr = FRA_1_INTERRUPTf;
    }
    if (460 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 460, 0);
        data->intr = FRWA_INTERRUPTf;
    }
    if (462 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 462, 0);
        data->intr = FRWBC_INTERRUPTf;
    }
    if (435 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 435, 0);
        data->intr = FSAR_INTERRUPTf;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->intr = FSRD_0_INTERRUPTf;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->intr = FSRD_1_INTERRUPTf;
    }
    if (40 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 40, 0);
        data->intr = FSRD_2_INTERRUPTf;
    }
    if (43 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 43, 0);
        data->intr = FSRD_3_INTERRUPTf;
    }
    if (46 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 46, 0);
        data->intr = FSRD_4_INTERRUPTf;
    }
    if (49 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 49, 0);
        data->intr = FSRD_5_INTERRUPTf;
    }
    if (52 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 52, 0);
        data->intr = FSRD_6_INTERRUPTf;
    }
    if (55 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 55, 0);
        data->intr = FSRD_7_INTERRUPTf;
    }
    if (198 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 198, 0);
        data->intr = HBC_0_INTERRUPTf;
    }
    if (199 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 199, 0);
        data->intr = HBC_1_INTERRUPTf;
    }
    if (200 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 200, 0);
        data->intr = HBC_2_INTERRUPTf;
    }
    if (201 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 201, 0);
        data->intr = HBC_3_INTERRUPTf;
    }
    if (202 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 202, 0);
        data->intr = HBC_4_INTERRUPTf;
    }
    if (203 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 203, 0);
        data->intr = HBC_5_INTERRUPTf;
    }
    if (204 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 204, 0);
        data->intr = HBC_6_INTERRUPTf;
    }
    if (205 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 205, 0);
        data->intr = HBC_7_INTERRUPTf;
    }
    if (197 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 197, 0);
        data->intr = HBMC_INTERRUPTf;
    }
    if (130 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 130, 0);
        data->intr = ILE_INTERRUPTf;
    }
    if (129 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 129, 0);
        data->intr = ILU_INTERRUPTf;
    }
    if (164 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 164, 0);
        data->intr = IPPA_INTERRUPTf;
    }
    if (165 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 165, 0);
        data->intr = IPPB_INTERRUPTf;
    }
    if (147 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 147, 0);
        data->intr = IPPC_INTERRUPTf;
    }
    if (148 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 148, 0);
        data->intr = IPPD_INTERRUPTf;
    }
    if (224 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 224, 0);
        data->intr = IPPE_INTERRUPTf;
    }
    if (163 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 163, 0);
        data->intr = IPPF_INTERRUPTf;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->intr = IPS_INTERRUPTf;
    }
    if (72 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 72, 0);
        data->intr = IPT_INTERRUPTf;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->intr = IQM_INTERRUPTf;
    }
    if (67 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 67, 0);
        data->intr = IRE_INTERRUPTf;
    }
    if (73 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 73, 0);
        data->intr = ITPP_INTERRUPTf;
    }
    if (158 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 158, 0);
        data->intr = ITPPD_INTERRUPTf;
    }
    if (306 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 306, 0);
        data->intr = KAPS_INTERRUPTf;
    }
    if (310 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 310, 0);
        data->intr = MACT_INTERRUPTf;
    }
    if (86 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 86, 0);
        data->intr = MCP_INTERRUPTf;
    }
    if (307 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 307, 0);
        data->intr = MDB_INTERRUPTf;
    }
    if (2000 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 2000, 0);
        data->intr = MDB_ARM_INTERRUPTf;
    }
    if (81 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 81, 0);
        data->intr = MESH_TOPOLOGY_INTERRUPTf;
    }
    if (83 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 83, 0);
        data->intr = MRPS_INTERRUPTf;
    }
    if (236 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 236, 0);
        data->intr = MSW_0_INTERRUPTf;
    }
    if (237 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 237, 0);
        data->intr = MSW_1_INTERRUPTf;
    }
    if (161 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 161, 0);
        data->intr = MTM_INTERRUPTf;
    }
    if (308 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 308, 0);
        data->intr = OAMP_INTERRUPTf;
    }
    if (262 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 262, 0);
        data->intr = OCB_0_INTERRUPTf;
    }
    if (263 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 263, 0);
        data->intr = OCB_1_INTERRUPTf;
    }
    if (264 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 264, 0);
        data->intr = OCB_2_INTERRUPTf;
    }
    if (265 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 265, 0);
        data->intr = OCB_3_INTERRUPTf;
    }
    if (261 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 261, 0);
        data->intr = OCBM_INTERRUPTf;
    }
    if (179 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 179, 0);
        data->intr = OFR_INTERRUPTf;
    }
    if (189 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 189, 0);
        data->intr = OFT_INTERRUPTf;
    }
    if (311 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 311, 0);
        data->intr = OLP_INTERRUPTf;
    }
    if (84 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 84, 0);
        data->intr = PDM_INTERRUPTf;
    }
    if (152 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 152, 0);
        data->intr = PEM_0_INTERRUPTf;
    }
    if (153 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 153, 0);
        data->intr = PEM_1_INTERRUPTf;
    }
    if (154 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 154, 0);
        data->intr = PEM_2_INTERRUPTf;
    }
    if (155 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 155, 0);
        data->intr = PEM_3_INTERRUPTf;
    }
    if (143 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 143, 0);
        data->intr = PQP_INTERRUPTf;
    }
    if (142 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 142, 0);
        data->intr = RQP_INTERRUPTf;
    }
    if (77 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 77, 0);
        data->intr = RTP_INTERRUPTf;
    }
    if (89 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 89, 0);
        data->intr = SCH_INTERRUPTf;
    }
    if (64 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 64, 0);
        data->intr = SIF_INTERRUPTf;
    }
    if (157 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 157, 0);
        data->intr = SPB_INTERRUPTf;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->intr = SQM_INTERRUPTf;
    }
    if (305 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 305, 0);
        data->intr = KTM_INTERRUPTf;
    }
    if (196 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 196, 0);
        data->intr = TDU_INTERRUPTf;
    }
    if (140 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 140, 0);
        data->intr = TSN_INTERRUPTf;
    }
    if (461 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 461, 0);
        data->intr = W_40_INTERRUPTf;
    }
    if (463 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 463, 0);
        data->intr = WB_INTERRUPTf;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_skip_clear_on_init_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_skip_clear_on_init_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_skip_clear_on_init;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 2;
    table->info_get.key_size[0] = 2;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_skip_clear_on_init_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_skip_clear_on_init");

    
    default_data = (dnx_data_intr_general_skip_clear_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_skip_clear_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_skip_clear_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = J2X_INT_ILE_ILKN_RX_PORT_0_STATUS_CHANGE_INT;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_skip_clear_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2X_INT_ILE_ILKN_RX_PORT_1_STATUS_CHANGE_INT;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_force_unmask_on_init_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_force_unmask_on_init_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_force_unmask_on_init;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 0;
    table->info_get.key_size[0] = 0;

    
    table->values[0].default_val = "INVALIDr";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_force_unmask_on_init_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_force_unmask_on_init");

    
    default_data = (dnx_data_intr_general_force_unmask_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_force_unmask_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_general_framer_module_map_set(
    int unit)
{
    int offset_index;
    dnx_data_intr_general_framer_module_map_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int table_index = dnx_data_intr_general_table_framer_module_map;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 53;
    table->info_get.key_size[0] = 53;

    
    table->values[0].default_val = "INVALIDm";
    table->values[1].default_val = "0";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_general_framer_module_map_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_general_table_framer_module_map");

    
    default_data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->core_regs = INVALIDm;
    default_data->instance = 0;
    
    for (offset_index = 0; offset_index < table->keys[0].size; offset_index++)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, offset_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->core_regs = FLFR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->core_regs = FLFR_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->core_regs = FLFR_CORE_REGISTERSm;
        data->instance = 2;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->core_regs = FLFR_CORE_REGISTERSm;
        data->instance = 3;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->core_regs = FLFT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->core_regs = FLFT_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->core_regs = FFOA_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->core_regs = FFOB_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->core_regs = FDMX_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->core_regs = FDMX_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->core_regs = FAWR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->core_regs = FAWT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->core_regs = FBSW_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->core_regs = FOSW_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->core_regs = FPMR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->core_regs = FPMT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->core_regs = FLOTN_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->core_regs = FFLXO_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->core_regs = FDMXC_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->core_regs = FDMXBI_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->core_regs = FDMXA_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->core_regs = FDMXT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->core_regs = FMXC_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->core_regs = FMXBI_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->core_regs = FMXBJ_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->core_regs = FMXAI_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->core_regs = FMXT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->core_regs = FMX_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->core_regs = FMX_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->core_regs = FRA_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->core_regs = FRA_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->core_regs = FMAP_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->core_regs = FDMP_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->core_regs = FOAM_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->core_regs = FOAM_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->core_regs = FSAR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->core_regs = FGMAP_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->core_regs = FMACR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->core_regs = FGDMP_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->core_regs = FMACT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (40 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 40, 0);
        data->core_regs = FECRL_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (41 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 41, 0);
        data->core_regs = FDCRL_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (42 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 42, 0);
        data->core_regs = FECRS_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (43 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 43, 0);
        data->core_regs = FDCRS_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (44 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 44, 0);
        data->core_regs = FPTPR_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (45 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 45, 0);
        data->core_regs = FPTPT_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (46 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 46, 0);
        data->core_regs = FODUO_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (47 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 47, 0);
        data->core_regs = FODUO_CORE_REGISTERSm;
        data->instance = 1;
    }
    if (48 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 48, 0);
        data->core_regs = FMXAJ_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (49 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 49, 0);
        data->core_regs = FMXBK_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (50 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 50, 0);
        data->core_regs = FDMXBJ_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (51 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 51, 0);
        data->core_regs = FECPB_CORE_REGISTERSm;
        data->instance = 0;
    }
    if (52 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_framer_module_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 52, 0);
        data->core_regs = FICPB_CORE_REGISTERSm;
        data->instance = 0;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}






static shr_error_e
j2x_a0_dnx_data_intr_ser_tcam_protection_issue_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int define_index = dnx_data_intr_ser_define_tcam_protection_issue;
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
j2x_a0_dnx_data_intr_ser_mem_mask_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_mem_mask_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_mem_mask;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 0;
    table->info_get.key_size[0] = 0;

    
    table->values[0].default_val = "INVALIDr";
    table->values[1].default_val = "INVALIDf";
    table->values[2].default_val = "dnxc_mem_mask_mode_none";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_mem_mask_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_mem_mask");

    
    default_data = (dnx_data_intr_ser_mem_mask_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->reg = INVALIDr;
    default_data->field = INVALIDf;
    default_data->mode = dnxc_mem_mask_mode_none;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_mem_mask_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_ser_xor_mem_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_xor_mem_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_xor_mem;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 40;
    table->info_get.key_size[0] = 40;

    
    table->values[0].default_val = "INVALIDm";
    table->values[1].default_val = "0";
    table->values[2].default_val = "0";
    table->values[3].default_val = "0";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_xor_mem_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_xor_mem");

    
    default_data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->mem = INVALIDm;
    default_data->sram_banks_bits = 0;
    default_data->entry_used_bits = 0;
    default_data->flags = 0;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->mem = CGM_IPP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 8;
        data->flags = 0;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->mem = CGM_SYS_RED_QSIZE_RANGESm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->mem = CGM_VOQ_DRAM_BOUND_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->mem = CGM_VOQ_DRAM_RECOVERY_CACHE_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->mem = CGM_VOQ_DRAM_RECOVERY_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->mem = CGM_VOQ_GRNTD_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->mem = CGM_VOQ_PROFILESm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->mem = CGM_VOQ_VSQS_PRMSm;
        data->sram_banks_bits = 3;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->mem = ECGM_PDCT_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 7;
        data->flags = 0;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->mem = ECGM_PQST_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 7;
        data->flags = 0;
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->mem = ECGM_QDCT_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->mem = ECGM_QQST_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->mem = ERPP_OUTLIF_BTA_SOPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->mem = ERPP_PER_PP_PORT_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 8;
        data->flags = 0;
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->mem = ETPPA_LAYER_NWK_QOS_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->mem = ETPPA_PRP_EES_ARR_CONFIGURATIONm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->mem = ETPPB_EGRESS_MEMBERSHIPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->mem = ETPPB_ENC_2_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->mem = ETPPB_ENC_3_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->mem = ETPPB_ENC_4_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->mem = ETPPB_ENC_5_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->mem = ETPPB_NEW_QOS_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->mem = ETPPB_QOS_DP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->mem = ETPPB_SOURCE_ADRS_MAP_0m;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->mem = ETPPB_SOURCE_ADRS_MAP_1m;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->mem = ETPPC_ENC_1_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->mem = ETPPC_NEW_QOS_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->mem = ETPPC_PROTECTION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->mem = ETPPC_QOS_DP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->mem = IPPB_ETHERNET_OAM_OPCODE_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 7;
        data->flags = 0;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->mem = IPPD_DESTINATION_STATUSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->mem = IPS_CRBAL_THm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 4;
        data->flags = 0;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->mem = IPS_QFMm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->mem = IPS_QPRIORITYm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->mem = IPS_QSPMm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->mem = IPS_QTYPEm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->mem = IPS_SLOW_FACTOR_THm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 4;
        data->flags = 0;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->mem = IPS_SPMm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 14;
        data->flags = 0;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->mem = PQP_TXQ_THm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 8;
        data->flags = 0;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->mem = INVALIDm;
        data->sram_banks_bits = 0;
        data->entry_used_bits = 0;
        data->flags = 0;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_ser_intr_exception_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_intr_exception_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_intr_exception;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 2;
    table->info_get.key_size[0] = 2;

    
    table->values[0].default_val = "INVALIDr";
    table->values[1].default_val = "NULL";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_intr_exception_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_intr_exception");

    
    default_data = (dnx_data_intr_ser_intr_exception_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->intr = INVALIDr;
    default_data->exception_get_cb = NULL;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_intr_exception_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_intr_exception_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = J2X_INT_CGM_CONGESTION_GLBL_DRAM_BDBS_FC_INT;
        data->exception_get_cb = dnx_intr_exceptional_dram_not_present;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_intr_exception_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2X_INT_CGM_LOW_PRIORITY_GLBL_DRAM_BDBS_FC_LP_INT;
        data->exception_get_cb = dnx_intr_exceptional_dram_not_present;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_ser_static_mem_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_static_mem_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_static_mem;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 1;
    table->info_get.key_size[0] = 1;

    
    table->values[0].default_val = "INVALIDm";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_static_mem_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_static_mem");

    
    default_data = (dnx_data_intr_ser_static_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->mem = INVALIDm;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_static_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_static_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->mem = ETPPA_PRP_EES_ARR_CONFIGURATIONm;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_ser_mdb_mem_map_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_mdb_mem_map_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_mdb_mem_map;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 8;
    table->info_get.key_size[0] = 8;

    
    table->values[0].default_val = "INVALIDm";
    table->values[1].default_val = "INVALIDm";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_mdb_mem_map_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_mdb_mem_map");

    
    default_data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->acc_mem = INVALIDm;
    default_data->phy_mem = INVALIDm;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }
    
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->acc_mem = DDHA_MACRO_0_ENTRY_BANKm;
        data->phy_mem = DDHA_MACRO_0_PHYSICAL_ENTRY_BANKm;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->acc_mem = DDHA_MACRO_1_ENTRY_BANKm;
        data->phy_mem = DDHA_MACRO_1_PHYSICAL_ENTRY_BANKm;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->acc_mem = DDHA_MACRO_2_ENTRY_BANKm;
        data->phy_mem = DDHA_MACRO_2_PHYSICAL_ENTRY_BANKm;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->acc_mem = DDHA_MACRO_3_ENTRY_BANKm;
        data->phy_mem = DDHA_MACRO_3_PHYSICAL_ENTRY_BANKm;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->acc_mem = DDHB_MACRO_0_ENTRY_BANKm;
        data->phy_mem = DDHB_MACRO_0_PHYSICAL_ENTRY_BANKm;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->acc_mem = DDHB_MACRO_1_ENTRY_BANKm;
        data->phy_mem = DDHB_MACRO_1_PHYSICAL_ENTRY_BANKm;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->acc_mem = DDHB_MACRO_2_ENTRY_BANKm;
        data->phy_mem = DDHB_MACRO_2_PHYSICAL_ENTRY_BANKm;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mdb_mem_map_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->acc_mem = DDHB_MACRO_3_ENTRY_BANKm;
        data->phy_mem = DDHB_MACRO_3_PHYSICAL_ENTRY_BANKm;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2x_a0_dnx_data_intr_ser_severity_mem_set(
    int unit)
{
    int index_index;
    dnx_data_intr_ser_severity_mem_t *data, *default_data;
    dnxc_data_table_t *table;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_ser;
    int table_index = dnx_data_intr_ser_table_severity_mem;
    SHR_FUNC_INIT_VARS(unit);

    table = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].tables[table_index];
    
    table->flags |= DNXC_DATA_F_SUPPORTED;

    
    table->keys[0].size = 0;
    table->info_get.key_size[0] = 0;

    
    table->values[0].default_val = "INVALIDm";
    table->values[1].default_val = "0";
    
    if (table->keys[0].size == 0 || table->info_get.key_size[0] == 0)
    {
        SHR_EXIT();
    }

    
    DNXC_DATA_ALLOC(table->data, dnx_data_intr_ser_severity_mem_t, (1 * (table->keys[0].size) + 1  ), "data of dnx_data_intr_ser_table_severity_mem");

    
    default_data = (dnx_data_intr_ser_severity_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, -1, -1);
    default_data->mem = INVALIDm;
    default_data->severity = 0;
    
    for (index_index = 0; index_index < table->keys[0].size; index_index++)
    {
        data = (dnx_data_intr_ser_severity_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, index_index, 0);
        sal_memcpy(data, default_data, table->size_of_values);
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}



shr_error_e
j2x_a0_data_intr_attach(
    int unit)
{
    dnxc_data_module_t *module = NULL;
    dnxc_data_submodule_t *submodule = NULL;
    dnxc_data_define_t *define = NULL;
    dnxc_data_feature_t *feature = NULL;
    dnxc_data_table_t *table = NULL;
    int module_index = dnx_data_module_intr;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnxc_data[unit].modules[module_index];
    
    submodule_index = dnx_data_intr_submodule_general;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_intr_general_define_nof_interrupts;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_intr_general_nof_interrupts_set;
    data_index = dnx_data_intr_general_define_epni_ebd_interrupt_assert_enable;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_intr_general_epni_ebd_interrupt_assert_enable_set;

    

    
    data_index = dnx_data_intr_general_table_active_on_init;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_active_on_init_set;
    data_index = dnx_data_intr_general_table_disable_on_init;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_disable_on_init_set;
    data_index = dnx_data_intr_general_table_disable_print_on_init;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_disable_print_on_init_set;
    data_index = dnx_data_intr_general_table_block_intr_field_mapping;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_block_intr_field_mapping_set;
    data_index = dnx_data_intr_general_table_skip_clear_on_init;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_skip_clear_on_init_set;
    data_index = dnx_data_intr_general_table_force_unmask_on_init;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_force_unmask_on_init_set;
    data_index = dnx_data_intr_general_table_framer_module_map;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_general_framer_module_map_set;
    
    submodule_index = dnx_data_intr_submodule_ser;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_intr_ser_define_tcam_protection_issue;
    define = &submodule->defines[data_index];
    define->set = j2x_a0_dnx_data_intr_ser_tcam_protection_issue_set;

    

    
    data_index = dnx_data_intr_ser_table_mem_mask;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_mem_mask_set;
    data_index = dnx_data_intr_ser_table_xor_mem;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_xor_mem_set;
    data_index = dnx_data_intr_ser_table_intr_exception;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_intr_exception_set;
    data_index = dnx_data_intr_ser_table_static_mem;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_static_mem_set;
    data_index = dnx_data_intr_ser_table_mdb_mem_map;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_mdb_mem_map_set;
    data_index = dnx_data_intr_ser_table_severity_mem;
    table = &submodule->tables[data_index];
    table->set = j2x_a0_dnx_data_intr_ser_severity_mem_set;

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

