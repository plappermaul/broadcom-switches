/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_MOD_H
#define BCMINT_LTSW_MBCM_MOD_H

#include <bcm/cosq.h>

#include <bcm_int/ltsw/mod.h>

#include <sal/sal_types.h>

/*!
 * \brief Init Cosq mirror-on-drop module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_init_f)(int unit);

/*!
 * \brief detach Cosq mirror-on-drop module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_detach_f)(int unit);

/*!
 * \brief Set mirror-on-drop control configuration.
 *
 * \param [in] unit Unit Number.
 * \param [in] control Cosq mirror-on-drop control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_control_set_f)(
    int unit,
    bcm_cosq_mod_control_t *control);

/*!
 * \brief Get mirror-on-drop control configuration.
 *
 * \param [in] unit Unit Number.
 * \param [out] control Cosq mirror-on-drop control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_control_get_f)(
    int unit,
    bcm_cosq_mod_control_t *control);

/*!
 * \brief Create a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] options Options flags.
 * \param [out] profile_id Profile id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_profile_create_f)(
    int unit,
    uint32_t options,
    int *profile_id);

/*!
 * \brief Destroy a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_profile_destroy_f)(
    int unit,
    int profile_id);

/*!
 * \brief Set a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 * \param [in] profile Mirror-on-drop profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_profile_set_f)(
    int unit,
    int profile_id,
    bcm_cosq_mod_profile_t *profile);

/*!
 * \brief Get a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 * \param [out] profile Mirror-on-drop profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_profile_get_f)(
    int unit,
    int profile_id,
    bcm_cosq_mod_profile_t *profile);

/*!
 * \brief Add mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [in] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_dest_add_f)(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Delete mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [in] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_dest_delete_f)(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Get mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [out] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_dest_get_f)(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Get mirror instance id.
 *
 * \param [in] unit Unit Number.
 * \param [out] instance_id Mirror instance id. -1 if not found.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_instance_id_get_f)(
    int unit,
    int *instance_id);

/*!
 * \brief Get mirror-on-drop statistics.
 *
 * \param [in] unit Unit Number.
 * \param [in] id Statistics object id.
 * \param [in] stat Cosq mirror-on-drop Statistics type.
 * \param [out] value Statistics value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_stat_get_f)(
    int unit,
    bcm_cosq_object_id_t *id,
    bcm_cosq_mod_stat_t stat,
    uint64_t *value);

/*!
 * \brief Set mirror-on-drop statistics counter.
 *
 * \param [in] unit Unit Number.
 * \param [in] id Statistics object id.
 * \param [in] stat Cosq mirror-on-drop Statistics type
 * \param [in] value Statistics value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*cosq_mod_stat_set_f)(
    int unit,
    bcm_cosq_object_id_t *id,
    bcm_cosq_mod_stat_t stat,
    uint64_t value);

/*!
 * \brief Cosq_mod driver structure.
 */
typedef struct mbcm_ltsw_cosq_mod_drv_s {

    /*! Init Cosq mirror-on-drop module. */
    cosq_mod_init_f cosq_mod_init;

    /*! detach Cosq mirror-on-drop module. */
    cosq_mod_detach_f cosq_mod_detach;

    /*! Set mirror-on-drop control configuration. */
    cosq_mod_control_set_f cosq_mod_control_set;

    /*! Get mirror-on-drop control configuration. */
    cosq_mod_control_get_f cosq_mod_control_get;

    /*! Create a mirror-on-drop profile. */
    cosq_mod_profile_create_f cosq_mod_profile_create;

    /*! Destroy a mirror-on-drop profile. */
    cosq_mod_profile_destroy_f cosq_mod_profile_destroy;

    /*! Set a mirror-on-drop profile. */
    cosq_mod_profile_set_f cosq_mod_profile_set;

    /*! Get a mirror-on-drop profile. */
    cosq_mod_profile_get_f cosq_mod_profile_get;

    /*! Add mirror-on-drop destination. */
    cosq_mod_dest_add_f cosq_mod_dest_add;

    /*! Delete mirror-on-drop destination. */
    cosq_mod_dest_delete_f cosq_mod_dest_delete;

    /*! Get mirror-on-drop destination. */
    cosq_mod_dest_get_f cosq_mod_dest_get;

    /*! Get mirror instance id. */
    cosq_mod_instance_id_get_f cosq_mod_instance_id_get;

    /*! Get mirror-on-drop statistics. */
    cosq_mod_stat_get_f cosq_mod_stat_get;

    /*! Set mirror-on-drop statistics counter. */
    cosq_mod_stat_set_f cosq_mod_stat_set;

} mbcm_ltsw_cosq_mod_drv_t;

/*!
 * \brief Set the COSQ_MOD driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Cosq_mod driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_drv_set(
    int unit,
    mbcm_ltsw_cosq_mod_drv_t *drv);

/*!
 * \brief Init Cosq mirror-on-drop module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_init(int unit);

/*!
 * \brief detach Cosq mirror-on-drop module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_detach(int unit);

/*!
 * \brief Set mirror-on-drop control configuration.
 *
 * \param [in] unit Unit Number.
 * \param [in] control Cosq mirror-on-drop control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_control_set(
    int unit,
    bcm_cosq_mod_control_t *control);

/*!
 * \brief Get mirror-on-drop control configuration.
 *
 * \param [in] unit Unit Number.
 * \param [out] control Cosq mirror-on-drop control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_control_get(
    int unit,
    bcm_cosq_mod_control_t *control);

/*!
 * \brief Create a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] options Options flags.
 * \param [out] profile_id Profile id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_profile_create(
    int unit,
    uint32_t options,
    int *profile_id);

/*!
 * \brief Destroy a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_profile_destroy(
    int unit,
    int profile_id);

/*!
 * \brief Set a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 * \param [in] profile Mirror-on-drop profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_profile_set(
    int unit,
    int profile_id,
    bcm_cosq_mod_profile_t *profile);

/*!
 * \brief Get a mirror-on-drop profile.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile id.
 * \param [out] profile Mirror-on-drop profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_profile_get(
    int unit,
    int profile_id,
    bcm_cosq_mod_profile_t *profile);

/*!
 * \brief Add mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [in] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_dest_add(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Delete mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [in] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_dest_delete(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Get mirror-on-drop destination.
 *
 * \param [in] unit Unit Number.
 * \param [out] mod_dest Mirror-on-drop destination.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_dest_get(
    int unit,
    bcmi_ltsw_cosq_mod_dest_t *mod_dest);

/*!
 * \brief Get mirror instance id.
 *
 * \param [in] unit Unit Number.
 * \param [out] instance_id Mirror instance id. -1 if not found.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_instance_id_get(
    int unit,
    int *instance_id);

/*!
 * \brief Get mirror-on-drop statistics.
 *
 * \param [in] unit Unit Number.
 * \param [in] id Statistics object id.
 * \param [in] stat Cosq mirror-on-drop Statistics type.
 * \param [out] value Statistics value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_stat_get(
    int unit,
    bcm_cosq_object_id_t *id,
    bcm_cosq_mod_stat_t stat,
    uint64_t *value);

/*!
 * \brief Set mirror-on-drop statistics counter.
 *
 * \param [in] unit Unit Number.
 * \param [in] id Statistics object id.
 * \param [in] stat Cosq mirror-on-drop Statistics type
 * \param [in] value Statistics value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_cosq_mod_stat_set(
    int unit,
    bcm_cosq_object_id_t *id,
    bcm_cosq_mod_stat_t stat,
    uint64_t value);

#endif /* BCMINT_LTSW_MBCM_MOD_H */
