/*
 * $Id: a09f700e0ee85effb6e7c03de422602eed72866a $
 * 
 * $Copyright: Copyright 2012 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_L3_H__
#define __BCMX_L3_H__

#if defined(INCLUDE_L3)

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcmx/lplist.h>
#include <bcm/l3.h>

/* L3 Interface Structure */
typedef bcm_l3_intf_t bcmx_l3_intf_t;

/* 
 * L3 Egress Structure.
 * 
 * Description of an L3 forwarding destination.
 */
typedef struct bcmx_l3_egress_s {
    uint32 flags;                   /* Interface flags (BCM_L3_TGID,
                                       BCM_L3_L2TOCPU). */
    bcm_if_t intf;                  /* L3 interface (source MAC, tunnel). */
    bcm_mac_t mac_addr;             /* Next hop forwarding destination mac. */
    bcm_vlan_t vlan;                /* Next hop vlan id. */
    bcmx_lport_t lport;             /* Port packet switched to (if
                                       !BCM_L3_TGID). */
    bcm_trunk_t trunk;              /* Trunk packet switched to (if
                                       BCM_L3_TGID). */
    uint32 mpls_flags;              /* BCM_MPLS flag definitions. */
    bcm_mpls_label_t mpls_label;    /* MPLS label. */
    int mpls_qos_map_id;            /* MPLS EXP map ID. */
    int mpls_ttl;                   /* MPLS TTL threshold. */
    uint8 mpls_pkt_pri;             /* MPLS Packet Priority Value. */
    uint8 mpls_pkt_cfi;             /* MPLS Packet CFI Value. */
    uint8 mpls_exp;                 /* MPLS Exp. */
    bcm_if_t encap_id;              /* Encapsulation index. */
    bcm_failover_t failover_id;     /* Failover Object Index. */
    bcm_if_t failover_if_id;        /* Failover Egress Object index. */
} bcmx_l3_egress_t;

/* 
 * L3 Host Structure.
 * 
 * Contains information required for manipulating L3 host table entries.
 * 
 * The BCM_L3_IP6 flag in l3a_flags must be set to specify whether the
 * IPv4 or IPv6 addresses are valid.
 */
typedef struct bcmx_l3_host_s {
    uint32 l3a_flags;           /* See BCM_L3_xxx flag definitions. */
    bcm_vrf_t l3a_vrf;          /* Virtual router instance. */
    bcm_ip_t l3a_ip_addr;       /* Destination host IP address (IPv4). */
    bcm_ip6_t l3a_ip6_addr;     /* Destination host IP address (IPv6). */
    bcm_cos_t l3a_pri;          /* New priority in packet. */
    bcm_if_t l3a_intf;          /* L3 intf associated with this address. */
    bcm_mac_t l3a_nexthop_mac;  /* Next hop MAC addr. */
    bcmx_lport_t l3a_lport;     /* if !(flags & BCM_L3_TGID) */
    bcm_trunk_t l3a_trunk;      /* if (flags & BCM_L3_TGID) */
    int l3a_ipmc_ptr;           /* Pointer to IPMC table. */
    int l3a_lookup_class;       /* Classification lookup class ID. */
} bcmx_l3_host_t;

/* Backward compatibility. */
typedef bcmx_l3_host_t bcmx_l3_ip_t;

/* 
 * L3 Route Structure
 * 
 * Contains information required for manipulating L3 route table entries.
 * 
 * @pThe BCM_L3_IP6 flag in l3a_flags must be set to specify whether the
 * IPv4 or IPv6 addresses are valid.
 */
typedef struct bcmx_l3_route_s {
    uint32 l3a_flags;                   /* See BCM_L3_xxx flag definitions. */
    bcm_vrf_t l3a_vrf;                  /* Virtual router instance. */
    bcm_ip_t l3a_subnet;                /* IP subnet address (IPv4). */
    bcm_ip6_t l3a_ip6_net;              /* IP subnet address (IPv6). */
    bcm_ip_t l3a_ip_mask;               /* IP subnet mask (IPv4). */
    bcm_ip6_t l3a_ip6_mask;             /* IP subnet mask (IPv6). */
    bcm_if_t l3a_intf;                  /* L3 interface associated with route. */
    bcm_mac_t l3a_nexthop_mac;          /* Next hop MAC address. */
    bcm_ip_t l3a_nexthop_ip;            /* Next hop IP address (XGS1/2, IPv4). */
    bcmx_lport_t l3a_lport;             /* if !(flags & BCM_L3_TGID) */
    bcm_trunk_t l3a_trunk;              /* if (flags & BCM_L3_TGID) */
    bcm_vlan_t l3a_vid;                 /* BCM5695 only - for per-VLAN def
                                           route. */
    bcm_cos_t l3a_pri;                  /* Priority (COS). */
    uint32 l3a_tunnel_option;           /* Tunnel option value. */
    bcm_mpls_label_t l3a_mpls_label;    /* MPLS label. */
    int l3a_lookup_class;               /* Classification class ID. */
} bcmx_l3_route_t;

/* L3 Information Structure */
typedef bcm_l3_info_t bcmx_l3_info_t;

/* L3 Key Structure */
typedef bcm_l3_key_t bcmx_l3_key_t;

/* L3 Source Binding Information Structure */
typedef bcm_l3_source_bind_t bcmx_l3_source_bind_t;

/* L3 Ingress Structure */
typedef bcm_l3_ingress_t bcmx_l3_ingress_t;

/* Initialize the L3 subsystem. */
extern int bcmx_l3_init(void);

/* De-initialize the L3 subsystem. */
extern int bcmx_l3_cleanup(void);

/* Enable/disable L3 function without clearing any L3 tables. */
extern int bcmx_l3_enable_set(
    int enable);

/* Get the status of hardware tables. */
extern int bcmx_l3_info(
    bcm_l3_info_t *l3info);

/* Initialize a bcm_l3_intf_t/bcmx_l3_intf_t structure. */
extern int bcmx_l3_intf_t_init(
    bcmx_l3_intf_t *intf);

/* Create a new L3 interface. */
extern int bcmx_l3_intf_create(
    bcm_l3_intf_t *intf);

/* Delete an L3 interface. */
extern int bcmx_l3_intf_delete(
    bcm_l3_intf_t *intf);

/* Delete all L3 interfaces. */
extern int bcmx_l3_intf_delete_all(void);

/* Search for L3 interface by MAC address and VLAN. */
extern int bcmx_l3_intf_find(
    bcm_l3_intf_t *intf);

/* Search for L3 interface by VLAN only. */
extern int bcmx_l3_intf_find_vlan(
    bcm_l3_intf_t *intf);

/* Given the L3 interface number, return the interface information. */
extern int bcmx_l3_intf_get(
    bcm_l3_intf_t *intf);

/* Initialize a bcm_l3_egress_t/bcmx_l3_egress_t structure. */
extern void bcmx_l3_egress_t_init(
    bcmx_l3_egress_t *egr);

/* Create an Egress forwarding object. */
extern int bcmx_l3_egress_create(
    uint32 flags, 
    bcmx_l3_egress_t *egr, 
    bcm_if_t *intf);

/* Destroy an Egress forwarding object. */
extern int bcmx_l3_egress_destroy(
    bcm_if_t intf);

/* Get an Egress forwarding object. */
extern int bcmx_l3_egress_get(
    bcm_if_t intf, 
    bcmx_l3_egress_t *egr);

/* Find an interface pinting to an Egress forwarding object. */
extern int bcmx_l3_egress_find(
    bcmx_l3_egress_t *egr, 
    bcm_if_t *intf);

/* 
 * Traverse through the egress object table and run callback at each
 * valid entry.
 */
extern int bcmx_l3_egress_traverse(
    bcm_l3_egress_traverse_cb trav_fn, 
    void *user_data);

/* Create a Multipath Egress forwarding object. */
extern int bcmx_l3_egress_multipath_create(
    uint32 flags, 
    int intf_count, 
    bcm_if_t *intf_array, 
    bcm_if_t *mpintf);

/* Create a Multipath Egress forwarding object with specified path-width. */
extern int bcmx_l3_egress_multipath_max_create(
    uint32 flags, 
    int max_paths, 
    int intf_count, 
    bcm_if_t *intf_array, 
    bcm_if_t *mpintf);

/* Destroy an Egress Multipath forwarding object. */
extern int bcmx_l3_egress_multipath_destroy(
    bcm_if_t mpintf);

/* Get an Egress Multipath forwarding object. */
extern int bcmx_l3_egress_multipath_get(
    bcm_if_t mpintf, 
    int intf_size, 
    bcm_if_t *intf_array, 
    int *intf_count);

/* 
 * Add an Egress forwarding object to an Egress Multipath forwarding
 * object.
 */
extern int bcmx_l3_egress_multipath_add(
    bcm_if_t mpintf, 
    bcm_if_t intf);

/* 
 * Delete an Egress forwarding object from an Egress Multipath forwarding
 * object.
 */
extern int bcmx_l3_egress_multipath_delete(
    bcm_if_t mpintf, 
    bcm_if_t intf);

/* Find an interface pinting to an Egress Multipath forwarding object. */
extern int bcmx_l3_egress_multipath_find(
    int intf_count, 
    bcm_if_t *intf_array, 
    bcm_if_t *mpintf);

/* 
 * Traverse through the multipath egress object table and run callback at
 * each valid entry.
 */
extern int bcmx_l3_egress_multipath_traverse(
    bcm_l3_egress_multipath_traverse_cb trav_fn, 
    void *user_data);

/* Initialize a bcm_l3_ingress_t/bcmx_l3_ingress_t structure. */
extern void bcmx_l3_ingress_t_init(
    bcmx_l3_ingress_t *ing_intf);

/* Create an L3 Ingress Interface. */
extern int bcmx_l3_ingress_create(
    bcmx_l3_ingress_t *ing_intf, 
    bcm_if_t *intf_id);

/* Destroy an L3 Ingress Interface. */
extern int bcmx_l3_ingress_destroy(
    bcm_if_t intf_id);

/* Get L3 Ingress Interface configuration. */
extern int bcmx_l3_ingress_get(
    bcm_if_t intf, 
    bcmx_l3_ingress_t *ing_intf);

/* Find an identical ingress L3 interface. */
extern int bcmx_l3_ingress_find(
    bcmx_l3_ingress_t *ing_intf, 
    bcm_if_t *intf_id);

/* Initialize a bcm_l3_host_t/bcmx_l3_host_t structure. */
extern int bcmx_l3_host_t_init(
    bcmx_l3_host_t *ip);

/* Look up an L3 host table entry based on IP address. */
extern int bcmx_l3_host_find(
    bcmx_l3_host_t *info);

/* Add an entry into the L3 switching table. */
extern int bcmx_l3_host_add(
    bcmx_l3_host_t *info);

/* Delete an entry from the L3 host table. */
extern int bcmx_l3_host_delete(
    bcmx_l3_host_t *ip_addr);

/* Delete L3 entries based on IP prefix (network). */
extern int bcmx_l3_host_delete_by_network(
    bcmx_l3_route_t *ip_addr);

/* 
 * Deletes L3 entries that match or do not match a specified L3 interface
 * number.
 */
extern int bcmx_l3_host_delete_by_interface(
    bcmx_l3_host_t *info);

/* Deletes all L3 host table entries. */
extern int bcmx_l3_host_delete_all(
    bcmx_l3_host_t *info);

/* Return list of conflicts in the L3 table. */
extern int bcmx_l3_host_conflict_get(
    bcm_l3_key_t *ipkey, 
    bcm_l3_key_t *cf_array, 
    int cf_max, 
    int *cf_count);

/* Run L3 table aging. */
extern int bcmx_l3_host_age(
    uint32 flags, 
    bcm_l3_host_traverse_cb age_cb, 
    void *user_data);

/* Traverse through the L3 table and run callback at each valid L3 entry. */
extern int bcmx_l3_host_traverse(
    uint32 flags, 
    uint32 start, 
    uint32 end, 
    bcm_l3_host_traverse_cb cb, 
    void *user_data);

/* bcm_l3_host_invalidate_entry */
extern int bcmx_l3_host_invalidate_entry(
    bcm_ip_t info);

/* bcm_l3_host_validate_entry */
extern int bcmx_l3_host_validate_entry(
    bcm_ip_t info);

/* Initialize a bcm_l3_route_t/bcmx_l3_route_t_init structure. */
extern int bcmx_l3_route_t_init(
    bcmx_l3_route_t *route);

/* Add an IP route to the L3 route table. */
extern int bcmx_l3_route_add(
    bcmx_l3_route_t *info);

/* Delete an IP route from the DEFIP table. */
extern int bcmx_l3_route_delete(
    bcmx_l3_route_t *info);

/* Delete routes based on matching or non-matching L3 interface number. */
extern int bcmx_l3_route_delete_by_interface(
    bcmx_l3_route_t *info);

/* Delete all routes. */
extern int bcmx_l3_route_delete_all(
    bcmx_l3_route_t *info);

/* Look up a route given the network and netmask. */
extern int bcmx_l3_route_get(
    bcmx_l3_route_t *info);

/* Given a network, return all the paths for this route. */
extern int bcmx_l3_route_multipath_get(
    bcmx_l3_route_t *the_route, 
    bcmx_l3_route_t *path_array, 
    int max_path, 
    int *path_count);

/* Age the route table. */
extern int bcmx_l3_route_age(
    uint32 flags, 
    bcm_l3_route_traverse_cb age_out, 
    void *user_data);

/* Traverse through the routing table and run callback at each route. */
extern int bcmx_l3_route_traverse(
    uint32 flags, 
    uint32 start, 
    uint32 end, 
    bcm_l3_route_traverse_cb trav_fn, 
    void *user_data);

/* Set the maximum ECMP paths allowed for a route (XGS only). */
extern int bcmx_l3_route_max_ecmp_set(
    int max);

/* Get the maximum ECMP paths allowed for a route (XGS only). */
extern int bcmx_l3_route_max_ecmp_get(
    int *max);

/* 
 * Extract list of IPv6 prefixes which are forwarded based on lower 32
 * bit of IPv6 address, treated as IPv4 address.
 */
extern int bcmx_l3_ip6_prefix_map_get(
    int map_size, 
    bcm_ip6_t *ip6_array, 
    int *ip6_count);

/* 
 * Add IPv6 prefix to the list of prefixes which are forwarded based on
 * lower 32 bit of IPv6 address, treated as IPv4 address.
 */
extern int bcmx_l3_ip6_prefix_map_add(
    bcm_ip6_t ip6_addr);

/* 
 * Remove IPv6 prefix from the list of prefixes which are forwarded based
 * on lower 32 bit of IPv6 address, treated as IPv4 address.
 */
extern int bcmx_l3_ip6_prefix_map_delete(
    bcm_ip6_t ip6_addr);

/* 
 * Remove all the IPv6 prefixes from the list of prefixes which are
 * forwarded based on lower 32 bit of IPv6 address, treated as IPv4
 * address.
 */
extern int bcmx_l3_ip6_prefix_map_delete_all(void);

/* Enable/disable packet and byte counters for the selected VRF. */
extern int bcmx_l3_vrf_stat_enable_set(
    bcm_vrf_t vrf, 
    int enable);

/* Get 64-bit counter value for specified VRF statistic type. */
extern int bcmx_l3_vrf_stat_get(
    bcm_vrf_t vrf, 
    bcm_l3_vrf_stat_t stat, 
    uint64 *val);

/* Get lower 32-bit counter value for specified VRF statistic type. */
extern int bcmx_l3_vrf_stat_get32(
    bcm_vrf_t vrf, 
    bcm_l3_vrf_stat_t stat, 
    uint32 *val);

/* Set 64-bit counter value for specified VRF statistic type. */
extern int bcmx_l3_vrf_stat_set(
    bcm_vrf_t vrf, 
    bcm_l3_vrf_stat_t stat, 
    uint64 val);

/* Set lower 32-bit counter value for specified VRF statistic type. */
extern int bcmx_l3_vrf_stat_set32(
    bcm_vrf_t vrf, 
    bcm_l3_vrf_stat_t stat, 
    uint32 val);

/* Get 64-bit counter value for multiple VRF statistic types. */
extern int bcmx_l3_vrf_stat_multi_get(
    bcm_vrf_t vrf, 
    int nstat, 
    bcm_l3_vrf_stat_t *stat_arr, 
    uint64 *value_arr);

/* Get lower 32-bit counter value for multiple VRF statistic types. */
extern int bcmx_l3_vrf_stat_multi_get32(
    bcm_vrf_t vrf, 
    int nstat, 
    bcm_l3_vrf_stat_t *stat_arr, 
    uint32 *value_arr);

/* Set 64-bit counter value for multiple VRF statistic types. */
extern int bcmx_l3_vrf_stat_multi_set(
    bcm_vrf_t vrf, 
    int nstat, 
    bcm_l3_vrf_stat_t *stat_arr, 
    uint64 *value_arr);

/* Set lower 32-bit counter value for multiple VRF statistic types. */
extern int bcmx_l3_vrf_stat_multi_set32(
    bcm_vrf_t vrf, 
    int nstat, 
    bcm_l3_vrf_stat_t *stat_arr, 
    uint32 *value_arr);

/* Initialize a bcm_l3_source_bind_t structure. */
extern void bcmx_l3_source_bind_t_init(
    bcmx_l3_source_bind_t *info);

/* Enable or disable l3 source binding checks on an ingress port. */
extern int bcmx_l3_source_bind_enable_set(
    bcm_gport_t port, 
    int enable);

/* 
 * Retrieve whether l3 source binding checks are performed on an ingress
 * port.
 */
extern int bcmx_l3_source_bind_enable_get(
    bcm_gport_t port, 
    int *enable);

/* Add or replace an L3 source binding. */
extern int bcmx_l3_source_bind_add(
    bcmx_l3_source_bind_t *info);

/* Remove an existing L3 source binding. */
extern int bcmx_l3_source_bind_delete(
    bcmx_l3_source_bind_t *info);

/* Remove all existing L3 source bindings. */
extern int bcmx_l3_source_bind_delete_all(void);

/* Retrieve the details of an existing L3 source binding. */
extern int bcmx_l3_source_bind_get(
    bcmx_l3_source_bind_t *info);

/* 
 * Add VRID for the given VSI. Adding a VRID using this API means the
 * physical node has become the master for the virtual router
 */
extern int bcmx_l3_vrrp_add(
    bcm_vlan_t vlan, 
    uint32 vrid);

/* Delete VRID for a particulat VLAN/VSI */
extern int bcmx_l3_vrrp_delete(
    bcm_vlan_t vlan, 
    uint32 vrid);

/* Delete all the VRIDs for a particular VLAN/VSI */
extern int bcmx_l3_vrrp_delete_all(
    bcm_vlan_t vlan);

/* 
 * Get all the VRIDs for which the physical node is master for the
 * virtual routers on the given VLAN/VSI
 */
extern int bcmx_l3_vrrp_get(
    bcm_vlan_t vlan, 
    int alloc_size, 
    int *vrid_array, 
    int *count);

/* Backward compatibility. */
#define bcmx_l3_enable          bcmx_l3_enable_set 

/* Backward compatibility. */
#define bcmx_l3_defip_ecmp_max_get  bcmx_l3_defip_max_ecmp_get 

#endif /* defined(INCLUDE_L3) */

#endif /* __BCMX_L3_H__ */