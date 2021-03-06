/*
 *         
 * $Id: phymod.xml,v 1.1.2.5 Broadcom SDK $
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
 *     
 * DO NOT EDIT THIS FILE!
 *
 */

#ifndef _PHYMOD_DIAGNOSTICS_DISPATCH_H__H_
#define _PHYMOD_DIAGNOSTICS_DISPATCH_H__H_

#include <phymod/phymod_definitions.h>
#include <phymod/phymod_diagnostics.h>

typedef int (*__dispatch__phymod_phy_rx_slicer_position_set_f)(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position);
typedef int (*__dispatch__phymod_phy_rx_slicer_position_get_f)(const phymod_phy_access_t* phy, uint32_t flags, phymod_slicer_position_t* position);
typedef int (*__dispatch__phymod_phy_rx_slicer_position_max_get_f)(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position_min, const phymod_slicer_position_t* position_max);
typedef int (*__dispatch__phymod_phy_prbs_config_set_f)(const phymod_phy_access_t* phy, uint32_t flags , const phymod_prbs_t* prbs);
typedef int (*__dispatch__phymod_phy_prbs_config_get_f)(const phymod_phy_access_t* phy, uint32_t flags , phymod_prbs_t* prbs);
typedef int (*__dispatch__phymod_phy_prbs_enable_set_f)(const phymod_phy_access_t* phy, uint32_t flags , uint32_t enable);
typedef int (*__dispatch__phymod_phy_prbs_enable_get_f)(const phymod_phy_access_t* phy, uint32_t flags , uint32_t* enable);
typedef int (*__dispatch__phymod_phy_prbs_status_get_f)(const phymod_phy_access_t* phy, uint32_t flags, phymod_prbs_status_t* prbs_status);
typedef int (*__dispatch__phymod_phy_pattern_config_set_f)(const phymod_phy_access_t* phy, const phymod_pattern_t* pattern);
typedef int (*__dispatch__phymod_phy_pattern_config_get_f)(const phymod_phy_access_t* phy, phymod_pattern_t* pattern);
typedef int (*__dispatch__phymod_phy_pattern_enable_set_f)(const phymod_phy_access_t* phy, uint32_t enable, const phymod_pattern_t* pattern);
typedef int (*__dispatch__phymod_phy_pattern_enable_get_f)(const phymod_phy_access_t* phy, uint32_t* enable);
typedef int (*__dispatch__phymod_core_diagnostics_get_f)(const phymod_core_access_t* core, phymod_core_diagnostics_t* diag);
typedef int (*__dispatch__phymod_phy_diagnostics_get_f)(const phymod_phy_access_t* phy, phymod_phy_diagnostics_t* diag);
typedef int (*__dispatch__phymod_phy_pmd_info_dump_f)(const phymod_phy_access_t* phy, char* type);
typedef int (*__dispatch__phymod_phy_pcs_info_dump_f)(const phymod_phy_access_t* phy, char* type);
typedef int (*__dispatch__phymod_phy_meas_lowber_eye_f)(const phymod_phy_access_t* phy, phymod_phy_eyescan_options_t eyescan_options, uint32_t* buffer);
typedef int (*__dispatch__phymod_phy_display_lowber_eye_f)(const phymod_phy_access_t* phy, phymod_phy_eyescan_options_t eyescan_options, uint32_t* buffer);
typedef int (*__dispatch__phymod_phy_pmd_ber_end_cmd_f)(const phymod_phy_access_t* phy, uint8_t supp_info, uint32_t timeout_ms);
typedef int (*__dispatch__phymod_phy_meas_eye_scan_start_f)(const phymod_phy_access_t* phy, uint8_t direction);
typedef int (*__dispatch__phymod_phy_read_eye_scan_stripe_f)(const phymod_phy_access_t* phy, uint32_t* buffer, uint16_t* status);
typedef int (*__dispatch__phymod_phy_display_eye_scan_header_f)(const phymod_phy_access_t* phy, int8_t i);
typedef int (*__dispatch__phymod_phy_display_eye_scan_stripe_f)(const phymod_phy_access_t* phy, int8_t y, uint32_t* buffer);
typedef int (*__dispatch__phymod_phy_display_eye_scan_footer_f)(const phymod_phy_access_t* phy, int8_t i);
typedef int (*__dispatch__phymod_phy_meas_eye_scan_done_f)(const phymod_phy_access_t* phy);
typedef int (*__dispatch__phymod_phy_eye_scan_debug_info_dump_f)(const phymod_phy_access_t* phy);

typedef struct __phymod_diagnostics__dispatch__s__ {
    __dispatch__phymod_phy_rx_slicer_position_set_f                                  f_phymod_phy_rx_slicer_position_set;
    __dispatch__phymod_phy_rx_slicer_position_get_f                                  f_phymod_phy_rx_slicer_position_get;
    __dispatch__phymod_phy_rx_slicer_position_max_get_f                              f_phymod_phy_rx_slicer_position_max_get;
    __dispatch__phymod_phy_prbs_config_set_f                                         f_phymod_phy_prbs_config_set;
    __dispatch__phymod_phy_prbs_config_get_f                                         f_phymod_phy_prbs_config_get;
    __dispatch__phymod_phy_prbs_enable_set_f                                         f_phymod_phy_prbs_enable_set;
    __dispatch__phymod_phy_prbs_enable_get_f                                         f_phymod_phy_prbs_enable_get;
    __dispatch__phymod_phy_prbs_status_get_f                                         f_phymod_phy_prbs_status_get;
    __dispatch__phymod_phy_pattern_config_set_f                                      f_phymod_phy_pattern_config_set;
    __dispatch__phymod_phy_pattern_config_get_f                                      f_phymod_phy_pattern_config_get;
    __dispatch__phymod_phy_pattern_enable_set_f                                      f_phymod_phy_pattern_enable_set;
    __dispatch__phymod_phy_pattern_enable_get_f                                      f_phymod_phy_pattern_enable_get;
    __dispatch__phymod_core_diagnostics_get_f                                        f_phymod_core_diagnostics_get;
    __dispatch__phymod_phy_diagnostics_get_f                                         f_phymod_phy_diagnostics_get;
    __dispatch__phymod_phy_pmd_info_dump_f                                           f_phymod_phy_pmd_info_dump;
    __dispatch__phymod_phy_pcs_info_dump_f                                           f_phymod_phy_pcs_info_dump;
    __dispatch__phymod_phy_meas_lowber_eye_f                                         f_phymod_phy_meas_lowber_eye;
    __dispatch__phymod_phy_display_lowber_eye_f                                      f_phymod_phy_display_lowber_eye;
    __dispatch__phymod_phy_pmd_ber_end_cmd_f                                         f_phymod_phy_pmd_ber_end_cmd;
    __dispatch__phymod_phy_meas_eye_scan_start_f                                     f_phymod_phy_meas_eye_scan_start;
    __dispatch__phymod_phy_read_eye_scan_stripe_f                                    f_phymod_phy_read_eye_scan_stripe;
    __dispatch__phymod_phy_display_eye_scan_header_f                                 f_phymod_phy_display_eye_scan_header;
    __dispatch__phymod_phy_display_eye_scan_stripe_f                                 f_phymod_phy_display_eye_scan_stripe;
    __dispatch__phymod_phy_display_eye_scan_footer_f                                 f_phymod_phy_display_eye_scan_footer;
    __dispatch__phymod_phy_meas_eye_scan_done_f                                      f_phymod_phy_meas_eye_scan_done;
    __dispatch__phymod_phy_eye_scan_debug_info_dump_f                                f_phymod_phy_eye_scan_debug_info_dump;
} __phymod_diagnostics__dispatch__t__;

extern __phymod_diagnostics__dispatch__t__* __phymod_diagnostics__dispatch__[phymodDispatchTypeCount];

#endif /*_PHYMOD_DIAGNOSTICS_DISPATCH_H_*/
