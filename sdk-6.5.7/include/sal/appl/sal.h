/*
 * $Id: sal.h,v 1.21 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:     sal.h
 * Purpose:     SAL Support definitions
 */

#ifndef _SAL_H
#define _SAL_H

#include <sal/types.h>
#include <sal/core/thread.h>
#include <sal/core/time.h>

/*
 * Include the system types so that things like uint32_t are always
 * available and consistent.
 */
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#endif

#if defined(UNIX) && !defined(__STRICT_ANSI__)
#define sal_strncasecmp strncasecmp
#define sal_strcasecmp strcasecmp
#else
extern int sal_strncasecmp(const char *s1, const char *s2, size_t n);
extern int sal_strcasecmp(const char *s1, const char *s2);
#endif

extern char *sal_strcasestr(const char *haystack, const char *needle);


/*
 * SAL Initialization
 */

int    sal_appl_init(void);
/*
 * Console Initialization
 */
void    sal_readline_init(void);
void    sal_readline_term(void);

/*
 * SAL Date Routines
 */

int    sal_date_set(sal_time_t *val);
int    sal_date_get(sal_time_t *val);

/*
 * SAL Booting/Rebooting routines.
 */

void    sal_reboot(void);

/*
 * SAL Shell routine - to escape to shell on whatever platform.
 */

void    sal_shell(void);

/* 
 * LED Support, platform specific routine to set led value.
 *  sal_led: sets the state of one or more discrete LEDs; bit 0 is first LED
 *  sal_led_string: sets the state of an alphanumeric display, if any.
 */

extern uint32    sal_led(uint32 led);
extern void    sal_led_string(const char *s);

/*
 * Watchdog timer, platform specific
 */

extern int    sal_watchdog_arm(uint32 usec);

/* 
 * Memory Testing 
 */

extern int    sal_memory_check(uint32 addr);

#ifdef BROADCOM_DEBUG
#ifdef INCLUDE_BCM_SAL_PROFILE
extern void sal_dma_alloc_resource_usage_get(unsigned int *alloc_curr,
                                             unsigned int *alloc_max);
#endif
#endif

/*
 * Compatibility macros
 */

#ifdef __KERNEL__
#include <linux/kernel.h>
#define strtol simple_strtol 
#define strtoul simple_strtoul 
#endif

/*
 * Some time functions only available on UNIX/vxWorks
 * For now other systems just won't use these.
 */
#if defined(UNIX) || defined(VXWORKS)
#define sal_mktime mktime
#define sal_localtime localtime
#define sal_strftime strftime
#endif

#endif    /* !_SAL_H */

