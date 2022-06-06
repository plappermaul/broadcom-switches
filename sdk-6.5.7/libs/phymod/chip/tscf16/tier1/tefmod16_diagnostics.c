/*----------------------------------------------------------------------
 * $Id: tefmod16_diagnostics.c,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * $Copyright: (c) 2014 Broadcom Corporation All Rights Reserved.$
 * Broadcom Corporation
 * Proprietary and Confidential information
 * Copyright: (c) 2012 Broadcom Corp.
 * All rights reserved
 * This source file is the property of Broadcom Corporation, and
 * may not be copied or distributed in any isomorphic form without the
 * prior written consent of Broadcom Corporation.
 *---------------------------------------------------------------------
 * File       : tefmod16_diagnostics.c
 * Description: C sequences which utilize tier1s to build configurations
 *              which target only the diagnostic features of the Serdes 
 *-----------------------------------------------------------------------*/
#ifndef _SDK_TEFMOD16_
#ifndef _DV_TB_
 #define _SDK_TEFMOD16_ 1
#endif
#endif

#ifdef _DV_TB_
#include "tefmod_main.h"
#include "tefmod_defines.h"
#include "tefmod_phyreg.h"
#include <math.h>
#endif

#ifdef _DV_TB_
#include "bcmi_tscf_xgxs_defs.h"
#endif

#ifdef _SDK_TEFMOD16_
#include <phymod/chip/bcmi_tscf_16nm_xgxs_defs.h>
#include "tefmod16_enum_defines.h"
#endif

#define _TRGO_(a,l) PHYMOD_DEBUG_ERROR(("<< PHY_TRG: Adr:%06d Ln:%02d\n",a,l));
#define _TRGC_(a,l) PHYMOD_DEBUG_ERROR((">> PHY TRG: Adr:%06d Ln:%02d\n",a,l));
#ifdef _SDK_TEFMOD16_
  #define PHYMOD_ST const phymod_access_t
  #define _TRG_OPN(a) _TRGO_(a->addr, a->lane_mask)
  #define _TRG_CLS(a) _TRGC_(a->addr, a->lane_mask)
#endif
#ifdef _DV_TB_
  #define PHYMOD_ST  tefmod_st
  #define _TRG_OPN(a) _TRGO_(a->prt_ad, a->this_lane)
  #define _TRG_CLS(a) _TRGC_(a->prt_ad, a->this_lane)
#endif

extern int tefmod16_diag_st(PHYMOD_ST *pc, int resolved_speed);

/**
@brief   Field OVR diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Field OVR Details
   Check if the field OVR is set, and if OVR enabled, print the OVR value
*/
int tefmod16_diag_field_or(PHYMOD_ST *pc)
{
  SC_X4_SC_X4_OVRRr_t       num_lanes_or;
  TX_X4_MISCr_t             tx_misc_or;
  TX_X4_ENC0r_t             encode_or;
  RX_X4_PCS_CTL0r_t         rx_x4_pcs_ctrl_or;
  TX_X4_CRED0r_t            crd_or;
  TX_X4_CRED1r_t            crd1_or;
  TX_X4_LOOPCNTr_t          loopcnt_or;
  TX_X4_MAC_CREDGENCNTr_t   mac_cgc_or;


  READ_SC_X4_SC_X4_OVRRr(pc,&num_lanes_or);

  READ_RX_X4_PCS_CTL0r(pc,&rx_x4_pcs_ctrl_or);

  READ_TX_X4_MISCr(pc,&tx_misc_or);
  READ_TX_X4_ENC0r(pc,&encode_or);
  READ_TX_X4_CRED0r(pc,&crd_or);
  READ_TX_X4_CRED1r(pc,&crd1_or);
  READ_TX_X4_LOOPCNTr(pc,&loopcnt_or);
  READ_TX_X4_MAC_CREDGENCNTr(pc,&mac_cgc_or);

  PHYMOD_DEBUG_ERROR(("|                          OEN SET OVR VALUE                                   |\n"));
  PHYMOD_DEBUG_ERROR(("+----------------+----------------+--------------------------------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| NUM LANES: 0x%x | FEC ENA  : 0x%X | %-43s|\n",
    SC_X4_SC_X4_OVRRr_NUM_LANES_OVERRIDE_VALUEf_GET(num_lanes_or),
    TX_X4_MISCr_FEC_ENABLEf_GET(tx_misc_or),
    e2s_tefmod16_scr_mode[TX_X4_MISCr_SCR_MODEf_GET(tx_misc_or)]+6));
  
  PHYMOD_DEBUG_ERROR(("|                | %-15s| %-43s|\n",
    e2s_tefmod16_os_mode_type[TX_X4_MISCr_OS_MODEf_GET(tx_misc_or)]+6,
    e2s_tefmod16_encode_mode[TX_X4_ENC0r_T_ENC_MODEf_GET(encode_or)]+6));
  
  PHYMOD_DEBUG_ERROR(("| TXLN RSTB: 0x%X | TX LN ENA: 0x%X | %-43s|\n",
    TX_X4_MISCr_RSTB_TX_LANEf_GET(tx_misc_or),
    TX_X4_MISCr_ENABLE_TX_LANEf_GET(tx_misc_or),
    e2s_tefmod16_descrambler_mode[RX_X4_PCS_CTL0r_DESCR_MODEf_GET(rx_x4_pcs_ctrl_or)]+6));
  
  PHYMOD_DEBUG_ERROR(("| CLKCNT0  : 0x%x | CLKCNT1  : 0x%X | %-43s|\n",
    TX_X4_CRED0r_CLOCKCNT0f_GET(crd_or),
    TX_X4_CRED1r_CLOCKCNT1f_GET(crd1_or),
    e2s_tefmod16_dec_tl_mode[RX_X4_PCS_CTL0r_DEC_TL_MODEf_GET(rx_x4_pcs_ctrl_or)]+6));
  
  PHYMOD_DEBUG_ERROR(("| LP CNT0  : 0x%x | LP CNT1  : 0x%X | %-43s|\n",
    TX_X4_LOOPCNTr_LOOPCNT0f_GET(loopcnt_or),
    TX_X4_LOOPCNTr_LOOPCNT1f_GET(loopcnt_or),
    e2s_tefmod16_deskew_mode[RX_X4_PCS_CTL0r_DESKEW_MODEf_GET(rx_x4_pcs_ctrl_or)]+6));
  
  PHYMOD_DEBUG_ERROR(("| MACCRDGEN: 0x%x | CL74FECEN: %d   | %-43s|\n",
    TX_X4_MAC_CREDGENCNTr_MAC_CREDITGENCNTf_GET(mac_cgc_or),
    RX_X4_PCS_CTL0r_FEC_ENABLEf_GET(rx_x4_pcs_ctrl_or),
    e2s_tefmod16_dec_fsm_mode[RX_X4_PCS_CTL0r_DEC_FSM_MODEf_GET(rx_x4_pcs_ctrl_or)]+6));
  

  PHYMOD_DEBUG_ERROR(("+----------------+--+-------------+------+------------------+------------------+\n"));

  return PHYMOD_E_NONE;
}

/**
@brief   pmd pcs lane cfg diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Get the uc controls for pmd lane cfg
  Read the pcs_lane_cfg information from uc RAM. To check if the pmd is to be
  programmed from pcs lane cfg.
*/
int tefmod16_diag_pmd_pcs_lane_cfg(PHYMOD_ST *pc, int lane)
{
  _TRG_OPN(pc);
  _TRG_CLS(pc);
  return PHYMOD_E_NONE;
} /* tefmod16_diag_pmd_pcs_lane_cfg */

/**
@brief   Speed diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Speed Details
 1. Get the Speed set
 2. Get the Speed valis and Speed Done Status
 3. Get the final stats of the speed
*/
int tefmod16_diag_speed(PHYMOD_ST *pc)
{
  SC_X4_STSr_t sc_ctrl_status;
  SC_X4_RSLVD_SPDr_t sc_final_resolved_speed;
  SC_X4_RSLVD0r_t sc_final_resolved_0;
  SC_X4_RSLVD1r_t sc_final_resolved_1;
  SC_X4_RSLVD2r_t sc_final_resolved_2;
  SC_X4_RSLVD3r_t sc_final_resolved_3;
  SC_X4_RSLVD4r_t sc_final_resolved_4;
  MAIN0_SETUPr_t setup_reg;
  PLL_CAL_CTL7r_t pll_div_reg;

  PHYMOD_DEBUG_ERROR(("| TRG ADR : %06d LANE: %02d    SPEED                                           |\n", pc->addr, pc->lane_mask));
  PHYMOD_DEBUG_ERROR(("+---------------------+-----------------------------+-----------+--------------+\n"));

  /*1. Get the Speed valid and Speed Done Status */
  READ_SC_X4_STSr(pc,&sc_ctrl_status);

  /*2. Get the Speed set */
  /*2a PLLDIV and the refclk settings */
  READ_SC_X4_RSLVD_SPDr(pc,&sc_final_resolved_speed);
  READ_MAIN0_SETUPr(pc,&setup_reg);
  READ_PLL_CAL_CTL7r(pc,&pll_div_reg);

  /*2b Check if the Speed id is existent in ST */
  tefmod16_diag_st(pc, SC_X4_RSLVD_SPDr_SPEEDf_GET(sc_final_resolved_speed));
  tefmod16_diag_field_or(pc);

  /*3. Get the final stats for the speed */
  READ_SC_X4_RSLVD0r(pc,&sc_final_resolved_0);
  READ_SC_X4_RSLVD2r(pc,&sc_final_resolved_2);


  READ_SC_X4_RSLVD1r(pc,&sc_final_resolved_1);

  READ_SC_X4_RSLVD3r(pc,&sc_final_resolved_3);
  READ_SC_X4_RSLVD4r(pc,&sc_final_resolved_4);


  PHYMOD_DEBUG_ERROR(("|        SPEED      |        STATS0      |      STATS1      |   CREDIT STATS   |\n"));
  PHYMOD_DEBUG_ERROR(("+-------------------+--------------------+------------------+------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| SPD CHG VLD  %d    |     OS MODE %-6s | DESCR MODE %-6s|                  |\n",
     SC_X4_STSr_SW_SPEED_CONFIG_VLDf_GET(sc_ctrl_status),
     e2s_tefmod16_os_mode_type[SC_X4_RSLVD0r_OS_MODEf_GET(sc_final_resolved_0)]+18,
     e2s_tefmod16_descrambler_mode[SC_X4_RSLVD1r_DESCR_MODEf_GET(sc_final_resolved_1)]+20));

  PHYMOD_DEBUG_ERROR(("| SPD CHG DONE %d    |SCR MODE %-6s|DECODE MODE %-6s|  CLK CNT0 : %04d |\n",
     SC_X4_STSr_SW_SPEED_CHANGE_DONEf_GET(sc_ctrl_status),
     e2s_tefmod16_scr_mode[SC_X4_RSLVD0r_SCR_MODEf_GET(sc_final_resolved_0)]+15,
     e2s_tefmod16_dec_tl_mode[SC_X4_RSLVD1r_DEC_TL_MODEf_GET(sc_final_resolved_1)]+19,
     SC_X4_RSLVD2r_CLOCKCNT0f_GET(sc_final_resolved_2)));

  PHYMOD_DEBUG_ERROR(("| SPD RESOLVED %04d |    ENC MODE %-6s |DESKEW MODE %-6s|  CLK CNT1 : %04d |\n",
     SC_X4_RSLVD_SPDr_SPEEDf_GET(sc_final_resolved_speed),
     e2s_tefmod16_encode_mode[SC_X4_RSLVD0r_T_ENC_MODEf_GET(sc_final_resolved_0)]+18,
     e2s_tefmod16_deskew_mode[SC_X4_RSLVD1r_DESKEW_MODEf_GET(sc_final_resolved_1)]+20,
     SC_X4_RSLVD3r_CLOCKCNT1f_GET(sc_final_resolved_3)));

  PHYMOD_DEBUG_ERROR(("| #LN RESOLVED %04d |BS SM SYNC MODE %d   |DECFSM MODE %-6s| LP CNT0 : %04d |\n",
     SC_X4_RSLVD_SPDr_NUM_LANESf_GET(sc_final_resolved_speed),
     SC_X4_RSLVD1r_BS_SM_SYNC_MODEf_GET(sc_final_resolved_1),
     e2s_tefmod16_dec_fsm_mode[SC_X4_RSLVD1r_DEC_FSM_MODEf_GET(sc_final_resolved_1)]+17,
     SC_X4_RSLVD4r_LOOPCNT0f_GET(sc_final_resolved_4)));

  PHYMOD_DEBUG_ERROR(("| PLL DIV      %04d |   CL72  ENA : %02d   | REF CLOCK   :%04d | LP CNT1 : %04d |\n",
     PLL_CAL_CTL7r_PLL_MODEf_GET(pll_div_reg),
     SC_X4_RSLVD0r_CL72_ENf_GET(sc_final_resolved_0),
     MAIN0_SETUPr_REFCLK_SELf_GET(setup_reg),
     SC_X4_RSLVD4r_LOOPCNT1f_GET(sc_final_resolved_4)));

  PHYMOD_DEBUG_ERROR(("+-------------------+--------------------+------------------+------------------+\n"));

  return PHYMOD_E_NONE;
}

/**
@brief   Autoneg diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Autoneg diags
 1.  Get the AN complete status for AN37/73
 2.  Get the resolved HCD
 3.  Get the link up status after AN
*/
int tefmod16_diag_autoneg(PHYMOD_ST *pc)
{

  AN_X4_CL73_CFGr_t         an_cfg;         
  AN_X4_LD_UP1_ABIL0r_t     up1_abil0;     
  AN_X4_LD_UP1_ABIL1r_t     up1_abil1;    
  AN_X4_LD_BASE_ABIL0r_t    base_abil0;  
  AN_X4_LD_BASE_ABIL1r_t    base_abil1; 
  AN_X4_LD_BAM_ABILr_t      bam_abil;
  AN_X4_CL73_CTLSr_t        an_ctrl;
  uint16_t                  base_sel, lanes;

  READ_AN_X4_CL73_CFGr      (pc, &an_cfg);  
  READ_AN_X4_LD_UP1_ABIL0r  (pc, &up1_abil0);
  READ_AN_X4_LD_UP1_ABIL1r  (pc, &up1_abil1);
  READ_AN_X4_LD_BASE_ABIL0r (pc, &base_abil0);
  READ_AN_X4_LD_BASE_ABIL1r (pc, &base_abil1);
  READ_AN_X4_LD_BAM_ABILr   (pc, &bam_abil);
  READ_AN_X4_CL73_CTLSr     (pc, &an_ctrl);

  PHYMOD_DEBUG_ERROR(("+-------------------+-------+------+-----------------------+-------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| ANX4 CFG  : 0x%04X | UP1 ABIL0 : 0x%04X | UP1 ABIL1 : 0x%04X                 |\n",  
	AN_X4_CL73_CFGr_GET(an_cfg), AN_X4_LD_UP1_ABIL0r_GET(up1_abil0), AN_X4_LD_UP1_ABIL1r_GET(up1_abil1)));
  PHYMOD_DEBUG_ERROR(("| BASE ABIL0: 0x%04X | BASE ABIL1: 0x%04X | BAM ABIL :0x%04X CTLS:0x%04X       |\n",  
        AN_X4_LD_BASE_ABIL0r_GET(base_abil0), AN_X4_LD_BASE_ABIL1r_GET(base_abil1),
        AN_X4_LD_BAM_ABILr_GET(bam_abil), AN_X4_CL73_CTLSr_GET(an_ctrl)));
  
  lanes = AN_X4_CL73_CFGr_NUM_ADVERTISED_LANESf_GET(an_cfg);
  PHYMOD_DEBUG_ERROR(("+-------------------+--------------+----+--------------------------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| BAM ENA       : %d | HPAM ENA     : %d  | NUM ADV LANES : %d                    |\n",
        AN_X4_CL73_CFGr_CL73_BAM_ENABLEf_GET(an_cfg),
        AN_X4_CL73_CFGr_CL73_HPAM_ENABLEf_GET(an_cfg),
        (lanes == 0? 1 : (lanes == 1? 2 : (lanes == 3? 10 : 1)))));

  PHYMOD_DEBUG_ERROR(("| AN RESTART    : %d | BAM2HPAM ENA : %d  | FAIL COUNT LIM: %d                    |\n",
        AN_X4_CL73_CFGr_CL73_AN_RESTARTf_GET(an_cfg),
        AN_X4_CL73_CFGr_BAM_TO_HPAM_AD_ENf_GET(an_cfg),
        AN_X4_CL73_CTLSr_AN_FAIL_COUNT_LIMITf_GET(an_ctrl)));

  base_sel  = AN_X4_LD_BASE_ABIL0r_CL73_BASE_SELECTORf_GET(base_abil0);

  PHYMOD_DEBUG_ERROR(("+-------------------+-------------------+--------------------------------------+\n"));
  PHYMOD_DEBUG_ERROR(("|          UP 0/1 ABILITIES             |       BASE 0/1 ABILITIES             |\n"));
  PHYMOD_DEBUG_ERROR(("+----------+---------------+------------+-----------------+--------------------+\n"));

  PHYMOD_DEBUG_ERROR(("| HG2    :%d | BAM CODE : 0x%X         | TX NONCE : 0x%X | BASE SEL : %-7s    |\n", 
    AN_X4_LD_UP1_ABIL0r_BAM_HG2f_GET(up1_abil0),
    AN_X4_LD_BAM_ABILr_CL73_BAM_CODEf_GET(bam_abil),
    AN_X4_LD_BASE_ABIL0r_TX_NONCEf_GET(base_abil0),
       (base_sel == 0? "Rsvd" : 
        (base_sel == 1? "802.3" : 
         (base_sel == 2? "802.9" : 
          (base_sel == 3? "802.5": 
           (base_sel == 4? "1394": "UNKNOWN"))))) ));

  PHYMOD_DEBUG_ERROR(("| 50G CR4:%d | 40G CR2:%d | 20G CR2 :%d | PAUSE    : %d     | FEC : %d  RMT FLT : %d |\n",
    AN_X4_LD_UP1_ABIL0r_BAM_50G_CR4f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL0r_BAM_40G_CR2f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL0r_BAM_20G_CR2f_GET(up1_abil0),
    AN_X4_LD_BASE_ABIL1r_CL73_PAUSEf_GET(base_abil1),
    AN_X4_LD_BASE_ABIL1r_FEC_REQf_GET(base_abil1),
    AN_X4_LD_BASE_ABIL1r_CL73_REMOTE_FAULTf_GET(base_abil1)));

  PHYMOD_DEBUG_ERROR(("| 50G KR4:%d | 40G KR2:%d | 20G KR2 :%d | BASE 1G KX1: %d   | BASE 100G CR4 : %d    |\n",
    AN_X4_LD_UP1_ABIL0r_BAM_50G_KR4f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL0r_BAM_40G_KR2f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL0r_BAM_20G_KR2f_GET(up1_abil0),
    AN_X4_LD_BASE_ABIL1r_BASE_1G_KX1f_GET(base_abil1),
    AN_X4_LD_BASE_ABIL1r_BASE_100G_CR4f_GET(base_abil1)));

  PHYMOD_DEBUG_ERROR(("| 50G CR2:%d | 25G CR1:%d | 20G CR1 :%d | BASE 40G CR4: %d  | BASE 40G KR4 : %d     |\n", 
    AN_X4_LD_UP1_ABIL0r_BAM_50G_CR2f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL1r_BAM_25G_CR1f_GET(up1_abil1),
    AN_X4_LD_UP1_ABIL1r_BAM_20G_CR1f_GET(up1_abil1),
    AN_X4_LD_BASE_ABIL1r_BASE_40G_CR4f_GET(base_abil1),
    AN_X4_LD_BASE_ABIL1r_BASE_40G_KR4f_GET(base_abil1)));

  PHYMOD_DEBUG_ERROR(("| 50G KR2:%d | 25G KR1:%d | 20G KR1 :%d | BASE 10G KR1 : %d | NEXT PAGE   : %d      |\n", 
    AN_X4_LD_UP1_ABIL0r_BAM_50G_KR2f_GET(up1_abil0),
    AN_X4_LD_UP1_ABIL1r_BAM_25G_KR1f_GET(up1_abil1),
    AN_X4_LD_UP1_ABIL1r_BAM_20G_KR1f_GET(up1_abil1),
    AN_X4_LD_BASE_ABIL1r_BASE_10G_KR1f_GET(base_abil1),
    AN_X4_LD_BASE_ABIL1r_NEXT_PAGEf_GET(base_abil1)));

  return PHYMOD_E_NONE;
}


/**
@brief   Topology diagnostic function.
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Get the topology details
1. LoopBack
   a. Remote Loopback - RX loopback
      i.  PCS loopback
      ii. PMD loopback  
   b. Local loopback/gloop - TX loopback
      i.  PCS loopback
      ii. PMD loopback
2. LaneSwap
   a. PCS LaneSwap(RX)
   b. PMD LaneSwap(TX)
3. Master PortNum, port mode and single port mode

*/
int tefmod16_diag_topology(PHYMOD_ST *pc)
{
  MAIN0_LPBK_CTLr_t pcs_loopback_ctrl;
  TLB_RX_DIG_LPBK_CFGr_t pmd_loopback_dig;
  TLB_TX_RMT_LPBK_CFGr_t pmd_loopback_remote;
  TX_X1_TX_LN_SWPr_t pcs_tx_lane_swap;
  RX_X1_RX_LN_SWPr_t pcs_rx_lane_swap;
  LN_ADDR0r_t ln_addr0;
  LN_ADDR1r_t ln_addr1;
  LN_ADDR2r_t ln_addr2;
  LN_ADDR3r_t ln_addr3;
  MAIN0_SPD_CTLr_t pcs_spd_ctrl;
  SC_X4_SC_X4_OVRRr_t sc_x4_override;
  MAIN0_SETUPr_t reg_main0_setup;
 
  /* Main0 Setup Register */
  /*1. Loopback  */
  READ_MAIN0_LPBK_CTLr(pc, &pcs_loopback_ctrl);
  READ_TLB_RX_DIG_LPBK_CFGr(pc, &pmd_loopback_dig);
  READ_TLB_TX_RMT_LPBK_CFGr(pc, &pmd_loopback_remote);

 
   /*2. LaneSwap  */
  READ_TX_X1_TX_LN_SWPr(pc, &pcs_tx_lane_swap);
  READ_RX_X1_RX_LN_SWPr(pc, &pcs_rx_lane_swap);
  READ_LN_ADDR0r(pc, &ln_addr0); 
  READ_LN_ADDR1r(pc, &ln_addr1); 
  READ_LN_ADDR2r(pc, &ln_addr2); 
  READ_LN_ADDR3r(pc, &ln_addr3); 

 
   /* 3. Master PortNum, port mode abd single port mode */
  READ_MAIN0_SETUPr(pc, &reg_main0_setup);
 
  /* 4. FEC, PLL reset enale */
  READ_SC_X4_SC_X4_OVRRr(pc, &sc_x4_override);
  READ_MAIN0_SPD_CTLr(pc, &pcs_spd_ctrl);

  /* Print the information here */
  {
    uint16_t pcs_lcl_lb, pcs_rmt_lb, pmd_dig_lb, pmd_rmt_lb;

  pcs_lcl_lb = MAIN0_LPBK_CTLr_LOCAL_PCS_LOOPBACK_ENABLEf_GET(pcs_loopback_ctrl);
  pcs_rmt_lb = MAIN0_LPBK_CTLr_REMOTE_PCS_LOOPBACK_ENABLEf_GET(pcs_loopback_ctrl);
  pmd_dig_lb = TLB_RX_DIG_LPBK_CFGr_DIG_LPBK_ENf_GET(pmd_loopback_dig);
  pmd_rmt_lb = TLB_TX_RMT_LPBK_CFGr_RMT_LPBK_ENf_GET(pmd_loopback_remote);

  PHYMOD_DEBUG_ERROR(("\n"));
  PHYMOD_DEBUG_ERROR(("+------------------------------------------------------------------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| TRG ADR : %06d LANE: %02d     TOPOLOGY                                       |\n", pc->addr, pc->lane_mask));
  PHYMOD_DEBUG_ERROR(("+-------------------+--------------------------+---------------+---------------+\n"));
  PHYMOD_DEBUG_ERROR(("| PCSLCL LPBK: %c%c%c%c | PCS TX LANE SWAP L2P : %d%d%d%d | FEC SEL: %d  |\n",
	(pcs_lcl_lb & 8) ? 'Y':'N',
	(pcs_lcl_lb & 4) ? 'Y':'N',
	(pcs_lcl_lb & 2) ? 'Y':'N',
	(pcs_lcl_lb & 1) ? 'Y':'N',
        TX_X1_TX_LN_SWPr_LOGICAL3_TO_PHY_SELf_GET(pcs_tx_lane_swap),    
        TX_X1_TX_LN_SWPr_LOGICAL2_TO_PHY_SELf_GET(pcs_tx_lane_swap),    
        TX_X1_TX_LN_SWPr_LOGICAL1_TO_PHY_SELf_GET(pcs_tx_lane_swap),    
        TX_X1_TX_LN_SWPr_LOGICAL0_TO_PHY_SELf_GET(pcs_tx_lane_swap),    
        SC_X4_SC_X4_OVRRr_AN_FEC_SEL_OVERRIDEf_GET(sc_x4_override)));

  PHYMOD_DEBUG_ERROR(("| PCSRMT LPBK: %c%c%c%c | PCS RX LANE SWAP L2P : %d%d%d%d | PLLRST EN: %d  |\n",
	(pcs_rmt_lb & 8) ? 'Y':'N',
	(pcs_rmt_lb & 4) ? 'Y':'N',
	(pcs_rmt_lb & 2) ? 'Y':'N',
	(pcs_rmt_lb & 1) ? 'Y':'N',
        RX_X1_RX_LN_SWPr_LOGICAL3_TO_PHY_SELf_GET(pcs_rx_lane_swap),
        RX_X1_RX_LN_SWPr_LOGICAL2_TO_PHY_SELf_GET(pcs_rx_lane_swap),
        RX_X1_RX_LN_SWPr_LOGICAL1_TO_PHY_SELf_GET(pcs_rx_lane_swap),
        RX_X1_RX_LN_SWPr_LOGICAL0_TO_PHY_SELf_GET(pcs_rx_lane_swap),
        MAIN0_SPD_CTLr_PLL_RESET_ENf_GET(pcs_spd_ctrl)));

  PHYMOD_DEBUG_ERROR(("| PMDDIG LPBK: %c%c%c%c | PMD TX LANE ADDR IDX  : %d%d%d%d | PORT MODE: %d  |\n",
	(pmd_dig_lb & 8) ? 'Y':'N',
	(pmd_dig_lb & 4) ? 'Y':'N',
	(pmd_dig_lb & 2) ? 'Y':'N',
	(pmd_dig_lb & 1) ? 'Y':'N',
        LN_ADDR3r_TX_LANE_ADDR_3f_GET(ln_addr3),
        LN_ADDR2r_TX_LANE_ADDR_2f_GET(ln_addr2),
        LN_ADDR1r_TX_LANE_ADDR_1f_GET(ln_addr1),
        LN_ADDR0r_TX_LANE_ADDR_0f_GET(ln_addr0),
        MAIN0_SETUPr_PORT_MODE_SELf_GET(reg_main0_setup)));

  PHYMOD_DEBUG_ERROR(("| PMDRET LPBK: %c%c%c%c | PMD RX LANE ADDR IDX : %d%d%d%d  | SNGLMODE : %d         |\n",
	(pmd_rmt_lb & 8) ? 'Y':'N',
	(pmd_rmt_lb & 4) ? 'Y':'N',
	(pmd_rmt_lb & 2) ? 'Y':'N',
	(pmd_rmt_lb & 1) ? 'Y':'N',
        LN_ADDR3r_RX_LANE_ADDR_3f_GET(ln_addr3),
        LN_ADDR2r_RX_LANE_ADDR_2f_GET(ln_addr2),
        LN_ADDR1r_RX_LANE_ADDR_1f_GET(ln_addr1),
        LN_ADDR0r_RX_LANE_ADDR_0f_GET(ln_addr0),
        MAIN0_SETUPr_SINGLE_PORT_MODEf_GET(reg_main0_setup)));
  PHYMOD_DEBUG_ERROR(("+-------------------+--------------------------+-------------------------------+\n"));

  }
  return PHYMOD_E_NONE;
}

/**
@brief   Get the link status
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Print the link status.
1. pmd_live_status
   a. pll_lock_sts : PMD PLL lock indication
   b. tx_clk_vld_sts : The txclk is valid Don't see a need for this now, so will not check this bit
2a. pmd_lane_live_status
   a. signal_detect_sts : The signal detected on the lane
   b. rx_lock_sts : The recovery clock locked for the rx lane
   c. rx_clk_vld_sts : The rxclk is valid for the lane. Don't see a need for this now, so will not check this bit
2b. pmd_lane_latched_status
3a. pcs_live_status
   a. sync status : per port: Live Sync Status indicator for cl36, cl48, brcm mode. Comma detect/code sync
                    per lane: Live Block Lock Status indicator for cl49, cl82.Sync bits locked, on the 2 sync bits of the 66 bits(of 64/66 encoding)
   b. link_status : Link status for that port
3b. pcs_latched_status
*/
int tefmod16_diag_link(PHYMOD_ST *pc)
{
  PMD_X1_STSr_t pmd_live_status;
  PMD_X4_STSr_t pmd_lane_live_status;
  phymod_access_t *lpc = (phymod_access_t*)pc;

  int i,rx_lock_sts[4],sig_det_sts[4], this_lane;


  /*1. pmd_live_status  */
  READ_PMD_X1_STSr(pc, &pmd_live_status);

  /*2a. pmd_lane_live_status  */

  this_lane = lpc->lane_mask; 
  for(i=0; i < 4; i++) {
    lpc->lane_mask = 1<<i;
    READ_PMD_X4_STSr(lpc, &pmd_lane_live_status);
    sig_det_sts[i] = PMD_X4_STSr_SIGNAL_DETECT_STSf_GET(pmd_lane_live_status);
    rx_lock_sts[i] = PMD_X4_STSr_RX_LOCK_STSf_GET(pmd_lane_live_status);
  }
  lpc->lane_mask = this_lane; 
   


  /* Print the information here */
  PHYMOD_DEBUG_ERROR(("| TRG ADR : %06d LANE: %02d    LINK STATE                   |   LH    |   LL   |\n", pc->addr, pc->lane_mask));
  PHYMOD_DEBUG_ERROR(("+-----------------------+-------------------+---------------+---------+--------+\n"));
  PHYMOD_DEBUG_ERROR(("| PMD PLL LOCK   : %c    | PCS SYNC   : ??   |                                  |\n", 
    (PMD_X1_STSr_PLL_LOCK_STSf_GET(pmd_live_status) == 1) ? 'Y' : 'N'));

  PHYMOD_DEBUG_ERROR(("| PMD SIG DETECT : %c%c%c%c | PCS LINK   : ??   |                                  |\n", 
    (sig_det_sts[3] == 1)? 'Y':'N',
    (sig_det_sts[2] == 1)? 'Y':'N',
    (sig_det_sts[1] == 1)? 'Y':'N',
    (sig_det_sts[0] == 1)? 'Y':'N'));

  PHYMOD_DEBUG_ERROR(("| PMD LOCKED     : %c%c%c%c | PCS HI BER : ??   |                                  |\n", 
    (rx_lock_sts[3] == 1)? 'Y':'N',
    (rx_lock_sts[2] == 1)? 'Y':'N',
    (rx_lock_sts[1] == 1)? 'Y':'N',
    (rx_lock_sts[0] == 1)? 'Y':'N'));


  PHYMOD_DEBUG_ERROR(("|                       | PCS AMLOCK : ??   | PCS AM LOCK   : ??    : ??       |\n"));

  PHYMOD_DEBUG_ERROR(("+-----------------------+-------------------+----------------------------------+\n"));

  return PHYMOD_E_NONE;
}

/**
@brief   Get the link status
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Call all the Diags
*/
int tefmod16_diag_general(PHYMOD_ST *pc)
{
  tefmod16_diag_topology(pc);
  tefmod16_diag_link(pc);
  tefmod16_diag_autoneg(pc);
  tefmod16_diag_speed(pc);

  return (PHYMOD_E_NONE);
}

/**
@brief   Internal Tfc info (PRBS vs. PRTP. vs. PktGen etc.).
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Print the Internal Tfc types.
 TBD.
*/
int tefmod16_diag_internal_tfc(PHYMOD_ST *pc)
{
  PKTGEN_PRTPCTLr_t reg_prtp_ctl;

  READ_PKTGEN_PRTPCTLr(pc, &reg_prtp_ctl);

  PHYMOD_DEBUG_ERROR(("| TRG ADR : %06d LANE: %02d    INTERNAL TFC                                    |\n",
                       pc->addr, pc->lane_mask));
  PHYMOD_DEBUG_ERROR(("+------------------------------------------+-----------------------------------+\n"));

  PHYMOD_DEBUG_ERROR(("| TX PRTP EN : %d | RX PRTP EN : %d |                                          |\n",
           PKTGEN_PRTPCTLr_TX_PRTP_ENf_GET(reg_prtp_ctl),
           PKTGEN_PRTPCTLr_RX_PRTP_ENf_GET(reg_prtp_ctl)));

  PHYMOD_DEBUG_ERROR(("+------------------------------------------+-----------------------------------+\n"));

  return PHYMOD_E_NONE;
}

/**
@brief   Soft Table diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details ST diags
 Print the soft table enteries
 if the resolved_speed is not set, then all the table enteries are displayed
*/

#define _TRG_ST_PRINT(pc, st_num, ovr, sc_st)                                        \
{                                                                                    \
  SC_X1_SPD_##ovr##_SPDr_t sc_st##_speed;                                            \
  SC_X1_SPD_##ovr##_0r_t sc_st##_0;                                                  \
  SC_X1_SPD_##ovr##_1r_t sc_st##_1;                                                  \
  SC_X1_SPD_##ovr##_2r_t sc_st##_2;                                                  \
  SC_X1_SPD_##ovr##_3r_t sc_st##_3;                                                  \
  SC_X1_SPD_##ovr##_4r_t sc_st##_4;                                                  \
  SC_X1_SPD_##ovr##_5r_t sc_st##_5;                                                  \
  uint8_t x1_speed;                                                                  \
                                                                                     \
  READ_SC_X1_SPD_##ovr##_SPDr(pc,&sc_st##_speed);                                    \
  x1_speed = SC_X1_SPD_##ovr##_SPDr_SPEEDf_GET(sc_st##_speed);                       \
  x1_speed = (x1_speed >= TEFMOD16_SPD_ILLEGAL) ? TEFMOD16_SPD_ILLEGAL: x1_speed;        \
  PHYMOD_DEBUG_ERROR(("| ST [%d]              | %-28s| NUM LANES : %d            |\n",\
       st_num, e2s_tefmod16_spd_intfc_type_t[x1_speed]+6,                                \
       SC_X1_SPD_##ovr##_SPDr_NUM_LANESf_GET(sc_st##_speed)));                       \
	                                                                             \
  if((resolved_speed == 0xff) ||                                                     \
     (resolved_speed==SC_X1_SPD_##ovr##_SPDr_SPEEDf_GET(sc_st##_speed))) {           \
    READ_SC_X1_SPD_##ovr##_0r(pc,&sc_st##_0);                                        \
    READ_SC_X1_SPD_##ovr##_1r(pc,&sc_st##_1);                                        \
    READ_SC_X1_SPD_##ovr##_2r(pc,&sc_st##_2);                                        \
    READ_SC_X1_SPD_##ovr##_3r(pc,&sc_st##_3);                                        \
    READ_SC_X1_SPD_##ovr##_4r(pc,&sc_st##_4);                                        \
    READ_SC_X1_SPD_##ovr##_5r(pc,&sc_st##_5);                                        \
                                                                                     \
  PHYMOD_DEBUG_ERROR(("| %-19s |                                                |\n",\
        e2s_tefmod16_os_mode_type[SC_X1_SPD_##ovr##_0r_OS_MODEf_GET(sc_st##_0)]+6));   \
                                                                                     \
  PHYMOD_DEBUG_ERROR(("| %-19s |                                                |\n",\
        e2s_tefmod16_scr_mode[SC_X1_SPD_##ovr##_0r_SCR_MODEf_GET(sc_st##_0)]+6));      \
                                                                                     \
  PHYMOD_DEBUG_ERROR(("+---------------------+-----------------------------+-----------+--------------+\n"));\
  }                                                                                  \
}

/**
@brief   Soft Table diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@param   resolved_speed The speed to which AN is already resolved.
@returns PHYMOD_E_NONE
@details Soft Table diags
 Print the soft table enteries
 if the resolved_speed is not set, then all the table enteries are displayed
*/
int tefmod16_diag_st(PHYMOD_ST *pc, int resolved_speed)
{
  _TRG_ST_PRINT(pc, 0, OVRR0, sc_st0)
  _TRG_ST_PRINT(pc, 1, OVRR1, sc_st1) 
  _TRG_ST_PRINT(pc, 2, OVRR2, sc_st2)
  _TRG_ST_PRINT(pc, 3, OVRR2, sc_st3)

  PHYMOD_DEBUG_ERROR(("+---------------------+-----------------------------+-----------+--------------+\n"));
  return PHYMOD_E_NONE;
}

/**
@brief   AN Timers diagnostic function
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Prints the AN timers
*/
int tefmod16_diag_an_timers(PHYMOD_ST* pc)
{
  AN_X1_CL73_BRK_LNKr_t                reg_cl73_break_link;
  AN_X1_CL73_ERRr_t                    reg_cl73_err;
  AN_X1_CL73_DME_LOCKr_t               reg_cl73_dme_lock;
  AN_X1_PD_SD_TMRr_t                   reg_pd_sd_timer;
  AN_X1_IGNORE_LNK_TMRr_t              reg_ignore_link_timer;
  AN_X1_LNK_FAIL_INHBT_TMR_CL72r_t     reg_inhibit_timer;
  AN_X1_LNK_FAIL_INHBT_TMR_NOT_CL72r_t reg_inhibit_not_timer;
  AN_X1_DME_PAGE_TMR_TYPEr_t           reg_dme_page_timer;


  READ_AN_X1_CL73_ERRr       (pc, &reg_cl73_err);
  READ_AN_X1_PD_SD_TMRr      (pc, &reg_pd_sd_timer);
  READ_AN_X1_CL73_BRK_LNKr   (pc, &reg_cl73_break_link);
  READ_AN_X1_CL73_DME_LOCKr  (pc, &reg_cl73_dme_lock);
  READ_AN_X1_IGNORE_LNK_TMRr (pc, &reg_ignore_link_timer);

  READ_AN_X1_DME_PAGE_TMR_TYPEr           (pc, &reg_dme_page_timer);
  READ_AN_X1_LNK_FAIL_INHBT_TMR_CL72r     (pc, &reg_inhibit_timer);
  READ_AN_X1_LNK_FAIL_INHBT_TMR_NOT_CL72r (pc, &reg_inhibit_not_timer);

  PHYMOD_DEBUG_ERROR(("| TRG ADR : %06d LANE: %02d    AN TIMERS                                       |\n", 
                       pc->addr, pc->lane_mask));
  PHYMOD_DEBUG_ERROR(("+--------------------------------------+---------------------------------------+\n"));

  PHYMOD_DEBUG_ERROR(("|                                      | CL73 BREAK TIMER PERIOD  : 0x%08X   |\n",
      AN_X1_CL73_BRK_LNKr_CL73_BREAK_TIMER_PERIODf_GET(reg_cl73_break_link)));

  PHYMOD_DEBUG_ERROR(("| CL73 ERR              : 0x%08X   | CL73 DME LOCK          : 0x%08X   |\n", 
      AN_X1_CL73_ERRr_CL73_ERROR_TIMER_PERIODf_GET(reg_cl73_err),
      AN_X1_CL73_DME_LOCKr_PD_DME_LOCK_TIMER_PERIODf_GET(reg_cl73_dme_lock)));

  PHYMOD_DEBUG_ERROR(("| IGNORE LINK           : 0x%08X   | PS SD                  : 0x%08x   |\n",
      AN_X1_IGNORE_LNK_TMRr_IGNORE_LINK_TIMER_PERIODf_GET(reg_ignore_link_timer),
      AN_X1_PD_SD_TMRr_PD_SD_TIMER_PERIODf_GET(reg_pd_sd_timer)));

  PHYMOD_DEBUG_ERROR(("| DME PAGE MIN          : 0x%08X   | DME PAGE MAX           : 0x%08X   |\n",
      AN_X1_DME_PAGE_TMR_TYPEr_CL73_PAGE_TEST_MIN_TIMERf_GET(reg_dme_page_timer),
      AN_X1_DME_PAGE_TMR_TYPEr_CL73_PAGE_TEST_MAX_TIMERf_GET(reg_dme_page_timer)));

  PHYMOD_DEBUG_ERROR(("| FAIL INHIBIT W/O CL72 : 0x%08X   | FAIL INHIBIT WITH CL72 : 0x%08X   |\n",
      AN_X1_LNK_FAIL_INHBT_TMR_NOT_CL72r_LINK_FAIL_INHIBIT_TIMER_NCL72_PERIODf_GET(reg_inhibit_not_timer),
      AN_X1_LNK_FAIL_INHBT_TMR_CL72r_LINK_FAIL_INHIBIT_TIMER_CL72_PERIODf_GET(reg_inhibit_timer)));

  PHYMOD_DEBUG_ERROR(("+--------------------------------------+---------------------------------------+\n"));
  return PHYMOD_E_NONE;
}

/**
@brief   Prints out the debug state register
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details Prints the debug state regs.
*/
int tefmod16_diag_state(PHYMOD_ST* pc)
{
  SC_X4_DBGr_t               reg_sc_dbg_st;
  AN_X4_TLA_SEQUENCER_STSr_t reg_an_tla_seq;

  READ_SC_X4_DBGr(pc, &reg_sc_dbg_st);
  READ_AN_X4_TLA_SEQUENCER_STSr(pc, &reg_an_tla_seq);

  PHYMOD_DEBUG_ERROR(("|                                 DEBUG STATE                                  |\n"));
  PHYMOD_DEBUG_ERROR(("+--------------------------------------+---------------------------------------+\n"));
  PHYMOD_DEBUG_ERROR(("| SC_DEBUG_STATE    : 0x%08x       |  FSM_STATUS : 0x%08X              |\n",
       SC_X4_DBGr_GET(reg_sc_dbg_st), SC_X4_DBGr_SC_FSM_STATUSf_GET(reg_sc_dbg_st)));

  PHYMOD_DEBUG_ERROR(("| TLA_SEQUENCER STS : 0x%08x       |                                       |\n",
       AN_X4_TLA_SEQUENCER_STSr_GET(reg_an_tla_seq)));
  PHYMOD_DEBUG_ERROR(("+--------------------------------------+---------------------------------------+\n"));
  
  return PHYMOD_E_NONE;
}


/**
@brief  TBD
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details  Misc debug
*/
int tefmod16_diag_debug(PHYMOD_ST* pc)
{
  _TRG_OPN(pc);
  _TRG_CLS(pc);
  return PHYMOD_E_NONE;
}

/**
@brief  TBD
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details  Misc debug
*/
int tefmod16_diag_ieee(PHYMOD_ST *pc)
{
  _TRG_OPN(pc);
  _TRG_CLS(pc);
  return PHYMOD_E_NONE;
}

/**
@brief  TBD
@param   pc handle to current TSCE context (#PHYMOD_ST)
@returns PHYMOD_E_NONE if no errors. PHYMOD_E_ERROR else.
@details  Misc debug
*/
int tefmod16_diag(PHYMOD_ST *pc, tefmod16_diag_type_t diag_type)
{
  int rv;
  rv = PHYMOD_E_NONE;

  _TRG_OPN(pc);

  if(diag_type & TEFMOD16_DIAG_GENERAL)    rv |= tefmod16_diag_general(pc);
  if(diag_type & TEFMOD16_DIAG_TOPOLOGY)   rv |= tefmod16_diag_topology(pc);
  if(diag_type & TEFMOD16_DIAG_LINK)       rv |= tefmod16_diag_link(pc);
  if(diag_type & TEFMOD16_DIAG_SPEED)      rv |= tefmod16_diag_speed(pc);
  if(diag_type & TEFMOD16_DIAG_ANEG)       rv |= tefmod16_diag_autoneg(pc);
  if(diag_type & TEFMOD16_DIAG_TFC)        rv |= tefmod16_diag_internal_tfc(pc);
  if(diag_type & TEFMOD16_DIAG_AN_TIMERS)  rv |= tefmod16_diag_an_timers(pc);
  if(diag_type & TEFMOD16_DIAG_STATE)      rv |= tefmod16_diag_state(pc);
  if(diag_type & TEFMOD16_DIAG_DEBUG)      rv |= tefmod16_diag_debug(pc);
  _TRG_CLS(pc);
  return rv;
}
 
/*!
*/
int tefmod16_diag_disp(PHYMOD_ST *pc, char* cmd_str)
{
    tefmod16_diag_type_t type;

    if (!cmd_str)
        type = TEFMOD16_DIAG_GENERAL;
    else if (!PHYMOD_STRCMP(cmd_str, "topo"))
        type = TEFMOD16_DIAG_TOPOLOGY;
    else if (!PHYMOD_STRCMP(cmd_str, "link"))
        type = TEFMOD16_DIAG_LINK;
    else if (!PHYMOD_STRCMP(cmd_str, "speed"))
        type = TEFMOD16_DIAG_SPEED;
    else if (!PHYMOD_STRCMP(cmd_str, "aneg"))
        type = TEFMOD16_DIAG_ANEG;
    else if (!PHYMOD_STRCMP(cmd_str, "tfc"))
        type = TEFMOD16_DIAG_TFC;
    else if (!PHYMOD_STRCMP(cmd_str, "antimers"))
        type = TEFMOD16_DIAG_AN_TIMERS;
    else if (!PHYMOD_STRCMP(cmd_str, "state"))
        type = TEFMOD16_DIAG_STATE;
    else if (!PHYMOD_STRCMP(cmd_str, "debug"))
        type = TEFMOD16_DIAG_DEBUG;
    else
        type = TEFMOD16_DIAG_GENERAL;

     return (tefmod16_diag(pc, type));
}
