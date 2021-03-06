/*
 * $Id: m48t59y.c,v 1.1 2004/02/24 07:46:53 csm Exp $
 *
 * SGS M48-T59Y TOD/NVRAM Driver
 *
 * The SGS M48 an 8K NVRAM starting at offset M48_BASE_ADDR and
 * continuing for 8176 bytes. After that starts the Time-Of-Day (TOD)
 * registers which are used to set/get the internal date/time functions.
 *
 * This module implements Y2K compliance by taking full year numbers
 * and translating back and forth from the TOD 2-digit year.
 *
 * NOTE: for proper interaction with an operating system, the TOD should
 * be used to store Universal Coordinated Time (GMT) and timezone
 * conversions should be used.
 *
 * Here is a diagram of the memory layout:
 *
 * +---------------------------------------------+ 0xffe0a000
 * | Non-volatile memory                         | .
 * |                                             | .
 * | (8176 bytes of Non-volatile memory)         | .
 * |                                             | .
 * +---------------------------------------------+ 0xffe0bff0
 * | Flags                                       |
 * +---------------------------------------------+ 0xffe0bff1
 * | Unused                                      |
 * +---------------------------------------------+ 0xffe0bff2
 * | Alarm Seconds                               |
 * +---------------------------------------------+ 0xffe0bff3
 * | Alarm Minutes                               |
 * +---------------------------------------------+ 0xffe0bff4
 * | Alarm Date                                  |
 * +---------------------------------------------+ 0xffe0bff5
 * | Interrupts                                  |
 * +---------------------------------------------+ 0xffe0bff6
 * | WatchDog                                    |
 * +---------------------------------------------+ 0xffe0bff7
 * | Calibration                                 |
 * +---------------------------------------------+ 0xffe0bff8
 * | Seconds                                     |
 * +---------------------------------------------+ 0xffe0bff9
 * | Minutes                                     |
 * +---------------------------------------------+ 0xffe0bffa
 * | Hours                                       |
 * +---------------------------------------------+ 0xffe0bffb
 * | Day                                         |
 * +---------------------------------------------+ 0xffe0bffc
 * | Date                                        |
 * +---------------------------------------------+ 0xffe0bffd
 * | Month                                       |
 * +---------------------------------------------+ 0xffe0bffe
 * | Year (2 digits only)                        |
 * +---------------------------------------------+ 0xffe0bfff
 */

#include "vxWorks.h"
#include "bmw.h"

/*
 * Imported from bmw.h:
 *
 *   TOD_REG_BASE		Base of m48t59y TOD registers
 *   SYS_TOD_UNPROTECT()	Disable NVRAM write protect
 *   SYS_TOD_PROTECT()		Re-enable NVRAM write protect
 */

#define YEAR		0xf
#define MONTH		0xe
#define DAY		0xd
#define DAY_OF_WEEK	0xc
#define HOUR		0xb
#define MINUTE		0xa
#define SECOND		0x9
#define CONTROL		0x8
#define WATCH		0x7
#define INTCTL		0x6
#define WD_DATE		0x5
#define WD_HOUR		0x4
#define WD_MIN		0x3
#define WD_SEC		0x2
#define _UNUSED		0x1
#define FLAGS		0x0

static UINT8 *m48BaseAddr;

#define M48_ADDR     ((volatile UINT8 *) (m48BaseAddr + 8*1024 - 16))


STATUS m48_tod_init(UINT8 *addr)
{
    m48BaseAddr = addr;
    SYS_TOD_UNPROTECT();

    M48_ADDR[CONTROL] = 0;
    M48_ADDR[WATCH] = 0;
    M48_ADDR[INTCTL] = 0;

    /*
     * If the oscillator is currently stopped (as on a new part shipped
     * from the factory), start it running.
     *
     * Here is an example of the TOD bytes on a brand new M48T59Y part:
     *		00 00 00 00 00 00 00 00 00 88 8c c3 bf c8 f5 01
     */

    if (M48_ADDR[SECOND] & 0x80)
	M48_ADDR[SECOND] = 0;

    SYS_TOD_PROTECT();

    return OK;
}

/*
 * m48_tod_set
 */

static int to_bcd(int value)
{
    return value / 10 * 16 + value % 10;
}

static int from_bcd(int value)
{
    return value / 16 * 10 + value % 16;
}

static int day_of_week(int y, int m, int d)	/* 0-6 ==> Sun-Sat */
{       
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; 
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}       

/*
 * Note: the TOD should store the current GMT
 */

STATUS m48_tod_set(int year,		/* 1980-2079 */
		   int month,		/* 01-12 */
		   int day,		/* 01-31 */
		   int hour,		/* 00-23 */
		   int minute,		/* 00-59 */
		   int second)		/* 00-59 */
  
{
    SYS_TOD_UNPROTECT();

    M48_ADDR[CONTROL] |= 0x80;	/* Set WRITE bit */

    M48_ADDR[YEAR] = to_bcd(year % 100);
    M48_ADDR[MONTH] = to_bcd(month);
    M48_ADDR[DAY] = to_bcd(day);
    M48_ADDR[DAY_OF_WEEK] = day_of_week(year, month, day) + 1;
    M48_ADDR[HOUR] = to_bcd(hour);
    M48_ADDR[MINUTE] = to_bcd(minute);
    M48_ADDR[SECOND] = to_bcd(second);

    M48_ADDR[CONTROL] &= ~0x80;	/* Clear WRITE bit */

    SYS_TOD_PROTECT();

    return OK;
}

/*
 * Note: the TOD should store the current GMT
 */

STATUS m48_tod_get(int *year,		/* 1980-2079 */
		   int *month,		/* 01-12 */
		   int *day,		/* 01-31 */
		   int *hour,		/* 00-23 */
		   int *minute,		/* 00-59 */
		   int *second)		/* 00-59 */
{
    int y;

    SYS_TOD_UNPROTECT();

    M48_ADDR[CONTROL] |= 0x40;	/* Set READ bit */

    y = from_bcd(M48_ADDR[YEAR]);
    *year = y < 80 ? 2000 + y : 1900 + y;
    *month = from_bcd(M48_ADDR[MONTH]);
    *day = from_bcd(M48_ADDR[DAY]);
    /* day_of_week = M48_ADDR[DAY_OF_WEEK] & 0xf; */
    *hour = from_bcd(M48_ADDR[HOUR]);
    *minute = from_bcd(M48_ADDR[MINUTE]);
    *second = from_bcd(M48_ADDR[SECOND] & 0x7f);

    M48_ADDR[CONTROL] &= ~0x40;	/* Clear READ bit */

    SYS_TOD_PROTECT();

    return OK;
}

int m48_tod_get_second(void)
{
    return from_bcd(M48_ADDR[SECOND] & 0x7f);
}

/*
 * Watchdog function
 *
 *  If usec is 0, the watchdog timer is disarmed.
 *
 *  If usec is non-zero, the watchdog timer is armed (or re-armed) for
 *    approximately usec microseconds (if the exact requested usec is
 *    not supported by the chip, the next higher available value is used).
 *
 *  Minimum watchdog timeout = 62500 usec
 *  Maximum watchdog timeout = 124 sec (124000000 usec)
 */

void m48_watchdog_arm(int usec)
{
    int		mpy, res;

    SYS_TOD_UNPROTECT();

    if (usec == 0) {
	res = 0;
	mpy = 0;
    } else if (usec < 2000000) {	/* Resolution: 1/16s if below 2s */
	res = 0;
	mpy = (usec + 62499) / 62500;
    } else if (usec < 8000000) {	/* Resolution: 1/4s if below 8s */
	res = 1;
	mpy = (usec + 249999) / 250000;
    } else if (usec < 32000000) {	/* Resolution: 1s if below 32s */
	res = 2;
	mpy = (usec + 999999) / 1000000;
    } else {				/* Resolution: 4s up to 124s */
	res = 3;
	mpy = (usec + 3999999) / 4000000;
	if (mpy > 31)
	    mpy = 31;
    }

    M48_ADDR[WATCH] = (0x80 |		/* Steer to RST signal (IRQ = N/C) */
		       mpy << 2 |
		       res);

    SYS_TOD_PROTECT();
}
