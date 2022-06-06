/***********************************************************************************
 ***********************************************************************************
 *  File Name     :  falcon16_tsc_access.c                                           *
 *  Created On    :  03 Nov 2015                                                   *
 *  Created By    :  Brent Roberts                                                 *
 *  Description   :  APIs for Serdes IPs                                           *
 *  Revision      :    *
 *                                                                                 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                           *
 *  No portions of this material may be reproduced in any form without             *
 *  the written permission of:                                                     *
 *      Broadcom Corporation                                                       *
 *      5300 California Avenue                                                     *
 *      Irvine, CA  92617                                                          *
 *                                                                                 *
 *  All information contained in this document is Broadcom Corporation             *
 *  company private proprietary, and trade secret.                                 *
 *                                                                                 *
 ***********************************************************************************
 ***********************************************************************************/

/** @file falcon16_tsc_access.c
 * Implementation of API uC access functions
 */

#include "../include/falcon16_tsc_access.h"
#include "../include/falcon16_tsc_common.h"
#include "../include/falcon16_tsc_functions.h"
#include "../include/falcon16_tsc_internal.h"
#include "../include/falcon16_tsc_internal_error.h"
#include "../include/falcon16_tsc_select_defns.h"

/******************************************************/
/*  Commands through Serdes FW DSC Command Interface  */
/******************************************************/

err_code_t falcon16_tsc_pmd_uc_cmd_return_immediate(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info) {
  err_code_t err_code;
  uint16_t   cmddata;

  err_code = falcon16_tsc_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(sa__, 1, cmd); /* Poll for uc_dsc_ready_for_cmd = 1 to indicate falcon16_tsc ready for command */
  if (err_code) {
    EFUN_PRINTF(("ERROR : DSC ready for command timed out (before cmd) cmd = %d, supp_info = x%02x err=%d !\n", cmd, supp_info, err_code));
    return (err_code);
  }
  /*EFUN(wr_uc_dsc_supp_info(supp_info)); */                   /* Supplement info field */
  /*EFUN(wr_uc_dsc_error_found(0x0)    ); */                   /* Clear error found field */
  /*EFUN(wr_uc_dsc_gp_uc_req(cmd)      ); */                   /* Set command code */
  /*EFUN(wr_uc_dsc_ready_for_cmd(0x0)  ); */                   /* Issue command, by clearing "ready for command" field */
  cmddata = (((uint16_t)supp_info)<<8) | (uint16_t)cmd;        /* Combine writes to single write instead of 4 RMW */

  EFUN(reg_wr_DSC_A_DSC_UC_CTRL(cmddata));         

  return(ERR_CODE_NONE);
}

err_code_t falcon16_tsc_pmd_uc_cmd(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint32_t timeout_ms) {

  EFUN(falcon16_tsc_pmd_uc_cmd_return_immediate(sa__, cmd, supp_info));    /* Invoke falcon16_tsc_uc_cmd and return control immediately */

  EFUN(falcon16_tsc_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(sa__, timeout_ms, cmd)); /* Poll for uc_dsc_ready_for_cmd = 1 to indicate falcon16_tsc ready for command */
  
  return(ERR_CODE_NONE);
}


err_code_t falcon16_tsc_pmd_uc_cmd_with_data_return_immediate(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint16_t data) {
  uint16_t   cmddata;
  err_code_t err_code;

  err_code = falcon16_tsc_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(sa__, 1, cmd); /* Poll for uc_dsc_ready_for_cmd = 1 to indicate falcon16_tsc ready for command */
  if (err_code) {
     EFUN_PRINTF(("ERROR : DSC ready for command timed out (before cmd) cmd = %d, supp_info = x%02x, data = x%04x err=%d !\n", cmd, supp_info, data,err_code));
    return (err_code);
  }

  EFUN(wr_uc_dsc_data(data));                                  /* Write value written to uc_dsc_data field */
  /*EFUN(wr_uc_dsc_supp_info(supp_info)); */                   /* Supplement info field */
  /*EFUN(wr_uc_dsc_error_found(0x0));     */                   /* Clear error found field */
  /*EFUN(wr_uc_dsc_gp_uc_req(cmd));       */                   /* Set command code */
  /*EFUN(wr_uc_dsc_ready_for_cmd(0x0));   */                   /* Issue command, by clearing "ready for command" field */
  cmddata = (((uint16_t)supp_info)<<8) | (uint16_t)cmd;        /* Combine writes to single write instead of 4 RMW */

  EFUN(reg_wr_DSC_A_DSC_UC_CTRL(cmddata));         

  return(ERR_CODE_NONE);
}


err_code_t falcon16_tsc_pmd_uc_cmd_with_data(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint16_t data, uint32_t timeout_ms) {

    EFUN(falcon16_tsc_pmd_uc_cmd_with_data_return_immediate(sa__, cmd, supp_info, data)); /* Invoke falcon16_tsc_uc_cmd and return control immediately */

    EFUN(falcon16_tsc_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(sa__, timeout_ms, cmd)); /* Poll for uc_dsc_ready_for_cmd = 1 to indicate falcon16_tsc ready for command */
    return(ERR_CODE_NONE);
}

err_code_t falcon16_tsc_pmd_uc_control_return_immediate(srds_access_t *sa__, enum srds_pmd_uc_ctrl_cmd_enum control) {
  return(falcon16_tsc_pmd_uc_cmd_return_immediate(sa__, CMD_UC_CTRL, (uint8_t) control));
}

err_code_t falcon16_tsc_pmd_uc_control(srds_access_t *sa__, enum srds_pmd_uc_ctrl_cmd_enum control, uint32_t timeout_ms) {
  return(falcon16_tsc_pmd_uc_cmd(sa__, CMD_UC_CTRL, (uint8_t) control, timeout_ms));
}

err_code_t falcon16_tsc_pmd_uc_diag_cmd(srds_access_t *sa__, enum srds_pmd_uc_diag_cmd_enum control, uint32_t timeout_ms) {
  return(falcon16_tsc_pmd_uc_cmd(sa__, CMD_DIAG_EN, (uint8_t) control, timeout_ms));
}

/************************************************************/
/*      Serdes IP RAM access - Lane RAM Variables           */
/*----------------------------------------------------------*/
/*   - through Micro Register Interface for PMD IPs         */
/*   - through Serdes FW DSC Command Interface for Gallardo */
/************************************************************/

/* Micro RAM Lane Byte Read */
uint8_t falcon16_tsc_rdbl_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
  uint8_t rddata;
  uint16_t lane_var_ram_base=0, lane_var_ram_size=0;

  if(!err_code_p) {
      return(0);
  }

  lane_var_ram_base = LANE_VAR_RAM_BASE;
  lane_var_ram_size = LANE_VAR_RAM_SIZE;

  EPSTM(rddata = falcon16_tsc_rdb_uc_ram(sa__, err_code_p, (lane_var_ram_base+addr+(falcon16_tsc_get_lane(sa__)*lane_var_ram_size)))); /* Use Micro register interface for reading RAM */
  return (rddata);
}

/* Micro RAM Lane Byte Signed Read */
int8_t falcon16_tsc_rdbls_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
  return ((int8_t) falcon16_tsc_rdbl_uc_var(sa__, err_code_p, addr));
}

/* Micro RAM Lane Word Read */
uint16_t falcon16_tsc_rdwl_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
  uint16_t rddata;
  uint16_t lane_var_ram_base=0, lane_var_ram_size=0;

  if(!err_code_p) {
      return(0);
  }
  if (addr%2 != 0) {                                                                /* Validate even address */
      *err_code_p = ERR_CODE_INVALID_RAM_ADDR;
      return (0);
  }

  lane_var_ram_base = LANE_VAR_RAM_BASE;
  lane_var_ram_size = LANE_VAR_RAM_SIZE;

  EPSTM(rddata = falcon16_tsc_rdw_uc_ram(sa__, err_code_p, (lane_var_ram_base+addr+(falcon16_tsc_get_lane(sa__)*lane_var_ram_size)))); /* Use Micro register interface for reading RAM */
  return (rddata);
}


/* Micro RAM Lane Word Signed Read */
int16_t falcon16_tsc_rdwls_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
  return ((int16_t) falcon16_tsc_rdwl_uc_var(sa__, err_code_p, addr));
}

/* Micro RAM Lane Byte Write */
err_code_t falcon16_tsc_wrbl_uc_var(srds_access_t *sa__, uint16_t addr, uint8_t wr_val) {

  uint16_t lane_var_ram_base=0, lane_var_ram_size=0;

  lane_var_ram_base = LANE_VAR_RAM_BASE;
  lane_var_ram_size = LANE_VAR_RAM_SIZE;

  return (falcon16_tsc_wrb_uc_ram(sa__, (lane_var_ram_base+addr+(falcon16_tsc_get_lane(sa__)*lane_var_ram_size)), wr_val));    /* Use Micro register interface for writing RAM */
}

/* Micro RAM Lane Byte Signed Write */
err_code_t falcon16_tsc_wrbls_uc_var(srds_access_t *sa__, uint16_t addr, int8_t wr_val) {
  return (falcon16_tsc_wrbl_uc_var(sa__, addr, wr_val));
}

/* Micro RAM Lane Word Write */
err_code_t falcon16_tsc_wrwl_uc_var(srds_access_t *sa__, uint16_t addr, uint16_t wr_val) {
    uint16_t lane_var_ram_base=0, lane_var_ram_size=0;

  if (addr%2 != 0) {                                                                /* Validate even address */
      return (_error(ERR_CODE_INVALID_RAM_ADDR));
  }

  lane_var_ram_base = LANE_VAR_RAM_BASE;
  lane_var_ram_size = LANE_VAR_RAM_SIZE;

  return (falcon16_tsc_wrw_uc_ram(sa__, (lane_var_ram_base+addr+(falcon16_tsc_get_lane(sa__)*lane_var_ram_size)), wr_val));    /* Use Micro register interface for writing RAM */
}

/* Micro RAM Lane Word Signed Write */
err_code_t falcon16_tsc_wrwls_uc_var(srds_access_t *sa__, uint16_t addr, int16_t wr_val) {
  return (falcon16_tsc_wrwl_uc_var(sa__, addr,wr_val));
}


/************************************************************/
/*      Serdes IP RAM access - Core RAM Variables           */
/*----------------------------------------------------------*/
/*   - through Micro Register Interface for PMD IPs         */
/*   - through Serdes FW DSC Command Interface for Gallardo */
/************************************************************/

/* Micro RAM Core Byte Read */
uint8_t falcon16_tsc_rdbc_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint8_t addr) {

    uint8_t rddata;
    uint16_t core_var_ram_base=0;

  if(!err_code_p) {
      return(0);
  }

  core_var_ram_base = CORE_VAR_RAM_BASE;

  EPSTM(rddata = falcon16_tsc_rdb_uc_ram(sa__, err_code_p, (core_var_ram_base+addr)));    /* Use Micro register interface for reading RAM */
  return (rddata);
}

/* Micro RAM Core Byte Signed Read */
int8_t falcon16_tsc_rdbcs_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint8_t addr) {
  return ((int8_t) falcon16_tsc_rdbc_uc_var(sa__, err_code_p, addr));
}

/* Micro RAM Core Word Read */
uint16_t falcon16_tsc_rdwc_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint8_t addr) {
    uint16_t rddata;
    uint16_t core_var_ram_base=0;

    if(!err_code_p) {
        return(0);
    }
    if (addr%2 != 0) {                                                                /* Validate even address */
        *err_code_p = ERR_CODE_INVALID_RAM_ADDR;
        return (0);
    }

    core_var_ram_base = CORE_VAR_RAM_BASE;

    EPSTM(rddata = falcon16_tsc_rdw_uc_ram(sa__, err_code_p, (core_var_ram_base+addr)));    /* Use Micro register interface for reading RAM */
    return (rddata);
}

/* Micro RAM Core Word Signed Read */
int16_t falcon16_tsc_rdwcs_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint8_t addr) {
  return ((int16_t) falcon16_tsc_rdwc_uc_var(sa__, err_code_p, addr));
}

/* Micro RAM Core Byte Write  */
err_code_t falcon16_tsc_wrbc_uc_var(srds_access_t *sa__, uint8_t addr, uint8_t wr_val) {
    uint16_t core_var_ram_base=0;

    core_var_ram_base = CORE_VAR_RAM_BASE;

    return (falcon16_tsc_wrb_uc_ram(sa__, (core_var_ram_base+addr), wr_val));               /* Use Micro register interface for writing RAM */
}


/* Micro RAM Core Byte Signed Write */
err_code_t falcon16_tsc_wrbcs_uc_var(srds_access_t *sa__, uint8_t addr, int8_t wr_val) {
  return (falcon16_tsc_wrbc_uc_var(sa__, addr, wr_val));
}

/* Micro RAM Core Word Write  */
err_code_t falcon16_tsc_wrwc_uc_var(srds_access_t *sa__, uint8_t addr, uint16_t wr_val) {
    uint16_t core_var_ram_base=0;

    core_var_ram_base = CORE_VAR_RAM_BASE;

    if (addr%2 != 0) {                                                              /* Validate even address */
        return (_error(ERR_CODE_INVALID_RAM_ADDR));
    }
    return (falcon16_tsc_wrw_uc_ram(sa__, (core_var_ram_base+addr), wr_val));             /* Use Micro register interface for writing RAM */
}

/* Micro RAM Core Word Signed Write */
err_code_t falcon16_tsc_wrwcs_uc_var(srds_access_t *sa__, uint8_t addr, int16_t wr_val) {
  return(falcon16_tsc_wrwc_uc_var(sa__, addr,wr_val));
}

/*********************************************************/
/*  Program RAM access through Micro Register Interface  */
/*********************************************************/

/* Micro Program Ram Long Read */
uint32_t falcon16_tsc_rd_long_uc_prog_ram(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
   uint32_t rddata;
   if(!err_code_p) {
        return(0);
   }
   *err_code_p = ERR_CODE_NONE;
   EPFUN(wrc_micro_autoinc_rdaddr_en(1));
   EPFUN(wrc_micro_ra_rddatasize(0x1));                 /* Select 16bit read datasize */
   EPFUN(wrc_micro_ra_rdaddr_msw(0));                   /* Upper 16bits of ROM address to be read */
   EPFUN(wrc_micro_ra_rdaddr_lsw(addr));                /* Lower 16bits of ROM address to be read */
   EPSTM(rddata = rdc_micro_ra_rddata_lsw());           /* 16bit read data */
   EPSTM(rddata |= (rdc_micro_ra_rddata_lsw() << 16));  /* 16bit read data */
   return (rddata);
}

/* Micro Program RAM Block Read */
err_code_t falcon16_tsc_rdblk_uc_prog_ram(srds_access_t *sa__, uint8_t *mem, uint16_t addr, uint16_t cnt) {
    falcon16_tsc_INTERNAL_rdblk_callback_arg_t arg;

    if(!mem) {
        return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    arg.mem_ptr = mem;
    return falcon16_tsc_INTERNAL_rdblk_uc_generic_ram(sa__, addr, cnt, 0, cnt, &arg, falcon16_tsc_INTERNAL_rdblk_callback);
}

/*************************************************/
/*  RAM access through Micro Register Interface  */
/*************************************************/

/* Micro RAM Long Write */
err_code_t falcon16_tsc_wr_long_uc_ram(srds_access_t *sa__, uint16_t offset, uint32_t value) {
    EFUN(falcon16_tsc_wrw_uc_ram(sa__, offset+2, value >> 16));
    EFUN(falcon16_tsc_wrw_uc_ram(sa__, offset, value & 0xFFFF));
    return (ERR_CODE_NONE);
}
/* Micro RAM Word Write */
err_code_t falcon16_tsc_wrw_uc_ram(srds_access_t *sa__, uint16_t addr, uint16_t wr_val) {

    EFUN(wrc_micro_autoinc_wraddr_en(0));
    EFUN(wrc_micro_ra_wrdatasize(0x1));                 /* Select 16bit write datasize */
    EFUN(wrc_micro_ra_wraddr_msw(0x2000));              /* Upper 16bits of RAM address to be written to */
    EFUN(wrc_micro_ra_wraddr_lsw(addr));                /* Lower 16bits of RAM address to be written to */
    EFUN(wrc_micro_ra_wrdata_lsw(wr_val));              /* uC RAM lower 16bits write data */
    return (ERR_CODE_NONE);
}

/* Micro RAM Byte Write */
err_code_t falcon16_tsc_wrb_uc_ram(srds_access_t *sa__, uint16_t addr, uint8_t wr_val) {
    EFUN(wrc_micro_autoinc_wraddr_en(0));
    EFUN(wrc_micro_ra_wrdatasize(0x0));                 /* Select 8bit write datasize */
    EFUN(wrc_micro_ra_wraddr_msw(0x2000));              /* Upper 16bits of RAM address to be written to */
    EFUN(wrc_micro_ra_wraddr_lsw(addr));                /* Lower 16bits of RAM address to be written to */
    EFUN(wrc_micro_ra_wrdata_lsw(wr_val));              /* uC RAM lower 16bits write data */
    return (ERR_CODE_NONE);
}

/* Micro RAM Long Read */
uint32_t falcon16_tsc_rd_long_uc_ram(srds_access_t *sa__, err_code_t *err, uint16_t offset) {
    uint32_t result;
    ESTM(result = falcon16_tsc_rdw_uc_ram(sa__, err, offset+2) << 16);
    ESTM(result |= falcon16_tsc_rdw_uc_ram(sa__, err, offset) & 0xFFFF);
    return result;
}
/* Micro RAM Word Read */
uint16_t falcon16_tsc_rdw_uc_ram(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
   uint16_t rddata;
   if(!err_code_p) {
        return(0);
   }
   *err_code_p = ERR_CODE_NONE;
   EPFUN(wrc_micro_autoinc_rdaddr_en(0));
   EPFUN(wrc_micro_ra_rddatasize(0x1));                 /* Select 16bit read datasize */
   EPFUN(wrc_micro_ra_rdaddr_msw(0x2000));              /* Upper 16bits of RAM address to be read */
   EPFUN(wrc_micro_ra_rdaddr_lsw(addr));                /* Lower 16bits of RAM address to be read */
   EPSTM(rddata = rdc_micro_ra_rddata_lsw());           /* 16bit read data */
   return (rddata);
}

/* Micro RAM Byte Read */
uint8_t falcon16_tsc_rdb_uc_ram(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
    uint8_t rddata;
    if(!err_code_p) {
        return(0);
    }
    *err_code_p = ERR_CODE_NONE;
    EPFUN(wrc_micro_autoinc_rdaddr_en(0));
    EPFUN(wrc_micro_ra_rddatasize(0x0));                 /* Select 8bit read datasize */
    EPFUN(wrc_micro_ra_rdaddr_msw(0x2000));              /* Upper 16bits of RAM address to be read */
    EPFUN(wrc_micro_ra_rdaddr_lsw(addr));                /* Lower 16bits of RAM address to be read */
    EPSTM(rddata = (uint8_t) rdc_micro_ra_rddata_lsw()); /* 16bit read data */
    return (rddata);
}

/* Micro RAM Word Signed Write */
err_code_t falcon16_tsc_wrws_uc_ram(srds_access_t *sa__, uint16_t addr, int16_t wr_val) {
    return (falcon16_tsc_wrw_uc_ram(sa__, addr, wr_val));
}

/* Micro RAM Byte Signed Write */
err_code_t falcon16_tsc_wrbs_uc_ram(srds_access_t *sa__, uint16_t addr, int8_t wr_val) {
    return (falcon16_tsc_wrb_uc_ram(sa__, addr, wr_val));
}

/* Micro RAM Word Signed Read */
int16_t falcon16_tsc_rdws_uc_ram(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
    return ((int16_t)falcon16_tsc_rdw_uc_ram(sa__, err_code_p, addr));
}

/* Micro RAM Byte Signed Read */
int8_t falcon16_tsc_rdbs_uc_ram(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
    return ((int8_t)falcon16_tsc_rdb_uc_ram(sa__, err_code_p, addr));
}

/* Micro RAM Block Read */
err_code_t falcon16_tsc_rdblk_uc_ram(srds_access_t *sa__, uint8_t *mem, uint16_t addr, uint16_t cnt) {
    falcon16_tsc_INTERNAL_rdblk_callback_arg_t arg;

    if(!mem) {
        return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    arg.mem_ptr = mem;
    return falcon16_tsc_INTERNAL_rdblk_uc_generic_ram(sa__, (uint32_t)addr | 0x20000000, cnt, 0, cnt, &arg, falcon16_tsc_INTERNAL_rdblk_callback);
}

/**************************/
/*  Temperature reading   */
/**************************/

err_code_t falcon16_tsc_read_die_temperature (srds_access_t *sa__, int16_t *die_temp) {

    uint16_t die_temp_sensor_reading;

    EFUN(falcon16_tsc_pmd_uc_cmd(sa__, CMD_UC_DBG,CMD_UC_DBG_DIE_TEMP,100));
    ESTM(die_temp_sensor_reading=rd_uc_dsc_data());
    *die_temp = _bin_to_degC(die_temp_sensor_reading);

    return(ERR_CODE_NONE);
}


err_code_t falcon16_tsc_read_die_temperature_double (srds_access_t *sa__, USR_DOUBLE *die_temp) {
#ifdef SERDES_API_FLOATING_POINT
    uint16_t die_temp_sensor_reading;

    EFUN(falcon16_tsc_pmd_uc_cmd(sa__, CMD_UC_DBG,CMD_UC_DBG_DIE_TEMP,50));
    ESTM(die_temp_sensor_reading=rd_uc_dsc_data());

    *die_temp = _bin_to_degC_double(die_temp_sensor_reading);

    return(ERR_CODE_NONE);
#else
    *die_temp = 0;
    EFUN_PRINTF((" Function 'falcon16_tsc_read_die_temperature_double' needs SERDES_API_FLOATING_POINT defined to operate \n"));
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
#endif
}

