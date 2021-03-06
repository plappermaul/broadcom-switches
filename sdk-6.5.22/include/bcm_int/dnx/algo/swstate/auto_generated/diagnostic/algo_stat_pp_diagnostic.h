/** \file algo/swstate/auto_generated/diagnostic/algo_stat_pp_diagnostic.h
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
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifndef __ALGO_STAT_PP_DIAGNOSTIC_H__
#define __ALGO_STAT_PP_DIAGNOSTIC_H__

#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnxc/swstate/dnx_sw_state_size.h>
#include <soc/dnxc/swstate/dnx_sw_state_diagnostic_operation_counters.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/algo_stat_pp_types.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
typedef enum {
    STAT_PP_DB_INFO,
    STAT_PP_DB_STAT_PP_IRPP_PROFILE_ID_INFO,
    STAT_PP_DB_STAT_PP_ETPP_PROFILE_ID_INFO,
    STAT_PP_DB_STAT_PP_ETPP_COUNTING_PROFILE_ID_INFO,
    STAT_PP_DB_STAT_PP_ETPP_METERING_PROFILE_ID_INFO,
    STAT_PP_DB_STAT_PP_ERPP_PROFILE_ID_INFO,
    STAT_PP_DB_INFO_NOF_ENTRIES
} sw_state_stat_pp_db_layout_e;


extern dnx_sw_state_diagnostic_info_t stat_pp_db_info[SOC_MAX_NUM_DEVICES][STAT_PP_DB_INFO_NOF_ENTRIES];

extern const char* stat_pp_db_layout_str[STAT_PP_DB_INFO_NOF_ENTRIES];
int stat_pp_db_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int stat_pp_db_stat_pp_irpp_profile_id_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int stat_pp_db_stat_pp_etpp_profile_id_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int stat_pp_db_stat_pp_etpp_counting_profile_id_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int stat_pp_db_stat_pp_etpp_metering_profile_id_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

int stat_pp_db_stat_pp_erpp_profile_id_dump(
    int unit, dnx_sw_state_dump_filters_t filters);

#endif /* DNX_SW_STATE_DIAGNOSTIC */

#endif /* __ALGO_STAT_PP_DIAGNOSTIC_H__ */
