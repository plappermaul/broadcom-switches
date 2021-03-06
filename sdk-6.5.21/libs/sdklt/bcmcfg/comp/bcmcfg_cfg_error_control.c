/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by cfg from component configuration definition files.
 *
 * Tool: $SDK/INTERNAL/cfg/bin/cfg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <sal/sal_types.h>
#include <sal/sal_libc.h>
#include <shr/shr_error.h>
#include <bcmcfg/bcmcfg_types.h>
#include <bcmcfg/bcmcfg_internal.h>
#include <bcmcfg/comp/bcmcfg_cfg_error_control.h>
#include "bcmcfg_cfg_error_control_scanner.h"
#include <bcmcfg/comp/bcmcfg_comp_scanner_conf.h>
static const bcmcfg_comp_node_t bcmcfg_cfg_error_control_schema[] = {
    {
        .node = BCMCFG_COMP_MAP,
        .key = "cfg_error_control",
        .next = BCMCFG_NO_IDX,
        .offset = 0,
        .size = sizeof(bcmcfg_cfg_error_control_config_t),
    }, /* [top] 0 */
    {
        .node = BCMCFG_COMP_SCALAR,
        .key = "stop_playback_on_error",
        .next = BCMCFG_NO_IDX,
        .offset = offsetof(bcmcfg_cfg_error_control_config_t, stop_playback_on_error),
        .size = sizeof(((bcmcfg_cfg_error_control_config_t *)0)->stop_playback_on_error),
    }, /* stop_playback_on_error 1 */
};

static bcmcfg_cfg_error_control_config_t *bcmcfg_cfg_error_control_data;

static const bcmcfg_cfg_error_control_config_t bcmcfg_cfg_error_control_default_data =
    {
        .stop_playback_on_error = 0,
    };

const bcmcfg_comp_scanner_t bcmcfg_cfg_error_control_scanner = {
    .schema_count = 2,
    .schema = bcmcfg_cfg_error_control_schema,
    .data_size = sizeof(*bcmcfg_cfg_error_control_data),
    .data = (uint32_t **)(char *)&bcmcfg_cfg_error_control_data,
    .default_data = (uint32_t *)(char *)&bcmcfg_cfg_error_control_default_data,
};

int
bcmcfg_cfg_error_control_default_config_get(bcmcfg_cfg_error_control_config_t *config)
{
    return bcmcfg_comp_scanner_default_config_get(&bcmcfg_cfg_error_control_scanner,
                                                  sizeof(bcmcfg_cfg_error_control_config_t),
                                                  (void*) config);
}

int
bcmcfg_cfg_error_control_config_get(bcmcfg_cfg_error_control_config_t *config)
{
    return bcmcfg_comp_scanner_config_get(&bcmcfg_cfg_error_control_scanner,
                                          sizeof(bcmcfg_cfg_error_control_config_t),
                                          (void*) config);
}

int
bcmcfg_cfg_error_control_config_set(bcmcfg_cfg_error_control_config_t *config)
{
    return bcmcfg_comp_scanner_config_set(&bcmcfg_cfg_error_control_scanner,
                                          sizeof(bcmcfg_cfg_error_control_config_t),
                                          (void*) config);
}

