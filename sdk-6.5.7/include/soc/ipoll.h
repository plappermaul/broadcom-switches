/*
 * $Id: ipoll.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Functions for polling IRQs instead of using hardware interrupts.
 *
 */

#ifndef _SOC_IPOLL_H
#define _SOC_IPOLL_H

typedef void (*ipoll_handler_t)(void *data);

extern int soc_ipoll_connect(int dev, ipoll_handler_t handler, void *data);
extern int soc_ipoll_disconnect(int dev);
extern int soc_ipoll_pause(int dev);
extern int soc_ipoll_continue(int dev);

#endif /* _SOC_IPOLL_H */
