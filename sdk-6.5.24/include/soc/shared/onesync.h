/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File:    onesync.h
 * Purpose: OneSync definitions common to SDK and uKernel
 *
 * Notes:   Definition changes should be avoided in order to
 *          maintain compatibility between SDK and uKernel since
 *          both images are built and loaded separately.
 */

#ifndef _SOC_SHARED_ONESYNC_H
#define _SOC_SHARED_ONESYNC_H

/* OneSync message subclasses */
#define MOS_MSG_SUBCALSS_ONESYNC_ID_APPL_READY            0x0
#define MOS_MSG_SUBCALSS_ONESYNC_ID_INIT                  0x1
#define MOS_MSG_SUBCALSS_ONESYNC_ID_UNINIT                0x2
#define MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG               0x3
#define MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_SET       0x4
#define MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_GET       0x5
#define MOS_MSG_SUBCALSS_ONESYNC_ID_VERSION_EXCHANGE      0x6

#define MOS_MSG_SUBCLASS_ONESYNC_APPL_READY               MOS_MSG_SUBCALSS_ONESYNC_ID_APPL_READY
#define MOS_MSG_SUBCLASS_ONESYNC_INIT                     MOS_MSG_SUBCALSS_ONESYNC_ID_INIT
#define MOS_MSG_SUBCLASS_ONESYNC_INIT_REPLY               MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_INIT)
#define MOS_MSG_SUBCLASS_ONESYNC_UNINIT                   MOS_MSG_SUBCALSS_ONESYNC_ID_UNINIT
#define MOS_MSG_SUBCLASS_ONESYNC_UNINIT_REPLY             MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_UNINIT)
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG                  MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG_REPLY            MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG)
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG_LVL_SET          MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_SET
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG_LVL_SET_REPLY    MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_SET)
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG_LVL_GET          MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_GET
#define MOS_MSG_SUBCLASS_ONESYNC_HOSTLOG_LVL_GET_REPLY    MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_HOSTLOG_LVL_GET)
#define MOS_MSG_SUBCLASS_ONESYNC_VERSION_EXCHANGE         MOS_MSG_SUBCALSS_ONESYNC_ID_VERSION_EXCHANGE
#define MOS_MSG_SUBCLASS_ONESYNC_VERSION_EXCHANGE_REPLY   MOS_MSG_SUBCLASS_REPLY(MOS_MSG_SUBCALSS_ONESYNC_ID_VERSION_EXCHANGE)


#ifdef BCM_UKERNEL
  /* Build for uKernel not SDK */
  #include "sdk_typedefs.h"
#else
  #include <sal/types.h>
#endif

#define ONESYNC_SDK_VERSION    ((1 << 8) | (0))

#define ONESYNC_APPL_VERSION   ((1 << 8) | (0))


/* OneSync HostLog levels. */
#define ONESYNC_HOSTLOG_LEVEL_BS_INFO           (0x0001)
#define ONESYNC_HOSTLOG_LEVEL_BS_ERR            (0x0002)
#define ONESYNC_HOSTLOG_LEVEL_BS_VERB           (0x0004)
#define ONESYNC_HOSTLOG_LEVEL_PKTIO_INFO        (0x0008)
#define ONESYNC_HOSTLOG_LEVEL_PKTIO_ERR         (0x0010)
#define ONESYNC_HOSTLOG_LEVEL_PKTIO_VERB        (0x0020)
#define ONESYNC_HOSTLOG_LEVEL_GPIO_INFO         (0x0040)
#define ONESYNC_HOSTLOG_LEVEL_GPIO_ERR          (0x0080)
#define ONESYNC_HOSTLOG_LEVEL_GPIO_VERB         (0x0100)
#define ONESYNC_HOSTLOG_LEVEL_PHC_INFO          (0x0200)
#define ONESYNC_HOSTLOG_LEVEL_CONFIG_INFO       (0x0400)

/*****************************************
 * ONESYNC uController Error codes
 */
typedef enum shr_onesync_uc_error_e {
    SHR_ONESYNC_UC_E_NONE = 0,
    SHR_ONESYNC_UC_E_INTERNAL,
    SHR_ONESYNC_UC_E_MEMORY,
    SHR_ONESYNC_UC_E_UNIT,
    SHR_ONESYNC_UC_E_PARAM,
    SHR_ONESYNC_UC_E_EMPTY,
    SHR_ONESYNC_UC_E_FULL,
    SHR_ONESYNC_UC_E_NOT_FOUND,
    SHR_ONESYNC_UC_E_EXISTS,
    SHR_ONESYNC_UC_E_TIMEOUT,
    SHR_ONESYNC_UC_E_BUSY,
    SHR_ONESYNC_UC_E_FAIL,
    SHR_ONESYNC_UC_E_DISABLED,
    SHR_ONESYNC_UC_E_BADID,
    SHR_ONESYNC_UC_E_RESOURCE,
    SHR_ONESYNC_UC_E_CONFIG,
    SHR_ONESYNC_UC_E_UNAVAIL,
    SHR_ONESYNC_UC_E_INIT,
    SHR_ONESYNC_UC_E_PORT
} shr_onesync_uc_error_t;

#define ONESYNC_HOSTLOG_ENTRY_SIZE   128

typedef struct onesync_hostlog_info_s {
    uint32 rd_ptr;
    uint32 wr_ptr;
    uint32 num_entries;

    uint32 wr_ptr_addr;
    uint32 log_addr;
} onesync_hostlog_info_t;

#endif /* _SOC_SHARED_ONESYNC_H */
