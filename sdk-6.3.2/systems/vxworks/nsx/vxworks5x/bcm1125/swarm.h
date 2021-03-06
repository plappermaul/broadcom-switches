/* bcm1250_int.h - Broadcom BCM1250 swarm board I/O, CS, & GPIO constants & macros */

/* Copyright 2002 Wind River Systems, Inc. */

/*********************************************************************
*
*  Copyright 2000,2001
*  Broadcom Corporation. All rights reserved.
*
*  This software is furnished under license to Wind River Systems, Inc.
*  and may be used only in accordance with the terms and conditions
*  of this license.  No title or ownership is transferred hereby.
********************************************************************* */

/* $Id: swarm.h,v 1.5 2012/03/02 15:28:51 yaronm Exp $
 * This file has been developed or significantly modified by the
 * MIPS Center of Excellence Dedicated Engineering Staff.
 * This notice is as per the MIPS Center of Excellence Master Partner
 * Agreement, do not remove this notice without checking first with
 * WR/Platforms MIPS Center of Excellence engineering management.
 */

/*
modification history
--------------------
01a,15nov01,agf  written.
*/

#ifndef __INCswarmh
#define __INCswarmh

/*
 * I/O Address assignments for the CSWARM board
 *
 * Summary of address map:
 *
 * Address         Size   CSel    Description
 * --------------- ----   ------  --------------------------------
 * 0x1FC00000      2MB     CS0    Boot ROM
 * 0x1F800000      2MB     CS1    Alternate boot ROM
 *                         CS2    Unused
 * 0x100A0000	   64KB    CS3    LED display
 * 0x100B0000      64KB    CS4    IDE Disk
 *                         CS5    Unused
 * 0x11000000      64MB    CS6    PCMCIA
 *                         CS7    Unused
 *
 * GPIO assignments
 *
 * GPIO#    Direction   Description
 * -------  ---------   ------------------------------------------
 * GPIO0    Output      Debug LED
 * GPIO1    Output      Sturgeon NMI
 * GPIO2    Input       PHY Interrupt               (interrupt)
 * GPIO3    Input       Nonmaskable Interrupt       (interrupt)
 * GPIO4    Input       IDE Disk Interrupt          (interrupt)
 * GPIO5    Input       Temperature Sensor Alert    (interrupt)
 * GPIO6    N/A         PCMCIA interface
 * GPIO7    N/A         PCMCIA interface
 * GPIO8    N/A         PCMCIA interface
 * GPIO9    N/A         PCMCIA interface
 * GPIO10   N/A         PCMCIA interface
 * GPIO11   N/A         PCMCIA interface
 * GPIO12   N/A         PCMCIA interface
 * GPIO13   N/A         PCMCIA interface
 * GPIO14   N/A         PCMCIA interface
 * GPIO15   N/A         PCMCIA interface
 */


#define MB (1024*1024)
#define K64 65536
#define NUM64K(x) (((x)+(K64-1))/K64)


/*  GPIO pins */

#define GPIO_DEBUG_LED		0
#define GPIO_STURGEON_NMI	1
#define GPIO_PHY_INTERRUPT	2
#define GPIO_NONMASKABLE_INT	3
#define GPIO_IDE_INTERRUPT	4
#define GPIO_TEMP_SENSOR_INT	5

#define M_GPIO_DEBUG_LED	_SB_MAKEMASK1(GPIO_DEBUG_LED)
#define M_GPIO_STURGEON_NMI	_SB_MAKEMASK1(GPIO_STURGEON_NMI)

#define GPIO_OUTPUT_MASK (_SB_MAKEMASK1(GPIO_DEBUG_LED) | \
                          _SB_MAKEMASK1(GPIO_STURGEON_NMI) )

#define GPIO_INTERRUPT_MASK ((V_GPIO_INTR_TYPEX(GPIO_PHY_INTERRUPT,K_GPIO_INTR_LEVEL)) | \
                             (V_GPIO_INTR_TYPEX(GPIO_IDE_INTERRUPT,K_GPIO_INTR_LEVEL)))


/*  Generic Bus */

/*
 * Boot ROM:  non-multiplexed, byte width, no parity, no ack
 * XXX: These are the (very slow) default parameters.   This can be sped up!
 */
#define BOOTROM_CS		0
#define BOOTROM_PHYS		0x1FC00000	/* address of boot ROM (CS0) */
#define BOOTROM_SIZE		NUM64K(2*MB)	/* size of boot ROM */
#define BOOTROM_TIMING0		V_IO_ALE_WIDTH(4) | \
                                V_IO_ALE_TO_CS(2) | \
                                V_IO_CS_WIDTH(24) | \
                                V_IO_RDY_SMPLE(1)
#define BOOTROM_TIMING1		V_IO_ALE_TO_WRITE(7) | \
                                V_IO_WRITE_WIDTH(7) | \
                                V_IO_IDLE_CYCLE(6) | \
                                V_IO_CS_TO_OE(0) | \
                                V_IO_OE_TO_CS(0)
#define BOOTROM_CONFIG		V_IO_WIDTH_SEL(K_IO_WIDTH_SEL_1) | M_IO_NONMUX

/*
 * Alternate Boot ROM:  non-multiplexed, byte width, no parity, no ack
 * XXX: These are the (very slow) default parameters.   This can be sped up!
 */
#define ALT_BOOTROM_CS		1
#define ALT_BOOTROM_PHYS	0x1F800000	/* address of alternate boot ROM (CS1) */
#define ALT_BOOTROM_SIZE	NUM64K(2*MB)	/* size of alternate boot ROM */
#define ALT_BOOTROM_TIMING0	V_IO_ALE_WIDTH(4) | \
                                V_IO_ALE_TO_CS(2) | \
                                V_IO_CS_WIDTH(24) | \
                                V_IO_RDY_SMPLE(1)
#define ALT_BOOTROM_TIMING1	V_IO_ALE_TO_WRITE(7) | \
                                V_IO_WRITE_WIDTH(7) | \
                                V_IO_IDLE_CYCLE(6) | \
                                V_IO_CS_TO_OE(0) | \
                                V_IO_OE_TO_CS(0)
#define ALT_BOOTROM_CONFIG	V_IO_WIDTH_SEL(K_IO_WIDTH_SEL_1) | M_IO_NONMUX

/*
 * LEDs:  non-multiplexed, byte width, no parity, no ack
 */
#define LEDS_CS			3
#define LEDS_PHYS		0x100A0000
#define LEDS_SIZE		NUM64K(4)
#define LEDS_TIMING0		V_IO_ALE_WIDTH(4) | \
                                V_IO_ALE_TO_CS(2) | \
                                V_IO_CS_WIDTH(13) | \
                                V_IO_RDY_SMPLE(1)
#define LEDS_TIMING1		V_IO_ALE_TO_WRITE(2) | \
                                V_IO_WRITE_WIDTH(8) | \
                                V_IO_IDLE_CYCLE(6) | \
                                V_IO_CS_TO_OE(0) | \
                                V_IO_OE_TO_CS(0)
#define LEDS_CONFIG		V_IO_WIDTH_SEL(K_IO_WIDTH_SEL_1) | M_IO_NONMUX
                              

/*
 * IDE: non-multiplexed, word(16) width, no parity, ack mode
 * See BCM12500 Application Note: "BCM12500 Generic Bus Interface
 * to ATA/ATAPI PIO Mode 3 (IDE) Hard Disk"
 */
#define IDE_CS			4
#define IDE_PHYS		0x100B0000
#define IDE_SIZE		NUM64K(256)
#define IDE_TIMING0		V_IO_ALE_WIDTH(3) | \
                                V_IO_ALE_TO_CS(1) | \
                                V_IO_CS_WIDTH(8) | \
                                V_IO_RDY_SMPLE(2)
#define IDE_TIMING1		V_IO_ALE_TO_WRITE(4) | \
                                V_IO_WRITE_WIDTH(0xA) | \
                                V_IO_IDLE_CYCLE(1) | \
                                V_IO_CS_TO_OE(3) | \
                                V_IO_OE_TO_CS(2)
#define IDE_CONFIG		V_IO_WIDTH_SEL(K_IO_WIDTH_SEL_2) | \
                                M_IO_RDY_ACTIVE | \
                                M_IO_ENA_RDY
                                


/*
 * PCMCIA: this information was derived from chapter 12, table 12-5
 */
#define PCMCIA_CS		6
#define PCMCIA_PHYS		0x11000000
#define PCMCIA_SIZE		NUM64K(64*MB)
#define PCMCIA_TIMING0		V_IO_ALE_WIDTH(3) | \
                                V_IO_ALE_TO_CS(1) | \
                                V_IO_CS_WIDTH(17) | \
                                V_IO_RDY_SMPLE(1)
#define PCMCIA_TIMING1		V_IO_ALE_TO_WRITE(8) | \
                                V_IO_WRITE_WIDTH(8) | \
                                V_IO_IDLE_CYCLE(2) | \
                                V_IO_CS_TO_OE(0) | \
                                V_IO_OE_TO_CS(0)
#define PCMCIA_CONFIG		V_IO_WIDTH_SEL(K_IO_WIDTH_SEL_2)


/*  SMBus Devices */

#define TEMPSENSOR_SMBUS_CHAN	0
#define TEMPSENSOR_SMBUS_DEV	0x2A
#define DRAM_SMBUS_CHAN		0
#define DRAM_SMBUS_DEV		0x54
#define BIGEEPROM_SMBUS_CHAN	0
#define BIGEEPROM_SMBUS_DEV	0x50
#define X1240_SMBUS_CHAN	1
#define X1240_SMBUS_DEV		0x50

#endif /* __INCswarmh */
