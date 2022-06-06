/* $Id: chip_sim_FAP20V.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/



#ifndef _CHIP_SIM_FAP20V_H_
/* { */
#define _CHIP_SIM_FAP20V_H_


#include "chip_sim.h"
#include "chip_sim_counter.h"
#include "chip_sim_interrupts.h"
#include "chip_sim_indirect.h"

extern
  CHIP_SIM_INDIRECT_BLOCK
    Fap20v_indirect_blocks[];

extern
  CHIP_SIM_INDIRECT_BLOCK
    Fap20v_b_indirect_blocks[];

extern
  CHIP_SIM_COUNTER
    Fap20v_counters[];
extern
  CHIP_SIM_INTERRUPT
    Fap20v_interrupts[];

/* } _CHIP_SIM_FAP20V_H_*/
#endif

