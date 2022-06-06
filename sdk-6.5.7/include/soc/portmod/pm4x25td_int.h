/*
 *         
 * $Id:$
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *         
 *
 */

#ifndef _PM4X25TD_INT_H_
#define _PM4X25TD_INT_H_

#include <soc/portmod/portmod.h>

int _pm4x25td_pm_enable(int unit, int port, pm_info_t pm_info, int port_index, const portmod_port_add_info_t* add_info, int enable);
int _pm4x25td_pm_port_init(int unit, int port, int internal_port, const portmod_port_add_info_t* add_info, int enable);

#endif /*_PM4X25TD_INT_H_*/
