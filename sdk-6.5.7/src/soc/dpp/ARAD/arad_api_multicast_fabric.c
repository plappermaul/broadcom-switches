#include <soc/mcm/memregs.h>
#if defined(BCM_88650_A0)
/* $Id: arad_api_multicast_fabric.c,v 1.8 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FABRIC
/*************
 * INCLUDES  *
 *************/
/* { */
#include <shared/bsl.h>
#include <soc/dcmn/error.h>
#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/ARAD/arad_multicast_fabric.h>
#include <soc/dpp/ARAD/arad_api_general.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
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
*     Maps the embedded traffic class in the packet header to
*     a multicast class (0..3). This multicast class will be
*     further used for egress/fabric replication.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_traffic_class_to_multicast_cls_map_set(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  ARAD_TR_CLS         tr_cls_ndx,
    SOC_SAND_IN  ARAD_MULT_FABRIC_CLS     new_mult_cls
  )
{
  uint32  res;
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MULT_FABRIC_TRAFFIC_CLASS_TO_MULTICAST_CLS_MAP_SET);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;


  res = arad_mult_fabric_traffic_class_to_multicast_cls_map_verify(
    unit,
    tr_cls_ndx,
    new_mult_cls
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

   SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_traffic_class_to_multicast_cls_map_set_unsafe(
    unit,
    tr_cls_ndx,
    new_mult_cls
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_traffic_class_to_multicast_cls_map_set()",0,0);
}

/*********************************************************************
*     Maps the embedded traffic class in the packet header to
*     a multicast class (0..3). This multicast class will be
*     further used for egress/fabric replication.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_traffic_class_to_multicast_cls_map_get(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  ARAD_TR_CLS         tr_cls_ndx,
    SOC_SAND_OUT ARAD_MULT_FABRIC_CLS     *new_mult_cls
  )
{
  uint32  res;
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MULT_FABRIC_TRAFFIC_CLASS_TO_MULTICAST_CLS_MAP_GET);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(new_mult_cls);

   SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_traffic_class_to_multicast_cls_map_get_unsafe(
    unit,
    tr_cls_ndx,
    new_mult_cls
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_traffic_class_to_multicast_cls_map_get()",0,0);
}
/*********************************************************************
*     This procedure configures the base queue of the
*     multicast egress/fabric.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_base_queue_set(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                  queue_id
  )
{
  uint32  res;
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MULT_FABRIC_BASE_QUEUE_SET);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;


  res = arad_mult_fabric_base_queue_verify(
    unit,
    queue_id
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_base_queue_set_unsafe(
    unit,
    queue_id
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_base_queue_set()",0,0);
}

/*********************************************************************
*     This procedure configures the base queue of the
*     multicast egress/fabric.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_base_queue_get(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_OUT uint32                  *queue_id
  )
{
  uint32  res;
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MULT_FABRIC_BASE_QUEUE_GET);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  SOC_SAND_CHECK_NULL_INPUT(queue_id);

   SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_base_queue_get_unsafe(
    unit,
    queue_id
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_base_queue_get()",0,0);
}

/*********************************************************************
*     Configure the Enhanced Fabric Multicast Queue
*     configuration: the fabric multicast queues are defined
*     in a configured range, and the credits are coming to
*     these queues according to a scheduler scheme.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_enhanced_set(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  int                                 core_id,
    SOC_SAND_IN  SOC_SAND_U32_RANGE                  *queue_range
  )
{
  int
    res = SOC_E_NONE;
  SOCDNX_INIT_FUNC_DEFS;
  SOCDNX_NULL_CHECK(queue_range);

  res = arad_mult_fabric_enhanced_set_verify(
          unit,
          queue_range
        );
  SOCDNX_IF_ERR_EXIT(res);

  res = arad_mult_fabric_enhanced_set_unsafe(
          unit,
          queue_range
        );
  SOCDNX_IF_ERR_EXIT(res);

exit:
  SOCDNX_FUNC_RETURN;
}

/*********************************************************************
*     Configure the Enhanced Fabric Multicast Queue
*     configuration: the fabric multicast queues are defined
*     in a configured range, and the credits are coming to
*     these queues according to a scheduler scheme.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mult_fabric_enhanced_get(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  int                                 core_id,
    SOC_SAND_OUT SOC_SAND_U32_RANGE                            *queue_range
  )
{
  int
    res = SOC_E_NONE;
  SOCDNX_INIT_FUNC_DEFS;
  SOCDNX_NULL_CHECK(queue_range);

  res = arad_mult_fabric_enhanced_get_unsafe(
          unit,
          queue_range
        );
  SOCDNX_IF_ERR_EXIT(res);

exit:
  SOCDNX_FUNC_RETURN;
}

/*********************************************************************
* NAME:
*     arad_mult_fabric_active_links_set\get 
* TYPE:
*   PROC
* FUNCTION:
*   This procedure sets the FMQ with GCI LB level
* INPUT:
*   SOC_SAND_IN  int                                 unit -
*     Identifier of the device to access.
*   SOC_SAND_IN ARAD_MULT_FABRIC_FLOW_CONTROL_MAP       fc_map (set) - 
*   SOC_SAND_OUT ARAD_MULT_FABRIC_FLOW_CONTROL_MAP      fc_map (get) - 
*     See in struct description.
* REMARKS:
*   None.
* RETURNS:
*   OK or ERROR indication.
*********************************************************************/

uint32
  arad_mult_fabric_flow_control_set(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  ARAD_MULT_FABRIC_FLOW_CONTROL_MAP      *fc_map
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  res = arad_mult_fabric_flow_control_set_verify(unit, fc_map);
  SOC_SAND_CHECK_FUNC_RESULT(res, 1, exit);

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_flow_control_set_unsafe(unit, fc_map);
  SOC_SAND_CHECK_FUNC_RESULT(res, 2, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_flow_control_set()",0,0);
}

uint32
  arad_mult_fabric_flow_control_get(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_OUT ARAD_MULT_FABRIC_FLOW_CONTROL_MAP     *fc_map
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  res = arad_mult_fabric_flow_control_get_verify(unit, fc_map);
  SOC_SAND_CHECK_FUNC_RESULT(res, 1, exit);

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mult_fabric_flow_control_get_unsafe(unit, fc_map);
  SOC_SAND_CHECK_FUNC_RESULT(res, 2, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mult_fabric_flow_control_get()",0,0);
}

void
  arad_ARAD_MULT_FABRIC_PORT_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_PORT_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_PORT_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_SHAPER_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_SHAPER_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_SHAPER_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_BE_CLASS_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_BE_CLASS_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_BE_CLASS_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_BE_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_BE_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_BE_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_GR_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_GR_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_GR_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_INFO_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_INFO_clear(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_ACTIVE_LINKS_clear(
    SOC_SAND_IN uint32 unit,
    SOC_SAND_OUT ARAD_MULT_FABRIC_ACTIVE_LINKS *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_ACTIVE_LINKS_clear(unit, info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_FLOW_CONTROL_MAP_clear(
    SOC_SAND_OUT ARAD_MULT_FABRIC_FLOW_CONTROL_MAP *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_FLOW_CONTROL_MAP_clear(info);

exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

#if ARAD_DEBUG_IS_LVL1

const char*
  arad_ARAD_MULT_FABRIC_CLS_RNG_to_string(
    SOC_SAND_IN  ARAD_MULT_FABRIC_CLS_RNG enum_val
  )
{
  return SOC_TMC_MULT_FABRIC_CLS_RNG_to_string(enum_val);
}

void
  arad_ARAD_MULT_FABRIC_PORT_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_PORT_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_PORT_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_SHAPER_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_SHAPER_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_SHAPER_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_BE_CLASS_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_BE_CLASS_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_BE_CLASS_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_BE_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_BE_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_BE_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_GR_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_GR_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_GR_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_INFO_print(
    SOC_SAND_IN  ARAD_MULT_FABRIC_INFO *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS_NO_DEVID(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_INFO_print(info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

void
  arad_ARAD_MULT_FABRIC_ACTIVE_LINKS_print(
    SOC_SAND_IN uint32 unit,
    SOC_SAND_IN  ARAD_MULT_FABRIC_ACTIVE_LINKS *info
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(info);

  SOC_TMC_MULT_FABRIC_ACTIVE_LINKS_print(unit, info);
exit:
  SOC_SAND_VOID_EXIT_AND_SEND_ERROR(0, 0, 0);
}

#endif /* ARAD_DEBUG_IS_LVL1 */
/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif /* of #if defined(BCM_88650_A0) */
