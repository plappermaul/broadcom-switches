/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_SW_STATE_SIZE_H
#define _DNX_SW_STATE_SIZE_H

#include <soc/dnxc/swstate/sw_state_defs.h>
#if defined(DNX_SW_STATE_DIAGNOSTIC)
#include <bcm/types.h>
#include <soc/types.h>
#include <soc/dnxc/swstate/sw_state_diagnostics.h>

uint32 dnx_sw_state_info_size_get(
    dnx_sw_state_diagnostic_info_t * info,
    const char *layout_str[],
    uint32 nof_entries,
    char *node,
    uint32 static_element_size);

void dnx_sw_state_size_print(
    uint32 size);

#endif
#endif
