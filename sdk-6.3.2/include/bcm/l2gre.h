/*
 * $Id: l2gre.h 1.11 Broadcom SDK $
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

#ifndef __BCM_L2GRE_H__
#define __BCM_L2GRE_H__

#if defined(INCLUDE_L3)

#include <bcm/types.h>
#include <bcm/l3.h>
#include <bcm/tunnel.h>

#if defined(INCLUDE_L3)
/* BCM_L2GRE_VPN_* flags for L2GRE VPN Create. */
#define BCM_L2GRE_VPN_ELINE     0x00000001 
#define BCM_L2GRE_VPN_ELAN      0x00000002 
#define BCM_L2GRE_VPN_WITH_ID   0x00000004 
#endif

#if defined(INCLUDE_L3)
/* L2GRE VPN Definitions. */
#define BCM_L2GRE_VPN_INVALID   0xFFFF     /* Invalid L2GRE VPN */
#endif

#if defined(INCLUDE_L3)
/* L2GRE VPN Config Structure. */
typedef struct bcm_l2gre_vpn_config_s {
    uint32 flags;                       /* BCM_L2GRE_VPN_xxx. */
    bcm_vpn_t vpn;                      /* L2GRE VPN */
    bcm_multicast_t broadcast_group; 
    bcm_multicast_t unknown_unicast_group; 
    bcm_multicast_t unknown_multicast_group; 
} bcm_l2gre_vpn_config_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize L2GRE VPN config structure. */
extern void bcm_l2gre_vpn_config_t_init(
    bcm_l2gre_vpn_config_t *info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Create L2VPN instance */
extern int bcm_l2gre_vpn_create(
    int unit, 
    bcm_l2gre_vpn_config_t *info);
#endif

#if defined(INCLUDE_L3)
/* Delete L2_VPN Instance */
extern int bcm_l2gre_vpn_destroy(
    int unit, 
    bcm_vpn_t l2vpn);
#endif

#if defined(INCLUDE_L3)
/* Delete all L2-VPN instances */
extern int bcm_l2gre_vpn_destroy_all(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Get L2-VPN instance by ID */
extern int bcm_l2gre_vpn_get(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_l2gre_vpn_config_t *info);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* L2GRE port match criteria. */
typedef enum bcm_l2gre_port_match_e {
    BCM_L2GRE_PORT_MATCH_INVALID,       /* Illegal. */
    BCM_L2GRE_PORT_MATCH_NONE,          /* No source match criteria. */
    BCM_L2GRE_PORT_MATCH_PORT,          /* {Module, Port} or Trunk. */
    BCM_L2GRE_PORT_MATCH_PORT_VLAN,     /* Mod/port/trunk + outer VLAN. */
    BCM_L2GRE_PORT_MATCH_PORT_INNER_VLAN, /* Mod/port/trunk + inner VLAN. */
    BCM_L2GRE_PORT_MATCH_PORT_VLAN_STACKED, /* Mod/port/trunk + outer/inner VLAN. */
    BCM_L2GRE_PORT_MATCH_VLAN_PRI,      /* Mod/port/trunk + VLAN-PRI + VLAN-CFI. */
    BCM_L2GRE_PORT_MATCH_VPNID,         /* Match L2GRE VPNID */
    BCM_L2GRE_PORT_MATCH_TUNNEL_VPNID,  /* Match L2GRE TUNNEL-SIP + VPNID */
    BCM_L2GRE_PORT_MATCH_COUNT          /* Must be last. */
} bcm_l2gre_port_match_t;
#endif

#if defined(INCLUDE_L3)
/* BCM_L2GRE_* flags. */
#define BCM_L2GRE_PORT_WITH_ID              (1 << 0)   /* create L2GRE port with
                                                          specified ID */
#define BCM_L2GRE_PORT_DROP                 (1 << 1)   /* Drop matching packets */
#define BCM_L2GRE_PORT_NETWORK              (1 << 2)   /* Network facing
                                                          interface */
#define BCM_L2GRE_PORT_COUNTED              (1 << 3)   /* Maintain packet/byte
                                                          counts */
#define BCM_L2GRE_PORT_REPLACE              (1 << 4)   /* Replace existing entry */
#define BCM_L2GRE_PORT_INT_PRI_SET          (1 << 5)   /* Use Specified pri for
                                                          internal priority */
#define BCM_L2GRE_PORT_INT_PRI_MAP          (1 << 6)   /* Use specified exp_map
                                                          for internal priority */
#define BCM_L2GRE_PORT_EGRESS_TUNNEL        (1 << 7)   /* Specified Egress
                                                          tunnel is valid */
#define BCM_L2GRE_PORT_SERVICE_TAGGED       (1 << 8)   /* Service tag mode */
#define BCM_L2GRE_PORT_SERVICE_VLAN_ADD     (1 << 9)   /* Add SD-tag */
#define BCM_L2GRE_PORT_SERVICE_VLAN_REPLACE (1 << 10)  /* Replace VLAN not TPID */
#define BCM_L2GRE_PORT_SERVICE_VLAN_DELETE  (1 << 11)  /* Delete VLAN Tag */
#define BCM_L2GRE_PORT_SERVICE_VLAN_TPID_REPLACE (1 << 12)  /* Replace VLAN and TPID */
#define BCM_L2GRE_PORT_SERVICE_VLAN_PRI_TPID_REPLACE (1 << 13)  /* Replace VLAN, VLAN-PRI
                                                          and TPID */
#define BCM_L2GRE_PORT_SERVICE_VLAN_PRI_REPLACE (1 << 14)  /* Replace VLAN and
                                                          VLAN-PRI */
#define BCM_L2GRE_PORT_SERVICE_PRI_REPLACE  (1 << 15)  /* Replace VLAN-PRI only */
#define BCM_L2GRE_PORT_SERVICE_TPID_REPLACE (1 << 16)  /* Replace TPID only */
#define BCM_L2GRE_MULTIPATH                 (1 << 18)  /* Multipath flag for
                                                          L2GRE */
#define BCM_L2GRE_PORT_MULTICAST            (1 << 19)  /* Create Multicast-only
                                                          L2GRE Tunnel */
#endif

#if defined(INCLUDE_L3)
/* L2GRE Gport */
typedef struct bcm_l2gre_port_s {
    bcm_gport_t l2gre_port_id;          /* GPORT identifier. */
    uint32 flags;                       /* BCM_L2GRE_PORT_xxx. */
    uint32 if_class;                    /* Interface class ID. */
    uint16 int_pri;                     /* Internal Priority */
    uint8 pkt_pri;                      /* Packet Priority */
    uint8 pkt_cfi;                      /* Packet CFI */
    uint16 egress_service_tpid;         /* Service Vlan TPID Value */
    bcm_vlan_t egress_service_vlan;     /* SD-TAG Vlan ID. */
    uint16 mtu;                         /* MTU */
    bcm_gport_t match_port;             /* Match port / trunk */
    bcm_l2gre_port_match_t criteria;    /* Match criteria */
    bcm_vlan_t match_vlan;              /* Outer VLAN ID to match. */
    bcm_vlan_t match_inner_vlan;        /* Inner VLAN ID to match. */
    uint32 vpnid;                       /* VPNID per (VFI, Tunnel) */
    bcm_gport_t egress_tunnel_id;       /* Tunnel Initiator ID */
    bcm_gport_t match_tunnel_id;        /* Tunnel Terminator ID */
    bcm_if_t egress_if;                 /* L2GRE egress object. */
} bcm_l2gre_port_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize the L2GRE port structure. */
extern void bcm_l2gre_port_t_init(
    bcm_l2gre_port_t *l2gre_port);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Initialize l2gre module. */
extern int bcm_l2gre_init(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Detach l2gre module. */
extern int bcm_l2gre_cleanup(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Add Access/Network L2GRE port */
extern int bcm_l2gre_port_add(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_l2gre_port_t *l2gre_port);
#endif

#if defined(INCLUDE_L3)
/* Delete Access/Network L2GRE port */
extern int bcm_l2gre_port_delete(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_gport_t l2gre_port_id);
#endif

#if defined(INCLUDE_L3)
/* bcm_l2gre_port_delete_all */
extern int bcm_l2gre_port_delete_all(
    int unit, 
    bcm_vpn_t l2vpn);
#endif

#if defined(INCLUDE_L3)
/* Get Access/Network L2GRE port */
extern int bcm_l2gre_port_get(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_l2gre_port_t *l2gre_port);
#endif

#if defined(INCLUDE_L3)
/* Get all Access/Network L2GRE port */
extern int bcm_l2gre_port_get_all(
    int unit, 
    bcm_vpn_t l2vpn, 
    int port_max, 
    bcm_l2gre_port_t *port_array, 
    int *port_count);
#endif

#if defined(INCLUDE_L3)
/* Create L2GRE Tunnel Initiator */
extern int bcm_l2gre_tunnel_initiator_create(
    int unit, 
    bcm_tunnel_initiator_t *info);
#endif

#if defined(INCLUDE_L3)
/* Destroy L2GRE Tunnel Initiator */
extern int bcm_l2gre_tunnel_initiator_destroy(
    int unit, 
    bcm_gport_t l2gre_tunnel_id);
#endif

#if defined(INCLUDE_L3)
/* Get L2GRE Tunnel Initiator Info */
extern int bcm_l2gre_tunnel_initiator_get(
    int unit, 
    bcm_tunnel_initiator_t *info);
#endif

#if defined(INCLUDE_L3)
/* Traverse L2GRE Tunnel Initiator Info */
extern int bcm_l2gre_tunnel_initiator_traverse(
    int unit, 
    bcm_tunnel_initiator_traverse_cb cb, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
/* Create L2GRE Tunnel Terminator */
extern int bcm_l2gre_tunnel_terminator_create(
    int unit, 
    bcm_tunnel_terminator_t *info);
#endif

#if defined(INCLUDE_L3)
/* Destroy L2GRE Tunnel terminator */
extern int bcm_l2gre_tunnel_terminator_destroy(
    int unit, 
    bcm_gport_t l2gre_tunnel_id);
#endif

#if defined(INCLUDE_L3)
/* Get L2GRE Tunnel terminator Info */
extern int bcm_l2gre_tunnel_terminator_get(
    int unit, 
    bcm_tunnel_terminator_t *info);
#endif

#if defined(INCLUDE_L3)
/* Traverse L2GRE Tunnel Terminator Info */
extern int bcm_l2gre_tunnel_terminator_traverse(
    int unit, 
    bcm_tunnel_terminator_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* Ingress and Egress statistics maintained per L2gre. */
typedef enum bcm_l2gre_stat_e {
    bcmL2greInPackets,  /* Packets that ingress on the L2gre */
    bcmL2greOutPackets, /* Packets that egress on the L2gre */
    bcmL2greInBytes,    /* Bytes that ingress on the L2gre */
    bcmL2greOutBytes    /* Bytes that egress on the L2gre */
} bcm_l2gre_stat_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Attach counter entries to the given L2gre gport and vpn. */
extern int bcm_l2gre_stat_attach(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    uint32 stat_counter_id);

/* Detach counter entries to the given L2gre gport and vpn. */
extern int bcm_l2gre_stat_detach(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn);

/* Get L2gre counter value for specified L2gre gport and vpn */
extern int bcm_l2gre_stat_counter_get(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    bcm_l2gre_stat_t stat, 
    uint32 num_entries, 
    uint32 *counter_indexes, 
    bcm_stat_value_t *counter_values);

/* Set L2gre counter value for specified L2gre gport and vpn */
extern int bcm_l2gre_stat_counter_set(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    bcm_l2gre_stat_t stat, 
    uint32 num_entries, 
    uint32 *counter_indexes, 
    bcm_stat_value_t *counter_values);

/* Get 64-bit counter value for multiple L2gre statistic types. */
extern int bcm_l2gre_stat_multi_get(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    int nstat, 
    bcm_l2gre_stat_t *stat_arr, 
    uint64 *value_arr);

/* Get lower 32-bit counter value for multiple L2gre statistic types. */
extern int bcm_l2gre_stat_multi_get32(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    int nstat, 
    bcm_l2gre_stat_t *stat_arr, 
    uint32 *value_arr);

/* Set 64-bit counter value for multiple L2gre statistic types. */
extern int bcm_l2gre_stat_multi_set(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    int nstat, 
    bcm_l2gre_stat_t *stat_arr, 
    uint64 *value_arr);

/* Set lower 32-bit counter value for multiple L2gre statistic types. */
extern int bcm_l2gre_stat_multi_set32(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    int nstat, 
    bcm_l2gre_stat_t *stat_arr, 
    uint32 *value_arr);

/* Provide stat counter ids associated with given L2gre gport and vpn */
extern int bcm_l2gre_stat_id_get(
    int unit, 
    bcm_gport_t port, 
    bcm_vpn_t vpn, 
    bcm_l2gre_stat_t stat, 
    uint32 *stat_counter_id);

#endif /* defined(INCLUDE_L3) */

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_L2GRE_H__ */
