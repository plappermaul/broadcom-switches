/** \file algo/swstate/auto_generated/access/cosq_latency_access.h
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

#ifndef __COSQ_LATENCY_ACCESS_H__
#define __COSQ_LATENCY_ACCESS_H__

#include <bcm_int/dnx/algo/swstate/auto_generated/types/cosq_latency_types.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr.h>
#include <bcm_int/dnx/algo/res_mngr/res_mngr_internal.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr.h>
#include <bcm_int/dnx/algo/template_mngr/template_mngr_internal.h>
#include <bcm_int/dnx/cosq/latency/cosq_latency.h>
/*
 * TYPEDEFs
 */

/**
 * implemented by: dnx_cosq_latency_db_is_init
 */
typedef int (*dnx_cosq_latency_db_is_init_cb)(
    int unit, uint8 *is_init);

/**
 * implemented by: dnx_cosq_latency_db_init
 */
typedef int (*dnx_cosq_latency_db_init_cb)(
    int unit);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_set
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_set_cb)(
    int unit, dnx_algo_res_t ingress_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_get
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_get_cb)(
    int unit, dnx_algo_res_t *ingress_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_create
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_allocate_single
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_free_single
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_free_single_cb)(
    int unit, int element);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_is_allocated
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_set
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_set_cb)(
    int unit, void *algorithm_info);

/**
 * implemented by: dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_get
 */
typedef int (*dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_get_cb)(
    int unit, void *algorithm_info);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_set
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_set_cb)(
    int unit, dnx_algo_res_t end_to_end_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_get
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_get_cb)(
    int unit, dnx_algo_res_t *end_to_end_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_create
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_allocate_single
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_free_single
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_free_single_cb)(
    int unit, int element);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_latency_res_is_allocated
 */
typedef int (*dnx_cosq_latency_db_end_to_end_latency_res_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_set
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_set_cb)(
    int unit, dnx_algo_res_t end_to_end_aqm_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_get
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_get_cb)(
    int unit, dnx_algo_res_t *end_to_end_aqm_latency_res);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_create
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_allocate_single
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_free_single
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_free_single_cb)(
    int unit, int element);

/**
 * implemented by: dnx_cosq_latency_db_end_to_end_aqm_latency_res_is_allocated
 */
typedef int (*dnx_cosq_latency_db_end_to_end_aqm_latency_res_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_set
 */
typedef int (*dnx_cosq_latency_db_queue_template_set_cb)(
    int unit, dnx_algo_template_t queue_template);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_get
 */
typedef int (*dnx_cosq_latency_db_queue_template_get_cb)(
    int unit, dnx_algo_template_t *queue_template);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_create
 */
typedef int (*dnx_cosq_latency_db_queue_template_create_cb)(
    int unit, dnx_algo_template_create_data_t * data, void *extra_arguments);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_allocate_single
 */
typedef int (*dnx_cosq_latency_db_queue_template_allocate_single_cb)(
    int unit, uint32 flags, dnx_cosq_latency_queue_template_t *profile_data, void *extra_arguments, int *profile, uint8 *first_reference);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_free_single
 */
typedef int (*dnx_cosq_latency_db_queue_template_free_single_cb)(
    int unit, int profile, uint8 *last_reference);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_exchange
 */
typedef int (*dnx_cosq_latency_db_queue_template_exchange_cb)(
    int unit, uint32 flags, const dnx_cosq_latency_queue_template_t *profile_data, int old_profile, const void *extra_arguments, int *new_profile, uint8 *first_reference, uint8 *last_reference);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_replace_data
 */
typedef int (*dnx_cosq_latency_db_queue_template_replace_data_cb)(
    int unit, int profile,  const dnx_cosq_latency_queue_template_t *new_profile_data);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_profile_data_get
 */
typedef int (*dnx_cosq_latency_db_queue_template_profile_data_get_cb)(
    int unit, int profile, int *ref_count, dnx_cosq_latency_queue_template_t *profile_data);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_profile_get
 */
typedef int (*dnx_cosq_latency_db_queue_template_profile_get_cb)(
    int unit, const dnx_cosq_latency_queue_template_t *profile_data, int *profile);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_clear
 */
typedef int (*dnx_cosq_latency_db_queue_template_clear_cb)(
    int unit);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_free_all
 */
typedef int (*dnx_cosq_latency_db_queue_template_free_all_cb)(
    int unit, int profile);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_get_next
 */
typedef int (*dnx_cosq_latency_db_queue_template_get_next_cb)(
    int unit, int *current_profile);

/**
 * implemented by: dnx_cosq_latency_db_queue_template_dump_info_get
 */
typedef int (*dnx_cosq_latency_db_queue_template_dump_info_get_cb)(
    int unit, dnx_algo_template_dump_data_t * info);

/**
 * implemented by: dnx_cosq_latency_db_probability_convert_exponent_b_set
 */
typedef int (*dnx_cosq_latency_db_probability_convert_exponent_b_set_cb)(
    int unit, uint32 exponent_b);

/**
 * implemented by: dnx_cosq_latency_db_probability_convert_exponent_b_get
 */
typedef int (*dnx_cosq_latency_db_probability_convert_exponent_b_get_cb)(
    int unit, uint32 *exponent_b);

/**
 * implemented by: dnx_cosq_latency_db_probability_convert_coefficient_a_set
 */
typedef int (*dnx_cosq_latency_db_probability_convert_coefficient_a_set_cb)(
    int unit, uint32 coefficient_a);

/**
 * implemented by: dnx_cosq_latency_db_probability_convert_coefficient_a_get
 */
typedef int (*dnx_cosq_latency_db_probability_convert_coefficient_a_get_cb)(
    int unit, uint32 *coefficient_a);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_set
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_set_cb)(
    int unit, dnx_algo_res_t aqm_flow_id_res);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_get
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_get_cb)(
    int unit, dnx_algo_res_t *aqm_flow_id_res);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_create
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_create_cb)(
    int unit, dnx_algo_res_create_data_t * data, void *extra_arguments);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_allocate_single
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_allocate_single_cb)(
    int unit, uint32 flags, void *extra_arguments, int *element);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_free_single
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_free_single_cb)(
    int unit, int element);

/**
 * implemented by: dnx_cosq_latency_db_aqm_flow_id_res_is_allocated
 */
typedef int (*dnx_cosq_latency_db_aqm_flow_id_res_is_allocated_cb)(
    int unit, int element, uint8 *is_allocated);

/*
 * STRUCTs
 */

/**
 * This structure holds the access functions for the variable ingress_latency_res
 */
typedef struct {
    dnx_cosq_latency_db_ingress_latency_res_set_cb set;
    dnx_cosq_latency_db_ingress_latency_res_get_cb get;
    dnx_cosq_latency_db_ingress_latency_res_create_cb create;
    dnx_cosq_latency_db_ingress_latency_res_allocate_single_cb allocate_single;
    dnx_cosq_latency_db_ingress_latency_res_free_single_cb free_single;
    dnx_cosq_latency_db_ingress_latency_res_is_allocated_cb is_allocated;
    dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_set_cb advanced_algorithm_info_set;
    dnx_cosq_latency_db_ingress_latency_res_advanced_algorithm_info_get_cb advanced_algorithm_info_get;
} dnx_cosq_latency_db_ingress_latency_res_cbs;

/**
 * This structure holds the access functions for the variable end_to_end_latency_res
 */
typedef struct {
    dnx_cosq_latency_db_end_to_end_latency_res_set_cb set;
    dnx_cosq_latency_db_end_to_end_latency_res_get_cb get;
    dnx_cosq_latency_db_end_to_end_latency_res_create_cb create;
    dnx_cosq_latency_db_end_to_end_latency_res_allocate_single_cb allocate_single;
    dnx_cosq_latency_db_end_to_end_latency_res_free_single_cb free_single;
    dnx_cosq_latency_db_end_to_end_latency_res_is_allocated_cb is_allocated;
} dnx_cosq_latency_db_end_to_end_latency_res_cbs;

/**
 * This structure holds the access functions for the variable end_to_end_aqm_latency_res
 */
typedef struct {
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_set_cb set;
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_get_cb get;
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_create_cb create;
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_allocate_single_cb allocate_single;
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_free_single_cb free_single;
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_is_allocated_cb is_allocated;
} dnx_cosq_latency_db_end_to_end_aqm_latency_res_cbs;

/**
 * This structure holds the access functions for the variable queue_template
 */
typedef struct {
    dnx_cosq_latency_db_queue_template_set_cb set;
    dnx_cosq_latency_db_queue_template_get_cb get;
    dnx_cosq_latency_db_queue_template_create_cb create;
    dnx_cosq_latency_db_queue_template_allocate_single_cb allocate_single;
    dnx_cosq_latency_db_queue_template_free_single_cb free_single;
    dnx_cosq_latency_db_queue_template_exchange_cb exchange;
    dnx_cosq_latency_db_queue_template_replace_data_cb replace_data;
    dnx_cosq_latency_db_queue_template_profile_data_get_cb profile_data_get;
    dnx_cosq_latency_db_queue_template_profile_get_cb profile_get;
    dnx_cosq_latency_db_queue_template_clear_cb clear;
    dnx_cosq_latency_db_queue_template_free_all_cb free_all;
    dnx_cosq_latency_db_queue_template_get_next_cb get_next;
    dnx_cosq_latency_db_queue_template_dump_info_get_cb dump_info_get;
} dnx_cosq_latency_db_queue_template_cbs;

/**
 * This structure holds the access functions for the variable exponent_b
 */
typedef struct {
    dnx_cosq_latency_db_probability_convert_exponent_b_set_cb set;
    dnx_cosq_latency_db_probability_convert_exponent_b_get_cb get;
} dnx_cosq_latency_db_probability_convert_exponent_b_cbs;

/**
 * This structure holds the access functions for the variable coefficient_a
 */
typedef struct {
    dnx_cosq_latency_db_probability_convert_coefficient_a_set_cb set;
    dnx_cosq_latency_db_probability_convert_coefficient_a_get_cb get;
} dnx_cosq_latency_db_probability_convert_coefficient_a_cbs;

/**
 * This structure holds the access functions for the variable dnx_cosq_latency_ecn_probability_convert_t
 */
typedef struct {
    /**
     * Access struct for exponent_b
     */
    dnx_cosq_latency_db_probability_convert_exponent_b_cbs exponent_b;
    /**
     * Access struct for coefficient_a
     */
    dnx_cosq_latency_db_probability_convert_coefficient_a_cbs coefficient_a;
} dnx_cosq_latency_db_probability_convert_cbs;

/**
 * This structure holds the access functions for the variable aqm_flow_id_res
 */
typedef struct {
    dnx_cosq_latency_db_aqm_flow_id_res_set_cb set;
    dnx_cosq_latency_db_aqm_flow_id_res_get_cb get;
    dnx_cosq_latency_db_aqm_flow_id_res_create_cb create;
    dnx_cosq_latency_db_aqm_flow_id_res_allocate_single_cb allocate_single;
    dnx_cosq_latency_db_aqm_flow_id_res_free_single_cb free_single;
    dnx_cosq_latency_db_aqm_flow_id_res_is_allocated_cb is_allocated;
} dnx_cosq_latency_db_aqm_flow_id_res_cbs;

/**
 * This structure holds the access functions for the variable dnx_cosq_latency_db_t
 */
typedef struct {
    dnx_cosq_latency_db_is_init_cb is_init;
    dnx_cosq_latency_db_init_cb init;
    /**
     * Access struct for ingress_latency_res
     */
    dnx_cosq_latency_db_ingress_latency_res_cbs ingress_latency_res;
    /**
     * Access struct for end_to_end_latency_res
     */
    dnx_cosq_latency_db_end_to_end_latency_res_cbs end_to_end_latency_res;
    /**
     * Access struct for end_to_end_aqm_latency_res
     */
    dnx_cosq_latency_db_end_to_end_aqm_latency_res_cbs end_to_end_aqm_latency_res;
    /**
     * Access struct for queue_template
     */
    dnx_cosq_latency_db_queue_template_cbs queue_template;
    /**
     * Access struct for probability_convert
     */
    dnx_cosq_latency_db_probability_convert_cbs probability_convert;
    /**
     * Access struct for aqm_flow_id_res
     */
    dnx_cosq_latency_db_aqm_flow_id_res_cbs aqm_flow_id_res;
} dnx_cosq_latency_db_cbs;

/*
 * Global Variables
 */

/*
 * Global Variables
 */

extern dnx_cosq_latency_db_cbs dnx_cosq_latency_db;

#endif /* __COSQ_LATENCY_ACCESS_H__ */
