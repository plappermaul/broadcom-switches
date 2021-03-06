/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
 *
 * File:        st_sdk_pack.c
 * Purpose:     ST pack and unpack routines for:
 *              - ST Control messages
 *
 * This file is shared between SDK and uKernel.
 */

#if defined(INCLUDE_TELEMETRY)
#include <shared/pack.h>
#include <bcm_int/esw/st_sdk_msg.h>
#include <bcm_int/esw/st_sdk_pack.h>

/*
 * ST Initialization control message - pack
 */
uint8*
st_sdk_msg_ctrl_init_pack(uint8 *buf, st_sdk_msg_ctrl_init_t *msg)
{
    _SHR_PACK_U32(buf, msg->flags);
    _SHR_PACK_U32(buf, msg->init_time_u);
    _SHR_PACK_U32(buf, msg->init_time_l);
    _SHR_PACK_U16(buf, msg->max_export_pkt_length);
    _SHR_PACK_U16(buf, msg->num_instances);
    _SHR_PACK_U16(buf, msg->num_collectors);

    if (ST_UC_FEATURE_CHECK(ST_MAX_NUM_PORTS)) {
        _SHR_PACK_U16(buf, msg->max_num_ports);
    }

    return buf;
}
#else /* INCLUDE_TELEMETRY */
typedef int bcm_int_telemetry_not_emtpy;
#endif /* INCLUDE_TELEMETRY */
