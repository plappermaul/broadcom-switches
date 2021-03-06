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
#ifndef FRONT_E_HA_H
#define FRONT_E_HA_H

#include <shr/shr_ha.h>

#define BCMIMM_MAX_TRANS_SIZE 100

/*!
 * Basic field information required by the front end.
 */
typedef struct {
   /*! Field ID */
   uint32_t fid;
   /*! Field data width (bits) */
   uint32_t width;
   /*! The data default value */
   uint64_t def_val;
   /*! The data minimal value */
   uint64_t min_val;
   /*! The data maximal value */
   uint64_t max_val;
   /*! Indicates if the field is read only */
   bool read_only;
   /*! If array this field is the array size, otherwise 0 */
   uint32_t array_size;
} field_info_t;

#define FIELD_INFO_T_ID 0xefeecbbdf0423823

/*!
 * Different types of IMM tables
 */
typedef enum {
   IMM_BASIC = 0,
   IMM_DIRECT = 1
} table_type_t;

/*!
 * The information required to maintain a LT
 */
typedef struct {
   /*! Table ID */
   uint32_t sid;
   /*! The sub module ID */
   uint16_t sub;
   /*! The size of the array index */
   uint8_t array_idx_size;
   /*! Number of key fields */
   uint8_t key_fld_cnt;
   /*! Total number of key bits */
   uint8_t key_arry_size;
   /*! Offset to buffer containing the key bits */
   uint32_t key_bits;
   /*! Backend block handle */
   uint32_t blk_hdl;
   /*! Offset to buffer containing the entire data entry */
   uint32_t data_bits;
   /*! The memory size of data_bits (bytes) includes FID & del indication */
   uint32_t data_bits_len;
   /*! Maximal number of data fields in entry (arrays counted based on their size) */
   uint32_t max_num_field;
   /*! Number of different data field (array is counted as 1) */
   uint32_t data_fld_cnt;
   /*! The data size of a single field */
   uint8_t f_data_size;
   /*! The number of pending operations (during trans) */
   uint8_t pending_ops;
   /*! The index of the table where the dynamic info available */
   uint16_t fields_idx;
   /*! Indicate if table type is interactive */
   bool interactive;
   /*! Determine the behavior of the IMM table */
   table_type_t type;
   /*! On-going transaction ID */
   uint32_t trans_id;
   /*! The entry_limit attribute value */
   uint32_t entry_limit;
} table_info_t;

#define TABLE_INFO_T_ID 0x32ef9334d541e373

/*!
 * Structure to maintain the header of a generic array blocks. These blocks used as
 * buffers for various operations
 */
typedef struct {
   /*! Block validation */
   uint32_t signature;
   /*! The offset of the next available item */
   uint32_t free_offset;
   /*! Total data length of the block */
   uint32_t blk_size;
} generic_array_blk_t;

#define GENERIC_ARRAY_BLK_T_ID 0x666bf2f9903d5e47

/*!
 * Header of an HA block that keeps track of all IMM LT
 */
typedef struct {
   /*! Block validation */
   uint32_t signature;
   /*! The index of the next available item */
   uint32_t free_idx;
   /*! How many tables can be allocated */
   uint32_t array_size;
   /*! How many tables had been allocated */
   uint32_t allocated;
   /*! table info array */
   table_info_t tbl[];
} imm_ha_blk_t;

#define IMM_HA_BLK_T_ID 0x6fa9396cb76d9cb8

/*!
 * Table that maintains the information of a single transaction
 */
typedef struct {
   /*! Table sub module ID */
   uint16_t sub;
   /*! Determine the behavior of the IMM table */
   table_type_t type;
   /*! Pointer (HA) to the data portion */
   uint32_t data;
   /*! Pointer (HA) to the key portion */
   uint32_t key;
} imm_trans_elem_t;

#define IMM_TRANS_ELEM_T_ID 0x03335644e14b147b

/*!
 * Table that maintains the list of all active transactions
 */
typedef struct {
   /*! start trans element index */
   uint32_t start_idx;
   /*! Transaction ID */
   uint32_t trans_id;
   /*! Unit associated with the transaction */
   int unit;
   /*! Total elements in transaction */
   uint32_t elem_cnt;
   /*! Array of transaction elements */
   imm_trans_elem_t elem[BCMIMM_MAX_TRANS_SIZE];
} imm_trans_info_t;

#define IMM_TRANS_INFO_T_ID 0x4cd9c0050948b44e

#endif /* FRONT_E_HA_H */
