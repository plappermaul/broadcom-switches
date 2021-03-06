/*
* This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
* 
* Copyright 2007-2019 Broadcom Inc. All rights reserved.
* 
*/

/*
 *         
 * 
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
 */

#include <phymod/phymod.h>
#include <phymod/phymod_system.h>
#include <phymod/phymod_diagnostics.h>
#include "sesto_address_defines.h"
#include "sesto_reg_structs.h"
#include "sesto_cfg_seq.h"
#include "sesto_diag_seq.h"
#include "sesto_serdes/falcon_furia_sesto_src/falcon_furia_sesto_functions.h"
#include "sesto_serdes/merlin_sesto_src/merlin_sesto_functions.h"
#include "sesto_serdes/common/srds_api_enum.h"

int _sesto_phymod_prbs_poly_to_serdes_prbs_poly(uint16_t phymod_poly, uint16_t *serdes_poly)
{
    switch (phymod_poly) {
        case phymodPrbsPoly7:
            *serdes_poly = PRBS_7;
        break;
        case phymodPrbsPoly9:
            *serdes_poly = PRBS_9;
        break;
        case phymodPrbsPoly11:
            *serdes_poly = PRBS_11;
        break;
        case phymodPrbsPoly15:
            *serdes_poly = PRBS_15;
        break;
        case phymodPrbsPoly23:
            *serdes_poly = PRBS_23;
        break;
        case phymodPrbsPoly31:
            *serdes_poly = PRBS_31;
        break;
        case phymodPrbsPoly58:
            *serdes_poly = PRBS_58;
        break;
        default: 
            return PHYMOD_E_PARAM;
    }

    return PHYMOD_E_NONE;
}
int _sesto_serdes_prbs_poly_to_phymod_prbs_poly(uint16_t serdes_poly, phymod_prbs_poly_t *phymod_poly)
{
    switch (serdes_poly) {
        case PRBS_7:
            *phymod_poly = phymodPrbsPoly7;
        break; 
        case PRBS_9:
            *phymod_poly = phymodPrbsPoly9;
        break; 
        case PRBS_11:
            *phymod_poly = phymodPrbsPoly11;
        break; 
        case PRBS_15:
            *phymod_poly = phymodPrbsPoly15;
        break; 
        case PRBS_23:
            *phymod_poly = phymodPrbsPoly23;
        break; 
        case PRBS_31:
            *phymod_poly = phymodPrbsPoly31;
        break; 
        case PRBS_58:
            *phymod_poly = phymodPrbsPoly58;
        break; 
        default: 
            return PHYMOD_E_PARAM;
    }

    return PHYMOD_E_NONE;
}

int _sesto_phy_prbs_config_set(const phymod_phy_access_t* phy, uint32_t flags , const phymod_prbs_t* prbs)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0;
    uint16_t lane_mask = 0, cast_type = 0, mcast_val = 0;
    uint16_t prbs_poly = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));
    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    SESTO_GET_IP(phy, config, ip);
    SESTO_IF_ERR_RETURN(
          _sesto_phymod_prbs_poly_to_serdes_prbs_poly (prbs->poly, &prbs_poly));

    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    
    SESTO_GET_CORE_SLICE_INFO(phy, ip, config, &cast_type, &mcast_val);

    PHYMOD_DEBUG_VERBOSE(("IP:%s Max_lane:%d lanemask:0x%x cast Type:%x\n", 
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask, cast_type));

    if (cast_type != SESTO_CAST_INVALID) {
        for (lane = 0; lane < max_lane; lane ++) {
            if (lane_mask & (1 << lane)) {
                SESTO_IF_ERR_RETURN (
                  _sesto_set_slice_reg (pa, cast_type, ip,
                              SESTO_DEV_PMA_PMD, mcast_val, lane));
                if (flags == 0 || PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
                    if (ip == SESTO_FALCON_CORE) {
                        SESTO_IF_ERR_RETURN(
                            falcon_furia_sesto_config_tx_prbs(pa, prbs_poly, (uint8_t)prbs->invert)); 
                    } else {
                        SESTO_IF_ERR_RETURN(
                            merlin_sesto_config_tx_prbs(pa, prbs_poly, (uint8_t)prbs->invert)); 
                    }
                }
                if (PHYMOD_PRBS_DIRECTION_RX_GET(flags) || flags == 0) {
                    /* Enabling PRBS CHECKER SELF SYNC HYS*/
                    if (ip == SESTO_FALCON_CORE) {
                        SESTO_IF_ERR_RETURN(
                                falcon_furia_sesto_config_rx_prbs(pa, prbs_poly, 0, 
                                (uint8_t)prbs->invert));
                    } else {
                        SESTO_IF_ERR_RETURN(
                            merlin_sesto_config_rx_prbs(pa, prbs_poly, 0, 
                                (uint8_t)prbs->invert)); 
                    }
                }
                if (SESTO_IS_MULTI_BROAD_CAST_SET(cast_type)) {
                    break;
                }
            }
        }
    } else {
        PHYMOD_DEBUG_VERBOSE(("Lane mask not Fine \n"));
        return PHYMOD_E_PARAM;
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_prbs_enable_set(const phymod_phy_access_t* phy, uint32_t flags, uint32_t enable)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0, dig_lpbk_en = 0;
    uint16_t lane_mask = 0, cast_type = 0, mcast_val = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    SESTO_GET_CORE_SLICE_INFO(phy, ip, config, &cast_type, &mcast_val);

    PHYMOD_DEBUG_VERBOSE(("IP:%s Max_lane:%d lanemask:0x%x cast type:%d\n", 
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask, cast_type));

    if (cast_type != SESTO_CAST_INVALID) {
        for (lane = 0; lane < max_lane; lane ++) {
            if (lane_mask & (1 << lane)) {
                SESTO_IF_ERR_RETURN (
                  _sesto_set_slice_reg (pa, cast_type, ip,
                              SESTO_DEV_PMA_PMD, mcast_val, lane));
               if (flags == 0 || PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
                   if (ip == SESTO_FALCON_CORE) {
                       SESTO_IF_ERR_RETURN(
                           falcon_furia_sesto_tx_prbs_en(pa, enable));
                   } else {
                       SESTO_IF_ERR_RETURN(
                            merlin_sesto_tx_prbs_en(pa, enable));
                   }
               }
               if (PHYMOD_PRBS_DIRECTION_RX_GET(flags) || flags == 0) {
                    /* Enabling PRBS CHECKER SELF SYNC HYS*/
                   if (ip == SESTO_FALCON_CORE) {
                       if (enable) {
                           SESTO_IF_ERR_RETURN (
	                          wr_falcon_furia_sesto_prbs_chk_clk_en_frc_on(1));
                           /* Check for digital loopback enable or not */
                           SESTO_CHIP_FIELD_READ(pa, F25G_TLB_RX_DIG_LPBK_CONFIG,
                                      dig_lpbk_en, dig_lpbk_en);
                           /* Do not enable prbs_chk_en_auto_mode when digital lpbk is set */
                           if (!dig_lpbk_en) {
                               SESTO_IF_ERR_RETURN (
                                  wr_falcon_furia_sesto_prbs_chk_en_auto_mode(1));
                           }
                       }
                       SESTO_IF_ERR_RETURN(
                            falcon_furia_sesto_rx_prbs_en(pa, enable));
                       if (!enable) {
                           SESTO_IF_ERR_RETURN (
	                          wr_falcon_furia_sesto_prbs_chk_clk_en_frc_on(0));
                           SESTO_IF_ERR_RETURN (
                              wr_falcon_furia_sesto_prbs_chk_en_auto_mode(0));
                       }
                   } else {
                       if (enable) {
                           SESTO_IF_ERR_RETURN (
                                wr_prbs_chk_clk_en_frc_on(1));
                           /* Check for digital loopback enable or not */
                           SESTO_CHIP_FIELD_READ(pa, M10G_TLB_RX_DIG_LPBK_CONFIG,
                                      dig_lpbk_en, dig_lpbk_en);
                           /* Do not enable prbs_chk_en_auto_mode when digital lpbk is set */
                           if (!dig_lpbk_en) {
                               SESTO_IF_ERR_RETURN (
                                    wr_prbs_chk_en_auto_mode(1));
                           } 
                       }
                       SESTO_IF_ERR_RETURN (
                            merlin_sesto_rx_prbs_en(pa, enable));
                       if (!enable) {
                           SESTO_IF_ERR_RETURN (
	                          wr_prbs_chk_clk_en_frc_on(0));
                           SESTO_IF_ERR_RETURN (
                                wr_prbs_chk_en_auto_mode(0));
                       }
                   }
               }
               if (SESTO_IS_MULTI_BROAD_CAST_SET(cast_type)) {
                   break;
               }
            }
        }
    } else {
        PHYMOD_DEBUG_VERBOSE(("Lane mask not Fine \n"));
        return PHYMOD_E_PARAM;
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_prbs_enable_get(const phymod_phy_access_t* phy, uint32_t flags, uint32_t *enable)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0, lane_mask = 0;
    uint8_t gen_en = 0, ckr_en = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("%s :: IP:%s Max_lane:%d lanemask:0x%x\n", __func__,
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));

    for (lane = 0; lane < max_lane; lane ++) {
        if ((lane_mask & (1 << lane))) {
            SESTO_IF_ERR_RETURN (
                _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                     SESTO_DEV_PMA_PMD, 0, lane));
            if (flags == 0 || PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
                if (ip == SESTO_FALCON_CORE) {
                    SESTO_IF_ERR_RETURN(
                          falcon_furia_sesto_get_tx_prbs_en(pa, &gen_en));
                    *enable = gen_en;
                } else {
                    SESTO_IF_ERR_RETURN(
                          merlin_sesto_get_tx_prbs_en(pa, &gen_en));
                    *enable = gen_en;
                }
            }
            if (flags == 0 || PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {
                if (ip == SESTO_FALCON_CORE) {
                    SESTO_IF_ERR_RETURN(
                            falcon_furia_sesto_get_rx_prbs_en(pa, &ckr_en));
                    *enable = ckr_en;
                } else {
                    SESTO_IF_ERR_RETURN(
                          merlin_sesto_get_rx_prbs_en(pa, &ckr_en));
                    *enable = ckr_en;
                }
            }
            break;
        }
    }
    if (flags == 0) {
        *enable = ckr_en & gen_en;
    } 

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_prbs_config_get(const phymod_phy_access_t* phy, uint32_t flags , phymod_prbs_t* prbs)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0, lane_mask;
    uint8_t data = 0;
    int rv = PHYMOD_E_NONE;
    enum srds_prbs_checker_mode_enum temp = 0;
    enum srds_prbs_polynomial_enum serdes_poly = 0;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("IP:%s Max_lane:%d lanemask:0x%x\n", 
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));

    for (lane = 0; lane < max_lane; lane ++) {
        if ((lane_mask & (1 << lane))) {
           SESTO_IF_ERR_RETURN (
              _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                   SESTO_DEV_PMA_PMD, 0, lane));

            if (flags == 0 || PHYMOD_PRBS_DIRECTION_TX_GET(flags)) {
                if (ip == SESTO_FALCON_CORE) {
                    SESTO_IF_ERR_RETURN(falcon_furia_sesto_get_tx_prbs_config(pa, &serdes_poly, &data));
                    prbs->invert = data;                                          
                } else { 
                    SESTO_IF_ERR_RETURN(merlin_sesto_get_tx_prbs_config(pa, &serdes_poly, &data));
                    prbs->invert = data;                                          
                }                                                                 
            }                                                                     
            if (flags == 0 || PHYMOD_PRBS_DIRECTION_RX_GET(flags)) {              
                if (ip == SESTO_FALCON_CORE) {                                    
                    SESTO_IF_ERR_RETURN(falcon_furia_sesto_get_rx_prbs_config(pa, &serdes_poly, &temp, &data));
                    prbs->invert = data;                                          
                } else {
                    SESTO_IF_ERR_RETURN(merlin_sesto_get_rx_prbs_config(pa, &serdes_poly, &temp, &data));
                    prbs->invert = data;
                }
            }
            break;
        }
    }

    PHYMOD_DEBUG_VERBOSE(("Checker POLY:%d\n", serdes_poly));
    SESTO_IF_ERR_RETURN (
        _sesto_serdes_prbs_poly_to_phymod_prbs_poly(serdes_poly, &prbs->poly));

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_prbs_status_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_prbs_status_t* prbs_status)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0, lane_mask = 0;
    uint8_t data = 0;
    uint32_t err_cnt = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("%s ::IP:%s Max_lane:%d lanemask:0x%x\n",  __func__,
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));
    prbs_status->prbs_lock = 1;
    prbs_status->prbs_lock_loss = 1;
    for (lane = 0; lane < max_lane; lane ++) {
        if ((lane_mask & (1 << lane))) {
            SESTO_IF_ERR_RETURN (
                _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                     SESTO_DEV_PMA_PMD, 0, lane));
            if (ip == SESTO_FALCON_CORE) {
                SESTO_IF_ERR_RETURN(falcon_furia_sesto_prbs_chk_lock_state(pa, &data));
                prbs_status->prbs_lock &= data;
                SESTO_IF_ERR_RETURN(falcon_furia_sesto_prbs_err_count_state(pa, &err_cnt, &data));
                prbs_status->prbs_lock_loss &= data;
                prbs_status->error_count |= err_cnt;
            } else {
                SESTO_IF_ERR_RETURN(merlin_sesto_prbs_chk_lock_state(pa, &data));
                prbs_status->prbs_lock &= data;
                SESTO_IF_ERR_RETURN(merlin_sesto_prbs_err_count_state(pa, &err_cnt, &data));
                prbs_status->prbs_lock_loss &= data;
                prbs_status->error_count |= err_cnt;
            }
            READ_SESTO_PMA_PMD_REG(pa, 0x8000, data);
        }
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_display_eyescan(const phymod_phy_access_t *phy)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0;
    uint16_t lane_mask = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip)
    max_lane = (ip == SESTO_FALCON_CORE) ? 
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("%s::IP:%s Max_lane:%d lanemask:0x%x\n", __func__,
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));

    SESTO_IF_ERR_RETURN (
        _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip, SESTO_DEV_PMA_PMD, 0, 0));
    if (ip == SESTO_FALCON_CORE) {
        SESTO_IF_ERR_RETURN
            (falcon_furia_sesto_display_core_state_hdr(pa));
        SESTO_IF_ERR_RETURN
            (falcon_furia_sesto_display_core_state_line(pa));
    } else {
        SESTO_IF_ERR_RETURN
            (merlin_sesto_display_core_state_hdr(pa)) ;
        SESTO_IF_ERR_RETURN
            (merlin_sesto_display_core_state_line(pa));
    }

    for (lane = 0; lane < max_lane; lane++) {
        if ((lane_mask & (1 << lane))) {
            SESTO_IF_ERR_RETURN (
                _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                     SESTO_DEV_PMA_PMD, 0, lane));
            
            /* set the core to get the master /slave */
            /* System side always master Line side considered as slave*/
            if (ip == SESTO_FALCON_CORE) {
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_display_lane_state_hdr(pa));
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_display_lane_state(pa)); 
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_display_eye_scan(pa));
            } else {
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_display_lane_state_hdr(pa));
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_display_lane_state(pa)); 
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_display_eye_scan(pa));
            }
        }
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_core_diagnostics_get(const phymod_core_access_t* core, phymod_core_diagnostics_t* diag)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    int16_t temp = 0;
    phymod_phy_access_t phy;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &core->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    PHYMOD_MEMCPY(&phy, core, sizeof(phymod_core_access_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(&phy, 0, &config));

    SESTO_GET_IP(core, config, ip);
    PHYMOD_DEBUG_VERBOSE(("%s::IP:%s \n", __func__,(ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON"));

    if (ip == SESTO_FALCON_CORE) { /* Falcon core status dump */
        SESTO_IF_ERR_RETURN(
            _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST,
                SESTO_FALCON_CORE, SESTO_DEV_PMA_PMD, 0, 0));
        SESTO_IF_ERR_RETURN(falcon_furia_sesto_read_die_temperature(pa, &temp));
        diag->temperature = temp;
        SESTO_IF_ERR_RETURN(_sesto_falcon_read_pll_range(&core->access, &diag->pll_range));
    } else {  /* Merlin core status dump */
        SESTO_IF_ERR_RETURN(
            _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST,
                SESTO_MERLIN_CORE, SESTO_DEV_PMA_PMD, 0, 0));
        SESTO_IF_ERR_RETURN(merlin_sesto_read_die_temperature(pa, &temp));
        diag->temperature = temp;
        SESTO_IF_ERR_RETURN(_sesto_merlin_read_pll_range(&core->access, &diag->pll_range));
    }
ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}

int _sesto_phy_diagnostics_get(const phymod_phy_access_t *phy, phymod_phy_diagnostics_t* diag)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0;
    uint16_t lane_mask = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ?
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("%s::IP:%s Max_lane:%d lanemask:0x%x\n", __func__,
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));

    for (lane = 0; lane < max_lane; lane++) {
        if ((lane_mask & (1 << lane))) {
            SESTO_IF_ERR_RETURN (
                _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                     SESTO_DEV_PMA_PMD, 0, lane));
            PHYMOD_DEBUG_VERBOSE(("Phy Diagnostics for Lane:%d \n",lane));
            /* set the core to get the master /slave */
            /* System side always master Line side considered as slave*/
            if (ip == SESTO_FALCON_CORE) {
                falcon_furia_sesto_lane_state_define_st state;
                PHYMOD_MEMSET(&state, 0, sizeof(falcon_furia_sesto_lane_state_define_st));
                SESTO_IF_ERR_RETURN
                        (_falcon_furia_sesto_read_lane_state_define(pa, &state));
                diag->signal_detect = state.sig_det;
                diag->osr_mode = state.osr_mode.tx_rx;
                diag->rx_lock = state.rx_lock;
                diag->tx_ppm = state.tx_ppm;
                diag->clk90_offset = state.clk90;
                diag->clkp1_offset = state.clkp1;
                diag->p1_lvl = state.p1_lvl;
                diag->dfe1_dcd = state.dfe1_dcd;
                diag->dfe2_dcd = state.dfe2_dcd;
                diag->slicer_offset.offset_pe = state.pe;
                diag->slicer_offset.offset_ze = state.ze;
                diag->slicer_offset.offset_me = state.me;
                diag->slicer_offset.offset_po = state.po;
                diag->slicer_offset.offset_zo = state.zo;
                diag->slicer_offset.offset_mo = state.mo;
                diag->eyescan.heye_left = state.heye_left;
                diag->eyescan.heye_right = state.heye_right;
                diag->eyescan.veye_upper = state.veye_upper;
                diag->eyescan.veye_lower = state.veye_lower;
                diag->link_time = state.link_time;
                diag->pf_main = state.pf_main;
                diag->pf_hiz = state.pf_hiz;
                diag->pf_bst = state.pf_bst;
                diag->pf_low = 0; /* Not Availble in serdes API*/
                diag->pf2_ctrl = state.pf2_ctrl;
                diag->vga = state.vga;
                diag->dc_offset = state.dc_offset;
                diag->p1_lvl_ctrl = state.p1_lvl_ctrl;
                diag->dfe1 = state.dfe1;
                diag->dfe2 = state.dfe2;
                diag->dfe3 = state.dfe3;
                diag->dfe4 = state.dfe4;
                diag->dfe5 = state.dfe5;
                diag->dfe6 = state.dfe6;
                diag->txfir_pre = state.txfir_pre;
                diag->txfir_main = state.txfir_main;
                diag->txfir_post1 = state.txfir_post1;
                diag->txfir_post2 = state.txfir_post2;
                diag->txfir_post3 = state.txfir_post3;
                diag->tx_amp_ctrl = 0; /* Not Availble in serdes API*/
                diag->br_pd_en = state.br_pd_en;
                break;
            } else {
                merlin_sesto_lane_state_define_st state;
                SESTO_IF_ERR_RETURN
                    (_merlin_sesto_read_lane_state_define(pa, &state));
                diag->signal_detect = state.sig_det;
                diag->osr_mode = state.osr_mode.tx_rx;
                diag->rx_lock = state.rx_lock;
                diag->tx_ppm = state.tx_ppm;
                diag->clk90_offset = state.clk90;
                diag->clkp1_offset = state.clkp1;
                diag->p1_lvl = state.p1_lvl;
                diag->dfe1_dcd = state.dfe1_dcd;
                diag->dfe2_dcd = state.dfe2_dcd;
                diag->slicer_offset.offset_pe = state.pe;
                diag->slicer_offset.offset_ze = state.ze;
                diag->slicer_offset.offset_me = state.me;
                diag->slicer_offset.offset_po = state.po;
                diag->slicer_offset.offset_zo = state.zo;
                diag->slicer_offset.offset_mo = state.mo;
                diag->eyescan.heye_left = state.heye_left;
                diag->eyescan.heye_right = state.heye_right;
                diag->eyescan.veye_upper = state.veye_upper;
                diag->eyescan.veye_lower = state.veye_lower;
                diag->link_time = state.link_time;
                diag->pf_main = state.pf_main;
                diag->pf_hiz = state.pf_hiz;
                diag->pf_bst = state.pf_bst;
                diag->pf_low = 0; /* Not Availble in serdes API*/
                diag->pf2_ctrl = state.pf2_ctrl;
                diag->vga = state.vga;
                diag->dc_offset = state.dc_offset;
                diag->p1_lvl_ctrl = state.p1_lvl_ctrl;
                diag->dfe1 = state.dfe1;
                diag->dfe2 = state.dfe2;
                diag->dfe3 = state.dfe3;
                diag->dfe4 = state.dfe4;
                diag->dfe5 = state.dfe5;
                diag->dfe6 = state.dfe6;
                diag->txfir_pre = state.txfir_pre;
                diag->txfir_main = state.txfir_main;
                diag->txfir_post1 = state.txfir_post1;
                diag->txfir_post2 = state.txfir_post2;
                diag->txfir_post3 = state.txfir_post3;
                diag->tx_amp_ctrl = 0; /* Not Availble in serdes API*/
                diag->br_pd_en = state.br_pd_en;
                break;
            }
        }
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}
int _sesto_ber_proj_lane_speed_get(phymod_phy_inf_config_t *config, uint16_t ip, uint32_t *speed)
{
    SESTO_DEVICE_AUX_MODE_T  *aux;
    aux = (SESTO_DEVICE_AUX_MODE_T*) config->device_aux_modes;

    if (ip == SESTO_FALCON_CORE) {
        if (config->data_rate == SESTO_SPD_100G || config->data_rate == SESTO_SPD_106G) {
            *speed = 25781250;
            if (PHYMOD_INTF_MODES_OTN_GET(config)) {
                *speed = 27952500;
            } else if (PHYMOD_INTF_MODES_HIGIG_GET(config)) {
                *speed = 27343750;
            }
        } else if ((config->data_rate == SESTO_SPD_40G || config->data_rate == SESTO_SPD_42G) && (!aux->pass_thru)) {
            *speed = 20625000;
            if (PHYMOD_INTF_MODES_HIGIG_GET(config)) {
                *speed = 21875000;
            }
        } else if ((config->data_rate == SESTO_SPD_20G || config->data_rate == SESTO_SPD_21G) && (!aux->pass_thru)) {
            *speed = 20625000;
            if (PHYMOD_INTF_MODES_HIGIG_GET(config)) {
                *speed = 21875000;
            }
        } else {
            *speed = 10312500;
            if (PHYMOD_INTF_MODES_OTN_GET(config)) {
                *speed = 10937500;
            } else if (PHYMOD_INTF_MODES_HIGIG_GET(config)) {
                *speed = 11180000;
            }
        }
    } else {
            *speed = 10312500;
            if (PHYMOD_INTF_MODES_OTN_GET(config)) {
                *speed = 10937500;
            } else if (PHYMOD_INTF_MODES_HIGIG_GET(config)) {
                *speed = 11180000;
            }
    }

    return PHYMOD_E_NONE;
}
int _sesto_ber_proj(const phymod_phy_access_t *phy, const phymod_phy_eyescan_options_t* eyescan_options)
{
    phymod_phy_inf_config_t config;
    uint16_t ip = 0;
    uint16_t lane = 0, max_lane = 0;
    uint16_t lane_mask = 0;
    int rv = PHYMOD_E_NONE;
    const phymod_access_t *pa = &phy->access;
#ifdef SERDES_API_FLOATING_POINT
    uint32_t speed;
    USR_DOUBLE speedD;
#endif

    PHYMOD_MEMSET(&config, 0, sizeof(phymod_phy_inf_config_t));
    config.device_aux_modes = PHYMOD_MALLOC(sizeof(SESTO_DEVICE_AUX_MODE_T), "sesto_device_aux_mode");
    SESTO_IF_ERR_RETURN(
       _sesto_phy_interface_config_get(phy, 0, &config));

    SESTO_GET_IP(phy, config, ip);
    max_lane = (ip == SESTO_FALCON_CORE) ?
                   SESTO_MAX_FALCON_LANE : SESTO_MAX_MERLIN_LANE;

    lane_mask = PHYMOD_ACC_LANE_MASK(pa);
    PHYMOD_DEBUG_VERBOSE(("%s::IP:%s Max_lane:%d lanemask:0x%x\n", __func__,
                (ip == SESTO_MERLIN_CORE)?"MERLIN":"FALCON", max_lane, lane_mask));

    SESTO_IF_ERR_RETURN (
        _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip, SESTO_DEV_PMA_PMD, 0, 0));
    if (ip == SESTO_FALCON_CORE) {
        SESTO_IF_ERR_RETURN
            (falcon_furia_sesto_display_core_state_hdr(pa));
        SESTO_IF_ERR_RETURN
            (falcon_furia_sesto_display_core_state_line(pa));
    } else {
        SESTO_IF_ERR_RETURN
            (merlin_sesto_display_core_state_hdr(pa)) ;
        SESTO_IF_ERR_RETURN
            (merlin_sesto_display_core_state_line(pa));
    }

    for (lane = 0; lane < max_lane; lane++) {
        if ((lane_mask & (1 << lane))) {
            SESTO_IF_ERR_RETURN (
                _sesto_set_slice_reg (pa, SESTO_SLICE_UNICAST, ip,
                     SESTO_DEV_PMA_PMD, 0, lane));

            PHYMOD_DEBUG_VERBOSE(("eye_margin_proj for Lane:%d \n",lane));
            if (ip == SESTO_FALCON_CORE) {
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_display_lane_state_hdr(pa));
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_display_lane_state(pa));
#ifdef SERDES_API_FLOATING_POINT
                SESTO_IF_ERR_RETURN
                    (_sesto_ber_proj_lane_speed_get(&config, ip, &speed));
                speedD = speed * 1000.0;
                SESTO_IF_ERR_RETURN
                    (falcon_furia_sesto_eye_margin_proj(pa, speedD, eyescan_options->ber_proj_scan_mode,
                               eyescan_options->ber_proj_timer_cnt, eyescan_options->ber_proj_err_cnt));
#else
                PHYMOD_DIAG_OUT(("BER PROJ needs SERDES_API_FLOATING_POINT define to operate\n"));
                rv = PHYMOD_E_RESOURCE;
#endif

            } else {
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_display_lane_state_hdr(pa));
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_display_lane_state(pa));
#ifdef SERDES_API_FLOATING_POINT
                SESTO_IF_ERR_RETURN
                    (_sesto_ber_proj_lane_speed_get(&config, ip, &speed));
                speedD = speed * 1000.0;
                SESTO_IF_ERR_RETURN
                    (merlin_sesto_eye_margin_proj(pa, speedD, eyescan_options->ber_proj_scan_mode,
                               eyescan_options->ber_proj_timer_cnt, eyescan_options->ber_proj_err_cnt));
                /*Work around for PRBS Locked with Errors */
#else
                PHYMOD_DIAG_OUT(("BER PROJ needs SERDES_API_FLOATING_POINT define to operate\n"));
                rv = PHYMOD_E_RESOURCE;
#endif
                SESTO_IF_ERR_RETURN
                    (wr_trnsum_error_count_en(0));
            }
        }
    }

ERR:
    SESTO_RESET_SLICE(pa, SESTO_DEV_PMA_PMD);
    PHYMOD_FREE(config.device_aux_modes);

    return rv;
}
