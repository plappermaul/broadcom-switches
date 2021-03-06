/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_DPP_SOC_ARAD_DIAGNOSTIC_H_
#define _SHR_SW_STATE_DPP_SOC_ARAD_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_API_TEST
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_arad_sw_db_src_binds_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_bfd_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_dbal_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_diag_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_dpp_parser_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_ecmp_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_eg_encap_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_eg_mirror_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_eth_policer_mtr_profile_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_extender_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_fec_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_frwrd_ip_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_fwd_mact_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_header_data_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_ilm_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_ipv4_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_ipv4_pat_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_kaps_db_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_l2_lif_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_l2_lif_ac_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_lag_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_lif_cos_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_lif_table_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_llp_cos_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_llp_filter_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_llp_mirror_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_llp_trap_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_llp_vid_assign_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_oam_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_oam_my_mac_lsb_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_oamp_pe_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_occupation_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_oper_mode_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_overlay_arp_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_policer_size_profile_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_pon_double_lookup_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_pp_port_map_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_ptp_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_rif_to_lif_group_map_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_slb_config_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_vdc_vsi_map_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_vrrp_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_vt_profile_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_vtt_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_pp_vtt_mask_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_arad_egr_ports_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_arad_em_blocks_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_arad_multicast_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_arad_plus_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_cell_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_chip_definitions_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_dram_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_egr_prog_editor_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_guaranteed_q_resource_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_interrupts_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_jer_modid_group_map_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_kbp_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_lag_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_logical_ports_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_modport2sysport_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_op_mode_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_phy_ports_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_pmf_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_q_type_map_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_q_type_ref_count_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_reassembly_ctxt_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_sysport2modport_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_tcam_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_tcam_mgmt_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_tdm_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_tm_info_diagnostic.h>
#include <shared/swstate/diagnostic/sw_state_dpp_soc_arad_tm_vsi_diagnostic.h>
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_dump */
typedef int (*sw_state_dpp_soc_arad_dump_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_tm_dump */
typedef int (*sw_state_dpp_soc_arad_tm_dump_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
/* implemented by: sw_state_dpp_soc_arad_pp_dump */
typedef int (*sw_state_dpp_soc_arad_pp_dump_cb)(
    int unit);
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_q_type_ref_count_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_tm_q_type_ref_count_dump_cb dump;
} sw_state_dpp_soc_arad_tm_q_type_ref_count_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_q_type_map_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_tm_q_type_map_dump_cb dump;
} sw_state_dpp_soc_arad_tm_q_type_map_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_modport2sysport_diagnostic_cbs_s {
    sw_state_dpp_soc_arad_tm_modport2sysport_dump_cb dump;
} sw_state_dpp_soc_arad_tm_modport2sysport_diagnostic_cbs_t;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_tm_diagnostic_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_soc_arad_tm_dump_cb dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_soc_arad_tm_op_mode_diagnostic_cbs_t op_mode;
    sw_state_dpp_soc_arad_tm_jer_modid_group_map_diagnostic_cbs_t jer_modid_group_map;
    sw_state_dpp_soc_arad_tm_arad_plus_diagnostic_cbs_t arad_plus;
    sw_state_dpp_soc_arad_tm_tm_info_diagnostic_cbs_t tm_info;
    sw_state_dpp_soc_arad_tm_tcam_mgmt_diagnostic_cbs_t tcam_mgmt;
    sw_state_dpp_soc_arad_tm_tcam_diagnostic_cbs_t tcam;
    sw_state_dpp_soc_arad_tm_pmf_diagnostic_cbs_t pmf;
    sw_state_dpp_soc_arad_tm_arad_egr_ports_diagnostic_cbs_t arad_egr_ports;
#if defined(INCLUDE_KBP)
    sw_state_dpp_soc_arad_tm_kbp_info_diagnostic_cbs_t kbp_info;
#endif /* defined(INCLUDE_KBP)*/ 
    sw_state_dpp_soc_arad_tm_lag_diagnostic_cbs_t lag;
    sw_state_dpp_soc_arad_tm_cell_diagnostic_cbs_t cell;
    sw_state_dpp_soc_arad_tm_tdm_diagnostic_cbs_t tdm;
    sw_state_dpp_soc_arad_tm_dram_diagnostic_cbs_t dram;
    sw_state_dpp_soc_arad_tm_arad_multicast_diagnostic_cbs_t arad_multicast;
    sw_state_dpp_soc_arad_tm_q_type_ref_count_diagnostic_cbs_t q_type_ref_count;
    sw_state_dpp_soc_arad_tm_q_type_map_diagnostic_cbs_t q_type_map;
    sw_state_dpp_soc_arad_tm_vsi_diagnostic_cbs_t vsi;
    sw_state_dpp_soc_arad_tm_modport2sysport_diagnostic_cbs_t modport2sysport;
    sw_state_dpp_soc_arad_tm_sysport2modport_diagnostic_cbs_t sysport2modport;
    sw_state_dpp_soc_arad_tm_interrupts_diagnostic_cbs_t interrupts;
    sw_state_dpp_soc_arad_tm_chip_definitions_diagnostic_cbs_t chip_definitions;
    sw_state_dpp_soc_arad_tm_phy_ports_info_diagnostic_cbs_t phy_ports_info;
    sw_state_dpp_soc_arad_tm_logical_ports_info_diagnostic_cbs_t logical_ports_info;
    sw_state_dpp_soc_arad_tm_reassembly_ctxt_diagnostic_cbs_t reassembly_ctxt;
    sw_state_dpp_soc_arad_tm_arad_em_blocks_diagnostic_cbs_t arad_em_blocks;
    sw_state_dpp_soc_arad_tm_guaranteed_q_resource_diagnostic_cbs_t guaranteed_q_resource;
    sw_state_dpp_soc_arad_tm_egr_prog_editor_diagnostic_cbs_t egr_prog_editor;
} sw_state_dpp_soc_arad_tm_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_pp_diagnostic_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_soc_arad_pp_dump_cb dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
    sw_state_dpp_soc_arad_pp_ecmp_info_diagnostic_cbs_t ecmp_info;
    sw_state_dpp_soc_arad_pp_dbal_info_diagnostic_cbs_t dbal_info;
    sw_state_dpp_soc_arad_pp_llp_vid_assign_diagnostic_cbs_t llp_vid_assign;
    sw_state_dpp_soc_arad_pp_llp_filter_diagnostic_cbs_t llp_filter;
    sw_state_dpp_soc_arad_pp_pon_double_lookup_diagnostic_cbs_t pon_double_lookup;
    sw_state_dpp_soc_arad_pp_vrrp_info_diagnostic_cbs_t vrrp_info;
    sw_state_dpp_soc_arad_pp_lif_table_diagnostic_cbs_t lif_table;
    sw_state_dpp_soc_arad_pp_header_data_diagnostic_cbs_t header_data;
    sw_state_dpp_soc_arad_pp_fwd_mact_diagnostic_cbs_t fwd_mact;
    sw_state_dpp_soc_arad_pp_lag_diagnostic_cbs_t lag;
    sw_state_dpp_soc_arad_pp_ilm_info_diagnostic_cbs_t ilm_info;
    sw_state_dpp_soc_arad_pp_oam_my_mac_lsb_diagnostic_cbs_t oam_my_mac_lsb;
    sw_state_dpp_soc_arad_pp_diag_diagnostic_cbs_t diag;
    sw_state_dpp_soc_arad_pp_lif_cos_diagnostic_cbs_t lif_cos;
    sw_state_dpp_soc_arad_pp_occupation_diagnostic_cbs_t occupation;
    sw_state_dpp_soc_arad_pp_fec_diagnostic_cbs_t fec;
    sw_state_dpp_soc_arad_pp_slb_config_diagnostic_cbs_t slb_config;
    sw_state_dpp_soc_arad_pp_arad_sw_db_src_binds_diagnostic_cbs_t arad_sw_db_src_binds;
    sw_state_dpp_soc_arad_pp_pp_port_map_diagnostic_cbs_t pp_port_map;
    sw_state_dpp_soc_arad_pp_extender_info_diagnostic_cbs_t extender_info;
#if defined(BCM_88675_A0)&&defined(INCLUDE_KBP)
    sw_state_dpp_soc_arad_pp_kaps_db_diagnostic_cbs_t kaps_db;
#endif /* defined(BCM_88675_A0)&&defined(INCLUDE_KBP)*/ 
    sw_state_dpp_soc_arad_pp_vtt_diagnostic_cbs_t vtt;
    sw_state_dpp_soc_arad_pp_vtt_mask_diagnostic_cbs_t vtt_mask;
    sw_state_dpp_soc_arad_pp_frwrd_ip_diagnostic_cbs_t frwrd_ip;
    sw_state_dpp_soc_arad_pp_l2_lif_diagnostic_cbs_t l2_lif;
    sw_state_dpp_soc_arad_pp_l2_lif_ac_diagnostic_cbs_t l2_lif_ac;
    sw_state_dpp_soc_arad_pp_eth_policer_mtr_profile_diagnostic_cbs_t eth_policer_mtr_profile;
    sw_state_dpp_soc_arad_pp_llp_cos_diagnostic_cbs_t llp_cos;
    sw_state_dpp_soc_arad_pp_eg_encap_diagnostic_cbs_t eg_encap;
    sw_state_dpp_soc_arad_pp_llp_mirror_diagnostic_cbs_t llp_mirror;
    sw_state_dpp_soc_arad_pp_eg_mirror_diagnostic_cbs_t eg_mirror;
    sw_state_dpp_soc_arad_pp_llp_trap_diagnostic_cbs_t llp_trap;
    sw_state_dpp_soc_arad_pp_ipv4_pat_diagnostic_cbs_t ipv4_pat;
    sw_state_dpp_soc_arad_pp_oam_diagnostic_cbs_t oam;
    sw_state_dpp_soc_arad_pp_oamp_pe_diagnostic_cbs_t oamp_pe;
    sw_state_dpp_soc_arad_pp_bfd_diagnostic_cbs_t bfd;
    sw_state_dpp_soc_arad_pp_oper_mode_diagnostic_cbs_t oper_mode;
    sw_state_dpp_soc_arad_pp_overlay_arp_diagnostic_cbs_t overlay_arp;
    sw_state_dpp_soc_arad_pp_ipv4_info_diagnostic_cbs_t ipv4_info;
    sw_state_dpp_soc_arad_pp_rif_to_lif_group_map_diagnostic_cbs_t rif_to_lif_group_map;
    sw_state_dpp_soc_arad_pp_vdc_vsi_map_diagnostic_cbs_t vdc_vsi_map;
    sw_state_dpp_soc_arad_pp_vt_profile_info_diagnostic_cbs_t vt_profile_info;
    sw_state_dpp_soc_arad_pp_dpp_parser_info_diagnostic_cbs_t dpp_parser_info;
#if defined(BCM_88675_A0)
    sw_state_dpp_soc_arad_pp_policer_size_profile_diagnostic_cbs_t policer_size_profile;
#endif /* defined(BCM_88675_A0)*/ 
    sw_state_dpp_soc_arad_pp_ptp_diagnostic_cbs_t ptp;
} sw_state_dpp_soc_arad_pp_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
typedef struct sw_state_dpp_soc_arad_diagnostic_cbs_s {
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_soc_arad_dump_cb dump;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_soc_arad_tm_diagnostic_cbs_t tm;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#if defined(BCM_PETRA_SUPPORT)
    sw_state_dpp_soc_arad_pp_diagnostic_cbs_t pp;
#endif /* defined(BCM_PETRA_SUPPORT)*/ 
} sw_state_dpp_soc_arad_diagnostic_cbs_t;

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 

int sw_state_dpp_soc_arad_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_API_TEST */

#endif /* _SHR_SW_STATE_DPP_SOC_ARAD_DIAGNOSTIC_H_ */
