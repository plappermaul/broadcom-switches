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
#ifndef UDF_HA_H
#define UDF_HA_H

#include <shr/shr_ha.h>

#define BCMINT_UDF_HA_UDF_CHUNKS_MAX 16
#define BCMINT_UDF_HA_UDF_ID_LIST_MAX 512

typedef struct {
   /*! UDF Object Identifier */
   int udf_id;
   /*! UDF base offset. bcmUdfLayerXxx */
   int layer;
   /*! Relative offset from 'layer' in the packet (in bytes) */
   uint8_t start;
   /*! Width of data to extract (in bytes) */
   uint8_t width;
   /*! UDF object flags */
   uint32_t flags;
   /*! Base UDF chunk Id */
   uint8_t grp_id;
   /*! Bitmap in bytes for used chunks */
   uint32_t byte_bmap;
   /*! Number of packet format associated with the UDF */
   uint16_t num_pkt_formats;
   /*! Pipe number for UDF */
   int pipe;
} bcmint_udf_offset_info_t;

#define BCMINT_UDF_OFFSET_INFO_T_ID 0x5425ee76734f1106

typedef struct {
   /*! UDF packt format Object Identifier */
   int pkt_fmt_id;
   /*! UDF packet format object flags */
   uint16_t flags;
   /*! Priority of the UDF */
   uint16_t priority;
   /*! Ethertype in the packet */
   uint16_t ethertype;
   /*! Ethertype Mask */
   uint16_t ethertype_mask;
   /*! IP Protocol in the packet */
   uint8_t ip_protocol;
   /*! IP Protocol Mask */
   uint8_t ip_protocol_mask;
   /*! L2 packet format (BCM_PKT_FORMAT_L2_XXX) */
   uint8_t l2;
   /*! Vlan tag format (BCM_PKT_FORMAT_VLAN_XXX) */
   uint8_t vlan_tag;
   /*! Outer IP header type (BCM_PKT_FORMAT_IP_XXX) */
   uint8_t outer_ip;
   /*! Inner IP header type (BCM_PKT_FORMAT_IP_XXX) */
   uint8_t inner_ip;
   /*! Tunnel type (BCM_PKT_FORMAT_TUNNEL_XXX) */
   uint8_t tunnel;
   /*! Mpls labels (BCM_PKT_FORMAT_MPLS_XXX) */
   uint8_t mpls;
   /*! Boolean Key fields */
   uint32_t misc_en_fields;
   /*! Inner protocol field in the packet */
   uint8_t inner_protocol;
   /*! Inner protocol mask */
   uint8_t inner_protocol_mask;
   /*! TCP or UDP Destination Port number */
   uint16_t l4_dst_port;
   /*! TCP or UDP Destination Port number mask */
   uint16_t l4_dst_port_mask;
   /*! Tag type field in the opaque tag */
   uint8_t opaque_tag_type;
   /*! Mask for tag type field in the opaque tag */
   uint8_t opaque_tag_type_mask;
   /*! Logical Source Port number */
   uint16_t src_port;
   /*! Logical Source Port number mask */
   uint16_t src_port_mask;
   /*! First 2 bytes after BOS in an mpls packet */
   uint16_t first_2B_after_mpls_bos;
   /*! Mask for First 2 bytes after BOS in an mpls packet */
   uint16_t first_2B_after_mpls_bos_mask;
   /*! Class Id associated with UDF packet format */
   uint16_t class_id;
   /*! Number of UDF objects associated per pipe */
   uint16_t num_udfs[BCMINT_UDF_HA_UDF_CHUNKS_MAX];
   /*! List of UDF objects associated */
   int udf_id_list[BCMINT_UDF_HA_UDF_ID_LIST_MAX];
} bcmint_udf_pkt_format_info_t;

#define BCMINT_UDF_PKT_FORMAT_INFO_T_ID 0x4c2be0e754c3c7b3

#endif /* UDF_HA_H */
