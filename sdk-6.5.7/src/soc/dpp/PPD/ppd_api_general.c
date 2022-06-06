/* $Id: arad_pp_api_general.c,v 1.7 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_COMMON

/*************
 * INCLUDES  *
 *************/
/* { */
#include <shared/bsl.h>
#include <soc/dcmn/error.h>

#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>
#include <soc/dpp/SAND/Utils/sand_os_interface.h>
#include <soc/dpp/SAND/Management/sand_chip_descriptors.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_framework.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_general.h>

#include <soc/dpp/PPD/ppd_api_general.h>


/* } */
/*************
 * DEFINES   *
 *************/
/* { */

/* } */
/*************
 *  MACROS   *
 *************/
/* { */

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

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */
/*********************************************************************
* Checks whether l2_next_prtcl_type can be successfully allocated.
*********************************************************************/
uint32
  soc_ppd_l2_next_prtcl_type_allocate_test(
    SOC_SAND_IN  int 							  unit,
    SOC_SAND_IN  uint32 							  l2_next_prtcl_type,
    SOC_SAND_OUT  SOC_SAND_SUCCESS_FAILURE			  *success
  )
{ 
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  res = arad_pp_l2_next_prtcl_type_allocate_test_verify(
          unit,
          l2_next_prtcl_type,
          success
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_pp_l2_next_prtcl_type_allocate_test_unsafe(
		unit,
		l2_next_prtcl_type,
		success
	  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
  ARAD_PP_DO_NOTHING_AND_EXIT;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in soc_ppd_l2_next_prtcl_type_allocate_test()", l2_next_prtcl_type, 0);
}


uint32
  soc_ppd_fwd_decision_to_sand_dest(
    SOC_SAND_IN int              unit,
    SOC_SAND_IN SOC_PPC_FRWRD_DECISION_INFO *fwd_decision,
    SOC_SAND_OUT SOC_SAND_PP_DESTINATION_ID *dest_id
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPD_FWD_DECISION_TO_SAND_DEST);

  soc_sand_SAND_PP_DESTINATION_ID_clear(dest_id);
  switch (fwd_decision->type)
  {
  case SOC_PPC_FRWRD_DECISION_TYPE_DROP:
    dest_id->dest_type = SOC_SAND_PP_DEST_TYPE_DROP;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_UC_FLOW:
    dest_id->dest_type = SOC_SAND_PP_DEST_EXPLICIT_FLOW;
    dest_id->dest_val = fwd_decision->dest_id;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_UC_LAG:
    dest_id->dest_type = SOC_SAND_PP_DEST_LAG;
    dest_id->dest_val = fwd_decision->dest_id;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_UC_PORT:
    dest_id->dest_type = SOC_SAND_PP_DEST_SINGLE_PORT;
    dest_id->dest_val = fwd_decision->dest_id;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_MC:
    dest_id->dest_type = SOC_SAND_PP_DEST_MULTICAST;
    dest_id->dest_val = fwd_decision->dest_id;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_FEC:
    dest_id->dest_type = SOC_SAND_PP_DEST_FEC;
    dest_id->dest_val = fwd_decision->dest_id;
  break;
  case SOC_PPC_FRWRD_DECISION_TYPE_TRAP:
    dest_id->dest_type = SOC_SAND_PP_DEST_TRAP;
    SOC_PPD_DEST_TRAP_VAL_SET(dest_id->dest_val,
                          fwd_decision->additional_info.trap_info.action_profile.trap_code,
                          fwd_decision->additional_info.trap_info.action_profile.frwrd_action_strength,
                          fwd_decision->additional_info.trap_info.action_profile.snoop_action_strength
                         );
  break;
  default:
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_VALUE_OUT_OF_RANGE_ERR,10,exit);
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in soc_ppd_fwd_decision_to_sand_dest()",0,0);
}


uint32
  soc_ppd_sand_dest_to_fwd_decision(
    SOC_SAND_IN int              unit,
    SOC_SAND_IN SOC_SAND_PP_DESTINATION_ID *dest_id,
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO *fwd_decision
  )
{
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(SOC_PPD_SAND_DEST_TO_FWD_DECISION);

  SOC_PPC_FRWRD_DECISION_INFO_clear(fwd_decision);
  switch (dest_id->dest_type)
  {
  case SOC_SAND_PP_DEST_TYPE_DROP:
    SOC_PPD_FRWRD_DECISION_DROP_SET(unit, fwd_decision, res);
  break;
  case SOC_SAND_PP_DEST_EXPLICIT_FLOW:
    SOC_PPD_FRWRD_DECISION_EXPL_FLOW_SET(unit, fwd_decision, dest_id->dest_val, res);
  break;
  case SOC_SAND_PP_DEST_LAG:
    SOC_PPD_FRWRD_DECISION_LAG_SET(unit, fwd_decision, dest_id->dest_val, res);
  break;
  case SOC_SAND_PP_DEST_SINGLE_PORT:
    SOC_PPD_FRWRD_DECISION_PHY_SYS_PORT_SET(unit, fwd_decision, dest_id->dest_val, res);
  break;
  case SOC_SAND_PP_DEST_MULTICAST:
    SOC_PPD_FRWRD_DECISION_MC_GROUP_SET(unit, fwd_decision, dest_id->dest_val, res);
  break;
  case SOC_SAND_PP_DEST_FEC:
    SOC_PPD_FRWRD_DECISION_FEC_SET(unit, fwd_decision, dest_id->dest_val, res);
  break;
  case SOC_SAND_PP_DEST_TRAP: /* not supported as no trap information in soc_sand-dest */
      SOC_PPD_FRWRD_DECISION_TRAP_SET(unit, fwd_decision, SOC_PPD_DEST_TRAP_VAL_GET_TRAP_CODE(dest_id->dest_val), SOC_PPD_DEST_TRAP_VAL_GET_FWD_STRENGTH(dest_id->dest_val), SOC_PPD_DEST_TRAP_VAL_GET_SNP_STRENGTH(dest_id->dest_val), res);
  break;
  default:
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_VALUE_OUT_OF_RANGE_ERR, 10, exit);
  }
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in soc_ppd_sand_dest_to_fwd_decision()",0,0);
}


/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>



