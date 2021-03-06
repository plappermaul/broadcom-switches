/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_IPSEC_H__
#define __BCM_IPSEC_H__

#include <bcm/types.h>

typedef struct bcm_ipsec_config_s {
    uint32 num_of_ipsec_channels; 
    uint32 num_of_bfd_channels; 
} bcm_ipsec_config_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* configure the IPSEC feature. */
extern int bcm_ipsec_config_set(
    int unit, 
    bcm_ipsec_config_t *config);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef struct bcm_ipsec_tunnel_info_s {
    uint32 context_id; 
    bcm_ipv4_ipv6_addr_t local_ip;  /* Local tunnel endpoint; IPV4 or IPV6 */
    bcm_ipv4_ipv6_addr_t remote_ip; /* Remote tunnel endpoint; IPV4 or IPV6 */
    uint32 protocol;                /* Tunnel protocol; IPV4 or IPV6 */
    uint8 ttl; 
    uint8 dscp; 
    bcm_port_t port;                /* Port id will determine the lif port for
                                       ipsec output packets */
} bcm_ipsec_tunnel_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Add IPSEC tunnel */
extern int bcm_ipsec_tunnel_create(
    int unit, 
    uint32 flags, 
    uint32 *tunnel_id, 
    bcm_ipsec_tunnel_info_t *tunnel_info);

/* Delete IPSEC tunnel */
extern int bcm_ipsec_tunnel_delete(
    int unit, 
    uint32 flags, 
    uint32 tunnel_id);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef enum bcm_ipsec_algo_type_e {
    bcmEncNone = 0, 
    bcmAES128 = 1, 
    bcmAES256 = 2 
} bcm_ipsec_algo_type_t;

typedef enum bcm_ipsec_auth_type_e {
    bcmAuthNone = 0, 
    bcmSHA1 = 1, 
    bcmSHA256 = 2 
} bcm_ipsec_auth_type_t;

typedef enum bcm_ipsec_direction_e {
    bcmENCRYPTION = 0, 
    bcmDECRYPTION = 1 
} bcm_ipsec_direction_t;

typedef struct bcm_ipsec_sa_info_s {
    uint32 initial_seq_num; 
    uint32 mtu; 
    uint8 fragmentation_enable; 
    bcm_ipsec_algo_type_t encrypt_type; 
    bcm_ipsec_auth_type_t auth_type; 
    bcm_ipsec_direction_t sa_direction; 
    uint32 spi; 
} bcm_ipsec_sa_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Add IPSEC SA */
extern int bcm_ipsec_sa_create(
    int unit, 
    uint32 flags, 
    uint32 *sa_id, 
    bcm_ipsec_sa_info_t *sa);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_IPSEC_MAX_ENCR_KEY_LEN  256        

#define BCM_IPSEC_MAX_AUTH_KEY_LEN  256        

#define BCM_IPSEC_ENCR_KEY_MAX_NUM_WORDS    8          
#define BCM_IPSEC_AUTH_KEY_MAX_NUM_WORDS    8          

#define BCM_IPSEC_SA_CREATE_WITH_ID 1 << 0     /* IPSEC SA create API optional
                                                  flags. */

#define BCM_IPSEC_TUNNEL_CREATE_WITH_ID 1 << 0     /* IPSEC TUNNEL create API
                                                      optional flags. */

typedef struct bcm_ipsec_sa_keys_s {
    uint32 enc_key[BCM_IPSEC_ENCR_KEY_MAX_NUM_WORDS]; 
    uint32 auth_key[BCM_IPSEC_AUTH_KEY_MAX_NUM_WORDS]; 
} bcm_ipsec_sa_keys_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Add IPSEC SA Keys */
extern int bcm_ipsec_sa_key_update(
    int unit, 
    uint32 flags, 
    uint32 sa_id, 
    bcm_ipsec_sa_keys_t *sa_keys);

/* Delete IPSEC SA */
extern int bcm_ipsec_sa_delete(
    int unit, 
    uint32 flags, 
    uint32 sa_id);

/* Map IPSEC SA to tunnel */
extern int bcm_ipsec_sa_to_tunnel_map_set(
    int unit, 
    uint32 tunnel_id, 
    uint32 sa_id);

/* Map IPSEC SA to tunnel */
extern int bcm_ipsec_sa_to_tunnel_map_get(
    int unit, 
    uint32 sa_id, 
    uint32 *tunnel_id);

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_IPSEC_H__ */
