/* $Id: chip_sim_engine.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef _CHIP_SIM_ENGINE_H_
/* { */
#define _CHIP_SIM_ENGINE_H_

#include "chip_sim.h"

#ifdef SAND_LOW_LEVEL_SIMULATION
/* { */
/* } */
#else
/* { */
/* } */
#endif



STATUS chip_sim_engine_run(SOC_SAND_IN UINT32 time_diff);
void chip_sim_engine_set_cntr_enable(SOC_SAND_IN uint8 i_cntr_enable);
uint8 chip_sim_engine_get_cntr_enable(void);
void chip_sim_engine_set_int_enable(SOC_SAND_IN uint8 i_int_enable);


/* } _CHIP_SIM_ENGINE_H_*/
#endif


