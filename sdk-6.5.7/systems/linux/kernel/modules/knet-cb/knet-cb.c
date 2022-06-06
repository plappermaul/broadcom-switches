/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

/*
 * Test driver for call-back functions in Linux KNET driver.
 *
 * The module can be built from the standard Linux user mode target
 * directories using the following command (assuming bash), e.g.
 *
 *   cd $SDK/systems/linux/user/gto-2_6
 *   BUILD_KNET_CB=1 make -s mod
 *
 */

#include <gmodule.h> /* Must be included first */
#include <kcom.h>
#include <bcm-knet.h>

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Broadcom Linux KNET Call-Back Driver");
MODULE_LICENSE("Proprietary");

/* Module Information */
#define MODULE_MAJOR 121
#define MODULE_NAME "linux-knet-cb"

static void
show_mac(struct sk_buff *skb)
{
    printk("DMAC=%02X:%02X:%02X:%02X:%02X:%02X\n",
           skb->data[0], skb->data[1], skb->data[2],
           skb->data[3], skb->data[4], skb->data[5]);
}

static struct sk_buff *
test_rx_cb(struct sk_buff *skb, int dev_no, void *meta)
{
    printk("rx_cb for dev %d\n", dev_no);
    if (skb->data[5] == 0x03) {
        dev_kfree_skb(skb);
        return NULL;
    }
    show_mac(skb);
    return skb;
}

static struct sk_buff *
test_tx_cb(struct sk_buff *skb, int dev_no, void *meta)
{
    printk("tx_cb for dev %d\n", dev_no);
    show_mac(skb);
    skb->data[5] += 1;
    return skb;
}

static int
test_filter_cb(uint8_t *pkt, int size, int dev_no, void *meta,
               int chan, kcom_filter_t *kf)
{
    printk("filter_cb (%d) for dev %d\n", kf->dest_id, dev_no);
    if (pkt[12] == 0x81 && pkt[13] == 0x00) {
        printk("  VTAG %d\n", pkt[15]);
        kf->dest_type = KCOM_DEST_T_NETIF;
        kf->dest_id = pkt[15];
        return 1;
    }
    return 0;
}

/*
 * Generic module functions
 */

static int
_pprint(void)
{   
    pprintf("Broadcom Linux KNET Call-Back Driver\n");

    return 0;
}

static int
_cleanup(void)
{
    bkn_rx_skb_cb_unregister(test_rx_cb);
    bkn_tx_skb_cb_unregister(test_tx_cb);
    bkn_filter_cb_unregister(test_filter_cb);

    return 0;
}   

static int
_init(void)
{
    bkn_rx_skb_cb_register(test_rx_cb);
    bkn_tx_skb_cb_register(test_tx_cb);
    bkn_filter_cb_register(test_filter_cb);

    return 0;
}

static gmodule_t _gmodule = {
    name: MODULE_NAME, 
    major: MODULE_MAJOR, 
    init: _init,
    cleanup: _cleanup, 
    pprint: _pprint, 
    ioctl: NULL,
    open: NULL, 
    close: NULL, 
}; 

gmodule_t*
gmodule_get(void)
{
    EXPORT_NO_SYMBOLS;
    return &_gmodule;
}
