/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 */
#ifndef BM9600_ZPOLARISPROPERTIES_H
#define BM9600_ZPOLARISPROPERTIES_H

/* $Id: bm9600_properties.h,v 1.3 Broadcom SDK $ */
/**
 * @file
 * @brief This file will contain all the defines for the Polaris chip.  This file should 
 *        only contain defines.  There shall be no includes, class forwarding, enumerations
 *        or c++ specific keywords (e.g. const).
 * @version 0.0
 */

#define BM9600_NUM_LINKS (96)
#define BM9600_NUM_NODES (72)
#define BM9600_CHANNELS_PER_LINK (2)
#define BM9600_NUM_TIMESLOTSIZE (32)

#define BM9600_QE_KA_LINE_RATE_GBPS (20)
#define BM9600_QE_SS_LINE_RATE_GBPS (50)
#define BM9600_QE_NODE_NUM_LINKS 20

#define BM9600_QE_NODE_NUM_SFI_LINKS (24)
#define BM9600_QE_NODE_NUM_SCI_LINKS (2)


#define BM9600_MAX_NUM_LOGICAL_XBARS (24)
#define BM9600_MAX_LOGICAL_XBAR_MASK (0xFFFFFF)
#define BM9600_MAX_NUM_PHYSICAL_XBARS_PER_PLANE (24) 
#define BM9600_MAX_NUM_PHYSICAL_BM_PER_PLANE (1)
#define BM9600_MAX_NUM_PLANES (2)
#define BM9600_PRIMARY_PLANE (0)
#define BM9600_SECONDARY_PLANE (1)

#define BM9600_BW_WRED_DP_NUM (3)
#define BM9600_BW_WRED_CURVE_TABLE_SIZE (256)
#define BM9600_BW_WRED_ROWS_PER_TABLE_ENTRY (8)
#define BM9600_BW_REPOSITORY_SIZE (0x7000)
#define BM9600_BW_AI_CHAIN_NUM (3)
#define BM9600_BW_MAX_BAG_NUM (0x1000)
#define BM9600_BW_MAX_VOQ_NUM (0x4000)
#define BM9600_BW_MAX_VOQS_IN_BAG (16)

#define BM9600_ARB_CLOCK_PERIOD_IN_NS (5)

#define BM9600_NUMBER_OF_SI       (BM9600_NUM_LINKS)
#define BM9600_NUMBER_OF_AI_INA   (BM9600_NUM_NODES)
#define BM9600_NUMBER_OF_HC 24

#define BM9600_DEFAULT_BASE_ADDRESS  (0x80000000)
#define BM9600_DEFAULT_REGMAP_OFFSET (0x00100000)


#define NODE_GROUP_REGMAP_BASE_ADDRESS (0x90000000)
#define NODE_GROUP_REGMAP_OFFSET       (0x00020000)

#define BM9600_BROADCAST_OFFSET          (127)

#define BM9600_DEFAULT_TIMESLOT_SIZE (760)

#define BM9600_MAX_NUM_SYSPORTS 2816
#define BM9600_MAX_MULTICAST_EXTENDED_ESETS 1024
#define BM9600_MAX_MULTICAST_ESETS 128
#define BM9600_MAX_PORTSETS 176

#define BM9600_BW_CLOCK_PERIOD_200MHZ 5

#define BM9600_ESET_PER_ESET_ENTRY 16

#define BM9600_FO_NUM_FORCE_LINK_SELECT (6)
#define BM9600_FO_NUM_FORCE_LINK_ACTIVE (4)

#define BM9600_NUM_CLKS_PER_GENERATOR 10

#define BM9600_SS_CHANNELS_PER_LINK (2)
#define BM9600_SS_SERX_CLK_PERIOD_PS 1600
#define BM9600_KA_CHANNELS_PER_LINK (1)
#define BM9600_KA_SERX_CLK_PERIOD_PS 3200

#define BM9600_MIN_AC_TS_TO_GRANT_OFFSET_IN_CLOCKS_DEFAULT 0xB9

#endif /* BM9600_ZPOLARISPROPERTIES_H */
