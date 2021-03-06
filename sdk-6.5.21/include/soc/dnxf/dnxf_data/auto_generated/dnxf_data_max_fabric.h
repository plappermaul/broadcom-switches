

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef _DNXF_DATA_MAX_FABRIC_H_

#define _DNXF_DATA_MAX_FABRIC_H_




#ifndef BCM_DNXF_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



#define DNXF_DATA_MAX_FABRIC_GENERAL_MAX_NOF_PIPES (3)


#define DNXF_DATA_MAX_FABRIC_PIPES_MAX_NOF_PIPES (3)


#define DNXF_DATA_MAX_FABRIC_MULTICAST_TABLE_ROW_SIZE_IN_UINT32 (7)


#define DNXF_DATA_MAX_FABRIC_FIFOS_GRANULARITY (4)


#define DNXF_DATA_MAX_FABRIC_FIFOS_MIN_DEPTH (16)


#define DNXF_DATA_MAX_FABRIC_FIFOS_MAX_UNUSED_RESOURCES (36)


#define DNXF_DATA_MAX_FABRIC_FIFOS_RX_RESOURCES (768)


#define DNXF_DATA_MAX_FABRIC_FIFOS_MID_RESOURCES (4608)


#define DNXF_DATA_MAX_FABRIC_FIFOS_TX_RESOURCES (4096)


#define DNXF_DATA_MAX_FABRIC_FIFOS_RX_FULL_OFFSET (4)


#define DNXF_DATA_MAX_FABRIC_FIFOS_MID_FULL_OFFSET (12)


#define DNXF_DATA_MAX_FABRIC_FIFOS_TX_FULL_OFFSET (12)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_PROFILES (2)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_THRESHOLD_PRIORITIES (4)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_THRESHOLD_LEVELS (3)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_THRESHOLD_INDEX_DIMENSIONS (2)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_THRESHOLDS (37)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_DFL_BANKS (10)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_DFL_SUB_BANKS (2)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_DFL_BANK_ENTRIES (3600)


#define DNXF_DATA_MAX_FABRIC_CONGESTION_NOF_RCI_BITS (2)


#define DNXF_DATA_MAX_FABRIC_CELL_FIFO_DMA_FABRIC_CELL_NOF_ENTRIES_PER_CELL (4)


#define DNXF_DATA_MAX_FABRIC_CELL_RX_CPU_CELL_MAX_PAYLOAD_SIZE (1024)


#define DNXF_DATA_MAX_FABRIC_CELL_RX_MAX_NOF_CPU_BUFFERS (8)


#define DNXF_DATA_MAX_FABRIC_CELL_SOURCE_ROUTED_CELLS_HEADER_OFFSET (32)


#define DNXF_DATA_MAX_FABRIC_CELL_FIFO_DMA_NOF_CHANNELS (8)


#define DNXF_DATA_MAX_FABRIC_CELL_FIFO_DMA_ENTRY_SIZE (80)


#define DNXF_DATA_MAX_FABRIC_CELL_FIFO_DMA_MAX_NOF_ENTRIES (16384)


#define DNXF_DATA_MAX_FABRIC_CELL_FIFO_DMA_MIN_NOF_ENTRIES (64)


#define DNXF_DATA_MAX_FABRIC_TOPOLOGY_MAX_LINK_SCORE (120)


#define DNXF_DATA_MAX_FABRIC_TOPOLOGY_NOF_LOCAL_MODID (192)


#define DNXF_DATA_MAX_FABRIC_TOPOLOGY_NOF_LOCAL_MODID_FE13 (96)


#define DNXF_DATA_MAX_FABRIC_GPD_IN_TIME (60)


#define DNXF_DATA_MAX_FABRIC_GPD_OUT_TIME (200)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_TABLE_ROW_SIZE_IN_UINT32 (7)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_TABLE_GROUP_SIZE (32)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_RMGR_UNITS (32)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_RMGR_NOF_LINKS (192)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_GPD_RMGR_TIME_FACTOR (4)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_GEN_RATE_LINK_DELTA (15)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_GEN_RATE_FULL_CYCLE (44800)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_GPD_GEN_RATE_FULL_CYCLE (10000)


#define DNXF_DATA_MAX_FABRIC_REACHABILITY_WATCHDOG_RATE (100000)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_MAX_MASTER_ID (2047)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_0 (10)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_1 (8388607)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_2 (10000)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_3 (300000)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_4 (1000)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_5 (100000)


#define DNXF_DATA_MAX_FABRIC_SYSTEM_UPGRADE_PARAM_7 (100)




#endif 

