/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_FLOW_H__
#define __BCM_FLOW_H__

#include <bcm/types.h>
#include <bcm/l3.h>
#include <bcm/tunnel.h>
#include <bcm/vlan.h>
#include <bcm/qos.h>
#include <bcm/flexctr.h>

/* flow handle */
typedef uint32 bcm_flow_handle_t;

/* flow option ID */
typedef uint32 bcm_flow_option_id_t;

/* flow field ID */
typedef uint32 bcm_flow_field_id_t;

#if defined(INCLUDE_L3)
/* Initialize bcm_flow_logical_field_t structure. */
extern void bcm_flow_logical_field_t_init(
    bcm_flow_logical_field_t *logical_field);
#endif

#if defined(INCLUDE_L3)
/* Flow match options */
#define BCM_FLOW_MATCH_OPTION_REPLACE   (1 << 1)   /* replace object attributes
                                                      for a match criteria. */
#endif

#if defined(INCLUDE_L3)
/* Flow match flags for hardware control parameters */
#define BCM_FLOW_MATCH_FLAG_DROP    (1 << 0)   /* drop packet. */
#endif

#if defined(INCLUDE_L3)
/* Flow match valid elements. */
#define BCM_FLOW_MATCH_VNID_VALID           (1 << 0)   /* VN_ID element is valid
                                                          for this match. */
#define BCM_FLOW_MATCH_VLAN_VALID           (1 << 1)   /* vlan element valid. */
#define BCM_FLOW_MATCH_INNER_VLAN_VALID     (1 << 2)   /* inner vlan element
                                                          valid. */
#define BCM_FLOW_MATCH_PORT_VALID           (1 << 3)   /* port element valid. */
#define BCM_FLOW_MATCH_SIP_VALID            (1 << 4)   /* source IPv4 address
                                                          element valid. */
#define BCM_FLOW_MATCH_SIP_V6_VALID         (1 << 5)   /* source IPv6 address
                                                          element valid. */
#define BCM_FLOW_MATCH_DIP_VALID            (1 << 6)   /* destination IPv4
                                                          address element valid. */
#define BCM_FLOW_MATCH_DIP_V6_VALID         (1 << 7)   /* destination IPv6
                                                          address element valid. */
#define BCM_FLOW_MATCH_VPN_VALID            (1 << 8)   /* VPN element valid. */
#define BCM_FLOW_MATCH_FLOW_PORT_VALID      (1 << 9)   /* flow port element
                                                          valid. */
#define BCM_FLOW_MATCH_IIF_VALID            (1 << 10)  /* iif element valid. */
#define BCM_FLOW_MATCH_FLEX_KEY_VALID       (1 << 11)  /* logical field elements
                                                          for key valid. */
#define BCM_FLOW_MATCH_FLEX_DATA_VALID      (1 << 12)  /* logical field elements
                                                          for data valid. */
#define BCM_FLOW_MATCH_UDP_SRC_PORT_VALID   (1 << 13)  /* udp source port field
                                                          valid. */
#define BCM_FLOW_MATCH_UDP_DST_PORT_VALID   (1 << 14)  /* udp destination port
                                                          field valid. */
#define BCM_FLOW_MATCH_PROTOCOL_VALID       (1 << 15)  /* protocol field valid. */
#define BCM_FLOW_MATCH_MPLS_LABEL_VALID     (1 << 16)  /*  mpls label field
                                                          valid. */
#define BCM_FLOW_MATCH_FLAGS_VALID          (1 << 17)  /*  flags field valid. */
#endif

#if defined(INCLUDE_L3)
/* flow match criteria. */
typedef enum bcm_flow_match_criteria_e {
    BCM_FLOW_MATCH_CRITERIA_INVALID = 0, /* Illegal. */
    BCM_FLOW_MATCH_CRITERIA_PORT = 1,   /* {Module, Port} or Trunk. */
    BCM_FLOW_MATCH_CRITERIA_PORT_VLAN = 2, /* Mod/port/trunk + outer VLAN. */
    BCM_FLOW_MATCH_CRITERIA_PORT_INNER_VLAN = 3, /* Mod/port/trunk + inner VLAN. */
    BCM_FLOW_MATCH_CRITERIA_PORT_VLAN_STACKED = 4, /* Mod/port/trunk + outer/inner VLAN. */
    BCM_FLOW_MATCH_CRITERIA_VLAN_PRI = 5, /* Mod/port/trunk + VLAN-PRI + VLAN-CFI. */
    BCM_FLOW_MATCH_CRITERIA_VN_ID = 6,  /* Match VN_ID */
    BCM_FLOW_MATCH_CRITERIA_SIP = 7,    /* Match source IP address */
    BCM_FLOW_MATCH_CRITERIA_SIP_VNID = 8, /* Match source IP address and vn_id */
    BCM_FLOW_MATCH_CRITERIA_FLEX = 9,   /* Match key specified in logical fields */
    BCM_FLOW_MATCH_CRITERIA_MPLS_LABEL = 10, /* Match MPLS label */
    BCM_FLOW_MATCH_CRITERIA_PORT_GROUP_VLAN = 11, /* Match port group + outer VID */
    BCM_FLOW_MATCH_CRITERIA_PORT_GROUP_INNER_VLAN = 12, /* Match port group + inner VID */
    BCM_FLOW_MATCH_CRITERIA_PORT_GROUP_VLAN_STACKED = 13, /* Match port group + outer VID + inner
                                           VID */
    BCM_FLOW_MATCH_CRITERIA_SVP_VNID = 14, /* Match Source VP and VN_ID */
    BCM_FLOW_MATCH_CRITERIA_COUNT = 15  /* Must be last. */
} bcm_flow_match_criteria_t;
#endif

#if defined(INCLUDE_L3)
/* Flow match config structure */
typedef struct bcm_flow_match_config_s {
    uint32 vnid;                        /* VN_ID. */
    bcm_vlan_t vlan;                    /* Outer VLAN ID to match. */
    bcm_vlan_t inner_vlan;              /* Inner VLAN ID to match. */
    bcm_gport_t port;                   /* Match port / trunk */
    bcm_ip_t sip;                       /* source IPv4 address */
    bcm_ip_t sip_mask;                  /* source IPv4 address mask */
    bcm_ip6_t sip6;                     /* source IPv6 address */
    bcm_ip6_t sip6_mask;                /* source IPv6 address mask */
    bcm_ip_t dip;                       /* destination IPv4 address */
    bcm_ip_t dip_mask;                  /* destination IPv4 address mask */
    bcm_ip6_t dip6;                     /* destination IPv6 address */
    bcm_ip6_t dip6_mask;                /* destination IPv6 address mask */
    uint16 udp_dst_port;                /* udp destination port. */
    uint16 udp_src_port;                /* udp source port. */
    uint16 protocol;                    /* IP protocol type. */
    bcm_mpls_label_t mpls_label;        /* MPLS label. */
    bcm_gport_t flow_port;              /* flow port id representing a SVP */
    bcm_vpn_t vpn;                      /* VPN representing a vfi or vrf */
    bcm_if_t intf_id;                   /* l3 interface id */
    uint32 options;                     /* BCM_FLOW_MATCH_OPTION_xxx. */
    bcm_flow_match_criteria_t criteria; /* flow match criteria. */
    uint32 valid_elements;              /* bitmap of valid fields for the match
                                           action */
    bcm_flow_handle_t flow_handle;      /* flow handle derived from flow name */
    uint32 flow_option;                 /* flow option derived from flow option
                                           name */
    uint32 flags;                       /* BCM_FLOW_MATCH_FLAG_xxx */
} bcm_flow_match_config_t;
#endif

/* bcm_flow_match_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_match_traverse_cb)(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
/* initialize the bcm_flow_match_config_t structure. */
extern void bcm_flow_match_config_t_init(
    bcm_flow_match_config_t *flow_match);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Add a match entry to assign switch objects for the matched packets */
extern int bcm_flow_match_add(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Delete a match entry for the given matching key */
extern int bcm_flow_match_delete(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Get a match entry for the given matching key */
extern int bcm_flow_match_get(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Traverse match entries */
extern int bcm_flow_match_traverse(
    int unit, 
    bcm_flow_match_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow tunnel initiator valid elements. */
#define BCM_FLOW_TUNNEL_INIT_TTL_VALID      (1 << 0)   
#define BCM_FLOW_TUNNEL_INIT_DMAC_VALID     (1 << 1)   
#define BCM_FLOW_TUNNEL_INIT_DIP_VALID      (1 << 2)   
#define BCM_FLOW_TUNNEL_INIT_SIP_VALID      (1 << 3)   
#define BCM_FLOW_TUNNEL_INIT_SIP6_VALID     (1 << 4)   
#define BCM_FLOW_TUNNEL_INIT_DIP6_VALID     (1 << 5)   
#define BCM_FLOW_TUNNEL_INIT_FLOW_LABEL_VALID (1 << 6)   
#define BCM_FLOW_TUNNEL_INIT_DSCP_VALID     (1 << 7)   
#define BCM_FLOW_TUNNEL_INIT_UDP_SPORT_VALID (1 << 8)   
#define BCM_FLOW_TUNNEL_INIT_UDP_DPORT_VALID (1 << 9)   
#define BCM_FLOW_TUNNEL_INIT_SMAC_VALID     (1 << 10)  
#define BCM_FLOW_TUNNEL_INIT_VLAN_VALID     (1 << 11)  
#define BCM_FLOW_TUNNEL_INIT_TPID_VALID     (1 << 12)  
#define BCM_FLOW_TUNNEL_INIT_PKT_PRI_VALID  (1 << 13)  
#define BCM_FLOW_TUNNEL_INIT_PKT_CFI_VALID  (1 << 14)  
#define BCM_FLOW_TUNNEL_INIT_IP4_ID_VALID   (1 << 15)  
#define BCM_FLOW_TUNNEL_INIT_FLEX_VALID     (1 << 16)  
#define BCM_FLOW_TUNNEL_INIT_DSCP_ECN_VALID (1 << 17)  
#endif

#if defined(INCLUDE_L3)
/* Flow tunneling initiator structure. */
typedef struct bcm_flow_tunnel_initiator_s {
    uint32 flags;                       /* Configuration flags. */
    bcm_tunnel_type_t type;             /* Tunnel type. */
    int ttl;                            /* Tunnel header TTL. */
    bcm_mac_t dmac;                     /* Destination MAC address. */
    bcm_ip_t dip;                       /* Tunnel header DIP (IPv4). */
    bcm_ip_t sip;                       /* Tunnel header SIP (IPv4). */
    bcm_ip6_t sip6;                     /* Tunnel header SIP (IPv6). */
    bcm_ip6_t dip6;                     /* Tunnel header DIP (IPv6). */
    uint32 flow_label;                  /* Tunnel header flow label (IPv6). */
    bcm_tunnel_dscp_select_t dscp_sel;  /* Tunnel header DSCP select. */
    int dscp;                           /* Tunnel header DSCP value. */
    int dscp_map;                       /* DSCP-map ID. */
    bcm_gport_t tunnel_id;              /* Tunnel ID */
    uint16 udp_dst_port;                /* Destination UDP port */
    uint16 udp_src_port;                /* Source UDP port */
    bcm_mac_t smac;                     /* Src MAC */
    bcm_vlan_t vlan;                    /* Tunnel VLAN */
    uint16 tpid;                        /* Tunnel TPID */
    uint8 pkt_pri;                      /* Tunnel priority */
    uint8 pkt_cfi;                      /* Tunnel CFI */
    uint16 ip4_id;                      /* IPv4 ID. */
    bcm_if_t l3_intf_id;                /* l3 Interface ID. */
    bcm_gport_t flow_port;              /* Flow port ID */
    uint32 valid_elements;              /* bitmap of valid fields */
    uint32 flow_handle;                 /* flow handle */
    uint32 flow_option;                 /* flow option */
    bcm_tunnel_dscp_ecn_select_t dscp_ecn_sel; /* Tunnel header DSCP and ECN select. */
    int dscp_ecn_map;                   /* DSCP and ECN map ID. */
    uint8 ecn;                          /* Tunnel header ECN value */
} bcm_flow_tunnel_initiator_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize bcm_flow_tunnel_initiator_t structure. */
extern void bcm_flow_tunnel_initiator_t_init(
    bcm_flow_tunnel_initiator_t *info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Create a flow tunnel initiator object */
extern int bcm_flow_tunnel_initiator_create(
    int unit, 
    bcm_flow_tunnel_initiator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Delete the flow tunnel initiator object */
extern int bcm_flow_tunnel_initiator_destroy(
    int unit, 
    bcm_gport_t flow_tunnel_id);
#endif

#if defined(INCLUDE_L3)
/* Get the flow tunnel initiator object */
extern int bcm_flow_tunnel_initiator_get(
    int unit, 
    bcm_flow_tunnel_initiator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_flow_tunnel_initiator_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_tunnel_initiator_traverse_cb)(
    int unit, 
    bcm_flow_tunnel_initiator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    void *user_data);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Traverse tunnel initiator entries */
extern int bcm_flow_tunnel_initiator_traverse(
    int unit, 
    bcm_flow_tunnel_initiator_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow tunnel terminator valid elements. */
#define BCM_FLOW_TUNNEL_TERM_VRF_VALID      (1 << 0)   
#define BCM_FLOW_TUNNEL_TERM_SIP_VALID      (1 << 1)   
#define BCM_FLOW_TUNNEL_TERM_DIP_VALID      (1 << 2)   
#define BCM_FLOW_TUNNEL_TERM_SIP_MASK_VALID (1 << 3)   
#define BCM_FLOW_TUNNEL_TERM_DIP_MASK_VALID (1 << 4)   
#define BCM_FLOW_TUNNEL_TERM_SIP6_VALID     (1 << 5)   
#define BCM_FLOW_TUNNEL_TERM_DIP6_VALID     (1 << 6)   
#define BCM_FLOW_TUNNEL_TERM_SIP6_MASK_VALID (1 << 7)   
#define BCM_FLOW_TUNNEL_TERM_DIP6_MASK_VALID (1 << 8)   
#define BCM_FLOW_TUNNEL_TERM_UDP_SRC_PORT_VALID (1 << 9)   
#define BCM_FLOW_TUNNEL_TERM_UDP_DST_PORT_VALID (1 << 10)  
#define BCM_FLOW_TUNNEL_TERM_VLAN_VALID     (1 << 11)  
#define BCM_FLOW_TUNNEL_TERM_PROTOCOL_VALID (1 << 12)  
#define BCM_FLOW_TUNNEL_TERM_FLEX_KEY_VALID (1 << 13)  
#define BCM_FLOW_TUNNEL_TERM_FLEX_DATA_VALID (1 << 14)  
#define BCM_FLOW_TUNNEL_TERM_VLAN_MASK_VALID (1 << 15)  
#define BCM_FLOW_TUNNEL_TERM_CLASS_ID_VALID (1 << 16)  
#define BCM_FLOW_TUNNEL_TERM_PBMP_VALID     (1 << 17)  
#endif

#if defined(INCLUDE_L3)
/* Flow tunnel terminator multicast tunnel states - bug flags */
#define BCM_FLOW_MULTICAST_TUNNEL_STATE_BUD_ENABLE (1 << 1)   
#define BCM_FLOW_MULTICAST_TUNNEL_STATE_BUD_DISABLE (1 << 2)   
#endif

#if defined(INCLUDE_L3)
/* Flow tunneling terminator structure. */
typedef struct bcm_flow_tunnel_terminator_s {
    uint32 flags;           /* Configuration flags. Use BCM_TUNNEL flags */
    uint32 multicast_flag;  /* VXLAN Multicast trigger flags. */
    bcm_vrf_t vrf;          /* Virtual router instance. */
    bcm_ip_t sip;           /* SIP for tunnel header match. */
    bcm_ip_t dip;           /* DIP for tunnel header match. */
    bcm_ip_t sip_mask;      /* Source IP mask. */
    bcm_ip_t dip_mask;      /* Destination IP mask. */
    bcm_ip6_t sip6;         /* SIP for tunnel header match (IPv6). */
    bcm_ip6_t dip6;         /* DIP for tunnel header match (IPv6). */
    bcm_ip6_t sip6_mask;    /* Source IP mask (IPv6). */
    bcm_ip6_t dip6_mask;    /* Destination IP mask (IPv6). */
    uint32 udp_dst_port;    /* UDP dst port for UDP packets. */
    uint32 udp_src_port;    /* UDP src port for UDP packets */
    bcm_tunnel_type_t type; /* Tunnel type */
    bcm_vlan_t vlan;        /* VLAN ID for tunnel header match. */
    uint32 protocol;        /* Protocol type */
    uint32 valid_elements;  /* bitmap of valid fields */
    uint32 flow_handle;     /* flow handle */
    uint32 flow_option;     /* flow option */
    bcm_vlan_t vlan_mask;   /* The VLAN ID mask. */
    uint32 class_id;        /* Class ID as a key in VFP. */
    bcm_pbmp_t term_pbmp;   /* Allowed port bitmap for tunnel termination. */
} bcm_flow_tunnel_terminator_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize bcm_flow_tunnel_terminator_t structure. */
extern void bcm_flow_tunnel_terminator_t_init(
    bcm_flow_tunnel_terminator_t *info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Create a flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_create(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Delete a flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_destroy(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Get the flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_get(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_flow_tunnel_terminator_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_tunnel_terminator_traverse_cb)(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    void *user_data);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Traverse tunnel terminator entries */
extern int bcm_flow_tunnel_terminator_traverse(
    int unit, 
    bcm_flow_tunnel_terminator_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow port encap valid elements. */
#define BCM_FLOW_PORT_ENCAP_PORT_VALID      (1 << 0)   /* flow port(DVP) valid */
#define BCM_FLOW_PORT_ENCAP_CLASS_ID_VALID  (1 << 1)   /* class id valid. */
#define BCM_FLOW_PORT_ENCAP_MTU_VALID       (1 << 2)   /* mtu valid. */
#define BCM_FLOW_PORT_ENCAP_FLAGS_VALID     (1 << 3)   /* flags valid. */
#define BCM_FLOW_PORT_ENCAP_NETWORK_GROUP_VALID (1 << 4)   /* network group valid. */
#define BCM_FLOW_PORT_ENCAP_DVP_GROUP_VALID (1 << 5)   /* DVP group valid. */
#define BCM_FLOW_PORT_ENCAP_FLEX_DATA_VALID (1 << 6)   /* logical field valid. */
#define BCM_FLOW_PORT_ENCAP_PKT_PRI_VALID   (1 << 7)   /* service tag priority
                                                          valid. */
#define BCM_FLOW_PORT_ENCAP_PKT_CFI_VALID   (1 << 8)   /* service tag CFI valid. */
#define BCM_FLOW_PORT_ENCAP_TPID_VALID      (1 << 9)   /* service tag TPID
                                                          valid. */
#define BCM_FLOW_PORT_ENCAP_VLAN_VALID      (1 << 10)  /* vlan valid */
#define BCM_FLOW_PORT_ENCAP_EGRESS_IF_VALID (1 << 11)  /* egress object valid */
#define BCM_FLOW_PORT_ENCAP_IP4_ID_VALID    (1 << 12)  /* IPv4 ID valid */
#define BCM_FLOW_PORT_ENCAP_VLAN_PRI_MAP_ID_VALID (1 << 13)  /* vlan_pri_map_id Valid */
#define BCM_FLOW_PORT_ENCAP_DST_PORT_VALID  (1 << 14)  /* Destination port
                                                          valid. */
#define BCM_FLOW_PORT_ENCAP_ES_ID_VALID     (1 << 15)  /* Ethernet segment ID
                                                          valid. */
#endif

#if defined(INCLUDE_L3)
/* Flow port encap set options */
#define BCM_FLOW_PORT_ENCAP_OPTION_REPLACE  (1 << 1)   /* replace encap
                                                          attributes. */
#define BCM_FLOW_PORT_ENCAP_OPTION_CLEAR    (1 << 2)   /* clear encap
                                                          attributes. */
#endif

#if defined(INCLUDE_L3)
/* Flow port encap flags for hardware control parameters */
#define BCM_FLOW_PORT_ENCAP_FLAG_SERVICE_TAGGED (1 << 0)   /* service tag mode for
                                                          the given encap
                                                          criteria. */
#define BCM_FLOW_PORT_ENCAP_FLAG_SERVICE_VLAN_ADD (1 << 1)   /* Add SD-TAG vlan. */
#define BCM_FLOW_PORT_ENCAP_FLAG_SERVICE_VLAN_TPID_REPLACE (1 << 2)   /* Replace Vlan and TPID. */
#define BCM_FLOW_PORT_ENCAP_FLAG_VLAN_REPLACE (1 << 3)   /* Replace vlan. */
#define BCM_FLOW_PORT_ENCAP_FLAG_VLAN_DELETE (1 << 4)   /* Delete vlan. */
#define BCM_FLOW_PORT_ENCAP_FLAG_VLAN_PRI_TPID_REPLACE (1 << 5)   /* Replace vlan, priority
                                                          and tpid. */
#define BCM_FLOW_PORT_ENCAP_FLAG_VLAN_PRI_REPLACE (1 << 6)   /* Replace vlan and
                                                          priority. */
#define BCM_FLOW_PORT_ENCAP_FLAG_PRI_REPLACE (1 << 7)   /* Replace priority. */
#define BCM_FLOW_PORT_ENCAP_FLAG_TPID_REPLACE (1 << 8)   /* Replace TPID. */
#define BCM_FLOW_PORT_ENCAP_FLAG_LOCAL      (1 << 9)   /* Encapsulate the FLOW
                                                          tunnel on local device */
#define BCM_FLOW_PORT_ENCAP_FLAG_DROP       (1 << 10)  /* Multicast drop control */
#define BCM_FLOW_PORT_ENCAP_FLAG_MACSEC_ENCRYPT (1 << 11)  /* Enable macsec
                                                          encryption for this
                                                          flow. */
#define BCM_FLOW_PORT_ENCAP_FLAG_IP4_ID_SET_FIXED (1 << 12)  /* Use fixed starting ID
                                                          for IPv4 tunnel */
#define BCM_FLOW_PORT_ENCAP_FLAG_IP4_ID_SET_RANDOM (1 << 13)  /* Use random starting ID
                                                          for IPv4 tunnel */
#define BCM_FLOW_PORT_ENCAP_FLAG_EGRESS_CASCADED (1 << 14)  /* Indicate the egress
                                                          object is linked to
                                                          the flow port. */
#define BCM_FLOW_PORT_ENCAP_FLAG_OPAQUE_TAGGED (1 << 15)  /* Opaque tag mode. */
#define BCM_FLOW_PORT_ENCAP_FLAG_OPAQUE_TAG_REPLACE (1 << 16)  /* Replace the opaque
                                                          tag. */
#define BCM_FLOW_PORT_ENCAP_FLAG_OPAQUE_TAG_DELETE (1 << 17)  /* Delete the opaque tag. */
#define BCM_FLOW_PORT_ENCAP_FLAG_OPAQUE_TAG_ADD (1 << 18)  /* Add the opaque tag. */
#endif

#if defined(INCLUDE_L3)
/* flow port encap config structure. */
typedef struct bcm_flow_port_encap_s {
    bcm_gport_t flow_port;          /* flow port id identifies DVP. */
    uint32 options;                 /* BCM_FLOW_PORT_ENCAP_xxx. */
    uint32 flags;                   /* BCM_FLOW_PORT_ENCAP_FLAG_xxx. */
    uint32 class_id;                /* class id. */
    uint16 mtu;                     /* MTU. */
    uint32 network_group;           /* network group ID */
    uint32 dvp_group;               /* DVP group ID */
    uint8 pri;                      /* service tag priority. */
    uint8 cfi;                      /* service tag cfi */
    uint16 tpid;                    /* service tag tpid */
    bcm_vlan_t vlan;                /* service VLAN ID. */
    bcm_if_t egress_if;             /* egress object */
    uint32 valid_elements;          /* bitmap of valid fields for hardware
                                       parameters */
    bcm_flow_handle_t flow_handle;  /* flow handle */
    uint32 flow_option;             /* flow option id */
    uint16 ip4_id;                  /* IPv4 ID */
    int vlan_pri_map_id;            /* Remarking map ID for the payload VLAN
                                       priority and CFI. */
    bcm_gport_t dst_port;           /* Destination port / trunk. */
    uint32 es_id;                   /* Ethernet segment ID. */
} bcm_flow_port_encap_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize the bcm_flow_port_encap_t configuration structure */
extern void bcm_flow_port_encap_t_init(
    bcm_flow_port_encap_t *info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Set the L2 tunnel encapsulation parameters for the given flow_port */
extern int bcm_flow_port_encap_set(
    int unit, 
    bcm_flow_port_encap_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Get the encapsulation configuration parameters */
extern int bcm_flow_port_encap_get(
    int unit, 
    bcm_flow_port_encap_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow encap options */
#define BCM_FLOW_ENCAP_OPTION_REPLACE   (1 << 1)   /* replace object attributes
                                                      for a encap criteria. */
#endif

#if defined(INCLUDE_L3)
/* Flow encap flags for hardware control parameters */
#define BCM_FLOW_ENCAP_FLAG_SERVICE_TAGGED  (1 << 0)   /* service tag mode for
                                                          the given encap
                                                          criteria. */
#define BCM_FLOW_ENCAP_FLAG_SERVICE_VLAN_ADD (1 << 1)   /* Add SD-TAG vlan. */
#define BCM_FLOW_ENCAP_FLAG_SERVICE_VLAN_TPID_REPLACE (1 << 2)   /* Replace Vlan and TPID. */
#define BCM_FLOW_ENCAP_FLAG_VLAN_REPLACE    (1 << 3)   /* Replace vlan. */
#define BCM_FLOW_ENCAP_FLAG_VLAN_DELETE     (1 << 4)   /* Delete vlan. */
#define BCM_FLOW_ENCAP_FLAG_VLAN_PRI_TPID_REPLACE (1 << 5)   /* Replace vlan, priority
                                                          and tpid. */
#define BCM_FLOW_ENCAP_FLAG_VLAN_PRI_REPLACE (1 << 6)   /* Replace vlan and
                                                          priority. */
#define BCM_FLOW_ENCAP_FLAG_PRI_REPLACE     (1 << 7)   /* Replace priority. */
#define BCM_FLOW_ENCAP_FLAG_TPID_REPLACE    (1 << 8)   /* Replace TPID. */
#define BCM_FLOW_ENCAP_FLAG_DROP            (1 << 9)   /* Drop packet. */
#define BCM_FLOW_ENCAP_FLAG_HIGIG3_EXTENSION_ADD (1 << 10)  /* Add a HIGIG3 extension
                                                          header. */
#define BCM_FLOW_ENCAP_FLAG_OPAQUE_TAGGED   (1 << 11)  /* Opaque tag mode. */
#define BCM_FLOW_ENCAP_FLAG_OPAQUE_TAG_REPLACE (1 << 12)  /* Replace the opaque
                                                          tag. */
#define BCM_FLOW_ENCAP_FLAG_OPAQUE_TAG_DELETE (1 << 13)  /* Delete the opaque tag. */
#define BCM_FLOW_ENCAP_FLAG_OPAQUE_TAG_ADD  (1 << 14)  /* Add the opaque tag. */
#endif

#if defined(INCLUDE_L3)
/* Flow encap valid elements for hardware parameters. */
#define BCM_FLOW_ENCAP_VNID_VALID           (1 << 0)   /* VN_ID element is valid
                                                          for this encap. */
#define BCM_FLOW_ENCAP_VLAN_VALID           (1 << 1)   /* vlan valid */
#define BCM_FLOW_ENCAP_FLOW_PORT_VALID      (1 << 2)   /* flow port valid */
#define BCM_FLOW_ENCAP_VPN_VALID            (1 << 3)   /* vpn valid. */
#define BCM_FLOW_ENCAP_INTF_VALID           (1 << 4)   /* interface valid. */
#define BCM_FLOW_ENCAP_PKT_PRI_VALID        (1 << 5)   /* service tag priority
                                                          valid. */
#define BCM_FLOW_ENCAP_PKT_CFI_VALID        (1 << 6)   /* service tag CFI valid. */
#define BCM_FLOW_ENCAP_TPID_VALID           (1 << 7)   /* service tag TPID
                                                          valid. */
#define BCM_FLOW_ENCAP_FLEX_KEY_VALID       (1 << 8)   /* logical fields for
                                                          key. */
#define BCM_FLOW_ENCAP_FLEX_DATA_VALID      (1 << 9)   /* logical fields for
                                                          attributes. */
#define BCM_FLOW_ENCAP_DVP_GROUP_VALID      (1 << 10)  /* DVP group ID. */
#define BCM_FLOW_ENCAP_OIF_GROUP_VALID      (1 << 11)  /* L3 OUT interface group
                                                          ID. */
#define BCM_FLOW_ENCAP_FLAGS_VALID          (1 << 12)  /* flags for hardware
                                                          control parameters. */
#define BCM_FLOW_ENCAP_SRC_FLOW_PORT_VALID  (1 << 13)  /* SVP valid. */
#define BCM_FLOW_ENCAP_CLASS_ID_VALID       (1 << 14)  /* CLASS_ID valid. */
#define BCM_FLOW_ENCAP_INT_PRI_VALID        (1 << 15)  /* INT_PRI valid. */
#define BCM_FLOW_ENCAP_PORT_GROUP_VALID     (1 << 16)  /* PORT_GROUP valid. */
#endif

#if defined(INCLUDE_L3)
/* flow encap criteria. */
typedef enum bcm_flow_encap_criteria_e {
    BCM_FLOW_ENCAP_CRITERIA_INVALID = 0, /* Illegal. */
    BCM_FLOW_ENCAP_CRITERIA_DVP = 1,    /* DVP used as a key. */
    BCM_FLOW_ENCAP_CRITERIA_VFI = 2,    /* VFI key. */
    BCM_FLOW_ENCAP_CRITERIA_VFI_DVP = 3, /* VFI + DVP key. */
    BCM_FLOW_ENCAP_CRITERIA_VRF_MAPPING = 4, /* VRF mapping key. */
    BCM_FLOW_ENCAP_CRITERIA_L3_INTF = 5, /* L3 interface key. */
    BCM_FLOW_ENCAP_CRITERIA_VFI_DVP_GROUP = 6, /* VFI + DVP group key. */
    BCM_FLOW_ENCAP_CRITERIA_FLEX = 7,   /* flex key from logical fields */
    BCM_FLOW_ENCAP_CRITERIA_SVP_DVP = 8, /* SVP + DVP key. */
    BCM_FLOW_ENCAP_CRITERIA_DGPP_INT_PRI = 9, /* DGPP + INT_PRI key. */
    BCM_FLOW_ENCAP_CRITERIA_DGPP_VLAN_INT_PRI = 10, /* DGPP + VLAN + INT_PRI key. */
    BCM_FLOW_ENCAP_CRITERIA_VFI_PORT_GROUP = 11, /* VFI + port group key. */
    BCM_FLOW_ENCAP_CRITERIA_COUNT = 12  /* Must be last. */
} bcm_flow_encap_criteria_t;
#endif

#if defined(INCLUDE_L3)
/* Flow encap config structure */
typedef struct bcm_flow_encap_config_s {
    bcm_vpn_t vpn;                      /* VPN representing a vfi or vrf */
    bcm_gport_t flow_port;              /* flow port id representing a DVP */
    bcm_if_t intf_id;                   /* l3 interface id */
    uint32 dvp_group;                   /* DVP group ID */
    uint32 oif_group;                   /* L3 OUT interface group ID */
    uint32 vnid;                        /* VN_ID. */
    uint8 pri;                          /* service tag priority. */
    uint8 cfi;                          /* service tag cfi */
    uint16 tpid;                        /* service tag tpid */
    bcm_vlan_t vlan;                    /* service VLAN ID. */
    uint32 flags;                       /* BCM_FLOW_ENCAP_FLAG_xxx. */
    uint32 options;                     /* BCM_FLOW_ENCAP_OPTION_xxx. */
    bcm_flow_encap_criteria_t criteria; /* flow encap criteria. */
    uint32 valid_elements;              /* bitmap of valid fields for the encap
                                           action */
    bcm_flow_handle_t flow_handle;      /* flow handle derived from flow name */
    uint32 flow_option;                 /* flow option derived from flow option
                                           name */
    bcm_gport_t src_flow_port;          /* flow port id representing a SVP */
    uint32 class_id;                    /* Class ID as a key in EFP. */
    uint32 port_group;                  /* Port group ID. */
} bcm_flow_encap_config_t;
#endif

/* bcm_flow_encap_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_encap_traverse_cb)(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
/* Initialize flow encap config structure. */
extern void bcm_flow_encap_config_t_init(
    bcm_flow_encap_config_t *flow_encap);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Add a flow encap entry for L2 tunnel or L3 tunnel. */
extern int bcm_flow_encap_add(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Delete a flow encap entry based on the given key. */
extern int bcm_flow_encap_delete(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Get a flow encap entry based on the given key. */
extern int bcm_flow_encap_get(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field);
#endif

#if defined(INCLUDE_L3)
/* Traverse encap entries */
extern int bcm_flow_encap_traverse(
    int unit, 
    bcm_flow_encap_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* BCM_FLOW_* flags. */
#define BCM_FLOW_PORT_WITH_ID               (1 << 0)   /* create FLOW port with
                                                          specified ID */
#define BCM_FLOW_PORT_NETWORK               (1 << 1)   /* Network facing
                                                          interface */
#define BCM_FLOW_PORT_COUNTED               (1 << 2)   /* Maintain packet/byte
                                                          counts */
#define BCM_FLOW_PORT_REPLACE               (1 << 3)   /* Replace existing entry */
#define BCM_FLOW_PORT_SERVICE_TAGGED        (1 << 4)   /* Service tag mode */
#define BCM_FLOW_PORT_DEFAULT               (1 << 5)   /* Create VXLAN Default
                                                          Network Port */
#define BCM_FLOW_PORT_DEFAULT_L2GRE         (1 << 6)   /* Create L2GRE Default
                                                          Network Port */
#define BCM_FLOW_PORT_DEFAULT_GPE           (1 << 7)   /* Create GPE Default
                                                          Network Port */
#define BCM_FLOW_PORT_DEFAULT_GENEVE        (1 << 8)   /* Create GENEVE Default
                                                          Network Port */
#define BCM_FLOW_PORT_MULTICAST_GROUP_REMAP (1 << 9)   /* Remap the multicast
                                                          group to a new one. */
#endif

/* Flow port split horizon drop mode. */
typedef enum bcm_flow_port_split_horizon_drop_e {
    bcmFlowPortSplitHorizonDropAll = 0, /* Drop both L2 and L3 packets. */
    bcmFlowPortSplitHorizonDropL2 = 1,  /* Drop L2 packets. */
    bcmFlowPortSplitHorizonDropCount = 2 /* Must be last. */
} bcm_flow_port_split_horizon_drop_t;

/* Flex Flow VP Config Structure. */
typedef struct bcm_flow_port_s {
    bcm_gport_t flow_port_id;           /* GPORT identifier. */
    uint32 flags;                       /* BCM_FLOW_PORT_xxx. */
    uint32 if_class;                    /* Interface Class ID. */
    bcm_switch_network_group_t network_group_id; /* Split Horizon network group
                                           identifier. */
    uint16 egress_service_tpid;         /* Service Vlan TPID Value. */
    int dscp_map_id;                    /* DSCP based PHB map ID. */
    int vlan_pri_map_id;                /* VLAN priority and CFI based PHB map
                                           ID. */
    bcm_flow_port_split_horizon_drop_t drop_mode; /* Drop mode when the split horizon
                                           check fails. */
    int ingress_opaque_ctrl_id;         /* Ingress opaque control ID. */
} bcm_flow_port_t;

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Initialize the BCM FOW subsystem. */
extern int bcm_flow_init(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Detach the BCM FLOW subsystem. */
extern int bcm_flow_cleanup(
    int unit);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Initialize the bcm_flow_port_t structure */
extern void bcm_flow_port_t_init(
    bcm_flow_port_t *flow_port);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* 
 * Create a flow port
 * bcm_flow_port_create creates an Access or Network FLOW port
 */
extern int bcm_flow_port_create(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_flow_port_t *flow_port);
#endif

#if defined(INCLUDE_L3)
/* bcm_flow_port_destroy FLOW port from FLOW network. */
extern int bcm_flow_port_destroy(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_gport_t flow_port);
#endif

#if defined(INCLUDE_L3)
/* Get FLOW port information. */
extern int bcm_flow_port_get(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_flow_port_t *flow_port);
#endif

#if defined(INCLUDE_L3)
/* Get all FLOW port information. */
extern int bcm_flow_port_get_all(
    int unit, 
    bcm_vpn_t vpn, 
    int port_max, 
    bcm_flow_port_t *flow_port, 
    int *port_count);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* FLOW VPN Definitions. */
#define BCM_FLOW_VPN_ELINE      0x0001     /* ELINE FLOW VPN */
#define BCM_FLOW_VPN_ELAN       0x0002     /* ELAN FLOW VPN */
#define BCM_FLOW_VPN_WITH_ID    0x0004     /* With Id FLOW VPN */
#define BCM_FLOW_VPN_REPLACE    0x0008     /* Replace VPN Id FLOW VPN */
#define BCM_FLOW_VPN_INVALID    0xFFFF     /* Invalid FLOW VPN */
#endif

/* Flow L2 VPN Config Structure. */
typedef struct bcm_flow_vpn_config_s {
    uint32 flags;                       /* L2 VPN config flags. */
    bcm_multicast_t broadcast_group;    /* broadcast group. */
    bcm_multicast_t unknown_unicast_group; /* unknown unicast group. */
    bcm_multicast_t unknown_multicast_group; /* unknown multicast group. */
    bcm_vlan_protocol_packet_ctrl_t protocol_pkt; /* protocol packet handling */
    bcm_gport_t match_port_class;       /* port class */
} bcm_flow_vpn_config_t;

/* bcm_flow_vpn_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_vpn_traverse_cb)(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_flow_vpn_config_t *info, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
/* Initialize FLOW VPN config structure. */
extern void bcm_flow_vpn_config_t_init(
    bcm_flow_vpn_config_t *info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* 
 * Create or update FLOW VPN. The FLOW VPN provides a forwarding domain
 * between FLOW ports, independent of a FLOW port's transport protocol.
 *  Use BCM_FLOW_VPN_REPLACE to update an existing FLOW_VPN.
 */
extern int bcm_flow_vpn_create(
    int unit, 
    bcm_vpn_t *vpn, 
    bcm_flow_vpn_config_t *info);
#endif

#if defined(INCLUDE_L3)
/* Destroy FLOW VPN. */
extern int bcm_flow_vpn_destroy(
    int unit, 
    bcm_vpn_t vpn);
#endif

#if defined(INCLUDE_L3)
/* Destroy FLOW VPN. */
extern int bcm_flow_vpn_destroy_all(
    int unit);
#endif

#if defined(INCLUDE_L3)
/* Get FLOW VPN. */
extern int bcm_flow_vpn_get(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_flow_vpn_config_t *info);
#endif

#if defined(INCLUDE_L3)
/* 
 * Traverse all valid FLOW VPN entries and call the supplied callback
 * routine.
 */
extern int bcm_flow_vpn_traverse(
    int unit, 
    bcm_flow_vpn_traverse_cb cb, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
/* Get a handle for a given flow name string. */
extern int bcm_flow_handle_get(
    int unit, 
    const char *flow_name, 
    bcm_flow_handle_t *handle);
#endif

#if defined(INCLUDE_L3)
/* Get a flow option ID for a given flow handle and flow option name. */
extern int bcm_flow_option_id_get(
    int unit, 
    bcm_flow_handle_t flow_handle, 
    const char *flow_option_name, 
    bcm_flow_option_id_t *option_id);
#endif

#if defined(INCLUDE_L3)
/* Get the field id for logical field name in a flow. */
extern int bcm_flow_logical_field_id_get(
    int unit, 
    bcm_flow_handle_t flow_handle, 
    const char *field_name, 
    bcm_flow_field_id_t *field_id);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow Functions. */
typedef enum bcm_flow_function_type_e {
    bcmFlowFuncTypeStart = 0,           /* Start. */
    bcmFlowFuncTypeMatch = 1,           /* Match. */
    bcmFlowFuncTypeEncap = 2,           /* Encap. */
    bcmFlowFuncTypeTunnelInit = 3,      /* Tunnel Initiator. */
    bcmFlowFuncTypeTunnelTerm = 4,      /* Tunnel Terminator. */
    bcmFlowFuncTypeEncapSet = 5,        /* Encap set. */
    bcmFlowFuncTypeL2Switch = 6,        /* L2 Switch. */
    bcmFlowFuncTypeL3Host = 7,          /* L3 Host. */
    bcmFlowFuncTypeL3Route = 8,         /* L3 Route. */
    bcmFlowFuncTypeEgressObj = 9,       /* Egress  Object. */
    bcmFlowFuncTypeEgressLabel = 10,    /* Egress Label. */
    bcmFlowFuncTypeEgressIntf = 11,     /* Egress Intf. */
    bcmFlowFuncTypeEnd = 12             /* End. */
} bcm_flow_function_type_t;

#define BCM_FLOW_FUNCTION_TYPE \
{ \
    "bcmFlowFuncTypeStart", \
    "bcmFlowFuncTypeMatch", \
    "bcmFlowFuncTypeEncap", \
    "bcmFlowFuncTypeTunnelInit", \
    "bcmFlowFuncTypeTunnelTerm", \
    "bcmFlowFuncTypeEncapSet", \
    "bcmFlowFuncTypeL2Switch", \
    "bcmFlowFuncTypeL3Host", \
    "bcmFlowFuncTypeL3Route", \
    "bcmFlowFuncTypeEgressObj", \
    "bcmFlowFuncTypeEgressLabel", \
    "bcmFlowFuncTypeEgressIntf", \
    "bcmFlowFuncTypeEnd"  \
}
#endif

#if defined(INCLUDE_L3)
/* Flow stat info valid elements. */
#define BCM_FLOW_STAT_FLOW_PORT_VALID       (1 << 0)   /* flow port element
                                                          valid. */
#define BCM_FLOW_STAT_PORT_VALID            (1 << 1)   /* port element valid. */
#define BCM_FLOW_STAT_VPN_VALID             (1 << 2)   /* VN_ID element is valid
                                                          for this match. */
#define BCM_FLOW_STAT_INNER_VLAN_VALID      (1 << 3)   /* Inner vlan element
                                                          valid. */
#define BCM_FLOW_STAT_VLAN_VALID            (1 << 4)   /* vlan element valid. */
#define BCM_FLOW_STAT_VNID_VALID            (1 << 5)   /* VNID element valid. */
#define BCM_FLOW_STAT_SIP_VALID             (1 << 6)   /* source IPv4 address
                                                          element valid. */
#define BCM_FLOW_STAT_DIP_VALID             (1 << 7)   /* Destination IPv4
                                                          address element valid. */
#define BCM_FLOW_STAT_SIP_MASK_VALID        (1 << 8)   /* source IPv4 address
                                                          mask element valid. */
#define BCM_FLOW_STAT_DIP_MASK_VALID        (1 << 9)   /* Destination IPv4
                                                          address mask element
                                                          valid. */
#define BCM_FLOW_STAT_SIP6_VALID            (1 << 10)  /* source IPv6 address
                                                          element valid. */
#define BCM_FLOW_STAT_DIP6_VALID            (1 << 11)  /* Destination IPv6
                                                          address element valid. */
#define BCM_FLOW_STAT_SIP6_MASK_VALID       (1 << 12)  /* source IPv6 address
                                                          mask element valid. */
#define BCM_FLOW_STAT_DIP6_MASK_VALID       (1 << 13)  /* Destination IPv6
                                                          address mask element
                                                          valid. */
#define BCM_FLOW_STAT_UDP_SRC_PORT_VALID    (1 << 14)  /* UDP source port
                                                          element valid. */
#define BCM_FLOW_STAT_UDP_DST_PORT_VALID    (1 << 15)  /* UDP Destination port
                                                          element valid. */
#define BCM_FLOW_STAT_PROTOCOL_VALID        (1 << 16)  /* Protocol element
                                                          valid. */
#define BCM_FLOW_STAT_TUNNEL_ID_VALID       (1 << 17)  /* Tunnel ID element
                                                          valid. */
#define BCM_FLOW_STAT_EGRESS_IF_VALID       (1 << 18)  /* Egress nexthop object
                                                          index element valid. */
#define BCM_FLOW_STAT_INTF_ID_VALID         (1 << 19)  /* L3 interface id
                                                          element valid. */
#define BCM_FLOW_STAT_MAC_VALID             (1 << 20)  /* L2 MAC address element
                                                          valid. */
#define BCM_FLOW_STAT_L3A_FLAGS_VALID       (1 << 21)  /* BCM_L3_xxx flag
                                                          definitions. */
#define BCM_FLOW_STAT_L3A_VRF_VALID         (1 << 22)  /* Virtual router
                                                          instance. */
#define BCM_FLOW_STAT_DVP_GROUP_VALID       (1 << 23)  /* DVP group ID */
#define BCM_FLOW_STAT_OIF_GROUP_VALID       (1 << 24)  /* L3 Out Interface Group
                                                          ID */
#define BCM_FLOW_STAT_MATCH_CRITERIA_VALID  (1 << 25)  /* DVP group ID */
#define BCM_FLOW_STAT_ENCAP_CRITERIA_VALID  (1 << 26)  /* L3 Out Interface Group
                                                          ID */
#define BCM_FLOW_STAT_MPLS_LABEL_VALID      (1 << 27)  /* Mpls Label is valid */
#define BCM_FLOW_STAT_FLEX_KEY_VALID        (1 << 28)  /* Flex Key element
                                                          valid. */
#endif

#if defined(INCLUDE_L3)
/* flow Stat. */
typedef enum bcm_flow_stat_e {
    bcmFlowInPackets = 1,   /* Flow InPackets */
    bcmFlowOutPackets = 2,  /* Flow OutPackets */
    bcmFlowInBytes = 3,     /* Flow InBytes */
    bcmFlowOutBytes = 4     /* Flow OutBytes */
} bcm_flow_stat_t;
#endif

#if defined(INCLUDE_L3)
/* Flow stat info structure */
typedef struct bcm_flow_stat_info_s {
    bcm_gport_t flow_port;              /* Flow port */
    bcm_gport_t port;                   /* Match port /trunk */
    bcm_vpn_t vpn;                      /* VPN representing a vfi or vrf */
    bcm_vlan_t inner_vlan;              /* Inner VLAN ID to match. */
    bcm_vlan_t vlan;                    /* VLAN ID to match. */
    uint32 vnid;                        /* VN_ID. */
    bcm_ip_t sip;                       /* source IPv4 address */
    bcm_ip_t dip;                       /* destination IPv4 address */
    bcm_ip_t sip_mask;                  /* source IPv4 address mask */
    bcm_ip_t dip_mask;                  /* destination IPv4 address mask */
    bcm_ip6_t sip6;                     /* source IPv6 address */
    bcm_ip6_t dip6;                     /* destination IPv6 address */
    bcm_ip6_t sip6_mask;                /* source IPv6 address mask */
    bcm_ip6_t dip6_mask;                /* destination IPv6 address mask */
    uint16 udp_src_port;                /* udp source port. */
    uint16 udp_dst_port;                /* udp destination port. */
    uint16 protocol;                    /* IP protocol type. */
    bcm_gport_t tunnel_id;              /* Tunnel ID */
    bcm_if_t egress_if;                 /* egress object */
    bcm_if_t intf_id;                   /* l3 interface id */
    bcm_mac_t mac;                      /* 802.3 MAC address. */
    uint32 l3a_flags;                   /* See BCM_L3_xxx flag definitions. */
    bcm_vrf_t l3a_vrf;                  /* Virtual router instance. */
    uint32 oif_group;                   /* L3 OUT interface group ID */
    uint32 dvp_group;                   /* DVP group ID */
    bcm_flow_match_criteria_t match_criteria; /* flow match criteria. */
    bcm_flow_encap_criteria_t encap_criteria; /* flow encap criteria. */
    bcm_mpls_label_t mpls_label;        /* MPLS label. */
    uint32 valid_elements;              /* bitmap of valid fields */
    uint32 flow_handle;                 /* flow handle derived from flow name */
    uint32 flow_option;                 /* flow option derived from flow option
                                           name */
    bcm_flow_function_type_t function_type; /* flow function type */
} bcm_flow_stat_info_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize flow stat info structure. */
extern void bcm_flow_stat_info_t_init(
    bcm_flow_stat_info_t *flow_stat_info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Get the stat object for the flex view of flexible table. */
extern int bcm_flow_stat_object_get(
    int unit, 
    bcm_flow_handle_t flow_handle, 
    bcm_flow_option_id_t flow_option_id, 
    bcm_flow_function_type_t function_type, 
    bcm_stat_object_t *stat_object);
#endif

#if defined(INCLUDE_L3)
/* Attach counter for an entry. */
extern int bcm_flow_stat_attach(
    int unit, 
    bcm_flow_stat_info_t *flow_stat_info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries to the given flex view table. */
extern int bcm_flow_stat_detach(
    int unit, 
    bcm_flow_stat_info_t *flow_stat_info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get Stat Counter id for flow tables. */
extern int bcm_flow_stat_id_get(
    int unit, 
    bcm_flow_stat_info_t *flow_stat_info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    bcm_stat_object_t stat_object, 
    uint32 *stat_counter_id);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flags for Flow handle info */
#define BCM_FLOW_HANDLE_INFO_WITH_ID    (1 << 0)   /* With ID */
#define BCM_FLOW_HANDLE_INFO_REPLACE    (1 << 1)   /* Replace */
#endif

#if defined(INCLUDE_L3)
#define BCM_FLOW_SPECIAL_FIELD_MAX_NOF_FIELDS 32         
#endif

#if defined(INCLUDE_L3)
/* Flow Terminator elements (for valid, clear bitmaps). */
#define BCM_FLOW_TERMINATOR_ELEMENT_VRF_VALID (1 << 0)   
#define BCM_FLOW_TERMINATOR_ELEMENT_STAT_ID_VALID (1 << 1)   
#define BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID (1 << 2)   
#define BCM_FLOW_TERMINATOR_ELEMENT_STAT_PP_PROFILE_VALID (1 << 3)   
#define BCM_FLOW_TERMINATOR_ELEMENT_QOS_MAP_ID_VALID (1 << 4)   
#define BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID (1 << 5)   
#define BCM_FLOW_TERMINATOR_ELEMENT_FLOW_SERVICE_TYPE_VALID (1 << 6)   
#define BCM_FLOW_TERMINATOR_ELEMENT_FLOW_DEST_INFO_VALID (1 << 7)   
#define BCM_FLOW_TERMINATOR_ELEMENT_ADDITIONAL_DATA_VALID (1 << 8)   
#define BCM_FLOW_TERMINATOR_ELEMENT_ACTION_GPORT_VALID (1 << 9)   
#endif

#if defined(INCLUDE_L3)
/* Flow Initiator elements (for valid, clear bitmaps). */
#define BCM_FLOW_INITIATOR_ELEMENT_STAT_ID_VALID (1 << 0)   
#define BCM_FLOW_INITIATOR_ELEMENT_STAT_PP_PROFILE_VALID (1 << 1)   
#define BCM_FLOW_INITIATOR_ELEMENT_QOS_MAP_ID_VALID (1 << 2)   
#define BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID (1 << 3)   
#define BCM_FLOW_INITIATOR_ELEMENT_ENCAP_ACCESS_VALID (1 << 4)   
#define BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID (1 << 5)   
#define BCM_FLOW_INITIATOR_ELEMENT_ACTION_GPORT_VALID (1 << 6)   
#define BCM_FLOW_INITIATOR_ELEMENT_MTU_PROFILE_VALID (1 << 7)   
#endif

#if defined(INCLUDE_L3)
/* Service type */
typedef enum bcm_flow_service_type_e {
    bcmFlowServiceTypeMultiPoint = 0, 
    bcmFlowServiceTypeCrossConnect = 1, 
    bcmFlowServiceTypeCount = 2 
} bcm_flow_service_type_t;
#endif

#if defined(INCLUDE_L3)
/* Flow handle info structure */
typedef struct bcm_flow_handle_info_s {
    uint32 flags;           /* BCM_FLOW_HADNLE_XXX flags */
    uint32 flow_handle;     /* Flow handle */
    uint32 flow_option;     /* Flow option */
    bcm_gport_t flow_id;    /* Flow Gport Identifier */
} bcm_flow_handle_info_t;
#endif

#if defined(INCLUDE_L3)
/* Flow destination info */
typedef struct bcm_flow_dest_info_s {
    bcm_gport_t dest_port; 
    bcm_if_t dest_encap; 
} bcm_flow_dest_info_t;
#endif

#if defined(INCLUDE_L3)
/* L3 ingress info */
typedef struct bcm_flow_l3_ingress_info_s {
    uint32 l3_ingress_flags;            /* BCM_L3_INGRESS_XXX */
    bcm_l3_ingress_urpf_mode_t urpf_mode; /* URPF mode setting */
} bcm_flow_l3_ingress_info_t;
#endif

#if defined(INCLUDE_L3)
/* Flow special field structure */
typedef struct bcm_flow_special_field_s {
    bcm_flow_field_id_t field_id;   /* Field id */
    uint8 is_clear;                 /* Relevant for replace, indicate field
                                       should be ignored */
    uint32 shr_var_uint32; 
    bcm_mac_t shr_var_mac_addr; 
    bcm_ip_t shr_var_ipv4; 
} bcm_flow_special_field_t;
#endif

#if defined(INCLUDE_L3)
/* Flow special fields structure */
typedef struct bcm_flow_special_fields_s {
    uint32 actual_nof_special_fields; 
    bcm_flow_special_field_t special_fields[BCM_FLOW_SPECIAL_FIELD_MAX_NOF_FIELDS]; 
} bcm_flow_special_fields_t;
#endif

#if defined(INCLUDE_L3)
/* bcm_flow_terminator_info_t header type */
typedef struct bcm_flow_terminator_info_s {
    uint32 flags;                       /* Use BCM_FLOW_TERMINATOR_INFO_XXX
                                           flags */
    bcm_flow_l3_ingress_info_t l3_ingress_info; /* L3 ingress intf info */
    bcm_vrf_t vrf;                      /* Virtual router instance */
    uint32 stat_id;                     /* Statistics ID */
    int stat_pp_profile;                /* Statistics PP Profile */
    int qos_map_id;                     /* QOS Map ID */
    bcm_qos_ingress_model_t ingress_qos_model; /* Ingress QOS and TTL model */
    bcm_flow_service_type_t service_type; /* Service type */
    bcm_flow_dest_info_t dest_info;     /* Destination info */
    uint64 additional_data;             /* Additional data for LIF */
    bcm_gport_t action_gport;           /* Hold the Trap Action-Profile */
    uint32 valid_elements_set;          /* bitmap of set fields
                                           BCM_FLOW_TERMINATOR_ELEMENT_XXX_VALID
                                           upon creation/replace */
    uint32 valid_elements_clear;        /* bitmap of clear fields
                                           BCM_FLOW_TERMINATOR_ELEMENT_XXX_VALID
                                           upon replace */
} bcm_flow_terminator_info_t;
#endif

#if defined(INCLUDE_L3)
/* bcm_flow_initiator_info_t header type */
typedef struct bcm_flow_initiator_info_s {
    uint32 flags;                       /* Use BCM_FLOW_INITIATOR_INFO_XXX flags */
    uint32 stat_id;                     /* Statistics ID */
    int stat_pp_profile;                /* Statistics PP Profile */
    int qos_map_id;                     /* QOS Map ID */
    bcm_qos_egress_model_t egress_qos_model; /* Egress QOS and TTL model */
    bcm_if_t l3_intf_id;                /* Next intf pointing following tunnel. */
    bcm_encap_access_t encap_access;    /* Encapsulation phase to be used */
    uint32 valid_elements_set;          /* Bitmap of set fields
                                           BCM_FLOW_INITIATOR_ELEMENT_XXX upon
                                           creation/replace */
    uint32 valid_elements_clear;        /* Bitmap of clear fields
                                           BCM_FLOW_INITIATOR_ELEMENT_XXX upon
                                           replace */
    bcm_gport_t action_gport;           /* Trap action gport */
    uint8 mtu_profile;                  /* Trap MTU profile */
} bcm_flow_initiator_info_t;
#endif

#if defined(INCLUDE_L3)
/* Initialize a structure that holds flow handle information. */
extern void bcm_flow_handle_info_t_init(
    bcm_flow_handle_info_t *flow_handle_info);
#endif

#if defined(INCLUDE_L3)
/* Initialize a structure that holds L3 ingress information. */
extern void bcm_flow_l3_ingress_info_t_init(
    bcm_flow_l3_ingress_info_t *flow_l3_ingress_info);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
extern int bcm_flow_terminator_info_create(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_terminator_info_t *terminator_info, 
    bcm_flow_special_fields_t *special_fields);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_terminator_info_destroy(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_terminator_info_get(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_terminator_info_t *terminator_info, 
    bcm_flow_special_fields_t *special_fields);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_flow_terminator_info_traverse_cb */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_terminator_info_traverse_cb)(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_terminator_info_t *terminator_info, 
    bcm_flow_special_fields_t *special_fields, 
    void *user_data);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
extern int bcm_flow_terminator_info_traverse(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_terminator_info_traverse_cb cb, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_initiator_info_create(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_initiator_info_t *initiator_info, 
    bcm_flow_special_fields_t *special_fields);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_initiator_info_destroy(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_initiator_info_get(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_initiator_info_t *initiator_info, 
    bcm_flow_special_fields_t *special_fields);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_flow_initiator_info_traverse_cb */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_initiator_info_traverse_cb)(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_initiator_info_t *initiator_info, 
    bcm_flow_special_fields_t *special_fields, 
    void *user_data);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
extern int bcm_flow_initiator_info_traverse(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_initiator_info_traverse_cb cb, 
    void *user_data);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_match_info_add(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_special_fields_t *key_special_fields);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_match_info_delete(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_special_fields_t *key_special_fields);
#endif

#if defined(INCLUDE_L3)
extern int bcm_flow_match_info_get(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_special_fields_t *key_special_fields);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_flow_match_info_traverse callback */
#if defined(INCLUDE_L3)
typedef int (*bcm_flow_match_info_traverse_cb)(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_special_fields_t *key_special_fields, 
    void *user_data);
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
extern int bcm_flow_match_info_traverse(
    int unit, 
    bcm_flow_handle_info_t *flow_handle_info, 
    bcm_flow_match_info_traverse_cb cb, 
    void *user_data);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#if defined(INCLUDE_L3)
/* Flow port ethernet filtering control. */
#define BCM_FLOW_ES_FILTER_DROP_ENABLE  (1 << 0)   /* Enable/Disable ethernet
                                                      segment filtering */
#endif

#ifndef BCM_HIDE_DISPATCHABLE

#if defined(INCLUDE_L3)
/* Specify the filter control for the given ethernet segment identifier. */
extern int bcm_flow_es_filter_set(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 es_id, 
    uint32 flags);
#endif

#if defined(INCLUDE_L3)
/* Get the filter status for the given ethernet segment identifier. */
extern int bcm_flow_es_filter_get(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 es_id, 
    uint32 *flags);
#endif

#if defined(INCLUDE_L3)
/* Set the flex counter object value to the ethernet segment filter. */
extern int bcm_flow_es_filter_flexctr_object_set(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 value);
#endif

#if defined(INCLUDE_L3)
/* Get the flex counter object value from the ethernet segment filter. */
extern int bcm_flow_es_filter_flexctr_object_get(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 *value);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the ethernet segment filter. */
extern int bcm_flow_es_filter_stat_attach(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the ethernet segment filter. */
extern int bcm_flow_es_filter_stat_detach(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter id associated from ethernet segment filter. */
extern int bcm_flow_es_filter_stat_id_get(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the given flow VPN. */
extern int bcm_flow_vpn_stat_attach(
    int unit, 
    bcm_vpn_t vpn, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the given flow VPN. */
extern int bcm_flow_vpn_stat_detach(
    int unit, 
    bcm_vpn_t vpn, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the given flow VPN. */
extern int bcm_flow_vpn_stat_id_get(
    int unit, 
    bcm_vpn_t vpn, 
    bcm_flexctr_direction_t direction, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the given flow port. */
extern int bcm_flow_port_stat_attach(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the given flow port. */
extern int bcm_flow_port_stat_detach(
    int unit, 
    bcm_gport_t flow_port_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the given flow port. */
extern int bcm_flow_port_stat_id_get(
    int unit, 
    bcm_gport_t flow_port_id, 
    bcm_flexctr_direction_t direction, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Set the flex counter object value to the given match entry. */
extern int bcm_flow_match_flexctr_object_set(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 value);
#endif

#if defined(INCLUDE_L3)
/* Get the flex counter object value from the given match entry. */
extern int bcm_flow_match_flexctr_object_get(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 *value);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the given match entry. */
extern int bcm_flow_match_stat_attach(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the given match entry. */
extern int bcm_flow_match_stat_detach(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the given match entry. */
extern int bcm_flow_match_stat_id_get(
    int unit, 
    bcm_flow_match_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Set the flex counter object value to the encap entry. */
extern int bcm_flow_encap_flexctr_object_set(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 value);
#endif

#if defined(INCLUDE_L3)
/* Get the flex counter object value from the encap entry. */
extern int bcm_flow_encap_flexctr_object_get(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 *value);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the encap entry. */
extern int bcm_flow_encap_stat_attach(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach the counter entries from the encap entry. */
extern int bcm_flow_encap_stat_detach(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the encap entry. */
extern int bcm_flow_encap_stat_id_get(
    int unit, 
    bcm_flow_encap_config_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the flow tunnel initiator. */
extern int bcm_flow_tunnel_initiator_stat_attach(
    int unit, 
    bcm_gport_t tunnel_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the flow tunnel initiator. */
extern int bcm_flow_tunnel_initiator_stat_detach(
    int unit, 
    bcm_gport_t tunnel_id, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the flow tunnel initiator. */
extern int bcm_flow_tunnel_initiator_stat_id_get(
    int unit, 
    bcm_gport_t tunnel_id, 
    uint32 *stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* 
 * Set the flex counter object value to the flow tunnel terminator
 * object.
 */
extern int bcm_flow_tunnel_terminator_flexctr_object_set(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 value);
#endif

#if defined(INCLUDE_L3)
/* 
 * Get the flex counter object value from the flow tunnel terminator
 * object.
 */
extern int bcm_flow_tunnel_terminator_flexctr_object_get(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 *value);
#endif

#if defined(INCLUDE_L3)
/* Attach counter entries to the flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_stat_attach(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Detach counter entries from the flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_stat_detach(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 stat_counter_id);
#endif

#if defined(INCLUDE_L3)
/* Get stat counter ID associated to the flow tunnel terminator object. */
extern int bcm_flow_tunnel_terminator_stat_id_get(
    int unit, 
    bcm_flow_tunnel_terminator_t *info, 
    uint32 num_of_fields, 
    bcm_flow_logical_field_t *field, 
    uint32 *stat_counter_id);
#endif

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_FLOW_H__ */
