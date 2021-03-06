/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
*/
#ifndef _SHR_sw_state_dpp_bcm_linkbonding_H_
#define _SHR_sw_state_dpp_bcm_linkbonding_H_


#define SW_STATE_DPP_BCM_LINKBONDING_LBG_VALID_BMP_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_PORTS_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_LINKBONDING_PORT_TO_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_ING_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_EGR_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_LINKBONDING_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_LINKBONDING_LBG_VALID_BMP_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_PORTS_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_LINKBONDING_PORT_TO_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_ING_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_LINKBONDING_MODEM_TO_EGR_LBG_LAYOUT_NOF_MAX_LAYOUT_NODES) 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_lbg_valid_bmp_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_modem_to_ports_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_port_to_lbg_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_modem_to_ing_lbg_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_linkbonding_modem_to_egr_lbg_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#endif /* _SHR_sw_state_dpp_bcm_linkbonding_H_ */
