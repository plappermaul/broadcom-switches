/*
* $Id: $
* $Copyright: (c) 2016 Broadcom.
* Broadcom Proprietary and Confidential. All rights reserved.$
*
* $File:  tomahawk2_idb_flexport.h
*/


#ifndef TOMAHAWK2_IDB_FLEXPORT_H
#define TOMAHAWK2_IDB_FLEXPORT_H

extern int soc_tomahawk2_get_pipe_from_phy_pnum(int pnum);


extern int soc_tomahawk2_get_pm_from_phy_pnum(int pnum);


extern int soc_tomahawk2_get_subp_from_phy_pnum(int pnum);


extern int soc_tomahawk2_idb_init_pnum_mapping_tbl(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_invalidate_pnum_mapping_tbl(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_clear_stats_new_ports(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_obm_reset_buffer(int unit, int pipe_num,
                                              int pm_num, int subp,
                                              int reset_buffer);


extern int soc_tomahawk2_idb_ca_reset_buffer(int unit, int pipe_num, int pm_num,
                                             int subp,
                                             int reset_buffer);

extern int soc_tomahawk2_idb_lpbk_ca_reset_buffer(int unit, int pipe_num,
                                                  int reset_buffer);

extern int soc_tomahawk2_idb_cpu_ca_reset_buffer(int unit, int pipe_num,
                                                 int reset_buffer);


extern int soc_tomahawk2_idb_port_mode_set(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_wr_obm_shared_config(int unit, int pipe_num,
                                                  int pm_num, int subp,
                                                  int num_lanes,
                                                  int lossless);


extern int soc_tomahawk2_idb_wr_obm_flow_ctrl_cfg(int unit, int pipe_num,
                                                  int pm_num,
                                                  int subp,
                                                  int lossless);


extern int soc_tomahawk2_idb_wr_obm_ovs_en(int unit, int pipe_num, int pm_num,
                                            int subp);

extern int soc_tomahawk2_idb_wr_obm_thresh(int unit, int pipe_num, int pm_num,
                                           int subp,
                                           int num_lanes,
                                           int lossless);

extern int soc_tomahawk2_idb_wr_obm_fc_threshold(int unit, int pipe_num,
                                                 int pm_num, int subp,
                                                 int num_lanes,
                                                 int lossless);


extern int soc_tomahawk2_idb_obm_poll_buffer_empty(int unit, int pipe_num,
                                                   int pm_num,
                                                   int subp);


extern int soc_tomahawk2_idb_ca_poll_buffer_empty(int unit, int pipe_num,
                                                  int pm_num,
                                                  int subp);


extern int soc_tomahawk2_speed_to_bmop_class_map(int speed);


extern int soc_tomahawk2_flex_dis_forwarding_traffic(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_flex_en_forwarding_traffic(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_obm_port_config_set(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_obm_dscp_map_set(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_obm_bubble_mop_set(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_obm_force_saf_set(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_flex_idb_port_down(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_flex_idb_reconfigure(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_flex_idb_port_up(
    int unit, soc_port_schedule_state_t *port_schedule_state);


extern int soc_tomahawk2_idb_flexport(
    int unit, soc_port_schedule_state_t *port_schedule_state);


#endif
