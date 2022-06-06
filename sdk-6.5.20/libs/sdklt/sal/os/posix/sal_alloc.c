/*! \file sal_alloc.c
 *
 * Memory allocation API.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <stdlib.h>

#include <sal/sal_alloc.h>
#include <sal/sal_alloc_debug.h>

SAL_ALLOC_DEBUG_DECL;

void *
sal_alloc(size_t sz, char *s)
{
    SAL_ALLOC_DEBUG_ALLOC(sz, s);

    return malloc(sz);
}

void
sal_free(void *addr)
{
    SAL_ALLOC_DEBUG_FREE(addr);

    free(addr);
}
