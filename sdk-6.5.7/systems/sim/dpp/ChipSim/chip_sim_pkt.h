/* $Id: chip_sim_pkt.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __CHIP_SIM_PKT_H__
/* { */
#define __CHIP_SIM_PKT_H__

#include "chip_sim.h"



/*
 * INCLUDE FILES:
 * {
 */

#ifdef SAND_LOW_LEVEL_SIMULATION
/* { */
/* } */
#else
/* { */
/* } */
#endif

/*  } */


void chip_sim_pkt_init(void);
STATUS chip_sim_pkt_init_chip_specifics(SOC_SAND_IN UINT32 i_access_trig_address);
STATUS chip_sim_send_pkt_op(SOC_SAND_IN int chip_ndx);
uint8 chip_sim_mem_is_pkt_trig(SOC_SAND_IN UINT32 chip_offset);
UINT32 chip_sim_mem_get_pkt_delay(void);
STATUS chip_sim_mem_set_pkt_delay_op(SOC_SAND_IN int chip_ndx);


/* } __CHIP_SIM_PKT_H__*/
#endif
