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
#ifndef TNL_HA_H
#define TNL_HA_H

#include <shr/shr_ha.h>

typedef struct {
   /*! indicate the array size of base_tnl_bitmap. */
   uint32_t array_size;
   /*! base tunnel bitmap array. */
   uint32_t base_tnl_bitmap[];
} bcmcth_tnl_base_tnl_state_t;

#define BCMCTH_TNL_BASE_TNL_STATE_T_ID 0x881a626eed7bf8ac

typedef struct {
   /*! indicate the array size of mpls_tnl_bitmap. */
   uint32_t array_size;
   /*! mpls tunnel bitmap array. */
   uint32_t mpls_tnl_bitmap[];
} bcmcth_tnl_mpls_tnl_state_t;

#define BCMCTH_TNL_MPLS_TNL_STATE_T_ID 0xb456e6de748c2da7

typedef struct {
   /*! indicate the array size of base_tnl_type. */
   uint32_t array_size;
   /*! base tunnel type array. */
   uint8_t base_tnl_type[];
} bcmcth_tnl_base_tnl_type_t;

#define BCMCTH_TNL_BASE_TNL_TYPE_T_ID 0x7bff52c04fef30a4

typedef struct {
   /*! indicate tunnel interface type. */
   uint8_t type;
   /*! IPv4 tunnel index of tunnel interface. */
   uint32_t tnl_ipv4_id;
   /*! IPv6 tunnel index of tunnel interface. */
   uint32_t tnl_ipv6_id;
   /*! MPLS tunnel index of tunnel interface. */
   uint32_t tnl_mpls_id;
} bcmcth_tnl_l3_intf_t;

#define BCMCTH_TNL_L3_INTF_T_ID 0x921bd2375c507d94

typedef struct {
   /*! indicate the array size of l3_intf_array. */
   uint32_t array_size;
   /*! tunnel L3 interface array. */
   bcmcth_tnl_l3_intf_t l3_intf_array[];
} bcmcth_tnl_l3_intf_info_t;

#define BCMCTH_TNL_L3_INTF_INFO_T_ID 0xa09fc35a6273cd08

#endif /* TNL_HA_H */
