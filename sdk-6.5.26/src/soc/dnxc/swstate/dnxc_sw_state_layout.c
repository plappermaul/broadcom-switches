
/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */



#include <shared/bsl.h>
#include <soc/types.h>
#include <sal/core/libc.h>
#include <soc/dnxc/swstate/sw_state_defs.h>
#include <soc/dnxc/swstate/dnxc_sw_state_verifications.h>
#include <soc/dnxc/swstate/dnxc_sw_state.h>
#include <soc/dnxc/swstate/dnxc_sw_state_layout.h>
#include <soc/dnxc/swstate/auto_generated/types/dnxc_module_ids_types.h>
#include <soc/dnxc/swstate/auto_generated/access/dnxc_module_ids_access.h>
#include <soc/dnxc/swstate/dnxc_sw_state_common.h>


#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif

#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#define LAYOUT_ALLOC_COPY_STRING(unit, param, node_id) {\
    data = param;\
    length = sal_strnlen(data, 2000)+1;\
    param = NULL;\
    DNX_SW_STATE_ALLOC(unit, node_id, param, char, length, DNXC_SW_STATE_NONE, "dnxc_sw_state_layout_alloc_string");\
    sal_strncpy(param, data, length);\
}


int dnxc_sw_state_node_id_verify(int unit, uint32 module_id, uint32 param_idx)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    if (module_id == DNXC_SW_STATE_LAYOUT_INVALID_MODULE_ID || module_id > NOF_MODULE_ID)
    {
        SHR_IF_ERR_EXIT_WITH_LOG(SOC_E_EMPTY, "Trying to access invalid module id = %d and param index = %d \n%s", module_id, param_idx, EMPTY);
    }

    if (param_idx == DNXC_SW_STATE_LAYOUT_INVALID_PARAM_IDX)
    {
        SHR_IF_ERR_EXIT_WITH_LOG(SOC_E_EMPTY, "Trying to access invalid module id = %d and param index = %d \n%s", module_id, param_idx, EMPTY);
    }

    SHR_EXIT();

    DNXC_SW_STATE_FUNC_RETURN;
}


int dnxc_sw_state_layout_node_array_size_update(int unit, uint32 node_id, dnxc_sw_state_layout_t * layout_address, uint32 nof_instances_to_alloc, uint8 dimension){

    uint32 param_idx = DNXC_SW_STATE_LAYOUT_INVALID;
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    param_idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(node_id);
    
    if (param_idx > 0 && layout_address != NULL && dimension < (DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS+1))
    {
        if(dimension == DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS)
        {
            
            for (dimension = 0; dimension < DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS; dimension++)
            {
                if (layout_address[param_idx].array_indexes[dimension].num_elements == DNXC_SW_STATE_LAYOUT_INVALID
                    && layout_address[param_idx].array_indexes[dimension].index_type !=  DNXC_SWSTATE_ARRAY_INDEX_INVALID)
                {
                    layout_address[param_idx].array_indexes[dimension].num_elements = nof_instances_to_alloc;
                    break;
                }
            }
            
            layout_address[param_idx].labels |= DNXC_SW_STATE_LAYOUT_LABEL_ARRAY_INDEX_NOT_EQUAL_SIZE;
        }
        else if (layout_address[param_idx].array_indexes[dimension].num_elements != nof_instances_to_alloc
            && layout_address[param_idx].array_indexes[dimension].num_elements != DNXC_SW_STATE_LAYOUT_INVALID)
        {
            layout_address[param_idx].labels |= DNXC_SW_STATE_LAYOUT_LABEL_ARRAY_INDEX_NOT_EQUAL_SIZE;
        }
        else
        {
            layout_address[param_idx].array_indexes[dimension].num_elements = nof_instances_to_alloc;
        }
    }

    DNXC_SW_STATE_FUNC_RETURN;
}


int dnxc_sw_state_layout_node_total_size_update(int unit, uint32 node_id, dnxc_sw_state_layout_t * layout_address, uint32 alloc_size, uint8 increment){

    uint32 param_idx = DNXC_SW_STATE_LAYOUT_INVALID;
    uint32 module_id = DNXC_SW_STATE_LAYOUT_INVALID;
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    param_idx = DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(node_id);
    module_id = DNXC_SW_STATE_LAYOUT_GET_MODULE_ID(node_id);
    if (layout_address != NULL)
    {
        dnxc_sw_state_node_id_verify(unit, module_id, param_idx);
        
        if (increment){
            if (layout_address[param_idx].total_size!=DNXC_SW_STATE_LAYOUT_INVALID)
            {
                layout_address[param_idx].total_size += alloc_size;
            }
            else
            {
                layout_address[param_idx].total_size = alloc_size;
            }
        }
        else
        {
            layout_address[param_idx].total_size -= alloc_size;
        }
    }
    else
    {
        SHR_IF_ERR_EXIT_WITH_LOG(SOC_E_EMPTY, "Trying to update total size of module = %s and param_idx %d a layout NULL address.\n%s", dnxc_module_id_e_get_name(module_id), param_idx, EMPTY);
    }

    DNXC_SW_STATE_FUNC_RETURN;
}


int dnxc_sw_state_layout_init_structure(
    int unit,
    uint32 node_id,
    dnxc_sw_state_layout_t * layout_struct,
    dnxc_sw_state_layout_t * layout_address,
    uint32 nof_params)
{
    char *data;
    int idx, length, node_ids = 0;
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    DNX_SW_STATE_MEMCPY_BASIC(unit, node_id, layout_address, layout_struct, sizeof(*layout_struct) * DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(nof_params), DNXC_SW_STATE_NONE, "dnxc_sw_state_layout_init_structure");

    for (idx = 0; idx < DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(nof_params); idx++)
    {
        node_ids = node_id + idx;

        if (layout_address[idx].name != NULL){
            LAYOUT_ALLOC_COPY_STRING(unit, layout_address[idx].name, node_ids);
        }

        if (layout_address[idx].type != NULL){
            LAYOUT_ALLOC_COPY_STRING(unit, layout_address[idx].type, node_ids);
        }

        if (layout_address[idx].doc != NULL){
            LAYOUT_ALLOC_COPY_STRING(unit, layout_address[idx].doc, node_ids);
        }

        for (int dimension = 0 ; dimension < DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS; dimension++)
        {
            if (layout_address[idx].array_indexes[dimension].name != NULL){
                LAYOUT_ALLOC_COPY_STRING(unit, layout_address[idx].array_indexes[dimension].name, node_ids);
            }
        }
    }

    DNXC_SW_STATE_FUNC_RETURN;

}

