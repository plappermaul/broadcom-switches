/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <cdk/chip/bcm56680_b0_defs.h>

/* Block types */
extern const char *bcm56680_b0_blktype_names[];

/* Block structures */
extern cdk_xgs_block_t bcm56680_b0_blocks[];

/* Symbol table */
#if CDK_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
#if CDK_CONFIG_CHIP_SYMBOLS_USE_DSYMS == 1
extern cdk_symbols_t bcm56680_b0_dsymbols;
#else
extern cdk_symbols_t bcm56680_b0_symbols;
#endif
#endif

/* Physical port numbers */
#if CDK_CONFIG_INCLUDE_PORT_MAP == 1
static cdk_port_map_port_t _ports[] = {
    0, 2, 27, 28, 29, 30, 31
};
#endif

/* Chip-specific memory sizes */
static uint32_t
bcm56680_b0_mem_maxidx(uint32_t addr, uint32_t maxidx)
{
    switch (addr) {
    case FP_COUNTER_TABLEm:
    case FP_GLOBAL_MASK_TCAMm:
    case FP_METER_TABLEm:
    case FP_POLICY_TABLEm:
    case FP_TCAMm:
        return 2047; /* 0x7ff */
    case EFP_COUNTER_TABLEm:
    case EFP_METER_TABLEm:
    case EFP_POLICY_TABLEm:
    case EFP_TCAMm:
    case VFP_POLICY_TABLEm:
    case VFP_TCAMm:
        return 255; /* 0xff */
    }
    return maxidx;
}

/* Variable register array info */
extern cdk_xgs_numel_info_t bcm56680_b0_numel_info;

/* Chip information structure */
static cdk_xgs_chip_info_t bcm56686_b0_chip_info = {

    /* CMIC block */
    BCM56680_B0_CMIC_BLOCK,

    /* Other (non-CMIC) block types */
    7,
    bcm56680_b0_blktype_names,

    /* Address calculation */
    NULL,

    /* Other (non-CMIC) blocks */
    12,
    bcm56680_b0_blocks,

    /* Valid ports for this chip */
    CDK_PBMP_1(0xf8000005),

    /* Chip flags */
    CDK_XGS_CHIP_FLAG_CLAUSE45 |
    CDK_XGS_CHIP_FLAG_SCHAN_EXT |
    BCM56680_B0_CHIP_FLAG_XG_16G |
    BCM56680_B0_CHIP_FLAG_CLK_125 |
    0,

#if CDK_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
#if CDK_CONFIG_CHIP_SYMBOLS_USE_DSYMS == 1
    /* Use regenerated symbol tables from the DSYM program */
    &bcm56680_b0_dsymbols,
#else
    /* Use the static per-chip symbol tables */
    &bcm56680_b0_symbols,
#endif
#endif

    /* Port map */
#if CDK_CONFIG_INCLUDE_PORT_MAP == 1
    sizeof(_ports)/sizeof(_ports[0]),
    _ports,
#endif

    /* Variable register array info */
    &bcm56680_b0_numel_info,

    /* Configuration dependent memory max index */
    &bcm56680_b0_mem_maxidx,

};

/* Declare function first to prevent compiler warnings */
extern int
bcm56686_b0_setup(cdk_dev_t *dev);

int
bcm56686_b0_setup(cdk_dev_t *dev)
{
    dev->chip_info = &bcm56686_b0_chip_info;

    return cdk_xgs_setup(dev);
}

