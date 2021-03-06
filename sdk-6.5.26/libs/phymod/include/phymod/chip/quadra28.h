/*
 *         
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *     
 * DO NOT EDIT THIS FILE!
 *
 */

#ifndef _QUADRA28_H__H_
#define _QUADRA28_H__H_

#include <phymod/phymod_definitions.h>
/*Initialize phymod module*/
int quadra28_core_identify(const phymod_core_access_t* core, uint32_t core_id, uint32_t* is_identified);

/*Retrive core information*/
int quadra28_core_info_get(const phymod_core_access_t* core, phymod_core_info_t* info);

/*Set\get lane mapping*/
int quadra28_core_lane_map_set(const phymod_core_access_t* core, const phymod_lane_map_t* lane_map);
int quadra28_core_lane_map_get(const phymod_core_access_t* core, phymod_lane_map_t* lane_map);

/*Reset Core*/
int quadra28_core_reset_set(const phymod_core_access_t* core, phymod_reset_mode_t reset_mode, phymod_reset_direction_t direction);
int quadra28_core_reset_get(const phymod_core_access_t* core, phymod_reset_mode_t reset_mode, phymod_reset_direction_t* direction);

/*Retrive firmware information*/
int quadra28_core_firmware_info_get(const phymod_core_access_t* core, phymod_core_firmware_info_t* fw_info);

/*Set\get firmware operation mode*/
int quadra28_phy_firmware_core_config_set(const phymod_phy_access_t* phy, phymod_firmware_core_config_t fw_core_config);
int quadra28_phy_firmware_core_config_get(const phymod_phy_access_t* phy, phymod_firmware_core_config_t* fw_core_config);

/*Set\get firmware operation mode*/
int quadra28_phy_firmware_lane_config_set(const phymod_phy_access_t* phy, phymod_firmware_lane_config_t fw_lane_config);
int quadra28_phy_firmware_lane_config_get(const phymod_phy_access_t* phy, phymod_firmware_lane_config_t* fw_lane_config);

/*Start\Stop the sequencer*/
int quadra28_core_pll_sequencer_restart(const phymod_core_access_t* core, uint32_t flags, phymod_sequencer_operation_t operation);

/* re-tune rx path*/
int quadra28_phy_rx_restart(const phymod_phy_access_t* phy);

/*Set phy polarity*/
int quadra28_phy_polarity_set(const phymod_phy_access_t* phy, const phymod_polarity_t* polarity);
int quadra28_phy_polarity_get(const phymod_phy_access_t* phy, phymod_polarity_t* polarity);

/*Set\Get TX Parameters*/
int quadra28_phy_tx_set(const phymod_phy_access_t* phy, const phymod_tx_t* tx);
int quadra28_phy_tx_get(const phymod_phy_access_t* phy, phymod_tx_t* tx);

/*Request for default TX parameters configuration per media type*/
int quadra28_phy_media_type_tx_get(const phymod_phy_access_t* phy, phymod_media_typed_t media, phymod_tx_t* tx);

/*Set\Get RX Parameters*/
int quadra28_phy_rx_set(const phymod_phy_access_t* phy, const phymod_rx_t* rx);
int quadra28_phy_rx_get(const phymod_phy_access_t* phy, phymod_rx_t* rx);

/*PHY Rx adaptation resume*/
int quadra28_phy_rx_adaptation_resume(const phymod_phy_access_t* phy);

/*Reset phy*/
int quadra28_phy_reset_set(const phymod_phy_access_t* phy, const phymod_phy_reset_t* reset);
int quadra28_phy_reset_get(const phymod_phy_access_t* phy, phymod_phy_reset_t* reset);

/*Control phy power*/
int quadra28_phy_power_set(const phymod_phy_access_t* phy, const phymod_phy_power_t* power);
int quadra28_phy_power_get(const phymod_phy_access_t* phy, phymod_phy_power_t* power);

/*TX transmission control*/
int quadra28_phy_tx_lane_control_set(const phymod_phy_access_t* phy, phymod_phy_tx_lane_control_t tx_control);
int quadra28_phy_tx_lane_control_get(const phymod_phy_access_t* phy, phymod_phy_tx_lane_control_t* tx_control);

/*Rx control*/
int quadra28_phy_rx_lane_control_set(const phymod_phy_access_t* phy, phymod_phy_rx_lane_control_t rx_control);
int quadra28_phy_rx_lane_control_get(const phymod_phy_access_t* phy, phymod_phy_rx_lane_control_t* rx_control);

/*forced speed FEC control*/
int quadra28_phy_fec_enable_set(const phymod_phy_access_t* phy, uint32_t enable);
int quadra28_phy_fec_enable_get(const phymod_phy_access_t* phy, uint32_t* enable);

/*Set the interface mode, speed and other configuration related to interface.*/
int quadra28_phy_interface_config_set(const phymod_phy_access_t* phy, uint32_t flags, const phymod_phy_inf_config_t* config);
int quadra28_phy_interface_config_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_ref_clk_t ref_clock, phymod_phy_inf_config_t* config);

/*Set\Get CL72*/
int quadra28_phy_cl72_set(const phymod_phy_access_t* phy, uint32_t cl72_en);
int quadra28_phy_cl72_get(const phymod_phy_access_t* phy, uint32_t* cl72_en);

/*Get CL72 status*/
int quadra28_phy_cl72_status_get(const phymod_phy_access_t* phy, phymod_cl72_status_t* status);

/*Set\Get autoneg*/
int quadra28_phy_autoneg_ability_set(const phymod_phy_access_t* phy, const phymod_autoneg_ability_t* an_ability_set_type);
int quadra28_phy_autoneg_ability_get(const phymod_phy_access_t* phy, phymod_autoneg_ability_t* an_ability_get_type);

/*Get  remote link autoneg*/
int quadra28_phy_autoneg_remote_ability_get(const phymod_phy_access_t* phy, phymod_autoneg_ability_t* an_ability_get_type);

/*Set\Get autoneg*/
int quadra28_phy_autoneg_set(const phymod_phy_access_t* phy, const phymod_autoneg_control_t* an);
int quadra28_phy_autoneg_get(const phymod_phy_access_t* phy, phymod_autoneg_control_t* an, uint32_t* an_done);

/*Core Initialization*/
int quadra28_core_init(const phymod_core_access_t* core, const phymod_core_init_config_t* init_config, const phymod_core_status_t* core_status);

/*Phy Initialization*/
int quadra28_phy_init(const phymod_phy_access_t* phy, const phymod_phy_init_config_t* init_config);

/*Set\get loopback mode*/
int quadra28_phy_loopback_set(const phymod_phy_access_t* phy, phymod_loopback_mode_t loopback, uint32_t enable);
int quadra28_phy_loopback_get(const phymod_phy_access_t* phy, phymod_loopback_mode_t loopback, uint32_t* enable);

/*Get rx pmd locked indication. This API will clear the pmd_lock_change indication.*/
int quadra28_phy_rx_pmd_locked_get(const phymod_phy_access_t* phy, uint32_t* rx_pmd_locked);

/*Get link up status indication*/
int quadra28_phy_link_status_get(const phymod_phy_access_t* phy, uint32_t* link_status);

/*Get the serdes status*/
int quadra28_phy_status_dump(const phymod_phy_access_t* phy);

/*Read phymod register*/
int quadra28_phy_reg_read(const phymod_phy_access_t* phy, uint32_t reg_addr, uint32_t* val);

/*Write phymod register*/
int quadra28_phy_reg_write(const phymod_phy_access_t* phy, uint32_t reg_addr, uint32_t val);

/*Read data from I2C device attached to PHY*/
int quadra28_phy_i2c_read(const phymod_phy_access_t* phy, uint32_t flags, uint32_t addr, uint32_t offset, uint32_t size, uint8_t* data);

/*Write data to I2C device attached to PHY*/
int quadra28_phy_i2c_write(const phymod_phy_access_t* phy, uint32_t flags, uint32_t addr, uint32_t offset, uint32_t size, const uint8_t* data);

/*Set/Get the configuration of a PHY GPIO pin*/
int quadra28_phy_gpio_config_set(const phymod_phy_access_t* phy, int pin_no, phymod_gpio_mode_t gpio_mode);
int quadra28_phy_gpio_config_get(const phymod_phy_access_t* phy, int pin_no, phymod_gpio_mode_t* gpio_mode);

/*Set/Get the output/input value of a PHY GPIO pin*/
int quadra28_phy_gpio_pin_value_set(const phymod_phy_access_t* phy, int pin_no, int value);
int quadra28_phy_gpio_pin_value_get(const phymod_phy_access_t* phy, int pin_no, int* value);

/*Set/get EDC mode*/
int quadra28_edc_config_set(const phymod_phy_access_t* phy, const phymod_edc_config_t* edc_config);
int quadra28_edc_config_get(const phymod_phy_access_t* phy, phymod_edc_config_t* edc_config);

/*Set/get failover mode */
int quadra28_failover_mode_set(const phymod_phy_access_t* phy, phymod_failover_mode_t failover_mode);
int quadra28_failover_mode_get(const phymod_phy_access_t* phy, phymod_failover_mode_t* failover_mode);

/*phy multi data get*/
int quadra28_phy_multi_get(const phymod_phy_access_t* phy, phymod_multi_data_t* multi_data);

/* get phy working operation mode*/
int quadra28_phy_op_mode_get(const phymod_phy_access_t* phy, phymod_operation_mode_t* op_mode);

#endif /*_QUADRA28_H_*/
