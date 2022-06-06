
/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifndef _DNXC_SW_STATE_LAYOUT_H_INCLUDED
#define _DNXC_SW_STATE_LAYOUT_H_INCLUDED
#include <shared/utilex/utilex_integer_arithmetic.h>

#define DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS 5

#define DNXC_SW_STATE_LAYOUT_INVALID -1

#define DNXC_SW_STATE_LAYOUT_INVALID_MODULE_ID UTILEX_HIHALF(-1)

#define DNXC_SW_STATE_LAYOUT_INVALID_PARAM_IDX UTILEX_LOHALF(-1)

#define DNXC_SW_STATE_LAYOUT_NODE_ID_GET(module_id,param_idx) UTILEX_TOHIGH(module_id)|param_idx

#define DNXC_SW_STATE_LAYOUT_GET_MODULE_ID(node_id) UTILEX_HIHALF(node_id)
#define DNXC_SW_STATE_LAYOUT_GET_PARAM_IDX(node_id) UTILEX_LOHALF(node_id)

#define DNXC_SW_STATE_LAYOUT_LABEL_NULL                             SAL_BIT(0)
#define DNXC_SW_STATE_LAYOUT_LABEL_ACCESS_PTR                       SAL_BIT(1)
#define DNXC_SW_STATE_LAYOUT_LABEL_COUNTER                          SAL_BIT(2)
#define DNXC_SW_STATE_LAYOUT_LABEL_ALLOC_EXCEPTION                  SAL_BIT(3)
#define DNXC_SW_STATE_LAYOUT_LABEL_ARRAY_RANGE_FUNCTIONS            SAL_BIT(4)
#define DNXC_SW_STATE_LAYOUT_LABEL_ALLOC_AFTER_INIT                 SAL_BIT(5)
#define DNXC_SW_STATE_LAYOUT_LABEL_DONT_DUMP                        SAL_BIT(6)
#define DNXC_SW_STATE_LAYOUT_LABEL_GENERATE_FREE                    SAL_BIT(7)
#define DNXC_SW_STATE_LAYOUT_LABEL_GENERATE_IS_ALLOC                SAL_BIT(8)
#define DNXC_SW_STATE_LAYOUT_LABEL_ATTRIBUTE_PACKED                 SAL_BIT(9)
#define DNXC_SW_STATE_LAYOUT_LABEL_UNION                            SAL_BIT(10)
#define DNXC_SW_STATE_LAYOUT_LABEL_LEAF_STRUCT                      SAL_BIT(11)
#define DNXC_SW_STATE_LAYOUT_LABEL_FORCE_INTERFACE_GENERATION       SAL_BIT(12)
#define DNXC_SW_STATE_LAYOUT_LABEL_EXPANDED_INTERFACE               SAL_BIT(13)
#define DNXC_SW_STATE_LAYOUT_LABEL_DIFF_ALLOC_SIZES                 SAL_BIT(14)
#define DNXC_SW_STATE_LAYOUT_LABEL_ALLOW_WRITES_DURING_WB           SAL_BIT(15)

#define DNXC_SW_STATE_LAYOUT_LABEL_ARRAY_INDEX_NOT_EQUAL_SIZE       SAL_BIT(16)

typedef uint32 dnxc_sw_state_layout_node_id_t;

typedef void (
    *dnxc_sw_state_print_cb) (
    int unit,
    const void *data);

typedef enum
{

    DNXC_SWSTATE_ARRAY_INDEX_INVALID = 0,

    DNXC_SWSTATE_ARRAY_INDEX_STATIC,

    DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC,

    DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_DNX_DATA,

    DNXC_SWSTATE_ARRAY_INDEX_BITMAP,

    DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_BITMAP,

    DNXC_SWSTATE_ARRAY_INDEX_BUFFER,

    DNXC_SWSTATE_ARRAY_INDEX_DYNAMIC_BUFFER,

    DNXC_SWSTATE_ARRAY_INDEX_STRING,
} dnxc_sw_state_array_index_type_e;

typedef struct dnxc_sw_state_layout_array_index_info_s
{

    char *name;

    uint32 num_elements;

    dnxc_sw_state_array_index_type_e index_type;
} dnxc_sw_state_layout_array_index_info_t;

typedef struct dnxc_sw_state_layout_s
{

    char *name;

    char *type;

    char *doc;

    void *default_value;

    uint32 total_size;

    uint32 size_of;

    dnxc_sw_state_layout_array_index_info_t array_indexes[DNXC_SWSTATE_LAYOUT_MAX_ARRAY_DIMENSIONS];

    dnxc_sw_state_layout_node_id_t parent;

    dnxc_sw_state_layout_node_id_t first_child_index;

    dnxc_sw_state_layout_node_id_t last_child_index;

    uint32 labels;

    dnxc_sw_state_print_cb print_cb;

} dnxc_sw_state_layout_t;

int dnxc_sw_state_node_id_verify(
    int unit,
    uint32 module_id,
    uint32 param_idx);

int dnxc_sw_state_layout_node_array_size_update(
    int unit,
    uint32 node_id,
    dnxc_sw_state_layout_t * layout_address,
    uint32 nof_instances,
    uint8 dimension);

int dnxc_sw_state_layout_node_total_size_update(
    int unit,
    uint32 node_id,
    dnxc_sw_state_layout_t * layout_address,
    uint32 alloc_size,
    uint8 increment);

int dnxc_sw_state_layout_init_structure(
    int unit,
    uint32 node_id,
    dnxc_sw_state_layout_t * layout_struct,
    dnxc_sw_state_layout_t * layout_address,
    uint32 nof_params);

#endif
