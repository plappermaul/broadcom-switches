/*----------------------------------------------------------------------
 * $Id: tefmod16.h,
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * $Copyright: (c) 2014 Broadcom Corporation All Rights Reserved.$
 *  Broadcom Corporation
 *  Proprietary and Confidential information
 *  All rights reserved
 *  This source file is the property of Broadcom Corporation, and
 *  may not be copied or distributed in any isomorphic form without the
 *  prior written consent of Broadcom Corporation.
 *----------------------------------------------------------------------
 *  Description: define enumerators  
 *----------------------------------------------------------------------*/
/*
 * $Id: $ 
 * $Copyright:
 * All Rights Reserved.$
 */

#ifndef _tefmod16_H_
#define _tefmod16_H_

#ifndef _DV_TB_
 #define _SDK_TEFMOD16_ 1 
#endif

#ifdef _DV_TB_
#ifdef LINUX
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errno.h"
#endif

#ifdef _SDK_TEFMOD16_
#include <phymod/phymod.h>
#include <phymod/phymod_debug.h>
#include "tefmod16_enum_defines.h"
#endif

#ifdef _SDK_TEFMOD16_
#define PHYMOD_ST const phymod_access_t
#else
#define PHYMOD_ST tefmod16_st
#endif

#define TEFMOD16_FEC_NOT_SUPRTD         0
#define TEFMOD16_FEC_SUPRTD_NOT_REQSTD  1
#define TEFMOD16_FEC_CL74_SUPRTD_REQSTD 4
#define TEFMOD16_FEC_CL91_SUPRTD_REQSTD 8

/* matching SOC_PHY_CONTROL_FEC_OFF
 *          SOC_PHY_CONTROL_FEC_ON
 *          SOC_PHY_CONTROL_FEC_AUTO
 */
#define TEFMOD16_PHY_CONTROL_FEC_OFF           0
#define TEFMOD16_PHY_CONTROL_FEC_ON            1
#define TEFMOD16_PHY_CONTROL_FEC_AUTO          2
#define TEFMOD16_PHY_CONTROL_FEC_MASK          3

/* So far 4 bits debug mask are used by TEFMOD16 */
#define TEFMOD16_DBG_MEM        (1L << 4) /* allocation/object */
#define TEFMOD16_DBG_REGACC     (1L << 3) /* Print all register accesses */ 
#define TEFMOD16_DBG_FUNCVALOUT (1L << 2) /* All values returned by Tier1*/
#define TEFMOD16_DBG_FUNCVALIN  (1L << 1) /* All values pumped into Tier1*/
#define TEFMOD16_DBG_FUNC       (1L << 0) /* Every time we enter a  Tier1*/

#define TEFMOD16_VCO_RATE_15G       22500


typedef enum {
    TEFMOD16_AN_MODE_CL73 = 0,
    TEFMOD16_AN_MODE_CL73BAM,
    TEFMOD16_AN_MODE_HPAM,       
    TEFMOD16_AN_MODE_NONE,
    TEFMOD16_AN_MODE_TYPE_COUNT
}tefmod16_an_mode_type_t;


typedef enum {
    TEFMOD16_AN_SET_RF_DISABLE = 0,
    TEFMOD16_AN_SET_SGMII_SPEED,
    TEFMOD16_AN_SET_SGMII_MASTER,
    TEFMOD16_AN_SET_HG_MODE,
    TEFMOD16_AN_SET_FEC_MODE,
    TEFMOD16_AN_SET_CL72_MODE,
    TEFMOD16_AN_SET_CL37_ATTR,
    TEFMOD16_AN_SET_CL48_SYNC,
    TEFMOD16_AN_SET_LK_FAIL_INHIBIT_TIMER_NO_CL72,
    TEFMOD16_AN_SET_CL73_FEC_OFF,
    TEFMOD16_AN_SET_CL73_FEC_ON,
    TEFMOD16_AN_SET_SPEED_PAUSE,
    TEFMOD16_AN_SET_TYPE_COUNT
}tefmod16_an_set_type_t;

typedef union {
    int tefmod16_an_set_rf_disable;
    int tefmod16_an_set_sgmii_speed;
    int tefmod16_an_set_sgmii_master;
    tefmod16_an_set_type_t tefmod16_an_set_cl37_attr;
}tefmod16_an_set_value_t;

typedef enum {
    TEFMOD16_CL73_10GBASE_KR1 = 0,  
    TEFMOD16_CL73_40GBASE_KR4,  
    TEFMOD16_CL73_40GBASE_CR4,  
    TEFMOD16_CL73_100GBASE_KR4,
    TEFMOD16_CL73_100GBASE_CR4,
    TEFMOD16_CL73_1GBASE_KX,
    TEFMOD16_CL73_25GBASE_KR1,
    TEFMOD16_CL73_25GBASE_CR1,
    TEFMOD16_CL73_25GBASE_KRS1,
    TEFMOD16_CL73_25GBASE_CRS1,
    TEFMOD16_CL73_50GBASE_KR2,
    TEFMOD16_CL73_50GBASE_CR2,
    TEFMOD16_CL73_SPEED_COUNT
}tefmod16_cl73_speed_t;

typedef enum {
    TEFMOD16_CL73_BAM_20GBASE_KR2 = 0,  
    TEFMOD16_CL73_BAM_20GBASE_CR2,  
    TEFMOD16_CL73_BAM_40GBASE_KR2,  
    TEFMOD16_CL73_BAM_40GBASE_CR2,
    TEFMOD16_CL73_BAM_RESERVED_1,
    TEFMOD16_CL73_BAM_RESERVED_2,
    TEFMOD16_CL73_BAM_50GBASE_KR2,  
    TEFMOD16_CL73_BAM_50GBASE_CR2,
    TEFMOD16_CL73_BAM_50GBASE_KR4,
    TEFMOD16_CL73_BAM_50GBASE_CR4,
    TEFMOD16_CL73_BAM_SPEED_COUNT
}tefmod16_cl73_bam_speed_t;

typedef enum {
    TEFMOD16_CL73_BAM_20GBASE_KR1 = 1,  
    TEFMOD16_CL73_BAM_20GBASE_CR1,  
    TEFMOD16_CL73_BAM_25GBASE_KR1,  
    TEFMOD16_CL73_BAM_25GBASE_CR1,
    TEFMOD16_CL73_BAM_SPEED1_COUNT
}tefmod16_cl73_bam_speed1_t;

typedef enum {
    TEFMOD16_NO_PAUSE = 0,  
    TEFMOD16_SYMM_PAUSE,
    TEFMOD16_ASYM_PAUSE,
    TEFMOD16_ASYM_SYMM_PAUSE,  
    TEFMOD16_AN_PAUSE_COUNT
}tefmod16_an_pause_t;

typedef enum {
  OVERRIDE_CLEAR                  =  0x0000,
  OVERRIDE_NUM_LANES              =  0x0001,
  OVERRIDE_OS_MODE                =  0x0002,
  OVERRIDE_T_FIFO_MODE            =  0x0004,
  OVERRIDE_T_ENC_MODE             =  0x0008,
  OVERRIDE_T_HG2_ENABLE           =  0x0010,
  OVERRIDE_T_PMA_BTMX_MODE_OEN    =  0x0020,
  OVERRIDE_SCR_MODE               =  0x0040,
  OVERRIDE_DESCR_MODE_OEN         =  0x0100,
  OVERRIDE_DEC_TL_MODE            =  0x0200,
  OVERRIDE_DESKEW_MODE            =  0x0400,
  OVERRIDE_DEC_FSM_MODE           =  0x0800,
  OVERRIDE_R_HG2_ENABLE           =  0x8001,
  OVERRIDE_BS_SM_SYNC_MODE_OEN    =  0x8002,
  OVERRIDE_BS_SYNC_EN_OEN         =  0x8004,
  OVERRIDE_BS_DIST_MODE_OEN       =  0x8008,
  OVERRIDE_BS_BTMX_MODE_OEN       =  0x8010,
  OVERRIDE_CL72_EN                =  0x8020,
  OVERRIDE_CLOCKCNT0              =  0xf040,
  OVERRIDE_CLOCKCNT1              =  0x8080,
  OVERRIDE_LOOPCNT0               =  0x8100,
  OVERRIDE_LOOPCNT1               =  0x8200,
  OVERRIDE_MAC_CREDITGENCNT       =  0x8400,
  OVERRIDE_NUM_LANES_DIS          =  0xff01,
  OVERRIDE_OS_MODE_DIS            =  0xff02,
  OVERRIDE_T_FIFO_MODE_DIS        =  0xff03,
  OVERRIDE_T_ENC_MODE_DIS         =  0xff04,
  OVERRIDE_T_HG2_ENABLE_DIS       =  0xff05,
  OVERRIDE_T_PMA_BTMX_MODE_OEN_DIS=  0xff06,
  OVERRIDE_SCR_MODE_DIS           =  0xff07,
  OVERRIDE_DESCR_MODE_OEN_DIS     =  0xff08,
  OVERRIDE_DEC_TL_MODE_DIS        =  0xff09,
  OVERRIDE_DESKEW_MODE_DIS        =  0xff0a,
  OVERRIDE_DEC_FSM_MODE_DIS       =  0xff0b,
  OVERRIDE_R_HG2_ENABLE_DIS       =  0xff0c,
  OVERRIDE_BS_SM_SYNC_MODE_OEN_DIS=  0xff0d,
  OVERRIDE_BS_SYNC_EN_OEN_DIS     =  0xff0e,
  OVERRIDE_BS_DIST_MODE_OEN_DIS   =  0xff0f,
  OVERRIDE_BS_BTMX_MODE_OEN_DIS   =  0xff10,
  OVERRIDE_CL72_EN_DIS            =  0xff11,
  OVERRIDE_CLOCKCNT0_DIS          =  0xff12,
  OVERRIDE_CLOCKCNT1_DIS          =  0xff13,
  OVERRIDE_LOOPCNT0_DIS           =  0xff14,
  OVERRIDE_LOOPCNT1_DIS           =  0xff15,
  OVERRIDE_MAC_CREDITGENCNT_DIS   =  0xff16
} override_type_t;

typedef enum {
    TEFMOD16_HG2_CODEC_OFF = 0,  
    TEFMOD16_HG2_CODEC_ON_8BYTE_IPG,
    TEFMOD16_HG2_CODEC_ON_9BYTE_IPG,
    TEFMOD16_HG2_CODEC_COUNT
}tefmod16_hg2_codec_t;

/**
\struct tefmod16_an_adv_ability_s
TBD
*/

typedef struct tefmod16_an_adv_ability_s{
  tefmod16_cl73_speed_t an_base_speed; 
  tefmod16_cl73_bam_speed_t an_bam_speed; 
  tefmod16_cl73_bam_speed1_t an_bam_speed1; 
  uint16_t an_nxt_page; 
  tefmod16_an_pause_t an_pause; 
  uint16_t an_fec; 
  uint16_t an_cl72;
  uint16_t an_hg2; 
}tefmod16_an_adv_ability_t;

typedef struct tefmod16_an_ability_s{
  tefmod16_an_adv_ability_t cl73_adv; /*includes cl73 and cl73-bam related */
}tefmod16_an_ability_t;

/**
\struct tefmod16_an_oui_s
  oui                : New oui
  override_bam73_adv : Adv new OUI for BAM73
  override_bam73_det : Detect new OUI for BAM73
  override_hpam_adv  : Adv new OUI for HPAM
  override_hpam_det  : Detect new OUI for HPAM
**/
typedef struct tefmod16_an_oui_s {
  uint32_t oui;
  uint16_t oui_override_bam73_adv;
  uint16_t oui_override_bam73_det;
  uint16_t oui_override_hpam_adv;
  uint16_t oui_override_hpam_det;
} tefmod16_an_oui_t;

typedef struct tefmod16_an_control_s {
  tefmod16_an_type_t an_type; 
  uint16_t num_lane_adv; 
  uint16_t enable;
  uint16_t pd_kx_en;
  an_property_enable  an_property_type;
  uint16_t cl72_config_allowed; 
} tefmod16_an_control_t;

/**
\struct tefmod16_an_init_s

This embodies all parameters required to program autonegotiation features in the
PHY for both CL37 and CL73 type autonegotiation and the BAM variants.
For details of individual fields, please refer to microarchitectural doc.
*/
typedef struct tefmod16_an_init_s{
  uint16_t  an_fail_cnt;
  uint16_t  linkfailtimer_dis; 
  uint16_t  linkfailtimerqua_en; 
  uint16_t  an_good_check_trap; 
  uint16_t  an_good_trap; 
  uint16_t  bam73_adv_oui;  
  uint16_t  bam73_det_oui;  
  uint16_t  hpam_adv_oui;  
  uint16_t  hpam_det_oui;  
  uint16_t  disable_rf_report; 
  uint16_t  cl73_remote_fault; 
  uint16_t  cl73_nonce_match_over;
  uint16_t  cl73_nonce_match_val;
  uint16_t  cl73_transmit_nonce; 
  uint16_t  base_selector;
} tefmod16_an_init_t;

typedef enum {
    TEFMOD16_NO_CL91_FEC = 0,
    TEFMOD16_CL91_SINGLE_LANE_FC,
    TEFMOD16_CL91_SINGLE_LANE_BRCM_PROP,
    TEFMOD16_CL91_DUAL_LANE_BRCM_PROP,
    TEFMOD16_CL91_QUAD_LANE,
    TEFMOD16_IEEE_25G_CL91_SINGLE_LANE,
    TEFMOD16_CL91_COUNT
}tefmod16_cl91_fec_mode_t;

typedef enum {
    TEFMOD16_NO_OVERRIDE = 0,
    TEFMOD16_NO_FEC,
    TEFMOD16_FEC_CL74,
    TEFMOD16_FEC_CL91
}tefmod16_an_fec_sel_override_t;


/**
\struct tefmod16_an_timers_s
TBD
*/
typedef struct tefmod16_an_timers_s{
  uint16_t  value;
} tefmod16_an_timers_t;


#ifdef CHECKIFNEEDED

extern int get_mapped_speed(tefmod16_spd_intfc_type spd_intf, int *speed);
extern int get_actual_speed(int speed_id, int *speed);

#endif
extern int phymod_debug_check(uint32_t flags, PHYMOD_ST *pc);

extern int configure_st_entry(int st_entry_num, int st_entry_speed, PHYMOD_ST* pc);

extern int tefmod16_pmd_osmode_set(PHYMOD_ST* pc, tefmod16_spd_intfc_type_t spd_intf, phymod_ref_clk_t refclk, int os_mode);
extern int tefmod16_pmd_x4_reset(PHYMOD_ST* pc);
extern int tefmod16_pmd_x4_reset_get(PHYMOD_ST* pc, int *is_in_reset);
extern int tefmod16_tx_loopback_get(PHYMOD_ST* pc, uint32_t *enable);
extern int tefmod16_firmware_set(PHYMOD_ST* pc);
extern int tefmod16_tx_lane_disable (PHYMOD_ST* pc, int tx);
extern int tefmod16_init_pcs_ilkn(PHYMOD_ST* pc);


#ifdef CHECKIFNEEDED

extern int get_mapped_speed(tefmod16_spd_intfc_type spd_intf, int *speed);
extern int get_actual_speed(int speed_id, int *speed);

#endif
extern int phymod_debug_check(uint32_t flags, PHYMOD_ST *pc);

extern int configure_st_entry(int st_entry_num, int st_entry_speed, PHYMOD_ST* pc);

extern int tefmod16_pmd_osmode_set(PHYMOD_ST* pc, tefmod16_spd_intfc_type_t spd_intf, phymod_ref_clk_t refclk, int os_mode);
extern int tefmod16_pmd_x4_reset(PHYMOD_ST* pc);
extern int tefmod16_tx_loopback_get(PHYMOD_ST* pc, uint32_t *enable);
extern int tefmod16_firmware_set(PHYMOD_ST* pc);
extern int tefmod16_tx_lane_disable (PHYMOD_ST* pc, int tx);
extern int tefmod16_init_pcs_ilkn(PHYMOD_ST* pc);
extern int tefmod16_pcs_ilkn_chk(PHYMOD_ST* pc, int *ilkn_set);
extern int tefmod16_pmd_reset_bypass (PHYMOD_ST* pc, int pmd_reset_control);
extern int tefmod16_afe_speed_up_dsc_vga(PHYMOD_ST* pc);
extern int tefmod16_get_plldiv(PHYMOD_ST* pc, uint32_t *plldiv_r_val);
extern int tefmod16_update_port_mode_select(PHYMOD_ST* pc, tefmod16_port_type_t port_type, int master_port, int tsc_clk_freq_pll_by_48, int pll_reset_en);
extern int tefmod16_set_port_mode(PHYMOD_ST* pc, int refclk, int plldiv, tefmod16_port_type_t port_type, int master_port, int tsc_clk_freq_pll_by_48, int pll_reset_en);
extern int tefmod16_tx_loopback_control(PHYMOD_ST* pc, int pcs_gloop_en, int starting_lane, int num_lanes);
extern int tefmod16_tx_pmd_loopback_control(PHYMOD_ST* pc, int pmd_gloop_en);
extern int tefmod16_rx_loopback_control(PHYMOD_ST* pc, int pcs_rloop_en);
extern int tefmod16_rx_pmd_loopback_control(PHYMOD_ST* pc, int pcs_rloop_en, int pmd_rloop_en, int lane);
extern int tefmod16_credit_set(PHYMOD_ST* pc);
extern int tefmod16_encode_set(PHYMOD_ST* pc, int per_lane_control, tefmod16_spd_intfc_type_t spd_intf, int tx_am_timer_init_val);
extern int tefmod16_lf_rf_control(PHYMOD_ST* pc);
extern int tefmod16_decode_set(PHYMOD_ST* pc);
extern int tefmod16_trigger_speed_change(PHYMOD_ST* pc);
extern int tefmod16_set_override_0(PHYMOD_ST* pc, int per_field_override_en);
extern int tefmod16_set_override_1(PHYMOD_ST* pc, int per_lane_control, uint32_t per_field_override_en);
extern int tefmod16_credit_control(PHYMOD_ST* pc, int per_lane_control);
extern int tefmod16_tx_lane_control_set(PHYMOD_ST* pc, tefmod16_tx_disable_enum_t dis);
extern int tefmod16_tx_lane_control_get(PHYMOD_ST* pc,  int *reset, int *enable);
extern int tefmod16_rx_lane_control(PHYMOD_ST* pc, int accData, int per_lane_control);
extern int tefmod16_bypass_sc(PHYMOD_ST* pc);
extern int tefmod16_revid_read(PHYMOD_ST* pc, uint32_t *revIdV);
extern int tefmod16_clause72_control(PHYMOD_ST* pc, int per_lane_control);
extern int tefmod16_prbs_check(PHYMOD_ST* pc, int accData);
extern int tefmod16_prbs_mode(PHYMOD_ST* pc, int per_lane_control);
extern int tefmod16_prbs_control(PHYMOD_ST* pc, int per_lane_control);
extern int tefmod16_cjpat_crpat_control(PHYMOD_ST* pc);
extern int tefmod16_cjpat_crpat_check(PHYMOD_ST* pc);
extern int tefmod16_tx_bert_control(PHYMOD_ST* pc);
extern int tefmod16_FEC_control(PHYMOD_ST* pc, tefmod16_fec_type_t fec_type, int enable, int cl74or91);
extern int tefmod16_FEC_get(PHYMOD_ST* pc,  tefmod16_fec_type_t fec_type, int *fec_en);
extern int tefmod16_power_control(PHYMOD_ST* pc, int tx, int rx);
extern int tefmod16_duplex_control(PHYMOD_ST* pc);
extern int tefmod16_disable_pcs_falcon(PHYMOD_ST* pc);
extern int tefmod16_pmd_addr_lane_swap(PHYMOD_ST *pc, int per_lane_control);
extern int tefmod16_pmd_lane_swap_tx(PHYMOD_ST *pc, int per_lane_control);
extern int tefmod16_pmd_reset_remove(PHYMOD_ST *pc, int pmd_touched);
extern int tefmod16_pcs_lane_swap(PHYMOD_ST *pc, int per_lane_control);
extern int tefmod16_init_pcs_falcon(PHYMOD_ST* pc);
extern int tefmod16_init_pmd_falcon(PHYMOD_ST* pc);
extern int tefmod16_set_sc_speed(PHYMOD_ST* pc);
extern int tefmod16_poll_for_sc_done(PHYMOD_ST* pc, int mapped_speed);
extern int tefmod16_check_status(PHYMOD_ST* pc);
extern int tefmod16_set_port_mode_sel(PHYMOD_ST* pc, int tsc_touched, tefmod16_port_type_t port_type);
extern int tefmod16_init_pcs_fs(PHYMOD_ST* pc);
extern int tefmod16_init_pcs_an(PHYMOD_ST* pc);
extern int tefmod16_set_an_override(PHYMOD_ST* pc);
extern int tefmod16_tx_squelch_get(PHYMOD_ST *pc, int *val);
extern int tefmod16_refclk_get(PHYMOD_ST* pc, phymod_ref_clk_t* ref_clk);
extern int tefmod16_refclk_set(PHYMOD_ST* pc, phymod_ref_clk_t ref_clock);

extern int tefmod16_get_mapped_speed(tefmod16_spd_intfc_type_t spd_intf, int *speed);
extern int tefmod16_toggle_sw_speed_change(PHYMOD_ST* pc);

extern int tefmod16_autoneg_get(PHYMOD_ST* pc);
#ifdef _DV_TB_
extern int tefmod16_autoneg_set(PHYMOD_ST* pc);
extern int tefmod16_autoneg_control(PHYMOD_ST* pc);
extern int tefmod16_set_spd_intf(PHYMOD_ST* pc);
extern int tefmod16_set_an_port_mode(PHYMOD_ST* pc);
#endif /* _DV_TB_ */
#ifdef _SDK_TEFMOD16_
int tefmod16_set_an_port_mode(PHYMOD_ST* pc, int num_of_lanes, int starting_lane, int single_port);
extern int tefmod16_set_an_single_port_mode(PHYMOD_ST* pc, int an_enable); 
extern int tefmod16_set_spd_intf(PHYMOD_ST *pc, tefmod16_spd_intfc_type_t spd_intf);
extern int tefmod16_autoneg_set_init(PHYMOD_ST* pc, tefmod16_an_init_t *an_init_st);
extern int tefmod16_autoneg_timer_init(PHYMOD_ST* pc);
extern int tefmod16_autoneg_control(PHYMOD_ST* pc, tefmod16_an_control_t *an_control);
extern int tefmod16_autoneg_set(PHYMOD_ST* pc, tefmod16_an_adv_ability_t *an_ability_st);
extern int tefmod16_diag(PHYMOD_ST *ws, tefmod16_diag_type_t diag_type);
extern int tefmod16_diag_disp(PHYMOD_ST *ws, char* arg);
extern int tefmod16_autoneg_control_get(PHYMOD_ST* pc, tefmod16_an_control_t *an_control, int *an_complete);
extern int tefmod16_autoneg_local_ability_get(PHYMOD_ST* pc, tefmod16_an_adv_ability_t *an_ability_st);
extern int tefmod16_autoneg_remote_ability_get(PHYMOD_ST* pc, tefmod16_an_adv_ability_t *an_ability_st);
extern int tefmod16_st_control_field_set (PHYMOD_ST* pc,uint16_t st_entry_no, override_type_t  st_control_field, uint16_t st_field_value);
extern int tefmod16_port_enable_set(PHYMOD_ST *pc, int enable);
extern int tefmod16_tx_squelch_set(PHYMOD_ST *pc, int enable);
extern int tefmod16_rx_squelch_set(PHYMOD_ST *pc, int enable);
extern int tefmod16_rx_squelch_get(PHYMOD_ST *pc, int *enable);

#endif /* _SDK_TEFMOD16_ */

extern int tefmod16_port_enable_get(PHYMOD_ST *pc, int *tx_enable, int *rx_enable);
extern int tefmod16_pll_lock_get(PHYMOD_ST* pc, int* lockStatus);
extern int tefmod16_pmd_lock_get(PHYMOD_ST* pc, uint32_t* lockStatus);
extern int tefmod16_set_pll_mode(PHYMOD_ST* pc, int pmd_touched, tefmod16_spd_intfc_type_t spd_intf, int pll_mode);   /* SET_PLL_MODE */
extern int tefmod16_speed_id_get(PHYMOD_ST* pc, int *speed_id);
extern int tefmod16_pmd_reset_seq(PHYMOD_ST* pc, int pmd_touched); /* PMD_RESET_SEQ */
extern int tefmod16_master_port_num_set( PHYMOD_ST *pc,  int port_num);
extern int tefmod16_update_port_mode( PHYMOD_ST *pa, int *pll_restart);
extern int tefmod16_pll_reset_enable_set (PHYMOD_ST *pa , int enable);
extern int tefmod16_get_pcs_link_status(PHYMOD_ST* pc, uint32_t *link);
extern int tefmod16_get_pcs_latched_link_status(PHYMOD_ST* pc, uint32_t *link);
extern int tefmod16_disable_get(PHYMOD_ST* pc, uint32_t* enable);
extern int tefmod16_disable_set(PHYMOD_ST* pc);
extern int tefmod16_enable_set(PHYMOD_ST* pc);
extern int tefmod16_plldiv_lkup_get(PHYMOD_ST* pc, tefmod16_spd_intfc_type_t spd_intf, phymod_ref_clk_t refclk, uint32_t *plldiv);
extern int tefmod16_osmode_lkup_get(PHYMOD_ST* pc, tefmod16_spd_intfc_type_t spd_intf, uint32_t *osmode);
extern int tefmod16_pcs_lane_swap_get ( PHYMOD_ST *pc,  uint32_t *tx_rx_swap);
extern int tefmod16_pmd_lane_swap_tx_get ( PHYMOD_ST *pc, uint32_t *tx_lane_map);
extern int tefmod16_rx_lane_control_set(PHYMOD_ST* pc, int enable);         /* RX_LANE_CONTROL */
extern int tefmod16_rx_lane_control_get(PHYMOD_ST* pc, int *enable);
extern int tefmod16_tx_rx_polarity_get ( PHYMOD_ST *pc, uint32_t* tx_polarity, uint32_t* rx_polarity);
extern int tefmod16_tx_rx_polarity_set ( PHYMOD_ST *pc, uint32_t tx_polarity, uint32_t rx_polarity);

int tefmod16_eee_control_set(PHYMOD_ST* pc, uint32_t enable);
int tefmod16_eee_control_get(PHYMOD_ST* pc, uint32_t* enable);
int tefmod16_cl74_chng_default (PHYMOD_ST* pc);

extern int tefmod16_an_oui_set(PHYMOD_ST* pc, tefmod16_an_oui_t oui);
extern int tefmod16_an_oui_get(PHYMOD_ST* pc, tefmod16_an_oui_t *oui);
extern int tefmod16_autoneg_status_get(PHYMOD_ST* pc, int *an_en, int *an_done);
extern int tefmod16_autoneg_lp_status_get(PHYMOD_ST* pc, tefmod16_an_adv_ability_t *an_ability_st);
extern int tefmod16_hg2_codec_set(PHYMOD_ST* pc, tefmod16_hg2_codec_t hg2_codec);
extern int tefmod16_hg2_codec_get(PHYMOD_ST* pc, tefmod16_hg2_codec_t* hg2_codec);
extern int tefmod16_pmd_lane_swap(PHYMOD_ST *pc, int tx_lane_swap, int rx_lane_swap);
extern int tefmod16_pmd_lane_swap_tx_rx_get ( PHYMOD_ST *pc, uint32_t *tx_lane_map, uint32_t *rx_lane_map);
extern int tefmod16_pcs_tx_lane_swap_get ( PHYMOD_ST *pc,  uint32_t *tx_swap);
extern int tefmod16_pcs_rx_lane_swap_get ( PHYMOD_ST *pc,  uint32_t *rx_swap);
extern int tefmod16_pmd_lane_swap(PHYMOD_ST *pc, int tx_lane_swap, int rx_lane_swap);
extern int tefmod16_pcs_tx_lane_swap(PHYMOD_ST *pc, int tx_lane_swap);
extern int tefmod16_pcs_rx_lane_swap(PHYMOD_ST *pc, int rx_lane_swap);
extern int tefmod16_pll_vco2_select(PHYMOD_ST* pc,  uint32_t vco_rate);
extern int tefmod16_default_init(PHYMOD_ST* pc);
extern int tefmod16_fec_correctable_counter_get(PHYMOD_ST* pc, uint32_t* count);
extern int tefmod16_fec_uncorrectable_counter_get(PHYMOD_ST* pc, uint32_t* count);
#endif  /*  _tefmod16_H_ */

