/** \file algo/swstate/auto_generated/access/algo_tunnel_access.h
 *
 * sw state functions declarations
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

#ifndef __ALGO_TUNNEL_ACCESS_H__
#define __ALGO_TUNNEL_ACCESS_H__

#include <bcm_int/dnx/algo/swstate/auto_generated/types/algo_tunnel_types.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
#include <bcm_int/dnx/algo/tunnel/algo_tunnel.h>
#include <soc/dnxc/swstate/callbacks/sw_state_htb_callbacks.h>
#include <soc/dnxc/swstate/types/sw_state_hash_table.h>
/*
 * TYPEDEFs
 */

/**
 * implemented by: algo_tunnel_db_is_init
 */
typedef int (*algo_tunnel_db_is_init_cb)(
    int unit, uint8 *is_init);

/**
 * implemented by: algo_tunnel_db_init
 */
typedef int (*algo_tunnel_db_init_cb)(
    int unit);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_set
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_set_cb)(
    int unit, dnx_algo_template_t tunnel_ipv6_sip_profile);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_get
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_get_cb)(
    int unit, dnx_algo_template_t *tunnel_ipv6_sip_profile);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_create
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_allocate_single
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_allocate_single_cb)(
    int unit, uint32 flags, bcm_ip6_t *profile_data, void *extra_arguments, int *profile, uint8 *first_reference);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_free_single
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_free_single_cb)(
    int unit, int profile, uint8 *last_reference);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_profile_data_get
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_profile_data_get_cb)(
    int unit, int profile, int *ref_count, bcm_ip6_t *profile_data);

/**
 * implemented by: algo_tunnel_db_tunnel_ipv6_sip_profile_profile_get
 */
typedef int (*algo_tunnel_db_tunnel_ipv6_sip_profile_profile_get_cb)(
    int unit, const bcm_ip6_t *profile_data, int *profile);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_set
 */
typedef int (*algo_tunnel_db_udp_ports_profile_set_cb)(
    int unit, dnx_algo_template_t udp_ports_profile);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_get
 */
typedef int (*algo_tunnel_db_udp_ports_profile_get_cb)(
    int unit, dnx_algo_template_t *udp_ports_profile);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_create
 */
typedef int (*algo_tunnel_db_udp_ports_profile_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_allocate_single
 */
typedef int (*algo_tunnel_db_udp_ports_profile_allocate_single_cb)(
    int unit, uint32 flags, udp_ports_t *profile_data, void *extra_arguments, int *profile, uint8 *first_reference);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_free_single
 */
typedef int (*algo_tunnel_db_udp_ports_profile_free_single_cb)(
    int unit, int profile, uint8 *last_reference);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_exchange
 */
typedef int (*algo_tunnel_db_udp_ports_profile_exchange_cb)(
    int unit, uint32 flags, const udp_ports_t *profile_data, int old_profile, const void *extra_arguments, int *new_profile, uint8 *first_reference, uint8 *last_reference);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_profile_data_get
 */
typedef int (*algo_tunnel_db_udp_ports_profile_profile_data_get_cb)(
    int unit, int profile, int *ref_count, udp_ports_t *profile_data);

/**
 * implemented by: algo_tunnel_db_udp_ports_profile_profile_get
 */
typedef int (*algo_tunnel_db_udp_ports_profile_profile_get_cb)(
    int unit, const udp_ports_t *profile_data, int *profile);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_create
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_create_cb)(
    int unit, sw_state_htbl_init_info_t *init_info);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_find
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_find_cb)(
    int unit, const int *key, bcm_tunnel_type_t *value, uint8 *found);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_insert
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_insert_cb)(
    int unit, const int *key, const bcm_tunnel_type_t *value, uint8 *success);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_get_next
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_get_next_cb)(
    int unit, SW_STATE_HASH_TABLE_ITER *iter, const int *key, const bcm_tunnel_type_t *value);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_clear
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_clear_cb)(
    int unit);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_delete
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_delete_cb)(
    int unit, const int *key);

/**
 * implemented by: algo_tunnel_db_flow_gport_to_tunnel_type_delete_all
 */
typedef int (*algo_tunnel_db_flow_gport_to_tunnel_type_delete_all_cb)(
    int unit);

/*
 * STRUCTs
 */

/**
 * This structure holds the access functions for the variable tunnel_ipv6_sip_profile
 */
typedef struct {
    algo_tunnel_db_tunnel_ipv6_sip_profile_set_cb set;
    algo_tunnel_db_tunnel_ipv6_sip_profile_get_cb get;
    algo_tunnel_db_tunnel_ipv6_sip_profile_create_cb create;
    algo_tunnel_db_tunnel_ipv6_sip_profile_allocate_single_cb allocate_single;
    algo_tunnel_db_tunnel_ipv6_sip_profile_free_single_cb free_single;
    algo_tunnel_db_tunnel_ipv6_sip_profile_profile_data_get_cb profile_data_get;
    algo_tunnel_db_tunnel_ipv6_sip_profile_profile_get_cb profile_get;
} algo_tunnel_db_tunnel_ipv6_sip_profile_cbs;

/**
 * This structure holds the access functions for the variable udp_ports_profile
 */
typedef struct {
    algo_tunnel_db_udp_ports_profile_set_cb set;
    algo_tunnel_db_udp_ports_profile_get_cb get;
    algo_tunnel_db_udp_ports_profile_create_cb create;
    algo_tunnel_db_udp_ports_profile_allocate_single_cb allocate_single;
    algo_tunnel_db_udp_ports_profile_free_single_cb free_single;
    algo_tunnel_db_udp_ports_profile_exchange_cb exchange;
    algo_tunnel_db_udp_ports_profile_profile_data_get_cb profile_data_get;
    algo_tunnel_db_udp_ports_profile_profile_get_cb profile_get;
} algo_tunnel_db_udp_ports_profile_cbs;

/**
 * This structure holds the access functions for the variable flow_gport_to_tunnel_type
 */
typedef struct {
    algo_tunnel_db_flow_gport_to_tunnel_type_create_cb create;
    algo_tunnel_db_flow_gport_to_tunnel_type_find_cb find;
    algo_tunnel_db_flow_gport_to_tunnel_type_insert_cb insert;
    algo_tunnel_db_flow_gport_to_tunnel_type_get_next_cb get_next;
    algo_tunnel_db_flow_gport_to_tunnel_type_clear_cb clear;
    algo_tunnel_db_flow_gport_to_tunnel_type_delete_cb delete;
    algo_tunnel_db_flow_gport_to_tunnel_type_delete_all_cb delete_all;
} algo_tunnel_db_flow_gport_to_tunnel_type_cbs;

/**
 * This structure holds the access functions for the variable algo_tunnel_sw_state_t
 */
typedef struct {
    algo_tunnel_db_is_init_cb is_init;
    algo_tunnel_db_init_cb init;
    /**
     * Access struct for tunnel_ipv6_sip_profile
     */
    algo_tunnel_db_tunnel_ipv6_sip_profile_cbs tunnel_ipv6_sip_profile;
    /**
     * Access struct for udp_ports_profile
     */
    algo_tunnel_db_udp_ports_profile_cbs udp_ports_profile;
    /**
     * Access struct for flow_gport_to_tunnel_type
     */
    algo_tunnel_db_flow_gport_to_tunnel_type_cbs flow_gport_to_tunnel_type;
} algo_tunnel_db_cbs;

/*
 * Global Variables
 */

/*
 * Global Variables
 */

extern algo_tunnel_db_cbs algo_tunnel_db;

#endif /* __ALGO_TUNNEL_ACCESS_H__ */
