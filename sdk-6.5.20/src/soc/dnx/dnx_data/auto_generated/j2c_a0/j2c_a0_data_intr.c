

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_INTR

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_intr.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_property.h>
#include <soc/dnx/intr/auto_generated/j2c/j2c_intr.h>
#include <soc/mcm/allenum.h>
#include <soc/dnxc/dnxc_intr_corr_act_func.h>








static shr_error_e
j2c_a0_dnx_data_intr_general_nof_interrupts_set(
    int unit)
{
    dnxc_data_define_t *define;
    int module_index = dnx_data_module_intr;
    int submodule_index = dnx_data_intr_submodule_general;
    int define_index = dnx_data_intr_general_define_nof_interrupts;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnxc_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    
    define->default_data = J2C_INT_LAST;

    
    define->data = J2C_INT_LAST;

    
    define->flags |= DNXC_DATA_F_SUPPORTED;

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_define_value_set(unit, define));


exit:
    SHR_FUNC_EXIT;
}



static shr_error_e
j2c_a0_dnx_data_intr_general_active_on_init_set(
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

    
    table->keys[0].size = 315;
    table->info_get.key_size[0] = 315;

    
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
        data->intr = J2C_INT_OCB_ERROR_ECC;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2C_INT_OCB_ERROR_FBC_BANK;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->intr = J2C_INT_OLP_ERROR_ECC;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->intr = J2C_INT_FDA_ERROR_ECC;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->intr = J2C_INT_EPRE_ERROR_ECC;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->intr = J2C_INT_SPB_ERROR_ECC;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->intr = J2C_INT_SPB_ERROR_REASSEMBLY;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->intr = J2C_INT_SPB_ERROR_FIFO;
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->intr = J2C_INT_PQP_ERROR_ECC;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->intr = J2C_INT_DDHB_ERROR_ECC;
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->intr = J2C_INT_ETPPA_ERROR_ECC;
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->intr = J2C_INT_ETPPA_ETPP_PRP_INT_VEC;
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->intr = J2C_INT_ETPPA_ETPPA_INT_VEC;
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->intr = J2C_INT_FDTL_ERROR_ECC;
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->intr = J2C_INT_BDM_ERROR_ECC;
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->intr = J2C_INT_ILU_ERROR_ECC;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->intr = J2C_INT_ERPP_ERROR_ECC;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->intr = J2C_INT_ERPP_ERPP_DISCARD_INT_VEC;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->intr = J2C_INT_ERPP_ERPP_DISCARD_INT_VEC_2;
    }
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->intr = J2C_INT_IPT_ERROR_ECC;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->intr = J2C_INT_IPT_ERROR_FIFOS;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->intr = J2C_INT_IPT_ERROR_ITE;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->intr = J2C_INT_EVNT_ERROR_ECC;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->intr = J2C_INT_EVNT_EVENTOR_INTERRUPT;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->intr = J2C_INT_TCAM_ERROR_ECC;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->intr = J2C_INT_FDR_ERROR_ECC;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_0;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_1;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_2;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_3;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_4;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_1_MAC_5;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_0;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_1;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_2;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_3;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_4;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_2_MAC_5;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_0;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_1;
    }
    if (40 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 40, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_2;
    }
    if (41 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 41, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_3;
    }
    if (42 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 42, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_4;
    }
    if (43 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 43, 0);
        data->intr = J2C_INT_FDR_INT_REG_P_3_MAC_5;
    }
    if (44 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 44, 0);
        data->intr = J2C_INT_FDR_INT_REG_GENERAL;
    }
    if (45 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 45, 0);
        data->intr = J2C_INT_FMAC_ERROR_ECC;
    }
    if (46 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 46, 0);
        data->intr = J2C_INT_FMAC_INT_REG_1;
    }
    if (47 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 47, 0);
        data->intr = J2C_INT_FMAC_INT_REG_2;
    }
    if (48 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 48, 0);
        data->intr = J2C_INT_FMAC_INT_REG_3;
    }
    if (49 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 49, 0);
        data->intr = J2C_INT_FMAC_INT_REG_4;
    }
    if (50 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 50, 0);
        data->intr = J2C_INT_FMAC_INT_REG_5;
    }
    if (51 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 51, 0);
        data->intr = J2C_INT_FMAC_INT_REG_6;
    }
    if (52 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 52, 0);
        data->intr = J2C_INT_FMAC_INT_REG_7;
    }
    if (53 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 53, 0);
        data->intr = J2C_INT_FMAC_INT_REG_8;
    }
    if (54 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 54, 0);
        data->intr = J2C_INT_ITPPD_ERROR_ECC;
    }
    if (55 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 55, 0);
        data->intr = J2C_INT_FDT_ERROR_ECC;
    }
    if (56 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 56, 0);
        data->intr = J2C_INT_TDU_ERROR_ECC;
    }
    if (57 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 57, 0);
        data->intr = J2C_INT_CFC_ERROR_ECC;
    }
    if (58 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 58, 0);
        data->intr = J2C_INT_IPPC_ERROR_ECC;
    }
    if (59 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 59, 0);
        data->intr = J2C_INT_RTP_ERROR_ECC;
    }
    if (60 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 60, 0);
        data->intr = J2C_INT_IPPF_ERROR_ECC;
    }
    if (61 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 61, 0);
        data->intr = J2C_INT_SQM_ERROR_ECC;
    }
    if (62 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 62, 0);
        data->intr = J2C_INT_SQM_TX_FIFOS_ERR_INT;
    }
    if (63 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 63, 0);
        data->intr = J2C_INT_IPPE_ERROR_ECC;
    }
    if (64 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 64, 0);
        data->intr = J2C_INT_HBC_ERROR_ECC;
    }
    if (65 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 65, 0);
        data->intr = J2C_INT_HBC_DSI_ERROR;
    }
    if (66 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 66, 0);
        data->intr = J2C_INT_HBC_RDR_ERROR;
    }
    if (67 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 67, 0);
        data->intr = J2C_INT_HBC_PIPELINES_ERROR;
    }
    if (68 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 68, 0);
        data->intr = J2C_INT_CDU_ERROR_ECC;
    }
    if (69 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 69, 0);
        data->intr = J2C_INT_CDU_NBI_PKT_DROP_COUNTERS_0_75P_INTERRUPT;
    }
    if (70 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 70, 0);
        data->intr = J2C_INT_CDU_LINK_STATUS_CHANGE_INT;
    }
    if (71 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 71, 0);
        data->intr = J2C_INT_CGM_ERROR_ECC;
    }
    if (72 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 72, 0);
        data->intr = J2C_INT_CGM_VOQ_OCCUPANCY_ERR_INT;
    }
    if (73 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 73, 0);
        data->intr = J2C_INT_CGM_VSQ_OCCUPANCY_ERR_INT;
    }
    if (74 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 74, 0);
        data->intr = J2C_INT_CGM_VOQ_TOTAL_OCCUPANCY_ERR_INT;
    }
    if (75 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 75, 0);
        data->intr = J2C_INT_CGM_VSQ_TOTAL_OCCUPANCY_ERR_INT;
    }
    if (76 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 76, 0);
        data->intr = J2C_INT_CGM_CONGESTION_INT;
    }
    if (77 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 77, 0);
        data->intr = J2C_INT_CGM_LOW_PRIORITY_INT;
    }
    if (78 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 78, 0);
        data->intr = J2C_INT_ETPPC_ERROR_ECC;
    }
    if (79 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 79, 0);
        data->intr = J2C_INT_ETPPC_ETPP_TERM_INT_VEC;
    }
    if (80 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 80, 0);
        data->intr = J2C_INT_ETPPC_ETPPC_INT_VEC;
    }
    if (81 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 81, 0);
        data->intr = J2C_INT_OAMP_ERROR_ECC;
    }
    if (82 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 82, 0);
        data->intr = J2C_INT_FQP_ERROR_ECC;
    }
    if (83 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 83, 0);
        data->intr = J2C_INT_FCR_ERROR_ECC;
    }
    if (84 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 84, 0);
        data->intr = J2C_INT_SCH_ERROR_ECC;
    }
    if (85 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 85, 0);
        data->intr = J2C_INT_ETPPB_ERROR_ECC;
    }
    if (86 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 86, 0);
        data->intr = J2C_INT_ETPPB_BTC_INT_VEC;
    }
    if (87 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 87, 0);
        data->intr = J2C_INT_ETPPB_ETPPB_INT_VEC;
    }
    if (88 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 88, 0);
        data->intr = J2C_INT_MTM_ERROR_ECC;
    }
    if (89 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 89, 0);
        data->intr = J2C_INT_IQM_ERROR_ECC;
    }
    if (90 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 90, 0);
        data->intr = J2C_INT_FSRD_ERROR_ECC;
    }
    if (91 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 91, 0);
        data->intr = J2C_INT_FSRD_INT_REG_0;
    }
    if (92 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 92, 0);
        data->intr = J2C_INT_IRE_ERROR_ECC;
    }
    if (93 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 93, 0);
        data->intr = J2C_INT_IRE_RCY_INTERFACE_ERROR;
    }
    if (94 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 94, 0);
        data->intr = J2C_INT_IRE_INTERNAL_INTERFACE_ERROR;
    }
    if (95 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 95, 0);
        data->intr = J2C_INT_IRE_NIF_ERROR;
    }
    if (96 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 96, 0);
        data->intr = J2C_INT_IPS_ERROR_ECC;
    }
    if (97 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 97, 0);
        data->intr = J2C_INT_IPS_ACTIVE_QUEUE_COUNT_ERR_INT;
    }
    if (98 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 98, 0);
        data->intr = J2C_INT_KAPS_ERROR_ECC;
    }
    if (99 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 99, 0);
        data->intr = J2C_INT_IPPD_ERROR_ECC;
    }
    if (100 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 100, 0);
        data->intr = J2C_INT_EDB_ERROR_ECC;
    }
    if (101 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 101, 0);
        data->intr = J2C_INT_PDM_ERROR_ECC;
    }
    if (102 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 102, 0);
        data->intr = J2C_INT_SIF_ERROR_ECC;
    }
    if (103 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 103, 0);
        data->intr = J2C_INT_SIF_STATISTICS_BILLING_FIFOS_ERR_INT;
    }
    if (104 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 104, 0);
        data->intr = J2C_INT_SIF_STATISTICS_QSIZE_FIFOS_ERR_INT;
    }
    if (105 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 105, 0);
        data->intr = J2C_INT_SIF_STATISTICS_BILLING_OPCODE_ERR_INT;
    }
    if (106 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 106, 0);
        data->intr = J2C_INT_ECGM_ERROR_ECC;
    }
    if (107 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 107, 0);
        data->intr = J2C_INT_ECGM_CGM_REP_AROUND_INT_VEC;
    }
    if (108 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 108, 0);
        data->intr = J2C_INT_MACT_ERROR_ECC;
    }
    if (109 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 109, 0);
        data->intr = J2C_INT_MACT_LOCAL_MACT_INT;
    }
    if (110 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 110, 0);
        data->intr = J2C_INT_EPNI_ERROR_ECC;
    }
    if (111 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 111, 0);
        data->intr = J2C_INT_DDP_ERROR_ECC;
    }
    if (112 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 112, 0);
        data->intr = J2C_INT_DDP_ERROR_EXTERNAL_MEM;
    }
    if (113 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 113, 0);
        data->intr = J2C_INT_DDP_ERROR_ITE;
    }
    if (114 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 114, 0);
        data->intr = J2C_INT_PEM_ERROR_ECC;
    }
    if (115 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 115, 0);
        data->intr = J2C_INT_CRPS_ERROR_ECC;
    }
    if (116 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 116, 0);
        data->intr = J2C_INT_CRPS_ERROR_LM_COLLIDE;
    }
    if (117 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 117, 0);
        data->intr = J2C_INT_CRPS_ERROR_COUNTER_OVF;
    }
    if (118 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 118, 0);
        data->intr = J2C_INT_CRPS_DMA_0_FULLNESS_EVENT;
    }
    if (119 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 119, 0);
        data->intr = J2C_INT_CRPS_DMA_1_FULLNESS_EVENT;
    }
    if (120 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 120, 0);
        data->intr = J2C_INT_CRPS_CONDITIONAL_TRIGGER_OCCURED;
    }
    if (121 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 121, 0);
        data->intr = J2C_INT_CRPS_CONDITIONAL_TRIGGER_WHILE_FIFO_IS_FULL;
    }
    if (122 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 122, 0);
        data->intr = J2C_INT_IPPA_ERROR_ECC;
    }
    if (123 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 123, 0);
        data->intr = J2C_INT_ITPP_ERROR_ECC;
    }
    if (124 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 124, 0);
        data->intr = J2C_INT_MRPS_ERROR_ECC;
    }
    if (125 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 125, 0);
        data->intr = J2C_INT_MRPS_INGRESS_ENG_WRAP_ERR_INT;
    }
    if (126 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 126, 0);
        data->intr = J2C_INT_MRPS_EGRESS_ENG_WRAP_ERR_INT;
    }
    if (127 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 127, 0);
        data->intr = J2C_INT_MRPS_INGRESS_ENG_OOP_FIFO_NOT_EMPTY_INT;
    }
    if (128 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 128, 0);
        data->intr = J2C_INT_MRPS_EGRESS_ENG_OOP_FIFO_NOT_EMPTY_INT;
    }
    if (129 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 129, 0);
        data->intr = J2C_INT_FCT_ERROR_ECC;
    }
    if (130 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 130, 0);
        data->intr = J2C_INT_ESB_ERROR_ECC;
    }
    if (131 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 131, 0);
        data->intr = J2C_INT_DDHA_ERROR_ECC;
    }
    if (132 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 132, 0);
        data->intr = J2C_INT_EPS_ERROR_ECC;
    }
    if (133 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 133, 0);
        data->intr = J2C_INT_IPPB_ERROR_ECC;
    }
    if (134 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 134, 0);
        data->intr = J2C_INT_IPPB_LEL_INTERRUPT_REGISTER_ONE;
    }
    if (135 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 135, 0);
        data->intr = J2C_INT_NMG_ERROR_ECC;
    }
    if (136 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 136, 0);
        data->intr = J2C_INT_CLU_ERROR_ECC;
    }
    if (137 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 137, 0);
        data->intr = J2C_INT_CLU_NBI_PKT_DROP_COUNTERS_0_75P_INTERRUPT;
    }
    if (138 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 138, 0);
        data->intr = J2C_INT_CLU_LINK_STATUS_CHANGE_INT;
    }
    if (139 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 139, 0);
        data->intr = J2C_INT_DQM_ERROR_ECC;
    }
    if (140 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 140, 0);
        data->intr = J2C_INT_DQM_TX_FIFOS_ERR_INT;
    }
    if (141 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 141, 0);
        data->intr = J2C_INT_RQP_ERROR_ECC;
    }
    if (142 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 142, 0);
        data->intr = J2C_INT_RQP_PKT_REAS_INT_VEC;
    }
    if (143 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 143, 0);
        data->intr = J2C_INT_RQP_PRS_INT_VEC;
    }
    if (144 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 144, 0);
        data->intr = J2C_INT_MCP_ERROR_ECC;
    }
    if (145 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 145, 0);
        data->intr = J2C_INT_ECI_ERROR_ECC;
    }
    if (146 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 146, 0);
        data->intr = J2C_INT_ILE_ERROR_ECC;
    }
    if (147 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 147, 0);
        data->intr = J2C_INT_HBMC_ERROR_ECC;
    }
    if (148 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 148, 0);
        data->intr = J2C_INT_MDB_ERROR_ECC;
    }
    if (149 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 149, 0);
        data->intr = J2C_INT_SPB_ECC_PARITY_ERR_INT;
    }
    if (150 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 150, 0);
        data->intr = J2C_INT_ETPPA_ECC_PARITY_ERR_INT;
    }
    if (151 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 151, 0);
        data->intr = J2C_INT_ILU_ECC_PARITY_ERR_INT;
    }
    if (152 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 152, 0);
        data->intr = J2C_INT_ERPP_ECC_PARITY_ERR_INT;
    }
    if (153 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 153, 0);
        data->intr = J2C_INT_IPPC_ECC_PARITY_ERR_INT;
    }
    if (154 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 154, 0);
        data->intr = J2C_INT_IPPF_ECC_PARITY_ERR_INT;
    }
    if (155 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 155, 0);
        data->intr = J2C_INT_IPPE_ECC_PARITY_ERR_INT;
    }
    if (156 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 156, 0);
        data->intr = J2C_INT_HBC_ECC_PARITY_ERR_INT;
    }
    if (157 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 157, 0);
        data->intr = J2C_INT_CDU_ECC_PARITY_ERR_INT;
    }
    if (158 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 158, 0);
        data->intr = J2C_INT_ETPPC_ECC_PARITY_ERR_INT;
    }
    if (159 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 159, 0);
        data->intr = J2C_INT_SCH_ECC_PARITY_ERR_INT;
    }
    if (160 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 160, 0);
        data->intr = J2C_INT_ETPPB_ECC_PARITY_ERR_INT;
    }
    if (161 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 161, 0);
        data->intr = J2C_INT_IRE_ECC_PARITY_ERR_INT;
    }
    if (162 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 162, 0);
        data->intr = J2C_INT_IPPD_ECC_PARITY_ERR_INT;
    }
    if (163 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 163, 0);
        data->intr = J2C_INT_IPPA_ECC_PARITY_ERR_INT;
    }
    if (164 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 164, 0);
        data->intr = J2C_INT_IPPB_ECC_PARITY_ERR_INT;
    }
    if (165 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 165, 0);
        data->intr = J2C_INT_CLU_ECC_PARITY_ERR_INT;
    }
    if (166 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 166, 0);
        data->intr = J2C_INT_RQP_ECC_PARITY_ERR_INT;
    }
    if (167 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 167, 0);
        data->intr = J2C_INT_MDB_ECC_PARITY_ERR_INT;
    }
    if (168 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 168, 0);
        data->intr = J2C_INT_OCB_ECC_ECC_1B_ERR_INT;
    }
    if (169 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 169, 0);
        data->intr = J2C_INT_OLP_ECC_ECC_1B_ERR_INT;
    }
    if (170 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 170, 0);
        data->intr = J2C_INT_FDA_ECC_ECC_1B_ERR_INT;
    }
    if (171 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 171, 0);
        data->intr = J2C_INT_EPRE_ECC_ECC_1B_ERR_INT;
    }
    if (172 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 172, 0);
        data->intr = J2C_INT_SPB_ECC_ECC_1B_ERR_INT;
    }
    if (173 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 173, 0);
        data->intr = J2C_INT_PQP_ECC_ECC_1B_ERR_INT;
    }
    if (174 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 174, 0);
        data->intr = J2C_INT_DDHB_ECC_ECC_1B_ERR_INT;
    }
    if (175 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 175, 0);
        data->intr = J2C_INT_ETPPA_ECC_ECC_1B_ERR_INT;
    }
    if (176 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 176, 0);
        data->intr = J2C_INT_FDTL_ECC_ECC_1B_ERR_INT;
    }
    if (177 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 177, 0);
        data->intr = J2C_INT_BDM_ECC_ECC_1B_ERR_INT;
    }
    if (178 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 178, 0);
        data->intr = J2C_INT_ILU_ECC_ECC_1B_ERR_INT;
    }
    if (179 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 179, 0);
        data->intr = J2C_INT_ERPP_ECC_ECC_1B_ERR_INT;
    }
    if (180 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 180, 0);
        data->intr = J2C_INT_IPT_ECC_ECC_1B_ERR_INT;
    }
    if (181 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 181, 0);
        data->intr = J2C_INT_EVNT_ECC_ECC_1B_ERR_INT;
    }
    if (182 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 182, 0);
        data->intr = J2C_INT_TCAM_ECC_ECC_1B_ERR_INT;
    }
    if (183 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 183, 0);
        data->intr = J2C_INT_FDR_ECC_ECC_1B_ERR_INT;
    }
    if (184 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 184, 0);
        data->intr = J2C_INT_FMAC_ECC_ECC_1B_ERR_INT;
    }
    if (185 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 185, 0);
        data->intr = J2C_INT_ITPPD_ECC_ECC_1B_ERR_INT;
    }
    if (186 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 186, 0);
        data->intr = J2C_INT_FDT_ECC_ECC_1B_ERR_INT;
    }
    if (187 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 187, 0);
        data->intr = J2C_INT_TDU_ECC_ECC_1B_ERR_INT;
    }
    if (188 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 188, 0);
        data->intr = J2C_INT_CFC_ECC_ECC_1B_ERR_INT;
    }
    if (189 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 189, 0);
        data->intr = J2C_INT_IPPC_ECC_ECC_1B_ERR_INT;
    }
    if (190 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 190, 0);
        data->intr = J2C_INT_RTP_ECC_ECC_1B_ERR_INT;
    }
    if (191 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 191, 0);
        data->intr = J2C_INT_IPPF_ECC_ECC_1B_ERR_INT;
    }
    if (192 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 192, 0);
        data->intr = J2C_INT_SQM_ECC_ECC_1B_ERR_INT;
    }
    if (193 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 193, 0);
        data->intr = J2C_INT_IPPE_ECC_ECC_1B_ERR_INT;
    }
    if (194 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 194, 0);
        data->intr = J2C_INT_HBC_ECC_ECC_1B_ERR_INT;
    }
    if (195 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 195, 0);
        data->intr = J2C_INT_CDU_ECC_ECC_1B_ERR_INT;
    }
    if (196 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 196, 0);
        data->intr = J2C_INT_CGM_ECC_ECC_1B_ERR_INT;
    }
    if (197 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 197, 0);
        data->intr = J2C_INT_ETPPC_ECC_ECC_1B_ERR_INT;
    }
    if (198 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 198, 0);
        data->intr = J2C_INT_OAMP_ECC_ECC_1B_ERR_INT;
    }
    if (199 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 199, 0);
        data->intr = J2C_INT_FQP_ECC_ECC_1B_ERR_INT;
    }
    if (200 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 200, 0);
        data->intr = J2C_INT_FCR_ECC_ECC_1B_ERR_INT;
    }
    if (201 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 201, 0);
        data->intr = J2C_INT_SCH_ECC_ECC_1B_ERR_INT;
    }
    if (202 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 202, 0);
        data->intr = J2C_INT_ETPPB_ECC_ECC_1B_ERR_INT;
    }
    if (203 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 203, 0);
        data->intr = J2C_INT_MTM_ECC_ECC_1B_ERR_INT;
    }
    if (204 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 204, 0);
        data->intr = J2C_INT_IQM_ECC_ECC_1B_ERR_INT;
    }
    if (205 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 205, 0);
        data->intr = J2C_INT_IRE_ECC_ECC_1B_ERR_INT;
    }
    if (206 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 206, 0);
        data->intr = J2C_INT_IPS_ECC_ECC_1B_ERR_INT;
    }
    if (207 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 207, 0);
        data->intr = J2C_INT_KAPS_ECC_ECC_1B_ERR_INT;
    }
    if (208 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 208, 0);
        data->intr = J2C_INT_IPPD_ECC_ECC_1B_ERR_INT;
    }
    if (209 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 209, 0);
        data->intr = J2C_INT_EDB_ECC_ECC_1B_ERR_INT;
    }
    if (210 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 210, 0);
        data->intr = J2C_INT_PDM_ECC_ECC_1B_ERR_INT;
    }
    if (211 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 211, 0);
        data->intr = J2C_INT_SIF_ECC_ECC_1B_ERR_INT;
    }
    if (212 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 212, 0);
        data->intr = J2C_INT_ECGM_ECC_ECC_1B_ERR_INT;
    }
    if (213 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 213, 0);
        data->intr = J2C_INT_MACT_ECC_ECC_1B_ERR_INT;
    }
    if (214 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 214, 0);
        data->intr = J2C_INT_EPNI_ECC_ECC_1B_ERR_INT;
    }
    if (215 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 215, 0);
        data->intr = J2C_INT_DDP_ECC_ECC_1B_ERR_INT;
    }
    if (216 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 216, 0);
        data->intr = J2C_INT_PEM_ECC_ECC_1B_ERR_INT;
    }
    if (217 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 217, 0);
        data->intr = J2C_INT_CRPS_ECC_ECC_1B_ERR_INT;
    }
    if (218 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 218, 0);
        data->intr = J2C_INT_IPPA_ECC_ECC_1B_ERR_INT;
    }
    if (219 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 219, 0);
        data->intr = J2C_INT_ITPP_ECC_ECC_1B_ERR_INT;
    }
    if (220 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 220, 0);
        data->intr = J2C_INT_MRPS_ECC_ECC_1B_ERR_INT;
    }
    if (221 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 221, 0);
        data->intr = J2C_INT_FCT_ECC_ECC_1B_ERR_INT;
    }
    if (222 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 222, 0);
        data->intr = J2C_INT_ESB_ECC_ECC_1B_ERR_INT;
    }
    if (223 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 223, 0);
        data->intr = J2C_INT_DDHA_ECC_ECC_1B_ERR_INT;
    }
    if (224 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 224, 0);
        data->intr = J2C_INT_EPS_ECC_ECC_1B_ERR_INT;
    }
    if (225 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 225, 0);
        data->intr = J2C_INT_IPPB_ECC_ECC_1B_ERR_INT;
    }
    if (226 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 226, 0);
        data->intr = J2C_INT_NMG_ECC_ECC_1B_ERR_INT;
    }
    if (227 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 227, 0);
        data->intr = J2C_INT_CLU_ECC_ECC_1B_ERR_INT;
    }
    if (228 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 228, 0);
        data->intr = J2C_INT_DQM_ECC_ECC_1B_ERR_INT;
    }
    if (229 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 229, 0);
        data->intr = J2C_INT_RQP_ECC_ECC_1B_ERR_INT;
    }
    if (230 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 230, 0);
        data->intr = J2C_INT_MCP_ECC_ECC_1B_ERR_INT;
    }
    if (231 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 231, 0);
        data->intr = J2C_INT_ECI_ECC_ECC_1B_ERR_INT;
    }
    if (232 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 232, 0);
        data->intr = J2C_INT_ILE_ECC_ECC_1B_ERR_INT;
    }
    if (233 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 233, 0);
        data->intr = J2C_INT_MDB_ECC_ECC_1B_ERR_INT;
    }
    if (234 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 234, 0);
        data->intr = J2C_INT_OCB_ECC_ECC_2B_ERR_INT;
    }
    if (235 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 235, 0);
        data->intr = J2C_INT_OLP_ECC_ECC_2B_ERR_INT;
    }
    if (236 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 236, 0);
        data->intr = J2C_INT_FDA_ECC_ECC_2B_ERR_INT;
    }
    if (237 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 237, 0);
        data->intr = J2C_INT_EPRE_ECC_ECC_2B_ERR_INT;
    }
    if (238 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 238, 0);
        data->intr = J2C_INT_SPB_ECC_ECC_2B_ERR_INT;
    }
    if (239 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 239, 0);
        data->intr = J2C_INT_PQP_ECC_ECC_2B_ERR_INT;
    }
    if (240 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 240, 0);
        data->intr = J2C_INT_DDHB_ECC_ECC_2B_ERR_INT;
    }
    if (241 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 241, 0);
        data->intr = J2C_INT_ETPPA_ECC_ECC_2B_ERR_INT;
    }
    if (242 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 242, 0);
        data->intr = J2C_INT_FDTL_ECC_ECC_2B_ERR_INT;
    }
    if (243 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 243, 0);
        data->intr = J2C_INT_BDM_ECC_ECC_2B_ERR_INT;
    }
    if (244 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 244, 0);
        data->intr = J2C_INT_ILU_ECC_ECC_2B_ERR_INT;
    }
    if (245 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 245, 0);
        data->intr = J2C_INT_ERPP_ECC_ECC_2B_ERR_INT;
    }
    if (246 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 246, 0);
        data->intr = J2C_INT_IPT_ECC_ECC_2B_ERR_INT;
    }
    if (247 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 247, 0);
        data->intr = J2C_INT_EVNT_ECC_ECC_2B_ERR_INT;
    }
    if (248 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 248, 0);
        data->intr = J2C_INT_TCAM_ECC_ECC_2B_ERR_INT;
    }
    if (249 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 249, 0);
        data->intr = J2C_INT_FDR_ECC_ECC_2B_ERR_INT;
    }
    if (250 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 250, 0);
        data->intr = J2C_INT_FMAC_ECC_ECC_2B_ERR_INT;
    }
    if (251 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 251, 0);
        data->intr = J2C_INT_ITPPD_ECC_ECC_2B_ERR_INT;
    }
    if (252 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 252, 0);
        data->intr = J2C_INT_FDT_ECC_ECC_2B_ERR_INT;
    }
    if (253 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 253, 0);
        data->intr = J2C_INT_TDU_ECC_ECC_2B_ERR_INT;
    }
    if (254 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 254, 0);
        data->intr = J2C_INT_CFC_ECC_ECC_2B_ERR_INT;
    }
    if (255 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 255, 0);
        data->intr = J2C_INT_IPPC_ECC_ECC_2B_ERR_INT;
    }
    if (256 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 256, 0);
        data->intr = J2C_INT_RTP_ECC_ECC_2B_ERR_INT;
    }
    if (257 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 257, 0);
        data->intr = J2C_INT_IPPF_ECC_ECC_2B_ERR_INT;
    }
    if (258 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 258, 0);
        data->intr = J2C_INT_SQM_ECC_ECC_2B_ERR_INT;
    }
    if (259 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 259, 0);
        data->intr = J2C_INT_IPPE_ECC_ECC_2B_ERR_INT;
    }
    if (260 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 260, 0);
        data->intr = J2C_INT_HBC_ECC_ECC_2B_ERR_INT;
    }
    if (261 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 261, 0);
        data->intr = J2C_INT_CDU_ECC_ECC_2B_ERR_INT;
    }
    if (262 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 262, 0);
        data->intr = J2C_INT_CGM_ECC_ECC_2B_ERR_INT;
    }
    if (263 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 263, 0);
        data->intr = J2C_INT_ETPPC_ECC_ECC_2B_ERR_INT;
    }
    if (264 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 264, 0);
        data->intr = J2C_INT_OAMP_ECC_ECC_2B_ERR_INT;
    }
    if (265 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 265, 0);
        data->intr = J2C_INT_FQP_ECC_ECC_2B_ERR_INT;
    }
    if (266 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 266, 0);
        data->intr = J2C_INT_FCR_ECC_ECC_2B_ERR_INT;
    }
    if (267 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 267, 0);
        data->intr = J2C_INT_SCH_ECC_ECC_2B_ERR_INT;
    }
    if (268 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 268, 0);
        data->intr = J2C_INT_ETPPB_ECC_ECC_2B_ERR_INT;
    }
    if (269 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 269, 0);
        data->intr = J2C_INT_MTM_ECC_ECC_2B_ERR_INT;
    }
    if (270 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 270, 0);
        data->intr = J2C_INT_IQM_ECC_ECC_2B_ERR_INT;
    }
    if (271 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 271, 0);
        data->intr = J2C_INT_IRE_ECC_ECC_2B_ERR_INT;
    }
    if (272 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 272, 0);
        data->intr = J2C_INT_IPS_ECC_ECC_2B_ERR_INT;
    }
    if (273 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 273, 0);
        data->intr = J2C_INT_KAPS_ECC_ECC_2B_ERR_INT;
    }
    if (274 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 274, 0);
        data->intr = J2C_INT_IPPD_ECC_ECC_2B_ERR_INT;
    }
    if (275 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 275, 0);
        data->intr = J2C_INT_EDB_ECC_ECC_2B_ERR_INT;
    }
    if (276 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 276, 0);
        data->intr = J2C_INT_PDM_ECC_ECC_2B_ERR_INT;
    }
    if (277 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 277, 0);
        data->intr = J2C_INT_SIF_ECC_ECC_2B_ERR_INT;
    }
    if (278 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 278, 0);
        data->intr = J2C_INT_ECGM_ECC_ECC_2B_ERR_INT;
    }
    if (279 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 279, 0);
        data->intr = J2C_INT_MACT_ECC_ECC_2B_ERR_INT;
    }
    if (280 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 280, 0);
        data->intr = J2C_INT_EPNI_ECC_ECC_2B_ERR_INT;
    }
    if (281 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 281, 0);
        data->intr = J2C_INT_DDP_ECC_ECC_2B_ERR_INT;
    }
    if (282 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 282, 0);
        data->intr = J2C_INT_PEM_ECC_ECC_2B_ERR_INT;
    }
    if (283 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 283, 0);
        data->intr = J2C_INT_CRPS_ECC_ECC_2B_ERR_INT;
    }
    if (284 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 284, 0);
        data->intr = J2C_INT_IPPA_ECC_ECC_2B_ERR_INT;
    }
    if (285 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 285, 0);
        data->intr = J2C_INT_ITPP_ECC_ECC_2B_ERR_INT;
    }
    if (286 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 286, 0);
        data->intr = J2C_INT_MRPS_ECC_ECC_2B_ERR_INT;
    }
    if (287 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 287, 0);
        data->intr = J2C_INT_FCT_ECC_ECC_2B_ERR_INT;
    }
    if (288 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 288, 0);
        data->intr = J2C_INT_ESB_ECC_ECC_2B_ERR_INT;
    }
    if (289 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 289, 0);
        data->intr = J2C_INT_DDHA_ECC_ECC_2B_ERR_INT;
    }
    if (290 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 290, 0);
        data->intr = J2C_INT_EPS_ECC_ECC_2B_ERR_INT;
    }
    if (291 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 291, 0);
        data->intr = J2C_INT_IPPB_ECC_ECC_2B_ERR_INT;
    }
    if (292 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 292, 0);
        data->intr = J2C_INT_NMG_ECC_ECC_2B_ERR_INT;
    }
    if (293 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 293, 0);
        data->intr = J2C_INT_CLU_ECC_ECC_2B_ERR_INT;
    }
    if (294 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 294, 0);
        data->intr = J2C_INT_DQM_ECC_ECC_2B_ERR_INT;
    }
    if (295 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 295, 0);
        data->intr = J2C_INT_RQP_ECC_ECC_2B_ERR_INT;
    }
    if (296 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 296, 0);
        data->intr = J2C_INT_MCP_ECC_ECC_2B_ERR_INT;
    }
    if (297 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 297, 0);
        data->intr = J2C_INT_ECI_ECC_ECC_2B_ERR_INT;
    }
    if (298 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 298, 0);
        data->intr = J2C_INT_ILE_ECC_ECC_2B_ERR_INT;
    }
    if (299 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 299, 0);
        data->intr = J2C_INT_MDB_ECC_ECC_2B_ERR_INT;
    }
    if (300 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 300, 0);
        data->intr = J2C_INT_TCAM_TCAM_PROTECTION_ERROR_1_BIT_ECC;
    }
    if (301 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 301, 0);
        data->intr = J2C_INT_TCAM_TCAM_PROTECTION_ERROR_2_BIT_ECC;
    }
    if (302 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 302, 0);
        data->intr = J2C_INT_DDP_DELETE_BDB_FIFO_NOT_EMPTY;
    }
    if (303 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 303, 0);
        data->intr = J2C_INT_DDP_DELETE_BDB_FIFO_FULL;
    }
    if (304 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 304, 0);
        data->intr = J2C_INT_ILE_ILKN_RX_PORT_0_STATUS_CHANGE_INT;
    }
    if (305 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 305, 0);
        data->intr = J2C_INT_ILE_ILKN_RX_PORT_1_STATUS_CHANGE_INT;
    }
    if (306 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 306, 0);
        data->intr = J2C_INT_KAPS_TECC_A_0_1B_ERROR;
    }
    if (307 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 307, 0);
        data->intr = J2C_INT_KAPS_TECC_A_0_2B_ERROR;
    }
    if (308 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 308, 0);
        data->intr = J2C_INT_KAPS_TECC_B_0_1B_ERROR;
    }
    if (309 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 309, 0);
        data->intr = J2C_INT_KAPS_TECC_B_0_2B_ERROR;
    }
    if (310 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 310, 0);
        data->intr = J2C_INT_KAPS_TECC_A_1_1B_ERROR;
    }
    if (311 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 311, 0);
        data->intr = J2C_INT_KAPS_TECC_A_1_2B_ERROR;
    }
    if (312 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 312, 0);
        data->intr = J2C_INT_KAPS_TECC_B_1_1B_ERROR;
    }
    if (313 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 313, 0);
        data->intr = J2C_INT_KAPS_TECC_B_1_2B_ERROR;
    }
    if (314 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_active_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 314, 0);
        data->intr = INVALIDr;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_intr_general_disable_on_init_set(
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
j2c_a0_dnx_data_intr_general_disable_print_on_init_set(
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

    
    table->keys[0].size = 5;
    table->info_get.key_size[0] = 5;

    
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
        data->intr = J2C_INT_OAMP_PENDING_EVENT;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2C_INT_OAMP_STAT_PENDING_EVENT;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->intr = J2C_INT_ILE_ILKN_RX_PORT_0_STATUS_CHANGE_INT;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->intr = J2C_INT_ILE_ILKN_RX_PORT_1_STATUS_CHANGE_INT;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_disable_print_on_init_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->intr = INVALIDr;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_intr_general_block_intr_field_mapping_set(
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
    
    if (82 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 82, 0);
        data->intr = BDM_0_INTERRUPTf;
    }
    if (1072 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1072, 0);
        data->intr = CDU_0_PM_INTERRUPTf;
    }
    if (1088 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1088, 0);
        data->intr = CDU_1_PM_INTERRUPTf;
    }
    if (1104 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1104, 0);
        data->intr = CDU_2_PM_INTERRUPTf;
    }
    if (1120 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1120, 0);
        data->intr = CDU_3_PM_INTERRUPTf;
    }
    if (59 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 59, 0);
        data->intr = CDU_0_INTERRUPTf;
    }
    if (60 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 60, 0);
        data->intr = CDU_1_INTERRUPTf;
    }
    if (135 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 135, 0);
        data->intr = CDU_2_INTERRUPTf;
    }
    if (134 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 134, 0);
        data->intr = CDU_3_INTERRUPTf;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->intr = CFC_0_INTERRUPTf;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->intr = CGM_0_INTERRUPTf;
    }
    if (1136 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1136, 0);
        data->intr = CLUP_0_PM_0_INTERRUPTf;
    }
    if (1152 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1152, 0);
        data->intr = CLUP_0_PM_1_INTERRUPTf;
    }
    if (1168 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1168, 0);
        data->intr = CLUP_0_PM_2_INTERRUPTf;
    }
    if (1184 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1184, 0);
        data->intr = CLUP_0_PM_3_INTERRUPTf;
    }
    if (1200 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1200, 0);
        data->intr = CLUP_1_PM_0_INTERRUPTf;
    }
    if (1216 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1216, 0);
        data->intr = CLUP_1_PM_1_INTERRUPTf;
    }
    if (1232 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1232, 0);
        data->intr = CLUP_1_PM_2_INTERRUPTf;
    }
    if (1248 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1248, 0);
        data->intr = CLUP_1_PM_3_INTERRUPTf;
    }
    if (1264 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1264, 0);
        data->intr = CLUP_2_PM_0_INTERRUPTf;
    }
    if (1280 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1280, 0);
        data->intr = CLUP_2_PM_1_INTERRUPTf;
    }
    if (1296 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1296, 0);
        data->intr = CLUP_2_PM_2_INTERRUPTf;
    }
    if (1312 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1312, 0);
        data->intr = CLUP_2_PM_3_INTERRUPTf;
    }
    if (1328 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1328, 0);
        data->intr = CLUP_3_PM_0_INTERRUPTf;
    }
    if (1344 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1344, 0);
        data->intr = CLUP_3_PM_1_INTERRUPTf;
    }
    if (1360 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1360, 0);
        data->intr = CLUP_3_PM_2_INTERRUPTf;
    }
    if (1376 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1376, 0);
        data->intr = CLUP_3_PM_3_INTERRUPTf;
    }
    if (1392 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1392, 0);
        data->intr = CLUP_4_PM_0_INTERRUPTf;
    }
    if (1408 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1408, 0);
        data->intr = CLUP_4_PM_1_INTERRUPTf;
    }
    if (1424 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1424, 0);
        data->intr = CLUP_4_PM_2_INTERRUPTf;
    }
    if (1440 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1440, 0);
        data->intr = CLUP_4_PM_3_INTERRUPTf;
    }
    if (1456 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1456, 0);
        data->intr = CLUP_5_PM_0_INTERRUPTf;
    }
    if (1472 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1472, 0);
        data->intr = CLUP_5_PM_1_INTERRUPTf;
    }
    if (1488 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1488, 0);
        data->intr = CLUP_5_PM_2_INTERRUPTf;
    }
    if (1504 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1504, 0);
        data->intr = CLUP_5_PM_3_INTERRUPTf;
    }
    if (132 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 132, 0);
        data->intr = CLU_0_INTERRUPTf;
    }
    if (128 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 128, 0);
        data->intr = CLU_1_INTERRUPTf;
    }
    if (124 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 124, 0);
        data->intr = CLU_2_INTERRUPTf;
    }
    if (120 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 120, 0);
        data->intr = CLU_3_INTERRUPTf;
    }
    if (57 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 57, 0);
        data->intr = CLU_4_INTERRUPTf;
    }
    if (53 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 53, 0);
        data->intr = CLU_5_INTERRUPTf;
    }
    if (131 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 131, 0);
        data->intr = CLUP_0_INTERRUPTf;
    }
    if (127 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 127, 0);
        data->intr = CLUP_1_INTERRUPTf;
    }
    if (123 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 123, 0);
        data->intr = CLUP_2_INTERRUPTf;
    }
    if (119 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 119, 0);
        data->intr = CLUP_3_INTERRUPTf;
    }
    if (56 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 56, 0);
        data->intr = CLUP_4_INTERRUPTf;
    }
    if (52 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 52, 0);
        data->intr = CLUP_5_INTERRUPTf;
    }
    if (85 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 85, 0);
        data->intr = CRPS_0_INTERRUPTf;
    }
    if (195 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 195, 0);
        data->intr = DDHA_0_INTERRUPTf;
    }
    if (202 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 202, 0);
        data->intr = DDHB_0_INTERRUPTf;
    }
    if (203 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 203, 0);
        data->intr = DDHB_1_INTERRUPTf;
    }
    if (204 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 204, 0);
        data->intr = DDHB_2_INTERRUPTf;
    }
    if (205 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 205, 0);
        data->intr = DDHB_3_INTERRUPTf;
    }
    if (156 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 156, 0);
        data->intr = DDP_0_INTERRUPTf;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->intr = DQM_0_INTERRUPTf;
    }
    if (141 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 141, 0);
        data->intr = ECGM_0_INTERRUPTf;
    }
    if (0 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 0, 0);
        data->intr = ECI_INTERRUPTf;
    }
    if (133 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 133, 0);
        data->intr = EDB_0_INTERRUPTf;
    }
    if (66 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 66, 0);
        data->intr = EPNI_0_INTERRUPTf;
    }
    if (65 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 65, 0);
        data->intr = EPRE_0_INTERRUPTf;
    }
    if (139 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 139, 0);
        data->intr = EPS_0_INTERRUPTf;
    }
    if (144 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 144, 0);
        data->intr = ERPP_0_INTERRUPTf;
    }
    if (88 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 88, 0);
        data->intr = ESB_0_INTERRUPTf;
    }
    if (90 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 90, 0);
        data->intr = ETPPA_0_INTERRUPTf;
    }
    if (145 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 145, 0);
        data->intr = ETPPB_0_INTERRUPTf;
    }
    if (68 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 68, 0);
        data->intr = ETPPC_0_INTERRUPTf;
    }
    if (209 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 209, 0);
        data->intr = EVENTOR_INTERRUPTf;
    }
    if (78 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 78, 0);
        data->intr = FCR_0_INTERRUPTf;
    }
    if (80 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 80, 0);
        data->intr = FCT_0_INTERRUPTf;
    }
    if (79 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 79, 0);
        data->intr = FDA_0_INTERRUPTf;
    }
    if (74 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 74, 0);
        data->intr = FDR_0_INTERRUPTf;
    }
    if (75 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 75, 0);
        data->intr = FDT_0_INTERRUPTf;
    }
    if (76 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 76, 0);
        data->intr = FDTL_0_INTERRUPTf;
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
    if (138 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 138, 0);
        data->intr = FQP_0_INTERRUPTf;
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
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->intr = HBC_0_INTERRUPTf;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->intr = HBC_1_INTERRUPTf;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->intr = HBC_2_INTERRUPTf;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->intr = HBC_3_INTERRUPTf;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->intr = HBC_4_INTERRUPTf;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->intr = HBC_5_INTERRUPTf;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->intr = HBC_6_INTERRUPTf;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->intr = HBC_7_INTERRUPTf;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->intr = HBMC_0_INTERRUPTf;
    }
    if (130 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 130, 0);
        data->intr = ILE_0_INTERRUPTf;
    }
    if (126 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 126, 0);
        data->intr = ILE_1_INTERRUPTf;
    }
    if (122 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 122, 0);
        data->intr = ILE_2_INTERRUPTf;
    }
    if (118 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 118, 0);
        data->intr = ILE_3_INTERRUPTf;
    }
    if (55 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 55, 0);
        data->intr = ILE_4_INTERRUPTf;
    }
    if (51 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 51, 0);
        data->intr = ILE_5_INTERRUPTf;
    }
    if (62 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 62, 0);
        data->intr = ILE_6_INTERRUPTf;
    }
    if (137 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 137, 0);
        data->intr = ILE_7_INTERRUPTf;
    }
    if (129 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 129, 0);
        data->intr = ILU_0_INTERRUPTf;
    }
    if (125 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 125, 0);
        data->intr = ILU_1_INTERRUPTf;
    }
    if (121 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 121, 0);
        data->intr = ILU_2_INTERRUPTf;
    }
    if (117 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 117, 0);
        data->intr = ILU_3_INTERRUPTf;
    }
    if (54 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 54, 0);
        data->intr = ILU_4_INTERRUPTf;
    }
    if (50 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 50, 0);
        data->intr = ILU_5_INTERRUPTf;
    }
    if (61 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 61, 0);
        data->intr = ILU_6_INTERRUPTf;
    }
    if (136 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 136, 0);
        data->intr = ILU_7_INTERRUPTf;
    }
    if (164 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 164, 0);
        data->intr = IPPA_0_INTERRUPTf;
    }
    if (165 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 165, 0);
        data->intr = IPPB_0_INTERRUPTf;
    }
    if (147 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 147, 0);
        data->intr = IPPC_0_INTERRUPTf;
    }
    if (148 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 148, 0);
        data->intr = IPPD_0_INTERRUPTf;
    }
    if (162 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 162, 0);
        data->intr = IPPE_0_INTERRUPTf;
    }
    if (163 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 163, 0);
        data->intr = IPPF_0_INTERRUPTf;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->intr = IPS_0_INTERRUPTf;
    }
    if (72 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 72, 0);
        data->intr = IPT_0_INTERRUPTf;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->intr = IQM_0_INTERRUPTf;
    }
    if (67 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 67, 0);
        data->intr = IRE_0_PM_INTERRUPTf;
    }
    if (73 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 73, 0);
        data->intr = ITPPB_0_INTERRUPTf;
    }
    if (158 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 158, 0);
        data->intr = ITPPA_0_INTERRUPTf;
    }
    if (207 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 207, 0);
        data->intr = KAPS_0_INTERRUPTf;
    }
    if (210 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 210, 0);
        data->intr = MACT_INTERRUPTf;
    }
    if (86 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 86, 0);
        data->intr = MCP_0_INTERRUPTf;
    }
    if (192 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 192, 0);
        data->intr = MDB_INTERRUPTf;
    }
    if (1792 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 1792, 0);
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
        data->intr = MRPS_0_INTERRUPTf;
    }
    if (161 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 161, 0);
        data->intr = MTM_INTERRUPTf;
    }
    if (63 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 63, 0);
        data->intr = NMG_0_INTERRUPTf;
    }
    if (208 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 208, 0);
        data->intr = OAMP_INTERRUPTf;
    }
    if (58 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 58, 0);
        data->intr = OCB_0_INTERRUPTf;
    }
    if (211 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 211, 0);
        data->intr = OLP_INTERRUPTf;
    }
    if (84 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 84, 0);
        data->intr = PDM_0_INTERRUPTf;
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
        data->intr = PQP_0_INTERRUPTf;
    }
    if (142 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 142, 0);
        data->intr = RQP_0_INTERRUPTf;
    }
    if (77 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 77, 0);
        data->intr = RTP_0_INTERRUPTf;
    }
    if (89 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 89, 0);
        data->intr = SCH_0_INTERRUPTf;
    }
    if (64 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 64, 0);
        data->intr = SIF_0_INTERRUPTf;
    }
    if (157 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 157, 0);
        data->intr = SPB_0_INTERRUPTf;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->intr = SQM_0_INTERRUPTf;
    }
    if (206 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 206, 0);
        data->intr = KTM_0_INTERRUPTf;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_general_block_intr_field_mapping_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->intr = TDU_0_INTERRUPTf;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}






static shr_error_e
j2c_a0_dnx_data_intr_ser_tcam_protection_issue_set(
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
j2c_a0_dnx_data_intr_ser_mem_mask_set(
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

    
    table->keys[0].size = 10;
    table->info_get.key_size[0] = 10;

    
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
    
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mem_mask_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->reg = RTP_ECC_ERR_1B_MONITOR_MEM_MASKr;
        data->field = UNICAST_DISTRIBUTION_MEMORY_ECC_1B_ERR_MASKf;
        data->mode = dnxc_mem_mask_mode_zero;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_mem_mask_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->reg = RTP_ECC_ERR_2B_MONITOR_MEM_MASKr;
        data->field = UNICAST_DISTRIBUTION_MEMORY_ECC_2B_ERR_MASKf;
        data->mode = dnxc_mem_mask_mode_zero;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_intr_ser_xor_mem_set(
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

    
    table->keys[0].size = 48;
    table->info_get.key_size[0] = 48;

    
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
        data->mem = CGM_PB_VSQ_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->mem = CGM_IPP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = DNXC_XOR_MEMORY_FORCE_SEVERITY_FLAGS;
    }
    if (2 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 2, 0);
        data->mem = CGM_VSQF_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (3 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 3, 0);
        data->mem = CGM_VOQ_VSQS_PRMSm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 13;
        data->flags = DNXC_XOR_MEMORY_FORCE_SEVERITY_FLAGS;
    }
    if (4 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 4, 0);
        data->mem = CGM_VOQ_PROFILESm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (5 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 5, 0);
        data->mem = CGM_VOQ_DRAM_BOUND_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (6 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 6, 0);
        data->mem = CGM_VOQ_DRAM_RECOVERY_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (7 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 7, 0);
        data->mem = CGM_SYS_RED_QSIZE_RANGESm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (8 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 8, 0);
        data->mem = CGM_VOQ_DRAM_RECOVERY_CACHE_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (9 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 9, 0);
        data->mem = CGM_VOQ_GRNTD_PRMSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (10 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 10, 0);
        data->mem = DDHA_MACRO_0_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (11 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 11, 0);
        data->mem = DDHA_MACRO_1_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (12 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 12, 0);
        data->mem = DDHA_MACRO_2_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (13 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 13, 0);
        data->mem = DDHA_MACRO_3_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (14 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 14, 0);
        data->mem = DDHB_MACRO_0_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (15 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 15, 0);
        data->mem = DDHB_MACRO_1_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (16 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 16, 0);
        data->mem = DDHB_MACRO_2_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (17 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 17, 0);
        data->mem = DDHB_MACRO_3_PHYSICAL_ENTRY_BANKm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (18 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 18, 0);
        data->mem = ECGM_QQST_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (19 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 19, 0);
        data->mem = ECGM_QDCT_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 6;
        data->flags = 0;
    }
    if (20 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 20, 0);
        data->mem = ERPP_PER_PP_PORT_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (21 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 21, 0);
        data->mem = ETPPA_LAYER_NWK_QOS_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (22 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 22, 0);
        data->mem = ETPPB_NEW_QOS_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (23 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 23, 0);
        data->mem = ETPPB_QOS_DP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (24 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 24, 0);
        data->mem = ETPPB_EGRESS_MEMBERSHIPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (25 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 25, 0);
        data->mem = ETPPB_SOURCE_ADRS_MAP_0m;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (26 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 26, 0);
        data->mem = ETPPB_SOURCE_ADRS_MAP_1m;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (27 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 27, 0);
        data->mem = ETPPB_ENC_2_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (28 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 28, 0);
        data->mem = ETPPB_ENC_3_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (29 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 29, 0);
        data->mem = ETPPB_ENC_4_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (30 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 30, 0);
        data->mem = ETPPB_ENC_5_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (31 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 31, 0);
        data->mem = ETPPC_NEW_QOS_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (32 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 32, 0);
        data->mem = ETPPC_PROTECTION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (33 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 33, 0);
        data->mem = ETPPC_QOS_DP_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (34 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 34, 0);
        data->mem = ETPPC_ENC_1_ARR_0_FORMAT_CONFIGURATION_TABLEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 5;
        data->flags = 0;
    }
    if (35 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 35, 0);
        data->mem = MRPS_EGR_ENG_PRFCFGm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (36 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 36, 0);
        data->mem = MRPS_ING_ENG_PRFCFGm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 9;
        data->flags = 0;
    }
    if (37 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 37, 0);
        data->mem = IPPB_ETHERNET_OAM_OPCODE_MAPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 7;
        data->flags = 0;
    }
    if (38 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 38, 0);
        data->mem = IPPD_FEC_ECMPm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 10;
        data->flags = 0;
    }
    if (39 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 39, 0);
        data->mem = IPPD_DESTINATION_STATUSm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 12;
        data->flags = 0;
    }
    if (40 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 40, 0);
        data->mem = IPS_QTYPEm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 13;
        data->flags = 0;
    }
    if (41 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 41, 0);
        data->mem = IPS_QSPMm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 14;
        data->flags = 0;
    }
    if (42 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 42, 0);
        data->mem = IPS_QFMm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 14;
        data->flags = 0;
    }
    if (43 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 43, 0);
        data->mem = IPS_SPMm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 14;
        data->flags = 0;
    }
    if (44 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 44, 0);
        data->mem = IPS_CRBAL_THm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 4;
        data->flags = 0;
    }
    if (45 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 45, 0);
        data->mem = IPS_SLOW_FACTOR_THm;
        data->sram_banks_bits = 1;
        data->entry_used_bits = 4;
        data->flags = 0;
    }
    if (46 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 46, 0);
        data->mem = IPS_QPRIORITYm;
        data->sram_banks_bits = 2;
        data->entry_used_bits = 11;
        data->flags = 0;
    }
    if (47 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_xor_mem_t *) dnxc_data_mgmt_table_data_get(unit, table, 47, 0);
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
j2c_a0_dnx_data_intr_ser_intr_exception_set(
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
        data->intr = J2C_INT_CGM_CONGESTION_GLBL_DRAM_BDBS_FC_INT;
        data->exception_get_cb = dnx_intr_exceptional_dram_not_present;
    }
    if (1 < table->keys[0].size)
    {
        data = (dnx_data_intr_ser_intr_exception_t *) dnxc_data_mgmt_table_data_get(unit, table, 1, 0);
        data->intr = J2C_INT_CGM_LOW_PRIORITY_GLBL_DRAM_BDBS_FC_LP_INT;
        data->exception_get_cb = dnx_intr_exceptional_dram_not_present;
    }

    
    SHR_IF_ERR_EXIT(dnxc_data_dyn_loader_table_value_set(unit, table));


exit:
    SHR_FUNC_EXIT;
}


static shr_error_e
j2c_a0_dnx_data_intr_ser_mdb_mem_map_set(
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



shr_error_e
j2c_a0_data_intr_attach(
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
    define->set = j2c_a0_dnx_data_intr_general_nof_interrupts_set;

    

    
    data_index = dnx_data_intr_general_table_active_on_init;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_general_active_on_init_set;
    data_index = dnx_data_intr_general_table_disable_on_init;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_general_disable_on_init_set;
    data_index = dnx_data_intr_general_table_disable_print_on_init;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_general_disable_print_on_init_set;
    data_index = dnx_data_intr_general_table_block_intr_field_mapping;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_general_block_intr_field_mapping_set;
    
    submodule_index = dnx_data_intr_submodule_ser;
    submodule = &module->submodules[submodule_index];

    
    data_index = dnx_data_intr_ser_define_tcam_protection_issue;
    define = &submodule->defines[data_index];
    define->set = j2c_a0_dnx_data_intr_ser_tcam_protection_issue_set;

    

    
    data_index = dnx_data_intr_ser_table_mem_mask;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_ser_mem_mask_set;
    data_index = dnx_data_intr_ser_table_xor_mem;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_ser_xor_mem_set;
    data_index = dnx_data_intr_ser_table_intr_exception;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_ser_intr_exception_set;
    data_index = dnx_data_intr_ser_table_mdb_mem_map;
    table = &submodule->tables[data_index];
    table->set = j2c_a0_dnx_data_intr_ser_mdb_mem_map_set;

    SHR_FUNC_EXIT;
}
#undef BSL_LOG_MODULE

