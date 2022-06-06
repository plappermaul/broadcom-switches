/*
 * $Id:$
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * File:       eyescan.h
 */

#ifndef _EYESCAN_H_
#define _EYESCAN_H_

#include "types.h"

#define EYE_SCAN_HORIZ 0
#define EYE_SCAN_VERTICAL 1

#define _LADDER_SETTING_TO_MV(_y)     (_y * 300 / 127)

extern int bcm_do_eyescan(phy_ctrl_t *pc, int sys_port);


#endif /* _EYESCAN_H_ */
