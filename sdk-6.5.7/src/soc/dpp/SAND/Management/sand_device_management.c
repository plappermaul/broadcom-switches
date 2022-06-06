/* $Id: sand_device_management.c,v 1.9 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $
*/


#include <soc/dpp/SAND/Management/sand_device_management.h>
#include <soc/dpp/SAND/Management/sand_general_params.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>
#include <soc/dpp/SAND/Management/sand_chip_descriptors.h>
#include <soc/dpp/SAND/Management/sand_callback_handles.h>
#include <soc/dpp/SAND/Management/sand_low_level.h>

#include <soc/dpp/SAND/SAND_FM/sand_chip_defines.h>
#include <soc/dpp/SAND/SAND_FM/sand_mem_access.h>

#include <soc/dpp/SAND/Utils/sand_os_interface.h>

uint8 Soc_register_with_id = FALSE;

/* $Id: sand_device_management.c,v 1.9 Broadcom SDK $
 * Forward declaration
 */
STATIC SOC_SAND_RET
  soc_sand_device_mngmnt_load_chip_ver(
    SOC_SAND_IN int       unit,
    SOC_SAND_IN uint32*     base_address,
    SOC_SAND_IN SOC_SAND_DEV_VER_INFO* ver_info
  );



void
  soc_sand_clear_SAND_DEV_VER_INFO(
    SOC_SAND_OUT SOC_SAND_DEV_VER_INFO* ver_info
  )
{
  if(ver_info == NULL)
  {
    goto exit;
  }

  soc_sand_os_memset(ver_info, 0x0, sizeof(SOC_SAND_DEV_VER_INFO));

  /*
   * Just set it to invalid offset.
   */
  ver_info->ver_reg_offset = 0xFFFFFFFF;

exit:
  return;
}


/*
 *
 */
SOC_SAND_RET
  soc_sand_device_register_with_id(SOC_SAND_IN uint8 enable)
{
  Soc_register_with_id = enable;

  return SOC_SAND_OK;
}

SOC_SAND_RET
  soc_sand_device_register(
    uint32                 *base_address,
    uint32                 mem_size, /*in bytes*/
    SOC_SAND_UNMASK_FUNC_PTR               unmask_func_ptr,
    SOC_SAND_IS_BIT_AUTO_CLEAR_FUNC_PTR    is_bit_ac_func_ptr,
    SOC_SAND_IS_DEVICE_INTERRUPTS_MASKED   is_dev_int_mask_func_ptr,
    SOC_SAND_GET_DEVICE_INTERRUPTS_MASK    get_dev_mask_func_ptr,
    SOC_SAND_MASK_SPECIFIC_INTERRUPT_CAUSE mask_specific_interrupt_cause_ptr,
    SOC_SAND_RESET_DEVICE_FUNC_PTR    reset_device_ptr,
    SOC_SAND_IS_OFFSET_READ_OR_WRITE_PROTECT_FUNC_PTR is_read_write_protect_ptr,
    SOC_SAND_DEV_VER_INFO             *ver_info,
    SOC_SAND_INDIRECT_MODULE          *indirect_module,
    uint32                 interrupt_mask_address,/* If the general mask bit do not exist ,
                                                          * give 0xFFFFFFFF (invalid address) as the
                                                          * address (as in SOC_SAND_FAP10M)
                                                          */
    int                  *unit_ptr
  )
{
  SOC_SAND_RET
    ex;
  int
    err,
    result,
    handle;
  uint32
    version_reg;

  ex     = SOC_SAND_OK;
  result = SOC_SAND_OK;
  err = 0;
  version_reg = 0;

  /*
   */
  if (!soc_sand_general_get_driver_is_started())
  {
    ex = SOC_SAND_DRIVER_NOT_STARTED;
    err = 1;
    goto exit;
  }
  /*
   */
#ifdef SOC_SAND_CHECK_BASE_ADDRESS
  if (!base_address || !soc_sand_is_long_aligned((uint32)base_address))
  {
    /*
     * base_address was null or not long aligned
     */
    ex = SOC_SAND_REGISTER_DEVICE_004;
    err = 2;
    goto exit;
  }
#endif
  if (!unit_ptr)
  {
    /*
     * user supplied buffer is NULL
     */
    ex = SOC_SAND_REGISTER_DEVICE_NULL_BUFFER;
    err = 3;
    goto exit;
  }

  if (SOC_SAND_OK != soc_sand_array_mutex_take())
  {
    err = 50;
    goto exit;
  }

  /*
   */
  if (Soc_register_with_id)
  {
    handle = *unit_ptr;
  }
  else
  {
    handle = -1;
  }

  result = soc_sand_add_chip_descriptor (
              base_address,
              mem_size,
              unmask_func_ptr,
              is_bit_ac_func_ptr,
              is_dev_int_mask_func_ptr,
              get_dev_mask_func_ptr,
              mask_specific_interrupt_cause_ptr,
              reset_device_ptr,
              is_read_write_protect_ptr,
              ver_info->chip_type,
              ver_info->logic_chip_type,
              handle
           );
  /*
   */
  if (0 > result)
  {
    switch (result)
    {
      case -1:  /* descriptors array / mutex are not initialized */
      {
        ex = SOC_SAND_DESC_ARRAY_NOT_INIT;
        err = 4;
        break;
      }
      case -2:  /* failed to take the array semaphore */
      {
        ex = SOC_SAND_SEM_TAKE_FAIL;
        err = 5;
        break;
      }
      case -3:  /* reached max size */
      {
        ex = SOC_SAND_REGISTER_DEVICE_001;
        err = 6;
        break;
      }
      case -6:  /* failed to create the device semaphore */
      {
        ex = SOC_SAND_SEM_CREATE_FAIL;
        err = 8;
        break;
      }
      default:
      {
        /* unknown error */
        ex = SOC_SAND_ADD_CHIP_DESCRIPTOR_ERR;
        err = 9;
        break;
      }
    }
    goto exit_semaphore;
  }

  /* Do not perform the following for CMIC-based devices */
  if (ver_info->cmic_skip_verif == FALSE) {
	  /*
	   * OK, let's check that the device is alive.
	   * No need to take it's mutex' cause no one can access
	   * that mutex before the device descriptor is valid
	   */
	  ex = soc_sand_physical_read_from_chip(
			 &version_reg, base_address, ver_info->ver_reg_offset, SOC_SAND_REG_SIZE_BYTES
		   );
	  if (ex != SOC_SAND_OK)
	  {
		/* device was not accessable */
		err = 10;
		goto exit_semaphore;
	  }
	
	  ex = soc_sand_check_chip_type_decide_big_endian(
			 result,
			 version_reg,
			 ver_info->chip_type_shift,
			 ver_info->chip_type_mask
		   );
	  if(ex != SOC_SAND_OK)
	  {
		/* Chip type as reported by chip itself does not match registered chip type. */
		ex = SOC_SAND_REGISTER_DEVICE_005;
		err = 11;
		goto exit_semaphore;
	  }
  }

  ex = soc_sand_device_mngmnt_load_chip_ver(result, base_address, ver_info);
  if(ex != SOC_SAND_OK)
  {
	err = 20;
	goto exit_semaphore;
  }


  /* Set indirect information */
  if (soc_sand_indirect_set_info(result,indirect_module))
  {
	ex = SOC_SAND_ERR;
	err = 30;
	goto exit_semaphore;
  }
  /*
   * Set interrupt mask address information.
   */
  if (soc_sand_mem_interrupt_mask_address_set(result,interrupt_mask_address))
  {
	ex = SOC_SAND_ERR;
	err = 40;
	goto exit_semaphore;
  }
  /*
   */
  soc_sand_set_chip_descriptor_valid(result);
  /*
   */
  *unit_ptr = result;
  /*
   */
exit_semaphore:

  if (SOC_SAND_OK != soc_sand_array_mutex_give())
  {
	err = 52;
	goto exit;
  }
exit:

  SOC_SAND_ERROR_REPORT(ex,NULL,0,0,SOC_SAND_DEVICE_REGISTER,
        "General error in soc_sand_device_register()",err,0,0,0,0,0) ;
  return ex;
}
/*
 */
SOC_SAND_RET
  soc_sand_device_unregister(
    int unit
  )
{
  SOC_SAND_RET ex;
  int res;
  uint32 err;
  ex = SOC_SAND_OK;
  err = 0;
  /*
   */
  soc_sand_check_driver_and_device(unit,&err);
  if (err)
  {
    ex = (SOC_SAND_RET)err;
    err = 1;
    goto exit;
  }
  /*
   * Take mutexes in the right order -> delta_list, device, rest of them
   */
  if (SOC_SAND_OK != soc_sand_tcm_callback_delta_list_take_mutex())
  {
    ex = SOC_SAND_SEM_TAKE_FAIL;
    err = 2;
    goto exit;
  }
  res = soc_sand_take_chip_descriptor_mutex(unit);
  if( SOC_SAND_OK != res)
  {
    if (SOC_SAND_ERR == res)
    {
      soc_sand_tcm_callback_delta_list_give_mutex();
      ex = SOC_SAND_SEM_TAKE_FAIL;
      err = 3;
      goto exit;
    }
    if ( 0 > res )
    {
      soc_sand_tcm_callback_delta_list_give_mutex();
      ex = SOC_SAND_ILLEGAL_DEVICE_ID;
      err = 4;
      goto exit;
    }
  }
  /*
   * semaphore taken successfully
   */
  if (soc_sand_handles_unregister_all_device_handles(unit))
  {
    /* There is no need to chek the returned value of soc_sand_give_chip_descriptor_mutex, we are in failure*/
    /* coverity[unchecked_value : FALSE]*/
    soc_sand_give_chip_descriptor_mutex(unit);
    soc_sand_tcm_callback_delta_list_give_mutex();
    ex = SOC_SAND_UNREGISTER_DEVICE_001;
    err = 5;
    goto exit;
  }
  /*
   * Clear device indirect information.
   */
  soc_sand_indirect_clear_info(unit);
  /*
   * Clear device interrupt mask address information.
   */
  soc_sand_mem_interrupt_mask_address_clear(unit);
  /*
   * clears all registered interrupt callbacks (already cleared by the unregister handles)
   * deletes the mutex (all waiting tasks get error return value).
   * that's why we're giving back the mutex only in case of an error.
   */
  if (soc_sand_remove_chip_descriptor(unit))
  {
    ex = SOC_SAND_UNREGISTER_DEVICE_002;
    if (SOC_SAND_OK != soc_sand_give_chip_descriptor_mutex(unit))
    {
      ex = SOC_SAND_SEM_GIVE_FAIL;
      err = 6;
      goto exit;
    }
    err = 7;
    goto exit;
  }
  /*
   * Give list mutex back
   */
  if (SOC_SAND_OK != soc_sand_tcm_callback_delta_list_give_mutex())
  {
    ex = SOC_SAND_SEM_GIVE_FAIL;
    err = 8;
    goto exit;
  }
exit:
  SOC_SAND_ERROR_REPORT(ex,NULL,0,0,SOC_SAND_DEVICE_REGISTER,
        "General error in soc_sand_device_unregister()",err,0,0,0,0,0);
  return ex;
}


/*****************************************************
*NAME
* soc_sand_device_mngmnt_load_chip_ver
*TYPE:
*  PROC
*DATE:
*  18-Nov-04
*FUNCTION:
*  1. Read device version register.
*  2. Set it in the driver chip descriptor.
*INPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_IN int       unit -
*      Driver device ID.
*    SOC_SAND_IN uint32*     base_address -
*      Device base address.
*    SOC_SAND_IN SOC_SAND_DEV_VER_INFO* ver_info -
*      Pointer to the device's version information structure.
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_RET.
*  SOC_SAND_INDIRECT:
*    None
*REMARKS:
*  None.
*SEE ALSO:
*****************************************************/
STATIC SOC_SAND_RET
  soc_sand_device_mngmnt_load_chip_ver(
    SOC_SAND_IN int       unit,
    SOC_SAND_IN uint32*     base_address,
    SOC_SAND_IN SOC_SAND_DEV_VER_INFO* ver_info
  )
{
  SOC_SAND_RET
    soc_sand_ret;
  uint32
    err,
    version_reg,
    dbg_ver,
    chip_ver;

  err = 0;
  version_reg = 0;
  soc_sand_ret = SOC_SAND_OK;

  /*
   * 1. Read device version register
   */
  soc_sand_ret = soc_sand_physical_read_from_chip(
         &version_reg, base_address, ver_info->ver_reg_offset, sizeof(uint32)
       );
  if (soc_sand_ret != SOC_SAND_OK)
  {
    /* device was not accessable */
    err = 10;
    goto exit;
  }

  dbg_ver  = SOC_SAND_GET_FLD_FROM_PLACE(version_reg, ver_info->dbg_ver_shift, ver_info->dbg_ver_mask);
  chip_ver = SOC_SAND_GET_FLD_FROM_PLACE(version_reg, ver_info->chip_ver_shift, ver_info->chip_ver_mask);


  /*
   * 2. Set it in the driver chip descriptor.
   */
  soc_sand_ret = soc_sand_set_chip_descriptor_ver_info(unit, dbg_ver, chip_ver);
  if (soc_sand_ret != SOC_SAND_OK)
  {
    err = 20;
    goto exit;
  }


exit:
  SOC_SAND_ERROR_REPORT(soc_sand_ret,NULL,0,0,SOC_SAND_DEVICE_MNGMNT_LOAD_CHIP_VER,
        "General error in soc_sand_device_mngmnt_load_chip_ver()",err,unit,0,0,0,0);
  return soc_sand_ret;
}


/*****************************************************
*NAME
* soc_sand_get_device_type
*TYPE:
*  PROC
*DATE:
*  18-Nov-04
*FUNCTION:
*  Return the chip definitions:
*   Type and version.
*INPUT:
*  SOC_SAND_DIRECT:
*    uint32       unit -
*      Driver device ID.
*    SOC_SAND_DEVICE_TYPE   *chip_type -
*      SOC_SAND_FAP10M, SOC_SAND_FE200 ....
*    uint32      *chip_ver -
*      The chip revision. Each version has different
*      functionality.
*    uint32      *dbg_ver
*      In case of small changes in the device are
*      made between the revisions.
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    SOC_SAND_RET.
*  SOC_SAND_INDIRECT:
*    None
*REMARKS:
*  None.
*SEE ALSO:
*****************************************************/

SOC_SAND_RET
  soc_sand_get_device_type(
    int      unit,
    SOC_SAND_DEVICE_TYPE *chip_type,
    uint32    *chip_ver,
    uint32    *dbg_ver
  )
{
  SOC_SAND_RET
    soc_sand_ret=SOC_SAND_OK;
  uint32
    err = 0;

  if(!chip_type || !chip_ver || !dbg_ver)
  {
    soc_sand_ret = SOC_SAND_NULL_POINTER_ERR;
    err = 10;
    goto exit;
  }

  *chip_ver = soc_sand_get_chip_descriptor_chip_ver(unit);
  *dbg_ver = soc_sand_get_chip_descriptor_dbg_ver(unit);
  *chip_type = soc_sand_get_chip_descriptor_logic_chip_type(unit);


exit:
  SOC_SAND_ERROR_REPORT(soc_sand_ret,NULL,0,0,SOC_SAND_DEVICE_MNGMNT_GET_DEVICE_TYPE,
        "General error in soc_sand_get_device_type()",err,unit,0,0,0,0);
  return soc_sand_ret;
}
#ifdef SOC_SAND_DEBUG


/*
 * Printing utility.
 * Convert from enumerator to string.
 */
const char*
  soc_sand_DEVICE_TYPE_to_str(
    SOC_SAND_IN SOC_SAND_DEVICE_TYPE dev_type
  )
{
  const char
    *str;

  switch(dev_type)
  {
  case SOC_SAND_DEV_FE200:
    str = "SOC_SAND_FE200";
    break;
  case SOC_SAND_DEV_TIMNA:
    str = "TIMNA";
    break;
  case SOC_SAND_DEV_FE600:
    str = "SOC_SAND_FE600";
    break;
  case SOC_SAND_DEV_FAP10M:
    str = "SOC_SAND_FAP10M";
    break;

  case SOC_SAND_DEV_FAP20V:
    str = "SOC_SAND_FAP20V";
    break;
  case SOC_SAND_DEV_FAP21V:
    str = "SOC_SAND_FAP21V";
    break;
  case SOC_SAND_DEV_PETRA:
    str = "SOC_PETRA";
    break;
  case SOC_SAND_DEV_ARAD:
    str = "ARAD";
    break;
  case SOC_SAND_DEV_FAP20M:
    str = "SOC_SAND_FAP20M";
    break;
  case SOC_SAND_DEV_T20E:
    str = "T20E";
    break;


  default:
    str = "soc_sand_DEVICE_TYPE_to_str input parameters error (dev_type)";
  }

  return str;
}

#endif





