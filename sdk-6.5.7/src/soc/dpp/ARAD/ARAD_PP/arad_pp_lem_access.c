#include <shared/bsl.h>

#include <soc/mcm/memregs.h>
#if defined(BCM_88650_A0)
/* $Id: arad_pp_lem_access.c,v 1.83 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FORWARD

#include <shared/bsl.h>
/*************
 * INCLUDES  *
 *************/
/* { */

#include <shared/swstate/access/sw_state_access.h>
#include <soc/dcmn/error.h>
#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>
#include <soc/dpp/SAND/Utils/sand_os_interface.h>

#include <soc/dpp/ARAD/arad_general.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_frwrd_mact_mgmt.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_lem_access.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_flp_init.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_flp_dbal.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_frwrd_fcf.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_frwrd_bmact.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_sw_db.h>
#include <soc/dpp/ARAD/arad_tbl_access.h>
#include <soc/dpp/ARAD/arad_reg_access.h>
#include <soc/dpp/ARAD/arad_chip_regs.h>
#include <soc/dpp/drv.h>
#include <soc/dpp/ARAD/arad_sim_em.h>
#include <soc/dpp/soc_dpp_arad_config_defs.h>
#include <soc/drv.h>

#ifdef CRASH_RECOVERY_SUPPORT
#include <soc/hwstate/hw_log.h>
#include <soc/dcmn/dcmn_crash_recovery.h>
#endif /* CRASH_RECOVERY_SUPPORT */


/* } */
/*************
 * DEFINES   *
 *************/
/* { */

#define ARAD_PP_LEM_ACCESS_CMD_MAX                              (ARAD_PP_LEM_ACCESS_NOF_CMDS-1)
#define ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_IPV4      (2)
#define ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_DBAL_IPV4 (3)
#define ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_ILM_MIN            (3)
#define ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_ILM_MAX            (4)

#define ARAD_PP_LEM_STAMP_MIN                                   (10)
#define ARAD_PP_LEM_STAMP_MAX                                   (250)
#define ARAD_PP_LEM_STAMP_INVALID                               (0)


/* Payload destination parameter sizes in bits */

/* Polling before writing a CPU request */

/* Field values */
#define ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU_FLD_VAL              (0)
#define ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OLP_FLD_VAL              (1)

#define ARAD_PP_LEM_ACCESS_FAIL_REASON_FID_LIMIT_FLD_VAL        (0)
#define ARAD_PP_LEM_ACCESS_FAIL_REASON_EMC_PROBLEM_FLD_VAL      (1)

/* extend P2P */
#define ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_EXTEND_P2P       (1)

   /*
     * Extended
     */
#define ARAD_PP_L2_LIF_KEY_TYPE_EXTENDED_PARAM0_NOF_BITS                  (34)

#define SOC_PPC_FRWRD_MACT_ENTRY_KEY_AGE_STATUS_MAX              (6)


/* define for LEM payload formatting*/

#define ARAD_PP_LEM_EEI_NOF_BITS (24)
#define ARAD_PP_LEM_LIF_VALID_NOF_BITS (1)
#define ARAD_PP_LEM_SA_DROP_LSB (SOC_DPP_DEFS_GET(unit, lem_sa_drop_lsb))
#define ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB_ARAD (40)
#define ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB (SOC_IS_JERICHO(unit)?42:ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB_ARAD)
#define ARAD_PP_LEM_ENTRY_TYPE_IS_FEC_EEI_LSB (SOC_DPP_DEFS_GET(unit, lem_entry_type_is_fec_eei_lsb))

#define ARAD_PP_LEM_HAS_CW_LSB (SOC_IS_JERICHO(unit)?40:38)
#define ARAD_PP_LEM_TPID_PROFILE_LSB (SOC_IS_JERICHO(unit)?38:36)
#define ARAD_PP_LEM_LIF_VALID_LSB(_unit) (SOC_IS_JERICHO(_unit)?37:35)
#define ARAD_PP_LEM_LIF_VALUE_LSB (19)
#define ARAD_PP_LEM_FEC_VALUE_LSB (19)

#define ARAD_PP_LEM_EEI_FEC_VALUE_LSB(unit) SOC_DPP_DEFS_GET(unit, fec_nof_bits)


#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_LAG_LSB                    0
#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_LAG_NOF_BITS               15
#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_FEC_LSB                    ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_LAG_NOF_BITS
#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_FEC_NOF_BITS               15
#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_ENTRY_FORMAT_LSB           38
#define ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_ENTRY_FORMAT_NOF_BITS      2


/* format 3b (in arad+): entry_type (1b'1') native_vsi (12b) HI(4) arp_pointer/eei (14) fec_ptr (12)
 *           (in jer):   entry_type (1b'1') native_vsi (13b) HI(4) arp_pointer/eei (15) fec_ptr (12) 
 */

#define ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_DST_NOF_BITS        (32)
#define ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_ASD_NOF_BITS        (16) 
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_FEC_NOF_BITS         (12) /* format 3b: fec is truncated */
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_DEST_NOF_BITS        (ARAD_PP_LEM_3B_PAYLOAD_FORMAT_FEC_NOF_BITS) /* format 3b: dest is truncated (since fec is truncated) */
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_NOF_BITS          (4)  /* format 3b: HI(4) */
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_LSB(unit)         (SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_eei_nof_bits) + ARAD_PP_LEM_3B_PAYLOAD_FORMAT_FEC_NOF_BITS)  /* format 3b: HI(4) */
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_NATIVE_VSI_LSB(unit) (ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_NOF_BITS + ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_LSB(unit)) /* format 3b in arad+: native_vsi(12) */
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_LSB(unit)  (SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_native_vsi_nof_bits) + ARAD_PP_LEM_3B_PAYLOAD_FORMAT_NATIVE_VSI_LSB(unit))  
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_NOF_BITS (1) 
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_VALUE    (1) 



#define ARAD_PP_LEM_NOF_ANSER_RECV_ITERATIONS (5)

#define ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT (16)

#define ARAD_PP_LEM_CR_SHADOW_DELETED_ENTRY 2

/* } */
/*************
 * MACROS    *
 *************/
/* { */

/* For roo application, the eei/arp pointer/outlif encoded in the host table payload is truncated.
 * The user who configures the eei pass the entire eei.
 * 
 * Eei to host table payload eei: 
 * we need to truncate eei for the host table payload
 * In the system header, the eei is rebuilt from the truncated eei and the register MactFormat3EeiBits: 
 * 11 MactFormat3EeiBits[2:1] HI(4) EEI(15) MactFormat3EeiBits[0]
 * How we truncate the eei passed by the user: 
 * lsb is truncated 
 * msbs are truncated, nof bits (arad+: 14b, jer: 15b)
 * 
 * 
 * host table payload eei to eei
 * we need to build the eei from the truncated eei and the register.
 * 
 * 
 */      
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_SHIFT      (1)
#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_MASK(unit) ((1 << SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_eei_nof_bits)) -1)

#define ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_TO_TRUNCATED_EEI(unit, truncated_outlif, outlif) \
        ((truncated_outlif) = (((outlif)  >> ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_SHIFT) & ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_MASK(unit)))



#define ARAD_PP_LEM_3B_PAYLOAD_TRUNCATED_EEI_TO_EEI(unit, outlif, truncated_outlif) \
       ((outlif) |= ((truncated_outlif & ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_MASK(unit)) << ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_SHIFT))


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
  Arad_pp_procedure_desc_element_lem_access[] =
{
  /*
   * Auto generated. Do not edit following section {
   */
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_GET_PROCS_PTR),
  SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_GET_ERRS_PTR),

  /*
   * } Auto generated. Do not edit previous section.
   */

   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_KEY_ENCODED_BUILD),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_KEY_ENCODED_PARSE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_HW_PAYLOAD_BUILD),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_HW_PAYLOAD_PARSE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_REQUEST_SEND),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_REQUEST_ANSWER_RECEIVE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_ENTRY_ADD_UNSAFE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_ENTRY_REMOVE_UNSAFE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_ENTRY_BY_KEY_GET_UNSAFE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_ENTRY_BY_INDEX_GET_UNSAFE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_AGE_FLD_SET),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_AGE_FLD_GET),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_SA_BASED_ASD_BUILD),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ACCESS_SA_BASED_ASD_PARSE),
   SOC_PROCEDURE_DESC_ELEMENT_DEF(ARAD_PP_LEM_ILM_KEY_BUILD_SET),

   /*
   * Last element. Do no touch.
   */
  SOC_PROCEDURE_DESC_ELEMENT_DEF_LAST
};

CONST STATIC SOC_ERROR_DESC_ELEMENT
  Arad_pp_error_desc_element_lem_access[] =
{
  /*
   * Auto generated. Do not edit following section {
   */
  /*
   * } Auto generated. Do not edit previous section.
   */

  {
    ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR,
    "ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR",
    "In the key construction, a parameter has a size out of range. \n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_OUT_OF_RANGE_ERR,
    "ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_OUT_OF_RANGE_ERR",
    "In the key construction, the sum of the parameter size \n\r "
    "with the prefix size is greater than the maximal key size. \n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_UNKNOWN_KEY_PREFIX_ERR,
    "ARAD_PP_LEM_ACCESS_UNKNOWN_KEY_PREFIX_ERR",
    "The retrieved key has an unknown key prefix. \n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
    {
      ARAD_PP_LEM_ENTRY_INDEX_OUT_OF_RANGE_ERR,
    "ARAD_PP_LEM_ENTRY_INDEX_OUT_OF_RANGE_ERR",
    "'entry_ndx' is out of range. \n\r "
    "The range is: 0 to 64K+32-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR",
    "'command' is out of range. \n\r "
    "The range is: 0 to ARAD_PP_LEM_ACCESS_NOF_CMDS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_STAMP_OUT_OF_RANGE_ERR,
    "ARAD_PP_LEM_STAMP_OUT_OF_RANGE_ERR",
    "'stamp' is out of range. \n\r "
    "The stamp must be not set (set to zero).\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR",
    "'type' is out of range. \n\r "
    "The range is: 0 to ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR",
    "'nof_params' is out of range. \n\r "
    "The range is set according to the type.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR",
    "'nof_bits' is out of range. \n\r "
    "The range is set according to the type.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR",
    "'prefix' is out of range. \n\r "
    "The range is set according to the type.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR",
    "'nof_bits' is out of range. \n\r "
    "The range is set according to the type.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR",
    "'value' is out of range. \n\r "
    "The range is set according to the type.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ASD_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ASD_OUT_OF_RANGE_ERR",
    "'asd' is out of range. \n\r "
    "The range is: 0 to 1<<24-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_AGE_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_AGE_OUT_OF_RANGE_ERR",
    "'age' is out of range. \n\r "
    "The range is: 0 to 7.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_DEST_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_DEST_OUT_OF_RANGE_ERR",
    "'dest' is out of range. \n\r "
    "The range is: 0 to 1<<16-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
    {
      ARAD_PP_LEM_ACCESS_LOOKUP_POLL_TIMEOUT_ERR,
    "ARAD_PP_LEM_ACCESS_LOOKUP_POLL_TIMEOUT_ERR",
    "The polling on the bit indicating \n\r "
    "the end of the lookup has failed.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OUT_OF_RANGE_ERR",
    "'req_origin' is out of range. \n\r "
    "The range is: 0 to ARAD_PP_LEM_ACCCESS_NOF_REQ_ORIGINS-1.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_FAIL_REASON_OUT_OF_RANGE_ERR,
      "ARAD_PP_LEM_ACCESS_FAIL_REASON_OUT_OF_RANGE_ERR",
    "'reason' is out of range. \n\r "
    "The range is: 0,1 and 3.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_SA_BASED_ILLEGAL_VID_ERR,
    "ARAD_PP_LEM_ACCESS_SA_BASED_ILLEGAL_VID_ERR",
    " try to build ASD according to both SA-auth \n\r "
    " and VID-assign while VID value is not equal in both.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },
  {
    ARAD_PP_LEM_ACCESS_PROGRAM_NOT_FOUND,
    "ARAD_PP_LEM_ACCESS_PROGRAM_NOT_FOUND",
    " the requested FLP program was not found\n\r "
    " in the FLP key program map.\n\r ",
    SOC_SAND_SVR_ERR,
    FALSE
  },



  /*
   * Last element. Do no touch.
   */
SOC_ERR_DESC_ELEMENT_DEF_LAST
};

uint32
  Arad_pp_lem_actual_stamp[SOC_SAND_MAX_DEVICE] = {ARAD_PP_LEM_STAMP_MIN};

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */

  const char *
    arad_pp_lem_access_app_id_to_app_name(
                                                         SOC_SAND_IN  int    unit,
                                                         SOC_SAND_IN  uint32 app_id)
{
  const char* str = NULL;

  switch(app_id)                               
  {                                              
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_MC:
    str = "TRILL_MC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_COMP:
    str = "IPV4_COMP";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_MAC_IN_MAC_TUNNEL:
    str = "MAC_IN_MAC_TUNNEL";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_EXTEND:
    str = "EXTEND";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_UC:
    str = "TRILL_UC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_HOST:
    str = "IP_HOST";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ILM:
    str = "ILM";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SA_AUTH:
    str = "SA_AUTH";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_SPOOF_DHCP:
    str = "IP_SPOOF_DHCP";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_SPOOF_STATIC:
    str = "IPV4_SPOOF_STATIC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_GLOBAL_IPV4_COMP:
    str = "GLOBAL_IPV4_COMP";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL:
    str = "FC_LOCAL";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL_N_PORT:
      str = "FC_LOCAL_N_PORT";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_REMOTE:
    str = "FC_REMOTE";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_ZONING:
    str = "FC_ZONING";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPMC_BIDIR:
    str = "IPMC_BIDIR";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_OMAC_2_VMAC:
    str = "OMAC_2_VMAC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_VMAC:
    str = "VMAC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_VMAC_2_OMAC:
    str = "VMAC_2_OMAC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_ADJ:
    str = "TRILL_ADJ";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SLB:
    str = "SLB";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP6_SPOOF_STATIC:
    str = "IP6_SPOOF_STATIC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_DIP6_COMPRESSION:
    str = "DIP6_COMPRESSION";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BFD_SINGLE_HOP:
      str = "BFD-SINGLE_HOPE";
      break;
  default:
    if (app_id == ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BMAC(unit)) {
      str = "BMAC";
    }
    else if (app_id == ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ETH(unit)) {
      str = "ETH_MAC";
    }
    else {
      str = " Unknown";
    }
  }                                              

  return str;
}

uint32
    arad_pp_lem_access_app_to_prefix_get(
       SOC_SAND_IN  int                        unit,
       SOC_SAND_IN  uint32                        app_id,
       SOC_SAND_OUT  uint32                        *prefix)
{
    uint8
      cur_app_id;
    uint32
      res;
    uint32 
      indx;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    *prefix = ARAD_PP_FLP_MAP_PROG_NOT_SET;

    for (indx = 0; indx < ARAD_PP_LEM_ACCESS_NOF_PREFIXES; ++indx) {
        res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.get(unit,indx,&cur_app_id);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 10, exit);
        if (app_id == cur_app_id) {
            *prefix = indx;
            goto exit;
        }
    }
exit:
 SOC_SAND_EXIT_AND_SEND_ERROR("arad_pp_lem_access_app_to_prefix_get", 0, 0);
}

uint32
    arad_pp_lem_access_prefix_to_app_get(
       SOC_SAND_IN  int                        unit,
       SOC_SAND_IN  uint32                        key_type_msb,
       SOC_SAND_OUT  uint32                        *app_type)
{
    uint8
      cur_app_id;
    uint32
      res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* remove */
    res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.get(unit,key_type_msb,&cur_app_id);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 10, exit);

    if (cur_app_id == ARAD_PP_FLP_MAP_PROG_NOT_SET) {
        *app_type = (uint32)(-1);
        SOC_SAND_SET_ERROR_CODE(SOC_PPD_ERR_NOT_EXIST, 20, exit);
    }

    *app_type = cur_app_id;
exit:
 SOC_SAND_EXIT_AND_SEND_ERROR("arad_pp_lem_access_prefix_to_app_get", 0, 0);

}

uint32
    arad_pp_lem_access_prefix_dealloc(
       SOC_SAND_IN  int                        unit,
       SOC_SAND_IN  uint32                     key_type_msb)
{
    uint32 res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.set(unit,key_type_msb, ARAD_PP_FLP_MAP_PROG_NOT_SET);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 10, exit);    
    
exit:
 SOC_SAND_EXIT_AND_SEND_ERROR("arad_pp_lem_access_prefix_dealloc", 0, 0);

}


uint32
    arad_pp_lem_access_prefix_alloc(
       SOC_SAND_IN  int                        unit,
       SOC_SAND_IN  uint32                        flags,
       SOC_SAND_IN  uint32                        app_id,
       SOC_SAND_IN  uint32                        key_type_msb_nof,
       SOC_SAND_INOUT  uint32                     *key_type_msb
    )
{

    uint8
      cur_app_id;
    uint32
      res;
    uint32 
      indx;
    uint32
      key_type_msb_lcl =  ARAD_PP_LEM_ACCESS_NOF_PREFIXES; 
    uint32 key_type_msb_tmp = 0;
    uint32 i = 0;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    SOC_SAND_CHECK_NULL_INPUT(key_type_msb);    

    /* prefix given */
    if (flags & ARAD_PP_LEM_ACCESS_PREFIX_ALLOC_WITH_ID) {
        key_type_msb_lcl = *key_type_msb;
        for (i = 0; i < key_type_msb_nof; i++) {
            key_type_msb_tmp = (*key_type_msb + i);
            if (key_type_msb_tmp >= ARAD_PP_LEM_ACCESS_NOF_PREFIXES) {
                SOC_SAND_SET_ERROR_CODE(SOC_PPD_ERR_OUT_OF_RANGE_ERR, 10, exit);
            }

            res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.get(unit, key_type_msb_tmp, &cur_app_id);
            SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 20, exit);

            /* already in use */
            if (cur_app_id != ARAD_PP_FLP_MAP_PROG_NOT_SET) {
                LOG_CLI((BSL_META_U(unit,
                                    "Allocate a prefix for %s is failed because %s had token prefix %u\n"), 
                         arad_pp_lem_access_app_id_to_app_name(unit, app_id), 
                         arad_pp_lem_access_app_id_to_app_name(unit, cur_app_id),
                         key_type_msb_tmp));
                key_type_msb_lcl = ARAD_PP_LEM_ACCESS_NOF_PREFIXES;
                break;
            }
        }
    }
    else {
        for (indx = 0; indx < ARAD_PP_LEM_ACCESS_NOF_PREFIXES; ++indx) {
            res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.get(unit,indx,&cur_app_id);
            SOC_SAND_SOC_CHECK_FUNC_RESULT(res,30, exit);
            if (cur_app_id == ARAD_PP_FLP_MAP_PROG_NOT_SET) {
                key_type_msb_lcl = indx; /* found unsed prefix use it */
                break;
            }
        }
    }

    if (key_type_msb_lcl == ARAD_PP_LEM_ACCESS_NOF_PREFIXES) {
        SOC_SAND_SET_ERROR_CODE(SOC_PPD_ERR_NOT_EXIST, 40, exit);
    }

    /* set as used */
    for ( i = 0; i < key_type_msb_nof; i++) {
        res = sw_state_access[unit].dpp.soc.arad.pp.fec.lem_prefix_mapping.set(unit, (key_type_msb_lcl + i) ,app_id);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 40, exit);
    }
    
    *key_type_msb = key_type_msb_lcl;

exit:
 SOC_SAND_EXIT_AND_SEND_ERROR("arad_pp_lem_access_prefix_alloc", 0, 0);
}


uint32
  arad_pp_lem_access_init_static_prefix(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    flags=ARAD_PP_LEM_ACCESS_PREFIX_ALLOC_WITH_ID,
    prefix;
  uint8
    app_id,
    trill_mode,
    enabled = 0,
    authentication_enable;
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  trill_mode = soc_property_get(unit,spn_TRILL_MODE, 0);
  

  if (trill_mode) {
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_MC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 11, exit);
  }

  arad_pp_is_mac_in_mac_enabled(unit,&enabled);

  if (enabled) {
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BMAC(unit);
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);
  }

  prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_COMP;
  res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
  SOC_SAND_CHECK_FUNC_RESULT(res, 13, exit);

  if (enabled) {
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_MAC_IN_MAC_TUNNEL;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 14, exit);
  }

  prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_EXTEND;
  res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
  SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);

  if (trill_mode) {
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_UC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 16, exit);
  }

  prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_HOST;
  res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
  SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);

  prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ILM;
  res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
  SOC_SAND_CHECK_FUNC_RESULT(res, 18, exit);
  if (SOC_IS_JERICHO(unit)) {
     prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_LSR_CNT;
     res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
     SOC_SAND_CHECK_FUNC_RESULT(res, 26, exit);
  }

  res = sw_state_access[unit].dpp.soc.arad.pp.oper_mode.authentication_enable.get(unit, &authentication_enable);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 20, exit);

  if (authentication_enable) {
        prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SA_AUTH;
        res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }

  prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ETH(unit);
  res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
  SOC_SAND_CHECK_FUNC_RESULT(res, 21, exit);

  if (SOC_DPP_CONFIG(unit)->pp.l3_source_bind_mode) {
    /* the value shoud be 8 because prefix is 1xxx, but in old codes it's 1.
       * Let keep the value is 1 now, and update and check in another task.
       */
    if (SOC_DPP_CONFIG(unit)->pp.compression_spoof_ip6_enable) {
      prefix = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_SPOOF_DHCP;
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP6_SPOOF_STATIC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 22, exit);
    } else {
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_SPOOF_DHCP;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 22, exit);
    }

    prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_SPOOF_STATIC;
    res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
    SOC_SAND_CHECK_FUNC_RESULT(res, 23, exit);
  }
  if (!SOC_DPP_CONFIG(unit)->l3.ipmc_vpn_lookup_enable) {
    app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_GLOBAL_IPV4_COMP;
    /* 11xx, take 4 prefix, from 12 to 15 */
    prefix = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_GLOBAL_IPV4_COMP_VAL;
    res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_4,&prefix);
    SOC_SAND_CHECK_FUNC_RESULT(res, 24, exit);
  }

  if (SOC_IS_JERICHO(unit)) {
      /* 
       * In Jericho we need to allocate a prefix for SLB regardless of resilient hashing mode.
       * Furthermore, it should have 4 msb different from the 4 msb of the MACT prefix because these
       * bits are used in LEM for learning and we don't want the MACT to translate the consistent hashing 
       * format when doing learning. 
       */ 
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SLB;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 25, exit);

      /* 
       * MACT limit enforcement is done according to the prefixes of the entries. 
       * In general, MACT entries prefix is 0x0 (or 0x1 in ARAD_A0 and below), but the MACT limit enforcement 
       * mechanism is considering only the 4 msb (out of 5 bit prefix). 
       * The result is that both 0x0 and 0x1 prefix values are considered as MACT entries for MACT limit mechanism. 
       * This WA is a dummy allocation of the non-MACT prefix, in order to prevent it from being allocated to an 
       * actual logical table in the LEM. This way, no entry will be added to the LEM using this prefix. 
       */ 
      prefix = app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_DUMMY(unit);
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 26, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_init_static_prefix()", 0, 0);
}

uint32
  arad_pp_lem_access_init_dynamic_prefix(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    flags=0,
    prefix;
  uint8
    app_id;
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  
  if(SOC_DPP_CONFIG(unit)->pp.fcoe_enable){
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_REMOTE;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 11, exit);

      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_ZONING;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);

      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL_N_PORT;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 13, exit);
  }

  if(SOC_DPP_CONFIG(unit)->l3.nof_rps != 0){
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPMC_BIDIR;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 13, exit);
  }

  if (SOC_DPP_CONFIG(unit)->pp.vmac_enable) {
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_OMAC_2_VMAC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 14, exit);

      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_VMAC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);

      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_VMAC_2_OMAC;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 16, exit);
  }
  if (SOC_DPP_CONFIG(unit)->trill.mode) {
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_ADJ;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);
  }

  if (SOC_DPP_CONFIG(unit)->pp.bfd_ipv4_single_hop || 
      (SOC_DPP_CONFIG(unit)->pp.bfd_ipv6_enable==SOC_DPP_ARAD_BFD_IPV6_SUPPORT_WITH_LEM)) { 
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);
  }

  if (SOC_IS_ARADPLUS_A0(unit) && SOC_DPP_CONFIG(unit)->pp.oam_statistics) {
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_STATISTICS;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);
  }

  /* bfd echo- clasification in LEM*/
  if (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "bfd_echo_with_lem", 0)) {
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_ECHO;
      res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);
  }

#ifdef BCM_88660_A0
  if (SOC_IS_ARADPLUS_A0(unit) && (soc_property_get(unit, spn_RESILIENT_HASH_ENABLE, 0) == 1)) {
      /* In case of ARAD_PLUS, SLB prefix was not allocated statically. Allocate it dynamically. */
      app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SLB;
      res = arad_pp_lem_access_prefix_alloc(unit,flags,app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
      SOC_SAND_CHECK_FUNC_RESULT(res, 18, exit);
  }
#endif /* BCM_88660_A0 */

  if (SOC_DPP_CONFIG(unit)->pp.compression_ip6_enable) {
    app_id = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_DIP6_COMPRESSION;
    res = arad_pp_lem_access_prefix_alloc(unit,flags, app_id,ARAD_PP_LEM_ACCESS_PREFIX_NUM_1,&prefix);
    SOC_SAND_CHECK_FUNC_RESULT(res, 21, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_init_dynamic_prefix()", 0, 0);
}

uint32
  arad_pp_lem_access_init_unsafe(
    SOC_SAND_IN  int                                 unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  /* enable forwarding lookup for AC-MP PWE-MP, and ISID-VRL MP */

  /* allocate static perfix of "old" programs */
  res = arad_pp_lem_access_init_static_prefix(unit);
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  
  /* load rest according to soc property*/
  res = arad_pp_lem_access_init_dynamic_prefix(unit);
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  res = arad_pp_flp_init(unit,TRUE,FALSE,2);
  SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_init_unsafe()", 0, 0);
}

STATIC uint32
  arad_pp_lem_access_age_fld_get(
      SOC_SAND_IN  int  unit,
      SOC_SAND_IN  uint32   age_fld,
      SOC_SAND_OUT uint32   *age
    )
{
  uint32
    age_lcl = 0,
    age_without_aged_bit,
    age_3_bit,
    res;
  uint8
    is_age_resolution_low;
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_AGE_FLD_SET);

  SOC_SAND_CHECK_NULL_INPUT(age);  

  /* The msb that represents RBD in Jericho is not necessary here */
  age_3_bit = age_fld & 0x7;

  age_without_aged_bit = age_3_bit >> 1;

  res = arad_pp_frwrd_mact_is_age_resolution_low(unit, &is_age_resolution_low);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  
  if (is_age_resolution_low)
  {
    /*
     *  Low resolution: Age mapped to 0->0, 1->1, 2->2, 3->3, 4->0, 5->1, 6->2, 7->3
     */
      switch (age_without_aged_bit)
      {
      case 0:
      case 1:
      case 2:
      case 3:
          age_lcl = age_without_aged_bit;
          break;

      case 4:
      case 5:
      case 6:
      case 7:
          age_lcl = age_without_aged_bit - 4;
          break;

      default:
          SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_AGE_OUT_OF_RANGE_ERR, 20, exit);
      }
  }
  else
  {
    /*
     *  High resolution: Age mapped to 0->0, 1->1, 2->2, 3->6, 4->3, 5->4, 6->5, 7->6
     */
    switch (age_3_bit)
      {
      case 0:
      case 1:
      case 2:
          age_lcl = age_3_bit;
          break;

      case 4:
      case 5:
      case 6:      
          age_lcl = age_3_bit - 1;
          break;

      case 3:
      case 7:      
          age_lcl = 6;
          break;
      /* No need. It's a dead code 
      default:
          SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_AGE_OUT_OF_RANGE_ERR, 20, exit);
      */
      }
    }

  *age = age_lcl;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_age_fld_set()", age_fld, 0);
}

STATIC uint32
  arad_pp_lem_access_actual_stamp_get(
      SOC_SAND_IN int  unit
    )
{
  return Arad_pp_lem_actual_stamp[unit];
}

uint32
  arad_pp_lem_access_next_stamp_get(
      SOC_SAND_IN int  unit,
      SOC_SAND_OUT uint32 *next_stamp
    )
{
    uint32
        reg_val,
        res = SOC_SAND_OK;
    
    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_NEXT_STAMP_GET);

    SOC_SAND_CHECK_NULL_INPUT(next_stamp);
 /*
  * stamp is needed to get reply from EMC, 
  * when stamp !=0 then MACT return reply with operation status 
  * in normal process, for adding/remove entry stamp != 0 for success reply. 
  *  
  * However when FLUSH db is active and returns data to reply-FIFO 
  * we need to set stamp to zero inorder not to generate reply-FIFO, 
  * as it will collide with flush db replies. 
  */

  /* check if there already entries in reply fifo if so don't ask for reply */  
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
{
    soc_reg_above_64_val_t data;    
    res = ARAD_FAST_REGISER_GET(ARAD_FAST_REG_IHP_MACT_REPLY_FIFO_REPLY_FIFO_ENTRY_COUNT, data);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 18, exit);
    reg_val = data[0];
}    
#else
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, READ_PPDB_B_LARGE_EM_REPLY_FIFO_REPLY_FIFO_ENTRY_COUNTr(unit, &reg_val));
#endif
  if(reg_val > 0) {
      *next_stamp = ARAD_PP_LEM_STAMP_INVALID;
      ARAD_DO_NOTHING_AND_EXIT;
  }

  Arad_pp_lem_actual_stamp[unit]++;
  if (Arad_pp_lem_actual_stamp[unit] == ARAD_PP_LEM_STAMP_MAX)
  {
    Arad_pp_lem_actual_stamp[unit] = ARAD_PP_LEM_STAMP_MIN;
  }
  *next_stamp = Arad_pp_lem_actual_stamp[unit];
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_next_stamp_get()", 0, 0);
}
     
/* Check only if the entry type is 3b.
 * We only check the MSB, so the caller of this function must ensure he is getting host entries
   Return unknown if the entry type is not format 3b. */
uint32
  arad_pp_lem_access_entry_type_format3b_get(
      SOC_SAND_IN   int                       unit, 
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],
      SOC_SAND_OUT  uint32                    *entry_type
      ) 
{
    uint32
        tmp = 0;
    uint32
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0); 
  
    /* check format 3b entry type bit */
    res = soc_sand_bitstream_get_any_field(
       payload_data,
       ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_LSB(unit),
       ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_NOF_BITS,
       &(tmp)
    );

    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    if(tmp == 1) 
    {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B;
        goto exit;
    } 
    else 
    {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_UNKNOWN;
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_entry_type_format3b_get()", 0, 0); 
}



/* Get LEM payload from format 1 and check the prefix in order to decide which type it is (Out-LIF, EEI FEC-Ptr, EEI MC-ID) */
uint32
  arad_pp_lem_access_entry_type_get(
      SOC_SAND_IN   int                       unit, 
      SOC_SAND_IN   uint32                    *payload_data,
      SOC_SAND_IN   uint8                     is_learn_data,
      SOC_SAND_OUT  uint32                    *entry_type
    )
{
    uint32
        tmp = 0;
    uint32
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    res = soc_sand_bitstream_get_any_field(
        payload_data,
        is_learn_data ? ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB_ARAD : ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB,
        1,
        &(tmp)
      );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if(tmp == 1) 
    {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_EEI_MC;
        goto exit;
    }
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        /* In case of learn data the format stays as in Arad */
        is_learn_data ? SOC_DPP_LEM_ENTRY_TYPE_IS_FEC_EEI_LSB_ARAD : ARAD_PP_LEM_ENTRY_TYPE_IS_FEC_EEI_LSB,
        2,
        &(tmp)
      );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if(tmp == 1)
    {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_EEI_FEC;
        goto exit;
    }

    *entry_type = ARAD_PP_LEM_ENTRY_TYPE_OUTLIF;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_entry_type_get()", payload_data[0], payload_data[1]);
}


/*
 * set entry type bits in lem payload according to entry type
 * Entry type length is up to 2 bit length. 
 */
uint32
  arad_pp_lem_access_entry_type_set(
      SOC_SAND_IN   int                       unit,
      SOC_SAND_IN  uint32                    entry_type,
      SOC_SAND_IN  uint8                     is_learn_data,
      SOC_SAND_OUT   uint32                  payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S]
    )
{
    uint32
        tmp = 0;
    uint32
        res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* if EEI and MC set one bit */
    if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_EEI_MC)
    {
        tmp = 1;
        res = soc_sand_bitstream_set_any_field(
            &(tmp),
            ARAD_PP_LEM_ENTRY_TYPE_IS_MC_EEI_LSB,
            1,
            payload_data
            );
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    } 
    /* For format 3b, entry_type is msb of payload data, and entry_type=1
     * jericho payload format is lightly different from arad format.
     * The payload length changes: jer payload length: 44. arad payload length: 42. 
     * Consequently: in jer: entry_type is bit 44. Arad+: entry_type is bit 42.
     */
    else if (entry_type == ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) 
    {
       tmp = ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_VALUE;
       res = soc_sand_bitstream_set_any_field(
               &tmp,
               ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_LSB(unit),
               ARAD_PP_LEM_3B_PAYLOAD_FORMAT_ENTRY_TYPE_NOF_BITS,
               payload_data
             );
       SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    }
    /* set 2 bits entry type.
       format 3 and format 3b (native-VSI, HI, outlif, fec-ptr) don't have entry type */
    else if(entry_type != ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3)
    {
        res = soc_sand_bitstream_set_any_field(
                        &(entry_type),
                        /* In case of learn data the format stays as in Arad */
                        is_learn_data ? SOC_DPP_LEM_ENTRY_TYPE_IS_FEC_EEI_LSB_ARAD : ARAD_PP_LEM_ENTRY_TYPE_IS_FEC_EEI_LSB,
                        2,
                        payload_data
                    );
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_entry_type_set()", payload_data[0], payload_data[1]);
}


/* get optional sa_drop bit from LEM payload */
uint32
arad_pp_lem_access_sa_drop_get(
   SOC_SAND_IN   int                         unit,
   SOC_SAND_IN   uint32                      payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],
   SOC_SAND_IN   uint32                      entry_type,
   SOC_SAND_IN   uint8                       is_learn,
   SOC_SAND_OUT  uint8                       *sa_drop
   ) {
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* 
     * get sa drop bit.
     * Except for format 3B (Native_VSI, HI, OutLif, FEC) which don't have drop bit. */
    if ((entry_type != ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) && (entry_type != ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA)) {
        uint32 res;
        uint32 sa_drop_32 = 0;

        if (is_learn == 1) 
        {
            res = soc_sand_bitstream_get_any_field(
                    payload_data,
                    SOC_DPP_LEM_SA_DROP_LSB_ARAD,
                    1,
                    &sa_drop_32
                  );
        }
        else
        {
            res = soc_sand_bitstream_get_any_field(
                    payload_data,
                    ARAD_PP_LEM_SA_DROP_LSB,
                    1,
                    &sa_drop_32
                  );
        }

        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

        *sa_drop = (uint8) sa_drop_32;
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sa_drop_get()", payload_data[0], payload_data[1]);
}


/* set optional sa_drop bit in LEM payload. */
uint32 
arad_pp_lem_access_sa_drop_set(
   SOC_SAND_IN  int                          unit,
   SOC_SAND_IN  uint32                       entry_type,
   SOC_SAND_IN  uint8                        sa_drop,
   SOC_SAND_OUT uint32                       payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S]) {

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* set sa drop bit in lem payload.
       Except for format 3B (Native_VSI, HI, OutLif, FEC) which don't have drop bit. */
    if ((entry_type != ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) && (entry_type != ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA)) {
        uint32 res;
        uint32 sa_drop_32 = sa_drop;
        res = soc_sand_bitstream_set_any_field(
                &sa_drop_32,
                ARAD_PP_LEM_SA_DROP_LSB,
                1,
                payload_data
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sa_drop_set()", payload_data[0], payload_data[1]);
}

/* get optional is_dynamic in LEM payload */
uint32
arad_pp_lem_access_is_dynamic_get(
   SOC_SAND_IN   int                         unit, 
   SOC_SAND_IN   uint32                      payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],
   SOC_SAND_IN  uint32                       entry_type,
   SOC_SAND_OUT  uint8                       *is_dynamic
   ) {
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* 
     * get is_dynamic bit.
     * Except for format 3B (Native_VSI, HI, OutLif, FEC) which don't have is_dynamic bit. */
    if ((entry_type != ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) && (entry_type != ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA)) {
        uint32 res;
        uint32 is_dynamic_32 = 0;
        res = soc_sand_bitstream_get_any_field(
                payload_data,
                SOC_DPP_DEFS_GET(unit, lem_is_dynamic_lsb),
                1,
                &is_dynamic_32
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

        *is_dynamic = (uint8) is_dynamic_32;
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_is_dynamic_get()", payload_data[0], payload_data[1]);
}

/* set optional is_dynamic in LEM payload */
uint32 
arad_pp_lem_access_is_dynamic_set(
   SOC_SAND_IN   int                         unit, 
   SOC_SAND_IN  uint32                       entry_type,
   SOC_SAND_IN  uint8                        is_dynamic,
   SOC_SAND_OUT uint32                       payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S]) {

   SOC_SAND_INIT_ERROR_DEFINITIONS(0);
   
    /* set is_dynamic bit in lem payload
       Except for format 3B (Native_VSI, HI, OutLif, FEC) which don't have is_dynamic bit. */
    if ((entry_type != ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) && (entry_type != ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA)) {
       uint32 res;
       uint32 is_dynamic_32 = is_dynamic;
       res = soc_sand_bitstream_set_any_field(
               &is_dynamic_32,
               SOC_DPP_DEFS_GET(unit, lem_is_dynamic_lsb),
               1,
               payload_data
             );
       SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_is_dynamic_set()", payload_data[0], payload_data[1]);

}



uint32 arad_pp_lem_access_get_entry_type(SOC_SAND_IN   int                          unit,
                                         SOC_SAND_IN   uint32                       *payload_data,
                                         SOC_SAND_IN   ARAD_PP_LEM_ACCESS_KEY_TYPE  key_type,
                                         SOC_SAND_IN   uint8                        is_learn,
                                         SOC_SAND_OUT  uint32                       *entry_type)
{
    uint32 res = SOC_SAND_OK;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    *entry_type = ARAD_PP_LEM_ENTRY_TYPE_UNKNOWN;

    /* First get some info from the key_type */
#ifdef BCM_88660_A0
    if (SOC_IS_ARADPLUS(unit) && (key_type == ARAD_PP_LEM_ACCESS_KEY_TYPE_SLB)) {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_SLB;
        SOC_SAND_EXIT_NO_ERROR;
    }
#endif /* BCM_88660_A0 */

    if ((key_type == ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH) || (key_type == ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ))
    {
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3;
    }
    /* set entry type as host format so we can check the msb for format 3b (for raw application) */
    else if (key_type == ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST)
    {
        res = arad_pp_lem_access_entry_type_format3b_get(
                unit,             
                payload_data,
                entry_type);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }     
    else if ((key_type >= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_BASE) && (key_type <= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_END))
    {
        /* when the table is dynamic the payload structure is unknown - let the driver understand the structure according to value */
        *entry_type = ARAD_PP_LEM_ENTRY_TYPE_UNKNOWN;
    }
    
    /* Use the payload to decide what is the entry_type */
    if (*entry_type == ARAD_PP_LEM_ENTRY_TYPE_UNKNOWN)
    {
        res = arad_pp_lem_access_entry_type_get(
              unit, 
              payload_data,
              is_learn,
              entry_type);
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    }

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_get_entry_type()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_slb(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload)
{
    const uint32 nof_fields = 3;
    unsigned int i;
    uint32 res;

    struct {
        uint8 lsb;
        uint8 len;
        uint32 value;
    } fields[] = {
        { ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_LAG_LSB, ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_LAG_NOF_BITS },
        { ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_FEC_LSB, ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_FEC_NOF_BITS },
        { ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_ENTRY_FORMAT_LSB, ARAD_PP_LEM_SLB_PAYLOAD_FORMAT_ENTRY_FORMAT_NOF_BITS },
    };

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    for (i = 0; i < nof_fields; i++) {
        res = soc_sand_bitstream_get_any_field(
              payload_data,
              fields[i].lsb,
              fields[i].len,
              &fields[i].value
            );
        SOC_SAND_CHECK_FUNC_RESULT(res, 33, exit);
    }

    payload->slb_lag = fields[0].value;
    payload->slb_fec = fields[1].value;
    payload->slb_lag_valid = fields[2].value & 1;
    payload->slb_fec_valid = (fields[2].value >> 1) & 1;

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in _arad_pp_lem_access_payload_parse_slb()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_outlif(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{
    uint32 tmp, outlif_valid;
    uint32 res;
    SOC_PPC_FEC_TYPE fec_type = SOC_PPC_NOF_FEC_TYPES_ARAD;
    uint8 is_destination_fec = 0,is_dest_mc_for_learn = 0,is_dest_sys_port_for_learn = 0;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
          payload_data,
          0,
          ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
          &(tmp)
        );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    payload->dest = tmp;

    /* read TPID and CW, anyway, can be TPID,CW or group otherwise just zero */
    /* TPID pofile */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_TPID_PROFILE_LSB,
        2,
        &(tmp)
    );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    payload->tpid_profile = (uint8)tmp;

    /* HAS cw */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_HAS_CW_LSB,
        1,
        &(tmp)
    );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    payload->has_cw = (uint8)tmp;

    /* check if outlif is valid */
    outlif_valid = 0;
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_LIF_VALID_LSB(unit) - ((SOC_IS_JERICHO(unit) && (payload->is_learn_data)) ? 2 : 0),
        ARAD_PP_LEM_LIF_VALID_NOF_BITS,
        &(outlif_valid)
        );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    /* In case of learn data the format stays as in Arad */
    if (SOC_IS_JERICHO(unit) && !payload->is_learn_data) {
        is_destination_fec = ((payload->dest >> SOC_DPP_DEFS_GET(unit, fec_nof_bits)) == 1);
    }
    else {
        is_destination_fec = ((payload->dest >> SOC_DPP_FEC_NOF_BITS_ARAD) == 4);
    }


    if (SOC_IS_JERICHO(unit) && payload->is_learn_data) {
        is_dest_mc_for_learn = ((payload->dest >> (ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_ENCODE_DEST_PREFIX_NOF_BITS_19_MC_ID_ARAD)) == 5);
        is_dest_sys_port_for_learn = ((payload->dest >> ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT) == 4);
    }
     tmp = payload->dest;
     payload->dest = 0;
      /*
       * Convert dest value to the device-specified decode
       * MC and sys port need special process for their
       * prefix decode is diffrent between ARAD and JER.
       */
     if (is_dest_mc_for_learn) {
         res = soc_sand_bitstream_set_any_field(
                       &tmp,
                      0,
                      ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_ENCODE_DEST_PREFIX_NOF_BITS_19_MC_ID_ARAD,
                      &( payload->dest)
                     );
         tmp = SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_mc_id);
         res = soc_sand_bitstream_set_any_field(
                       &tmp,
                      ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_DEFS_GET(unit, encode_dest_prefix_nof_bits_19_mc_id),
                     SOC_DPP_DEFS_GET(unit, encode_dest_prefix_nof_bits_19_mc_id) ,
                      &( payload->dest)
                    );
     } else if (is_dest_sys_port_for_learn) {
         res = soc_sand_bitstream_set_any_field(
                       &tmp,
                      0,
                      ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                      &( payload->dest)
                   );
         tmp = SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_sys_port);
         res = soc_sand_bitstream_set_any_field(
                       &tmp,
                      ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                      ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                      &( payload->dest)
                   );
    } else {
        res = soc_sand_bitstream_get_any_field(
               payload_data,
              0,
              ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
             &(payload->dest)
           );
   }



    if (is_destination_fec) {
        /* destination is FEC */
        res = sw_state_access[unit].dpp.soc.arad.pp.fec.fec_entry_type.get(unit, payload->dest & 0xffff, &fec_type);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 25, exit);
    }

    if(outlif_valid || fec_type == SOC_PPC_FEC_TYPE_FORWARD) 
    {   /* outlif value */
        if (SOC_IS_JERICHO(unit) && payload->is_learn_data) {
            /* asd[15:0] is in payload_data[34:19] */
            res = soc_sand_bitstream_get_any_field(
                  payload_data,
                  ARAD_PP_LEM_LIF_VALUE_LSB,
                  SOC_DPP_DEFS_GET(unit, out_lif_nof_bits)-2,
                  &(payload->asd)
                );
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
            /* asd[17:16] is in payload_data[37:36] - read it to tmp */
            res = soc_sand_bitstream_get_any_field(
                  payload_data,
                  ARAD_PP_LEM_LIF_VALUE_LSB + SOC_DPP_DEFS_GET(unit, out_lif_nof_bits)-1,
                  2,
                  &(tmp)
                );
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
            /* add asd 2MSB to payload->asd */
            res = soc_sand_bitstream_set_any_field(
                  &tmp,
                  SOC_DPP_DEFS_GET(unit, out_lif_nof_bits)-2,
                  2,
                  &(payload->asd)
                );
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

        }
        else {
            res = soc_sand_bitstream_get_any_field(
                  payload_data,
                  ARAD_PP_LEM_LIF_VALUE_LSB,
                  SOC_DPP_DEFS_GET(unit, out_lif_nof_bits),
                  &(payload->asd)
                );
            SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
        }
        if (outlif_valid) {
		    payload->flags = ARAD_PP_FWD_DECISION_PARSE_OUTLIF;
        } else {
            payload->flags = ARAD_PP_FWD_DECISION_PARSE_OUTLIF_INVALID;
        }

    }
    else {
        /* learn fec value */
        res = soc_sand_bitstream_get_any_field(
              payload_data,
              ARAD_PP_LEM_FEC_VALUE_LSB,
              payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
              &(payload->asd)
            );
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    }
    payload->flags |= ARAD_PP_FWD_DECISION_PARSE_DEST;

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in _arad_pp_lem_access_payload_parse_outlif()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_eei_fec(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{   
    uint32 tmp;
    uint32 res;        

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    payload->flags = ARAD_PP_FWD_DECISION_PARSE_EEI;

    /* FEC value */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            payload_data,
            0,
            payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
            &(tmp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    /* encode as FEC */
    payload->dest = ((SOC_IS_JERICHO(unit)?1:4) << SOC_DPP_DEFS_GET(unit, fec_nof_bits) )| tmp;

    /* EEI value */
    res = soc_sand_bitstream_get_any_field(
            payload_data,
            /*same value as ARAD_PP_LEM_EEI_FEC_VALUE_LSB(unit),*/
            /* In case of learn data the format stays as in Arad */
            payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
            ARAD_PP_LEM_EEI_NOF_BITS,
            &(payload->asd));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    payload->flags |= ARAD_PP_FWD_DECISION_PARSE_DEST;
    
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in _arad_pp_lem_access_payload_parse_eei_fec()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_eei_mc(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{   
    uint32 tmp;
    uint32 res;        

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    payload->flags = ARAD_PP_FWD_DECISION_PARSE_EEI;

    /* MC value */
    res = soc_sand_bitstream_get_any_field(
            payload_data,
            0,
            /* In case of learn data the format stays as in Arad */
            payload->is_learn_data ? SOC_DPP_MULTICAST_GROUP_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits),
            &(tmp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    /* encode as MC */
    if (payload->is_learn_data) 
    {
        payload->dest =  (SOC_DPP_ENCODE_DEST_PREFIX_VALUE_19_MC_ID_ARAD << SOC_DPP_MULTICAST_GROUP_NOF_BITS_ARAD )| tmp;
    }
    else
    {
        payload->dest =  (SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_mc_id) << SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits) )| tmp;
    }

    /* EEI value */
    res = soc_sand_bitstream_get_any_field(
            payload_data,
            /* In case of learn data the format stays as in Arad */
            payload->is_learn_data ? SOC_DPP_MULTICAST_GROUP_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits),
            ARAD_PP_LEM_EEI_NOF_BITS,
            &(payload->asd)
        );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    payload->flags |= ARAD_PP_FWD_DECISION_PARSE_DEST;

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in _arad_pp_lem_access_payload_parse_eei_mc()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_format_3(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{       
    uint32 res;        

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    payload->flags = ARAD_PP_FWD_DECISION_PARSE_FORMAT_3;

    res = soc_sand_bitstream_get_any_field(
            payload_data,
            0,
            ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
            &(payload->dest));
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

    res = soc_sand_bitstream_get_any_field(
            payload_data,
            ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
            ARAD_PP_LEM_ACCESS_ASD_NOF_BITS,
            &(payload->asd));
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_payload_parse_format_3()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */

/* format 3b: Native-VSI/outRIF + HI + EEI/ARP-pointer/outLIF + FEC
* used for roo application
* changes between arad+ and jericho:
* arad+: Native-VSI/outRIF (12) + HI (4) + EEI/ARP-pointer/outLIF (14) + FEC(12)
* jer:   Native-VSI/outRIF (13) + HI (4) + EEI/ARP-pointer/outLIF (15) + FEC(12)
*/
uint32 _arad_pp_lem_access_payload_parse_format_3b(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{       
    uint32 res, tmp;
    uint32 hi_value;
    uint32 truncated_eei;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    
    /* update flag */
    payload->flags |= ARAD_PP_FWD_DECISION_PARSE_FORMAT_3B;

    /* FEC value  */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            payload_data, 
            0, 
            ARAD_PP_LEM_3B_PAYLOAD_FORMAT_DEST_NOF_BITS, 
            &(tmp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    /* fec type */
    payload->dest =  ((SOC_IS_JERICHO(unit)?1:4) << SOC_DPP_DEFS_GET(unit, fec_nof_bits) )| tmp;

    /* EEI/ARP-pointer/outLIF  from additional info.
    * Additional info is encoded as eei: {EEI identifier[23:20], HI[19:16], out_lif[15:0]} */ 

    /* eei value */

    /* truncated eei */
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_3B_PAYLOAD_FORMAT_DEST_NOF_BITS, 
        SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_eei_nof_bits), 
        &(truncated_eei));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    /* hi */
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_LSB(unit), 
        ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_NOF_BITS, 
        &hi_value);

    /* set hi for eei encoding = 24 bits = {EEI identifier[23:20], HI[19:16], out_lif[15:0]} */
    ARAD_PP_LEM_ACCESS_HI_TO_EEI(hi_value, payload->asd);

    /* set truncated eei */
    ARAD_PP_LEM_3B_PAYLOAD_TRUNCATED_EEI_TO_EEI(unit, payload->asd, truncated_eei); 

    /* for jericho: eei identifier is 11 MactFormat3EeiBits[2:1].
     MactFormat3EeiBits[2:1] is configured by soc property */
    if (SOC_IS_JERICHO(unit)) {
        uint32  arp_msbs =  SOC_ROO_HOST_ARP_MSBS(unit); 
        payload->asd = (SOC_PPC_EEI_IDENTIFIER_ENCAP_VAL_2_MSBS << ARAD_PP_ASD_BIT_POSITION_IDENTIFIER_OUTLIF) 
                        |  (arp_msbs <<ARAD_PP_ASD_BIT_POSITION_IDENTIFIER) 
                        | payload->asd;
    } 
    /* for arad+, eei identifier is 1111 */
    else {
        payload->asd = (SOC_PPC_EEI_IDENTIFIER_ENCAP_VAL << ARAD_PP_ASD_BIT_POSITION_IDENTIFIER) | payload->asd;
    }

    /* native vsi */
    res = soc_sand_bitstream_get_any_field(
        payload_data,
        ARAD_PP_LEM_3B_PAYLOAD_FORMAT_NATIVE_VSI_LSB(unit), 
        SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_native_vsi_nof_bits), 
        &(payload->native_vsi));

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_payload_parse_format_3b()", 0, 0);
}

/* This is an internal function to reduce the size of arad_pp_lem_access_payload_parse. Please do not use it outside arad_pp_lem_access_payload_parse */
uint32 _arad_pp_lem_access_payload_parse_format_raw(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],      
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{   
    uint32 tmp;
    uint32 res;        

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    /* update flag */
    payload->flags |= ARAD_PP_FWD_DECISION_PARSE_RAW_DATA;

    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
        payload_data, 
        0, 
        ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_DST_NOF_BITS, 
        &(tmp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    payload->dest =  tmp;

    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            payload_data, 
            ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_DST_NOF_BITS, 
            ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_ASD_NOF_BITS, 
            &(tmp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

    payload->asd = tmp;

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in _arad_pp_lem_access_payload_parse_format_raw()", 0, 0);
}


uint32
  arad_pp_lem_access_payload_parse(
      SOC_SAND_IN   int                   unit,      
      SOC_SAND_IN   uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],
      SOC_SAND_IN   ARAD_PP_LEM_ACCESS_KEY_TYPE key_type,
      SOC_SAND_OUT  ARAD_PP_LEM_ACCESS_PAYLOAD   *payload
    )
{
  uint32 res, entry_type;  

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_HW_PAYLOAD_PARSE);

  SOC_SAND_CHECK_NULL_INPUT(payload);

#ifdef BCM_88660_A0
  if (SOC_IS_ARADPLUS(unit) && (key_type == ARAD_PP_LEM_ACCESS_KEY_TYPE_SLB)) {

    /* Parse SLB payload */
    _arad_pp_lem_access_payload_parse_slb(unit,payload_data,payload);

    SOC_SAND_EXIT_NO_ERROR;
  }
#endif /* BCM_88660_A0 */

  payload->flags = 0;

  /* Parse the entry type to understand the format of the payload */
  res = arad_pp_lem_access_get_entry_type(unit,
                                         payload_data,
                                         key_type,
                                         payload->is_learn_data,
                                         &entry_type);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
										 

  /* set optional SA drop */
  res = arad_pp_lem_access_sa_drop_get(
     unit,
     payload_data,
     entry_type,
     payload->is_learn_data,
     &(payload->sa_drop)
   ); 
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  /* In case of learning the dynamic bit is parsed before this place. In Jericho payload_data doesn't hold the dynamic bit. */
  if (payload->is_learn_data == 0)
  {
      /* get optional is dynamic */
      res = arad_pp_lem_access_is_dynamic_get(
         unit, 
         payload_data, 
         entry_type, 
         &(payload->is_dynamic)
       );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }


  if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_OUTLIF)
  {
      res = _arad_pp_lem_access_payload_parse_outlif(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  /* FEC and EEI in entry */
  else if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_EEI_FEC)
  {
      res = _arad_pp_lem_access_payload_parse_eei_fec(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  /* MC and EEI in entry */
  else if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_EEI_MC)
  {
      res = _arad_pp_lem_access_payload_parse_eei_mc(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  else if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3)
  {
      res = _arad_pp_lem_access_payload_parse_format_3(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  else if (entry_type == ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) {
      res = _arad_pp_lem_access_payload_parse_format_3b(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  else if (entry_type == ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA) {
      /* In this case the payload is not parsed. The first 32 bits are set to the dest and the next 16 bits are set to the asd */
      res = _arad_pp_lem_access_payload_parse_format_raw(unit,payload_data,payload);
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_payload_parse()", 0, 0);
}


/* 
 * Convert logical payload to physical payload. 
 */ 
uint32
  arad_pp_lem_access_payload_build(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_PAYLOAD  *payload,
    SOC_SAND_OUT uint32                    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S]
  )
{
  uint32
    payload_ndx;
  uint32
    tmp,
    entry_type=0;
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_HW_PAYLOAD_BUILD);

  SOC_SAND_CHECK_NULL_INPUT(payload_data);

  for (payload_ndx = 0; payload_ndx < ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S; ++payload_ndx)
  {
    payload_data[payload_ndx] = 0;
  }
  /* entry is of outlif type */
  if(payload->flags & ARAD_PP_FWD_DECISION_PARSE_OUTLIF || payload->flags == ARAD_PP_FWD_DECISION_PARSE_DEST || payload->flags & ARAD_PP_FWD_DECISION_PARSE_OUTLIF_INVALID)
  {
      entry_type = ARAD_PP_LEM_ENTRY_TYPE_OUTLIF;
  }
  /* entry is of EEI type */
  else if(payload->flags & ARAD_PP_FWD_DECISION_PARSE_EEI) {
      /* according to destination type */
      if(payload->dest >> SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits) == SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_mc_id)) {
          entry_type = ARAD_PP_LEM_ENTRY_TYPE_EEI_MC;
      }
      else
      {
          entry_type = ARAD_PP_LEM_ENTRY_TYPE_EEI_FEC;
      }
  }
  else if(payload->flags & ARAD_PP_FWD_DECISION_PARSE_FORMAT_3)
  {
      entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3;
  } 
  /* entry is of outLif, HI, native-VSI type */
  else if (payload->flags & ARAD_PP_FWD_DECISION_PARSE_FORMAT_3B)
  {
      entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B;
  }
  else if (payload->flags & ARAD_PP_FWD_DECISION_PARSE_RAW_DATA)
  {
      entry_type = ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA;
  }
  else
  {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ASD_OUT_OF_RANGE_ERR, 10, exit);
  }

  /* entry type (up to 2 bits) */
  res = arad_pp_lem_access_entry_type_set(
          unit, 
          entry_type,
          payload->is_learn_data,
          payload_data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  /* SA drop (optional 1 bit) */
  res = arad_pp_lem_access_sa_drop_set(
          unit,
          entry_type, 
          payload->sa_drop,
          payload_data
     );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
 
  /* is dynamic (optional 1 bit) */
  res = arad_pp_lem_access_is_dynamic_set(
          unit, 
          entry_type, 
          payload->is_dynamic, 
          payload_data
     );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  /* build payload according to the entry type */

  if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_OUTLIF)
  {
      /*
       * there is some difference for MC decode between ARAD and JER,
       * So for learn data,need to convert prefix type to ARAD decode.
       * Sys port need same process.
       */
      if ((SOC_IS_JERICHO(unit)) && (payload->is_learn_data)) {
          int is_sys_port,is_mc;
          tmp = payload->dest >> (ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_DEFS_GET(unit, encode_dest_prefix_nof_bits_19_mc_id));
          is_mc = (tmp == SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_mc_id))?1:0;
          tmp = payload->dest >> ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT;
          is_sys_port = (tmp == SOC_DPP_DEFS_GET(unit, encode_dest_prefix_value_19_sys_port))?1:0;
          if (is_mc) {
                res = soc_sand_bitstream_set_any_field(
                               &(payload->dest),
                               0,
                               ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_ENCODE_DEST_PREFIX_NOF_BITS_19_MC_ID_ARAD,
                               payload_data
                              );
                tmp = SOC_DPP_ENCODE_DEST_PREFIX_VALUE_19_MC_ID_ARAD;
                res = soc_sand_bitstream_set_any_field(
                               &tmp,
                               ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-SOC_DPP_ENCODE_DEST_PREFIX_NOF_BITS_19_MC_ID_ARAD,
                               SOC_DPP_ENCODE_DEST_PREFIX_NOF_BITS_19_MC_ID_ARAD,
                               payload_data
                              );
                 SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
          } else if(is_sys_port) {
                res = soc_sand_bitstream_set_any_field(
                               &(payload->dest),
                               0,
                               ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                               payload_data
                              );
                tmp = SOC_DPP_ENCODE_DEST_PREFIX_VALUE_19_SYS_PORT_ARAD;
                res = soc_sand_bitstream_set_any_field(
                               &tmp,
                               ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                               ARAD_PP_LEM_ACCESS_DEST_NOF_BITS-ARAD_PP_LEM_ACCESS_DEST_NOF_SYS_PORT,
                               payload_data
                              );
                 SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
          } else {
                  res = soc_sand_bitstream_set_any_field(
                             &(payload->dest),
                             0,
                             ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
                             payload_data
                            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
         }
     } else {
        res = soc_sand_bitstream_set_any_field(
                &(payload->dest),
                0,
                ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
                payload_data
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
     }
      /* write these anyawy, if not used then zero */
      /* TPID pofile */
      tmp = payload->tpid_profile;
      res = soc_sand_bitstream_set_any_field(
            &(tmp),
            ARAD_PP_LEM_TPID_PROFILE_LSB,
            2,
            payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* HAS cw */
      tmp = payload->has_cw;
      res = soc_sand_bitstream_set_any_field(
          &(tmp),
          ARAD_PP_LEM_HAS_CW_LSB,
          1,
          payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
      /* check if outlif is valid */
      tmp = (payload->flags & ARAD_PP_FWD_DECISION_PARSE_OUTLIF) ? 1 : 0;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              ARAD_PP_LEM_LIF_VALID_LSB(unit) - ((SOC_IS_JERICHO(unit) && (payload->is_learn_data)) ? 2 : 0),
              ARAD_PP_LEM_LIF_VALID_NOF_BITS,
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

      if(tmp || payload->flags & ARAD_PP_FWD_DECISION_PARSE_OUTLIF_INVALID) {
          /* outlif value */
          if (SOC_IS_JERICHO(unit) && (payload->is_learn_data)) {
              /* asd should be devided on payload_data[37:36] and payload_data[34:19] */

              /* outlif bits [15:0]  to payload_data[34:19]*/
              res = soc_sand_bitstream_set_any_field(
                      &(payload->asd),
                      ARAD_PP_LEM_LIF_VALUE_LSB,
                      SOC_DPP_DEFS_GET(unit, out_lif_nof_bits)-2,
                      payload_data
                   );
              SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

              /* outlif bits [17:16]  to payload_data[37:36] */
              tmp = (payload->asd) >> (SOC_DPP_DEFS_GET(unit, out_lif_nof_bits)-2);
              res = soc_sand_bitstream_set_any_field(
                      &(tmp),
                      ARAD_PP_LEM_LIF_VALUE_LSB + SOC_DPP_DEFS_GET(unit, out_lif_nof_bits) - 1,
                      2,
                      payload_data
                   );
              SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
          }
          else {
              res = soc_sand_bitstream_set_any_field(
                      &(payload->asd),
                      ARAD_PP_LEM_LIF_VALUE_LSB,
                      SOC_DPP_DEFS_GET(unit, out_lif_nof_bits),
                      payload_data
                   );
              SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
          }
      }
      else {
          /* learn fec value */
          res = soc_sand_bitstream_set_any_field(
                  &(payload->asd),
                  ARAD_PP_LEM_FEC_VALUE_LSB,
                  /* In case of learn data the format stays as in Arad */
                  payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
                  payload_data
               );
          SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      }

  }
  /* FEC and EEI in entry */
  if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_EEI_FEC)
  {
      /* FEC value, set only 15 bits (Arad) or 17 bits (Jericho), i.e without type */
      tmp = payload->dest;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              0,
              /* In case of learn data the format stays as in Arad */
              payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* EEI value */
      res = soc_sand_bitstream_set_any_field(
              &(payload->asd),
              /*same value as ARAD_PP_LEM_EEI_FEC_VALUE_LSB(unit),*/
              /* In case of learn data the format stays as in Arad */
              payload->is_learn_data ? SOC_DPP_FEC_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, fec_nof_bits),
              ARAD_PP_LEM_EEI_NOF_BITS,
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  /* MC and EEI in entry */
  else if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_EEI_MC)
  {
      /* MC value, set only 16 bits, i.e. without type */
      tmp = payload->dest;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              0,
			   /* In case of learn data the format stays as in Arad */
              payload->is_learn_data ? SOC_DPP_MULTICAST_GROUP_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits),
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* EEI value */
      res = soc_sand_bitstream_set_any_field(
              &(payload->asd),
			   /* In case of learn data the format stays as in Arad */
              payload->is_learn_data ? SOC_DPP_MULTICAST_GROUP_NOF_BITS_ARAD : SOC_DPP_DEFS_GET(unit, multicast_group_nof_bits),
              ARAD_PP_LEM_EEI_NOF_BITS,
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }
  else if(entry_type == ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3)
  {
     res = soc_sand_bitstream_set_any_field(
              &(payload->dest),
              0,
              ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 35, exit);

      res = soc_sand_bitstream_set_any_field(
              &(payload->asd),
              ARAD_PP_LEM_ACCESS_DEST_NOF_BITS,
              ARAD_PP_LEM_ACCESS_ASD_NOF_BITS,
              payload_data
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
  }


  /* format 3b: Native-VSI/outRIF + HI + EEI/ARP-pointer/outLIF + FEC
   * used for roo application
   * changes between arad+ and jericho:
   * arad+: Native-VSI/outRIF (12) + HI (4) + EEI/ARP-pointer/outLIF (14) + FEC(12)
   * jer:   Native-VSI/outRIF (13) + HI (4) + EEI/ARP-pointer/outLIF (15) + FEC(12)
   */
  else if (entry_type == ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3B) 
  {
      uint32 hi_value; 
      uint32 truncated_eei;

      /* FEC value from destination. */
      res = soc_sand_bitstream_set_any_field(
         &(payload->dest), 
         0, 
         ARAD_PP_LEM_3B_PAYLOAD_FORMAT_DEST_NOF_BITS, 
         payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* EEI/ARP-pointer/outLIF  from additional info.
       * Additional info is encoded as eei: {EEI identifier[23:20], HI[19:16], out_lif[15:0]} */ 

      /* EEI/ARP-pointer/outLIF is truncated to 14 bits (arad+) and 15 bits (jer) */
      ARAD_PP_LEM_3B_PAYLOAD_FORMAT_EEI_TO_TRUNCATED_EEI(unit, truncated_eei, payload->asd);

      res = soc_sand_bitstream_set_any_field(
         &truncated_eei,
         ARAD_PP_LEM_3B_PAYLOAD_FORMAT_DEST_NOF_BITS, 
         SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_eei_nof_bits), 
         payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      /* HI from EEI */
      /* extract hi from eei encoding = 24 bits = {EEI identifier[23:20], HI[19:16], out_lif[15:0]} */
      ARAD_PP_LEM_ACCESS_HI_FROM_EEI(payload->asd, hi_value);

      res = soc_sand_bitstream_set_any_field(
         &hi_value,
         ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_LSB(unit), 
         ARAD_PP_LEM_3B_PAYLOAD_FORMAT_HI_NOF_BITS, 
         payload_data
        );

      /* native-VSI */
      res = soc_sand_bitstream_set_any_field(
         &(payload->native_vsi), 
         ARAD_PP_LEM_3B_PAYLOAD_FORMAT_NATIVE_VSI_LSB(unit), 
         SOC_DPP_DEFS_GET(unit, lem_3b_payload_format_native_vsi_nof_bits), 
         payload_data
        );


  } else if (entry_type == ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA) 
  {
      /* FEC value from destination. */
      res = soc_sand_bitstream_set_any_field(
         &(payload->dest), 
         0, 
         ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_DST_NOF_BITS, 
         payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

      res = soc_sand_bitstream_set_any_field(
         &(payload->asd), 
         ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_DST_NOF_BITS, 
         ARAD_PP_LEM_RAW_PAYLOAD_FORMAT_ASD_NOF_BITS, 
         payload_data
        );
      SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_payload_build()", 0, 0);
}

uint32
  arad_pp_lem_key_encoded_build(
     SOC_SAND_IN  int                        unit,
     SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY             *key,
     SOC_SAND_IN  uint32                             is_mask,
     SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY_ENCODED     *key_in_buffer
  )
{
  uint32
    current_param_lsb,
    res = SOC_SAND_OK,
    tmp = 0,
    buffer_lcl[SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S];
  uint32
    param_ndx,
    buffer_ndx;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_KEY_ENCODED_BUILD);

  SOC_SAND_CHECK_NULL_INPUT(key);
  SOC_SAND_CHECK_NULL_INPUT(key_in_buffer);

  /*
   *    Initialization
   */
  for (buffer_ndx = 0; buffer_ndx < SOC_DPP_DEFS_LEM_WIDTH_IN_UINT32S(unit); ++buffer_ndx)
  {
    buffer_lcl[buffer_ndx] = 0;
  }

  /*
   *    Copy of the parameters
   */
  current_param_lsb = 0;
  for (param_ndx = 0; param_ndx < key->nof_params; ++param_ndx)
  {
    if (((key->param[param_ndx].nof_bits == 0) && (key->prefix.value != ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC))  || (key->param[param_ndx].nof_bits > SOC_DPP_DEFS_GET(unit, lem_width)))
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR, 10, exit);
    }
    res = soc_sand_bitstream_set_any_field(
            key->param[param_ndx].value,
            current_param_lsb,
            key->param[param_ndx].nof_bits,
            buffer_lcl
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    current_param_lsb += key->param[param_ndx].nof_bits;
  }

  /*
   *    Copy of the prefix and verification of its length
   */
  if (current_param_lsb + key->prefix.nof_bits > SOC_DPP_DEFS_GET(unit, lem_width))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_OUT_OF_RANGE_ERR, 10, exit);
  }
#if 1

  /* map sw key to hw key */
  if (!is_mask) {
      if (SOC_DPP_CONFIG(unit)->pp.l3_source_bind_mode != SOC_DPP_L3_SOURCE_BIND_MODE_DISABLE &&
          (key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP || key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC) &&
          ((key->prefix.nof_bits < SOC_DPP_DEFS_GET(unit, nof_lem_prefixes))||(key->prefix.nof_bits == SOC_DPP_DEFS_GET(unit, nof_lem_prefixes))) &&
         (( key->prefix.value == 0x01)||( key->prefix.value == 0x8))) {
          
        if (key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP) {
          res = arad_pp_lem_access_app_to_prefix_get(unit,ARAD_PP_LEM_ACCESS_KEY_PREFIX_IPV4_SPOOF_DHCP,&tmp);
        } else {
          res = arad_pp_lem_access_app_to_prefix_get(unit,ARAD_PP_LEM_ACCESS_KEY_PREFIX_IP6_SPOOF_STATIC,&tmp);
        }
        SOC_SAND_CHECK_FUNC_RESULT(res, 21, exit);
        /* For IP spoofing dhcp,  Prefix values is 8-15.
         * Only msb is set in that case out of the 4 bits 
         */
       if (SOC_IS_JERICHO(unit)) {
          tmp = 0x08;
       } else{
          tmp = 0x01;
       }
      }
      else if (!(SOC_DPP_CONFIG(unit)->l3.ipmc_vpn_lookup_enable) && 
               key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC &&
               key->prefix.nof_bits < SOC_DPP_DEFS_GET(unit, nof_lem_prefixes) &&
               key->prefix.value == ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_GLOBAL_IPV4_MC) {
        res = arad_pp_lem_access_app_to_prefix_get(unit,ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_GLOBAL_IPV4_MC,&tmp);
        SOC_SAND_CHECK_FUNC_RESULT(res, 21, exit);
        /* For global IPMC, prefix values are 12-15, only 2msbs are set */
        tmp = 0x03;
      }
      else {
        res = arad_pp_lem_access_app_to_prefix_get(unit,key->prefix.value,&tmp);
        SOC_SAND_CHECK_FUNC_RESULT(res, 22, exit);
      }
  }
  else {
      tmp = key->prefix.value;
  }
#endif

  res = soc_sand_bitstream_set_any_field(
          &(tmp),
          SOC_DPP_DEFS_GET(unit, lem_width) - key->prefix.nof_bits,
          key->prefix.nof_bits,
          buffer_lcl
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 23, exit);

  for (buffer_ndx = 0; buffer_ndx < SOC_DPP_DEFS_LEM_WIDTH_IN_UINT32S(unit); ++buffer_ndx)
  {
    key_in_buffer->buffer[buffer_ndx] = buffer_lcl[buffer_ndx];
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_key_encoded_build()", 0, 0);
}



uint32
  arad_pp_lem_key_encoded_parse(
     SOC_SAND_IN  int                        unit,
     SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY_ENCODED     *key_in_buffer,
     SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY             *key
  )
{
  uint32
    curr_bit_lsb,
    app_type = 0,
    key_type_msb = 0,
    res = SOC_SAND_OK;
  uint32
    param_ndx;
  uint8
    mac_in_mac_enabled;
  int table_id, i;
  uint32 real_dbal_key_type = 0;
  SOC_DPP_DBAL_TABLE_INFO table;
/*  uint8
    garbage_bool,
    trill_mc_param1_decider;*/
  

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_KEY_ENCODED_PARSE);

  SOC_SAND_CHECK_NULL_INPUT(key);
  SOC_SAND_CHECK_NULL_INPUT(key_in_buffer);

  ARAD_PP_LEM_ACCESS_KEY_clear(key);

  /*
   *    Find the right type:
   *    sa authentication is longer that other prefixes. Retrieve by it's length.
   *    If the prefix is not SA auth, retrieve what's encoded in the four msb of the buffer.
   */
  key_type_msb = 0;

  res = soc_sand_bitstream_get_any_field(
          key_in_buffer->buffer,
          SOC_DPP_DEFS_GET(unit, lem_width) - ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit),
          ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit),
          &(key_type_msb)
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  /* key type tp app */
  if (key_type_msb != ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SA_AUTH)
  {
      key_type_msb = 0;
      res = soc_sand_bitstream_get_any_field(
              key_in_buffer->buffer,
              SOC_DPP_DEFS_GET(unit, lem_width) - SOC_DPP_DEFS_GET(unit, nof_lem_prefixes),
              SOC_DPP_DEFS_GET(unit, nof_lem_prefixes),
              &(key_type_msb)
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);
  }

  /* map prefix for app usage */
  res = arad_pp_lem_access_prefix_to_app_get(unit, key_type_msb, &app_type);
  SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);

  if ((app_type >= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE ) && (app_type <= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_END) ) {
      real_dbal_key_type = app_type;
      app_type = ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE;
  }
  /* according to 4 msb, get what is the type of the key */
  switch(app_type) { 
  case 0x0:
      /* ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BMAC or ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ETH */
      if (SOC_IS_ARAD_A0(unit)) {
          /*
           *  Backbone MAC
           */
          key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_BACKBONE_MAC;
          key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC;
          key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_BACKBONE_MAC(unit);
          key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_BACKBONE_MAC;
          key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_BACKBONE_MAC;
          key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_BACKBONE_MAC;
          key->param[2].nof_bits = 0;
          key->param[3].nof_bits = 0;
          key->param[4].nof_bits = 0;
      }
      else {
          /*
           *  MAC
           */
          key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC;
          key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC;
          key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC(unit);
          key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC;
          key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC;
          key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC;
          key->param[2].nof_bits = 0;
          key->param[3].nof_bits = 0;
          key->param[4].nof_bits = 0;
      }
      break;

  case 0x1:
      /* ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ETH */
      if (SOC_IS_ARAD_A0(unit)) {
          /*
           *  MAC
           */
          key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC;
          key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC;
          key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC(unit);
          key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC;
          key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC;
          key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC;
          key->param[2].nof_bits = 0;
          key->param[3].nof_bits = 0;
          key->param[4].nof_bits = 0;
      }
      break;

    /*
     *  IPv4 Compatible Multicast
     */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_COMP:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPV4_MC;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_MC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IPV4_MC;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_MC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_MC;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_MC;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;

    /*
     *  IPv4 Host
     */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_HOST:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_HOST;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IP_HOST;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_HOST;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_HOST;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_HOST;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
    
    /*
     *  trill UC
     */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_UC:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_UC;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_UC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_UC;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_UC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_UC;
    key->param[1].nof_bits = 0;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;

    /*
     *  trill MC
     */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_MC:
  
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_MC(unit);
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_MC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_MC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_MC;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_MC;
    key->param[2].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_TRILL_MC(unit);
    key->param[3].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_TRILL_MC(unit);
    break;
    
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SA_AUTH:
      if (SOC_IS_ARAD_A0(unit)) {
           /*
            *  SA AUTH 
            */
          key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_SA_AUTH;
          key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH;
          key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SA_AUTH;
          key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit);
          key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_SA_AUTH;
          key->param[1].nof_bits = 0;
          key->param[2].nof_bits = 0;
          key->param[3].nof_bits = 0;
          key->param[4].nof_bits = 0;
      }
      else {
          /* ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_SA_AUTH or ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BMAC */

          /* SA auth and MiM can't exist together - check if MiM is enabled */
          res = arad_pp_is_mac_in_mac_enabled(
              unit,
              &mac_in_mac_enabled
              );
          SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);

          if (mac_in_mac_enabled) {
              /*
               *  Backbone MAC
               */
              key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_BACKBONE_MAC;
              key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC;
              key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_BACKBONE_MAC(unit);
              key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_BACKBONE_MAC;
              key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_BACKBONE_MAC;
              key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_BACKBONE_MAC;
              key->param[2].nof_bits = 0;
              key->param[3].nof_bits = 0;
              key->param[4].nof_bits = 0;
          }
          else {
              /*
               *  SA AUTH 
               */
              key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_SA_AUTH;
              key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH;
              key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SA_AUTH;
              key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit);
              key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_SA_AUTH;
              key->param[1].nof_bits = 0;
              key->param[2].nof_bits = 0;
              key->param[3].nof_bits = 0;
              key->param[4].nof_bits = 0;
          }
      }
      break;

    case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_TRILL_ADJ:
      key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_ADJ;
      key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ;
      res = arad_pp_lem_access_app_to_prefix_get(
              unit, 
              ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_ADJ,
              &key->prefix.value);
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
      key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_ADJ;
      key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_ADJ;
      key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_ADJ;
      key->param[2].nof_bits = 0;
      key->param[3].nof_bits = 0;
      key->param[4].nof_bits = 0;
      break;

   /*
    *   ILM
    */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_ILM:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_ILM;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_ILM;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_ILM;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_ILM;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_ILM;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_ILM;
    key->param[2].nof_bits = SOC_IS_JERICHO(unit) ? ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_ILM_JERICHO : ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_ILM;
    key->param[3].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_ILM;
    key->param[4].nof_bits = 0;
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_EXTEND:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_EXTENDED;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_EXTENDED;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_EXTENDED;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_EXTENDED;
    key->param[0].nof_bits = ARAD_PP_L2_LIF_KEY_TYPE_EXTENDED_PARAM0_NOF_BITS;
    break;
  /*  FCOE local */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    key->param[0].nof_bits = ARAD_PP_FRWRD_FCF_D_ID_BITS;
    key->param[1].nof_bits = ARAD_PP_FRWRD_FCF_VFI_BITS;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  /*  FCOE local N_PORT */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL_N_PORT:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    key->param[0].nof_bits = ARAD_PP_FRWRD_FCF_S_ID_BITS;
    key->param[1].nof_bits = ARAD_PP_FRWRD_FCF_VFI_BITS;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
  /*  FCOE remote */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_REMOTE:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_REMOTE;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    key->param[0].nof_bits = ARAD_PP_FRWRD_FCF_D_ID_ROUTE_BITS;
    key->param[1].nof_bits = ARAD_PP_FRWRD_FCF_VFI_BITS;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  /*  FCOE Zoning */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_ZONING:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_ZONING;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    key->param[0].nof_bits = ARAD_PP_FRWRD_FCF_S_ID_BITS;
    key->param[1].nof_bits = ARAD_PP_FRWRD_FCF_D_ID_BITS;
    key->param[2].nof_bits = ARAD_PP_FRWRD_FCF_VFI_BITS;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
   /*
    *   SPOOF IPV4
    */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_SPOOF_DHCP:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_SPOOF_DHCP;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP_SPOOF_DHCP;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_SPOOF_DHCP;
    key->param[0].nof_bits = ( SOC_IS_JERICHO(unit))? ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO:ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP;
    key->param[1].nof_bits = (SOC_IS_JERICHO(unit))?ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO:ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP;
    key->param[2].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IP_SPOOF_DHCP;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_SPOOF_STATIC:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPV4_SPOOF_STATIC;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_SPOOF_STATIC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPV4_SPOOF_STATIC;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_SPOOF_STATIC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_SPOOF_STATIC;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_SPOOF_STATIC(unit);
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_GLOBAL_IPV4_COMP:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_GLOBAL_IPV4_MC;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_GLOBAL_IPV4_COMP;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_GLOBAL_IPV4_MC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_GLOBAL_IPV4_MC;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_GLOBAL_IPV4_MC;
    key->param[2].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_GLOBAL_IPV4_MC;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  /*
   *  MAC-in-MAC tunnel DB
   */
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_MAC_IN_MAC_TUNNEL:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TUNNEL;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC_IN_MAC_TUNNEL;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC_IN_MAC_TUNNEL;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC_IN_MAC_TUNNEL;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC_IN_MAC_TUNNEL;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC_IN_MAC_TUNNEL;
    key->param[2].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_MAC_IN_MAC_TUNNEL;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;

    if (SOC_DPP_CONFIG(unit)->pp.test2) {
        key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TEST2;
        key->param[2].nof_bits = 0;
    }
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IPMC_BIDIR:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPMC_BIDIR;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IPMC_BIDIR;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IPMC_BIDIR;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPMC_BIDIR;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPMC_BIDIR;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPMC_BIDIR;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP6_SPOOF_STATIC:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP6_SPOOF_STATIC;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_IP6_SPOOF_STATIC;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP6_SPOOF_STATIC;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP6_SPOOF_STATIC;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP6_SPOOF_STATIC;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_DIP6_COMPRESSION:
    key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_DIP6_COMPRSSION;
    key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_COMPRESSION_DIP;
    key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_DIP6_COMPRESSION;
    key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_DIP6_COMPRESSION;
    key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_DIP6_COMPRESSION;
    key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_DIP6_COMPRESSION;
    key->param[2].nof_bits = 0;
    key->param[3].nof_bits = 0;
    key->param[4].nof_bits = 0;
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_STATISTICS:      
      /*for dbal dynamic prefixes, we need to look for the related dbal table for the received prefix*/
  case ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE:
    /* find the relevant DBAL LEM SW table */
    for (table_id = 0; table_id < SOC_DPP_DBAL_SW_NOF_TABLES; table_id++) {
        res = sw_state_access[unit].dpp.soc.arad.pp.dbal_info.dbal_tables.get(unit, table_id, &table);
        SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

        if ((table.physical_db_type == SOC_DPP_DBAL_PHYSICAL_DB_TYPE_LEM) && (table.is_table_initiated)) {
            if (table.db_prefix == real_dbal_key_type) {
                break;
            }
        }
    }
    key->type = real_dbal_key_type;
    /* If not found use old implementation */
    if (table_id == SOC_DPP_DBAL_SW_NOF_TABLES) {
        key->nof_params = 0;
        key->prefix.value = real_dbal_key_type;
        key->prefix.nof_bits = 0;
        key->param[0].nof_bits = 0;
        key->param[1].nof_bits = 0;
        key->param[2].nof_bits = 0;
        key->param[3].nof_bits = 0;
        key->param[4].nof_bits = 0;
    } else {
        for (i=0; i < table.nof_qualifiers; i++) {
            key->param[i].nof_bits = table.qual_info[i].qual_nof_bits;
        }
        key->prefix.value = table.db_prefix;
        key->prefix.nof_bits = table.db_prefix_len;
        key->nof_params = table.nof_qualifiers;
    }

    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP:
      break; 

#ifdef BCM_88660_A0
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_SLB:
    if (SOC_IS_ARADPLUS(unit)) {
      
      key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_SLB;
      key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_SLB;
      res = arad_pp_lem_access_app_to_prefix_get(
              unit, 
              ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SLB,
              &key->prefix.value);
      SOC_SAND_CHECK_FUNC_RESULT(res, 1000, exit);
      key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
      /* LSB of destination is ignored. */
      key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_SLB;
      key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_SLB;
      key->param[2].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_SLB;
      key->param[3].nof_bits = 0;
      key->param[4].nof_bits = 0;
      key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
      key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SLB;
    } else {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_UNKNOWN_KEY_PREFIX_ERR, 201, exit);
    }
    break;
#endif /* BCM_88660_A0 */
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_UNKNOWN_KEY_PREFIX_ERR, 20, exit);
  }

  /*
   *    Extract the fields from the buffer
   */
  curr_bit_lsb = 0;
  for (param_ndx = 0; param_ndx < key->nof_params; ++param_ndx)
  {
    if (
        ((key->param[param_ndx].nof_bits == 0) && key->prefix.value != ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC)
        || (key->param[param_ndx].nof_bits > SOC_DPP_DEFS_GET(unit, lem_width))
        || (curr_bit_lsb > SOC_DPP_DEFS_GET(unit, lem_width))
       )
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR, 30, exit);
    }
    res = soc_sand_bitstream_get_any_field(
            key_in_buffer->buffer,
            curr_bit_lsb,
            key->param[param_ndx].nof_bits,
            key->param[param_ndx].value
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

    curr_bit_lsb += key->param[param_ndx].nof_bits;
  }

  /*
   *    Last verification of the key format
   */
  for (param_ndx = key->nof_params; param_ndx < ARAD_PP_LEM_KEY_MAX_NOF_PARAMS; ++param_ndx)
  {
    if (key->param[param_ndx].nof_bits != 0)
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR, 50, exit);
    }
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_key_encoded_parse()", app_type, key_type_msb);
}

uint32
  arad_pp_lem_key_encoded_parse_arad_format(
     SOC_SAND_IN  int                        unit,
     SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY_ENCODED     *key_in_buffer,
     SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY             *key
  )
{
  uint32
    curr_bit_lsb,
    res = SOC_SAND_OK;
  uint32
    param_ndx;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_CHECK_NULL_INPUT(key);
  SOC_SAND_CHECK_NULL_INPUT(key_in_buffer);

  ARAD_PP_LEM_ACCESS_KEY_clear(key);

  /* Parse MACT entry */
  key->nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC;
  key->type = ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC;
  key->prefix.value = ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC(unit);
  key->prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC;
  key->param[0].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC;
  key->param[1].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC;
  key->param[2].nof_bits = 0;
  key->param[3].nof_bits = 0;
  key->param[4].nof_bits = 0;

  /*
   *    Extract the fields from the buffer
   */
  curr_bit_lsb = 0;
  for (param_ndx = 0; param_ndx < key->nof_params; ++param_ndx)
  {
    if (
        ((key->param[param_ndx].nof_bits == 0) && key->prefix.value != ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC)
        || (key->param[param_ndx].nof_bits > SOC_DPP_DEFS_GET(unit, lem_width))
        || (curr_bit_lsb > SOC_DPP_DEFS_GET(unit, lem_width))
       )
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR, 30, exit);
    }
    res = soc_sand_bitstream_get_any_field(
            key_in_buffer->buffer,
            curr_bit_lsb,
            key->param[param_ndx].nof_bits,
            key->param[param_ndx].value
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

    curr_bit_lsb += key->param[param_ndx].nof_bits;
  }

  /*
   *    Last verification of the key format
   */
  for (param_ndx = key->nof_params; param_ndx < ARAD_PP_LEM_KEY_MAX_NOF_PARAMS; ++param_ndx)
  {
    if (key->param[param_ndx].nof_bits != 0)
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PARAM_SIZE_OUT_OF_RANGE_ERR, 50, exit);
    }
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_key_encoded_parse_arad_format()", 0, 0);
}

/*
 *  Write a request into the Request FIFO
 */


uint32
arad_pp_lem_sw_entry_update_unsafe(
          SOC_SAND_IN  int                        unit,
          SOC_SAND_IN ARAD_PP_LEM_ACCESS_REQUEST  *request,
/*          ARAD_PP_LEM_ACCESS_CMD                  access_cmd;*/
          SOC_SAND_IN ARAD_PP_LEM_ACCESS_PAYLOAD  *payload,
          SOC_SAND_OUT uint8                      *success
  )
{
    uint32                                  res = SOC_SAND_OK;
    uint32                                  key[SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S],
                                            data[ARAD_PP_LEM_ACCESS_PAYLOAD_IN_UINT32S];
    ARAD_PP_LEM_ACCESS_KEY_ENCODED          key_in_buffer;

    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_SW_ENTRY_UPDATE_UNSAFE);

    SOC_SAND_CHECK_NULL_INPUT(request);
    SOC_SAND_CHECK_NULL_INPUT(success);

    ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);
    soc_sand_os_memset(key, 0x0, ARAD_CHIP_SIM_LEM_KEY);

    res = arad_pp_lem_key_encoded_build(unit, &(request->key), 0, &key_in_buffer);

    key[0] = key_in_buffer.buffer[0];
    key[1] = key_in_buffer.buffer[1];
    key[2] = key_in_buffer.buffer[2];

    /* Do not add dynamic entries to the shadow DB */
    if (SOC_DPP_IS_LEM_SIM_ENABLE(unit) && payload->is_dynamic == 0)
    {

        if (request->command == ARAD_PP_LEM_ACCESS_CMD_DELETE)
        {
            /*
             *  Deletion command
             */
            res = chip_sim_exact_match_entry_remove_unsafe(unit,
                                                           ARAD_CHIP_SIM_LEM_BASE,
                                                           key,
                                                           ARAD_CHIP_SIM_LEM_KEY);
            SOC_SAND_CHECK_FUNC_RESULT(res, 190, exit);
            *success = TRUE;
        }
        else
        {
            /*
             *  Insertion command
             */
            SOC_SAND_CHECK_NULL_INPUT(payload);

            soc_sand_os_memset(data, 0x0, ARAD_CHIP_SIM_LEM_PAYLOAD);

            res = arad_pp_lem_access_payload_build(unit, payload, data);
            SOC_SAND_CHECK_FUNC_RESULT(res, 200, exit);

            res = chip_sim_exact_match_entry_add_unsafe(unit,
                                                        ARAD_CHIP_SIM_LEM_BASE,
                                                        key,
                                                        ARAD_CHIP_SIM_LEM_KEY,
                                                        data,/* payload_data */
                                                        ARAD_CHIP_SIM_LEM_PAYLOAD,
                                                        success);
            SOC_SAND_CHECK_FUNC_RESULT(res, 210, exit);
        }
    }

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_sw_entry_update_unsafe()", 0, 0);
}

uint32
arad_pp_lem_cr_sw_entry_update_unsafe(
          SOC_SAND_IN  int                        unit,
          SOC_SAND_IN ARAD_PP_LEM_ACCESS_REQUEST  *request,
          SOC_SAND_IN ARAD_PP_LEM_ACCESS_PAYLOAD  *payload,
          SOC_SAND_OUT uint8                      *success
  )
{
    uint32                                  res = SOC_SAND_OK;
    uint32                                  key[SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S],
                                            data[ARAD_PP_LEM_ACCESS_PAYLOAD_IN_UINT32S];
    ARAD_PP_LEM_ACCESS_KEY_ENCODED          key_in_buffer;

    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_SW_ENTRY_UPDATE_UNSAFE);

    SOC_SAND_CHECK_NULL_INPUT(request);
    SOC_SAND_CHECK_NULL_INPUT(success);

    ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);
    soc_sand_os_memset(key, 0x0, ARAD_CHIP_SIM_LEM_KEY);

    res = arad_pp_lem_key_encoded_build(unit, &(request->key), 0, &key_in_buffer);

    key[0] = key_in_buffer.buffer[0];
    key[1] = key_in_buffer.buffer[1];
    key[2] = key_in_buffer.buffer[2];

    SOC_SAND_CHECK_NULL_INPUT(payload);

    soc_sand_os_memset(data, 0x0, ARAD_CHIP_SIM_LEM_PAYLOAD);

    res = arad_pp_lem_access_payload_build(unit, payload, data);
    SOC_SAND_CHECK_FUNC_RESULT(res, 200, exit);

    if (request->command == ARAD_PP_LEM_ACCESS_CMD_DELETE) {
        /* set indication in the payload that this entry is deleted */
        SHR_BITSET(data,ARAD_CHIP_SIM_LEM_CR_DELETED_ENTRY_INDICATION_BIT);
    }

    res = chip_sim_exact_match_entry_add_unsafe(unit,
                                                ARAD_CHIP_SIM_LEM_CR_BASE,
                                                key,
                                                ARAD_CHIP_SIM_LEM_KEY,
                                                data,/* payload_data */
                                                ARAD_CHIP_SIM_LEM_PAYLOAD,
                                                success);
    SOC_SAND_CHECK_FUNC_RESULT(res, 210, exit);

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_sw_entry_update_unsafe()", 0, 0);
}





STATIC uint32
  arad_pp_lem_request_send(
      SOC_SAND_IN  int                          unit,
      SOC_SAND_IN  ARAD_PP_LEM_ACCESS_OUTPUT    *request_all,
      SOC_SAND_OUT uint8                        *is_poll_success
    )
{
  uint32    res             = SOC_SAND_OK;
  uint8     poll_success    = FALSE;
  uint8     data_len        = 0;
  soc_reg_above_64_val_t    reg_above_64_val;


  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_REQUEST_SEND);

  SOC_SAND_CHECK_NULL_INPUT(request_all);
  SOC_SAND_CHECK_NULL_INPUT(is_poll_success);


  /*
   *    Polling to verify no CPU request is waiting
   */

#ifdef PLISIM
  if (SAL_BOOT_PLISIM) {
      poll_success = TRUE;
  } else
#endif
  {
          res = arad_polling(
                 unit,
                 ARAD_TIMEOUT,
                 ARAD_MIN_POLLS,
                 PPDB_B_LARGE_EM_CPU_REQUEST_TRIGGERr,
                 REG_PORT_ANY,
                 0,
                 LARGE_EM_CPU_REQUEST_TRIGGERf,
                 0
               );

          SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
          poll_success = TRUE;
  }

  *is_poll_success = poll_success;

  res = arad_pp_lem_request_pack(
                                 unit,
                                 request_all,
                                 reg_above_64_val,
                                 &data_len);
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

/*  {
      uint8* data_as_uint8;
      int i;
      LOG_CLI((BSL_META("************* %s entry to LEM *************\n"), (request_all->request.command == ARAD_PP_LEM_ACCESS_CMD_DELETE)?"Deleting":"Adding"));
      data_as_uint8 = (uint8*)(&reg_above_64_val[0]);
      for (i = 0; i < data_len*8; i++) {        
          LOG_CLI((BSL_META("%x:"), (*(i+data_as_uint8))));
      }
      LOG_CLI((BSL_META("\n")));
  }*/
 
  /*
   *    Writing in this register is a trigger to send the request
   */
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
    res = ARAD_FAST_REGISER_SET(ARAD_FAST_REG_IHP_MACT_CPU_REQUEST_REQUEST, reg_above_64_val);    
#else
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 1000, exit, WRITE_PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr(unit, reg_above_64_val));
#endif




#ifdef CRASH_RECOVERY_SUPPORT
    if ((SOC_IS_DONE_INIT(unit)) && (BCM_UNIT_DO_HW_READ_WRITE(unit))) {
        /* Update the cache with new values */
        res = arad_pp_lem_cr_sw_entry_update_unsafe(unit, &(request_all->request), &(request_all->payload), is_poll_success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
    }
#endif /* CRASH_RECOVERY_SUPPORT */
    if (SOC_DPP_IS_LEM_SIM_ENABLE(unit)) {
        /* Update the cache with new values */
        res = arad_pp_lem_sw_entry_update_unsafe(unit, &(request_all->request), &(request_all->payload), is_poll_success);
        SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
    }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_request_send()", 0, 0);
}

/*
 *  Pack a CPU request request information
 */
uint32
  arad_pp_lem_request_pack(
      SOC_SAND_IN  int                        unit,
      SOC_SAND_IN  ARAD_PP_LEM_ACCESS_OUTPUT    *request_all,
      SOC_SAND_OUT uint32                       *data,
      SOC_SAND_OUT uint8                        *data_len
    )
{
  uint32
    fld_val,
    res = SOC_SAND_OK;
  uint8
    db_profile;
  ARAD_PP_LEM_ACCESS_KEY_ENCODED
    key_in_buffer;
  soc_reg_above_64_val_t
    fld_value, 
    reg_above_64_val;
  uint32
    payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S];

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_REQUEST_SEND);

  SOC_SAND_CHECK_NULL_INPUT(request_all);

  ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);


  /*
   *    Insert the request details
   */
  SOC_REG_ABOVE_64_CLEAR(reg_above_64_val);
  SOC_REG_ABOVE_64_CLEAR(fld_value);

  /*
   *    Request origin: CPU or OLP
   */
  switch(request_all->req_origin)
  {
  case ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU:
    fld_val = ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU_FLD_VAL;
    break;

  case ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OLP:
    fld_val = ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OLP_FLD_VAL;
    break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OUT_OF_RANGE_ERR, 10, exit);
  }

  fld_value[0] = fld_val;
        
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_QUALIFIER, reg_above_64_val, fld_value);
#else

  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_QUALIFIERf, fld_value, reg_above_64_val,  15, exit);
#endif
  /*
   *    Entry self-learned
   */
  fld_val = request_all->is_learned_first_by_me;
  fld_value[0] = fld_val;

#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_SELF, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_SELFf, fld_value, reg_above_64_val,  20, exit);
#endif
  

  /*
   *    No need to set the age (overridden)
   */
  fld_val = 7;
  fld_value[0] = fld_val;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_AGE_PAYLOAD, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_AGE_PAYLOADf, fld_value, reg_above_64_val, 25, exit);
#endif  

  /* The Refreshed by DSP bit (RBD) is always set. */
  fld_value[0] = 1;
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_AGE_PAYLOAD_REFRESHED_BY_DSPf, fld_value, reg_above_64_val, 25, exit);

  fld_val = request_all->payload.is_dynamic;
  fld_value[0] = fld_val;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PAYLOAD_IS_DYNAMIC, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PAYLOAD_IS_DYNAMICf, fld_value, reg_above_64_val, 30, exit);
#endif  
  

  res = arad_pp_lem_access_payload_build(unit,&request_all->payload,payload_data);
  SOC_SAND_CHECK_FUNC_RESULT(res, 47, exit);

  

  fld_value[0] = payload_data[0];
  fld_value[1] = payload_data[1];

  /* in the PMF we will copy the payload.asd (inlif) to the trap qualifier therefore we need the offset to be  byte aligned*/
  if (request_all->request.key.type==ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP || request_all->request.key.type==ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_ECHO) {
      /* Format will be as follows:
      {0 (2b) ,remote_detect_mult[0:3] (4b) ,OAM-ID (13/14b) ,remote_detect_mult[4:7] (4b) , 0 (1b) ,destination (19b)} when not monitoring remote detect multiplier
           {0 (2b)  ,OAM-ID (13/14b) , remote detect multilier (8b) , 0 (3b) snoop strength (2b) ,trap strength (3b) ,trap code (8b)} when monitoring remote detect multiplier*/
      int oam_id_size= SOC_DPP_DEFS_GET(unit,oam_2_id_nof_bits) ;
      fld_value[0]= 0;
      fld_value[1]= 0;
      /* First, shove the destination into the payload. 19 bits*/
      SHR_BITCOPY_RANGE(fld_value,0,&request_all->payload.dest, 0,19 ); 
      /* Now the OAM-ID. This value must be extracted by the PMF and thus must be byte alligned. 
         Starting at the third byte. First nibble of remote-detect-mult may follow, soc property dependent.*/
      SHR_BITCOPY_RANGE(fld_value,8*3,&request_all->payload.asd, 0,oam_id_size ); 

      /* Finally, the remote detect multiplier. 8 bits in 16-24. Will be 0 when bfd_ipv4_single_hop_extended!=2*/
      SHR_BITCOPY_RANGE(fld_value,16,&request_all->payload.asd,oam_id_size,8 /*remote detect multiplier is always 8 bits*/ ); 
  }

#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PAYLOAD, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PAYLOADf, fld_value, reg_above_64_val, 35, exit);
#endif    
  fld_value[1] = 0;

  /*
   *    Stamp
   */
  fld_value[0] = request_all->request.stamp;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_STAMP, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_STAMPf, fld_value, reg_above_64_val, 40, exit);
#endif    

  /*
   *    Command type
   */
  switch(request_all->request.command)
  {
  case ARAD_PP_LEM_ACCESS_CMD_DELETE:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_DELETE_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_INSERT:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_INSERT_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_REFRESH:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_REFRESH_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_LEARN:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_LEARN_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_DEFRAG:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_DEFRAG_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_ACK:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_ACK_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_TRANSPLANT:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_TRANSPLANT_FLD_VAL;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_ERROR:
    fld_val = ARAD_PP_LEM_ACCESS_CMD_ERROR_FLD_VAL;
    break;
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR, 45, exit);
  }
  fld_value[0] = fld_val;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_COMMAND, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_COMMANDf, fld_value, reg_above_64_val, 50, exit);
#endif  
  

  fld_val = 0x0;
  fld_value[0] = fld_val;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PART_OF_LAG, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PART_OF_LAGf, fld_value, reg_above_64_val, 55, exit);
#endif    

  /*
   *    Key insertion (or (System-VSI, MAC))
   */

  res = arad_pp_lem_key_encoded_build(
          unit,
          &(request_all->request.key),
          0,
          &key_in_buffer
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

  /*
   *    No insertion of System-VSI mode
   */
  /*if (request_all->request.key.type == ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC)*/
  {
    db_profile = 1;
    fld_value[0] = db_profile;
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_MFF_IS_KEY, reg_above_64_val, fld_value);
#else
    ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_MFF_IS_KEYf, fld_value, reg_above_64_val, 60, exit);
#endif      
  }

  fld_value[0] = key_in_buffer.buffer[0];
  fld_value[1] = key_in_buffer.buffer[1];
  fld_value[2] = key_in_buffer.buffer[2];
#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  ARAD_FAST_FIELD_SET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_MFF_KEY, reg_above_64_val, fld_value);
#else
  ARAD_FLD_TO_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_MFF_KEYf, fld_value, reg_above_64_val, 65, exit);
#endif    

  *data_len = 5;
  memcpy(data, reg_above_64_val, sizeof(uint32)*(*data_len));

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_request_send()", 0, 0);
}

/*
 * Parse event_buff into ARAD_PP_LEM_ACCESS_OUTPUT
 * event_buff is in CPU_REQUEST_REQUEST format. 
 * In case of CPU learning (flag==ARAD_PP_LEM_ACCESS_LEARN_EVENT) the event_buff format is Internal-DSP-Command in Arad format (see OLP doc).
 */
uint32
  arad_pp_lem_access_parse_only(
      SOC_SAND_IN  int                            unit,
      SOC_SAND_IN uint32                            flags,
      SOC_SAND_IN  uint32                           *event_buff,
      SOC_SAND_IN  uint32                            event_buff_len,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_OUTPUT        *request_all,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS    *ack_status
    )
{
  uint32
      res = SOC_SAND_OK;
  ARAD_PP_LEM_ACCESS_KEY_ENCODED
      key_in_buffer;
  soc_reg_above_64_val_t
      reg_above_64_val;
  soc_reg_above_64_val_t
    fld_value;
  uint32
      pld_val[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S],
      dma_supported;
  uint8 parse_learn_event;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_PARSE);

  SOC_SAND_CHECK_NULL_INPUT(request_all);
  SOC_SAND_CHECK_NULL_INPUT(ack_status);

  /* Learn events have a slightly different parsing */
  parse_learn_event = (flags & ARAD_PP_LEM_ACCESS_LEARN_EVENT) ? 1 : 0;

  /* DMA events on Jericho use the Arad learn format */
  res = arad_pp_frwrd_mact_is_dma_supported(unit, &dma_supported);
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  memcpy(reg_above_64_val, event_buff, sizeof(uint32)*event_buff_len);

    /*
     *    Stamp
     */ 
    fld_value[0] = 0; 

    if (parse_learn_event && dma_supported) 
    {
      SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 78, 8);
    } 
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
      SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 79, 8);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS  
      ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_STAMP, reg_above_64_val, fld_value);  
    #else
      ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_STAMPf, fld_value, reg_above_64_val, 20, exit);
    #endif   
    }
  
    request_all->stamp = fld_value[0];

    /*
     *    Command type
     */
    fld_value[0] = 0;

    if (parse_learn_event && dma_supported) 
    {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 75, 3);
    } 
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 76, 3);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_COMMAND, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_COMMANDf, fld_value, reg_above_64_val, 85, exit);
    #endif
    }

  switch(fld_value[0])
  {
  case ARAD_PP_LEM_ACCESS_CMD_DELETE_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_DELETE;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_INSERT_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_INSERT;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_REFRESH_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_REFRESH;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_LEARN_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_LEARN;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_DEFRAG_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_DEFRAG;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_ACK_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_ACK;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_TRANSPLANT_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_TRANSPLANT;
    break;
  case ARAD_PP_LEM_ACCESS_CMD_ERROR_FLD_VAL:
    request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_ERROR;
    break;
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR, 80, exit);
  }

#ifdef PLISIM
      request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_ACK;
      ack_status->is_success = TRUE;
#endif
#if SOC_DPP_IS_EM_HW_ENABLE    
    /*
     *    Handle ACK command
     */
    fld_value[0] = 0;

    if (request_all->request.command == ARAD_PP_LEM_ACCESS_CMD_ACK) 
    {
        if (parse_learn_event && dma_supported)
        {
          SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 86, 1);
        } 
        else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
          SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 133, 1);
        }
        else
        { 
        #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
            ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_SUCCESS, reg_above_64_val, fld_value);  
        #else
            ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_SUCCESSf, fld_value, reg_above_64_val, 37, exit);
        #endif 
        }

        if (fld_value[0] == 0x1)
        {
            ack_status->is_success = TRUE;
            ack_status->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;
        }
        else
        {
            ack_status->is_success = FALSE;
            fld_value[0] = 0;

            if (parse_learn_event && dma_supported) 
            {
                SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 87, 1);
            } 
            else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
                SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 134, 1);
            }
            else
            {
            #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
                ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_REASON, reg_above_64_val, fld_value);  
            #else
                ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_REASONf, fld_value, reg_above_64_val, 38, exit);
            #endif
            }

            switch(fld_value[0])
            {    
            case ARAD_PP_LEM_ACCESS_FAIL_REASON_FID_LIMIT_FLD_VAL:
              ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_FID_LIMIT;
              break;

            case ARAD_PP_LEM_ACCESS_FAIL_REASON_EMC_PROBLEM_FLD_VAL:
              ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_EMC_PROBLEM;
              break;

            default:
              SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_FAIL_REASON_OUT_OF_RANGE_ERR, 43, exit);
            }
        }
    }
    else
    {
      /* No ACK command just init it with SUCCESS */
      ack_status->is_success = 1;
      ack_status->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;
    }
#endif /* SOC_DPP_IS_EM_HW_ENABLE */

    if (flags & ARAD_PP_LEM_ACCESS_PARSE_FLAGS_ACK) {
        /* Spare time in entry insertion to get only the ack */
        ARAD_DO_NOTHING_AND_EXIT;
    }

    /*
    *    Request origin (REQ_QUALIFIER): CPU or OLP
    */
    fld_value[0] = 0;

    if (parse_learn_event && SOC_IS_JERICHO(unit) && !dma_supported) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 137, 1);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_QUALIFIER, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_QUALIFIERf, fld_value, reg_above_64_val, 30, exit);
    #endif
    }

    switch(fld_value[0])
    {
    case ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU_FLD_VAL:
        request_all->req_origin = ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU;
        break;
    case ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OLP_FLD_VAL:
        request_all->req_origin = ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OLP;
        break;
    default:
        SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_OUT_OF_RANGE_ERR, 25, exit);
    }

    /*
    *    Entry self-learnt
    */
    fld_value[0] = 0;

    if (parse_learn_event && dma_supported) {
      fld_value[0] = 1; /* DMA events are always self=1 */
    }
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
      SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 135, 1);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
      ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_SELF, reg_above_64_val, fld_value);  
    #else
      ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_SELFf, fld_value, reg_above_64_val, 35, exit);
    #endif
    }

    request_all->is_learned_first_by_me = SOC_SAND_NUM2BOOL(fld_value[0]);

    /*
     *    AGE PAYLOAD - Computation of the good aging payload according to the system configuration
     */
    fld_value[0] = 0;

    if (parse_learn_event && SOC_IS_JERICHO(unit) && !dma_supported) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 130, 3);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_AGE_PAYLOAD, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_AGE_PAYLOADf, fld_value, reg_above_64_val, 45, exit);
    #endif  
    }

    res = arad_pp_lem_access_age_fld_get(
          unit,
          fld_value[0],
          &(request_all->payload.age)
        );
    SOC_SAND_CHECK_FUNC_RESULT(res, 38, exit);  

    /* IS DYNAMIC */
    fld_value[0] = 0;
    
    if (parse_learn_event && dma_supported) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 131, 1);
    }
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 129, 1);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PAYLOAD_IS_DYNAMIC, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PAYLOAD_IS_DYNAMICf, fld_value, reg_above_64_val, 46, exit);
    #endif  
    }
    request_all->payload.is_dynamic = SOC_SAND_NUM2BOOL(fld_value[0]);

    /*
     *    Parse Key (or (System-VSI, MAC))
     */ 
    fld_value[0] = 0;
    fld_value[1] = 0;
    fld_value[2] = 0;

    if (parse_learn_event & SOC_IS_JERICHO(unit)) {
        fld_value[2]=0;
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 1, 32);
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[1]), 0, reg_above_64_val, 33, 32);
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[2]), 0, reg_above_64_val, 65, 10);
    }
    else
    {
        /*
         *  Copy bits 73:1 from the register to key_in_buffer (System-VSI & MAC)
         */
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_MFF_KEY, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_MFF_KEYf, fld_value, reg_above_64_val, 100, exit);
    #endif 
    }

    ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);
    key_in_buffer.buffer[0] = fld_value[0];
    key_in_buffer.buffer[1] = fld_value[1];
    key_in_buffer.buffer[2] = fld_value[2];
  
#ifdef PLISIM
    if (SAL_BOOT_PLISIM) {
      request_all->request.command = ARAD_PP_LEM_ACCESS_CMD_ACK;
      ack_status->is_success = TRUE;
    }
    else
#endif
    {
        if (parse_learn_event)
        {
            res = arad_pp_lem_key_encoded_parse_arad_format(
                unit,
                &key_in_buffer,
                &(request_all->request.key)
                );
        }
        else
        {
            res = arad_pp_lem_key_encoded_parse(
                unit,
                &key_in_buffer,
                &(request_all->request.key)
                );
        }
        SOC_SAND_CHECK_FUNC_RESULT(res, 95, exit);
    }

    /* REQ_PAYLOAD - read and parse payload buffer into lem-payload struct*/
    if (parse_learn_event && dma_supported) {
        fld_value[0]=0;
        fld_value[1]=0;
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 89, 32);
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[1]), 0, reg_above_64_val, 121, 10);
    }
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
        fld_value[0]=0;
        fld_value[1]=0;
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 87, 32);
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[1]), 0, reg_above_64_val, 119, 11);
    } 
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PAYLOAD, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PAYLOADf, fld_value, reg_above_64_val, 47, exit);
    #endif
    }

    pld_val[0] = fld_value[0];
    pld_val[1] = fld_value[1];

    request_all->payload.is_learn_data = parse_learn_event;
    res = arad_pp_lem_access_payload_parse(unit,pld_val,request_all->request.key.type,&request_all->payload);
    SOC_SAND_CHECK_FUNC_RESULT(res, 107, exit);


    /*
    * Part of LAG
    */
    fld_value[0]=0;

    if (parse_learn_event && dma_supported)
    {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 88, 1);
    }
    else if (parse_learn_event && SOC_IS_JERICHO(unit)) {
        SHR_BITCOPY_RANGE((uint32*)&(fld_value[0]), 0, reg_above_64_val, 75, 1);
    }
    else
    {
    #ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
        ARAD_FAST_FIELD_GET(ARAD_FAST_FIELD_IHP_MACT_CPU_REQUEST_REQUEST_MACT_REQ_PART_OF_LAG, reg_above_64_val, fld_value);  
    #else
        ARAD_FLD_FROM_REG_ABOVE_64(PPDB_B_LARGE_EM_CPU_REQUEST_REQUESTr, LARGE_EM_REQ_PART_OF_LAGf, fld_value, reg_above_64_val, 80, exit);
    #endif 
    }
    request_all->is_part_of_lag = SOC_SAND_NUM2BOOL(fld_value[0]);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_parse_only()", 0, 0);
}

/*
 *  Parse access output
 */
uint32
  arad_pp_lem_access_parse(
      SOC_SAND_IN  int                        unit,
      SOC_SAND_IN uint32                         flags,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_OUTPUT        *request_all,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS    *ack_status
    )
{
  uint32
      res = SOC_SAND_OK;
  ARAD_PP_LEM_ACCESS_KEY_ENCODED
      key_in_buffer;
  soc_reg_above_64_val_t
      reg_above_64_val;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_PARSE);

  SOC_SAND_CHECK_NULL_INPUT(request_all);
  SOC_SAND_CHECK_NULL_INPUT(ack_status);

  ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);

  /*
   *  Read registers into access buffer
   */

#ifdef ARAD_FAST_REGISTERS_AND_FIELDS_ACCESS
  res = ARAD_FAST_REGISER_GET(ARAD_FAST_REG_IHP_MACT_REPLY, reg_above_64_val);
  SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 1100, exit);
#else
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 1100, exit, READ_PPDB_B_LARGE_EM_REPLYr(unit, reg_above_64_val));    
#endif
  res = arad_pp_lem_access_parse_only(
          unit,
          flags,
          reg_above_64_val,
          SOC_REG_ABOVE_64_MAX_SIZE_U32,
          request_all,
          ack_status
        );

  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_parse()", 0, 0);
}

/*
 *  Read a request answer
 */
STATIC uint32
  arad_pp_lem_request_answer_receive(
      SOC_SAND_IN  int                        unit,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_OUTPUT          *request_all,
      SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS      *ack_status
    )
{
  uint32
    fld_val,
    reg_interrupt,
    reg_interrupt2,
    res = SOC_SAND_OK,
    iter;
  uint64
    reg64_interrupt2;   
  ARAD_PP_LEM_ACCESS_KEY_ENCODED
    key_in_buffer;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_REQUEST_ANSWER_RECEIVE);

  SOC_SAND_CHECK_NULL_INPUT(request_all);
  SOC_SAND_CHECK_NULL_INPUT(ack_status);

  ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);

  
  /*
   *  Parse access buffer contents.
   *  while look until valid ACK command.
   *  On most cases one iteration is enough.                                  .
   */
  for (iter = 0; iter < ARAD_PP_LEM_NOF_ANSER_RECV_ITERATIONS; iter++) 
  {
    /* Return only the ack and stamp for performance */
    res = arad_pp_lem_access_parse(
            unit,
            ARAD_PP_LEM_ACCESS_PARSE_FLAGS_ACK,
            request_all,
            ack_status
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    if (request_all->request.command == ARAD_PP_LEM_ACCESS_CMD_ACK) 
    {
      /* Found valid ACK break; */
      break;
    }
  }
  

  /*
   *    Read the stamp to check it is the right request
   */
  if (request_all->stamp != arad_pp_lem_access_actual_stamp_get(unit))
  {
    ack_status->is_success = FALSE;
    ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_WRONG_STAMP;
  }

  /*
   *    Update the ACK status
   */
  if (ack_status->reason == ARAD_PP_LEM_ACCESS_FAIL_REASON_EMC_PROBLEM)
  {
    /*
     *  Read the interrupts to know the error reason - an OLP interference is possible
     */
      SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, ARAD_REG_ACCESS_ERR,READ_PPDB_B_LARGE_EM_INTERRUPT_REGISTER_ONEr(unit, &reg_interrupt));
      if(SOC_IS_JERICHO(unit)) {
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  31,  exit, ARAD_REG_ACCESS_ERR,READ_PPDB_B_LARGE_EM_INTERRUPT_REGISTER_TWOr(unit, &reg64_interrupt2));
      } else {
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  32,  exit, ARAD_REG_ACCESS_ERR,READ_IHP_MACT_INTERRUPT_REGISTER_TWOr(unit, &reg_interrupt2));
      }


    ARAD_FLD_FROM_REG(PPDB_B_LARGE_EM_INTERRUPT_REGISTER_ONEr, MACT_ERROR_CAM_TABLE_FULLf, fld_val, reg_interrupt, 40, exit);
    if (fld_val != 0x0)
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_CAM_FULL;
    }

    ARAD_FLD_FROM_REG(PPDB_B_LARGE_EM_INTERRUPT_REGISTER_ONEr, MACT_ERROR_DELETE_UNKNOWN_KEYf, fld_val, reg_interrupt, 41, exit);
    if (fld_val != 0x0)
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_DELETE_UNKNOWN;
    }

    ARAD_FLD_FROM_REG(PPDB_B_LARGE_EM_INTERRUPT_REGISTER_ONEr, MACT_ERROR_REACHED_MAX_ENTRY_LIMITf, fld_val, reg_interrupt, 42, exit);
    if (fld_val != 0x0)
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_MACT_FULL;
    }

    if(SOC_IS_JERICHO(unit)) {
      fld_val = soc_reg64_field32_get(unit, PPDB_B_LARGE_EM_INTERRUPT_REGISTER_TWOr, reg64_interrupt2, LARGE_EM_ERROR_LEARN_REQUEST_OVER_STATICf);
    } else {
      fld_val = soc_reg_field_get(unit, IHP_MACT_INTERRUPT_REGISTER_TWOr, reg_interrupt2, MACT_ERROR_LEARN_REQUEST_OVER_STATICf);
    }
    if (fld_val != 0x0)
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_LEARN_STATIC;
    }

    if(SOC_IS_JERICHO(unit)) {
      fld_val = soc_reg64_field32_get(unit, PPDB_B_LARGE_EM_INTERRUPT_REGISTER_TWOr, reg64_interrupt2, LARGE_EM_ERROR_CHANGE_REQUEST_OVER_STATICf);
    } else {
      fld_val = soc_reg_field_get(unit, IHP_MACT_INTERRUPT_REGISTER_TWOr, reg_interrupt2, MACT_ERROR_CHANGE_REQUEST_OVER_STATICf);
    }
    if (fld_val != 0x0)
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_CHANGE_STATIC;
    }
  }

exit:
#ifdef PLISIM
  if (SAL_BOOT_PLISIM) {
      ack_status->is_success = TRUE;
  }
#endif
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_request_answer_receive()", 0, 0);
}


uint32
  arad_pp_lem_access_request_convert_to_hw(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_REQUEST         *request,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_PAYLOAD         *payload,
    ARAD_PP_LEM_ACCESS_OUTPUT                       *request_hw
    )
{
    uint32      res = SOC_SAND_OK;

    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_REQUEST_CONVERT_TO_HW);

    SOC_SAND_CHECK_NULL_INPUT(request);
    SOC_SAND_CHECK_NULL_INPUT(request_hw);
    ARAD_PP_LEM_ACCESS_OUTPUT_clear(request_hw);
    /*
     *    Verify the inputs
     */
    res = ARAD_PP_LEM_ACCESS_REQUEST_verify(unit, request);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    /*
     *    Build the HW request
     */
    ARAD_COPY(&(request_hw->request), request, ARAD_PP_LEM_ACCESS_REQUEST, 1);

    request_hw->req_origin = ARAD_PP_LEM_ACCCESS_REQ_ORIGIN_CPU;
    res = arad_pp_lem_access_next_stamp_get(unit, &(request_hw->request.stamp));
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    /* If Insert/Transplant/Learn/Refresh command - convert payload */
    if (request->command != ARAD_PP_LEM_ACCESS_CMD_DELETE)
    {
        /*
         *    Verify the inputs
         */
        SOC_SAND_CHECK_NULL_INPUT(payload);
        res = ARAD_PP_LEM_ACCESS_PAYLOAD_verify(unit, payload);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

        ARAD_COPY(&(request_hw->payload), payload, ARAD_PP_LEM_ACCESS_PAYLOAD, 1);
        request_hw->is_learned_first_by_me = TRUE;
    }

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_request_convert_to_hw()", 0, 0);
}

/*********************************************************************
 *     Add an entry to the Exact match table.
 *********************************************************************/
uint32
  arad_pp_lem_access_entry_add_unsafe(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_REQUEST         *request,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_PAYLOAD         *payload,
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS      *ack_status
  )
{
  uint32
    res = SOC_SAND_OK;
  ARAD_PP_LEM_ACCESS_OUTPUT
    request_hw,
    request_hw_rcv;
  uint8
    poll_success;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_ENTRY_ADD_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(ack_status);

  ARAD_PP_LEM_ACCESS_OUTPUT_clear(&request_hw_rcv);

  /* Convert the request+payload into the hw request representation */
  arad_pp_lem_access_request_convert_to_hw(unit, request, payload, &request_hw);

  if ((request_hw.request.command != ARAD_PP_LEM_ACCESS_CMD_INSERT)
    &&(request_hw.request.command != ARAD_PP_LEM_ACCESS_CMD_LEARN)
    &&(request_hw.request.command != ARAD_PP_LEM_ACCESS_CMD_REFRESH)
    &&(request_hw.request.command != ARAD_PP_LEM_ACCESS_CMD_TRANSPLANT))
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR, 25, exit);
  }

  /*
   *    Send the request
   */
  res = arad_pp_lem_request_send(
          unit,
          &request_hw,
          &poll_success
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

#ifdef CRASH_RECOVERY_SUPPORT
    /* In case we are in the "HW LOG Activated" state - don't check the hw for success.
     * It is useless because the Log wasn't written to the hw yet. the request is stored in the log until 'Commit' is performed.
     * Just skip the _answer_receive() part and assume Success. */
    if (SOC_IS_DONE_INIT(unit))
    {
      if (BCM_UNIT_DO_HW_READ_WRITE(unit))
      {
/*          LOG_ERROR(BSL_LS_SOC_HWLOG,
                  (BSL_META_U(unit, "!!!!!!!!!!!!!!!: arad_pp_lem_access_entry_add_unsafe\n")));*/

          ack_status->is_success = TRUE;
          ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_UNKNOWN;
          ARAD_PP_DO_NOTHING_AND_EXIT;
      }
    }
#endif /* CRASH_RECOVERY_SUPPORT */

  /*
   *    Case of an unsent request
   */
  if (poll_success == FALSE)
  {
    ack_status->is_success = FALSE;
    ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_REQUEST_NOT_SENT;
    ARAD_PP_DO_NOTHING_AND_EXIT;
  }


  /*
   *    Receive the answer
   */

  /* if was no request for reply we are done, assuming successfully */
  if(request_hw.request.stamp == ARAD_PP_LEM_STAMP_INVALID) {
      ack_status->is_success = 1;
      ack_status->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;
      goto exit;
  }

  res = arad_pp_lem_request_answer_receive(
          unit,
          &request_hw_rcv,
          ack_status
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

  if (request_hw_rcv.request.command != ARAD_PP_LEM_ACCESS_CMD_ACK)
  {
    ack_status->is_success = FALSE;
    /* Update ack status reason only in case it wasn't fill until now */
    if (ack_status->reason == ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS) 
    {
      ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_UNKNOWN;
    }
    
    ARAD_PP_DO_NOTHING_AND_EXIT;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_entry_add_unsafe()", 0, 0);
}

/*********************************************************************
 *     Remove an entry to the Exact match table.
 *********************************************************************/
uint32
  arad_pp_lem_access_entry_remove_unsafe(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_REQUEST         *request,
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS      *ack_status
  )
{
  uint32
    res = SOC_SAND_OK;
  ARAD_PP_LEM_ACCESS_OUTPUT
    request_hw,
    request_hw_rcv;
  uint8
    poll_success;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_ENTRY_REMOVE_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(ack_status);

  ARAD_PP_LEM_ACCESS_OUTPUT_clear(&request_hw_rcv);

  /* Convert the request into the hw request representation */
  arad_pp_lem_access_request_convert_to_hw(unit, request, NULL, &request_hw);

  if (request_hw.request.command != ARAD_PP_LEM_ACCESS_CMD_DELETE)
  {
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR, 25, exit);
  }

  /*
   *    Send the request
   */
  res = arad_pp_lem_request_send(
          unit,
          &request_hw,
          &poll_success
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

#ifdef CRASH_RECOVERY_SUPPORT
  /* In case we are in the "HW LOG Activated" state - don't check the hw for success.
   * It is useless because the Log wasn't written to the hw yet. the request is stored in the log until 'Commit' is performed.
   * Just skip the _answer_receive() part and assume Success. */
  if (SOC_IS_DONE_INIT(unit))
  {
    if (BCM_UNIT_DO_HW_READ_WRITE(unit))
    {
/*        LOG_ERROR(BSL_LS_SOC_HWLOG,
                (BSL_META_U(unit, "!!!!!!!!!!!!!!!: arad_pp_lem_access_entry_remove_unsafe.\n")));*/

        ack_status->is_success = TRUE;
        ack_status->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;
        ARAD_PP_DO_NOTHING_AND_EXIT;
    }
  }
#endif /* CRASH_RECOVERY_SUPPORT */

  /*
   *    Case of an unsent request
   */
  if (poll_success == FALSE)
  {
    ack_status->is_success = FALSE;
    ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_REQUEST_NOT_SENT;
    ARAD_PP_DO_NOTHING_AND_EXIT;
  }

  /*
   *    Receive the answer
   */

  /* if was no request for reply we are done, assuming successfully */
  if(request_hw.request.stamp == ARAD_PP_LEM_STAMP_INVALID) {
      ack_status->is_success = 1;
      ack_status->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;
      goto exit;
  }

  res = arad_pp_lem_request_answer_receive(
          unit,
          &request_hw_rcv,
          ack_status
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

  if (request_hw_rcv.request.command != ARAD_PP_LEM_ACCESS_CMD_ACK)
  {
    ack_status->is_success = FALSE;
    ack_status->reason = ARAD_PP_LEM_ACCESS_FAIL_REASON_UNKNOWN;
    ARAD_PP_DO_NOTHING_AND_EXIT;
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_frwrd_mact_entry_remove_unsafe()", 0, 0);
}

uint32
  arad_pp_lem_access_sw_entry_by_key_get_unsafe(
    SOC_SAND_IN  int                            unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY         *key,
    SOC_SAND_IN  int                            is_cr_shadow,
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_PAYLOAD     *payload,
    SOC_SAND_OUT uint8                          *is_found
  )
{
    ARAD_PP_LEM_ACCESS_KEY_ENCODED      key_in_buffer;
    uint32                              key_in[SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S],
                                        data_out[ARAD_PP_LEM_ACCESS_PAYLOAD_IN_UINT32S];
    uint8                               success;    
    uint32                              res = SOC_SAND_OK;


    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_SW_ENTRY_BY_KEY_GET_UNSAFE);

/*
    LOG_ERROR(BSL_LS_SOC_HWLOG,
            (BSL_META_U(unit,
                    "!!!!!!!!!!!!!!!: arad_pp_lem_access_sw_entry_by_key_get_unsafe\n")));
*/

    ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);

    /*
     *    Convert the key to buffer
     */
    res = arad_pp_lem_key_encoded_build(
            unit,
            key,
            0,
            &key_in_buffer
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);


    /* Read the key from CACHE */
    soc_sand_os_memset(key_in, 0x0, ARAD_CHIP_SIM_LEM_KEY);
    soc_sand_os_memset(data_out, 0x0, ARAD_CHIP_SIM_LEM_PAYLOAD);

    key_in[0] = key_in_buffer.buffer[0];
    key_in[1] = key_in_buffer.buffer[1];
    key_in[2] = key_in_buffer.buffer[2];

    res = chip_sim_exact_match_entry_get_unsafe(unit, (is_cr_shadow?(ARAD_CHIP_SIM_LEM_CR_BASE):(ARAD_CHIP_SIM_LEM_BASE)),
            key_in, ARAD_CHIP_SIM_LEM_KEY, data_out, ARAD_CHIP_SIM_LEM_PAYLOAD,
            &success);
    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

    if (is_cr_shadow) {
        if (success) {
            if (SHR_BITGET(data_out, ARAD_CHIP_SIM_LEM_CR_DELETED_ENTRY_INDICATION_BIT)) {
                *is_found = ARAD_PP_LEM_CR_SHADOW_DELETED_ENTRY;
            }
            else {
                *is_found =TRUE;
            }
        }
        else {
            *is_found = FALSE;
        }
    }
    else {
        *is_found = success;
    }
    
    res = arad_pp_lem_access_payload_parse(unit, data_out, key->type, payload);
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sw_entry_by_key_get_unsafe()", 0, 0);

}

uint32
  arad_pp_lem_access_get_entry_type_from_key_unsafe(
    SOC_SAND_IN  int                                unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY             *key
  )
{
    uint32 entry_type;

    if ((key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH)
            || (key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ))
    {
        entry_type = ARAD_PP_LEM_ENTRY_TYPE_FORMAT_3;
    }
    /* set entry type as host format so we can check the msb for format 3b (for raw application) */
    else if (key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST)
    {
        entry_type = ARAD_PP_LEM_ENTRY_TYPE_HOST_FORMAT;
    }else if (key->type == ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_BASE)
    {
        entry_type = ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA;
    }
    else
    {
        entry_type = ARAD_PP_LEM_ENTRY_TYPE_UNKNOWN;
    }
    return entry_type;
}
/*********************************************************************
 *     Get an entry in the Exact match table according to its key
 *********************************************************************/
uint32
  arad_pp_lem_access_entry_by_key_get_unsafe(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY             *key,
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_PAYLOAD         *payload,
    SOC_SAND_OUT uint8                        *is_found
  )
{
  uint32
    fld_val,
    res = SOC_SAND_OK;
  ARAD_PP_LEM_ACCESS_KEY_ENCODED
    key_in_buffer;
  soc_reg_above_64_val_t
    reg_above_64_val;
  uint64
    fld_val64,
    reg_val1;
  uint32
      pld_val[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S];

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_ENTRY_BY_KEY_GET_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(key);
  SOC_SAND_CHECK_NULL_INPUT(payload);
  SOC_SAND_CHECK_NULL_INPUT(is_found);


#ifdef CRASH_RECOVERY_SUPPORT
    /* In case we are in the "HW LOG Activated" state - There is no reason to read the hw.
     * It is useless because the Log wasn't written to the hw yet. the request is stored in the log until 'Commit' is performed.
     * Thus we will read the Cache, assuming it was updated on the "request Send"*/
    if (SOC_IS_DONE_INIT(unit))
    {
        if (BCM_UNIT_DO_HW_READ_WRITE(unit))
        {
            res = arad_pp_lem_access_sw_entry_by_key_get_unsafe(unit, key, TRUE /* CR transaction shadow */, payload, is_found);
            SOC_SAND_CHECK_FUNC_RESULT(res, 200, exit);


            if (*is_found == ARAD_PP_LEM_CR_SHADOW_DELETED_ENTRY)
            {
                *is_found = FALSE;
            }         
            else if (! *is_found)     /* Not found in transaction shadow - read directly from HW */
            {
                soc_hw_set_immediate_hw_access(unit);
                res = arad_pp_lem_access_entry_by_key_get_unsafe(unit, key, payload, is_found);
                soc_hw_restore_immediate_hw_access(unit);
            }

            ARAD_PP_DO_NOTHING_AND_EXIT;
        }
    }

#endif /* CRASH_RECOVERY_SUPPORT */


  ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);

  /*
   *    Verify the inputs
   */
  ARAD_PP_LEM_ACCESS_KEY_verify(unit, key);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  /*
   *    Write the key
   */
  res = arad_pp_lem_key_encoded_build(
          unit,
          key,
          0,
          &key_in_buffer
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  reg_above_64_val[0] = key_in_buffer.buffer[0];
  reg_above_64_val[1] = key_in_buffer.buffer[1];
  reg_above_64_val[2] = key_in_buffer.buffer[2];

  SOC_SAND_SOC_IF_ERROR_RETURN(res, 1200, exit, WRITE_PPDB_B_LARGE_EM_DIAGNOSTICS_KEYr(unit, reg_above_64_val));

  /*
   *    Set the trigger
   */
  fld_val = 0x1;
  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, PPDB_B_LARGE_EM_DIAGNOSTICSr, REG_PORT_ANY, 0, LARGE_EM_DIAGNOSTICS_LOOKUPf,  fld_val));



#ifdef PLISIM
  if (!SAL_BOOT_PLISIM) 
#endif
  {
  /*
   *    Poll on the trigger bit before getting the result
   */
   res = arad_polling(
          unit,
          ARAD_TIMEOUT,
          ARAD_MIN_POLLS,
          PPDB_B_LARGE_EM_DIAGNOSTICSr,
          REG_PORT_ANY,
          0,
          LARGE_EM_DIAGNOSTICS_LOOKUPf,
          0
        );
   SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  

  /*
   *    Get the lookup result
   */
  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  60,  exit, ARAD_REG_ACCESS_ERR,READ_PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr(unit, &reg_val1));

  ARAD_FLD_FROM_REG64(PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr, LARGE_EM_ENTRY_FOUNDf, fld_val64, reg_val1, 80, exit);
  *is_found = SOC_SAND_NUM2BOOL(COMPILER_64_LO(fld_val64));


#ifdef PLISIM
  if (!SAL_BOOT_PLISIM) 
#endif
  {
      if (!*is_found)
      {
        goto exit;
      }
  }

  ARAD_FLD_FROM_REG64(PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr, LARGE_EM_ENTRY_PAYLOADf, fld_val64, reg_val1, 90, exit);
  pld_val[0] = COMPILER_64_LO(fld_val64);
  pld_val[1] = COMPILER_64_HI(fld_val64);


  res = arad_pp_lem_access_payload_parse(unit,pld_val,key->type, payload);
  SOC_SAND_CHECK_FUNC_RESULT(res, 107, exit);

/* AEZ
  payload->dest = (COMPILER_64_LO(fld_val64)) % ARAD_PP_LEM_ACCESS_DEST_NOF_BITS;
  payload->asd = (COMPILER_64_LO(fld_val64) >> ARAD_PP_LEM_ACCESS_DEST_NOF_BITS) 
                    + (COMPILER_64_HI(fld_val64) << ARAD_PP_LEM_ACCESS_ASD_FIRST_REG_LSB);
*/
  /* ARAD_FLD_FROM_REG64(PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr, large_em_entry_payload_is_dyn, fld_val, reg_val2, 115, exit); */
/* AEZ
  payload->is_dynamic = SOC_SAND_NUM2BOOL(COMPILER_64_HI(fld_val64) >> ARAD_PP_LEM_ACCESS_ASD_SCND_REG_MSB);
*/
  ARAD_FLD_FROM_REG64(PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr, LARGE_EM_ENTRY_AGE_STATf, fld_val64, reg_val1, 120, exit);
  res = arad_pp_lem_access_age_fld_get(
          unit,
          COMPILER_64_LO(fld_val64),
          &(payload->age)
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 130, exit);

  ARAD_FLD_FROM_REG64(PPDB_B_LARGE_EM_DIAGNOSTICS_LOOKUP_RESULTr, LARGE_EM_ENTRY_ACCESSEDf, fld_val64, reg_val1, 130, exit);
  if(COMPILER_64_LO(fld_val64)){
      payload->flags |= ARAD_PP_FWD_DECISION_PARSE_ACCESSED;
  }
  }
#ifdef PLISIM
  if (SAL_BOOT_PLISIM) {
      res = arad_pp_lem_access_sw_entry_by_key_get_unsafe(unit, key, SOC_CR_ENABALED(unit), payload, is_found);
      SOC_SAND_CHECK_FUNC_RESULT(res, 140, exit);

   }
#endif


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_entry_by_key_get_unsafe()", 0, 0);
}

/*********************************************************************
 *     parse/build ASD field for SA-Auth usages
 *********************************************************************/
uint32
  arad_pp_lem_access_sa_based_asd_build(
    SOC_SAND_IN  int                       unit,
    SOC_SAND_IN  SOC_PPC_LLP_SA_AUTH_MAC_INFO      *auth_info,
    SOC_SAND_IN  SOC_PPC_LLP_VID_ASSIGN_MAC_INFO   *vid_assign_info,
    SOC_SAND_OUT uint32                        *asd
  )
{
  uint32
    tmp;
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_SA_BASED_ASD_BUILD);

  SOC_SAND_CHECK_NULL_INPUT(asd);

  if(auth_info && vid_assign_info)
  {
    if (auth_info->expect_tag_vid != SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_VIDS &&
        auth_info->expect_tag_vid != vid_assign_info->vid
       )
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_SA_BASED_ILLEGAL_VID_ERR, 5, exit);
    }
  }

  *asd = 0;
  /* if accept all vid then nothing to set, vid-check is disabled by default (0)*/
  /* if accept specific VID then 1. set this vid 2. enable-vid-check*/
  if (auth_info)
  {
    if (auth_info->expect_tag_vid != SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_VIDS)
    {
      /* check VID */
      tmp = 1;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_DROP_DIF_VID_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_DROP_DIF_VID_LEN,
              asd
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
      /* set the VID value */
      res = soc_sand_bitstream_set_any_field(
              &(auth_info->expect_tag_vid),
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LEN,
              asd
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }

    /* if accept all port then set to perform no check, port-check is enabled by default (0)*/
    /* if accept  traffic from specific PORT then nothing to do. port will be set in destination field*/
    if (auth_info->expect_system_port.sys_id == SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_PORTS || SOC_IS_ARAD_A0(unit))
    {
      /* permit all PORTs */
      tmp = 1;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_PRMT_ALL_PORTS_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_PRMT_ALL_PORTS_LEN,
              asd
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);
    }

    /* if accept all (including untagged) then set bit in order to accept untagged*/
    if (!auth_info->tagged_only)
    {
      /* accept untagged */
      tmp = 1;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_ACCPT_UNTGD_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_ACCPT_UNTGD_LEN,
              asd
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
    }
  }
  if (vid_assign_info)
  {
    tmp = SOC_SAND_BOOL2NUM(vid_assign_info->override_tagged);
    res = soc_sand_bitstream_set_any_field(
            &(tmp),
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_TAGGED_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_TAGGED_LEN,
            asd
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);

    tmp = SOC_SAND_BOOL2NUM(vid_assign_info->use_for_untagged);
    res = soc_sand_bitstream_set_any_field(
            &(tmp),
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_UNTAGGED_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_UNTAGGED_LEN,
            asd
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 80, exit);
    /* possible when copied from auth-info, to keep no change */
    if (vid_assign_info->vid != SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_VIDS)
    {
      tmp = vid_assign_info->vid;
      res = soc_sand_bitstream_set_any_field(
              &(tmp),
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LEN,
              asd
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);
    }
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sa_based_asd_build()", 0, 0);
}

/*********************************************************************
 *     parse/build ASD field for trill-SA-Auth usages
 *********************************************************************/
uint32
  arad_pp_lem_access_trill_sa_based_asd_build(
    SOC_SAND_IN  int                         unit,
    SOC_SAND_IN  SOC_PPC_TRILL_ADJ_INFO              *auth_info,
    SOC_SAND_OUT uint32                          *asd
  )
{
  uint32
    asd_lcl[1],
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_CHECK_NULL_INPUT(asd);
  
  *asd_lcl = 0;
  
  /* set the EEP value */
  res = soc_sand_bitstream_set_any_field(
          &(auth_info->expect_adjacent_eep),
          ARAD_PP_LEM_ACCESS_ASD_TRILL_SA_AUTH_EEP_LSB,
          ARAD_PP_LEM_ACCESS_ASD_TRILL_SA_AUTH_EEP_LEN(unit),
          asd_lcl
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);


  *asd = *asd_lcl;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_trill_sa_based_asd_build()", 0, 0);
}

uint32
  arad_pp_lem_access_sa_based_asd_parse(
    SOC_SAND_IN  int                       unit,
    SOC_SAND_IN  uint32                        asd,
    SOC_SAND_OUT SOC_PPC_LLP_SA_AUTH_MAC_INFO      *auth_info,
    SOC_SAND_OUT SOC_PPC_LLP_VID_ASSIGN_MAC_INFO   *vid_assign_info
  )
{
  uint32
    tmp;
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_PP_LEM_ACCESS_SA_BASED_ASD_PARSE);

  /* if auth_info null then don't fill it*/
  if(auth_info)
  {
    /* is check VID? */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            &asd,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_DROP_DIF_VID_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_DROP_DIF_VID_LEN,
            &tmp
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    /* if set then specific VID, otherwise ALL VIDs are permitted */
    if (!tmp)
    {
      auth_info->expect_tag_vid = SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_VIDS;
    }
    else /* specific VID value, get it! */
    {
      auth_info->expect_tag_vid = 0;
      /* set the VID value */
      res = soc_sand_bitstream_get_any_field(
              &asd,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LSB,
              ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LEN,
              &(auth_info->expect_tag_vid)
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }

    /* port */
    /* is permit all PORTs */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            &asd,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_PRMT_ALL_PORTS_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_PRMT_ALL_PORTS_LEN,
            &(tmp)
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    if (tmp)/* if set then permit all ports, other wise only the given port, get it from dest */
    {
      auth_info->expect_system_port.sys_id = SOC_PPC_LLP_SA_AUTH_ACCEPT_ALL_PORTS;
    }
    else
    {
      auth_info->expect_system_port.sys_id = 0; /* mark to get specific port*/
    }

    /* accept tagged*/
    /* is accept untagged */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            &(asd),
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_ACCPT_UNTGD_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_ACCPT_UNTGD_LEN,
            &(tmp)
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
    if (tmp) /* accept untagged ALSO */
    {
      auth_info->tagged_only = FALSE;
    }
    else
    {
      auth_info->tagged_only = TRUE;
    }
  }
  /* if vid_assign_info null then don't fill it*/
  if(vid_assign_info)
  {
    /* use VID in tagged packets */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            &asd,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_TAGGED_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_TAGGED_LEN,
            &tmp
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);
    vid_assign_info->override_tagged = SOC_SAND_NUM2BOOL(tmp);

    /* use VID in untagged packets */
    tmp = 0;
    res = soc_sand_bitstream_get_any_field(
            &asd,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_UNTAGGED_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_OVR_VID_IN_UNTAGGED_LEN,
            &tmp
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 80, exit);
    vid_assign_info->use_for_untagged = SOC_SAND_NUM2BOOL(tmp);

    /* set the VID value */
    res = soc_sand_bitstream_get_any_field(
            &asd,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LSB,
            ARAD_PP_LEM_ACCESS_ASD_SA_AUTH_VID_LEN,
            &(vid_assign_info->vid)
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sa_based_asd_parse()", 0, 0);
}


uint32
  arad_pp_lem_access_trill_sa_based_asd_parse(
    SOC_SAND_IN  int                       unit,
    SOC_SAND_IN  uint32                        asd,
    SOC_SAND_OUT SOC_PPC_TRILL_ADJ_INFO            *auth_info
  )
{
  uint32
    tmp;
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(auth_info);


  /* eep*/
  tmp = 0;
  res = soc_sand_bitstream_get_any_field(
          &(asd),
          ARAD_PP_LEM_ACCESS_ASD_TRILL_SA_AUTH_EEP_LSB,
          ARAD_PP_LEM_ACCESS_ASD_TRILL_SA_AUTH_EEP_LEN(unit),
          &(tmp)
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  auth_info->expect_adjacent_eep = tmp;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_pp_lem_access_sa_based_asd_parse()", 0, 0);
}

uint32
  arad_pp_lem_trill_mc_key_build_set(
    SOC_SAND_IN  int           unit,
    SOC_SAND_IN  uint8           mask_adjacent_nickname,
    SOC_SAND_IN  uint8           mask_fid,
    SOC_SAND_IN  uint8           mask_ing_nickname
  )
{
return 0;

}

uint32
  arad_pp_lem_trill_mc_key_build_get(
    SOC_SAND_IN   int           unit,
    SOC_SAND_OUT  uint8           *mask_adjacent_nickname,
    SOC_SAND_OUT  uint8           *mask_fid,
    SOC_SAND_OUT  uint8           *mask_ing_nickname
  )
{
return 0;
}


/*
 *  Clear functions
 */
void
  ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY_ENCODED *info
  )
{
  uint32
    indx;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY_ENCODED));
  for (indx = 0; indx < SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S; ++indx)
  {
    info->buffer[indx] = 0;
  }

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_KEY_PARAM_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY_PARAM *info
  )
{
  uint32
    indx;

  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY_PARAM));
  for (indx = 0; indx < ARAD_PP_LEM_KEY_PARAM_MAX_IN_UINT32S_MAX; ++indx)
  {
    info->value[indx] = 0;
  }
  info->nof_bits = 0;

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_KEY_PREFIX_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY_PREFIX *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY_PREFIX));

  info->value = 0;
  info->nof_bits = 0;

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_KEY_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_KEY));
  info->type = ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES;
  /* Remove unnecessary clear for performance 
  for (indx = 0; indx < ARAD_PP_LEM_KEY_MAX_NOF_PARAMS; ++indx)
  {
    ARAD_PP_LEM_ACCESS_KEY_PARAM_clear(&(info->param[indx]));
  }
  info->nof_params = 0;
  ARAD_PP_LEM_ACCESS_KEY_PREFIX_clear(&(info->prefix));

  SOC_SAND_MAGIC_NUM_SET; 
  */ 
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_REQUEST_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_REQUEST *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_REQUEST));
  ARAD_PP_LEM_ACCESS_KEY_clear(&(info->key));
  info->command = ARAD_PP_LEM_ACCESS_NOF_CMDS;
  info->stamp = 0;
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_PAYLOAD_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_PAYLOAD *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_PAYLOAD));
  info->dest = 0;
  info->age = 0;
  info->asd = 0;
  info->is_dynamic = 0;
    info->flags = ARAD_PP_FWD_DECISION_PARSE_DEST;
    info->has_cw = 0;
    info->is_dynamic = 0;
    info->sa_drop = 0;
    info->tpid_profile = 0;
  info->native_vsi = 0;
  info->is_learn_data = 0;
  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_OUTPUT_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_OUTPUT  *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_OUTPUT));
  ARAD_PP_LEM_ACCESS_REQUEST_clear(&(info->request));
  ARAD_PP_LEM_ACCESS_PAYLOAD_clear(&(info->payload));
  info->is_learned_first_by_me = 0;
  info->req_origin = ARAD_PP_LEM_ACCCESS_NOF_REQ_ORIGINS;

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_ACK_STATUS_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_ACK_STATUS  *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_ACK_STATUS));
  info->is_success = 0;
  info->reason = ARAD_PP_LEM_ACCESS_NOF_FAIL_REASONS;

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_ACCESS_BUFFER_clear(
    SOC_SAND_OUT ARAD_PP_LEM_ACCESS_BUFFER *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_ACCESS_BUFFER));

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  ARAD_PP_LEM_BFD_ONE_HOP_ENTRY_INFO_clear(
    SOC_SAND_OUT ARAD_PP_LEM_BFD_ONE_HOP_ENTRY_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  soc_sand_os_memset(info, 0x0, sizeof(ARAD_PP_LEM_BFD_ONE_HOP_ENTRY_INFO));

  SOC_SAND_MAGIC_NUM_SET;
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}



uint32
  ARAD_PP_LEM_ACCESS_KEY_PARAM_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN ARAD_PP_LEM_ACCESS_KEY_TYPE   type,
    SOC_SAND_IN ARAD_PP_LEM_ACCESS_KEY_PARAM *info,
    SOC_SAND_IN uint32                   params_ndx
  )
{
  int nof_bits;
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_SAND_ERR_IF_ABOVE_MAX(type, ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES-1, ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 10, exit);

  if ((type >= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE) && (type <= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE)) {
      goto exit; /* skip validations manage by DBAL */
  }

  switch(type)
  {
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC:
    if (params_ndx == 0)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 11, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 12, exit);
    }
    else  /* params_ndx == 1 */
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 13, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 14, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 15, exit);
    }
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC:
    if (params_ndx == 0)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 16, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_BACKBONE_MAC - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 17, exit);
    }
    else  /* params_ndx == 1 */
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 18, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_BACKBONE_MAC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 19, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 20, exit);
    }
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_MC:
    /* In case the SSM isn't disable the LEM IPV4 MC entry is implemented in the DBAL and the verifier need to be modify accordingly*/
    if (params_ndx == 0)
    {
      nof_bits = (SOC_IS_ARADPLUS(unit)) ? ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_MC_DBAL : ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_MC;
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, nof_bits, nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 21, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << nof_bits)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 22, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 23, exit);
    }
    else if (params_ndx == 1)  /* params_ndx == 1 */
    {
      nof_bits = (SOC_IS_ARADPLUS(unit)) ? ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_MC_DBAL : ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_MC;
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, nof_bits, nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 24, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << nof_bits)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 25, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 26, exit);
    }
    else /* params_ndx == 2 (DBAL only) */
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IPV4_MC_DBAL, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IPV4_MC_DBAL, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 326, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IPV4_MC_DBAL)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 327, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 328, exit);
    }
    break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST:
    if (params_ndx == 0)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 27, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 29, exit);
    }
    else  /* params_ndx == 1 */
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 30, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 33, exit);
    }
    break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 11, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 12, exit);
     }
   break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 11, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_ADJ - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 12, exit);
     }
     else if (params_ndx == 1)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 11, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_ADJ)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 12, exit);
     }
     break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_UC:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 43, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_UC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 44, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 47, exit);
     }
     break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_MC:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 43, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_TRILL_MC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 44, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 45, exit);
     }
     else if (params_ndx == 1)
     {       
       SOC_SAND_ERR_IF_ABOVE_MAX(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 46, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_TRILL_MC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 47, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 48, exit);
     }
     else if (params_ndx == 2)
     {       
       SOC_SAND_ERR_IF_ABOVE_MAX(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_TRILL_MC(unit), ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 48, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_TRILL_MC(unit))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 50, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 51, exit);
     }
     else if (params_ndx == 3)
     {       
       SOC_SAND_ERR_IF_ABOVE_MAX(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_TRILL_MC(unit), ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 51, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_TRILL_MC(unit))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 52, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 53, exit);
     }
  
     break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_EXTENDED:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_L2_LIF_KEY_TYPE_EXTENDED_PARAM0_NOF_BITS, ARAD_PP_L2_LIF_KEY_TYPE_EXTENDED_PARAM0_NOF_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 45, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_L2_LIF_KEY_TYPE_EXTENDED_PARAM0_NOF_BITS - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 65, exit);
     }
     break;


  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT:
   if (params_ndx == 0)
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 54, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_D_ID_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 55, exit);
   }
   else  /* params_ndx == 1 */
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 52, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_VFI_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 53, exit);
   }
   break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL:
   if (params_ndx == 0)
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 54, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_D_ID_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 55, exit);
   }
   else  /* params_ndx == 1 */
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 52, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_VFI_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 53, exit);
   }
   break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE:
   if (params_ndx == 0)
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_D_ID_ROUTE_BITS, ARAD_PP_FRWRD_FCF_D_ID_ROUTE_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 54, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_D_ID_ROUTE_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 55, exit);
   }
   else  /* params_ndx == 1 */
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 52, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_VFI_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 53, exit);
   }
   break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING:
   if (params_ndx == 0)
   {
     SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_S_ID_BITS, ARAD_PP_FRWRD_FCF_S_ID_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 58, exit);
     SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_S_ID_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 59, exit);
   }
   else if (params_ndx == 1)
   {
     SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_FRWRD_FCF_D_ID_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 60, exit);
     SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_D_ID_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 61, exit);
   }
   else  /* params_ndx == 2 */
   {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_FRWRD_FCF_VFI_BITS, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 56, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_FRWRD_FCF_VFI_BITS)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 57, exit);
   }
   break;

   case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP:
     if (params_ndx == 0)
     {
      if(SOC_IS_JERICHO(unit) ) {
          SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 81, exit);       
          SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 76, exit);
      }
      else{
          SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 81, exit);       
          SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_SPOOF_DHCP - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 83, exit);
      }
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 85, exit);       
     }
     else  if (params_ndx == 1 )
     {
       if(SOC_IS_JERICHO(unit) ) {
           SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 73, exit);
           SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP_JERICHO - 32))-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 83, exit);
       }
       else{
           SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 73, exit);
          SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP_SPOOF_DHCP)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 76, exit);
        }
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 79, exit);
     }
     else if ( params_ndx == 2 )
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 71, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_IP_SPOOF_DHCP) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 72, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 73, exit);
     }     
     break;
   case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_SPOOF_STATIC:
     if (params_ndx == 0)
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 71, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 79, exit);
     }
     else  if (params_ndx == 1 )
     {
       SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_SPOOF_STATIC(unit), ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_SPOOF_STATIC(unit), ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 73, exit);              
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IPV4_SPOOF_STATIC(unit)) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 72, exit);
       SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 73, exit);
      }
      break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC_IN_MAC_TUNNEL:
      if (params_ndx == 0) {
          if (!SOC_DPP_CONFIG(unit)->pp.test2) {
              SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 181, exit);
              SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], (1 << (ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_MAC_IN_MAC_TUNNEL - 32)) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 183, exit);
              SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 185, exit);
          }
      } else  if (params_ndx == 1) {
          if (!SOC_DPP_CONFIG(unit)->pp.test2) {
              SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 187, exit);
              SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_MAC_IN_MAC_TUNNEL) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 189, exit);
              SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 192, exit);
          }
      } else if (params_ndx == 2) {
          if (!SOC_DPP_CONFIG(unit)->pp.test2) {
              if (SOC_IS_JERICHO(unit) && !SOC_IS_QAX(unit)) {
                  SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 195, exit);
                  SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM3_IN_BITS_FOR_MAC_IN_MAC_TUNNEL) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 194, exit);
                  SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 196, exit);
              } else {
                  SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 195, exit);
                  SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_MAC_IN_MAC_TUNNEL) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 198, exit);
                  SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 201, exit);
              }
          }
      }
      break; 
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC:
    if (params_ndx == 0)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 210, exit);       
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_GLOBAL_IPV4_MC)-1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 211, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 212, exit);       
    }
    else  if (params_ndx == 1)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 213, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 214, exit);
    }
    else if ( params_ndx == 2 )
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 215, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM2_IN_BITS_FOR_GLOBAL_IPV4_MC) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 216, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 73, exit);
    }     
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_ILM:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPMC_BIDIR:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_STATISTICS:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IVL_LEARN:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_ECHO:
  case ARAD_PP_LEM_ACCESS_KEY_LSR_CUNT:

      /* no check*/
  break;  
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC:
    if (params_ndx == 0)
    {
      /* 40bits */
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 220, exit);       
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], 0xFFFFFFFF, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 221, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0xFFFFFFFF, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 222, exit);  
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 223, exit); 
    }
    else  if (params_ndx == 1)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 224, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_IP6_SPOOF_STATIC) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 225, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 226, exit);
    } 
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_COMPRESSION_DIP:
    if (params_ndx == 0)
    {
      /* 40bits */
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 230, exit);       
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], 0xFFFFFFFF, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 231, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0xFFFFFF, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 232, exit);  
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[2], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 233, exit); 
    }
    else  if (params_ndx == 1)
    {
      SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PARAM_NOF_BITS_OUT_OF_RANGE_ERR, 234, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[0], (1 << ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_DIP6_COMPRESSION) - 1, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 235, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->value[1], 0, ARAD_PP_LEM_ACCESS_KEY_PARAM_VALUE_OUT_OF_RANGE_ERR, 236, exit);
    } 
    break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 60, exit);
  }

  /* SOC_SAND_MAGIC_NUM_VERIFY(info); */

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in ARAD_PP_LEM_ACCESS_KEY_PARAM_verify()",type,params_ndx);
}

uint32
  ARAD_PP_LEM_ACCESS_KEY_PREFIX_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN ARAD_PP_LEM_ACCESS_KEY_TYPE type,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY_PREFIX *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_SAND_ERR_IF_ABOVE_MAX(type, ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES-1, ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 10, exit);

  if ((type >= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE) && (type <= ARAD_PP_FLP_DYNAMIC_DBAL_KEY_OR_MASK_BASE)) {
      goto exit; /* skip validations manage by DBAL */
  }


  switch(type)
  {
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC:
    if (info->nof_bits != ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC)
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 11, exit);
    }
    /*if (info->value != ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC)
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 12, exit);
    }*/
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 13, exit);
    /*SOC_SAND_ERR_IF_NOT_EQUALS_VALUE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_BACKBONE_MAC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 14, exit);*/
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_MC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 15, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 16, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 17, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_IP_HOST, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 18, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit), ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_SA_AUTH(unit), ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 19, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SA_AUTH, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_SA_AUTH, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 20, exit);
  break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 19, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 20, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_ILM:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_ILM, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_ILM, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 17, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_ILM, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_ILM, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 18, exit);
    break;
  
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_EXTENDED:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_EXTENDED, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_EXTENDED, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 17, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_EXTENDED, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_EXTENDED, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 18, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_UC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 19, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 20, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_MC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 19, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_TRILL_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 20, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 21, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 22, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 21, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL_N_PORT, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_LOCAL_N_PORT, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 22, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 23, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_REMOTE, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_REMOTE, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 24, exit);
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 25, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_ZONING, ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_FC_ZONING, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 26, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 27, exit);
    /*SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_IPV4_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PREFIX_IPV4_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 28, exit);
*/    
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_SPOOF_STATIC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 29, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 30, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC_IN_MAC_TUNNEL:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 35, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PREFIX_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 40, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 36, exit);  
    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPMC_BIDIR:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_STATISTICS:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IVL_LEARN:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_ECHO:
  case ARAD_PP_LEM_ACCESS_KEY_LSR_CUNT:

    break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_COMPRESSION_DIP:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_bits, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS_FOR_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PREFIX_NOF_BITS_OUT_OF_RANGE_ERR, 37, exit);
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->value, ARAD_PP_LEM_ACCESS_KEY_PREFIX_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PREFIX_DIP6_COMPRESSION, ARAD_PP_LEM_ACCESS_KEY_PREFIX_PREFIX_OUT_OF_RANGE_ERR, 38, exit); 
    break;
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 500, exit);
  }

  /* SOC_SAND_MAGIC_NUM_VERIFY(info); */

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in ARAD_PP_LEM_ACCESS_KEY_PREFIX_verify()",0,0);
}

uint32
  ARAD_PP_LEM_ACCESS_KEY_verify(
     SOC_SAND_IN  int                 unit,
     SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY *info
  )
{
  uint32
    res = SOC_SAND_OK;
  uint32
    params_ndx;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_SAND_ERR_IF_ABOVE_MAX(info->type, ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES-1, ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 10, exit);

  if ((info->type >= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_BASE) && (info->type <= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_END)) {
      goto exit;/* skip validations manage by DBAL */
  }

  switch(info->type)
  {
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_MC:
      /* In case the SSM isn't disable the LEM IPV4 MC entry is implemented in the DBAL and the verifier need to be modify accordingly*/
      if(SOC_IS_ARADPLUS(unit)) {
          SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_DBAL_IPV4, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_DBAL_IPV4, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 11, exit);
      } else {
          SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_IPV4, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_IPV4, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 11, exit);
      }
      break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST:

    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_IPV4, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_MAC_BMAC_IPV4, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 11, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_ILM:

    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_ILM_MIN, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_ILM_MAX, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 11, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_EXTENDED:
    SOC_SAND_ERR_IF_ABOVE_MAX(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_TYPE_NOF_PARAMS_EXTEND_P2P, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 11, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH:
      /* Trill b0 two params otherwise one parameter
         */
     SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_SA_AUTH, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_SA_AUTH_ARAD_B0, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 12, exit);
     break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ:
     SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_ADJ, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 12, exit);
     break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_UC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_UC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 13, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_MC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_MC(unit), ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_TRILL_MC(unit), ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 14, exit);
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 15, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_LOCAL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 15, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_REMOTE, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_REMOTE, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 16, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_ZONING, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_FC_ZONING, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 17, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_SPOOF_DHCP, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 18, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_SPOOF_STATIC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPV4_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 19, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC_IN_MAC_TUNNEL:
    if (SOC_DPP_CONFIG(unit)->pp.test2){
        SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TEST2, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TEST2, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 20, exit);
    } else {
        SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_MAC_IN_MAC_TUNNEL, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 20, exit);
    }
    break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPMC_BIDIR:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPMC_BIDIR, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IPMC_BIDIR, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 21, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_GLOBAL_IPV4_MC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 22, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP:
	  SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_BFD_SINGLE_HOP, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_BFD_SINGLE_HOP, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 22, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP6_SPOOF_STATIC, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 22, exit);
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_COMPRESSION_DIP:
    SOC_SAND_ERR_IF_OUT_OF_RANGE(info->nof_params, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_DIP6_COMPRSSION, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_DIP6_COMPRSSION, ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_OUT_OF_RANGE_ERR, 22, exit);
  break;

  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_STATISTICS:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_ECHO:
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IVL_LEARN:
  case ARAD_PP_LEM_ACCESS_KEY_LSR_CUNT:
 
      break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_PP_LEM_ACCESS_KEY_TYPE_OUT_OF_RANGE_ERR, 20, exit);
  }
  for (params_ndx = 0; params_ndx < info->nof_params; ++params_ndx)
  {
    res = ARAD_PP_LEM_ACCESS_KEY_PARAM_verify(unit, info->type, &(info->param[params_ndx]), params_ndx);
    SOC_SAND_CHECK_FUNC_RESULT(res, 20 + params_ndx, exit);
  }

  res = ARAD_PP_LEM_ACCESS_KEY_PREFIX_verify(unit, info->type, &(info->prefix));
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

  /* SOC_SAND_MAGIC_NUM_VERIFY(info); */

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in ARAD_PP_LEM_ACCESS_KEY_verify()",0,0);
}

uint32
  ARAD_PP_LEM_ACCESS_REQUEST_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_REQUEST *info
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  res = ARAD_PP_LEM_ACCESS_KEY_verify(unit, &(info->key));
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
  SOC_SAND_ERR_IF_ABOVE_MAX(info->command, ARAD_PP_LEM_ACCESS_CMD_MAX, ARAD_PP_LEM_ACCESS_CMD_OUT_OF_RANGE_ERR, 20, exit);
  SOC_SAND_ERR_IF_ABOVE_MAX(info->stamp, 0, ARAD_PP_LEM_STAMP_OUT_OF_RANGE_ERR, 30, exit);

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in ARAD_PP_LEM_ACCESS_REQUEST_verify()",0,0);
}

uint32
  ARAD_PP_LEM_ACCESS_PAYLOAD_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_PAYLOAD *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  if (info->flags != ARAD_PP_LEM_ENTRY_TYPE_RAW_DATA) {
      SOC_SAND_ERR_IF_ABOVE_MAX(info->dest, (1<<ARAD_PP_LEM_ACCESS_DEST_NOF_BITS)-1, ARAD_PP_LEM_DEST_OUT_OF_RANGE_ERR, 10, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->asd, (1<<ARAD_PP_LEM_ACCESS_ASD_NOF_BITS)-1, ARAD_PP_LEM_ASD_OUT_OF_RANGE_ERR, 20, exit);
      SOC_SAND_ERR_IF_ABOVE_MAX(info->age, SOC_PPC_FRWRD_MACT_ENTRY_KEY_AGE_STATUS_MAX+1, ARAD_PP_LEM_AGE_OUT_OF_RANGE_ERR, 30, exit);
  }  

  SOC_SAND_MAGIC_NUM_VERIFY(info);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in ARAD_PP_LEM_ACCESS_PAYLOAD_verify()",0,0);
}



#if SOC_PPC_DEBUG_IS_LVL1


  
const char*                                        
  ARAD_PP_LEM_ACCESS_KEY_TYPE_to_string(              
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY_TYPE enum_val
  )
{                                                
  const char* str = NULL;

  if ((enum_val >= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_BASE) && (enum_val <= ARAD_PP_LEM_ACCESS_KEY_TYPE_DBAL_END)) {
      str = "DYNAMIC DBAL TABLE";
      return str;
  }

  switch(enum_val)                               
  {                                              
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC:
    str = "MAC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BACKBONE_MAC:
    str = "BACKBONE_MAC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_MC:
    str = "IPV4_MC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_HOST:
    str = "IP_HOST";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_SA_AUTH:
    str = "SA_AUTH";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_ADJ:
    str = "TRILL_ADJ";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_ILM:
    str = "ILM";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_EXTENDED:
    str = "EXTENDED";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_UC:
    str = "TRILL_UC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_TRILL_MC:
    str = "TRILL_MC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP_SPOOF_DHCP:
    str = "IPV4_SPOOF_DHCP";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPV4_SPOOF_STATIC:
    str = "IPV4_SPOOF_STATIC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL:
    str = "FC_LOCAL";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_LOCAL_N_PORT:
    str = "FC_LOCAL_N_PORT";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_REMOTE:
    str = "FC_REMOTE";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_FC_ZONING:
    str = "FC_ZONING";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_MAC_IN_MAC_TUNNEL:
    str = "MAC_IN_MAC_AFTER_TERMINATION";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IPMC_BIDIR:
    str = "IPMC_BIDIR";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_GLOBAL_IPV4_MC:
    str = "GLOBAL_IPV4_MC";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_SPOOF_STATIC:
    str = "IP6_SPOOF_STATIC_HOST";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_IP6_COMPRESSION_DIP:
    str = "IP6_DIP_COMPRESSION";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP:
  str = "BFD_SINGLE_HOP";
  break;
  case ARAD_PP_LEM_ACCESS_KEY_TYPE_SLB:
  str = "SLB";
  break;  

  default:                                       
    str = "Unknown";                            
  }                                              
  return str;                                    
}                                                
                                                 

void
  ARAD_PP_LEM_ACCESS_KEY_print(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_KEY *info
  )
{
    uint16 indx;
    uint32
        hw_pref;
    ARAD_PP_LEM_ACCESS_KEY_ENCODED
        key_buffer;

    uint32 res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    SOC_SAND_CHECK_NULL_INPUT(info);

    LOG_CLI((BSL_META_U(unit,
                        "%-10s "), ARAD_PP_LEM_ACCESS_KEY_TYPE_to_string(info->type)));
    LOG_CLI((BSL_META_U(unit,
                        "|0x%02x"),info->prefix.value));

    arad_pp_lem_access_app_to_prefix_get(unit,info->prefix.value,&hw_pref);
    LOG_CLI((BSL_META_U(unit,
                        "(0x%02x)|"),hw_pref));
    
    res = arad_pp_lem_key_encoded_build(
             unit,
             info,
             0,
             &key_buffer
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    for(indx = 0; indx < SOC_DPP_DEFS_LEM_WIDTH_IN_UINT32S(unit); ++indx) {
        LOG_CLI((BSL_META_U(unit,
                            "%08x  "),key_buffer.buffer[SOC_DPP_DEFS_LEM_WIDTH_IN_UINT32S(unit)-indx-1]));
    }

exit:
    SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}


void
  ARAD_PP_LEM_ACCESS_PAYLOAD_print(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  ARAD_PP_LEM_ACCESS_PAYLOAD *info
  )
{
    uint16 indx;
    uint32                    
        payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S];
    uint32 res;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    SOC_SAND_CHECK_NULL_INPUT(info);

   
    res = arad_pp_lem_access_payload_build(
             unit,
             info,
             payload_data
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

    for(indx = 0; indx < ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S; ++indx) {
        LOG_CLI((BSL_META_U(unit,
                            "%08x  "),payload_data[ARAD_PP_LEM_ACCESS_PAYLOAD_NOF_UINT32S-indx-1]));
    }

exit:
    SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}


#endif

/*********************************************************************
*     Get the pointer to the list of procedures of the
 *     arad_pp_api_lem_access module.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
CONST SOC_PROCEDURE_DESC_ELEMENT*
  arad_pp_lem_access_get_procs_ptr(void)
{
  return Arad_pp_procedure_desc_element_lem_access;
}
/*********************************************************************
*     Get the pointer to the list of errors of the
 *     arad_pp_api_lem_access module.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
CONST SOC_ERROR_DESC_ELEMENT*
  arad_pp_lem_access_get_errs_ptr(void)
{
  return Arad_pp_error_desc_element_lem_access;
}

/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif /* of #if defined(BCM_88650_A0) */


/* Used for chipsim get block */
uint32
  arad_pp_lem_block_get(
     SOC_SAND_IN  int                                           unit,
/*     SOC_SAND_IN  SOC_PPC_FRWRD_MACT_TRAVERSE_MATCH_RULE        *rule,*/
     SOC_SAND_IN ARAD_PP_IHP_MACT_FLUSH_DB_TBL_DATA             *tbl_data,
     SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                  *block_range,
     SOC_SAND_OUT ARAD_PP_LEM_ACCESS_KEY                        *lem_keys,
     SOC_SAND_OUT ARAD_PP_LEM_ACCESS_PAYLOAD                    *payload,
     SOC_SAND_OUT uint32                                        *nof_entries
    )
{
    uint32      *key, *curr_key;
    uint32      *data_out, *curr_data;    
    uint32      res;
    int         i;
    ARAD_PP_LEM_ACCESS_KEY_ENCODED          key_in_buffer;

    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    key = NULL;
    data_out = NULL;

    key = (uint32*)sal_alloc(sizeof(uint32) * SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S * (block_range->entries_to_act), "arad_pp_lem_block_get");
    if (key == NULL)
    {
        SOC_SAND_SET_ERROR_CODE(SOC_PPD_ERR_OUT_OF_RESOURCES, 10, exit);
    }
    data_out= (uint32*)sal_alloc(sizeof(uint32) * ARAD_PP_LEM_ACCESS_PAYLOAD_IN_UINT32S * (block_range->entries_to_act), "arad_pp_lem_block_get");
    if (data_out == NULL)
    {
        SOC_SAND_SET_ERROR_CODE(SOC_PPD_ERR_OUT_OF_RESOURCES, 20, exit);
    }
    
    res = chip_sim_em_get_block(unit, ARAD_CHIP_SIM_LEM_BASE,  ARAD_CHIP_SIM_LEM_KEY, ARAD_CHIP_SIM_LEM_PAYLOAD,
                                (void *)tbl_data, key, data_out, nof_entries, block_range);

    SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);


    for (i=0; i < *nof_entries; i++) {
        curr_key = key +  (SOC_DPP_DEFS_MAX_LEM_WIDTH_IN_UINT32S * i);

        curr_data = data_out + (ARAD_PP_LEM_ACCESS_PAYLOAD_IN_UINT32S * i);
        res = arad_pp_lem_access_payload_parse(
            unit,
            curr_data,
            ARAD_PP_LEM_ACCESS_NOF_KEY_TYPES,
            &payload[i]
          );
        SOC_SAND_CHECK_FUNC_RESULT(res, 220, exit);

        ARAD_PP_LEM_ACCESS_KEY_ENCODED_clear(&key_in_buffer);

        key_in_buffer.buffer[0] = curr_key[0];
        key_in_buffer.buffer[1] = curr_key[1];
        key_in_buffer.buffer[2] = curr_key[2];

        res = arad_pp_lem_key_encoded_parse(
            unit,
            &key_in_buffer,
            &lem_keys[i]
          );
        SOC_SAND_CHECK_FUNC_RESULT(res, 230, exit);

    }
exit:
    if (key != NULL) sal_free(key);
    if (data_out != NULL) sal_free(data_out);
    SOC_SAND_EXIT_AND_SEND_ERROR("arad_pp_lem_block_get", 0, 0);
}


/* in ipv4/ipv6 single hop extended mode the LEM is used as the classifier
this function add an entry to the LEM: 
key: ttl[2B] + BFD.Your-Discriminator[4B] 
if trap_code==0, then setting the default values:
payload: BFD.Your-Discriminator[4B] + trap code-bcmRxTrapOamBfdIpv4 = 0xe3
or 
payload: BFD.Your-Discriminator[4B] + trap code-bcmRxTrapOamBfdIpv6 = 0xe8

else (trap_code!=0):
using the supplied trap code and strengths as payload.
*/ 

int arad_pp_lem_access_bfd_one_hop_lem_entry_add(int unit, const ARAD_PP_LEM_BFD_ONE_HOP_ENTRY_INFO * lem_entry) {

	uint32
		res;
	ARAD_PP_LEM_ACCESS_REQUEST
		request;
	ARAD_PP_LEM_ACCESS_PAYLOAD
		payload;
	ARAD_PP_LEM_ACCESS_ACK_STATUS
		ack;
	int i=0;
    uint32 trap_code = lem_entry->trap_code;
    uint32 fwd_strngth = lem_entry->fwd_strngth, snp_strength = lem_entry->snp_strength;
	ARAD_SOC_REG_FIELD	strength_fld_fwd;                                                              
	ARAD_SOC_REG_FIELD    strength_fld_snp; 
	SOC_PPC_TRAP_CODE_INTERNAL	internal_trap_code=0;	


	SOC_SAND_INIT_ERROR_DEFINITIONS(0);

	SOC_SAND_TAKE_DEVICE_SEMAPHORE;

	ARAD_PP_LEM_ACCESS_REQUEST_clear(&request);
	ARAD_PP_LEM_ACCESS_PAYLOAD_clear(&payload);

	request.command = ARAD_PP_LEM_ACCESS_CMD_INSERT;
	request.stamp = 0;

	request.key.type = ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP;
	request.key.nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_HOST;
    /* First instruction: local discriminator*/
	request.key.param[i].value[0] = lem_entry->local_discriminator;
    request.key.param[i].value[1] = 0; 
    request.key.param[i].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_HOST; /*32*/
    ++i;

/* Second instruction: TTL */
    request.key.param[i].value[0] = 0xff; /*ttl*/
    request.key.param[i].value[1] = 0; 
    request.key.param[i].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_SA_AUTH; /*8*/
    request.key.prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    request.key.prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BFD_SINGLE_HOP;
    ++i;

	/*extract HW trap code*/
    if (!trap_code) {
        if (lem_entry->is_ipv6) {
            if (lem_entry->is_accelerated) {
                res = sw_state_access[unit].dpp.bcm.bfd.trap_info.trap_ids.get(unit, SOC_PPC_BFD_TRAP_ID_UC_IPV6, &trap_code); 
                SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 212, exit_semaphore);
            } else {
                trap_code = SOC_PPC_TRAP_CODE_OAM_CPU;
            }
        } else {
            if (lem_entry->is_accelerated) {
                res = sw_state_access[unit].dpp.bcm.bfd.trap_info.trap_ids.get(unit, SOC_PPC_BFD_TRAP_ID_OAMP_IPV4, &trap_code); 
                SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 214, exit_semaphore);
            } else {
                trap_code = SOC_PPC_TRAP_CODE_OAM_CPU;
            }
        }
        fwd_strngth = _ARAD_PP_OAM_TRAP_STRENGTH;
        snp_strength = 0;
    }
	
	res = arad_pp_trap_mgmt_trap_code_to_internal(unit,trap_code, &internal_trap_code, &strength_fld_fwd, &strength_fld_snp);  	
	SOC_SAND_CHECK_FUNC_RESULT(res, 9999, exit_semaphore);

	payload.flags = ARAD_PP_FWD_DECISION_PARSE_OUTLIF;
    /* Destination[18:0] encoding: {5'd0,Snoop-strength[1:0],FWD-Strength[2:0],Trap-Code[7:0]} */
	payload.dest = (internal_trap_code | (fwd_strngth<<8) | (snp_strength<<(8+3)));
	if (lem_entry->is_ipv6) {
	    payload.asd = lem_entry->oam_id; 
	} else {
	    payload.asd = ((uint16)lem_entry->local_discriminator);
	}

    if (SOC_DPP_CONFIG(unit)->pp.bfd_ipv4_single_hop==2 && lem_entry->is_accelerated) {
        /* Shove the remote detect multiplier in the rest of the payload. Start after the OAM-ID (13/14 bits, device dependent.)*/
        uint32 remote_detect_mult = lem_entry->remote_detect_mult;
        SHR_BITCOPY_RANGE(&payload.asd,SOC_DPP_DEFS_GET(unit,oam_2_id_nof_bits),&remote_detect_mult,0,8);
    }

    res = arad_pp_lem_access_entry_add_unsafe(
			unit,
			&request,
			&payload,
			&ack);
	SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit_semaphore);
	if(!ack.is_success)
	{
		SOC_SAND_SET_ERROR_CODE(ARAD_DRAM_INVALID_DRAM_TYPE_ERR, 40, exit_semaphore);
	}

	
exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
  ARAD_PP_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in bcm_petra_bfd_ipv4_1_hop_lem_entry_add()",0,0);

}


/* in ipv4 single hop extended mode the LEM is used as the classifier
this function remove the bfd endpoint from the LEM: 
key: ttl[2B] + BFD.Your-Discriminator[4B] 
payload: BFD.Your-Discriminator[4B] + trap code-bcmRxTrapOamBfdIpv4 = 0xe3*/ 

int arad_pp_lem_access_bfd_one_hop_lem_entry_remove(int unit,uint32 local_discriminator) {

	uint32
		res;
	ARAD_PP_LEM_ACCESS_REQUEST
		request;
	ARAD_PP_LEM_ACCESS_ACK_STATUS
		ack;
    int i=0;
	SOC_SAND_INIT_ERROR_DEFINITIONS(0);




	SOC_SAND_TAKE_DEVICE_SEMAPHORE;

	ARAD_PP_LEM_ACCESS_REQUEST_clear(&request);
	ARAD_PP_LEM_ACCESS_ACK_STATUS_clear(&ack);


	request.command = ARAD_PP_LEM_ACCESS_CMD_DELETE;
	request.stamp = 0;

	request.key.type = ARAD_PP_LEM_ACCESS_KEY_TYPE_BFD_SINGLE_HOP;
	request.key.nof_params = ARAD_PP_LEM_ACCESS_KEY_NOF_PARAMS_FOR_IP_HOST;
	request.key.param[i].value[0] = local_discriminator;
    request.key.param[i].value[1] = 0; 
    request.key.param[i].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM0_IN_BITS_FOR_IP_HOST; /*32*/
    ++i;

	request.key.param[i].value[0] = 0xff; /*ttl*/
    request.key.param[i].value[1] = 0; 
    request.key.param[i].nof_bits = ARAD_PP_LEM_ACCESS_KEY_PARAM1_IN_BITS_FOR_SA_AUTH; /*8*/
    request.key.prefix.nof_bits = ARAD_PP_LEM_ACCESS_KEY_PREFIX_SIZE_IN_BITS;
    request.key.prefix.value = ARAD_PP_LEM_ACCESS_KEY_TYPE_PREFIX_BFD_SINGLE_HOP;


    res = arad_pp_lem_access_entry_remove_unsafe(
			unit,
			&request,
			&ack);
	SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit_semaphore);
	if(!ack.is_success)
	{
		SOC_SAND_SET_ERROR_CODE(ARAD_DRAM_INVALID_DRAM_TYPE_ERR, 40, exit_semaphore)
	}
	
exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
  ARAD_PP_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in bcm_petra_bfd_ipv4_1_hop_lem_entry_add()",0,0);
}

