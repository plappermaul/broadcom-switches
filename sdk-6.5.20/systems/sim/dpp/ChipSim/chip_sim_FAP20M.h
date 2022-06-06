/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
*/



#ifndef _CHIP_SIM_FAP20M_H_
/* { */
#define _CHIP_SIM_FAP20M_H_


#include "chip_sim.h"
#include "chip_sim_counter.h"
#include "chip_sim_interrupts.h"
#include "chip_sim_indirect.h"

extern
  CHIP_SIM_INDIRECT_BLOCK
    Fap20m_indirect_blocks[];

extern
  CHIP_SIM_COUNTER
    Fap20m_counters[];
extern
  CHIP_SIM_INTERRUPT
    Fap20m_interrupts[];

/* } _CHIP_SIM_FAP20M_H_*/
#endif

