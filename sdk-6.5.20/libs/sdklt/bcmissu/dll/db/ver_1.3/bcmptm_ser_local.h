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
#ifndef BCMPTM_SER_LOCAL_H
#define BCMPTM_SER_LOCAL_H

#include <shr/shr_ha.h>

#define DEPTH_WIDE_ENTRY_BITMAP 512
#define BCMPTM_SER_MAX_SLICE_NUM 16
#define BCMPTM_SER_SLICE_MODE_NUM 2

/*!
 * \brief Info to be stored for SER.
 */
typedef struct {
   /*! Enable the checking of protected ranges within the SER Engine. */
   uint32_t ser_range_enable;
   /*! The checking of protected ranges within the SER Engine is in usage. */
   uint32_t ser_range_in_usage;
   /*! SER_LOG is maintained as circle buffer, new SER log can overlay the old one. */
   bool ser_log_overlaid;
} bcmptm_ser_sinfo_t;

#define BCMPTM_SER_SINFO_T_ID 0x64dc17acce98d9f6

/*!
 * \brief Bit position corresponds to entry view.
 */
typedef struct {
   /*! Bit position corresponds to entry view. */
   uint32_t entry_view_bitmap[DEPTH_WIDE_ENTRY_BITMAP];
} bcmptm_ser_entry_view_bitmap_t;

#define BCMPTM_SER_ENTRY_VIEW_BITMAP_T_ID 0xe1a91c2579ad1012

typedef enum {
   BCMPTM_SER_NARROW_INDEX = 0,
   BCMPTM_SER_WIDE_INDEX = 1
} bcmptm_ser_mode_index_t;

/*!
 * \brief Information of one slice.
 */
typedef struct {
   /*! Depth of narrow and wide view slice. */
   uint16_t slice_depth[BCMPTM_SER_SLICE_MODE_NUM];
   /*! slice mode. */
   uint8_t slice_mode;
} bcmptm_ser_slice_mode_t;

#define BCMPTM_SER_SLICE_MODE_T_ID 0x2b3d8da417fc3090

/*!
 * \brief Information of all slices.
 */
typedef struct {
   /*! SIDs of wide and narrow view which share the same PT. */
   uint32_t sid_array[BCMPTM_SER_SLICE_MODE_NUM];
   /*! Number of SIDs. */
   uint8_t sid_num;
   /*! Properties for slices. */
   uint8_t flags;
   /*! Number of SIDs. */
   uint8_t slice_num;
   /*! Depth and mode of every slice. */
   bcmptm_ser_slice_mode_t slice_mode_array[BCMPTM_SER_MAX_SLICE_NUM];
} bcmptm_ser_slices_info_t;

#define BCMPTM_SER_SLICES_INFO_T_ID 0xb20a2ed7fdc628ec

#endif /* BCMPTM_SER_LOCAL_H */
