/*----------------------------------------------------------------------
 * $Id: temod16_defines.h,v 1.1.1.1 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 *  Broadcom Corporation
 *  Proprietary and Confidential information
 *  All rights reserved
 *  This source file is the property of Broadcom Corporation, and
 *  may not be copied or distributed in any isomorphic form without the
 *  prior written consent of Broadcom Corporation.
 *----------------------------------------------------------------------
 *  Description: define enumerators and temod16_st
 *----------------------------------------------------------------------*/

#ifndef _temod16_DEFINES_H_
#define _temod16_DEFINES_H_

#include "temod16_enum_defines.h"

/*! \struct temod16_st

This is the central TSCE context storing information of a single TE.
It has multiple sections, each with a set of elementtsc

\li Address Context: Elements to uniquely access a TSCE 

*/

typedef struct {
#if defined (_DV_TB_)
  /* ===== BEGIN ===== TSCE port address section */
  /*! chip number of the chip where the TSCE resides, defined by MDK/SDK
   * platforms. It is not user in verilog and customer software platforms. */
  int unit; 
  /*! Port no. defined in MDK/SDK platforms. It is not used in verilog and 
   * customer software platforms. */
  int port;
   /*! Port address strap value. Used only in TSCE verif. env. to identify
    * the TSCE to the station manager */
  int prt_ad;
#endif
  /*! lanes to access. Can simultaneously select multiple lanes. */
  temod16_lane_select   lane_select; 
  /*! Current lane being processed can be 0, 1, 2 or 3. */
  int this_lane;
  /*! May not need this. Legacy. Ignore lane number implications*/
  int adjust_port_mode;
  /*! Tier1 Features are controlled by adding values to this var. Currently we
   * manage with 32 bits (int type). If more bits are needed to specify a
   * feature,more feature control variables will be added or this will be
   * turned into an array. In general we try to use at most 8 bits per lane. */
  int                 per_lane_control; 

  int  pcs_gloop;
  int  pmd_gloop;
  int  pcs_rloop;

  int  pmd_reset_control;

  int  pkt_number;
  int  pkt_size;
  int  ipg_size;

  /*! see doc. on temod16_spd_intfc_type */
  temod16_spd_intfc_type spd_intf; 
  /*! Serial Bus protocol or PCB */
  temod16_regacc_type     regacc_type; 
  /*! combo or independent */
  temod16_port_type     port_type; 
  /*! Single copy registers cannot be modified repeatedly. Tier1 init function
   * access these registers has to do this only once between resets in
   * independent mode when the same Tier1 is called for multiple lanes. This
   * sticky bit, remembers if the single copy regs. were already modified. */
  int                 tsc_touched; 
  int                 pcs_lane_swap_touched; 
  int                 pmd_touched; 
  int                 uc_touched; 

  int                 master_portnum; 
  /*! Lane swap contl variable. The positional values of the 8 nibbles indicate
   * location of the logical lanes, like so. rx3_rx2_rx1_rx0_tx3_tx2_tx1_tx0.
   * Default value 0x32103210 indicates no lanes swapped. */
  int                 lane_swap; 
  /*! Address of reg. to access */
  int                 accAddr; 
  /*! Data context. Contains data to write or data that is just read. Also
   * contains revid after read_revid. */
  int                 accData; 
  /*! Various status of the link. Note check status of one link at a time. */
  int     diag_type;
  /*! Autoneg types. */
  temod16_an_type       an_type;
  /*! Autoneg parameter RF */
  int                 an_rf;
  /*! Autoneg parameter pause */
  int                 an_pause;
  /*! Autoneg parameter various speeds (bit encoded) */
  int                 an_tech_ability;
  int                 an_bam37_ability;
  /*! Autoneg parameter enable FEC */
  int                 an_fec;
  int                 an_higig2;
  int                 an_cl72;
  /*! Control information the driver prints. If set to zero, only warnings and
   * errors will be printed. If set to 1, msgs (developers discretion will be
   * printed) but not register transactions. If set to 2 transactions will be
   * printed (plus everything else).
   */
  int verbosity;
   /* last 4 bits are for TSCE state information */
  int ctrl_type;
  /*! for when user wants to provide custom refclk. Will also work for
  * standard values. The 10ths value of the frequency need not be entered. So 156.1
  * becomes 156
  */
  int refclk;
  /*! pll div value is auto computed. But override is provided. */
  int plldiv;

  int vga_frzval;
  int vga_frcfrz;
  int dfe_frzval;
  int dfe_frcfrz;
  int linkup_bert_cnt[4] ; 
  int firmware_mode ; 
  int an_ctl  ;
  int pattern_enable  ;
  int pattern_type  ;

  int prbs_inv;
  int prbs_enable;
  int prbs_check_mode;
  int real_check;

  /*! set multipipe_mode to select 100G/120G speeds */
  int cl82_multi_pipe_mode; 

  /*! set cl82_mld_phys_map to set the mode for 100G speeds */
  int cl82_mld_phys_map; 

  
  int tx_am_timer_init_val;
  int rx_am_timer_init_val;

  /* SC Table Entry  */
  int sc_mode;                      /* choose the override modes to pick the
                                        speed fields */
  int speed;                        /* Speed                            */
  int num_lanes;                    /* Number of lanes                  */

  int pll_mode;                     /* PLL mode                         */

  int t_pma_os_mode;                /* T_PMA over-sampling mode         */
  int t_scr_mode;                   /* T_SCR Scrambler mode             */
  int cl72_en;                      /* T_CL&@ enable                     */
  int fec_en;                       /* T_FEC enable                     */
  int t_encode_mode;                /* TX Encode mode                   */

  int r_descr1_mode;                /* R_DESC1 Descrambler mode         */
  int r_dec1_mode;                  /* R_DEC1 Decode mode               */
  int r_deskew_mode;                /* R_DESKEW mode                    */
  int r_desc2_mode;                 /* R_DESC2 mode                     */
  int r_desc2_byte_deletion;        /* R_DESC2 byte-deletion mode       */
  int r_dec1_brcm64b66_descr;       /* R_DEC1 Descramble mode           */

  int cl48_check_end;               /* CL48 link check end              */
  int sigdet_filter;                /* Signal detect mode               */
  int blk_sync_mode;                /* Block Sync Mode                  */
  int r_reorder_mode;               /* R_REORDER mode                   */
  int cl36_en;                      /* CL36_EN mode                   */

  int sgmii;
  int clkcnt0;
  int clkcnt1;
  int lpcnt0;
  int lpcnt1;
  int mac_cgc;
  int pcs_repcnt;
  int pcs_crdten;
  int pcs_clkcnt;
  int pcs_cgc;
  int uc_active;
  /*CL37 capabilities*/
  int cl37_an_pause; 
  int cl37_an_hd; 
  int cl37_an_fd; 
  int cl37_an_np; 
  int an_tick_override; 
  int an_tick_denominator; 
  int an_tick_numerator; 
  int an_fail_cnt; 
  int an_oui_ctrl; 
  int cl37_sgmii_speed; 
  int cl37_sgmii_duplex; 
  int cl37_sgmii_master_mode; 
  int cl37_bam_code; 
  int cl37_bam_ovr1g_en; 
  int cl37_bam_ovr1g_pgcnt; 
  int cl37_bam_speed; 
  int an_abilities_CL72; 
  int cl37_bam_fec; 
  int cl37_bam_hg2; 
  int cl37_bam_to_sgmii_auto_enable; 
  int sgmii_to_cl37_auto_enable; 
  int cl37_bam_en; 
  int cl37_sgmii_en; 
  int cl37_an_en; 
  int cl37_an_restart; 
  int cl73_an_en;
  int cl73_bam_en;
  int cl73_hpam_en;
  int cl73_an_restart;
  int cl73_bam_to_hpam_auto_enable;
  int hpam_to_cl73_auto_enable;
  int cl73_bam_speed;
  int cl73_bam_code;
  int cl73_speed;
  int cl73_nxt_page;
  int cl73_fec;
  int cl73_pause;
  int cl73_remote_fault;
  int transmit_nonce;
  int base_selector;
  int cl73_nonce_match_over;
  int cl73_nonce_match_val;
  int oui_upper_data;
  int oui_lower_data;
  int cl37_restart_timer_period;
  int cl37_ack_timer_period;
  int cl37_error_timer_period;
  int cl73_break_link_timer_period;
  int cl73_error_timer_period;
  int link_fail_inhibit_timer_cl72_period;
  int link_fail_inhibit_timer_not_cl72_period;
  int pd_sd_timer_period;
  int cl72_max_wait_timer;
  int cl72_wait_timer;
  int an_ignore_link_timer;
  int bam_spd_pri_5_0;
  int bam_spd_pri_11_6;
  int bam_spd_pri_17_12;
  int bam_spd_pri_23_18;
  int bam_spd_pri_29_24;
  int bam_spd_pri_35_30;
  int no_of_lanes;
  int pd_kx_en;
  int pd_kx4_en;
  int an_pd_to_cl37_enable;
  int cl37_high_vco;
  int cl73_low_vco;
  int single_port_mode;
  int pd_to_cl37_retry_cnt;
  int pd_to_cl37_wait_timer;
  int disable_rf_report;
  int bit_rep_native_mode;
  int temod16_use_higig2_knobs ;
  int tx_higig2_en;
  int tx_higig2_codec;
  int rx_higig2_en;
  int rx_higig2_codec;
  int linkfailtimer_dis;
  int linkfailtimerqua_en;
  int an_good_check_trap;
  int an_good_trap;
  int tx_reset_count;
  int credit_sel;
  int scramble_idle_tx_en;
  int scramble_idle_rx_en;
} temod16_st;

/* function table */
typedef int (*tier1FuncTable)(temod16_st* c);

#endif  /*  _temod16_DEFINES_H_ */
