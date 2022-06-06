/*
 * $Id$
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    bfd_feature.h
 */

#ifndef BFD_SDK_PACK_H_
#define BFD_SDK_PACK_H_

#if defined(INCLUDE_BFD)

#include <soc/shared/bfd_msg.h>
#include <bcm_int/esw/bfd_feature.h>
#include <soc/defs.h>
#include <bcm_int/esw/bfd_sdk_msg.h>

uint8 *
bfd_sdk_msg_ctrl_init_pack(uint8 *buf, bfd_sdk_msg_ctrl_init_t *msg);


uint8 *bfd_sdk_msg_ctrl_stat_reply_unpack(uint8 *buf, bfd_sdk_msg_ctrl_stat_reply_t *msg,
                                          uint8 stat64bit);

uint8 *bfd_sdk_msg_ctrl_stat_req_pack(uint8 *buf, bfd_sdk_msg_ctrl_stat_req_t *msg);

uint8 * bfd_sdk_version_exchange_msg_unpack(uint8 *buf,
                                    bfd_sdk_version_exchange_msg_t *msg);

uint8 * bfd_sdk_version_exchange_msg_pack(uint8 *buf,
                                    bfd_sdk_version_exchange_msg_t *msg);

uint8 *
bfd_sdk_msg_ctrl_sess_set_pack(uint8 *buf,
                               bfd_sdk_msg_ctrl_sess_set_t *msg);
uint8 *
bfd_sdk_msg_ctrl_sess_get_unpack(uint8 *buf,
                                 bfd_sdk_msg_ctrl_sess_get_t *msg);
uint8 *
bfd_sdk_msg_ctrl_discard_stat_get_unpack(uint8 *buf,
                                         bfd_sdk_msg_ctrl_discard_stat_get_t *msg);
#if defined( BCM_TOMAHAWK_SUPPORT)
uint8 *
bfd_sdk_msg_port_txqueue_map_pack(uint8 *buf,
                                bfd_sdk_msg_port_txqueue_map_t *msg);
#endif

#endif /* INCLUDE_BFD */

#endif /* BFD_SDK_PACK_H_ */

