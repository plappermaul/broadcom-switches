/*
 * $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_MIRROR_H__
#define __BCMX_MIRROR_H__

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcmx/lplist.h>
#include <bcm/mirror.h>

typedef bcm_mirror_destination_t bcmx_mirror_destination_t;

/* Initialize the mirror destination structure. */
extern void bcmx_mirror_destination_t_init(
    bcmx_mirror_destination_t *mirror_dest);

/* Initialize the mirroring module to its initial configuration. */
extern int bcmx_mirror_init(void);

/* Create a mirror (destination, encapsulation) pair. */
extern int bcmx_mirror_destination_create(
    bcmx_mirror_destination_t *mirror_dest);

/* Get  mirror (destination, encapsulation) pair. */
extern int bcmx_mirror_destination_get(
    bcm_gport_t mirror_dest_id, 
    bcmx_mirror_destination_t *mirror_dest);

/* Destroy a mirror (destination, encapsulation) pair. */
extern int bcmx_mirror_destination_destroy(
    bcm_gport_t mirror_dest_id);

/* Set the current mirroring mode for the device. */
extern int bcmx_mirror_mode_set(
    int mode);

/* Get the current mirroring mode for the device. */
extern int bcmx_mirror_mode_get(
    int *mode);

/* Set the mirror-to port for the device. */
extern int bcmx_mirror_to_set(
    bcmx_lport_t port);

/* Get the mirror-to port for the device. */
extern int bcmx_mirror_to_get(
    bcmx_lport_t *port);

/* Enable or Disable ingress mirroring for a port on the device. */
extern int bcmx_mirror_ingress_set(
    bcmx_lport_t port, 
    int val);

/* 
 * Get the ingress mirroring enabled/disabled status for a port on the
 * device.
 */
extern int bcmx_mirror_ingress_get(
    bcmx_lport_t port, 
    int *val);

/* Enable or Disable egress mirroring for a port on the device. */
extern int bcmx_mirror_egress_set(
    bcmx_lport_t port, 
    int val);

/* 
 * Get the egress mirroring enabled/disabled status for a port on the
 * device.
 */
extern int bcmx_mirror_egress_get(
    bcmx_lport_t port, 
    int *val);

/* Set mirror-to ports for a port. */
extern int bcmx_mirror_fabric_list_set(
    bcmx_lport_t port, 
    bcmx_lplist_t lplist);

/* Get mirror-to ports for a port. */
extern int bcmx_mirror_fabric_list_get(
    bcmx_lport_t port, 
    bcmx_lplist_t *lplist);

/* Set mirroring configuration for a port. */
extern int bcmx_mirror_port_set(
    bcmx_lport_t port, 
    bcmx_lport_t dest_port, 
    uint32 flags);

/* Get mirroring configuration for a port. */
extern int bcmx_mirror_port_get(
    bcmx_lport_t port, 
    bcmx_lport_t *dest_port, 
    uint32 *flags);

/* Set VLAN for egressing mirrored packets on a port (RSPAN). */
extern int bcmx_mirror_vlan_set(
    bcmx_lport_t port, 
    uint16 tpid, 
    uint16 vlan);

/* Get VLAN for egressing mirrored packets on a port (RSPAN). */
extern int bcmx_mirror_vlan_get(
    bcmx_lport_t port, 
    uint16 *tpid, 
    uint16 *vlan);

/* Add mirroring destination to a port */
extern int bcmx_mirror_port_dest_add(
    bcmx_lport_t port, 
    uint32 flags, 
    bcm_gport_t mirror_dest_id);

/* Delete mirroring destination from a port */
extern int bcmx_mirror_port_dest_delete(
    bcmx_lport_t port, 
    uint32 flags, 
    bcm_gport_t mirror_dest_id);

/* Delete all mirroring destinations from a port */
extern int bcmx_mirror_port_dest_delete_all(
    bcmx_lport_t port, 
    uint32 flags);

/* Get port mirroring destinations */
extern int bcmx_mirror_port_dest_get(
    bcmx_lport_t port, 
    uint32 flags, 
    int mirror_dest_size, 
    bcm_gport_t *mirror_dest, 
    int *mirror_dest_count);

#endif /* __BCMX_MIRROR_H__ */
