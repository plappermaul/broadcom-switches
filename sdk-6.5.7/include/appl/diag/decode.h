/*
 * $Id: decode.h,v 1.4 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * This header file defines the interface to the packet decode routines.
 */

#include <bcm/pkt.h>

extern	int	d_packet_format(char *, int type, void *, int, void *);
extern	int	d_packet_vformat(char *, char *, int type, void *, int, void *);

/* Packet "type" on entry into decode routines */

#define	DECODE_ETHER	1
#define	DECODE_IP	2
#define DECODE_TCP	3
#define	DECODE_ARP	4
#define	DECODE_GB_ETHER	5


#if defined(BROADCOM_DEBUG)
extern void bcm_pkt_dump(int unit, bcm_pkt_t *pkt, int dump_data);
#endif  /* BROADCOM_DEBUG */
