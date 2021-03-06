#include <shared/bsl.h>

#include <soc/mcm/memregs.h>
#if defined(BCM_88690_A0)
/* $Id: jer2_arad_scheduler_device.c,v 1.15 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_COSQ
#include <soc/mem.h>

/*************
 * INCLUDES  *
 *************/
/* { */
#include <soc/dnxc/legacy/error.h>
#include <soc/dnx/legacy/drv.h>
#include <soc/dnx/legacy/mbcm.h>
#include <soc/dnx/legacy/SAND/Utils/sand_header.h>
#include <soc/dnx/legacy/port_sw_db.h>


#include <soc/dnx/legacy/ARAD/arad_scheduler_device.h>
#include <soc/dnx/legacy/ARAD/arad_reg_access.h>
#include <soc/dnx/legacy/ARAD/arad_tbl_access.h>
#include <soc/dnx/legacy/ARAD/arad_chip_defines.h>
#include <soc/dnx/legacy/ARAD/arad_mgmt.h>

#include <soc/dnx/legacy/SAND/Utils/sand_integer_arithmetic.h>
#include <soc/dnx/legacy/SAND/Utils/sand_conv.h>
#include <soc/dnx/legacy/ARAD/arad_general.h>
#include <soc/dnx/legacy/dnx_config_imp_defs.h>

/* } */

/*************
 * DEFINES   *
 *************/
/* { */

/*
 *  Internal Rate resolution - Kilo-bits per second.
 */
#define JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION 128
#define JER2_ARAD_SCH_CHNL_NIF_SPEED_UP (4)

/* } */

/*************
 *  MACROS   *
 *************/
/* { */ 

/* } */


/*************
 * FUNCTIONS *
 *************/
/* { */


uint32
  jer2_arad_sch_if_shaper_rate_verify(
    DNX_SAND_IN     int           unit,
    DNX_SAND_IN     JER2_ARAD_INTERFACE_ID  if_ndx,
    DNX_SAND_IN     uint32            rate
  )
{
  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_IF_SHAPER_RATE_VERIFY);

  DNX_SAND_EXIT_AND_SEND_ERROR("error in jer2_arad_sch_if_shaper_rate_verify()",0,0);
}

int
  jer2_arad_sch_if_shaper_rate_get(
    DNX_SAND_IN  int           unit,
    DNX_SAND_IN  int           core,
    DNX_SAND_IN  uint32        tm_port,
    DNX_SAND_OUT uint32        *if_rate
  )
{
    uint32
        res,
        rate_internal,
        credit_rate,
        device_ticks_per_sec,
        credit_worth;
    uint32
        offset,
        sch_offset,
        egress_offset;
    uint8
        fast_traversal = FALSE,
        is_channelized;
    soc_reg_above_64_val_t
        tbl_data;

    DNXC_INIT_FUNC_DEFS;

    res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_port2egress_offset, (unit, core, tm_port, &egress_offset));
    DNXC_IF_ERR_EXIT(res);

    /* Get scheduler offset */
    SOC_REG_ABOVE_64_CLEAR(tbl_data);
    DNXC_IF_ERR_EXIT(READ_SCH_FC_MAP_FCMm(unit, SCH_BLOCK(unit, core), egress_offset, &tbl_data));
    sch_offset = soc_SCH_FC_MAP_FCMm_field32_get(unit,&tbl_data,FC_MAP_FCMf);

    if (JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
    {
        is_channelized = TRUE;
        offset = sch_offset;
    }
    else
    {
        is_channelized = FALSE;
        fast_traversal = TRUE;
        offset = JER2_ARAD_SCH_OFFSET_TO_NON_CHANNELIZED_OFFSET(unit, sch_offset);
    }      

    /* Get credit size and device frequency for the following calculations */
    res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_mgmt_credit_worth_get, (unit, &credit_worth));
    DNXC_IF_ERR_EXIT(res);

    /* Get Device Interface Max Credit Rate */
    if (is_channelized)
    {
        DNXC_IF_ERR_EXIT(READ_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
        credit_rate = soc_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm_field32_get(unit, tbl_data, CH_NI_FXX_MAX_CR_RATEf);
    }
    else
    {
        DNXC_IF_ERR_EXIT(READ_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
        credit_rate = soc_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm_field32_get(unit, tbl_data, PORT_NIF_MAX_CR_RATEf);
    }
  
    if (0 == credit_rate)
    {
        rate_internal = 0;
    }
    else
    {
        device_ticks_per_sec = jer2_arad_chip_ticks_per_sec_get(unit);

        res = dnx_sand_clocks_to_kbits_per_sec(
            credit_rate,
            credit_worth,
            device_ticks_per_sec,
            &rate_internal
          );
        DNXC_SAND_IF_ERR_EXIT(res);

        if (fast_traversal)
        {
            rate_internal = rate_internal * JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION;
        }

        if(is_channelized) {
            rate_internal = rate_internal * JER2_ARAD_SCH_CHNL_NIF_SPEED_UP;
        }
    }

    *if_rate = rate_internal;

exit:
    DNXC_FUNC_RETURN;
}

/*********************************************************************
*     Sets, for a specified device interface, (NIF-Ports,
*     recycling & CPU) its maximal credit rate. This API is
*     only valid for Channelized interfaces - see REMARKS
*     section below.
*     Details: in the H file. (search for prototype)
*********************************************************************/
int
  jer2_arad_sch_if_shaper_rate_set(
    DNX_SAND_IN     int                   unit,
    DNX_SAND_IN     int                   core,
    DNX_SAND_IN     uint32                tm_port,
    DNX_SAND_IN     uint32                rate
  )
{
    uint32
        res,
        credit_rate,
        device_ticks_per_sec,
        rate_internal = rate,
        credit_worth,
        credit_rate_nof_bits;
    uint32
        offset,
        egress_offset,
        sch_offset;
    uint8
        is_channelized;
    soc_reg_above_64_val_t
        tbl_data;

    DNXC_INIT_FUNC_DEFS;

    res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_port2egress_offset, (unit, core, tm_port, &egress_offset));
    DNXC_IF_ERR_EXIT(res);

    /* Get scheduler offset */
    SOC_REG_ABOVE_64_CLEAR(tbl_data);
    DNXC_IF_ERR_EXIT(READ_SCH_FC_MAP_FCMm(unit, SCH_BLOCK(unit, core), egress_offset, &tbl_data));
    sch_offset = soc_SCH_FC_MAP_FCMm_field32_get(unit,&tbl_data,FC_MAP_FCMf);

    if (JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
    {
        is_channelized = TRUE;
        offset = sch_offset;
        /* Obtain max credit rate nof bits */
        credit_rate_nof_bits = soc_mem_field_length(unit, SCH_CH_NIF_RATES_CONFIGURATION_CNRCm, CH_NI_FXX_MAX_CR_RATEf);
        rate_internal = rate;
    }
    else
    {
        is_channelized = FALSE;
        offset = JER2_ARAD_SCH_OFFSET_TO_NON_CHANNELIZED_OFFSET(unit, sch_offset);
        /* Obtain max credit rate nof bits */
        credit_rate_nof_bits = soc_mem_field_length(unit, SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm, PORT_NIF_MAX_CR_RATEf);
        rate_internal = DNX_SAND_DIV_ROUND_UP(rate, JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION);
    }      

    /* Get credit size and device frequency for the following calculations */
    res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_mgmt_credit_worth_get, (unit, &credit_worth));
    DNXC_IF_ERR_EXIT(res);

    device_ticks_per_sec = jer2_arad_chip_ticks_per_sec_get(unit);

    /* Get Device Interface Max Credit Rate */
    if (0 == rate_internal)
    {
        credit_rate = 0;
    }
    else
    {
        res = dnx_sand_kbits_per_sec_to_clocks(
            rate_internal,
            credit_worth,
            device_ticks_per_sec,
            &credit_rate
          );
        DNXC_SAND_IF_ERR_EXIT(res);

        if(is_channelized) {
            credit_rate = credit_rate * JER2_ARAD_SCH_CHNL_NIF_SPEED_UP;
        }

        /* credit_rate_nof_bits < 32 */
        /* coverity[large_shift:FALSE] */
        DNX_SAND_LIMIT_FROM_ABOVE(credit_rate, DNX_SAND_BITS_MASK((credit_rate_nof_bits-1),0));

        if (!is_channelized){
            DNX_SAND_LIMIT_FROM_BELOW(credit_rate, JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION);
        }
    }

    /* Set Device Interface Max Credit Rate */
    if (is_channelized)
    {
        DNXC_IF_ERR_EXIT(READ_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
        soc_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm_field32_set(unit,tbl_data,CH_NI_FXX_MAX_CR_RATEf,credit_rate);
        DNXC_IF_ERR_EXIT(WRITE_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
    }
    else
    {
        DNXC_IF_ERR_EXIT(READ_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
        soc_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm_field32_set(unit,tbl_data,PORT_NIF_MAX_CR_RATEf,credit_rate);
        DNXC_IF_ERR_EXIT(WRITE_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit, SCH_BLOCK(unit, core), offset, tbl_data));
    }

exit:
    DNXC_FUNC_RETURN;
}

/*********************************************************************
*     This function sets the device interfaces scheduler
*     weight configuration. Up to 8 weight configuration can
*     be pre-configured. Each scheduler interface will be
*     configured to use one of these pre-configured weights.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_if_weight_conf_verify(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  JER2_ARAD_SCH_IF_WEIGHTS      *if_weights
  )
{

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_IF_WEIGHT_CONF_VERIFY);

  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_if_weight_conf_verify()",0,0);
}

/*********************************************************************
*     This function sets the device interfaces scheduler
*     weight configuration. Up to 8 weight configuration can
*     be pre-configured. Each scheduler interface will be
*     configured to use one of these pre-configured weights.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_if_weight_conf_set_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  JER2_ARAD_SCH_IF_WEIGHTS      *if_weights
  )
{
  uint32
    res,
    weight_reg_idx;
  uint32
    entry_idx,
    weight_idx;
   
  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_IF_WEIGHT_CONF_SET_UNSAFE);

  DNX_SAND_CHECK_NULL_INPUT(if_weights);
  

  for (entry_idx = 0; entry_idx < if_weights->nof_enties; entry_idx++)
  {
    weight_idx = if_weights->weight[entry_idx].id;

    /* Weight value write { */
    weight_reg_idx = weight_idx/2;

    if ((weight_idx%2) == 0)
    {
      DNX_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, JER2_ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, SCH_DVS_WEIGHT_CONFIGr, SOC_CORE_ALL,  weight_reg_idx, WFQ_WEIGHT_XXf,  if_weights->weight[entry_idx].val));
    }
    else
    {
      DNX_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  40,  exit, JER2_ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, SCH_DVS_WEIGHT_CONFIGr, SOC_CORE_ALL,  weight_reg_idx, WFQ_WEIGHT_XX_PLUS_1f,  if_weights->weight[entry_idx].val));
    }
    /* Weight value write } */

  }

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_if_weight_conf_set_unsafe()",0,0);
}

/*********************************************************************
*     This function sets the device interfaces scheduler
*     weight configuration. Up to 8 weight configuration can
*     be pre-configured. Each scheduler interface will be
*     configured to use one of these pre-configured weights.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_if_weight_conf_get_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_OUT JER2_ARAD_SCH_IF_WEIGHTS      *if_weights
  )
{
  uint32
    res,
    weight_reg_idx;
  uint32
    weight_idx,
    entry_idx;
   
    

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_IF_WEIGHT_CONF_GET_UNSAFE);

  DNX_SAND_CHECK_NULL_INPUT(if_weights);
  

  if_weights->nof_enties = JER2_ARAD_SCH_NOF_IF_WEIGHTS;

  for (entry_idx = 0; entry_idx < if_weights->nof_enties; entry_idx++)
  {
    weight_idx = entry_idx;
    
    DNX_SAND_ERR_IF_ABOVE_MAX(
      weight_idx, JER2_ARAD_SCH_NOF_IF_WEIGHTS,
      JER2_ARAD_SCH_INTERFACE_WEIGHT_INDEX_OUT_OF_RANGE_ERR, 20, exit
    );

    if_weights->weight[entry_idx].id = entry_idx;
    /* Weight value write { */
    weight_reg_idx = weight_idx/2;

    if ((weight_idx%2) == 0)
    {
      DNX_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, JER2_ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, SCH_DVS_WEIGHT_CONFIGr, SOC_CORE_ALL,  weight_reg_idx, WFQ_WEIGHT_XXf, &if_weights->weight[entry_idx].val));
    }
    else
    {
      DNX_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  40,  exit, JER2_ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, SCH_DVS_WEIGHT_CONFIGr, SOC_CORE_ALL,  weight_reg_idx, WFQ_WEIGHT_XX_PLUS_1f, &if_weights->weight[entry_idx].val));
    }
    /* Weight value write } */
  }

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_if_weight_conf_get_unsafe()",0,0);
}

/*********************************************************************
*     Sets, for a specified device interface, (NIF-Ports,
*     recycling, OLP, ERP) its weight index. Range: 0-7. The
*     actual weight value (one of 8, configurable) is in range
*     1-1023, 0 meaning inactive interface. This API is only
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_device_if_weight_idx_verify(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  uint32              weight_index
  )
{
  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_DEVICE_IF_WEIGHT_IDX_VERIFY);

  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_device_if_weight_idx_verify()",0,0);
}

/*********************************************************************
*     Sets, for a specified device interface, (NIF-Ports,
*     recycling, OLP, ERP) its weight index. Range: 0-7. The
*     actual weight value (one of 8, configurable) is in range
*     1-1023, 0 meaning inactive interface. This API is only
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_device_if_weight_idx_get_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  soc_port_t          port,
    DNX_SAND_OUT uint32              *weight_index
  )
{
  uint32
    weight_idx_result,
    res,
    data,
    tbl_data;
  uint32
    egress_offset,
    sch_offset,
    offset,
    tm_port;
  uint8
    is_channelized;
  int
    core;

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_DEVICE_IF_WEIGHT_GET_UNSAFE);
  DNX_SAND_CHECK_NULL_INPUT(weight_index);


  res = dnx_port_sw_db_local_to_tm_port_get(unit, port, &tm_port, &core);
  DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 15, exit);  

  /* Get egress offset */
  res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_port2egress_offset, (unit, core, tm_port, &egress_offset));
  DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 20, exit);


  /* Get scheduler offset */
  DNX_SAND_SOC_IF_ERROR_RETURN(res, 1260, exit, READ_SCH_FC_MAP_FCMm(unit,SCH_BLOCK(unit, core),egress_offset,&data));
  sch_offset = soc_SCH_FC_MAP_FCMm_field32_get(unit,&data,FC_MAP_FCMf);

  if (JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
  {
    is_channelized = TRUE;
    offset = sch_offset;
  }
  else
  {
    is_channelized = FALSE;
    offset = JER2_ARAD_SCH_OFFSET_TO_NON_CHANNELIZED_OFFSET(unit, sch_offset);
  }     
  
  /*
   * Get Device Interface Max Credit Rate
   */
  if (is_channelized)
  {
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1270, exit, READ_SCH_CH_NIF_CALENDAR_CONFIGURATION_CNCCm(unit, SCH_BLOCK(unit, core), offset, &tbl_data));
    weight_idx_result = soc_SCH_CH_NIF_CALENDAR_CONFIGURATION_CNCCm_field32_get(unit, &tbl_data, CH_NI_FXX_WEIGHTf);
  }
  else
  {
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1280, exit, READ_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit,SCH_BLOCK(unit, core),offset,&tbl_data));
    weight_idx_result = soc_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm_field32_get(unit, &tbl_data, PORT_NIF_WEIGHTf);
  } 

  *weight_index = weight_idx_result;

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_device_if_weight_idx_get_unsafe()",0,0);
}

uint32
  jer2_arad_sch_device_if_weight_idx_set_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  soc_port_t          port,
    DNX_SAND_IN  uint32              weight_index
  )
{
  uint32
    res,
    tbl_data,
    data;
  uint32
    egress_offset,
    sch_offset,
    offset,
    tm_port;
  uint8
    is_channelized;
  int
    core;

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_DEVICE_IF_WEIGHT_SET_UNSAFE);

  res = dnx_port_sw_db_local_to_tm_port_get(unit, port, &tm_port, &core);
  DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 14, exit);
  /* Get egress offset */
  res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_port2egress_offset, (unit, core, tm_port, &egress_offset));
  DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 15, exit);


  /* Get scheduler offset */
  DNX_SAND_SOC_IF_ERROR_RETURN(res, 1380, exit, READ_SCH_FC_MAP_FCMm(unit,SCH_BLOCK(unit, core),egress_offset,&data));
  sch_offset = soc_SCH_FC_MAP_FCMm_field32_get(unit,&data,FC_MAP_FCMf);

  if (JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
  {
    is_channelized = TRUE;
    offset = sch_offset;
  }
  else
  {
    is_channelized = FALSE;
    offset = JER2_ARAD_SCH_OFFSET_TO_NON_CHANNELIZED_OFFSET(unit, sch_offset);
  }     
  
  /*
   * Get Device Interface Max Credit Rate
   */
  if (is_channelized)
  {
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1390, exit, READ_SCH_CH_NIF_CALENDAR_CONFIGURATION_CNCCm(unit, SCH_BLOCK(unit, core), offset, &tbl_data));
    soc_SCH_CH_NIF_CALENDAR_CONFIGURATION_CNCCm_field32_set(unit, &tbl_data, CH_NI_FXX_WEIGHTf,weight_index);
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1400, exit, WRITE_SCH_CH_NIF_CALENDAR_CONFIGURATION_CNCCm(unit, SCH_BLOCK(unit, core), offset, &tbl_data));
  }
  else
  {
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1410, exit, READ_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit,SCH_BLOCK(unit, core),offset,&tbl_data));
    soc_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm_field32_set(unit, &tbl_data, PORT_NIF_WEIGHTf,weight_index);
    DNX_SAND_SOC_IF_ERROR_RETURN(res, 1420, exit, WRITE_SCH_ONE_PORT_NIF_CONFIGURATION_OPNCm(unit,SCH_BLOCK(unit, core),offset,&tbl_data));
  } 

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_device_if_weight_idx_set_unsafe()",0,0);
}

/*********************************************************************
*     This function sets an entry in the device rate table.
*     Each entry sets a credit generation rate, for a given
*     pair of fabric congestion (presented by rci_level) and
*     the number of active fabric links. The driver writes to
*     the following tables: Device Rate Memory (DRM)
*     For Jericho, this table is SCH_SHARED_DEVICE_RATE_SHARED_DRMm
*     For Arad, this table is SCH_DEVICE_RATE_MEMORY__DRMm
*     Details: in the H file. (search for prototype)
*     Note that validity of input parameters is assumed to have
*     been checked by calling procedures.
*********************************************************************/
uint32
  jer2_arad_sch_device_rate_entry_set_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  uint32              rci_level_ndx,
    DNX_SAND_IN  uint32              nof_active_links_ndx,
    DNX_SAND_IN  uint32              rate
  )
{
  uint32
    interval_in_clock_128_th,
    calc,
    offset,
    credit_worth,
    res;
  JER2_ARAD_SCH_DRM_TBL_DATA
    drm_tbl_data;

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_DEVICE_RATE_ENTRY_SET_UNSAFE);
  /*
   * Get 'credit worth' value using device driver specific procedure.
   */
  res = (MBCM_DNX_DRIVER_CALL(unit,mbcm_dnx_mgmt_credit_worth_get,(unit, &credit_worth))) ;
  DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 4, exit);
  /*
   *  Calculate interval between credits (in Clocks/64) according to:
   *
   *
   *                       Credit [bits] * Num_of_Mega_clocks_64th_in_sec [(M * clocks)/(64 * sec)]
   *  Rate [Mbits/Sec] =   -----------------------------------------------------------------------
   *                          interval_between_credits_in_clock_64th [clocks/64]
   */

  if (0 == rate)
  {
    interval_in_clock_128_th = 0;
  }
  else
  {
    calc = (credit_worth * DNX_SAND_NOF_BITS_IN_CHAR) *
           (jer2_arad_chip_mega_ticks_per_sec_get(unit) * JER2_ARAD_SCH_DEVICE_RATE_INTERVAL_RESOLUTION);
    calc = DNX_SAND_DIV_ROUND(calc, rate);
    interval_in_clock_128_th = calc;
  }

  DNX_SAND_LIMIT_FROM_ABOVE(interval_in_clock_128_th, JER2_ARAD_SCH_DEVICE_RATE_INTERVAL_MAX);

  if (interval_in_clock_128_th != 0)
  {
    DNX_SAND_LIMIT_FROM_BELOW(interval_in_clock_128_th, JER2_ARAD_SCH_DEVICE_RATE_INTERVAL_MIN);
  }

  offset = (nof_active_links_ndx * SOC_DNX_DEFS_GET(unit, nof_rci_levels)) + rci_level_ndx;
  drm_tbl_data.device_rate = interval_in_clock_128_th;

  /*
   * Write indirect to DRM table, single entry
   */
  res = jer2_arad_sch_drm_tbl_set_unsafe(
          unit,
          offset,
          &drm_tbl_data
        );
  DNX_SAND_CHECK_FUNC_RESULT(res, 5, exit);

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_device_rate_entry_set_unsafe()",0,0);
}


/*********************************************************************
*     This function gets an entry in the device rate table.
*     Each entry contains a credit generation rate, for a given
*     pair of fabric congestion (presented by rci_level) and
*     the number of active fabric links. The driver reads from
*     the following tables: Device Rate Memory (DRM)
*     Details: in the H file. (search for prototype)
*     Note that validity of input parameters is assumed to have
*     been checked by calling procedures.
*********************************************************************/
uint32
  jer2_arad_sch_device_rate_entry_get_unsafe(
    DNX_SAND_IN  int                 unit,
    DNX_SAND_IN  uint32              rci_level_ndx,
    DNX_SAND_IN  uint32              nof_active_links_ndx,
    DNX_SAND_OUT uint32              *rate
  )
{
  uint32
    interval_in_clock_128_th,
    calc,
    offset,
  credit_worth,
    res;
  JER2_ARAD_SCH_DRM_TBL_DATA
    drm_tbl_data;

  DNX_SAND_INIT_ERROR_DEFINITIONS(JER2_ARAD_SCH_DEVICE_RATE_ENTRY_GET_UNSAFE);

  /*
   * Read indirect from DRM table
   */

  offset = (nof_active_links_ndx * SOC_DNX_DEFS_GET(unit, nof_rci_levels)) + rci_level_ndx;

  res = jer2_arad_sch_drm_tbl_get_unsafe(
          unit,
          offset,
          &drm_tbl_data
        );
  DNX_SAND_CHECK_FUNC_RESULT(res, 4, exit);

  interval_in_clock_128_th = drm_tbl_data.device_rate;
  /*
   *  Calculate Device Credit Rate Generation (in Mbits/secs) according to:
   *
   *
   *                       Credit [bits] * Num_of_Mega_clocks_128th_in_sec [(M * clocks)/(128 * sec)]
   *  Rate [Mbits/Sec] =   -----------------------------------------------------------------------
   *                          interval_between_credits_in_clock_128th [clocks/128]
   */
  if (0 == interval_in_clock_128_th)
  {
    *rate = 0;
  }
  else
  {
    /*
     * Get 'credit worth' value using device driver specific procedure.
     */
    res = (MBCM_DNX_DRIVER_CALL(unit,mbcm_dnx_mgmt_credit_worth_get,(unit, &credit_worth))) ;
    DNX_SAND_SOC_CHECK_FUNC_RESULT(res, 5, exit);

    calc = (credit_worth * DNX_SAND_NOF_BITS_IN_CHAR) *
           (jer2_arad_chip_mega_ticks_per_sec_get(unit) * JER2_ARAD_SCH_DEVICE_RATE_INTERVAL_RESOLUTION);
    calc = DNX_SAND_DIV_ROUND(calc, interval_in_clock_128_th);
    *rate = calc;
  }

exit:
  DNX_SAND_EXIT_AND_SEND_ERROR( "error in jer2_arad_sch_device_rate_entry_get_unsafe()",0,0);
}

/*********************************************************************
*     Sets, for a specified device MAC LANE or equivalent, (NIF-Ports,
*     recycling & CPU) its actual credit rate (sum of ports). This API is
*     only valid for Channelized interfaces - see REMARKS
*     section below.
*     Note: for SGMII - configures only the first interface in MAL
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  jer2_arad_sch_ch_if_rate_set_unsafe(
    DNX_SAND_IN     int               unit,
    DNX_SAND_IN     int               core,
    DNX_SAND_IN     uint32            sch_offset,
    DNX_SAND_IN     uint32            rate
  )
{
    uint32
        res,
        rate_internal,
        credit_rate,
        device_ticks_per_sec,
        credit_worth,
        credit_rate_nof_bits;
    soc_reg_above_64_val_t
        tbl_data;

    DNXC_INIT_FUNC_DEFS;

    SOC_REG_ABOVE_64_CLEAR(tbl_data);

    if (!JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
    {
        DNXC_EXIT_WITH_ERR(SOC_E_INTERNAL, (_BSL_SOC_MSG("SCH interface %d (core %d) isn't part of channalized interface"), sch_offset, core));
    }

    rate_internal = DNX_SAND_DIV_ROUND_UP(rate, JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION);

    /* Get Device Interface Max Credit Rate */
    if (0 == rate_internal)
    {
        credit_rate = 0;
    }
    else
    {
        /* Get credit size and device frequency for the following calculations */
        res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_mgmt_credit_worth_get, (unit, &credit_worth));
        DNXC_IF_ERR_EXIT(res);

        device_ticks_per_sec = jer2_arad_chip_ticks_per_sec_get(unit);

        res = dnx_sand_kbits_per_sec_to_clocks(
                rate_internal,
                credit_worth,
                device_ticks_per_sec,
                &credit_rate
              );
        DNXC_SAND_IF_ERR_EXIT(res);

        credit_rate_nof_bits = soc_mem_field_length(unit, SCH_CH_NIF_RATES_CONFIGURATION_CNRCm, CH_NI_FXX_SUM_OF_PORTSf);
        /* credit_rate_nof_bits < 32 */
        /* coverity[large_shift:FALSE] */
        DNX_SAND_LIMIT_FROM_ABOVE(credit_rate, DNX_SAND_BITS_MASK((credit_rate_nof_bits-1),0));
        DNX_SAND_LIMIT_FROM_BELOW(credit_rate, JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION);
    }

    DNXC_IF_ERR_EXIT(READ_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit,SCH_BLOCK(unit, core), sch_offset,&tbl_data));
    soc_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm_field32_set(unit,&tbl_data,CH_NI_FXX_SUM_OF_PORTSf,credit_rate);
    DNXC_IF_ERR_EXIT(WRITE_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit,SCH_BLOCK(unit, core), sch_offset,&tbl_data));

exit:
    DNXC_FUNC_RETURN;
}

uint32
  jer2_arad_sch_ch_if_rate_get_unsafe(
    DNX_SAND_IN     int               unit,
    DNX_SAND_IN     int               core,
    DNX_SAND_IN     uint32            sch_offset,
    DNX_SAND_OUT    uint32            *rate
  )
{
    uint32
        res,
        rate_internal,
        credit_rate,
        device_ticks_per_sec,
        credit_worth;
    soc_reg_above_64_val_t
        tbl_data;

    DNXC_INIT_FUNC_DEFS;

    if (!JER2_ARAD_SCH_IS_CHNIF_ID(unit, sch_offset))
    {
        DNXC_EXIT_WITH_ERR(SOC_E_INTERNAL, (_BSL_SOC_MSG("SCH interface %d (core %d) isn't part of channalized interface"), sch_offset, core));
    }

    DNXC_IF_ERR_EXIT(READ_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm(unit, SCH_BLOCK(unit, core), sch_offset, &tbl_data));
    credit_rate = soc_SCH_CH_NIF_RATES_CONFIGURATION_CNRCm_field32_get(unit, &tbl_data,CH_NI_FXX_SUM_OF_PORTSf);

    if(credit_rate == 0) {
        *rate = 0;
    } 
    else 
    {
        /* Get credit size and device frequency for the following calculations */
        res = MBCM_DNX_DRIVER_CALL(unit, mbcm_dnx_mgmt_credit_worth_get, (unit, &credit_worth));
        DNXC_IF_ERR_EXIT(res);

        device_ticks_per_sec = jer2_arad_chip_ticks_per_sec_get(unit);

        res = dnx_sand_clocks_to_kbits_per_sec(
            credit_rate, 
            credit_worth,   
            device_ticks_per_sec,
            &rate_internal     
            );
        DNXC_SAND_IF_ERR_EXIT(res);

        *rate = rate_internal * JER2_ARAD_SCH_DEV_RATE_INTERVAL_RESOLUTION;
    }


exit:
    DNXC_FUNC_RETURN;
}

/* } */


#include <soc/dnx/legacy/SAND/Utils/sand_footer.h>
#endif /* of #if defined(BCM_88690_A0) */
