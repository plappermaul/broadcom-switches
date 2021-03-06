/** \file algo/swstate/auto_generated/diagnostic/instru_diagnostic.h
 *
 * sw state functions declarations
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef __DNX_INSTRU_DIAGNOSTIC_H__
#define __DNX_INSTRU_DIAGNOSTIC_H__

#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/dnx_sw_state_size.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/instru_types.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
#include <include/bcm_int/dnx/algo/instru/algo_instru.h>
#include <include/soc/dnx/dnx_data/auto_generated/dnx_data_instru.h>
#include <soc/dnxc/swstate/callbacks/sw_state_htb_callbacks.h>
#include <soc/dnxc/swstate/types/sw_state_bitmap.h>
#include <soc/dnxc/swstate/types/sw_state_hash_table.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum {
    INSTRU_INFO,
    INSTRU_SFLOW_INFO_INFO,
    INSTRU_SFLOW_INFO_SFLOW_DESTINATION_PROFILE_INFO,
    INSTRU_SFLOW_INFO_RAW_OUTLIFS_BY_PROFILE_INFO,
    INSTRU_SFLOW_INFO_SFLOW_ENCAP_TO_SFLOW_DESTINATION_PROFILE_INFO,
    INSTRU_SFLOW_INFO_NOF_SFLOW_ENCAPS_INFO,
    INSTRU_SFLOW_INFO_AGENT_IP_ADDRESS_INFO,
    INSTRU_EVENTOR_INFO_INFO,
    INSTRU_EVENTOR_INFO_FLAGS_INFO,
    INSTRU_EVENTOR_INFO_RX_SRAM_BANKS_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_BUILDER_CONFIGURED_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_CONTEXT_TYPE_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_BUFFER0_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_BUFFER1_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_BUFFER_SIZE_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_PROGRAM0_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_RX_CONTEXTS_INFO_PROGRAM1_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_QUEUE_TYPE_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_TX_SRAM_BANKS_BITMAP_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_BUFFER0_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_BUFFER1_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_BUFFER_SIZE_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_PROGRAM0_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_PROGRAM1_OFFSET_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_NOF_OPERATIONS_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_NOF_CONST_VALUES_INFO,
    INSTRU_EVENTOR_INFO_TX_CONTEXTS_INFO_BUILDERS_INFO,
    INSTRU_TRIGGER_INFO_INFO,
    INSTRU_TRIGGER_INFO_FLAGS_INFO,
    INSTRU_TRIGGER_INFO_START_TIME_INFO,
    INSTRU_TRIGGER_INFO_TRIGGER_PERIOD_INFO,
    INSTRU_TRIGGER_INFO_NOF_TRIGGERS_INFO,
    INSTRU_TRIGGER_INFO_ENABLE_INFO,
    INSTRU_INFO_NOF_ENTRIES
} sw_state_instru_layout_e;


extern dnx_sw_state_diagnostic_info_t instru_info[SOC_MAX_NUM_DEVICES][INSTRU_INFO_NOF_ENTRIES];

extern const char* instru_layout_str[INSTRU_INFO_NOF_ENTRIES];
int instru_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_sflow_destination_profile_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_raw_outlifs_by_profile_dump(
    int unit, int raw_outlifs_by_profile_idx_0, int raw_outlifs_by_profile_idx_1, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_sflow_encap_to_sflow_destination_profile_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_nof_sflow_encaps_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_sflow_info_agent_ip_address_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_flags_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_sram_banks_bitmap_dump(
    int unit, int rx_sram_banks_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_builder_configured_bitmap_dump(
    int unit, int builder_configured_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_bitmap_dump(
    int unit, int rx_contexts_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_context_type_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_buffer0_offset_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_buffer1_offset_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_buffer_size_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_program0_offset_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_rx_contexts_info_program1_offset_dump(
    int unit, int rx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_bitmap_dump(
    int unit, int tx_contexts_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_queue_type_bitmap_dump(
    int unit, int tx_contexts_queue_type_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_sram_banks_bitmap_dump(
    int unit, int tx_sram_banks_bitmap_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_buffer0_offset_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_buffer1_offset_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_buffer_size_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_program0_offset_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_program1_offset_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_nof_operations_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_nof_const_values_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_eventor_info_tx_contexts_info_builders_dump(
    int unit, int tx_contexts_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_flags_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_start_time_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_trigger_period_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_nof_triggers_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

int instru_trigger_info_enable_dump(
    int unit, int trigger_info_idx_0, dnx_sw_state_dump_filters_t filters);

#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __DNX_INSTRU_DIAGNOSTIC_H__ */
