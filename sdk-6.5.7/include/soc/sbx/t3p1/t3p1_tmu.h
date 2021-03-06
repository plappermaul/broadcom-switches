
/*
 * $Id: tmu_h.stg,v 1.28 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * t3p1_tmu.h: Guadalupe2k V1.3 TMU table manager & wrappers
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from sdk/caladan_ucode/t3p1/src/t3p1_tmu_cfg.lrp.
 * Edits to this file will be lost when it is regenerated.
 *
 */
 
#ifndef _SOC_SBX_T3P1_TMU_H
#define _SOC_SBX_T3P1_TMU_H

#if defined(BCM_CALADAN3_SUPPORT) && defined(BCM_CALADAN3_T3P1_SUPPORT) 
#include <soc/sbx/t3p1/t3p1_defs.h>
#include <soc/sbx/caladan3/tmu/tmu.h>


typedef enum soc_sbx_t3p1_tmu_table_id_s {
    SOC_SBX_T3P1_TMU_MAC_TABLE_ID  = 0,
    SOC_SBX_T3P1_TMU_AGE2MAC_TABLE_ID  = 1,
    SOC_SBX_T3P1_TMU_PVV2E_TABLE_ID  = 2,
    SOC_SBX_T3P1_TMU_EMLFILTER_TABLE_ID  = 3,
    SOC_SBX_T3P1_TMU_SAMPLE_DM1_TABLE_ID  = 4,
SOC_SBX_T3P1_TMU_MAX_TABLE_ID
} soc_sbx_t3p1_tmu_table_id_t;


/*
 * TMU Table type.
 */
typedef enum soc_sbx_t3p1_tmu_table_type_s {
    SOC_SBX_T3P1_TMU_DM_TABLE_TYPE  = 0,
    SOC_SBX_T3P1_TMU_HASH_TABLE_TYPE = 1,
    SOC_SBX_T3P1_TMU_TAPS_TABLE_TYPE = 2,
    SOC_SBX_T3P1_TMU_MAX_TABLE_TYPE
} soc_sbx_t3p1_tmu_table_type_t;

typedef struct soc_sbx_t3p1_tmu_table_desc_s {
    char *name;
    soc_sbx_t3p1_tmu_table_type_t type;
    
    /* Used only for DM tables */
    int entry_size_bits;
    int num_entries;
    int entry_port;
    
    void *handle;
    
    /*Used only for taps tables */
    taps_handle_t taps;
} soc_sbx_t3p1_tmu_table_desc_t;


typedef struct soc_sbx_t3p1_tmu_table_manager_s {
    int behost;    
    soc_sbx_t3p1_tmu_table_desc_t *tables;
    soc_sbx_t3p1_index_desc_t *indexes;
    soc_sbx_t3p1_entry_desc_t *entries;
    soc_sbx_caladan3_tmu_program_info_t *prog_info;       
} soc_sbx_t3p1_tmu_table_manager_t;

extern int soc_sbx_t3p1_tmu_init(int unit);
extern int soc_sbx_t3p1_tmu_uninit(int unit);

typedef enum soc_sbx_t3p1_tmu_prog_id_s {
     SOC_SBX_T3P1_TMU_MAC_PROG_ID ,
     SOC_SBX_T3P1_TMU_AGE2MAC_PROG_ID ,
     SOC_SBX_T3P1_TMU_PVV2E_PROG_ID ,
     SOC_SBX_T3P1_TMU_EGR_SMAC_PROG_ID ,
     SOC_SBX_T3P1_TMU_EMLFILTER_PROG_ID ,
    SOC_SBX_T3P1_TMU_MAX_PROG_ID
} soc_sbx_t3p1_tmu_prog_id_t;
    


typedef struct soc_sbx_t3p1_mac_s {
      /* RESERVED: uint32 not_used; */
      /* UCODE: uint32 code */
      /* UCODE: uint32 e */
      /* RESERVED: uint32 reserved; */
      uint32 ageid;
      uint32 smac_hit;
      uint32 sdrop;
      uint32 dontage;
      uint32 sreserved;
      uint32 pending;
      uint32 poe;
      uint32 dmac_hit;
      uint32 dcopy;
      uint32 ddrop;
      uint32 dreserved;
      uint32 ftidx;
} soc_sbx_t3p1_mac_t;


extern void soc_sbx_t3p1_mac_t_init(soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_add(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_set(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_get(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_hw_get(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_update(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_mac_t *e);
extern int soc_sbx_t3p1_mac_delete(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi);
extern int soc_sbx_t3p1_mac_remove(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi);
extern int soc_sbx_t3p1_mac_first(int unit, soc_sbx_t3p1_6_byte_t nmac, int *nvsi);
extern int soc_sbx_t3p1_mac_next(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, soc_sbx_t3p1_6_byte_t nmac, int *nvsi);
extern int soc_sbx_t3p1_mac_data_add(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_mac_data_set(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_mac_data_get(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_mac_data_update(int unit, soc_sbx_t3p1_6_byte_t mac, int vsi, unsigned char *buf, unsigned char size);

typedef struct soc_sbx_t3p1_age2mac_s {
      /* RESERVED: uint32 not_used; */
      /* UCODE: uint32 code */
      /* UCODE: uint32 e */
      /* RESERVED: uint32 reserved; */
      uint32 reserved1;
      uint32 smac2;
      uint32 vsi;
      uint32 smac1;
} soc_sbx_t3p1_age2mac_t;


extern void soc_sbx_t3p1_age2mac_t_init(soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_add(int unit, int ageid, soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_set(int unit, int ageid, soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_get(int unit, int ageid, soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_hw_get(int unit, int ageid, soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_update(int unit, int ageid, soc_sbx_t3p1_age2mac_t *e);
extern int soc_sbx_t3p1_age2mac_delete(int unit, int ageid);
extern int soc_sbx_t3p1_age2mac_remove(int unit, int ageid);
extern int soc_sbx_t3p1_age2mac_first(int unit, int *nageid);
extern int soc_sbx_t3p1_age2mac_next(int unit, int ageid, int *nageid);
extern int soc_sbx_t3p1_age2mac_data_add(int unit, int ageid, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_age2mac_data_set(int unit, int ageid, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_age2mac_data_get(int unit, int ageid, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_age2mac_data_update(int unit, int ageid, unsigned char *buf, unsigned char size);

typedef struct soc_sbx_t3p1_pvv2e_s {
      /* RESERVED: uint32 reserved_1; */
      uint32 pvv2e_idx;
} soc_sbx_t3p1_pvv2e_t;


extern void soc_sbx_t3p1_pvv2e_t_init(soc_sbx_t3p1_pvv2e_t *e);
extern int soc_sbx_t3p1_pvv2e_set(int unit, int vrf, int port, int ovid, int ivid, int inetmask_num_bits, soc_sbx_t3p1_pvv2e_t *e);
extern int soc_sbx_t3p1_pvv2e_get(int unit, int vrf, int port, int ovid, int ivid, int inetmask_num_bits, soc_sbx_t3p1_pvv2e_t *e);
extern int soc_sbx_t3p1_pvv2e_lpm_raw_get(int unit, int vrf, int port, int ovid, int ivid, int *nvrf, int *nport, int *novid, int *nivid, int *onetmask_num_bits, unsigned char *obuf, int *osize);
extern int soc_sbx_t3p1_pvv2e_lpm_get(int unit, int vrf, int port, int ovid, int ivid, int *nvrf, int *nport, int *novid, int *nivid, int *onetmask_num_bits, soc_sbx_t3p1_pvv2e_t *e);
extern int soc_sbx_t3p1_pvv2e_table_size_get(int unit);
extern int soc_sbx_t3p1_pvv2e_table_size_set(int unit, int size);
extern int soc_sbx_t3p1_pvv2e_delete(int unit, int vrf, int port, int ovid, int ivid, int netmask_num_bits);
extern int soc_sbx_t3p1_pvv2e_remove(int unit, int vrf, int port, int ovid, int ivid, int netmask_num_bits);
extern int soc_sbx_t3p1_pvv2e_first(int unit, int *nvrf, int *nport, int *novid, int *nivid, int *ninetmask_num_bits);
extern int soc_sbx_t3p1_pvv2e_next(int unit, int vrf, int port, int ovid, int ivid, int inetmask_num_bits, int *nvrf, int *nport, int *novid, int *nivid, int *ninetmask_num_bits);
extern int soc_sbx_t3p1_pvv2e_raw_set(int unit, uint32 * key, int key_length, uint32 *payload);
extern int soc_sbx_t3p1_pvv2e_data_set(int unit, int vrf, int port, int ovid, int ivid, int inetmask_num_bits, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_pvv2e_data_get(int unit, int vrf, int port, int ovid, int ivid, int inetmask_num_bits, unsigned char *buf, unsigned char size);

typedef struct soc_sbx_t3p1_emlfilter_s {
      /* RESERVED: uint32 not_used; */
      /* UCODE: uint32 code */
      /* UCODE: uint32 e */
      uint32 reserved_0;
      uint32 reserved_1;
      uint32 reserved_2;
      uint32 reserved_3;
} soc_sbx_t3p1_emlfilter_t;


extern void soc_sbx_t3p1_emlfilter_t_init(soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_add(int unit, int emlindex, soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_set(int unit, int emlindex, soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_get(int unit, int emlindex, soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_hw_get(int unit, int emlindex, soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_update(int unit, int emlindex, soc_sbx_t3p1_emlfilter_t *e);
extern int soc_sbx_t3p1_emlfilter_delete(int unit, int emlindex);
extern int soc_sbx_t3p1_emlfilter_remove(int unit, int emlindex);
extern int soc_sbx_t3p1_emlfilter_first(int unit, int *nemlindex);
extern int soc_sbx_t3p1_emlfilter_next(int unit, int emlindex, int *nemlindex);
extern int soc_sbx_t3p1_emlfilter_data_add(int unit, int emlindex, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_emlfilter_data_set(int unit, int emlindex, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_emlfilter_data_get(int unit, int emlindex, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_emlfilter_data_update(int unit, int emlindex, unsigned char *buf, unsigned char size);

typedef struct soc_sbx_t3p1_sample_dm1_s {
      uint32 w0;
      uint32 w1;
      uint32 w2;
      uint32 w3;
} soc_sbx_t3p1_sample_dm1_t;


extern void soc_sbx_t3p1_sample_dm1_t_init(soc_sbx_t3p1_sample_dm1_t *e);
extern int soc_sbx_t3p1_sample_dm1_set(int unit, int index_1, int index_2, soc_sbx_t3p1_sample_dm1_t *e);
extern int soc_sbx_t3p1_sample_dm1_get(int unit, int index_1, int index_2, soc_sbx_t3p1_sample_dm1_t *e);
extern int soc_sbx_t3p1_sample_dm1_table_size_get(int unit);
extern int soc_sbx_t3p1_sample_dm1_table_size_set(int unit, int size);
extern int soc_sbx_t3p1_sample_dm1_data_set(int unit, int index_1, int index_2, unsigned char *buf, unsigned char size);
extern int soc_sbx_t3p1_sample_dm1_data_get(int unit, int index_1, int index_2, unsigned char *buf, unsigned char size);

#endif

#endif
