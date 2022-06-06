/*
 * $Id: dfe_fabric.h,v 1.2 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * DFE FABRIC H
 */
 
#ifndef _SOC_DFE_FABRIC_H_
#define _SOC_DFE_FABRIC_H_

#include <bcm/types.h>
#include <soc/error.h>
#include <soc/dfe/cmn/dfe_defs.h>
#include <bcm/fabric.h>

typedef enum soc_dfe_fabric_link_device_mode_e {
    soc_dfe_fabric_link_device_mode_fe2, 
    soc_dfe_fabric_link_device_mode_multi_stage_fe1,
    soc_dfe_fabric_link_device_mode_multi_stage_fe3,
    soc_dfe_fabric_link_device_mode_repeater
} soc_dfe_fabric_link_device_mode_t;


typedef struct soc_dfe_fabric_link_remote_pipe_mapping_s {
    uint32 num_of_remote_pipes;    /* Number of pipes supported by the remote device */

    uint32 remote_pipe_mapping[SOC_DFE_MAX_NOF_PIPES]; /*each pipe_map[remote_pipe] reprsents the local link pipe that connected to remote_pipe*/
} soc_dfe_fabric_link_remote_pipe_mapping_t;

soc_error_t soc_dfe_fabric_modid_group_find(int unit, bcm_module_t modid, bcm_module_t* group);
soc_error_t soc_dfe_fabric_link_status_all_get(int unit, int links_array_max_size, uint32* link_status, uint32* errored_token_count, int* links_array_count);

#endif /*_SOC_DFE_FABRIC_H_*/
