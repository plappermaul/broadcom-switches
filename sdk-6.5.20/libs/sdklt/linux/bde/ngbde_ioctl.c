/*! \file ngbde_ioctl.c
 *
 * NGBDE IOCTL interface.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <lkm/ngbde_ioctl.h>

#include <ngbde.h>

long
ngbde_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct ngbde_ioc_cmd_s ioc;
    struct ngbde_dev_s *swdev;
    struct ngbde_irq_reg_s ireg;
    struct ngbde_intr_ack_reg_s ackreg;
    phys_addr_t addr, size;
    unsigned int num_swdev;
    unsigned int rsrc_type, rsrc_idx;
    unsigned int irq_num, intr_cmd;
    uint32_t mreg, mval;

    if (copy_from_user(&ioc, (void *)arg, sizeof(ioc))) {
        return -EFAULT;
    }

    ioc.rc = NGBDE_IOC_SUCCESS;

    switch (cmd) {
    case NGBDE_IOC_MOD_INFO:
        ioc.op.mod_info.version = NGBDE_IOC_VERSION;
        break;
    case NGBDE_IOC_PROBE_INFO:
        ngbde_swdev_get_all(NULL, &num_swdev);
        ioc.op.probe_info.num_swdev = num_swdev;
        break;
    case NGBDE_IOC_DEV_INFO:
        swdev = ngbde_swdev_get(ioc.devid);
        if (!swdev) {
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        ioc.op.dev_info.vendor_id = swdev->vendor_id;
        ioc.op.dev_info.device_id = swdev->device_id;
        ioc.op.dev_info.revision = swdev->revision;
        ioc.op.dev_info.model = swdev->model;
        if (swdev->use_msi) {
            ioc.op.dev_info.flags |= NGBDE_DEV_F_MSI;
        }
        break;
    case NGBDE_IOC_PHYS_ADDR:
        swdev = ngbde_swdev_get(ioc.devid);
        if (!swdev) {
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        rsrc_type = ioc.op.rsrc_id.type;
        rsrc_idx = ioc.op.rsrc_id.inst;
        switch (rsrc_type) {
        case NGBDE_IO_RSRC_DEV_IO:
            if (rsrc_idx >= NGBDE_NUM_IOWIN_MAX) {
                printk(KERN_WARNING
                       "ngbde: invalid resource index (%d)\n",
                       rsrc_idx);
                ioc.rc = NGBDE_IOC_FAIL;
                break;
            }
            ioc.op.phys_addr.addr = swdev->iowin[rsrc_idx].addr;
            ioc.op.phys_addr.size = swdev->iowin[rsrc_idx].size;
            break;
        case NGBDE_IO_RSRC_DMA_MEM:
            if (rsrc_idx >= NGBDE_NUM_DMAPOOL_MAX) {
                printk(KERN_WARNING
                       "ngbde: invalid resource index (%d)\n",
                       rsrc_idx);
                ioc.rc = NGBDE_IOC_FAIL;
                break;
            }
            ioc.op.phys_addr.addr = swdev->dmapool[rsrc_idx].dmamem.paddr;
            ioc.op.phys_addr.size = swdev->dmapool[rsrc_idx].dmactrl.size;
            break;
        case NGBDE_IO_RSRC_DMA_BUS:
            if (rsrc_idx >= NGBDE_NUM_DMAPOOL_MAX) {
                printk(KERN_WARNING
                       "ngbde: invalid resource index (%d)\n",
                       rsrc_idx);
                ioc.rc = NGBDE_IOC_FAIL;
                break;
            }
            ioc.op.phys_addr.addr = swdev->dmapool[rsrc_idx].dmamem.baddr;
            ioc.op.phys_addr.size = swdev->dmapool[rsrc_idx].dmactrl.size;
            break;
        default:
            printk(KERN_WARNING
                   "ngbde: unknown resource type (%d)\n",
                   rsrc_type);
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        break;
    case NGBDE_IOC_INTR_CTRL:
        irq_num = ioc.op.intr_ctrl.irq_num;
        intr_cmd = ioc.op.intr_ctrl.cmd;
        switch (intr_cmd) {
        case NGBDE_ICTL_INTR_CONN:
            if (ngbde_intr_connect(ioc.devid, irq_num) < 0) {
                ioc.rc = NGBDE_IOC_FAIL;
            }
            break;
        case NGBDE_ICTL_INTR_DISC:
            if (ngbde_intr_disconnect(ioc.devid, irq_num) < 0) {
                ioc.rc = NGBDE_IOC_FAIL;
            }
            break;
        case NGBDE_ICTL_INTR_WAIT:
            if (ngbde_intr_wait(ioc.devid, irq_num) < 0) {
                ioc.rc = NGBDE_IOC_FAIL;
            }
            break;
        case NGBDE_ICTL_INTR_STOP:
            if (ngbde_intr_stop(ioc.devid, irq_num) < 0) {
                ioc.rc = NGBDE_IOC_FAIL;
            }
            break;
        case NGBDE_ICTL_REGS_CLR:
            if (ngbde_intr_regs_clr(ioc.devid, irq_num) < 0) {
                ioc.rc = NGBDE_IOC_FAIL;
            }
            break;
        default:
            printk(KERN_WARNING
                   "%s: unknown interrupt control command (%d)\n",
                   MOD_NAME, intr_cmd);
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        break;
    case NGBDE_IOC_IRQ_REG_ADD:
        irq_num = ioc.op.irq_reg_add.irq_num;
        ireg.status_reg = ioc.op.irq_reg_add.status_reg;
        ireg.mask_reg = ioc.op.irq_reg_add.mask_reg;
        ireg.kmask = ioc.op.irq_reg_add.kmask;
        if (ngbde_intr_reg_add(ioc.devid, irq_num, &ireg) < 0) {
            printk(KERN_WARNING
                   "%s: Unable to add interrupt register\n",
                   MOD_NAME);
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    case NGBDE_IOC_INTR_ACK_REG_ADD:
        irq_num = ioc.op.intr_ack_reg_add.irq_num;
        ackreg.ack_reg = ioc.op.intr_ack_reg_add.ack_reg;
        ackreg.ack_val = ioc.op.intr_ack_reg_add.ack_val;
        ackreg.flags = ioc.op.intr_ack_reg_add.flags;
        if (ngbde_intr_ack_reg_add(ioc.devid, irq_num, &ackreg) < 0) {
            printk(KERN_WARNING
                   "%s: Unable to add interrupt ack register\n",
                   MOD_NAME);
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    case NGBDE_IOC_IRQ_MASK_WR:
        irq_num = ioc.op.irq_mask_wr.irq_num;
        mreg = ioc.op.irq_mask_wr.offs;
        mval = ioc.op.irq_mask_wr.val;
        if (ngbde_intr_mask_write(ioc.devid, irq_num, 0, mreg, mval) < 0) {
            printk(KERN_WARNING
                   "%s: Unable to write shared register\n",
                   MOD_NAME);
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    case NGBDE_IOC_PIO_WIN_MAP:
        swdev = ngbde_swdev_get(ioc.devid);
        if (!swdev) {
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        addr = ioc.op.pio_win.addr;
        size = ioc.op.pio_win.size;
        if (ngbde_pio_map(swdev, addr, size) == NULL) {
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    case NGBDE_IOC_IIO_WIN_MAP:
        swdev = ngbde_swdev_get(ioc.devid);
        if (!swdev) {
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        addr = ioc.op.pio_win.addr;
        size = ioc.op.pio_win.size;
        if (ngbde_iio_map(swdev, addr, size) == NULL) {
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    case NGBDE_IOC_PAXB_WIN_MAP:
        swdev = ngbde_swdev_get(ioc.devid);
        if (!swdev) {
            ioc.rc = NGBDE_IOC_FAIL;
            break;
        }
        addr = ioc.op.pio_win.addr;
        size = ioc.op.pio_win.size;
        if (ngbde_paxb_map(swdev, addr, size) == NULL) {
            ioc.rc = NGBDE_IOC_FAIL;
        }
        break;
    default:
        printk(KERN_ERR "ngbde: invalid ioctl (%08x)\n", cmd);
        ioc.rc = NGBDE_IOC_FAIL;
        break;
    }

    if (copy_to_user((void *)arg, &ioc, sizeof(ioc))) {
        return -EFAULT;
    }

    return 0;
}
