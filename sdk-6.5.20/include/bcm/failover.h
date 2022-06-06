/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_FAILOVER_H__
#define __BCM_FAILOVER_H__

#include <bcm/types.h>
#include <bcm/l3.h>

/* BCM_FAILOVER_* flags. */
#define BCM_FAILOVER_WITH_ID            (1 << 0)   /* flag to create a failover
                                                      object whose Id is passed */
#define BCM_FAILOVER_REPLACE            (1 << 1)   /* flag to update a failover
                                                      object whose Id is passed */
#define BCM_FAILOVER_CLEAR              (1 << 2)   /* flag to clear the failover
                                                      state */
#define BCM_FAILOVER_LOOKUP_DISABLE     (1 << 3)   /* flag to enable flooding by
                                                      disabling l2 lookup */
#define BCM_FAILOVER_LEARN_DISABLE      (1 << 4)   /* flag to disable learning
                                                      while failover is in
                                                      progress */
#define BCM_FAILOVER_ID_LOCAL           -2         /* failover ID */
#define BCM_FAILOVER_L2_LOOKUP          (1 << 5)   /* flag to create a failover
                                                      object for L2 lookup (fast
                                                      flush) */
#define BCM_FAILOVER_PAIRED             (1 << 6)   /* flag to allocate a pair of
                                                      adjoining failover objects */
#define BCM_FAILOVER_FEC                (1 << 7)   /* flag to create a FEC
                                                      failover object for 1:1 UC
                                                      Protection */
#define BCM_FAILOVER_INGRESS            (1 << 8)   /* flag to create an Ingress
                                                      failover object for 1+1
                                                      Protection */
#define BCM_FAILOVER_ENCAP              (1 << 9)   /* flag to create an
                                                      encapsulation failover
                                                      object for 1:1 MC
                                                      Protection */
#define BCM_FAILOVER_VPN_TYPE           (1 << 10)  /* flag to use VPN ID for
                                                      failover */
#define BCM_FAILOVER_MULTI_LEVEL_TYPE   (1 << 11)  /* flag to create multi level
                                                      failover */
#define BCM_FAILOVER_FEC_2ND_HIERARCHY  (1 << 12)  /* flag to allocate failover
                                                      id to be used for FEC
                                                      second hierarchy */
#define BCM_FAILOVER_FEC_3RD_HIERARCHY  (1 << 13)  /* flag to allocate failover
                                                      id to be used for FEC
                                                      third hierarchy */

/* Invalid L3 Ingress Interface */
#define BCM_FAILOVER_INVALID    0xFFFF     /* Invalid Failover Index */

/* Flags for Failover Elements */
#define BCM_FAILOVER_ELEMENT_USE_SECONDARY  (1 << 0)   /* Use Secondary path */

#if defined(INCLUDE_L3)
/* Failover Element Structure. */
typedef struct bcm_failover_element_s {
    uint32 flags;               /* flags */
    uint32 element_flags;       /* BCM_FAILOVER_ELEMENT_XXX flags */
    bcm_gport_t port;           /* MPLS/MiM/VLAN gport */
    bcm_failover_t failover_id; /* Failover Object Index. */
    bcm_if_t intf;              /* Egress Object index. */
} bcm_failover_element_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize Failover element structure. */
extern void bcm_failover_element_t_init(
    bcm_failover_element_t *failover);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Initialize the Failover software module. */
extern int bcm_failover_init(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Detach the Failover software module. */
extern int bcm_failover_cleanup(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Create a failover object. */
extern int bcm_failover_create(
    int unit, 
    uint32 flags, 
    bcm_failover_t *failover_id);
#endif

#if defined(INCLUDE_L3)
/* Destroy a failover object. */
extern int bcm_failover_destroy(
    int unit, 
    bcm_failover_t failover_id);
#endif

#if defined(INCLUDE_L3)
/* 
 * Set a failover object to enable or disable (note that failover object
 * 0 is reserved.
 */
extern int bcm_failover_set(
    int unit, 
    bcm_failover_t failover_id, 
    int enable);
#endif

#if defined(INCLUDE_L3)
/* 
 * Set a failover object to enable or disable (note that failover object
 * 0 is reserved.
 */
extern int bcm_failover_status_set(
    int unit, 
    bcm_failover_element_t *failover, 
    int enable);
#endif

#if defined(INCLUDE_L3)
/* Get the enable status of a failover object. */
extern int bcm_failover_get(
    int unit, 
    bcm_failover_t failover_id, 
    int *enable);
#endif

#if defined(INCLUDE_L3)
/* 
 * Get a failover object to enable or disable (note that failover object
 * 0 is reserved.
 */
extern int bcm_failover_status_get(
    int unit, 
    bcm_failover_element_t *failover, 
    int *enable);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* Failover Ring Structure. */
typedef struct bcm_failover_ring_s {
    uint32 flags;                   /* flags */
    bcm_vlan_vector_t vlan_vector;  /* List of Vlans. */
    bcm_gport_t port0;              /* VLAN gport */
    bcm_gport_t port1;              /* VLAN gport */
    bcm_vpn_vector_t vpn_vector;    /* List of VPN IDs. */
} bcm_failover_ring_t;

/* Initialize an empty failover ring structure. */
extern void bcm_failover_ring_t_init(
    bcm_failover_ring_t *failover);

#ifndef BCM_HIDE_DISPATCHABLE

/* Configure the failover ring on the system. */
extern int bcm_failover_ring_config_set(
    int unit, 
    bcm_failover_ring_t *failover_ring);

/* Get the failover ring configuration. */
extern int bcm_failover_ring_config_get(
    int unit, 
    bcm_failover_ring_t *failover_ring);

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Failover Multi Level Structure. */
typedef struct bcm_failover_multi_level_s {
    bcm_failover_t failover_id;     /* parent failover id. */
    bcm_failover_t prot_group_1;    /* First protection group for 2 level
                                       failover. */
    bcm_failover_t prot_group_2;    /* Second protection group for 2 level
                                       failover. */
} bcm_failover_multi_level_t;
#endif

#if defined(INCLUDE_L3)
/* Initializes 2 level failover data structure. */
extern void bcm_failover_multi_level_t_init(
    bcm_failover_multi_level_t *multi_level_failover);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Attaches multiple levels of 2 level protection switching. */
extern int bcm_failover_multi_level_attach(
    int unit, 
    bcm_failover_multi_level_t multi_level_failover);
#endif

#if defined(INCLUDE_L3)
/* 
 * Set a failover egress object with fixed offset to protect primary NH
 * intf.
 */
extern int bcm_failover_egress_set(
    int unit, 
    bcm_if_t intf, 
    bcm_l3_egress_t *failover_egr);
#endif

#if defined(INCLUDE_L3)
/* 
 * Get a failover egress object with fixed offset to protect primary NH
 * intf.
 */
extern int bcm_failover_egress_get(
    int unit, 
    bcm_if_t intf, 
    bcm_l3_egress_t *failover_egr);
#endif

#if defined(INCLUDE_L3)
/* 
 * Clear the failover egress object set earlier to protect primary NH
 * intf.
 */
extern int bcm_failover_egress_clear(
    int unit, 
    bcm_if_t intf);
#endif

#if defined(INCLUDE_L3)
/* 
 * Enable/Disable the use of failover egress protected NH for primary NH
 * intf.
 */
extern int bcm_failover_egress_status_set(
    int unit, 
    bcm_if_t intf, 
    int enable);
#endif

#if defined(INCLUDE_L3)
/* 
 * Get the current status of failover egress protection switching for
 * primary NH intf.
 */
extern int bcm_failover_egress_status_get(
    int unit, 
    bcm_if_t intf, 
    int *enable);
#endif

#if defined(INCLUDE_L3)
/* 
 * Get the protection next hop interface for the primary next hop
 * interface.
 */
extern int bcm_failover_egress_protection_get(
    int unit, 
    bcm_if_t intf, 
    bcm_if_t *prot_intf);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_FAILOVER_H__ */
