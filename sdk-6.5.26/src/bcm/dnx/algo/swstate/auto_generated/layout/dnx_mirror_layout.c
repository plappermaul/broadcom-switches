/** \file algo/swstate/auto_generated/layout/dnx_mirror_layout.c
 *
 * sw state layout structure
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

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <bcm_int/dnx/algo/swstate/auto_generated/types/dnx_mirror_types.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/layout/dnx_mirror_layout.h>

dnxc_sw_state_layout_t layout_array_dnx_mirror[DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB_NOF_PARAMS)];

shr_error_e
dnx_mirror_init_layout_structure(int unit)
{

    int dnx_mirror_db__channel_to_egress_if__default_val = DNX_ALGO_PORT_EGR_IF_INVALID;
    int dnx_mirror_db__mirror_on_drop_profile__default_val = DNX_MIRROR_ON_DROP_INVALID_PROFILE;


    int idx;

    DNXC_SW_STATE_INIT_FUNC_DEFS;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db";
    layout_array_dnx_mirror[idx].type = "dnx_mirror_db_t";
    layout_array_dnx_mirror[idx].doc = "DB for used by mirror";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_mirror_db_t);
    layout_array_dnx_mirror[idx].parent  = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__FIRST);
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__LAST);

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__CHANNEL_TO_EGRESS_IF);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__channel_to_egress_if";
    layout_array_dnx_mirror[idx].type = "int";
    layout_array_dnx_mirror[idx].doc = "Per core mapping from Mirror channel to Egress interface";
    layout_array_dnx_mirror[idx].default_value= &(dnx_mirror_db__channel_to_egress_if__default_val);
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(int);
    layout_array_dnx_mirror[idx].array_indexes[0].num_elements = DNX_DATA_MAX_DEVICE_GENERAL_NOF_CORES;
    layout_array_dnx_mirror[idx].array_indexes[0].index_type = DNXC_SWSTATE_ARRAY_INDEX_STATIC;
    layout_array_dnx_mirror[idx].array_indexes[1].num_elements = DNX_DATA_MAX_PORT_GENERAL_MAX_NOF_CHANNELS;
    layout_array_dnx_mirror[idx].array_indexes[1].index_type = DNXC_SWSTATE_ARRAY_INDEX_STATIC;
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__EGRESS_MIRROR_PRIORITY);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__egress_mirror_priority";
    layout_array_dnx_mirror[idx].type = "bcm_pbmp_t";
    layout_array_dnx_mirror[idx].doc = "What mirror priorities are mapped for each egress port";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(bcm_pbmp_t);
    layout_array_dnx_mirror[idx].array_indexes[0].num_elements = DNX_MIRROR_RCY_PRIORITY_NOF;
    layout_array_dnx_mirror[idx].array_indexes[0].index_type = DNXC_SWSTATE_ARRAY_INDEX_STATIC;
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__SNIF_INGRESS_PROFILES_MIRROR);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__snif_ingress_profiles_mirror";
    layout_array_dnx_mirror[idx].type = "dnx_algo_res_t";
    layout_array_dnx_mirror[idx].doc = "SNIF_INGRESS_PROFILES_MIRROR";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_algo_res_t);
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__SNIF_INGRESS_PROFILES_SNOOP);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__snif_ingress_profiles_snoop";
    layout_array_dnx_mirror[idx].type = "dnx_algo_res_t";
    layout_array_dnx_mirror[idx].doc = "SNIF_INGRESS_PROFILES_SNOOP";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_algo_res_t);
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__SNIF_INGRESS_PROFILES_STAT_SAMPLING);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__snif_ingress_profiles_stat_sampling";
    layout_array_dnx_mirror[idx].type = "dnx_algo_res_t";
    layout_array_dnx_mirror[idx].doc = "SNIF_INGRESS_PROFILES_STAT_SAMPLING";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_algo_res_t);
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__SNIF_EGRESS_VLAN_PROFILES_MIRROR);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__snif_egress_vlan_profiles_mirror";
    layout_array_dnx_mirror[idx].type = "dnx_algo_template_t*";
    layout_array_dnx_mirror[idx].doc = "SNIF_EGRESS_VLAN_PROFILES_MIRROR";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_algo_template_t*);
    layout_array_dnx_mirror[idx].array_indexes[0].num_elements = dnx_data_device.general.nof_cores_get(unit);
    layout_array_dnx_mirror[idx].array_indexes[0].index_type = DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_DNX_DATA;
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__SNIF_INGRESS_VLAN_PROFILES_MIRROR);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__snif_ingress_vlan_profiles_mirror";
    layout_array_dnx_mirror[idx].type = "dnx_algo_template_t*";
    layout_array_dnx_mirror[idx].doc = "SNIF_INGRESS_VLAN_PROFILES_MIRROR";
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(dnx_algo_template_t*);
    layout_array_dnx_mirror[idx].array_indexes[0].num_elements = dnx_data_device.general.nof_cores_get(unit);
    layout_array_dnx_mirror[idx].array_indexes[0].index_type = DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_DNX_DATA;
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;

    idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(DNX_SW_STATE_DNX_MIRROR_DB__MIRROR_ON_DROP_PROFILE);
    layout_array_dnx_mirror[idx].name = "dnx_mirror_db__mirror_on_drop_profile";
    layout_array_dnx_mirror[idx].type = "int*";
    layout_array_dnx_mirror[idx].doc = "mirror on drop sniff profiles";
    layout_array_dnx_mirror[idx].default_value= &(dnx_mirror_db__mirror_on_drop_profile__default_val);
    layout_array_dnx_mirror[idx].total_size = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].size_of = sizeof(int*);
    layout_array_dnx_mirror[idx].array_indexes[0].num_elements = BCM_MIRROR_ADMIT_PROFILES_NOF;
    layout_array_dnx_mirror[idx].array_indexes[0].index_type = DNXC_SWSTATE_ARRAY_INDEX_STATIC;
    layout_array_dnx_mirror[idx].array_indexes[1].num_elements = dnx_data_snif.ingress.nof_mirror_on_drop_profiles_get(unit);
    layout_array_dnx_mirror[idx].array_indexes[1].index_type = DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_DNX_DATA;
    layout_array_dnx_mirror[idx].parent  = 0;
    layout_array_dnx_mirror[idx].first_child_index = DNXC_SW_STATE_LAYOUT_INVALID;
    layout_array_dnx_mirror[idx].last_child_index = DNXC_SW_STATE_LAYOUT_INVALID;


    dnxc_sw_state_layout_init_structure(unit, DNX_SW_STATE_DNX_MIRROR_DB, layout_array_dnx_mirror, sw_state_layout_array[unit][DNX_MIRROR_MODULE_ID], DNX_SW_STATE_DNX_MIRROR_DB_NOF_PARAMS);

    DNXC_SW_STATE_FUNC_RETURN;
}

#undef BSL_LOG_MODULE