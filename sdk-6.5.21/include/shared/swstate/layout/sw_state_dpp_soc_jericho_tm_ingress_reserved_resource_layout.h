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
#ifndef _SHR_sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_H_
#define _SHR_sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_H_


#define SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_DRAM_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_OCB_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES (1) 
#define SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_DRAM_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_OCB_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_DPP_SOC_JERICHO_TM_INGRESS_RESERVED_RESOURCE_RESERVED_LAYOUT_NOF_MAX_LAYOUT_NODES) 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_dram_reserved_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_ocb_reserved_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_reserved_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#endif /* _SHR_sw_state_dpp_soc_jericho_tm_ingress_reserved_resource_H_ */
