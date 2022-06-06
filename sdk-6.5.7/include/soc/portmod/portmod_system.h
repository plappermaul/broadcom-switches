/*
 *         
 * $Id:$
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *         
 *     
 *
 */

#ifndef _PORTMOD_SYSTEM_H_
#define _PORTMOD_SYSTEM_H_

int portmod_sys_get_endian(int unit, int *endian);
void* portmod_sys_dma_alloc(int unit, int size, char *name);
void portmod_sys_dma_free(int unit, void *ptr);

#endif /*_PORTMOD_SYSTEM_H_*/
