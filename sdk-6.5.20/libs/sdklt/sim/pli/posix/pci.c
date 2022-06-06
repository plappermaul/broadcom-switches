/*! \file pci.c
 *
 * PCI memory and configuration space routines.
 *
 * Under Solaris, these routines call the verinet model PLI
 * simulation.  All PCI operations are performed talking over a socket
 * to the model.
 *
 * NOTE: the base address used in this file is opaque. This means that
 * the PCI device must first have its base-address programmed by
 * writing the start offset to PCI configuration offset 0x10.  Writes
 * to the memory space afterwards requires the address to be based on
 * the base-address plus the offset to the desire register to be
 * accessed.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bsl/bsl.h>

#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>

#include <sim/pli/pci.h>

#include "verinet.h"

/* DMA offset for 64-bit support */
static unsigned long dma_offs[N_VERINET_DEVICES];

/*
 * Calculate true (64-bit) host address from 32-bit PLI address.
 *
 * This is done on a per-device basis under the assumption that each
 * device uses a separate DMA memory pool, i.e. the 64-bit offset
 * remains fixed for all DMA memory allocated on a given device.
 *
 * Note that we use the Verinet device number if a valid PCI device is
 * passed in, otherwise we use device zero. Currently the only usage
 * model is to pass in a NULL device (see dmac.c).
 */
#define DMA_ADDR(_addr, _dev)                                           \
    ((unsigned long)(_addr) + ((_dev) ? dma_offs[(_dev)->dev_no] : dma_offs[0]))

/*
 * NOTE:  Assume dev->dev_no is the verinet unit (for now always zero).
 */

/* In the DV model for the RTL, bit 12 selects Orion */
#define IDSEL 0x1000

#define VALID_DEV(_dev) \
    ((_dev)->bus_no == 0 &&                     \
     (_dev)->dev_no < pli_client_count() &&     \
     (_dev)->func_no == 0)

/* Track connection state */
static int pli_connected;

/*
 * Initialize the package if not already done
 */

static int pci_init_check(void)
{
    int client;
    static int attach_err[N_VERINET_DEVICES];

    if (! pli_connected) {
        /*
         * Initialize each PLI client.
         */

        for (client = 0; client < pli_client_count(); client++) {
            if (attach_err[client]) {
                /* Avoid duplicated error messages */
                continue;
            }
            if (pli_client_attach(client) < 0) {
                cli_out("pci_init_check: error attaching PLI client %d\n",
                        client);
                attach_err[client] = 1;
                return -1;
            }
        }

        pli_connected = 1;
    }

    return 0;
}

/*
 * Write a DWORD (32 bits) of data to PCI configuration space
 * at the specified offset.
 */

int pci_config_putw(pci_dev_t *dev, uint32_t addr, uint32_t data)
{
    if (pci_init_check() < 0) {
        return -1;
    }
    assert((addr & 3) == 0);

    if (VALID_DEV(dev)) {
        return pli_setreg(dev->dev_no, PCI_CONFIG, addr | IDSEL, data);
    }
    return -1;
}

/*
 * Read a DWORD (32 bits) of data from PCI configuration space
 * at the specified offset.
 */

uint32_t pci_config_getw(pci_dev_t *dev, uint32_t addr)
{
    if (pci_init_check() < 0) {
        return 0xffffffff;
    }
    assert((addr & 3) == 0);

    if (VALID_DEV(dev)) {
        return pli_getreg(dev->dev_no, PCI_CONFIG, addr | IDSEL);
    }
    return 0xffffffff;
}

/*
 * Write a DWORD (32 bits) of data to PCI memory space
 * at the specified offset. Return last value at address.
 */

int pci_memory_putw(pci_dev_t *dev, uint32_t addr, uint32_t data)
{
    if (pci_init_check() < 0) {
        return -1;
    }
    return pli_setreg(dev->dev_no, PCI_MEMORY, addr, data);
}

/*
 * Read a DWORD (32 bits) of data from PCI memory space.
 */

uint32_t pci_memory_getw(pci_dev_t *dev, uint32_t addr)
{
    if (pci_init_check() < 0) {
        return 0xffffffff;
    }
    return pli_getreg(dev->dev_no, PCI_MEMORY, addr);
}

/*
 * DMA memory routines.
 * The routines below operate on memory which the CMIC onboard the
 * Orion will read and write. Under the PLI simulator, these routines
 * read and write shared memory. On real hardware, these routines will
 * manipulate a DMA memory buffer pool.
 */

#define MAX_DMA_MEMORY  (64*1024)
static  uint32_t        kluge_dma_memory[MAX_DMA_MEMORY/sizeof(uint32_t)];

int pci_dma_putw(pci_dev_t *dev, unsigned long addr, uint32_t data)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    *(uint32_t *)DMA_ADDR(addr, dev) = data;

    return(0);
}

int pci_dma_puth(pci_dev_t *dev, unsigned long addr, uint16_t data)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    *(uint16_t *)DMA_ADDR(addr, dev) = data;

    return(0);
}

int pci_dma_putb(pci_dev_t *dev, unsigned long addr, uint8_t data)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    *(uint8_t *)DMA_ADDR(addr, dev) = data;

    return(0);
}

uint32_t pci_dma_getw(pci_dev_t *dev, unsigned long addr)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    return *(uint32_t *)DMA_ADDR(addr, dev);
}

uint16_t pci_dma_geth(pci_dev_t *dev, unsigned long addr)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    return *(uint16_t *)DMA_ADDR(addr, dev);
}

uint8_t pci_dma_getb(pci_dev_t *dev, unsigned long addr)
{
    if (addr < MAX_DMA_MEMORY) {
        addr = (unsigned long)kluge_dma_memory + addr;
    }

    return *(uint8_t *)DMA_ADDR(addr, dev);
}


/*
 * pci_dma_offset_set
 *
 *    The PLI protocol can only carry 32-bit addresses, so we need to
 *    compensate for that on 64-bit systems. Note that this assumes
 *    that all DMA memory is allocated from the same block of memory
 *    for a given device.
 */
int pci_dma_offset_set(int dev_no, unsigned long offs)
{
    if ((unsigned)dev_no < COUNTOF(dma_offs)) {
        dma_offs[dev_no] = offs;
    }
    return 0;
}

/*
 * pci_int_connect
 *
 *    For PLI simulation, the isr has already been attached.
 */
int pci_int_connect(int int_line,
                    pci_isr_t isr,
                    void *isr_data)
{
    if (pci_init_check() < 0) {
        return -1;
    }
    return pli_register_isr(int_line, isr, isr_data);
}

/*
 * pci_reset
 *
 *    Detach all PLI clients.
 */
int pci_reset(void)
{
    int client;

    if (pli_connected) {
        /*
         * Disconnect all PLI clients.
         */

        for (client = 0; client < pli_client_count(); client++) {
            if (pli_client_detach(client) < 0) {
                cli_out("pci_reset: error detaching PLI client %d\n",
                        client);
                return -1;
            }
        }

        pli_connected = 0;
    }

    /* Reset state */
    memset(dma_offs, 0, sizeof(dma_offs));

    return 0;
}
