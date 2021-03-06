
/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef _DNXC_SW_STATE_MUTEX_PLAIN_H_

#define _DNXC_SW_STATE_MUTEX_PLAIN_H_

#include <soc/types.h>
#include <soc/error.h>
#include <soc/dnxc/swstate/types/sw_state_mutex.h>
#include <shared/bsl.h>
#include <soc/sand/shrextend/shrextend_debug.h>
#include <sal/core/sync.h>

int dnxc_sw_state_mutex_create(
    int unit,
    uint32 node_id,
    sw_state_mutex_t * mtx,
    char *desc,
    uint32 flags);

int dnxc_sw_state_mutex_destroy(
    int unit,
    uint32 node_id,
    sw_state_mutex_t * ptr_mtx,
    uint32 flags);

int dnxc_sw_state_mutex_take(
    int unit,
    uint32 node_id,
    sw_state_mutex_t * ptr_mtx,
    int usec,
    uint32 flags);

int dnxc_sw_state_mutex_give(
    int unit,
    uint32 node_id,
    sw_state_mutex_t * ptr_mtx,
    uint32 flags);

#endif
