/*
 * $Id: ipoll.c,v 1.6 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Functions for polling IRQs instead of using hardware interrupts.
 * Primarily intended for board bringup and debugging.
 *
 */

#include <sal/core/libc.h>
#include <sal/core/spl.h>
#include <sal/core/sync.h>

#include <soc/drv.h>
#include <soc/ipoll.h>
#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif

#ifndef IPOLL_THREAD_DELAY
#define IPOLL_THREAD_DELAY      0
#endif

#ifndef IPOLL_THREAD_PRIO
#define IPOLL_THREAD_PRIO       100
#endif

typedef struct ipoll_ctrl_s {
    void *data;
    ipoll_handler_t handler;
    int paused;
} ipoll_ctrl_t;

static ipoll_ctrl_t _ictrl[SOC_MAX_NUM_DEVICES];
static int _ihandlers;

#define CMCx_IRQ0_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT0_OFFSET(_cmc))
#define CMCx_IRQ1_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT1_OFFSET(_cmc))
#define CMCx_IRQ2_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT2_OFFSET(_cmc))
#define CMCx_IRQ3_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT3_OFFSET(_cmc))
#define CMCx_IRQ4_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT4_OFFSET(_cmc))
#define CMCx_IRQ5_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT5_OFFSET(_cmc))
#define CMCx_IRQ6_STAT(_dev,_cmc) \
    soc_pci_read(_dev, CMIC_CMCx_IRQ_STAT6_OFFSET(_cmc))
/*
 * Function:
 *      soc_cmic_ipoll_handler
 * Description:
 *      Main CMIC interrupt handler in polled IRQ mode
 * Parameters:
 *      dev - device number
 * Returns:
 *      Nothing
 */
STATIC void
soc_cmic_ipoll_handler(int dev)
{
    if (soc_pci_read(dev, CMIC_IRQ_STAT) & SOC_IRQ_MASK(dev)) {
        _ictrl[dev].handler(_ictrl[dev].data);
    } else {
        if ((soc_feature(dev, soc_feature_extended_cmic_error)) ||
            (soc_feature(dev, soc_feature_short_cmic_error))) {
            if(soc_pci_read(dev, CMIC_IRQ_STAT_1) & SOC_IRQ1_MASK(dev) ||
               soc_pci_read(dev, CMIC_IRQ_STAT_2) & SOC_IRQ2_MASK(dev)) {
                _ictrl[dev].handler(_ictrl[dev].data);
            } 
        }
    }
}

/*
 * Function:
 *      soc_cmicm_ipoll_handler
 * Description:
 *      Main CMICm interrupt handler in polled IRQ mode
 * Parameters:
 *      dev - device number
 * Returns:
 *      Nothing
 */
STATIC void
soc_cmicm_ipoll_handler(int dev)
{
#ifdef BCM_CMICM_SUPPORT
    int cmc;

    cmc = SOC_PCI_CMC(dev);
#if defined(BCM_ESW_SUPPORT) || defined(BCM_CALADAN3_SUPPORT) || defined(BCM_SAND_SUPPORT)
    if ((CMCx_IRQ0_STAT(dev, cmc) & SOC_CMCx_IRQ0_MASK(dev, cmc)) || 
        (CMCx_IRQ1_STAT(dev, cmc) & SOC_CMCx_IRQ1_MASK(dev, cmc)) ||
        (CMCx_IRQ2_STAT(dev, cmc) & SOC_CMCx_IRQ2_MASK(dev, cmc))) {
        _ictrl[dev].handler(_ictrl[dev].data);
    } else {
        if (soc_feature(dev, soc_feature_cmicm_multi_dma_cmc)) {
            if ((CMCx_IRQ0_STAT(dev, SOC_ARM_CMC(dev, 0)) & 
                 SOC_CMCx_IRQ0_MASK(dev, SOC_ARM_CMC(dev, 0))) || 
                (CMCx_IRQ0_STAT(dev, SOC_ARM_CMC(dev, 1)) &
                 SOC_CMCx_IRQ0_MASK(dev, SOC_ARM_CMC(dev, 1)))) {
                _ictrl[dev].handler(_ictrl[dev].data);
            }
        }
        if (soc_feature(dev, soc_feature_extended_cmic_error) ||
            soc_feature(dev, soc_feature_short_cmic_error)) {
            if ((CMCx_IRQ3_STAT(dev, cmc) & SOC_CMCx_IRQ3_MASK(dev,cmc)) ||
                (CMCx_IRQ4_STAT(dev, cmc) & SOC_CMCx_IRQ4_MASK(dev,cmc))) {
                _ictrl[dev].handler(_ictrl[dev].data);
            }
            if (soc_feature(dev, soc_feature_cmicm_extended_interrupts)) {
                if ((CMCx_IRQ5_STAT(dev, cmc) & SOC_CMCx_IRQ5_MASK(dev,cmc)) ||
                    (CMCx_IRQ6_STAT(dev, cmc) & SOC_CMCx_IRQ6_MASK(dev,cmc))) {
                    _ictrl[dev].handler(_ictrl[dev].data);
                }
            }
        }
        if (soc_feature(dev, soc_feature_cmicm_extended_interrupts)) {
#ifdef BCM_CMICDV2_SUPPORT
            if (soc_feature(dev, soc_feature_cmicd_v2)) {
                if ((CMCx_IRQ3_STAT(dev, cmc) & SOC_CMCx_IRQ3_MASK(dev,cmc)) ||
                    (CMCx_IRQ4_STAT(dev, cmc) & SOC_CMCx_IRQ4_MASK(dev,cmc)) ||
                    (CMCx_IRQ5_STAT(dev, cmc) & SOC_CMCx_IRQ5_MASK(dev,cmc))) {
                    _ictrl[dev].handler(_ictrl[dev].data);
                }
#ifdef BCM_CMICDV4_SUPPORT
                if (soc_feature(dev, soc_feature_cmicd_v4)) {
                    if (CMCx_IRQ6_STAT(dev, cmc) & SOC_CMCx_IRQ6_MASK(dev,cmc)) {
                        _ictrl[dev].handler(_ictrl[dev].data);
                    }
                }
#endif /* BCM_CMICDV4_SUPPORT */
            }
#endif /* BCM_CMICDV2_SUPPORT */
        }
    }
#endif
#endif
}

/*
 * Function:
 *      soc_ipoll_thread
 * Description:
 *      Thread context for interrupt handlers in polled IRQ mode
 * Parameters:
 *      data - poll delay in usecs (passed as pointer)
 * Returns:
 *      Nothing
 */
STATIC void
soc_ipoll_thread(void *data)
{
    int dev, spl, udelay;

    udelay = PTR_TO_INT(data);

    while (_ihandlers) {

        spl = sal_splhi();

        for (dev = 0; dev < SOC_MAX_NUM_DEVICES; dev++) {
            if (_ictrl[dev].handler != NULL && !_ictrl[dev].paused) {
                if (soc_feature(dev, soc_feature_cmicm)) {
                    soc_cmicm_ipoll_handler(dev);
                } else {
                    soc_cmic_ipoll_handler(dev);
                }
            }
        }

        sal_spl(spl);

        if (udelay) {
            sal_usleep(udelay);
        } else {
            sal_thread_yield();
        }
    }
    sal_thread_exit(0);
}

/*
 * Function:
 *      soc_ipoll_connect
 * Description:
 *      Install interrupt handler in polled IRQ mode
 * Parameters:
 *      dev - device number
 *      handler - interrupt handler
 *      data - interrupt handler data
 * Return Value:
 *      BCM_E_NONE
 *      BCM_E_XXX - an error occurred
 * Notes:
 *      The first handler installed be cause the polling
 *      thread to be created.
 */
int
soc_ipoll_connect(int dev, ipoll_handler_t handler, void *data)
{
    int spl;
    int udelay, prio;
    int start_thread = 0;

    if (dev >= SOC_MAX_NUM_DEVICES) {
        return SOC_E_PARAM;
    }

    spl = sal_splhi();

    if (_ictrl[dev].handler == NULL) {
        if (_ihandlers++ == 0) {
            /* Create thread if first handler */
            start_thread = 1;
        }
    }
    _ictrl[dev].handler = handler;
    _ictrl[dev].data = data;
    _ictrl[dev].paused = 0;

    sal_spl(spl);

    if (start_thread) {
        udelay = soc_property_get(dev, spn_POLLED_IRQ_DELAY, IPOLL_THREAD_DELAY);
        prio = soc_property_get(dev, spn_POLLED_IRQ_PRIORITY, IPOLL_THREAD_PRIO);
        sal_thread_create("bcmPOLL",
                          SAL_THREAD_STKSZ, prio,
                          soc_ipoll_thread, INT_TO_PTR(udelay));
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_ipoll_disconnect
 * Description:
 *      Uninstall interrupt handler from polled IRQ mode
 * Parameters:
 *      dev - device number
 * Return Value:
 *      BCM_E_NONE
 *      BCM_E_XXX - an error occurred
 * Notes:
 *      When the last handler is uninstalled the polling
 *      thread will terminate.
 */
int
soc_ipoll_disconnect(int dev)
{
    int spl;

    if (dev >= SOC_MAX_NUM_DEVICES) {
        return SOC_E_PARAM;
    }

    spl = sal_splhi();

    if (_ictrl[dev].handler != NULL) {
        _ictrl[dev].handler = NULL;
        _ictrl[dev].data = NULL;
        _ihandlers--;
    }

    sal_spl(spl);

    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_ipoll_pause
 * Description:
 *      Temporarily suspend IRQ polling
 * Parameters:
 *      dev - device number
 * Return Value:
 *      BCM_E_NONE
 *      BCM_E_XXX - an error occurred
 */
int
soc_ipoll_pause(int dev)
{
    int spl;

    if (dev >= SOC_MAX_NUM_DEVICES) {
        return SOC_E_PARAM;
    }

    spl = sal_splhi();

    _ictrl[dev].paused++;

    sal_spl(spl);

    return SOC_E_NONE;
}

/*
 * Function:
 *      soc_ipoll_continue
 * Description:
 *      Resume suspended IRQ polling
 * Parameters:
 *      dev - device number
 * Return Value:
 *      BCM_E_NONE
 *      BCM_E_XXX - an error occurred
 */
int
soc_ipoll_continue(int dev)
{
    int spl;

    if (dev >= SOC_MAX_NUM_DEVICES) {
        return SOC_E_PARAM;
    }

    spl = sal_splhi();

    _ictrl[dev].paused--;

    sal_spl(spl);

    return SOC_E_NONE;
}
