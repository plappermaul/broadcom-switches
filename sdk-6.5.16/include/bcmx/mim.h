/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_MIM_H__
#define __BCMX_MIM_H__

#if defined(INCLUDE_L3)

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcm/mim.h>

typedef bcm_mim_vpn_config_t bcmx_mim_vpn_config_t;

typedef bcm_mim_port_t bcmx_mim_port_t;

/* Initialize the MIM VPN config structure. */
extern void bcmx_mim_vpn_config_t_init(
    bcmx_mim_vpn_config_t *mim_vpn_config);

/* Initialize the MIM port structure. */
extern void bcmx_mim_port_t_init(
    bcmx_mim_port_t *mim_port);

/* Initialize the MIM subsystem. */
extern int bcmx_mim_init(void);

/* Detach the MIM software module. */
extern int bcmx_mim_detach(void);

/* Create a VPN instance. */
extern int bcmx_mim_vpn_create(
    bcmx_mim_vpn_config_t *info);

/* Delete a VPN instance. */
extern int bcmx_mim_vpn_destroy(
    bcm_mim_vpn_t vpn);

/* Delete all VPN instances. */
extern int bcmx_mim_vpn_destroy_all(void);

/* Get a VPN instance by ID. */
extern int bcmx_mim_vpn_get(
    bcm_mim_vpn_t vpn, 
    bcmx_mim_vpn_config_t *info);

/* bcm_mim_port_add */
extern int bcmx_mim_port_add(
    bcm_mim_vpn_t vpn, 
    bcmx_mim_port_t *mim_port);

/* bcm_mim_port_delete */
extern int bcmx_mim_port_delete(
    bcm_mim_vpn_t vpn, 
    bcm_gport_t mim_port_id);

/* bcm_mim_port_delete_all */
extern int bcmx_mim_port_delete_all(
    bcm_mim_vpn_t vpn);

/* bcm_mim_port_get */
extern int bcmx_mim_port_get(
    bcm_mim_vpn_t vpn, 
    bcmx_mim_port_t *mim_port);

/* bcm_mim_port_get_all */
extern int bcmx_mim_port_get_all(
    bcm_mim_vpn_t vpn, 
    int port_max, 
    bcmx_mim_port_t *port_array, 
    int *port_count);

#endif /* defined(INCLUDE_L3) */

#endif /* __BCMX_MIM_H__ */