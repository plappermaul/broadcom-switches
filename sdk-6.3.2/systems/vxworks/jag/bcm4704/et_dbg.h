/*
    Copyright 2001, Broadcom Corporation
    All Rights Reserved.
    
    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Broadcom Corporation.
*/
/*
 * Minimal debug/trace/assert driver definitions for
 * Broadcom Home Networking Division 10/100 Mbit/s Ethernet
 * Device Driver.
 *
 * Copyright (C) 2000 Broadcom Corporation
 * $Id: et_dbg.h,v 1.4 2012/03/02 15:27:42 yaronm Exp $
 */

#ifndef _et_dbg_
#define _et_dbg_

/* #define	BCMDBG*/
#ifdef	BCMDBG
/*
 * et_msg_level is a bitvector:
 *	0	errors
 *	1	function-level tracing
 *	2	one-line frame tx/rx summary
 *	3	complex frame tx/rx in hex
 */
#define	ET_ERROR(args)	if (!(et_msg_level & 1)) ; else printf args
#define	ET_TRACE(args)	if (!(et_msg_level & 2)) ; else printf args
#define	ET_PRHDR(msg, eh, len)	if (!(et_msg_level & 4)) ; else etc_prhdr(msg, eh, len)
#define	ET_PRPKT(msg, buf, len)	if (!(et_msg_level & 8)) ; else prhex(msg, buf, len)
#define	EB_TRACE(args)	if (!(et_msg_level & 0x10)) ; else soc_cm_print args
extern void etc_prhdr(char *msg, struct ether_header *eh, uint len);
#else	/* BCMDBG */
#define	ET_ERROR(args)
#define	ET_TRACE(args)
#define	ET_PRHDR(msg, eh, len)
#define	ET_PRPKT(msg, buf, len)
#define	EB_TRACE(args)
#endif	/* BCMDBG */

extern int et_msg_level;

#ifdef BCMDBG
#define	ET_LOG(fmt, a1)		if (!(et_msg_level & 2048)) ; else et_log(fmt, a1)
extern void et_log(char *fmt, unsigned long a1);
#else
#define	ET_LOG(fmt, a1)
#endif

/* include port-specific tunables */
#ifdef NDIS
#include <et_ndis.h>
#elif vxworks
#include <et_vx.h>
#elif linux
#include <et_linux.h>
#elif PMON
#include <et_pmon.h>
#else
#error
#endif

#endif /* _et_dbg_ */
