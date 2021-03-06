/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_POLICER_H
#define BCMINT_LTSW_MBCM_POLICER_H

#include <sal/sal_types.h>
#include <bcm/policer.h>
#include <bcm_int/ltsw/policer.h>
#include <bcm_int/ltsw/policer_int.h>

/*!
 * \brief Deinit Policer module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_deinit_f)(int unit);

/*!
 * \brief Init Policer module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_init_f)(int unit);

/*!
 * \brief Create a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] pol_cfg Policer configuration.
 * \param [out] policer_id Policer ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_create_f)(
    int unit,
    bcm_policer_config_t *pol_cfg,
    bcm_policer_t *policer_id);

/*!
 * \brief Destroy a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_destroy_f)(
    int unit,
    bcm_policer_t policer_id);

/*!
 * \brief Destroy all policer entries.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_destroy_all_f)(int unit);

/*!
 * \brief Set the config settings for a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [in] pol_cfg Policer configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_set_f)(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg);

/*!
 * \brief Get the config settings for a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [out] pol_cfg Policer configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_get_f)(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg);

/*!
 * \brief Traverse all existing policer entries and call supplied callback function.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb Callback function.
 * \param [in] user_data User data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_traverse_f)(
    int unit,
    bcm_policer_traverse_cb cb,
    void *user_data);

/*!
 * \brief Get the maximum number of policers supported.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] num_meters Number of meters.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_capacity_get_f)(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *num_meters);

/*!
 * \brief Get the maximum number of meter pools.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] max_idx Maximum pool index.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_max_pool_idx_get_f)(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *max_idx);

/*!
 * \brief Install a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [in] pol_cfg Policer configuration.
 * \param [in] pipe Pipe instance.
 * \param [in] pool Pool number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_install_f)(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg,
    int pipe,
    int pool);

/*!
 * \brief Check if policers are supported in this stage.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] support Policer support status.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_supported_f)(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    bool *support);

/*!
 * \brief Check if policers are supported in this stage.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] flags Supported flags.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_stage_flags_get_f)(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *flags);

/*!
 * \brief Initialize mode of policer pools.
 *
 * \param [in] unit Unit Number.
 * \param [in] state Policer pool state.
 * \param [in] num_pipes Number of pipes.
 * \param [in] max_pools Number of pools.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*policer_pool_init_f)(
    int unit,
    bcmint_policer_pool_state_t *pool_state,
    int num_pipes,
    int max_pools);

/*!
 * \brief Policer driver structure.
 */
typedef struct mbcm_ltsw_policer_drv_s {

    /*! Deinit Policer module. */
    policer_deinit_f policer_deinit;

    /*! Init Policer module. */
    policer_init_f policer_init;

    /*! Create a policer entry. */
    policer_create_f policer_create;

    /*! Destroy a policer entry. */
    policer_destroy_f policer_destroy;

    /*! Destroy all policer entries. */
    policer_destroy_all_f policer_destroy_all;

    /*! Set the config settings for a policer entry. */
    policer_set_f policer_set;

    /*! Get the config settings for a policer entry. */
    policer_get_f policer_get;

    /*! Traverse all existing policer entries and call supplied callback function. */
    policer_traverse_f policer_traverse;

    /*! Get the maximum number of policers supported. */
    policer_capacity_get_f policer_capacity_get;

    /*! Get the maximum number of meter pools. */
    policer_max_pool_idx_get_f policer_max_pool_idx_get;

    /*! Install a policer entry. */
    policer_install_f policer_install;

    /*! Check if policers are supported. */
    policer_supported_f policer_supported;

    /*! Get policer stage flags. */
    policer_stage_flags_get_f policer_stage_flags_get;

    /*! Initialize policer pools. */
    policer_pool_init_f policer_pool_init;
} mbcm_ltsw_policer_drv_t;

/*!
 * \brief Set the POLICER driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Policer driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_drv_set(
    int unit,
    mbcm_ltsw_policer_drv_t *drv);

/*!
 * \brief Deinit Policer module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_deinit(int unit);

/*!
 * \brief Init Policer module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_init(int unit);

/*!
 * \brief Create a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] pol_cfg Policer configuration.
 * \param [out] policer_id Policer ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_create(
    int unit,
    bcm_policer_config_t *pol_cfg,
    bcm_policer_t *policer_id);

/*!
 * \brief Destroy a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_destroy(
    int unit,
    bcm_policer_t policer_id);

/*!
 * \brief Destroy all policer entries.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_destroy_all(int unit);

/*!
 * \brief Set the config settings for a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [in] pol_cfg Policer configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_set(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg);

/*!
 * \brief Get the config settings for a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [out] pol_cfg Policer configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_get(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg);

/*!
 * \brief Traverse all existing policer entries and call supplied callback function.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb Callback function.
 * \param [in] user_data User data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_traverse(
    int unit,
    bcm_policer_traverse_cb cb,
    void *user_data);

/*!
 * \brief Get the maximum number of policers supported.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] num_meters Number of meters.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_capacity_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *num_meters);

/*!
 * \brief Get the maximum number of meter pools.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] max_idx Maximum pool index.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_max_pool_idx_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *max_idx);

/*!
 * \brief Install a policer entry.
 *
 * \param [in] unit Unit Number.
 * \param [in] policer_id Policer ID.
 * \param [in] pol_cfg Policer configuration.
 * \param [in] pipe Pipe instance.
 * \param [in] pool Pool number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_install(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg,
    int pipe,
    int pool);

/*!
 * \brief Check if polices are supported in this stage.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] support Policer support status.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_supported(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    bool *support);

/*!
 * \brief Get supported flags for this stage.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Policer stage.
 * \param [out] flags Supported flags.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_stage_flags_get(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t *flags);

/*!
 * \brief Initialize mode of policer pools.
 *
 * \param [in] unit Unit Number.
 * \param [in] state Policer pool state.
 * \param [in] num_pipes Number of pipes.
 * \param [in] max_pools Number of pools.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_policer_pool_init(
    int unit,
    bcmint_policer_pool_state_t *pool_state,
    int num_pipes,
    int max_pools);

#endif /* BCMINT_LTSW_MBCM_POLICER_H */
