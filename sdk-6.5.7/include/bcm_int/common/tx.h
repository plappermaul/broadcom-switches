/*
 * $Id: tx.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        tx.h
 * Purpose:     Internal structures and definitions for TX module
 */
#ifndef   _BCM_COMMON_TX_H_
#define   _BCM_COMMON_TX_H_

#include <soc/dma.h>
#include <bcm/error.h>
#include <bcm/tx.h>

extern int
bcm_common_tx_pkt_setup(int unit, bcm_pkt_t *tx_pkt);

extern int
bcm_common_tx_init(int unit);

extern int
bcm_common_tx_deinit(int unit);

extern int
bcm_common_tx(int unit, bcm_pkt_t *pkt, void *cookie);

extern int
bcm_common_tx_array(int unit, bcm_pkt_t **pkt, int count, bcm_pkt_cb_f all_done_cb,
		    void *cookie);

extern int
bcm_common_tx_list(int unit, bcm_pkt_t *pkt, bcm_pkt_cb_f all_done_cb, void *cookie);

extern int
bcm_common_tx_pkt_l2_map(int unit, bcm_pkt_t *pkt, bcm_mac_t dest_mac, int vid);

#if defined(BROADCOM_DEBUG)
extern int
bcm_common_tx_show(int unit);

extern int
bcm_common_tx_dv_dump(int unit, void *dv_p);
#endif  /* BROADCOM_DEBUG */

#endif /* _BCM_COMMON_TX_H_ */
