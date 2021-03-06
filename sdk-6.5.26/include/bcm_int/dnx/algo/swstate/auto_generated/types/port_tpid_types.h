/** \file algo/swstate/auto_generated/types/port_tpid_types.h
 *
 * sw state types (structs/enums/typedefs)
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef __PORT_TPID_TYPES_H__
#define __PORT_TPID_TYPES_H__

#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <include/bcm/types.h>
#include <include/soc/dnx/dbal/auto_generated/dbal_defines_fields.h>
#include <include/soc/dnx/dnx_data/auto_generated/dnx_data_vlan.h>
#include <soc/dnxc/swstate/types/sw_state_bitmap.h>
/*
 * MACROs
 */

/**
 * The size of each LLVP (Link Layer VLAN Processing) table entry buffer
 */
#define DNX_PORT_TPID_BUFFER_BITS_PER_TAG_STRCT 19

/**
 * The size of the key of each LLVP (Link Layer VLAN Processing) table entry buffer
 */
#define DNX_PORT_TPID_LLVP_KEY_SIZE (1+3+3)

/**
 * The size of each LLVP (Link Layer VLAN Processing) block buffer
 */
#define DNX_PORT_TPID_LLVP_BLOCK_SIZE (SAL_BIT(DNX_PORT_TPID_LLVP_KEY_SIZE))

/**
 * The size of each LLVP (Link Layer VLAN Processing) template buffer
 */
#define DNX_PORT_TPID_CLASS_TEMPLATE_SIZE_NUM_OF_UINT32 ((DNX_PORT_TPID_BUFFER_BITS_PER_TAG_STRCT*DNX_PORT_TPID_LLVP_BLOCK_SIZE)/32)

/**
 * The size of TPID index
 */
#define DNX_PORT_TPID_TPID_INDEX_SIZE SAL_BIT(3)

/*
 * STRUCTs
 */

/**
 * A struct used to hold LLVP Template
 */
typedef struct {
    uint32 llvp_template[DNX_PORT_TPID_CLASS_TEMPLATE_SIZE_NUM_OF_UINT32];
} llvp_template_t;

/**
 * A struct used for holding Ingress and Egress Native AC LLVP Template
 */
typedef struct {
    llvp_template_t ingress;
    llvp_template_t egress;
} native_ac_llvp_template_t;

/**
 * A struct used for managing tag-struct allocation
 */
typedef struct {
    /**
     * SW-ID TAG-Struct Ingress allocation
     */
    dnx_algo_res_t bcm_tag_format_class_id_ingress;
    /**
     * SW-ID TAG-Struct Egress allocation
     */
    dnx_algo_res_t bcm_tag_format_class_id_egress;
    /**
     * SW-ID TAG-Struct Symmetric allocation
     */
    dnx_algo_res_t bcm_tag_format_class_id_symmetric;
    /**
     * SW-ID TAG-Struct Symmetric allocation indication
     */
    SHR_BITDCL* bcm_tag_format_class_id_symmetric_indication;
    /**
     * HW-ID TAG-Struct Ingress allocation
     */
    dnx_algo_res_t incoming_tag_structure_ingress;
    /**
     * HW-ID TAG-Struct Egress allocation
     */
    dnx_algo_res_t incoming_tag_structure_egress;
} port_tpid_tag_struct_t;

/**
 * port tpid DB for handling native-AC LLVP, Egress IP Tunnel LLVP and TAG-Struct allocation
 */
typedef struct {
    native_ac_llvp_template_t native_ac_llvp_templates;
    llvp_template_t egress_ip_tunnel_llvp_template;
    port_tpid_tag_struct_t tag_struct_allocation;
} port_tpid_db_t;

/**
 * A struct used to hold acceptable frame type Template
 */
typedef struct {
    dbal_enum_value_field_egress_acceptable_frame_type_filter_action_e acceptable_frame_type_template[DNX_PORT_TPID_TPID_INDEX_SIZE][DNX_PORT_TPID_TPID_INDEX_SIZE];
} acceptable_frame_type_template_t;


#endif /* __PORT_TPID_TYPES_H__ */
