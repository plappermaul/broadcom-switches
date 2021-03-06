/**************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by HA parser from YAML formated file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: bcmha/scripts/ha_yml_parser.py
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef BCMCFG_CFG_ERROR_CONTROL_HA_CONFIG_H
#define BCMCFG_CFG_ERROR_CONTROL_HA_CONFIG_H

#include <shr/shr_ha.h>

/*!
 * \brief Software component configuration for software error control.
 * The software component configuration for software error control is derived
 * from the BCMCFG configuration YAML file using the following
 * schema:
 * \code{.unparsed}
 *  component:
 *    cfg_error_control:
 *      stop_playback_on_error: integer
 * \endcode
 */
typedef struct {
   /*! \brief Software error control.
     *
     * When true, logical table playback will stop and an initialization
     * error will be reported if a logical table operation returns an error.
     * Otherwise, the error will be logged and playback will continue. */
   uint32_t stop_playback_on_error;
} bcmcfg_cfg_error_control_ha_config_t;

#define BCMCFG_CFG_ERROR_CONTROL_HA_CONFIG_T_ID 0xaffb27621464059f

#endif /* BCMCFG_CFG_ERROR_CONTROL_HA_CONFIG_H */
