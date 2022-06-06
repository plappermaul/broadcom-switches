/*
 * $Id: mbcm.c,v 1.27 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        fe1600_mbcm.c
 * Purpose:     Init the bcm multiplexing function table for FE1600
 *
 */


#include <soc/dfe/cmn/mbcm.h>
/* The header files containing the declerations of the functions mbcm_dfe calls */
#ifdef BCM_88750_A0
#include <soc/dcmn/dcmn_captured_buffer.h>
#include <soc/dcmn/dcmn_cells_buffer.h>
#include <soc/dfe/fe1600/fe1600_drv.h>
#include <soc/dfe/fe1600/fe1600_fabric_cell.h>
#include <soc/dfe/fe1600/fe1600_fabric_cell_snake_test.h>
#include <soc/dfe/fe1600/fe1600_fabric_flow_control.h>
#include <soc/dfe/fe1600/fe1600_fabric_links.h>
#include <soc/dfe/fe1600/fe1600_fabric_multicast.h>
#include <soc/dfe/fe1600/fe1600_fabric_status.h>
#include <soc/dfe/fe1600/fe1600_fabric_topology.h>
#include <soc/dfe/fe1600/fe1600_interrupts.h>
#include <soc/dfe/fe1600/fe1600_multicast.h>
#include <soc/dfe/fe1600/fe1600_port.h>
#include <soc/dfe/fe1600/fe1600_stack.h>
#include <soc/dfe/fe1600/fe1600_stat.h>
#include <soc/dfe/fe1600/fe1600_link.h>
#include <soc/dfe/fe1600/fe1600_defs.h>
#include <soc/dfe/fe1600/fe1600_property.h>
#include <soc/dfe/fe1600/fe1600_warm_boot.h>
#include <soc/dfe/fe1600/fe1600_diag.h>

mbcm_dfe_functions_t mbcm_fe1600_driver = {
    soc_fe1600_tbl_is_dynamic,
    soc_fe1600_ser_init,
    soc_fe1600_reset_device,
    soc_fe1600_drv_soft_init,
    soc_fe1600_drv_blocks_reset,
    NULL, /*mbcm_dfe_drv_reg_access_only_reset*/
    soc_fe1600_TDM_fragment_validate,
    soc_fe1600_sr_cell_send,
    soc_fe1600_cell_filter_set,
    soc_fe1600_cell_filter_clear,
    soc_fe1600_cell_filter_count_get,
    soc_fe1600_control_cell_filter_set,
    soc_fe1600_control_cell_filter_clear,
    soc_fe1600_control_cell_filter_receive,
    soc_fe1600_cell_snake_test_prepare,
    soc_fe1600_cell_snake_test_run,
    soc_fe1600_diag_fabric_cell_snake_test_interrupts_name_get,
    NULL, /*mbcm_dfe_diag_cell_pipe_counter_get*/
    NULL, /*mbcm_dfe_diag_mesh_topology_get*/
    soc_fe1600_drv_graceful_shutdown_set,
    soc_fe1600_drv_fe13_graceful_shutdown_set,
    soc_fe1600_drv_fe13_isolate_set,
    soc_fe1600_drv_soc_properties_validate,
    soc_fe1600_drv_mbist,
    soc_fe1600_drv_link_to_block_mapping,
    soc_fe1600_drv_block_pbmp_get,
    soc_fe1600_fabric_cell_get,
    soc_fe1600_fabric_cell_type_get,
    soc_fe1600_fabric_cell_parse_table_get,
    soc_fe1600_fabric_cell_is_cell_in_two_parts,
    soc_fe1600_fabric_flow_control_rci_gci_control_source_set,
    soc_fe1600_fabric_flow_control_rci_gci_control_source_get,
    soc_fe1600_fabric_flow_control_thresholds_flags_validate,
    soc_fe1600_fabric_flow_control_rx_llfc_threshold_validate,
    soc_fe1600_fabric_flow_control_rx_llfc_threshold_set,
    soc_fe1600_fabric_flow_control_rx_llfc_threshold_get,
    soc_fe1600_fabric_flow_control_rx_gci_threshold_validate,
    soc_fe1600_fabric_flow_control_rx_gci_threshold_set,
    soc_fe1600_fabric_flow_control_rx_gci_threshold_get,
    NULL, /*mbcm_dfe_fabric_flow_control_rx_rci_threshold_validate */
    NULL, /*mbcm_dfe_fabric_flow_control_rx_rci_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_rci_threshold_get*/
    soc_fe1600_fabric_flow_control_rx_drop_threshold_validate,
    soc_fe1600_fabric_flow_control_rx_drop_threshold_set,
    soc_fe1600_fabric_flow_control_rx_drop_threshold_get,
    NULL, /*mbcm_dfe_fabric_flow_control_rx_full_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_full_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_full_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_fifo_size_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_fifo_size_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_fifo_size_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_multicast_low_prio_drop_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_multicast_low_prio_drop_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_rx_multicast_low_prio_drop_threshold_get*/
    soc_fe1600_fabric_flow_control_tx_rci_threshold_validate,
    soc_fe1600_fabric_flow_control_tx_rci_threshold_set,
    soc_fe1600_fabric_flow_control_tx_rci_threshold_get,
    soc_fe1600_fabric_flow_control_tx_bypass_llfc_threshold_validate,
    soc_fe1600_fabric_flow_control_tx_bypass_llfc_threshold_set,
    soc_fe1600_fabric_flow_control_tx_bypass_llfc_threshold_get,
    soc_fe1600_fabric_flow_control_tx_gci_threshold_validate,
    soc_fe1600_fabric_flow_control_tx_gci_threshold_set,
    soc_fe1600_fabric_flow_control_tx_gci_threshold_get,
    soc_fe1600_fabric_flow_control_tx_drop_threshold_validate,
    soc_fe1600_fabric_flow_control_tx_drop_threshold_set,
    soc_fe1600_fabric_flow_control_tx_drop_threshold_get,
    NULL, /*mbcm_dfe_fabric_flow_control_tx_rci_threshold_fc_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_rci_threshold_fc_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_rci_threshold_fc_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_almost_full_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_almost_full_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_almost_full_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_fifo_size_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_fifo_size_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_tx_fifo_size_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_gci_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_gci_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_gci_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_rci_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_rci_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_rci_thrshold_get */
    NULL, /*mbcm_dfe_fabric_flow_control_mid_prio_drop_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_prio_drop_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_prio_drop_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_almost_full_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_almost_full_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_almost_full_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_fifo_size_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_fifo_size_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_fifo_size_threshold_get*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_full_threshold_validate*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_full_threshold_set*/
    NULL, /*mbcm_dfe_fabric_flow_control_mid_full_threshold_get*/
    soc_fe1600_fabric_links_link_type_set,
    soc_fe1600_fabric_links_link_type_get,
    soc_fe1600_fabric_links_nof_links_get,
    soc_fe1600_fabric_links_validate_link,
    soc_fe1600_fabric_links_isolate_set,
    soc_fe1600_fabric_links_bmp_isolate_set,
    soc_fe1600_fabric_links_isolate_get,
    soc_fe1600_fabric_links_cell_format_verify,
    soc_fe1600_fabric_links_cell_format_set,
    soc_fe1600_fabric_links_cell_format_get,
    NULL, /*mbcm_dfe_fabric_links_flow_status_control_cell_format_set*/
    NULL, /*mbcm_dfe_fabric_links_flow_status_control_cell_format_get*/
    soc_fe1600_fabric_links_cell_interleaving_set,
    soc_fe1600_fabric_links_cell_interleaving_get,
    soc_fe1600_fabric_links_weight_validate,
    soc_fe1600_fabric_links_weight_set,
    soc_fe1600_fabric_links_weight_get,
    soc_fe1600_fabric_links_secondary_only_set,
    soc_fe1600_fabric_links_secondary_only_get,
    soc_fe1600_fabric_links_llf_control_source_set,
    soc_fe1600_fabric_links_llf_control_source_get,
    soc_fe1600_fabric_links_aldwp_config,
    soc_fe1600_fabric_links_aldwp_init,
    NULL, /*mbcm_dfe_fabric_links_pcp_enable_set*/
    soc_fe1600_fabric_links_pcp_enable_get,
    soc_fe1600_fabric_multicast_low_prio_drop_select_priority_set,
    soc_fe1600_fabric_multicast_low_prio_drop_select_priority_get,
    soc_fe1600_fabric_multicast_low_prio_threshold_validate,
    soc_fe1600_fabric_multicast_low_prio_threshold_set,
    soc_fe1600_fabric_multicast_low_prio_threshold_get,
    soc_fe1600_fabric_multicast_low_priority_drop_enable_set,
    soc_fe1600_fabric_multicast_low_priority_drop_enable_get,
    soc_fe1600_fabric_multicast_priority_range_validate,
    soc_fe1600_fabric_multicast_priority_range_set,
    soc_fe1600_fabric_multicast_priority_range_get,
    soc_fe1600_fabric_multicast_multi_set,
    soc_fe1600_fabric_multicast_multi_get,
    soc_fe1600_fabric_multicast_multi_write_range,
    soc_fe1600_fabric_multicast_multi_read_info_get,
    NULL, /*mbcm_dfe_fabric_link_repeater_enable_set*/
    soc_fe1600_fabric_link_repeater_enable_get,
    soc_fe1600_fabric_link_status_all_get,
    soc_fe1600_fabric_link_status_get,
    soc_fe1600_fabric_reachability_status_get,
    soc_fe1600_fabric_link_connectivity_status_get,
    NULL, /*mbcm_dfe_fabric_links_pipe_map_set*/
    NULL, /*mbcm_dfe_fabric_links_pipe_map_get*/
    NULL, /*mbcm_dfe_fabric_links_repeater_nof_remote_pipe_set*/
    NULL, /*mbcm_dfe_fabric_links_repeater_nof_remote_pipe_get*/
    soc_fe1600_fabric_topology_isolate_set,
    soc_fe1600_fabric_topology_isolate_get,
    soc_fe1600_fabric_topology_rmgr_set,
    soc_fe1600_fabric_link_topology_set,
    soc_fe1600_fabric_link_topology_get,
    soc_fe1600_fabric_topology_min_nof_links_set,
    soc_fe1600_fabric_topology_min_nof_links_get,
    soc_fe1600_fabric_topology_nof_links_to_min_nof_links_default,
    soc_fe1600_fabric_topology_repeater_destination_set,
    soc_fe1600_fabric_topology_repeater_destination_get,
    soc_fe1600_fabric_topology_reachability_mask_set,
    NULL, /*mbcm_dfe_fifo_dma_channel_init*/
    NULL, /*mbcm_dfe_fifo_dma_channel_deinit*/
    NULL, /*mbcm_dfe_fifo_dma_channel_clear*/
    NULL, /*mbcm_dfe_fifo_dma_channel_read_entries*/
    NULL, /*mbcm_dfe_fifo_dma_fabric_cell_validate*/
    soc_fe1600_nof_interrupts,
    soc_fe1600_nof_block_instances,
    soc_fe1600_multicast_egress_add,
    soc_fe1600_multicast_egress_delete,
    soc_fe1600_multicast_egress_delete_all,
    soc_fe1600_multicast_egress_get,
    soc_fe1600_multicast_egress_set,
    soc_fe1600_multicast_mode_get,
    soc_fe1600_multicast_table_size_get,
    soc_fe1600_multicast_table_entry_size_get,
    soc_fe1600_port_soc_init,
    soc_fe1600_port_init,
    soc_fe1600_port_deinit,
    soc_fe1600_port_detach,
    soc_fe1600_port_probe,
    soc_fe1600_port_control_pcs_set,
    soc_fe1600_port_control_pcs_get,
    soc_fe1600_port_control_power_set,
    soc_fe1600_port_control_power_get,
    soc_fe1600_port_control_strip_crc_set,
    soc_fe1600_port_control_strip_crc_get,
    soc_fe1600_port_control_rx_enable_set,
    soc_fe1600_port_control_tx_enable_set,
    soc_fe1600_port_control_rx_enable_get,
    soc_fe1600_port_control_tx_enable_get,
    soc_fe1600_port_control_low_latency_set,
    soc_fe1600_port_control_low_latency_get,
    soc_fe1600_port_control_fec_error_detect_set,
    soc_fe1600_port_control_fec_error_detect_get,
    soc_fe1600_port_phy_enable_set,
    soc_fe1600_port_phy_enable_get,
    soc_fe1600_port_phy_cl72_set,
    soc_fe1600_port_phy_cl72_get,
    soc_fe1600_port_phy_control_set,
    soc_fe1600_port_phy_control_get,
    soc_fe1600_port_loopback_set,
    soc_fe1600_port_loopback_get,
    soc_fe1600_port_fault_get,
    soc_fe1600_port_speed_get,
    soc_fe1600_port_speed_max,
    soc_fe1600_port_speed_set,
    NULL, /* mbcm_dfe_port_interface_set */
    soc_fe1600_port_interface_get,
    soc_fe1600_port_serdes_power_disable,
    soc_fe1600_port_link_status_get,
    soc_fe1600_port_bucket_fill_rate_validate,
    soc_fe1600_port_prbs_tx_enable_set,
    soc_fe1600_port_prbs_tx_enable_get,
    soc_fe1600_port_prbs_rx_enable_set,
    soc_fe1600_port_prbs_rx_enable_get,
    soc_fe1600_port_prbs_rx_status_get,
    soc_fe1600_port_prbs_polynomial_set,
    soc_fe1600_port_prbs_polynomial_get,
    soc_fe1600_port_prbs_tx_invert_data_set,
    soc_fe1600_port_prbs_tx_invert_data_get,
    soc_fe1600_port_pump_enable_set,
    soc_fe1600_port_rate_egress_ppt_set,
    soc_fe1600_port_rate_egress_ppt_get,
    soc_fe1600_port_burst_control_set,
    NULL, /*mbcm_dfe_port_extract_cig_from_llfc_enable_set*/
    NULL, /*mbcm_dfe_port_extract_cig_from_llfc_enable_get*/
    soc_fe1600_port_phy_get,
    soc_fe1600_port_phy_set,
    soc_fe1600_port_phy_modify,
    soc_fe1600_port_dynamic_port_update,
    soc_fe1600_port_enable_set,
    soc_fe1600_port_enable_get,
    NULL, /*mbcm_dfe_port_sync_e_link_set*/
    NULL, /*mbcm_dfe_port_sync_e_divider_set*/
    NULL, /*mbcm_dfe_port_sync_e_link_get*/
    NULL, /*mbcm_dfe_port_sync_e_divider_get*/
    soc_fe1600_set_mesh_topology_config,
    soc_fe1600_stk_modid_set,
    soc_fe1600_stk_modid_get,
    NULL, /*mbcm_dfe_stk_module_max_all_reachable_verify*/
    NULL, /*mbcm_dfe_stk_module_max_all_reachable_set*/
    NULL, /*mbcm_dfe_stk_module_max_all_reachable_get*/
    NULL, /*mbcm_dfe_stk_module_max_fap_verify*/
    NULL, /*mbcm_dfe_stk_module_max_fap_set*/
    NULL, /*mbcm_dfe_stk_module_max_fap_get*/
    NULL, /*mbcm_dfe_stk_module_all_reachable_ignore_id_set*/
    NULL, /*mbcm_dfe_stk_module_all_reachable_ignore_id_get*/
    NULL, /*mbcm_dfe_stk_valid_module_id_verify*/
    soc_fe1600_stat_init,
    soc_fe1600_stat_is_supported_type,
    soc_fe1600_stat_counter_length_get,
    soc_fe1600_stat_get,
    soc_fe1600_mapping_stat_get,
    soc_fe1600_counters_get_info,
    soc_fe1600_queues_get_info,
    soc_fe1600_fabric_link_device_mode_get,
    soc_fe1600_controlled_counter_set,
    soc_fe1600_soc_properties_array_get,
    soc_fe1600_avs_value_get,
    soc_fe1600_linkctrl_init,
    soc_fe1600_interrupt_init,
    soc_fe1600_interrupt_deinit,
    fe1600_interrupts_dfe_control_data_init,
    fe1600_interrupts_dfe_control_data_deinit,
    soc_fe1600_interrupt_all_enable_set,
    soc_fe1600_interrupt_all_enable_get,
    soc_fe1600_drv_sw_ver_set,
    soc_fe1600_drv_temperature_monitor_get,
    soc_fe1600_drv_test_reg_filter,
    soc_fe1600_drv_test_reg_default_val_filter,
    soc_fe1600_drv_test_mem_filter,
    soc_fe1600_drv_test_brdc_blk_filter,
    soc_fe1600_drv_test_brdc_blk_info_get,
    NULL,/*mbcm_dfe_drv_asymmetrical_quad_get_f*/
    soc_fe1600_warm_boot_buffer_id_supported_get,
    NULL,/*mbcm_dfe_cosq_pipe_rx_weight_set*/
    NULL,/*mbcm_dfe_cosq_pipe_rx_weight_get*/
    NULL,/*mbcm_dfe_cosq_pipe_mid_weight_set*/
    NULL,/*mbcm_dfe_cosq_pipe_mid_weight_get*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_weight_set*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_weight_get*/
    NULL,/*mbcm_dfe_cosq_pipe_rx_threshold_set*/
    NULL,/*mbcm_dfe_cosq_pipe_rx_threshold_get*/
    NULL,/*mbcm_dfe_cosq_pipe_mid_threshold_set*/
    NULL,/*mbcm_dfe_cosq_pipe_mid_threshold_get*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_threshold_set*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_threshold_get*/
    NULL,/*mbcm_dfe_cosq_pipe_rx_rate_set*/
    NULL,/*mbcm_dfe_cosq_pipe_rx_rate_get*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_rate_set*/
    NULL, /*mbcm_dfe_cosq_pipe_tx_rate_get*/
    NULL,/*mbcm_dfe_cosq_pipe_tx_rate_enable_set*/
    NULL, /*mbcm_dfe_cosq_pipe_tx_rate_enable_get*/
    NULL, /*mbcm_dfe_rx_cpu_address_modid_set*/
    NULL, /*mbcm_dfe_rx_cpu_address_modid_init*/
    soc_fe1600_port_quad_disabled,
    soc_fe1600_drv_block_valid_get
};

#endif /* BCM_88750_A0 */

