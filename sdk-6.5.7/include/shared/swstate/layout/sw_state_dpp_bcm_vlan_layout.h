/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
*/
#ifndef _SHR_sw_state_dpp_bcm_vlan_H_
#define _SHR_sw_state_dpp_bcm_vlan_H_


#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_INIT_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_PBMP_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_UBMP_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LOCAL_LIF_INDEX_DROP_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LOCAL_LIF_INDEX_SIMPLE_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_INIT_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_PBMP_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_DEFL_UBMP_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LOCAL_LIF_INDEX_DROP_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LOCAL_LIF_INDEX_SIMPLE_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_REF_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_FID_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_REF_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_FID_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_PACKET_IS_TAGGED_AFTER_EVE_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_OUTER_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_INNER_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_PACKET_IS_TAGGED_AFTER_EVE_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_VID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_PCP_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_TPID_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_TPID_VAL_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_PACKET_IS_TAGGED_AFTER_EVE_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_OUTER_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_INNER_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_PACKET_IS_TAGGED_AFTER_EVE_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_MAPPING_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_BCM_VLAN_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_BCM_VLAN_VLAN_INFO_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_FID_REF_COUNT_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_ING_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_BCM_VLAN_EDIT_EG_ACTION_MAPPING_LAYOUT_NOF_MAX_LAYOUT_NODES) 

#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_init_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_defl_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_defl_pbmp_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_defl_ubmp_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_count_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_local_lif_index_drop_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_vlan_info_local_lif_index_simple_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_fid_ref_count_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_fid_ref_count_ref_count_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_fid_ref_count_fid_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_outer_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_outer_vid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_outer_pcp_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_outer_tpid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_outer_tpid_val_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_inner_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_inner_vid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_inner_pcp_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_inner_tpid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_inner_tpid_val_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_ing_action_packet_is_tagged_after_eve_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_outer_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_outer_vid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_outer_pcp_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_outer_tpid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_outer_tpid_val_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_inner_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_inner_vid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_inner_pcp_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_inner_tpid_action_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_inner_tpid_val_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_packet_is_tagged_after_eve_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
#ifdef BCM_PETRA_SUPPORT
int sw_state_dpp_bcm_vlan_edit_eg_action_mapping_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* BCM_PETRA_SUPPORT*/ 
#endif /* defined(BCM_PETRA_SUPPORT)*/ 

#endif /* _SHR_sw_state_dpp_bcm_vlan_H_ */