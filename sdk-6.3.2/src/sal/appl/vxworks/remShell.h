/* remShell.h - remote shell service library header */

/* Copyright 1997 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* $Id: remShell.h,v 1.4 2012/03/02 16:24:05 yaronm Exp $
modification history
--------------------
01a,28may97,mjc  written.
*/

#ifndef __INCremShellh
#define __INCremShellh

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

extern void remShellInit (void);

#else	/* __STDC__ */

extern void remShellInit ();

#endif	/* __STDC__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCremShellh */
