/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
*/


#ifndef __UI_PURE_DEFI_GSA_API_INCLUDED__
/* { */
#define __UI_PURE_DEFI_GSA_API_INCLUDED__

#include <soc/dpp/SAND/Utils/sand_header.h>
/*
 * Note:
 * the following definitions must range between PARAM_GSA_API_START_RANGE_ID
 * and PARAM_GSA_API_END_RANGE_ID.
 * See ui_pure_defi.h
 */
typedef enum
{
  PARAM_GSA_VLAN_SET_SET_ID = PARAM_GSA_START_RANGE_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_NDX_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_INFO_ENABLE_FLOODING_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_INFO_TOPOLOGY_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_INFO_MEMBERSHIP_TAGGED_PORTS_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_INFO_MEMBERSHIP_UNTAGGED_PORTS_ID,
  PARAM_GSA_VLAN_SET_SET_VLAN_INFO_VALID_ID,
  PARAM_GSA_VLAN_SET_RANGE_SET_ID,
  PARAM_GSA_VLAN_SET_SET_UP_TO_VLAN_ID,
  PARAM_GSA_VLAN_GET_GET_ID,
  PARAM_GSA_VLAN_GET_GET_VLAN_NDX_ID,
  PARAM_GSA_VLAN_MEMBER_ADD_MEMBER_ADD_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_INFO_SET_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_INFO_SET_PORT_NDX_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_INFO_SET_VID_NDX_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_INFO_SET_TAGGED_ID,
  PARAM_GSA_EDIT_PORT_INFO_GET_PORT_INFO_GET_ID,
  PARAM_GSA_EDIT_PORT_INFO_GET_PORT_INFO_GET_PORT_NDX_ID,
  PARAM_GSA_EDIT_PORT_INFO_GET_PORT_INFO_GET_VID_NDX_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_PVID_SET_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_PVID_SET_PORT_NDX_ID,
  PARAM_GSA_EDIT_PORT_INFO_SET_PORT_PVID_SET_PVID_ID,
  PARAM_GSA_EDIT_PORT_INFO_GET_PORT_PVID_GET_ID,
  PARAM_GSA_EDIT_PORT_INFO_GET_PORT_PVID_GET_PORT_NDX_ID,
  PARAM_GSA_VLAN_MEMBER_ADD_MEMBER_ADD_VLAN_NDX_ID,
  PARAM_GSA_VLAN_MEMBER_ADD_MEMBER_ADD_SYS_PORT_ID,
  PARAM_GSA_VLAN_MEMBER_ADD_MEMBER_ADD_TAGGED_ID,
  PARAM_GSA_VLAN_MEMBER_REMOVE_MEMBER_REMOVE_ID,
  PARAM_GSA_VLAN_MEMBER_REMOVE_MEMBER_REMOVE_VLAN_NDX_ID,
  PARAM_GSA_VLAN_MEMBER_REMOVE_MEMBER_REMOVE_SYS_PORT_ID,
  PARAM_GSA_API_VLAN_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_KEY_VID_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_KEY_MAC_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_VALUE_SA_DROP_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_VALUE_IS_DYNAMIC_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_VALUE_DESTS_DESTS_PHY_PORTS_ID_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_VALUE_DESTS_DESTS_LAG_PORTS_ID_ID,
  PARAM_GSA_MACT_ENTRY_ADD_ENTRY_ADD_VALUE_FRWRD_ACTION_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_KEY_VID_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_KEY_MAC_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_UP_TO_MAC_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_VALUE_SA_DROP_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_VALUE_IS_DYNAMIC_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_VALUE_DESTS_DESTS_PORTS_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_VALUE_DESTS_DESTS_LAGS_ID,
  PARAM_GSA_MACT_RANGE_ADD_RANGE_ADD_VALUE_FRWRD_ACTION_ID,
  PARAM_GSA_MACT_ENTRY_REMOVE_ENTRY_REMOVE_ID,
  PARAM_GSA_MACT_ENTRY_REMOVE_ENTRY_REMOVE_KEY_VID_ID,
  PARAM_GSA_MACT_ENTRY_REMOVE_ENTRY_REMOVE_KEY_MAC_ID,
  PARAM_GSA_MACT_RANGE_REMOVE_RANGE_REMOVE_ID,
  PARAM_GSA_MACT_RANGE_REMOVE_RANGE_REMOVE_KEY_VID_ID,
  PARAM_GSA_MACT_RANGE_REMOVE_RANGE_REMOVE_KEY_MAC_ID,
  PARAM_GSA_MACT_RANGE_REMOVE_RANGE_REMOVE_UP_TO_MAC_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_TRANSPLANT_BLOCK_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_NEW_DEST_DEST_VAL_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_NEW_DEST_DEST_TYPE_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_INCLUDING_DYNAMIC_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_INCLUDING_STATIC_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_DEST_DEST_VAL_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_DEST_DEST_TYPE_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_COMPARE_DEST_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_FID_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_RULE_COMPARE_FID_ID,
  PARAM_GSA_MACT_FLUSH_BLOCK_FLUSH_BLOCK_FLUSH_INFO_MODE_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_INCLUDING_DYNAMIC_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_INCLUDING_STATIC_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_DEST_DEST_VAL_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_DEST_DEST_TYPE_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_COMPARE_DEST_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_FID_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_BLOCK_RULE_COMPARE_FID_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_FAP_ID_ID,
  PARAM_GSA_MACT_GET_BLOCK_GET_PRINT_MINIMAL_ID,
  PARAM_GSA_MACT_PRINT_ALL_PRINT_ALL_ID,
  PARAM_GSA_MACT_AGING_INFO_SET_AGING_INFO_SET_ID,
  PARAM_GSA_MACT_AGING_INFO_SET_AGING_INFO_SET_AGING_INFO_META_CYCLE_NANO_SEC_ID,
  PARAM_GSA_MACT_AGING_INFO_SET_AGING_INFO_SET_AGING_INFO_META_CYCLE_SEC_ID,
  PARAM_GSA_MACT_AGING_INFO_SET_AGING_INFO_SET_AGING_INFO_ENABLE_AGING_ID,
  PARAM_GSA_MACT_AGING_INFO_GET_AGING_INFO_GET_ID,
  PARAM_GSA_MACT_MY_MAC_ADD_MY_MAC_ADD_ID,
  PARAM_GSA_MACT_MY_MAC_ADD_MY_MAC_ADD_KEY_VID_ID,
  PARAM_GSA_MACT_MY_MAC_ADD_MY_MAC_ADD_KEY_MAC_ID,
  PARAM_GSA_MACT_MY_MAC_REMOVE_MY_MAC_REMOVE_ID,
  PARAM_GSA_MACT_MY_MAC_REMOVE_MY_MAC_REMOVE_KEY_VID_ID,
  PARAM_GSA_MACT_MY_MAC_REMOVE_MY_MAC_REMOVE_KEY_MAC_ID,
  PARAM_GSA_API_MACT_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_ADD_TYPE_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_INDEX_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_STATISTICS_COUNTER_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_ENABLE_RPF_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_CPU_CODE_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_OUT_VID_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_MAC_ADDR_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_DEST_SYS_PORT_ID_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_DEST_SYS_PORT_TYPE_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_ARRAY_ACTION_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_RANGE_INC_MAC_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_RANGE_INC_CPU_CODE_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_RANGE_UP_TO_SYS_FEC_ID,
  PARAM_GSA_FEC_UC_ECMP_ADD_UC_ECMP_ADD_UC_FEC_RANGE_INC_VID_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_ENTRY_NDX_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_STATISTICS_COUNTER_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_ENABLE_RPF_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_CPU_CODE_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_OUT_VID_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_MAC_ADDR_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_DEST_SYS_PORT_ID_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_DEST_SYS_PORT_TYPE_ID,
  PARAM_GSA_FEC_UC_ECMP_UPDATE_ENTRY_UC_ECMP_UPDATE_ENTRY_UC_FEC_ENTRY_ACTION_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_ADD_TYPE_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_STATISTICS_COUNTER_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_CPU_CODE_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_PORT_SYS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_PORT_SYS_PORT_TYPE_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_VID_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_RPF_CHECK_RPF_CHECK_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_PORTS_VIDS_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_LAGS_VIDS_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_DESTS_DESTS_PHY_PORTS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_DESTS_DESTS_LAGS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_RANGE_UP_TO_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_DESTS_DESTS_PORTS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_ADD_MC_FEC_ADD_MC_FEC_ENTRY_ACTION_ID,
  PARAM_GSA_FEC_UC_EMCP_REMOVE_ENTRY_UC_EMCP_REMOVE_ENTRY_ID,
  PARAM_GSA_FEC_UC_EMCP_REMOVE_ENTRY_UC_EMCP_REMOVE_ENTRY_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_EMCP_REMOVE_ENTRY_UC_EMCP_REMOVE_ENTRY_FEC_ENTRY_NDX_ID,
  PARAM_GSA_FEC_REMOVE_REMOVE_ID,
  PARAM_GSA_FEC_REMOVE_REMOVE_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_REMOVE_UP_TO_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_GET_UC_GET_ID,
  PARAM_GSA_FEC_UC_GET_UC_GET_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_MC_GET_MC_GET_ID,
  PARAM_GSA_FEC_MC_GET_MC_GET_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_TABLE_UC_GET_BLOCK_TABLE_UC_GET_BLOCK_ID,
  PARAM_GSA_FEC_TABLE_UC_GET_BLOCK_TABLE_UC_GET_BLOCK_START_FROM_ID,
  PARAM_GSA_FEC_TABLE_MC_GET_BLOCK_TABLE_MC_GET_BLOCK_ID,
  PARAM_GSA_FEC_TABLE_MC_GET_BLOCK_TABLE_MC_GET_BLOCK_START_FROM_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UP_TO_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_INDEX_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_STATISTICS_COUNTER_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_ENABLE_RPF_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_CPU_CODE_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_OUT_VID_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_MAC_ADDR_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_DEST_SYS_PORT_ID_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_DEST_SYS_PORT_TYPE_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_UC_FEC_ARRAY_ACTION_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_NOF_ENTRIES_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_RANGE_INFO_INC_CPU_CODE_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_RANGE_INFO_INC_VID_ID,
  PARAM_GSA_FEC_UC_ECMP_RANGE_ADD_UC_ECMP_RANGE_ADD_RANGE_INFO_INC_MAC_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_UP_TO_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_STATISTICS_COUNTER_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_CPU_CODE_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_PORT_SYS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_PORT_SYS_PORT_TYPE_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_RPF_CHECK_EXP_IN_VID_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_RPF_CHECK_RPF_CHECK_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_VIDS_INDEX_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_VIDS_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_DESTS_NOF_PORTS_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_DESTS_DESTS_PORTS_INDEX_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_DESTS_DESTS_PORTS_ID_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_DESTS_DESTS_PORTS_TYPE_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_MC_FEC_ENTRY_ACTION_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_RANGE_INFO_INC_CPU_CODE_ID,
  PARAM_GSA_FEC_MC_FEC_RANGE_ADD_MC_FEC_RANGE_ADD_RANGE_INFO_INC_DEST_ID,
  PARAM_GSA_FEC_RANGE_REMOVE_RANGE_REMOVE_ID,
  PARAM_GSA_FEC_RANGE_REMOVE_RANGE_REMOVE_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_RANGE_REMOVE_RANGE_REMOVE_UP_TO_SYS_FEC_NDX_ID,
  PARAM_GSA_FEC_TABLE_CLEAR_TABLE_CLEAR_ID,
  PARAM_GSA_FEC_TABLE_CLEAR_TABLE_CLEAR_REMOVE_DEFAULT_ID,
  PARAM_GSA_API_FEC_ID,
  PARAM_GSA_IPV4_UC_ROUTE_ADD_ROUTE_ADD_ID,
  PARAM_GSA_IPV4_UC_ROUTE_ADD_ROUTE_ADD_ROUTE_KEY_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_UC_ROUTE_ADD_ROUTE_ADD_ROUTE_KEY_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_UC_ROUTE_ADD_ROUTE_ADD_SYS_FEC_ID_ID,
  PARAM_GSA_IPV4_UC_ROUTE_ADD_ROUTE_ADD_OVERRIDE_ID,
  PARAM_GSA_IPV4_UC_ROUTE_GET_ROUTE_GET_ID,
  PARAM_GSA_IPV4_UC_ROUTE_GET_ROUTE_GET_ROUTE_KEY_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_UC_ROUTE_GET_ROUTE_GET_ROUTE_KEY_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_UC_ROUTE_GET_ROUTE_GET_EXACT_MATCH_ID,
  PARAM_GSA_IPV4_UC_ROUTE_LPM_GET_ROUTE_LPM_GET_ID,
  PARAM_GSA_IPV4_UC_ROUTE_LPM_GET_ROUTE_LPM_GET_ROUTE_KEY_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_UC_ROUTE_LPM_GET_ROUTE_LPM_GET_ROUTE_KEY_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_UC_GET_BLOCK_GET_BLOCK_ID,
  PARAM_GSA_IPV4_UC_GET_BLOCK_GET_BLOCK_SHOW_FEC_INFO_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_ROUTE_TABLE_SYS_FEC_ID_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_ROUTE_TABLE_KEY_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_ROUTE_TABLE_KEY_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_NOF_ENTRIES_ID,
  PARAM_GSA_IPV4_UC_PRINT_BLOCK_PRINT_BLOCK_FLAVORS_ID,
  PARAM_GSA_IPV4_UC_ROUTE_REMOVE_ROUTE_REMOVE_ID,
  PARAM_GSA_IPV4_UC_ROUTE_REMOVE_ROUTE_REMOVE_ROUTE_KEY_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_UC_ROUTE_REMOVE_ROUTE_REMOVE_ROUTE_KEY_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_UC_ROUTES_CLEAR_ROUTES_CLEAR_ID,
  PARAM_GSA_API_IPV4_UC_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_INIT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_INIT_USE_PORT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ENTRY_ID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_COUNT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_INC_SYS_FEC_NDX_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV4_MC_ROUTE_ADD_OVERRIDE_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_IPV4_MC_ROUTE_GET_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_LPM_GET_IPV4_MC_ROUTE_LPM_GET_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_BLOCK_IPV4_MC_ROUTE_GET_BLOCK_ID,
  PARAM_GSA_IPV4_MC_ROUTE_GET_BLOCK_IPV4_MC_ROUTE_GET_BLOCK_BLOCK_RANGE_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ALL_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV4_MC_ROUTE_REMOVE_IPV4_MC_ROUTE_REMOVE_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_INIT_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_INIT_USE_PORT_ID,
  PARAM_GSA_IPV4_MC_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV6_ROUTE_INIT_ID,
  PARAM_GSA_IPV4_MC_ROUTE_ADD_IPV6_ROUTE_INIT_USE_PORT_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ENTRY_ID_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_COUNT_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_SYS_FEC_NDX_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_INC_SYS_FEC_NDX_ID,
  PARAM_GSA_IPV6_ROUTE_ADD_IPV6_ROUTE_ADD_OVERRIDE_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_GET_IPV6_ROUTE_GET_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_LPM_GET_IPV6_ROUTE_LPM_GET_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_GET_BLOCK_IPV6_ROUTE_GET_BLOCK_ID,
  PARAM_GSA_IPV6_ROUTE_GET_BLOCK_IPV6_ROUTE_GET_BLOCK_BLOCK_RANGE_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_IN_VID_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_IN_VID_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_IN_PORT_VALID_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_IN_PORT_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_GROUP_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_GROUP_IP_ADDRESS_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_SOURCE_PREFIX_LEN_ID,
  PARAM_GSA_IPV6_ROUTE_REMOVE_IPV6_ROUTE_REMOVE_ROUTE_KEY_SOURCE_IP_ADDRESS_ID,
  PARAM_GSA_API_IPV6_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACL_NDX_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_VALID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MAX_DEST_PORT_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MIN_DEST_PORT_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MAX_SRC_PORT_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MIN_SRC_PORT_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_DEST_PORT_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_DEST_PORT_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_SRC_PORT_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_SRC_PORT_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_TCP_CTRL_OPS_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_TCP_CTRL_OPS_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_DEST_IP_PREFIX_LEN_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_DEST_IP_IP_ADDRESS_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_SRC_IP_PREFIX_LEN_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_SRC_IP_IP_ADDRESS_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_PROTOCOL_CODE_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_PROTOCOL_CODE_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_TOS_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_TOS_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_VID_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_VID_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_LOCAL_PORT_ID_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_LOCAL_PORT_ID_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_DEST_MAC_MASK_INDEX_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_DEST_MAC_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_DEST_MAC_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_SRC_MAC_MASK_INDEX_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_SRC_MAC_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_SRC_MAC_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_ETHER_TYPE_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_ETHER_TYPE_DSAP_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_ETHER_TYPE_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_DEI_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_DEI_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_PCP_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_PCP_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_VID_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_VID_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_TAG_EXIST_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_S_TAG_TAG_EXIST_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_DEI_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_DEI_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_PCP_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_PCP_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_VID_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_VID_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_TAG_EXIST_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_C_TAG_TAG_EXIST_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_IN_LOCAL_PORT_ID_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_IN_LOCAL_PORT_ID_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_IN_PORT_TYPE_VALID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_VAL_L2_INFO_IN_PORT_TYPE_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_TYPE_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_PRIORITY_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACE_ID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_ACTION_MASK_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_COS_ACTION_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L3_INFO_MAC_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L3_INFO_VID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L3_INFO_DESTINATION_DEST_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L3_SYS_FEC_ID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L3_INFO_DESTINATION_DEST_TYPE_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L2_INFO_MAC_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L2_INFO_VID_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L2_INFO_DESTINATION_DEST_VAL_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_L2_INFO_DESTINATION_DEST_TYPE_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_ACTION_INFO_UPDATE_TYPE_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_OVERRIDE_ID,
  PARAM_GSA_ACL_ACE_REMOVE_ACE_REMOVE_ID,
  PARAM_GSA_ACL_ACE_REMOVE_ACE_REMOVE_ACL_NDX_ID,
  PARAM_GSA_ACL_ACE_REMOVE_ACE_REMOVE_ACE_ID_NDX_ID,
  PARAM_GSA_ACL_ACE_GET_ACE_GET_ID,
  PARAM_GSA_ACL_ACE_GET_ACE_GET_ACL_NDX_ID,
  PARAM_GSA_ACL_ACE_GET_ACE_GET_ACE_ID_NDX_ID,
  PARAM_GSA_ACL_ACE_GET_BLOCK_ACE_GET_BLOCK_ID,
  PARAM_GSA_ACL_ACE_GET_BLOCK_ACE_GET_BLOCK_ACL_NDX_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACL_NDX_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_VALID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MAX_DEST_PORT_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MIN_DEST_PORT_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MAX_SRC_PORT_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_PORT_RANGE_MIN_SRC_PORT_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_DEST_PORT_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_DEST_PORT_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_SRC_PORT_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_SRC_PORT_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_TCP_CTRL_OPS_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L4_INFO_TCP_CTRL_OPS_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_DEST_IP_PREFIX_LEN_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_DEST_IP_IP_ADDRESS_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_SRC_IP_PREFIX_LEN_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_SRC_IP_IP_ADDRESS_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_PROTOCOL_CODE_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_PROTOCOL_CODE_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_TOS_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_TOS_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_VID_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_VID_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_LOCAL_PORT_ID_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L3_L4_INFO_L3_INFO_IN_LOCAL_PORT_ID_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_DEST_MAC_MASK_INDEX_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_DEST_MAC_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_DEST_MAC_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_SRC_MAC_MASK_INDEX_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_SRC_MAC_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_SRC_MAC_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_ETHER_TYPE_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_ETHER_TYPE_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_DEI_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_DEI_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_PCP_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_PCP_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_VID_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_VID_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_TAG_EXIST_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_S_TAG_TAG_EXIST_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_DEI_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_DEI_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_PCP_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_PCP_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_VID_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_VID_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_TAG_EXIST_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_C_TAG_TAG_EXIST_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_IN_LOCAL_PORT_ID_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_IN_LOCAL_PORT_ID_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_IN_PORT_TYPE_VALID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_VAL_L2_INFO_IN_PORT_TYPE_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_TYPE_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_PRIORITY_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACE_ID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_INC_VID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_INC_SYS_FEC_ID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_STEP_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_COUNT_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_TO_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_FIELD_TO_INC_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_RANGE_INFO_TYPE_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_ACTION_MASK_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_COS_ACTION_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L3_INFO_SYS_FEC_ID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L3_INFO_VID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L3_INFO_DESTINATION_DEST_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L3_INFO_DESTINATION_DEST_TYPE_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L2_INFO_MAC_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L2_INFO_VID_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L2_INFO_DESTINATION_DEST_VAL_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_L2_INFO_DESTINATION_DEST_TYPE_ID,
  PARAM_GSA_ACL_RANGE_ADD_RANGE_ADD_ACTION_UPDATE_TYPE_ID,
  PARAM_GSA_ACL_RANGE_REMOVE_RANGE_REMOVE_ID,
  PARAM_GSA_ACL_RANGE_REMOVE_RANGE_REMOVE_ACL_NDX_ID,
  PARAM_GSA_ACL_RANGE_REMOVE_RANGE_REMOVE_FROM_ACE_NDX_ID,
  PARAM_GSA_ACL_RANGE_REMOVE_RANGE_REMOVE_TO_ACE_NDX_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_L3A_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_L3B_ID,
  PARAM_GSA_ACL_ACE_ADD_ACE_ADD_L2_ID,
  PARAM_GSA_API_ACL_ID,
}
PARAM_GSA_API_IDS;

/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>
/* } __UI_PURE_DEFI_GSA_API_INCLUDED__*/
#endif
