#include <shared/bsl.h>

#include <soc/mcm/memregs.h>
#if defined(BCM_88650_A0)
/* $Id: arad_mgmt.c,v 1.100 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_INIT

/*************
 * INCLUDES  *
 *************/
/* { */

#include <sal/core/stats.h>
#include <shared/swstate/access/sw_state_access.h>
#include <shared/shr_template.h>
#include <soc/dcmn/error.h>
#include <soc/dcmn/dcmn_mem.h>

#include <soc/mem.h>
#include <soc/drv.h>
#include <soc/dpp/drv.h>

#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/SAND/SAND_FM/sand_link.h>

#include <soc/dpp/ARAD/arad_defs.h>
#include <soc/dpp/ARAD/arad_mgmt.h>
#include <soc/dpp/ARAD/arad_chip_defines.h>
#include <soc/dpp/ARAD/arad_reg_access.h>
#include <soc/dpp/ARAD/arad_tbl_access.h>
#include <soc/dpp/ARAD/arad_general.h>
#include <soc/dpp/ARAD/arad_sw_db.h>
#include <soc/dpp/ARAD/arad_fabric.h>
#include <soc/dpp/ARAD/arad_tbl_access.h>
#include <soc/dpp/SAND/Management/sand_device_management.h>
#include <soc/dpp/SAND/Management/sand_low_level.h>
#include <soc/dpp/SAND/Management/sand_general_params.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_api_framework.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_api_mgmt.h>

#include <soc/dpp/ARAD/NIF/ports_manager.h>
#include <soc/dpp/port_sw_db.h>


/* } */

/*************
 * DEFINES   *
 *************/
/* { */
#define ARAD_PORT_NDX_MAX                                        (ARAD_NOF_FAP_PORTS-1)
#define ARAD_CONF_MODE_NDX_MAX                                   (ARAD_MGMT_NOF_PCKT_SIZE_CONF_MODES - 1)

/* System Frequency factor */
#define ARAD_MGMT_SYST_FREQ_RES_19 (1 << 19)

#define ARAD_MGMT_INIT_CTRL_CELLS_TIMER_ITERATIONS 1500
#define ARAD_MGMT_INIT_CTRL_RCH_STATUS_ITERATIONS  24
#define ARAD_MGMT_INIT_STANDALONE_ITERATIONS       16
#define ARAD_MGMT_INIT_CTRL_CELLS_TIMER_DELAY_MSEC 32

/* For port rates of 50G - 100G */
#define ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_XE     8
/* For port rates of 100G - 200G */
#define ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_CAUI   16
/* For port rates of 200G - 400G */
#define ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_ILKN   32

#define ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_START  (9)
#define ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_LENGTH (2)
#define ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_START      (3)
#define ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_LENGTH     (6)
#define ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_START   (0)
#define ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_LENGTH  (3)

#define ARAD_MGMT_IQM_OCBPRM_MANTISSA_NOF_BITS(unit) (SOC_IS_ARADPLUS_AND_BELOW(unit) ? 7 : 8)
#define ARAD_MGMT_IQM_OCBPRM_EXPONENT_NOF_BITS       (5)

#define ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_MANTISSA_NOF_BITS(unit) (SOC_IS_ARADPLUS_AND_BELOW(unit) ? 7 : 8)
#define ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_EXPONENT_NOF_BITS       (4)

#define ARAD_MGMT_IQM_WORDS_RESOLUTION  (16)

/* value was calculated in the following way:
    (4k for every 10G of the port type) 
    10G  ports : (4 * 1024 * 1)   = 4096
    40G  ports : (4 * 1024 * 4)   = 16384
    100G ports : (4 * 1024 * 10)  = 40960
    200G ports : (4 * 1024 * 20)  = 81920
    The biggest value suppossed to be sufficient to be used by default,
    but as a precaution we take a bigger value (4*1024*50) as default.
    the logic behind this default is to allow for each voq a reasonable burst size for the maximu possible speed of each port without consuming unreasonable amount of OCB resources.
    the prefered approch should be to devide to the VOQs all of the available OCB resources, however doing so is too specific per user's application so those defaults were chosen and users can farther
    optimize the values per application according to thier needs */
#define ARAD_MGMT_IQM_OCB_PRM_DEFAULT_SIZE          (4 * 1024 * 50)
/* same as default size only devided by ocb buff size.
 * When packet size is 64B, every packet consume 1 256B OCB buffer,
 * so we treat buffer size as 64B. */
#define ARAD_MGMT_IQM_OCB_PRM_DEFAULT_BUFF_SIZE     (ARAD_MGMT_IQM_OCB_PRM_DEFAULT_SIZE / 64)

/*
 * PVT monitor
 */
#define _SOC_ARAD_PVT_MON_NOF                             (2)
#define _SOC_ARAD_PVT_FACTOR                              (5660)
#define _SOC_ARAD_PVT_BASE                                (4283900)

#define _SOC_ARDON_ECI_PVT_MON_CONTROL_REG_POWERDOWN_BIT   (32)
#define _SOC_ARDON_ECI_PVT_MON_CONTROL_REG_RESET_BIT       (33)
#define _SOC_ARDON_PVT_MON_NOF                             (1)
#define _SOC_ARDON_PVT_FACTOR                              (48517)
#define _SOC_ARDON_PVT_BASE                                (41016000)

/*
 * The number of fields being enabled / disabled while enabling / disabling ctrl cells
 */
#define ARAD_MGMT_CTRL_CELLS_MAX_NOF_CONFS 100

#define ARAD_MGMT_DBG_ON                   0

#if ARAD_MGMT_DBG_ON
  #define ARAD_MGMT_TRACE(iter_index)                         \
  {                                                           \
    if (iter_index == 0)                                      \
    {                                                         \
      LOG_CLI( \
               (BSL_META_U(unit, \
                           " %s\r\n"), FUNCTION_NAME()));            \
    }                                                         \
  }
#else
  #define ARAD_MGMT_TRACE(iter_index)
#endif

extern char *_build_release;


/*
 *  Chip identification fields (Version Register).
 *  Needed by register_device, before access database is initialized
 */
/* } */

/*************
 *  MACROS   *
 *************/

/* { */


typedef uint32 (*ARAD_CTRL_CELL_POLL_FUNC) (SOC_SAND_IN  int unit, SOC_SAND_IN uint32 iter_index, SOC_SAND_OUT uint8 *success) ;
typedef uint32 (*ARAD_CTRL_CELL_FNLY_FUNC) (SOC_SAND_IN  int unit) ;

typedef struct
{
  soc_reg_t                reg;
  soc_field_t              field;

  uint32                  instance_id;

  ARAD_CTRL_CELL_POLL_FUNC   polling_func;

  ARAD_CTRL_CELL_FNLY_FUNC  failure_func;

  uint32                    val;

  /*
   *  In case a polling function is used,
   *  this is the number of iterations (at least one is performed).
   *  Otherwise - this is the delay in milliseconds.
   */
  uint32                    delay_or_polling_iters;

  uint32                    err_on_fail;

}ARAD_CTRL_CELL_DATA;

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


STATIC uint32
  arad_mgmt_module_init(int unit)
{
  uint32
    res;

  SOC_SAND_INTERRUPT_INIT_DEFS;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_MODULE_INIT);

  /* Stopping interrupts, since the following functions are global, and not device specific
     (device semaphore can't work here). Possible optimization - maintain another global mutex, in
     order not to stop interrupts for this */
  SOC_SAND_INTERRUPTS_STOP;

  res = arad_chip_defines_init(unit);
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  res = arad_sw_db_init(
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  /* Now add list of ARAD procedure descriptors to all-system pool.                                     */
  res = arad_procedure_desc_add() ;
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

exit:
  SOC_SAND_INTERRUPTS_START_IF_STOPPED;

  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_module_init()",0,0);
}

STATIC uint32
  arad_mgmt_device_init(
    SOC_SAND_IN  int  unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_DEVICE_INIT);

  res = arad_sw_db_device_init(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_device_init()",0,0);
}

STATIC uint32
  arad_mgmt_device_close(
    SOC_SAND_IN  int  unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_DEVICE_CLOSE);

  res = arad_sw_db_device_close(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_device_close()",0,0);
}


/*********************************************************************
*     This procedure registers a new device to be taken care
*     of by this device driver. Physical device must be
*     accessible by CPU when this call is made..
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_register_device_unsafe(
             uint32                  *base_address,
    SOC_SAND_IN  SOC_SAND_RESET_DEVICE_FUNC_PTR reset_device_ptr,
    SOC_SAND_INOUT int                 *unit_ptr
  )
{
  uint32
    res;
  int
    unit = *unit_ptr;
  SOC_SAND_DEV_VER_INFO
    ver_info;
  ARAD_REG_FIELD
    chip_type_fld,
    dbg_ver_fld,
    chip_ver_fld;
  uint32
    *base;
  uint32
    reg_val;
  soc_error_t
    rv = SOC_E_NONE;

  SOC_SAND_INTERRUPT_INIT_DEFS;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_REGISTER_DEVICE_UNSAFE);

  unit = (*unit_ptr);

  res = arad_mgmt_module_init(unit);
  SOC_SAND_CHECK_FUNC_RESULT(res, 5, exit);


  soc_sand_clear_SAND_DEV_VER_INFO(&ver_info);
  chip_type_fld.lsb = ARAD_MGMT_CHIP_TYPE_FLD_LSB;
  chip_type_fld.msb = ARAD_MGMT_CHIP_TYPE_FLD_MSB;
  dbg_ver_fld.lsb   = ARAD_MGMT_DBG_VER_FLD_LSB;
  dbg_ver_fld.msb   = ARAD_MGMT_DBG_VER_FLD_MSB;
  chip_ver_fld.lsb  = ARAD_MGMT_CHIP_VER_FLD_LSB;
  chip_ver_fld.msb  = ARAD_MGMT_CHIP_VER_FLD_MSB;

  ver_info.ver_reg_offset = ARAD_MGMT_VER_REG_BASE;
  ver_info.logic_chip_type= SOC_SAND_DEV_ARAD;
  ver_info.chip_type      = ARAD_EXPECTED_CHIP_TYPE;

  ver_info.chip_type_shift= ARAD_FLD_SHIFT_OLD(chip_type_fld);
  ver_info.chip_type_mask = ARAD_FLD_MASK_OLD(chip_type_fld);
  ver_info.dbg_ver_shift  = ARAD_FLD_SHIFT_OLD(dbg_ver_fld);
  ver_info.dbg_ver_mask   = ARAD_FLD_MASK_OLD(dbg_ver_fld);
  ver_info.chip_ver_shift = ARAD_FLD_SHIFT_OLD(chip_ver_fld);
  ver_info.chip_ver_mask  = ARAD_FLD_MASK_OLD(chip_ver_fld);

  /* Skip verification since Arad is CMIC-based */
  ver_info.cmic_skip_verif = TRUE;


  base = (uint32*)base_address;

  /*
   *  Validate ECI access to the Arad
   */
    reg_val = 0xaaff5500;
    SOC_DPP_ALLOW_WARMBOOT_WRITE(WRITE_ECI_TEST_REGISTERr(*unit_ptr, reg_val), rv);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 1000, exit);
    READ_ECI_TEST_REGISTERr(*unit_ptr, &reg_val);
#ifdef SAL_BOOT_PLISIM
    if ((SAL_BOOT_PLISIM) == 0)
    {
        if (reg_val != ~(0xaaff5500))
        {
          SOC_SAND_SET_ERROR_CODE(ARAD_ECI_ACCESS_ERR, 49, exit);
        }
    }
    else 
#endif /* SAL_BOOT_PLISIM */
    {
        if (reg_val != (0xaaff5500))
        {
          SOC_SAND_SET_ERROR_CODE(ARAD_ECI_ACCESS_ERR, 49, exit);
        }
    }


  /* Register the device in SOC_SAND.                                                                         */
  res = soc_sand_device_register(
          base,
          ARAD_TOTAL_SIZE_OF_REGS,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          reset_device_ptr,
          NULL,
          &ver_info,
          NULL,
          0xFFFFFFFF,                         /* Given invalid address as the general mask bit.           */
                                              /* This bit do not exist on the ARAD                      */
          &unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);

  *unit_ptr = unit ;

  res = arad_mgmt_sw_ver_set_unsafe(unit);
  SOC_SAND_CHECK_FUNC_RESULT(res, 55, exit);

  /* Stopping interrupts, since error/proc desc is not device specific, but global */
  SOC_SAND_INTERRUPTS_STOP; 
  {
      /* Add list of PB_PP errors to all-system errors pool                                                  */
      res = arad_pp_errors_desc_add();
      SOC_SAND_CHECK_FUNC_RESULT(res, 90, exit);

      /* Now add list of PB_PP procedure descriptors to all-system pool.                                     */
      res = arad_pp_procedure_desc_add() ;
      SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit);
  }
  SOC_SAND_INTERRUPTS_START_IF_STOPPED;

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mgmt_device_init(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_DPP_WARMBOOT_RELEASE_HW_MUTEX(rv);
  if(rv != SOC_E_NONE) {
    LOG_ERROR(BSL_LS_SOC_INIT,
              (BSL_META_U(unit,
                          " Failed while executing the macro SOC_DPP_WARMBOOT_RELEASE_HW_MUTEX.\n")));
  }
  SOC_SAND_INTERRUPTS_START_IF_STOPPED;
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_register_device_unsafe()",0,0);
}

/*********************************************************************
*     Undo arad_register_device()
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_unregister_device_unsafe(
    SOC_SAND_IN  int        unit
  )
{
  uint32
    res = SOC_SAND_OK;
  int32
    soc_sand_ret;
  SOC_SAND_RET
    ret;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_UNREGISTER_DEVICE_UNSAFE);

/* #ifdef ARAD_PP */
  if (1/*arad_sw_db_pp_enable_get(unit)*/)
  {
    res = arad_pp_mgmt_device_close(
            unit
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 50, exit);
  }
/* #endif */
  res = arad_mgmt_device_close(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);

  /*
   * Take mutexes in the right order -> delta_list, device, rest of them
   */
  if (SOC_SAND_OK != soc_sand_tcm_callback_delta_list_take_mutex())
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_SEM_TAKE_FAIL, 2, exit) ;
  }

  soc_sand_ret = soc_sand_take_chip_descriptor_mutex(unit) ;
  if (SOC_SAND_OK != soc_sand_ret)
  {
    if (SOC_SAND_ERR == soc_sand_ret)
    {
      soc_sand_tcm_callback_delta_list_give_mutex();
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_SEM_TAKE_FAIL, 3, exit);
    }
    if (0 > soc_sand_ret)
    {
      soc_sand_tcm_callback_delta_list_give_mutex();
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_ILLEGAL_DEVICE_ID, 4, exit) ;
    }
  }
  /* semaphore taken successfully */

  /*
   * Give device mutex back
   */
  if (SOC_SAND_OK != soc_sand_give_chip_descriptor_mutex(unit))
  {
    soc_sand_tcm_callback_delta_list_give_mutex();
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_SEM_GIVE_FAIL, 5, exit) ;
  }
  /*
   * Give list mutex back
   */
  if (SOC_SAND_OK != soc_sand_tcm_callback_delta_list_give_mutex())
  {
    SOC_SAND_SET_ERROR_CODE(SOC_SAND_SEM_GIVE_FAIL, 6, exit) ;
  }

  ret = soc_sand_device_unregister(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(ret, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_unregister_device_unsafe()",0,0);
}
/*
   [31:20] - SDK version of the last regular init;
   [19:8] - if ISSU took place then set the SDK version after ISSU;
   [7:4] - Did WB take place?(=0x0,0x1) ;
   [3:0] - Did ISSU take place? (=0x0,0x1) 
*/
uint32
  arad_mgmt_sw_ver_set_unsafe(
    SOC_SAND_IN  int                      unit
  )
{
    int      ver_val[3] = {0,0,0};
    uint32   prev_ver_val[3] = {0,0,0};
    uint32   regval, i, prev_regval;
    char     *ver;
    int      wb, issu, bit_ndx ;
  soc_error_t
    rv;
  char
    *cur_number_ptr;
  soc_reg_t  ver_reg;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_SW_VER_UNSAFE);

  regval = 0;
  wb = 0;
  issu = 0;


  ver = _build_release;
  cur_number_ptr = sal_strchr(ver, '-');
  if(cur_number_ptr == NULL) {
      SOC_SAND_EXIT_AND_SEND_ERROR_SOCDNX((_BSL_SOCDNX_MSG_STR("Invalid version format.")));
  }
  ++cur_number_ptr;
  ver_val[0] = _shr_ctoi (cur_number_ptr);
  cur_number_ptr = sal_strchr(cur_number_ptr, '.');
  if(cur_number_ptr == NULL) {
      SOC_SAND_EXIT_AND_SEND_ERROR_SOCDNX((_BSL_SOCDNX_MSG_STR("Invalid version format.")));
  }
  ++cur_number_ptr;
  ver_val[1] = _shr_ctoi (cur_number_ptr);
  cur_number_ptr = sal_strchr(cur_number_ptr, '.');
  if(cur_number_ptr == NULL) {
      SOC_SAND_EXIT_AND_SEND_ERROR_SOCDNX((_BSL_SOCDNX_MSG_STR("Invalid version format.")));
  }
  ++cur_number_ptr;
  ver_val[2] = _shr_ctoi (cur_number_ptr);
  
  ver_reg = SOC_IS_JERICHO(unit)? ECI_SW_VERSIONr: ECI_REG_0093r;

  /*	[31:20] - SDK version of the last regular init */
  if (SOC_WARM_BOOT(unit)) {
      SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(rv,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg32_get(unit, ver_reg, REG_PORT_ANY,  0, &prev_regval));

      wb = 1;
      bit_ndx = 28;

      for (i=0; i<3; i++) {
          /* Get last regular init version*/
          prev_ver_val[i]= ( prev_regval >> (bit_ndx - i*4)) & 0xf;

          if (prev_ver_val[i] != ver_val[i]) {
                issu = 1;
          }
          regval = (regval | (0xf & prev_ver_val[i])) << 4;
      }

  }else {
      for (i=0; i<3; i++) {
          regval = (regval | (0xf & ver_val[i])) << 4;
      }
  }

  /* If issu set current version in 19:8*/
  for (i=0; i<3; i++) {
      if (issu) {
          regval = (regval | (0xf & ver_val[i]));
      }
      regval = regval << 4;
  }

  /* 7:4 is wb */
  regval = (regval | (0xf & wb)) << 4;

  /* 3:0 is issu */
  regval = (regval | (0xf & issu));

  SOC_DPP_ALLOW_WARMBOOT_WRITE(soc_reg32_set(unit, ver_reg, REG_PORT_ANY, 0, regval), rv);
  
  SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 1000, exit);
 
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_sw_ver_set_unsafe()",0,0);
}
 

/***********************************************************************************
* Helper utility for *_mgmt_credit_worth_set():
* Calculate value for SCH_ASSIGNED_CREDIT_CONFIGURATION, SCH_REBOUNDED_CREDIT_CONFIGURATION,
* given input 'credit_worth'
* Output goes into *fld_val to be loaded into registers specified above, into 
* ASSIGNED_CREDIT_WORTH, REBOUNDED_CREDIT_WORTH fields, correspondingly
***********************************************************************************/
uint32
  arad_calc_assigned_rebounded_credit_conf(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32              credit_worth,
    SOC_SAND_OUT uint32              *fld_val
  )
{
  uint32
    res ;
  soc_port_t  
    port_i;
  bcm_pbmp_t      
    ports_map;
  soc_port_if_t 
    interface_type;
  uint32
    credit_div;
  uint8
    is_ilkn_used,
    is_caui_used;
  
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_CALC_ASSIGNED_REBOUNDED_CREDIT_CONF);
  SOC_SAND_CHECK_NULL_INPUT(fld_val);
  *fld_val = 0 ;

  /* SCH { */
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 1000, exit, soc_port_sw_db_valid_ports_get(unit, SOC_PORT_FLAGS_NETWORK_INTERFACE, &ports_map));

  is_ilkn_used = FALSE;
  is_caui_used = FALSE;
  BCM_PBMP_ITER(ports_map, port_i) { /* Look for ILKN/CAUI interfaces that need value different than default */
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 1010, exit, soc_port_sw_db_interface_type_get(unit, port_i, &interface_type));
      if(interface_type == SOC_PORT_IF_ILKN) 
      {
        is_ilkn_used = TRUE;
      } 
      else if(interface_type == SOC_PORT_IF_CAUI) 
      {
        is_caui_used = TRUE;
      }
      if(is_ilkn_used && is_caui_used)
      {
        break; /* No need to continue iterating */
      }
  }

  if(SOC_DPP_CONFIG(unit)->arad->init.credit.credit_worth_resolution == ARAD_MGMT_CREDIT_WORTH_RESOLUTION_AUTO || 
     SOC_DPP_CONFIG(unit)->arad->init.credit.credit_worth_resolution == ARAD_MGMT_CREDIT_WORTH_RESOLUTION_LOW)
  { 
    if(is_ilkn_used) 
    {
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_ILKN; /* Allow max rate of 400G */
    } 
    else if(is_caui_used) 
    {
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_CAUI; /* Allow max rate of 200G */
    }
    else
    {
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_XE; /* Allow max rate of 100G */
    }
  }
  else
  {
    switch(SOC_DPP_CONFIG(unit)->arad->init.credit.credit_worth_resolution)
    {
    case ARAD_MGMT_CREDIT_WORTH_RESOLUTION_HIGH:
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_XE; /* Always, regardless of interfaces used */
      break;
    case ARAD_MGMT_CREDIT_WORTH_RESOLUTION_MEDIUM:
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_CAUI;
      break;
    default:
      /* Should only reach here in case of miss-configuration */
      credit_div = ARAD_MGMT_DIV_ASSIGNED_CREDIT_WORTH_XE; /* Assign default value */
      break;
    }
  }
  *fld_val = credit_worth / credit_div ;
  /* SCH } */
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_calc_assigned_rebounded_credit_conf()",0,0);
}


/*********************************************************************
*     Bytes-worth of a single credit. It should be configured
*     the same in all the FAPs in the systems, and should be
*     set before programming the scheduler.
*     Details: in the H file. (search for prototype)
*     This procedure is for Arad and Arad+
*********************************************************************/
int
  arad_mgmt_credit_worth_set(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32              credit_worth
  )
{
  uint32
    fld_val = 0;
  soc_reg_t credit_value_reg;
  soc_field_t credit_value_lcl_field;
  
  SOCDNX_INIT_FUNC_DEFS ;

  SOCDNX_SAND_IF_ERR_EXIT(arad_mgmt_credit_worth_verify(unit,credit_worth)) ;

  fld_val = credit_worth;
  /*
   * Arad: Note that the Jericho register is different than in Arad and the fields
   * are marked '0' and '1' while in Arad, the same fields are marked '1' and '2'.
   */
  credit_value_reg = SOC_IS_ARADPLUS(unit)? IPS_IPS_CREDIT_CONFIG_1r: IPS_IPS_CREDIT_CONFIGr;
  credit_value_lcl_field = SOC_IS_ARADPLUS(unit)? CREDIT_VALUE_1f: CREDIT_VALUEf;

  SOCDNX_IF_ERR_EXIT(soc_reg_field32_modify(unit, credit_value_reg, REG_PORT_ANY, credit_value_lcl_field, fld_val)) ;

  /* SCH { */
  
  SOCDNX_SAND_IF_ERR_EXIT(arad_calc_assigned_rebounded_credit_conf(unit, credit_worth, &fld_val));

  {
    /*
     * This clause will probably change when two calendars feature is
     * added (SCH_ASSIGNED_CREDIT_CONFIGURATION_1, SCH_REBOUNDED_CREDIT_CONFIGURATION_1, ...)
     */
    SOCDNX_IF_ERR_EXIT(
        soc_reg_above_64_field32_modify(unit, SCH_ASSIGNED_CREDIT_CONFIGURATIONr, SOC_CORE_ALL, 0, ASSIGNED_CREDIT_WORTHf,  fld_val));
    /* Set Rebounded Credit worth equals to assigned credit worth */
    SOCDNX_IF_ERR_EXIT(
        soc_reg_above_64_field32_modify(unit, SCH_REBOUNDED_CREDIT_CONFIGURATIONr, SOC_CORE_ALL, 0, REBOUNDED_CREDIT_WORTHf,  fld_val));
  }
  /* SCH } */
exit:
    SOCDNX_FUNC_RETURN;
}

/*********************************************************************
*     Bytes-worth of a single credit. It should be configured
*     the same in all the FAPs in the systems, and should be
*     set before programming the scheduler.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_credit_worth_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32              credit_worth
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_CREDIT_WORTH_VERIFY);

  SOC_SAND_ERR_IF_OUT_OF_RANGE(
    credit_worth, SOC_TMC_CREDIT_SIZE_BYTES_MIN, SOC_TMC_CREDIT_SIZE_BYTES_MAX,
    ARAD_CREDIT_SIZE_OUT_OF_RANGE_ERR, 10, exit
  );

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_credit_worth_verify()",0,0);
}

/*********************************************************************
*     Bytes-worth of a single credit. It should be configured
*     the same in all the FAPs in the systems, and should be
*     set before programming the scheduler.
*     Details: in the H file. (search for prototype)
*********************************************************************/
int
  arad_mgmt_credit_worth_get(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_OUT uint32              *credit_worth
  )
{
    uint32
        reg_val,
        fld_val = 0;
    soc_reg_t credit_value_reg;
    soc_field_t credit_value_lcl_field;

    SOCDNX_INIT_FUNC_DEFS;
    SOCDNX_NULL_CHECK(credit_worth);

    credit_value_reg = SOC_IS_ARADPLUS(unit)? IPS_IPS_CREDIT_CONFIG_1r: IPS_IPS_CREDIT_CONFIGr ;
    credit_value_lcl_field = SOC_IS_ARADPLUS(unit)? CREDIT_VALUE_1f: CREDIT_VALUEf ;
    /*
     * Read selected register and, then, extract the filed.
     */
    SOCDNX_IF_ERR_EXIT(soc_reg32_get(unit, credit_value_reg, REG_PORT_ANY, 0, &reg_val)) ;
    fld_val = soc_reg_field_get(unit, credit_value_reg, reg_val, credit_value_lcl_field) ;
    *credit_worth = fld_val;

exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Arad+ only: set local and remote (1 and 2) credit worth values
 */
uint32
  arad_plus_mgmt_credit_worth_remote_set(
    SOC_SAND_IN  int    unit,
	SOC_SAND_IN  uint32    credit_worth_remote
  )
{
    uint32 res, reg_val;
    uint16 nof_remote_faps_with_remote_credit_value;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

	
	if (credit_worth_remote < SOC_TMC_CREDIT_SIZE_BYTES_MIN || credit_worth_remote > SOC_TMC_CREDIT_SIZE_BYTES_MAX) {
		LOG_ERROR(BSL_LS_SOC_MANAGEMENT, 
				  (BSL_META_U(unit,
							  "Remote size %d is not between %u..%u") , credit_worth_remote, SOC_TMC_CREDIT_SIZE_BYTES_MIN, SOC_TMC_CREDIT_SIZE_BYTES_MAX));
		SOC_SAND_SET_ERROR_CODE(SOC_SAND_VALUE_OUT_OF_RANGE_ERR, 10, exit);

	} else {
		uint32 arg_local, arg_remote;
		res = arad_plus_mgmt_credit_worth_remote_get(unit, &arg_remote);
		SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);
		res = arad_mgmt_credit_worth_get(unit, &arg_local);
		SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);
		if (credit_worth_remote != arg_remote) { /* are we changing the value? */
            res = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.nof_remote_faps_with_remote_credit_value.get(unit, &nof_remote_faps_with_remote_credit_value);
    		SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);
			if (nof_remote_faps_with_remote_credit_value) { /* is the current value being used (by remote FAPs)? */
				if (credit_worth_remote != arg_local) {
					LOG_ERROR(BSL_LS_SOC_MANAGEMENT,
							  (BSL_META_U(unit,"The Remote credit value is assigned to remote devices. To change the value you must first assign the local credit value to these devices.")));
					SOC_SAND_SET_ERROR_CODE(SOC_SAND_VALUE_OUT_OF_RANGE_ERR, 10, exit);
				} else { /* the local and (previous) remote values are equal, so we can just mark all FAPs as using the local value */
					res = arad_plus_mgmt_change_all_faps_credit_worth_unsafe(unit, SOC_TMC_FAP_CREDIT_VALUE_LOCAL); /*need to be in sand_error???*/
					SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 30, exit);
					}
				} else {
					SOC_SAND_SOC_IF_ERROR_RETURN(res, 100, exit, READ_IPS_IPS_CREDIT_CONFIG_1r(unit, &reg_val));
					soc_reg_field_set(unit, IPS_IPS_CREDIT_CONFIG_1r, &reg_val, CREDIT_VALUE_2f, credit_worth_remote);
					SOC_SAND_SOC_IF_ERROR_RETURN(res, 1100, exit, WRITE_IPS_IPS_CREDIT_CONFIG_1r(unit, reg_val));
				}
			}
		}
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_credit_worth_remote_set()", unit, 0);
}


/*
 * Arad+ only: set local and remote (1 and 2) credit worth values
 */
uint32
  arad_plus_mgmt_credit_worth_remote_get(
    SOC_SAND_IN  int    unit,
    SOC_SAND_OUT uint32    *credit_worth_remote
  )
{
    uint32 res, reg_val;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
	SOC_SAND_CHECK_NULL_INPUT(credit_worth_remote);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 100, exit, READ_IPS_IPS_CREDIT_CONFIG_1r(unit, &reg_val));
	*credit_worth_remote = soc_reg_field_get(unit, IPS_IPS_CREDIT_CONFIG_1r, reg_val, CREDIT_VALUE_2f);
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_credit_worth_remote_get()", unit, 0);
}

/*
 * Arad+ only: Set the mapping the module (fap_id) to the given credit value (local, remote or non mapped).
 * Has no special handling of the local device (should not be used for the local device).
 */
STATIC uint32
  arad_plus_mgmt_per_module_credit_value_set(
    SOC_SAND_IN  int    unit,
    SOC_SAND_IN  uint32    fap_id,
    SOC_SAND_IN  uint32    credit_value_type /* will be one of ARAD_PLUS_FAP_CREDIT_VALUE_* */
  )
{
    soc_error_t rv;
    uint32 offset = fap_id % ARAD_PLUS_CREDIT_VALUE_MODES_PER_WORD;
    uint32 per_module_credit_value;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.per_module_credit_value.get(unit,
                                                                                     fap_id / ARAD_PLUS_CREDIT_VALUE_MODES_PER_WORD,
                                                                                     &per_module_credit_value);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 10, exit);
    per_module_credit_value = (per_module_credit_value & ~(1 << offset)) | (credit_value_type << offset); 

    rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.per_module_credit_value.set(unit,
                                                                                     fap_id / ARAD_PLUS_CREDIT_VALUE_MODES_PER_WORD,
                                                                                     per_module_credit_value);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 10, exit);

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_module_to_credit_worth_map_get_unsafe()", unit, fap_id);
}


/*
 * Arad+ only: map the module (fap_id) to the given credit value (local, remote or non mapped).
 * Has no special handling of the local device (should not be used for the local device).
 */
uint32
  arad_plus_mgmt_module_to_credit_worth_map_set_unsafe(
    SOC_SAND_IN  int    unit,
    SOC_SAND_IN  uint32    fap_id,
    SOC_SAND_IN  uint32    credit_value_type /* should be one of ARAD_PLUS_FAP_CREDIT_VALUE_* */
  )
{
    uint32 prev_type, res;
    uint16 nof_remote_faps_with_remote_credit_value;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    if (!SOC_IS_ARADPLUS(unit)) {
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 20, exit);
    }
    res = arad_plus_mgmt_module_to_credit_worth_map_get_unsafe(unit, fap_id, &prev_type);
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    if (credit_value_type != prev_type) {
        res = arad_plus_mgmt_per_module_credit_value_set(unit, fap_id, credit_value_type);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
        res = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.nof_remote_faps_with_remote_credit_value.get(unit, &nof_remote_faps_with_remote_credit_value);
        SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
        if (credit_value_type == SOC_TMC_FAP_CREDIT_VALUE_REMOTE) {
            ++nof_remote_faps_with_remote_credit_value;
        } else if (prev_type == SOC_TMC_FAP_CREDIT_VALUE_REMOTE) {
            --nof_remote_faps_with_remote_credit_value;
        }
        res = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.nof_remote_faps_with_remote_credit_value.set(unit, nof_remote_faps_with_remote_credit_value);
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
    }

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_module_to_credit_worth_map_set_unsafe()", unit, fap_id);
}

/*
 * Arad+ only: Get the mapping the module (fap_id) to the given credit value (local, remote or non mapped).
 * Has no special handling of the local device (should not be used for the local device).
 */
uint32
  arad_plus_mgmt_module_to_credit_worth_map_get_unsafe(
    SOC_SAND_IN  int    unit,
    SOC_SAND_IN  uint32    fap_id,
    SOC_SAND_OUT uint32    *credit_value_type /* will be one of ARAD_PLUS_FAP_CREDIT_VALUE_* */
  )
{
    soc_error_t rv;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    SOC_SAND_CHECK_NULL_INPUT(credit_value_type);
    rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.per_module_credit_value.get(unit,
                                                                                     fap_id / ARAD_PLUS_CREDIT_VALUE_MODES_PER_WORD,
                                                                                     credit_value_type);
    SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 130, exit);
    *credit_value_type = (*credit_value_type >> (fap_id % ARAD_PLUS_CREDIT_VALUE_MODES_PER_WORD)) & 0x1; 

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_module_to_credit_worth_map_get_unsafe()", unit, fap_id);
}

/*
 * Arad+ only: in case the local and remote credit values are equal, change all configure remote FAPS to use the local or remote value.
 * The credit_value_to_use selects to which value we should make the FAPS use:
 *   ARAD_PLUS_FAP_CREDIT_VALUE_LOCAL  - use the local credit value
 *   ARAD_PLUS_FAP_CREDIT_VALUE_REMOTE - use the local credit value
 */
uint32
  arad_plus_mgmt_change_all_faps_credit_worth_unsafe(
    SOC_SAND_IN  int    unit,
    SOC_SAND_OUT uint8     credit_value_to_use
  )
{
    uint32 reg_val;
    soc_error_t rv;
    uint32 credit_worth_local, credit_worth_remote;
    uint32 per_module_credit_idx;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    SOC_SAND_SOC_IF_ERROR_RETURN(rv, 100, exit, READ_IPS_IPS_CREDIT_CONFIG_1r(unit, &reg_val));
    credit_worth_local = soc_reg_field_get(unit, IPS_IPS_CREDIT_CONFIG_1r, reg_val, CREDIT_VALUE_1f);
    credit_worth_remote = soc_reg_field_get(unit, IPS_IPS_CREDIT_CONFIG_1r, reg_val, CREDIT_VALUE_2f);
    if (credit_worth_local != credit_worth_remote) {
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 110, exit);
    } else if (credit_value_to_use == SOC_TMC_FAP_CREDIT_VALUE_LOCAL) {
        rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.nof_remote_faps_with_remote_credit_value.set(unit, 0);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 100, exit);
    } else if (credit_value_to_use == SOC_TMC_FAP_CREDIT_VALUE_REMOTE) {
        rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.nof_remote_faps_with_remote_credit_value.set(unit, ARAD_NOF_FAPS_IN_SYSTEM);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 100, exit);
    } else {
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 120, exit);
    }

    /* change all fap modes to credit_value_to_use */
    for(per_module_credit_idx = 0; per_module_credit_idx < ARAD_PLUS_CREDIT_VALUE_MODE_WORDS; ++per_module_credit_idx) {
        rv = sw_state_access[unit].dpp.soc.arad.tm.arad_plus.per_module_credit_value.set(unit, per_module_credit_idx, credit_value_to_use ? 255 : 0);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(rv, 130, exit);
    }
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_plus_mgmt_change_all_faps_credit_worth_unsafe()", unit, credit_value_to_use);
}

/*********************************************************************
*     Set the fabric system ID of the device. Must be unique
*     in the system.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_system_fap_id_set_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 sys_fap_id
  )
{
  uint32
    res;
  
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_SYSTEM_FAP_ID_SET_UNSAFE);

  
  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, ECI_GLOBAL_0r, REG_PORT_ANY, 0, PIPEIDf,  sys_fap_id));
  if (SOC_DPP_CONFIG(unit)->tdm.is_bypass &&
      SOC_DPP_CONFIG(unit)->arad->init.fabric.is_128_in_system &&
      SOC_DPP_CONFIG(unit)->tm.is_petrab_in_system) {
          SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  20,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, FDT_TDM_CONFIGURATIONr, REG_PORT_ANY, 0, TDM_SOURCE_FAP_IDf,  sys_fap_id + SOC_DPP_CONFIG(unit)->arad->tdm_source_fap_id_offset));
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_system_fap_id_set_unsafe()", unit, sys_fap_id);
}

/*********************************************************************
*     Set the fabric system ID of the device. Must be unique
*     in the system.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_system_fap_id_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 sys_fap_id
  )
{
 
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_SYSTEM_FAP_ID_VERIFY);

  SOC_SAND_ERR_IF_ABOVE_MAX(
    sys_fap_id, ARAD_NOF_FAPS_IN_SYSTEM-1,
    ARAD_FAP_FABRIC_ID_OUT_OF_RANGE_ERR, 10, exit
  );


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_system_fap_id_verify()",0,0);
}

/*********************************************************************
*     Set the fabric system ID of the device. Must be unique
*     in the system.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_system_fap_id_get_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_OUT uint32              *sys_fap_id
  )
{
  SOCDNX_INIT_FUNC_DEFS;
  SOCDNX_NULL_CHECK(sys_fap_id);

  SOCDNX_IF_ERR_EXIT(soc_reg_above_64_field32_read(unit, ECI_GLOBAL_0r, REG_PORT_ANY, 0, PIPEIDf, sys_fap_id));

exit:
  SOCDNX_FUNC_RETURN;
}

/*********************************************************************
*     Set the device TM-Domain. Must be unique
*     in a stackable system.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_tm_domain_set_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 tm_domain
  )
{
  uint32
    res;
  
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IHB_LBP_GENERAL_CONFIG_0r, SOC_CORE_ALL, 0, TM_DOMAINf,  tm_domain));

  res = arad_egr_prog_editor_stacking_lfems_set(unit, SOC_TMC_STACK_EGR_PROG_TM_PORT_PROFILE_STACK_ALL, 0x0);
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_tm_domain_set_unsafe()",tm_domain,0);
}

uint32
  arad_mgmt_tm_domain_verify(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 tm_domain
  )
{
 
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);

  SOC_SAND_ERR_IF_ABOVE_MAX(tm_domain, ARAD_NOF_TM_DOMAIN_IN_SYSTEM-1, SOC_SAND_GEN_ERR, 10, exit);


exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_tm_domain_verify()",0,0);
}

uint32
  arad_mgmt_tm_domain_get_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_OUT uint32                 *tm_domain
  )
{
  uint32
    res,
    fld_val;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  SOC_SAND_CHECK_NULL_INPUT(tm_domain);

  

  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IHB_LBP_GENERAL_CONFIG_0r, SOC_CORE_ALL, 0, TM_DOMAINf, &fld_val));

  /* Get device internal field.                                                                           */
  *tm_domain = fld_val;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_tm_domain_get_unsafe()",0,0);
}

/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting traffic.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_enable_traffic_set(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint8                 enable
  )
{
  uint32
    res;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ENABLE_TRAFFIC_SET);
  SOC_SAND_PCID_LITE_SKIP(unit);
  SOC_SAND_CHECK_DRIVER_AND_DEVICE;

  res = arad_mgmt_enable_traffic_verify(
    unit,
    enable
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  SOC_SAND_TAKE_DEVICE_SEMAPHORE;

  res = arad_mgmt_enable_traffic_set_unsafe(
    unit,
    enable
  );
  SOC_SAND_CHECK_FUNC_RESULT(res, 100, exit_semaphore);

exit_semaphore:
  SOC_SAND_GIVE_DEVICE_SEMAPHORE;
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_enable_traffic_set()",0,0);
}


STATIC uint32
  arad_mgmt_all_ctrl_cells_fct_disable_polling(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 iter_index,
    SOC_SAND_OUT uint8                 *all_down
  )
{
  uint32
    res = SOC_SAND_OK;
  uint64 
      rtp_mask,
      links_up_bm;
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_FCT_DISABLE_POLLING);
  ARAD_MGMT_TRACE(iter_index);
  if(SOC_IS_QAX(unit) && !SOC_IS_QAX_WITH_FABRIC_ENABLED(unit)) {
    *all_down = TRUE;
  }else{
    *all_down = TRUE;
    res = soc_sand_os_memset(&rtp_mask, 0x0, sizeof(rtp_mask));
    SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);
    res = soc_sand_os_memset(&links_up_bm, 0x0, sizeof(links_up_bm));
    SOC_SAND_CHECK_FUNC_RESULT(res, 11, exit);

    /*Get link down bitmap*/
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 1110, exit, READ_RTP_LINK_ACTIVE_MASKr_REG64(unit, &rtp_mask));

    /*Convert to link up bitmap*/
    COMPILER_64_MASK_CREATE(links_up_bm, SOC_DPP_DEFS_GET(unit, nof_fabric_links), 0);
    COMPILER_64_NOT(rtp_mask);
    COMPILER_64_AND(links_up_bm, rtp_mask);

    if (!COMPILER_64_IS_ZERO(links_up_bm))
    {
      *all_down = FALSE;
      /* Exit with SOC_SAND_OK */
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_OK, 20, exit);
    }
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_fct_disable_polling()",0,0);
}

STATIC uint32
  arad_mgmt_all_ctrl_cells_standalone_polling(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 iter_index,
    SOC_SAND_OUT uint8                 *success
  )
{
  uint32
    res,
    buffer;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  ARAD_MGMT_TRACE(iter_index);


  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, MESH_TOPOLOGY_FAP_DETECT_CTRL_CELLS_CNTr, REG_PORT_ANY, 0, RCV_CTL_1f, &buffer));

  /*ignore first iteration in order to clear counter*/
  *success = SOC_SAND_NUM2BOOL(buffer) && (iter_index != 0);
  
   /*cfg not stand alone in success case */
  if (*success) {
        res = arad_fabric_stand_alone_fap_mode_set_unsafe(
          unit,
          FALSE
        );
        SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);
  }


  ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mngr_standalone_polling()",0,0);
}

STATIC uint32
  arad_mgmt_all_ctrl_cells_standalone_failure(
    SOC_SAND_IN  int                 unit
  )
{
  uint32
    res = SOC_SAND_OK;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  
  res = arad_fabric_stand_alone_fap_mode_set_unsafe(
          unit,
          TRUE
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_standalone_failure()",0,0);
}

STATIC uint32
  arad_mgmt_all_ctrl_cells_status_polling(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 iter_index,
    SOC_SAND_OUT uint8                 *success
  )
{
  uint32
    res = SOC_SAND_OK;
  uint32
    /*buffer_1, buffer_2,*/ buffer;
  uint8
    stand_alone;

  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  ARAD_MGMT_TRACE(iter_index);
  res = arad_fabric_stand_alone_fap_mode_get_unsafe(
          unit,
          &stand_alone
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res, 10,  exit, ARAD_REG_ACCESS_ERR,MBCM_DPP_DRIVER_CALL(unit, mbcm_dpp_fabric_mesh_check, (unit, stand_alone, success)));

  if (*success == FALSE) {
  
  

      /*recheck stand alone once every ARAD_MGMT_INIT_STANDALONE_ITERATIONS*/
      if ((iter_index % ARAD_MGMT_INIT_STANDALONE_ITERATIONS  == 0) && (iter_index != 0)) {
          
          SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, MESH_TOPOLOGY_FAP_DETECT_CTRL_CELLS_CNTr, REG_PORT_ANY, 0, RCV_CTL_1f, &buffer));

          /*cfg stand alone when the counter is zero*/
          if (!SOC_SAND_NUM2BOOL(buffer)) {
                res = arad_fabric_stand_alone_fap_mode_set_unsafe(
                  unit,
                  TRUE
                );
                SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);

                *success = TRUE;

#if ARAD_MGMT_DBG_ON
                LOG_CLI(
                         (BSL_META_U(unit,
                                     "!!DBG: stand alone mode detected \n")));
#endif
          }

      }
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_status_polling()",0,0);
}

STATIC uint32
  arad_mgmt_all_ctrl_cells_fct_enable_polling(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  uint32                 iter_index,
    SOC_SAND_OUT uint8                 *any_up
  )
{
  uint32
    res = SOC_SAND_OK;
  uint8
    all_down;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_FCT_ENABLE_POLLING);
  ARAD_MGMT_TRACE(iter_index);
  res = arad_mgmt_all_ctrl_cells_fct_disable_polling(
          unit,
          SAL_UINT32_MAX, /*no polling */
          &all_down
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  *any_up = SOC_SAND_NUM2BOOL_INVERSE(all_down);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_fct_enable_polling()",0,0);
}

STATIC uint32
  arad_mgmt_ctrl_cells_counter_clear(
    SOC_SAND_IN  int                  unit
  )
{
  uint32
    res,
    reg_val;
  uint64
    reg_val64;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_CTRL_CELLS_COUNTER_CLEAR);

  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,READ_MESH_TOPOLOGY_FAP_DETECT_CTRL_CELLS_CNTr(unit, &reg_val));
  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  15,  exit, ARAD_REG_ACCESS_ERR,READ_MESH_TOPOLOGY_STATUS_2r(unit, &reg_val64));

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_ctrl_cells_counter_clear()",0,0);
}

STATIC uint32
  arad_mgmt_all_ctrl_cells_enable_write(
    SOC_SAND_IN  int                  unit,
    SOC_SAND_IN  ARAD_CTRL_CELL_DATA      *data,
    SOC_SAND_IN  uint8                  silent
  )
{
  uint32
    res = SOC_SAND_OK;
  uint8
    success = TRUE,
    is_low_sim_active;
  uint32
    wait_iter = 0;
   
    

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_ENABLE_WRITE);

  SOC_SAND_CHECK_NULL_INPUT(data);

  

#ifndef SAND_LOW_LEVEL_SIMULATION
  is_low_sim_active = FALSE;
#else
  is_low_sim_active = soc_sand_low_is_sim_active_get();
#endif

  /* Write the data */
  if (data->reg != INVALIDr)
  {
      SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  25,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, data->reg,  data->instance_id,  0, data->field,  data->val));
  }

  if (data->polling_func)
  {
    /* If there polling field exists, refer to delay as the expected value */
#if ARAD_MGMT_DBG_ON
  if (data->polling_func)
  {
    LOG_CLI(
             (BSL_META_U(unit,
                         "!!DBG: Polling on ")));
  }
#endif
    do
    {
      if (is_low_sim_active)
      {
        success = data->failure_func ? FALSE : TRUE;
      }
      else
      {
        res = data->polling_func(
                unit,
                wait_iter,
                &success
              );
        SOC_SAND_CHECK_FUNC_RESULT(res, 27, exit);
      }

      if (success)
      {
        /* Exit with SOC_SAND_OK */
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_OK, 30, exit);
      }
      soc_sand_os_task_delay_milisec(ARAD_MGMT_INIT_CTRL_CELLS_TIMER_DELAY_MSEC);

    } while ((wait_iter++) < data->delay_or_polling_iters);
  }
  else
  {
    /* If there is no polling field, refer to delay as delay */
    if (data->delay_or_polling_iters)
    {
      soc_sand_os_task_delay_milisec(data->delay_or_polling_iters);
    }
  }

  if (!success)
  {
    if (data->failure_func)
    {
      res = data->failure_func(
              unit
            );
      SOC_SAND_CHECK_FUNC_RESULT(res, 40, exit);
    }

    if (data->err_on_fail)
    {
      SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 50, exit);
    }
  }

exit:
#if ARAD_MGMT_DBG_ON
  if (data->polling_func)
  {
    LOG_CLI(
             (BSL_META_U(unit,
                         "!!DBG: Polling Time: %u[ms], Success: %s\n\r"),
              wait_iter*ARAD_MGMT_INIT_CTRL_CELLS_TIMER_DELAY_MSEC,
              success?"TRUE":"FALSE"
              ));
    if (wait_iter >= data->delay_or_polling_iters)
    {
      LOG_CLI(
               (BSL_META_U(unit,
                           "!!DBG: Exceeded maximal polling time %u[ms] (%u * %u iterations)\n\r"),
                data->delay_or_polling_iters*ARAD_MGMT_INIT_CTRL_CELLS_TIMER_DELAY_MSEC,
                data->delay_or_polling_iters,
                ARAD_MGMT_INIT_CTRL_CELLS_TIMER_DELAY_MSEC
                ));
    }
  }
#endif
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_enable_write()",0,0);
}

uint32
  arad_mgmt_all_ctrl_cells_enable_get_unsafe(
    SOC_SAND_IN   int  unit,
    SOC_SAND_OUT  uint8  *enable
  )
{
  uint32
    res,
    fld_val;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_ENABLE_GET_UNSAFE);

  /*
   *  Just read one register as a representative
   */
  SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, MESH_TOPOLOGY_MESH_TOPOLOGYr, REG_PORT_ANY, 0, CONFIG_6f, &fld_val));
  *enable = SOC_SAND_NUM2BOOL(fld_val);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_enable_get_unsafe()",0,0);
}

/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting control cells.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_all_ctrl_cells_enable_set_unsafe(
    SOC_SAND_IN  int  unit,
    SOC_SAND_IN  uint8  enable,
    SOC_SAND_IN  uint32  flags

  )
{
  uint32
    res = SOC_SAND_OK;
  ARAD_CTRL_CELL_DATA
    *conf = NULL;
  uint32
    conf_idx = 0,
    nof_confs = 0,
    write_idx = 0,
    inst_idx = 0;
   

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_ENABLE_SET_UNSAFE);
  
  ARAD_ALLOC(conf, ARAD_CTRL_CELL_DATA, ARAD_MGMT_CTRL_CELLS_MAX_NOF_CONFS, "arad_mgmt_all_ctrl_cells_enable_set_unsafe.conf");
  res = soc_sand_os_memset(
          conf,
          0x0,
          ARAD_MGMT_CTRL_CELLS_MAX_NOF_CONFS * sizeof(ARAD_CTRL_CELL_DATA)
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 10, exit);

  for (inst_idx = 0; inst_idx < SOC_DPP_DEFS_GET(unit, nof_instances_fmac); inst_idx++)
  {
    conf[conf_idx].reg   = (enable == TRUE ? FMAC_LEAKY_BUCKET_CONTROL_REGISTERr:INVALIDr);
    conf[conf_idx].field = (enable == TRUE ? BKT_FILL_RATEf:INVALIDr);
    conf[conf_idx].val = soc_sand_link_fap_bkt_fill_rate_get();
    conf[conf_idx].instance_id = inst_idx;
    conf[conf_idx].polling_func = NULL;
    conf[conf_idx].failure_func = NULL;
    conf[conf_idx].err_on_fail = FALSE;
    conf[conf_idx++].delay_or_polling_iters = 0;

    conf[conf_idx].reg = (enable == TRUE ? FMAC_LEAKY_BUCKET_CONTROL_REGISTERr:INVALIDr);
    conf[conf_idx].field = (enable == TRUE ? BKT_LINK_DN_THf:INVALIDr);
    conf[conf_idx].val = soc_sand_link_fap_dn_link_th_get();
    conf[conf_idx].instance_id = inst_idx;
    conf[conf_idx].polling_func = NULL;
    conf[conf_idx].failure_func = NULL;
    conf[conf_idx].err_on_fail = FALSE;
    conf[conf_idx++].delay_or_polling_iters = 0;

    conf[conf_idx].reg = (enable == TRUE ? FMAC_LEAKY_BUCKET_CONTROL_REGISTERr:INVALIDr);
    conf[conf_idx].field = (enable == TRUE ? BKT_LINK_UP_THf:INVALIDr);
    conf[conf_idx].val = soc_sand_link_fap_up_link_th_get();
    conf[conf_idx].instance_id = inst_idx;
    conf[conf_idx].polling_func = NULL;
    conf[conf_idx].failure_func = NULL;
    conf[conf_idx].err_on_fail = FALSE;
    conf[conf_idx++].delay_or_polling_iters = 0;
  }

  conf[conf_idx].reg = RTP_RTP_ENABLEr;
  conf[conf_idx].field = RMGRf;
  conf[conf_idx].val = (enable == TRUE ? soc_sand_link_fap_reachability_rate_get(arad_chip_ticks_per_sec_get(unit), SOC_DPP_DEFS_GET(unit, fabric_rmgr_nof_links), SOC_DPP_DEFS_GET(unit, fabric_rmgr_units)) : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = RTP_RTP_ENABLEr;
  conf[conf_idx].field = RTPWPf;
  conf[conf_idx].val = (enable == TRUE ? soc_sand_link_fap_reachability_watchdog_period_get(arad_chip_ticks_per_sec_get(unit)) : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = RTP_RTP_ENABLEr;
  conf[conf_idx].field = RTP_EN_MSKf;
  conf[conf_idx].val = (enable == TRUE ? 0x1 : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = RTP_RTP_ENABLEr;
  conf[conf_idx].field = RTP_UP_ENf;
  conf[conf_idx].val = (enable == TRUE ? 0x1 : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_STSf;
  conf[conf_idx].val = 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_CRDf;
  conf[conf_idx].val = 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_RCHf;
  conf[conf_idx].val = (enable == TRUE ? 0x0 : 0x1);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCR_FCR_ENABLERS_AND_FILTER_MATCH_INPUT_LINKr;
  conf[conf_idx].field = TRAP_ALL_CNTf;
  conf[conf_idx].val = (enable == TRUE ? 0x0 : 0x1);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_fct_enable_polling : NULL);
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? ARAD_MGMT_INIT_CTRL_RCH_STATUS_ITERATIONS : 0);

  
  for (inst_idx = 0; inst_idx < SOC_DPP_DEFS_GET(unit, nof_instances_fmac); inst_idx++)
  {
      if ( !SOC_IS_JERICHO(unit) || (SOC_DPP_CONFIG(unit)->arad->init.fabric.connect_mode != SOC_TMC_FABRIC_CONNECT_MODE_MESH) ) 
      {
          conf[conf_idx].reg = FMAC_LINK_TOPO_MODE_REG_0r;
          conf[conf_idx].field = LINK_TOPO_MODE_0f;
          conf[conf_idx].val = (enable == TRUE ? 0xf : 0x0);
          conf[conf_idx].instance_id = inst_idx;
          conf[conf_idx].polling_func = NULL;
          conf[conf_idx].failure_func = NULL;
          conf[conf_idx].err_on_fail = FALSE;
          conf[conf_idx++].delay_or_polling_iters = 0;
      }

      conf[conf_idx].reg = FMAC_LINK_TOPO_MODE_REG_2r;
      conf[conf_idx].field = LINK_TOPO_MODE_2f;
      conf[conf_idx].val = (enable == TRUE ? 0xf : 0x0);
      conf[conf_idx].instance_id = inst_idx;
      conf[conf_idx].polling_func = NULL;
      conf[conf_idx].failure_func = NULL;
      conf[conf_idx].err_on_fail = FALSE;
      conf[conf_idx++].delay_or_polling_iters = 0;
  
  }
  
  conf[conf_idx].reg = MESH_TOPOLOGY_THRESHOLDSr;
  conf[conf_idx].field = THRESHOLD_2f;
  conf[conf_idx].val = (enable == TRUE ? 0x14 : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = (flags & ARAD_MGMT_ALL_CTRL_CELLS_FLAGS_SOFT_RESET) == 0 ? MESH_TOPOLOGY_MESH_TOPOLOGYr : INVALIDr; /*Do not config as part of soft reset sequance*/
  conf[conf_idx].field = (flags & ARAD_MGMT_ALL_CTRL_CELLS_FLAGS_SOFT_RESET) == 0 ? CONFIG_6f : INVALIDf; /*Do not config as part of soft reset sequance*/
  conf[conf_idx].val = (enable == TRUE ? 0x1 : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = MESH_TOPOLOGY_INITr;
  conf[conf_idx].field = INITf;
  conf[conf_idx].val = (enable == TRUE ? 0xd : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_standalone_polling : NULL);
  conf[conf_idx].failure_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_standalone_failure : NULL);
  conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? ARAD_MGMT_INIT_STANDALONE_ITERATIONS : 0);

  conf[conf_idx].reg = INVALIDr;
  conf[conf_idx].field = INVALIDr;
  conf[conf_idx].val = 0;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_status_polling : NULL);
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = TRUE; /* Throw error in case polling failed */
  conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? ARAD_MGMT_INIT_CTRL_CELLS_TIMER_ITERATIONS : 0);

  conf[conf_idx].reg = MESH_TOPOLOGY_MESH_TOPOLOGYr;
  conf[conf_idx].field = CONFIG_5f;
  conf[conf_idx].val = (enable == TRUE ? 0x7 : 0x0);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCR_FCR_ENABLERS_AND_FILTER_MATCH_INPUT_LINKr;
  conf[conf_idx].field = TRAP_ALL_CNTf;
  conf[conf_idx].val = (enable == TRUE)?0x0 : 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = (enable == TRUE ? NULL : arad_mgmt_all_ctrl_cells_fct_disable_polling);
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? 0 : ARAD_MGMT_INIT_CTRL_CELLS_TIMER_ITERATIONS);

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_STSf;
  conf[conf_idx].val = (enable == TRUE)?0x0 : 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_CRDf;
  conf[conf_idx].val = (enable == TRUE)?0x0 : 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  conf[conf_idx].reg = FCT_FCT_ENABLER_REGISTERr;
  conf[conf_idx].field = DIS_RCHf;
  conf[conf_idx].val = (enable == TRUE)?0x0 : 0x1;
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  for (inst_idx = 0; inst_idx < SOC_DPP_DEFS_GET(unit, nof_instances_fmac); inst_idx++)
  {
    conf[conf_idx].reg = FMAC_GENERAL_CONFIGURATION_REGISTERr;
    conf[conf_idx].field = ENABLE_SERIAL_LINKf;
    conf[conf_idx].val = (enable == TRUE ? 0x0 : 0x1);
    conf[conf_idx].instance_id = inst_idx;
    conf[conf_idx].polling_func = NULL;
    conf[conf_idx].failure_func = NULL;
    conf[conf_idx].err_on_fail = FALSE;
    conf[conf_idx++].delay_or_polling_iters = (((enable == TRUE) && (inst_idx == (SOC_DPP_DEFS_GET(unit, nof_instances_fmac) - 1))) || ((enable == FALSE) && (inst_idx == 0)) ? 16 : 0);
  }

  conf[conf_idx].reg = FDT_FDT_ENABLER_REGISTERr;
  conf[conf_idx].field = DISCARD_DLL_PKTSf;
  conf[conf_idx].val = (enable == TRUE ? 0x0 : 0x1);
  conf[conf_idx].instance_id = REG_PORT_ANY;
  conf[conf_idx].polling_func = NULL;
  conf[conf_idx].failure_func = NULL;
  conf[conf_idx].err_on_fail = FALSE;
  conf[conf_idx++].delay_or_polling_iters = 0;

  if (SOC_IS_JERICHO(unit)) {
      conf[conf_idx].reg = MESH_TOPOLOGY_REG_0117r; 
      conf[conf_idx].field = FIELD_0_2f;
      conf[conf_idx].val = (enable == TRUE ? 0x5 : 0x0);
      conf[conf_idx].instance_id = REG_PORT_ANY;
      conf[conf_idx].polling_func = NULL;
      conf[conf_idx].failure_func = NULL;
      conf[conf_idx].err_on_fail = FALSE;
      conf[conf_idx++].delay_or_polling_iters = 0;
  }
  nof_confs = conf_idx;

  res = arad_mgmt_ctrl_cells_counter_clear(
          unit
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

#if ARAD_MGMT_DBG_ON
  LOG_CLI(
           (BSL_META_U(unit,
                       "\n\r!!DBG ON, print write-accesses (Addr,Mask,Value) and function calls\n\r")));
  if (enable){soc_sand_set_print_when_writing(1, 1, 0);}
#endif
  for (conf_idx = 0; conf_idx < nof_confs; ++conf_idx)
  {
    write_idx = (enable == TRUE ? conf_idx : nof_confs - conf_idx - 1);

    res = arad_mgmt_all_ctrl_cells_enable_write(
            unit,
            &(conf[write_idx]),
            TRUE
         );
    SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);
  }

#if ARAD_MGMT_DBG_ON
  if (enable){soc_sand_set_print_when_writing(0, 0, 0);}
#endif

exit:
  ARAD_FREE(conf);
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_enable_set_unsafe()",conf_idx,SOC_SAND_BOOL2NUM(enable));
}

/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting control cells.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_all_ctrl_cells_enable_verify(
    SOC_SAND_IN  int  unit,
    SOC_SAND_IN  uint8  enable
  )
{

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ALL_CTRL_CELLS_ENABLE_VERIFY);

  ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_all_ctrl_cells_enable_verify()",0,0);
}

/*********************************************************************
*     Enable / Disable the forcing of (bypass) TDM cells to fabric
*********************************************************************/
uint32
  arad_force_tdm_bypass_traffic_to_fabric_set_unsafe(
    SOC_SAND_IN  int     unit,
    SOC_SAND_IN  int     enable
  )
{
    uint32 res;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res, 110, exit, ARAD_REG_ACCESS_ERR,
      soc_reg_field32_modify(unit, FDT_FDT_ENABLER_REGISTERr, REG_PORT_ANY, DIS_LCLRTf, enable ? 1 : 0));

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_force_tdm_bypass_traffic_to_fabric_set_unsafe()", enable, 0);
}
/*********************************************************************
*     Check if forcing of (bypass) TDM cells to fabric
*********************************************************************/
uint32
  arad_force_tdm_bypass_traffic_to_fabric_get_unsafe(
    SOC_SAND_IN  int     unit,
    SOC_SAND_OUT int     *enable
  )
{
    uint32 res, reg_val;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res, 110, exit, ARAD_REG_ACCESS_ERR,
      READ_FDT_FDT_ENABLER_REGISTERr_REG32(unit, &reg_val));
    *enable = (int)soc_reg_field_get(unit, FDT_FDT_ENABLER_REGISTERr, reg_val, DIS_LCLRTf);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_force_tdm_bypass_traffic_to_fabric_get_unsafe()", 0, 0);
}


/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting traffic.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32 arad_mgmt_enable_traffic_set_unsafe(
    SOC_SAND_IN  int  unit,
    SOC_SAND_IN  uint8  enable)
{
    uint32
        res,
        enable_val,
        disable_val;
    uint64 
        reg_val_64,
        fld64_val;
    ARAD_CTRL_CELL_DATA
        *conf = NULL;
    uint32
        conf_idx = 0,
        write_idx = 0,
        nof_confs = 0,
        reg32_val;
    soc_reg_above_64_val_t 
        reg_above_64_val;

    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ENABLE_TRAFFIC_SET_UNSAFE);

    enable_val  = SOC_SAND_BOOL2NUM(enable);
    disable_val = SOC_SAND_BOOL2NUM_INVERSE(enable);
    COMPILER_64_ZERO(reg_val_64);
    /*
    *  Note: the comments are for disabling the traffic.
    *  For enabling the traffic, the action is the opposite of the described
    */

    /*
    * cfg MESH_TOPOLOGY status check 
    */
    if (enable) {
        ARAD_ALLOC(conf, ARAD_CTRL_CELL_DATA, ARAD_MGMT_CTRL_CELLS_MAX_NOF_CONFS, "arad_mgmt_enable_traffic_set_unsafe.conf");
        conf[conf_idx].reg = INVALIDr;
        conf[conf_idx].field = INVALIDr;
        conf[conf_idx].val = 0;
        conf[conf_idx].instance_id = REG_PORT_ANY;
        conf[conf_idx].polling_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_standalone_polling : NULL);
        conf[conf_idx].failure_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_standalone_failure : NULL);
        conf[conf_idx].err_on_fail = FALSE; /* Do not throw error in case polling failed */
        conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? ARAD_MGMT_INIT_STANDALONE_ITERATIONS : 0);

        conf[conf_idx].reg = INVALIDr;
        conf[conf_idx].field = INVALIDr;
        conf[conf_idx].val = 0;
        conf[conf_idx].instance_id = REG_PORT_ANY;
        conf[conf_idx].polling_func = (enable == TRUE ? arad_mgmt_all_ctrl_cells_status_polling : NULL);
        conf[conf_idx].failure_func = NULL;
        conf[conf_idx].err_on_fail = TRUE; /* Throw error in case polling failed */
        conf[conf_idx++].delay_or_polling_iters = (enable == TRUE ? ARAD_MGMT_INIT_CTRL_CELLS_TIMER_ITERATIONS : 0);

        nof_confs = conf_idx;


        for (conf_idx = 0; conf_idx < nof_confs; ++conf_idx) {
            write_idx = (enable == TRUE ? conf_idx : nof_confs - conf_idx - 1);

            res = arad_mgmt_all_ctrl_cells_enable_write(unit, &(conf[write_idx]), TRUE);
            SOC_SAND_CHECK_FUNC_RESULT(res, 100 + write_idx, exit);
        }
    }
    /*
    *  Disable data path at the ingress -
    *  I.e., if disabled - no packet will enter the ingress DRAM and fabric.
    *  IDR, IRR, IRE
    */
    if (enable == FALSE) {

        if (SOC_IS_ARDON(unit)) {
            reg32_val = 0;
            /* Disable traffic from NPU and signal NPU traffic is disabled */
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_INGRESS_DATA_PATH_READYf, 0x1);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_DRAINf, 0x1);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_FLOW_CONTROLf, 0x0);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ATMF_DATA_PATH_STATUSf, 0x0);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_MASKf, 0x0);
            SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  6,  exit, ARAD_REG_ACCESS_ERR, soc_reg32_set(unit, NBI_ENABLE_DATA_PATHr, REG_PORT_ANY, 0, reg32_val));            
        }

        /*
        *  Stop traffic from the NIFs
        */
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  8,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IRE_STATIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATHf,  enable_val));
        /*
        * Stop Data traffic from the fabric.
        */
         SOC_REG_ABOVE_64_ALLONES(reg_above_64_val);
         SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, soc_reg_above_64_set(unit, FDR_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_0r, REG_PORT_ANY, 0, reg_above_64_val));
         COMPILER_64_ALLONES(reg_val_64);
         SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  12,  exit, ARAD_REG_ACCESS_ERR,WRITE_FDR_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_1r(unit,  reg_val_64));
         
         COMPILER_64_ZERO(reg_val_64);
         COMPILER_64_SET(fld64_val,0,0x2);
         ARAD_FLD_TO_REG64(FDR_FDR_ENABLERS_REGISTER_1r, FDR_MTCH_ACTf, fld64_val, reg_val_64, 14, exit);
         SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  14,  exit, ARAD_REG_ACCESS_ERR,WRITE_FDR_FDR_ENABLERS_REGISTER_1r(unit,  reg_val_64)); 

         /* Stop Traffic from FDR */
         SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  16,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, FDR_FDR_ENABLERS_REGISTER_1r, REG_PORT_ANY, 0, FIELD_31_31f, 0x1));
         SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  18,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, FDR_FDR_ENABLERS_REGISTER_1r, REG_PORT_ANY, 0, FIELD_32_32f, 0x1));
      }
  
      /*
      *  Disable internal data-path - just in case
      */
     SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  20,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IDR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATHf,  enable_val));
     SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  22,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IRR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATH_IDRf,  enable_val));
     SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  24,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IRR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATH_IQMf,  enable_val));
     SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  26,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IHP_IHP_ENABLERSr, SOC_CORE_ALL, 0, ENABLE_DATA_PATHf,  enable_val));
     if (SOC_IS_JERICHO(unit) && SOC_DPP_CONFIG(unit)->arad->init.ocb.ocb_enable == OCB_ENABLED) {
         res = soc_reg_above_64_field32_modify(unit, OCB_GENERAL_CONFIGr, REG_PORT_ANY, 0, ENABLE_IDR_TRAFFICf, enable_val);
         SOC_SAND_CHECK_FUNC_RESULT(res, 28, exit);
     }
    /*
    *  Stop credit reception from the fabric
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  50,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, SCH_SCHEDULER_CONFIGURATION_REGISTERr, SOC_CORE_ALL, 0, DISABLE_FABRIC_MSGSf,  disable_val));

    /*
    *  Stop credit generation
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  60,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, SCH_DVS_CONFIG_1r, REG_PORT_ANY, 0, FORCE_PAUSEf,  disable_val));


    /*
    *  Stop dequeue from all queues
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  70,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IPS_IPS_GENERAL_CONFIGURATIONSr, SOC_CORE_ALL, 0, DIS_DEQ_CMDSf,  disable_val));

    /*
    *  Discard all credits in the IPS
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  80,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IPS_IPS_GENERAL_CONFIGURATIONSr, SOC_CORE_ALL, 0, DISCARD_ALL_CRDTf,  disable_val));

    /*
    *  Discard all packets currently in the queues
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  90,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IQM_IQM_ENABLERSr, SOC_CORE_ALL, 0, DSCRD_ALL_PKTf,  disable_val));

    if (enable == TRUE) {
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  100,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IRE_STATIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATHf,  enable_val));

        if (SOC_IS_ARDON(unit)) {
            /* Enable traffic from NPU and signal NPU traffic is enabled*/
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_INGRESS_DATA_PATH_READYf, 0x1);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_DRAINf, 0x0);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_FLOW_CONTROLf, 0x0);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ATMF_DATA_PATH_STATUSf, 0x1);
            soc_reg_field_set(unit, NBI_ENABLE_DATA_PATHr, &reg32_val, ENABLE_EGRESS_DATA_PATH_MASKf, 0x0);
            SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  105,  exit, ARAD_REG_ACCESS_ERR, soc_reg32_set(unit, NBI_ENABLE_DATA_PATHr, REG_PORT_ANY, 0, reg32_val));
        }
        /*
        * Set the device to receive from the fabric.
        * Note: may be overridden later by "fixes_apply" API.
        */
        COMPILER_64_SET(reg_val_64, ARAD_MGMT_FDR_TRFC_ENABLE_VAR_CELL_MSB, ARAD_MGMT_FDR_TRFC_ENABLE_VAR_CELL_LSB);
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  110,  exit, ARAD_REG_ACCESS_ERR,WRITE_FDR_FDR_ENABLERS_REGISTER_1r(unit,  reg_val_64));
        /*
         * Reconfigure ALDWP
         */
        res = arad_fabric_aldwp_config(unit);
        SOC_SAND_SOC_CHECK_FUNC_RESULT(res, 120, exit);

        SOC_REG_ABOVE_64_CLEAR(reg_above_64_val);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, soc_reg_above_64_set(unit, FDR_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_0r, REG_PORT_ANY, 0, reg_above_64_val));
        COMPILER_64_ZERO(reg_val_64);
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  20,  exit, ARAD_REG_ACCESS_ERR,WRITE_FDR_PROGRAMMABLE_DATA_CELL_COUNTER_MASK_1r(unit,  reg_val_64)); 
    }

exit:
    ARAD_FREE(conf);
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_enable_traffic_set_unsafe()",0,0);
}


/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting traffic.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_enable_traffic_verify(
    SOC_SAND_IN  int  unit,
    SOC_SAND_IN  uint8  enable
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ENABLE_TRAFFIC_VERIFY);

  ARAD_DO_NOTHING_AND_EXIT;

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_enable_traffic_verify()",0,0);
}

/*********************************************************************
*     Enable / Disable the device from receiving and
*     transmitting traffic.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32 arad_mgmt_enable_traffic_get_unsafe(
    SOC_SAND_IN  int  unit,
    SOC_SAND_OUT uint8  *enable)
{
    uint32
        res,
        enable_val;
    uint8
        enable_curr = FALSE,
        enable_all = TRUE;
    uint64
        enable_val64;

    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_ENABLE_TRAFFIC_GET_UNSAFE);

    SOC_SAND_CHECK_NULL_INPUT(enable);

    /*
    *  Check the ingress data path at the ingress -
    *  I.e., if disabled - no packet will enter the ingress DRAM and fabric.
    *  IDR, IRR, IRE
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  2,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IDR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATHf, &enable_val));
    enable_curr = SOC_SAND_NUM2BOOL(enable_val);
    enable_all = (enable_all && enable_curr);

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  4,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IRR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATH_IDRf, &enable_val));
    enable_curr = SOC_SAND_NUM2BOOL(enable_val);
    enable_all = (enable_all && enable_curr);

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  6,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IRR_DYNAMIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATH_IQMf, &enable_val));
    enable_curr = SOC_SAND_NUM2BOOL(enable_val);
    enable_all = (enable_all && enable_curr);

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  8,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IRE_STATIC_CONFIGURATIONr, REG_PORT_ANY, 0, ENABLE_DATA_PATHf, &enable_val));
    enable_curr = SOC_SAND_NUM2BOOL(enable_val);
    enable_all = (enable_all && enable_curr);

    /*
    * Check fabric receive.
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  8,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field64_read(unit, FDR_FDR_ENABLERS_REGISTER_1r, REG_PORT_ANY, 0, FDR_MTCH_ACTf, &enable_val64));
    enable_curr = SOC_SAND_NUM2BOOL_INVERSE(COMPILER_64_IS_ZERO(enable_val64));
    enable_all = (enable_all && enable_curr);


    /*
    * Check SMP messages are enabled (scheduler).
    */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  70,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, SCH_SCHEDULER_CONFIGURATION_REGISTERr, SOC_CORE_ALL, 0, DISABLE_FABRIC_MSGSf, &enable_val));
    enable_curr = !(SOC_SAND_NUM2BOOL(enable_val));
    enable_all = (enable_all && enable_curr);

    *enable = enable_all;

exit:
    SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_enable_traffic_get_unsafe()",0,0);
}

/*********************************************************************
*     Set the maximal allowed packet size. The limitation can
 *     be performed based on the packet size before or after
 *     the ingress editing (external and internal configuration
 *     mode, accordingly). Packets above the specified value
 *     are dropped.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_max_pckt_size_set_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      port_ndx,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx,
    SOC_SAND_IN  uint32                       max_size
  )
{
  uint32
    max_size_lcl,
    res = SOC_SAND_OK;
  ARAD_IDR_CONTEXT_MRU_TBL_DATA
    tbl_data;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_MAX_PCKT_SIZE_SET_UNSAFE);

  res = arad_idr_context_mru_tbl_get_unsafe(
          unit,
          port_ndx,
          &tbl_data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  switch(conf_mode_ndx) {
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_INTERN:
    max_size_lcl = (max_size)-ARAD_MGMT_PCKT_RNG_CORRECTION_INTERNAL;
    tbl_data.size = max_size_lcl;
    break;

  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_EXTERN:
    max_size_lcl = (max_size)-ARAD_MGMT_PCKT_RNG_CORRECTION_EXTERNAL;
    tbl_data.org_size = max_size_lcl;
    break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 10, exit);
  }

  res = arad_idr_context_mru_tbl_set_unsafe(
          unit,
          port_ndx,
          &tbl_data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 30, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_max_pckt_size_set_unsafe()", port_ndx, 0);
}

uint32
  arad_mgmt_max_pckt_size_set_verify(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      port_ndx,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx,
    SOC_SAND_IN  uint32                       max_size
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_MAX_PCKT_SIZE_SET_VERIFY);

  SOC_SAND_ERR_IF_ABOVE_MAX(port_ndx, ARAD_PORT_NDX_MAX, ARAD_PORT_NDX_OUT_OF_RANGE_ERR, 10, exit);
  SOC_SAND_ERR_IF_ABOVE_MAX(conf_mode_ndx, ARAD_CONF_MODE_NDX_MAX, ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 20, exit);
  switch(conf_mode_ndx) {
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_INTERN:
    SOC_SAND_ERR_IF_ABOVE_MAX(max_size, ARAD_MGMT_PCKT_MAX_SIZE_INTERNAL_MAX, ARAD_MGMT_PCKT_MAX_SIZE_INTERNAL_OUT_OF_RANGE_ERROR, 30, exit);
    break;
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_EXTERN:
        SOC_SAND_ERR_IF_ABOVE_MAX(max_size, ARAD_MGMT_PCKT_MAX_SIZE_EXTERNAL_MAX, ARAD_MGMT_PCKT_MAX_SIZE_EXTERNAL_OUT_OF_RANGE_ERROR, 40, exit);
    break;
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 50, exit);
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_max_pckt_size_set_verify()", port_ndx, 0);
}

uint32
  arad_mgmt_max_pckt_size_get_verify(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      port_ndx,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx
  )
{
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_MAX_PCKT_SIZE_GET_VERIFY);

  SOC_SAND_ERR_IF_ABOVE_MAX(port_ndx, ARAD_PORT_NDX_MAX, ARAD_PORT_NDX_OUT_OF_RANGE_ERR, 10, exit);
  SOC_SAND_ERR_IF_ABOVE_MAX(conf_mode_ndx, ARAD_CONF_MODE_NDX_MAX, ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 20, exit);

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_max_pckt_size_get_verify()", port_ndx, 0);
}

/*********************************************************************
*     Set the maximal allowed packet size. The limitation can
 *     be performed based on the packet size before or after
 *     the ingress editing (external and internal configuration
 *     mode, accordingly). Packets above the specified value
 *     are dropped.
 *     Details: in the H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_max_pckt_size_get_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      port_ndx,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx,
    SOC_SAND_OUT uint32                       *max_size
  )
{
  uint32
    max_size_lcl,
    res = SOC_SAND_OK;
  ARAD_IDR_CONTEXT_MRU_TBL_DATA
    tbl_data;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_MAX_PCKT_SIZE_GET_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(max_size);

  res = arad_idr_context_mru_tbl_get_unsafe(
          unit,
          port_ndx,
          &tbl_data
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 20, exit);

  switch(conf_mode_ndx) {
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_INTERN:
    max_size_lcl = tbl_data.size;
    *max_size = max_size_lcl + ARAD_MGMT_PCKT_RNG_CORRECTION_INTERNAL;
    break;

  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_EXTERN:
    max_size_lcl = tbl_data.org_size;
    *max_size = max_size_lcl + ARAD_MGMT_PCKT_RNG_CORRECTION_EXTERNAL;
    break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 70, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_max_pckt_size_get_unsafe()", port_ndx, 0);
}


/*********************************************************************
 * Set the MTU (maximal allowed packet size) for any packet,
 * according to the buffer size.
 *********************************************************************/
uint32
  arad_mgmt_set_mru_by_dbuff_size(
    SOC_SAND_IN  int     unit
  )
{
    uint32 mru = SOC_DPP_CONFIG(unit)->arad->init.dram.dbuff_size * ARAD_MGMT_MAX_BUFFERS_PER_PACKET;
    uint32 entry = 0;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    if (mru > ARAD_MGMT_PCKT_SIZE_BYTES_VSC_MAX) {
         mru = ARAD_MGMT_PCKT_SIZE_BYTES_VSC_MAX;
    }
    soc_IDR_CONTEXT_MRUm_field32_set(unit, &entry, MAX_ORG_SIZEf, mru + 1);
    soc_IDR_CONTEXT_MRUm_field32_set(unit, &entry, MAX_SIZEf, mru - 1);
    SOC_SAND_CHECK_FUNC_RESULT( arad_fill_table_with_entry(unit, IDR_CONTEXT_MRUm, MEM_BLOCK_ANY, &entry), 100, exit);
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_set_mru_by_dbuff_size()", mru, entry);
}

uint32
  arad_mgmt_pckt_size_range_set_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE          *size_range
  )
{
  uint32
    max_size,
    res;
   
    
  uint32
    port_ndx;

  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_PCKT_SIZE_RANGE_SET_UNSAFE);

  SOC_SAND_CHECK_NULL_INPUT(size_range);

  

  /*
   *    Minimum size configuration
   */
  switch(conf_mode_ndx) {
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_INTERN:
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IDR_PACKET_SIZESr, REG_PORT_ANY, 0, MIN_PACKET_SIZEf,  (size_range->min)-ARAD_MGMT_PCKT_RNG_CORRECTION_INTERNAL));
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  20,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EGQ_MAXIMUM_AND_MINIMUM_PACKET_SIZEr, SOC_CORE_ALL, 0, FABRIC_MIN_PKT_SIZEf,  size_range->min));
    break;
 
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_EXTERN:
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  30,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IDR_PACKET_SIZESr, REG_PORT_ANY, 0, MIN_ORG_PACKET_SIZEf,  (size_range->min)-ARAD_MGMT_PCKT_RNG_CORRECTION_EXTERNAL));
    break;

  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 40, exit);
  }

  /*
   *    Maximum size configuration
   */

  max_size = size_range->max;

  for (port_ndx = 0; port_ndx < ARAD_NOF_FAP_PORTS; ++port_ndx)
  {
    res = arad_mgmt_max_pckt_size_set_unsafe(
            unit,
            port_ndx,
            conf_mode_ndx,
            max_size
          );
    SOC_SAND_CHECK_FUNC_RESULT(res, 60, exit);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_pckt_size_range_set_unsafe()",0,0);
}

uint32
  arad_mgmt_pckt_size_range_get_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  ARAD_MGMT_PCKT_SIZE_CONF_MODE conf_mode_ndx,
    SOC_SAND_OUT ARAD_MGMT_PCKT_SIZE          *size_range
  )
{
  uint32
    port_ndx,
    max_size,
    idr_min,
    egq_min,
    fld_val,
    res;
   
    


  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_PCKT_SIZE_RANGE_GET_UNSAFE);
  SOC_SAND_CHECK_NULL_INPUT(size_range);

  

  arad_ARAD_MGMT_PCKT_SIZE_clear(size_range);

  switch(conf_mode_ndx) {
  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_INTERN:
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  10,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IDR_PACKET_SIZESr, REG_PORT_ANY, 0, MIN_PACKET_SIZEf, &idr_min));
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  20,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, EGQ_MAXIMUM_AND_MINIMUM_PACKET_SIZEr, SOC_CORE_ALL, 0, FABRIC_MIN_PKT_SIZEf, &egq_min));

    if ((idr_min + ARAD_MGMT_PCKT_RNG_CORRECTION_INTERNAL) != egq_min)
    {
      SOC_SAND_SET_ERROR_CODE(ARAD_MIN_PCKT_SIZE_INCONSISTENT_ERR, 30, exit);
    }

    size_range->min = idr_min + ARAD_MGMT_PCKT_RNG_CORRECTION_INTERNAL;
    break;

  case ARAD_MGMT_PCKT_SIZE_CONF_MODE_EXTERN:

    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  40,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IDR_PACKET_SIZESr, REG_PORT_ANY, 0, MIN_ORG_PACKET_SIZEf, &fld_val));
    if (fld_val == 0x0)
    {
      size_range->min = ARAD_MGMT_PCKT_SIZE_EXTERN_NO_LIMIT;
    }
    else
    {
      SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  50,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_read(unit, IDR_PACKET_SIZESr, REG_PORT_ANY, 0, MIN_ORG_PACKET_SIZEf, &idr_min));
       size_range->min = idr_min + ARAD_MGMT_PCKT_RNG_CORRECTION_EXTERNAL;
    }
    break;
  default:
    SOC_SAND_SET_ERROR_CODE(ARAD_MGMT_PCKT_SIZE_CONF_MODE_OUT_OF_RANGE_ERR, 60, exit);
  }

  /*
   *    Get the max size from port 0
   */
  port_ndx = 0;
  res = arad_mgmt_max_pckt_size_get_unsafe(
          unit,
          port_ndx,
          conf_mode_ndx,
          &max_size
        );
  SOC_SAND_CHECK_FUNC_RESULT(res, 70, exit);

  size_range->max = max_size;
  

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR( "error in arad_mgmt_pckt_size_range_get_unsafe()",0,0);
}

/* 
 * To disable the Multicast range, it is the user's responsibility to set both Multicast min & max to -1.
 * In case of -1, the Driver configures the min value to 64K-1 and the max value to 0.
 */
uint32
  arad_mgmt_ocb_mc_range_set_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      range_ndx,
    SOC_SAND_IN  ARAD_MGMT_OCB_MC_RANGE         *range
  )
{
  uint32
    res,
    reg_val;
  const static soc_reg_t
    idr_ocb_multicast_range[] = {IDR_OCB_MULTICAST_RANGE_0r, IDR_OCB_MULTICAST_RANGE_1r};
  const static soc_field_t
    ocb_multicast_range_low[] = {OCB_MULTICAST_RANGE_0_LOWf, OCB_MULTICAST_RANGE_1_LOWf},
    ocb_multicast_range_high[] = {OCB_MULTICAST_RANGE_0_HIGHf, OCB_MULTICAST_RANGE_1_HIGHf};
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_MC_RANGE_SET_UNSAFE);

  if (SOC_IS_JERICHO(unit)) 
  {
      uint64 reg_64;
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, soc_reg64_get(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, REG_PORT_ANY, range_ndx, &reg_64)); 
      soc_reg64_field32_set(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, &reg_64, OCB_ELIGIBLE_MULTICAST_RANGE_N_LOWf, range->min);
      soc_reg64_field32_set(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, &reg_64, OCB_ELIGIBLE_MULTICAST_RANGE_N_HIGHf, range->max);
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, soc_reg64_set(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, REG_PORT_ANY, range_ndx, reg_64));
  } else 
  {
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 30, exit, soc_reg32_get(unit, idr_ocb_multicast_range[range_ndx], REG_PORT_ANY, 0, &reg_val)); 
      soc_reg_field_set(unit, idr_ocb_multicast_range[range_ndx], &reg_val, ocb_multicast_range_low[range_ndx], range->min);
      soc_reg_field_set(unit, idr_ocb_multicast_range[range_ndx], &reg_val, ocb_multicast_range_high[range_ndx], range->max);
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 40, exit, soc_reg32_set(unit, idr_ocb_multicast_range[range_ndx], REG_PORT_ANY, 0, reg_val));
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_mc_range_set_unsafe()", 0, 0);
}

/*
 * In case min value is 64K-1 or max value is 0, the Driver returns value '-1'. 
 */ 
uint32
  arad_mgmt_ocb_mc_range_get_unsafe(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  uint32                      range_ndx,
    SOC_SAND_OUT ARAD_MGMT_OCB_MC_RANGE         *range
  )
{
  uint32
    res,
    reg_val;
  const static soc_reg_t
    idr_ocb_multicast_range[] = {IDR_OCB_MULTICAST_RANGE_0r, IDR_OCB_MULTICAST_RANGE_1r};
  const static soc_field_t
    ocb_multicast_range_low[] = {OCB_MULTICAST_RANGE_0_LOWf, OCB_MULTICAST_RANGE_1_LOWf},
    ocb_multicast_range_high[] = {OCB_MULTICAST_RANGE_0_HIGHf, OCB_MULTICAST_RANGE_1_HIGHf};
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_MC_RANGE_GET_UNSAFE);
  
  if(SOC_IS_JERICHO(unit))
  {
      uint64 reg_64;
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, soc_reg64_get(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, REG_PORT_ANY, range_ndx, &reg_64)); 
      range->min = soc_reg64_field32_get(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, reg_64, OCB_ELIGIBLE_MULTICAST_RANGE_N_LOWf );
      range->max = soc_reg64_field32_get(unit, IDR_OCB_ELIGIBLE_MULTICAST_RANGEr, reg_64, OCB_ELIGIBLE_MULTICAST_RANGE_N_HIGHf );
  } else
  {
      SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, soc_reg32_get(unit, idr_ocb_multicast_range[range_ndx], REG_PORT_ANY, 0, &reg_val)); 
      range->min = soc_reg_field_get(unit, idr_ocb_multicast_range[range_ndx], reg_val, ocb_multicast_range_low[range_ndx]);
      range->max = soc_reg_field_get(unit, idr_ocb_multicast_range[range_ndx], reg_val, ocb_multicast_range_high[range_ndx]);
  }

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_mc_range_get_unsafe()", 0, 0);
}

/* This function get the value we would like to insert and retuen the field value should be inserted to the files
   OCB_QUE_BUFF_SIZE_EN_TH_[0/1]f and OCB_QUE_SIZE_EN_TH_[0/1]f
*/
STATIC uint32
    arad_mgmt_ocb_prm_value_to_field_value(
        SOC_SAND_IN  int unit,
        SOC_SAND_IN  uint32 value,
        SOC_SAND_OUT uint32 *field_value
    )
{
    uint32
        res;
    uint32
        mantissa,
        exponent;
        
    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_PRM_VALUE_TO_FIELD_VALUE);

    res = soc_sand_break_to_mnt_exp_round_up(
            SOC_SAND_DIV_ROUND_UP(value, ARAD_MGMT_IQM_WORDS_RESOLUTION), /* convert value to 16B resolution */
            ARAD_MGMT_IQM_OCBPRM_MANTISSA_NOF_BITS(unit),
            ARAD_MGMT_IQM_OCBPRM_EXPONENT_NOF_BITS,
            0,
            &mantissa,
            &exponent
          );
  SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);

  arad_iqm_mantissa_exponent_set(unit,
    mantissa,
    exponent,
    ARAD_MGMT_IQM_OCBPRM_MANTISSA_NOF_BITS(unit),
    field_value
  );
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_prm_value_to_field_value()", 0, 0);
}

STATIC uint32
    arad_mgmt_ocb_prm_buff_size_value_to_field_value(
        SOC_SAND_IN  int unit,
        SOC_SAND_IN  uint32 value,
        SOC_SAND_OUT uint32 *field_value
    )
{
    uint32
        res;
    uint32
        mantissa,
        exponent;
        
    SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_PRM_VALUE_TO_FIELD_VALUE);

    res = soc_sand_break_to_mnt_exp_round_up(value,
            ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_MANTISSA_NOF_BITS(unit),
            ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_EXPONENT_NOF_BITS,
            0,
            &mantissa,
            &exponent
          );
  SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);

  arad_iqm_mantissa_exponent_set(unit,
    mantissa,
    exponent,
    ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_MANTISSA_NOF_BITS(unit),
    field_value
  );
  
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_prm_buff_size_value_to_field_value()", 0, 0);
}
/* This function takes the field value of OCB_QUE_SIZE_EN_TH_[0/1]f,
   and give back the exact value */
STATIC uint32
    arad_mgmt_ocb_prm_field_value_to_exact_value(
        SOC_SAND_IN int unit,
        SOC_SAND_IN uint32 field_value
    )
{
    uint32
        exponent,
        mantissa,
        exact_value = 0;
        
    arad_iqm_mantissa_exponent_get(unit,
      field_value,
      ARAD_MGMT_IQM_OCBPRM_MANTISSA_NOF_BITS(unit),
      &mantissa,
      &exponent
    );

    exact_value = (mantissa * (1 << exponent));
    /* Convert value from 16B to Bytes resolution */
    exact_value *= ARAD_MGMT_IQM_WORDS_RESOLUTION;

    return exact_value;
}

/* This function takes the field value of OCB_QUE_BUFF_SIZE_EN_TH_[0/1]f,
   and give back the exact value */
STATIC uint32
    arad_mgmt_ocb_prm_buff_size_field_value_to_exact_value(
        SOC_SAND_IN int unit,
        SOC_SAND_IN uint32 field_value
    )
{
    uint32
        exponent,
        mantissa;
        
    arad_iqm_mantissa_exponent_get(unit,
      field_value,
      ARAD_MGMT_IQM_OCBPRM_BUFF_SIZE_MANTISSA_NOF_BITS(unit),
      &mantissa,
      &exponent
    );
    
    return mantissa * (1 << exponent);
}

uint32 arad_mgmt_ocb_voq_info_defaults_set(
    SOC_SAND_IN     int                         unit,
    SOC_SAND_OUT    ARAD_MGMT_OCB_VOQ_INFO      *ocb_info
    )
{
    SOCDNX_INIT_FUNC_DEFS;
    SOCDNX_NULL_CHECK(ocb_info);

    ocb_info->th_words[0] = ARAD_MGMT_IQM_OCB_PRM_DEFAULT_SIZE;
    ocb_info->th_words[1] = ARAD_MGMT_IQM_OCB_PRM_DEFAULT_SIZE;
    ocb_info->th_buffers[0] = ARAD_MGMT_IQM_OCB_PRM_DEFAULT_BUFF_SIZE;
    ocb_info->th_buffers[1] = ARAD_MGMT_IQM_OCB_PRM_DEFAULT_BUFF_SIZE;

    exit:
        SOCDNX_FUNC_RETURN;
}
 
uint32
  arad_mgmt_ocb_voq_eligible_set_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    q_category_ndx,
    SOC_SAND_IN  uint32                    rate_class_ndx,
    SOC_SAND_IN  uint32                    tc_ndx,
    SOC_SAND_IN  ARAD_MGMT_OCB_VOQ_INFO       *info,
    SOC_SAND_OUT ARAD_MGMT_OCB_VOQ_INFO    *exact_info
  )
{
  uint32
    res;
  uint32
    index = 0,
    q_category_ndx_lcl = q_category_ndx,
    rate_class_ndx_lcl = rate_class_ndx,
    tc_ndx_lcl = tc_ndx,
    voq_eligible_lcl = info->voq_eligible,
    field_val;
  uint64
    reg_val;
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_VOQ_ELIGIBLE_SET_UNSAFE);
  if (!SOC_IS_QAX(unit)) {  
  /* calculating the index in the table */
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_START, &q_category_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_LENGTH);
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_START, &rate_class_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_LENGTH);
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_START, &tc_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_LENGTH);
  
  /* set the information in the table */
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, READ_IQM_OCBPRMm(unit, MEM_BLOCK_ANY, index, &reg_val));
  
  if (SOC_IS_JERICHO(unit)) {
      soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, DRAM_ADMISSION_EXEMPTf, info->dram_admission_exempt & 0x1);
  } else {
      soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, OCB_ENf, voq_eligible_lcl & 0x1);
  }

  arad_ARAD_MGMT_OCB_VOQ_INFO_clear(exact_info);

  res =  arad_mgmt_ocb_prm_buff_size_value_to_field_value(unit, info->th_buffers[0], &field_val );
  SOC_SAND_CHECK_FUNC_RESULT(res, 12, exit);
  soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_BUFF_SIZE_EN_TH_0f, field_val);  
  exact_info->th_buffers[0] = arad_mgmt_ocb_prm_buff_size_field_value_to_exact_value(unit, field_val);

  res =  arad_mgmt_ocb_prm_buff_size_value_to_field_value(unit, info->th_buffers[1], &field_val );
  SOC_SAND_CHECK_FUNC_RESULT(res, 15, exit);
  soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_BUFF_SIZE_EN_TH_1f, field_val);
  exact_info->th_buffers[1] = arad_mgmt_ocb_prm_buff_size_field_value_to_exact_value(unit, field_val);

  res =  arad_mgmt_ocb_prm_value_to_field_value(unit, info->th_words[0], &field_val );
  SOC_SAND_CHECK_FUNC_RESULT(res, 17, exit);
  soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_SIZE_EN_TH_0f, field_val);
  exact_info->th_words[0] = arad_mgmt_ocb_prm_field_value_to_exact_value(unit, field_val);

  res =  arad_mgmt_ocb_prm_value_to_field_value(unit, info->th_words[1], &field_val );
  SOC_SAND_CHECK_FUNC_RESULT(res, 19, exit);
  soc_mem_field32_set(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_SIZE_EN_TH_1f, field_val);
  exact_info->th_words[1] = arad_mgmt_ocb_prm_field_value_to_exact_value(unit, field_val);

  SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, WRITE_IQM_OCBPRMm(unit, MEM_BLOCK_ANY, index, &reg_val));
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_voq_eligible_set_unsafe()", 0, 0);
}

uint32
  arad_mgmt_ocb_voq_eligible_dynamic_set_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    qid,
    SOC_SAND_IN  uint32                    enable
  )
{
  uint32
    res,
    mem_val,
    line,
    bit_in_line;   
    
  soc_reg_above_64_val_t
     old_wr_mask_val,
     wr_mask_val; 
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(0);
  
  
  sal_memset(wr_mask_val, 0,sizeof(wr_mask_val));
    
  line = qid / 32;
  bit_in_line = qid % 32;

  /*
   * Set indirect wr mask to 1 to allowe writing
   */

  SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, READ_IDR_INDIRECT_WR_MASKr(unit, old_wr_mask_val));
  
  wr_mask_val[0] = 0xffffffff;
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, WRITE_IDR_INDIRECT_WR_MASKr(unit, wr_mask_val)); 

  /*
   * Set relevent bit in IDR_MEM_1F0000 table.
   */
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 30, exit, READ_IDR_MEM_1F0000m(unit, MEM_BLOCK_ANY, line, &mem_val));
  SOC_SAND_SET_BIT(mem_val, enable, bit_in_line);
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 40, exit, WRITE_IDR_MEM_1F0000m(unit, MEM_BLOCK_ANY, line, &mem_val));  

  /* return to the old indirect wr mask value*/
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, READ_IDR_INDIRECT_WR_MASKr(unit, old_wr_mask_val)); 

exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_voq_eligible_dynamic_set_unsafe()", 0, 0);
}



uint32
  arad_mgmt_ocb_voq_eligible_get_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    q_category_ndx,
    SOC_SAND_IN  uint32                    rate_class_ndx,
    SOC_SAND_IN  uint32                    tc_ndx,
    SOC_SAND_OUT ARAD_MGMT_OCB_VOQ_INFO       *info
  )
{
  uint32
    res;
  uint32
    index = 0,
    q_category_ndx_lcl = q_category_ndx,
    rate_class_ndx_lcl = rate_class_ndx,
    tc_ndx_lcl = tc_ndx,
    field_val;
  uint64
    reg_val;
    
  SOC_SAND_INIT_ERROR_DEFINITIONS(ARAD_MGMT_OCB_VOQ_ELIGIBLE_GET_UNSAFE);

  if (!SOC_IS_QAX(unit)) {  
  /* calculating the index in the table */
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_START, &q_category_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_QUEUE_CATEGORY_LENGTH);
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_START, &rate_class_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_RATE_CLASS_LENGTH);
  SHR_BITCOPY_RANGE(&index, ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_START, &tc_ndx_lcl, 0, ARAD_MGMT_IQM_OCB_PRM_TRAFFIC_CLASS_LENGTH);

  /* get the information from the table */  
  SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, READ_IQM_OCBPRMm(unit, MEM_BLOCK_ANY, index, &reg_val));
  if (SOC_IS_JERICHO(unit)) {
      info->voq_eligible = 0;
      info->dram_admission_exempt = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, DRAM_ADMISSION_EXEMPTf);
  } else {
      info->voq_eligible = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, OCB_ENf);
      info->dram_admission_exempt = 0;
  }
  
  field_val = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_BUFF_SIZE_EN_TH_0f);
  info->th_buffers[0] = arad_mgmt_ocb_prm_buff_size_field_value_to_exact_value(unit, field_val);;

  field_val = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_BUFF_SIZE_EN_TH_1f);
  info->th_buffers[1] = arad_mgmt_ocb_prm_buff_size_field_value_to_exact_value(unit, field_val);;
  
  field_val = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_SIZE_EN_TH_0f);
  info->th_words[0] = arad_mgmt_ocb_prm_field_value_to_exact_value(unit, field_val);;

  field_val = soc_mem_field32_get(unit, IQM_OCBPRMm, &reg_val, OCB_QUE_SIZE_EN_TH_1f);
  info->th_words[1] = arad_mgmt_ocb_prm_field_value_to_exact_value(unit, field_val);;
  }
exit:
  SOC_SAND_EXIT_AND_SEND_ERROR("error in arad_mgmt_ocb_voq_eligible_get_unsafe()", 0, 0);
}

/*********************************************************************
 *     Set core clock frequency according to given information in
 *     ARAD_MGMT_INIT struct.
 *     Details: in H file. (search for prototype)
*********************************************************************/
uint32
  arad_mgmt_init_set_core_clock_frequency (
    SOC_SAND_IN int unit,
    SOC_SAND_IN ARAD_MGMT_INIT* init
    ) 
{    
    uint32 /*res,*/ freq_arad_khz, intg;
    SOC_SAND_U64 frac, frac_x, frac_x2;

    SOCDNX_INIT_FUNC_DEFS;

    arad_chip_kilo_ticks_per_sec_set(
        unit,
        init->core_freq.frequency 
        );
    /* System Ref clock to set */
    if(!SOC_IS_QUX(unit))
    {
        /* Enable the fraction mode over cal. */
        SOCDNX_IF_ERR_EXIT(soc_reg_above_64_field32_modify(unit, MESH_TOPOLOGY_MESH_TOPOLOGYr, REG_PORT_ANY, 0, MULTI_FAP_3f,  0x1));

        freq_arad_khz = arad_chip_kilo_ticks_per_sec_get(unit);
        intg =  init->core_freq.system_ref_clock / freq_arad_khz;

        soc_sand_u64_clear(&frac);
        soc_sand_u64_clear(&frac_x);

        soc_sand_u64_multiply_longs(init->core_freq.system_ref_clock, ARAD_MGMT_SYST_FREQ_RES_19, &frac_x);
        soc_sand_u64_devide_u64_long(&frac_x, freq_arad_khz, &frac);

        if (freq_arad_khz <= init->core_freq.system_ref_clock) {
            soc_sand_u64_multiply_longs(intg, ARAD_MGMT_SYST_FREQ_RES_19, &frac_x2);
            soc_sand_u64_subtract_u64(&frac, &frac_x2);
        }

        /* Set the System clock frequency */
        SOCDNX_IF_ERR_EXIT(soc_reg_above_64_field32_modify(unit, MESH_TOPOLOGY_SYS_CONFIG_2r, REG_PORT_ANY, 0, SYS_CONFIG_21f,  frac.arr[0]));
        SOCDNX_IF_ERR_EXIT(soc_reg_above_64_field32_modify(unit, MESH_TOPOLOGY_SYS_CONFIG_2r, REG_PORT_ANY, 0, SYS_CONFIG_22f,  intg));
    }
exit:
    SOCDNX_FUNC_RETURN;
}

/*********************************************************************
*     Set the Soc_ARAD B0 revision specific features.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32 soc_arad_mgmt_rev_b0_set_unsafe(
    SOC_SAND_IN  int       unit)
{
    uint32
        res;
    uint32
        field,
        reg_val;
    uint64 
        reg_val64;
    soc_reg_above_64_val_t 
        reg_above_64_val;
    soc_field_t field_lcl;
    soc_reg_t cfc_sch_oob_reg;


    SOC_SAND_INIT_ERROR_DEFINITIONS(0);
    if (SOC_IS_QAX(unit)) {
		SOC_SAND_SET_ERROR_MSG((_BSL_SOCDNX_SAND_MSG("access to registers should be fixed for QAX at places we used _REG(32|64) access routines")));
    }


    /* 
     * TM Fixes
     */

    /* IPT Fixes */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 15, exit, READ_IPT_CNM_CONTORLr(unit, &reg_val));
    /* IPT-CNM CRC calculation in IPT */
    field = 0x0;
    soc_reg_field_set(unit, IPT_CNM_CONTORLr, &reg_val, CNM_CALC_FABRIC_CRC_DISf, field);
    /* IPT-Remove original packet CRC for CNM */
    field = 0x0;
    soc_reg_field_set(unit, IPT_CNM_CONTORLr, &reg_val, CNM_REMOVE_DRAM_CRC_DISf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, WRITE_IPT_CNM_CONTORLr(unit, reg_val));

    /* IPT-'Other' header format missing parity calculation and stamping */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 25, exit, READ_IPT_STAMPING_FABRIC_HEADER_ENABLEr(unit, &reg_val64));
    field = 0x0;
    soc_reg64_field32_set(unit, IPT_STAMPING_FABRIC_HEADER_ENABLEr, &reg_val64, DIS_PARITY_STAMP_WHEN_OTHERf, field);
    field = 0x0;
    soc_reg64_field32_set(unit, IPT_STAMPING_FABRIC_HEADER_ENABLEr, &reg_val64, DIS_PKT_SIZE_STAMP_WHEN_OTHERf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 30, exit, WRITE_IPT_STAMPING_FABRIC_HEADER_ENABLEr(unit, reg_val64));
  
    /* IPT-Clos WFQ scheduling bug */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 35, exit, READ_IPT_WFQ_GENERAL_CONFIGSr(unit, &reg_val));
    field = 0x0;
    soc_reg_field_set(unit, IPT_WFQ_GENERAL_CONFIGSr, &reg_val, DIS_CLOS_WFQ_MSB_MODIFICATIONf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 40, exit, WRITE_IPT_WFQ_GENERAL_CONFIGSr(unit, reg_val));

    /* IQM Enablers Setting */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 45, exit, READ_IQM_IQM_ENABLERSr(unit, REG_PORT_ANY, &reg_val));
    if(SOC_IS_ARADPLUS_AND_BELOW(unit))
    {
        /* IQM-Mask CNM generation when packet is on OCB buffer */
        field = 0x0;
        soc_reg_field_set(unit, IQM_IQM_ENABLERSr, &reg_val, FIELD_29_29f, field);

        /* IQM-Round-Robin Fix */
        field = 0x0;
        soc_reg_field_set(unit, IQM_IQM_ENABLERSr, &reg_val, FIELD_30_30f, field);

        if (SOC_IS_ARADPLUS(unit)) {
            soc_reg_field_set(unit, IQM_IQM_ENABLERSr, &reg_val, FIELD_31_31f, 1);
        }
    }
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 50, exit, WRITE_IQM_IQM_ENABLERSr(unit, REG_PORT_ANY, reg_val));

    /* IQM-Round-Robin weights */ 
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 54, exit, READ_IQM_RATE_CLASS_RD_WEIGHTr(unit, REG_PORT_ANY, &reg_val));
    field = 0x0;
    soc_reg_field_set(unit, IQM_RATE_CLASS_RD_WEIGHTr, &reg_val, RATE_CLASS_RD_WEIGHTf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 65, exit, WRITE_IQM_RATE_CLASS_RD_WEIGHTr(unit, REG_PORT_ANY, reg_val));

    /* IQM- DEQ process BDBs coherency issue */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 70, exit, READ_IQM_RESERVED_SPARE_2r(unit, REG_PORT_ANY, &reg_val));
    field = soc_reg_field_get(unit, IQM_RESERVED_SPARE_2r, reg_val, IQM_RESERVED_SPARE_2f);
    field |= SOC_SAND_SET_BITS_RANGE(0x1, 0x0, 0x0);
    soc_reg_field_set(unit, IQM_RESERVED_SPARE_2r, &reg_val, IQM_RESERVED_SPARE_2f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 75, exit, WRITE_IQM_RESERVED_SPARE_2r(unit, REG_PORT_ANY, reg_val));

    if (SOC_IS_ARDON(unit)) {
        /* enable PDM bug fixes */
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 80, exit, WRITE_IQM_REG_0259r(unit, -1));
    } else {
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 80, exit, READ_IQM_REG_AFr(unit, &reg_val));
        field = soc_reg_field_get(unit, IQM_REG_AFr, reg_val, ITEM_6_13f);
        /* PDM's Read/Write priority TAG compare is wrong */
        field |= SOC_SAND_SET_BITS_RANGE(0x1, 7, 7); /* Bit 7 in the field */
        /* PDM's read pending FIFO too shallow, should be increased */
        field |= SOC_SAND_SET_BITS_RANGE(0x1, 6, 6); /* Bit 6 in the field */
        soc_reg_field_set(unit, IQM_REG_AFr, &reg_val, ITEM_6_13f, field);
        /* PDM- Backpressure threshold too small */
        field = 0x1;
        soc_reg_field_set(unit, IQM_REG_AFr, &reg_val, ITEM_14_14f, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 85, exit, WRITE_IQM_REG_AFr(unit, reg_val));
    }

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 90, exit, READ_IPS_IPS_GENERAL_CONFIGURATIONSr_REG32(unit, REG_PORT_ANY, &reg_val));
    field = 0x0;
    /* IPS-Multiply TH will send FSM on every ENQ report once queue size is over 128K */
    soc_reg_field_set(unit, IPS_IPS_GENERAL_CONFIGURATIONSr, &reg_val, FIELD_19_19f, field);
    /* IPS-Back to Back messages may result in normal message loss */
    soc_reg_field_set(unit, IPS_IPS_GENERAL_CONFIGURATIONSr, &reg_val, FIELD_17_17f, field);
    /* IPS-FMS bin counter might be wrong if using SLOW to NORM delay */
    soc_reg_field_set(unit, IPS_IPS_GENERAL_CONFIGURATIONSr, &reg_val, FIELD_18_18f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 100, exit, WRITE_IPS_IPS_GENERAL_CONFIGURATIONSr_REG32(unit, REG_PORT_ANY, reg_val));

    /* CRPS-OVF address debug feature latch bug - No SW action */
    
    /* NIF-False parity error indication from NIF - no SW action, can use relevant interrupt */

    /* NIF (NBI) - ELK can't work on CLPORT as 10G, 20G or 40G - no SW action */
    
    /* IDR-Partial packet snoop with invalid FWD decision */    
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 105, exit, READ_IDR_SPARE_REGISTER_2r(unit, &reg_val));
    field = 0x1;
    soc_reg_field_set(unit, IDR_SPARE_REGISTER_2r, &reg_val, SYS_CONFIG_2f, field);
#ifdef BCM_88660_A0
    if (SOC_IS_ARADPLUS(unit) && !SOC_IS_ARDON(unit)) {
        /* The field name in the Arad+ register file is InvalidateMtrPtrForErrSop. */
        soc_reg_field_set(unit, IDR_SPARE_REGISTER_2r, &reg_val, SYS_CONFIG_4f, field);    
    }
#endif /* BCM_88660_A0 */
    
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 110, exit, WRITE_IDR_SPARE_REGISTER_2r(unit, reg_val));

    /* In Ardon, SYS_CONFIG_4f is placed in a different register */
    if (SOC_IS_ARDON(unit)) {
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 111, exit, READ_IDR_COMPATIBILITY_REGISTERr(unit, &reg_val));
        field = 0x1;
        soc_reg_field_set(unit, IDR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_4f, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 112, exit, WRITE_IDR_COMPATIBILITY_REGISTERr(unit, reg_val));
    }

    /* IDR-FBC ECC error indication - No SW action, can use this interrupt */
    
    /* IRE-Burst-Err-Follow-Sop-Not-Valid interrupt out of use - No SW action, can use this interrupt */

    /* IRE-TXI reset removed - No SW action, affects Chip resets */

    /* IRE-NifErrPacketSize interrupt - No SW action, can use this interrupt */

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 115, exit, READ_EGQ_GENERAL_RQP_CONFIGr(unit, REG_PORT_ANY, &reg_val));
    field = 0x1;
    /* EGQ-Reassembly with Errors */
    soc_reg_field_set(unit, EGQ_GENERAL_RQP_CONFIGr, &reg_val, SOP_IN_MOP_CHICKEN_ENf, field);
    soc_reg_field_set(unit, EGQ_GENERAL_RQP_CONFIGr, &reg_val, SOP_IN_MOP_CHICKEN_EN_1f, field);
    /* EGQ - RQP flush all fabric traffic when delete FIFO is full, 
    soc_reg_field_set(unit, EGQ_GENERAL_RQP_CONFIGr, &reg_val, FLUSH_FABRIC_ENABLEf, field); */ 
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 120, exit, WRITE_EGQ_GENERAL_RQP_CONFIGr(unit, REG_PORT_ANY, reg_val));

    /* EGQ - Q-PAIR/TCG shapers calendar is limited to 256x9.6G / 245x10G */
    /* EGQ-Total Q-Pair/TCG shapers are limited to 122X10G */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 125, exit, READ_EGQ_EGRESS_SHAPER_ENABLE_SETTINGSr(unit, REG_PORT_ANY, &reg_val));
    field = 0x0;
    soc_reg_field_set(unit, EGQ_EGRESS_SHAPER_ENABLE_SETTINGSr, &reg_val, QPAIR_SPR_RESOLUTIONf, field);
    soc_reg_field_set(unit, EGQ_EGRESS_SHAPER_ENABLE_SETTINGSr, &reg_val, TCG_SPR_RESOLUTIONf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 130, exit, WRITE_EGQ_EGRESS_SHAPER_ENABLE_SETTINGSr(unit, REG_PORT_ANY, reg_val));

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 135, exit, READ_EGQ_QPAIR_SPR_DISr(unit, REG_PORT_ANY, reg_above_64_val));
    field = 0x0;
    soc_reg_field_set(unit, EGQ_QPAIR_SPR_DISr, reg_above_64_val, QPAIR_SPR_DISf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 140, exit, WRITE_EGQ_QPAIR_SPR_DISr(unit, REG_PORT_ANY, reg_above_64_val));

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 145, exit, READ_EGQ_TCG_SPR_DISr(unit, REG_PORT_ANY, reg_above_64_val));
    field = 0x0;
    soc_reg_field_set(unit, EGQ_TCG_SPR_DISr, reg_above_64_val, TCG_SPR_DISf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 150, exit, WRITE_EGQ_TCG_SPR_DISr(unit, REG_PORT_ANY, reg_above_64_val));

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 155, exit, READ_EGQ_RESERVED_SPARE_0r(unit, REG_PORT_ANY, &reg_val));
    /* EGQ - Multicast replication machine get stuck - disable the fix on all Arads */ 
    field_lcl = SOC_IS_ARADPLUS(unit) ? ITEM_0_0f: FIELD_0_0f;
    soc_reg_field_set(unit, EGQ_REG_0090r, &reg_val, field_lcl, 1);
    /* EGQ: Wrong Cell-type (VSC128/256) signal from FDR may cause CGM counters to do Rip-Around in TDM applications */ 
    /* Fe600/PetraA have vsc128 traffic, fix cant be operate */
    field_lcl = SOC_IS_ARADPLUS(unit)? ITEM_1_1f: FIELD_1_1f;
    soc_reg_field_set(unit, EGQ_REG_0090r, &reg_val, field_lcl,
      SOC_DPP_CONFIG(unit)->arad->init.fabric.is_128_in_system ? 1 : 0);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 165, exit, WRITE_EGQ_RESERVED_SPARE_0r(unit, REG_PORT_ANY, reg_val));

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 170, exit, READ_EGQ_RESERVED_SPARE_2r(unit, REG_PORT_ANY, &reg_val));
    field = 0x1;
    /* EGQ: Arbitration issue between Multicast Replicators when getting Flow-Control from Multicast Sorter */ 
    field_lcl = SOC_IS_ARADPLUS(unit)? ITEM_0_0f: FIELD_0_0f;
    soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, field_lcl, field);
    /* EGQ: Reassembly context may get lost in dynamic allocation when SOP-IN_MOP error occurs and CGM discards the first packet */
    field_lcl = SOC_IS_ARADPLUS(unit)? ITEM_3_3f: FIELD_3_3f;
    soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, field_lcl, field);
    field = soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "8865003", 2);
    /* Set egress multicast to keep order (between packets in the same MC group) or not based on soc property */
    if (field < 2) {
        field_lcl = SOC_IS_ARADPLUS(unit)? ITEM_1_1f: FIELD_1_1f;
        soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, field_lcl, field);
        field_lcl = SOC_IS_ARADPLUS(unit)? ITEM_2_2f: FIELD_2_2f;
        soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, field_lcl, field);
    }
    if (SOC_IS_ARADPLUS(unit)) { /* In Arad+ enable its egress multicast fix allowing more replicators */
        soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, ITEM_5_5f, 1);
        soc_reg_field_set(unit, EGQ_REG_0092r, &reg_val, ITEM_7_7f, 1);
    }
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 175, exit, WRITE_EGQ_RESERVED_SPARE_2r(unit, REG_PORT_ANY, reg_val));

    /* CFC-SCH OOB CRC is calculated wrong */
    cfc_sch_oob_reg = CFC_REG_0284r;
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  180,  exit, ARAD_REG_ACCESS_ERR,soc_reg32_get(unit, cfc_sch_oob_reg, REG_PORT_ANY,  0, &reg_val));
    field = 0x7;
    soc_reg_field_set(unit, cfc_sch_oob_reg, &reg_val, FIELD_19_21f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  60,  exit, ARAD_REG_ACCESS_ERR,soc_reg32_set(unit, cfc_sch_oob_reg, REG_PORT_ANY,  0,  reg_val));

    /* DPRC - cant support different read data delay between different byte lanes */
    /* Done before Dprc out of reset in arad_mgmt_dram_init_enable_ddr_clk_dividers() */

    /* 
     * PP Fixes
     */

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 185, exit, READ_IHP_REG_0070r(unit, &reg_val));
    field = soc_reg_field_get(unit, IHP_REG_0070r, reg_val, REG_0070f);
    /* IHP Large-EM: Bandwidth guaranteed to MAC Table (MACT) Management mechanisms (Flush-Machine /Aging-Mechanism / Management) is used by Opportunistic Accesses */
    field &= ~SOC_SAND_SET_BITS_RANGE(0x1, 1, 1);
    /* IHP - Large-EM Opportunistic Learning while pipe lookups in high packet rate. */
    field &= ~SOC_SAND_SET_BITS_RANGE(0x1, 2, 2);

    soc_reg_field_set(unit, IHP_REG_0070r, &reg_val, REG_0070f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 190, exit, WRITE_IHP_REG_0070r(unit, reg_val));

    /* OAMP - BFD diag field common to all sessions - Arad-B0 only */
    if (!SOC_IS_ARADPLUS(unit)) {
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 195, exit, READ_OAMP_SPARE_REGISTER_3r(unit, &reg_val));
        field = 0x1;
        soc_reg_field_set(unit, OAMP_SPARE_REGISTER_3r, &reg_val, CHICKEN_BIT_BFD_DIAG_PROFILEf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 60, exit, WRITE_OAMP_SPARE_REGISTER_3r(unit, reg_val));
    }

    /* EPNI Access to OutLIF 0 */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 295, exit, READ_EPNI_PP_CONFIGr(unit, REG_PORT_ANY, &reg_val));
    field = 0x1;
    soc_reg_field_set(unit, EPNI_PP_CONFIGr, &reg_val, FIELD_20_20f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 300, exit, WRITE_EPNI_PP_CONFIGr(unit, REG_PORT_ANY, reg_val));

    /*PetraB FTMH and ARAD PPH mode*/
    if ((TRUE == SOC_DPP_CONFIG(unit)->arad->init.pp_enable) && (TRUE == SOC_DPP_CONFIG(unit)->tm.is_petrab_in_system) &&
        soc_property_suffix_num_get(unit,-1, spn_CUSTOM_FEATURE, "petrab_in_tm_mode", 0) )
    {
        /*PETRA_FTMH_WITH_ARAD_PPH_MODE = 1 */
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 400, exit, READ_IHB_LBP_GENERAL_CONFIG_0r(unit, REG_PORT_ANY, &reg_val64));
        field = 0x1;
        soc_reg64_field32_set(unit, IHB_LBP_GENERAL_CONFIG_0r, &reg_val64, PETRA_FTMH_WITH_ARAD_PPH_MODEf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 410, exit, WRITE_IHB_LBP_GENERAL_CONFIG_0r(unit, REG_PORT_ANY, reg_val64));
        /*EGQ_REGFILE.PetrabWithAradPpMode = 1*/
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 400, exit, READ_EGQ_PP_CONFIGr(unit, REG_PORT_ANY, &reg_val));
        field = 0x1;
        soc_reg_field_set(unit, EGQ_PP_CONFIGr, &reg_val, FIELD_13_13f, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 410, exit, WRITE_EGQ_PP_CONFIGr(unit, REG_PORT_ANY, reg_val));
        /*EPNI_REGFILE.PetrabWithAradPpMode = 1*/
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 400, exit, READ_EPNI_PP_CONFIGr(unit, REG_PORT_ANY, &reg_val));
        field = 0x1;
        soc_reg_field_set(unit, EPNI_PP_CONFIGr, &reg_val, FIELD_21_21f, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 410, exit, WRITE_EPNI_PP_CONFIGr(unit, REG_PORT_ANY, reg_val));
        /*SystemHeadersMode == 2'b01 (petrab mode)*/
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 400, exit, READ_ECI_GLOBALFr(unit, &reg_val));
        field = ARAD_PP_SYSTEM_HEADERS_MODE_PETRAB;
        soc_reg_field_set(unit, ECI_GLOBALFr, &reg_val, SYSTEM_HEADERS_MODEf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 410, exit, WRITE_ECI_GLOBALFr(unit, reg_val));
    }
exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in soc_arad_mgmt_rev_b0_set_unsafe()", 0, 0);
}

#ifdef BCM_88660_A0
/*********************************************************************
*     Set the Soc ARAD PLUS revision specific features.
*     Details: in the H file. (search for prototype)
*********************************************************************/
uint32 soc_arad_mgmt_rev_arad_plus_set_unsafe(
    SOC_SAND_IN  int       unit)
{
    uint32
        res,
        field,
        reg_val;
    uint64 
        reg_val_64,
        fld64_val;
    SOC_SAND_INIT_ERROR_DEFINITIONS(0);

    if (!SOC_IS_ARADPLUS(unit)) {
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_GEN_ERR, 5, exit);
    }

    /* 
     * TM Fixes
     */

    /* 
     * Added Features
     */

    /* MMU - Performance improvements */
    if (!SOC_IS_ARDON(unit)) 
    {
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 6, exit, READ_MMU_BANK_ACCESS_CONTROLLER_CONFIGURATIONSr(unit, &reg_val)); 
        field = 0x1;
        soc_reg_field_set(unit, MMU_BANK_ACCESS_CONTROLLER_CONFIGURATIONSr, &reg_val, BAC_SM_OPT_ENf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 7, exit, WRITE_MMU_BANK_ACCESS_CONTROLLER_CONFIGURATIONSr(unit, reg_val));
    }

    SOC_SAND_SOC_IF_ERROR_RETURN(res, 8, exit, READ_MMU_GENERAL_CONFIGURATION_REGISTERr_REG32(unit, &reg_val));
    field = 0x1;
    soc_reg_field_set(unit, MMU_GENERAL_CONFIGURATION_REGISTERr, &reg_val, WAIT_FINISH_CUR_STATE_BEFORE_STOP_FOR_REFRESHf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 9, exit, WRITE_MMU_GENERAL_CONFIGURATION_REGISTERr_REG32(unit, reg_val));

    /* QPTS - Indirect priority over CRWD events */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 10, exit, READ_IPS_CREDIT_WATCHDOG_CONFIGURATIONr(unit, REG_PORT_ANY, &reg_val_64));
    COMPILER_64_SET(fld64_val, 0x0, 0x1);
    soc_reg64_field_set(unit, IPS_CREDIT_WATCHDOG_CONFIGURATIONr, &reg_val_64, CR_WD_MASK_WHEN_PENDING_INDIRECTf, fld64_val);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 11, exit, WRITE_IPS_CREDIT_WATCHDOG_CONFIGURATIONr(unit, REG_PORT_ANY, reg_val_64));

    /* 
     * Bug Fixes
     */

    /* ECI - CMICm BISR bypass */
    /* Should not be enabled by default. */
    
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 15, exit, READ_IRR_COMPATIBILITY_REGISTERr(unit, &reg_val));
    field = 0x1;
    /* IRDP - Drop snoop/mirror copies when UC FIFO full    */
    soc_reg_field_set(unit, IRR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_1f, field);
    /* IRDP - Enable 256 Lag members    */
    soc_reg_field_set(unit, IRR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_3f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 20, exit, WRITE_IRR_COMPATIBILITY_REGISTERr(unit, reg_val));
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 25, exit, READ_IDR_COMPATIBILITY_REGISTERr(unit, &reg_val));
    field = 0x1;
    /* IRDP - Header delta is not transferred to the IDR Ethernet meters    */
    soc_reg_field_set(unit, IDR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_3f, field);
    soc_reg_field_set(unit, IDR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_4f, field);
    /* IRDP - Header compensation is not transferred to the Meter processor    */
    soc_reg_field_set(unit, IDR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_2f, field);
    /* IRDP - Ethernet policer refresh    */
    soc_reg_field_set(unit, IDR_COMPATIBILITY_REGISTERr, &reg_val, SYS_CONFIG_5f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 30, exit, WRITE_IDR_COMPATIBILITY_REGISTERr(unit, reg_val));

    /* MMU - Can't read/write indirect packet from/to dram if last packet read/write directly was OCB    */
    /* No SW action */

    /* QPTS - Turning statistics scrubber machine off can cause IQM DEQ pipe to stuck    */ 
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 35, exit, READ_IQM_STATISTICS_REPORT_CONFIGURATIONS_4r(unit, REG_PORT_ANY, &reg_val));
    field = 0x1;
    soc_reg_field_set(unit, IQM_STATISTICS_REPORT_CONFIGURATIONS_4r, &reg_val, SOC_IS_ARDON(unit) ? ST_SCR_MIN_PRD_FIX_ENf : FIELD_28_28f, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 40, exit, WRITE_IQM_STATISTICS_REPORT_CONFIGURATIONS_4r(unit, REG_PORT_ANY, reg_val));

    /* QPTS - CNM: IPT parser takes VLAN Edit command from the wrong place when FHEI extension size is 5 bytes    */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 55, exit, READ_IPT_IPT_ENABLESr(unit, &reg_val));
    field = 0x1;
    soc_reg_field_set(unit, IPT_IPT_ENABLESr, &reg_val, CNM_FHEI_5_BYTES_FIX_ENf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 60, exit, WRITE_IPT_IPT_ENABLESr(unit, reg_val));

    /* EGQ - Fixing MC performance issue    */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 75, exit, READ_EGQ_TDM_GENERAL_CONFIGURATIONr(unit, REG_PORT_ANY, &reg_val));
    if ((SOC_IS_DPP_TDM_STANDARD(unit)) || (SOC_IS_DPP_TDM_OPTIMIZE(unit))) {
        field = 0x1;
    } else {
        field = 0x0;
    }
    soc_reg_field_set(unit, EGQ_TDM_GENERAL_CONFIGURATIONr, &reg_val, TDM_BITMAP_UC_PERF_ENf, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 80, exit, WRITE_EGQ_TDM_GENERAL_CONFIGURATIONr(unit, REG_PORT_ANY, reg_val));

    if (!SOC_IS_ARDON(unit)) {
        /* ECI - CMICm BISR bypass: SW BISR cannot handle CIMCm memories, and therefore they are bypassed */
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 85, exit, READ_ECI_GENERAL_CONFIGURATION_2r(unit, &reg_val));
        field = 0x1;
        soc_reg_field_set(unit, ECI_GENERAL_CONFIGURATION_2r, &reg_val, CMICM_BISR_BYPASS_ENABLEf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 90, exit, WRITE_ECI_GENERAL_CONFIGURATION_2r(unit, reg_val));
    }

    /* FDRC - unreachable credit destination: FDT send flow status to the wrong queue when receiving credit from the SCH destined to unreachable destination */
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 85, exit, READ_FCR_REG_0093r(unit, &reg_val));
    field = SOC_DPP_CONFIG(unit)->arad->init.fabric.is_fe600 ? 0x0 : 0x1;  /* This fix is relevant only for VSC256 mode*/
    soc_reg_field_set(unit, FCR_REG_0093r, &reg_val, REG_0093f, field);
    SOC_SAND_SOC_IF_ERROR_RETURN(res, 90, exit, WRITE_FCR_REG_0093r(unit, reg_val));

    if (!SOC_IS_ARDON(unit)) {
        /* 
         * PP fixes
         */
        /* Default MACT limit per FID  */ 
        COMPILER_64_SET(fld64_val, 0, 0x1);
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  84,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field64_modify(unit, PPDB_B_LARGE_EM_COUNTER_DB_CNTR_PTR_CONFIGURATIONr, REG_PORT_ANY, 0, LARGE_EM_CFG_LIMIT_MODE_FIDf,  fld64_val));


        SOC_SAND_SOC_IF_ERROR_RETURN(res, 86, exit, READ_EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_2r(unit, REG_PORT_ANY, &reg_val));

        if ((SOC_DPP_CONFIG(unit)->pp.next_hop_mac_extension_enable)) 
        {
            /* for compatibilty with ARAD, Disable hardware computation of Host-Index for DA. 
             * Instead, do it the Arad way: PMF will add pph learn extension (system header ), egress program editor will stamp the DA  
             * if this soc property is disabled, then use hardware computation using the chicken bit */ 
            if (SOC_IS_NEXT_HOP_MAC_EXT_ARAD_COMPATIBLE(unit)) {
                field = 0x0;
            }
            else {
                /* enable computation of Host-Index for DA-Calc  */ 
                field = 0x1;
            }
            soc_reg_field_set(unit, EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_2r, &reg_val, CFG_USE_HOST_INDEX_FOR_DA_CALC_ENABLEf, field);
        }
    
        /* Enable MPLS Pipe fix */ 
        field = 0x1;
        soc_reg_field_set(unit, EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_2r, &reg_val, CFG_MPLS_PIPE_FIX_ENABLEf, field);

        SOC_SAND_SOC_IF_ERROR_RETURN(res, 88, exit, WRITE_EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_2r(unit, REG_PORT_ANY, reg_val));

        /* Prevent bounce back filtering for UC packets.
           Fixes Plus-EBF1 */
        if (soc_property_suffix_num_get(unit,-1, spn_CUSTOM_FEATURE, "refilter_uc_pkt_en", 0)) {
            field = 0x1;
        } else {
            field = 0x0;
        }
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  90,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EGQ_CFG_BUG_FIX_CHICKEN_BITS_REG_1r, SOC_CORE_ALL, 0, CFG_BUG_FIX_98_DISABLEf,  field));

        /* Disable ETPP filtering for the Snoop/Mirroring forwarding code (code 4'hB).
           Fixes Plus-EBF5 */
        field = 0xF7FF;
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  100,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EPNI_CFG_ENABLE_FILTERING_PER_FWD_CODEr, SOC_CORE_ALL, 0, CFG_ENABLE_FILTERING_PER_FWD_CODEf,  field));

        /*VxLAN: native support */
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  12,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EPNI_CFG_DC_OVERLAYr, SOC_CORE_ALL, 0, CFG_EN_VXLAN_ENCAPSULATIONf,  0x1));

        /* Enable Routing Over Overlay and ERSPAN */
        if (soc_property_get(unit, spn_BCM886XX_ROO_ENABLE, 0) || soc_property_get(unit, spn_BCM886XX_ERSPAN_TUNNEL_ENABLE, 0)) {
            field = 0x0;
        }
        else {
            /* Disable DC Overlay to enable EEDB Outlif + EEI of type outlif Model 3 support (see encap optimized VPLS documentation for details) */
            field = 0x1;
        }
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  110,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_1r, SOC_CORE_ALL, 0, CFG_PLUS_F_5_DISABLEf,  field));

        /* Enable passing inlif profile in FHEI. */
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  120,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IHB_LBP_GENERAL_CONFIG_0r, SOC_CORE_ALL, 0, ENABLE_FHEI_WITH_IN_LIF_PROFILEf,  1));

        /* If set, and forwarding header is mpls while Terminated-TTL is not Valid, cos marking will be updated according to mpls header. */
        SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  130,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, IHP_VTT_GENERAL_CONFIGS_1r, SOC_CORE_ALL, 0, ENABLE_COS_MARKING_UPGRADESf,  1));

        /* Enable PON 3tag fix  */
        field = 0x0;

        SOC_SAND_SOC_IF_ERROR_RETURN(res, 140, exit, READ_EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_1r(unit, REG_PORT_ANY, &reg_val));
        soc_reg_field_set(unit, EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_1r, &reg_val, CFG_NEW_TRILL_FORMAT_DISABLEf, field);
        soc_reg_field_set(unit, EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_1r, &reg_val, CFG_PLUS_F_24_DISABLEf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 150, exit, WRITE_EPNI_CFG_BUG_FIX_CHICKEN_BITS_REG_1r(unit, REG_PORT_ANY, reg_val));
    
        /* IGMP feature: enable compatible-mc after tunnel-termination and upgrade second stage parsing for IGMP */
         field = 0x0;
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 170, exit, READ_IHP_VTT_GENERAL_CONFIGS_1r_REG32(unit, REG_PORT_ANY, &reg_val));
        soc_reg_field_set(unit, IHP_VTT_GENERAL_CONFIGS_1r, &reg_val, DISABLE_INNER_COMPATIBLE_MCf, field);
        field= 0x1;
        soc_reg_field_set(unit, IHP_VTT_GENERAL_CONFIGS_1r, &reg_val, ENABLE_SECOND_STAGE_PARSING_UPGRADESf, field);
        SOC_SAND_SOC_IF_ERROR_RETURN(res, 180, exit, WRITE_IHP_VTT_GENERAL_CONFIGS_1r_REG32(unit, REG_PORT_ANY, reg_val));

    }

    /* EGQ is mishandling packets that being terminate to size of 192-255B */
    SOC_SAND_SOC_IF_ERROR_RETURN_ERR_VAL(res,  160,  exit, ARAD_REG_ACCESS_ERR,soc_reg_above_64_field32_modify(unit, EPNI_GLOBAL_CONFIGr, SOC_CORE_ALL, 0, ALIGNER_STRIPPING_64B_FIX_ENf,  0x1));


exit:
    SOC_SAND_EXIT_AND_SEND_ERROR("error in soc_arad_mgmt_rev_arad_plus_set_unsafe()", 0, 0);
}

#endif /* BCM_88660_A0 */

int arad_mgmt_nof_block_instances(int unit, soc_block_types_t block_types, int *nof_block_instances) 
{
    int block_index;
    uint32 nof_instances = 0;
    SOCDNX_INIT_FUNC_DEFS;

    SOCDNX_NULL_CHECK(nof_block_instances);
    SOCDNX_NULL_CHECK(block_types);

    SOC_BLOCK_ITER_ALL(unit, block_index, *block_types)
    {
        ++nof_instances;
    }

    *nof_block_instances = nof_instances;

exit:
    SOCDNX_FUNC_RETURN;
}

/*********************************************************************
*     PVTMON
*********************************************************************/

/*
 * Note: Ardon procedure is based on soc_fe3200_drv_pvt_monitor_enable() (and Jericho). Apparently, they both use the same silicon piece.
 */
int ardon_mgmt_drv_pvt_monitor_enable(int unit)
{
    uint64 reg64_val;
    soc_reg_t pvt_monitors[] = {ECI_PVT_MON_A_CONTROL_REGr};
    int pvt_index;

    SOCDNX_INIT_FUNC_DEFS;

    /* Init */
    COMPILER_64_ZERO(reg64_val);
    for (pvt_index = 0; pvt_index < (sizeof(pvt_monitors) / sizeof(soc_reg_t)); pvt_index++) {
        SOCDNX_IF_ERR_EXIT(soc_reg_set(unit, pvt_monitors[pvt_index], REG_PORT_ANY, 0, reg64_val));
    }

    /* Powerdown */
    COMPILER_64_BITSET(reg64_val, _SOC_ARDON_ECI_PVT_MON_CONTROL_REG_POWERDOWN_BIT);
    for (pvt_index = 0; pvt_index < (sizeof(pvt_monitors) / sizeof(soc_reg_t)); pvt_index++) {
        SOCDNX_IF_ERR_EXIT(soc_reg_set(unit, pvt_monitors[pvt_index], REG_PORT_ANY, 0, reg64_val));
    }

    /* Powerup */
    COMPILER_64_ZERO(reg64_val);
    for (pvt_index = 0; pvt_index < (sizeof(pvt_monitors) / sizeof(soc_reg_t)); pvt_index++) {
        SOCDNX_IF_ERR_EXIT(soc_reg_set(unit, pvt_monitors[pvt_index], REG_PORT_ANY, 0, reg64_val));
    }

    /* Reset */
    COMPILER_64_BITSET(reg64_val, _SOC_ARDON_ECI_PVT_MON_CONTROL_REG_RESET_BIT);
    for (pvt_index = 0; pvt_index < (sizeof(pvt_monitors) / sizeof(soc_reg_t)); pvt_index++) {
        SOCDNX_IF_ERR_EXIT(soc_reg_set(unit, pvt_monitors[pvt_index], REG_PORT_ANY, 0, reg64_val));
    }

exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Note: This procedure is based on soc_fe3200_drv_temperature_monitor_get(). Apparently, they both use the same silicon piece.
 */
int ardon_mgmt_temp_pvt_get(int unit, int temperature_max, soc_switch_temperature_monitor_t *temperature_array, int *temperature_count)
{
    int ii;
    uint32 reg32_val;
    int peak, curr;
    soc_reg_t temp_reg[] = {ECI_PVT_MON_A_THERMAL_DATAr};
    soc_field_t curr_field[] = {THERMAL_DATAf};
    soc_field_t peak_field[] = {PEAK_THERMAL_DATAf};

    SOCDNX_INIT_FUNC_DEFS;

    if (temperature_max < _SOC_ARDON_PVT_MON_NOF) {
        SOCDNX_EXIT_WITH_ERR(SOC_E_FULL, (_BSL_SOCDNX_MSG("Array size should be equal to or larger than %d.\n"), _SOC_ARDON_PVT_MON_NOF));
    }

    for (ii = 0; ii < _SOC_ARDON_PVT_MON_NOF; ii++) {
        SOCDNX_IF_ERR_EXIT(soc_reg32_get(unit, temp_reg[ii], REG_PORT_ANY, 0, &reg32_val));

        curr = soc_reg_field_get(unit, temp_reg[ii], reg32_val, curr_field[ii]);
        /* curr [0.1 C] = 4101.6 - curr * 4.8517 */
        temperature_array[ii].curr =  (_SOC_ARDON_PVT_BASE - curr * _SOC_ARDON_PVT_FACTOR) / 10000;

        peak = soc_reg_field_get(unit, temp_reg[ii], reg32_val, peak_field[ii]);
        /* peak [0.1 C] = 4101.6 - peak * 4.8517 */
        temperature_array[ii].peak = (_SOC_ARDON_PVT_BASE - peak * _SOC_ARDON_PVT_FACTOR) / 10000;
    }

    *temperature_count = _SOC_ARDON_PVT_MON_NOF;

exit:
    SOCDNX_FUNC_RETURN; 
}

/*
 * Note: This procedure is based on soc_fe1600_drv_temperature_monitor_get(). Apparently, they both use the same silicon piece.
 */
int arad_mgmt_temp_pvt_get(int unit, int temperature_max, soc_switch_temperature_monitor_t *temperature_array, int *temperature_count) 
{
    int i, rv;
    uint32 reg32_val;
    int curr;
    soc_reg_t temp_reg[] = {ECI_PVT_MON_A_THERMAL_DATAr, ECI_PVT_MON_B_THERMAL_DATAr};

    SOCDNX_INIT_FUNC_DEFS;

    if (SOC_IS_ARDON(unit)) {
        SOCDNX_IF_ERR_EXIT(ardon_mgmt_temp_pvt_get(unit, temperature_max, temperature_array, temperature_count));
    } else {
        if (temperature_max < _SOC_ARAD_PVT_MON_NOF)
        {
            SOCDNX_EXIT_WITH_ERR(SOC_E_FULL, (_BSL_SOCDNX_MSG("Array size should be equal to or larger than %d.\n"), _SOC_ARAD_PVT_MON_NOF));
        }

        for (i = 0; i < _SOC_ARAD_PVT_MON_NOF; i++)
        {
            rv = soc_reg32_get(unit, temp_reg[i], REG_PORT_ANY, 0, &reg32_val) ;
            SOCDNX_IF_ERR_EXIT(rv);

            curr = soc_reg_field_get(unit, temp_reg[i], reg32_val, THERMAL_DATAf);
            /* curr [0.1 C] = 428.39 - curr * 5.66 */
            temperature_array[i].curr =  (_SOC_ARAD_PVT_BASE - curr * _SOC_ARAD_PVT_FACTOR) / 1000;
            temperature_array[i].peak = -1;
        }

        *temperature_count = _SOC_ARAD_PVT_MON_NOF;
    }
        
exit:
    SOCDNX_FUNC_RETURN;
}


/*********************************************************************
*     Get the AVS - Adjustable Voltage Scaling value of the Arad
*********************************************************************/
int arad_mgmt_avs_value_get(
            int       unit,
            uint32*      avs_val)
{
    uint32
        reg_val;

    SOCDNX_INIT_FUNC_DEFS;

    SOCDNX_NULL_CHECK(avs_val);

    *avs_val = 0;

    if (SOC_IS_ARADPLUS(unit)) {
        SOCDNX_IF_ERR_EXIT(soc_reg32_get(unit, ECI_AVS_STATUSr, REG_PORT_ANY, 0, &reg_val));
        *avs_val = soc_reg_field_get(unit, ECI_AVS_STATUSr, reg_val, AVS_STATUS_VALUEf);
    } else {
        SOCDNX_EXIT_WITH_ERR(SOC_E_UNAVAIL, (_BSL_SOCDNX_MSG("Device not supported.")));
    }

exit:
    SOCDNX_FUNC_RETURN;
}

uint32
soc_arad_cache_table_update_all(int unit)
{
    return soc_dcmn_cache_table_update_all(unit);
}

#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif /* of #if defined(BCM_88650_A0) */
