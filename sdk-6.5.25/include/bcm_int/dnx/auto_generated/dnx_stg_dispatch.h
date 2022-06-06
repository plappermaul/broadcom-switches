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
 * File:        dnx_stg_dispatch.h
 * Purpose:     dnx stg driver BCM API dispatch table
 * Generator:   modules_dispatch.py
 */

#ifndef _BCM_INT_DNX_STG_DISPATCH_H
#define _BCM_INT_DNX_STG_DISPATCH_H

#include <bcm/types.h>
#include <bcm_int/dispatch.h>

#ifdef BCM_DNX_SUPPORT

extern int bcm_dnx_stg_vlan_list(int,bcm_stg_t,bcm_vlan_t **,int *);
extern int bcm_dnx_stg_create(int,bcm_stg_t *);
extern int bcm_dnx_stg_create_id(int,bcm_stg_t);
extern int bcm_dnx_stg_list(int,bcm_stg_t **,int *);
extern int bcm_dnx_stg_vlan_list_destroy(int,bcm_vlan_t *,int);
extern int bcm_dnx_stg_default_set(int,bcm_stg_t);
extern int bcm_dnx_stg_vlan_remove(int,bcm_stg_t,bcm_vlan_t);
extern int bcm_dnx_stg_vlan_add(int,bcm_stg_t,bcm_vlan_t);
extern int bcm_dnx_stg_init(int);
extern int bcm_dnx_stg_clear(int);
extern int bcm_dnx_stg_stp_set(int,bcm_stg_t,bcm_port_t,int);
extern int bcm_dnx_stg_count_get(int,int *);
extern int bcm_dnx_stg_vlan_remove_all(int,bcm_stg_t);
extern int bcm_dnx_stg_destroy(int,bcm_stg_t);
extern int bcm_dnx_stg_list_destroy(int,bcm_stg_t *,int);
extern int bcm_dnx_stg_default_get(int,bcm_stg_t *);
extern int bcm_dnx_stg_stp_get(int,bcm_stg_t,bcm_port_t,int *);

#endif /* BCM_DNX_SUPPORT */
#endif /* !_BCM_INT_DNX_STG_DISPATCH_H */