/** \file algo/swstate/auto_generated/diagnostic/dnx_ipq_alloc_mngr_diagnostic.h
 *
 * sw state functions declarations
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef __DNX_IPQ_ALLOC_MNGR_DIAGNOSTIC_H__
#define __DNX_IPQ_ALLOC_MNGR_DIAGNOSTIC_H__

#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/dnx_sw_state_size.h>
#include <soc/dnxc/swstate/dnx_sw_state_diagnostic_operation_counters.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/dnx_ipq_alloc_mngr_types.h>
#include <bcm_int/dnx/algo/ipq/ipq_alloc_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
#include <bcm_int/dnx/cosq/cosq.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum {
    DNX_IPQ_ALLOC_MNGR_DB_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_REGIONS_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_REGIONS_TYPE_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_REGIONS_NOF_ALLOCATED_QUEUE_QUARTETS_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_FMQ_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_FMQ_IS_ENHANCE_SCHEDULER_MODE_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_FMQ_MAX_MULTICAST_QUEUE_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_IPQ_ALLOC_MNGR_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_TC_VOQ_FLOW_MAP_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_TC_SYSPORT_MAP_INFO,
    DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES
} sw_state_dnx_ipq_alloc_mngr_db_layout_e;


extern dnx_sw_state_diagnostic_info_t dnx_ipq_alloc_mngr_db_info[SOC_MAX_NUM_DEVICES][DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES];

extern const char* dnx_ipq_alloc_mngr_db_layout_str[DNX_IPQ_ALLOC_MNGR_DB_INFO_NOF_ENTRIES];
int dnx_ipq_alloc_mngr_db_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_regions_dump(
    int unit, int regions_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_regions_type_dump(
    int unit, int regions_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_regions_nof_allocated_queue_quartets_dump(
    int unit, int regions_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_fmq_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_fmq_is_enhance_scheduler_mode_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_fmq_max_multicast_queue_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_ipq_alloc_mngr_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_tc_voq_flow_map_dump(
    int unit, int tc_voq_flow_map_idx_0, dnx_sw_state_dump_filters_t filters);

int dnx_ipq_alloc_mngr_db_tc_sysport_map_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __DNX_IPQ_ALLOC_MNGR_DIAGNOSTIC_H__ */
