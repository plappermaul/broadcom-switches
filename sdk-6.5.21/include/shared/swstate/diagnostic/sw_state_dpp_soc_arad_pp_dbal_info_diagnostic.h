/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_PP_DBAL_INFO_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_PP_DBAL_INFO_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_API_TEST
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dump_cb)(
    int unit, int pp_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_dump_cb)(
    int unit, int pp_idx_0, int dbal_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_dump_cb dump;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_diagnostic_cbs_t qual_type;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_diagnostic_cbs_t qual_offset;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_diagnostic_cbs_t ignore_qual_offset_for_entry_mngmnt;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_diagnostic_cbs_t qual_nof_bits;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_diagnostic_cbs_t qual_full_size;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_diagnostic_cbs_t qual_is_in_hdr;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_dump_cb dump;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_diagnostic_cbs_t stage;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_diagnostic_cbs_t program_id;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_diagnostic_cbs_t lookup_number;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_diagnostic_cbs_t key_id;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_diagnostic_cbs_t nof_bits_used_in_key;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_diagnostic_cbs_t ce_assigned;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_diagnostic_cbs_t public_lpm_lookup_size;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_dump_cb dump;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_diagnostic_cbs_t is_table_initiated;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_diagnostic_cbs_t physical_db_type;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_diagnostic_cbs_t db_prefix;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_diagnostic_cbs_t db_prefix_len;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_diagnostic_cbs_t qual_info;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_diagnostic_cbs_t nof_qualifiers;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_diagnostic_cbs_t additional_table_info;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_diagnostic_cbs_t table_name;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_diagnostic_cbs_t table_programs;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_diagnostic_cbs_t nof_table_programs;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_diagnostic_cbs_t nof_entries_added_to_table;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_diagnostic_cbs_t action;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_diagnostic_cbs_t nof_actions;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_diagnostic_cbs_t table_iterator;
} sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_dump_cb dump;
} sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_dbal_info_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_pp_dbal_info_dump_cb dump;
    sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_diagnostic_cbs_t dbal_tables;
    sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_diagnostic_cbs_t sem_a_prefix_mapping;
    sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_diagnostic_cbs_t sem_b_prefix_mapping;
} sw_state_dpp_soc_arad_pp_dbal_info_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_pp_dbal_info_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_API_TEST */

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_PP_DBAL_INFO_DIAGNOSTIC_H_ */
