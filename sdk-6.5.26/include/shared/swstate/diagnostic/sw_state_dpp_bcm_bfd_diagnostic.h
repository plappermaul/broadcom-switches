/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_BCM_BFD_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_BCM_BFD_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_SUPPORT
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_dump */
typedef int (*sw_state_dpp_bcm_bfd_dump_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_dump */
typedef int (*sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_dump */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_s_hop_tos_value_dump */
typedef int (*sw_state_dpp_bcm_bfd_s_hop_tos_value_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_dump */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_trap_info_trap_ids_dump */
typedef int (*sw_state_dpp_bcm_bfd_trap_info_trap_ids_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_dump */
typedef int (*sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_pdu_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_pdu_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_value_dump */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_value_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_tx_rate_ref_counter_dump */
typedef int (*sw_state_dpp_bcm_bfd_tx_rate_ref_counter_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_dump */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_trap_id_dump */
typedef int (*sw_state_dpp_bcm_bfd_reflector_trap_id_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_reflector_status_dump */
typedef int (*sw_state_dpp_bcm_bfd_reflector_status_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_dump */
typedef int (*sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_dump_cb)(
    int unit, int bfd_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_dump_cb dump;
} sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_s_hop_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_tos_value_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_tos_value_dump_cb dump;
} sw_state_dpp_bcm_bfd_s_hop_tos_value_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_s_hop_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_s_hop_dump_cb dump;
    sw_state_dpp_bcm_bfd_s_hop_ref_counter_diagnostic_cbs_t ref_counter;
    sw_state_dpp_bcm_bfd_s_hop_tos_value_diagnostic_cbs_t tos_value;
} sw_state_dpp_bcm_bfd_s_hop_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_trap_info_trap_ids_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_dump_cb dump;
} sw_state_dpp_bcm_bfd_trap_info_trap_ids_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_trap_info_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_trap_info_dump_cb dump;
    sw_state_dpp_bcm_bfd_trap_info_trap_ids_diagnostic_cbs_t trap_ids;
} sw_state_dpp_bcm_bfd_trap_info_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_dump_cb dump;
} sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_pdu_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_pdu_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_pdu_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_tx_rate_value_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_tx_rate_value_dump_cb dump;
} sw_state_dpp_bcm_bfd_tx_rate_value_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_tx_rate_ref_counter_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_dump_cb dump;
} sw_state_dpp_bcm_bfd_tx_rate_ref_counter_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_dump_cb dump;
} sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_reflector_trap_id_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_reflector_trap_id_dump_cb dump;
} sw_state_dpp_bcm_bfd_reflector_trap_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_reflector_status_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_reflector_status_dump_cb dump;
} sw_state_dpp_bcm_bfd_reflector_status_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_diagnostic_cbs_s {
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_dump_cb dump;
} sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_bfd_diagnostic_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_bfd_dump_cb dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_bcm_bfd_YourDiscriminator_ref_count_diagnostic_cbs_t YourDiscriminator_ref_count;
    sw_state_dpp_bcm_bfd_s_hop_diagnostic_cbs_t s_hop;
    sw_state_dpp_bcm_bfd_trap_info_diagnostic_cbs_t trap_info;
    sw_state_dpp_bcm_bfd_mpls_udp_sport_ref_counter_diagnostic_cbs_t mpls_udp_sport_ref_counter;
    sw_state_dpp_bcm_bfd__bcm_bfd_endpoint_list_diagnostic_cbs_t _bcm_bfd_endpoint_list;
    sw_state_dpp_bcm_bfd_ipv4_udp_sport_ref_counter_diagnostic_cbs_t ipv4_udp_sport_ref_counter;
    sw_state_dpp_bcm_bfd_ipv4_multi_hop_acc_ref_counter_diagnostic_cbs_t ipv4_multi_hop_acc_ref_counter;
    sw_state_dpp_bcm_bfd_pdu_ref_counter_diagnostic_cbs_t pdu_ref_counter;
    sw_state_dpp_bcm_bfd_mpls_tp_cc_ref_counter_diagnostic_cbs_t mpls_tp_cc_ref_counter;
    sw_state_dpp_bcm_bfd_tx_rate_value_diagnostic_cbs_t tx_rate_value;
    sw_state_dpp_bcm_bfd_tx_rate_ref_counter_diagnostic_cbs_t tx_rate_ref_counter;
    sw_state_dpp_bcm_bfd_no_of_sbfd_endpoints_diagnostic_cbs_t no_of_sbfd_endpoints;
    sw_state_dpp_bcm_bfd_reflector_trap_id_diagnostic_cbs_t reflector_trap_id;
    sw_state_dpp_bcm_bfd_reflector_status_diagnostic_cbs_t reflector_status;
    sw_state_dpp_bcm_bfd_no_of_sbfd_reflector_diagnostic_cbs_t no_of_sbfd_reflector;
} sw_state_dpp_bcm_bfd_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_bcm_bfd_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_SUPPORT */

#endif /* _SHR_SW_STATE_DPP_BCM_BFD_DIAGNOSTIC_H_ */
