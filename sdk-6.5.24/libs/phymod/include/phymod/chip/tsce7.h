/*
 *         
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *         
 *     
 * DO NOT EDIT THIS FILE!
 */

#ifndef _TSCE7_H__H_
#define _TSCE7_H__H_

#include <phymod/phymod_definitions.h>
/*Initialize phymod module*/
int tsce7_core_identify(const phymod_core_access_t* core, uint32_t core_id, uint32_t* is_identified);

/*Retrive core information*/
int tsce7_core_info_get(const phymod_core_access_t* core, phymod_core_info_t* info);

/*Set\get lane mapping*/
int tsce7_core_lane_map_set(const phymod_core_access_t* core, const phymod_lane_map_t* lane_map);
int tsce7_core_lane_map_get(const phymod_core_access_t* core, phymod_lane_map_t* lane_map);

/*Set\get firmware operation mode*/
int tsce7_phy_firmware_core_config_set(const phymod_phy_access_t* phy, phymod_firmware_core_config_t fw_core_config);
int tsce7_phy_firmware_core_config_get(const phymod_phy_access_t* phy, phymod_firmware_core_config_t* fw_core_config);

/*Set\get firmware operation mode*/
int tsce7_phy_firmware_lane_config_set(const phymod_phy_access_t* phy, phymod_firmware_lane_config_t fw_lane_config);
int tsce7_phy_firmware_lane_config_get(const phymod_phy_access_t* phy, phymod_firmware_lane_config_t* fw_lane_config);

/*Set phy polarity*/
int tsce7_phy_polarity_set(const phymod_phy_access_t* phy, const phymod_polarity_t* polarity);
int tsce7_phy_polarity_get(const phymod_phy_access_t* phy, phymod_polarity_t* polarity);

/*Set\Get TX Parameters*/
int tsce7_phy_tx_set(const phymod_phy_access_t* phy, const phymod_tx_t* tx);
int tsce7_phy_tx_get(const phymod_phy_access_t* phy, phymod_tx_t* tx);

/*Control phy power*/
int tsce7_phy_power_set(const phymod_phy_access_t* phy, const phymod_phy_power_t* power);
int tsce7_phy_power_get(const phymod_phy_access_t* phy, phymod_phy_power_t* power);

/*TX transmission control*/
int tsce7_phy_tx_lane_control_set(const phymod_phy_access_t* phy, phymod_phy_tx_lane_control_t tx_control);
int tsce7_phy_tx_lane_control_get(const phymod_phy_access_t* phy, phymod_phy_tx_lane_control_t* tx_control);

/*Rx control*/
int tsce7_phy_rx_lane_control_set(const phymod_phy_access_t* phy, phymod_phy_rx_lane_control_t rx_control);
int tsce7_phy_rx_lane_control_get(const phymod_phy_access_t* phy, phymod_phy_rx_lane_control_t* rx_control);

/*forced speed FEC control*/
int tsce7_phy_fec_enable_set(const phymod_phy_access_t* phy, uint32_t enable);
int tsce7_phy_fec_enable_get(const phymod_phy_access_t* phy, uint32_t* enable);

/*energy efficient control*/
int tsce7_phy_eee_set(const phymod_phy_access_t* phy, uint32_t enable);
int tsce7_phy_eee_get(const phymod_phy_access_t* phy, uint32_t* enable);

/*Set the interface mode, speed and other configuration related to interface.*/
int tsce7_phy_interface_config_set(const phymod_phy_access_t* phy, uint32_t flags, const phymod_phy_inf_config_t* config);
int tsce7_phy_interface_config_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_ref_clk_t ref_clock, phymod_phy_inf_config_t* config);

/*Set\Get CL72*/
int tsce7_phy_cl72_set(const phymod_phy_access_t* phy, uint32_t cl72_en);
int tsce7_phy_cl72_get(const phymod_phy_access_t* phy, uint32_t* cl72_en);

/*Get CL72 status*/
int tsce7_phy_cl72_status_get(const phymod_phy_access_t* phy, phymod_cl72_status_t* status);

/*Set\Get autoneg*/
int tsce7_phy_autoneg_ability_set(const phymod_phy_access_t* phy, const phymod_autoneg_ability_t* an_ability_set_type);
int tsce7_phy_autoneg_ability_get(const phymod_phy_access_t* phy, phymod_autoneg_ability_t* an_ability_get_type);

/*Get  remote link autoneg*/
int tsce7_phy_autoneg_remote_ability_get(const phymod_phy_access_t* phy, phymod_autoneg_ability_t* an_ability_get_type);

/*Set\Get autoneg*/
int tsce7_phy_autoneg_set(const phymod_phy_access_t* phy, const phymod_autoneg_control_t* an);
int tsce7_phy_autoneg_get(const phymod_phy_access_t* phy, phymod_autoneg_control_t* an, uint32_t* an_done);

/*Core Initialization*/
int tsce7_core_init(const phymod_core_access_t* core, const phymod_core_init_config_t* init_config, const phymod_core_status_t* core_status);

/*Core vco freq get function*/
int tsce7_phy_pll_multiplier_get(const phymod_phy_access_t* phy, uint32_t* core_vco_pll_multiplier);

/*Phy Initialization*/
int tsce7_phy_init(const phymod_phy_access_t* phy, const phymod_phy_init_config_t* init_config);

/*Set\get loopback mode*/
int tsce7_phy_loopback_set(const phymod_phy_access_t* phy, phymod_loopback_mode_t loopback, uint32_t enable);
int tsce7_phy_loopback_get(const phymod_phy_access_t* phy, phymod_loopback_mode_t loopback, uint32_t* enable);

/*Get rx pmd locked indication. This API will clear the pmd_lock_change indication.*/
int tsce7_phy_rx_pmd_locked_get(const phymod_phy_access_t* phy, uint32_t* rx_pmd_locked);

/*Get link up status indication*/
int tsce7_phy_link_status_get(const phymod_phy_access_t* phy, uint32_t* link_status);

/*Read phymod register*/
int tsce7_phy_reg_read(const phymod_phy_access_t* phy, uint32_t reg_addr, uint32_t* val);

/*Write phymod register*/
int tsce7_phy_reg_write(const phymod_phy_access_t* phy, uint32_t reg_addr, uint32_t val);

/* Set/Get synce clock config*/
int tsce7_phy_synce_clk_ctrl_set(const phymod_phy_access_t* phy, phymod_synce_clk_ctrl_t cfg);
int tsce7_phy_synce_clk_ctrl_get(const phymod_phy_access_t* phy, phymod_synce_clk_ctrl_t* cfg);

/*Get rx ppm*/
int tsce7_phy_rx_ppm_get(const phymod_phy_access_t* phy, int16_t* rx_ppm);

#endif /*_TSCE7_H_*/
