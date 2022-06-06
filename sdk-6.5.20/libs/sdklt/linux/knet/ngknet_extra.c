/*! \file ngknet_extra.c
 *
 * Utility routines for NGKNET enhancement.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <linux/kconfig.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <asm/io.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/net_tstamp.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <linux/time.h>

#include <lkm/ngknet_dev.h>
#include <bcmcnet/bcmcnet_core.h>
#include "ngknet_main.h"
#include "ngknet_extra.h"
#include "ngknet_callback.h"

static struct ngknet_rl_ctrl rl_ctrl;

int
ngknet_filter_create(struct ngknet_dev *dev, ngknet_filter_t *filter)
{
    struct filt_ctrl *fc = NULL;
    struct list_head *list = NULL;
    ngknet_filter_t *filt = NULL;
    unsigned long flags;
    int num, id, done = 0;

    switch (filter->type) {
    case NGKNET_FILTER_T_RX_PKT:
        break;
    default:
        return SHR_E_UNAVAIL;
    }

    spin_lock_irqsave(&dev->lock, flags);

    num = (long)dev->fc[0];
    for (id = 1; id < num + 1; id++) {
        if (!dev->fc[id]) {
            break;
        }
    }
    if (id > NUM_FILTER_MAX) {
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_RESOURCE;
    }

    fc = kzalloc(sizeof(*fc), GFP_KERNEL);
    if (!fc) {
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_MEMORY;
    }

    dev->fc[id] = fc;
    num += id == (num + 1) ? 1 : 0;
    dev->fc[0] = (void *)(long)num;

    memcpy(&fc->filt, filter, sizeof(fc->filt));
    fc->filt.id = id;

    list_for_each(list, &dev->filt_list) {
        filt = &((struct filt_ctrl *)list)->filt;
        if (filt->flags & NGKNET_FILTER_F_MATCH_CHAN) {
            if (!(fc->filt.flags & NGKNET_FILTER_F_MATCH_CHAN) ||
                fc->filt.chan > filt->chan) {
                continue;
            }
            if (fc->filt.chan < filt->chan ||
                fc->filt.priority < filt->priority) {
                list_add_tail(&fc->list, list);
                done = 1;
                break;
            }
        } else {
            if (fc->filt.flags & NGKNET_FILTER_F_MATCH_CHAN ||
                fc->filt.priority < filt->priority) {
                list_add_tail(&fc->list, list);
                done = 1;
                break;
            }
        }
    }
    if (!done) {
        list_add_tail(&fc->list, &dev->filt_list);
    }

    filter->id = fc->filt.id;

    spin_unlock_irqrestore(&dev->lock, flags);

    return SHR_E_NONE;
}

int
ngknet_filter_destroy(struct ngknet_dev *dev, int id)
{
    struct filt_ctrl *fc = NULL;
    unsigned long flags;
    int num;

    if (id <= 0 || id > NUM_FILTER_MAX) {
        return SHR_E_PARAM;
    }

    spin_lock_irqsave(&dev->lock, flags);

    fc = (struct filt_ctrl *)dev->fc[id];
    if (!fc) {
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_NOT_FOUND;
    }

    list_del(&fc->list);
    kfree(fc);

    dev->fc[id] = NULL;
    num = (long)dev->fc[0];
    while (num-- == id--) {
        if (dev->fc[id]) {
            dev->fc[0] = (void *)(long)num;
            break;
        }
    }

    spin_unlock_irqrestore(&dev->lock, flags);

    return SHR_E_NONE;
}

int
ngknet_filter_destroy_all(struct ngknet_dev *dev)
{
    int id;
    int rv;

    for (id = 1; id <= NUM_FILTER_MAX; id++) {
        rv = ngknet_filter_destroy(dev, id);
        if (SHR_FAILURE(rv)) {
            return rv;
        }
    }

    return SHR_E_NONE;
}

int
ngknet_filter_get(struct ngknet_dev *dev, int id, ngknet_filter_t *filter)
{
    struct filt_ctrl *fc = NULL;
    unsigned long flags;
    int num;

    if (id <= 0 || id > NUM_FILTER_MAX) {
        return SHR_E_PARAM;
    }

    spin_lock_irqsave(&dev->lock, flags);

    fc = (struct filt_ctrl *)dev->fc[id];
    if (!fc) {
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_NOT_FOUND;
    }

    memcpy(filter, &fc->filt, sizeof(*filter));

    num = (long)dev->fc[0];
    for (id++; id < num + 1; id++) {
        if (dev->fc[id]) {
            break;
        }
    }
    filter->next = id == (num + 1) ? 0 : id;

    spin_unlock_irqrestore(&dev->lock, flags);

    return SHR_E_NONE;
}

int
ngknet_filter_get_next(struct ngknet_dev *dev, ngknet_filter_t *filter)
{
    int id;
    int rv;

    if (!filter->next) {
        for (id = 1; id <= NUM_FILTER_MAX; id++) {
            rv = ngknet_filter_get(dev, id, filter);
            if (SHR_SUCCESS(rv)) {
                return rv;
            }
        }
        if (id > NUM_FILTER_MAX) {
            return SHR_E_NOT_FOUND;
        }
    }

    return ngknet_filter_get(dev, filter->next, filter);
}

int
ngknet_rx_pkt_filter(struct ngknet_dev *dev, struct sk_buff *skb, struct net_device **ndev,
                     struct net_device **mndev, struct sk_buff **mskb)
{
    struct pkt_buf *pkb = (struct pkt_buf *)skb->data;
    struct net_device *dest_ndev = NULL, *mirror_ndev = NULL;
    struct sk_buff *mirror_skb = NULL;
    struct ngknet_private *priv = NULL;
    struct filt_ctrl *fc = NULL;
    struct list_head *list = NULL;
    ngknet_filter_t scratch, *filt = NULL;
    uint8_t *oob = &pkb->data, *data = NULL;
    uint16_t tpid;
    unsigned long flags;
    int wsize;
    int chan_id;
    int idx, match = 0;

    bcmcnet_pdma_dev_queue_to_chan(&dev->pdma_dev, pkb->pkh.queue_id,
                                   PDMA_Q_RX, &chan_id);

    spin_lock_irqsave(&dev->lock, flags);

    dest_ndev = dev->bdev[chan_id];
    if (dest_ndev) {
        skb->dev = dest_ndev;
        priv = netdev_priv(dest_ndev);
        priv->users++;
        *ndev = dest_ndev;
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_NONE;
    }

    if (list_empty(&dev->filt_list)) {
        spin_unlock_irqrestore(&dev->lock, flags);
        return SHR_E_NONE;
    }

    list_for_each(list, &dev->filt_list) {
        fc = (struct filt_ctrl *)list;
        filt = &fc->filt;
        if (filt->flags & NGKNET_FILTER_F_ANY_DATA) {
            match = 1;
            break;
        }
        if (filt->flags & NGKNET_FILTER_F_MATCH_CHAN && filt->chan != chan_id) {
            continue;
        }
        memcpy(&scratch.data.b[0],
               &oob[filt->oob_data_offset], filt->oob_data_size);
        memcpy(&scratch.data.b[filt->oob_data_size],
               &pkb->data + pkb->pkh.meta_len + filt->pkt_data_offset,
               filt->pkt_data_size);
        wsize = NGKNET_BYTES2WORDS(filt->oob_data_size + filt->pkt_data_size);
        for (idx = 0; idx < wsize; idx++) {
            scratch.data.w[idx] &= filt->mask.w[idx];
            if (scratch.data.w[idx] != filt->data.w[idx]) {
                break;
            }
        }
        if (idx == wsize) {
            match = 1;
            break;
        }
    }

    if (match) {
        fc->hits++;
        switch (filt->dest_type) {
        case NGKNET_FILTER_DEST_T_NETIF:
            if (filt->dest_id == 0) {
                dest_ndev = dev->net_dev;
            } else {
                dest_ndev = dev->vdev[filt->dest_id];
            }
            if (dest_ndev) {
                skb->dev = dest_ndev;
                if (filt->dest_proto) {
                    pkb->pkh.attrs |= PDMA_RX_SET_PROTO;
                    skb->protocol = filt->dest_proto;
                }
                priv = netdev_priv(dest_ndev);
                priv->users++;
            }
            break;
        case NGKNET_FILTER_DEST_T_VNET:
            pkb->pkh.attrs |= PDMA_RX_TO_VNET;
            spin_unlock_irqrestore(&dev->lock, flags);
            return SHR_E_NO_HANDLER;
        case NGKNET_FILTER_DEST_T_NULL:
        default:
            spin_unlock_irqrestore(&dev->lock, flags);
            return SHR_E_UNAVAIL;
        }
    }

    spin_unlock_irqrestore(&dev->lock, flags);

    if (!dest_ndev) {
        return SHR_E_NONE;
    } else {
        *ndev = dest_ndev;
    }

    if (filt->flags & NGKNET_FILTER_F_STRIP_TAG) {
        pkb->pkh.attrs |= PDMA_RX_STRIP_TAG;
        data = skb->data + PKT_HDR_SIZE + pkb->pkh.meta_len;
        tpid = data[12] << 8 | data[13];
        if (tpid == ETH_P_8021Q || tpid == ETH_P_8021AD) {
            pkb->pkh.data_len -= VLAN_HLEN;
            memmove(skb->data + VLAN_HLEN, skb->data,
                    PKT_HDR_SIZE + pkb->pkh.meta_len + 2 * ETH_ALEN);
            skb_pull(skb, VLAN_HLEN);
        }
    }

    if (dev->cbc->rx_cb) {
        NGKNET_SKB_CB(skb)->filt = filt;
    }

    if (filt->mirror_type == NGKNET_FILTER_DEST_T_NETIF) {
        spin_lock_irqsave(&dev->lock, flags);
        if (filt->mirror_id == 0) {
            mirror_ndev = dev->net_dev;
        } else {
            mirror_ndev = dev->vdev[filt->mirror_id];
        }
        if (mirror_ndev) {
            mirror_skb = pskb_copy(skb, GFP_ATOMIC);
            if (mirror_skb) {
                mirror_skb->dev = mirror_ndev;
                if (filt->mirror_proto) {
                    pkb->pkh.attrs |= PDMA_RX_SET_PROTO;
                    mirror_skb->protocol = filt->mirror_proto;
                }
                if (dev->cbc->rx_cb) {
                    NGKNET_SKB_CB(mirror_skb)->filt = filt;
                }
                priv = netdev_priv(mirror_ndev);
                priv->users++;
                *mndev = mirror_ndev;
                *mskb = mirror_skb;
            }
        }
        spin_unlock_irqrestore(&dev->lock, flags);
    }

    return SHR_E_NONE;
}

static void
ngknet_rl_process(timer_context_t data)
{
    struct ngknet_rl_ctrl *rc = timer_arg(rc, data, timer);
    struct ngknet_dev *dev;
    unsigned long flags;
    int idx;

    spin_lock_irqsave(&rc->lock, flags);
    rc->rx_pkts = 0;
    for (idx = 0; idx < NUM_PDMA_DEV_MAX; idx++) {
        dev = &rc->devs[idx];
        if (rc->dev_active[idx] && rc->dev_paused[idx]) {
            bcmcnet_pdma_dev_rx_resume(&dev->pdma_dev);
            rl_ctrl.dev_paused[dev->dev_no] = 0;
        }
    }
    spin_unlock_irqrestore(&rc->lock, flags);

    rc->timer.expires = jiffies + HZ / rc->rx_ticks;
    add_timer(&rc->timer);
}

void
ngknet_rx_rate_limit_init(struct ngknet_dev *devs)
{
    sal_memset(&rl_ctrl, 0, sizeof(rl_ctrl));
    rl_ctrl.rx_ticks = 10;
    setup_timer(&rl_ctrl.timer, ngknet_rl_process, (timer_context_t)&rl_ctrl);
    spin_lock_init(&rl_ctrl.lock);
    rl_ctrl.devs = devs;
}

void
ngknet_rx_rate_limit_cleanup(void)
{
    del_timer_sync(&rl_ctrl.timer);
}

int
ngknet_rx_rate_limit_started(void)
{
    return rl_ctrl.started;
}

void
ngknet_rx_rate_limit_start(struct ngknet_dev *dev)
{
    unsigned long flags;

    spin_lock_irqsave(&rl_ctrl.lock, flags);
    rl_ctrl.dev_active[dev->dev_no] = 1;
    spin_unlock_irqrestore(&rl_ctrl.lock, flags);

    if (!rl_ctrl.started) {
        rl_ctrl.started = 1;
        rl_ctrl.timer.expires = jiffies + HZ / rl_ctrl.rx_ticks;
        add_timer(&rl_ctrl.timer);
    }
}

void
ngknet_rx_rate_limit_stop(struct ngknet_dev *dev)
{
    unsigned long flags;

    spin_lock_irqsave(&rl_ctrl.lock, flags);
    rl_ctrl.dev_active[dev->dev_no] = 0;
    spin_unlock_irqrestore(&rl_ctrl.lock, flags);
}

void
ngknet_rx_rate_limit(struct ngknet_dev *dev, int limit)
{
    unsigned long flags;

    spin_lock_irqsave(&rl_ctrl.lock, flags);
    if ((++rl_ctrl.rx_pkts + rl_ctrl.rx_overruns > limit / rl_ctrl.rx_ticks) &&
        !rl_ctrl.dev_paused[dev->dev_no] && rl_ctrl.dev_active[dev->dev_no]) {
        rl_ctrl.dev_paused[dev->dev_no] = 1;
        rl_ctrl.rx_overruns = 0;
        bcmcnet_pdma_dev_rx_suspend(&dev->pdma_dev);
    }
    if (rl_ctrl.dev_paused[dev->dev_no]) {
        rl_ctrl.rx_overruns++;
    }
    spin_unlock_irqrestore(&rl_ctrl.lock, flags);
}

void
ngknet_tx_queue_schedule(struct ngknet_dev *dev, struct sk_buff *skb, int *queue)
{
    struct pkt_buf *pkb = (struct pkt_buf *)skb->data;

    if (pkb->pkh.attrs & PDMA_TX_BIND_QUE) {
        *queue = pkb->pkh.queue_id;
    }
}

