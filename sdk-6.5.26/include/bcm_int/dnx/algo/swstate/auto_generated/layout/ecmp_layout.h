/** \file algo/swstate/auto_generated/layout/ecmp_layout.h
 *
 * sw state layout enum
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

#ifndef __ECMP_LAYOUT_H__
#define __ECMP_LAYOUT_H__

#include <include/soc/dnxc/swstate/dnxc_sw_state_layout.h>
#include <include/soc/dnxc/swstate/auto_generated/types/dnxc_module_ids_types.h>


int ecmp_init_layout_structure(int unit);

typedef enum
{
    DNX_SW_STATE_ECMP_DB_INFO = DNXC_SW_STATE_LAYOUT_NODE_ID_GET(ECMP_MODULE_ID, 0),
    DNX_SW_STATE_ECMP_DB_INFO__FIRST,
    DNX_SW_STATE_ECMP_DB_INFO__FIRST_SUB = DNX_SW_STATE_ECMP_DB_INFO__FIRST - 1,
    DNX_SW_STATE_ECMP_DB_INFO__MEMBERS_TBL_ADDR_PROFILE_LINK_LISTS,
    DNX_SW_STATE_ECMP_DB_INFO__CONSISTENT_HASHING_MANAGER_HANDLE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_RES_MANAGER,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_EXTENDED_RES_MANAGER,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_RES_MNGR,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_FEC_MEMBERS_TABLE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_GROUP_SIZE_MINUS_ONE,
    DNX_SW_STATE_ECMP_DB_INFO__LAST,
    DNX_SW_STATE_ECMP_DB_INFO__LAST_SUB = DNX_SW_STATE_ECMP_DB_INFO__LAST - 1,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__FIRST,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__FIRST_SUB = DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__FIRST - 1,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__ECMP_MODE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__CONSISTENT_MEMBERS_TABLE_ADDR,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__TUNNEL_PRIORITY_MODE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__RPF_MODE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__HIERARCHY,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__NOF_REFERENCES,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__CHM_ALLOC_STATE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__TUNNEL_PRIORITY_MAP_PROFILE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__GROUP_SIZE,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__LAST,
    DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__LAST_SUB = DNX_SW_STATE_ECMP_DB_INFO__ECMP_USER_PROFILE_DATA__LAST - 1,
    /*
     * Use DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX macro to get the number of parameters
     */
    DNX_SW_STATE_ECMP_DB_INFO_NOF_PARAMS,
} dnxc_sw_state_layout_ecmp_node_id_e;

#endif /* __ECMP_LAYOUT_H__ */