/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
*/
#ifndef _SHR_sw_state_dpp_soc_arad_tm_egr_prog_editor_H_
#define _SHR_sw_state_dpp_soc_arad_tm_egr_prog_editor_H_


#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_TM_PROFILES_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_PROG_USED_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_LFEM_BK_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_PROGRAM_POINTER_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_TM_PROFILE_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_NUM_VALUE_1_OFFSET_PROFILES_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_VALUE_1_OFFSET_FIRST_PROFILE_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_PROG_USED_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_LFEM_BK_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_PROGRAM_POINTER_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_TM_PROFILE_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_NUM_VALUE_1_OFFSET_PROFILES_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_VALUE_1_OFFSET_FIRST_PROFILE_LAYOUT_NOF_MAX_LAYOUT_NODES) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_DE_SIGNAL_RETRIEVAL_MODE_PROGRAM_SELECTION_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_TM_PROFILES_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_PROGRAMS_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_ARAD_TM_EGR_PROG_EDITOR_DE_SIGNAL_RETRIEVAL_MODE_PROGRAM_SELECTION_LAYOUT_NOF_MAX_LAYOUT_NODES) 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_tm_profiles_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_prog_used_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_lfem_bk_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_program_pointer_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_tm_profile_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_num_value_1_offset_profiles_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_egr_prog_editor_programs_value_1_offset_first_profile_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#endif /* BCM_PETRA_SUPPORT*/ 

#endif /* _SHR_sw_state_dpp_soc_arad_tm_egr_prog_editor_H_ */
