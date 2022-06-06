/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_BCM_GPORT_MGMT_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_BCM_GPORT_MGMT_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_SUPPORT
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_dump_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_outlif_info_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_outlif_info_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_dump_cb)(
    int unit, int gport_mgmt_idx_0, int outlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_dump_cb)(
    int unit, int gport_mgmt_idx_0, int outlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_port_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_port_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_dump_cb)(
    int unit, int gport_mgmt_idx_0, int inlif_info_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_dump */
typedef int (*sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_dump_cb)(
    int unit, int gport_mgmt_idx_0);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_outlif_info_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_outlif_info_dump_cb dump;
    sw_state_dpp_bcm_gport_mgmt_outlif_info_lif_type_diagnostic_cbs_t lif_type;
    sw_state_dpp_bcm_gport_mgmt_outlif_info_flags_diagnostic_cbs_t flags;
} sw_state_dpp_bcm_gport_mgmt_outlif_info_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_port_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_port_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_port_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_inlif_info_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_inlif_info_dump_cb dump;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_lif_type_diagnostic_cbs_t lif_type;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_flags_diagnostic_cbs_t flags;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_criteria_diagnostic_cbs_t criteria;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_port_diagnostic_cbs_t port;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match1_diagnostic_cbs_t match1;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match2_diagnostic_cbs_t match2;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match_tunnel_diagnostic_cbs_t match_tunnel;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_match_ethertype_diagnostic_cbs_t match_ethertype;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_gport_id_diagnostic_cbs_t gport_id;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_key1_diagnostic_cbs_t key1;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_peer_gport_diagnostic_cbs_t peer_gport;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_learn_gport_id_diagnostic_cbs_t learn_gport_id;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_tpid_profile_type_diagnostic_cbs_t tpid_profile_type;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_vsi_diagnostic_cbs_t vsi;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_additional_flags_diagnostic_cbs_t additional_flags;
} sw_state_dpp_bcm_gport_mgmt_inlif_info_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_diagnostic_cbs_s {
    sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_dump_cb dump;
} sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_bcm_gport_mgmt_diagnostic_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_bcm_gport_mgmt_dump_cb dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_bcm_gport_mgmt_fec_remote_lif_diagnostic_cbs_t fec_remote_lif;
    sw_state_dpp_bcm_gport_mgmt_bcm_dpp_sw_resources_fec_to_gport_diagnostic_cbs_t bcm_dpp_sw_resources_fec_to_gport;
    sw_state_dpp_bcm_gport_mgmt_outlif_info_diagnostic_cbs_t outlif_info;
    sw_state_dpp_bcm_gport_mgmt_inlif_info_diagnostic_cbs_t inlif_info;
    sw_state_dpp_bcm_gport_mgmt_vlan_port_db_htb_handle_diagnostic_cbs_t vlan_port_db_htb_handle;
    sw_state_dpp_bcm_gport_mgmt_trill_info_db_htb_handle_diagnostic_cbs_t trill_info_db_htb_handle;
    sw_state_dpp_bcm_gport_mgmt_mc_trill_db_htb_handle_diagnostic_cbs_t mc_trill_db_htb_handle;
    sw_state_dpp_bcm_gport_mgmt_trill_vpn_db_htb_handle_diagnostic_cbs_t trill_vpn_db_htb_handle;
} sw_state_dpp_bcm_gport_mgmt_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_bcm_gport_mgmt_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_SUPPORT */

#endif /* _SHR_SW_STATE_DPP_BCM_GPORT_MGMT_DIAGNOSTIC_H_ */