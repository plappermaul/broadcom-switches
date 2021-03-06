/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * Tool: INTERNAL/drd/instpkgs.pl
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*
 * This file contains the complete list of supported devices.
 * No other device lists should be used anywhere in the SDK.
 */

#ifndef BCMDRD_DEVIDS_H
#define BCMDRD_DEVIDS_H

#include <bcmdrd_config.h>

/*
 * All Supported Devices and Revisions
 */

#define BROADCOM_VENDOR_ID                0x14e4
#define BROADCOM_PHYID_MSB                0x0143

/* BCM56780 */
#define BCM56780_VENDOR_ID                0x14e4
#define BCM56780_DEVICE_ID                0xb780
#define BCM56780_REV_A0                     0x01

/* BCM56782 */
#define BCM56782_VENDOR_ID                0x14e4
#define BCM56782_DEVICE_ID                0xb782
#define BCM56782_REV_A0                     0x01

/* BCM56784 */
#define BCM56784_VENDOR_ID                0x14e4
#define BCM56784_DEVICE_ID                0xb784
#define BCM56784_REV_A0                     0x01

/* BCM56786 */
#define BCM56786_VENDOR_ID                0x14e4
#define BCM56786_DEVICE_ID                0xb786
#define BCM56786_REV_A0                     0x01

/* BCM56788 */
#define BCM56788_VENDOR_ID                0x14e4
#define BCM56788_DEVICE_ID                0xb788
#define BCM56788_REV_A0                     0x01

/* BCM56880 */
#define BCM56880_VENDOR_ID                0x14e4
#define BCM56880_DEVICE_ID                0xb880
#define BCM56880_REV_A0                     0x01
#define BCM56880_REV_B0                     0x11

/* BCM56881 */
#define BCM56881_VENDOR_ID                0x14e4
#define BCM56881_DEVICE_ID                0xb881
#define BCM56881_REV_A0                     0x01
#define BCM56881_REV_B0                     0x11

/* BCM56883 */
#define BCM56883_VENDOR_ID                0x14e4
#define BCM56883_DEVICE_ID                0xb883
#define BCM56883_REV_A0                     0x01
#define BCM56883_REV_B0                     0x11

/* BCM56990 */
#define BCM56990_VENDOR_ID                0x14e4
#define BCM56990_DEVICE_ID                0xb990
#define BCM56990_REV_A0                     0x01
#define BCM56990_REV_B0                     0x11

/* BCM56991 */
#define BCM56991_VENDOR_ID                0x14e4
#define BCM56991_DEVICE_ID                0xb991
#define BCM56991_REV_A0                     0x01
#define BCM56991_REV_B0                     0x11

/* BCM56996 */
#define BCM56996_VENDOR_ID                0x14e4
#define BCM56996_DEVICE_ID                0xb996
#define BCM56996_REV_A0                     0x01

/* BCM56997 */
#define BCM56997_VENDOR_ID                0x14e4
#define BCM56997_DEVICE_ID                0xb997
#define BCM56997_REV_A0                     0x01

/*
 * End of Supported Devices and Revisions
 */

#endif /* BCMDRD_DEVIDS_H */

#ifdef BCMDRD_DEVLIST_ENTRY
/*
 * BCMDRD_DEVLIST_ENTRY macros.
 *
 * Before including this file, define BCMDRD_DEVLIST_ENTRY
 * as a macro to operate on the following parameters:
 *
 *     #define BCMDRD_DEVLIST_ENTRY(_nm,_vn,_dv,_rv,_md,_pi,_bd,_bc,_fn,_cn,_pf,_pd,_r0,_r1)
 *
 *     _nm: Chip Name
 *     _vn: Chip Vendor ID
 *     _dv: Chip Device ID
 *     _rv: Chip Revision
 *     _md: Chip Model
 *     _pi: Probe Information
 *     _bd: SW Base Driver
 *     _bc: SW Base Configuration
 *     _fn: SW Full Name
 *     _cn: Code Name
 *     _pf: Product Family
 *     _pd: Product Description
 *     _r0: Reserved
 *     _r1: Reserved
 *
 * Note that this macro will be undefined at the end of this file.
 */

#if BCMDRD_CONFIG_INCLUDE_BCM56780_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
BCMDRD_DEVLIST_ENTRY(BCM56780, BCM56780_VENDOR_ID, BCM56780_DEVICE_ID, BCM56780_REV_A0, \
                     0, 0, \
                     bcm56780_a0, bcm56780_a0, bcm56780_a0, \
                     "Trident4-X9", "BCM56780", \
                     "8 Tb/s 160x50G-PAM4 Programmable Switch", 0, 0)
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56782_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56782, BCM56782_VENDOR_ID, BCM56782_DEVICE_ID, BCM56782_REV_A0, \
                     0, 0, \
                     bcm56780_a0, bcm56782_a0, bcm56782_a0, \
                     "Trident4-X9", "BCM56780", \
                     "8 Tb/s 160x50G-PAM4 Programmable Switch w/MACsec", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56784_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56784, BCM56784_VENDOR_ID, BCM56784_DEVICE_ID, BCM56784_REV_A0, \
                     0, 0, \
                     bcm56780_a0, bcm56784_a0, bcm56784_a0, \
                     "Trident4-X9", "BCM56780", \
                     "5.6 Tb/s 96x50G-PAM4/32x35G-NRZ Programmable Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56786_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56786, BCM56786_VENDOR_ID, BCM56786_DEVICE_ID, BCM56786_REV_A0, \
                     0, 0, \
                     bcm56780_a0, bcm56786_a0, bcm56786_a0, \
                     "Trident4-X9", "BCM56780", \
                     "5.6 Tb/s 96x50G-PAM4/32x35G-NRZ Programmable Switch w/MACsec", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56788_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56788, BCM56788_VENDOR_ID, BCM56788_DEVICE_ID, BCM56788_REV_A0, \
                     0, 0, \
                     bcm56780_a0, bcm56788_a0, bcm56788_a0, \
                     "Trident4-X9", "BCM56780", \
                     "8 Tb/s 160x50G-PAM4 Programmable Switch w/MACsec w/MTop", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56880_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
BCMDRD_DEVLIST_ENTRY(BCM56880, BCM56880_VENDOR_ID, BCM56880_DEVICE_ID, BCM56880_REV_A0, \
                     0, 0, \
                     bcm56880_a0, bcm56880_a0, bcm56880_a0, \
                     "Trident4", "BCM56880", \
                     "12.8 Tb/s Switch Fabric 128x100G/64x200G/32x400G Multilayer Switch", 0, 0)
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56880_B0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56880, BCM56880_VENDOR_ID, BCM56880_DEVICE_ID, BCM56880_REV_B0, \
                     0, 0, \
                     bcm56880_a0, bcm56880_a0, bcm56880_b0, \
                     "Trident4", "BCM56880", \
                     "12.8 Tb/s Switch Fabric 128x100G/64x200G/32x400G Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56881_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56881, BCM56881_VENDOR_ID, BCM56881_DEVICE_ID, BCM56881_REV_A0, \
                     0, 0, \
                     bcm56880_a0, bcm56881_a0, bcm56881_a0, \
                     "Trident4", "BCM56880", \
                     "12.8 Tb/s Switch Fabric 128x100G/64x200G/32x400G Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56881_B0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56881, BCM56881_VENDOR_ID, BCM56881_DEVICE_ID, BCM56881_REV_B0, \
                     0, 0, \
                     bcm56880_a0, bcm56881_a0, bcm56881_b0, \
                     "Trident4", "BCM56880", \
                     "12.8 Tb/s Switch Fabric 128x100G/64x200G/32x400G Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56883_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56883, BCM56883_VENDOR_ID, BCM56883_DEVICE_ID, BCM56883_REV_A0, \
                     0, 0, \
                     bcm56880_a0, bcm56883_a0, bcm56883_a0, \
                     "Trident4", "BCM56880", \
                     "8.0 Tb/s Switch Fabric 80x100G/40x200G/20x400G Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56883_B0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56883, BCM56883_VENDOR_ID, BCM56883_DEVICE_ID, BCM56883_REV_B0, \
                     0, 0, \
                     bcm56880_a0, bcm56883_a0, bcm56883_b0, \
                     "Trident4", "BCM56880", \
                     "8.0 Tb/s Switch Fabric 80x100G/40x200G/20x400G Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56990_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
BCMDRD_DEVLIST_ENTRY(BCM56990, BCM56990_VENDOR_ID, BCM56990_DEVICE_ID, BCM56990_REV_A0, \
                     0, 0, \
                     bcm56990_a0, bcm56990_a0, bcm56990_a0, \
                     "Tomahawk4", "BCM56990", \
                     "25.6 Tbps Multilayer Switch", 0, 0)
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56990_B0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
BCMDRD_DEVLIST_ENTRY(BCM56990, BCM56990_VENDOR_ID, BCM56990_DEVICE_ID, BCM56990_REV_B0, \
                     0, 0, \
                     bcm56990_b0, bcm56990_b0, bcm56990_b0, \
                     "Tomahawk4", "BCM56990", \
                     "25.6 Tbps Multilayer Switch", 0, 0)
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56991_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56991, BCM56991_VENDOR_ID, BCM56991_DEVICE_ID, BCM56991_REV_A0, \
                     0, 0, \
                     bcm56990_a0, bcm56991_a0, bcm56991_a0, \
                     "Tomahawk4", "BCM56990", \
                     "12.8 Tbps Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56991_B0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56991, BCM56991_VENDOR_ID, BCM56991_DEVICE_ID, BCM56991_REV_B0, \
                     0, 0, \
                     bcm56990_b0, bcm56991_b0, bcm56991_b0, \
                     "Tomahawk4", "BCM56990", \
                     "12.8 Tbps Multilayer Switch", 0, 0)
#endif
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56996_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
BCMDRD_DEVLIST_ENTRY(BCM56996, BCM56996_VENDOR_ID, BCM56996_DEVICE_ID, BCM56996_REV_A0, \
                     0, 0, \
                     bcm56996_a0, bcm56996_a0, bcm56996_a0, \
                     "Tomahawk4G", "BCM56996", \
                     "25.6 Tbps Multilayer Switch", 0, 0)
#endif

#if BCMDRD_CONFIG_INCLUDE_BCM56997_A0 == 1 || defined(BCMDRD_DEVLIST_OVERRIDE)
#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
BCMDRD_DEVLIST_ENTRY(BCM56997, BCM56997_VENDOR_ID, BCM56997_DEVICE_ID, BCM56997_REV_A0, \
                     0, 0, \
                     bcm56996_a0, bcm56997_a0, bcm56997_a0, \
                     "Tomahawk4G", "BCM56996", \
                     "12.8 Tbps Multilayer Switch", 0, 0)
#endif
#endif

/* End BCMDRD_DEVLIST_ENTRY Macros */

#ifdef BCMDRD_DEVLIST_INCLUDE_ALL
#undef BCMDRD_DEVLIST_INCLUDE_ALL
#endif
#ifdef BCMDRD_DEVLIST_OVERRIDE
#undef BCMDRD_DEVLIST_OVERRIDE
#endif
#undef BCMDRD_DEVLIST_ENTRY
#endif /* BCMDRD_DEVLIST_ENTRY */
