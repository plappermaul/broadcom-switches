/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#include <sal/core/libc.h>
#include <shared/swstate/sw_state.h>
#include <shared/error.h>
#include <shared/swstate/sw_state_defs.h>
#include <shared/bsl.h>
#include <shared/swstate/sw_state_dump.h>
#include <shared/swstate/diagnostic/sw_state_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_dbal_info_diagnostic.h>

#ifdef BCM_WARM_BOOT_API_TEST
#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dump(int unit, int pp_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        uint32 j0, J0;
        sw_state_get_nof_elements(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info, sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info), &j0 ,&J0);
        for (; j0 < J0; j0++) {
            shr_sw_state_dump_update_current_idx(unit, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.dump(unit, pp_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.sem_a_prefix_mapping.dump(unit, pp_idx_0, j0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.sem_b_prefix_mapping.dump(unit, pp_idx_0, j0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_dump(int unit, int pp_idx_0, int dbal_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables), SOC_DPP_DBAL_SW_NOF_TABLES) ? SOC_DPP_DBAL_SW_NOF_TABLES - 1 : 0;
        for (; i0 < SOC_DPP_DBAL_SW_NOF_TABLES; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.is_table_initiated.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.physical_db_type.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.db_prefix.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.db_prefix_len.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_qualifiers.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.additional_table_info.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_name.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_table_programs.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_entries_added_to_table.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.action.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_actions.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_iterator.dump(unit, pp_idx_0, dbal_info_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].is_table_initiated,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].is_table_initiated: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].physical_db_type,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].physical_db_type: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].db_prefix,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].db_prefix: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].db_prefix_len,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].db_prefix_len: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info), SOC_PPC_FP_NOF_QUALS_PER_DB_MAX) ? SOC_PPC_FP_NOF_QUALS_PER_DB_MAX - 1 : 0;
        for (; i0 < SOC_PPC_FP_NOF_QUALS_PER_DB_MAX; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_type.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_offset.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.ignore_qual_offset_for_entry_mngmnt.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_nof_bits.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_full_size.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_is_in_hdr.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].qual_type,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].qual_type: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].qual_offset,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].qual_offset: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].ignore_qual_offset_for_entry_mngmnt,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].ignore_qual_offset_for_entry_mngmnt: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].qual_nof_bits,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].qual_nof_bits: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].qual_full_size,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].qual_full_size: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int qual_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].qual_info[qual_info_idx_0].qual_is_in_hdr,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].qual_info[%d].qual_is_in_hdr: ", unit, dbal_tables_idx_0, qual_info_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].nof_qualifiers,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].nof_qualifiers: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].additional_table_info,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].additional_table_info: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_name
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_name), DBAL_MAX_NAME_LENGTH) ? DBAL_MAX_NAME_LENGTH - 1 : 0;
        for (; i0 < DBAL_MAX_NAME_LENGTH; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_name[i0],
                "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_name[%d]: ", unit, dbal_tables_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs), DBAL_MAX_PROGRAMS_PER_TABLE) ? DBAL_MAX_PROGRAMS_PER_TABLE - 1 : 0;
        for (; i0 < DBAL_MAX_PROGRAMS_PER_TABLE; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.stage.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.program_id.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.lookup_number.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.key_id.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.nof_bits_used_in_key.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.ce_assigned.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
            sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.public_lpm_lookup_size.dump(unit, pp_idx_0, dbal_info_idx_0, dbal_tables_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].stage,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].stage: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].program_id,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].program_id: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].lookup_number,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].lookup_number: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].key_id,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].key_id: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].nof_bits_used_in_key,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].nof_bits_used_in_key: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].ce_assigned
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].ce_assigned), SOC_PPC_FP_NOF_QUALS_PER_DB_MAX) ? SOC_PPC_FP_NOF_QUALS_PER_DB_MAX - 1 : 0;
        for (; i0 < SOC_PPC_FP_NOF_QUALS_PER_DB_MAX; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].ce_assigned[i0],
                "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].ce_assigned[%d]: ", unit, dbal_tables_idx_0, table_programs_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0, int table_programs_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_programs[table_programs_idx_0].public_lpm_lookup_size,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_programs[%d].public_lpm_lookup_size: ", unit, dbal_tables_idx_0, table_programs_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].nof_table_programs,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].nof_table_programs: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].nof_entries_added_to_table,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].nof_entries_added_to_table: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].action
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].action), SOC_PPC_FP_NOF_ACTIONS_PER_DB_MAX) ? SOC_PPC_FP_NOF_ACTIONS_PER_DB_MAX - 1 : 0;
        for (; i0 < SOC_PPC_FP_NOF_ACTIONS_PER_DB_MAX; i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].action[i0],
                "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].action[%d]: ", unit, dbal_tables_idx_0, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].nof_actions,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].nof_actions: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_dump(int unit, int pp_idx_0, int dbal_info_idx_0, int dbal_tables_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->dbal_tables[dbal_tables_idx_0].table_iterator,
        "sw_state[%d]->dpp.soc.arad.pp->dbal_info->dbal_tables[%d].table_iterator: ", unit, dbal_tables_idx_0);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_dump(int unit, int pp_idx_0, int dbal_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_a_prefix_mapping
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_a_prefix_mapping), (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER)) ? (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER) - 1 : 0;
        for (; i0 < (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_a_prefix_mapping[i0],
                "sw_state[%d]->dpp.soc.arad.pp->dbal_info->sem_a_prefix_mapping[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_dump(int unit, int pp_idx_0, int dbal_info_idx_0){
    SW_STATE_INIT_FUNC_DEFS;
    {
        int i0;
        i0 = shr_sw_state_dump_check_all_the_same(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_b_prefix_mapping
            , sizeof(*sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_b_prefix_mapping), (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER)) ? (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER) - 1 : 0;
        for (; i0 < (1<<ARAD_PP_ISEM_ACCESS_MAX_PREFIX_SIZE_JER); i0++) {
            shr_sw_state_dump_update_current_idx(unit, i0);
            SW_STATE_PRINT_OPAQUE(unit, sw_state[unit]->dpp.soc.arad.pp->dbal_info->sem_b_prefix_mapping[i0],
                "sw_state[%d]->dpp.soc.arad.pp->dbal_info->sem_b_prefix_mapping[%d]: ", unit, i0);
        }
        shr_sw_state_dump_end_of_stride(unit);
    }
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 


int
sw_state_dpp_soc_arad_pp_dbal_info_diagnostic_cb_init(int unit){
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dump = sw_state_dpp_soc_arad_pp_dbal_info_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.is_table_initiated.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_is_table_initiated_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.physical_db_type.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_physical_db_type_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.db_prefix.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.db_prefix_len.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_db_prefix_len_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_type.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_type_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_offset.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_offset_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.ignore_qual_offset_for_entry_mngmnt.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_ignore_qual_offset_for_entry_mngmnt_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_nof_bits.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_nof_bits_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_full_size.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_full_size_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.qual_info.qual_is_in_hdr.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_qual_info_qual_is_in_hdr_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_qualifiers.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_qualifiers_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.additional_table_info.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_additional_table_info_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_name.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_name_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.stage.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_stage_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.program_id.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_program_id_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.lookup_number.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_lookup_number_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.key_id.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_key_id_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.nof_bits_used_in_key.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_nof_bits_used_in_key_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.ce_assigned.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_ce_assigned_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_programs.public_lpm_lookup_size.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_programs_public_lpm_lookup_size_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_table_programs.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_table_programs_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_entries_added_to_table.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_entries_added_to_table_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.action.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_action_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.nof_actions.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_nof_actions_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.table_iterator.dump = sw_state_dpp_soc_arad_pp_dbal_info_dbal_tables_table_iterator_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.sem_a_prefix_mapping.dump = sw_state_dpp_soc_arad_pp_dbal_info_sem_a_prefix_mapping_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
    sw_state_diagnostic[unit].dpp.soc.arad.pp.dbal_info.sem_b_prefix_mapping.dump = sw_state_dpp_soc_arad_pp_dbal_info_sem_b_prefix_mapping_dump;
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
#endif /* BCM_WARM_BOOT_API_TEST */
