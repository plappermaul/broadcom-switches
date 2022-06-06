/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File:        dnx_gport_stat_dispatch.h
 * Purpose:     dnx gport_stat driver BCM API dispatch table
 * Generator:   modules_dispatch.py
 */

#ifndef _BCM_INT_DNX_GPORT_STAT_DISPATCH_H
#define _BCM_INT_DNX_GPORT_STAT_DISPATCH_H

#include <bcm/types.h>
#include <bcm_int/dispatch.h>

#ifdef BCM_DNX_SUPPORT

extern int bcm_dnx_gport_stat_get(int,bcm_gport_t,bcm_core_t,bcm_stat_counter_interface_type_t,bcm_stat_pp_info_t *);
extern int bcm_dnx_gport_stat_set(int,bcm_gport_t,bcm_core_t,bcm_stat_counter_interface_type_t,bcm_stat_pp_info_t);

#endif /* BCM_DNX_SUPPORT */
#endif /* !_BCM_INT_DNX_GPORT_STAT_DISPATCH_H */