/*
* $Id: $
* $Copyright: (c) 2016 Broadcom.
* Broadcom Proprietary and Confidential. All rights reserved.$
*
* $File:  tomahawk2_mmu_flex_init.h
*/


#ifndef _SOC_TOMAHAWK2_MMU_FLEXPORT_H_
#define _SOC_TOMAHAWK2_MMU_FLEXPORT_H_


#include <soc/flexport/flexport_common.h>
#include <soc/flexport/tomahawk2_flexport_defines.h>

#include <soc/flexport/tomahawk2_mmu_defines.h>
#include <soc/flexport/tomahawk2_tdm_flexport.h>


/*! @file tomahawk2_mmu_flex_init.h
 *  @brief Mmu FlexPort functions and structures for Tomahawk2.
 *  Details are shown below.
 */


/* Some functions from MMU_CONFIG */
extern int soc_tomahawk2_mmu_get_num_l0_nodes_per_port(int unit, int lcl_port,
                                                       int pipe,
                                                       int *number_l0_nodes);


extern int soc_tomahawk2_mmu_get_num_l1_uc_nodes_per_port(
    int unit, int lcl_port, int pipe, int *number_l1_uc_nodes);


extern int soc_tomahawk2_mmu_get_num_l1_mc_nodes_per_port(
    int unit, int lcl_port, int pipe, int *number_l1_mc_nodes);


extern int soc_tomahawk2_mmu_get_l0_base_offset_for_port(int unit, int lcl_port,
                                                         int *l0_offset);


extern int soc_tomahawk2_mmu_get_l1_base_offset_for_port(int unit, int lcl_port,
                                                         int uc_bit,
                                                         int *l1_offset);

extern int soc_tomahawk2_mmu_get_valid_ipipes_for_xpe(int unit, int xpe,
                                                      int *ipipes);


extern int soc_tomahawk2_mmu_get_valid_epipes_for_xpe(int unit, int xpe,
                                                      int *epipes);

extern int soc_tomahawk2_mmu_is_xpe_valid(int unit, int xpe, int *valid);


extern int soc_tomahawk2_get_min_max_src_ct_speed(int unit,int speed_decode,
                                                  int ct_mode, int ct_profile,
                                                  uint64 *min_speed,
                                                  uint64 *max_speed);


extern int soc_tomahawk2_get_asf_xmit_start_count(int unit, int dst_decode,
                                                  int ct_mode,
                                                  uint64 *asf_count);


extern int soc_tomahawk2_mmu_get_bst_mode(int unit, int *bst_mode);


extern int soc_tomahawk2_mmu_get_pktstat_mode(int unit, int *pktstat_mode);


extern int soc_tomahawk2_mmu_get_ct_ep_credit_low_high_threshold(
    int unit, int speed_decode, int frequency, int oversub, int oversub_ratio,
    uint64 *ep_credit_low, uint64 *ep_credit_hi);


extern int soc_tomahawk2_get_ct_fifo_threshold_depth(int unit, int dst_decode,
                                                     int ct_mode, int oversub,
                                                     uint64 *fifo_thresh,
                                                     uint64 *fifo_depth);


extern int soc_tomahawk2_mmu_return_dip_delay_amount(int unit, int port_speed,
                                                     uint64 *dip_amount);


extern int soc_tomahawk2_mmu_thdi_get_pg_hdrm_setting(int unit, int speed,
                                                      int line_rate,
                                                      uint32 *setting);


/*PORT FLUSH Functions from RQE_VBS_MTRO */
extern int soc_tomahawk2_mmu_vbs_port_flush(
    int unit, soc_port_resource_t *port_resource_t, uint64 set_val);


extern int soc_tomahawk2_mmu_rqe_port_flush(
    int unit, soc_port_resource_t *port_resource_t, uint64 set_val);


extern int soc_tomahawk2_mmu_mtro_port_flush(
    int unit, soc_port_resource_t *port_resource_t, uint64 set_val);


/* THDI/WRED Clear Functions */
extern int soc_tomahawk2_mmu_thdi_bst_clr(int                  unit,
                                          soc_port_resource_t *port_resource_t);


extern int soc_tomahawk2_mmu_wred_clr(int                  unit,
                                      soc_port_resource_t *port_resource_t);


extern int soc_tomahawk2_mmu_get_xpe_from_pipe(int pipe, pipe_t direction,
                                               int* xpe_offs);


extern int soc_tomahawk2_mmu_ctr_clr(int                  unit,
                                     soc_port_resource_t *port_resource_t);


/* THDI Setup function */
extern int soc_tomahawk2_mmu_thdi_setup(int                  unit,
                                        soc_port_resource_t *port_resource_t,
                                        int                  lossy);


/* TDM Port EP Credit Clear function*/
extern int soc_tomahawk2_mmu_clear_prev_ep_credits(
    int unit, soc_port_resource_t *port_resource_t);


/*PORT MAPPING */
extern int soc_tomahawk2_mmu_set_mmu_to_phy_port_mapping(
    int unit,soc_port_resource_t *port_resource_t);


/*VBS Credit Clear Functions */
extern int soc_tomahawk2_mmu_clear_vbs_credit_memories(
    int unit,soc_port_resource_t *port_resource_t);


/* MTRO Credit Clear Functions */
extern int soc_tomahawk2_mmu_clear_mtro_bucket_mems(
    int unit, soc_port_resource_t *port_resource_t);


/*CT Setting Functions */
extern int soc_tomahawk2_mmu_reinit_ct_setting(
    int unit,soc_port_resource_t *port_resource_t, soc_asf_mode_e ct_mode,
    soc_asf_mem_profile_e ct_profile, int frequency,
    int exact_speed, int oversub_ratio);


/* Port Up Sequence */
extern int soc_tomahawk2_flex_mmu_port_up(
    int unit, soc_port_schedule_state_t *port_schedule_state_t);


/* Port Down Sequence */
extern int soc_tomahawk2_flex_mmu_port_down(
    int unit, soc_port_schedule_state_t *port_schedule_state_t);


/* Reconfigure Sequence Phase 1 Phase 2 */
extern int soc_tomahawk2_flex_mmu_reconfigure_phase1(
    int unit, soc_port_schedule_state_t *port_schedule_state_t);


extern int soc_tomahawk2_flex_mmu_reconfigure_phase2(
    int unit, soc_port_schedule_state_t *port_schedule_state_t);


/* DIP/ForceSAF Sequence */
extern int soc_tomahawk2_mmu_reinit_dip_fsaf_setting(
    int unit, soc_port_resource_t *port_resource_t);


/* THDU functions */
extern int soc_tomahawk2_mmu_thdu_bst_clr(
    int                  unit,
    soc_port_resource_t *port_resource_t
    );


extern int soc_tomahawk2_mmu_thdu_pktstat_clr(
    int                  unit,
    soc_port_resource_t *port_resource_t
    );


extern int soc_tomahawk2_mmu_thdu_qgrp_min_limit_config(
    int                  unit,
    soc_port_resource_t *port_resource_t,
    int                  lossy
    );


/*THDM functions */
extern int soc_tomahawk2_mmu_thdm_bst_clr (
    int                  unit,
    soc_port_resource_t *port_resource_t
    );


#endif
