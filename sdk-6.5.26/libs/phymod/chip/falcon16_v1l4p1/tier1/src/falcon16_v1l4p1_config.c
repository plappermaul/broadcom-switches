/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2019 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 *                                                                                 *
 ***********************************************************************************/

/***********************************************************************************
 ***********************************************************************************
 *  File Name     :  falcon16_v1l4p1_config.c                                           *
 *  Created On    :  03 Nov 2015                                                   *
 *  Created By    :  Brent Roberts                                                 *
 *  Description   :  APIs for Serdes IPs                                           *
 *  Revision      :    *
 *                                                                                 *
 ***********************************************************************************
 ***********************************************************************************/

/** @file falcon16_v1l4p1_config.c
 * Implementation of API config functions
 */

#include <phymod/phymod.h>
#include "../include/falcon16_v1l4p1_config.h"
#include "../include/falcon16_v1l4p1_access.h"
#include "../include/falcon16_v1l4p1_common.h"
#include "../include/falcon16_v1l4p1_functions.h"
#include "../include/falcon16_v1l4p1_internal.h"
#include "../include/falcon16_v1l4p1_internal_error.h"
#include "../include/falcon16_v1l4p1_select_defns.h"
#include "../include/falcon16_v1l4p1_enum.h"
#include "../include/falcon16_v1l4p1_dependencies.h"

/* If SERDES_EVAL is defined, then is_ate_log_enabled() is queried to *\
\* know whether to log ATE.  falcon16_v1l4p1_access.h provides that function.  */

/* User specific implementation for creating syncronization object, used for critical section access in initialization routine below */
/* USR_CREATE_LOCK; */

/*******************************/
/*  Stop/Resume RX Adaptation  */
/*******************************/

err_code_t falcon16_v1l4p1_stop_rx_adaptation(srds_access_t *sa__, uint8_t enable) {

  if (enable) {
      err_code_t err_code;
      err_code = falcon16_v1l4p1_pmd_uc_control(sa__, CMD_UC_CTRL_STOP_GRACEFULLY,GRACEFUL_STOP_TIME);
      if(err_code) {
          uint8_t temp=0;
          USR_PRINTF(("Warning Graceful stop request returned error %d; Requesting a forceful stop\n",err_code));
          temp = falcon16_v1l4p1_INTERNAL_stop_micro(sa__,0,&err_code);
          /* return value from falcon16_v1l4p1_INTERNAL_stop_micro - immediate stop is not required */
          /* void casting to avoid compiler warning. */
          UNUSED(temp);
      }
      return(err_code);
  }
  else {
    return(falcon16_v1l4p1_pmd_uc_control(sa__, CMD_UC_CTRL_RESUME,50));
  }
}

err_code_t falcon16_v1l4p1_request_stop_rx_adaptation(srds_access_t *sa__) {

  return(falcon16_v1l4p1_pmd_uc_control_return_immediate(sa__, CMD_UC_CTRL_STOP_GRACEFULLY));
}

/**********************/
/*  uCode CRC Verify  */
/**********************/

err_code_t falcon16_v1l4p1_ucode_crc_verify(srds_access_t *sa__, uint16_t ucode_len,uint16_t expected_crc_value) {
    uint16_t calc_crc;
    UNUSED(sa__);
    UNUSED(ucode_len);

    EFUN(falcon16_v1l4p1_pmd_uc_cmd_with_data(sa__, CMD_CALC_CRC,0,ucode_len,200));

    ESTM(calc_crc = rd_uc_dsc_data());
    if(calc_crc != expected_crc_value) {
        EFUN_PRINTF(("Microcode CRC did not match expected=%04x : calculated=%04x\n",expected_crc_value, calc_crc));
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_UC_CRC_NOT_MATCH));
    }
    return(ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_start_ucode_crc_calc(srds_access_t *sa__, uint16_t ucode_len) {
    EFUN(falcon16_v1l4p1_pmd_uc_cmd_with_data_return_immediate(sa__, CMD_CALC_CRC, 0, ucode_len)); /* Invoke Calculate CRC command and return control immediately */
    return(ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_check_ucode_crc(srds_access_t *sa__, uint16_t expected_crc_value, uint32_t timeout_ms) {
    uint16_t calc_crc;
    err_code_t err_code;

    err_code = falcon16_v1l4p1_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(sa__, timeout_ms, CMD_CALC_CRC); /* Poll for uc_dsc_ready_for_cmd = 1 to indicate falcon16_v1l4p1 ready for command */
    if (err_code) {
      EFUN_PRINTF(("ERROR : DSC ready for command timed out. Previous uC command not finished yet\n"));
      return (err_code);
    }
    ESTM(calc_crc = rd_uc_dsc_data());
    if(calc_crc != expected_crc_value) {
        EFUN_PRINTF(("UC CRC did not match expected=%04x : calculated=%04x\n",expected_crc_value, calc_crc));
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_UC_CRC_NOT_MATCH));
    }

    return(ERR_CODE_NONE);
}


/**************************************************************/
/*  APIs to Write Lane Config and User variables into uC RAM  */
/**************************************************************/

err_code_t falcon16_v1l4p1_set_uc_lane_cfg(srds_access_t *sa__, struct falcon16_v1l4p1_uc_lane_config_st struct_val) {
  uint8_t reset_state;
  ESTM(reset_state = rd_rx_lane_dp_reset_state());
  if(reset_state < 7) {
      EFUN_PRINTF(("ERROR: falcon16_v1l4p1_set_uc_lane_cfg(..) called without ln_dp_s_rstb=0\n"));
      return (falcon16_v1l4p1_error(sa__, ERR_CODE_LANE_DP_NOT_RESET));
  }
  EFUN(falcon16_v1l4p1_INTERNAL_update_uc_lane_config_word(&struct_val));
  return(wrv_config_word(struct_val.word));
}

/******************************************************************/
/*  APIs to Read Core/Lane Config and User variables from uC RAM  */
/******************************************************************/

err_code_t falcon16_v1l4p1_get_uc_core_config(srds_access_t *sa__, struct falcon16_v1l4p1_uc_core_config_st *get_val)
{
    
    if(!get_val) {
        return falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT);
    }
        ESTM(get_val->word = rdcv_config_word());
    EFUN(falcon16_v1l4p1_INTERNAL_update_uc_core_config_st(get_val));
    return ERR_CODE_NONE;
}

err_code_t falcon16_v1l4p1_get_uc_lane_cfg(srds_access_t *sa__, struct falcon16_v1l4p1_uc_lane_config_st *get_val) {

  if(!get_val) {
     return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }
  ESTM(get_val->word = rdv_config_word());
  EFUN(falcon16_v1l4p1_INTERNAL_update_uc_lane_config_st(get_val));
  return (ERR_CODE_NONE);
}

/*--------------------------------------------*/
/*  APIs to Enable or Disable datapath reset  */
/*--------------------------------------------*/

err_code_t falcon16_v1l4p1_tx_dp_reset(srds_access_t *sa__, uint8_t enable) {
    if (enable) {
        EFUN(wr_tx_ln_dp_s_rstb(0x0));
    } else {
        EFUN(wr_tx_ln_dp_s_rstb(0x1));
        EFUN(wr_tx_uc_ack_lane_dp_reset(0x1));
        EFUN(wr_tx_uc_ack_lane_cfg_done(0x1));
    }
    return ERR_CODE_NONE;
}

err_code_t falcon16_v1l4p1_rx_dp_reset(srds_access_t *sa__, uint8_t enable) {
    if (enable) {
        EFUN(wr_rx_ln_dp_s_rstb(0x0));
    } else {
        EFUN(wr_rx_ln_dp_s_rstb(0x1));
    }
    return ERR_CODE_NONE;
}

err_code_t falcon16_v1l4p1_core_dp_reset(srds_access_t *sa__, uint8_t enable){
    if (enable) {
        EFUN(wrc_core_dp_s_rstb(0x0));
    } else {
        EFUN(wrc_core_dp_s_rstb(0x1));
    }
    return ERR_CODE_NONE;
}

/********************************/
/*  Loading ucode through PRAM  */
/********************************/
err_code_t falcon16_v1l4p1_ucode_pram_load(srds_access_t *sa__, char const * ucode_image, uint32_t ucode_len) {
    uint16_t ucode_len_remainder = (uint16_t)(((ucode_len + 3) & 0xFFFFFFFC) - ucode_len);

    if(!ucode_image) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    if (ucode_len > UCODE_MAX_SIZE) {                       /* uCode size should be less than UCODE_MAX_SIZE  */
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_INVALID_UCODE_LEN));
    }

    EFUN(wrc_micro_master_clk_en(0x1));                     /* Enable clock to microcontroller subsystem */
    EFUN(wrc_micro_master_rstb(0x1));                       /* De-assert reset to microcontroller sybsystem */
    EFUN(wrc_micro_master_rstb(0x0));                       /* Assert reset to microcontroller sybsystem - Toggling reset*/
    EFUN(wrc_micro_master_rstb(0x1));                       /* De-assert reset to microcontroller sybsystem */

    EFUN(wrc_micro_ra_init(0x1));                           /* Set initialization command to initialize code RAM */
    EFUN(falcon16_v1l4p1_INTERNAL_poll_micro_ra_initdone(sa__, 250));/* Poll for micro_ra_initdone = 1 to indicate initialization done */

    EFUN(wrc_micro_ra_init(0x2));                         /* Write command for data RAM initialization */
    EFUN(falcon16_v1l4p1_INTERNAL_poll_micro_ra_initdone(sa__, 250)); /* Poll status of data RAM initialization */

    EFUN(wrc_micro_ra_init(0x0));                           /* Clear initialization command */


    EFUN(wrc_micro_pramif_ahb_wraddr_msw(0x0000));
    EFUN(wrc_micro_pramif_ahb_wraddr_lsw(0x0000));
    EFUN(wrc_micro_pram_if_rstb(1));
    EFUN(wrc_micro_pramif_en(1));

    /* Wait 8 pram clocks */
    EFUN(USR_DELAY_US(1));
    
    /* write ucode into pram */
    while (ucode_len > 0) {
        EFUN(falcon16_v1l4p1_pmd_wr_pram(sa__, (uint8_t)*ucode_image));
        --ucode_len;
        ++ucode_image;
    }
    
    /* Pad to 32 bits */
    while (ucode_len_remainder > 0) {
        EFUN(falcon16_v1l4p1_pmd_wr_pram(sa__, 0));
        --ucode_len_remainder;
    }
    
    /* Wait 8 pram clocks */
    EFUN(USR_DELAY_US(1));


    EFUN(wrc_micro_pramif_en(0));
    EFUN(wrc_micro_core_clk_en(1));
   /* EFUN(wrc_micro_core_rstb(1)); */
    return(ERR_CODE_NONE);
}

/************************************/
/*  Accessing core_config_from_pcs  */
/************************************/

err_code_t falcon16_v1l4p1_set_core_config_from_pcs(srds_access_t *sa__, uint8_t core_cfg_from_pcs) {
    struct falcon16_v1l4p1_uc_core_config_st core_config = {{0}};
    EFUN(falcon16_v1l4p1_get_uc_core_config(sa__, &core_config));
    core_config.field.core_cfg_from_pcs = core_cfg_from_pcs;
    EFUN(falcon16_v1l4p1_INTERNAL_set_uc_core_config(sa__, core_config));
    return (ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_set_osr_2p5_en(srds_access_t *sa__, uint8_t osr_2p5_en) {
    struct falcon16_v1l4p1_uc_core_config_st core_config = {{0}};
    EFUN(falcon16_v1l4p1_get_uc_core_config(sa__, &core_config));
    core_config.field.osr_2p5_en = osr_2p5_en;
    EFUN(falcon16_v1l4p1_INTERNAL_set_uc_core_config(sa__, core_config));
    return (ERR_CODE_NONE);
}


/******************/
/*  Lane Mapping  */
/******************/

err_code_t falcon16_v1l4p1_map_lanes(srds_access_t *sa__, uint8_t num_lanes, uint8_t const *tx_lane_map, uint8_t const *rx_lane_map) {
    uint8_t rd_val = 0;

    /* Verify that the core data path is held in reset. */
    ESTM(rd_val = rdc_core_dp_s_rstb());
    if (rd_val != 0) {
        EFUN_PRINTF(("ERROR: core data path reset is not de-asserted\n"));
        return (ERR_CODE_UC_NOT_RESET);
    }

    /* Verify that all micros are held in reset. */

    ESTM(rd_val = rdc_micro_core_rstb());
    if (rd_val != 0) {
        return (ERR_CODE_UC_NOT_RESET);
    }

    /* Verify that the num_lanes parameter is correct. */
    ESTM(rd_val = rdc_revid_multiplicity());
    if (rd_val != num_lanes) {
        return (ERR_CODE_BAD_LANE_COUNT);
    }

    /* Verify that tx_lane_map and rx_lane_map are valid. */
    {
        uint8_t index1, index2;
#ifdef ENABLE_LANE_MAP_WARNING
        uint8_t lp_message_printed = 0;
#endif /* ENABLE_LANE_MAP_WARNING */
        for (index1=0; index1<num_lanes; ++index1) {

            /* Verify that a lane map is not to an invalid lane. */
            if ((tx_lane_map[index1] >= num_lanes)
                || (rx_lane_map[index1] >= num_lanes)){
                return (ERR_CODE_BAD_LANE);
            }
#ifdef ENABLE_LANE_MAP_WARNING
            /* Warn if an RX lane mapping is not the same as TX. */
            if ((tx_lane_map[index1] != rx_lane_map[index1])
                && !lp_message_printed) {
                ESTM_PRINTF(("Warning:  In core %d, TX lane %d is mapped to %d, while RX lane %d is mapped to %d.\n          Digital and remote loopback will not operate as expected.\n          Further warnings are suppressed.\n", falcon16_v1l4p1_get_core(sa__), index1, tx_lane_map[index1], index1, rx_lane_map[index1]));
                lp_message_printed = 1;
            }
#endif /* ENABLE_LANE_MAP_WARNING */

            /* Verify that a lane map is not used twice. */
            for (index2=(uint8_t)(index1+1); index2<num_lanes; ++index2) {
                if ((tx_lane_map[index1] == tx_lane_map[index2])
                    || (rx_lane_map[index1] == rx_lane_map[index2])) {
                    return (ERR_CODE_BAD_LANE);
                }
            }
        }
    }

    /* Write the map bitfields.
     * Support up to 8 lanes.
     */
    EFUN(wrc_tx_lane_addr_0(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_0(*(rx_lane_map++)));
#if defined(wrc_tx_lane_addr_1)
    if (num_lanes > 1) { EFUN(wrc_tx_lane_addr_1(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_1(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_2)
    if (num_lanes > 2) { EFUN(wrc_tx_lane_addr_2(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_2(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_3)
    if (num_lanes > 3) { EFUN(wrc_tx_lane_addr_3(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_3(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_4)
    if (num_lanes > 4) { EFUN(wrc_tx_lane_addr_4(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_4(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_5)
    if (num_lanes > 5) { EFUN(wrc_tx_lane_addr_5(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_5(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_6)
    if (num_lanes > 6) { EFUN(wrc_tx_lane_addr_6(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_6(*(rx_lane_map++))); }
#if defined(wrc_tx_lane_addr_7)
    if (num_lanes > 7) { EFUN(wrc_tx_lane_addr_7(*(tx_lane_map++))); EFUN(wrc_rx_lane_addr_7(*(rx_lane_map++))); }
#endif
#endif
#endif
#endif
#endif
#endif
#endif
    return (ERR_CODE_NONE);
}

/************************/
/*  Serdes API Version  */
/************************/

err_code_t falcon16_v1l4p1_version(srds_access_t *sa__, uint32_t *api_version) {

    if(!api_version) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    *api_version = 0xA1072F;
    return (ERR_CODE_NONE);
}

/*****************/
/*  PMD_RX_LOCK  */
/*****************/

err_code_t falcon16_v1l4p1_pmd_lock_status(srds_access_t *sa__, uint8_t *pmd_rx_lock) {
    if(!pmd_rx_lock) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    ESTM(*pmd_rx_lock = rd_pmd_rx_lock());
    return (ERR_CODE_NONE);
}

/**********************************/
/*  Serdes TX disable/RX Restart  */
/**********************************/

err_code_t falcon16_v1l4p1_tx_disable(srds_access_t *sa__, uint8_t enable) {

  if (enable) {
    EFUN(wr_sdk_tx_disable(0x1));
  }
  else {
    EFUN(wr_sdk_tx_disable(0x0));
  }
  return(ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_rx_restart(srds_access_t *sa__, uint8_t enable) {

  EFUN(wr_rx_restart_pmd_hold(enable));
  return(ERR_CODE_NONE);
}
/******************************************************/
/*  Single function to set/get all RX AFE parameters  */
/******************************************************/

err_code_t falcon16_v1l4p1_write_rx_afe(srds_access_t *sa__, enum srds_rx_afe_settings_enum param, int8_t val) {
  /* Assumes the micro is not actively tuning */

    switch(param) {
    case RX_AFE_PF:
        return(falcon16_v1l4p1_INTERNAL_set_rx_pf_main(sa__, (uint8_t)val));

    case RX_AFE_PF2:
        return(falcon16_v1l4p1_INTERNAL_set_rx_pf2(sa__, (uint8_t)val));

    case RX_AFE_VGA:
      return(falcon16_v1l4p1_INTERNAL_set_rx_vga(sa__, (uint8_t)val));

    case RX_AFE_DFE1:
        return(falcon16_v1l4p1_INTERNAL_set_rx_dfe1(sa__, val));

    case RX_AFE_DFE2:
        return(falcon16_v1l4p1_INTERNAL_set_rx_dfe2(sa__, val));

    case RX_AFE_DFE3:
        return(falcon16_v1l4p1_INTERNAL_set_rx_dfe3(sa__, val));

    case RX_AFE_DFE4:
        return(falcon16_v1l4p1_INTERNAL_set_rx_dfe4(sa__, val));

    case RX_AFE_DFE5:
        return(falcon16_v1l4p1_INTERNAL_set_rx_dfe5(sa__, val));

    case RX_AFE_DFE6:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe6(sa__, val));

    case RX_AFE_DFE7:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe7(sa__, val));

    case RX_AFE_DFE8:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe8(sa__, val));

    case RX_AFE_DFE9:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe9(sa__, val));

    case RX_AFE_DFE10:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe10(sa__, val));

    case RX_AFE_DFE11:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe11(sa__, val));

    case RX_AFE_DFE12:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe12(sa__, val));

    case RX_AFE_DFE13:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe13(sa__, val));

    case RX_AFE_DFE14:
        return (falcon16_v1l4p1_INTERNAL_set_rx_dfe14(sa__, val));
    default:
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
}

err_code_t falcon16_v1l4p1_read_rx_afe(srds_access_t *sa__, enum srds_rx_afe_settings_enum param, int8_t *val) {
    /* Assumes the micro is not actively tuning */

    uint8_t rd_val;

    if(!val) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    switch(param) {
    case RX_AFE_PF:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_pf_main(sa__, &rd_val));
        *val = (int8_t)rd_val;
        break;
    case RX_AFE_PF2:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_pf2(sa__, &rd_val));
        *val = (int8_t)rd_val;
        break;
    case RX_AFE_VGA:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_vga(sa__, &rd_val));
        *val = (int8_t)rd_val;
      break;

    case RX_AFE_DFE1:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe1(sa__, val));
        break;
    case RX_AFE_DFE2:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe2(sa__, val));
        break;
    case RX_AFE_DFE3:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe3(sa__, val));
        break;
    case RX_AFE_DFE4:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe4(sa__, val));
        break;
    case RX_AFE_DFE5:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe5(sa__, val));
        break;
    case RX_AFE_DFE6:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe6(sa__, val));
        break;
    case RX_AFE_DFE7:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe7(sa__, val));
        break;
    case RX_AFE_DFE8:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe8(sa__, val));
        break;
    case RX_AFE_DFE9:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe9(sa__, val));
        break;
    case RX_AFE_DFE10:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe10(sa__, val));
        break;
    case RX_AFE_DFE11:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe11(sa__, val));
        break;
    case RX_AFE_DFE12:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe12(sa__, val));
        break;
    case RX_AFE_DFE13:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe13(sa__, val));
        break;
    case RX_AFE_DFE14:
        EFUN(falcon16_v1l4p1_INTERNAL_get_rx_dfe14(sa__, val));
        break;
    default:
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    UNUSED(rd_val);
    return(ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_validate_txfir_cfg(srds_access_t *sa__, int8_t pre, int8_t main, int8_t post1, int8_t post2, int8_t post3) {
  err_code_t err_code = ERR_CODE_NONE;

  if ((main  < TXFIR_MAIN_MIN)
#if (TXFIR_MAIN_MAX < 127) /* Inhibit this comparison to avoid "comparison is always false" warning */
                                || (main  > TXFIR_MAIN_MAX)
#endif
                                                            ) err_code |= ERR_CODE_TXFIR_MAIN_INVALID;

  if ((pre   > TXFIR_PRE_MAX)   || (pre   < TXFIR_PRE_MIN))   err_code |= ERR_CODE_TXFIR_PRE_INVALID;    
  if ((post1 > TXFIR_POST1_MAX) || (post1 < TXFIR_POST1_MIN)) err_code |= ERR_CODE_TXFIR_POST1_INVALID;
  if ((post2 > TXFIR_POST2_MAX) || (post2 < TXFIR_POST2_MIN)) err_code |= ERR_CODE_TXFIR_POST2_INVALID;
  if ((post3 > TXFIR_POST3_MAX) || (post3 < TXFIR_POST3_MIN)) err_code |= ERR_CODE_TXFIR_POST3_INVALID;

  if ((pre + main + SRDS_ABS(post1) + SRDS_ABS(post2) + SRDS_ABS(post3)) > TXFIR_SUM_LIMIT) err_code |= ERR_CODE_TXFIR_SUM_LIMIT;
  if ((main - SRDS_ABS(pre) - SRDS_ABS(post1) - SRDS_ABS(post2) - SRDS_ABS(post3)) < TXFIR_V2_LIMIT) err_code |= ERR_CODE_TXFIR_V2_LIMIT;

  return (falcon16_v1l4p1_error(sa__, err_code));
}


err_code_t falcon16_v1l4p1_write_tx_afe(srds_access_t *sa__, enum falcon16_v1l4p1_tx_afe_settings_enum param, int8_t val) {

  switch(param) {
    case TX_AFE_PRE:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_pre(sa__, (uint8_t)val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    case TX_AFE_MAIN:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_main(sa__, (uint8_t)val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    case TX_AFE_POST1:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_post1(sa__, val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    case TX_AFE_POST2:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_post2(sa__, val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    case TX_AFE_POST3:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_post3(sa__, val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    case TX_AFE_RPARA:
        EFUN(falcon16_v1l4p1_INTERNAL_set_tx_rpara(sa__, (uint8_t)val));
        EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
        break;
    default:
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
  }
  return(ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_read_tx_afe(srds_access_t *sa__, enum falcon16_v1l4p1_tx_afe_settings_enum param, int8_t *val) {
    if(!val) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    switch(param) {
    case TX_AFE_PRE:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_pre(sa__, val));
        break;
    case TX_AFE_MAIN:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_main(sa__, val));
        break;
    case TX_AFE_POST1:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_post1(sa__, val));
        break;
    case TX_AFE_POST2:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_post2(sa__, val));
        break;
    case TX_AFE_POST3:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_post3(sa__, val));
        break;
    case TX_AFE_RPARA:
        EFUN(falcon16_v1l4p1_INTERNAL_get_tx_rpara(sa__, val));
        break;
    default:
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    return(ERR_CODE_NONE);
}
err_code_t falcon16_v1l4p1_apply_txfir_cfg(srds_access_t *sa__, int8_t pre, int8_t main, int8_t post1, int8_t post2, int8_t post3) {

  err_code_t failcode = falcon16_v1l4p1_validate_txfir_cfg(sa__, pre, main, post1, post2, post3);
  if (!failcode) {
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_pre(sa__, (uint8_t)pre);
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_main(sa__, (uint8_t)main);
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_post1(sa__, post1);
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_post2(sa__, post2);
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_post3(sa__, post3);
    failcode |= falcon16_v1l4p1_INTERNAL_set_tx_rpara(sa__, (uint8_t)(TXFIR_SUM_LIMIT - (pre + main + SRDS_ABS(post1) + SRDS_ABS(post2) + SRDS_ABS(post3))));
    EFUN(falcon16_v1l4p1_INTERNAL_load_txfir_taps(sa__));
  }
  
  return (falcon16_v1l4p1_error(sa__, failcode));
}


/**********************************************/
/*  Loopback and Ultra-Low Latency Functions  */
/**********************************************/

/* Locks TX_PI to Loop timing */
err_code_t falcon16_v1l4p1_loop_timing(srds_access_t *sa__, uint8_t enable) {

    if (enable) {
        EFUN(wr_tx_pi_loop_timing_src_sel(0x1)); /* RX phase_sum_val_logic enable */
        EFUN(wr_tx_pi_en(0x1));                  /* TX_PI enable: 0 = diabled, 1 = enabled */
        EFUN(wr_tx_pi_jitter_filter_en(0x1));    /* Jitter filter enable to lock freq: 0 = diabled, 1 = enabled */
        EFUN(USR_DELAY_US(25));               /* Wait for tclk to lock to CDR */
    } 
    else {
        EFUN(wr_tx_pi_jitter_filter_en(0x0));    /* Jitter filter enable to lock freq: 0 = diabled, 1 = enabled */
        EFUN(wr_tx_pi_en(0x0));                  /* TX_PI enable: 0 = diabled, 1 = enabled */
        EFUN(wr_tx_pi_loop_timing_src_sel(0x0)); /* RX phase_sum_val_logic enable */
    }
    return (ERR_CODE_NONE);
}

/* Setup Remote Loopback mode  */
err_code_t falcon16_v1l4p1_rmt_lpbk(srds_access_t *sa__, uint8_t enable) {
    if (enable) {
        EFUN(falcon16_v1l4p1_loop_timing(sa__, enable));
        EFUN(wr_tx_pi_ext_ctrl_en(0x1));  /* PD path enable: 0 = diabled, 1 = enabled */
        EFUN(wr_rmt_lpbk_en(0x1));        /* Remote Loopback Enable: 0 = diabled, 1 = enable  */
        EFUN(USR_DELAY_US(50));        /* Wait for rclk and tclk phase lock before expecing good data from rmt loopback */
    } else {                              /* Might require longer wait time for smaller values of tx_pi_ext_phase_bwsel_integ */
        EFUN(wr_rmt_lpbk_en(0x0));        /* Remote Loopback Enable: 0 = diabled, 1 = enable  */
        EFUN(wr_tx_pi_ext_ctrl_en(0x0));  /* PD path enable: 0 = diabled, 1 = enabled */
        EFUN(falcon16_v1l4p1_loop_timing(sa__, enable));
    }
    return (ERR_CODE_NONE);
}


/********************************/
/*  TX_PI Fixed Frequency Mode  */
/********************************/
/* TX_PI Frequency Override (Fixed Frequency Mode) */
err_code_t falcon16_v1l4p1_tx_pi_freq_override(srds_access_t *sa__, uint8_t enable, int16_t freq_override_val) {
    if (enable) {
        EFUN(wr_tx_pi_en(0x1));                              /* TX_PI enable :            0 = disabled, 1 = enabled */
        EFUN(wr_tx_pi_freq_override_val((uint16_t)freq_override_val)); /* Fixed Freq Override Value (+/-8192) */
        EFUN(wr_tx_pi_freq_override_en(0x1));                /* Fixed freq mode enable:   0 = disabled, 1 = enabled */
    }
    else {
        EFUN(wr_tx_pi_freq_override_en(0x0));                /* Fixed freq mode enable:   0 = disabled, 1 = enabled */
        EFUN(wr_tx_pi_freq_override_val(0));                 /* Fixed Freq Override Value to 0 */
        EFUN(wr_tx_pi_en(0x0));                              /* TX_PI enable :            0 = disabled, 1 = enabled */
    }
  return (ERR_CODE_NONE);
}

/*********************************/
/*  uc_active and uc_reset APIs  */
/*********************************/
err_code_t falcon16_v1l4p1_get_micro_num_uc_cores(srds_access_t *sa__, uint8_t *num_micros) {
    return (ERR_CODE_NONE);
}

/* Enable or Disable the uC reset */
err_code_t falcon16_v1l4p1_uc_reset(srds_access_t *sa__, uint8_t enable) {
  ucode_info_t ucode_info = {0};
  return falcon16_v1l4p1_uc_reset_with_info(sa__, enable, ucode_info);
}

err_code_t falcon16_v1l4p1_uc_reset_with_info(srds_access_t *sa__, uint8_t enable, ucode_info_t ucode_info) {
  if (enable) {
#ifdef wrc_micro_micro_s_rstb
    EFUN(wrc_micro_micro_s_rstb(0x0));                    /* Toggle micro_reset to reset all micro registers to default value */
    EFUN(wrc_micro_micro_s_rstb(0x1));
#else
    /* Assert micro reset and reset all micro registers (all non-status registers written to default value) */
    EFUN(wrc_micro_core_clk_en(0x0));                     /* Disable clock to M0 core */

    EFUN(wrc_micro_master_clk_en(0x0));                   /* Disable clock to microcontroller subsystem */
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD200, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD201, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD202, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD204, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD205, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD206, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD207, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD208, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD209, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD20A, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD20B, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD20C, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD20D, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD20E, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD211, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD212, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD213, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD214, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD215, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD216, 0x0007));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD217, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD218, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD219, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD21A, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD21B, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD220, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD221, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD224, 0x0000));

    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD225, 0x8701));
#endif /* wrc_micro_micro_s_rstb */

    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD226, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD228, 0x0101));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD229, 0x0000));
    EFUN(falcon16_v1l4p1_acc_wr_reg(sa__, 0xD22A, 0x0000));
  }
  else {
#ifdef wrc_micro_micro_s_rstb
    EFUN(wrc_micro_micro_s_rstb(0x1));               /* De-assert micro reset */
#endif
    /* De-assert micro reset - Start executing code */
    EFUN(wrc_micro_master_clk_en (0x1));             /* Enable clock to microcontroller subsystem */
    EFUN(wrc_micro_master_rstb   (0x1));             /* De-assert reset to microcontroller sybsystem */

    EFUN(wrc_micro_core_clk_en   (0x1));            /* Enable clock to M0 core */
    EFUN(wrc_micro_pmi_hp_fast_read_en(0));         /* Prevent micro exceptions when REFCLK not present */
    EFUN(wrc_micro_core_rstb     (0x1));
  }
 return (ERR_CODE_NONE);
}

/* Wait for uC to become active */
err_code_t falcon16_v1l4p1_wait_uc_active(srds_access_t *sa__) {
    uint16_t loop;
    for (loop = 0; loop < 10000; loop++) {
        uint16_t rddata;
        ESTM(rddata = rdc_uc_active());
        if (rddata) {
            return (ERR_CODE_NONE);
        }
        if (loop>10) {
            EFUN(USR_DELAY_US(1));
        }
    }
    return (falcon16_v1l4p1_error(sa__, ERR_CODE_MICRO_INIT_NOT_DONE));
}

err_code_t falcon16_v1l4p1_init_falcon16_v1l4p1_info(srds_access_t *sa__) {
    uint32_t info_table[INFO_TABLE_END / sizeof(uint32_t)];
    uint32_t info_table_signature;
    uint8_t  info_table_version;
    err_code_t err_code = ERR_CODE_NONE;
    srds_info_t * falcon16_v1l4p1_info_ptr = falcon16_v1l4p1_INTERNAL_get_falcon16_v1l4p1_info_ptr(sa__);

    ENULL_MEMSET(info_table, 0, sizeof(info_table));

    USR_ACQUIRE_LOCK;
    /* Handle error cases when falcon16_v1l4p1_info_ptr returned as zero */
    if (falcon16_v1l4p1_info_ptr == NULL) {
        EFUN_PRINTF(("ERROR:  Info table pointer is null.\n"));
        RELEASE_LOCK_AND_RETURN (falcon16_v1l4p1_error(sa__, ERR_CODE_INFO_TABLE_ERROR));
    }
    if (falcon16_v1l4p1_info_ptr->signature == SRDS_INFO_SIGNATURE) {
        /*Already initalized and so check for microcode version and exit with proper status*/
        err_code = falcon16_v1l4p1_INTERNAL_match_ucode_from_info(sa__);

        if (err_code == ERR_CODE_NONE) {
            /* ucode version match */
            RELEASE_LOCK_AND_RETURN (ERR_CODE_NONE);
        } else {
            /* ucode version mismatch */
            RELEASE_LOCK_AND_RETURN (falcon16_v1l4p1_error(sa__, ERR_CODE_MICRO_INIT_NOT_DONE));
        }
    }
    /*Never initialized so far, continue with initialization */

    err_code = falcon16_v1l4p1_rdblk_uc_prog_ram(sa__, (uint8_t *)info_table, INFO_TABLE_RAM_BASE, INFO_TABLE_END);
    /* Release lock before returning from the call to avoid deadlock */
    if (ERR_CODE_NONE != err_code) {
       RELEASE_LOCK_AND_RETURN(falcon16_v1l4p1_error(sa__, err_code));
    }

    info_table_signature = info_table[INFO_TABLE_OFFS_SIGNATURE / sizeof(uint32_t)];
    info_table_version = (uint8_t)(info_table_signature >> 24);
    if (((info_table_signature & 0x00FFFFFF) != 0x666E49)
        || (!(   ((info_table_version >= 0x32) && (info_table_version <= 0x39))
              || ((info_table_version >= 0x41) && (info_table_version <= 0x5A))))) {
        RELEASE_LOCK_AND_RETURN (falcon16_v1l4p1_error(sa__, ERR_CODE_INFO_TABLE_ERROR));
    }
    
    falcon16_v1l4p1_info_ptr->lane_count = info_table[INFO_TABLE_OFFS_OTHER_SIZE / sizeof(uint32_t)] & 0xFF;
    falcon16_v1l4p1_info_ptr->trace_memory_descending_writes = ((info_table[INFO_TABLE_OFFS_OTHER_SIZE / sizeof(uint32_t)] & 0x1000000) != 0);

    falcon16_v1l4p1_info_ptr->core_var_ram_size = (info_table[INFO_TABLE_OFFS_OTHER_SIZE / sizeof(uint32_t)] >> 8) & 0xFF;

    falcon16_v1l4p1_info_ptr->lane_var_ram_size = (info_table[INFO_TABLE_OFFS_TRACE_LANE_MEM_SIZE / sizeof(uint32_t)] >> 16) & 0xFFFF;

    falcon16_v1l4p1_info_ptr->diag_mem_ram_size = (info_table[INFO_TABLE_OFFS_TRACE_LANE_MEM_SIZE / sizeof(uint32_t)] & 0xFFFF) / falcon16_v1l4p1_info_ptr->lane_count;
    falcon16_v1l4p1_info_ptr->trace_mem_ram_size = (info_table[INFO_TABLE_OFFS_TRACE_LANE_MEM_SIZE / sizeof(uint32_t)] & 0xFFFF);
    
    falcon16_v1l4p1_info_ptr->diag_mem_ram_base = info_table[INFO_TABLE_OFFS_TRACE_MEM_PTR / sizeof(uint32_t)];
    falcon16_v1l4p1_info_ptr->trace_mem_ram_base = falcon16_v1l4p1_info_ptr->diag_mem_ram_base;
    
    falcon16_v1l4p1_info_ptr->core_var_ram_base = info_table[INFO_TABLE_OFFS_CORE_MEM_PTR / sizeof(uint32_t)];
    
    falcon16_v1l4p1_info_ptr->micro_var_ram_base = info_table[INFO_TABLE_OFFS_MICRO_MEM_PTR / sizeof(uint32_t)];
    falcon16_v1l4p1_info_ptr->micro_var_ram_size = (info_table[INFO_TABLE_OFFS_OTHER_SIZE_2 / sizeof(uint32_t)] >> 4) & 0xFF;

    falcon16_v1l4p1_info_ptr->lane_var_ram_base = info_table[INFO_TABLE_OFFS_LANE_MEM_PTR / sizeof(uint32_t)];

    

    if (info_table_version < 0x34) {
        falcon16_v1l4p1_info_ptr->micro_count = 1;
    } else {
        falcon16_v1l4p1_info_ptr->micro_count = info_table[INFO_TABLE_OFFS_OTHER_SIZE_2 / sizeof(uint32_t)] & 0xF;
    }

    if (info_table_version < 0x36) {
        falcon16_v1l4p1_info_ptr->grp_ram_size = 0;
    } else {
        falcon16_v1l4p1_info_ptr->grp_ram_size = info_table[INFO_TABLE_OFFS_GROUP_RAM_SIZE / sizeof(uint32_t)] & 0xFFFF;
    }
    falcon16_v1l4p1_info_ptr->ucode_version = info_table[INFO_TABLE_OFFS_UC_VERSION /sizeof(uint32_t)];
    falcon16_v1l4p1_info_ptr->signature = SRDS_INFO_SIGNATURE;

    RELEASE_LOCK_AND_RETURN (ERR_CODE_NONE);
}

/*-----------------*/
/*  Configure PLL  */
/*-----------------*/

err_code_t falcon16_v1l4p1_configure_pll_refclk_div(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           enum falcon16_v1l4p1_pll_div_enum srds_div) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, srds_div, 0, FALCON16_V1L4P1_PLL_OPTION_NONE);
}

err_code_t falcon16_v1l4p1_configure_pll_refclk_vco(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, FALCON16_V1L4P1_PLL_DIV_UNKNOWN, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_NONE);
}

err_code_t falcon16_v1l4p1_configure_pll_div_vco(srds_access_t *sa__,
                                        enum falcon16_v1l4p1_pll_div_enum srds_div,
                                        uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, FALCON16_V1L4P1_PLL_REFCLK_UNKNOWN, srds_div, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_NONE);
}

/* following routines divide refclk input by 2 to prevent fracn mode */
err_code_t falcon16_v1l4p1_configure_pll_refclk_div_div2refclk(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           enum falcon16_v1l4p1_pll_div_enum srds_div) {
   return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, srds_div, 0, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV2_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_refclk_vco_div2refclk(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, FALCON16_V1L4P1_PLL_DIV_UNKNOWN, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV2_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_div_vco_div2refclk(srds_access_t *sa__,
                                        enum falcon16_v1l4p1_pll_div_enum srds_div,
                                        uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, FALCON16_V1L4P1_PLL_REFCLK_UNKNOWN, srds_div, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV2_EN);
}
/* following routines divide refclk input by 2 to prevent fracn mode */
err_code_t falcon16_v1l4p1_configure_pll_refclk_div_div4refclk(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           enum falcon16_v1l4p1_pll_div_enum srds_div) {
   return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, srds_div, 0, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV4_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_refclk_vco_div4refclk(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, FALCON16_V1L4P1_PLL_DIV_UNKNOWN, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV4_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_div_vco_div4refclk(srds_access_t *sa__,
                                        enum falcon16_v1l4p1_pll_div_enum srds_div,
                                        uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, FALCON16_V1L4P1_PLL_REFCLK_UNKNOWN, srds_div, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DIV4_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_refclk_div_refclk_2x(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           enum falcon16_v1l4p1_pll_div_enum srds_div) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, srds_div, 0, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DOUBLER_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_refclk_vco_refclk_2x(srds_access_t *sa__,
                                           enum falcon16_v1l4p1_pll_refclk_enum refclk,
                                           uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, refclk, FALCON16_V1L4P1_PLL_DIV_UNKNOWN, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DOUBLER_EN);
}

err_code_t falcon16_v1l4p1_configure_pll_div_vco_refclk_2x(srds_access_t *sa__,
                                        enum falcon16_v1l4p1_pll_div_enum srds_div,
                                        uint32_t vco_freq_khz) {
    return falcon16_v1l4p1_INTERNAL_configure_pll(sa__, FALCON16_V1L4P1_PLL_REFCLK_UNKNOWN, srds_div, vco_freq_khz, FALCON16_V1L4P1_PLL_OPTION_REFCLK_DOUBLER_EN);
}

err_code_t falcon16_v1l4p1_get_vco_from_refclk_div(srds_access_t *sa__, uint32_t refclk_freq_hz, enum falcon16_v1l4p1_pll_div_enum srds_div, uint32_t *vco_freq_khz, enum falcon16_v1l4p1_pll_option_enum pll_option) {
    EFUN(falcon16_v1l4p1_INTERNAL_get_vco_from_refclk_div(sa__, refclk_freq_hz, srds_div, vco_freq_khz, pll_option));
    return (ERR_CODE_NONE);
}


/***********************************************/
/*  Microcode Load into Program RAM Functions  */
/***********************************************/

/* uCode Load through Register (MDIO) Interface [Return Val = Error_Code (0 = PASS)] */
err_code_t falcon16_v1l4p1_ucode_mdio_load(srds_access_t *sa__, uint8_t *ucode_image, uint32_t ucode_len) {
    uint32_t   ucode_len_padded, count = 0;
    uint16_t   wrdata_lsw;
    uint8_t    wrdata_lsb;

    if(!ucode_image) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    if (ucode_len > UCODE_MAX_SIZE) {                     /* uCode size should be less than UCODE_MAX_SIZE  */
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_INVALID_UCODE_LEN));
    }

    EFUN(wrc_micro_master_clk_en(0x1));                   /* Enable clock to microcontroller subsystem */
    EFUN(wrc_micro_master_rstb(0x1));                     /* De-assert reset to microcontroller sybsystem */
    EFUN(wrc_micro_master_rstb(0x0));                     /* Assert reset to microcontroller sybsystem - Toggling reset*/
    EFUN(wrc_micro_master_rstb(0x1));                     /* De-assert reset to microcontroller sybsystem */

    EFUN(wrc_micro_ra_init(0x1));                         /* Set initialization command to initialize code RAM */
    EFUN(falcon16_v1l4p1_INTERNAL_poll_micro_ra_initdone(sa__, 250)); /* Poll for micro_ra_initdone = 1 to indicate initialization done */

    EFUN(wrc_micro_ra_init(0x2));                         /* Write command for data RAM initialization */
    EFUN(falcon16_v1l4p1_INTERNAL_poll_micro_ra_initdone(sa__, 250)); /* Poll status of data RAM initialization */

    EFUN(wrc_micro_ra_init(0x0));                         /* Clear initialization command */

    ucode_len_padded = ((ucode_len + 3) & 0xFFFFFFFC);    /* Aligning ucode size to 4-byte boundary */

    /* Code to Load microcode */
    EFUN(wrc_micro_autoinc_wraddr_en(0x1));               /* To auto increment RAM write address */
    EFUN(wrc_micro_ra_wrdatasize(0x1));                   /* Select 16bit transfers */
    EFUN(wrc_micro_ra_wraddr_msw(0x0));                   /* Upper 16bits of start address of Program RAM where the ucode is to be loaded */
    EFUN(wrc_micro_ra_wraddr_lsw(0x0));                   /* Lower 16bits of start address of Program RAM where the ucode is to be loaded */

    do {                                                  /* ucode_image loaded 16bits at a time */
        wrdata_lsb = (count < ucode_len) ? ucode_image[count] : 0x0; /* wrdata_lsb read from ucode_image; zero padded to 4byte boundary */
        count++;
        wrdata_lsw = (count < ucode_len) ? ucode_image[count] : 0x0; /* wrdata_msb read from ucode_image; zero padded to 4byte boundary */
        count++;
        wrdata_lsw = (uint16_t)((wrdata_lsw << 8) | wrdata_lsb);               /* 16bit wrdata_lsw formed from 8bit msb and lsb values read from ucode_image */
        EFUN(wrc_micro_ra_wrdata_lsw(wrdata_lsw));                   /* Program RAM lower 16bits write data */
    }   while (count < ucode_len_padded);                 /* Loop repeated till entire image loaded (upto the 4byte boundary) */

    EFUN(wrc_micro_ra_wrdatasize(0x2));                   /* Select 32bit transfers as default */
    EFUN(wrc_micro_core_clk_en(0x1));                     /* Enable clock to M0 core */
    /* EFUN(wrc_micro_core_rstb(0x1)); */                 /* De-assert reset to micro to start executing microcode */
    return (ERR_CODE_NONE);                               /* NO Errors while loading microcode (uCode Load PASS) */
  }


/* Read-back uCode from Program RAM and verify against ucode_image [Return Val = Error_Code (0 = PASS)]  */
err_code_t falcon16_v1l4p1_ucode_load_verify(srds_access_t *sa__, uint8_t *ucode_image, uint32_t ucode_len) {

    uint32_t ucode_len_padded, count = 0;
    uint16_t rddata_lsw;
    uint16_t data_lsw;
    uint8_t  rddata_lsb;

    if(!ucode_image) {
        return(falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    ucode_len_padded = ((ucode_len + 3) & 0xFFFFFFFC);    /* Aligning ucode size to 4-byte boundary */

    if (ucode_len_padded > UCODE_MAX_SIZE) {              /* uCode size should be less than UCODE_MAX_SIZE */
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_INVALID_UCODE_LEN));
    }

    EFUN(wrc_micro_autoinc_rdaddr_en(0x1));               /* To auto increment RAM read address */
    EFUN(wrc_micro_ra_rddatasize(0x1));                   /* Select 16bit transfers */
    EFUN(wrc_micro_ra_rdaddr_msw(0x0));                   /* Upper 16bits of start address of Program RAM from where to read ucode */
    EFUN(wrc_micro_ra_rdaddr_lsw(0x0));                   /* Lower 16bits of start address of Program RAM from where to read ucode */

    do {                                                  /* ucode_image read 16bits at a time */
        rddata_lsb = (count < ucode_len) ? ucode_image[count] : 0x0; /* rddata_lsb read from ucode_image; zero padded to 4byte boundary */
        count++;
        rddata_lsw = (count < ucode_len) ? ucode_image[count] : 0x0; /* rddata_msb read from ucode_image; zero padded to 4byte boundary */
        count++;
        rddata_lsw = (uint16_t)((rddata_lsw << 8) | rddata_lsb);               /* 16bit rddata_lsw formed from 8bit msb and lsb values read from ucode_image */
                                                                     /* Compare Program RAM ucode to ucode_image (Read to micro_ra_rddata_lsw reg auto-increments the ram_address) */
        ESTM(data_lsw = rdc_micro_ra_rddata_lsw());
        if (data_lsw != rddata_lsw) {
            EFUN_PRINTF(("Ucode_Load_Verify_FAIL: Addr = 0x%x: Read_data = 0x%x :  Expected_data = 0x%x \n",(count-2),data_lsw,rddata_lsw));
            return (falcon16_v1l4p1_error(sa__, ERR_CODE_UCODE_VERIFY_FAIL));             /* Verify uCode FAIL */
        }

    } while (count < ucode_len_padded);                   /* Loop repeated till entire image loaded (upto the 4byte boundary) */

    EFUN(wrc_micro_ra_rddatasize(0x2));                   /* Select 32bit transfers ad default */
    return (ERR_CODE_NONE);                               /* Verify uCode PASS */
}

/******************************************************************/
/*  APIs to Align TX clocks                                       */
/******************************************************************/
err_code_t falcon16_v1l4p1_tx_clock_align(srds_access_t *sa__, int num_lanes, int enable) {
    uint8_t current_lane;
    int lane;

    if(num_lanes < 0) {
        return (ERR_CODE_BAD_PTR_OR_INVALID_INPUT);
    }
    ESTM(current_lane = falcon16_v1l4p1_get_lane(sa__));

    for(lane = 0;lane<num_lanes;++lane) {
        EFUN(falcon16_v1l4p1_set_lane(sa__,(uint8_t)lane));
        if(enable) {
            if(lane == current_lane) {                   /* Current Lane should be the master lane */
                EFUN(wr_ams_tx_sel_txmaster(1));         /* Select one lane as txmaster. For all other lanes, this should be 0. */
                EFUN(wr_ams_tx_sel_ll(0));
                EFUN(wr_ams_tx_pdet_ll_clk(0));
            } else {
           /* Initial TX lane clock phase alignment. Program following registers only for slave lanes, master lane registers should not be programmed (can be kept in default values). */
                EFUN(wr_tx_pi_pd_bypass_vco(1));         /* for quick phase lock time */
                EFUN(wr_tx_pi_pd_bypass_flt(1));         /* for quick phase lock time */
#ifdef wr_tx_pi_hs_fifo_phserr_sel
                EFUN(wr_tx_pi_hs_fifo_phserr_sel(1));    /* selects PD from afe as input */
#else
                EFUN(wr_tx_pi_repeater_mode_en(1));      /* selects external PD from afe as input to tx_pi */
#endif
#ifdef wr_tx_pi_ext_pd_sel
                EFUN(wr_tx_pi_ext_pd_sel(0));            /* selects PD path based on tx_pi_repeater_mode_en */
#endif
                EFUN(wr_tx_pi_en(1));                    /* Enable TX_PI    */
                EFUN(wr_tx_pi_jitter_filter_en(0));      /* turn off frequency lock for tx_pi as all te tclk clocks are derived from VCO and same frequency */
                EFUN(wr_tx_pi_ext_ctrl_en(1));           /* turn on PD path to TX_PI to lock the clocks */
                EFUN(USR_DELAY_US(25));                  /* wait for the slave lane clocks to lock to the master lane clock */
                EFUN(wr_tx_pi_ext_phase_bwsel_integ(3)); /* 0 to 7: higher value means faster lock time */
                EFUN(wr_tx_pi_pd_bypass_vco(0));         /* disable filter bypass for more accurate lock */
                EFUN(wr_tx_pi_pd_bypass_flt(0));         /* disable filter bypass for more accurate lock */
                EFUN(USR_DELAY_US(60));                  /* wait for the slave lane clocks to lock to the master lane clock withing 1/16th of UI. */
                /* that all TX lane clocks are correct frequency and aligned together within 1/16th of UI. */
            }
        
        } else {
            if(lane == current_lane) {                   /* Current Lane should be the master lane */
                EFUN(wr_ams_tx_sel_txmaster(0));         /* Select one lane as txmaster. For all other lanes, this should be 0. */
                EFUN(wr_ams_tx_sel_ll(0));
                EFUN(wr_ams_tx_pdet_ll_clk(0));
            } else {
           /* Initial TX lane clock phase alignment. Program following registers only for slave lanes, master lane registers should not be programmed (can be kept in default values). */
                EFUN(wr_tx_pi_pd_bypass_vco(0));         /* for quick phase lock time */
                EFUN(wr_tx_pi_pd_bypass_flt(0));         /* for quick phase lock time */
#ifdef wr_tx_pi_hs_fifo_phserr_sel
                EFUN(wr_tx_pi_hs_fifo_phserr_sel(0));    /* selects PD from afe as input */
#else
                EFUN(wr_tx_pi_repeater_mode_en(1));      /* selects external PD from afe as input to tx_pi */
#endif
#ifdef wr_tx_pi_ext_pd_sel
                EFUN(wr_tx_pi_ext_pd_sel(0));            /* selects PD path based on tx_pi_repeater_mode_en */
#endif
                EFUN(wr_tx_pi_en(0));                    /* Enable TX_PI    */
                EFUN(wr_tx_pi_jitter_filter_en(0));      /* turn off frequency lock for tx_pi as all te tclk clocks are derived from VCO and same frequency */
                EFUN(wr_tx_pi_ext_ctrl_en(0));           /* turn on PD path to TX_PI to lock the clocks */
                EFUN(wr_tx_pi_ext_phase_bwsel_integ(0)); /* 0 to 7: higher value means faster lock time */
                EFUN(wr_tx_pi_pd_bypass_vco(0));         /* disable filter bypass for more accurate lock */
                /* that all TX lane clocks are correct frequency and aligned together within 1/16th of UI. */
            }

        }
    }
    EFUN(falcon16_v1l4p1_set_lane(sa__,current_lane)); /* return lane to previous state */
    return(ERR_CODE_NONE);
 }



/************************/
/*  Fast Link Recovery  */
/************************/
err_code_t falcon16_v1l4p1_configure_fast_link_recovery(srds_access_t *sa__, uint8_t enable, uint8_t enable_timeout, enum falcon16_v1l4p1_flr_timeout_enum timeout_time, uint8_t enable_2ms_yield, uint8_t filter_count) {
    uint8_t const timeout_time_val = (uint8_t)timeout_time;

    uint8_t reset_state;
    ESTM(reset_state = rd_rx_lane_dp_reset_state());
    if(reset_state < 7) {
        EFUN_PRINTF(("ERROR: falcon16_v1l4p1_configure_fast_link_recovery(..) called without ln_dp_s_rstb=0 Lane=%d reset_state=%d\n",falcon16_v1l4p1_get_lane(sa__),reset_state));
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_LANE_DP_NOT_RESET));
    }

    if (timeout_time_val > 3) {
        ESTM_PRINTF(("timeout_time value for falcon16_v1l4p1_configure_fast_link_recovery() is too large:  %d\n", timeout_time_val));
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    if (filter_count > 7) {
        ESTM_PRINTF(("filter_count value for falcon16_v1l4p1_configure_fast_link_recovery() is too large:  %d\n", filter_count));
        return (falcon16_v1l4p1_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
    EFUN(wrv_usr_ctrl_fast_link_recovery_byte((uint8_t)((enable ? 1 : 0)
                                              | (enable_timeout ? 2 : 0)
                                              | (timeout_time_val << 2)
                                              | (enable_2ms_yield ? 16 : 0)
                                              | (filter_count << 5))));
    /* DISP(rdv_usr_ctrl_fast_link_recovery_byte()); */
    return (ERR_CODE_NONE);
}

err_code_t falcon16_v1l4p1_get_configure_fast_link_recovery(srds_access_t *sa__, uint8_t *enable, uint8_t *enable_timeout, enum falcon16_v1l4p1_flr_timeout_enum *timeout_time, uint8_t *enable_2ms_yield, uint8_t *filter_count) {
    uint8_t flr_byte = 0;
    if((enable == NULL) || (enable_timeout == NULL) ||( timeout_time == NULL) || (enable_2ms_yield == NULL) || (filter_count == NULL)) {
        return (ERR_CODE_BAD_PTR_OR_INVALID_INPUT);
    }

    ESTM(flr_byte = rdv_usr_ctrl_fast_link_recovery_byte());
    *enable = flr_byte & 0x1;
    *enable_timeout = (flr_byte >> 1) & 0x1;
    *timeout_time = (enum falcon16_v1l4p1_flr_timeout_enum)((flr_byte >> 2) & 0x3);
    *enable_2ms_yield = (flr_byte >> 4) & 0x1;
    *filter_count = (flr_byte >> 5) & 0x7;
    return (ERR_CODE_NONE);
}
