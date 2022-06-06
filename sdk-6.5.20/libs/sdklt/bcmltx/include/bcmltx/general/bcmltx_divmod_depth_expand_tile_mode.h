/*! \file bcmltx_divmod_depth_expand_tile_mode.h
 *
 * Transforms for tile mode multi-entry L2P map depth expanded index tables.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMLTX_DIVMOD_DEPTH_EXPAND_TILE_MODE_H
#define BCMLTX_DIVMOD_DEPTH_EXPAND_TILE_MODE_H

#include <bcmltd/bcmltd_handler.h>

/*!
 * \brief Multi-entry depth expansion transform
 *
 * Index transform for tables that implement depth expansion with
 * multi-entry L2P maps.
 *
 * The index is divided by the number of tables;
 *   The quotient selects the table
 *   The remainder forms the physical table index.
 *
 * \param [in]  unit          Unit Number.
 * \param [in]  in            Input field values.
 * \param [out] out           Output field values.
 * \param [in]  arg           Transform arguments.
 *
 * \retval SHR_E_NONE         No errors.
 * \retval !SHR_E_NONE        Failure.
 */

extern int
bcmltx_divmod_depth_expand_tile_mode_transform(int unit,
                                               const bcmltd_fields_t *in,
                                               bcmltd_fields_t *out,
                                     const bcmltd_transform_arg_t *arg);

/*!
 * \brief Multi-entry depth expansion reverse transform
 *
 * The inverse of the above transform.
 *
 * \param [in]  unit          Unit Number.
 * \param [in]  in            Input field values.
 * \param [out] out           Output field values.
 * \param [in]  arg           Transform arguments.
 *
 * \retval SHR_E_NONE         No errors.
 * \retval !SHR_E_NONE        Failure.
 */

extern int
bcmltx_divmod_depth_expand_tile_mode_rev_transform(int unit,
                                                   const bcmltd_fields_t *in,
                                                   bcmltd_fields_t *out,
                                         const bcmltd_transform_arg_t *arg);

/*!
 * \brief Multi-entry depth expansion Atomic transaction preparation
 *
 * \param [in]  unit            Unit number.
 * \param [in]  trans_id        Transaction identifier.
 * \param [in]  arg             Handler arguments.
 *
 * Handler should return SHR_E_NONE if operation was performed.
 *
 * \retval SHR_E_NONE  OK
 * \retval !SHR_E_NONE ERROR
 */
extern int
bcmltx_divmod_depth_expand_tile_mode_prepare(int unit,
                                             uint32_t trans_id,
                                             const bcmltd_generic_arg_t *arg);

/*!
 * \brief Multi-entry depth expansion Atomic transaction commit
 *
 * \param [in]  unit            Unit number.
 * \param [in]  trans_id        Transaction identifier.
 * \param [in]  arg             Handler arguments.
 *
 * Handler should return SHR_E_NONE if operation was performed.
 *
 * \retval SHR_E_NONE  OK
 * \retval !SHR_E_NONE ERROR
 */
extern int
bcmltx_divmod_depth_expand_tile_mode_commit(int unit,
                                            uint32_t trans_id,
                                            const bcmltd_generic_arg_t *arg);

/*!
 * \brief Multi-entry depth expansion Atomic transaction abort
 *
 * \param [in]  unit            Unit number.
 * \param [in]  trans_id        Transaction identifier.
 * \param [in]  arg             Handler arguments.
 *
 * Handler should return SHR_E_NONE if operation was performed.
 *
 * \retval SHR_E_NONE  OK
 * \retval !SHR_E_NONE ERROR
 */
extern int
bcmltx_divmod_depth_expand_tile_mode_abort(int unit,
                                           uint32_t trans_id,
                                           const bcmltd_generic_arg_t *arg);

#endif /* BCMLTX_DIVMOD_DEPTH_EXPAND_TILE_MODE_H */
