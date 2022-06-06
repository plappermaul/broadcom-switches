/*
 * $Id: l3.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * This file defines L3 constants 
 *
 * Its contents are not used directly by applications; it is used only
 * by header files of parent APIs which need to share  L3 constants.
 */

#ifndef _SHR_L3_H
#define _SHR_L3_H

/* 
 *  Reserved VRF values 
 */
#define _SHR_L3_VRF_OVERRIDE  (-1) /* Matches before vrf specific entries.*/
#define _SHR_L3_VRF_GLOBAL    (-2) /* Matches after vrf specific entries. */
#define _SHR_L3_VRF_DEFAULT    (0) /* Default vrf id.                     */

#define _SHR_L3_IP6_ADDRLEN       (16)  /* IPv6 address length in bytes.      */
#define _SHR_L3_IP6_ADDR_WORD_LEN (4)  /* IPv6 address length in bytes.      */
#define _SHR_L3_IP6_MAX_NETLEN    ((_SHR_L3_IP6_ADDRLEN) * (8))
#define _SHR_L3_IP_ADDRLEN     (4)   /* IPv4 address length in bytes.      */
#define _SHR_L3_IP_MAX_NETLEN  ((_SHR_L3_IP_ADDRLEN) * (8))
#define _SHR_L3_EGRESS_IDX_MIN   (0x186A0) /* Egress object start index    */

typedef uint32  _shr_ip_addr_t;                          /* IP Address   */
typedef uint8   _shr_ip6_addr_t[_SHR_L3_IP6_ADDRLEN];    /* IPv6 Address */

extern int _shr_ip6_mask_create(uint8 *ip6, int len);
extern int _shr_ip6_mask_length(uint8 *mask);
extern uint32 _shr_ip_mask_create(int len);
extern int _shr_ip_mask_length(uint32 mask);

#endif	/* !_SHR_L3_H */
