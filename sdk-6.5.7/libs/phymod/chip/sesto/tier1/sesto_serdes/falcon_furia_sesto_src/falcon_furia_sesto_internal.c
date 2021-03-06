/***********************************************************************************
 ***********************************************************************************
 *  File Name     :  falcon_furia_internal.c                                         *
 *  Created On    :  13/02/2014                                                    *
 *  Created By    :  Justin Gaither                                                *
 *  Description   :  APIs for Serdes IPs                                           *
 *  Revision      :  $Id: falcon_furia_internal.c 865 2015-01-22 16:43:59Z jgaither $ *
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

/** @file falcon_furia_internal.c
 * Implementation of API functions
 */

#include "falcon_furia_sesto_internal.h"

SDK_STATIC uint32_t _mult_with_overflow_check(uint32_t a, uint32_t b, uint8_t *of) {
    uint16_t c,d;
    uint32_t r,s;
    if (a > b) return _mult_with_overflow_check(b, a, of);
    *of = 0;
    c = b >> 16;
    d = UINT16_MAX & b;
    r = a * c;
    s = a * d;
    if (r > UINT16_MAX) *of = 1;
    r <<= 16;
    return (s + r);
}

/* Formula for PVTMON: T = 410.04-0.48705*reg10bit (from PVTMON Analog Module Specification v5.0 section 6.2) */
#define _bin_to_degC(bin) ((int16_t)(410 + ((2212 - 1995*(int32_t)bin)>>12)))

  
  

SDK_STATIC int16_t _ladder_setting_to_mV(const phymod_access_t *pa,int8_t ctrl, uint8_t range_250) {
    uint8_t absv;                                     /* Absolute value of ctrl */
    int16_t nlmv;                                     /* Non-linear value */
    
    /* Get absolute value */
    absv = _abs(ctrl);

    {
       nlmv = absv*300/127;
       range_250 = 0; /* to avoid compile warinings */ 
    }
    
    USR_UNUSED_VAR(range_250);
    /* Add sign and return */
    return( (ctrl>=0) ? nlmv : -nlmv);

}

void falcon_furia_sesto_set_lms_hoffset ( const phymod_access_t *pa, char delta) {

    unsigned char loop;
    if (delta != 0) {
      
        wr_falcon_furia_sesto_rx_pi_slicers_en (0x30);
        wr_falcon_furia_sesto_rx_pi_phase_step_cnt (0);
        wr_falcon_furia_sesto_rx_pi_phase_step_dir (delta>0);
       

        for (loop=0;loop<(uint8_t)(_abs(delta));loop++) {
            wr_falcon_furia_sesto_rx_pi_manual_strobe (1);
        }

        /* Undo setup */
        
        wr_falcon_furia_sesto_rx_pi_slicers_en (0x0);
        wr_falcon_furia_sesto_rx_pi_phase_step_cnt (0);
        wr_falcon_furia_sesto_rx_pi_phase_step_dir (0);
        
    }

    return;
}

err_code_t falcon_furia_sesto_log_full_pmd_state ( const phymod_access_t *pa, struct falcon_furia_sesto_detailed_lane_status_st *lane_st) {
    err_code_t err_code = 0;
    uint16_t reg_data;
    int8_t tmp;
    uint8_t is_micro_stopped;

    if(!lane_st) 
	  return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

    ESTM(lane_st->pmd_lock = rd_falcon_furia_sesto_pmd_rx_lock());

    is_micro_stopped = 0;
    if (lane_st->pmd_lock == 1) {
      ESTM(is_micro_stopped = rdv_falcon_furia_sesto_usr_sts_micro_stopped());
      if (!is_micro_stopped) {
          EFUN(falcon_furia_sesto_stop_rx_adaptation( pa, 1));
      }
    } else {
      USR_PRINTF(("\n NOTE: Micro NOT stopped since pmd_lock not achieved.\n"));
    }
  
    ESTM(lane_st->temp_idx = rdcv_falcon_furia_sesto_temp_idx());
    ESTM(lane_st->ams_tx_drv_hv_disable = rd_falcon_furia_sesto_ams_tx_drv_hv_disable());    
    ESTM(lane_st->ams_tx_ana_rescal = rd_falcon_furia_sesto_ams_tx_ana_rescal());    
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_AMP, &tmp)); lane_st->amp_ctrl = (uint8_t) tmp;
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_PRE, &tmp)); lane_st->pre_tap = (uint8_t) tmp;
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_MAIN, &tmp)); lane_st->main_tap = (uint8_t) tmp;
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_POST1, &tmp)); lane_st->post1_tap = (uint8_t) tmp;
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_POST2, &lane_st->post2_tap));
    EFUN(falcon_furia_sesto_read_tx_afe( pa, TX_AFE_POST3, &lane_st->post3_tap));
    ESTM(lane_st->sigdet = rd_falcon_furia_sesto_signal_detect());
    ESTM(lane_st->dsc_sm[0] = rd_falcon_furia_sesto_dsc_state_one_hot());
    ESTM(lane_st->dsc_sm[1] = rd_falcon_furia_sesto_dsc_state_one_hot());
#ifdef SERDES_API_FLOATING_POINT 
    ESTM(lane_st->ppm = (((double)1e6/64/20/128/16)*(int16_t)(rd_falcon_furia_sesto_cdr_integ_reg()/32)));
#else
    ESTM(lane_st->ppm = ((int16_t)(rd_falcon_furia_sesto_cdr_integ_reg())*12/1000));
#endif
    ESTM(lane_st->vga = rd_falcon_furia_sesto_rx_vga_ctrl());
    ESTM(lane_st->pf = rd_falcon_furia_sesto_rx_pf_ctrl());
    ESTM(lane_st->pf2 = rd_falcon_furia_sesto_rx_pf2_ctrl());
#ifdef SERDES_API_FLOATING_POINT 
    ESTM(lane_st->main_tap_est = rdv_falcon_furia_sesto_usr_main_tap_est()/32.0);
#else
    ESTM(lane_st->main_tap_est = rdv_falcon_furia_sesto_usr_main_tap_est()/32);
#endif
    ESTM(lane_st->data_thresh = rd_falcon_furia_sesto_rx_data_thresh_sel());
    ESTM(lane_st->phase_thresh = rd_falcon_furia_sesto_rx_phase_thresh_sel());
    ESTM(lane_st->lms_thresh = rd_falcon_furia_sesto_rx_lms_thresh_sel());
    EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_D,&reg_data));
    lane_st->ddq_hoffset = (uint8_t)dist_ccw(((reg_data>>8)&0xFF),(reg_data&0xFF));
    EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_P,&reg_data));
    lane_st->ppq_hoffset = (uint8_t)dist_ccw(((reg_data>>8)&0xFF),(reg_data&0xFF));
    EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_L,&reg_data));
    lane_st->llq_hoffset = (uint8_t)dist_ccw(((reg_data>>8)&0xFF),(reg_data&0xFF));
    EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_PD,&reg_data));
    lane_st->dp_hoffset = (uint8_t)dist_cw(((reg_data>>8)&0xFF),(reg_data&0xFF));
    EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_LD,&reg_data));
    lane_st->dl_hoffset = (uint8_t)dist_cw(((reg_data>>8)&0xFF),(reg_data&0xFF));
    ESTM(lane_st->dc_offset = rd_falcon_furia_sesto_dc_offset_bin());
    ESTM(lane_st->dfe[1][0] = rd_falcon_furia_sesto_rxa_dfe_tap2());
    ESTM(lane_st->dfe[1][1] = rd_falcon_furia_sesto_rxb_dfe_tap2());
    ESTM(lane_st->dfe[1][2] = rd_falcon_furia_sesto_rxc_dfe_tap2());
    ESTM(lane_st->dfe[1][3] = rd_falcon_furia_sesto_rxd_dfe_tap2());
    ESTM(lane_st->dfe[2][0] = rd_falcon_furia_sesto_rxa_dfe_tap3());
    ESTM(lane_st->dfe[2][1] = rd_falcon_furia_sesto_rxb_dfe_tap3());
    ESTM(lane_st->dfe[2][2] = rd_falcon_furia_sesto_rxc_dfe_tap3());
    ESTM(lane_st->dfe[2][3] = rd_falcon_furia_sesto_rxd_dfe_tap3());
    ESTM(lane_st->dfe[3][0] = rd_falcon_furia_sesto_rxa_dfe_tap4());
    ESTM(lane_st->dfe[3][1] = rd_falcon_furia_sesto_rxb_dfe_tap4());
    ESTM(lane_st->dfe[3][2] = rd_falcon_furia_sesto_rxc_dfe_tap4());
    ESTM(lane_st->dfe[3][3] = rd_falcon_furia_sesto_rxd_dfe_tap4());
    ESTM(lane_st->dfe[4][0] = rd_falcon_furia_sesto_rxa_dfe_tap5());
    ESTM(lane_st->dfe[4][1] = rd_falcon_furia_sesto_rxb_dfe_tap5());
    ESTM(lane_st->dfe[4][2] = rd_falcon_furia_sesto_rxc_dfe_tap5());
    ESTM(lane_st->dfe[4][3] = rd_falcon_furia_sesto_rxd_dfe_tap5());
    ESTM(lane_st->dfe[5][0] = rd_falcon_furia_sesto_rxa_dfe_tap6());
    ESTM(lane_st->dfe[5][1] = rd_falcon_furia_sesto_rxb_dfe_tap6());
    ESTM(lane_st->dfe[5][2] = rd_falcon_furia_sesto_rxc_dfe_tap6());
    ESTM(lane_st->dfe[5][3] = rd_falcon_furia_sesto_rxd_dfe_tap6());
    ESTM(lane_st->dfe[6][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap7_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap7():0));
    ESTM(lane_st->dfe[6][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap7_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap7():0));
    ESTM(lane_st->dfe[6][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap7_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap7():0));
    ESTM(lane_st->dfe[6][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap7_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap7():0));
    ESTM(lane_st->dfe[7][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap8_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap8():0));
    ESTM(lane_st->dfe[7][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap8_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap8():0));
    ESTM(lane_st->dfe[7][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap8_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap8():0));
    ESTM(lane_st->dfe[7][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap8_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap8():0));
    ESTM(lane_st->dfe[8][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap9_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap9():0));
    ESTM(lane_st->dfe[8][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap9_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap9():0));
    ESTM(lane_st->dfe[8][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap9_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap9():0));
    ESTM(lane_st->dfe[8][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap9_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap9():0));
    ESTM(lane_st->dfe[9][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap10_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap10():0));
    ESTM(lane_st->dfe[9][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap10_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap10():0));
    ESTM(lane_st->dfe[9][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap10_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap10():0));
    ESTM(lane_st->dfe[9][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap10_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap10():0));
    ESTM(lane_st->dfe[10][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap7_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap7():(rd_falcon_furia_sesto_rxa_dfe_tap11_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap11():0));
    ESTM(lane_st->dfe[10][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap7_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap7():(rd_falcon_furia_sesto_rxb_dfe_tap11_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap11():0));
    ESTM(lane_st->dfe[10][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap7_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap7():(rd_falcon_furia_sesto_rxc_dfe_tap11_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap11():0));
    ESTM(lane_st->dfe[10][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap7_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap7():(rd_falcon_furia_sesto_rxd_dfe_tap11_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap11():0));
    ESTM(lane_st->dfe[11][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap8_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap8():(rd_falcon_furia_sesto_rxa_dfe_tap12_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap12():0));
    ESTM(lane_st->dfe[11][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap8_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap8():(rd_falcon_furia_sesto_rxb_dfe_tap12_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap12():0));
    ESTM(lane_st->dfe[11][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap8_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap8():(rd_falcon_furia_sesto_rxc_dfe_tap12_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap12():0));
    ESTM(lane_st->dfe[11][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap8_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap8():(rd_falcon_furia_sesto_rxd_dfe_tap12_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap12():0));
    ESTM(lane_st->dfe[12][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap9_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap9():(rd_falcon_furia_sesto_rxa_dfe_tap13_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap13():0));
    ESTM(lane_st->dfe[12][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap9_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap9():(rd_falcon_furia_sesto_rxb_dfe_tap13_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap13():0));
    ESTM(lane_st->dfe[12][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap9_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap9():(rd_falcon_furia_sesto_rxc_dfe_tap13_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap13():0));
    ESTM(lane_st->dfe[12][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap9_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap9():(rd_falcon_furia_sesto_rxd_dfe_tap13_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap13():0));
    ESTM(lane_st->dfe[13][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap10_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap10():(rd_falcon_furia_sesto_rxa_dfe_tap14_mux()==0)?rd_falcon_furia_sesto_rxa_dfe_tap14():0));
    ESTM(lane_st->dfe[13][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap10_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap10():(rd_falcon_furia_sesto_rxb_dfe_tap14_mux()==0)?rd_falcon_furia_sesto_rxb_dfe_tap14():0));
    ESTM(lane_st->dfe[13][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap10_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap10():(rd_falcon_furia_sesto_rxc_dfe_tap14_mux()==0)?rd_falcon_furia_sesto_rxc_dfe_tap14():0));
    ESTM(lane_st->dfe[13][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap10_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap10():(rd_falcon_furia_sesto_rxd_dfe_tap14_mux()==0)?rd_falcon_furia_sesto_rxd_dfe_tap14():0));
    ESTM(lane_st->dfe[14][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap7_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap7():(rd_falcon_furia_sesto_rxa_dfe_tap11_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap11():0));
    ESTM(lane_st->dfe[14][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap7_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap7():(rd_falcon_furia_sesto_rxb_dfe_tap11_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap11():0));
    ESTM(lane_st->dfe[14][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap7_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap7():(rd_falcon_furia_sesto_rxc_dfe_tap11_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap11():0));
    ESTM(lane_st->dfe[14][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap7_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap7():(rd_falcon_furia_sesto_rxd_dfe_tap11_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap11():0));
    ESTM(lane_st->dfe[15][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap8_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap8():(rd_falcon_furia_sesto_rxa_dfe_tap12_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap12():0));
    ESTM(lane_st->dfe[15][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap8_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap8():(rd_falcon_furia_sesto_rxb_dfe_tap12_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap12():0));
    ESTM(lane_st->dfe[15][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap8_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap8():(rd_falcon_furia_sesto_rxc_dfe_tap12_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap12():0));
    ESTM(lane_st->dfe[15][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap8_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap8():(rd_falcon_furia_sesto_rxd_dfe_tap12_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap12():0));
    ESTM(lane_st->dfe[16][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap9_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap9():(rd_falcon_furia_sesto_rxa_dfe_tap13_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap13():0));
    ESTM(lane_st->dfe[16][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap9_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap9():(rd_falcon_furia_sesto_rxb_dfe_tap13_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap13():0));
    ESTM(lane_st->dfe[16][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap9_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap9():(rd_falcon_furia_sesto_rxc_dfe_tap13_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap13():0));
    ESTM(lane_st->dfe[16][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap9_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap9():(rd_falcon_furia_sesto_rxd_dfe_tap13_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap13():0));
    ESTM(lane_st->dfe[17][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap10_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap10():(rd_falcon_furia_sesto_rxa_dfe_tap14_mux()==1)?rd_falcon_furia_sesto_rxa_dfe_tap14():0));
    ESTM(lane_st->dfe[17][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap10_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap10():(rd_falcon_furia_sesto_rxb_dfe_tap14_mux()==1)?rd_falcon_furia_sesto_rxb_dfe_tap14():0));
    ESTM(lane_st->dfe[17][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap10_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap10():(rd_falcon_furia_sesto_rxc_dfe_tap14_mux()==1)?rd_falcon_furia_sesto_rxc_dfe_tap14():0));
    ESTM(lane_st->dfe[17][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap10_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap10():(rd_falcon_furia_sesto_rxd_dfe_tap14_mux()==1)?rd_falcon_furia_sesto_rxd_dfe_tap14():0));
    ESTM(lane_st->dfe[18][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap7_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap7():(rd_falcon_furia_sesto_rxa_dfe_tap11_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap11():0));
    ESTM(lane_st->dfe[18][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap7_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap7():(rd_falcon_furia_sesto_rxb_dfe_tap11_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap11():0));
    ESTM(lane_st->dfe[18][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap7_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap7():(rd_falcon_furia_sesto_rxc_dfe_tap11_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap11():0));
    ESTM(lane_st->dfe[18][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap7_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap7():(rd_falcon_furia_sesto_rxd_dfe_tap11_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap11():0));
    ESTM(lane_st->dfe[19][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap8_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap8():(rd_falcon_furia_sesto_rxa_dfe_tap12_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap12():0));
    ESTM(lane_st->dfe[19][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap8_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap8():(rd_falcon_furia_sesto_rxb_dfe_tap12_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap12():0));
    ESTM(lane_st->dfe[19][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap8_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap8():(rd_falcon_furia_sesto_rxc_dfe_tap12_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap12():0));
    ESTM(lane_st->dfe[19][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap8_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap8():(rd_falcon_furia_sesto_rxd_dfe_tap12_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap12():0));
    ESTM(lane_st->dfe[20][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap9_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap9():(rd_falcon_furia_sesto_rxa_dfe_tap13_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap13():0));
    ESTM(lane_st->dfe[20][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap9_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap9():(rd_falcon_furia_sesto_rxb_dfe_tap13_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap13():0));
    ESTM(lane_st->dfe[20][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap9_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap9():(rd_falcon_furia_sesto_rxc_dfe_tap13_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap13():0));
    ESTM(lane_st->dfe[20][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap9_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap9():(rd_falcon_furia_sesto_rxd_dfe_tap13_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap13():0));
    ESTM(lane_st->dfe[21][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap10_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap10():(rd_falcon_furia_sesto_rxa_dfe_tap14_mux()==2)?rd_falcon_furia_sesto_rxa_dfe_tap14():0));
    ESTM(lane_st->dfe[21][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap10_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap10():(rd_falcon_furia_sesto_rxb_dfe_tap14_mux()==2)?rd_falcon_furia_sesto_rxb_dfe_tap14():0));
    ESTM(lane_st->dfe[21][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap10_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap10():(rd_falcon_furia_sesto_rxc_dfe_tap14_mux()==2)?rd_falcon_furia_sesto_rxc_dfe_tap14():0));
    ESTM(lane_st->dfe[21][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap10_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap10():(rd_falcon_furia_sesto_rxd_dfe_tap14_mux()==2)?rd_falcon_furia_sesto_rxd_dfe_tap14():0));
    ESTM(lane_st->dfe[22][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap11_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap11():0));
    ESTM(lane_st->dfe[22][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap11_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap11():0));
    ESTM(lane_st->dfe[22][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap11_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap11():0));
    ESTM(lane_st->dfe[22][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap11_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap11():0));
    ESTM(lane_st->dfe[23][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap12_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap12():0));
    ESTM(lane_st->dfe[23][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap12_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap12():0));
    ESTM(lane_st->dfe[23][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap12_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap12():0));
    ESTM(lane_st->dfe[23][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap12_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap12():0));
    ESTM(lane_st->dfe[24][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap13_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap13():0));
    ESTM(lane_st->dfe[24][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap13_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap13():0));
    ESTM(lane_st->dfe[24][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap13_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap13():0));
    ESTM(lane_st->dfe[24][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap13_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap13():0));
    ESTM(lane_st->dfe[25][0] = ((rd_falcon_furia_sesto_rxa_dfe_tap14_mux()==3)?rd_falcon_furia_sesto_rxa_dfe_tap14():0));
    ESTM(lane_st->dfe[25][1] = ((rd_falcon_furia_sesto_rxb_dfe_tap14_mux()==3)?rd_falcon_furia_sesto_rxb_dfe_tap14():0));
    ESTM(lane_st->dfe[25][2] = ((rd_falcon_furia_sesto_rxc_dfe_tap14_mux()==3)?rd_falcon_furia_sesto_rxc_dfe_tap14():0));
    ESTM(lane_st->dfe[25][3] = ((rd_falcon_furia_sesto_rxd_dfe_tap14_mux()==3)?rd_falcon_furia_sesto_rxd_dfe_tap14():0));
    ESTM(lane_st->thctrl_dp[0] = rd_falcon_furia_sesto_rxa_slicer_offset_adj_dp());
    ESTM(lane_st->thctrl_dp[1] = rd_falcon_furia_sesto_rxb_slicer_offset_adj_dp());
    ESTM(lane_st->thctrl_dp[2] = rd_falcon_furia_sesto_rxc_slicer_offset_adj_dp());
    ESTM(lane_st->thctrl_dp[3] = rd_falcon_furia_sesto_rxd_slicer_offset_adj_dp());
    ESTM(lane_st->thctrl_dn[0] = rd_falcon_furia_sesto_rxa_slicer_offset_adj_dn());
    ESTM(lane_st->thctrl_dn[1] = rd_falcon_furia_sesto_rxb_slicer_offset_adj_dn());
    ESTM(lane_st->thctrl_dn[2] = rd_falcon_furia_sesto_rxc_slicer_offset_adj_dn());
    ESTM(lane_st->thctrl_dn[3] = rd_falcon_furia_sesto_rxd_slicer_offset_adj_dn());
    ESTM(lane_st->thctrl_zp[0] = rd_falcon_furia_sesto_rxa_slicer_offset_adj_zp());
    ESTM(lane_st->thctrl_zp[1] = rd_falcon_furia_sesto_rxb_slicer_offset_adj_zp());
    ESTM(lane_st->thctrl_zp[2] = rd_falcon_furia_sesto_rxc_slicer_offset_adj_zp());
    ESTM(lane_st->thctrl_zp[3] = rd_falcon_furia_sesto_rxd_slicer_offset_adj_zp());
    ESTM(lane_st->thctrl_zn[0] = rd_falcon_furia_sesto_rxa_slicer_offset_adj_zn());
    ESTM(lane_st->thctrl_zn[1] = rd_falcon_furia_sesto_rxb_slicer_offset_adj_zn());
    ESTM(lane_st->thctrl_zn[2] = rd_falcon_furia_sesto_rxc_slicer_offset_adj_zn());
    ESTM(lane_st->thctrl_zn[3] = rd_falcon_furia_sesto_rxd_slicer_offset_adj_zn());
    ESTM(lane_st->thctrl_l[0] = rd_falcon_furia_sesto_rxa_slicer_offset_adj_lms());
    ESTM(lane_st->thctrl_l[1] = rd_falcon_furia_sesto_rxb_slicer_offset_adj_lms());
    ESTM(lane_st->thctrl_l[2] = rd_falcon_furia_sesto_rxc_slicer_offset_adj_lms());
    ESTM(lane_st->thctrl_l[3] = rd_falcon_furia_sesto_rxd_slicer_offset_adj_lms());
    ESTM(lane_st->heye_left = _eye_to_mUI(pa, rdv_falcon_furia_sesto_usr_sts_heye_left()));
    ESTM(lane_st->heye_right = _eye_to_mUI(pa, rdv_falcon_furia_sesto_usr_sts_heye_right()));
    ESTM(lane_st->veye_upper = _eye_to_mV(pa, rdv_falcon_furia_sesto_usr_sts_veye_upper(), 0));
    ESTM(lane_st->veye_lower = _eye_to_mV(pa, rdv_falcon_furia_sesto_usr_sts_veye_lower(), 0));
    ESTM(lane_st->link_time = (((uint32_t)rdv_falcon_furia_sesto_usr_sts_link_time())*8)/10);
    ESTM(lane_st->prbs_chk_en = rd_falcon_furia_sesto_prbs_chk_en());
    ESTM(reg_data = rd_falcon_furia_sesto_prbs_chk_mode_sel());
    lane_st->prbs_chk_order = (reg_data==0)?7:
                              (reg_data==1)?9:
                              (reg_data==2)?11:
                              (reg_data==3)?15:
                              (reg_data==4)?23:
                              (reg_data==5)?31:
                              (reg_data==6)?58:0;
    EFUN(falcon_furia_sesto_prbs_chk_lock_state( pa, &lane_st->prbs_chk_lock));
    EFUN(falcon_furia_sesto_prbs_err_count_ll( pa, &lane_st->prbs_chk_errcnt));

    if (lane_st->pmd_lock == 1) {
      if (!is_micro_stopped) {
        EFUN(falcon_furia_sesto_stop_rx_adaptation( pa, 0));
      }
    }

    return(err_code);
}

err_code_t falcon_furia_sesto_disp_full_pmd_state ( const phymod_access_t *pa, struct falcon_furia_sesto_detailed_lane_status_st *lane_st, uint8_t num_lanes) {
    uint8_t i;
    err_code_t err_code = 0;
    USR_PRINTF(("\n\n"));
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    USR_PRINTF(("Falcon PMD State\n"));
    USR_PRINTF(("%-16s\t%12s%12s%12s%12s\n", "PARAMETER","LN0","LN1","LN2","LN3"));
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LN_VARS ("HEYE Left (mUI)",heye_left,"%12d");
    DISP_LN_VARS ("HEYE Rght (mUI)",heye_right,"%12d");
    DISP_LN_VARS ("VEYE Top  (mV)",veye_upper,"%12d");
    DISP_LN_VARS ("VEYE Bott (mV)",veye_lower,"%12d");
#ifdef SERDES_API_FLOATING_POINT
    DISP_LN_VARS ("Link Time (ms)",link_time/10.0,"%12.1f");
#else
    DISP_LN_VARS ("Link Time (ms)",link_time/10,"%12d");
#endif
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LN_VARS ("TX drv_hv_disable",ams_tx_drv_hv_disable,"%12d");
    DISP_LN_VARS ("TX ana_rescal",ams_tx_ana_rescal,"%12d");
    DISP_LN_VARS ("TX amp_ctrl",amp_ctrl,"%12d");
    DISP_LN_VARS ("TX pre_tap",pre_tap,"%12d");
    DISP_LN_VARS ("TX main_tap",main_tap,"%12d");
    DISP_LN_VARS ("TX post1_tap",post1_tap,"%12d");
    DISP_LN_VARS ("TX post2_tap",post2_tap,"%12d");
    DISP_LN_VARS ("TX post3_tap",post3_tap,"%12d");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LN_VARS ("Sigdet",sigdet,"%12d");
    DISP_LN_VARS ("PMD_lock",pmd_lock,"%12d");
    DISP_LN_VARS ("DSC_SM (1st read)",dsc_sm[0],"       %4xh");
    DISP_LN_VARS ("DSC_SM (2nd read)",dsc_sm[1],"       %4xh");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
#ifdef SERDES_API_FLOATING_POINT
    DISP_LN_VARS ("PPM",ppm,"%12.2f");
#else
    DISP_LN_VARS ("PPM",ppm,"%12d");
#endif
    DISP_LN_VARS ("VGA",vga,"%12d");
    DISP_LN_VARS ("PF",pf,"%12d");
    DISP_LN_VARS ("PF2",pf2,"%12d");
#ifdef SERDES_API_FLOATING_POINT
    DISP_LN_VARS ("main_tap",main_tap_est,"%12.2f");
#else
    DISP_LN_VARS ("main_tap",main_tap_est,"%12d");
#endif
    DISP_LN_VARS ("data_thresh",data_thresh,"%12d");
    DISP_LN_VARS ("phase_thresh",phase_thresh,"%12d");
    DISP_LN_VARS ("lms_thresh",lms_thresh,"%12d");
    USR_PRINTF(("------------------------------------------------------------------------\n"));	  
    DISP_LN_VARS ("d/dq hoffset",ddq_hoffset,"%12u");
    DISP_LN_VARS ("p/pq hoffset",ppq_hoffset,"%12u");
    DISP_LN_VARS ("l/lq hoffset",llq_hoffset,"%12u");
    DISP_LN_VARS ("d/p hoffset",dp_hoffset,"%12u");
    DISP_LN_VARS ("d/l hoffset",dl_hoffset,"%12u");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LNQ_VARS("dfe[2][a,b]",dfe[1][0],dfe[1][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[2][c,d]",dfe[1][2],dfe[1][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[3][a,b]",dfe[2][0],dfe[2][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[3][c,d]",dfe[2][2],dfe[2][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[4][a,b]",dfe[3][0],dfe[3][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[4][c,d]",dfe[3][2],dfe[3][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[5][a,b]",dfe[4][0],dfe[4][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[5][c,d]",dfe[4][2],dfe[4][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[6][a,b]",dfe[5][0],dfe[5][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[6][c,d]",dfe[5][2],dfe[5][3],"%8d,%3d");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LNQ_VARS("dfe[7][a,b]",dfe[6][0],dfe[6][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[7][c,d]",dfe[6][2],dfe[6][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[8][a,b]",dfe[7][0],dfe[7][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[8][c,d]",dfe[7][2],dfe[7][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[9][a,b]",dfe[8][0],dfe[8][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[9][c,d]",dfe[8][2],dfe[8][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[10][a,b]",dfe[9][0],dfe[9][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[10][c,d]",dfe[9][2],dfe[9][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[11][a,b]",dfe[10][0],dfe[10][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[11][c,d]",dfe[10][2],dfe[10][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[12][a,b]",dfe[11][0],dfe[11][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[12][c,d]",dfe[11][2],dfe[11][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[13][a,b]",dfe[12][0],dfe[12][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[13][c,d]",dfe[12][2],dfe[12][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[14][a,b]",dfe[13][0],dfe[13][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[14][c,d]",dfe[13][2],dfe[13][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[15][a,b]",dfe[14][0],dfe[14][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[15][c,d]",dfe[14][2],dfe[14][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[16][a,b]",dfe[15][0],dfe[15][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[16][c,d]",dfe[15][2],dfe[15][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[17][a,b]",dfe[16][0],dfe[16][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[17][c,d]",dfe[16][2],dfe[16][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[18][a,b]",dfe[17][0],dfe[17][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[18][c,d]",dfe[17][2],dfe[17][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[19][a,b]",dfe[18][0],dfe[18][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[19][c,d]",dfe[18][2],dfe[18][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[20][a,b]",dfe[19][0],dfe[19][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[20][c,d]",dfe[19][2],dfe[19][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[21][a,b]",dfe[20][0],dfe[20][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[21][c,d]",dfe[20][2],dfe[20][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[22][a,b]",dfe[21][0],dfe[21][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[22][c,d]",dfe[21][2],dfe[21][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[23][a,b]",dfe[22][0],dfe[22][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[23][c,d]",dfe[22][2],dfe[22][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[24][a,b]",dfe[23][0],dfe[23][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[24][c,d]",dfe[23][2],dfe[23][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[25][a,b]",dfe[24][0],dfe[24][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[25][c,d]",dfe[24][2],dfe[24][3],"%8d,%3d");
    DISP_LNQ_VARS("dfe[26][a,b]",dfe[25][0],dfe[25][1],"%8d,%3d");
    DISP_LNQ_VARS("dfe[26][c,d]",dfe[25][2],dfe[25][3],"%8d,%3d");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LN_VARS ("dc_offset",dc_offset,"%12d");
    DISP_LNQ_VARS("data_p[a,b]",thctrl_dp[0],thctrl_dp[1],"%8d,%3d");
    DISP_LNQ_VARS("data_p[c,d]",thctrl_dp[2],thctrl_dp[3],"%8d,%3d");
    DISP_LNQ_VARS("data_n[a,b]",thctrl_dn[0],thctrl_dn[1],"%8d,%3d");
    DISP_LNQ_VARS("data_n[c,d]",thctrl_dn[2],thctrl_dn[3],"%8d,%3d");
    DISP_LNQ_VARS("phase_p[a,b]",thctrl_zp[0],thctrl_zp[1],"%8d,%3d");
    DISP_LNQ_VARS("phase_p[c,d]",thctrl_zp[2],thctrl_zp[3],"%8d,%3d");
    DISP_LNQ_VARS("phase_n[a,b]",thctrl_zn[0],thctrl_zn[1],"%8d,%3d");
    DISP_LNQ_VARS("phase_n[c,d]",thctrl_zn[2],thctrl_zn[3],"%8d,%3d");
    DISP_LNQ_VARS("lms[a,b]",thctrl_l[0],thctrl_l[1],"%8d,%3d");
    DISP_LNQ_VARS("lms[c,d]",thctrl_l[2],thctrl_l[3],"%8d,%3d");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    DISP_LN_VARS ("PRBS_CHECKER",prbs_chk_en,"%12u");
    DISP_LN_VARS ("PRBS_ORDER",prbs_chk_order,"%12u");
    DISP_LN_VARS ("PRBS_LOCK",prbs_chk_lock,"%12u");
    DISP_LN_VARS ("PRBS_ERRCNT",prbs_chk_errcnt,"%12u");
    USR_PRINTF(("------------------------------------------------------------------------\n"));
    USR_PRINTF(("\n"));

    return(err_code);
}


static err_code_t _compute_bin( const phymod_access_t *pa, char var, char bin[]) {

  switch (var) {
    case '0':  USR_STRCPY(bin,"0000");
               break;
    case '1':  USR_STRCPY(bin,"0001");
               break;
    case '2':  USR_STRCPY(bin,"0010");
               break;
    case '3':  USR_STRCPY(bin,"0011");
               break;
    case '4':  USR_STRCPY(bin,"0100");
               break;
    case '5':  USR_STRCPY(bin,"0101");
               break;
    case '6':  USR_STRCPY(bin,"0110");
               break;
    case '7':  USR_STRCPY(bin,"0111");
               break;
    case '8':  USR_STRCPY(bin,"1000");
               break;
    case '9':  USR_STRCPY(bin,"1001");
               break;
    case 'a':
    case 'A':  USR_STRCPY(bin,"1010");
               break;
    case 'b':
    case 'B':  USR_STRCPY(bin,"1011");
               break;
    case 'c':
    case 'C':  USR_STRCPY(bin,"1100");
               break;
    case 'd':
    case 'D':  USR_STRCPY(bin,"1101");
               break;
    case 'e':
    case 'E':  USR_STRCPY(bin,"1110");
               break;
    case 'f':
    case 'F':  USR_STRCPY(bin,"1111");
               break;
    case '_':  USR_STRCPY(bin,"");
               break;
    default :  USR_STRCPY(bin,"");
               USR_PRINTF(("ERROR: Invalid Hexadecimal Pattern\n"));
               return (_error(ERR_CODE_CFG_PATT_INVALID_HEX));
               break;
  }
  return (ERR_CODE_NONE);
}


static err_code_t _compute_hex( const phymod_access_t *pa, char bin[],uint8_t *hex) {
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
    USR_PRINTF(("ERROR: Invalid Binary to Hex Conversion\n"));
    *hex = 0x0;
	return (_error(ERR_CODE_CFG_PATT_INVALID_BIN2HEX));

  } 
  return (ERR_CODE_NONE);
}

/* Repeater Only APIs (Not applicable to PMD) */

/* TX PI setup for Repeater Mode */
static err_code_t _rptr_mode_timing( const phymod_access_t *pa ) {

    err_code_t err_code;

    /* Enable TX PI and turn on 1st order loop in jitter filter */
    wr_falcon_furia_sesto_tx_pi_repeater_mode_en(0x1);
    wr_falcon_furia_sesto_tx_pi_en(0x1);                                   /* TX_PI enable: 0 = diabled, 1 = enabled */
    wr_falcon_furia_sesto_tx_pi_jitter_filter_en(0x1);                     /* Jitter filter enable to lock freq: 0 = diabled, 1 = enabled */
    err_code = falcon_furia_sesto_delay_us(25);                     /* Wait for TX_PI to settle */
    return(err_code);
  }

/* Setup the Ultra low latency clk and datapath [Return Val = Error Code (0 = PASS)] */
static err_code_t _ull_mode_setup( const phymod_access_t *pa ) {

    err_code_t err_code = 0;

      wr_falcon_furia_sesto_ams_rx_data12t_ll_en(0x1);                     /* Enable RX ULL datapath */
      wr_falcon_furia_sesto_ams_tx_pdet_mode(0x1);                         /* Randomize bit[0] */
      wr_falcon_furia_sesto_ams_tx_pdet_ll_clk(0x1);                       /* New reister bit to enable ull PD mode */

    wr_falcon_furia_sesto_tx_pi_hs_fifo_phserr_invert(0x0);
    wr_falcon_furia_sesto_tx_pi_ext_phase_bwsel_integ(0x7);                /* 0 to 7 */
    wr_falcon_furia_sesto_tx_pi_ext_ctrl_en(0x1);                          /* Turn on jitter filter's phase detector input path  */

    /* Wait for phase detector path to settle */
    err_code = falcon_furia_sesto_delay_us(1024); 

    if (err_code) {
      return(err_code);
    }

    wr_falcon_furia_sesto_tx_pi_second_order_loop_en(0x1);                 /* Turn on 2nd order loop in jitter filter */

      wr_falcon_furia_sesto_ams_tx_fifo_depth(0x6);

    wr_falcon_furia_sesto_afe_tx_fifo_resetb(0x1);                         /* Release user reset to FIFO */
                                                        /* When the correct event occurs, reset is released to AFE's actual FIFO */
    err_code = falcon_furia_sesto_poll_st_afe_tx_fifo_resetb_equals_1( pa, 1);  /* Check if FIFO reset is reported to be released within 1ms time interval */
    if (err_code) {
      return (err_code);
    }

      wr_falcon_furia_sesto_ams_tx_sel_ll(0x1);                            /* Switch the tx data mux to ULL traffic and check for data integrity */

    return (ERR_CODE_NONE);
  }

  

/*************************************************/
/*  RAM access through Micro Register Interface  */
/*************************************************/

 
err_code_t _falcon_display_event(const phymod_access_t *pa,uint8_t event_id,
	                uint8_t entry_len,
				    uint8_t prev_cursor,
				    uint8_t curr_cursor,
				    uint8_t post_cursor,
					uint8_t *supp_info) {
    char *s1, *s2, *s3;
	int ii=0;

	switch (event_id) {                                                       /* decode event code */
	case EVENT_CODE_ENTRY_TO_DSC_RESET:
		USR_PRINTF(("  Entry to DSC reset"));
		break;
	case EVENT_CODE_RELEASE_USER_RESET:
		USR_PRINTF(("  Release user reset"));
		break;
	case EVENT_CODE_EXIT_FROM_DSC_RESET:
		USR_PRINTF(("  Exit from DSC reset"));
		break;
	case EVENT_CODE_ENTRY_TO_CORE_RESET:
		USR_PRINTF(("  Entry to core reset"));
		break;
	case EVENT_CODE_RELEASE_USER_CORE_RESET:
		USR_PRINTF(("  Release user core reset"));
		break;
	case EVENT_CODE_ACTIVE_RESTART_CONDITION:
		USR_PRINTF(("  Active restart condition"));
		break;
	case EVENT_CODE_EXIT_FROM_RESTART:
		USR_PRINTF(("  Exit from restart"));
		break;
	case EVENT_CODE_WRITE_TR_COARSE_LOCK:
		USR_PRINTF(("  Write timing recovery coarse lock\n"));
		break;
	case EVENT_CODE_CL72_READY_FOR_COMMAND:
		s1 = _status_val_2_str( pa, prev_cursor);
		s2 = _status_val_2_str( pa, curr_cursor);
		s3 = _status_val_2_str( pa, post_cursor);
		if (entry_len == 4) {
			USR_PRINTF(("  Cl72 ready for command"));
		} else {
			USR_PRINTF(("  Cl72 ready for command, prev command returned (%s, %s, %s)", s1, s2, s3 ));
		}
        USR_UNUSED_VAR(s1);
        USR_UNUSED_VAR(s2);
        USR_UNUSED_VAR(s3);
		break;
	case EVENT_CODE_EACH_WRITE_TO_CL72_TX_CHANGE_REQUEST:
		s1 = _update_val_2_str( pa, prev_cursor);
		s2 = _update_val_2_str( pa, curr_cursor);
		s3 = _update_val_2_str( pa, post_cursor);
		if (entry_len == 4) {
			USR_PRINTF(("  Write to Cl72 transmit change request"));
		} else {
			USR_PRINTF(("  Write to Cl72 transmit change request (%s, %s, %s)", s1, s2, s3 ));
		}
        USR_UNUSED_VAR(s1);
        USR_UNUSED_VAR(s2);
        USR_UNUSED_VAR(s3);
		break;
	case EVENT_CODE_REMOTE_RX_READY:
		USR_PRINTF(("  Remote Rx ready"));
		break;
	case EVENT_CODE_LOCAL_RX_TRAINED:
		USR_PRINTF(("  Local Rx trained"));
		break;
	case EVENT_CODE_DSC_LOCK:
		USR_PRINTF(("  DSC lock"));
		break;
	case EVENT_CODE_FIRST_RX_PMD_LOCK:
		USR_PRINTF(("  Rx PMD lock"));
		break;
	case EVENT_CODE_PMD_RESTART_FROM_CL72_CMD_INTF_TIMEOUT:
		USR_PRINTF(("  PMD restart due to CL72 ready for command timeout"));
		break;
	case EVENT_CODE_LP_RX_READY:
		USR_PRINTF(("  Remote receiver ready in CL72"));
		break;
	case EVENT_CODE_STOP_EVENT_LOG:
		USR_PRINTF(("  Start reading event log"));
		break;
	case EVENT_CODE_GENERAL_EVENT_0:
		USR_PRINTF(("  General event 0, (0x%x%x)",post_cursor,prev_cursor));
		break;
	case EVENT_CODE_GENERAL_EVENT_1:
		USR_PRINTF(("  General event 1, (0x%x%x)",post_cursor,prev_cursor));
		break;
	case EVENT_CODE_GENERAL_EVENT_2:
		USR_PRINTF(("  General event 2, (0x%x%x)",post_cursor,prev_cursor));
		break;
    case EVENT_CODE_ERROR_EVENT:
        s1 = _error_val_2_str( pa, post_cursor);
        USR_PRINTF(("  UC error event: %s", s1));
        USR_UNUSED_VAR(s1);
        break;
    case EVENT_CODE_NUM_TIMESTAMP_WRAPAROUND_MAXOUT:
        USR_PRINTF(("  Reset number of timestamp wraparounds"));
        break;
    case EVENT_CODE_RESTART_PMD_ON_CDR_LOCK_LOST:
        USR_PRINTF(("  Restart Rx PMD on CDR lock lost"));
        break;
    case EVENT_CODE_RESTART_PMD_ON_CLOSE_EYE:
        USR_PRINTF(("  Restart Rx PMD because of closed eye"));
        break;
    case EVENT_CODE_RESTART_PMD_ON_DFE_TAP_CONFIG:
        USR_PRINTF(("  Restart Rx PMD on maxed out DFE tap magnitude"));
        break;
    case EVENT_CODE_SM_STATUS_RESTART:
        USR_PRINTF(("  Check DSC SM status restart reg value"));
        break;
    case EVENT_CODE_CORE_PROGRAMMING:
        USR_PRINTF(("  Program core config value"));
        break;
    case EVENT_CODE_LANE_PROGRAMMING:
        USR_PRINTF(("  Program lane config value"));
        break;
    case EVENT_CODE_CL72_AUTO_POLARITY_CHANGE:
        USR_PRINTF(("  CL72 Auto Polarity Change"));
        break;
    case EVENT_CODE_RESTART_FROM_CL72_MAX_TIMEOUT:
        USR_PRINTF(("  Restart Rx PMD due to F-CL72 training timeout"));
        break;

    default:
		USR_PRINTF(("  UNRECOGNIZED EVENT CODE (0x%x) !!!",event_id));
		break;
	}

	if (entry_len == 4) {
		USR_PRINTF(("\n"));
	}
	else {
		USR_PRINTF((", SUP_INFO={"));
		supp_info += (entry_len-5);
		for (ii=0; ii<entry_len-4; ii++) {
			if (ii != 0) {
				USR_PRINTF((", "));
			}
			USR_PRINTF(("0x%x",*supp_info));
			supp_info--;
		}
		USR_PRINTF(("}\n"));
		if ((event_id >= EVENT_CODE_MAX) && (event_id < EVENT_CODE_TIMESTAMP_WRAP_AROUND)) {
			/* newline for unrecognized event */
			USR_PRINTF(("\n"));
		}
	}

  return(ERR_CODE_NONE);
}


static char* _status_val_2_str( const phymod_access_t *pa, uint8_t val) {
	switch (val) {
	case 3:
		return ("MAX");
	case 2:
		return ("MIN");
	case 1:
		return ("UPDATED");
	case 0:
		return ("NOT_UPDATED");
	default:
		return ("UNDEFINED");
	}
}

static char* _update_val_2_str( const phymod_access_t *pa, uint8_t val) {
	switch (val) {
	case 2:
		return ("DEC");
	case 1:
		return ("INC");
	case 0:
		return ("HOLD");
	default:
		return ("UNDEFINED");
	}
}

static char* _error_val_2_str( const phymod_access_t *pa, uint8_t val) {
	switch (val) {
	case 3:
		return ("INVALID OTP CONFIGURATION");
	case 2:
		return ("DSC CONFIG INVALID REENTRY");
	case 1:
		return ("INVALID REENTRY");
	case 0:
		return ("GENERIC UC ERROR");
	default:
		return ("UNDEFINED");
	}
}

#ifdef TO_FLOATS
/* convert uint32_t to float8 */
static float8_t _int32_to_float8(uint32_t input) {
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
static uint32_t _float8_to_int32(float8_t input) {
	uint32_t x;
	if(input == 0) return(0);
	x = (input>>5) + 8;
	if((input & 0x1F) < 3) {
		return(x>>(3-(input & 0x1f)));
	} 
	return(x<<((input & 0x1F)-3));
}

/* convert float12 to uint32 */
static uint32_t _float12_to_uint32( const phymod_access_t *pa, uint8_t byte, uint8_t multi) {

	return(((uint32_t)byte)<<multi);
}


static err_code_t _set_rx_pf_main( const phymod_access_t *pa, uint8_t val) {
	if (val > 15) {
		return (_error(ERR_CODE_PF_INVALID));
	}
	wr_falcon_furia_sesto_rx_pf_ctrl(val);
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_pf_main( const phymod_access_t *pa, int8_t *val) {
		ESTM(*val = (int8_t)rd_falcon_furia_sesto_rx_pf_ctrl());
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_pf2( const phymod_access_t *pa, uint8_t val) {
	if (val > 7) {
		return (_error(ERR_CODE_PF_INVALID));
	}
	wr_falcon_furia_sesto_rx_pf2_ctrl(val);
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_pf2( const phymod_access_t *pa, int8_t *val) {
		ESTM(*val = (int8_t)rd_falcon_furia_sesto_rx_pf2_ctrl());
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_vga( const phymod_access_t *pa, uint8_t val) {

    EFUN(_check_uc_lane_stopped( pa ));                     /* make sure uC is stopped to avoid race conditions */

	if (val > 39) {
		return (_error(ERR_CODE_VGA_INVALID));
	} 
	wr_falcon_furia_sesto_rx_vga_ctrl_val(val);
	wr_falcon_furia_sesto_vga_write(1);
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_vga( const phymod_access_t *pa, int8_t *val) {
		ESTM(*val = (int8_t)rd_falcon_furia_sesto_rx_vga_ctrl());
 return (ERR_CODE_NONE);
}


SDK_STATIC err_code_t _set_rx_dfe1( const phymod_access_t *pa, int8_t val) {

    EFUN(_check_uc_lane_stopped( pa ));                     /* make sure uC is stopped to avoid race conditions */

	wr_falcon_furia_sesto_data_thresh_sel_val(val);
	wr_falcon_furia_sesto_data_thresh_write(1);
	return(ERR_CODE_NONE); 
}
SDK_STATIC err_code_t _get_rx_dfe1( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = (int8_t)rd_falcon_furia_sesto_rx_data_thresh_sel());
 return (ERR_CODE_NONE);
}


SDK_STATIC err_code_t _set_rx_dfe2( const phymod_access_t *pa, int8_t val) {
	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap2(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap2(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap2(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap2(val); 
	return(ERR_CODE_NONE); 
}

SDK_STATIC err_code_t _get_rx_dfe2( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = (int8_t)((rd_falcon_furia_sesto_rxa_dfe_tap2() + rd_falcon_furia_sesto_rxb_dfe_tap2() + rd_falcon_furia_sesto_rxc_dfe_tap2() + rd_falcon_furia_sesto_rxd_dfe_tap2())>>2));
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_dfe3( const phymod_access_t *pa, int8_t val) {
	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap3(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap3(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap3(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap3(val); 
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_dfe3( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap3() + rd_falcon_furia_sesto_rxb_dfe_tap3() + rd_falcon_furia_sesto_rxc_dfe_tap3() + rd_falcon_furia_sesto_rxd_dfe_tap3())>>2);
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_dfe4( const phymod_access_t *pa, int8_t val) {
	if ((val > 7) || (val < -7))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap4(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap4(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap4(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap4(val); 
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_dfe4( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap4() + rd_falcon_furia_sesto_rxb_dfe_tap4() + rd_falcon_furia_sesto_rxc_dfe_tap4() + rd_falcon_furia_sesto_rxd_dfe_tap4())>>2);
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_dfe5( const phymod_access_t *pa, int8_t val) {
	if ((val > 7) || (val < -7))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap5(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap5(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap5(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap5(val); 
	return(ERR_CODE_NONE); 
}

static err_code_t _get_rx_dfe5( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap5() + rd_falcon_furia_sesto_rxb_dfe_tap5() + rd_falcon_furia_sesto_rxc_dfe_tap5() + rd_falcon_furia_sesto_rxd_dfe_tap5())>>2);
 return (ERR_CODE_NONE);
}

static err_code_t _set_rx_dfe6( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap6(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap6(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap6(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap6(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe6( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap6() + rd_falcon_furia_sesto_rxb_dfe_tap6() + rd_falcon_furia_sesto_rxc_dfe_tap6() + rd_falcon_furia_sesto_rxd_dfe_tap6())>>2);
	return (ERR_CODE_NONE);
}

static err_code_t _set_rx_dfe7( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap7(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap7(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap7(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap7(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe7( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap7() + rd_falcon_furia_sesto_rxb_dfe_tap7() + rd_falcon_furia_sesto_rxc_dfe_tap7() + rd_falcon_furia_sesto_rxd_dfe_tap7())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe8( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap8(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap8(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap8(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap8(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe8( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap8() + rd_falcon_furia_sesto_rxb_dfe_tap8() + rd_falcon_furia_sesto_rxc_dfe_tap8() + rd_falcon_furia_sesto_rxd_dfe_tap8())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe9( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap9(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap9(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap9(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap9(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe9( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap9() + rd_falcon_furia_sesto_rxb_dfe_tap9() + rd_falcon_furia_sesto_rxc_dfe_tap9() + rd_falcon_furia_sesto_rxd_dfe_tap9())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe10( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap10(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap10(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap10(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap10(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe10( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap10() + rd_falcon_furia_sesto_rxb_dfe_tap10() + rd_falcon_furia_sesto_rxc_dfe_tap10() + rd_falcon_furia_sesto_rxd_dfe_tap10())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe11( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap11(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap11(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap11(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap11(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe11( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap11() + rd_falcon_furia_sesto_rxb_dfe_tap11() + rd_falcon_furia_sesto_rxc_dfe_tap11() + rd_falcon_furia_sesto_rxd_dfe_tap11())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe12( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap12(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap12(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap12(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap12(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe12( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap12() + rd_falcon_furia_sesto_rxb_dfe_tap12() + rd_falcon_furia_sesto_rxc_dfe_tap12() + rd_falcon_furia_sesto_rxd_dfe_tap12())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe13( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap13(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap13(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap13(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap13(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe13( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap13() + rd_falcon_furia_sesto_rxb_dfe_tap13() + rd_falcon_furia_sesto_rxc_dfe_tap13() + rd_falcon_furia_sesto_rxd_dfe_tap13())>>2);
 return (ERR_CODE_NONE);
}


static err_code_t _set_rx_dfe14( const phymod_access_t *pa, int8_t val) {

	if ((val > 15) || (val < -15))
		return (_error(ERR_CODE_DFE_TAP));  
	wr_falcon_furia_sesto_rxa_dfe_tap14(val); 
	wr_falcon_furia_sesto_rxb_dfe_tap14(val); 
	wr_falcon_furia_sesto_rxc_dfe_tap14(val); 
	wr_falcon_furia_sesto_rxd_dfe_tap14(val); 
	return(ERR_CODE_NONE); 
}
static err_code_t _get_rx_dfe14( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = (rd_falcon_furia_sesto_rxa_dfe_tap14() + rd_falcon_furia_sesto_rxb_dfe_tap14() + rd_falcon_furia_sesto_rxc_dfe_tap14() + rd_falcon_furia_sesto_rxd_dfe_tap14())>>2);
 return (ERR_CODE_NONE);
}

static err_code_t _set_tx_post1( const phymod_access_t *pa, uint8_t val) {

  if (val > 63) {
    return (_error(ERR_CODE_TXFIR_POST1_INVALID));  
  }
  wr_falcon_furia_sesto_cl93n72_txfir_post(val); 

  return(ERR_CODE_NONE); 
}


static err_code_t _set_tx_pre( const phymod_access_t *pa, uint8_t val) {

  if (val > 31) {
    return (_error(ERR_CODE_TXFIR_PRE_INVALID));  
  }
  wr_falcon_furia_sesto_cl93n72_txfir_pre(val); 
  return(ERR_CODE_NONE); 
}


static err_code_t _set_tx_main( const phymod_access_t *pa, uint8_t val) {

  if (val > 112) {
    return (_error(ERR_CODE_TXFIR_MAIN_INVALID));  
  }
  wr_falcon_furia_sesto_cl93n72_txfir_main(val); 
  return(ERR_CODE_NONE); 
}

static err_code_t _set_tx_post2( const phymod_access_t *pa, int8_t val) {

  if ((val > 15) || (val < -15)) {
    return (_error(ERR_CODE_TXFIR_POST2_INVALID));  
  }
  /* sign? */
  wr_falcon_furia_sesto_txfir_post2(val); 

  return(ERR_CODE_NONE); 
}


static err_code_t _get_tx_pre( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = rd_falcon_furia_sesto_txfir_pre_after_ovr());
 return (ERR_CODE_NONE);
}

static err_code_t _get_tx_main( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = rd_falcon_furia_sesto_txfir_main_after_ovr());
 return (ERR_CODE_NONE);
}

static err_code_t _get_tx_post1( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = rd_falcon_furia_sesto_txfir_post_after_ovr());
 return (ERR_CODE_NONE);
}

static err_code_t _get_tx_post2( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = rd_falcon_furia_sesto_txfir_post2_adjusted());
 return (ERR_CODE_NONE);
}

static err_code_t _set_tx_post3( const phymod_access_t *pa, int8_t val) {

  if ((val > 7) || (val < -7)) {
    return (_error(ERR_CODE_TXFIR_POST3_INVALID));  
  }
  /* sign? */
  wr_falcon_furia_sesto_txfir_post3(val); 

  return(ERR_CODE_NONE); 
}

static err_code_t _get_tx_post3( const phymod_access_t *pa, int8_t *val) {

	ESTM(*val = rd_falcon_furia_sesto_txfir_post3_adjusted());
  return(ERR_CODE_NONE); 
}

static err_code_t _set_tx_amp( const phymod_access_t *pa, int8_t val) {

 if (val > 15) {
    return (_error(ERR_CODE_TX_AMP_CTRL_INVALID));
 }

 wr_falcon_furia_sesto_ams_tx_amp_ctl(val);
  return(ERR_CODE_NONE); 
}

static err_code_t _get_tx_amp( const phymod_access_t *pa, int8_t *val) {
	ESTM(*val = rd_falcon_furia_sesto_ams_tx_amp_ctl());
  return(ERR_CODE_NONE); 
}



static err_code_t _falcon_furia_sesto_core_clkgate( const phymod_access_t *pa, uint8_t enable) {

  if (enable) {
  }
  else {
  }
  return (ERR_CODE_NONE);
}

static err_code_t _falcon_furia_sesto_lane_clkgate( const phymod_access_t *pa, uint8_t enable) {

  if (enable) {
    /* Use frc/frc_val to force all RX and TX clk_vld signals to 0 */
    wr_falcon_furia_sesto_pmd_rx_clk_vld_frc_val(0x0);
    wr_falcon_furia_sesto_pmd_rx_clk_vld_frc(0x1);
      wr_falcon_furia_sesto_pmd_tx_clk_vld_frc_val(0x0);
      wr_falcon_furia_sesto_pmd_tx_clk_vld_frc(0x1);

      /* Use frc_on to force clkgate */
      wr_falcon_furia_sesto_ln_rx_s_clkgate_frc_on(0x1);
      wr_falcon_furia_sesto_ln_tx_s_clkgate_frc_on(0x1);
    
  }
  else {
    wr_falcon_furia_sesto_pmd_rx_clk_vld_frc_val(0x0);
    wr_falcon_furia_sesto_pmd_rx_clk_vld_frc(0x0);
      wr_falcon_furia_sesto_pmd_tx_clk_vld_frc_val(0x0);
      wr_falcon_furia_sesto_pmd_tx_clk_vld_frc(0x1);

      wr_falcon_furia_sesto_ln_rx_s_clkgate_frc_on(0x0);
      wr_falcon_furia_sesto_ln_tx_s_clkgate_frc_on(0x0);
  }
  return (ERR_CODE_NONE);
}

SDK_STATIC uint16_t _eye_to_mUI(const phymod_access_t *pa, uint8_t var) 
{   
    /* var is in units of 1/512 th UI, so need to multiply by 1000/512 */
    return ((uint16_t)var)*125/64;
}


SDK_STATIC uint16_t _eye_to_mV(const phymod_access_t *pa, uint8_t var, uint8_t ladder_range) 
{       
    /* find nearest two vertical levels */
    uint16_t vl = _ladder_setting_to_mV(pa,var, ladder_range);    
    return (vl);
}

SDK_STATIC err_code_t _falcon_furia_sesto_get_osr_mode( const phymod_access_t *pa, falcon_furia_sesto_osr_mode_st *imode) {
    falcon_furia_sesto_osr_mode_st mode;

     USR_MEMSET(&mode, 0, sizeof(falcon_furia_sesto_osr_mode_st));

    if(!imode) 
        return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  ESTM(mode.tx_rx = rd_falcon_furia_sesto_osr_mode());
  mode.tx = 255;mode.rx = 255;
  *imode = mode;
  return (ERR_CODE_NONE);

}

SDK_STATIC err_code_t _falcon_furia_sesto_read_lane_state( const phymod_access_t *pa, falcon_furia_sesto_lane_state_st *istate) {  
       
  uint8_t is_micro_stopped;
  falcon_furia_sesto_lane_state_st state;
  uint8_t ladder_range = 0;
  uint16_t reg_data;

  USR_MEMSET(&state, 0, sizeof(falcon_furia_sesto_lane_state_st));  

  if(!istate) 
	  return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  ESTM(state.rx_lock = rd_falcon_furia_sesto_pmd_rx_lock());

  is_micro_stopped = 0;
  if (state.rx_lock == 1) {
    ESTM(is_micro_stopped = rdv_falcon_furia_sesto_usr_sts_micro_stopped());
    if (!is_micro_stopped) {
        EFUN(falcon_furia_sesto_stop_rx_adaptation( pa, 1));
    }
  }

  {  falcon_furia_sesto_osr_mode_st osr_mode;
  USR_MEMSET(&osr_mode, 0, sizeof(falcon_furia_sesto_osr_mode_st));  
  EFUN(_falcon_furia_sesto_get_osr_mode( pa, &osr_mode));
  state.osr_mode = osr_mode;
  }
  ESTM(state.ucv_config = rdv_falcon_furia_sesto_config_word());
  ESTM(state.sig_det = rd_falcon_furia_sesto_signal_detect());
  ESTM(state.rx_ppm = rd_falcon_furia_sesto_cdr_integ_reg()/84);
  EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_PD,&reg_data));
  state.clk90 = (uint8_t)dist_ccw(((reg_data>>8)&0xFF),(reg_data&0xFF));
  EFUN(falcon_furia_sesto_pmd_rdt_reg(pa,DSC_E_RX_PI_CNT_BIN_LD,&reg_data));
  state.clkp1 = (uint8_t)dist_ccw(((reg_data>>8)&0xFF),(reg_data&0xFF));
  ESTM(state.br_pd_en = rd_falcon_furia_sesto_br_pd_en());
  /* drop the MSB, the result is actually valid modulo 128 */
  /* Also flip the sign to account for d-m1, versus m1-d */
  state.clk90 = state.clk90<<1;
  state.clk90 = -(state.clk90>>1);
  state.clkp1 = state.clkp1<<1;
  state.clkp1 = -(state.clkp1>>1);
  
  EFUN(_get_rx_pf_main( pa, &state.pf_main));
  state.pf_hiz = 0;
  EFUN(_get_rx_pf2( pa, &state.pf2_ctrl)); 
  EFUN(_get_rx_vga( pa, &state.vga));
  ESTM(state.dc_offset = rd_falcon_furia_sesto_dc_offset_bin());
  ESTM(state.p1_lvl = rdv_falcon_furia_sesto_usr_main_tap_est()/32);
  state.p1_lvl = _ladder_setting_to_mV(pa,(int8_t)state.p1_lvl, 0);
  ESTM(state.m1_lvl = rd_falcon_furia_sesto_rx_phase_thresh_sel());
  state.m1_lvl = _ladder_setting_to_mV(pa,(int8_t)state.m1_lvl, 0);

  
  state.pf_bst = 0;

  EFUN(_get_rx_dfe1( pa, &state.dfe1));  
  EFUN(_get_rx_dfe2( pa, &state.dfe2));  
  EFUN(_get_rx_dfe3( pa, &state.dfe3));  
  EFUN(_get_rx_dfe4( pa, &state.dfe4));
  EFUN(_get_rx_dfe5( pa, &state.dfe5));
  EFUN(_get_rx_dfe6( pa, &state.dfe6));


  /* tx_ppm = register/10.84 */
  ESTM(state.tx_ppm = (int16_t)(((int32_t)(rd_falcon_furia_sesto_tx_pi_integ2_reg()))*3125/32768));

  EFUN(_get_tx_pre( pa, &state.txfir_pre));
  EFUN(_get_tx_main( pa, &state.txfir_main));
  EFUN(_get_tx_post1( pa, &state.txfir_post1));
  EFUN(_get_tx_post2( pa, &state.txfir_post2)); 
  EFUN(_get_tx_post3( pa, &state.txfir_post3));    
  ESTM(state.heye_left = _eye_to_mUI(pa, rdv_falcon_furia_sesto_usr_sts_heye_left()));
  ESTM(state.heye_right = _eye_to_mUI(pa, rdv_falcon_furia_sesto_usr_sts_heye_right()));
  ESTM(state.veye_upper = _eye_to_mV(pa, rdv_falcon_furia_sesto_usr_sts_veye_upper(), ladder_range));
  ESTM(state.veye_lower = _eye_to_mV(pa, rdv_falcon_furia_sesto_usr_sts_veye_lower(), ladder_range));
  ESTM(state.link_time = (((uint32_t)rdv_falcon_furia_sesto_usr_sts_link_time())*8)/10);     /* convert from 80us to 0.1 ms units */

  if (state.rx_lock == 1) {
    if (!is_micro_stopped)  
    EFUN(falcon_furia_sesto_stop_rx_adaptation( pa, 0));
  } 

  *istate = state;
  return (ERR_CODE_NONE);
}

static err_code_t _falcon_furia_sesto_display_lane_state_no_newline( const phymod_access_t *pa ) {     
  uint16_t lane_idx;
  falcon_furia_sesto_lane_state_st state;

  const char* e2s_osr_mode_enum[13] = {
    "x1   ",
    "x2   ",
    "x4   ",
    "ERR  ",
    "ERR  ",
    "ERR  ",
    "ERR  ",
    "ERR  ",
    "x16.5",
    "ERR  ",
    "ERR  ",
    "ERR  ",
    "x20.6"
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
    "x8",
    "xA"
  };

  char *s;

  USR_MEMSET(&state, 0, sizeof(falcon_furia_sesto_lane_state_st));

  EFUN(_falcon_furia_sesto_read_lane_state( pa, &state));
 
  lane_idx = falcon_furia_sesto_get_lane(pa); 
  USR_PRINTF(("%2d ", lane_idx));
  if(state.osr_mode.tx_rx != 255) {
      s = (char *)e2s_osr_mode_enum[state.osr_mode.tx_rx];
      USR_PRINTF(("(%2s%s,0x%02x)", (state.br_pd_en) ? "BR" : "OS", s, state.ucv_config & 0xFF));  /* Show lower byte only */
  } else {
      char *r;
      s = (char *)e2s_tx_osr_mode_enum[state.osr_mode.tx];
      r = (char *)e2s_tx_osr_mode_enum[state.osr_mode.rx];
      USR_PRINTF(("(%2s%s:%s,0x%2x)", (state.br_pd_en) ? "BR" : "OS", s,r, state.ucv_config & 0xFF));  /* Show lower byte only */
  }
  USR_PRINTF(("   %1d   %1d %4d ", state.sig_det, state.rx_lock, state.rx_ppm));
  USR_PRINTF(("  %3d   %3d ", state.clk90, state.clkp1));
  USR_PRINTF(("  %2d,%1d ", state.pf_main, state.pf2_ctrl));
  USR_PRINTF(("   %2d ", state.vga));
  USR_PRINTF(("%3d ", state.dc_offset));
  USR_PRINTF(("%4d ", state.p1_lvl));
  USR_PRINTF(("%4d ", state.m1_lvl));
  USR_PRINTF(("%3d,%3d,%3d,%3d,%3d,%3d ", state.dfe1, state.dfe2, state.dfe3, state.dfe4, state.dfe5, state.dfe6));
  USR_PRINTF((" %4d ", state.tx_ppm));
  USR_PRINTF(("  %2d,%3d,%2d,%2d,%2d  ", state.txfir_pre, state.txfir_main, state.txfir_post1, state.txfir_post2,state.txfir_post3));
  USR_PRINTF((" %3d,%3d,%3d,%3d ", state.heye_left, state.heye_right, state.veye_upper, state.veye_lower)); 
  USR_PRINTF((" %4d.%01d", state.link_time/10, state.link_time%10));
  return (ERR_CODE_NONE);
}


SDK_STATIC err_code_t _falcon_furia_sesto_read_core_state( const phymod_access_t *pa, falcon_furia_sesto_core_state_st *istate) {  

  err_code_t  err_code;
  falcon_furia_sesto_core_state_st state;
  struct falcon_furia_sesto_uc_core_config_st core_cfg;

  USR_MEMSET(&state, 0, sizeof(falcon_furia_sesto_core_state_st));
  USR_MEMSET(&core_cfg, 0, sizeof(struct falcon_furia_sesto_uc_core_config_st));

  if(!istate) 
	  return(_error(ERR_CODE_BAD_PTR_OR_INVALID_INPUT));

  err_code = falcon_furia_sesto_get_uc_core_config( pa, &core_cfg);
  if (err_code) {
    return (err_code);
  }
  ESTM(state.rescal               = rd_falcon_furia_sesto_ams_tx_ana_rescal());
  ESTM(state.core_reset           = rdc_falcon_furia_sesto_core_dp_reset_state());
  ESTM(state.pll_pwrdn            = rdc_falcon_furia_sesto_afe_s_pll_pwrdn());
  ESTM(state.uc_active            = rdc_falcon_furia_sesto_uc_active());
  ESTM(state.comclk_mhz           = rdc_falcon_furia_sesto_heartbeat_count_1us());   
  ESTM(state.ucode_version        = rdcv_falcon_furia_sesto_common_ucode_version()); 
  ESTM(state.ucode_minor_version  = rdcv_falcon_furia_sesto_common_ucode_minor_version());
  ESTM(state.afe_hardware_version = rdcv_falcon_furia_sesto_afe_hardware_version());
  ESTM(state.temp_idx             = rdcv_falcon_furia_sesto_temp_idx());
  {int16_t die_temp = 0;
     EFUN(falcon_furia_sesto_read_die_temperature( pa, &die_temp));
     state.die_temp             = die_temp;
  }
  ESTM(state.avg_tmon             = rdcv_falcon_furia_sesto_avg_tmon_reg13bit());
    state.vco_rate_mhz = (((((uint16_t) core_cfg.field.vco_rate) * 125)/2) + 14000);
  ESTM(state.analog_vco_range     = rdc_falcon_furia_sesto_ams_pll_range());  
  ESTM(state.pll_div              = rdc_falcon_furia_sesto_ams_pll_ndiv());   
  ESTM(state.pll_lock = rdc_falcon_furia_sesto_pll_lock());

  *istate = state;
  return (ERR_CODE_NONE);
}

static err_code_t _falcon_furia_sesto_display_core_state_no_newline( const phymod_access_t *pa ) {
  
  falcon_furia_sesto_core_state_st state;

  USR_MEMSET(&state, 0, sizeof(falcon_furia_sesto_core_state_st));
  
  EFUN(_falcon_furia_sesto_read_core_state( pa, &state));
  USR_PRINTF(("%02d  ",falcon_furia_sesto_get_core(pa)));
  USR_PRINTF(("   %x   ",state.core_reset));
  USR_PRINTF(("    %1d     ",state.pll_pwrdn));
  USR_PRINTF(("   %1d    ",state.uc_active));
  USR_PRINTF((" %3d.%2dMHz",state.comclk_mhz/4,((state.comclk_mhz % 4)*25))); /* comclk in Mhz = heartbeat_count_1us / 4 */
  USR_PRINTF(("   %4X_%02X ",state.ucode_version, state.ucode_minor_version));
  USR_PRINTF(("    0x%02x   ",state.afe_hardware_version));

  USR_PRINTF(("   %3dC    ",state.die_temp));
 
  {int16_t temp;
     temp = _bin_to_degC((state.avg_tmon>>3));
     USR_PRINTF(("    %3dC  ", temp));
  }
  USR_PRINTF(("     0x%02x  ", state.rescal));
  USR_PRINTF(("  %2d.%2dGHz ",state.vco_rate_mhz/1000, state.vco_rate_mhz % 1000)); 
  USR_PRINTF(("    %03d       ",state.analog_vco_range));
  USR_PRINTF(("   %02d   ",state.pll_div));
  USR_PRINTF(("      %01d   ",state.pll_lock));

  return (ERR_CODE_NONE);
}


/* returns 000111 (7 = 8-1), for n = 3  */
#define BFMASK(n) ((1<<((n)))-1)

static void _falcon_furia_sesto_update_uc_lane_config_st( const phymod_access_t *pa, struct  falcon_furia_sesto_uc_lane_config_st *st) {

  uint16_t in = st->word;

  st->field.lane_cfg_from_pcs = in & BFMASK(1); in >>= 1;
  st->field.an_enabled = in & BFMASK(1); in >>= 1;
  st->field.dfe_on = in & BFMASK(1); in >>= 1;
  st->field.dfe_lp_mode = in & BFMASK(1); in >>= 1;
  st->field.force_brdfe_on = in & BFMASK(1); in >>= 1;
  st->field.media_type = in & BFMASK(2); in >>= 2;
  st->field.unreliable_los = in & BFMASK(1); in >>= 1;
  st->field.scrambling_dis = in & BFMASK(1); in >>= 1;  
  st->field.cl72_auto_polarity_en = in & BFMASK(1); in >>= 1;
  st->field.cl72_restart_timeout_en = in & BFMASK(1); in >>= 1;
  st->field.reserved = in & BFMASK(5); in >>= 5;

}

static void _falcon_furia_sesto_update_usr_ctrl_disable_functions_st( const phymod_access_t *pa, struct falcon_furia_sesto_usr_ctrl_disable_functions_st *st) {

  uint16_t in = st->word;
  st->field.pf_adaptation = in & BFMASK(1); in >>= 1;
  st->field.pf2_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dc_adaptation = in & BFMASK(1); in >>= 1;
  st->field.vga_adaptation = in & BFMASK(1); in >>= 1;
  st->field.slicer_voffset_tuning = in & BFMASK(1); in >>= 1;
  st->field.slicer_hoffset_tuning = in & BFMASK(1); in >>= 1;
  st->field.phase_offset_adaptation = in & BFMASK(1); in >>= 1;
  st->field.eye_adaptation = in & BFMASK(1); in >>= 1;
  st->field.all_adaptation = in & BFMASK(1); in >>= 1;
  st->field.reserved = in & BFMASK(7); in >>= 7;

}


static void _falcon_furia_sesto_update_usr_ctrl_disable_dfe_functions_st( const phymod_access_t *pa, struct falcon_furia_sesto_usr_ctrl_disable_dfe_functions_st *st) {
  
  uint8_t in = st->byte;
  
  st->field.dfe_tap1_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_fx_taps_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_fl_taps_adaptation = in & BFMASK(1); in >>= 1;
  st->field.dfe_dcd_adaptation = in & BFMASK(1); in >>= 1;

}

/* word to fields, for display */
static void _falcon_furia_sesto_update_uc_core_config_st( const phymod_access_t *pa, struct falcon_furia_sesto_uc_core_config_st *st) {

  uint16_t in = st->word;

  st->field.vco_rate = in & BFMASK(8); in >>= 8;
  st->field.core_cfg_from_pcs = in & BFMASK(1); in >>= 1;
  st->field.reserved = in & BFMASK(7); in >>= 7;

}

/* fields to word, to write into uC RAM */
static void _falcon_furia_sesto_update_uc_core_config_word( const phymod_access_t *pa, struct falcon_furia_sesto_uc_core_config_st *st) {
  uint16_t in = 0;
  in <<= 7; in |= 0/*st->field.reserved*/ & BFMASK(7);
  in <<= 1; in |= st->field.core_cfg_from_pcs & BFMASK(1);
  in <<= 8; in |= st->field.vco_rate & BFMASK(8);
  st->word = in;
}

static void _falcon_furia_sesto_update_uc_lane_config_word( const phymod_access_t *pa, struct falcon_furia_sesto_uc_lane_config_st *st) {

  uint16_t in = 0;

  in <<= 5; in |= 0 /*st->field.reserved*/ & BFMASK(5);
  in <<= 1; in |= st->field.cl72_restart_timeout_en & BFMASK(1);  
  in <<= 1; in |= st->field.cl72_auto_polarity_en & BFMASK(1);  
  in <<= 1; in |= st->field.scrambling_dis & BFMASK(1);
  in <<= 1; in |= st->field.unreliable_los & BFMASK(1);
  in <<= 2; in |= st->field.media_type & BFMASK(2);
  in <<= 1; in |= st->field.force_brdfe_on & BFMASK(1);
  in <<= 1; in |= st->field.dfe_lp_mode & BFMASK(1);
  in <<= 1; in |= st->field.dfe_on & BFMASK(1);
  in <<= 1; in |= st->field.an_enabled & BFMASK(1);
  in <<= 1; in |= st->field.lane_cfg_from_pcs & BFMASK(1);

  st->word = in;
}

static void _falcon_furia_sesto_update_usr_ctrl_disable_functions_byte( const phymod_access_t *pa, struct falcon_furia_sesto_usr_ctrl_disable_functions_st *st) {


  uint16_t in = 0;
  in <<= 7; in |= 0/*st->field.reserved*/ & BFMASK(7);
  in <<= 1; in |= st->field.all_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.eye_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.phase_offset_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.slicer_hoffset_tuning & BFMASK(1);
  in <<= 1; in |= st->field.slicer_voffset_tuning & BFMASK(1);
  in <<= 1; in |= st->field.vga_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dc_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.pf2_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.pf_adaptation & BFMASK(1);
  st->word = in;

}


static void _falcon_furia_sesto_update_usr_ctrl_disable_dfe_functions_byte( const phymod_access_t *pa, struct  falcon_furia_sesto_usr_ctrl_disable_dfe_functions_st *st) {
  
  uint8_t in = 0;
  
  in <<= 1; in |= st->field.dfe_dcd_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_fl_taps_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_fx_taps_adaptation & BFMASK(1);
  in <<= 1; in |= st->field.dfe_tap1_adaptation & BFMASK(1);

  st->byte = in;
}

#undef BFMASK


SDK_STATIC err_code_t _check_uc_lane_stopped( const phymod_access_t *pa ) {

  uint8_t is_micro_stopped;
  ESTM(is_micro_stopped = rdv_falcon_furia_sesto_usr_sts_micro_stopped());
  if (!is_micro_stopped) {
      return(_error(ERR_CODE_UC_NOT_STOPPED));
  } else {
      return(ERR_CODE_NONE);
  }
}

SDK_STATIC err_code_t _calc_patt_gen_mode_sel( const phymod_access_t *pa, uint8_t *mode_sel, uint8_t *zero_pad_len, uint8_t patt_length) {

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
    USR_PRINTF(("ERROR: Unsupported Pattern Length\n"));
    return (_error(ERR_CODE_CFG_PATT_INVALID_PATT_LENGTH));
  }
  return(ERR_CODE_NONE);
}


