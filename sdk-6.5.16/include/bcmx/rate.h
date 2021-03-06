/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_RATE_H__
#define __BCMX_RATE_H__

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcmx/lport.h>
#include <bcm/rate.h>

/* 
 * Configure/retrieve rate limit and on/off state of DLF, MCAST, and
 * BCAST limiting.
 */
extern int bcmx_rate_set(
    int val, 
    int flags);

/* 
 * Configure/retrieve rate limit and on/off state of DLF, MCAST, and
 * BCAST limiting.
 */
extern int bcmx_rate_get(
    int *val, 
    int *flags);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_mcast_set(
    int limit, 
    int flags, 
    bcmx_lport_t port);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_mcast_get(
    int *limit, 
    int *flags, 
    bcmx_lport_t port);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_dlfbc_set(
    int limit, 
    int flags, 
    bcmx_lport_t port);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_dlfbc_get(
    int *limit, 
    int *flags, 
    bcmx_lport_t port);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_bcast_set(
    int limit, 
    int flags, 
    bcmx_lport_t port);

/* 
 * Configure or retrieve rate limit for specified packet type on given
 * port.
 */
extern int bcmx_rate_bcast_get(
    int *limit, 
    int *flags, 
    bcmx_lport_t port);

/* 
 * Front ends to bcmx_*_rate_set/get functions. Uses a single data
 * structure to write into all the 3 rate control registers.
 */
extern int bcmx_rate_type_set(
    bcm_rate_limit_t *rl);

/* 
 * Front ends to bcmx_*_rate_set/get functions. Uses a single data
 * structure to write into all the 3 rate control registers.
 */
extern int bcmx_rate_type_get(
    bcm_rate_limit_t *rl);

/* 
 * Configure/retrieve metering rate limit for specified packet type on
 * given port.
 */
extern int bcmx_rate_bandwidth_set(
    bcmx_lport_t port, 
    int flags, 
    uint32 kbits_sec, 
    uint32 kbits_burst);

/* 
 * Configure/retrieve metering rate limit for specified packet type on
 * given port.
 */
extern int bcmx_rate_bandwidth_get(
    bcmx_lport_t port, 
    int flags, 
    uint32 *kbits_sec, 
    uint32 *kbits_burst);

#endif /* __BCMX_RATE_H__ */
