/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_BCM_BFD_ACCESS_H_
#define _SHR_SW_STATE_DPP_BCM_BFD_ACCESS_H_

/********************************* access calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the access calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the access struct by 'sw_state_access_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_set */
typedef int (*sw_state_dpp_bcm_bfd_set_cb)(
    int unit, CONST bcm_dpp_bfd_info_t *bfd);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_get */
typedef int (*sw_state_dpp_bcm_bfd_get_cb)(
    int unit, bcm_dpp_bfd_info_t *bfd);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_alloc */
typedef int (*sw_state_dpp_bcm_bfd_alloc_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_is_allocated */
typedef int (*sw_state_dpp_bcm_bfd_is_allocated_cb)(
    int unit, uint8 *is_allocated);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_free */
typedef int (*sw_state_dpp_bcm_bfd_free_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_verify */
typedef int (*sw_state_dpp_bcm_bfd_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_set */
typedef int (*sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_set_cb)(
    int unit, int YourDiscriminator_ref_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_get */
typedef int (*sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_get_cb)(
    int unit, int *YourDiscriminator_ref_count);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_verify */
typedef int (*sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_set */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_set_cb)(
    int unit, CONST bcm_dpp_bfd_pp_s_hop *s_hop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_get */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_get_cb)(
    int unit, bcm_dpp_bfd_pp_s_hop *s_hop);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_verify */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_ref_counter_set_cb)(
    int unit, int ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_ref_counter_get_cb)(
    int unit, int *ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_tos_value_set */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_tos_value_set_cb)(
    int unit, int tos_value);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_tos_value_get */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_tos_value_get_cb)(
    int unit, int *tos_value);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_tos_value_verify */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_tos_value_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_set */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_set_cb)(
    int unit, CONST SOC_PPC_BFD_INIT_TRAP_INFO *trap_info);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_get */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_get_cb)(
    int unit, SOC_PPC_BFD_INIT_TRAP_INFO *trap_info);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_verify */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_trap_ids_set */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_trap_ids_set_cb)(
    int unit, int trap_ids_idx_0, uint32 trap_ids);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_trap_ids_get */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_trap_ids_get_cb)(
    int unit, int trap_ids_idx_0, uint32 *trap_ids);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_trap_ids_verify */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_trap_ids_verify_cb)(
    int unit, int trap_ids_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_set_cb)(
    int unit, uint32 mpls_udp_sport_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_get_cb)(
    int unit, uint32 *mpls_udp_sport_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_set */
typedef int (*sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_set_cb)(
    int unit, ENDPOINT_LIST_PTR _bcm_bfd_endpoint_list);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_get */
typedef int (*sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_get_cb)(
    int unit, ENDPOINT_LIST_PTR *_bcm_bfd_endpoint_list);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_verify */
typedef int (*sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_set_cb)(
    int unit, uint32 ipv4_udp_sport_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_get_cb)(
    int unit, uint32 *ipv4_udp_sport_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_set_cb)(
    int unit, uint32 ipv4_multi_hop_acc_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_get_cb)(
    int unit, uint32 *ipv4_multi_hop_acc_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_pdu_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_pdu_ref_counter_set_cb)(
    int unit, uint32 pdu_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_pdu_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_pdu_ref_counter_get_cb)(
    int unit, uint32 *pdu_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_pdu_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_pdu_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_set_cb)(
    int unit, uint32 mpls_tp_cc_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_get_cb)(
    int unit, uint32 *mpls_tp_cc_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_value_set */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_value_set_cb)(
    int unit, int tx_rate_value_idx_0, uint32 tx_rate_value);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_value_get */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_value_get_cb)(
    int unit, int tx_rate_value_idx_0, uint32 *tx_rate_value);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_value_verify */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_value_verify_cb)(
    int unit, int tx_rate_value_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_ref_counter_set */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_ref_counter_set_cb)(
    int unit, int tx_rate_ref_counter_idx_0, uint32 tx_rate_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_ref_counter_get */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_ref_counter_get_cb)(
    int unit, int tx_rate_ref_counter_idx_0, uint32 *tx_rate_ref_counter);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_ref_counter_verify */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_ref_counter_verify_cb)(
    int unit, int tx_rate_ref_counter_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_set */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_set_cb)(
    int unit, uint32 no_of_sbfd_endpoints);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_get */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_get_cb)(
    int unit, uint32 *no_of_sbfd_endpoints);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_verify */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_trap_id_set */
typedef int (*sw_state_dpp_bcm_bfd_reflector_trap_id_set_cb)(
    int unit, uint32 reflector_trap_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_trap_id_get */
typedef int (*sw_state_dpp_bcm_bfd_reflector_trap_id_get_cb)(
    int unit, uint32 *reflector_trap_id);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_trap_id_verify */
typedef int (*sw_state_dpp_bcm_bfd_reflector_trap_id_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_status_set */
typedef int (*sw_state_dpp_bcm_bfd_reflector_status_set_cb)(
    int unit, bcm_bfd_state_t reflector_status);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_status_get */
typedef int (*sw_state_dpp_bcm_bfd_reflector_status_get_cb)(
    int unit, bcm_bfd_state_t *reflector_status);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_status_verify */
typedef int (*sw_state_dpp_bcm_bfd_reflector_status_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_set */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_set_cb)(
    int unit, uint8 no_of_sbfd_reflector);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_get */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_get_cb)(
    int unit, uint8 *no_of_sbfd_reflector);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_verify */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_verify_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** access calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the access layer for the entire SW state.*/
/* use this tree to alloc/free/set/get fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_cbs_s {
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_set_cb set;
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_get_cb get;
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_verify_cb verify;
} sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_s_hop_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_tos_value_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_tos_value_set_cb set;
    sw_state_dpp_bcm_bfd_s_hop_tos_value_get_cb get;
    sw_state_dpp_bcm_bfd_s_hop_tos_value_verify_cb verify;
} sw_state_dpp_bcm_bfd_s_hop_tos_value_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_set_cb set;
    sw_state_dpp_bcm_bfd_s_hop_get_cb get;
    sw_state_dpp_bcm_bfd_s_hop_verify_cb verify;
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_cbs_t ref_counter;
    sw_state_dpp_bcm_bfd_s_hop_tos_value_cbs_t tos_value;
} sw_state_dpp_bcm_bfd_s_hop_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_trap_info_trap_ids_cbs_s {
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_set_cb set;
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_get_cb get;
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_verify_cb verify;
} sw_state_dpp_bcm_bfd_trap_info_trap_ids_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_trap_info_cbs_s {
    sw_state_dpp_bcm_bfd_trap_info_set_cb set;
    sw_state_dpp_bcm_bfd_trap_info_get_cb get;
    sw_state_dpp_bcm_bfd_trap_info_verify_cb verify;
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_cbs_t trap_ids;
} sw_state_dpp_bcm_bfd_trap_info_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_cbs_s {
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_set_cb set;
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_get_cb get;
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_verify_cb verify;
} sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_pdu_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_pdu_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_pdu_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_pdu_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_pdu_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_tx_rate_value_cbs_s {
    sw_state_dpp_bcm_bfd_tx_rate_value_set_cb set;
    sw_state_dpp_bcm_bfd_tx_rate_value_get_cb get;
    sw_state_dpp_bcm_bfd_tx_rate_value_verify_cb verify;
} sw_state_dpp_bcm_bfd_tx_rate_value_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_tx_rate_ref_counter_cbs_s {
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_set_cb set;
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_get_cb get;
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_verify_cb verify;
} sw_state_dpp_bcm_bfd_tx_rate_ref_counter_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_cbs_s {
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_set_cb set;
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_get_cb get;
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_verify_cb verify;
} sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_reflector_trap_id_cbs_s {
    sw_state_dpp_bcm_bfd_reflector_trap_id_set_cb set;
    sw_state_dpp_bcm_bfd_reflector_trap_id_get_cb get;
    sw_state_dpp_bcm_bfd_reflector_trap_id_verify_cb verify;
} sw_state_dpp_bcm_bfd_reflector_trap_id_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_reflector_status_cbs_s {
    sw_state_dpp_bcm_bfd_reflector_status_set_cb set;
    sw_state_dpp_bcm_bfd_reflector_status_get_cb get;
    sw_state_dpp_bcm_bfd_reflector_status_verify_cb verify;
} sw_state_dpp_bcm_bfd_reflector_status_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_cbs_s {
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_set_cb set;
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_get_cb get;
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_verify_cb verify;
} sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_set_cb set;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_get_cb get;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_alloc_cb alloc;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_is_allocated_cb is_allocated;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_free_cb free;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_verify_cb verify;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_cbs_t YourDiscriminator_ref_count;
    sw_state_dpp_bcm_bfd_s_hop_cbs_t s_hop;
    sw_state_dpp_bcm_bfd_trap_info_cbs_t trap_info;
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_cbs_t mpls_udp_sport_ref_counter;
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_cbs_t _bcm_bfd_endpoint_list;
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_cbs_t ipv4_udp_sport_ref_counter;
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_cbs_t ipv4_multi_hop_acc_ref_counter;
    sw_state_dpp_bcm_bfd_pdu_ref_counter_cbs_t pdu_ref_counter;
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_cbs_t mpls_tp_cc_ref_counter;
    sw_state_dpp_bcm_bfd_tx_rate_value_cbs_t tx_rate_value;
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_cbs_t tx_rate_ref_counter;
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_cbs_t no_of_sbfd_endpoints;
    sw_state_dpp_bcm_bfd_reflector_trap_id_cbs_t reflector_trap_id;
    sw_state_dpp_bcm_bfd_reflector_status_cbs_t reflector_status;
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_cbs_t no_of_sbfd_reflector;
} sw_state_dpp_bcm_bfd_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_bcm_bfd_access_cb_init(int unit);

#endif /* _SHR_SW_STATE_DPP_BCM_BFD_ACCESS_H_ */
