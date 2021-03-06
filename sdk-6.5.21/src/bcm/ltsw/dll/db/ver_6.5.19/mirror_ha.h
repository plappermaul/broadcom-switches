/**************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by HA parser from YAML formated file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: bcmha/scripts/ha_yml_parser.py
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef MIRROR_HA_H
#define MIRROR_HA_H

#include <shr/shr_ha.h>

typedef struct {
   /*! Reference count. */
   int ref_cnt;
   /*! For egress or ingress Mirroring. */
   bool egr;
   /*! For CoSQ MOD Mirroring. */
   bool cosq_mod;
   /*! Encap id. */
   int encap_id;
   /*! Associated Mirror destination id. */
   int mirror_dest_id;
   /*! Associated multiple destination IP group id. */
   int multi_dip_group_id;
} bcmint_mirror_session_cfg_t;

#define BCMINT_MIRROR_SESSION_CFG_T_ID 0x98d74f758b033951

typedef struct {
   /*! Reference count. */
   int ref_cnt;
   /*! Current source types on the Mirror container. */
   uint32_t source;
   /*! For egress or ingress Mirroring. */
   bool egr;
} bcmint_mirror_container_cfg_t;

#define BCMINT_MIRROR_CONTAINER_CFG_T_ID 0x38c0f295ab9e5f40

typedef struct {
   /*! Reference count. */
   int ref_cnt;
   /*! Event group id. */
   int group_id;
} bcmint_mirror_ingress_mod_event_group_cfg_t;

#define BCMINT_MIRROR_INGRESS_MOD_EVENT_GROUP_CFG_T_ID 0xd89393788874e680

typedef struct {
   /*! Event profile id. */
   int profile_id;
   /*! Configurable reason code for ingress mirror-on-drop packets. */
   uint8_t reason_code;
   /*! Sampler threshold. */
   uint32_t sample_rate;
   /*! Profile priority. Higher value means higher priority. */
   uint32_t priority;
   /*! Event group bitmap */
   uint32_t group_bmp;
   /*! Event group mask bitmap */
   uint32_t group_mask_bmp;
} bcmint_mirror_ingress_mod_event_profile_cfg_t;

#define BCMINT_MIRROR_INGRESS_MOD_EVENT_PROFILE_CFG_T_ID 0x6659137abc9157b2

typedef struct {
   /*! Reference count. */
   int ref_cnt;
   /*! Multi DIP group id. */
   int group_id;
   /*! Size of the group. */
   int group_size;
   /*! Offset to LT entry. */
   int offset;
   /*! IPv6 group. */
   bool ipv6;
   /*! IPv4 index in a table entry. An entry can contain two IPv4 addresses. */
   int ipv4_index;
} bcmint_mirror_multi_dip_group_cfg_t;

#define BCMINT_MIRROR_MULTI_DIP_GROUP_CFG_T_ID 0xe0aef714f7b6d007

#endif /* MIRROR_HA_H */
