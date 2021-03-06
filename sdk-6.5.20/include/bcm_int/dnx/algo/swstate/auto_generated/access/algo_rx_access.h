/** \file algo/swstate/auto_generated/access/algo_rx_access.h
 *
 * sw state functions declarations
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef __ALGO_RX_ACCESS_H__
#define __ALGO_RX_ACCESS_H__

#include <soc/dnxc/swstate/dnxc_sw_state_h_includes.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/types/algo_rx_types.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <bcm_int/dnx/algo/rx/algo_rx.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
/*
 * TYPEDEFs
 */

/**
 * implemented by: algo_rx_db_is_init
 */
typedef int (*algo_rx_db_is_init_cb)(
    int unit, uint8 *is_init);

/**
 * implemented by: algo_rx_db_init
 */
typedef int (*algo_rx_db_init_cb)(
    int unit);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_set
 */
typedef int (*algo_rx_db_trap_ingress_predefined_set_cb)(
    int unit, dnx_algo_res_t trap_ingress_predefined);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_get
 */
typedef int (*algo_rx_db_trap_ingress_predefined_get_cb)(
    int unit, dnx_algo_res_t *trap_ingress_predefined);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_create
 */
typedef int (*algo_rx_db_trap_ingress_predefined_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_allocate_single
 */
typedef int (*algo_rx_db_trap_ingress_predefined_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_free_single
 */
typedef int (*algo_rx_db_trap_ingress_predefined_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_ingress_predefined_is_allocated
 */
typedef int (*algo_rx_db_trap_ingress_predefined_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_set
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_set_cb)(
    int unit, dnx_algo_res_t trap_ingress_user_defined);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_get
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_get_cb)(
    int unit, dnx_algo_res_t *trap_ingress_user_defined);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_create
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_allocate_single
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_free_single
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_ingress_user_defined_is_allocated
 */
typedef int (*algo_rx_db_trap_ingress_user_defined_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_erpp_set
 */
typedef int (*algo_rx_db_trap_erpp_set_cb)(
    int unit, dnx_algo_res_t trap_erpp);

/**
 * implemented by: algo_rx_db_trap_erpp_get
 */
typedef int (*algo_rx_db_trap_erpp_get_cb)(
    int unit, dnx_algo_res_t *trap_erpp);

/**
 * implemented by: algo_rx_db_trap_erpp_create
 */
typedef int (*algo_rx_db_trap_erpp_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_erpp_allocate_single
 */
typedef int (*algo_rx_db_trap_erpp_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_erpp_free_single
 */
typedef int (*algo_rx_db_trap_erpp_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_erpp_is_allocated
 */
typedef int (*algo_rx_db_trap_erpp_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_etpp_set
 */
typedef int (*algo_rx_db_trap_etpp_set_cb)(
    int unit, dnx_algo_res_t trap_etpp);

/**
 * implemented by: algo_rx_db_trap_etpp_get
 */
typedef int (*algo_rx_db_trap_etpp_get_cb)(
    int unit, dnx_algo_res_t *trap_etpp);

/**
 * implemented by: algo_rx_db_trap_etpp_create
 */
typedef int (*algo_rx_db_trap_etpp_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_etpp_allocate_single
 */
typedef int (*algo_rx_db_trap_etpp_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_etpp_free_single
 */
typedef int (*algo_rx_db_trap_etpp_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_etpp_is_allocated
 */
typedef int (*algo_rx_db_trap_etpp_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_set
 */
typedef int (*algo_rx_db_trap_etpp_oam_set_cb)(
    int unit, dnx_algo_res_t trap_etpp_oam);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_get
 */
typedef int (*algo_rx_db_trap_etpp_oam_get_cb)(
    int unit, dnx_algo_res_t *trap_etpp_oam);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_create
 */
typedef int (*algo_rx_db_trap_etpp_oam_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_allocate_single
 */
typedef int (*algo_rx_db_trap_etpp_oam_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_free_single
 */
typedef int (*algo_rx_db_trap_etpp_oam_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_etpp_oam_is_allocated
 */
typedef int (*algo_rx_db_trap_etpp_oam_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_set
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_set_cb)(
    int unit, dnx_algo_res_t trap_protocol_icmp_type_map);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_get
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_get_cb)(
    int unit, dnx_algo_res_t *trap_protocol_icmp_type_map);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_create
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_allocate_single
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_free_single
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_free_single_cb)(
    int unit, int element);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_is_allocated
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: algo_rx_db_trap_protocol_icmp_type_map_get_next
 */
typedef int (*algo_rx_db_trap_protocol_icmp_type_map_get_next_cb)(
    int unit, int *element);

/**
 * implemented by: algo_rx_db_trap_recycle_cmd_set
 */
typedef int (*algo_rx_db_trap_recycle_cmd_set_cb)(
    int unit, dnx_algo_template_t trap_recycle_cmd);

/**
 * implemented by: algo_rx_db_trap_recycle_cmd_get
 */
typedef int (*algo_rx_db_trap_recycle_cmd_get_cb)(
    int unit, dnx_algo_template_t *trap_recycle_cmd);

/**
 * implemented by: algo_rx_db_trap_recycle_cmd_create
 */
typedef int (*algo_rx_db_trap_recycle_cmd_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_recycle_cmd_exchange
 */
typedef int (*algo_rx_db_trap_recycle_cmd_exchange_cb)(
    int unit, uint32 flags, const dnx_rx_trap_recycle_cmd_config_t *profile_data, int old_profile, const void *extra_arguments, int *new_profile, uint8 *first_reference, uint8 *last_reference);

/**
 * implemented by: algo_rx_db_trap_recycle_cmd_profile_data_get
 */
typedef int (*algo_rx_db_trap_recycle_cmd_profile_data_get_cb)(
    int unit, int profile, int *ref_count, dnx_rx_trap_recycle_cmd_config_t *profile_data);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_set
 */
typedef int (*algo_rx_db_trap_lif_ingress_set_cb)(
    int unit, dnx_algo_template_t trap_lif_ingress);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_get
 */
typedef int (*algo_rx_db_trap_lif_ingress_get_cb)(
    int unit, dnx_algo_template_t *trap_lif_ingress);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_create
 */
typedef int (*algo_rx_db_trap_lif_ingress_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_exchange
 */
typedef int (*algo_rx_db_trap_lif_ingress_exchange_cb)(
    int unit, uint32 flags, const bcm_gport_t *profile_data, int old_profile, const void *extra_arguments, int *new_profile, uint8 *first_reference, uint8 *last_reference);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_profile_data_get
 */
typedef int (*algo_rx_db_trap_lif_ingress_profile_data_get_cb)(
    int unit, int profile, int *ref_count, bcm_gport_t *profile_data);

/**
 * implemented by: algo_rx_db_trap_lif_ingress_profile_get
 */
typedef int (*algo_rx_db_trap_lif_ingress_profile_get_cb)(
    int unit, const bcm_gport_t *profile_data, int *profile);

/**
 * implemented by: algo_rx_db_trap_lif_etpp_set
 */
typedef int (*algo_rx_db_trap_lif_etpp_set_cb)(
    int unit, dnx_algo_template_t trap_lif_etpp);

/**
 * implemented by: algo_rx_db_trap_lif_etpp_get
 */
typedef int (*algo_rx_db_trap_lif_etpp_get_cb)(
    int unit, dnx_algo_template_t *trap_lif_etpp);

/**
 * implemented by: algo_rx_db_trap_lif_etpp_create
 */
typedef int (*algo_rx_db_trap_lif_etpp_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: algo_rx_db_trap_lif_etpp_exchange
 */
typedef int (*algo_rx_db_trap_lif_etpp_exchange_cb)(
    int unit, uint32 flags, const bcm_gport_t *profile_data, int old_profile, const void *extra_arguments, int *new_profile, uint8 *first_reference, uint8 *last_reference);

/*
 * STRUCTs
 */

/**
 * This structure holds the access functions for the variable trap_ingress_predefined
 */
typedef struct {
    algo_rx_db_trap_ingress_predefined_set_cb set;
    algo_rx_db_trap_ingress_predefined_get_cb get;
    algo_rx_db_trap_ingress_predefined_create_cb create;
    algo_rx_db_trap_ingress_predefined_allocate_single_cb allocate_single;
    algo_rx_db_trap_ingress_predefined_free_single_cb free_single;
    algo_rx_db_trap_ingress_predefined_is_allocated_cb is_allocated;
} algo_rx_db_trap_ingress_predefined_cbs;

/**
 * This structure holds the access functions for the variable trap_ingress_user_defined
 */
typedef struct {
    algo_rx_db_trap_ingress_user_defined_set_cb set;
    algo_rx_db_trap_ingress_user_defined_get_cb get;
    algo_rx_db_trap_ingress_user_defined_create_cb create;
    algo_rx_db_trap_ingress_user_defined_allocate_single_cb allocate_single;
    algo_rx_db_trap_ingress_user_defined_free_single_cb free_single;
    algo_rx_db_trap_ingress_user_defined_is_allocated_cb is_allocated;
} algo_rx_db_trap_ingress_user_defined_cbs;

/**
 * This structure holds the access functions for the variable trap_erpp
 */
typedef struct {
    algo_rx_db_trap_erpp_set_cb set;
    algo_rx_db_trap_erpp_get_cb get;
    algo_rx_db_trap_erpp_create_cb create;
    algo_rx_db_trap_erpp_allocate_single_cb allocate_single;
    algo_rx_db_trap_erpp_free_single_cb free_single;
    algo_rx_db_trap_erpp_is_allocated_cb is_allocated;
} algo_rx_db_trap_erpp_cbs;

/**
 * This structure holds the access functions for the variable trap_etpp
 */
typedef struct {
    algo_rx_db_trap_etpp_set_cb set;
    algo_rx_db_trap_etpp_get_cb get;
    algo_rx_db_trap_etpp_create_cb create;
    algo_rx_db_trap_etpp_allocate_single_cb allocate_single;
    algo_rx_db_trap_etpp_free_single_cb free_single;
    algo_rx_db_trap_etpp_is_allocated_cb is_allocated;
} algo_rx_db_trap_etpp_cbs;

/**
 * This structure holds the access functions for the variable trap_etpp_oam
 */
typedef struct {
    algo_rx_db_trap_etpp_oam_set_cb set;
    algo_rx_db_trap_etpp_oam_get_cb get;
    algo_rx_db_trap_etpp_oam_create_cb create;
    algo_rx_db_trap_etpp_oam_allocate_single_cb allocate_single;
    algo_rx_db_trap_etpp_oam_free_single_cb free_single;
    algo_rx_db_trap_etpp_oam_is_allocated_cb is_allocated;
} algo_rx_db_trap_etpp_oam_cbs;

/**
 * This structure holds the access functions for the variable trap_protocol_icmp_type_map
 */
typedef struct {
    algo_rx_db_trap_protocol_icmp_type_map_set_cb set;
    algo_rx_db_trap_protocol_icmp_type_map_get_cb get;
    algo_rx_db_trap_protocol_icmp_type_map_create_cb create;
    algo_rx_db_trap_protocol_icmp_type_map_allocate_single_cb allocate_single;
    algo_rx_db_trap_protocol_icmp_type_map_free_single_cb free_single;
    algo_rx_db_trap_protocol_icmp_type_map_is_allocated_cb is_allocated;
    algo_rx_db_trap_protocol_icmp_type_map_get_next_cb get_next;
} algo_rx_db_trap_protocol_icmp_type_map_cbs;

/**
 * This structure holds the access functions for the variable trap_recycle_cmd
 */
typedef struct {
    algo_rx_db_trap_recycle_cmd_set_cb set;
    algo_rx_db_trap_recycle_cmd_get_cb get;
    algo_rx_db_trap_recycle_cmd_create_cb create;
    algo_rx_db_trap_recycle_cmd_exchange_cb exchange;
    algo_rx_db_trap_recycle_cmd_profile_data_get_cb profile_data_get;
} algo_rx_db_trap_recycle_cmd_cbs;

/**
 * This structure holds the access functions for the variable trap_lif_ingress
 */
typedef struct {
    algo_rx_db_trap_lif_ingress_set_cb set;
    algo_rx_db_trap_lif_ingress_get_cb get;
    algo_rx_db_trap_lif_ingress_create_cb create;
    algo_rx_db_trap_lif_ingress_exchange_cb exchange;
    algo_rx_db_trap_lif_ingress_profile_data_get_cb profile_data_get;
    algo_rx_db_trap_lif_ingress_profile_get_cb profile_get;
} algo_rx_db_trap_lif_ingress_cbs;

/**
 * This structure holds the access functions for the variable trap_lif_etpp
 */
typedef struct {
    algo_rx_db_trap_lif_etpp_set_cb set;
    algo_rx_db_trap_lif_etpp_get_cb get;
    algo_rx_db_trap_lif_etpp_create_cb create;
    algo_rx_db_trap_lif_etpp_exchange_cb exchange;
} algo_rx_db_trap_lif_etpp_cbs;

/**
 * This structure holds the access functions for the variable algo_rx_sw_state_t
 */
typedef struct {
    algo_rx_db_is_init_cb is_init;
    algo_rx_db_init_cb init;
    /**
     * Access struct for trap_ingress_predefined
     */
    algo_rx_db_trap_ingress_predefined_cbs trap_ingress_predefined;
    /**
     * Access struct for trap_ingress_user_defined
     */
    algo_rx_db_trap_ingress_user_defined_cbs trap_ingress_user_defined;
    /**
     * Access struct for trap_erpp
     */
    algo_rx_db_trap_erpp_cbs trap_erpp;
    /**
     * Access struct for trap_etpp
     */
    algo_rx_db_trap_etpp_cbs trap_etpp;
    /**
     * Access struct for trap_etpp_oam
     */
    algo_rx_db_trap_etpp_oam_cbs trap_etpp_oam;
    /**
     * Access struct for trap_protocol_icmp_type_map
     */
    algo_rx_db_trap_protocol_icmp_type_map_cbs trap_protocol_icmp_type_map;
    /**
     * Access struct for trap_recycle_cmd
     */
    algo_rx_db_trap_recycle_cmd_cbs trap_recycle_cmd;
    /**
     * Access struct for trap_lif_ingress
     */
    algo_rx_db_trap_lif_ingress_cbs trap_lif_ingress;
    /**
     * Access struct for trap_lif_etpp
     */
    algo_rx_db_trap_lif_etpp_cbs trap_lif_etpp;
} algo_rx_db_cbs;

/*
 * Global Variables
 */

/*
 * Global Variables
 */

extern algo_rx_db_cbs algo_rx_db;

#endif /* __ALGO_RX_ACCESS_H__ */
