/*
 * $Id$
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    bfd_feature.h
 */

#ifndef BFD_FEATURE_H_
#define BFD_FEATURE_H_


/*Bit 1 : BFD 64bit packet statistics*/
#define BFD_SDK_BASE_VERSION    0x01000001
#define BFD_SDK_VERSION         BFD_SDK_BASE_VERSION |              \
                                (1 << BFD_FEATURE_64BIT_STAT) | \
                                (1 << BFD_FEATURE_POLL_BIT) | \
                                (1 << BFD_FEATURE_ECHO_MODE) | \
                                (1 << BFD_FEATURE_REMOTE_MEP_ID) | \
                                (1 << BFD_FEATURE_RX_PKT_VLAN_ID) |\
                                (1 << BFD_FEATURE_INIT_CONFIG_FLAGS)

#define BFD_UC_MIN_VERSION 0x01000200

/* Indicates the feature supported : Same bit has to be used in SDK and UKERNEL
 * 0 , 9 and 24th bit cannot be used as they are set in base version string
 * BFD_SDK_VERSION : 0x01000001 BFD_APPL_VERSION : 0x01000200*/

#define BFD_FEATURE_RESERVED1  0
#define BFD_FEATURE_RESERVED2  9
#define BFD_FEATURE_RESERVED3  24
#define BFD_FEATURE_64BIT_STAT 1
#define BFD_FEATURE_POLL_BIT   2
#define BFD_FEATURE_ECHO_MODE  3
#define BFD_FEATURE_REMOTE_MEP_ID 4
#define BFD_FEATURE_RX_PKT_VLAN_ID 5
/*
 * Below feature also introduced msg_init.config_flags
 * which could be used for configuring multiple
 * init time flags on BFD firmware. This is useful
 * for conveying config properties on the SDK side.
 */
#define BFD_FEATURE_INIT_CONFIG_FLAGS 6

extern uint32 bfd_firmware_version;

/*Macro for BFD feature check*/
#define BFD_UC_FEATURE_CHECK(feature)  (bfd_firmware_version & (1 << feature))

#endif /* BFD_FEAT_H_ */
