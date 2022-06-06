/*
 * $Id: mbcm.c,v 1.1.2.4 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        fe3200_mbcm.c
 * Purpose:     Init the bcm multiplexing function table for FE3200
 *
 */


#include <soc/dfe/cmn/mbcm.h>

#ifdef BCM_88950_A0
#include <soc/dcmn/dcmn_captured_buffer.h>
#include <soc/dcmn/dcmn_cells_buffer.h>
/* 3200 includes */
#include <soc/dfe/fe3200/fe3200_defs.h>
#include <soc/dfe/fe3200/fe3200_stat.h>
#include <soc/dfe/fe3200/fe3200_property.h>
#include <soc/dfe/fe3200/fe3200_stack.h>
#include <soc/dfe/fe3200/fe3200_drv.h>
#include <soc/dfe/fe3200/fe3200_diag.h>
#include <soc/dfe/fe3200/fe3200_link.h>
#include <soc/dfe/fe3200/fe3200_intr.h>
#include <soc/dfe/fe3200/fe3200_fabric_flow_control.h>
#include <soc/dfe/fe3200/fe3200_fabric_links.h>
#include <soc/dfe/fe3200/fe3200_fabric_topology.h>
#include <soc/dfe/fe3200/fe3200_fabric_cell.h>
#include <soc/dfe/fe3200/fe3200_port.h>
#include <soc/dfe/fe3200/fe3200_fabric_cell_snake_test.h>
#include <soc/dfe/fe3200/fe3200_warm_boot.h>
#include <soc/dfe/fe3200/fe3200_fifo_dma.h>
#include <soc/dfe/fe3200/fe3200_cosq.h>
#include <soc/dfe/fe3200/fe3200_multicast.h>
#include <soc/dfe/fe3200/fe3200_rx.h>
#include <soc/dfe/fe3200/fe3200_fabric_status.h>
#include <soc/dfe/fe3200/fe3200_fabric_multicast.h>

/* portmod include */
#include <soc/portmod/portmod.h>
/* 1600 includes */
#include <soc/dfe/fe1600/fe1600_port.h>
#include <soc/dfe/fe1600/fe1600_fabric_topology.h>
#include <soc/dfe/fe1600/fe1600_stat.h>
#include <soc/dfe/fe1600/fe1600_multicast.h>
#include <soc/dfe/fe1600/fe1600_fabric_links.h>
#include <soc/dfe/fe1600/fe1600_fabric_multicast.h>
#include <soc/dfe/fe1600/fe1600_drv.h>
#include <soc/dfe/fe1600/fe1600_fabric_status.h>
#include <soc/dfe/fe1600/fe1600_fabric_cell.h>


mbcm_dfe_functions_t mbcm_fe3200_driver = {
    soc_fe3200_tbl_is_dynamic,
    soc_fe3200_ser_init,
    soc_fe3200_reset_device,
    soc_fe3200_drv_soft_init,
    soc_fe3200_drv_blocks_reset,
    soc_fe3200_drv_reg_access_only_reset,
    NULL,/*mbcm_dfe_TDM_fragment_validate*/
    soc_fe1600_sr_cell_send,
    soc_fe3200_cell_filter_set,
    soc_fe3200_cell_filter_clear,
    soc_fe3200_cell_filter_count_get,
    soc_fe3200_control_cell_filter_set,
    soc_fe3200_control_cell_filter_clear,
    soc_fe3200_control_cell_filter_receive,
    soc_fe3200_cell_snake_test_prepare,
    soc_fe3200_cell_snake_test_run,
    soc_fe3200_diag_fabric_cell_snake_test_interrupts_name_get,
    soc_fe3200_diag_cell_pipe_counter_get,
    soc_fe3200_diag_mesh_topology_get,
    soc_fe3200_drv_graceful_shutdown_set,
    soc_fe3200_drv_fe13_graceful_shutdown_set,
    soc_fe3200_drv_fe13_isolate_set,
    soc_fe3200_drv_soc_properties_validate,
    soc_fe3200_drv_mbist,/*mbcm_dfe_drv_mbist*/
    soc_fe1600_drv_link_to_block_mapping,
    soc_fe3200_drv_block_pbmp_get,
    soc_fe3200_fabric_cell_get,
    soc_fe3200_fabric_cell_type_get,
    soc_fe3200_fabric_cell_parse_table_get,
    soc_fe3200_fabric_cell_is_cell_in_two_parts,
    soc_fe3200_fabric_flow_control_rci_gci_control_source_set,
    soc_fe3200_fabric_flow_control_rci_gci_control_source_get,
    soc_fe3200_fabric_flow_control_thresholds_flags_validate,
    soc_fe3200_fabric_flow_control_rx_llfc_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_llfc_threshold_set,
    soc_fe3200_fabric_flow_control_rx_llfc_threshold_get,
    soc_fe3200_fabric_flow_control_rx_gci_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_gci_threshold_set,
    soc_fe3200_fabric_flow_control_rx_gci_threshold_get,
    soc_fe3200_fabric_flow_control_rx_rci_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_rci_threshold_set,
    soc_fe3200_fabric_flow_control_rx_rci_threshold_get,
    NULL,/*mbcm_dfe_fabric_flow_control_rx_drop_threshold_validate*/
    NULL,/*mbcm_dfe_fabric_flow_control_rx_drop_threshold_set*/
    NULL,/*mbcm_dfe_fabric_flow_control_rx_drop_threshold_get*/
    soc_fe3200_fabric_flow_control_rx_full_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_full_threshold_set,
    soc_fe3200_fabric_flow_control_rx_full_threshold_get,
    soc_fe3200_fabric_flow_control_rx_fifo_size_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_fifo_size_threshold_set,
    soc_fe3200_fabric_flow_control_rx_fifo_size_threshold_get,
    soc_fe3200_fabric_flow_control_rx_multicast_low_prio_drop_threshold_validate,
    soc_fe3200_fabric_flow_control_rx_multicast_low_prio_drop_threshold_set,
    soc_fe3200_fabric_flow_control_rx_multicast_low_prio_drop_threshold_get,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_set,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_get,
    soc_fe3200_fabric_flow_control_tx_bypass_llfc_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_bypass_llfc_threshold_set,
    soc_fe3200_fabric_flow_control_tx_bypass_llfc_threshold_get,
    soc_fe3200_fabric_flow_control_tx_gci_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_gci_threshold_set,
    soc_fe3200_fabric_flow_control_tx_gci_threshold_get,
    soc_fe3200_fabric_flow_control_tx_drop_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_drop_threshold_set,
    soc_fe3200_fabric_flow_control_tx_drop_threshold_get,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_set,
    soc_fe3200_fabric_flow_control_tx_rci_threshold_get,
    soc_fe3200_fabric_flow_control_tx_almost_full_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_almost_full_threshold_set,
    soc_fe3200_fabric_flow_control_tx_almost_full_threshold_get,
    soc_fe3200_fabric_flow_control_tx_fifo_size_threshold_validate,
    soc_fe3200_fabric_flow_control_tx_fifo_size_threshold_set,
    soc_fe3200_fabric_flow_control_tx_fifo_size_threshold_get,
    soc_fe3200_fabric_flow_control_mid_gci_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_gci_threshold_set,
    soc_fe3200_fabric_flow_control_mid_gci_threshold_get,
    soc_fe3200_fabric_flow_control_mid_rci_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_rci_threshold_set,
    soc_fe3200_fabric_flow_control_mid_rci_threshold_get,
    soc_fe3200_fabric_flow_control_mid_prio_drop_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_prio_drop_threshold_set,
    soc_fe3200_fabric_flow_control_mid_prio_drop_threshold_get,
    soc_fe3200_fabric_flow_control_mid_almost_full_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_almost_full_threshold_set,
    soc_fe3200_fabric_flow_control_mid_almost_full_threshold_get,
    soc_fe3200_fabric_flow_control_mid_fifo_size_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_fifo_size_threshold_set,
    soc_fe3200_fabric_flow_control_mid_fifo_size_threshold_get,
    soc_fe3200_fabric_flow_control_mid_full_threshold_validate,
    soc_fe3200_fabric_flow_control_mid_full_threshold_set,
    soc_fe3200_fabric_flow_control_mid_full_threshold_get,
    soc_fe3200_fabric_links_link_type_set,
    soc_fe3200_fabric_links_link_type_get,
    NULL,/*mbcm_dfe_fabric_links_nof_links_get*/
    soc_fe1600_fabric_links_validate_link,
    soc_fe1600_fabric_links_isolate_set,
    soc_fe1600_fabric_links_bmp_isolate_set,
    soc_fe1600_fabric_links_isolate_get,
    soc_fe3200_fabric_links_cell_format_verify,
    NULL, /*mbcm_dfe_fabric_links_cell_format_set*/
    soc_fe3200_fabric_links_cell_format_get,
    soc_fe3200_fabric_links_flow_status_control_cell_format_set,
    soc_fe3200_fabric_links_flow_status_control_cell_format_get,
    NULL, /*mbcm_dfe_fabric_links_cell_interleaving_set*/
    NULL, /*mbcm_dfe_fabric_links_cell_interleaving_get*/
    soc_fe3200_fabric_links_weight_validate,
    soc_fe3200_fabric_links_weight_set,
    soc_fe3200_fabric_links_weight_get,
    soc_fe3200_fabric_links_secondary_only_set,
    soc_fe3200_fabric_links_secondary_only_get,
    soc_fe1600_fabric_links_llf_control_source_set,
    soc_fe1600_fabric_links_llf_control_source_get,
    soc_fe1600_fabric_links_aldwp_config,
    soc_fe1600_fabric_links_aldwp_init,
    soc_fe3200_fabric_links_pcp_enable_set,
    soc_fe3200_fabric_links_pcp_enable_get,
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
    soc_fe3200_fabric_multicast_multi_write_range,
    soc_fe3200_fabric_multicast_multi_read_info_get,
    soc_fe3200_fabric_link_repeater_enable_set,
    soc_fe3200_fabric_link_repeater_enable_get,
    soc_fe1600_fabric_link_status_all_get,
    soc_fe3200_fabric_link_status_get,
    soc_fe1600_fabric_reachability_status_get,
    soc_fe1600_fabric_link_connectivity_status_get,
    soc_fe3200_fabric_links_pipe_map_set,
    soc_fe3200_fabric_links_pipe_map_get,
    soc_fe3200_fabric_links_repeater_nof_remote_pipe_set,
    soc_fe3200_fabric_links_repeater_nof_remote_pipe_get,
    soc_fe3200_fabric_topology_isolate_set,
    soc_fe1600_fabric_topology_isolate_get,
    soc_fe1600_fabric_topology_rmgr_set,
    soc_fe1600_fabric_link_topology_set,
    soc_fe1600_fabric_link_topology_get,
    soc_fe3200_fabric_topology_min_nof_links_set,
    soc_fe3200_fabric_topology_min_nof_links_get,
    soc_fe3200_fabric_topology_nof_links_to_min_nof_links_default,
    soc_fe1600_fabric_topology_repeater_destination_set,
    soc_fe1600_fabric_topology_repeater_destination_get,
    soc_fe3200_fabric_topology_reachability_mask_set,
    soc_fe3200_fifo_dma_channel_init,
    soc_fe3200_fifo_dma_channel_deinit,
    soc_fe3200_fifo_dma_channel_clear,
    soc_fe3200_fifo_dma_channel_read_entries,
    soc_fe3200_fifo_dma_fabric_cell_validate,
    soc_fe3200_nof_interrupts,
    soc_fe3200_nof_block_instances,
    soc_fe1600_multicast_egress_add,
    soc_fe1600_multicast_egress_delete,
    soc_fe1600_multicast_egress_delete_all,
    soc_fe1600_multicast_egress_get,
    soc_fe1600_multicast_egress_set,
    soc_fe3200_multicast_mode_get,
    soc_fe3200_multicast_table_size_get,
    soc_fe3200_multicast_table_entry_size_get,
    soc_fe3200_port_soc_init,
    soc_fe3200_port_init,
    soc_fe3200_port_deinit,
    soc_fe3200_port_detach,
    soc_fe3200_port_probe,
    soc_dcmn_port_control_pcs_set,
    soc_dcmn_port_control_pcs_get,
    soc_dcmn_port_control_power_set,
    soc_dcmn_port_control_power_get,
    NULL,/*mbcm_dfe_port_control_strip_crc_set*/
    NULL,/*mbcm_dfe_port_control_strip_crc_get*/
    NULL,/*mbcm_dfe_port_control_rx_enable_set*/
    NULL,/*mbcm_dfe_port_control_tx_enable_set*/
    NULL,/*mbcm_dfe_port_control_rx_enable_get,*/
    NULL,/*mbcm_dfe_port_control_tx_enable_get*/
    soc_dcmn_port_control_low_latency_llfc_set,
    soc_dcmn_port_control_low_latency_llfc_get,
    soc_dcmn_port_control_fec_error_detect_set,
    soc_dcmn_port_control_fec_error_detect_get,
    soc_fe3200_port_phy_enable_set,
    soc_fe3200_port_phy_enable_get,
    soc_dcmn_port_cl72_set,
    soc_dcmn_port_cl72_get,
    soc_dcmn_port_phy_control_set,
    soc_dcmn_port_phy_control_get,
    soc_dcmn_port_loopback_set,
    soc_dcmn_port_loopback_get,
    soc_fe1600_port_fault_get,
    soc_fe3200_port_speed_get,
    soc_fe3200_port_speed_max,
    soc_fe3200_port_speed_set,
    soc_fe3200_port_interface_set,
    soc_fe3200_port_interface_get,
    soc_fe3200_port_serdes_power_disable,
    soc_fe3200_port_link_status_get,
    soc_fe1600_port_bucket_fill_rate_validate,
    soc_dcmn_port_prbs_tx_enable_set,
    soc_dcmn_port_prbs_tx_enable_get,
    soc_dcmn_port_prbs_rx_enable_set,
    soc_dcmn_port_prbs_rx_enable_get,
    soc_dcmn_port_prbs_rx_status_get,
    soc_dcmn_port_prbs_polynomial_set,
    soc_dcmn_port_prbs_polynomial_get,
    soc_dcmn_port_prbs_tx_invert_data_set,
    soc_dcmn_port_prbs_tx_invert_data_get,
    soc_fe3200_port_pump_enable_set,
    soc_fe1600_port_rate_egress_ppt_set,
    soc_fe1600_port_rate_egress_ppt_get,
    soc_fe3200_port_burst_control_set,
    soc_dcmn_port_extract_cig_from_llfc_enable_set,
    soc_dcmn_port_extract_cig_from_llfc_enable_get,
    soc_dcmn_port_phy_reg_get,
    soc_dcmn_port_phy_reg_set,
    soc_dcmn_port_phy_reg_modify,
    soc_fe1600_port_dynamic_port_update,
    soc_dcmn_port_enable_set,
    soc_dcmn_port_enable_get,
    soc_fe3200_port_sync_e_link_set,
    soc_fe3200_port_sync_e_divider_set,
    soc_fe3200_port_sync_e_link_get,
    soc_fe3200_port_sync_e_divider_get,
    soc_fe1600_set_mesh_topology_config,
    soc_fe3200_stk_modid_set,
    soc_fe3200_stk_modid_get,
    soc_fe3200_stk_module_max_all_reachable_verify,
    soc_fe3200_stk_module_max_all_reachable_set,
    soc_fe3200_stk_module_max_all_reachable_get,
    soc_fe3200_stk_module_max_fap_verify,
    soc_fe3200_stk_module_max_fap_set,
    soc_fe3200_stk_module_max_fap_get,
    soc_fe3200_stk_module_all_reachable_ignore_id_set,
    soc_fe3200_stk_module_all_reachable_ignore_id_get,
    soc_fe3200_stk_valid_module_id_verify,
    soc_fe3200_stat_init,
    soc_fe1600_stat_is_supported_type,
    soc_fe3200_stat_counter_length_get,
    soc_fe1600_stat_get,
    soc_fe3200_mapping_stat_get,
    soc_fe3200_counters_get_info,
    soc_fe3200_queues_get_info,
    soc_fe1600_fabric_link_device_mode_get,
    soc_fe3200_controlled_counter_set,
    soc_fe3200_soc_properties_array_get,
    soc_fe3200_avs_value_get,
    soc_fe3200_linkctrl_init,
    soc_fe3200_interrupts_init,
    soc_fe3200_interrupts_deinit,
    NULL,/*fe1600_interrupts_dfe_control_data_init*/
    NULL,/*fe1600_interrupts_dfe_control_data_deinit*/
    soc_fe3200_interrupt_all_enable_set,
    soc_fe3200_interrupt_all_enable_get,
    soc_fe3200_drv_sw_ver_set,
    soc_fe3200_drv_temperature_monitor_get,
    soc_fe3200_drv_test_reg_filter,
    soc_fe3200_drv_test_reg_default_val_filter,
    soc_fe3200_drv_test_mem_filter,
    soc_fe3200_drv_test_brdc_blk_filter,
    soc_fe3200_drv_test_brdc_blk_info_get,
    soc_fe3200_drv_asymmetrical_quad_get,
    soc_fe3200_warm_boot_buffer_id_supported_get,
    soc_fe3200_cosq_pipe_rx_weight_set,
    soc_fe3200_cosq_pipe_rx_weight_get,
    soc_fe3200_cosq_pipe_mid_weight_set,
    soc_fe3200_cosq_pipe_mid_weight_get,
    soc_fe3200_cosq_pipe_tx_weight_set,
    soc_fe3200_cosq_pipe_tx_weight_get,
    soc_fe3200_cosq_pipe_rx_threshold_set,
    soc_fe3200_cosq_pipe_rx_threshold_get,
    soc_fe3200_cosq_pipe_mid_threshold_set,
    soc_fe3200_cosq_pipe_mid_threshold_get,
    soc_fe3200_cosq_pipe_tx_threshold_set,
    soc_fe3200_cosq_pipe_tx_threshold_get,
    soc_fe3200_cosq_pipe_rx_rate_set,
    soc_fe3200_cosq_pipe_rx_rate_get,
    soc_fe3200_cosq_pipe_tx_rate_set,
    soc_fe3200_cosq_pipe_tx_rate_get,
    soc_fe3200_cosq_pipe_tx_rate_enable_set,
    soc_fe3200_cosq_pipe_tx_rate_enable_get,
    soc_fe3200_rx_cpu_address_modid_set,
    soc_fe3200_rx_cpu_address_modid_init,
    soc_fe3200_port_quad_disabled,
    soc_fe3200_drv_block_valid_get
};

#endif /* BCM_88950_A0 */

