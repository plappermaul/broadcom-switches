/* swarm.s - swarm board specific initialization routines */

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

/* $Id: rhone.s,v 1.5 2012/03/02 15:28:57 yaronm Exp $
 * This file has been developed or significantly modified by the
 * MIPS Center of Excellence Dedicated Engineering Staff.
 * This notice is as per the MIPS Center of Excellence Master Partner
 * Agreement, do not remove this notice without checking first with
 * WR/Platforms MIPS Center of Excellence engineering management.
 */

/*
modification history
--------------------
01a,15nov01,agf  Created for the bcm1250 BSP

*/

/*
DESCRIPTION
This module contains the assembly-language part of the init code for 
this board support package.  The routines "board_earlyinit" and 
"board_draminfo" live here.
*/

        /* includes */
#include "bcm1250Lib.h"


        /* defines */
#define K_EOT 0xFF

        /* macros */
#define DRAMINFO(mcchan,smbusdev,rows,cols,banks,sides) \
      .byte mcchan,smbusdev,rows,cols,banks,sides,0,0


		.text

/******************************************************************************
*
* board_earlyinit - entry point for VxWorks in ROM
*
*  Initialize board registers.  This is the earliest 
*  time the BSP gets control.  This routine cannot assume that
*  memory is operational, and therefore all code in this routine
*  must run from registers only.  The $ra register must not
*  be modified, as it contains the return address.
*
*  This routine will be called from uncached space, before
*  the caches are initialized.  If you want to make
*  subroutine calls from here, you must use the CALLKSEG1 macro.
*
*  Among other things, this is where the GPIO registers get 
*  programmed to make on-board LEDs function, or other startup
*  that has to be done before anything will work.
*  
* void board_earlyinit (void)
*  	   
*  RETURNS: N/A
*/

        .globl  board_earlyinit
        .ent    board_earlyinit
board_earlyinit:

       /*
        * Configure the GPIOs
        */

		li	t0,PHYS_TO_K1(A_GPIO_DIRECTION)
		li	t1,GPIO_OUTPUT_MASK
		sd	t1,0(t0)

		li	t0,PHYS_TO_K1(A_GPIO_INT_TYPE)
		li	t1,GPIO_INTERRUPT_MASK
		sd	t1,0(t0)

       #
       # Turn off the diagnostic LED 
       #
		li	t0,PHYS_TO_K1(A_GPIO_PIN_CLR)
		li	t1,M_GPIO_DEBUG_LED
		sd	t1,0(t0)


       #
       # Configure the LEDs
       #     

		li	t0,PHYS_TO_K1(A_IO_EXT_CS_BASE(LEDS_CS))
		li	t1,LEDS_PHYS >> S_IO_ADDRBASE
		sd	t1,R_IO_EXT_START_ADDR(t0)

		li	t1,LEDS_SIZE-1	/* Needs to be 1 smaller, se UM for details */
		sd	t1,R_IO_EXT_MULT_SIZE(t0)

		li	t1,LEDS_TIMING0
		sd	t1,R_IO_EXT_TIME_CFG0(t0)

		li	t1,LEDS_TIMING1
		sd	t1,R_IO_EXT_TIME_CFG1(t0)

		li	t1,LEDS_CONFIG
		sd	t1,R_IO_EXT_CFG(t0)



       #
       # Configure the alternate boot ROM
       #     

		li	t0,PHYS_TO_K1(A_IO_EXT_CS_BASE(ALT_BOOTROM_CS))

		li	t1,ALT_BOOTROM_PHYS >> S_IO_ADDRBASE
		sd	t1,R_IO_EXT_START_ADDR(t0)

		li	t1,ALT_BOOTROM_SIZE-1
		sd	t1,R_IO_EXT_MULT_SIZE(t0)

		li	t1,ALT_BOOTROM_TIMING0
		sd	t1,R_IO_EXT_TIME_CFG0(t0)

		li	t1,ALT_BOOTROM_TIMING1
		sd	t1,R_IO_EXT_TIME_CFG1(t0)

		li	t1,ALT_BOOTROM_CONFIG
		sd	t1,R_IO_EXT_CFG(t0)

		j	ra

	.end	board_earlyinit

#define _HARDWIRED_MEMORY_TABLE

#define LOADREL(toreg,address) \
        bal     9f; \
9:; \
        la      toreg,address; \
        addu    toreg,ra; \
        la      ra,9b; \
        subu    toreg,ra

/***************************************************************************
*
* board_draminfo - return the address of the DRAM information table
*
* int * board_draminfo (void)
*
*  RETURNS: pointer DRAM info table, return 0 to use default in the
*           bcm1250DramInit module
*/

        .globl  board_draminfo
        .ent    board_draminfo
board_draminfo:

                move    t0,ra

#ifdef _HARDWIRED_MEMORY_TABLE
                LOADREL(v0,myinfo)
#else
                move    v0,zero         # auto configure
#endif

                move    ra,t0
                j       ra



myinfo:
        #       Chan SMBusAdr   Rows Cols Bnks ChpSel
        #       ---- ---------  ---- ---- ---- ------
        DRAMINFO(1,  0,         13,  9,   2,   0);
        DRAMINFO(K_EOT,0,0,0,0,0);

        .end    board_draminfo

