/* $Id: arad_pp_trap_mgmt.h,v 1.14 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifndef __ARAD_PP_TRAP_MGMT_INCLUDED__
/* { */
#define __ARAD_PP_TRAP_MGMT_INCLUDED__

/*************
 * INCLUDES  *
 *************/
/* { */

#include <soc/dpp/pkt.h>
#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/ARAD/arad_tbl_access.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_framework.h>

#include <soc/dpp/ARAD/arad_chip_defines.h>

#include <soc/dpp/PPC/ppc_api_trap_mgmt.h>


/* } */
/*************
 * DEFINES   *
 *************/
/* { */

/* } */
/*************
 * MACROS    *
 *************/
/* { */

/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */
typedef struct
{
  soc_reg_t      reg;
  soc_field_t      field;
} ARAD_SOC_REG_FIELD;


typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_SET = ARAD_PP_PROC_DESC_BASE_TRAP_MGMT_FIRST,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_SET_UNSAFE,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_SET_VERIFY,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_GET,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_GET_PRINT,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_GET_VERIFY,
  ARAD_PP_TRAP_FRWRD_PROFILE_INFO_GET_UNSAFE,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_SET,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_SET_PRINT,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_SET_UNSAFE,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_SET_VERIFY,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_GET,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_GET_PRINT,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_GET_VERIFY,
  ARAD_PP_TRAP_SNOOP_PROFILE_INFO_GET_UNSAFE,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_SET,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_SET_PRINT,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_SET_UNSAFE,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_SET_VERIFY,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_GET,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_GET_PRINT,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_GET_VERIFY,
  ARAD_PP_TRAP_TO_EG_ACTION_MAP_GET_UNSAFE,
  ARAD_PP_TRAP_EG_PROFILE_INFO_SET,
  ARAD_PP_TRAP_EG_PROFILE_INFO_SET_PRINT,
  ARAD_PP_TRAP_EG_PROFILE_INFO_SET_UNSAFE,
  ARAD_PP_TRAP_EG_PROFILE_INFO_SET_VERIFY,
  ARAD_PP_TRAP_EG_PROFILE_INFO_GET,
  ARAD_PP_TRAP_EG_PROFILE_INFO_GET_PRINT,
  ARAD_PP_TRAP_EG_PROFILE_INFO_GET_VERIFY,
  ARAD_PP_TRAP_EG_PROFILE_INFO_GET_UNSAFE,
  ARAD_PP_TRAP_MACT_EVENT_GET,
  ARAD_PP_TRAP_MACT_EVENT_GET_PRINT,
  ARAD_PP_TRAP_MACT_EVENT_GET_UNSAFE,
  ARAD_PP_TRAP_MACT_EVENT_GET_VERIFY,
  ARAD_PP_TRAP_MACT_EVENT_PARSE,
  ARAD_PP_TRAP_MACT_EVENT_PARSE_PRINT,
  ARAD_PP_TRAP_MACT_EVENT_PARSE_UNSAFE,
  ARAD_PP_TRAP_MACT_EVENT_PARSE_VERIFY,
  ARAD_PP_TRAP_MGMT_GET_PROCS_PTR,
  ARAD_PP_TRAP_MGMT_GET_ERRS_PTR,
  /*
   * } Auto generated. Do not edit previous section.
   */

  ARAD_PP_TRAP_MGMT_TRAP_CODE_TO_INTERNAL,
  ARAD_PP_TRAP_CPU_TRAP_CODE_FROM_INTERNAL_UNSAFE,
  ARAD_PP_TRAP_PACKET_PARSE_INFO_GET_UNSAFE,
  ARAD_PP_TRAP_PACKET_PARSE_UNSAFE,
  ARAD_PP_TRAP_SB_TO_TRAP_CODE_MAP_GET,
  ARAD_PP_TRAP_SB_VTT_TO_INTERNAL_TRAP_CODE_MAP_GET,
  ARAD_PP_TRAP_PACKET_PARSE_OAM,
  /*
   * Last element. Do no touch.
   */
  ARAD_PP_TRAP_MGMT_PROCEDURE_DESC_LAST
} ARAD_PP_TRAP_MGMT_PROCEDURE_DESC;

typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  ARAD_PP_TRAP_MGMT_TRAP_CODE_NDX_OUT_OF_RANGE_ERR = ARAD_PP_ERR_DESC_BASE_TRAP_MGMT_FIRST,
  ARAD_PP_TRAP_MGMT_TRAP_TYPE_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_EG_ACTION_PROFILE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_PROFILE_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_BUFF_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_BUFF_LEN_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_VSI_SHIFT_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_COUNTER_SELECT_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_COUNTER_ID_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_METER_SELECT_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_METER_ID_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_METER_COMMAND_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_ETHERNET_POLICE_ID_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_FRWRD_OFFSET_INDEX_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_DA_TYPE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_FRWRD_OFFSET_BYTES_FIX_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_FRWRD_TYPE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_STRENGTH_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_BITMAP_MASK_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_SIZE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_SAMPLING_PROBABILITY_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_SNOOP_CMND_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_CUD_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_TYPE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_CPU_TRAP_CODE_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_CPU_TRAP_QUALIFIER_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_SRC_SYS_PORT_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_LL_HEADER_PTR_OUT_OF_RANGE_ERR,
  /*
   * } Auto generated. Do not edit previous section.
   */

  ARAD_PP_TRAP_MGMT_TRAP_CODE_NOT_SUPPORTED_ERR,
  ARAD_PP_TRAP_MGMT_SNP_STRENGTH_OUT_OF_RANGE_ERR,
  ARAD_PP_TRAP_MGMT_MORE_THAN_ONE_BIT_ENABLED_ERR,
  ARAD_PP_TRAP_MGMT_NOF_ACTIVE_CORES_OUT_OF_RANGE_ERR,

  /*
   * Last element. Do no touch.
   */
  ARAD_PP_TRAP_MGMT_ERR_LAST
} ARAD_PP_TRAP_MGMT_ERR;


typedef struct
{
  uint8 system_headers_mode;
  uint8 ftmh_stacking_ext_enable;
  uint8 ftmh_lb_key_ext_en;
} ARAD_PP_HEADER_DATA;


/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */
  
uint32
  arad_pp_trap_mgmt_trap_code_to_internal(
    SOC_SAND_IN  int                                         unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                           trap_code_ndx,
    SOC_SAND_OUT SOC_PPC_TRAP_CODE_INTERNAL                  *trap_code_internal,
    SOC_SAND_OUT ARAD_SOC_REG_FIELD                          *strength_fld_fwd,
    SOC_SAND_OUT ARAD_SOC_REG_FIELD                          *strength_fld_snp
  );

void
  arad_pp_trap_mgmt_tunnel_termination_trap_code_to_reg(
    SOC_SAND_IN  int                                         unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                           trap_code_ndx,
    SOC_SAND_OUT ARAD_SOC_REG_FIELD                          *strength_fld_fwd,
    SOC_SAND_OUT ARAD_SOC_REG_FIELD                          *strength_fld_snp
  );

int 
arad_pp_trap_mgmt_trap_code_to_trap_strength(int unit, int trap_code, 
                                                              uint8 *trap_strength, uint8 *additional_trap_strength);

uint32
  arad_pp_trap_mgmt_init_unsafe(
    SOC_SAND_IN  int                                 unit
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_frwrd_profile_info_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set forwarding action profile information.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  SOC_PPC_TRAP_CODE                           trap_code_ndx -
 *     Trap code. AradB range: 0-255. T20E range: 0-255, only
 *     enumarators mentioned as supported in T20e.user can use
 *     values not in the enum for used defined values.
 *   SOC_SAND_IN  SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO      *profile_info -
 *     Information to set to the forwarding profile.
 *   SOC_SAND_IN int                                    core_id-
 *      For which core the info being set, in case of SOC_PPC_CORE_ANY same info will be set for all cores.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_frwrd_profile_info_set_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO          *profile_info,
    SOC_SAND_IN  int                                    core_id
  );

uint32
  arad_pp_trap_frwrd_profile_info_set_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO          *profile_info,
    SOC_SAND_IN  int                                    core_id
  );

uint32
  arad_pp_trap_frwrd_profile_info_get_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_trap_frwrd_profile_info_set_unsafe" API.
 *     Refer to "arad_pp_trap_frwrd_profile_info_set_unsafe" API
 *     for details.
*********************************************************************/
uint32
  arad_pp_trap_frwrd_profile_info_get_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_OUT SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO          *profile_info
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_snoop_profile_info_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set snoop action profile information.
 * INPUT:
 *   SOC_SAND_IN  int                                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx -
 *     Trap code. AradB range: 0-255. T20E range: 0-255, only
 *     enumarators mentioned as supported in T20E.user can use
 *     values not in the enum for used defined values.
 *   SOC_SAND_IN  SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO          *profile_info -
 *     Information to set to the snoop profile.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_snoop_profile_info_set_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO          *profile_info
  );

uint32
  arad_pp_trap_snoop_profile_info_set_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO          *profile_info
  );

uint32
  arad_pp_trap_snoop_profile_info_get_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_trap_snoop_profile_info_set_unsafe" API.
 *     Refer to "arad_pp_trap_snoop_profile_info_set_unsafe" API
 *     for details.
*********************************************************************/
uint32
  arad_pp_trap_snoop_profile_info_get_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  SOC_PPC_TRAP_CODE                               trap_code_ndx,
    SOC_SAND_OUT SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO          *profile_info
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_to_eg_action_map_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Maps egress trap type to egress action profile.
 * INPUT:
 *   SOC_SAND_IN  int                                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                      trap_type_bitmap_ndx -
 *     Trap Type bitmap, the cause for trapping/filtering the packets
 *     (for example MTU). Use SOC_PPC_TRAP_EG_TYPE for bit offsets.
 *   SOC_SAND_IN  uint32                                      eg_action_profile -
 *     Egress action profile, to process/forward the packet
 *     according. To set the action pointed by this parameter
 *     use soc_ppd_trap_eg_profile_info_set(). Use
 *     SOC_PPC_TRAP_EG_NO_ACTION in order to bypass this trapping
 *     and then the packet will be processed/forwarded normal
 *     (as no trap was identified).
 * REMARKS:
 *   - Arad-B only. Error is returned if called for T20E.-
 *   In T20E: use soc_ppd_trap_snoop_profile_info_set() and
 *   soc_ppd_trap_frwrd_profile_info_set() to set action for Trap
 *   occurs in the T20E egress.- For part of the
 *   filters/traps (see SOC_PPC_EG_FILTER_PORT_ENABLE) user can
 *   set whether to perform the filter/trap per port. See
 *   soc_ppd_eg_filter_port_info_set()
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_to_eg_action_map_set_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      trap_type_bitmap_ndx,
    SOC_SAND_IN  uint32                                      eg_action_profile
  );

uint32
  arad_pp_trap_to_eg_action_map_set_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      trap_type_bitmap_ndx,
    SOC_SAND_IN  uint32                                      eg_action_profile
  );

uint32
  arad_pp_trap_to_eg_action_map_get_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      trap_type_bitmap_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_trap_to_eg_action_map_set_unsafe" API.
 *     Refer to "arad_pp_trap_to_eg_action_map_set_unsafe" API
 *     for details.
*********************************************************************/
uint32
  arad_pp_trap_to_eg_action_map_get_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      trap_type_bitmap_ndx,
    SOC_SAND_OUT uint32                                      *eg_action_profile
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_eg_profile_info_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set egress action profile information.
 * INPUT:
 *   SOC_SAND_IN  int                                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                      profile_ndx -
 *     Egress action profile. Range: 0 - 8.
 *   SOC_SAND_IN  SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO             *profile_info -
 *     Information to set to the egress profile.
 * REMARKS:
 *   - Arad-B only. Error is returned if called for T20E.-
 *   In T20E: use soc_ppd_trap_snoop_profile_info_set() and
 *   soc_ppd_trap_frwrd_profile_info_set() to set action for Trap
 *   occurs in the T20E egress.- Use
 *   soc_ppd_trap_to_eg_action_map_set() to map trap (reason) to
 *   action profile
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_eg_profile_info_set_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      profile_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO             *profile_info
  );

uint32
  arad_pp_trap_eg_profile_info_set_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      profile_ndx,
    SOC_SAND_IN  SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO             *profile_info
  );

uint32
  arad_pp_trap_eg_profile_info_get_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      profile_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_trap_eg_profile_info_set_unsafe" API.
 *     Refer to "arad_pp_trap_eg_profile_info_set_unsafe" API for
 *     details.
*********************************************************************/
uint32
  arad_pp_trap_eg_profile_info_get_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      profile_ndx,
    SOC_SAND_OUT SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO             *profile_info
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_mact_event_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Read MACT event from the events FIFO into buffer.
 * INPUT:
 *   SOC_SAND_IN  int                                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_OUT uint32                                      buff -
 *     SOC_PPC_TRAP_EVENT_BUFF_MAX_SIZE]- Buffer to copy the Event
 *     to. Event is copied to buff starting from buff[0] lsb.
 *   SOC_SAND_OUT uint32                                      *buff_len -
 *     the actual length of the returned buffer (in longs)
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_mact_event_get_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_OUT uint32                                      buff[SOC_PPC_TRAP_EVENT_BUFF_MAX_SIZE],
    SOC_SAND_OUT uint32                                      *buff_len
  );

uint32
  arad_pp_trap_mact_event_get_verify(
    SOC_SAND_IN  int                                     unit
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_mact_event_parse_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   The MACT may report different events using the event
 *   FIFO (e.g., learn, age, transplant, and retrieve). This
 *   API Parses the event buffer into a meaningful structure.
 * INPUT:
 *   SOC_SAND_IN  int                                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                      buff -
 *     SOC_PPC_TRAP_EVENT_BUFF_MAX_SIZE]- Buffer includes MACT
 *     event
 *   SOC_SAND_IN  uint32                                      buff_len -
 *     the actual length of the given buffer (in longs)
 *   SOC_SAND_OUT SOC_PPC_TRAP_MACT_EVENT_INFO                    *mact_event -
 *     MACT Event parsed into structure
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_mact_event_parse_unsafe(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      buff[SOC_PPC_TRAP_EVENT_BUFF_MAX_SIZE],
    SOC_SAND_IN  uint32                                      buff_len,
    SOC_SAND_OUT SOC_PPC_TRAP_MACT_EVENT_INFO                    *mact_event
  );

uint32
  arad_pp_trap_mact_event_parse_verify(
    SOC_SAND_IN  int                                     unit,
    SOC_SAND_IN  uint32                                      buff[SOC_PPC_TRAP_EVENT_BUFF_MAX_SIZE],
    SOC_SAND_IN  uint32                                      buff_len
  );

/*********************************************************************
* NAME:
*   arad_pp_trap_packet_parse_info_get_unsafe
* TYPE:
*   PROC
* FUNCTION:
*   Fill information required to parse packet arriving to CPU.
* INPUT:
*   SOC_SAND_IN  int                                unit -
*     Identifier of the device to access.
*   SOC_SAND_IN  SOC_PPC_TRAP_MGMT_PACKET_PARSE_INFO          *packet_parse_info-
*     Information required to parse packet arriving to CPU.
* REMARKS:
*   None.
* RETURNS:
*   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_packet_parse_info_get_unsafe(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_OUT SOC_PPC_TRAP_MGMT_PACKET_PARSE_INFO  *packet_parse_info
    );

uint32
  arad_pp_trap_packet_parse_unsafe(
    SOC_SAND_IN  int                               unit,
    SOC_SAND_IN  uint8                                *buff,
    SOC_SAND_IN  uint32                               buff_len,
    SOC_SAND_IN  SOC_PPC_TRAP_MGMT_PACKET_PARSE_INFO  *packet_parse_info,
    SOC_SAND_OUT SOC_PPC_TRAP_PACKET_INFO             *packet_info,
    SOC_SAND_OUT soc_pkt_t                            *dnx_pkt

  );

/*********************************************************************
* NAME:
*   arad_pp_trap_cpu_trap_code_from_internal_unsafe
* TYPE:
*   PROC
* FUNCTION:
*   Convert CPU trap code as received on packet to API enumerator.
* INPUT:
*   SOC_SAND_IN  int                                unit -
*     Identifier of the device to access.
*   SOC_SAND_IN  uint32                                internal_trap_code -
*     CPU trap code as received on packet.
*   SOC_SAND_IN  T20E_TRAP_CODE                           *trap_code-
*     Trap code enumerator.
* REMARKS:
*   None.
* RETURNS:
*   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_cpu_trap_code_from_internal_unsafe(
    SOC_SAND_IN  int                                unit,
    SOC_SAND_IN  int32                                 internal_trap_code,
    SOC_SAND_OUT SOC_PPC_TRAP_CODE                          *trap_code
  );

/*********************************************************************
* NAME:
*   arad_pp_trap_sb_to_trap_code_map_get
* TYPE:
*   PROC
* FUNCTION:
*   Convert CPU trap code Sticky bit packet to API enumerator.
* INPUT:
*   SOC_SAND_IN  int                                unit -
*     Identifier of the device to access.
*   uint32                   rng_num    -
*     which range of the sticky bit.
*   uint32                   sb_trap_indx    -
*     sticky bit trap index
*   SOC_SAND_IN  T20E_TRAP_CODE                           *trap_code-
*     Trap code enumerator.
* REMARKS:
*   None.
* RETURNS:
*   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_trap_sb_to_trap_code_map_get(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN   uint32                   rng_num,
    SOC_SAND_IN   uint32                   sb_trap_indx,
    SOC_SAND_OUT  SOC_PPC_TRAP_CODE           *trap_code
  );

/*********************************************************************
* NAME:
 *   arad_pp_trap_mgmt_get_procs_ptr
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the pointer to the list of procedures of the
 *   arad_pp_api_trap_mgmt module.
 * INPUT:
 * REMARKS:
 *
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
CONST SOC_PROCEDURE_DESC_ELEMENT*
  arad_pp_trap_mgmt_get_procs_ptr(void);

/*********************************************************************
* NAME:
 *   arad_pp_trap_mgmt_get_errs_ptr
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the pointer to the list of errors of the
 *   arad_pp_api_trap_mgmt module.
 * INPUT:
 * REMARKS:
 *
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
CONST SOC_ERROR_DESC_ELEMENT*
  arad_pp_trap_mgmt_get_errs_ptr(void);

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_DEST_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_DEST_INFO *info
  );

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_COUNT_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_COUNT_INFO *info
  );

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_METER_INFO_verify(
    SOC_SAND_IN  int                                    unit,
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_METER_INFO *info
  );

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_POLICE_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_POLICE_INFO *info
  );

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_COS_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_COS_INFO *info
  );

uint32
  SOC_PPC_TRAP_ACTION_PROFILE_PROCESS_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_ACTION_PROFILE_PROCESS_INFO *info
  );

uint32
  SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO_verify(
    SOC_SAND_IN  int                                    unit,
    SOC_SAND_IN  SOC_PPC_TRAP_FRWRD_ACTION_PROFILE_INFO *info
  );

uint32
  SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_SNOOP_ACTION_PROFILE_INFO *info
  );

uint32
  SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO *info
  );

uint32
  SOC_PPC_TRAP_MACT_EVENT_INFO_verify(
    SOC_SAND_IN  int                          unit,
    SOC_SAND_IN  SOC_PPC_TRAP_MACT_EVENT_INFO *info
  );

uint32
  SOC_PPC_TRAP_PACKET_INFO_verify(
    SOC_SAND_IN  SOC_PPC_TRAP_PACKET_INFO *info
  );

void
  arad_pp_trap_eg_profile_info_cud_to_hdr_data(
    SOC_SAND_IN  int                             unit,
    SOC_SAND_IN  uint32                             cud,
    SOC_SAND_OUT SOC_PPC_TRAP_EG_ACTION_HDR_DATA    *profile_hdr_data
  );
void
  arad_pp_trap_eg_profile_info_hdr_data_to_cud(
    SOC_SAND_IN  int                             unit,
    SOC_SAND_IN  SOC_PPC_TRAP_EG_ACTION_HDR_DATA    profile_hdr_data,
    SOC_SAND_OUT uint32                             *cud
  );

int
   arad_pp_trap_is_jericho_ingress_trap( 
       SOC_SAND_IN  int32 trap_code_internal,
       uint8 *is_ingress);

int
   arad_pp_trap_base_q_pair_to_port(
  /*in*/SOC_SAND_IN int unit,
   SOC_SAND_IN  int core,
   SOC_SAND_IN  uint32 base_q_pair, 
  /*out*/ SOC_SAND_OUT  uint32 *tm_port);

int
arad_pp_trap_get_first_cpu_port(
  /*in*/SOC_SAND_IN int unit,
        SOC_SAND_IN int core,
  /*out*/ SOC_SAND_OUT  uint32 *cpu_port_tm);

int
arad_pp_trap_cpu_ports_on_all_cores(
   /*in*/ SOC_SAND_IN int unit, 
   /*out*/ SOC_SAND_OUT uint8 *defined_on_all_cores);
 
int 
arad_pp_trap_action_profile_set_tm_port(
   /*in*/ SOC_SAND_IN int unit,
    SOC_SAND_IN int core,
    SOC_SAND_IN SOC_PPC_TM_PORT tm_port,
    SOC_SAND_IN SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO *profile_info, 
   /*out*/ SOC_SAND_OUT ARAD_PP_EGQ_ACTION_PROFILE_TABLE_TBL_DATA *tbl_data);

int 
arad_pp_trap_action_profile_get_tm_port_and_hdr_data(
   /*in*/ SOC_SAND_IN int unit,
   SOC_SAND_IN int core,
   SOC_SAND_IN ARAD_PP_EGQ_ACTION_PROFILE_TABLE_TBL_DATA *tbl_data,
   /*out*/  SOC_SAND_OUT SOC_PPC_TM_PORT *tm_port, 
   SOC_SAND_OUT SOC_PPC_TRAP_EG_ACTION_HDR_DATA    *profile_hdr_data,
   SOC_SAND_OUT SOC_PPC_TRAP_EG_ACTION_PROFILE_INFO *profile_info);
/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>

/* } __ARAD_PP_TRAP_MGMT_INCLUDED__*/
#endif


