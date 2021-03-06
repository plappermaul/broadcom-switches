

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_MAX_NIF_H_

#define _DNX_DATA_MAX_NIF_H_




#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



#define DNX_DATA_MAX_NIF_GLOBAL_NOF_LCPLLS (2)


#define DNX_DATA_MAX_NIF_GLOBAL_MAX_CORE_ACCESS_PER_PORT (6)


#define DNX_DATA_MAX_NIF_GLOBAL_NOF_NIF_INTERFACES_PER_CORE (144)


#define DNX_DATA_MAX_NIF_GLOBAL_NIF_INTERFACE_ID_TO_UNIT_ID_GRANULARITY (8)


#define DNX_DATA_MAX_NIF_GLOBAL_REASSEMBLER_FIFO_THRESHOLD (90)


#define DNX_DATA_MAX_NIF_GLOBAL_LAST_PORT_LED_SCAN (88)


#define DNX_DATA_MAX_NIF_PHYS_NOF_PHYS  (144)


#define DNX_DATA_MAX_NIF_PHYS_NOF_PHYS_PER_CORE (128)


#define DNX_DATA_MAX_NIF_PHYS_PM8X50_GEN (2)


#define DNX_DATA_MAX_NIF_PHYS_IS_PAM4_SPEED_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_ILKN_ILU_NOF   (8)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_UNIT_NOF (8)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_UNIT_IF_NOF (2)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_IF_NOF (16)


#define DNX_DATA_MAX_NIF_ILKN_FEC_UNITS_PER_CORE_NOF (16)


#define DNX_DATA_MAX_NIF_ILKN_NOF_LANES_PER_FEC_UNIT (2)


#define DNX_DATA_MAX_NIF_ILKN_LANES_MAX_NOF (24)


#define DNX_DATA_MAX_NIF_ILKN_LANES_MAX_NOF_USING_FEC (24)


#define DNX_DATA_MAX_NIF_ILKN_LANES_ALLOWED_NOF (24)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_OVER_ETH_PMS_MAX (6)


#define DNX_DATA_MAX_NIF_ILKN_SEGMENTS_MAX_NOF (8)


#define DNX_DATA_MAX_NIF_ILKN_SEGMENTS_HALF_NOF (4)


#define DNX_DATA_MAX_NIF_ILKN_PMS_NOF   (6)


#define DNX_DATA_MAX_NIF_ILKN_FMAC_BUS_SIZE (70)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_RX_HRF_NOF (4)


#define DNX_DATA_MAX_NIF_ILKN_ILKN_TX_HRF_NOF (2)


#define DNX_DATA_MAX_NIF_ILKN_DATA_RX_HRF_SIZE (2048)


#define DNX_DATA_MAX_NIF_ILKN_TDM_RX_HRF_SIZE (256)


#define DNX_DATA_MAX_NIF_ILKN_TX_HRF_CREDITS (192)


#define DNX_DATA_MAX_NIF_ILKN_NOF_RX_HRF_PER_PORT (2)


#define DNX_DATA_MAX_NIF_ILKN_WATERMARK_HIGH_ELK (3)


#define DNX_DATA_MAX_NIF_ILKN_WATERMARK_LOW_ELK (4)


#define DNX_DATA_MAX_NIF_ILKN_WATERMARK_HIGH_DATA (11)


#define DNX_DATA_MAX_NIF_ILKN_WATERMARK_LOW_DATA (12)


#define DNX_DATA_MAX_NIF_ILKN_IS_20G_SPEED_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_ILKN_IS_25G_SPEED_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_ILKN_IS_27G_SPEED_SUPPORTED (0)


#define DNX_DATA_MAX_NIF_ILKN_IS_28G_SPEED_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_ILKN_IS_53G_SPEED_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_ILKN_PAD_SIZE  (60)


#define DNX_DATA_MAX_NIF_ETH_CDU_NOF    (18)


#define DNX_DATA_MAX_NIF_ETH_CLU_NOF    (6)


#define DNX_DATA_MAX_NIF_ETH_CDUM_NOF   (2)


#define DNX_DATA_MAX_NIF_ETH_NOF_PMS_IN_CDU (1)


#define DNX_DATA_MAX_NIF_ETH_NOF_PMS_IN_CLU (4)


#define DNX_DATA_MAX_NIF_ETH_TOTAL_NOF_ETHU_PMS_IN_DEVICE (28)


#define DNX_DATA_MAX_NIF_ETH_NOF_CDU_LANES_IN_PM (8)


#define DNX_DATA_MAX_NIF_ETH_NOF_CLU_LANES_IN_PM (4)


#define DNX_DATA_MAX_NIF_ETH_NOF_CDU_PMS (18)


#define DNX_DATA_MAX_NIF_ETH_NOF_CLU_PMS (24)


#define DNX_DATA_MAX_NIF_ETH_ETHU_NOF   (18)


#define DNX_DATA_MAX_NIF_ETH_ETHU_NOF_PER_CORE (10)


#define DNX_DATA_MAX_NIF_ETH_CDU_NOF_PER_CORE (9)


#define DNX_DATA_MAX_NIF_ETH_CLU_NOF_PER_CORE (6)


#define DNX_DATA_MAX_NIF_ETH_NOF_LANES_IN_CDU (8)


#define DNX_DATA_MAX_NIF_ETH_NOF_LANES_IN_CLU (16)


#define DNX_DATA_MAX_NIF_ETH_MAX_NOF_LANES_IN_ETHU (16)


#define DNX_DATA_MAX_NIF_ETH_TOTAL_NOF_CDU_LANES_IN_DEVICE (144)


#define DNX_DATA_MAX_NIF_ETH_CDU_MAC_MODE_CONFIG_NOF (2)


#define DNX_DATA_MAX_NIF_ETH_MAC_MODE_CONFIG_LANES_NOF (4)


#define DNX_DATA_MAX_NIF_ETH_CDU_MAC_NOF (2)


#define DNX_DATA_MAX_NIF_ETH_MAC_LANES_NOF (8)


#define DNX_DATA_MAX_NIF_ETH_ETHU_LOGICAL_FIFO_NOF (16)


#define DNX_DATA_MAX_NIF_ETH_CDU_MEMORY_ENTRIES_NOF (1024)


#define DNX_DATA_MAX_NIF_ETH_CLU_MEMORY_ENTRIES_NOF (1024)


#define DNX_DATA_MAX_NIF_ETH_PRIORITY_GROUP_NOF_ENTRIES_MIN (4)


#define DNX_DATA_MAX_NIF_ETH_PRIORITY_GROUPS_NOF (3)


#define DNX_DATA_MAX_NIF_ETH_PAD_SIZE_MIN (64)


#define DNX_DATA_MAX_NIF_ETH_PAD_SIZE_MAX (96)


#define DNX_DATA_MAX_NIF_ETH_PACKET_SIZE_MAX (16383)


#define DNX_DATA_MAX_NIF_ETH_AN_MAX_NOF_ABILITIES (20)


#define DNX_DATA_MAX_NIF_ETH_PHY_MAP_GRANULARITY (8)


#define DNX_DATA_MAX_NIF_ETH_IS_400G_SUPPORTED (1)


#define DNX_DATA_MAX_NIF_FLEXE_FEU_NOF  (1)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_CLIENTS (82)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_FLEXE_CLIENTS (80)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_SPECIAL_CLIENTS (2)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_GROUPS (8)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_GROUP_ID (1048574)


#define DNX_DATA_MAX_NIF_FLEXE_MIN_GROUP_ID (1)


#define DNX_DATA_MAX_NIF_FLEXE_NB_TX_DELAY (6)


#define DNX_DATA_MAX_NIF_FLEXE_NB_TX_FIFO_TH_FOR_FIRST_READ (25)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_PCS  (8)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_PHY_SPEEDS (4)


#define DNX_DATA_MAX_NIF_FLEXE_PHY_SPEED_GRANULARITY (50000)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_FLEXE_CORE_SPEED (400000)


#define DNX_DATA_MAX_NIF_FLEXE_FLEXE_CORE_CLOCK_SOURCE_PM (0)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_LOGICAL_FIFOS (128)


#define DNX_DATA_MAX_NIF_FLEXE_SAR_CHANNEL_BASE (80)


#define DNX_DATA_MAX_NIF_FLEXE_MAC_CHANNEL_BASE (160)


#define DNX_DATA_MAX_NIF_FLEXE_CLIENT_SPEED_GRANULARITY (5000)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_SB_RX_FIFOS (256)


#define DNX_DATA_MAX_NIF_FLEXE_OAM_CLIENT_CHANNEL (80)


#define DNX_DATA_MAX_NIF_FLEXE_PTP_CLIENT_CHANNEL (81)


#define DNX_DATA_MAX_NIF_FLEXE_OAM_TX_CAL_SLOT (0)


#define DNX_DATA_MAX_NIF_FLEXE_PTP_TX_CAL_SLOT (40)


#define DNX_DATA_MAX_NIF_FLEXE_PRIORITY_GROUPS_NOF (2)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_NOF_SLOTS (80)


#define DNX_DATA_MAX_NIF_FLEXE_MIN_CLIENT_ID (1)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_CLIENT_ID (65534)


#define DNX_DATA_MAX_NIF_FLEXE_PTP_CHANNEL_MAX_BANDWIDTH (500)


#define DNX_DATA_MAX_NIF_FLEXE_OAM_CHANNEL_MAX_BANDWIDTH (2500)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_MEM_ENTRIES_PER_SLOT (128)


#define DNX_DATA_MAX_NIF_FLEXE_PTP_OAM_FIFO_ENTRIES_IN_SB_RX (4)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_FLEXE_INSTANCES (8)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_FLEXE_LPHYS (8)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_SAR_TIMESLOTS (80)


#define DNX_DATA_MAX_NIF_FLEXE_NOF_MAC_TIMESLOTS (81)


#define DNX_DATA_MAX_NIF_FLEXE_DISTRIBUTED_ILU_ID (1)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_NIF_RATE_CENTRALIZED (1220000)


#define DNX_DATA_MAX_NIF_FLEXE_RMC_CAL_NOF_SLOTS (128)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_NIF_RATE_DISTRIBUTED (420000)


#define DNX_DATA_MAX_NIF_FLEXE_MAX_ILU_RATE_DISTRIBUTED (618744)


#define DNX_DATA_MAX_NIF_FLEXE_ILKN_BURST_SIZE_IN_FEU (244)


#define DNX_DATA_MAX_NIF_FLEXE_AVERAGE_IPG_FOR_L1 (88)


#define DNX_DATA_MAX_NIF_FLEXE_MAC_TX_THRESHOLD (4)


#define DNX_DATA_MAX_NIF_PRD_NOF_CONTROL_FRAMES (4)


#define DNX_DATA_MAX_NIF_PRD_NOF_ETHER_TYPE_CODES (16)


#define DNX_DATA_MAX_NIF_PRD_NOF_TCAM_ENTRIES (32)


#define DNX_DATA_MAX_NIF_PRD_NOF_MPLS_SPECIAL_LABELS (4)


#define DNX_DATA_MAX_NIF_PRD_NOF_PRIORITIES (4)


#define DNX_DATA_MAX_NIF_PRD_CUSTOM_ETHER_TYPE_CODE_MIN (1)


#define DNX_DATA_MAX_NIF_PRD_CUSTOM_ETHER_TYPE_CODE_MAX (6)


#define DNX_DATA_MAX_NIF_PRD_ETHER_TYPE_CODE_MAX (15)


#define DNX_DATA_MAX_NIF_PRD_ETHER_TYPE_MAX (65535)


#define DNX_DATA_MAX_NIF_PRD_RMC_THRESHOLD_MAX (32767)


#define DNX_DATA_MAX_NIF_PRD_CDU_RMC_THRESHOLD_MAX (32767)


#define DNX_DATA_MAX_NIF_PRD_HRF_THRESHOLD_MAX (8191)


#define DNX_DATA_MAX_NIF_PRD_MPLS_SPECIAL_LABEL_MAX (15)


#define DNX_DATA_MAX_NIF_PRD_FLEX_KEY_OFFSET_KEY_MAX (127)


#define DNX_DATA_MAX_NIF_PRD_FLEX_KEY_OFFSET_RESULT_MAX (255)


#define DNX_DATA_MAX_NIF_PRD_TPID_MAX   (65535)


#define DNX_DATA_MAX_NIF_PRD_NOF_CLU_PORT_PROFILES (4)


#define DNX_DATA_MAX_NIF_PRD_NOF_FEU_PORT_PROFILES (8)


#define DNX_DATA_MAX_NIF_PRD_RMC_FIFO_2_THRESHOLD_RESOLUTION (16)


#define DNX_DATA_MAX_NIF_PORTMOD_PM_TYPES_NOF (4)


#define DNX_DATA_MAX_NIF_SCHEDULER_RATE_PER_ETHU_BIT (100000)


#define DNX_DATA_MAX_NIF_SCHEDULER_RATE_PER_ILU_BIT (150000)


#define DNX_DATA_MAX_NIF_SCHEDULER_RATE_PER_CDU_RMC_BIT (100000)


#define DNX_DATA_MAX_NIF_SCHEDULER_RATE_PER_CLU_RMC_BIT (25000)


#define DNX_DATA_MAX_NIF_SCHEDULER_NOF_WEIGHT_BITS (4)


#define DNX_DATA_MAX_NIF_SCHEDULER_NOF_RMC_BITS (4)


#define DNX_DATA_MAX_NIF_DBAL_CDU_RX_RMC_ENTRY_SIZE (12)


#define DNX_DATA_MAX_NIF_DBAL_CLU_RX_RMC_ENTRY_SIZE (10)


#define DNX_DATA_MAX_NIF_DBAL_CDU_TX_START_THRESHOLD_ENTRY_SIZE (14)


#define DNX_DATA_MAX_NIF_DBAL_CLU_TX_START_THRESHOLD_ENTRY_SIZE (14)


#define DNX_DATA_MAX_NIF_DBAL_ILU_BURST_MIN_ENTRY_SIZE (2)


#define DNX_DATA_MAX_NIF_DBAL_ETHU_RX_RMC_COUNTER_ENTRY_SIZE (15)


#define DNX_DATA_MAX_NIF_DBAL_CDU_LANE_FIFO_LEVEL_AND_OCCUPANCY_ENTRY_SIZE (14)


#define DNX_DATA_MAX_NIF_DBAL_CLU_LANE_FIFO_LEVEL_AND_OCCUPANCY_ENTRY_SIZE (14)


#define DNX_DATA_MAX_NIF_DBAL_SCH_CNT_DEC_THRESHOLD_BIT_SIZE (15)


#define DNX_DATA_MAX_NIF_DBAL_TX_CREDITS_BITS (8)




#endif 

