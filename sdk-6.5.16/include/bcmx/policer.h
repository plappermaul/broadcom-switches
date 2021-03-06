/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_POLICER_H__
#define __BCMX_POLICER_H__

#include <bcm/types.h>
#include <bcmx/bcmx.h>
#include <bcm/policer.h>

/* Policer Config Structure */
typedef bcm_policer_config_t bcmx_policer_config_t;

/* Initialize a policer config structure. */
extern void bcmx_policer_config_t_init(
    bcmx_policer_config_t *pol_cfg);

/* Initialize policer module. */
extern int bcmx_policer_init(void);

/* Create a policer entry. */
extern int bcmx_policer_create(
    bcmx_policer_config_t *pol_cfg, 
    bcm_policer_t *policer_id);

/* Destroy a policer entry. */
extern int bcmx_policer_destroy(
    bcm_policer_t policer_id);

/* Destroy all policer entries. */
extern int bcmx_policer_destroy_all(void);

/* Set the config settings for a policer entry. */
extern int bcmx_policer_set(
    bcm_policer_t policer_id, 
    bcmx_policer_config_t *pol_cfg);

/* Get the config settings for a policer entry. */
extern int bcmx_policer_get(
    bcm_policer_t policer_id, 
    bcmx_policer_config_t *pol_cfg);

#endif /* __BCMX_POLICER_H__ */
