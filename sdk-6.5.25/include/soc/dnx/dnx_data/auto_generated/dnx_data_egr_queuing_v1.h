
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_EGR_QUEUING_V1_H_

#define _DNX_DATA_EGR_QUEUING_V1_H_

#include <sal/limits.h>
#include <soc/property.h>
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_framework.h>
#include <soc/dnxc/dnxc_data/dnxc_data_utils.h>
#include <soc/dnxc/dnxc_data/dnxc_data_mgmt.h>
#include <bcm/port.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_max_egr_queuing_v1.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



shr_error_e dnx_data_if_egr_queuing_v1_init(
    int unit);







typedef enum
{
    
    dnx_data_egr_queuing_v1_general_v1_supported,

    
    _dnx_data_egr_queuing_v1_general_feature_nof
} dnx_data_egr_queuing_v1_general_feature_e;



typedef int(
    *dnx_data_egr_queuing_v1_general_feature_get_f) (
    int unit,
    dnx_data_egr_queuing_v1_general_feature_e feature);



typedef struct
{
    
    dnx_data_egr_queuing_v1_general_feature_get_f feature_get;
} dnx_data_if_egr_queuing_v1_general_t;






typedef struct
{
    
    int base_offset;
    
    int count;
} dnx_data_egr_queuing_v1_egress_interfaces_mapping_data_t;


typedef struct
{
    
    bcm_port_if_t interface_type;
    
    int speed;
    
    int high_priority_th;
    
    int low_priority_th;
    
    int txi_irdy_th;
    
    int fqp_min_gap;
    
    int eps_min_gap;
    
    int allow_consecutive_select;
    
    int credit_size;
} dnx_data_egr_queuing_v1_egress_interfaces_per_speed_mapping_data_t;



typedef enum
{
    
    dnx_data_egr_queuing_v1_egress_interfaces_channelized_interface_mapping,

    
    _dnx_data_egr_queuing_v1_egress_interfaces_feature_nof
} dnx_data_egr_queuing_v1_egress_interfaces_feature_e;



typedef int(
    *dnx_data_egr_queuing_v1_egress_interfaces_feature_get_f) (
    int unit,
    dnx_data_egr_queuing_v1_egress_interfaces_feature_e feature);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_interfaces_nof_egr_interfaces_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_interfaces_nof_channelized_egr_interfaces_get_f) (
    int unit);


typedef const dnx_data_egr_queuing_v1_egress_interfaces_mapping_data_t *(
    *dnx_data_egr_queuing_v1_egress_interfaces_mapping_data_get_f) (
    int unit,
    int interface_type);


typedef const dnx_data_egr_queuing_v1_egress_interfaces_per_speed_mapping_data_t *(
    *dnx_data_egr_queuing_v1_egress_interfaces_per_speed_mapping_data_get_f) (
    int unit,
    int index);



typedef struct
{
    
    dnx_data_egr_queuing_v1_egress_interfaces_feature_get_f feature_get;
    
    dnx_data_egr_queuing_v1_egress_interfaces_nof_egr_interfaces_get_f nof_egr_interfaces_get;
    
    dnx_data_egr_queuing_v1_egress_interfaces_nof_channelized_egr_interfaces_get_f nof_channelized_egr_interfaces_get;
    
    dnx_data_egr_queuing_v1_egress_interfaces_mapping_data_get_f mapping_data_get;
    
    dnxc_data_table_info_get_f mapping_data_info_get;
    
    dnx_data_egr_queuing_v1_egress_interfaces_per_speed_mapping_data_get_f per_speed_mapping_data_get;
    
    dnxc_data_table_info_get_f per_speed_mapping_data_info_get;
} dnx_data_if_egr_queuing_v1_egress_interfaces_t;






typedef struct
{
    
    int supported;
} dnx_data_egr_queuing_v1_egress_queues_supported_nof_priorities_t;



typedef enum
{
    
    dnx_data_egr_queuing_v1_egress_queues_rcy_reserved_queues,

    
    _dnx_data_egr_queuing_v1_egress_queues_feature_nof
} dnx_data_egr_queuing_v1_egress_queues_feature_e;



typedef int(
    *dnx_data_egr_queuing_v1_egress_queues_feature_get_f) (
    int unit,
    dnx_data_egr_queuing_v1_egress_queues_feature_e feature);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_nof_egr_queues_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_nof_egr_priorities_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_ps_queue_resolution_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_rcy_reserved_queue_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_nof_queue_pairs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_egr_queuing_v1_egress_queues_total_nof_egr_queues_get_f) (
    int unit);


typedef const dnx_data_egr_queuing_v1_egress_queues_supported_nof_priorities_t *(
    *dnx_data_egr_queuing_v1_egress_queues_supported_nof_priorities_get_f) (
    int unit,
    int priorities);



typedef struct
{
    
    dnx_data_egr_queuing_v1_egress_queues_feature_get_f feature_get;
    
    dnx_data_egr_queuing_v1_egress_queues_nof_egr_queues_get_f nof_egr_queues_get;
    
    dnx_data_egr_queuing_v1_egress_queues_nof_egr_priorities_get_f nof_egr_priorities_get;
    
    dnx_data_egr_queuing_v1_egress_queues_ps_queue_resolution_get_f ps_queue_resolution_get;
    
    dnx_data_egr_queuing_v1_egress_queues_rcy_reserved_queue_get_f rcy_reserved_queue_get;
    
    dnx_data_egr_queuing_v1_egress_queues_nof_queue_pairs_get_f nof_queue_pairs_get;
    
    dnx_data_egr_queuing_v1_egress_queues_total_nof_egr_queues_get_f total_nof_egr_queues_get;
    
    dnx_data_egr_queuing_v1_egress_queues_supported_nof_priorities_get_f supported_nof_priorities_get;
    
    dnxc_data_table_info_get_f supported_nof_priorities_info_get;
} dnx_data_if_egr_queuing_v1_egress_queues_t;







typedef enum
{

    
    _dnx_data_egr_queuing_v1_scheduling_feature_nof
} dnx_data_egr_queuing_v1_scheduling_feature_e;



typedef int(
    *dnx_data_egr_queuing_v1_scheduling_feature_get_f) (
    int unit,
    dnx_data_egr_queuing_v1_scheduling_feature_e feature);


typedef uint32(
    *dnx_data_egr_queuing_v1_scheduling_nof_interface_calendar_slots_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_egr_queuing_v1_scheduling_feature_get_f feature_get;
    
    dnx_data_egr_queuing_v1_scheduling_nof_interface_calendar_slots_get_f nof_interface_calendar_slots_get;
} dnx_data_if_egr_queuing_v1_scheduling_t;







typedef enum
{

    
    _dnx_data_egr_queuing_v1_shaping_feature_nof
} dnx_data_egr_queuing_v1_shaping_feature_e;



typedef int(
    *dnx_data_egr_queuing_v1_shaping_feature_get_f) (
    int unit,
    dnx_data_egr_queuing_v1_shaping_feature_e feature);


typedef uint32(
    *dnx_data_egr_queuing_v1_shaping_default_max_burst_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_egr_queuing_v1_shaping_feature_get_f feature_get;
    
    dnx_data_egr_queuing_v1_shaping_default_max_burst_get_f default_max_burst_get;
} dnx_data_if_egr_queuing_v1_shaping_t;





typedef struct
{
    
    dnx_data_if_egr_queuing_v1_general_t general;
    
    dnx_data_if_egr_queuing_v1_egress_interfaces_t egress_interfaces;
    
    dnx_data_if_egr_queuing_v1_egress_queues_t egress_queues;
    
    dnx_data_if_egr_queuing_v1_scheduling_t scheduling;
    
    dnx_data_if_egr_queuing_v1_shaping_t shaping;
} dnx_data_if_egr_queuing_v1_t;




extern dnx_data_if_egr_queuing_v1_t dnx_data_egr_queuing_v1;


#endif 

