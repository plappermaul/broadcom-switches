/*
 * $Id: eav.h 1.25 Broadcom SDK $
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

#ifndef __BCM_EAV_H__
#define __BCM_EAV_H__

#include <bcm/types.h>
#include <bcm/port.h>

/* bcm_eav_control_e */
typedef enum bcm_eav_control_e {
    bcmEAVControlMaxFrameSize, 
    bcmEAVControlTimeBase, 
    bcmEAVControlTimeAdjust, 
    bcmEAVControlTickCounter, 
    bcmEAVControlSlotNumber, 
    bcmEAVControlMacroSlotTime, 
    bcmEAVControlSlotAdjust, 
    bcmEAVControlStreamClassAPCP, 
    bcmEAVControlStreamClassBPCP 
} bcm_eav_control_t;

#define BCM_EAV_TIMEREF_MODE_FREERUN        0x0        
#define BCM_EAV_TIMEREF_MODE_PHASE_ADJUSTMENT 0x1        
#define BCM_EAV_TIMEREF_MODE_FREQ_ADJUMENT1 0x2        
#define BCM_EAV_TIMEREF_MODE_FREQ_ADJUMENT2 0x3        

/* bcm_eav_stream_class_e */
typedef enum bcm_eav_stream_class_e {
    bcmEAVCStreamClassA, 
    bcmEAVCStreamClassB 
} bcm_eav_stream_class_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* bcm_eav_init */
extern int bcm_eav_init(
    int unit);

/* bcm_eav_port_enable_get */
extern int bcm_eav_port_enable_get(
    int unit, 
    bcm_port_t port, 
    int *enable);

/* bcm_eav_port_enable_set */
extern int bcm_eav_port_enable_set(
    int unit, 
    bcm_port_t port, 
    int enable);

/* bcm_eav_link_status_get */
extern int bcm_eav_link_status_get(
    int unit, 
    bcm_port_t port, 
    int *link);

/* bcm_eav_link_status_set */
extern int bcm_eav_link_status_set(
    int unit, 
    bcm_port_t port, 
    int link);

/* bcm_eav_control_get */
extern int bcm_eav_control_get(
    int unit, 
    bcm_eav_control_t type, 
    uint32 *arg, 
    uint32 *arg2);

/* bcm_eav_control_set */
extern int bcm_eav_control_set(
    int unit, 
    bcm_eav_control_t type, 
    uint32 arg, 
    uint32 arg2);

/* bcm_eav_timestamp_get */
extern int bcm_eav_timestamp_get(
    int unit, 
    bcm_port_t port, 
    uint32 *timestamp);

/* bcm_eav_timesync_mac_set */
extern int bcm_eav_timesync_mac_set(
    int unit, 
    bcm_mac_t eav_mac);

/* bcm_eav_timesync_mac_get */
extern int bcm_eav_timesync_mac_get(
    int unit, 
    bcm_mac_t eav_mac);

/* bcm_eav_srp_mac_ethertype_set */
extern int bcm_eav_srp_mac_ethertype_set(
    int unit, 
    bcm_mac_t mac, 
    bcm_port_ethertype_t ethertype);

/* bcm_eav_srp_mac_ethertype_get */
extern int bcm_eav_srp_mac_ethertype_get(
    int unit, 
    bcm_mac_t mac, 
    bcm_port_ethertype_t *ethertype);

/* bcm_eav_pcp_mapping_set */
extern int bcm_eav_pcp_mapping_set(
    int unit, 
    bcm_eav_stream_class_t type, 
    int pcp, 
    int remapped_pcp);

/* bcm_eav_pcp_mapping_get */
extern int bcm_eav_pcp_mapping_get(
    int unit, 
    bcm_eav_stream_class_t type, 
    int *pcp, 
    int *remapped_pcp);

/* bcm_bandwidth_set */
extern int bcm_eav_bandwidth_set(
    int unit, 
    bcm_port_t port, 
    bcm_eav_stream_class_t type, 
    uint32 bytes_sec, 
    uint32 bytes_burst);

/* bcm_eav_bandwidth_get */
extern int bcm_eav_bandwidth_get(
    int unit, 
    bcm_port_t port, 
    bcm_eav_stream_class_t type, 
    uint32 *bytes_sec, 
    uint32 *bytes_burst);

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_EAV_H__ */
