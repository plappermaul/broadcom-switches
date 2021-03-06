/** \file init_custom_funcs.h
 * Custom functions to be used to initialize DNX memories. 
 */

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifndef _BCMINT_DNX_INIT_CUSTOM_FUNCS_H_INCLUDED
/*
 * { 
 */
#define _BCMINT_DNX_INIT_CUSTOM_FUNCS_H_INCLUDED

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX family only!"
#endif

/*
 * Include files.
 * {
 */

/*
 * }
 */

/**
 * \brief - set entry_data with the init value of SCH_FLOW_TO_FIP_MAPPING_FFM  memory
 * 
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_flow_to_fip_mapping_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of  SCH_SHAPER_DESCRIPTOR_MEMORY_STATIC_SHDS memory
 * 
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_shaper_descr_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of  SCH_PORT_SCHEDULER_WEIGHTS_PSW memory
 * 
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_port_schduler_weights_psw_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of  SCH_FLOW_DESCRIPTOR_MEMORY_STATIC_FDMSm memory
 * 
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_flow_descr_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of IPT_PCP_CFGm memory
 * 
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_fabric_pcp_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of ERPP_TM_PP_DSP_PTR_TABLEm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_erpp_tm_pp_dsp_ptr_table_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of ERPP_PRP_PP_DSP_PTR_TABLEm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_erpp_prp_pp_dsp_ptr_table_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of ETPPA_DSP_DATA_TABLEm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_etppa_dsp_data_table_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of IPPE_PRT_PP_PORT_INFO memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_ippe_prt_pp_port_info_table_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of ETPPA_PRP_FES_PROGRAM_TABLEm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_etppa_prp_fes_program_table_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of SCH_CIR_SHAPERS_STATIC_TABEL_CSST memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_csst_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of SCH_PIR_SHAPERS_STATIC_TABEL_PSST memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sch_psst_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of CGM_IPP_MAP memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_cgm_ipp_map_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of IPPC_FEM_BIT_SELECTm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_ippc_fem_bit_select_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set entry_data with the init value of TCAM_TCAM_BANK_COMMANDm memory
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_tcam_tcam_bank_command_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set MTU of MACSEC_ESEC_SC_TABLE to 64
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_xflow_macsec_default_esec_sc_entry_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - set MTU of MACSEC_SUB_PORT_POLICY_TABLE to 300
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_xflow_macsec_default_isec_policy_entry_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - Set IPPB_PINFO_FLP_MEM ACTION_PROFILE_SA_NOT_FOUND_INDEX to 1
 *
 * \param [in] unit - unit number
 * \param [in] array_index - irrelevant for this cb
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_sa_not_found_action_profile_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);

/**
 * \brief - return EPS_CH_IFC_CFG default value per array_index
 *
 * \param [in] unit - unit number
 * \param [in] array_index - array index
 * \param [in] copyno - irrelevant for this cb
 * \param [in] index - irrelevant for this cb
 * \param [in] entry_data - returned value to set to memory
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_init_custom_eps_ch_ifc_cfg_default_get(
    int unit,
    unsigned array_index,
    int copyno,
    int index,
    uint32 *entry_data);
#endif /** _BCMINT_DNX_INIT_CUSTOM_FUNCS_H_INCLUDED */
