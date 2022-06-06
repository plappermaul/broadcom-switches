#include <soc/mcm/memregs.h>
#if defined(BCM_88650_A0)
/* $Id: arad_pp_frwrd_ipv6.c,v 1.33 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FORWARD

/*************
 * INCLUDES  *
 *************/
/* { */
#include <shared/bsl.h>
#include <soc/dcmn/error.h>
#include <soc/dpp/SAND/Utils/sand_header.h>
#include <shared/swstate/access/sw_state_access.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>
#include <soc/dpp/SAND/Utils/sand_os_interface.h>
#include <soc/dpp/SAND/Utils/sand_multi_set.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_framework.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_frwrd_ipv6.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_frwrd_ip_tcam.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_general.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_sw_db.h>
#include <soc/dpp/PPD/ppd_api_trap_mgmt.h>

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
#include <soc/dpp/ARAD/arad_kbp.h>
#include <soc/dpp/drv.h>
#include <soc/dpp/JER/JER_PP/jer_pp_kaps.h>
#include <soc/dpp/JER/JER_PP/jer_pp_kaps_entry_mgmt.h>
#endif
/* } */
/*************
 * DEFINES   *
 *************/
/* { */

#define ARAD_PP_FRWRD_IPV6_TYPE_MAX                              (SOC_PPC_NOF_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_TYPES-1)

/* } */
/*************
 * local     *
 * function  *
 *************/
/* { */
uint32 arad_pp_frwrd_ipv6_uc_or_vpn_get_payload(
    SOC_SAND_IN  int                           unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO  *route_info,
    SOC_SAND_OUT uint32                       *payload_data);

uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_decode_payload(
    SOC_SAND_IN  int                           unit,
    SOC_SAND_IN  ARAD_TCAM_ACTION              *action,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO   *route_info);
/* } */
/*************
 * MACROS    *
 *************/
/* { */

/* In case of a not default (prefix_lengteh > 0) private entry (VRF > 0) the highest priority will be given (0-127).
*  In case of a not default public entry (VRF = 0) the next priorities will be given (128-255).
*  A default private entry will get the priority of 256 and a public default a 257. (0 is the highest priority)
*/
#define ARAD_PP_FRWRD_IPV6_UC_TCAM_GET_PRIORITY(prefix_lengteh) (SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - prefix_lengteh)

/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */

/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

CONST STATIC SOC_PROCEDURE_DESC_ELEMENT
  Arad_pp_procedure_desc_element_frwrd_ipv6[] =
{
  /*
   * Auto generated. Do not edit following section {
   */
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR_PRINT),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR_VERIFY),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_GET_PROCS_PTR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IPV6_GET_ERRS_PTR),
  /*
   * } Auto generated. Do not edit previous section.
   */

  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IP_TCAM_ROUTE_ADD_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IP_TCAM_ROUTE_GET_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IP_TCAM_ROUTE_GET_BLOCK_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IP_TCAM_ROUTE_REMOVE_UNSAFE),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_FRWRD_IP_TCAM_ROUTING_TABLE_CLEAR_UNSAFE),

  /*
   * Last element. Do no touch.
   */
  SOC_PROCEDURE_DESC_ELEMENT_DEF_LAST
};

CONST STATIC SOC_ERROR_DESC_ELEMENT
  Arad_pp_error_desc_element_frwrd_ipv6[] =
{
  /*
   * Auto generated. Do not edit following section {
   */
  {
    ARAD_PP_FRWRD_IPV6_SUCCESS_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_SUCCESS_OUT_OF_RANGE_ERR",
    "The parameter 'success' is out of range. \n\r "
    "The range is: 0 - SOC_SAND_NOF_SUCCESS_FAILURES-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_ROUTE_STATUS_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_ROUTE_STATUS_OUT_OF_RANGE_ERR",
    "The parameter 'route_status' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_STATUSS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_LOCATION_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_LOCATION_OUT_OF_RANGE_ERR",
    "The parameter 'location' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_LOCATIONS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_FOUND_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_FOUND_OUT_OF_RANGE_ERR",
    "The parameter 'found' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_LOCATIONS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_ROUTES_STATUS_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_ROUTES_STATUS_OUT_OF_RANGE_ERR",
    "The parameter 'routes_status' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_STATUSS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_ROUTES_LOCATION_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_ROUTES_LOCATION_OUT_OF_RANGE_ERR",
    "The parameter 'routes_location' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_LOCATIONS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_NOF_ENTRIES_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_NOF_ENTRIES_OUT_OF_RANGE_ERR",
    "The parameter 'nof_entries' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IP_ROUTE_LOCATIONS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_EXACT_MATCH_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_EXACT_MATCH_OUT_OF_RANGE_ERR",
    "The parameter 'exact_match' is out of range. \n\r "
    "The range is: 1 - 255.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_TYPE_OUT_OF_RANGE_ERR,
    "ARAD_PP_FRWRD_IPV6_TYPE_OUT_OF_RANGE_ERR",
    "The parameter 'type' is out of range. \n\r "
    "The range is: 0 - SOC_PPC_NOF_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_TYPES-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  /*
   * } Auto generated. Do not edit previous section.
   */

  {
    ARAD_PP_IPV6_DEFAULT_ACTION_TYPE_NOT_SUPPORTED_ERR  ,
    "ARAD_PP_IPV6_DEFAULT_ACTION_TYPE_NOT_SUPPORTED_ERR  ",
    "IPv6 default action type is not supported \n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_IPV6_DEFAULT_ACTION_WRONG_TRAP_CODE_ERR  ,
    "ARAD_PP_IPV6_DEFAULT_ACTION_WRONG_TRAP_CODE_ERR  ",
    "Trap code of IPv6 default action is wrong. \n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IPV6_MC_ILLEGAL_DEST_TYPE_ERR,
    "ARAD_PP_FRWRD_IPV6_MC_ILLEGAL_DEST_TYPE_ERR",
    "Destination in IPv6 MC routing info can be \n\r"
    "FEC-ptr or MC-group only\n\r",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_FRWRD_IP_TCAM_ENTRY_DOESNT_EXIST_ERR,
    "ARAD_PP_FRWRD_IP_TCAM_ENTRY_DOESNT_EXIST_ERR",
    "No entry matching the routing key was found.\n\r",
    SOC_SAND_SVR_ERR,
    FALSE
  },

  /*
   * Last element. Do no touch.
   */
SOC_ERR_DESC_ELEMENT_DEF_LAST
};

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */

uint32
  arad_pp_frwrd_ipv6_init_unsafe(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_TODO_IMPLEMENT_WARNING;
  ARAD_PP_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_init_unsafe()", 0, 0);
}

void
  arad_pp_frwrd_ipv6_prefix_to_mask(
    SOC_SAND_IN  uint32 prefix,
    SOC_SAND_OUT uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S]
  )
{
    int32
        counter = prefix,
        word = SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S - 1,
        i;

    for (i=0; i<SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S; i++) {
        mask[i] = 0;
    }

    while (counter >= SOC_SAND_NOF_BITS_IN_UINT32)
    {
        mask[word--] = SOC_SAND_U32_MAX;
        counter -= SOC_SAND_NOF_BITS_IN_UINT32;
    }
    if (counter > 0)
    {
        mask[word] = SOC_SAND_BITS_MASK(SOC_SAND_REG_MAX_BIT, SOC_SAND_NOF_BITS_IN_UINT32 - counter);
    }
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_add(
      SOC_SAND_IN  int                           unit,
      SOC_SAND_IN  uint32                        vrf_ndx,
      SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET      *route_key,
      SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO  *route_info,
      SOC_SAND_IN  uint8                         route_scale,
      SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE     *success
    )
{
#if defined(BCM_88675_A0) && defined(INCLUDE_KBP) && !defined(BCM_88030)
    uint32 payload;
#endif
    uint32 priority, res = SOC_SAND_OK;
    uint8 use_tcam = soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "l3_ipv6_uc_use_tcam", 0);
    uint32  payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S] = {0};
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_PPC_FP_QUAL_VAL qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, mask);    

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, mask);

    if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
        use_tcam = 1;
    }

    if (use_tcam) {
        priority = ARAD_PP_FRWRD_IPV6_UC_TCAM_GET_PRIORITY(route_key->prefix_len);

        res = arad_pp_frwrd_ipv6_uc_or_vpn_get_payload(unit, route_info, payload_data);
        SOC_SAND_CHECK_FUNC_RESULT(res, 5, exit);
        /*
         * In this case the payload data (payload_data) that will be used will not exceed  64bits so
         * this isn't a coverity overrun-buffer-val issue.
         */
        /* coverity[overrun-buffer-val:FALSE] */
        res = arad_pp_dbal_entry_add(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE, qual_vals, priority,  payload_data, success);
    }
#if defined(BCM_88675_A0) && defined(INCLUDE_KBP) && !defined(BCM_88030)
    else if(JER_KAPS_ENABLE(unit)) {
        SOC_DPP_DBAL_SW_TABLE_IDS table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE;

        if (route_scale && ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long))) {
            if (route_key->prefix_len >= (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long)) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_LONG_KAPS;
            } else if (route_key->prefix_len <= (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short)) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_SHORT_KAPS;
            } else {
                LOG_CLI((BSL_META("When using FIB scale, routes must have a prefix length greater than %d or smaller than %d, added prefix_length: %d.\n"),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short),
                         route_key->prefix_len));
                SOC_SAND_CHECK_FUNC_RESULT(1, 15, exit);
            }
        }

        if (route_info->flags & _BCM_L3_FLAGS2_RAW_ENTRY) {
            payload = route_info->dest_id;
        } else {
            payload = JER_PP_KAPS_FEC_ENCODE(route_info->dest_id); /* FEC encoding in PP destination */
        }
        
        if (route_key->prefix_len == 0 && !(route_info->flags & _BCM_L3_FLAGS2_RAW_ENTRY)) {
            payload = JER_PP_KAPS_DEFAULT_ENCODE(payload);
        }

        res = jer_pp_kaps_db_enabled(unit, vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        res = arad_pp_dbal_entry_add(unit, table_id, qual_vals, 0/*priority*/,  &payload, success);
    }
#endif
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv4_uc_rpf_kaps_route_add()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_remove(
      SOC_SAND_IN  int                        unit,
      SOC_SAND_IN  uint32                     vrf_ndx,
      SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET   *route_key,
      SOC_SAND_IN  uint8                      route_scale
    )
{
    uint32 res = SOC_SAND_OK;
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    uint8 use_tcam = soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "l3_ipv6_uc_use_tcam", 0);
    SOC_PPC_FP_QUAL_VAL          qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    SOC_SAND_SUCCESS_FAILURE     success;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, mask);

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, mask);

    if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
        use_tcam = 1;
    }

    if (use_tcam) {
        res = arad_pp_dbal_entry_delete(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE, qual_vals, &success);
    }
#if defined(BCM_88675_A0) && defined(INCLUDE_KBP) && !defined(BCM_88030)
    else if(JER_KAPS_ENABLE(unit)) {
        SOC_DPP_DBAL_SW_TABLE_IDS table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE;

        if (route_scale && ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long))) {
            if (route_key->prefix_len >= ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long))) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_LONG_KAPS;
           } else if (route_key->prefix_len <= ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short))) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_SHORT_KAPS;
            } else {
                LOG_CLI((BSL_META("When using FIB scale, routes must have a prefix length greater than %d or smaller than %d, added prefix_length: %d.\n"),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short),
                         route_key->prefix_len));
               SOC_SAND_CHECK_FUNC_RESULT(1, 15, exit);
            }
        }

        res = jer_pp_kaps_db_enabled(unit, vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        res = arad_pp_dbal_entry_delete(unit, table_id, qual_vals, &success);
    }
#endif
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv4_uc_rpf_kaps_route_add()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_get(
      SOC_SAND_IN  int                           unit,
      SOC_SAND_IN  uint32                        vrf_ndx,
      SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET      *route_key,
      SOC_SAND_IN  uint8                         route_scale,
      SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO  *route_info,
      SOC_SAND_OUT uint8                        *found,
      uint8                                     *hit_bit
    )
{
#if defined(BCM_88675_A0) && defined(INCLUDE_KBP) && !defined(BCM_88030)
    uint32 payload;
#endif
    uint32 priority, res = SOC_SAND_OK;
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    uint8 use_tcam = soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "l3_ipv6_uc_use_tcam", 0);
    SOC_PPC_FP_QUAL_VAL qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    ARAD_TCAM_ACTION    action;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    *hit_bit = 0;
    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, mask);

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, mask);


    if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
        use_tcam = 1;
    }

    if (use_tcam) {
        /*
        * As always the shorter mask/ longer prefix (more valid bits) entries will have a stronger priority (0 is the strongest).
        * In case of a public entry (VRF=0) the search in the TCAM should be performed after the private (VRF>0) search had failed
        * without VRF value consideration hence the VRF is masked and the priority lowered in case of a public entry.
        * The default route (full IP mask) will have the lowest priority of all (private default is stronger than the public one)
        */
        priority = ARAD_PP_FRWRD_IPV6_UC_TCAM_GET_PRIORITY(route_key->prefix_len);

        res = arad_pp_dbal_entry_get(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE, qual_vals, action.value, &priority, hit_bit, found);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        res = arad_pp_frwrd_ipv6_uc_or_vpn_decode_payload(unit, &action, route_info);
    }
#if defined(BCM_88675_A0) && defined(INCLUDE_KBP) && !defined(BCM_88030)
    else if(JER_KAPS_ENABLE(unit)) {
        SOC_DPP_DBAL_SW_TABLE_IDS table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE;

        if (route_scale && ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long))) {
            if (route_key->prefix_len >= ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long))) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_LONG_KAPS;
           } else if (route_key->prefix_len <= ((SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short))) {
                table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE_SCALE_SHORT_KAPS;
            } else {
                LOG_CLI((BSL_META("When using FIB scale, routes must have a prefix length greater than %d or smaller than %d, added prefix_length: %d.\n"),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_long),
                         (SOC_DPP_CONFIG(unit)->pp.enhanced_fib_scale_prefix_length_ipv6_short),
                         route_key->prefix_len));
               SOC_SAND_CHECK_FUNC_RESULT(1, 15, exit);
            }
        }

        res = jer_pp_kaps_db_enabled(unit, vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        res = arad_pp_dbal_entry_get(unit, table_id, qual_vals, &payload, 0/*priority*/, NULL/*hit_bit*/, found);

        if (*found) {
               route_info->dest_id = JER_PP_KAPS_DEFAULT_DECODE(JER_PP_KAPS_FEC_DECODE(payload));
        }
    }
#endif
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);



exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv4_uc_rpf_kaps_route_get()",0,0);

}

#if defined(INCLUDE_KBP) && !defined(BCM_88030)

STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_encode(
      SOC_SAND_IN  int                   unit,
      SOC_SAND_IN  ARAD_KBP_FRWRD_IP_TBL_ID   frwrd_table_id,
      SOC_SAND_IN  uint32                   vrf_ndx,
      SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET  *route_key,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY       *data,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY       *mask
    )
{
    uint32
        res,
        mask_full = SOC_SAND_U32_MAX,
        mask_void = 0;
    SOC_SAND_PP_IPV6_ADDRESS
        route_key_mask;
    

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    ARAD_PP_LEM_ACCESS_KEY_clear(data);
    ARAD_PP_LEM_ACCESS_KEY_clear(mask);
    sal_memset(&route_key_mask, 0x0, sizeof(SOC_SAND_PP_IPV6_ADDRESS));


    /* Build the logical IPv6 address Mask */
    SHR_BITSET_RANGE(route_key_mask.address, 
                     SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS-route_key->prefix_len, 
                     route_key->prefix_len);
    
    /* Encode Key into buffer according to the following format:
     *  
     * {VRF(11:0) in 139:128, IPv6 address(127:0) in 127:0}  
     */

    /* Param 0  - IPv6 address */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 0, 0 /* Copy the bits after */, data);
    SHR_BITCOPY_RANGE(data->param[0].value, 0 /* LSB */, route_key->ipv6_address.address, 0 /* LSB */, data->param[0].nof_bits);

    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 0, 0 /* Copy the bits after */, mask);
    SHR_BITCOPY_RANGE(mask->param[0].value, 0 /* LSB */, route_key_mask.address, 0 /* LSB */, mask->param[0].nof_bits);

    /* Param 1  - VRF */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 1, vrf_ndx, data);
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 1, ((vrf_ndx == 0)? mask_void: mask_full), mask);

    ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_encode()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_decode(
      SOC_SAND_IN  int                   unit,
      SOC_SAND_IN  ARAD_KBP_FRWRD_IP_TBL_ID   frwrd_table_id,
      SOC_SAND_IN ARAD_PP_LEM_ACCESS_KEY       *data,
      SOC_SAND_IN ARAD_PP_LEM_ACCESS_KEY       *mask,
      SOC_SAND_OUT  uint32                   *vrf_ndx,
      SOC_SAND_OUT  SOC_SAND_PP_IPV6_SUBNET  *route_key
    )
{
    uint32
        bit_ndx, d_word_ndx;
	uint8 found_lpm;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    SOC_SAND_CHECK_NULL_INPUT(data);
    SOC_SAND_CHECK_NULL_INPUT(mask);
    SOC_SAND_CHECK_NULL_INPUT(vrf_ndx);
    SOC_SAND_CHECK_NULL_INPUT(route_key);

    soc_sand_SAND_PP_IPV6_SUBNET_clear(route_key);
    *vrf_ndx = 0;

    /* Encode Key into buffer according to the following format:
     *  
     * {VRF(11:0) in 139:128, IPv6 address(127:0) in 127:0}  
     */

    /* Param 0  - IPv6 address */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 0, route_key->ipv6_address.address, data, mask);

    /* Find the prefix length according to the first bit set in the mask LSBs */
    route_key->prefix_len = 0;
	found_lpm = 0;
	for (d_word_ndx = 0; d_word_ndx < 4; d_word_ndx++) {
		for (bit_ndx = 0; bit_ndx < 32; bit_ndx++) {
			if (!SHR_BITGET(&mask->param[0].value[3-d_word_ndx], bit_ndx)) {
				route_key->prefix_len = d_word_ndx*32 + 31 - bit_ndx;
				found_lpm = 1;
			}
		}
		if (found_lpm) {
			break;
		}
	}

    /* Param 1  - VRF */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 1, vrf_ndx, data, mask);

    ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_decode()",0,0);
}


STATIC
  int
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode(
      SOC_SAND_IN  int                       unit,
      SOC_SAND_IN  uint32                       frwrd_table_id,
      SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO  *route_info,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_PAYLOAD    *payload
    )
{
    uint32
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* Encode Result */
    ARAD_PP_LEM_ACCESS_PAYLOAD_clear(payload);
    res = arad_pp_fwd_decision_in_buffer_build(
            unit,
            ARAD_PP_FRWRD_DECISION_APPLICATION_TYPE_DFLT,
            route_info,
            &payload->dest,
            &payload->asd
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 5, exit);
    if (frwrd_table_id == ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0) 
    {
        if(route_info->additional_info.outlif.type != SOC_PPC_OUTLIF_ENCODE_TYPE_NONE) {
            payload->flags = ARAD_PP_FWD_DECISION_PARSE_OUTLIF;
        }
        else if(route_info->additional_info.eei.type != SOC_PPC_EEI_TYPE_EMPTY) {
            payload->flags = ARAD_PP_FWD_DECISION_PARSE_EEI;
        }
        else{
            payload->flags = ARAD_PP_FWD_DECISION_PARSE_DEST;
        }
    }
    
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode()",0,0);
}

#endif

STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kaps_dbal_route_add(
      SOC_SAND_IN  int                               unit,
      SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY      *route_key,
      SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO     *route_info,
      SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE             *success
    )
{
    uint32 res, priority = 0;
    int use_tcam = 0;
    SOC_PPC_FP_QUAL_VAL qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    uint32  payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S] = {0};

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    if(SOC_IS_ARADPLUS_AND_BELOW(unit) || (SOC_DPP_CONFIG(unit)->pp.l3_mc_use_tcam != ARAD_PP_FLP_L3_MC_USE_TCAM_DISABLE)){
        use_tcam = 1;
    }

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, mask);

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->group.ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->group.ipv6_address.address, mask);
    /* Only use full or no mask for inrif, consistent with ipv4 inrif_valid */
    DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[3], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);

    if (use_tcam) {
        priority = SOC_SAND_U32_MAX - (uint32)soc_sand_bitstream_get_nof_on_bits(&route_key->inrif.mask, 1) - route_key->group.prefix_len;

        /*in case of raw data payload no encoding is needed*/
        if (route_info->flags & _BCM_IPMC_FLAGS_RAW_ENTRY) {
            payload_data[0] = route_info->dest_id.dest_val;
        } else {
            res = arad_pp_frwrd_ipv4_sand_dest_to_em_dest(unit, &(route_info->dest_id), &(payload_data[0])); 
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
        }

        /* coverity explanation: coverity has found that the entry is added to LEM physical db.
           However, if SW DB is SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, the chosen physical db
           is TCAM and its entry is 8 bytes */
        /* coverity[overrun-buffer-val:FALSE] */
        res = arad_pp_dbal_entry_add(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, qual_vals, priority,  payload_data, success);
    } else {
#if defined(INCLUDE_KBP) && !defined(BCM_88030)
        uint32 payload = 0;


        if (route_info->dest_id.dest_type == SOC_SAND_PP_DEST_MULTICAST) {
            payload = JER_PP_KAPS_MC_ENCODE(route_info->dest_id.dest_val); /* MC encoding in PP destination */
        }/* in case of raw data payload no encoding is needed */
        else if ((route_info->flags & _BCM_IPMC_FLAGS_RAW_ENTRY) && (route_info->dest_id.dest_type == SOC_SAND_PP_DEST_FEC)) {
            payload = route_info->dest_id.dest_val;
        } else if (route_info->dest_id.dest_type == SOC_SAND_PP_DEST_FEC) {
            payload = JER_PP_KAPS_FEC_ENCODE(route_info->dest_id.dest_val); /* FEC encoding in PP destination */
        }


        res = jer_pp_kaps_db_enabled(unit, route_key->vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        res = arad_pp_dbal_entry_add(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, qual_vals, priority,  &payload, success); 
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
#else
        SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_kaps_dbal_route_add()", 0, 0);
#endif        
    }
          
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kaps_dbal_route_add()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kaps_dbal_route_remove(
       SOC_SAND_IN  int                               unit,
       SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY      *route_key
    )
{
    uint32 res;

    SOC_PPC_FP_QUAL_VAL qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    SOC_SAND_SUCCESS_FAILURE     success;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, mask);

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->group.ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->group.ipv6_address.address, mask);
    /* Only use full or no mask for inrif, consistent with ipv4 inrif_valid */
    DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[3], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)        
    if(JER_KAPS_ENABLE(unit)){
        res = jer_pp_kaps_db_enabled(unit, route_key->vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }
#endif

    res = arad_pp_dbal_entry_delete(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, qual_vals, &success);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kaps_dbal_route_remove()",0,0);

}


STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kaps_dbal_route_get(
      SOC_SAND_IN  int                               unit,
      SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY      *route_key,
      SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO     *route_info,
      SOC_SAND_OUT uint8                                *found,
      SOC_SAND_OUT uint8                                *hit_bit
    )
{
    uint32 res, priority;
    uint32 mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    ARAD_TCAM_ACTION    action;

    SOC_PPC_FP_QUAL_VAL qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, mask);

    DBAL_QUAL_VALS_CLEAR(qual_vals);
    DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->group.ipv6_address.address, mask);
    DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->group.ipv6_address.address, mask);
    /* Only use full or no mask for inrif, consistent with ipv4 inrif_valid */
    DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[3], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
    if (JER_KAPS_ENABLE(unit)) {
        res = jer_pp_kaps_db_enabled(unit, route_key->vrf_ndx);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }
#endif

    res = arad_pp_dbal_entry_get(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, qual_vals, action.value, &priority, hit_bit, found);
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    if (*found) {

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
        if (JER_KAPS_ENABLE(unit)) {
            if (route_info->flags & _BCM_IPMC_FLAGS_RAW_ENTRY) {
                route_info->dest_id.dest_val= action.value[0];
            } else {
                res = jer_pp_kaps_payload_to_mc_dest_id(unit, action.value[0], &route_info->dest_id); 
                SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
            } 
        }else
#endif
        {
            if (route_info->flags &_BCM_IPMC_FLAGS_RAW_ENTRY) {
                route_info->dest_id.dest_val = action.value[0];
            } else {
                res = arad_pp_frwrd_ipv4_em_dest_to_sand_dest(unit, action.value[0], &(route_info->dest_id));
                SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
            }
        }
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kaps_dbal_route_get()",0,0);
}


#if defined(INCLUDE_KBP) && !defined(BCM_88030)
STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_add(
        SOC_SAND_IN  int                          unit,
        SOC_SAND_IN  uint32                       vrf_ndx,
        SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET      *route_key,
        SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO  *route_info,
        SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE     *success
    )
{
    uint32
        priority,
        table_ndx,
        nof_tables = 2,
        res = SOC_SAND_OK;
    ARAD_TCAM_ACTION
        action;
    ARAD_PP_LEM_ACCESS_KEY
        data,
        mask;
    ARAD_PP_LEM_ACCESS_PAYLOAD
        payload;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id,
        frwrd_table_id_base;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32
        ipv6_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);


    /* The priority is derived from the prefix length, longest prefix first */
    priority = SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - route_key->prefix_len;

    arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, ipv6_mask);

    if(!ARAD_KBP_ENABLE_IPV6_EXTENDED && SOC_IS_JERICHO(unit)) {

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, ipv6_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, ipv6_mask);

        frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0;
        table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_FWD_KBP;

        res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode(
                unit, 
                frwrd_table_id,
                route_info, 
                &payload);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

        res = arad_pp_dbal_entry_add(
                unit,
                table_id,
                qual_vals,
                priority,
                &payload,
                success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }
    else {

#ifdef BCM_88660_A0
        if(SOC_IS_ARADPLUS(unit) && (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "ext_rpf_fwd_parallel", 0) == 0)) {
            nof_tables = 1;
        }
#endif /* BCM_88660 */
        
        if(soc_property_get(unit, spn_EXT_IP6_FWD_TABLE_SIZE, 0x0) && (soc_property_get(unit, spn_EXT_IP6_UC_RPF_FWD_TABLE_SIZE, 0x0) == 0)) {
            nof_tables = 1;
        }

        if(ARAD_KBP_ENABLE_IPV6_EXTENDED) {
            frwrd_table_id_base = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_1;
        }
        else {
            frwrd_table_id_base = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0;
        }    

        for(table_ndx = 0; table_ndx < nof_tables; table_ndx++)
        {
            frwrd_table_id = frwrd_table_id_base + table_ndx;
            
            soc_sand_os_memset(&action, 0x0, sizeof(action));

            /* Encode Key */
            res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_encode(
                    unit,
                    frwrd_table_id,
                    vrf_ndx, 
                    route_key, 
                    &data,
                    &mask);
            SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

            /* Encode Result */
            res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode(
                    unit, 
                    frwrd_table_id,
                    route_info, 
                    &payload);
            SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 

            /* Add record */
            res = arad_pp_tcam_route_kbp_add_unsafe(
                    unit,
                    frwrd_table_id,
                    &data,
                    &mask,
                    priority,
                    &payload,
                    success);
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit); 
        }
    }
    
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_add()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_remove(
        SOC_SAND_IN  int                       unit,
        SOC_SAND_IN  uint32                    vrf_ndx,
        SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET   *route_key
    )
{
    uint32
        table_ndx,
        nof_tables = 2,
        res = SOC_SAND_OK;
    ARAD_PP_LEM_ACCESS_KEY       
        data,
        mask;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id,
        frwrd_table_id_base;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    SOC_SAND_SUCCESS_FAILURE
        success;
    uint32
        ipv6_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);


    if(!ARAD_KBP_ENABLE_IPV6_EXTENDED && SOC_IS_JERICHO(unit)) {

        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, ipv6_mask);

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, ipv6_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, ipv6_mask);

        table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_FWD_KBP;

        res = arad_pp_dbal_entry_delete(
                unit, 
                table_id,
                qual_vals,
                &success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }
    else {
    
#ifdef BCM_88660_A0
        if(SOC_IS_ARADPLUS(unit) && (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "ext_rpf_fwd_parallel", 0) == 0)) {
            nof_tables = 1;
        }
#endif /* BCM_88660 */
        
        if(soc_property_get(unit, spn_EXT_IP6_FWD_TABLE_SIZE, 0x0) && (soc_property_get(unit, spn_EXT_IP6_UC_RPF_FWD_TABLE_SIZE, 0x0) == 0)) {
            nof_tables = 1;
        }

        if(ARAD_KBP_ENABLE_IPV6_EXTENDED) {
            frwrd_table_id_base = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_1;
        }
        else {
            frwrd_table_id_base = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0;
        }    

        for(table_ndx = 0; table_ndx < nof_tables; table_ndx++)
        {
            frwrd_table_id = frwrd_table_id_base + table_ndx;

            /* Encode Key */
            res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_encode(
                    unit,
                    frwrd_table_id,
                    vrf_ndx, 
                    route_key, 
                    &data,
                    &mask);
            SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

            /* Remove Record */
            res = arad_pp_tcam_route_kbp_remove_unsafe(
                    unit,
                    frwrd_table_id,
                    &data,
                    &mask);
            SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 
        }
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_remove()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_get(
        SOC_SAND_IN  int                          unit,
        SOC_SAND_IN  uint32                       vrf_ndx,
        SOC_SAND_IN  SOC_SAND_PP_IPV6_SUBNET      *route_key,
        SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO  *route_info,
        SOC_SAND_OUT uint8                        *found
    )
{
    uint32
        priority,
        res = SOC_SAND_OK;
    ARAD_PP_LEM_ACCESS_PAYLOAD
        payload;
    ARAD_PP_LEM_ACCESS_KEY       
        data,
        mask;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FRWRD_IPV4_HOST_ROUTE_INFO 
        host_route_info;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32
        ipv6_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);


    if(!ARAD_KBP_ENABLE_IPV6_EXTENDED && SOC_IS_JERICHO(unit)) {

        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->prefix_len, ipv6_mask);

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[1], route_key->ipv6_address.address, ipv6_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[2], route_key->ipv6_address.address, ipv6_mask);

        table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_FWD_KBP;

        res = arad_pp_dbal_entry_get(
                unit, 
                table_id,
                qual_vals,
                (void*)&payload,
                &priority,
                NULL,
                found);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
    }
    else {
        if(ARAD_KBP_ENABLE_IPV6_EXTENDED) {
            frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_1;
        }
        else {
            frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0;
        }

        /* Encode Key */
        res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_encode(
                unit,
                frwrd_table_id,
                vrf_ndx, 
                route_key, 
                &data,
                &mask);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

        /* get record */
        res = arad_pp_tcam_route_kbp_get_unsafe(
                unit,
                frwrd_table_id,
                &data,
                &mask,
                &priority,
                &payload,
                found);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 
    }

    if(*found) 
    {
        SOC_PPC_FRWRD_IPV4_HOST_ROUTE_INFO_clear(&host_route_info);
        res = arad_pp_frwrd_em_dest_to_fec(
                unit, 
                &payload, 
                &host_route_info);
        SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

        sal_memcpy(route_info, &host_route_info.frwrd_decision, sizeof(SOC_PPC_FRWRD_DECISION_INFO));
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_get()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_key_mask_encode(
      SOC_SAND_IN  int                           unit,
      SOC_SAND_IN  ARAD_KBP_FRWRD_IP_TBL_ID     frwrd_table_id,
      SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY  *route_key,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY       *data,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY       *mask
    )
{
    uint32
        res;
	uint32
	  mask_full = SOC_SAND_U32_MAX;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    ARAD_PP_LEM_ACCESS_KEY_clear(data);
    ARAD_PP_LEM_ACCESS_KEY_clear(mask);

	if ((route_key->inrif.mask != 0)&&(route_key->source.prefix_len != 128)) {
		SOC_SAND_SET_ERROR_CODE(ARAD_PP_FRWRD_IPV6_MC_ILLEGAL_DEST_TYPE_ERR,10,exit);
	}

	if ((route_key->source.prefix_len != 0)&&(route_key->group.prefix_len != 128)) {
		SOC_SAND_SET_ERROR_CODE(ARAD_PP_FRWRD_IPV6_MC_ILLEGAL_DEST_TYPE_ERR,10,exit);
	}

    /* Encode Key into buffer according to the following format:
     *  
    * {VRF(14:0) in 2287:272, IP-MC-address(119:0) 271:144, Source-IP(119:0) in 143:16 IN-RIF(11:0) in 15:0} 
     *  
     */

	/* Param 0 - IN-RIF */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 0, route_key->inrif.val, data);
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 0, route_key->inrif.mask, mask);

    /* Param 1  - SIP address */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 1, 0, data);
    SHR_BITCOPY_RANGE(data->param[1].value, 0 /* LSB */, route_key->source.ipv6_address.address, 0 /* LSB */, data->param[1].nof_bits);
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 1, 0 /* Copy the bits after */, mask);
    /* Build the logical IPv6 address full Mask */
    if (route_key->source.prefix_len) {
        SHR_BITSET_RANGE(mask->param[1].value, SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - route_key->source.prefix_len, route_key->source.prefix_len);
    }

    /* Param 2  - IPv6 address */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 2, 0 /* Copy the bits after */, data);
    SHR_BITCOPY_RANGE(data->param[2].value, 0 /* LSB */, route_key->group.ipv6_address.address, 0 /* LSB */, data->param[2].nof_bits);

    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 2, 0 /* Copy the bits after */, mask);
    SHR_BITSET_RANGE(mask->param[2].value, SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - route_key->group.prefix_len, route_key->group.prefix_len);

    /* Param 3 - VRF */
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 3, route_key->vrf_ndx, data);
    ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_ENCODE(unit, frwrd_table_id, 3, mask_full, mask);

    ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_key_mask_encode()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_key_mask_decode(
      SOC_SAND_IN  int                           unit,
      SOC_SAND_IN  ARAD_KBP_FRWRD_IP_TBL_ID     frwrd_table_id,
      SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY       *data,
      SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY       *mask,
      SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY  *route_key
    )
{
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    SOC_SAND_CHECK_NULL_INPUT(data);
    SOC_SAND_CHECK_NULL_INPUT(mask);

    SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_clear(route_key);

    /* Encode Key into buffer according to the following format:
     *  
    * {VRF(14:0) in 2287:272, IP-MC-address(119:0) 271:144, Source-IP(119:0) in 143:16 IN-RIF(11:0) in 15:0} 
     *  
     */

    {
        uint32
          bit_ndx;

        /* Param 0  - In-RIF */
        ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 0, &route_key->inrif.val, data, mask);
        ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 0, &route_key->inrif.mask, mask, mask); /* get also the mask */

		/* Param 1  - SIP address */
        ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 1, route_key->source.ipv6_address.address, data, mask);
        route_key->source.prefix_len = 0;
        for (bit_ndx = 0; bit_ndx < SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS; bit_ndx++) {
            if (SHR_BITGET(mask->param[0].value, bit_ndx)) {
                route_key->source.prefix_len = SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - bit_ndx;
                break;
            }
        }

        /* Param 2  - IPv6 address */
        ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 2, route_key->group.ipv6_address.address, data, mask);
        route_key->group.prefix_len = 0;
        for (bit_ndx = 0; bit_ndx < SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS; bit_ndx++) {
            if (SHR_BITGET(mask->param[0].value, bit_ndx)) {
                route_key->group.prefix_len = SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - bit_ndx;
                break;
            }
        }

        /* Param 3  - VRF */
        ARAD_PP_FRWRD_IP_TCAM_LEM_KEY_DECODE(unit, frwrd_table_id, 3, &route_key->vrf_ndx, data, mask);
    }

    ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_key_mask_decode()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_route_add(
        SOC_SAND_IN  int                                  unit,
        SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY      *route_key,
        SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO     *route_info,
        SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE             *success
    )
{
    uint32
        priority,
        res = SOC_SAND_OK;
    ARAD_TCAM_ACTION                                
        action;
    ARAD_PP_LEM_ACCESS_KEY       
        data,
        mask;
    ARAD_PP_LEM_ACCESS_PAYLOAD
        payload;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FRWRD_DECISION_INFO 
        em_dest;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32
        ipv6_dip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S],
        ipv6_sip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    soc_sand_os_memset(&action, 0x0, sizeof(action));

    frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_MC;
    table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC_FWD_KBP;

    priority = SOC_SAND_PP_IPV6_ADDRESS_NOF_BITS - route_key->group.prefix_len;


    if(SOC_IS_JERICHO(unit)) {

        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, ipv6_dip_mask);
        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->source.prefix_len, ipv6_sip_mask);

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[1], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[2], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[3], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_LOW(&qual_vals[4], route_key->source.ipv6_address.address, ipv6_sip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_HIGH(&qual_vals[5], route_key->source.ipv6_address.address, ipv6_sip_mask);

        res = arad_pp_frwrd_ipv4_sand_dest_to_fwd_decision(
                unit,
                &(route_info->dest_id),
                &em_dest);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

        res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode(
                unit,
                frwrd_table_id,
                &em_dest,
                &payload);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        res = arad_pp_dbal_entry_add(
                unit,
                table_id,
                qual_vals,
                priority,
                &payload,
                success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }
    else {

        /* Encode Key */
        res = arad_pp_frwrd_ipv6_mc_kbp_key_mask_encode(
                unit,
                frwrd_table_id,
                route_key,
                &data,
                &mask);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

        /* Encode Result - same as Unicast logic */
        res = arad_pp_frwrd_ipv4_sand_dest_to_fwd_decision(
                unit,
                &(route_info->dest_id),
                &em_dest);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

        res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_result_encode(
                unit,
                frwrd_table_id,
                &em_dest,
                &payload);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        /* Add record */
        res = arad_pp_tcam_route_kbp_add_unsafe(
                unit,
                frwrd_table_id,
                &data,
                &mask,
                priority,
                &payload,
                success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_route_add()",0,0);
}


STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_route_remove(
        SOC_SAND_IN  int                               unit,
        SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY   *route_key
    )
{
    uint32
        res = SOC_SAND_OK;
    ARAD_PP_LEM_ACCESS_KEY
        data,
        mask;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    SOC_SAND_SUCCESS_FAILURE
        success;
    uint32
        ipv6_dip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S],
        ipv6_sip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);


    if(SOC_IS_JERICHO(unit)) {

        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, ipv6_dip_mask);
        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->source.prefix_len, ipv6_sip_mask);

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[1], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[2], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[3], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_LOW(&qual_vals[4], route_key->source.ipv6_address.address, ipv6_sip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_HIGH(&qual_vals[5], route_key->source.ipv6_address.address, ipv6_sip_mask);

        table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC_FWD_KBP;

        res = arad_pp_dbal_entry_delete(
                unit, 
                table_id,
                qual_vals,
                &success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }
    else {

        frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_MC;

        /* Encode Key */
        res = arad_pp_frwrd_ipv6_mc_kbp_key_mask_encode(
                unit,
                frwrd_table_id,
                route_key,
                &data,
                &mask);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 


        /* Remove Record */
        res = arad_pp_tcam_route_kbp_remove_unsafe(
                unit,
                frwrd_table_id,
                &data,
                &mask);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_route_remove()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_route_get(
        SOC_SAND_IN  int                                  unit,
        SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY      *route_key,
        SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO     *route_info,
        SOC_SAND_OUT uint8                                *found
    )
{
    uint32
        priority,
        res = SOC_SAND_OK;
    ARAD_PP_LEM_ACCESS_PAYLOAD
        payload;
    ARAD_PP_LEM_ACCESS_KEY       
        data,
        mask;
    ARAD_KBP_FRWRD_IP_TBL_ID
        frwrd_table_id;
    SOC_DPP_DBAL_SW_TABLE_IDS
        table_id;
    SOC_PPC_FP_QUAL_VAL
        qual_vals[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
    uint32
        ipv6_dip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S],
        ipv6_sip_mask[SOC_SAND_PP_IPV6_ADDRESS_NOF_UINT32S];
    uint32                    
        payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S];

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);


    if(SOC_IS_JERICHO(unit)) {

        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->group.prefix_len, ipv6_dip_mask);
        arad_pp_frwrd_ipv6_prefix_to_mask(route_key->source.prefix_len, ipv6_sip_mask);

        DBAL_QUAL_VALS_CLEAR(qual_vals);
        DBAL_QUAL_VAL_ENCODE_VRF(&qual_vals[0], route_key->vrf_ndx, SOC_SAND_U32_MAX);
        DBAL_QUAL_VAL_ENCODE_IN_RIF(&qual_vals[1], route_key->inrif.val, route_key->inrif.val > 0 ? SOC_SAND_U32_MAX : 0);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_LOW(&qual_vals[2], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_DIP_HIGH(&qual_vals[3], route_key->group.ipv6_address.address, ipv6_dip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_LOW(&qual_vals[4], route_key->source.ipv6_address.address, ipv6_sip_mask);
        DBAL_QUAL_VAL_ENCODE_IPV6_SIP_HIGH(&qual_vals[5], route_key->source.ipv6_address.address, ipv6_sip_mask);

        table_id = SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC_FWD_KBP;

        res = arad_pp_dbal_entry_get(
                unit,
                table_id,
                qual_vals,
                (void*)&payload,
                &priority,
                NULL,
                found);
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    }
    else {

        frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_MC;

        /* Encode Key */
        res = arad_pp_frwrd_ipv6_mc_kbp_key_mask_encode(
                unit,
                frwrd_table_id,
                route_key,
                &data,
                &mask);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

        /* get record */
        res = arad_pp_tcam_route_kbp_get_unsafe(
                unit,
                frwrd_table_id,
                &data,
                &mask,
                &priority,
                &payload,
                found);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 
    }

    if(*found) {
        sal_memset(&payload_data, 0x0, ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S * sizeof(uint32));

        /* Decode Result */
        res = arad_pp_lem_access_payload_build(
                unit,
                &payload,
                payload_data);
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

        res = arad_pp_frwrd_ipv4_em_dest_to_sand_dest(
                unit,
                payload_data[0], /* Get only the destination part */
                &(route_info->dest_id));
        SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_route_get()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_uc_kbp_table_clear(
      SOC_SAND_IN  int   unit
    )
{
    uint32
        table_ndx,
        nof_tables,
        frwrd_table_id,
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    if (ARAD_KBP_ENABLE_IPV6_EXTENDED) { /* both tables should be cleared */
        frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_EXTENDED_IPV6;

        /* Clear Table */
        res = arad_pp_tcam_kbp_table_clear(
                unit,
                frwrd_table_id
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_1;

        /* Clear Table */
        res = arad_pp_tcam_kbp_table_clear(
                unit,
                frwrd_table_id
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    } else {

#ifdef BCM_88660_A0
        if (SOC_IS_ARADPLUS(unit)
            && (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "ext_rpf_fwd_parallel", 0) == 0)) {
            nof_tables = 1;
        } else
#endif /* BCM_88660 */
        {
            nof_tables = 2;
        }

		for (table_ndx = 0; table_ndx < nof_tables; table_ndx++) {
            frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0 + table_ndx;

            /* Clear Table */
            res = arad_pp_tcam_kbp_table_clear(
               unit,
               frwrd_table_id
               );
            SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        }
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_uc_kbp_table_clear()",0,0);
}

STATIC
  uint32
    arad_pp_frwrd_ipv6_mc_kbp_table_clear(
      SOC_SAND_IN  int   unit
    )
{
    uint32
        frwrd_table_id,
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    frwrd_table_id = ARAD_KBP_FRWRD_TBL_ID_IPV6_MC;

    /* Clear Table */
    res = arad_pp_tcam_kbp_table_clear(
            unit,
            frwrd_table_id
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit); 

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_mc_kbp_table_clear()",0,0);
}

#endif

STATIC
 uint32
  arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_DPP_DBAL_SW_TABLE_IDS              frwrd_table_id,
    SOC_SAND_IN  uint32                                 vrf_ndx,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_keys_uc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_keys_mc,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_infos_uc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *route_infos_mc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *routes_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *routes_location,
    SOC_SAND_OUT uint32                                 *nof_entries
  )
{
  uint32
      res = SOC_SAND_OK,
      *payload = NULL,
      i,
      route_ndx,
	  block_start_ndx,
      indx;

  int current_location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_LPM;

  SOC_PPC_FP_QUAL_VAL
      *qual_vals_array = NULL;

  SOC_PPC_FP_QUAL_TYPE qual_type;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_CHECK_NULL_INPUT(block_range_key);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);


  if (frwrd_table_id == SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC) {
      SOC_SAND_CHECK_NULL_INPUT(route_keys_mc);
      SOC_SAND_CHECK_NULL_INPUT(route_infos_mc);
      if (SOC_IS_ARADPLUS_AND_BELOW(unit)) {
          current_location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
      }
  }
  else {
      SOC_SAND_CHECK_NULL_INPUT(route_keys_uc);
      SOC_SAND_CHECK_NULL_INPUT(route_infos_uc);
      if (SOC_IS_ARADPLUS_AND_BELOW(unit) || (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "l3_ipv6_uc_use_tcam", 0))) {
          current_location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
      }
  }

  if(block_range_key->entries_to_act == 0)
  {
    *nof_entries = 0;
    goto exit;
  }

  /*allocate and initialize qual_vals and payload arrays*/
  qual_vals_array = soc_sand_os_malloc_any_size(sizeof(SOC_PPC_FP_QUAL_VAL) * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX * block_range_key->entries_to_act,"qual_vals_array");
  payload = soc_sand_os_malloc_any_size(sizeof(uint32) * BYTES2WORDS(sizeof(uint32)) * block_range_key->entries_to_act,"payload");

  for (i=0; i < block_range_key->entries_to_act; i++) {
      DBAL_QUAL_VALS_CLEAR(qual_vals_array + i * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX);
  }
  res = soc_sand_os_memset(payload, 0x0, sizeof(uint32) * BYTES2WORDS(sizeof(uint32)) * block_range_key->entries_to_act);
  SOC_SAND_CHECK_FUNC_RESULT(res, 19, exit);

   if (vrf_ndx != -1) {
      qual_vals_array[0].type = SOC_PPC_FP_QUAL_IRPP_VRF;
      qual_vals_array[0].val.arr[0] = vrf_ndx;
      qual_vals_array[0].is_valid.arr[0] = SOC_SAND_U32_MAX;
  }

  /* Get the entries */
  block_start_ndx = *nof_entries;
  res = arad_pp_dbal_block_get(unit, block_range_key, frwrd_table_id, qual_vals_array, payload, nof_entries);
  SOC_SAND_CHECK_FUNC_RESULT(res, 25, exit);

  /* Decode the entries: content and payload */
  
  for(indx = 0; indx < (*nof_entries - block_start_ndx); ++indx)
  {
	  route_ndx = indx;
      /* Decode the key */
      for (i = 0; i < SOC_PPC_FP_NOF_QUALS_PER_DB_MAX; i++) {
          qual_type = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].type;
          if ((qual_type == SOC_PPC_NOF_FP_QUAL_TYPES) || (qual_type == BCM_FIELD_ENTRY_INVALID)) {
              break;
          }
          if (frwrd_table_id == SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC) {
              if (qual_type == SOC_PPC_FP_QUAL_HDR_FWD_IPV6_DIP_LOW) {
                  route_keys_mc[route_ndx].group.ipv6_address.address[0] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
                  route_keys_mc[route_ndx].group.ipv6_address.address[1] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[1];
                  route_keys_mc[route_ndx].group.prefix_len += _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[0]) +
                                                               _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[1]);
              }
              if (qual_type == SOC_PPC_FP_QUAL_HDR_FWD_IPV6_DIP_HIGH) {
                  route_keys_mc[route_ndx].group.ipv6_address.address[2] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
                  route_keys_mc[route_ndx].group.ipv6_address.address[3] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[1];
                  route_keys_mc[route_ndx].group.prefix_len += _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[0]) +
                                                               _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[1]);
              }
              if (qual_type == SOC_PPC_FP_QUAL_IRPP_IN_RIF) {
                  route_keys_mc[route_ndx].inrif.val = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
                  route_keys_mc[route_ndx].inrif.mask = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[0];
              }
              if (qual_type == SOC_PPC_FP_QUAL_IRPP_VRF) {
                  route_keys_mc[route_ndx].vrf_ndx = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
              }
          } else if (frwrd_table_id == SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE) {
              if (qual_type == SOC_PPC_FP_QUAL_HDR_FWD_IPV6_DIP_LOW) {
                  route_keys_uc[route_ndx].subnet.ipv6_address.address[0] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
                  route_keys_uc[route_ndx].subnet.ipv6_address.address[1] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[1];
                  route_keys_uc[route_ndx].subnet.prefix_len += _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[0]) +
                                                                _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[1]);
              }
              if (qual_type == SOC_PPC_FP_QUAL_HDR_FWD_IPV6_DIP_HIGH) {
                  route_keys_uc[route_ndx].subnet.ipv6_address.address[2] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[0];
                  route_keys_uc[route_ndx].subnet.ipv6_address.address[3] = qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].val.arr[1];
                  route_keys_uc[route_ndx].subnet.prefix_len += _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[0]) +
                                                                _shr_ip_mask_length(qual_vals_array[indx * SOC_PPC_FP_NOF_QUALS_PER_DB_MAX + i].is_valid.arr[1]);
              }
          }
      }
      if (frwrd_table_id == SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC) {

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
          if (JER_KAPS_ENABLE(unit)) {
              res = jer_pp_kaps_payload_to_mc_dest_id(unit, payload[indx], &route_infos_mc[route_ndx].dest_id);
              SOC_SAND_CHECK_FUNC_RESULT(res, 35, exit);
          }else
#endif
          {
              res = arad_pp_frwrd_ipv4_em_dest_to_sand_dest(unit, payload[indx], &(route_infos_mc[route_ndx].dest_id));
              SOC_SAND_CHECK_FUNC_RESULT(res, 35, exit);
          }
      }

      if (frwrd_table_id == SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE) {
          ARAD_TCAM_ACTION  action;
          action.value[0] = payload[indx];
          action.value[1] = payload[indx+1];
          res = arad_pp_frwrd_ipv6_uc_or_vpn_decode_payload(unit, &action, &(route_infos_uc[route_ndx]));
          SOC_SAND_CHECK_FUNC_RESULT(res, 35, exit);          
      }

      if(routes_location)
      {
          routes_location[route_ndx] = current_location;          
      }
      if(routes_status)
      {
          routes_status[route_ndx] = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      }
  }

exit:
  if (qual_vals_array)
  {
    soc_sand_os_free_any_size(qual_vals_array);
  }
  if (payload)
  {
    soc_sand_os_free_any_size(payload);
  }

  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_kaps_dbal_route_get_block()", 0, 0);
}

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
STATIC
 uint32
  arad_pp_frwrd_ipv6_kbp_route_get_block_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  ARAD_KBP_FRWRD_IP_TBL_ID               frwrd_table_id,
    SOC_SAND_IN  uint32                                 vrf_ndx,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_keys_uc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_keys_mc,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_infos_uc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *route_infos_mc,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *routes_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *routes_location,
    SOC_SAND_OUT uint32                                 *nof_entries
  )
{
  uint32
    res = SOC_SAND_OK,
      nof_entries_lcl,
      route_ndx,
    indx;
  uint32                    
    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S] = {0};
  ARAD_PP_LEM_ACCESS_KEY
      *read_keys = NULL,
      *read_keys_mask = NULL;
  ARAD_PP_LEM_ACCESS_PAYLOAD
      *read_vals = NULL;
  uint32                   
      vrf_ndx_lcl;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_CHECK_NULL_INPUT(block_range_key);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);
  if (frwrd_table_id == ARAD_KBP_FRWRD_TBL_ID_IPV6_MC) {
      SOC_SAND_CHECK_NULL_INPUT(route_keys_mc);
      SOC_SAND_CHECK_NULL_INPUT(route_infos_mc);
  }
  else {
      SOC_SAND_CHECK_NULL_INPUT(route_keys_uc);
      SOC_SAND_CHECK_NULL_INPUT(route_infos_uc);
  }

  if(block_range_key->entries_to_act == 0)
  {
    *nof_entries = 0;
    goto exit;
  }

  /* Initialize read_keys_mask */
  read_keys_mask = soc_sand_os_malloc_any_size(sizeof(ARAD_PP_LEM_ACCESS_KEY) * block_range_key->entries_to_act,"read_keys");
  read_keys = soc_sand_os_malloc_any_size(sizeof(ARAD_PP_LEM_ACCESS_KEY) * block_range_key->entries_to_act,"read_keys");
  read_vals = soc_sand_os_malloc_any_size(sizeof(ARAD_PP_LEM_ACCESS_PAYLOAD) * block_range_key->entries_to_act,"read_vals");
  res = soc_sand_os_memset(read_keys, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY) * block_range_key->entries_to_act);
  SOC_SAND_CHECK_FUNC_RESULT(res, 13, exit);
  res = soc_sand_os_memset(read_keys_mask, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY) * block_range_key->entries_to_act);
  SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);
  res = soc_sand_os_memset(read_vals, 0x0, sizeof(ARAD_PP_LEM_ACCESS_PAYLOAD) * block_range_key->entries_to_act);
  SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);

  /* Get the entries */
  res = arad_pp_tcam_route_kbp_get_block_unsafe(
            unit,
            frwrd_table_id,
            block_range_key,
            read_keys,
            read_keys_mask,
            read_vals,
            &nof_entries_lcl
          );
  SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);

  /* Decode the entries: content and payload */
  route_ndx = 0;
  for(indx = 0; indx < nof_entries_lcl; ++indx)
  {
      /* Decode the key */
      if (frwrd_table_id == ARAD_KBP_FRWRD_TBL_ID_IPV6_MC) {
          res = arad_pp_frwrd_ipv6_mc_kbp_key_mask_decode(
                    unit,
                    frwrd_table_id,
                    &read_keys[indx],
                    &read_keys_mask[route_ndx],
                    &route_keys_mc[route_ndx]
                  );
          SOC_SAND_CHECK_FUNC_RESULT(res, 14, exit);

          /* Decode Result */
          sal_memset(&payload_data, 0x0, ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S * sizeof(uint32));
          res = arad_pp_lem_access_payload_build(
                  unit,
                  &read_vals[indx],
                  payload_data
                );
          SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

          res = arad_pp_frwrd_ipv4_em_dest_to_sand_dest(
                  unit,
                  payload_data[0], /* Get only the destination part */
                  &(route_infos_mc[route_ndx].dest_id)
                );
          SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

      }
      else {
          SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY_clear(&route_keys_uc[indx]);
          res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_key_mask_decode(
                    unit,
                    frwrd_table_id,
                    &read_keys[indx],
                    &read_keys_mask[indx],
                    &vrf_ndx_lcl,
                    &(route_keys_uc[route_ndx].subnet)
                  );
          SOC_SAND_CHECK_FUNC_RESULT(res, 14, exit);

          /* For IPv6 Unicast, verify VRFs are equal */
          if (vrf_ndx_lcl != vrf_ndx) {
              continue;
          }

          res = arad_pp_fwd_decision_in_buffer_parse(
                unit,                
                read_vals[indx].dest,
                read_vals[indx].asd,
                ARAD_PP_FWD_DECISION_PARSE_LEGACY | ARAD_PP_FWD_DECISION_PARSE_DEST,
                &(route_infos_uc[route_ndx])
            );
          SOC_SAND_CHECK_FUNC_RESULT(res, 16, exit);
      }

      if(routes_location)
      {
        routes_location[route_ndx] = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
      }
      if(routes_status)
      {
        routes_status[route_ndx] = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      }
      route_ndx ++;
  }
  *nof_entries = route_ndx;

exit:
  if (read_keys)
  {
    soc_sand_os_free_any_size(read_keys);
  }
  if (read_keys_mask)
  {
    soc_sand_os_free_any_size(read_keys_mask);
  }
  if (read_vals)
  {
    soc_sand_os_free_any_size(read_vals);
  }

  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_kbp_route_get_block_unsafe()", 0, 0);
}



#endif

/*********************************************************************
*     Add IPv6 route entry to the routing table. Binds between
 *     Ipv6 Unicast route key (IPv6-address/prefix) and a FEC
 *     entry identified by fec_id for a given virtual router.
 *     As a result of this operation, Unicast Ipv6 packets
 *     designated to IP address matching the given key (as long
 *     there is no more-specific route key) will be routed
 *     according to the information in the FEC entry identified
 *     by fec_id.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_uc_route_add_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO            *route_info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(success);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED)
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_add(
                unit,
                0, /* vrf ndx */
                &(route_key->subnet),
                route_info,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  } else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_add(
                unit,
                0, /* vrf ndx */
                &(route_key->subnet),
                route_info,
                route_key->route_scale,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_uc_route_add_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_uc_route_add_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO            *route_info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_ADD_VERIFY);

  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY, route_key, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_DECISION_INFO, route_info, 20, exit);
  SOC_SAND_ERR_IF_ABOVE_NOF(route_info->dest_id, SOC_DPP_DEFS_GET(unit, nof_fecs), SOC_PPC_FEC_ID_OUT_OF_RANGE_ERR, 30, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_route_add_verify()", 0, 0);
}

/*********************************************************************
*     Gets the routing information (system-fec-id) associated
 *     with the given route key.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_uc_route_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  uint8                                  exact_match,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_info,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *route_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *location,
    SOC_SAND_OUT uint8                                  *found
  )
{
  uint32 res;

  uint8 hit_bit;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(route_info);
  SOC_SAND_CHECK_NULL_INPUT(found);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_get(
                unit,
                0, /* vrf ndx */
                &(route_key->subnet),
                route_info,
                found);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
  } else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_get(
                unit,
                0, /* vrf ndx */
                &(route_key->subnet),
                route_key->route_scale,
                route_info,
                found,
                &hit_bit);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      if (hit_bit) {
          *route_status |= SOC_PPC_FRWRD_IP_ROUTE_STATUS_ACCESSED;
      }

       *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_LPM;

      if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
         *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
      }     
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_route_get_unsafe()",0,0);
}


uint32
  arad_pp_frwrd_ipv6_uc_route_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  uint8                                  exact_match
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_VERIFY);

  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY, route_key, 10, exit);

  SOC_SAND_TODO_IMPLEMENT_WARNING;
  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_route_get_verify()", 0, 0);
}

/*********************************************************************
*     Gets the Ipv6 UC routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_uc_route_get_block_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_keys,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_infos,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *routes_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *routes_location,
    SOC_SAND_OUT uint32                                 *nof_entries
  )
{
  uint32 res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(block_range_key);
  SOC_SAND_CHECK_NULL_INPUT(route_keys);
  SOC_SAND_CHECK_NULL_INPUT(route_infos);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);

  if(block_range_key->entries_to_act == 0)
  {
    *nof_entries = 0;
    goto exit;
  }

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_kbp_route_get_block_unsafe(
                unit,
                ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0,
                0 /* vrf_ndx */,
                block_range_key,
                route_keys,
                NULL,
                route_infos,
                NULL,
                routes_status,
                routes_location,
                nof_entries
              );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  } else
#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */
  {
      res = arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
                unit,
                SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE,
                0 /* vrf_ndx */,
                block_range_key,
                route_keys,
                NULL,
                route_infos,
                NULL,
                routes_status,
                routes_location,
                nof_entries
              );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  } 

exit:  
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_route_get_block_unsafe()", 0, 0);
}

uint32
  arad_pp_frwrd_ipv6_uc_route_get_block_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_GET_BLOCK_VERIFY);

  ARAD_PP_STRUCT_VERIFY(SOC_PPC_IP_ROUTING_TABLE_RANGE, block_range_key, 10, exit);

  SOC_SAND_TODO_IMPLEMENT_WARNING;
  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_route_get_block_verify()", 0, 0);
}

/*********************************************************************
*     Remove IPv6 route entry from the routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_uc_route_remove_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(success);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_remove(
                unit,
                0, /* vrf ndx */
                &(route_key->subnet));
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  } else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_remove(unit,0/* vrf ndx */, &(route_key->subnet), route_key->route_scale);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_route_remove_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_uc_route_remove_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY        *route_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTE_REMOVE_VERIFY);

  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY, route_key, 10, exit);

  SOC_SAND_TODO_IMPLEMENT_WARNING;
  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_route_remove_verify()", 0, 0);
}

/*********************************************************************
*     Clear the IPv6 UC routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_uc_routing_table_clear_unsafe(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  /* Check if Exteral Lookup device is used */
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_kbp_table_clear(
                unit
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else
#endif  
  {
      res = arad_pp_dbal_table_clear(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_uc_route_clear_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_uc_routing_table_clear_verify(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32 res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_UC_ROUTING_TABLE_CLEAR_VERIFY);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_routing_table_clear_verify()", 0, 0);
}

/*********************************************************************
*     Add IPv6 MC route entry to the routing table. Binds
 *     between Ipv6 Unicast route key (IPv6-address/prefix) and
 *     a FEC entry identified by fec_id for a given virtual
 *     router. As a result of this operation, Unicast Ipv6
 *     packets designated to IP address matching the given key
 *     (as long there is no more-specific route key) will be
 *     routed according to the information in the FEC entry
 *     identified by fec_id.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_mc_route_add_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *route_info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(route_info);
  SOC_SAND_CHECK_NULL_INPUT(success);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_MC) 
  {
      res = arad_pp_frwrd_ipv6_mc_kbp_route_add(
                unit,
                route_key,
                route_info,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }
  else
#endif
  {
      res = arad_pp_frwrd_ipv6_mc_kaps_dbal_route_add(
                unit,
                route_key,
                route_info,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_mc_route_add_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_mc_route_add_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *route_info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_ADD_VERIFY);

  if (ARAD_PP_IPMC_VRF_ID_MIN > 0)
  {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(route_key->vrf_ndx, ARAD_PP_IPMC_VRF_ID_MIN, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  else
  {
      SOC_SAND_ERR_IF_ABOVE_MAX(route_key->vrf_ndx, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  res = SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_verify(unit, route_key);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  if (!(route_info->flags & _BCM_IPMC_FLAGS_RAW_ENTRY)) {
      res = SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO_verify(unit, route_info);
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_route_add_verify()", 0, 0);
}

/*********************************************************************
*     Gets the routing information (system-fec-id) associated
 *     with the given route key.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_mc_route_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  uint8                                  exact_match,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *route_info,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *route_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *location,
    SOC_SAND_OUT uint8                                  *found
  )
{
  uint32
    res;
  uint8
    hit_bit;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(route_info);
  SOC_SAND_CHECK_NULL_INPUT(found);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_MC) 
  {
      res = arad_pp_frwrd_ipv6_mc_kbp_route_get(
                unit,
                route_key,
                route_info,
                found
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
  }
  else
#endif
  {
      res = arad_pp_frwrd_ipv6_mc_kaps_dbal_route_get(
                unit,
                route_key,
                route_info,
                found,
                &hit_bit
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
       if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
           *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_LPM;
       }else{
           *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
       }

       if (hit_bit) {
            *route_status |= SOC_PPC_FRWRD_IP_ROUTE_STATUS_ACCESSED;
       }
  }



exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_mc_route_get_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_mc_route_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_IN  uint8                                  exact_match
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_VERIFY);

  if (ARAD_PP_IPMC_VRF_ID_MIN > 0)
  {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(route_key->vrf_ndx, ARAD_PP_IPMC_VRF_ID_MIN, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  else
  {
      SOC_SAND_ERR_IF_ABOVE_MAX(route_key->vrf_ndx, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  res = SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_verify(unit, route_key);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_route_get_verify()", 0, 0);
}

/*********************************************************************
*     Gets the Ipv6 MC routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_mc_route_get_block_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO       *routes_info,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *routes_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *routes_location,
    SOC_SAND_OUT uint32                                 *nof_entries
  )
{
  uint32 res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(block_range_key);
  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(routes_info);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);

  if(block_range_key->entries_to_act == 0)
  {
    *nof_entries = 0;
    goto exit;
  }

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_MC) 
  {
      res = arad_pp_frwrd_ipv6_kbp_route_get_block_unsafe(
                unit,
                ARAD_KBP_FRWRD_TBL_ID_IPV6_MC,
                0 /* vrf_ndx */,
                block_range_key,
                NULL,
                route_key,
                NULL,
                routes_info,
                routes_status,
                routes_location,
                nof_entries
              );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  } else
#endif
  {
      /* (*block_range_key).start.payload.arr[0] holds the last entry traversed.
         (*block_range_key).start.payload.arr[1] holds the last vrf traversed.
       */
      uint32 vrf_ndx = (*block_range_key).start.payload.arr[1];
      SOC_DPP_DBAL_TABLE_INFO table;

      res = sw_state_access[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.get(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC, &table);
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
#if defined(BCM_JERICHO_SUPPORT) && defined(INCLUDE_KBP) && !defined(BCM_88030)
      if (table.physical_db_type == SOC_DPP_DBAL_PHYSICAL_DB_TYPE_KAPS) {
          if ((JER_KAPS_ENABLE_PUBLIC_DB(unit)) && (vrf_ndx == 0)) {
			  /*for KAPS we need to retrive the public entries before the private entries (VRF = 0)*/
			  res = arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
						unit,
						SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC,
						0, /*vrf_ndx*/
						block_range_key,
						NULL,
						route_key,
						NULL,
						routes_info,
						routes_status,
						routes_location,
						nof_entries
					  );
			  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
          }
          else {
			  res = arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
						unit,
						SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC,
						vrf_ndx,
						block_range_key,
						NULL,
						route_key,
						NULL,
						routes_info,
						routes_status,
						routes_location,
						nof_entries
					  );
			  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
          }
      }
      else
#endif
      {
		  res = arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
				  unit,
				  SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC,
				  vrf_ndx,
				  block_range_key,
				  NULL,
				  route_key,
				  NULL,
				  routes_info,
				  routes_status,
				  routes_location,
				  nof_entries
				);
		  SOC_SAND_CHECK_FUNC_RESULT(res, 35, exit);
      }
  }

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_route_get_block_unsafe()", 0, 0);
}

uint32
  arad_pp_frwrd_ipv6_mc_route_get_block_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_GET_BLOCK_VERIFY);

  ARAD_PP_STRUCT_VERIFY(SOC_PPC_IP_ROUTING_TABLE_RANGE, block_range_key, 10, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_route_get_block_verify()", 0, 0);
}

/*********************************************************************
*     Remove IPv6 route entry from the routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_mc_route_remove_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(success);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_MC) 
  {
      res = arad_pp_frwrd_ipv6_mc_kbp_route_remove(
                unit,
                route_key);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  }else
#endif
  {
      res = arad_pp_frwrd_ipv6_mc_kaps_dbal_route_remove(
                unit,
                route_key);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  } 

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_mc_route_remove_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_mc_route_remove_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY        *route_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTE_REMOVE_VERIFY);

  if (ARAD_PP_IPMC_VRF_ID_MIN > 0)
  {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(route_key->vrf_ndx, ARAD_PP_IPMC_VRF_ID_MIN, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  else
  {
      SOC_SAND_ERR_IF_ABOVE_MAX(route_key->vrf_ndx, (SOC_DPP_DEFS_GET(unit, nof_vrfs) -1), SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  }
  res = SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_verify(unit, route_key);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_route_remove_verify()", 0, 0);
}

/*********************************************************************
*     Clear the IPv6 MC routing table.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_mc_routing_table_clear_unsafe(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  /* Check if Exteral Lookup device is used */
  if(ARAD_KBP_ENABLE_IPV6_MC) 
  {
      res = arad_pp_frwrd_ipv6_mc_kbp_table_clear(
                unit
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else
#endif
  {
      res = arad_pp_dbal_table_clear(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6MC);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_mc_route_clear_unsafe()",0,0);
}
uint32
  arad_pp_frwrd_ipv6_mc_routing_table_clear_verify(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_MC_ROUTING_TABLE_CLEAR_VERIFY);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_mc_routing_table_clear_verify()", 0, 0);
}

/*********************************************************************
*     Setting global information of the VRF including
 *     (defaults forwarding).
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_info_set_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VRF_INFO            *vrf_info
  )
{
  uint32
    res = SOC_SAND_OK;
  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY
    route_key;
  SOC_SAND_SUCCESS_FAILURE
    success;
  SOC_PPC_FRWRD_DECISION_INFO            
    route_info;

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(vrf_info);

  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY_clear(&route_key);
  SOC_PPC_FRWRD_DECISION_INFO_clear(&route_info);

  /*
   *  Enter a unicast catch-all route
   */
  route_key.subnet.prefix_len = 0;

  route_info.type = SOC_PPC_FRWRD_DECISION_TYPE_FEC;
  route_info.dest_id = vrf_info->router_info.uc_default_action.value.fec_id;

  res = arad_pp_frwrd_ipv6_vrf_route_add_unsafe(
          unit,
          vrf_ndx,
          &route_key,
          &route_info,
          &success
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_info_set_unsafe()", vrf_ndx, 0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_info_set_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VRF_INFO            *vrf_info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPC_FRWRD_IPV6_VRF_INFO_SET_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  res = SOC_PPC_FRWRD_IPV6_VRF_INFO_verify(unit, vrf_info);
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  if (vrf_info->router_info.uc_default_action.type != SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_TYPE_FEC)
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_IPV6_DEFAULT_ACTION_TYPE_NOT_SUPPORTED_ERR, 30, exit);
  }

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_info_set_verify()", vrf_ndx, 0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_info_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_info_get_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Setting global information of the VRF including
 *     (defaults forwarding).
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_info_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_VRF_INFO            *vrf_info
  )
{
  uint32
    res = SOC_SAND_OK;
  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY
    route_key;
  SOC_PPC_FRWRD_IP_ROUTE_STATUS
    route_status;
  SOC_PPC_FRWRD_IP_ROUTE_LOCATION
    location;
  uint8
    found;
  SOC_PPC_FRWRD_DECISION_INFO
    route_info;

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPC_FRWRD_IPV6_VRF_INFO_GET_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(vrf_info);

  SOC_PPC_FRWRD_IPV6_VRF_INFO_clear(vrf_info);
  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY_clear(&route_key);

  vrf_info->router_info.uc_default_action.type = SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_TYPE_FEC;

  /*
   *  Get the unicast catch-all route
   */
  route_key.subnet.prefix_len = 0;

  res = arad_pp_frwrd_ipv6_vrf_route_get_unsafe(
          unit,
          vrf_ndx,
          &route_key,
          TRUE,
          &route_info,
          &route_status,
          &location,
          &found
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  vrf_info->router_info.uc_default_action.value.fec_id = route_info.dest_id;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_info_get_unsafe()", vrf_ndx, 0);
}

/*********************************************************************
*     Add IPv6 route entry to the virtual routing table (VRF).
 *     Binds between Ipv6 route key (UC/MC IPv6-address\prefix)
 *     and a FEC entry identified by fec_id for a given virtual
 *     router. As a result of this operation, Unicast Ipv6
 *     packets designated to IP address matching the given key
 *     (as long there is no more-specific route key) will be
 *     routed according to the information in the FEC entry
 *     identified by fec_id.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_route_add_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO            *route_info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(success);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED)
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_add(
                unit,
                vrf_ndx,
                &(route_key->subnet),
                route_info,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  } else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_add(
                unit,
                vrf_ndx,
                &(route_key->subnet),
                route_info,
                route_key->route_scale,
                success);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }

  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_frwrd_ipv6_vrf_route_add_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_route_add_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO            *route_info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_ADD_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY, route_key, 20, exit);
  if (!(route_info->flags & _BCM_L3_FLAGS2_RAW_ENTRY)) {
      ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_DECISION_INFO, route_info, 30, exit);
      SOC_SAND_ERR_IF_ABOVE_NOF(route_info->dest_id, SOC_DPP_DEFS_GET(unit, nof_fecs), SOC_PPC_FEC_ID_OUT_OF_RANGE_ERR, 40, exit);
  }
  else
  {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(route_info->dest_id, ARAD_PP_RAW_ID_MIN, ARAD_PP_RAW_ID_MAX, SOC_PPC_FEC_ID_OUT_OF_RANGE_ERR, 50, exit);
  }
  
  SOC_SAND_TODO_IMPLEMENT_WARNING;
  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_route_add_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Gets the routing information (system-fec-id) associated
 *     with the given route key on VRF.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_route_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key,
    SOC_SAND_IN  uint8                                  exact_match,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_info,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *route_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *location,
    SOC_SAND_OUT uint8                                  *found
  )
{
  uint32 res;

  uint8 hit_bit;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  SOC_SAND_CHECK_NULL_INPUT(route_info);
  SOC_SAND_CHECK_NULL_INPUT(found);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_get(
                unit,
                vrf_ndx, 
                &(route_key->subnet),
                route_info,
                found);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
  }
  else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_get(
                unit,
                vrf_ndx, 
                &(route_key->subnet),
                route_key->route_scale,
                route_info,
                found,
                &hit_bit);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 

      *route_status = SOC_PPC_FRWRD_IP_ROUTE_STATUS_COMMITED;
      if (hit_bit) {
          *route_status |= SOC_PPC_FRWRD_IP_ROUTE_STATUS_ACCESSED;
      }      
      *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_LPM;
      if(SOC_IS_ARADPLUS_AND_BELOW(unit)){
        *location = SOC_PPC_FRWRD_IP_ROUTE_LOCATION_TCAM;
      }
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_vrf_route_get_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_route_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key,
    SOC_SAND_IN  uint8                                  exact_match
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY, route_key, 20, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_route_get_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Gets the routing table of a virtual router (VRF).
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_route_get_block_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key,
    SOC_SAND_OUT SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_keys,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO            *route_infos,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_STATUS          *routes_status,
    SOC_SAND_OUT SOC_PPC_FRWRD_IP_ROUTE_LOCATION        *routes_location,
    SOC_SAND_OUT uint32                                 *nof_entries
  )
{
  uint32 res = SOC_SAND_OK;
  
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(block_range_key);
  SOC_SAND_CHECK_NULL_INPUT(route_keys);
  SOC_SAND_CHECK_NULL_INPUT(route_infos);
  SOC_SAND_CHECK_NULL_INPUT(nof_entries);

  if(block_range_key->entries_to_act == 0)
  {
    *nof_entries = 0;
    goto exit;
  }

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_kbp_route_get_block_unsafe(
                unit,
                ARAD_KBP_FRWRD_TBL_ID_IPV6_UC_RPF_0,
                vrf_ndx,
                block_range_key,
                route_keys,
                NULL,
                route_infos,
                NULL,
                routes_status,
                routes_location,
                nof_entries
              );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  } else
#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */
 {
      res = arad_pp_frwrd_ipv6_kaps_dbal_route_get_block(
                unit,
                SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE,
                vrf_ndx,
                block_range_key,
                route_keys,
                NULL,
                route_infos,
                NULL,
                routes_status,
                routes_location,
                nof_entries
              );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }

exit:

  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_route_get_block_unsafe()", 0, 0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_route_get_block_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_INOUT SOC_PPC_IP_ROUTING_TABLE_RANGE       *block_range_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_GET_BLOCK_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_IP_ROUTING_TABLE_RANGE, block_range_key, 20, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_route_get_block_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Remove IPv6 route entry from the routing table of a
 *     virtual router (VRF).
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_route_remove_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE               *success
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(route_key);
  
#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_kbp_route_remove(
                unit,
                vrf_ndx,
                &(route_key->subnet));
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  } else
#endif
  {
      res = arad_pp_frwrd_ipv6_uc_or_vpn_dbal_route_remove(
                unit,
                vrf_ndx,
                &(route_key->subnet),
                route_key->route_scale);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
      *success = SOC_SAND_SUCCESS;
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_vrf_route_remove_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_route_remove_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY       *route_key
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTE_REMOVE_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY, route_key, 20, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_route_remove_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Clear IPv6 routing table of VRF
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_routing_table_clear_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx
  )
{
  uint32 res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR_UNSAFE);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  /* Check if Exteral Lookup device is used */
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_kbp_table_clear(unit);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else
#endif
  {
      res = arad_pp_dbal_table_clear(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_pp_ipv6_vrf_route_clear_unsafe()",0,0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_routing_table_clear_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_VRF_ID                         vrf_ndx
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ROUTING_TABLE_CLEAR_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(vrf_ndx, ARAD_PP_VRF_ID_MIN, SOC_DPP_DEFS_GET(unit, nof_vrfs) - 1, SOC_PPC_VRF_ID_OUT_OF_RANGE_ERR, 10, exit);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_routing_table_clear_verify()", vrf_ndx, 0);
}

/*********************************************************************
*     Clear IPv6 routing tables for all VRFs.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_pp_frwrd_ipv6_vrf_all_routing_tables_clear_unsafe(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32 res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR_UNSAFE);

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
  /* Check if Exteral Lookup device is used */
  if(ARAD_KBP_ENABLE_IPV6_UC || ARAD_KBP_ENABLE_IPV6_EXTENDED) 
  {
      res = arad_pp_frwrd_ipv6_uc_kbp_table_clear(
                unit
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  }
  else 
#endif
  {
      res = arad_pp_dbal_table_clear(unit, SOC_DPP_DBAL_SW_TABLE_ID_IPV6UC_ROUTE);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit); 
  }
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_all_routing_tables_clear_unsafe()", 0, 0);
}

uint32
  arad_pp_frwrd_ipv6_vrf_all_routing_tables_clear_verify(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_FRWRD_IPV6_VRF_ALL_ROUTING_TABLES_CLEAR_VERIFY);

  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_vrf_all_routing_tables_clear_verify()", 0, 0);
}

/*********************************************************************
*     Get the pointer to the list of procedures of the
 *     arad_pp_api_frwrd_ipv6 module.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
CONST SOC_PROCEDURE_DESC_ELEMENT*
  arad_pp_frwrd_ipv6_get_procs_ptr(void)
{
  return Arad_pp_procedure_desc_element_frwrd_ipv6;
}
/*********************************************************************
*     Get the pointer to the list of errors of the
 *     arad_pp_api_frwrd_ipv6 module.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
CONST SOC_ERROR_DESC_ELEMENT*
  arad_pp_frwrd_ipv6_get_errs_ptr(void)
{
  return Arad_pp_error_desc_element_frwrd_ipv6;
}

uint32
  SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_VAL_verify(
    SOC_SAND_IN  int                                          unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_VAL *info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_SAND_ERR_IF_ABOVE_NOF(info->fec_id, SOC_DPP_DEFS_GET(unit, nof_fecs), SOC_PPC_FEC_ID_OUT_OF_RANGE_ERR, 10, exit);
  ARAD_PP_STRUCT_VERIFY(SOC_PPC_ACTION_PROFILE, &(info->action_profile), 11, exit);

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_VAL_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_verify(
    SOC_SAND_IN  int                                      unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION *info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_SAND_ERR_IF_ABOVE_MAX(info->type, ARAD_PP_FRWRD_IPV6_TYPE_MAX, ARAD_PP_FRWRD_IPV6_TYPE_OUT_OF_RANGE_ERR, 10, exit);
  res = SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_VAL_verify(unit, &(info->value));
  SOC_SAND_CHECK_FUNC_RESULT(res, 11, exit);

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_ROUTER_INFO_verify(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_ROUTER_INFO *info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  res = SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_verify(unit, &(info->uc_default_action));
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  res = SOC_PPC_FRWRD_IPV6_ROUTER_DEFAULT_ACTION_verify(unit, &(info->mc_default_action));
  SOC_SAND_CHECK_FUNC_RESULT(res, 11, exit);

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_ROUTER_INFO_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_VRF_INFO_verify(
    SOC_SAND_IN  int                          unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VRF_INFO *info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  res = SOC_PPC_FRWRD_IPV6_ROUTER_INFO_verify(unit, &(info->router_info));
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_VRF_INFO_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY_verify(
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_UC_ROUTE_KEY_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_verify(
      SOC_SAND_IN  int                                 unit,
      SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  
  if (info->source.prefix_len) {
#if defined(INCLUDE_KBP) && !defined(BCM_88030)
      if(ARAD_KBP_ENABLE_IPV6_MC) 
      {
          /* Allow non-zero Source-IP addresses */
      }
      else 
#endif
      {
          SOC_SAND_SET_ERROR_CODE(ARAD_PP_FRWRD_IPV4_MC_ILLEGAL_DEST_TYPE_ERR,10,exit);
      }
  }

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_MC_ROUTE_KEY_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY_verify(
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_VPN_ROUTE_KEY_verify()",0,0);
}

uint32
  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO_verify(
    SOC_SAND_IN int                               unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO *info
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  if (info->dest_id.dest_type != SOC_SAND_PP_DEST_MULTICAST && info->dest_id.dest_type != SOC_SAND_PP_DEST_FEC)
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_FRWRD_IPV4_MC_ILLEGAL_DEST_TYPE_ERR,10,exit);
  }
  if (info->dest_id.dest_type == SOC_SAND_PP_DEST_FEC)
  {
    SOC_SAND_ERR_IF_ABOVE_NOF(info->dest_id.dest_val, SOC_DPP_DEFS_GET(unit, nof_fecs), SOC_PPC_FEC_ID_OUT_OF_RANGE_ERR, 20, exit);
  }
  if (info->dest_id.dest_type == SOC_SAND_PP_DEST_MULTICAST)
  {
    SOC_SAND_ERR_IF_ABOVE_MAX(info->dest_id.dest_val, ARAD_MAX_MC_ID(unit),ARAD_MULT_MC_ID_OUT_OF_RANGE_ERR,30,exit);
  }

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in SOC_PPC_FRWRD_IPV6_MC_ROUTE_INFO_verify()",0,0);
}

uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_get_payload(
    SOC_SAND_IN int                           unit,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO  *route_info,
    SOC_SAND_OUT uint32                       *payload_data
  )
{
    uint32 res;
    ARAD_PP_LEM_ACCESS_PAYLOAD payload;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    ARAD_PP_LEM_ACCESS_PAYLOAD_clear(&payload);

    res = arad_pp_fwd_decision_in_buffer_build(
            unit,
            ARAD_PP_FRWRD_DECISION_APPLICATION_TYPE_DFLT,
            route_info,
            &payload.dest,
            &payload.asd
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 5, exit);

    if (route_info->additional_info.outlif.type != SOC_PPC_OUTLIF_ENCODE_TYPE_NONE) {
        payload.flags = ARAD_PP_FWD_DECISION_PARSE_OUTLIF;
    }
    else if (route_info->additional_info.eei.type != SOC_PPC_EEI_TYPE_EMPTY) {
        payload.flags = ARAD_PP_FWD_DECISION_PARSE_EEI;
    }
    else {
        payload.flags = ARAD_PP_FWD_DECISION_PARSE_DEST;
    }

    /* FEC + OutLif action is similar to LEM payload, only without 2 MSBs (40 bits) */
    res = arad_pp_lem_access_payload_build(
        unit,
        &payload,
        payload_data);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_or_vpn_get_payload()",0,0);
}


uint32
    arad_pp_frwrd_ipv6_uc_or_vpn_decode_payload(
    SOC_SAND_IN  int                           unit,
    SOC_SAND_IN  ARAD_TCAM_ACTION              *action,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO   *route_info
  )
{
    uint32 res = SOC_SAND_OK, action_value_mask;
    uint32 payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S] = {0};
    ARAD_PP_LEM_ACCESS_PAYLOAD  payload;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    ARAD_PP_LEM_ACCESS_PAYLOAD_clear(&payload);
    SOC_PPC_FRWRD_DECISION_INFO_clear(route_info);

    if(SOC_IS_JERICHO(unit)) {
        /*The returned action size will be  45 bits and as the 3 last MSBs bits are zeros a 42 bit will be used*/
        action_value_mask = 0x3ff;
    } else {
        /* convert action to FEC pointer */
        /* FEC + OutLif action is similar to LEM payload, only without 2 MSBs (40 bits) */
        action_value_mask = 0xff;
    }


    payload_data[0] =  action->value[0];
    payload_data[1] = (action->value[1] & action_value_mask);

    res = arad_pp_lem_access_payload_parse(
             unit,             
             payload_data,
             ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES,
             &payload);
    SOC_SAND_CHECK_FUNC_RESULT(res, 47, exit);

    res = arad_pp_fwd_decision_in_buffer_parse(
             unit,             
             payload.dest,
             payload.asd,
             ARAD_PP_FWD_DECISION_PARSE_LEGACY | ARAD_PP_FWD_DECISION_PARSE_DEST,
             route_info
    );
    SOC_SAND_CHECK_FUNC_RESULT(res, 5, exit);
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_ipv6_uc_or_vpn_decode_payload()",0,0);
}
/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>

#endif /* of #if defined(BCM_88650_A0) */



