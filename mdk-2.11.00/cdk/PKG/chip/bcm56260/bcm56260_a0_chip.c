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

#include <cdk/chip/bcm56260_a0_defs.h>

/* Block types */
const char *bcm56260_a0_blktype_names[] = {
    "avs",
    "ci",
    "cmic",
    "epipe",
    "iecell",
    "ipipe",
    "iproc",
    "lls",
    "mmu",
    "mxqport",
    "oamp",
    "rxlp",
    "ser",
    "top",
    "txlp",
    "xlport"
};

/* Block structures */
cdk_xgsd_block_t bcm56260_a0_blocks[] = 
{
    { BCM56260_A0_BLKTYPE_MXQPORT,       6,   0,   CDK_PBMP_1(0x0000001e) },
    { BCM56260_A0_BLKTYPE_MXQPORT,       7,   0,   CDK_PBMP_1(0x000001e0) },
    { BCM56260_A0_BLKTYPE_MXQPORT,       8,   0,   CDK_PBMP_1(0x00001e00) },
    { BCM56260_A0_BLKTYPE_MXQPORT,       9,   0,   CDK_PBMP_1(0x0001e000) },
    { BCM56260_A0_BLKTYPE_MXQPORT,      10,   0,   CDK_PBMP_1(0x001e0000) },
    { BCM56260_A0_BLKTYPE_MXQPORT,      11,   0,   CDK_PBMP_1(0x01e00000) },
    { BCM56260_A0_BLKTYPE_XLPORT,       12,   0,   CDK_PBMP_1(0x1e000000) },
    { BCM56260_A0_BLKTYPE_IPIPE,         1,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_EPIPE,         2,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_MMU,           3,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_TOP,          13,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_LLS,          14,   0,   CDK_PBMP_1(0x00000001) },
    { BCM56260_A0_BLKTYPE_CI,           15,   0,   CDK_PBMP_1(0x00000001) },
    { BCM56260_A0_BLKTYPE_CI,           16,   0,   CDK_PBMP_1(0x00000001) },
    { BCM56260_A0_BLKTYPE_RXLP,         17,   0,   CDK_PBMP_1(0x0000000f) },
    { BCM56260_A0_BLKTYPE_TXLP,         18,   0,   CDK_PBMP_1(0x0000000f) },
    { BCM56260_A0_BLKTYPE_SER,          19,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_IPROC,        20,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_IECELL,       21,   0,   CDK_PBMP_1(0x0000001e) },
    { BCM56260_A0_BLKTYPE_AVS,          22,   0,   CDK_PBMP_1(0x3fffffff) },
    { BCM56260_A0_BLKTYPE_OAMP,         23,   0,   CDK_PBMP_1(0x3fffffff) }
};

/* Symbol table */
#if CDK_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
#if CDK_CONFIG_CHIP_SYMBOLS_USE_DSYMS == 1
extern cdk_symbols_t bcm56260_a0_dsymbols;
#else
extern cdk_symbols_t bcm56260_a0_symbols;
#endif
#endif

/* Physical port numbers */
#if CDK_CONFIG_INCLUDE_PORT_MAP == 1
static cdk_port_map_port_t _ports[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 21, 22, 23, 24, 25, 26, 27, 28
};
#endif

/* Index ranges for this chip */
static cdk_xgsd_numel_range_t _numel_ranges[] = {
    {  0,  0, CDK_PBMP_1(0x3ffffffe)                         }, /*  0 */
    {  0,  0, CDK_PBMP_1(0x3fffffff)                         }, /*  1 */
    {  0,  0, CDK_PBMP_1(0x3e000001)                         }, /*  2 */
    {  0,  0, CDK_PBMP_1(0x1fffffff)                         }, /*  3 */
    {  0, 11, CDK_PBMP_1(0x3fffffff)                         }  /*  4 */
};

/* Register array encodings for this chip */
static cdk_xgsd_numel_encoding_t _numel_encodings[] = {
    { { 6 } },
    { {  0, -1 } },
    { {  1, -1 } },
    { {  2, -1 } },
    { {  3, -1 } },
    { {  4, -1 } }
};

/* Variable register array info */
cdk_xgsd_numel_info_t bcm56260_a0_numel_info = {
    _numel_ranges,
    _numel_encodings
};

/* Chip information structure */
static cdk_xgsd_chip_info_t bcm56260_a0_chip_info = {

    /* CMIC block */
    BCM56260_A0_CMIC_BLOCK,

    /* CMC instance */
    0,

    /* Other (non-CMIC) block types */
    16,
    bcm56260_a0_blktype_names,

    /* Address calculation */
    NULL,

    /* Other (non-CMIC) blocks */
    21,
    bcm56260_a0_blocks,

    /* Valid ports for this chip */
    CDK_PBMP_1(0x1fe001ff),

    /* Chip flags */
    CDK_XGSD_CHIP_FLAG_IPROC |
    BCM56260_A0_CHIP_FLAG_ACCESS |
    BCM56260_A0_CHIP_FLAG_FREQ118 |
    0,

#if CDK_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
#if CDK_CONFIG_CHIP_SYMBOLS_USE_DSYMS == 1
    /* Use regenerated symbol tables from the DSYM program */
    &bcm56260_a0_dsymbols,
#else
    /* Use the static per-chip symbol tables */
    &bcm56260_a0_symbols,
#endif
#endif

    /* Port map */
#if CDK_CONFIG_INCLUDE_PORT_MAP == 1
    sizeof(_ports)/sizeof(_ports[0]),
    _ports,
#endif

    /* Variable register array info */
    &bcm56260_a0_numel_info,

    /* Configuration dependent memory max index */
    NULL,

    /* Pipe info */
    NULL,

};

/* Declare function first to prevent compiler warnings */
extern int
bcm56260_a0_setup(cdk_dev_t *dev);

int
bcm56260_a0_setup(cdk_dev_t *dev)
{
    dev->chip_info = &bcm56260_a0_chip_info;

    return cdk_xgsd_setup(dev);
}

