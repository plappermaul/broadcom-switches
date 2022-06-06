/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:        hurricane4_flexport.h
 * Purpose:
 * Requires:
 */

#ifndef SOC_HURRICANE4_FLEXPORT_H
#define SOC_HURRICANE4_FLEXPORT_H

#include <soc/flexport/flexport_common.h>

/*** START SDK API COMMON CODE ***/

/*! @file hurricane4_flexport.h
    @brief FlexPort functions and structures for Trident3.
    Details are shown below.
*/

#include <soc/flexport/hurricane4/hurricane4_flexport_defines.h>

#include <soc/flexport/hurricane4/hurricane4_tdm_flexport.h>
#include <soc/flexport/hurricane4/hurricane4_idb_flexport.h>
#include <soc/flexport/hurricane4/hurricane4_ep_flexport.h>
#include <soc/flexport/hurricane4/hurricane4_mac_flexport.h>
#include <soc/flexport/hurricane4/hurricane4_mmu_flex_init.h>


#define MAX_FLEX_PHASES       25
#define HR4_GPHY_START         1
#define HR4_GPHY_END          24
#define HR4_PMQ_START         25
#define HR4_PMQ_END           56
#define HR4_CLP_START         57
#define HR4_CLP_END           60
#define HR4_XLP_START         61
#define HR4_XLP_END           64
#define HR4_NUM_PMQ_PORTS     16
#define HR4_NUM_QGPHY_PORTS   12
#define HR4_PORT_PER_PM        4
#define HR4_TOTAL_PHY_PORTS   64
#define HR4_MACSEC_START      65
#define HR4_MACSEC_END        68
#define HR4_NUM_MACSEC_PORTS   4

/*! @struct soc_hurricane4_flex_scratch_t
    @brief Scratch pad locals. Caller must not allocate. Callee allocates
           at entry and deallocates before return. This area is pointed
           to by the cookie in soc_port_schedule_state_t struct type.
*/
typedef struct soc_hurricane4_flex_scratch_s {
    /*!< Previous OVS tables needed by the OVS consolidation sequence */
    soc_tdm_schedule_pipe_t   prev_tdm_ingress_schedule_pipe[SOC_MAX_NUM_PIPES];
    soc_tdm_schedule_pipe_t   prev_tdm_egress_schedule_pipe[SOC_MAX_NUM_PIPES];
    /*!< Current state of OVS tables; is changing during flexport sequence
         initial state is prev OVS tables and after OVS flexport sequence
         converges to new_ OVS tables*/
    soc_tdm_schedule_pipe_t   curr_tdm_ingress_schedule_pipe[SOC_MAX_NUM_PIPES];
    soc_tdm_schedule_pipe_t   curr_tdm_egress_schedule_pipe[SOC_MAX_NUM_PIPES];
    /*!< Previous and new Oversub ratios; per PIPE and per HALF PIPE
     * These are coded as follows:
     *  10  - this is LR;
     *  15  - this is OVS with 3:2;         ovs_ratio <= 1.5;
     *  20  - this is OVS with 2:1;  1.5 <  ovs_ratio <= 2.0;
     */
    int prev_pipe_ovs_ratio[SOC_MAX_NUM_PIPES]; /* per PIPE ovs_ratio
                                                 * per Half PIPE ovs_ratio */
    int prev_hpipe_ovs_ratio[SOC_MAX_NUM_PIPES][HURRICANE4_OVS_HPIPE_COUNT_PER_PIPE];
    int new_pipe_ovs_ratio[SOC_MAX_NUM_PIPES]; /* per PIPE ovs_ratio
                                                * per Half PIPE ovs_ratio */
    int new_hpipe_ovs_ratio[SOC_MAX_NUM_PIPES][HURRICANE4_OVS_HPIPE_COUNT_PER_PIPE];

    /* This keeps the exact bitrate of the out_port_map.log_port_speed[] */
    enum port_speed_e exact_out_log_port_speed[SOC_MAX_NUM_PORTS];
} soc_hurricane4_flex_scratch_t;


/*! @fn void *soc_hurricane4_port_lane_info_alloc(void)
    @brief Allocates memory pointed to by soc_port_lane_info_t. Need this
           special function because SystemVerilog-C DPI limitations in
           synchronizing memory space that is a pointer to a struct type.
           Since s/w owns the the pointer to soc_port_lane_info_t struct
           in soc_port_resource_t, we cannot change it.
*/
extern void *soc_hurricane4_port_lane_info_alloc (
  void
  );


/*! @fn int soc_hurricane4_set_port_lane_info(int unit, const void *lane_info_ptr, int port_idx, int pgw, int xlp)
    @param unit Chip unit number.
    @param lane_info_ptr Pointer to a soc_port_lane_info_t struct variable.
    @param port_idx Index within XLP.
    @param pgw PGW instance where lane resides.
    @param xlp XLP number within PGW.
    @brief Sets all members in a soc_port_lane_info_t struct variable.
*/
extern int soc_hurricane4_set_port_lane_info (
  int unit,
  const void *lane_info_ptr,
  int port_idx,
  int pgw,
  int xlp
  );


/*! @fn int soc_hurricane4_get_port_lane_info(int unit, const void *lane_info_ptr, int *port_idx, int *pgw, int *xlp)
    @param unit Chip unit number.
    @param lane_info_ptr Pointer to a soc_port_lane_info_t struct variable.
    @param port_idx Index within XLP.
    @param pgw PGW instance where lane resides.
    @param xlp XLP number within PGW.
    @brief Gets all members in a soc_port_lane_info_t struct variable.
*/
extern int soc_hurricane4_get_port_lane_info (
  int unit,
  const void *lane_info_ptr,
  int *port_idx,
  int *pgw,
  int *xlp
  );


/*! @fn int soc_hurricane4_reconfigure_ports(int unit, soc_port_schedule_state_t *port_schedule_state)
    @param unit Chip unit number.
    @param port_schedule_state Pointer to data holding all required information for FlexPort opeations.
    @brief Top-level call carrying out all FlexPort functionalities.
*/
extern int soc_hurricane4_reconfigure_ports (
  int unit,
  soc_port_schedule_state_t *port_schedule_state
  );


/*! @fn int soc_hurricane4_flex_start(int unit, soc_port_schedule_state_t *port_schedule_state)
    @param unit Chip unit number.
    @param port_schedule_state Pointer to data holding all required information for FlexPort opeations.
    @brief The start of FlexPort series of operations along with allocations of required program memory.
*/
extern int soc_hurricane4_flex_start (
  int unit,
  soc_port_schedule_state_t *port_schedule_state
  );


/*! @fn int soc_hurricane4_flex_end(int unit, soc_port_schedule_state_t *port_schedule_state)
    @param unit Chip unit number.
    @param port_schedule_state Pointer to data holding all required information for FlexPort opeations.
    @brief The end of FlexPort series of operations along with deallocations of program memory.
*/
extern int soc_hurricane4_flex_end (
  int unit,
  soc_port_schedule_state_t *port_schedule_state
  );


extern int soc_hurricane4_flex_top_port_down (
    int                        unit,
    soc_port_schedule_state_t *port_schedule_state
    );


extern int soc_hurricane4_flex_top_port_up (
    int                        unit,
    soc_port_schedule_state_t *port_schedule_state
    );

extern int soc_hurricane4_flex_mmu_port_up_top  (
    int                        unit,
    soc_port_schedule_state_t *port_schedule_state
    );

extern int soc_hurricane4_get_ct_class(int speed);
/*! @fn int (*flexport_phases[MAX_FLEX_PHASES]) (int unit, soc_port_schedule_state_t *port_schedule_state)
    @param unit Chip unit number.
    @param port_schedule_state Pointer to data holding all required information for FlexPort opeations.
    @brief Phase control array containing the right sequence of FlexPort phases implemented as functions.
*/
extern int (*hr4_flexport_phases[MAX_FLEX_PHASES]) (int unit, soc_port_schedule_state_t *port_schedule_state);

extern int soc_hurricane4_flex_is_gphy_pmq_port(unsigned int phy_port);

extern int soc_hurricane4_flex_is_qgphy_port(unsigned int phy_port);

extern int soc_hurricane4_flex_is_pmq_port(unsigned int phy_port);

extern int soc_hurricane4_flex_is_eagle_port(unsigned int phy_port);

extern int soc_hurricane4_flex_is_falcon_port(unsigned int phy_port);

extern int soc_hurricane4_flex_is_macsec_port(unsigned int phy_port);

/*** END SDK API COMMON CODE ***/


#endif /* SOC_HURRICANE4_FLEXPORT_H */
