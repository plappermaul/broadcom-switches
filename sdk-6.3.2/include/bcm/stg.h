/*
 * $Id: stg.h 1.22 Broadcom SDK $
 * 
 * $Copyright: Copyright 2012 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCM_STG_H__
#define __BCM_STG_H__

#include <shared/port.h>
#include <bcm/types.h>

/* bcm_stg_stp_e */
typedef enum bcm_stg_stp_e {
    BCM_STG_STP_DISABLE = _SHR_PORT_STP_DISABLE, /* Disabled. */
    BCM_STG_STP_BLOCK = _SHR_PORT_STP_BLOCK, /* BPDUs/no learns. */
    BCM_STG_STP_LISTEN = _SHR_PORT_STP_LISTEN, /* BPDUs/no learns. */
    BCM_STG_STP_LEARN = _SHR_PORT_STP_LEARN, /* BPDUs/learns. */
    BCM_STG_STP_FORWARD = _SHR_PORT_STP_FORWARD, /* Normal operation. */
    BCM_STG_STP_COUNT = _SHR_PORT_STP_COUNT 
} bcm_stg_stp_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Initialize the Spanning Tree Group (STG) module to its initial
 * configuration.
 */
extern int bcm_stg_init(
    int unit);

/* 
 * Destroy all STGs and initialize the Spanning Tree Group (STG) module
 * to its initial configuration.
 */
extern int bcm_stg_clear(
    int unit);

/* Designate the default STG ID for the chip. */
extern int bcm_stg_default_set(
    int unit, 
    bcm_stg_t stg);

/* Return the current default STG ID for the chip. */
extern int bcm_stg_default_get(
    int unit, 
    bcm_stg_t *stg_ptr);

/* Add a VLAN to a specified STG. */
extern int bcm_stg_vlan_add(
    int unit, 
    bcm_stg_t stg, 
    bcm_vlan_t vid);

/* Remove a VLAN from a Spanning Tree Group. */
extern int bcm_stg_vlan_remove(
    int unit, 
    bcm_stg_t stg, 
    bcm_vlan_t vid);

/* Remove all VLANs from a Spanning Tree Group. */
extern int bcm_stg_vlan_remove_all(
    int unit, 
    bcm_stg_t stg);

/* Generates a list of VLANs in a specified STG. */
extern int bcm_stg_vlan_list(
    int unit, 
    bcm_stg_t stg, 
    bcm_vlan_t **list, 
    int *count);

/* Destroy a list returned by =bcm_stg_vlan_list. */
extern int bcm_stg_vlan_list_destroy(
    int unit, 
    bcm_vlan_t *list, 
    int count);

/* Create a new STG, using a new STG ID. */
extern int bcm_stg_create(
    int unit, 
    bcm_stg_t *stg_ptr);

/* Create a new STG, using a specific STG ID. */
extern int bcm_stg_create_id(
    int unit, 
    bcm_stg_t stg);

/* Destroys the specified STG. */
extern int bcm_stg_destroy(
    int unit, 
    bcm_stg_t stg);

/* Generates a list of IDs for currently defined STGs. */
extern int bcm_stg_list(
    int unit, 
    bcm_stg_t **list, 
    int *count);

/* 
 * Destroys a previously returned list of STG IDs; the STGs themselves
 * are not affected.
 */
extern int bcm_stg_list_destroy(
    int unit, 
    bcm_stg_t *list, 
    int count);

/* Set the Spanning Tree Protocol state of a port in the specified STG. */
extern int bcm_stg_stp_set(
    int unit, 
    bcm_stg_t stg, 
    bcm_port_t port, 
    int stp_state);

/* 
 * Return the Spanning Tree Protocol state of a port in the specified
 * STG.
 */
extern int bcm_stg_stp_get(
    int unit, 
    bcm_stg_t stg, 
    bcm_port_t port, 
    int *stp_state);

/* 
 * Return the maximum number of STGs that the underlying hardware can
 * support.
 */
extern int bcm_stg_count_get(
    int unit, 
    int *max_stg);

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_STG_H__ */
