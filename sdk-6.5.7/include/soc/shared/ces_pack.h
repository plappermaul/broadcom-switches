/* 
 * $Id: ces_pack.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        ces_pack.h
 * Purpose:     Interface to pack and unpack CES control messages.
 *              This is to be shared between SDK host and uKernel.
 */

#ifndef   _SOC_SHARED_CES_PACK_H_
#define   _SOC_SHARED_CES_PACK_H_

#include <soc/shared/mos_msg_ces.h>

extern uint8 *bcm_ces_crm_init_msg_pack(uint8 *buf, bcm_ces_crm_init_msg_t *msg);
extern uint8 *bcm_ces_crm_init_msg_unpack(uint8 *buf, bcm_ces_crm_init_msg_t *msg);
extern uint8 *bcm_ces_crm_cclk_config_msg_pack(uint8 *buf, bcm_ces_crm_cclk_config_msg_t *msg);
extern uint8 *bcm_ces_crm_cclk_config_msg_unpack(uint8 *buf, bcm_ces_crm_cclk_config_msg_t *msg);
extern uint8 *bcm_ces_crm_rclock_config_msg_pack(uint8 *buf, bcm_ces_crm_rclock_config_msg_t *msg);
extern uint8 *bcm_ces_crm_rclock_config_msg_unpack(uint8 *buf, bcm_ces_crm_rclock_config_msg_t *msg);
extern uint8 *bcm_ces_crm_status_msg_pack(uint8 *buf, bcm_ces_crm_status_msg_t *msg);
extern uint8 *bcm_ces_crm_status_msg_unpack(uint8 *buf, bcm_ces_crm_status_msg_t *msg);

#endif /* _SOC_SHARED_CES_PACK_H_ */
