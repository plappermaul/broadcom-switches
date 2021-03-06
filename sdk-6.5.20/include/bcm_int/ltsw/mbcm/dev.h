/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_DEV_H
#define BCMINT_LTSW_MBCM_DEV_H

#include <bcm/types.h>

#include <bcm_int/ltsw/dev_int.h>

/*!
 * \brief Initialize the device information of the unit.
 *
 * \param [in] unit Unit Number.
 * \param [out] dev_info Device Information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*dev_info_init_f)(
    int unit,
    bcmint_dev_info_t *dev_info);

/*!
 * \brief Get PP pipe index of a given logical port for this unit.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [out] pipe PP pipe index, or -1 on error.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*dev_logic_port_pp_pipe_f)(
    int unit,
    bcm_port_t port,
    int *pipe);

/*!
 * \brief Get port index inside its PP pipe, for a given logical port for this unit.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [out] pp_port Port index inside its PP pipe, or -1 on error.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*dev_logic_port_pp_pipe_port_f)(
    int unit,
    bcm_port_t port,
    int *pp_port);

/*!
 * \brief Get logical port, from given PP pipe and port index inside its PP pipe.
 *
 * \param [in] unit Unit Number.
 * \param [in] pipe PP pipe index.
 * \param [in] pp_port Port index inside its PP pipe.
 * \param [out] port Port Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*dev_pp_pipe_port_logic_port_f)(
    int unit,
    int pipe,
    int pp_port,
    bcm_port_t *port);

/*!
 * \brief Get bit map of logical ports associated with a given PP pipe.
 *
 * \param [in] unit Unit Number.
 * \param [in] pipe PP pipe index.
 * \param [out] pbmp Bitmap of logical ports associated with the specified PP pipe.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*dev_pp_pipe_logic_pbmp_f)(
    int unit,
    int pipe,
    bcm_pbmp_t *pbmp);

/*!
 * \brief Dev driver structure.
 */
typedef struct mbcm_ltsw_dev_drv_s {

    /*! Initialize the device information of the unit. */
    dev_info_init_f dev_info_init;

    /*! Get PP pipe index of a given logical port for this unit. */
    dev_logic_port_pp_pipe_f dev_logic_port_pp_pipe;

    /*! Get port index inside its PP pipe, for a given logical port for this unit. */
    dev_logic_port_pp_pipe_port_f dev_logic_port_pp_pipe_port;

    /*! Get logical port, from given PP pipe and port index inside its PP pipe. */
    dev_pp_pipe_port_logic_port_f dev_pp_pipe_port_logic_port;

    /*! Get bit map of logical ports associated with a given PP pipe. */
    dev_pp_pipe_logic_pbmp_f dev_pp_pipe_logic_pbmp;

} mbcm_ltsw_dev_drv_t;

/*!
 * \brief Set the DEV driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Dev driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_drv_set(
    int unit,
    mbcm_ltsw_dev_drv_t *drv);

/*!
 * \brief Initialize the device information of the unit.
 *
 * \param [in] unit Unit Number.
 * \param [out] dev_info Device Information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_info_init(
    int unit,
    bcmint_dev_info_t *dev_info);

/*!
 * \brief Get PP pipe index of a given logical port for this unit.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [out] pipe PP pipe index, or -1 on error.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_logic_port_pp_pipe(
    int unit,
    bcm_port_t port,
    int *pipe);

/*!
 * \brief Get port index inside its PP pipe, for a given logical port for this unit.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [out] pp_port Port index inside its PP pipe, or -1 on error.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_logic_port_pp_pipe_port(
    int unit,
    bcm_port_t port,
    int *pp_port);

/*!
 * \brief Get logical port, from given PP pipe and port index inside its PP pipe.
 *
 * \param [in] unit Unit Number.
 * \param [in] pipe PP pipe index.
 * \param [in] pp_port Port index inside its PP pipe.
 * \param [out] port Port Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_pp_pipe_port_logic_port(
    int unit,
    int pipe,
    int pp_port,
    bcm_port_t *port);

/*!
 * \brief Get bit map of logical ports associated with a given PP pipe.
 *
 * \param [in] unit Unit Number.
 * \param [in] pipe PP pipe index.
 * \param [out] pbmp Bitmap of logical ports associated with the specified PP pipe.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_dev_pp_pipe_logic_pbmp(
    int unit,
    int pipe,
    bcm_pbmp_t *pbmp);

#endif /* BCMINT_LTSW_MBCM_DEV_H */
