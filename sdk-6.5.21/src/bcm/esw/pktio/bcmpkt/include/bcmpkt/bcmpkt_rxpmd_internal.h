/*! \file bcmpkt_rxpmd_internal.h
 *
 * RX Packet MetaData internal library.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMPKT_RXPMD_INTERNAL_H
#define BCMPKT_RXPMD_INTERNAL_H

#ifdef PKTIO_IMPL
#include <pktio_dep.h>
#else
#include <shr/shr_types.h>
#endif
#include <bcmpkt/bcmpkt_rxpmd.h>
#include <bcmpkt/bcmpkt_internal.h>

/*!
 * Array of RXPMD field getter functions for a particular device
 * type.
 */
typedef struct bcmpkt_rxpmd_fget_s {
    bcmpkt_field_get_f fget[BCMPKT_RXPMD_FID_COUNT];
} bcmpkt_rxpmd_fget_t;

/*!
 * Array of RXPMD field setter functions for a particular device
 * type. These functions are used for internally configuring packet
 * filter.
 */
typedef struct bcmpkt_rxpmd_fset_s {
    bcmpkt_field_set_f fset[BCMPKT_RXPMD_FID_COUNT];
} bcmpkt_rxpmd_fset_t;

/*!
 * Array of RXPMD field address and length getter functions for a multiple
 * words field of a particular device type. *addr is output address and return
 * length.
 */
typedef struct bcmpkt_rxpmd_figet_s {
    bcmpkt_ifield_get_f fget[BCMPKT_RXPMD_I_FID_COUNT];
} bcmpkt_rxpmd_figet_t;

#define BCMDRD_DEVLIST_ENTRY(_nm,_vn,_dv,_rv,_md,_pi,_bd,_bc,_fn,_cn,_pf,_pd,_r0,_r1) \
    extern void _bd##_rx_reason_decode(const uint32_t* data, bcmpkt_rx_reasons_t* reasons);
#define BCMDRD_DEVLIST_OVERRIDE
#include <bcmdrd/bcmdrd_devlist.h>

#define BCMDRD_DEVLIST_ENTRY(_nm,_vn,_dv,_rv,_md,_pi,_bd,_bc,_fn,_cn,_pf,_pd,_r0,_r1) \
    extern void _bd##_rx_reason_encode(const bcmpkt_rx_reasons_t* reasons, uint32_t* data);
#define BCMDRD_DEVLIST_OVERRIDE
#include <bcmdrd/bcmdrd_devlist.h>

#define BCMDRD_DEVLIST_ENTRY(_nm,_vn,_dv,_rv,_md,_pi,_bd,_bc,_fn,_cn,_pf,_pd,_r0,_r1) \
    extern void _bd##_rxpmd_view_info_get(bcmpkt_pmd_view_info_t *info);
#define BCMDRD_DEVLIST_OVERRIDE
#include <bcmdrd/bcmdrd_devlist.h>

#endif  /* BCMPKT_RXPMD_INTERNAL_H */
