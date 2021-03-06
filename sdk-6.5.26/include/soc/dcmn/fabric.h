/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef _SOC_DCMN_FABRIC_H
#define _SOC_DCMN_FABRIC_H

#include <soc/error.h>
#include <shared/fabric.h>

#define SOC_DCMN_FABRIC_MESH_TOPOLOGY_LIST_MAX          (72)
#define SOC_DCMN_FABRIC_MAX_NOF_PIPES                   (3)

#define DCMN_FABRIC_LINK_NO_CONNECTIVITY          _SHR_FABRIC_LINK_NO_CONNECTIVITY
#define DCMN_FABRIC_LINK_STATUS_CRC_ERROR         _SHR_FABRIC_LINK_STATUS_CRC_ERROR
#define DCMN_FABRIC_LINK_STATUS_SIZE_ERROR        _SHR_FABRIC_LINK_STATUS_SIZE_ERROR
#define DCMN_FABRIC_LINK_STATUS_CODE_GROUP_ERROR  _SHR_FABRIC_LINK_STATUS_CODE_GROUP_ERROR
#define DCMN_FABRIC_LINK_STATUS_MISALIGN          _SHR_FABRIC_LINK_STATUS_MISALIGN
#define DCMN_FABRIC_LINK_STATUS_NO_SIG_LOCK       _SHR_FABRIC_LINK_STATUS_NO_SIG_LOCK
#define DCMN_FABRIC_LINK_STATUS_NO_SIG_ACCEP      _SHR_FABRIC_LINK_STATUS_NO_SIG_ACCEP
#define DCMN_FABRIC_LINK_STATUS_ERRORED_TOKENS    _SHR_FABRIC_LINK_STATUS_ERRORED_TOKENS

typedef enum dcmn_fabric_device_type_e {
    dcmnFabricDeviceTypeUnknown, 
    dcmnFabricDeviceTypeFE13,    
    dcmnFabricDeviceTypeFE2,     
    dcmnFabricDeviceTypeFAP,     
    dcmnFabricDeviceTypeFE1,     
    dcmnFabricDeviceTypeFE3,     
    dcmnFabricDeviceTypeFIP,     
    dcmnFabricDeviceTypeFOP      
} dcmn_fabric_device_type_t;

typedef enum soc_dcmn_fabric_direction_e {
    soc_dcmn_fabric_direction_rx = 0,
    soc_dcmn_fabric_direction_tx = 1,
    soc_dcmn_fabric_direction_middle = 2,
    soc_dcmn_fabric_direction_middle_local_switch_low = 3,
    soc_dcmn_fabric_direction_middle_local_switch_high = 4
} soc_dcmn_fabric_direction_t;


typedef uint32 soc_dcmn_fabric_pipe_t;

#define SOC_DCMN_FABRIC_PIPE_INIT(fabric_pipe) (fabric_pipe=0)
#define SOC_DCMN_FABRIC_PIPE_SET(fabric_pipe,pipe_num) SHR_BITSET(fabric_pipe,pipe_num)
#define SOC_DCMN_FABRIC_PIPE_CLEAR(fabric_pipe,pipe_num) SHR_BITCLR(fabric_pipe,pipe_num)
#define SOC_DCMN_FABRIC_PIPE_ALL_PIPES_SET(fabric_pipe,nof_pipes) SHR_BITSET_RANGE(fabric_pipe,0,nof_pipes)
#define SOC_DCMN_FABRIC_PIPE_IS_SET(fabric_pipe,pipe_num) (SHR_BITGET(fabric_pipe,pipe_num) ? 1 : 0)
#define SOC_DCMN_FABRIC_PIPE_IS_CLEAR(fabric_pipe,pipe_num) (!(SOC_DCMN_FABRIC_PIPE_IS_SET(fabric_pipe,pipe_num)))
#define SOC_DCMN_FABRIC_PIPE_ALL_PIPES_IS_CLEAR(fabric_pipe) SOC_DCMN_FABRIC_PIPE_IS_INIT(fabric_pipe)
#define SOC_DCMN_FABRIC_PIPE_IS_INIT(fabric_pipe) (fabric_pipe==0)			
#define SOC_DCMN_FABRIC_PIPE_MAX_NUM_OF_PRIORITIES (4)
#define SOC_DCMN_FABRIC_PIPE_NUM_OF_FABRIC_PIPE_VALID_CONFIGURATIONS (6)
#define SOC_DCMN_FABRIC_PIPE_MAX_LEN_NAME_PRIORITY_CONFIG (30)																																				
																																						
#define SOC_DCMN_FABRIC_PIPE_ALL_PIPES_IS_SET(fabric_pipe,mask_pipes) (fabric_pipe==mask_pipes)
#define SOC_DCMN_FABRIC_PIPE_ALL_PIPES_GET(fabric_pipe,mask) (fabric_pipe&mask)

typedef enum soc_dcmn_fabric_pipe_map_type_e
{
    soc_dcmn_fabric_pipe_map_triple_uc_mc_tdm,
    soc_dcmn_fabric_pipe_map_triple_uc_hp_mc_lp_mc,
    soc_dcmn_fabric_pipe_map_dual_tdm_non_tdm,
    soc_dcmn_fabric_pipe_map_dual_uc_mc,
    soc_dcmn_fabric_pipe_map_triple_custom,
    soc_dcmn_fabric_pipe_map_dual_custom,
    soc_dcmn_fabric_pipe_map_single
} soc_dcmn_fabric_pipe_map_type_t ;

typedef struct soc_dcmn_fabric_pipe_map_s {
    int nof_pipes;
    int config_uc[SOC_DCMN_FABRIC_PIPE_MAX_NUM_OF_PRIORITIES];
    int config_mc[SOC_DCMN_FABRIC_PIPE_MAX_NUM_OF_PRIORITIES];
    char name[SOC_DCMN_FABRIC_PIPE_MAX_LEN_NAME_PRIORITY_CONFIG];
    soc_dcmn_fabric_pipe_map_type_t mapping_type;
} soc_dcmn_fabric_pipe_map_t;

typedef struct soc_dcmn_fabric_mesh_topology_diag_s {
    
    uint32 indication_1;
    uint32 indication_2;
    uint32 indication_3;
    uint32 indication_4;

    
    int type2_list[SOC_DCMN_FABRIC_MESH_TOPOLOGY_LIST_MAX];
    uint32 link_list_count;

    
    uint32 rx_control_cells_type1;
    uint32 rx_control_cells_type2;
    uint32 control_cells_type3;

    
    uint32 status_1;
    uint32 status_1_id1;
    uint32 status_1_id2;

    
    uint32 status_2;
    uint32 status_2_id1;
    uint32 status_2_id2;
    
} soc_dcmn_fabric_mesh_topology_diag_t;


int soc_dcmn_fabric_pipe_map_initalize_valid_configurations(int unit, int min_tdm_priority, soc_dcmn_fabric_pipe_map_t* fabric_pipe_map_valid_config);
soc_error_t soc_dcmn_fabric_mesh_topology_diag_get(int unit, soc_dcmn_fabric_mesh_topology_diag_t *mesh_topology_diag);


#endif 
