/*
 * $Id: libc.h,v 1.17 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File: 	libc.h
 * Purpose: 	Some C library functions to remove dependencies
 *		of the driver on external libraries.
 *
 * The compile-time flag -DRTOS_STRINGS should be used if your RTOS
 * supports the standard <string.h> routines.  The RTOS library or
 * built-in versions of these routines are likely to be much more
 * efficient than the stand-in versions below.
 */

#ifndef _SAL_LIBC_H
#define _SAL_LIBC_H

#ifdef VXWORKS
#include <sys/types.h>		/* VxWorks needs this for stdarg */
#endif

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#endif

#include <sal/types.h>
#include <stdarg.h>

#ifndef NULL
#define	NULL	0
#endif

#if defined(VXWORKS) || defined(UNIX) || defined(__KERNEL__) || \
    defined(LINUX) 
#define RTOS_STRINGS
#endif

#if defined(RTOS_STRINGS) && defined(__KERNEL__)
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifdef RTOS_STRINGS


#define sal_strlen		strlen
#define sal_strcpy		strcpy
#define sal_strncpy		strncpy
#define sal_strcmp		strcmp
#define sal_strstr              strstr
#define sal_strcat              strcat
#define sal_strncat             strncat
#define sal_strncmp             strncmp
#define sal_strchr              strchr
#define sal_strrchr             strrchr
#define sal_strspn              strspn
#define sal_strtok              strtok
#define sal_strtoul             strtoul
#define sal_strtol              strtol
#define sal_toupper             toupper
#define sal_tolower             tolower
#define sal_atoi                atoi

#define sal_abort               abort 
#define sal_fgets               fgets 
#define sal_fgetc               fgetc 
#define sal_fputs               fputs 
#define sal_fputc               fputc 
#define sal_fwrite              fwrite 
#define sal_fprintf             fprintf 
#define sal_fflush              fflush 
#define sal_vfprintf            vfprintf 



#if defined(memcpy)
void *sal_memcpy_wrapper(void *, const void *, size_t);
#define sal_memcpy		sal_memcpy_wrapper
#else
#define sal_memcpy		memcpy
#endif

#define sal_memset		memset

#else /* !RTOS_STRINGS */

extern int sal_strlen(const char *s);
extern char *sal_strcpy(char *, const char *);
extern char *sal_strncpy(char *, const char *, size_t);
extern int sal_strcmp(const char *, const char *);

extern void *sal_memcpy(void *, const void *, size_t);
extern void *sal_memset(void *, int, size_t);
extern char *sal_strstr(const char *haystack, const char *needle);
extern char *sal_strcat(const char *dest, const char *src);
extern char *sal_strncat(const char *dest, const char *src, size_t n);
extern int sal_strncmp(const char *s1, const char *s2, size_t n);
extern char *sal_strchr(const char *s, int c);
extern char *sal_strrchr(const char *s, int c);
extern size_t sal_strspn(const char *s, const char *accept);
 /* Should be avoided, not thread safe !!! */
extern char *sal_strtok(char *str, const char *delim);
extern unsigned long int sal_strtoul(const char *nptr, 
        char **endptr, int base);
extern long int sal_strtol(const char *nptr, char **endptr, int base);
extern int sal_toupper(int c);
extern int sal_tolower(int c);
extern int sal_atoi(const char *nptr);
extern void sal_abort(void);

#endif /* !RTOS_STRINGS */

/* Always use our version of memcmp, since it is broken 
 * in certain OS-versions (e..g Linux 2.4.18)
 */
extern int sal_memcmp(const void *, const void *, size_t);

/* Always use our version of strdup, strndup, which uses 
 * sal_alloc() instead of malloc(), and can be freed by sal_free()
 */
extern char *sal_strdup(const char *s);
extern char *sal_strndup(const char *s, size_t);

extern char *sal_strtok_r(char *s1, const char *delim, char **s2);

extern int sal_ctoi(const char *s, char **end);	/* C constant to integer */
extern void sal_itoa(char *buf, uint32 num,
		     int base, int caps, int prec);

#ifdef COMPILER_HAS_DOUBLE
extern void sal_ftoa(char *buf, double f, int decimals);
#endif

extern int sal_vsnprintf(char *buf, size_t bufsize,
			 const char *fmt, va_list ap)
    COMPILER_ATTRIBUTE ((format (printf, 3, 0)));
extern int sal_vsprintf(char *buf, const char *fmt, va_list ap)
    COMPILER_ATTRIBUTE ((format (printf, 2, 0)));
extern int sal_snprintf(char *buf, size_t bufsize, const char *fmt, ...)
    COMPILER_ATTRIBUTE ((format (printf, 3, 4)));
extern int sal_sprintf(char *buf, const char *fmt, ...)
    COMPILER_ATTRIBUTE ((format (printf, 2, 3)));
extern void sal_free_safe(void *ptr); 

#define SAL_RAND_MAX 32767

extern int sal_rand(void);
extern void sal_srand(unsigned seed);
extern uint32 sal_ceil_func(uint32 numerators , uint32 denominator);
extern uint32 sal_floor_func(uint32 numerators , uint32 denominator);

#endif	/* !_SAL_LIBC_H */
