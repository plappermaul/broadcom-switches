/** \file dbal_defines_hw_entities.h
 * Hw entities array\n 
 * DO NOT EDIT THIS FILE!\n 
 * This file is auto-generated.\n 
 * Edits to this file will be lost when it is regenerated.\n 
 * \n 
 * Extracted from xmls: \n 
 * hl_pp_l3_intf.xml\n 
 * hl_tm_snif_definition.xml\n 
 * hl_pp_vlan_port.xml\n 
 * hl_pp_vlan.xml\n 
 * hl_tm_crps_definition.xml\n 
 * hl_pp_port_pp.xml\n 
 * hl_tables_examples_definition.xml\n 
 * hl_pp_init_cb_wrappers.xml\n 
 * hl_pp_l3.xml\n 
 * hl_pp_stg_stp.xml\n 
 * hl_pp_vlan_translate.xml\n 
 * hl_pp_port_tpid.xml\n 
 */
/*
 * $Id: $
 $Copyright: (c) 2016 Broadcom.
 Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 */
#ifndef DBAL_DEFINES_HW_ENTITIES_H_INCLUDED
/*
 * {
 */
#define DBAL_DEFINES_HW_ENTITIES_H_INCLUDED

typedef struct
{
    char *hw_entity_name;
    int hw_entity_val;
} dbal_hw_entity_mapping;

/** Number of register, memories or fields that are used in the logical tables mapping  */
#define DBAL_NOF_HW_ENTITIES (400)
/*
 * }
 */
#endif /* DBAL_DEFINES_HW_ENTITIES_H_INCLUDED */
