/***********************************************************************************
 ***********************************************************************************
 *  File Name     :  merlin16_internal.c                                         *
 *  Created On    :  13/02/2014                                                    *
 *  Created By    :  Justin Gaither                                                *
 *  Description   :  APIs for Serdes IPs                                           *
 *  Revision      :   *
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

/** @file merlin16_internal.c
 * Implementation of API functions
 */

#include <phymod/phymod.h>
#include "merlin16_internal.h"
#include "merlin16_internal_error.h"
#include "merlin16_access.h"
#include "merlin16_common.h"
#include "merlin16_config.h"
#include "merlin16_functions.h"
#include "merlin16_select_defns.h"


/* If SERDES_EVAL is defined, then is_ate_log_enabled() is queried to *\
\* know whether to log ATE.  merlin16_access.h provides that function.  */

uint32_t merlin16_INTERNAL_mult_with_overflow_check(uint32_t a, uint32_t b, uint8_t *of) {
    uint16_t c,d;
    uint32_t r,s;
    if (a > b) return merlin16_INTERNAL_mult_with_overflow_check(b, a, of);
    *of = 0;
    c = b >> 16;
    d = UINT16_MAX & b;
    r = a * c;
    s = a * d;
    if (r > UINT16_MAX) *of = 1;
    r <<= 16;
    return (s + r);
}

static merlin16_info_t merlin16_info;

merlin16_info_t *merlin16_INTERNAL_get_merlin16_info_ptr(void) {
    return &merlin16_info;
}

err_code_t merlin16_INTERNAL_verify_merlin16_info(merlin16_info_t const *test_info)
{
    if (test_info->signature != SRDS_INFO_SIGNATURE) {
        EFUN_PRINTF(("ERROR:  Mismatch in merlin16_info signature.  Expected 0x%08X at index %d, but received 0x%08X.\n",
                     SRDS_INFO_SIGNATURE, 10, test_info->signature));
        return (_error(ERR_CODE_INFO_TABLE_ERROR));
    }
    return (ERR_CODE_NONE);
}

/* Store a cached AFE version for re-use */
err_code_t merlin16_INTERNAL_get_afe_hw_version(srds_access_t *sa__, uint8_t *afe_hw_version) {
  static uint8_t _cached_afe_hw_version = 255;
  
  if (!afe_hw_version)
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  if (_cached_afe_hw_version == 255)
    ESTM(_cached_afe_hw_version = rdcv_afe_hardware_version());
  *afe_hw_version = _cached_afe_hw_version;
  return(ERR_CODE_NONE);
}
  
  

err_code_t merlin16_INTERNAL_get_p1_threshold(srds_access_t *sa__, int8_t *val) {
 ESTM(*val = -rd_p1_eyediag_bin()); 
 return (ERR_CODE_NONE);
}

/* Setup the P1 slicer vertical level  */
err_code_t merlin16_INTERNAL_set_p1_threshold(srds_access_t *sa__, int8_t threshold) {

  EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */
  EFUN(wr_hwtune_ovr_write_sel(0xd));                 /* Configure hwtune_ovr_write_sel to p1_eyediag mode  */
  /* invert polarity for MERLIN, JIRA CRMERLIN_PMD-159 */
  EFUN(wr_hwtune_ovr_write_val((-threshold)<<3));     /* hwtune_ovr_write_val[8:3] are used to drive the analog control port. */
  EFUN(wr_hwtune_ovr_write_en(0x1));                  /* Enable f/w to write the tap values */
  return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_move_clkp1_offset(srds_access_t *sa__, int8_t delta) {
  int8_t cnt;

  EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */

  EFUN(wr_rx_pi_slicers_en(0x2));                     /* Select p1 slicer to adjust */
  EFUN(wr_rx_pi_phase_step_dir(delta>0));             /* 1 for positive step   */
  EFUN(wr_rx_pi_phase_step_cnt(1));                   /* Step set to 1 */
  for (cnt=0; cnt < SRDS_ABS(delta); cnt++) {
    EFUN(wr_rx_pi_manual_strobe(1));                  /* Increments/Decrements by 1 every strobe */
  } 
  return(ERR_CODE_NONE);
}



int16_t merlin16_INTERNAL_ladder_setting_to_mV(srds_access_t *sa__, int8_t ctrl, uint8_t range_250) {
    uint16_t absv;                                     /* Absolute value of ctrl */
    int16_t nlmv;                                     /* Non-linear value */
    
    /* Get absolute value */
    absv = SRDS_ABS(ctrl);

    { 
        if (range_250) {
            /* 28nm range is 250mV, but 16nm range is 200mV */
            /* 200mV range, 6.5519mV linear units */
            nlmv = absv*190/29;
        }
        else {
            /* 120mV range, 3.8854mV linear units */
            nlmv = absv*136/35;
        }
    }
    /* Add sign and return */
    return( (ctrl>=0) ? nlmv : -nlmv);
}

err_code_t merlin16_INTERNAL_compute_bin(char var, char bin[]) {
    if(!bin) {
        return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

  switch (var) {
    case '0':  ENULL_STRCPY(bin,"0000");
               break;
    case '1':  ENULL_STRCPY(bin,"0001");
               break;
    case '2':  ENULL_STRCPY(bin,"0010");
               break;
    case '3':  ENULL_STRCPY(bin,"0011");
               break;
    case '4':  ENULL_STRCPY(bin,"0100");
               break;
    case '5':  ENULL_STRCPY(bin,"0101");
               break;
    case '6':  ENULL_STRCPY(bin,"0110");
               break;
    case '7':  ENULL_STRCPY(bin,"0111");
               break;
    case '8':  ENULL_STRCPY(bin,"1000");
               break;
    case '9':  ENULL_STRCPY(bin,"1001");
               break;
    case 'a':
    case 'A':  ENULL_STRCPY(bin,"1010");
               break;
    case 'b':
    case 'B':  ENULL_STRCPY(bin,"1011");
               break;
    case 'c':
    case 'C':  ENULL_STRCPY(bin,"1100");
               break;
    case 'd':
    case 'D':  ENULL_STRCPY(bin,"1101");
               break;
    case 'e':
    case 'E':  ENULL_STRCPY(bin,"1110");
               break;
    case 'f':
    case 'F':  ENULL_STRCPY(bin,"1111");
               break;
    case '_':  ENULL_STRCPY(bin,"");
               break;
    default :  ENULL_STRCPY(bin,"");
               EFUN_PRINTF(("ERROR: Invalid Hexadecimal Pattern\n"));
               return (_error(ERR_CODE_CFG_PATT_INVALID_HEX));
               break;
  }
  return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_compute_hex(char bin[],uint8_t *hex) {
  if(!hex) {
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }
 
  if (!USR_STRCMP(bin,"0000")) {
    *hex = 0x0;
  } 
  else if (!USR_STRCMP(bin,"0001")) {
    *hex = 0x1;
  } 
  else if (!USR_STRCMP(bin,"0010")) {
    *hex = 0x2;
  } 
  else if (!USR_STRCMP(bin,"0011")) {
    *hex = 0x3;
  } 
  else if (!USR_STRCMP(bin,"0100")) {
    *hex = 0x4;
  } 
  else if (!USR_STRCMP(bin,"0101")) {
    *hex = 0x5;
  } 
  else if (!USR_STRCMP(bin,"0110")) {
    *hex = 0x6;
  } 
  else if (!USR_STRCMP(bin,"0111")) {
    *hex = 0x7;
  } 
  else if (!USR_STRCMP(bin,"1000")) {
    *hex = 0x8;
  } 
  else if (!USR_STRCMP(bin,"1001")) {
    *hex = 0x9;
  } 
  else if (!USR_STRCMP(bin,"1010")) {
    *hex = 0xA;
  } 
  else if (!USR_STRCMP(bin,"1011")) {
    *hex = 0xB;
  } 
  else if (!USR_STRCMP(bin,"1100")) {
    *hex = 0xC;
  } 
  else if (!USR_STRCMP(bin,"1101")) {
    *hex = 0xD;
  } 
  else if (!USR_STRCMP(bin,"1110")) {
    *hex = 0xE;
  } 
  else if (!USR_STRCMP(bin,"1111")) {
    *hex = 0xF;
  } 
  else {
    EFUN_PRINTF(("ERROR: Invalid Binary to Hex Conversion\n"));
    *hex = 0x0;
    return (_error(ERR_CODE_CFG_PATT_INVALID_BIN2HEX));
  } 
  return (ERR_CODE_NONE);
}

/* Repeater Only APIs (Not applicable to PMD) */

  

/********************************************************/
/*  Global RAM access through Micro Register Interface  */
/********************************************************/
/* Micro Global RAM Byte Read */
uint8_t merlin16_INTERNAL_rdb_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
    uint8_t rddata;

    if(!err_code_p) {
        return(0);
    }
    EPSTM(rddata = merlin16_rdb_uc_ram(sa__, err_code_p, addr)); /* Use Micro register interface for reading RAM */
    return (rddata);
}

/* Micro Global RAM Word Read */
uint16_t merlin16_INTERNAL_rdw_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr) {
  uint16_t rddata;

  if(!err_code_p) {
      return(0);
  }
  if (addr%2 != 0) {                                         /* Validate even address */
      *err_code_p = ERR_CODE_INVALID_RAM_ADDR;
      return (0);
  }
  EPSTM(rddata = merlin16_rdw_uc_ram(sa__, err_code_p, (addr))); /* Use Micro register interface for reading RAM */
  return (rddata);
}

/* Micro Global RAM Byte Write  */
err_code_t merlin16_INTERNAL_wrb_uc_var(srds_access_t *sa__, uint16_t addr, uint8_t wr_val) {

    return (merlin16_wrb_uc_ram(sa__, addr, wr_val));          /* Use Micro register interface for writing RAM */
}

/* Micro Global RAM Word Write  */
err_code_t merlin16_INTERNAL_wrw_uc_var(srds_access_t *sa__, uint16_t addr, uint16_t wr_val) {
    if (addr%2 != 0) {                                       /* Validate even address */
        return (_error(ERR_CODE_INVALID_RAM_ADDR));
    }
    return (merlin16_wrw_uc_ram(sa__, addr, wr_val));          /* Use Micro register interface for writing RAM */
}


/***********************/
/*  Event Log Display  */
/***********************/
err_code_t merlin16_INTERNAL_event_log_dump_callback(void *arg, uint8_t byte_count, uint16_t data) {
    merlin16_INTERNAL_event_log_dump_state_t * const state_ptr = (merlin16_INTERNAL_event_log_dump_state_t *)arg;
    const uint8_t bytes_per_row=16;

    if (byte_count == 0) {
        if (state_ptr->line_start_index != state_ptr->index) {
            EFUN_PRINTF(("%*s    %d\n", 4*(bytes_per_row - state_ptr->index + state_ptr->line_start_index), "", state_ptr->line_start_index));
        }
        return (ERR_CODE_NONE);
    }
    if (byte_count == 1) {
        /* There is a trailing byte in the event log.
         * The simplest way to handle it is to print out a whole word, but mask
         * the invalid upper byte.
         */
        data &= 0xFF;
    }
    EFUN_PRINTF(("  0x%04x", ((data & 0xFF) << 8) | (data >> 8)));
    state_ptr->index += 2;
    if (state_ptr->index % bytes_per_row == 0) {
        EFUN_PRINTF(("    %d\n", state_ptr->line_start_index));
        state_ptr->line_start_index = state_ptr->index;
    }

    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_read_event_log_with_callback(srds_access_t *sa__,
                                                        uint8_t micro_num,
                                                        uint8_t bypass_micro,
                                                        void *arg,
                                                        err_code_t (*callback)(void *, uint8_t, uint16_t)) {
    merlin16_info_t const * const merlin16_info_ptr = merlin16_INTERNAL_get_merlin16_info_ptr();
    uint16_t rd_idx;

    EFUN(merlin16_INTERNAL_verify_merlin16_info(merlin16_info_ptr));

    if (micro_num >= merlin16_info_ptr->micro_count) {
        return (_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    

    EFUN_PRINTF(("\n\n********************************************\n"));
    EFUN_PRINTF((    "**** SERDES UC TRACE MEMORY DUMP ***********\n"));
    EFUN_PRINTF((    "********************************************\n"));

    if (bypass_micro) {
        ESTM(rd_idx = rdcv_trace_mem_wr_idx());
        if (merlin16_info_ptr->trace_memory_descending_writes) {
            ++rd_idx;
            if (rd_idx >= merlin16_info_ptr->trace_mem_ram_size) {
                rd_idx = 0;
            }
        } else {
            if (rd_idx == 0) {
                rd_idx = merlin16_info_ptr->trace_mem_ram_size;
            }
            --rd_idx;
        }
    } else {
        /* Start Read to stop uC logging and read the word at last event written by uC */
        EFUN(merlin16_pmd_uc_cmd(sa__, CMD_EVENT_LOG_READ, CMD_EVENT_LOG_READ_START, 10));
        ESTM(rd_idx = rdcv_trace_mem_rd_idx());
    }
    
    EFUN_PRINTF(( "\n  DEBUG INFO: trace memory read index = 0x%04x\n", rd_idx));
    
    EFUN_PRINTF(("  DEBUG INFO: trace memory size = 0x%04x\n\n", merlin16_info_ptr->trace_mem_ram_size));

    if (merlin16_info_ptr->trace_memory_descending_writes) {
        /* Micro writes trace memory using descending addresses.
         * So read using ascending addresses using block read
         */
        EFUN(merlin16_INTERNAL_rdblk_uc_generic_ram(sa__,
                                                  merlin16_info_ptr->trace_mem_ram_base,
                                                  merlin16_info_ptr->trace_mem_ram_size,
                                                  rd_idx,
                                                  merlin16_info_ptr->trace_mem_ram_size,
                                                  arg,
                                                  callback));
    } else {
        /* Micro writes trace memory using descending addresses.
         * So read using ascending addresses using block read
         */
        EFUN(merlin16_INTERNAL_rdblk_uc_generic_ram_descending(sa__,
                                                             merlin16_info_ptr->trace_mem_ram_base,
                                                             merlin16_info_ptr->trace_mem_ram_size,
                                                             rd_idx,
                                                             merlin16_info_ptr->trace_mem_ram_size,
                                                             arg,
                                                             callback));
    }

    if (!bypass_micro) {
        /* Read Done to resume logging  */
        EFUN(merlin16_pmd_uc_cmd(sa__, CMD_EVENT_LOG_READ, CMD_EVENT_LOG_READ_DONE, 10));
    }

    return(ERR_CODE_NONE);
}

#ifdef TO_FLOATS
/* convert uint32_t to float8 */
float8_t merlin16_INTERNAL_int32_to_float8(uint32_t input) {
    int8_t cnt;
    uint8_t output;
    
    if(input == 0) {
      return(0);
    } else if(input == 1) {
      return(0xe0);
    } else {
      cnt = 0;
      input = input & 0x7FFFFFFF; /* get rid of MSB which may be lock indicator */
      do {
        input = input << 1;
        cnt++;
      } while ((input & 0x80000000) == 0);
    
      output = (uint8_t)((input & 0x70000000)>>23)+(31 - cnt%32);
      return(output);
    }
}
#endif

/* convert float8 to uint32_t */
uint32_t merlin16_INTERNAL_float8_to_int32(float8_t input) {
    uint32_t x;
    if(input == 0) return(0);
    x = (input>>5) + 8;
    if((input & 0x1F) < 3) {
      return(x>>(3-(input & 0x1f)));
    } 
    return(x<<((input & 0x1F)-3));
}

/* convert float12 to uint32 */
uint32_t merlin16_INTERNAL_float12_to_uint32(uint8_t byte, uint8_t multi) {

    return(((uint32_t)byte)<<multi);
}

#ifdef TO_FLOATS
/* convert uint32 to float12 */
uint8_t merlin16_INTERNAL_uint32_to_float12(uint32_t input, uint8_t *multi) {
    int8_t cnt;
    uint8_t output;
    if(!multi) {
      return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    
    if((input == 0) || (!multi)) {
      *multi = 0;
      return(0);
    } else {
      cnt = 0;
      if(input > 0x007FFFFF) input = 0x007FFFFF; /* limit to 23bits so multi is 4 bits */
      do {
        input = input << 1;
        cnt++;
      } while ((input & 0x80000000) == 0);
      
      *multi = (31 - (cnt%32));
      if(*multi < 8) {
        output = (uint8_t)((input & 0xFF000000)>>(24 + (7-*multi)));
        *multi = 0;
      } else {
        output = (uint8_t)((input & 0xFF000000)>>24);
        *multi = *multi - 7;
      }
      return(output);
    }
}
#endif

err_code_t merlin16_INTERNAL_set_rx_pf_main(srds_access_t *sa__, uint8_t val) {
    if (val > 15) {
      return (_error(ERR_CODE_PF_INVALID));
    }
    EFUN(WR_RX_PF_CTRL(val));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_pf_main(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = (int8_t)RD_RX_PF_CTRL());
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_set_rx_pf2(srds_access_t *sa__, uint8_t val) {
    if (val > 7) {
      return (_error(ERR_CODE_PF_INVALID));
    }
    EFUN(WR_RX_PF2_CTRL(val));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_pf2(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = (int8_t)RD_RX_PF2_CTRL());
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_set_rx_vga(srds_access_t *sa__, uint8_t val) {

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */

    if (val > RX_VGA_CTRL_VAL_MAX) {
      return (_error(ERR_CODE_VGA_INVALID));
    }
#if defined(RX_VGA_CTRL_VAL_MIN) && (RX_VGA_CTRL_VAL_MIN > 0)
    if (val < RX_VGA_CTRL_VAL_MIN) {
      return (_error(ERR_CODE_VGA_INVALID));
    } 
#endif
    
    EFUN(wr_hwtune_ovr_write_sel(0));                   /* Configure hwtune_ovr_write_sel to VGA */
    EFUN(wr_hwtune_ovr_write_val(val<<3));              /* hwtune_ovr_write_val[8:3] are used to drive the analog control port */
    EFUN(wr_hwtune_ovr_write_en(1));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_vga(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = (int8_t)rd_vga_bin()); 
    return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_set_rx_dfe1(srds_access_t *sa__, int8_t val) {

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */

    {
        int8_t tap_eo;

        if (val > 63) {
            return (_error(ERR_CODE_DFE1_INVALID));  
        }
        /* Compute tap1 even/odd component */
        tap_eo = 0;                                      /* Not supporting dfe_dcd values */
        EFUN(wr_hwtune_ovr_write_sel(2));                /* Write tap1_odd */
        EFUN(wr_hwtune_ovr_write_val(tap_eo));
        EFUN(wr_hwtune_ovr_write_en(1));
        EFUN(wr_hwtune_ovr_write_sel(3));                /* Write tap1_even */
        EFUN(wr_hwtune_ovr_write_val(tap_eo));
        EFUN(wr_hwtune_ovr_write_en(1));
        EFUN(wr_hwtune_ovr_write_sel(1));
        EFUN(wr_hwtune_ovr_write_val(val-tap_eo));       /* Write the common tap */
        EFUN(wr_hwtune_ovr_write_en(1));
    }
    return(ERR_CODE_NONE); 
}
err_code_t merlin16_INTERNAL_get_rx_dfe1(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = (int8_t)(rd_dfe_1_e() + rd_dfe_1_cmn()));
    return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_set_rx_dfe2(srds_access_t *sa__, int8_t val) {
    int8_t tap_eo;

    if ((val > 31) || (val < -31)) {
      return (_error(ERR_CODE_DFE2_INVALID));  
    }

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */

    /* Compute tap2 odd/even component */
    tap_eo = 0;                                         /* Not supporting dfe_dcd values */
    EFUN(wr_hwtune_ovr_write_sel(5));                   /* Write tap2_odd */
    EFUN(wr_hwtune_ovr_write_val(tap_eo));
    EFUN(wr_hwtune_ovr_write_en(1));
    EFUN(wr_hwtune_ovr_write_sel(6));                   /* Write tap2_even */
    EFUN(wr_hwtune_ovr_write_val(tap_eo));
    EFUN(wr_hwtune_ovr_write_en(1));
    EFUN(wr_hwtune_ovr_write_sel(4));
    EFUN(wr_hwtune_ovr_write_val(SRDS_ABS(val)-tap_eo));/* Write the common tap */
    EFUN(wr_hwtune_ovr_write_en(1));
    EFUN(wr_hwtune_ovr_write_sel(10));                  /* Write tap2_even_sign */
    EFUN(wr_hwtune_ovr_write_val(val<0?1:0));
    EFUN(wr_hwtune_ovr_write_en(1));
    EFUN(wr_hwtune_ovr_write_sel(11));                  /* Write tap2_odd_sign */
    EFUN(wr_hwtune_ovr_write_val(val<0?1:0));
    EFUN(wr_hwtune_ovr_write_en(1));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_dfe2(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = rd_dfe_2_se() ? -(rd_dfe_2_e()+rd_dfe_2_cmn()) : (rd_dfe_2_e()+rd_dfe_2_cmn()));
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_set_rx_dfe3(srds_access_t *sa__, int8_t val) {
    if ((val > 31) || (val < -31)) {
      return (_error(ERR_CODE_DFE3_INVALID));  
    }

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */

    EFUN(wr_hwtune_ovr_write_sel(7));                   /* Write tap3 */
    EFUN(wr_hwtune_ovr_write_val(val));
    EFUN(wr_hwtune_ovr_write_en(1));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_dfe3(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = rd_dfe_3_cmn());
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_set_rx_dfe4(srds_access_t *sa__, int8_t val) {
    if ((val > 15) || (val < -15)) {
      return (_error(ERR_CODE_DFE4_INVALID));  
    }

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */
    
    EFUN(wr_hwtune_ovr_write_sel(8));                   /* Write tap4 */
    EFUN(wr_hwtune_ovr_write_val(val));
    EFUN(wr_hwtune_ovr_write_en(1));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_dfe4(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = rd_dfe_4_cmn());
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_set_rx_dfe5(srds_access_t *sa__, int8_t val) {
    if ((val > 15) || (val < -15)) {
      return (_error(ERR_CODE_DFE5_INVALID));  
    }

    EFUN(merlin16_INTERNAL_check_uc_lane_stopped(sa__));  /* make sure uC is stopped to avoid race conditions */
    
    EFUN(wr_hwtune_ovr_write_sel(9));                   /* Write tap5 */
    EFUN(wr_hwtune_ovr_write_val(val));
    EFUN(wr_hwtune_ovr_write_en(1));
    return(ERR_CODE_NONE); 
}

err_code_t merlin16_INTERNAL_get_rx_dfe5(srds_access_t *sa__, int8_t *val) {
    ESTM(*val = rd_dfe_5_cmn());
    return (ERR_CODE_NONE);
}




err_code_t merlin16_INTERNAL_get_tx_pre(srds_access_t *sa__, int8_t *val) {
  ESTM(*val = rd_cl72_txfir_pre());
  return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_get_tx_main(srds_access_t *sa__, int8_t *val) {
  ESTM(*val = rd_cl72_txfir_main());
  return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_get_tx_post1(srds_access_t *sa__, int8_t *val) {
  ESTM(*val = rd_cl72_txfir_post());
  return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_get_tx_post2(srds_access_t *sa__, int8_t *val) {
  ESTM(*val = rd_txfir_post2());
  return (ERR_CODE_NONE);
}



err_code_t merlin16_INTERNAL_core_clkgate(srds_access_t *sa__, uint8_t enable) {

  if (enable) {
  }
  else {
  }
  return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_lane_clkgate(srds_access_t *sa__, uint8_t enable) {

  if (enable) {
    /* Use frc/frc_val to force all RX and TX clk_vld signals to 0 */
        EFUN(wr_pmd_rx_clk_vld_frc_val(0x0));
        EFUN(wr_pmd_rx_clk_vld_frc(0x1));
        EFUN(wr_pmd_tx_clk_vld_frc_val(0x0));
        EFUN(wr_pmd_tx_clk_vld_frc(0x1));

    /* Use frc_on to force clkgate */
        EFUN(wr_ln_rx_s_clkgate_frc_on(0x1));

        EFUN(wr_ln_tx_s_clkgate_frc_on(0x1));

  }
  else {
        EFUN(wr_pmd_rx_clk_vld_frc_val(0x0));
        EFUN(wr_pmd_rx_clk_vld_frc(0x0));
        EFUN(wr_pmd_tx_clk_vld_frc_val(0x0));
        EFUN(wr_pmd_tx_clk_vld_frc(0x1));

        EFUN(wr_ln_rx_s_clkgate_frc_on(0x0));

        EFUN(wr_ln_tx_s_clkgate_frc_on(0x0));
  }
  return (ERR_CODE_NONE);
}

uint16_t merlin16_INTERNAL_eye_to_mUI(srds_access_t *sa__, uint8_t var) 
{   
    /* var is in units of 1/512 th UI, so need to multiply by 1000/512 */
    return ((uint16_t)var)*125/64;
}


uint16_t merlin16_INTERNAL_eye_to_mV(srds_access_t *sa__, uint8_t var, uint8_t ladder_range) 
{       
    /* find nearest two vertical levels */
    uint16_t vl = merlin16_INTERNAL_ladder_setting_to_mV(sa__, var/8, ladder_range);    
    uint16_t vu = merlin16_INTERNAL_ladder_setting_to_mV(sa__, SRDS_MIN(31,var/8+1), ladder_range);
    return (vl + (vu-vl)*(var&7)/8);
}

err_code_t merlin16_INTERNAL_get_osr_mode(srds_access_t *sa__, merlin16_osr_mode_st *imode) {
    merlin16_osr_mode_st mode;

     ENULL_MEMSET(&mode, 0, sizeof(merlin16_osr_mode_st));

    if(!imode) 
        return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  ESTM(mode.tx = rd_tx_osr_mode());
  ESTM(mode.rx = rd_rx_osr_mode());
  mode.tx_rx = 255;
  *imode = mode;
  return (ERR_CODE_NONE);

}

err_code_t merlin16_INTERNAL_pmd_lock_status(srds_access_t *sa__, uint8_t *pmd_lock, uint8_t *pmd_lock_chg) {
    uint16_t rddata;
#if defined(reg_rd_TLB_RX_RXDBG_PMD_RX_LOCK_STATUS)
    ESTM(rddata = reg_rd_TLB_RX_RXDBG_PMD_RX_LOCK_STATUS());
    ESTM(*pmd_lock = ex_TLB_RX_RXDBG_PMD_RX_LOCK_STATUS__dbg_pmd_rx_lock(rddata));
    ESTM(*pmd_lock_chg = ex_TLB_RX_RXDBG_PMD_RX_LOCK_STATUS__dbg_pmd_rx_lock_change(rddata));
#else
    ESTM(rddata = reg_rd_TLB_RX_PMD_RX_LOCK_STATUS());
    ESTM(*pmd_lock = ex_TLB_RX_PMD_RX_LOCK_STATUS__pmd_rx_lock(rddata));
    ESTM(*pmd_lock_chg = ex_TLB_RX_PMD_RX_LOCK_STATUS__pmd_rx_lock_change(rddata));
#endif
    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_sigdet_status(srds_access_t *sa__, uint8_t *sig_det, uint8_t *sig_det_chg) {
    uint16_t rddata;

    ESTM(rddata = reg_rd_SIGDET_SDSTATUS_0());
    ESTM(*sig_det = ex_SIGDET_SDSTATUS_0__signal_detect(rddata));
    ESTM(*sig_det_chg = ex_SIGDET_SDSTATUS_0__signal_detect_change(rddata));
    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_pll_lock_status(srds_access_t *sa__, uint8_t *pll_lock, uint8_t *pll_lock_chg) {

    uint16_t rddata;
    uint8_t pll_fail;
    ESTM(rddata = reg_rdc_PLL_CAL_COM_STS_0());
    ESTM(*pll_lock = exc_PLL_CAL_COM_STS_0__pll_lock(rddata));
    ESTM(pll_fail = exc_PLL_CAL_COM_STS_0__pll_fail_stky(rddata));
    ESTM(*pll_lock_chg = ((*pll_lock ^ !exc_PLL_CAL_COM_STS_0__pll_lock_bar_stky(rddata)) | (*pll_lock ^ !pll_fail)));
    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_read_lane_state(srds_access_t *sa__, merlin16_lane_state_st *istate) {  
       
  merlin16_lane_state_st state;
  uint8_t ladder_range = 0;

  ENULL_MEMSET(&state, 0, sizeof(merlin16_lane_state_st));

  if(!istate) 
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  EFUN(merlin16_INTERNAL_pmd_lock_status(sa__,&state.rx_lock, &state.rx_lock_chg));

  if (state.rx_lock == 1) {
      ESTM(state.stop_state = rdv_usr_sts_micro_stopped());
      if (!state.stop_state) {
        EFUN(merlin16_stop_rx_adaptation(sa__, 1));
    }
  } else {
      EFUN(merlin16_pmd_uc_control(sa__, CMD_UC_CTRL_STOP_IMMEDIATE,200));
  }

  {  merlin16_osr_mode_st osr_mode;
  ENULL_MEMSET(&osr_mode, 0, sizeof(merlin16_osr_mode_st));
  EFUN(merlin16_INTERNAL_get_osr_mode(sa__, &osr_mode));
  state.osr_mode = osr_mode;
  }
  {
      struct merlin16_uc_lane_config_st lane_cfg;
      EFUN(merlin16_get_uc_lane_cfg(sa__, &lane_cfg));
      state.ucv_config = lane_cfg.word;
  }
  ESTM(state.reset_state    = rd_rx_lane_dp_reset_state());
  ESTM(state.tx_reset_state = rd_tx_lane_dp_reset_state());
  EFUN(merlin16_INTERNAL_sigdet_status(sa__,&state.sig_det, &state.sig_det_chg));
  ESTM(state.rx_ppm = rd_cdr_integ_reg()/84);
  ESTM(state.clk90 = rd_cnt_d_minus_m1());
  ESTM(state.clkp1 = rd_cnt_d_minus_p1());
  ESTM(state.br_pd_en = rd_br_pd_en());
  /* drop the MSB, the result is actually valid modulo 128 */
  /* Also flip the sign to account for d-m1, versus m1-d */
  state.clk90 = state.clk90<<1;
  state.clk90 = -(state.clk90>>1);
  state.clkp1 = state.clkp1<<1;
  state.clkp1 = -(state.clkp1>>1);
  
  EFUN(merlin16_INTERNAL_get_rx_pf_main(sa__, &state.pf_main));
  ESTM(state.pf_hiz = rd_pf_hiz());
  ESTM(state.pf2_ctrl = rd_pf2_lowp_ctrl());
  EFUN(merlin16_INTERNAL_get_rx_vga(sa__, &state.vga));
  ESTM(state.dc_offset = rd_dc_offset_bin());
  ESTM(ladder_range = rd_p1_thresh_sel());
  EFUN(merlin16_INTERNAL_get_p1_threshold(sa__, &state.p1_lvl_ctrl));         
  state.p1_lvl = merlin16_INTERNAL_ladder_setting_to_mV(sa__, state.p1_lvl_ctrl, ladder_range);
  state.m1_lvl = 0;
   /* ESTM(state.pf_bst = rd_ams_rx_pkbst()); KD */

  EFUN(merlin16_INTERNAL_get_rx_dfe1(sa__, &state.dfe1));  
  EFUN(merlin16_INTERNAL_get_rx_dfe2(sa__, &state.dfe2));  
  EFUN(merlin16_INTERNAL_get_rx_dfe3(sa__, &state.dfe3));  
  EFUN(merlin16_INTERNAL_get_rx_dfe4(sa__, &state.dfe4));
  EFUN(merlin16_INTERNAL_get_rx_dfe5(sa__, &state.dfe5));

  ESTM(state.dfe1_dcd = rd_dfe_1_e()-rd_dfe_1_o());
  ESTM(state.dfe2_dcd = (rd_dfe_2_se() ? -rd_dfe_2_e(): rd_dfe_2_e()) -(rd_dfe_2_so() ? -rd_dfe_2_o(): rd_dfe_2_o()));
  ESTM(state.pe = rd_p1_offset_evn_bin());
  ESTM(state.ze = rd_data_offset_evn_bin());
  ESTM(state.me = rd_m1_offset_evn_bin());
  
  ESTM(state.po = rd_p1_offset_odd_bin());
  ESTM(state.zo = rd_data_offset_odd_bin());
  ESTM(state.mo = rd_m1_offset_odd_bin());

  /* tx_ppm = register/10.84 */
  ESTM(state.tx_ppm = (int16_t)(((int32_t)(rd_tx_pi_integ2_reg()))*3125/32768));

  EFUN(merlin16_INTERNAL_get_tx_pre(sa__, &state.txfir_pre));
  EFUN(merlin16_INTERNAL_get_tx_main(sa__, &state.txfir_main));
  EFUN(merlin16_INTERNAL_get_tx_post1(sa__, &state.txfir_post1));
  EFUN(merlin16_INTERNAL_get_tx_post2(sa__, &state.txfir_post2)); 
  ESTM(state.heye_left = merlin16_INTERNAL_eye_to_mUI(sa__, rdv_usr_sts_heye_left()));
  ESTM(state.heye_right = merlin16_INTERNAL_eye_to_mUI(sa__, rdv_usr_sts_heye_right()));
  ESTM(state.veye_upper = merlin16_INTERNAL_eye_to_mV(sa__, rdv_usr_sts_veye_upper(), ladder_range));
  ESTM(state.veye_lower = merlin16_INTERNAL_eye_to_mV(sa__, rdv_usr_sts_veye_lower(), ladder_range));
  ESTM(state.soc_pos = rd_ams_rx_sigdet_offset_correction_pos());
  ESTM(state.soc_neg = rd_ams_rx_sigdet_offset_correction_neg());
  ESTM(state.link_time = (((uint32_t)rdv_usr_sts_link_time())*8)/10);     /* convert from 80us to 0.1 ms units */

  if (state.rx_lock == 1) {
      if (!state.stop_state) {
    EFUN(merlin16_stop_rx_adaptation(sa__, 0));
  } 
  } else {
      EFUN(merlin16_stop_rx_adaptation(sa__, 0));
  }

  *istate = state;
  return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_display_lane_state_no_newline(srds_access_t *sa__) {     
  uint16_t lane_idx;
  merlin16_lane_state_st state;

  const char* e2s_osr_mode_enum[10] = {
    "x1   ",
    "x2   ",
    "x3   ",
    "x3.3 ",
    "x4   ",
    "x5   ",
    "x7.5 ",
    "x8   ",
    "x8.25",
    "x10  "
  };
  const char* e2s_tx_osr_mode_enum[10] = {
    "x1",
    "x2",
    "x3",
    "x3",
    "x4",
    "x5",
    "x7",
    "x8",
    "x9",
    "xA"
  };

  char *s;

  ENULL_MEMSET(&state, 0, sizeof(merlin16_lane_state_st));

  EFUN(merlin16_INTERNAL_read_lane_state(sa__, &state));
 
  lane_idx = merlin16_get_lane(sa__); 
  EFUN_PRINTF(("%2d ", lane_idx));
  if(state.osr_mode.tx_rx != 255) {
      s = (char *)e2s_osr_mode_enum[state.osr_mode.tx_rx];
      EFUN_PRINTF(("(%2s%s, 0x%02x,", (state.br_pd_en) ? "BR" : "OS", s, state.ucv_config & 0xFF));  /* Show lower byte only */
  } else {
      char *r;
      s = (char *)e2s_tx_osr_mode_enum[state.osr_mode.tx];
      r = (char *)e2s_tx_osr_mode_enum[state.osr_mode.rx];
      EFUN_PRINTF(("(%2s%s:%s, 0x%02x,", (state.br_pd_en) ? "BR" : "OS", s,r, state.ucv_config & 0xFF));  /* Show lower byte only */
  }
  EFUN_PRINTF(("     %01x,%01x,    %01x)",state.tx_reset_state,state.reset_state,state.stop_state));
  EFUN_PRINTF(("  %1d%s", state.sig_det, state.sig_det_chg ? "*" : " "));
  EFUN_PRINTF(("  %1d%s", state.rx_lock, state.rx_lock_chg ? "*" : " "));
  EFUN_PRINTF(("%4d ", state.rx_ppm));
  EFUN_PRINTF(("  %3d   %3d ", state.clk90, state.clkp1));
  EFUN_PRINTF(("   %2d,%1d ", state.pf_main, state.pf2_ctrl));
  EFUN_PRINTF(("  %2d ", state.vga));
  EFUN_PRINTF(("%3d ", state.dc_offset));
  EFUN_PRINTF(("%3d ", state.p1_lvl));
  EFUN_PRINTF(("%3d,%3d,%3d,%3d,%3d,%3d,%3d ", state.dfe1, state.dfe2, state.dfe3, state.dfe4, state.dfe5, state.dfe1_dcd, state.dfe2_dcd));
  EFUN_PRINTF(("%3d,%3d,%3d,%3d,%3d,%3d  ", state.ze, state.zo, state.pe, state.po, state.me, state.mo));
  EFUN_PRINTF(("  %4d ", state.tx_ppm));
  EFUN_PRINTF(("   %2d,%2d,%2d,%2d  ", state.txfir_pre, state.txfir_main, state.txfir_post1, state.txfir_post2));
  EFUN_PRINTF((" %3d,%3d,%3d,%3d ", state.heye_left, state.heye_right, state.veye_upper, state.veye_lower));
  EFUN_PRINTF((" %2d,%2d  ", state.soc_pos, state.soc_neg));
  EFUN_PRINTF((" %4d.%01d", state.link_time/10, state.link_time%10));
  return (ERR_CODE_NONE);
}


err_code_t merlin16_INTERNAL_read_core_state(srds_access_t *sa__, merlin16_core_state_st *istate) {  
  merlin16_core_state_st state;
  struct merlin16_uc_core_config_st core_cfg;

  ENULL_MEMSET(&state, 0, sizeof(merlin16_core_state_st));
  ENULL_MEMSET(&core_cfg, 0, sizeof(struct merlin16_uc_core_config_st));

  if(!istate) 
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  EFUN(merlin16_get_uc_core_config(sa__, &core_cfg));
  /* pllpon_mux is read in case the rescal value is overriden. */
  ESTM(state.rescal = rdc_pllpon_mux());
  ESTM(state.core_reset           = rdc_core_dp_reset_state());
  ESTM(state.uc_active            = rdc_uc_active());
  ESTM(state.comclk_mhz           = rdc_heartbeat_count_1us());   
  ESTM(state.ucode_version        = rdcv_common_ucode_version()); 
  ESTM(state.ucode_minor_version  = rdcv_common_ucode_minor_version());
  ESTM(state.afe_hardware_version = rdcv_afe_hardware_version());
  ESTM(state.temp_idx             = rdcv_temp_idx());
  {  int16_t                           die_temp = 0;
     EFUN(merlin16_read_die_temperature(sa__, &die_temp));
     state.die_temp               =    die_temp;
  }
  ESTM(state.avg_tmon             = rdcv_avg_tmon_reg13bit());
  state.vco_rate_mhz            = (uint16_t)core_cfg.vco_rate_in_Mhz;
  ESTM(state.analog_vco_range     = rdc_pll_range());   
  EFUN(merlin16_INTERNAL_read_pll_div(sa__, &state.pll_div));
  EFUN(merlin16_INTERNAL_pll_lock_status(sa__, &state.pll_lock, &state.pll_lock_chg));
  ESTM(state.core_status          = rdcv_status_byte());

  *istate = state;
  return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_display_core_state_no_newline(srds_access_t *sa__) {
  merlin16_core_state_st state;
    ENULL_MEMSET(&state     , 0, sizeof(state     ));
  EFUN(merlin16_INTERNAL_read_core_state(sa__, &state));
    EFUN_PRINTF(("%02d  "              ,  merlin16_get_core(sa__)));
    EFUN_PRINTF(("  %x,%02x  "         ,  state.core_reset, state.core_status));
    EFUN_PRINTF(("    %1d     "        ,  state.pll_pwrdn));
    EFUN_PRINTF(("   %1d    "          ,  state.uc_active));
    EFUN_PRINTF((" %3d.%2dMHz"         , (state.comclk_mhz/4),((state.comclk_mhz%4)*25)));    /* comclk in Mhz = heartbeat_count_1us / 4 */
    EFUN_PRINTF(("   %4X_%02X "        ,  state.ucode_version, state.ucode_minor_version));
    EFUN_PRINTF(("    0x%02x   "       ,  state.afe_hardware_version));
    EFUN_PRINTF(("   %3dC   "          ,  state.die_temp));
    EFUN_PRINTF(("   (%02d)%3ldC "      ,  state.temp_idx, _bin_to_degC((state.avg_tmon>>3))));
    EFUN_PRINTF(("   0x%02x  "         ,  state.rescal));
    EFUN_PRINTF(("  %2d.%03dGHz "      ,  state.vco_rate_mhz/1000, state.vco_rate_mhz % 1000));
    EFUN_PRINTF(("    %03d       "     ,  state.analog_vco_range));
    EFUN(merlin16_INTERNAL_display_pll_to_divider(sa__, state.pll_div));
    EFUN_PRINTF(("     %01d%s  "       ,  state.pll_lock, state.pll_lock_chg ? "*" : " "));

    return (ERR_CODE_NONE);
  }


/* returns 000111 (7 = 8-1), for n = 3  */
#define BFMASK(n) ((1<<((n)))-1)

err_code_t merlin16_INTERNAL_update_uc_lane_config_st(struct  merlin16_uc_lane_config_st *st) {
  uint16_t in = st->word;
  st->field.lane_cfg_from_pcs = in & BFMASK(1); in >>= 1;
  st->field.an_enabled = in & BFMASK(1); in >>= 1;
  st->field.dfe_on = in & BFMASK(1); in >>= 1;
  st->field.force_brdfe_on = in & BFMASK(1); in >>= 1;
  st->field.media_type = in & BFMASK(2); in >>= 2;
  st->field.unreliable_los = in & BFMASK(1); in >>= 1;
  st->field.scrambling_dis = in & BFMASK(1); in >>= 1;  
  st->field.cl72_auto_polarity_en = in & BFMASK(1); in >>= 1;
  st->field.cl72_restart_timeout_en = in & BFMASK(1); in >>= 1;
  st->field.reserved = in & BFMASK(6); in >>= 6;
  return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_update_usr_ctrl_disable_functions_st(struct merlin16_usr_ctrl_disable_functions_st *st) {
  uint8_t in = st->byte;
  st->field.pf_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dc_adaptation = in & BFMASK(1); in >>= 1;
  st->field.vga_adaptation = in & BFMASK(1); in >>= 1;
  st->field.slicer_offset_tuning = in & BFMASK(1); in >>= 1;
  st->field.clk90_offset_adaptation = in & BFMASK(1); in >>= 1;
  st->field.p1_level_tuning = in & BFMASK(1); in >>= 1;
  st->field.eye_adaptation = in & BFMASK(1); in >>= 1;
  st->field.all_adaptation = in & BFMASK(1); in >>= 1;
  return ERR_CODE_NONE;
}


err_code_t merlin16_INTERNAL_update_usr_ctrl_disable_dfe_functions_st(struct merlin16_usr_ctrl_disable_dfe_functions_st *st) {
  uint8_t in = st->byte;
  st->field.dfe_tap1_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap2_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap3_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap4_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap5_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap1_dcd = in & BFMASK(1); in >>= 1;
  st->field.dfe_tap2_dcd = in & BFMASK(1); in >>= 1;
  return ERR_CODE_NONE;
}

/* word to fields, for display */
err_code_t merlin16_INTERNAL_update_uc_core_config_st(struct merlin16_uc_core_config_st *st) {
  uint16_t in = st->word;
  st->field.core_cfg_from_pcs = in & BFMASK(1); in >>= 1;
  st->field.vco_rate = in & BFMASK(5); in >>= 5;
  st->field.reserved1 = in & BFMASK(2); in >>= 2;
  st->field.reserved2 = in & BFMASK(8); in >>= 8;
  st->vco_rate_in_Mhz = VCO_RATE_TO_MHZ(st->field.vco_rate);
  return ERR_CODE_NONE;
}

/* fields to word, to write into uC RAM */
err_code_t merlin16_INTERNAL_update_uc_core_config_word(struct merlin16_uc_core_config_st *st) {
  uint16_t in = 0;
  in <<= 8; in |= 0/*st->field.reserved2*/ & BFMASK(8);
  in <<= 2; in |= 0/*st->field.reserved1*/ & BFMASK(2);
  in <<= 5; in |= st->field.vco_rate & BFMASK(5);
  in <<= 1; in |= st->field.core_cfg_from_pcs & BFMASK(1);
  st->word = in;
  return ERR_CODE_NONE;
}

err_code_t merlin16_INTERNAL_update_uc_lane_config_word(struct merlin16_uc_lane_config_st *st) {
  uint16_t in = 0;
  in <<= 6; in |= 0 /*st->field.reserved*/ & BFMASK(6);
  in <<= 1; in |= st->field.cl72_restart_timeout_en & BFMASK(1);
  in <<= 1; in |= st->field.cl72_auto_polarity_en & BFMASK(1);
  in <<= 1; in |= st->field.scrambling_dis & BFMASK(1);
  in <<= 1; in |= st->field.unreliable_los & BFMASK(1);
  in <<= 2; in |= st->field.media_type & BFMASK(2);
  in <<= 1; in |= st->field.force_brdfe_on & BFMASK(1);
  in <<= 1; in |= st->field.dfe_on & BFMASK(1);
  in <<= 1; in |= st->field.an_enabled & BFMASK(1);
  in <<= 1; in |= st->field.lane_cfg_from_pcs & BFMASK(1);
  st->word = in;
  return ERR_CODE_NONE;
}

err_code_t merlin16_INTERNAL_update_usr_ctrl_disable_functions_byte(struct merlin16_usr_ctrl_disable_functions_st *st) {
  uint8_t in = 0;
  in <<= 1; in |= st->field.all_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.eye_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.p1_level_tuning & BFMASK(1);
  in <<= 1; in |= st->field.clk90_offset_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.slicer_offset_tuning & BFMASK(1);
  in <<= 1; in |= st->field.vga_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dc_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.pf_adaptation & BFMASK(1);
  st->byte = in;
  return ERR_CODE_NONE;
}


err_code_t merlin16_INTERNAL_update_usr_ctrl_disable_dfe_functions_byte(struct  merlin16_usr_ctrl_disable_dfe_functions_st *st) {
  uint8_t in = 0;
  in <<= 1; in |= st->field.dfe_tap2_dcd & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap1_dcd & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap5_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap4_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap3_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap2_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap1_adaptation & BFMASK(1);
  st->byte = in;
  return ERR_CODE_NONE;
}

#undef BFMASK

err_code_t merlin16_INTERNAL_trnsum_clear_and_enable(srds_access_t *sa__) {
    /*the trnsum accumulator is cleared on the rising edge of trnsum_en signal */
    /*this function creates a rising edge on the trnsum_en signal */
    EFUN(wr_trnsum_en(0));
    EFUN(wr_trnsum_en(1));
    EFUN(wr_uc_trnsum_en(1));
    return ERR_CODE_NONE;
}

err_code_t merlin16_INTERNAL_check_uc_lane_stopped(srds_access_t *sa__) {

  uint8_t is_micro_stopped;
  ESTM(is_micro_stopped = rdv_usr_sts_micro_stopped());
  if (!is_micro_stopped) {
      return(_error(ERR_CODE_UC_NOT_STOPPED));
  } else {
      return(ERR_CODE_NONE);
  }
}

err_code_t merlin16_INTERNAL_calc_patt_gen_mode_sel(uint8_t *mode_sel, uint8_t *zero_pad_len, uint8_t patt_length) {

  if(!mode_sel) {
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }
  if(!zero_pad_len) {
    return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }

  /* Select the correct Pattern generator Mode depending on Pattern length */
  if (!(140 % patt_length)) {
    *mode_sel = 6;
    *zero_pad_len = 100;
  }
  else if (!(160 % patt_length)) {
    *mode_sel = 5;
    *zero_pad_len = 80;
  }
  else if (!(180 % patt_length)) {
    *mode_sel = 4;
    *zero_pad_len = 60;
  }
  else if (!(200 % patt_length)) {
    *mode_sel = 3;
    *zero_pad_len = 40;
  }
  else if (!(220 % patt_length)) {
    *mode_sel = 2;
    *zero_pad_len = 20;
  }
  else if (!(240 % patt_length)) {
    *mode_sel = 1;
    *zero_pad_len = 0;
  } else {
    EFUN_PRINTF(("ERROR: Unsupported Pattern Length\n"));
    return (_error(ERR_CODE_CFG_PATT_INVALID_PATT_LENGTH));
  }
  return(ERR_CODE_NONE);
}

/*-----------------------------------------*/
/*  Write Core Config variables to uC RAM  */
/*-----------------------------------------*/

err_code_t merlin16_INTERNAL_set_uc_core_config(srds_access_t *sa__, struct merlin16_uc_core_config_st struct_val) {
    {   uint8_t reset_state;
        ESTM(reset_state = rdc_core_dp_reset_state());
        if(reset_state < 7) {
            EFUN_PRINTF(("ERROR: merlin16_INTERNAL_set_uc_core_config(..) called without core_dp_s_rstb=0\n"));
            return (_error(ERR_CODE_CORE_DP_NOT_RESET));
        }
    }
    if(struct_val.vco_rate_in_Mhz > 0) {
        struct_val.field.vco_rate = MHZ_TO_VCO_RATE(struct_val.vco_rate_in_Mhz);
    }
    EFUN(merlin16_INTERNAL_update_uc_core_config_word(&struct_val));
        EFUN(wrcv_config_word(struct_val.word));
    return (ERR_CODE_NONE);
}

/*---------------------*/
/*  PLL Configuration  */
/*---------------------*/

/** Extract the refclk frequency in Hz, based on a merlin16_pll_refclk_enum value. */
static err_code_t _merlin16_get_refclk_in_hz(enum merlin16_pll_refclk_enum refclk, uint32_t *refclk_in_hz) {
    switch (refclk) {
        case MERLIN16_PLL_REFCLK_50MHZ:          *refclk_in_hz =  50000000; break;
        case MERLIN16_PLL_REFCLK_125MHZ:         *refclk_in_hz = 125000000; break;
        case MERLIN16_PLL_REFCLK_156P25MHZ:      *refclk_in_hz = 156250000; break;
        case MERLIN16_PLL_REFCLK_161P1328125MHZ: *refclk_in_hz = 161132813; break;
        default:
            EFUN_PRINTF(("ERROR: Unknown refclk frequency:  0x%08X\n", (uint32_t)refclk));
            *refclk_in_hz = 0xFFFFFFFF;
            return (_error(ERR_CODE_REFCLK_FREQUENCY_INVALID));
    }
    return (ERR_CODE_NONE);
}

/** Identify the ratio:
 *
 *     (numerator / denominator) = (1000 / divisor)
 *
 * such that this has as little rounding error as possible:
 *
 *     refclk_freq_hz = numerator * round(vco_freq_khz / denominator)
 *
 * This will yield the most accurate refclk_freq_hz.
 * Common values of vco_freq_khz are considered in this.
 */
static err_code_t _merlin16_get_divisor_ratio(enum merlin16_pll_div_enum div, uint16_t *numerator, uint16_t *denominator)
{
    switch (div) {
        case MERLIN16_PLL_DIV_52P751515:  *denominator = 162;  *numerator = 3071; break;
        case MERLIN16_PLL_DIV_58P181818:  *denominator =  16;  *numerator =  275; break;
        case MERLIN16_PLL_DIV_62P060606:  *denominator =  53;  *numerator =  854; break;
        case MERLIN16_PLL_DIV_67P878788:  *denominator =  56;  *numerator =  825; break;
        case MERLIN16_PLL_DIV_66P460703:  *denominator =  43;  *numerator =  647; break;
        case MERLIN16_PLL_DIV_66P743079:  *denominator =  58;  *numerator =  869; break;
        case MERLIN16_PLL_DIV_68P570764:  *denominator =  84;  *numerator = 1225; break;
        case MERLIN16_PLL_DIV_68P856242:  *denominator =  65;  *numerator =  944; break;
        case MERLIN16_PLL_DIV_69P152458:  *denominator = 102;  *numerator = 1475; break;
        case MERLIN16_PLL_DIV_69P389964:  *denominator = 124;  *numerator = 1787; break;
        case MERLIN16_PLL_DIV_69P818182:  *denominator =  96;  *numerator = 1375; break;
        case MERLIN16_PLL_DIV_71P112952:  *denominator = 177;  *numerator = 2489; break;
        case MERLIN16_PLL_DIV_54P4:       *denominator =  34;  *numerator =  625; break;
        case MERLIN16_PLL_DIV_60:         *denominator =   3;  *numerator =   50; break;
        case MERLIN16_PLL_DIV_64:         *denominator =   8;  *numerator =  125; break;
        case MERLIN16_PLL_DIV_66:         *denominator =  33;  *numerator =  500; break;
        case MERLIN16_PLL_DIV_68:         *denominator =  17;  *numerator =  250; break;
        case MERLIN16_PLL_DIV_68P537598:  *denominator = 127;  *numerator = 1853; break;
        case MERLIN16_PLL_DIV_68P828796:  *denominator = 104;  *numerator = 1511; break;
        case MERLIN16_PLL_DIV_70:         *denominator =   7;  *numerator =  100; break;
        case MERLIN16_PLL_DIV_70P713596:  *denominator = 106;  *numerator = 1499; break;
        case MERLIN16_PLL_DIV_71P008:     *denominator = 205;  *numerator = 2887; break;
        case MERLIN16_PLL_DIV_71P31347:   *denominator = 177;  *numerator = 2482; break;
        case MERLIN16_PLL_DIV_71P5584:    *denominator = 118;  *numerator = 1649; break;
        case MERLIN16_PLL_DIV_72:         *denominator =   9;  *numerator =  125; break;
        case MERLIN16_PLL_DIV_73P335232:  *denominator = 250;  *numerator = 3409; break;
        case MERLIN16_PLL_DIV_73P6:       *denominator =  46;  *numerator =  625; break;
        case MERLIN16_PLL_DIV_75:         *denominator =   3;  *numerator =   40; break;
        case MERLIN16_PLL_DIV_80:         *denominator =   2;  *numerator =   25; break;
        case MERLIN16_PLL_DIV_82P5:       *denominator =  33;  *numerator =  400; break;
        case MERLIN16_PLL_DIV_85P671997:  *denominator =  58;  *numerator =  677; break;
        case MERLIN16_PLL_DIV_86P036:     *denominator = 130;  *numerator = 1511; break;
        case MERLIN16_PLL_DIV_87P5:       *denominator =   7;  *numerator =   80; break;
        case MERLIN16_PLL_DIV_88P392:     *denominator =  83;  *numerator =  939; break;
        case MERLIN16_PLL_DIV_88P76:      *denominator = 199;  *numerator = 2242; break;
        case MERLIN16_PLL_DIV_89P141838:  *denominator = 243;  *numerator = 2726; break;
        case MERLIN16_PLL_DIV_89P447998:  *denominator = 128;  *numerator = 1431; break;
        case MERLIN16_PLL_DIV_90:         *denominator =   9;  *numerator =  100; break;
        case MERLIN16_PLL_DIV_91P669037:  *denominator = 296;  *numerator = 3229; break;
        case MERLIN16_PLL_DIV_92:         *denominator =  23;  *numerator =  250; break;
        case MERLIN16_PLL_DIV_100:        *denominator =   1;  *numerator =   10; break;
        case MERLIN16_PLL_DIV_170:        *denominator =  17;  *numerator =  100; break;
        case MERLIN16_PLL_DIV_187P5:      *denominator =   3;  *numerator =   16; break;
        case MERLIN16_PLL_DIV_200:        *denominator =   1;  *numerator =    5; break;
        case MERLIN16_PLL_DIV_206P25:     *denominator =  33;  *numerator =  160; break;
        default:
            EFUN_PRINTF(("ERROR: Unknown divider value:  0x%08X\n", (uint32_t)div));
            *numerator = 0;
            *denominator = 0;
            return (_error(ERR_CODE_PLL_DIV_INVALID));
    }
    return (ERR_CODE_NONE);
}

/** Get the Refclk frequency in Hz, based on the merlin16_pll_div_enum value and VCO frequency. */
static err_code_t _merlin16_get_refclk_from_div_vco(uint32_t *refclk_freq_hz, enum merlin16_pll_div_enum div, uint32_t vco_freq_khz, uint8_t refclk_doubler_en) {
    uint16_t numerator, denominator;
    EFUN(_merlin16_get_divisor_ratio(div, &numerator, &denominator));
    *refclk_freq_hz = ((vco_freq_khz+(denominator>>1)) / denominator) * numerator;
    if (refclk_doubler_en > 0) *refclk_freq_hz /= 2;
    return (ERR_CODE_NONE);
}

/** Get the VCO frequency in kHz, based on the reference clock frequency and merlin16_pll_div_enum value. */
static err_code_t _merlin16_get_vco_from_refclk_div(uint32_t refclk_freq_hz, enum merlin16_pll_div_enum div, uint32_t *vco_freq_khz,  uint8_t refclk_doubler_en) {
    uint16_t numerator, denominator;
    uint32_t temp_refclk_freq_hz = (refclk_doubler_en > 0) ? (2 * refclk_freq_hz) : refclk_freq_hz;
    EFUN(_merlin16_get_divisor_ratio(div, &numerator, &denominator));
    *vco_freq_khz = ((temp_refclk_freq_hz + (numerator>>1)) / numerator) * denominator;
    return (ERR_CODE_NONE);
}

/* Boundaries for allowed VCO frequency */
#    define SERDES_VCO_FREQ_KHZ_MIN ( 8500000)
#    define SERDES_VCO_FREQ_KHZ_MAX (12500000)

/* Allowed tolerance in resultant VCO frequency when auto-determining divider value */
#    define SERDES_VCO_FREQ_KHZ_TOLERANCE (4000)

/* The allowed PLL divider values */
static const enum merlin16_pll_div_enum _merlin16_div_candidates[] = {
    MERLIN16_PLL_DIV_52P751515,
    MERLIN16_PLL_DIV_54P4,
    MERLIN16_PLL_DIV_58P181818,
    MERLIN16_PLL_DIV_60,
    MERLIN16_PLL_DIV_62P060606,
    MERLIN16_PLL_DIV_64,
    MERLIN16_PLL_DIV_66,
    MERLIN16_PLL_DIV_66P460703,
    MERLIN16_PLL_DIV_66P743079,
    MERLIN16_PLL_DIV_67P878788,
    MERLIN16_PLL_DIV_68,
    MERLIN16_PLL_DIV_68P537598,
    MERLIN16_PLL_DIV_68P570764,
    MERLIN16_PLL_DIV_68P828796,
    MERLIN16_PLL_DIV_68P856242,
    MERLIN16_PLL_DIV_69P152458,
    MERLIN16_PLL_DIV_69P389964,
    MERLIN16_PLL_DIV_69P818182,
    MERLIN16_PLL_DIV_70,
    MERLIN16_PLL_DIV_70P713596,
    MERLIN16_PLL_DIV_71P008,
    MERLIN16_PLL_DIV_71P112952,
    MERLIN16_PLL_DIV_71P31347,
    MERLIN16_PLL_DIV_71P5584,
    MERLIN16_PLL_DIV_72,
    MERLIN16_PLL_DIV_73P335232,
    MERLIN16_PLL_DIV_73P6,
    MERLIN16_PLL_DIV_75,
    MERLIN16_PLL_DIV_80,
    MERLIN16_PLL_DIV_82P5,
    MERLIN16_PLL_DIV_85P671997,
    MERLIN16_PLL_DIV_86P036,
    MERLIN16_PLL_DIV_87P5,
    MERLIN16_PLL_DIV_88P392,
    MERLIN16_PLL_DIV_88P76,
    MERLIN16_PLL_DIV_89P141838,
    MERLIN16_PLL_DIV_89P447998,
    MERLIN16_PLL_DIV_90,
    MERLIN16_PLL_DIV_91P669037,
    MERLIN16_PLL_DIV_92,
    MERLIN16_PLL_DIV_100,
    MERLIN16_PLL_DIV_170,
    MERLIN16_PLL_DIV_187P5,
    MERLIN16_PLL_DIV_200,
    MERLIN16_PLL_DIV_206P25
};

static const uint8_t _merlin16_div_candidates_count = sizeof(_merlin16_div_candidates) / sizeof(_merlin16_div_candidates[0]);

static err_code_t _merlin16_check_div(enum merlin16_pll_div_enum div) {
    uint8_t i, found = 0;
    for (i=0; i<_merlin16_div_candidates_count; i++) {
        found |= div == _merlin16_div_candidates[i];
    }
    if (!found) {
        EFUN_PRINTF(("ERROR: Invalid divider value:  0x%08X\n", (uint32_t)div));
        return (_error(ERR_CODE_PLL_DIV_INVALID));
    }
    return (ERR_CODE_NONE);
}
    
static err_code_t _merlin16_check_vco_freq_khz(uint32_t vco_freq_khz) {
    if (vco_freq_khz < SERDES_VCO_FREQ_KHZ_MIN - SERDES_VCO_FREQ_KHZ_TOLERANCE) {
        EFUN_PRINTF(("ERROR: VCO frequency too low:  %d kHz is lower than minimum (%d kHz)\n", vco_freq_khz, SERDES_VCO_FREQ_KHZ_MIN));
        return (_error(ERR_CODE_VCO_FREQUENCY_INVALID));
    }
    if (vco_freq_khz > SERDES_VCO_FREQ_KHZ_MAX + SERDES_VCO_FREQ_KHZ_TOLERANCE) {
        EFUN_PRINTF(("ERROR: VCO frequency too high:  %d kHz is higher than maximum (%d kHz)\n", vco_freq_khz, SERDES_VCO_FREQ_KHZ_MAX));
        return (_error(ERR_CODE_VCO_FREQUENCY_INVALID));
    }
    return (ERR_CODE_NONE);
}
    
/** Find the entry out of _merlin16_div_candidates that is closest to matching refclk_freq_hz and vco_freq_khz. */
static err_code_t _merlin16_get_div(uint32_t refclk_freq_hz, uint32_t vco_freq_khz, enum merlin16_pll_div_enum *div, uint8_t refclk_doubler_en) {
    int32_t vco_freq_khz_min_error = 0x7FFFFFFF;
    uint8_t i;
    for (i=0; i<_merlin16_div_candidates_count; i++) {
        uint32_t actual_vco_freq_khz = 0;
        int32_t  vco_freq_khz_error;
        EFUN(_merlin16_get_vco_from_refclk_div(refclk_freq_hz, _merlin16_div_candidates[i], &actual_vco_freq_khz, refclk_doubler_en));
        vco_freq_khz_error = vco_freq_khz - actual_vco_freq_khz;
        vco_freq_khz_error = SRDS_ABS(vco_freq_khz_error);
        if (vco_freq_khz_min_error > vco_freq_khz_error) {
            vco_freq_khz_min_error = vco_freq_khz_error;
            *div = _merlin16_div_candidates[i];
        }
    }
    if (vco_freq_khz_min_error == 0xFFFFFFFF) {
      return (_error(ERR_CODE_CONFLICTING_PARAMETERS));
    }
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_resolve_pll_parameters(enum merlin16_pll_refclk_enum refclk,
                                                  uint32_t *refclk_freq_hz,
                                                  enum merlin16_pll_div_enum *div,
                                                  uint32_t *vco_freq_khz,
                                                  uint8_t refclk_doubler_en) {

    /* Parameter value and consistency checks */
    const uint8_t given_param_count = (((refclk == MERLIN16_PLL_REFCLK_UNKNOWN) ? 0 : 1)
                                       + ((*div == MERLIN16_PLL_DIV_UNKNOWN) ? 0 : 1)
                                       + ((*vco_freq_khz == 0) ? 0 : 1));
    const uint32_t original_vco_freq_khz = *vco_freq_khz;
    enum merlin16_pll_div_enum auto_div = 0;
        
    /* Verify that at least two of the three parameters is given. */
    if (given_param_count < 2) {
        return (_error(ERR_CODE_INSUFFICIENT_PARAMETERS));
    }

    /* The refclk value is checked in various functions below. */

    /* Verify that the requested div value is allowed. */
    if (*div != MERLIN16_PLL_DIV_UNKNOWN) {
        EFUN(_merlin16_check_div(*div));
    }

    /* Verify that the requested VCO frequency is allowed. */
    if (*vco_freq_khz != 0) {
        EFUN(_merlin16_check_vco_freq_khz(*vco_freq_khz));
    }

    if (refclk == MERLIN16_PLL_REFCLK_UNKNOWN) {
        /* Determine refclk from vco frequency and div */
        EFUN(_merlin16_get_refclk_from_div_vco(refclk_freq_hz, *div, *vco_freq_khz, refclk_doubler_en));
    } else {
        EFUN(_merlin16_get_refclk_in_hz(refclk, refclk_freq_hz));
    }

    if (*vco_freq_khz == 0) {
        /* Determine VCO frequency from refclk and divider */
        EFUN(_merlin16_get_vco_from_refclk_div(*refclk_freq_hz, *div, vco_freq_khz, refclk_doubler_en));
    }

    /* Determine divider from vco frequency and refclk.
     * This is done even if the div was provided, because if it is,
     * we still want to check whether the parameters are conflicting.
     */
    EFUN(_merlin16_get_div(*refclk_freq_hz, *vco_freq_khz, &auto_div, refclk_doubler_en));
    if (*div == MERLIN16_PLL_DIV_UNKNOWN) {
        /* Use the auto-determined divider value, since the divider was not supplied. */
        *div = auto_div;

        /* Determine resultant VCO frequency from refclk and divider */
        EFUN(_merlin16_get_vco_from_refclk_div(*refclk_freq_hz, *div, vco_freq_khz, refclk_doubler_en));
    }

    /* Verify the resultant VCO frequency */
    EFUN(_merlin16_check_vco_freq_khz(*vco_freq_khz));

    /* Verify that the requested VCO frequency is delivered. */
    if ((original_vco_freq_khz != 0)
        && ((*vco_freq_khz < original_vco_freq_khz - SERDES_VCO_FREQ_KHZ_TOLERANCE)
            || (*vco_freq_khz > original_vco_freq_khz + SERDES_VCO_FREQ_KHZ_TOLERANCE))) {
        EFUN_PRINTF(("ERROR: Could not achieve requested VCO frequency of %d kHz.\n       Refclk is %d Hz, refclk doubler is %s, and auto-determined divider is 0x%08X, yielding a VCO frequency of %d kHz.\n",
                     original_vco_freq_khz, *refclk_freq_hz, (refclk_doubler_en > 0)?"enabled":"disabled", *div, *vco_freq_khz));
        return (_error(ERR_CODE_VCO_FREQUENCY_INVALID));
    }
    
    /* Verify the auto-determined divider value. */
    if (auto_div != *div) {
        EFUN_PRINTF(("ERROR: Conflicting PLL parameters:  refclk is %d Hz, refclk doubler is %s, divider is 0x%08X, and VCO frequency is %d kHz.\n       Expected divider is 0x%08X\n",
                     *refclk_freq_hz, (refclk_doubler_en > 0)?"enabled":"disabled", *div, *vco_freq_khz, auto_div));
        return (_error(ERR_CODE_CONFLICTING_PARAMETERS));
    }

    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_display_pll_to_divider(srds_access_t *sa__, uint32_t div) {
    /* Adjust these to increase or decrease the number of digits to the right
     * of the decimal point.
     */
    const uint32_t ten_to_the_decimal_digits = 10000; /* 10**decimal_digits */

    /* Not a const, because of carry logic below. */
    uint16_t div_integer = SRDS_INTERNAL_GET_PLL_DIV_INTEGER(div);

    if (SRDS_INTERNAL_IS_PLL_DIV_FRACTIONAL(div)) {
        /* fraction_num will have this many bits (ending at bit 0). */
        const uint8_t  fraction_num_width = 28;
        const uint32_t fraction_num = SRDS_INTERNAL_GET_PLL_DIV_FRACTION_NUM(div, fraction_num_width);

        /* Identify the number that, when printed with left-padded zeros,
         * becomes the digits to the right of the decimal point.
         *
         * This value can be obtained by dividing fraction_num by:
         *
         *     (2^fraction_num_width)/(10^decimal_digits)
         */
        const uint32_t divisor = (((1 << fraction_num_width) + (ten_to_the_decimal_digits>>1))
                                  / ten_to_the_decimal_digits);
        uint32_t fraction_as_int = (fraction_num + (divisor>>1)) / divisor;

        /* In case the rounding above caused the fractional portion to overflow
         * (e.g. 4.9999999999 becomes 5.0000), implement the carry into the
         * integer portion.
         */
        if (fraction_as_int >= ten_to_the_decimal_digits) {
            fraction_as_int -= ten_to_the_decimal_digits;
            ++div_integer;
        }

    }
    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_print_uc_dsc_error(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd) {
    uint32_t supp_info;

    ESTM(supp_info = (rd_uc_dsc_supp_info()) & 0xFF);
    switch (SUPP_INFO_TO_ERROR_CODE(supp_info)) {
      case UC_CMD_ERROR_INVALID_COMMAND:
        ESTM_PRINTF(("ERROR : UC reported invalid command %d.  (other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      case UC_CMD_ERROR_BUSY:
        ESTM_PRINTF(("ERROR : UC reported busy for command %d.  (other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      case UC_CMD_ERROR_GET_EYE_SAMPLE_ERROR:
        ESTM_PRINTF(("ERROR : UC reported error in getting eye sample.  (command %d, other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      case UC_CMD_ERROR_PRBS_NOT_LOCKED:
        ESTM_PRINTF(("ERROR : UC reported PRBS not locked.  (command %d, other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      case UC_CMD_ERROR_COMMAND_IN_PROGRESS:
        ESTM_PRINTF(("ERROR : UC reported command already in progress.  (command %d, other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      case UC_CMD_ERROR_INVALID_MODE:
        ESTM_PRINTF(("ERROR : UC reported invalid mode for command %d.  (other_info = 0x%X)\n",
                     cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
        break;
      default:
        ESTM_PRINTF(("ERROR : UC reported unknown error 0x%X for command %d.  (other_info = 0x%X)\n",
                     SUPP_INFO_TO_ERROR_CODE(supp_info), cmd, SUPP_INFO_TO_OTHER_INFO(supp_info)));
    }
    return(ERR_CODE_NONE);
}
 
/******************************************/
/*  Serdes Register field Poll functions  */
/******************************************/

#ifndef CUSTOM_REG_POLLING

/* poll for microcontroller to populate the dsc_data register */
err_code_t merlin16_INTERNAL_poll_diag_done(srds_access_t *sa__, uint16_t *status, uint32_t timeout_ms) {
 uint8_t loop;

 if(!status) {
     return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }

 for(loop=0;loop < 100; loop++) {
     ESTM(*status=rdv_usr_diag_status());

     if((*status & 0x8000) > 0) {
         return(ERR_CODE_NONE);
     }
     if(loop>10) {
         USR_DELAY_US(10*timeout_ms);
     }
 }
 return(_error(ERR_CODE_DIAG_TIMEOUT));
}

/** Poll for field "uc_dsc_ready_for_cmd" = 1 [Return Val => Error_code (0 = Polling Pass)] */
err_code_t merlin16_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(srds_access_t *sa__, uint32_t timeout_ms, enum srds_pmd_uc_cmd_enum cmd) {
    /* read quickly for 10 tries */
    uint16_t loop;

    for (loop = 0; loop < 100; loop++) {
        uint16_t rddata;
        ESTM(rddata = reg_rd_DSC_A_DSC_UC_CTRL());
        if (rddata & 0x0080) {    /* bit 7 is uc_dsc_ready_for_cmd */
            if (rddata & 0x0040) {  /* bit 6 is uc_dsc_error_found   */
                EFUN(merlin16_INTERNAL_print_uc_dsc_error(sa__, cmd));
                return(_error(ERR_CODE_UC_CMD_RETURN_ERROR));
            }
            return (ERR_CODE_NONE);
        }
        if(loop>10) {
            USR_DELAY_US(10*timeout_ms);
        }
    }
  EFUN_PRINTF(("ERROR : DSC ready for command is not working, applying workaround and getting debug info !\n"));
  DISP(rd_uc_dsc_supp_info());
  DISP(rd_uc_dsc_gp_uc_req());
  DISP(rd_dsc_state());
  ESTM_PRINTF(("Uc Core Status Byte = %x\n",rdcv_status_byte()));
  DISP(rdv_usr_sts_micro_stopped());
  /* DISP(rdv_stop_graceful_trigger()); */
  /* artifically terminate the command to re-enable the command interface */
  EFUN(wr_uc_dsc_ready_for_cmd(0x1));
  return (_error(ERR_CODE_POLLING_TIMEOUT));          /* Error Code for polling timeout */
}

/* Poll for field "dsc_state" = DSC_STATE_UC_TUNE [Return Val => Error_code (0 = Polling Pass)] */
err_code_t merlin16_INTERNAL_poll_dsc_state_equals_uc_tune(srds_access_t *sa__, uint32_t timeout_ms) {
  uint16_t loop;
  uint16_t dsc_state;
  /* poll 10 times to avoid longer delays later */
  for (loop = 0; loop < 100; loop++) {
    ESTM(dsc_state = rd_dsc_state());
    if (dsc_state == DSC_STATE_UC_TUNE) {
      return (ERR_CODE_NONE);
    }
    if(loop>10) {
        USR_DELAY_US(10*timeout_ms);
    }
  }
  ESTM_PRINTF(("DSC_STATE = %d\n", rd_dsc_state()));
  return (_error(ERR_CODE_POLLING_TIMEOUT));          /* Error Code for polling timeout */
}


/* Poll for field "st_afe_tx_fifo_resetb" = 1 [Return Val => Error_code (0 = Polling Pass)] */
err_code_t merlin16_INTERNAL_poll_st_afe_tx_fifo_resetb_equals_1(srds_access_t *sa__, uint32_t timeout_ms) {
  uint16_t loop;

  for (loop = 0; loop <= 100; loop++) {
    uint8_t st_afe_tx_fifo_resetb;
    ESTM(st_afe_tx_fifo_resetb = rd_st_afe_tx_fifo_resetb());
    if (st_afe_tx_fifo_resetb) {
      return (ERR_CODE_NONE);
    }
    USR_DELAY_US(10*timeout_ms);
  }
  return (_error(ERR_CODE_POLLING_TIMEOUT));          /* Error Code for polling timeout */
}



/* Poll for field "micro_ra_initdone" = 1 [Return Val => Error_code (0 = Polling Pass)] */
err_code_t merlin16_INTERNAL_poll_micro_ra_initdone(srds_access_t *sa__, uint32_t timeout_ms) {
  uint16_t loop;

  uint8_t result;
  for (loop = 0; loop <= 100; loop++) {
    ESTM(result = rdc_micro_ra_initdone());
    if (result) {
      return (ERR_CODE_NONE);
    }
    USR_DELAY_US(10*timeout_ms);
  }
  return (_error(ERR_CODE_POLLING_TIMEOUT));          /* Error Code for polling timeout */
}

#endif /* CUSTOM_REG_POLLING */



err_code_t merlin16_INTERNAL_rdblk_callback(void *arg, uint8_t byte_count, uint16_t data) {
    merlin16_INTERNAL_rdblk_callback_arg_t * const cast_arg = (merlin16_INTERNAL_rdblk_callback_arg_t *)arg;
    *(cast_arg->mem_ptr + get_endian_offset(cast_arg->mem_ptr)) = data & 0xFF;
    cast_arg->mem_ptr++;
    if (byte_count == 2) {
        *(cast_arg->mem_ptr + get_endian_offset(cast_arg->mem_ptr)) = data >> 8;
        cast_arg->mem_ptr++;
    }
    return (ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_rdblk_uc_generic_ram(srds_access_t *sa__,
                                                uint32_t block_addr,
                                                uint16_t block_size,
                                                uint16_t start_offset,
                                                uint16_t cnt,
                                                void *arg,
                                                err_code_t (*callback)(void *, uint8_t, uint16_t)) {
    uint32_t read_val = 0;
    uint8_t defecit = 0;
    uint32_t addr = block_addr + start_offset;
    
    if (cnt == 0) {
        return (ERR_CODE_NONE);
    }

    /* Check for bad start offset and block size. */
    if (start_offset >= block_size) {
        return (ERR_CODE_BAD_PTR_OR_INVALID_INPUT);
    }

    while (cnt > 0) {
        /* Determine how many bytes to read before wrapping back to start of block. */
        uint16_t block_cnt = SRDS_MIN(cnt, block_addr + block_size - addr);
        cnt -= block_cnt;
        
        /* Set up the word reads. */
        EFUN(wrc_micro_autoinc_rdaddr_en(1));
        EFUN(wrc_micro_ra_rddatasize(0x1));                     /* Select 16bit read datasize */
        EFUN(wrc_micro_ra_rdaddr_msw(addr >> 16));              /* Upper 16bits of RAM address to be read */
        EFUN(wrc_micro_ra_rdaddr_lsw(addr & 0xFFFE));           /* Lower 16bits of RAM address to be read */

        /* Read the leading byte, if starting at an odd address. */
        if ((addr & 1) == 1) {
            ESTM(read_val |= ((rdc_micro_ra_rddata_lsw() >> 8) << defecit));
            if (defecit == 8) {
                EFUN(callback(arg, 2, (uint16_t)read_val));
                read_val = 0;
            }
            /* We just read a byte.  This toggles the defecit from 0 to 8 or from 8 to 0. */
            defecit ^= 8;
            --block_cnt;
        }

        /* Read the whole words, and call the callback with two bytes at a time. */
        while (block_cnt >= 2) {
            ESTM(read_val |= (rdc_micro_ra_rddata_lsw() << defecit));
            EFUN(callback(arg, 2, (uint16_t)read_val));
            read_val >>= 16;
            /* We just read two bytes.  This preserves whatever defecit (8 or 0) is there. */
            block_cnt -= 2;
        }

        /* Read the trailing byte, if leftover after reading whole words. */
        if (block_cnt > 0) {
            ESTM(read_val |= ((rdc_micro_ra_rddata_lsw() & 0xFF) << defecit));
            if (defecit == 8) {
                EFUN(callback(arg, 2, (uint16_t)read_val));
                read_val = 0;
            }
            /* We just read a byte.  This toggles the defecit from 0 to 8 or from 8 to 0. */
            defecit ^= 8;
        }

        addr = block_addr;
    }

    /* If a final byte is left behind, then call the callback with it. */
    if (defecit > 0) {
        EFUN(callback(arg, 1, (uint16_t)read_val));
    }

    return(ERR_CODE_NONE);
}

err_code_t merlin16_INTERNAL_rdblk_uc_generic_ram_descending(srds_access_t *sa__,
                                                           uint32_t block_addr,
                                                           uint16_t block_size,
                                                           uint16_t start_offset,
                                                           uint16_t cnt,
                                                           void *arg,
                                                           err_code_t (*callback)(void *, uint8_t, uint16_t)) {
    uint32_t read_val = 0;
    uint8_t defecit = 0;
    uint32_t addr = block_addr + start_offset;
    uint16_t configured_addr_msw = (addr >> 16) + 1;
    
    if (cnt == 0) {
        return (ERR_CODE_NONE);
    }

    /* Check for bad start offset and block size. */
    if (start_offset >= block_size) {
        return (ERR_CODE_BAD_PTR_OR_INVALID_INPUT);
    }

    EFUN(wrc_micro_autoinc_rdaddr_en(0));
    EFUN(wrc_micro_ra_rddatasize(0x1));                         /* Select 16bit read datasize */
    
    while (cnt > 0) {
        /* Determine how many bytes to read before wrapping back to end of block. */
        uint16_t block_cnt = SRDS_MIN(cnt, start_offset+1);
        cnt -= block_cnt;

        while (block_cnt > 0) {
            const uint16_t addr_msw = addr >> 16;
            uint16_t read_val2;
            if (addr_msw != configured_addr_msw) {
                EFUN(wrc_micro_ra_rdaddr_msw(addr_msw));        /* Upper 16bits of RAM address to be read */
                configured_addr_msw = addr_msw;
            }
            
            EFUN(wrc_micro_ra_rdaddr_lsw(addr & 0xFFFE));       /* Lower 16bits of RAM address to be read */
            ESTM(read_val2 = rdc_micro_ra_rddata_lsw());

            if (((addr & 1) == 1) && (block_cnt >= 2)) {
                /* Reading two bytes.  Since we're reading in descending address order, they
                 * will be reversed before they are sent out. */
                read_val |= ((((read_val2 & 0xFF) << 8) | (read_val2 >> 8)) << defecit);
                EFUN(callback(arg, 2, (uint16_t)read_val));
                read_val >>= 16;
                /* We just read two bytes.  This preserves whatever defecit (8 or 0) is there. */
                block_cnt -= 2;
                addr -= 2;
            } else {
                if ((addr & 1) == 1) {
                    /* Reading upper byte of word. */
                    read_val |= ((read_val2 >> 8) << defecit);
                } else {
                    /* Reading lower byte of word. */
                    read_val |= ((read_val2 & 0xFF) << defecit);
                }
                if (defecit == 8) {
                    EFUN(callback(arg, 2, (uint16_t)read_val));
                    read_val = 0;
                }
                /* We just read a byte.  This toggles the defecit from 0 to 8 or from 8 to 0. */
                defecit ^= 8;
                --block_cnt;
                --addr;
            }
        }
        
        addr = block_addr + block_size - 1;
        start_offset = block_size - 1;
    }

    /* If a final byte is left behind, then call the callback with it. */
    if (defecit > 0) {
        EFUN(callback(arg, 1, (uint16_t)read_val));
    }

    return(ERR_CODE_NONE);
}
