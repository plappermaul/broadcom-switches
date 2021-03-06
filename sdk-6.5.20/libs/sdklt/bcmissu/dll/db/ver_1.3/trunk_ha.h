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
#ifndef TRUNK_HA_H
#define TRUNK_HA_H

#include <shr/shr_ha.h>

typedef struct {
   /*! Indicate that this entry was set or not. */
   uint8_t inserted;
   /*! Loading balancing mode. */
   uint8_t lb_mode;
   /*! Hash rule tag for unicast packets. */
   uint8_t uc_rtag;
   /*! Assumed value of max members. */
   uint16_t uc_max_members;
   /*! Base ptr of group in TRUNK_MEMBER table. */
   uint32_t uc_base_ptr;
   /*! Acutal count of unicast members. */
   uint16_t uc_cnt;
   /*! Pointer to uc modid array. */
   uint8_t uc_modid[256];
   /*! Pointer to uc modport array. */
   uint16_t uc_modport[256];
   /*! Aggregation group monitor pool. */
   uint8_t uc_agm_pool;
   /*! Aggregation group monitor ID. */
   uint8_t uc_agm_id;
   /*! Actual members cnt for nonuc packets. */
   uint16_t nonuc_cnt;
   /*! Pointer to nonuc modid array. */
   uint8_t nonuc_modid[256];
   /*! Pointer to nonuc modport array. */
   uint16_t nonuc_modport[256];
} bcmcth_trunk_group_t;

#define BCMCTH_TRUNK_GROUP_T_ID 0x39eed4f64ea5a993

typedef struct {
   /*! Indicate that this entry was set or not. */
   uint8_t inserted;
   /*! Loading balancing mode. */
   uint8_t lb_mode;
   /*! Hash rule tag for unicast packets. */
   uint8_t uc_rtag;
   /*! Acutal count of unicast members. */
   uint8_t uc_cnt;
   /*! Pointer to uc modid array. */
   uint8_t uc_modid[64];
   /*! Pointer to uc modport array. */
   uint16_t uc_modport[64];
   /*! Aggregation group monitor pool. */
   uint8_t uc_agm_pool;
   /*! Aggregation group monitor ID. */
   uint8_t uc_agm_id;
   /*! Actual members cnt for nonuc packets. */
   uint8_t nonuc_cnt;
   /*! Pointer to nonuc modid array. */
   uint8_t nonuc_modid[64];
   /*! Pointer to nonuc modport array. */
   uint16_t nonuc_modport[64];
   /*! Select one enhanced flex counter action from TRUNK_CTR_ING_EFLEX_ACTION logical table. */
   uint8_t ing_eflex_action_id;
   /*! Enhanced flex counter object. */
   uint16_t ing_eflex_object;
} bcmcth_trunk_fast_group_t;

#define BCMCTH_TRUNK_FAST_GROUP_T_ID 0x2d6ea19b4f3cb1c5

typedef struct {
   /*! Indicate that this entry was set or not. */
   uint8_t set;
   /*! Rule tag for failover port selection. */
   uint8_t rtag;
   /*! Failover ports count. */
   uint8_t failover_cnt;
   /*! Pointer to failover modid array. */
   uint8_t failover_modid[8];
   /*! Pointer to failover modport array. */
   uint16_t failover_modport[8];
} bcmcth_trunk_failover_t;

#define BCMCTH_TRUNK_FAILOVER_T_ID 0xde358265ae77b5c1

typedef struct {
   /*! Indicate that this entry was set or not. */
   bool inserted;
   /*! Assumed value of max members. */
   uint16_t uc_max_members;
   /*! Acutal count of unicast members. */
   uint16_t uc_cnt;
   /*! Pointer to unicast system port array. */
   uint16_t uc_system_port[128];
   /*! Pointer to l2 EIF interface array. */
   uint16_t l2_eif[128];
   /*! Egress disable flag for unicast system ports. */
   uint32_t uc_egr_block[4];
   /*! Acutal count of nonunicast system port members. */
   uint16_t nonuc_cnt;
   /*! Pointer to nonunicast system port array. */
   uint16_t nonuc_system_port[128];
   /*! Egress disable flag for non-unicast system ports. */
   uint32_t nonuc_egr_block[4];
   /*! Base ptr of group in trunk group member table. */
   uint32_t base_ptr;
} bcmcth_trunk_imm_group_t;

#define BCMCTH_TRUNK_IMM_GROUP_T_ID 0x3798ffbf7adaa627

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! indicate the array size of group_array. */
   uint32_t array_size;
   /*! trunk group array. */
   bcmcth_trunk_imm_group_t group_array[];
} bcmcth_trunk_imm_group_ha_blk_t;

#define BCMCTH_TRUNK_IMM_GROUP_HA_BLK_T_ID 0x9d31c95a6b81cd94

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! indicate the array size of group bitmap mbmp. */
   uint32_t array_size;
   /*! trunk group bitmap. */
   uint32_t mbmp[];
} bcmcth_trunk_imm_group_mbmp_ha_blk_t;

#define BCMCTH_TRUNK_IMM_GROUP_MBMP_HA_BLK_T_ID 0xc63d332daa24c25a

typedef struct {
   /*! Indicate that this entry was set or not. */
   bool inserted;
   /*! Base ptr of group in system trunk group member table. */
   uint32_t base_ptr;
   /*! Assumed value of max members. */
   uint16_t max_members;
   /*! Acutal count of members. */
   uint16_t cnt;
} bcmcth_trunk_imm_sys_group_t;

#define BCMCTH_TRUNK_IMM_SYS_GROUP_T_ID 0xd39b5bc7b0488f28

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! Indicate the array size of sys_grp_array. */
   uint32_t array_size;
   /*! System trunk group array. */
   bcmcth_trunk_imm_sys_group_t sys_grp_array[];
} bcmcth_trunk_imm_sys_group_ha_blk_t;

#define BCMCTH_TRUNK_IMM_SYS_GROUP_HA_BLK_T_ID 0x6acd964079c51f44

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! Indicate the array size of system group bitmap mbmp. */
   uint32_t array_size;
   /*! System trunk group bitmap. */
   uint32_t mbmp[];
} bcmcth_trunk_imm_sys_group_mbmp_ha_blk_t;

#define BCMCTH_TRUNK_IMM_SYS_GROUP_MBMP_HA_BLK_T_ID 0x958423b3ac45415d

typedef struct {
   /*! Entry property flags value. */
   uint8_t flags;
} bcmcth_trunk_vp_hw_entry_attr_t;

#define BCMCTH_TRUNK_VP_HW_ENTRY_ATTR_T_ID 0xe15fec9889367b52

typedef struct {
   /*! Transaction identifer of this group. */
   uint32_t trans_id;
   /*! Trunk VP group lookup type. */
   uint32_t grp_type;
   /*! Logical table identifier. */
   uint32_t glt_sid;
   /*! Loading balancing mode. */
   uint32_t lb_mode;
   /*! Maximum paths supported. */
   uint32_t max_members;
   /*! Number of paths configured. */
   uint32_t member_cnt;
   /*! Group members start index in trunk VP member table. */
   int mstart_idx;
   /*! Flag indicating if the group is using itbm resource. */
   bool in_use_itbm;
} bcmcth_trunk_vp_grp_attr_t;

#define BCMCTH_TRUNK_VP_GRP_ATTR_T_ID 0xdfbe65178ad5a06c

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! How many hw entries need be allocated. */
   uint32_t array_size;
   /*! HW entries array. */
   bcmcth_trunk_vp_hw_entry_attr_t array[];
} bcmcth_trunk_vp_hw_entry_ha_blk_t;

#define BCMCTH_TRUNK_VP_HW_ENTRY_HA_BLK_T_ID 0x9dce628f881ad67a

typedef struct {
   /*! Block validation. */
   uint32_t signature;
   /*! How many group entries need be allocated. */
   uint32_t array_size;
   /*! Group entries array. */
   bcmcth_trunk_vp_grp_attr_t array[];
} bcmcth_trunk_vp_grp_ha_blk_t;

#define BCMCTH_TRUNK_VP_GRP_HA_BLK_T_ID 0xa0705844937913fc

#endif /* TRUNK_HA_H */
