/*
 * $Id: fe3200_fabric_multicast.h,v 1.3 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * FE3200 FABRIC MULTICAST H
 */
 
#ifndef _SOC_FE3200_FABRIC_MULTICAST_H_
#define _SOC_FE3200_FABRIC_MULTICAST_H_

soc_error_t soc_fe3200_fabric_multicast_multi_read_info_get(int unit, soc_dfe_multicast_read_range_info_t **info, int *info_size);
soc_error_t soc_fe3200_fabric_multicast_multi_write_range(int unit, int mem_flags, soc_multicast_t group_min, soc_multicast_t group_max, uint32 *entry_array);

#endif
