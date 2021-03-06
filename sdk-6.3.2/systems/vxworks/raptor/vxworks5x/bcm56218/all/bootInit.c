/* bootInit.c - ROM initialization module */

/* Copyright 1989-1999 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* $Id: bootInit.c,v 1.5 2012/03/02 15:29:13 yaronm Exp $
modification history
--------------------
04x,10jun99,jpd  fix error when BOOTCODE_IN_RAM defined (SPR #27775).
04w,13nov98,cdp  make Thumb support for ARM CPUs dependent on ARM_THUMB.
04w,10feb99,db   Fix to ensure that default bootline gets copied for
		 standalone and rom-resident images(SPR #21763).
04v,05oct98,jmp  doc: cleanup.
04u,17apr98,cdp  backed out 04t and made absEntry volatile for ARM.
04t,16apr98,cdp  for ARM, make UNCOMPRESS entry point in RAM.
04s,20mar98,cdp  make ROM_COPY_SIZE subject to #ifndef.
04r,11nov97,cdp  ARM7TDMI_T: force romStart to call entry point in Thumb state.
		 (SPR# 9716)
04q,14jul97,tam  changed remaining references to bfillLong to fillLong. 
04p,12feb97,dat  Added USER_RESERVED_MEM, SYS_MEM_TOP, SYS_MEM_BOTTOM, SPR 8030
04o,04feb97,ms   fixed compiler warning about protoype for bcopyLongs.
04o,28nov96,cdp  added ARM support.
04n,03sep96,hdn  added the compression support for pc[34]86 BSP.
04m,19aug96,ms   added UNCMP_RTN macro to use inflate instead of uncompress
04l,21jun96,jmb  long modhist -- deleted entries prior to 1994.  SPR #6528
03k,10jun96,tam  added rom resident support for PPC architecture. 
03j,14may96,dat  fixed compiler warnings for copyLongs, fillLongs. SPR #6536
03i,06mar96,tpr  changed absEntry to be volatile for PowerPC.
03h,22aug95,hdn  added support for I80X86.
03g,14mar95,caf  restored mips resident rom support (SPR #3856).
03f,16feb95,jdi  doc format change.
03f,23may95,yao  define binArrayStart and binArrayEnd for PowerPC
		 because tools don't prepend "_".
03e,09dec94,caf  undid mod 03a, use sdata for resident roms (SPR #3856).
03d,22jun94,caf  undid 16-byte alignment portion of mod 03c, below.
03c,14jun94,cd   corrected definitions of etext, edata and end.
	   +caf  for R4000 resident ROMs: data starts on 16-byte boundary.
		 for R4000 uncompressed ROMs: added volatile to absEntry type.
*/

/*
DESCRIPTION
This module provides a generic boot ROM facility.  The target-specific
romInit.s module performs the minimal preliminary board initialization and
then jumps to the C routine romStart().  This routine, still executing out
of ROM, copies the first stage of the startup code to a RAM address and
jumps to it.  The next stage clears memory and then uncompresses the
remainder of ROM into the final VxWorks ROM image in RAM.

A modified version of the Public Domain \f3zlib\fP library is used to
uncompress the VxWorks boot ROM executable linked with it.  Compressing
object code typically achieves over 55% compression, permitting much
larger systems to be burned into ROM.  The only expense is the added few
seconds delay while the first two stages complete.

ROM AND RAM MEMORY LAYOUT
Example memory layout for a 1-megabyte board:
.CS
    --------------  0x00100000 = LOCAL_MEM_SIZE = sysMemTop()
    |            |
    |    RAM     |
    |  0 filled  |
    |            |
    |------------| = (romInit+ROM_COPY_SIZE) or binArrayStart
    | ROM image  |
    |----------- |  0x00090000  = RAM_HIGH_ADRS
    | STACK_SAVE |
    |------------|
    |            |  0x00080000  = 0.5 Megabytes
    |            |
    |            |
    | 0 filled   |
    |            |
    |            |  0x00001000  = RAM_ADRS & RAM_LOW_ADRS
    |            |
    |            |  exc vectors, bp anchor, exc msg, bootline
    |            |
    |            |
    --------------  0x00000000  = LOCAL_MEM_LOCAL_ADRS
.CE
.CS
    --------------
    |    ROM     |
    |            |  0xff8xxxxx  = binArrayStart
    |            |
    |            |  0xff800008  = ROM_TEXT_ADRS
    --------------  0xff800000  = ROM_BASE_ADRS
.CE

SEE ALSO:
inflate(), romInit(), and deflate

AUTHOR
The original compression software for zlib was written by Jean-loup Gailly
and Mark Adler. See the manual pages of inflate and deflate for
more information on their freely available compression software.
*/

#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "errno.h"
#include "sioLib.h"
#include "bcm56218.h"

#define	UNCMP_RTN	inflate

#ifndef USER_RESERVED_MEM
#   define USER_RESERVED_MEM 0
#endif

/*
 * If memory is to be cleared, it will be cleared from SYS_MEM_BOTTOM
 * up to (but not including) SYS_MEM_TOP, except for text and data segments.
 * The user reserved area is not cleared.
 */

#define	SYS_MEM_TOP \
	(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM)

#define SYS_MEM_BOTTOM \
	(LOCAL_MEM_LOCAL_ADRS + RESERVED)

IMPORT void	romInit ();
IMPORT STATUS	UNCMP_RTN ();
IMPORT void	usrInit ();
IMPORT void	sysInitAlt ();
IMPORT void	start ();

#if	((CPU_FAMILY == MIPS) || (CPU_FAMILY==PPC))
#define	binArrayStart	_binArrayStart
#define	binArrayEnd	_binArrayEnd
#define	RESIDENT_DATA	RAM_DST_ADRS
#else
#define	RESIDENT_DATA	(&sdata)
IMPORT char	sdata;			/* defined in romInit.s */
#endif

IMPORT UCHAR	binArrayStart [];	/* compressed binary image */
IMPORT UCHAR	binArrayEnd;		/* end of compressed binary image */
IMPORT char	etext [];		/* defined by the loader */
IMPORT char	edata [];		/* defined by the loader */
IMPORT char	end [];			/* defined by the loader */


#ifndef RAM_DST_ADRS                	/* default uncompress dest. */
#define RAM_DST_ADRS        RAM_HIGH_ADRS
#endif

/* If the boot code is in RAM and the RAM is already initialized,
 * clearing the RAM is not necessary.  Macro BOOTCODE_IN_RAM is
 * used not to clear the RAM.
 */
#ifdef	BOOTCODE_IN_RAM			/* not to clear RAM */
#undef	ROM_TEXT_ADRS
#undef	ROM_BASE_ADRS
#define	ROM_TEXT_ADRS	((UINT)romInit)
#define	ROM_BASE_ADRS	((UINT)romInit)
#endif	/* BOOTCODE_IN_RAM */

#if	defined (UNCOMPRESS) || defined (ROM_RESIDENT)
#ifndef ROM_COPY_SIZE
#define	ROM_COPY_SIZE	(ROM_SIZE - (ROM_TEXT_ADRS - ROM_BASE_ADRS))
#endif
#endif	/* UNCOMPRESS */

#define ROM_OFFSET(adr)	(((UINT)adr - (UINT)romInit) + ROM_TEXT_ADRS)

/* forward declarations */

LOCAL void copyLongs (FAST UINT *source, FAST UINT *destination, UINT nlongs);
#ifndef	BOOTCODE_IN_RAM
LOCAL void fillLongs (FAST UINT *buf, UINT nlongs, FAST UINT val);
#endif	/* BOOTCODE_IN_RAM */


#ifdef BROADCOM_BSP
#include "drv/sio/ns16552Sio.h"

#undef UART_REG
#define XTAL UART_REF_CLK_FREQ
#define UART_REG(reg)  (reg)
void sysSerialPrintString2 (char a, char b, char c, char d);
void sysLedDsply2 (char a, char b, char c, char d);
void serialPollInit (int channel);
void serialPutc(int c);
void serialOutByte (int addr, UCHAR c);
UCHAR serialInByte (int addr);

#endif /* !BROADCOM_BSP */

#if (MIPSEB)
#define LED_DIGIT(n)  ((n)^3)
#else
#define LED_DIGIT(n)  (n)
#endif

#ifdef QUICK_TURN
#define RPRINT(a, b, c, d) \
        *((volatile unsigned char *)(KSEG1ADDR(LED_DIGIT(0x18000064)))) = a; \
        *((volatile unsigned char *)(KSEG1ADDR(LED_DIGIT(0x18000068)))) = b; \
        *((volatile unsigned char *)(KSEG1ADDR(LED_DIGIT(0x1800006c)))) = d;
#else
#define RPRINT(a, b, c, d)
#endif /* QUICK_TURN */

#define ROMPRINT(a,b,c,d) RPRINT(a,b,c,d)

/*******************************************************************************
*
* romStart - generic ROM initialization
*
* This is the first C code executed after reset.
*
* This routine is called by the assembly start-up code in romInit().
* It clears memory, copies ROM to RAM, and possibly invokes the uncompressor.
* It then jumps to the entry point of the uncompressed object code.
*
* RETURNS: N/A
*/

void romStart
    (
    FAST int startType		/* start type */
    )

    {
#if ((CPU_FAMILY==SPARC) || (CPU_FAMILY==MIPS) || (CPU_FAMILY==I80X86) || \
     (CPU_FAMILY==PPC) || (CPU_FAMILY==ARM))
    volatile			/* to force absolute adressing */
#endif /* (CPU_FAMILY==SPARC) */
    FUNCPTR absEntry;		/* to avoid PC Relative Jump Subroutine */


#if (CPU_FAMILY==ARM) && (!defined(ROM_RESIDENT)) && !defined(BOOTCODE_IN_RAM)

    VOIDFUNCPTR ramfillLongs = fillLongs;     /* force call to RAM */

#define fillLongs(a,b,c) ramfillLongs(a,b,c)

#endif  /* (CPU_FAMILY==ARM) */

    /* Check board type...see if we have LEDs or not.  If no LED's
     *  use serial
     */

#ifdef BROADCOM_BSP
    VOIDFUNCPTR romPrintRtn = NULL;
#endif

    ROMPRINT ('I', 'N', 'N', '0');
#ifdef BROADCOM_BSP
#ifdef BOOT_UP_MSG_ON_SERIAL
    romPrintRtn = (VOIDFUNCPTR) (ROM_OFFSET(sysSerialPrintString2));

    ((FUNCPTR) ROM_OFFSET (serialPollInit))(0);
#else
    romPrintRtn = (VOIDFUNCPTR) ROM_OFFSET(sysLedDsply2);
#endif
#endif

    ROMPRINT ('C', 'H', 'K', '0');
    /*
     * Copy from ROM to RAM, minus the compressed image
     * if compressed boot ROM which relies on binArray
     * appearing last in DATA segment.
     */
#ifdef ROM_RESIDENT
    /* If ROM resident code, then copy only data segment
     * from ROM to RAM, initialize memory and jump
     * to usrInit.
     */
#if  (CPU_FAMILY == SPARC)
    copyLongs ((UINT *)(etext + 8), (UINT *) RESIDENT_DATA,
#elif	((CPU_FAMILY == MIPS) || (CPU_FAMILY == PPC))
    copyLongs ((UINT *)(etext + 0), (UINT *) RESIDENT_DATA,
#else
    copyLongs ((UINT *)(etext + 4), (UINT *) RESIDENT_DATA,
#endif
		 ((UINT) edata - (UINT) RESIDENT_DATA) / sizeof (long));

#else	/* ROM_RESIDENT */


#ifdef UNCOMPRESS
#if	(CPU_FAMILY == MIPS)
    /*
     * copy text to uncached locations to avoid problems with
     * copy back caches
     */
    ((FUNCPTR)ROM_OFFSET(copyLongs)) (ROM_TEXT_ADRS, (UINT)K0_TO_K1(romInit),
		ROM_COPY_SIZE / sizeof (long));
#else	/* CPU_FAMILY == MIPS */
    ((FUNCPTR)ROM_OFFSET(copyLongs)) (ROM_TEXT_ADRS, (UINT)romInit,
		ROM_COPY_SIZE / sizeof (long));
#endif	/* CPU_FAMILY == MIPS */

    ROMPRINT ('C', 'H', 'K', '1');

#else	/* UNCOMPRESS */

#if	(CPU_FAMILY == MIPS)
    /*
     * copy text to uncached locations to avoid problems with
     * copy back caches
     * copy the entire data segment because there is no way to ensure that
     * binArray is the last thing in the data segment because of GP relative
     * addressing
     */
    ((FUNCPTR)ROM_OFFSET(copyLongs)) (ROM_TEXT_ADRS, (UINT)K0_TO_K1(romInit),
		((UINT)edata - (UINT)romInit) / sizeof (long));

#else	/* CPU_FAMILY == MIPS */
    ((FUNCPTR)ROM_OFFSET(copyLongs)) (ROM_TEXT_ADRS, (UINT)romInit,
		((UINT)binArrayStart + 32 + - (UINT)romInit) / sizeof (long));
#endif	/* CPU_FAMILY == MIPS */

    ROMPRINT ('C', 'H', 'K', '2');

#endif	/* UNCOMPRESS */
#endif	/* ROM_RESIDENT */

#if	(CPU_FAMILY != MIPS) && (!defined (BOOTCODE_IN_RAM))

    /* clear all memory if cold booting */

    if (startType & BOOT_CLEAR)
	{
#ifdef ROM_RESIDENT
	/* Clear memory not loaded with text & data.
	 *
	 * We are careful about initializing all memory (except
	 * STACK_SAVE bytes) due to parity error generation (on
	 * some hardware) at a later stage.  This is usually
	 * caused by read accesses without initialization.
	 */
	fillLongs ((UINT *)SYS_MEM_BOTTOM,
		((UINT) RESIDENT_DATA - STACK_SAVE - (UINT)SYS_MEM_BOTTOM)
		/ sizeof(long), 0);
	fillLongs (((UINT *) edata),
		((UINT)SYS_MEM_TOP - ((UINT) edata)) / sizeof(long), 0);

#else	/* ROM_RESIDENT */
	fillLongs ((UINT *)(SYS_MEM_BOTTOM),
		((UINT)romInit - STACK_SAVE - (UINT)SYS_MEM_BOTTOM) /
		sizeof(long), 0);

    ROMPRINT ('C', 'H', 'K', '3');

#if     defined (UNCOMPRESS)
	fillLongs ((UINT *)((UINT)romInit + ROM_COPY_SIZE),
		    ((UINT)SYS_MEM_TOP - ((UINT)romInit + ROM_COPY_SIZE))
		    / sizeof(long), 0);
#else
	fillLongs ((UINT *)binArrayStart,
		((UINT)SYS_MEM_TOP - (UINT)binArrayStart) / sizeof (long), 0);
#endif 	/* UNCOMPRESS */

    ROMPRINT ('C', 'H', 'K', '4');

#endif 	/* ROM_RESIDENT */

	/* 
	 * Ensure the boot line is null. This is necessary for those
	 * targets whose boot line is excluded from cleaning.
	 */

	*(BOOT_LINE_ADRS) = EOS;
	}

#endif	/* (CPU_FAMILY != MIPS) && (!defined (BOOTCODE_IN_RAM)) */

    /* jump to VxWorks entry point (after uncompressing) */

#if	defined (UNCOMPRESS) || defined (ROM_RESIDENT)
#if	(CPU_FAMILY == I960)
    absEntry = (FUNCPTR)sysInitAlt;			/* reinit proc tbl */
#else
    absEntry = (FUNCPTR)usrInit;			/* on to bootConfig */
#endif	/* CPU_FAMILY == I960 */

#else
    {
#if	(CPU_FAMILY == MIPS)
#ifdef QUICK_TURN
    if (0)
#else
    volatile FUNCPTR absUncompress = (FUNCPTR) UNCMP_RTN;

    if ((absUncompress) ((UCHAR *)ROM_OFFSET(binArrayStart),
			 (UCHAR *)K0_TO_K1(RAM_DST_ADRS),
			 (int)((UINT)&binArrayEnd - (UINT)binArrayStart)) != OK)
#endif /* QUICK_TURN */

#elif	(CPU_FAMILY == I80X86) || (CPU_FAMILY == ARM)
    volatile FUNCPTR absUncompress = (FUNCPTR) UNCMP_RTN;
    if ((absUncompress) ((UCHAR *)ROM_OFFSET(binArrayStart),
	            (UCHAR *)RAM_DST_ADRS, &binArrayEnd - binArrayStart) != OK)
#else
    if (UNCMP_RTN ((UCHAR *)ROM_OFFSET(binArrayStart),
	            (UCHAR *)RAM_DST_ADRS, &binArrayEnd - binArrayStart) != OK)
#endif	/* (CPU_FAMILY == MIPS) */
    {
        ROMPRINT ('F', 'A', 'I', 'L');
        return; 		/* if we return then ROM's will halt */

    }
    ROMPRINT ('C', 'H', 'K', '5');

    absEntry = (FUNCPTR)RAM_DST_ADRS;		/* compressedEntry () */
    }
#endif	/* defined UNCOMPRESS || defined ROM_RESIDENT */

#if	((CPU_FAMILY == ARM) && ARM_THUMB)
    absEntry = (FUNCPTR)((UINT32)absEntry | 1);		/* force Thumb state */
#endif	/* CPU_FAMILY == ARM */

    (absEntry) (startType);
    }

#if     (CPU_FAMILY==ARM) && (!defined(ROM_RESIDENT))
#undef fillLongs
#endif  /* (CPU_FAMILY==ARM) */

extern void displaymsg (char *);

/*****************************************************************************
*
* sysLedDsply2 - print 4 characters to alphanumeric LEDs
*
* This routine should only be used when running from the bootrom.
* After the bootrom image has been copied to ram and decompressed, you
* should use sysLedDsply() to write a string to the LEDs.
*
* Returns:  N/A
*/
void sysLedDsply2
    (
    char a,
    char b,
    char c,
    char d
    )
    {
    char str[4];
    VOIDFUNCPTR func = (VOIDFUNCPTR) (ROM_OFFSET(displaymsg));
    str[0] = a;
    str[1] = b;
    str[2] = c;
    str[3] = d;
    func (str);
    }


/*******************************************************************************
*
* copyLongs - copy one buffer to another a long at a time
*
* This routine copies the first <nlongs> longs from <source> to <destination>.
*/

LOCAL void copyLongs (source, destination, nlongs)
    FAST UINT *source;		/* pointer to source buffer      */
    FAST UINT *destination;	/* pointer to destination buffer */
    UINT nlongs;		/* number of longs to copy       */

    {
    FAST UINT *dstend = destination + nlongs;

    while (destination < dstend)
	*destination++ = *source++;
    }


#ifndef	BOOTCODE_IN_RAM
/*******************************************************************************
*
* fillLongs - fill a buffer with a value a long at a time
*
* This routine fills the first <nlongs> longs of the buffer with <val>.
*/

LOCAL void fillLongs (buf, nlongs, val)
    FAST UINT *buf;	/* pointer to buffer              */
    UINT nlongs;	/* number of longs to fill        */
    FAST UINT val;	/* char with which to fill buffer */

    {
    FAST UINT *bufend = buf + nlongs;

    while (buf < bufend)
	*buf++ = val;
    }
#endif	/* BOOTCODE_IN_RAM */


#ifdef BROADCOM_BSP

UCHAR serialInByte
    (
    int addr
    )
    {
    return *(volatile unsigned char *) ((INTERNAL_UART_COM1)+(addr^3));
    }


void serialOutByte
    (
    int addr,
    UCHAR c
    )
    {
    *(volatile unsigned char *) ((INTERNAL_UART_COM1)+(addr^3)) = c;
    }

void serialPutc(int c)
{
    int i = 10000;
    FUNCPTR inbyte = (FUNCPTR) (ROM_OFFSET(serialInByte));
    VOIDFUNCPTR outbyte = (VOIDFUNCPTR) (ROM_OFFSET(serialOutByte));

    while (!(inbyte (UART_REG(LSR)) & 0x40) && i--)
        ;
    outbyte (UART_REG(THR), c);
}

void serialPollInit
   (
   int channel
   )
   {
   VOIDFUNCPTR func = (VOIDFUNCPTR) (ROM_OFFSET(serialOutByte));
   func (UART_REG (LCR), LCR_DLAB);
   func (UART_REG (DLL), BAUD_LO (CONSOLE_BAUD_RATE));
   func (UART_REG (DLM), BAUD_HI (CONSOLE_BAUD_RATE));
   func (UART_REG (LCR), CHAR_LEN_8 | ONE_STOP | PARITY_NONE);
   func (UART_REG (MCR), 0xb);
   }

/*****************************************************************************
*
* sysSerialPrintString2 - print 4 characters to the serial port
*
* This routine should only be used when running from the bootrom.
* After the bootrom image has been copied to ram and decompressed, you
* should use sysSerialPrintString() for general purpose string printing.
*
* Returns:  N/A
*/
void sysSerialPrintString2(char a, char b, char c, char d)
{
    FUNCPTR func = (FUNCPTR) (ROM_OFFSET(serialPutc));
    func (a);
    func (b);
    func (c);
    func (d);
    func ('\n');
    func ('\r');
}

#endif /* !BROADCOM_BSP */
