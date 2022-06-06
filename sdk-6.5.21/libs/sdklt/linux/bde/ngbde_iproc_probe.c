/*! \file ngbde_iproc_probe.c
 *
 * BDE probe for IPROC internal bus devices.
 *
 * Validate CMICD existence on the platform. If Linux device tree matched,
 * probe function of platform driver is called and the switch device read from
 * CMICD register is added to the device list.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <ngbde.h>
#include <linux/platform_device.h>
#include <linux/of.h>

/*! \cond */
static int iproc_debug = 0;
module_param(iproc_debug, int, 0);
MODULE_PARM_DESC(iproc_debug,
"IPROC debug output enable (default 0).");
/*! \endcond */

/*!
 * \brief Probe devices on the IPROC internal bus.
 *
 * \param [in] pldev Platform device.
 *
 * \retval 0 No errors
 * \retval -1 Something went wrong.
 */
static int
iproc_cmicd_probe(struct platform_device *pldev)
{
    int rv;
    uint32_t size;
    void *base_address;
    uint32_t dev_rev_id;
    struct ngbde_dev_s ngbde_dev, *nd = &ngbde_dev;
    struct resource *memres, *irqres;

    memres = platform_get_resource(pldev, IORESOURCE_MEM, 0);
    if (memres == NULL) {
        printk("Unable to retrieve iProc CMIC memory resource.");
        return -1;
    }
    size = memres->end - memres->start + 1;

    if (iproc_debug) {
        printk("CMIC info : Memory start=%p, end=%p\n",
               (void *)memres->start, (void *)memres->end);
    }

    base_address = ioremap_nocache(memres->start, size);
    if (!base_address) {
        printk(KERN_WARNING "Error mapping iProc CMIC registers");
        return -1;
    }

    memset(nd, 0, sizeof(*nd));
    nd->pci_dev = NULL; /* No PCI bus */
    nd->dma_dev = &pldev->dev;

    /* Read switch device ID from CMIC */
    dev_rev_id = *((uint32_t*)(base_address + 0x10224));
    nd->vendor_id = 0x14e4;
    nd->device_id = dev_rev_id & 0xffff;
    nd->revision = (dev_rev_id >> 16) & 0xff;

    irqres = platform_get_resource(pldev, IORESOURCE_IRQ, 0);
    if (irqres == NULL) {
        printk(KERN_WARNING "Unable to retrieve iProc CMIC IRQ resource.");
        return -1;
    }
    nd->irq_line = irqres->start;
    if (iproc_debug) {
        printk("CMIC info : IRQ line=%p\n", (void *)irqres->start);
    }

    nd->iowin[0].addr = memres->start;
    nd->iowin[0].size = size;

    if (base_address) {
        iounmap(base_address);
    }
    rv = ngbde_swdev_add(nd);

    return rv;
}

/*!
 * \brief Remove the platform device.
 *
 * \param [in] pldev Platform device.
 *
 * \retval 0 No errors
 */
static int
iproc_cmicd_remove(struct platform_device *pldev)
{
    return 0;
}

/*! Matching compatible property with device tree. */
static const struct of_device_id iproc_cmicd_of_match[] = {
    { .compatible = "brcm,iproc-cmicd" },
    {},
};
MODULE_DEVICE_TABLE(of, iproc_cmicd_of_match);

static char iproc_cmicd_string[] = "bcmiproc-cmicd";

/*! Platform driver definition. */
static struct platform_driver iproc_cmicd_driver =
{
    .probe = iproc_cmicd_probe,
    .remove = iproc_cmicd_remove,
    .driver =
    {
        .name = iproc_cmicd_string,
        .owner = THIS_MODULE,
        .of_match_table = iproc_cmicd_of_match,
    },
};

/*!
 * \brief Probe for Broadcom switch devices on IPROC internal bus.
 *
 * \return 0 if no errors, otherwise -1.
 */
int
ngbde_iproc_probe(void)
{
    platform_driver_register(&iproc_cmicd_driver);

    return 0;
}

/*!
 * \brief Clean up resources for Broadcom switch devices on IPROC internal bus.
 *
 * \return 0 if no errors, otherwise -1.
 */
int
ngbde_iproc_cleanup(void)
{
    platform_driver_unregister(&iproc_cmicd_driver);

    return 0;
}
