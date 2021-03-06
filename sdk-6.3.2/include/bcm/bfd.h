/*
 * $Id: bfd.h 1.38.38.2 Broadcom SDK $
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

#ifndef __BCM_BFD_H__
#define __BCM_BFD_H__

#if defined(INCLUDE_BFD)

#include <shared/bfd.h>
#include <bcm/mpls.h>

/* Miscellaneous BFD defines. */
#define BCM_BFD_ENDPOINT_INVALID            -1         /* Invalid BFD endpoint
                                                          ID. */
#define BCM_BFD_INTPRI_MAX                  16         /* Internal priority max
                                                          value. */
#define BCM_BFD_ENDPOINT_MAX_MEP_ID_LENGTH  _SHR_BFD_ENDPOINT_MAX_MEP_ID_LENGTH /* MPLS-TP MEP_ID maximum
                                                          length. */

/* BFD endpoint flags. */
#define BCM_BFD_ENDPOINT_UPDATE             0x0001     /* Update an existing BFD
                                                          endpoint. */
#define BCM_BFD_ENDPOINT_WITH_ID            0x0002     /* Use the specified
                                                          endpoint ID. */
#define BCM_BFD_ENDPOINT_PASSIVE            0x0004     /* Specifies endpoint
                                                          takes passive role. */
#define BCM_BFD_ENDPOINT_DEMAND             0x0008     /* Specifies local
                                                          endpoint is in demand
                                                          mode. */
#define BCM_BFD_ENDPOINT_ENCAP_SET          0x0010     /* Update encapsulation
                                                          on existing BFD
                                                          endpoint. */
#define BCM_BFD_ENDPOINT_PWE_RAW            0x0020     /* Use MPLS PWE RAW mode.
                                                          Valid only for PWE
                                                          Tunnel types. */
#define BCM_BFD_ENDPOINT_PWE_ACH            0x0040     /* Use MPLS Associated
                                                          Channel Header.  Valid
                                                          only for PWE Tunnel
                                                          types. */
#define BCM_BFD_ENDPOINT_IPV6               0x0080     /* Use IPv6
                                                          encapsulation. */
#define BCM_BFD_ENDPOINT_MULTIHOP           0x0100     /* BFD IP/UDP multihop
                                                          endpoint. */
#define BCM_BFD_ENDPOINT_MPLS_TP_AIS_LDI    0x0200     /* Set MPLS-TP AIS LDI. 
                                                          Valid only for MPLS-TP
                                                          Tunnel types. */
#define BCM_BFD_ENDPOINT_MPLS_TP_LKR        0x0400     /* MPLS-TP Locked Report
                                                          (LKR). Valid only for
                                                          MPLS-TP Tunnel types. */
#define BCM_BFD_ENDPOINT_MPLS_TP_USE_LABEL_SESSION 0x0800     /* Resolve MPLS-TP BFD
                                                          session ID from MPLS
                                                          label. Valid only for
                                                          MPLS-TP Tunnel types. */
#define BCM_BFD_ENDPOINT_MPLS_IP_DEST_RANDOM 0x1000     /* Randomize outgoing IP
                                                          dest addr. Valid only
                                                          for MPLS/PWE Tunnel
                                                          types. */
#define BCM_BFD_ENDPOINT_MPLS_TP_POLL_SEQUENCE_ENABLE 0x2000     /* Enable poll sequence
                                                          for MPLS-TP
                                                          encapsulations.  Valid
                                                          only for MPLS-TP
                                                          Tunnel types. */
#define BCM_BFD_ENDPOINT_SHA1_SEQUENCE_INCR 0x4000     /* Increment sequence
                                                          number.  Valid for
                                                          SHA1 authentication
                                                          only. */
#define BCM_BFD_ENDPOINT_IN_HW              0x8000     /* Process the endpint in
                                                          HW. */
#define BCM_BFD_ENDPOINT_STACK              0x10000    /* Endpoint is managed by
                                                          other unit in the
                                                          stack. */

/* Remote status flags. */
#define BCM_BFD_ENDPOINT_REMOTE_DEMAND      0x0001     /* Remote endpoint is in
                                                          demand mode. */
#define BCM_BFD_ENDPOINT_REMOTE_RX_AUTH_SEQ_KNOWN 0x002      /* Rx Authentication
                                                          Sequence ID is known. 
                                                          Valid for SHA1
                                                          authentication only. */

/* Simple Password authentication data. */
#define BCM_BFD_SIMPLE_PASSWORD_KEY_LENGTH_MAX _SHR_BFD_AUTH_SIMPLE_PASSWORD_KEY_LENGTH 

typedef struct bcm_bfd_auth_simple_password_s {
    uint8 length; 
    uint8 password[BCM_BFD_SIMPLE_PASSWORD_KEY_LENGTH_MAX]; 
} bcm_bfd_auth_simple_password_t;

/* SHA1 authentication data. */
#define BCM_BFD_AUTH_SHA1_KEY_LENGTH    _SHR_BFD_AUTH_SHA1_KEY_LENGTH 

typedef struct bcm_bfd_auth_sha1_s {
    uint8 enable; 
    uint32 sequence; 
    uint8 key[BCM_BFD_AUTH_SHA1_KEY_LENGTH]; 
} bcm_bfd_auth_sha1_t;

/* BFD Session States. */
typedef enum bcm_bfd_state_e {
    bcmBFDStateAdminDown = _SHR_BFD_SESSION_STATE_ADMIN_DOWN, 
    bcmBFDStateDown = _SHR_BFD_SESSION_STATE_DOWN, 
    bcmBFDStateInit = _SHR_BFD_SESSION_STATE_INIT, 
    bcmBFDStateUp = _SHR_BFD_SESSION_STATE_UP, 
    bcmBFDStateCount = _SHR_BFD_SESSION_STATE_COUNT 
} bcm_bfd_state_t;

/* BFD Diagnostic Codes. */
typedef enum bcm_bfd_diag_code_e {
    bcmBFDDiagCodeNone = _SHR_BFD_DIAG_CODE_NONE, 
    bcmBFDDiagCodeCtrlDetectTimeExpired = _SHR_BFD_DIAG_CODE_CTRL_DETECT_TIME_EXPIRED, 
    bcmBFDDiagCodeEchoFailed = _SHR_BFD_DIAG_CODE_ECHO_FAILED, 
    bcmBFDDiagCodeNeighbourSignalledDown = _SHR_BFD_DIAG_CODE_NEIGHBOR_SIGNALED_SESSION_DOWN, 
    bcmBFDDiagCodeForwardingPlaneReset = _SHR_BFD_DIAG_CODE_FORWARDING_PLANE_RESET, 
    bcmBFDDiagCodePathDown = _SHR_BFD_DIAG_CODE_PATH_DOWN, 
    bcmBFDDiagCodeConcatPathDown = _SHR_BFD_DIAG_CODE_CONCATENATED_PATH_DOWN, 
    bcmBFDDiagCodeAdminDown = _SHR_BFD_DIAG_CODE_ADMIN_DOWN, 
    bcmBFDDiagCodeRevConcatPathDown = _SHR_BFD_DIAG_CODE_REVERSE_CONCATENATED_PATH_DOWN, 
    bcmBFDDiagCodeMisConnectivityDefect = _SHR_BFD_DIAG_CODE_MIS_CONNECTIVITY_DEFECT, 
    bcmBFDDiagCodeCount = _SHR_BFD_DIAG_CODE_COUNT 
} bcm_bfd_diag_code_t;

/* BFD Authentication Types. */
typedef enum bcm_bfd_auth_type_e {
    bcmBFDAuthTypeNone = _SHR_BFD_AUTH_TYPE_NONE, 
    bcmBFDAuthTypeSimplePassword = _SHR_BFD_AUTH_TYPE_SIMPLE_PASSWORD, 
    bcmBFDAuthTypeKeyedMD5 = _SHR_BFD_AUTH_TYPE_KEYED_MD5, 
    bcmBFDAuthTypeMeticulousKeyedMD5 = _SHR_BFD_AUTH_TYPE_METICULOUS_KEYED_MD5, 
    bcmBFDAuthTypeKeyedSHA1 = _SHR_BFD_AUTH_TYPE_KEYED_SHA1, 
    bcmBFDAuthTypeMeticulousKeyedSHA1 = _SHR_BFD_AUTH_TYPE_METICULOUS_KEYED_SHA1, 
    bcmBFDAuthTypeCount = _SHR_BFD_AUTH_TYPE_COUNT 
} bcm_bfd_auth_type_t;

/* BFD tunnel types. */
typedef enum bcm_bfd_tunnel_type_e {
    bcmBFDTunnelTypeUdp,                /* BFD over IPv4/v6. */
    bcmBFDTunnelTypeIp4in4,             /* BFD over IPv4-in-IPv4 tunnel. */
    bcmBFDTunnelTypeIp6in4,             /* BFD over IPv6-in-IPv4 tunnel. */
    bcmBFDTunnelTypeIp4in6,             /* BFD over IPv4-in-IPv6 tunnel. */
    bcmBFDTunnelTypeIp6in6,             /* BFD over IPv6-in-IPv6 tunnel. */
    bcmBFDTunnelTypeGRE,                /* BFD over GRE IPv4-in-IPv4 tunnel. */
    bcmBFDTunnelTypeGre4In4 = bcmBFDTunnelTypeGRE, /* BFD over GRE IPv4-in-IPv4 tunnel. */
    bcmBFDTunnelTypeMpls,               /* BFD over MPLS LSP. */
    bcmBFDTunnelTypePweControlWord,     /* BFD over PW with CW. */
    bcmBFDTunnelTypePweRouterAlert,     /* BFD over PW with Router Alert. */
    bcmBFDTunnelTypePweTtl,             /* BFD over PW with TTL=1. */
    bcmBFDTunnelTypeMplsTpCc,           /* BFD for MPLS-TP proactive CC. */
    bcmBFDTunnelTypeMplsTpCcCv,         /* BFD for MPLS-TP proactive CC&CV. */
    bcmBFDTunnelTypeGre6In4,            /* BFD over GRE IPv6-in-IPv4 tunnel. */
    bcmBFDTunnelTypeGre4In6,            /* BFD over GRE IPv4-in-IPv6 tunnel. */
    bcmBFDTunnelTypeGre6In6,            /* BFD over GRE IPv6-in-IPv6 tunnel. */
    bcmBFDTunnelTypeCount 
} bcm_bfd_tunnel_type_t;

/* BFD endpoint ID type. */
typedef int bcm_bfd_endpoint_t;

/* BFD endpoint object. */
typedef struct bcm_bfd_endpoint_info_s {
    uint32 flags;                       /* Control flags. */
    bcm_bfd_endpoint_t id;              /* Endpoint identifier. */
    bcm_bfd_tunnel_type_t type;         /* Type of BFD encapsulation. */
    bcm_gport_t gport;                  /* Gport identifier. */
    bcm_gport_t tx_gport;               /* TX gport associated with this
                                           endpoint. */
    bcm_gport_t remote_gport;           /* Gport identifier of trapping
                                           destination: OAMP or CPU, local or
                                           remote. */
    bcm_vpn_t vpn;                      /* VPN. */
    uint8 vlan_pri;                     /* VLAN tag priority. */
    uint8 inner_vlan_pri;               /* Inner VLAN tag priority. */
    bcm_vrf_t vrf_id;                   /* Vrf identifier. */
    bcm_ip_t dst_ip_addr;               /* Destination IPv4 address. */
    bcm_ip6_t dst_ip6_addr;             /* Destination IPv6 address. */
    bcm_ip_t src_ip_addr;               /* Source IPv4 address. */
    bcm_ip6_t src_ip6_addr;             /* Source IPv6 address. */
    uint8 ip_tos;                       /* IPv4 Tos / IPv6 Traffic Class. */
    uint8 ip_ttl;                       /* IPv4 TTL / IPv6 Hop Count. */
    bcm_ip_t inner_dst_ip_addr;         /* Inner destination IPv4 address. */
    bcm_ip6_t inner_dst_ip6_addr;       /* Inner destination IPv6 address. */
    bcm_ip_t inner_src_ip_addr;         /* Inner source IPv4 address. */
    bcm_ip6_t inner_src_ip6_addr;       /* Inner source IPv6 address. */
    uint8 inner_ip_tos;                 /* Inner IPv4 Tos / IPv6 Traffic Class. */
    uint8 inner_ip_ttl;                 /* Inner IPv4 TTL / IPv6 Hop Count. */
    uint32 udp_src_port;                /* UDP source port for Ipv4, Ipv6. */
    bcm_mpls_label_t label;             /* Incoming inner label. */
    bcm_mpls_egress_label_t egress_label; /* The MPLS outgoing label information. */
    bcm_if_t egress_if;                 /* Egress interface. */
    uint8 mep_id[BCM_BFD_ENDPOINT_MAX_MEP_ID_LENGTH]; /* MPLS-TP CC/CV TLV and Source MEP ID. */
    uint8 mep_id_length;                /* Length of MPLS-TP CC/CV TLV and
                                           MEP-ID. */
    bcm_cos_t int_pri;                  /* Egress queuing for outgoing BFD to
                                           remote. */
    uint8 cpu_qid;                      /* CPU queue for BFD. */
    bcm_bfd_state_t local_state;        /* Local session state. */
    uint32 local_discr;                 /* Local discriminator. */
    bcm_bfd_diag_code_t local_diag;     /* Local diagnostic code. */
    uint32 local_min_tx;                /* Desired local min tx interval. */
    uint32 local_min_rx;                /* Required local rx interval. */
    uint32 local_min_echo;              /* Local minimum echo interval. */
    uint8 local_detect_mult;            /* Local detection interval multiplier. */
    bcm_bfd_auth_type_t auth;           /* Authentication type. */
    uint32 auth_index;                  /* Authentication index. */
    uint32 tx_auth_seq;                 /* Tx authentication sequence id. */
    uint32 rx_auth_seq;                 /* Rx authentication sequence id. */
    uint32 remote_flags;                /* Remote flags. */
    bcm_bfd_state_t remote_state;       /* Remote session state. */
    uint32 remote_discr;                /* Remote discriminator. */
    bcm_bfd_diag_code_t remote_diag;    /* Remote diagnostic code. */
    uint32 remote_min_tx;               /* Desired remote min tx interval. */
    uint32 remote_min_rx;               /* Required remote rx interval. */
    uint32 remote_min_echo;             /* Remote minimum echo interval. */
    uint8 remote_detect_mult;           /* Remote detection interval multiplier. */
} bcm_bfd_endpoint_info_t;

/* BFD event types. */
typedef enum bcm_bfd_event_type_e {
    bcmBFDEventStateChange, 
    bcmBFDEventRemoteStateDiag, 
    bcmBFDEventSessionDiscriminatorChange, 
    bcmBFDEventAuthenticationChange, 
    bcmBFDEventParameterChange, 
    bcmBFDEventSessionError, 
    bcmBFDEventEndpointRemote, 
    bcmBFDEventEndpointRemoteUp, 
    bcmBFDEventEndpointTimeout, 
    bcmBFDEventEndpointTimein, 
    bcmBFDEventEndpointTimeoutEarly, 
    bcmBFDEventEndpointFlagsChange, 
    bcmBFDEventCount 
} bcm_bfd_event_type_t;

typedef struct bcm_bfd_event_types_s {
    SHR_BITDCL w[_SHR_BITDCLSIZE(bcmBFDEventCount)]; 
} bcm_bfd_event_types_t;

#define BCM_BFD_EVENT_TYPE_SET(_event_types, _event_type)  SHR_BITSET((_event_types).w, (_event_type)) 

#define BCM_BFD_EVENT_TYPE_GET(_event_types, _event_type)  SHR_BITGET((_event_types).w, (_event_type)) 

#define BCM_BFD_EVENT_TYPE_CLEAR(_event_types, _event_type)  SHR_BITCLR((_event_types).w, (_event_type)) 

#define BCM_BFD_EVENT_TYPE_SET_ALL(_event_types)  SHR_BITSET_RANGE((_event_types).w, 0, bcmBFDEventCount) 

#define BCM_BFD_EVENT_TYPE_CLEAR_ALL(_event_types)  SHR_BITCLR_RANGE((_event_types).w, 0, bcmBFDEventCount) 

/* BFD endpoint statistics. */
typedef struct bcm_bfd_endpoint_stat_s {
    uint32 packets_in;          /* Total packets in. */
    uint32 packets_out;         /* Total packets out. */
    uint32 packets_drop;        /* Total packets drop. */
    uint32 packets_auth_drop;   /* Packets drop due to authentication failure. */
} bcm_bfd_endpoint_stat_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Get BFD endpoint statistics. */
extern int bcm_bfd_endpoint_stat_get(
    int unit, 
    bcm_bfd_endpoint_t endpoint, 
    bcm_bfd_endpoint_stat_t *ctr_info, 
    uint8 clear);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Callback function type for BFD event handling. */
typedef int (*bcm_bfd_event_cb)(
    int unit, 
    uint32 flags, 
    bcm_bfd_event_types_t events, 
    bcm_bfd_endpoint_t endpoint, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Initialize the BFD subsystem. */
extern int bcm_bfd_init(
    int unit);

/* Shut down the BFD subsystem. */
extern int bcm_bfd_detach(
    int unit);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Initialize an BFD endpoint info structure. */
extern void bcm_bfd_endpoint_info_t_init(
    bcm_bfd_endpoint_info_t *endpoint_info);

#ifndef BCM_HIDE_DISPATCHABLE

/* Create or replace an BFD endpoint object. */
extern int bcm_bfd_endpoint_create(
    int unit, 
    bcm_bfd_endpoint_info_t *endpoint_info);

/* Get an BFD endpoint object. */
extern int bcm_bfd_endpoint_get(
    int unit, 
    bcm_bfd_endpoint_t endpoint, 
    bcm_bfd_endpoint_info_t *endpoint_info);

/* Destroy an BFD endpoint object. */
extern int bcm_bfd_endpoint_destroy(
    int unit, 
    bcm_bfd_endpoint_t endpoint);

/* Destroy all BFD endpoint objects. */
extern int bcm_bfd_endpoint_destroy_all(
    int unit);

/* Register a callback for handling BFD events. */
extern int bcm_bfd_event_register(
    int unit, 
    bcm_bfd_event_types_t event_types, 
    bcm_bfd_event_cb cb, 
    void *user_data);

/* Unregister a callback for handling BFD events. */
extern int bcm_bfd_event_unregister(
    int unit, 
    bcm_bfd_event_types_t event_types, 
    bcm_bfd_event_cb cb);

/* 
 * Poll an BFD endpoint object.  Valid only for BFD sessions in Demand
 * Mode.
 */
extern int bcm_bfd_endpoint_poll(
    int unit, 
    bcm_bfd_endpoint_t endpoint);

/* Set SHA1 authentication entry. */
extern int bcm_bfd_auth_sha1_set(
    int unit, 
    int index, 
    bcm_bfd_auth_sha1_t *sha1);

/* Get SHA1 authentication entry. */
extern int bcm_bfd_auth_sha1_get(
    int unit, 
    int index, 
    bcm_bfd_auth_sha1_t *sha1);

/* Set Simple Password authentication entry. */
extern int bcm_bfd_auth_simple_password_set(
    int unit, 
    int index, 
    bcm_bfd_auth_simple_password_t *sp);

/* Get Simple Password authentication entry. */
extern int bcm_bfd_auth_simple_password_get(
    int unit, 
    int index, 
    bcm_bfd_auth_simple_password_t *sp);

#endif /* defined(INCLUDE_BFD) */

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_BFD_H__ */
