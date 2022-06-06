/*! \file bcmfp_egr_grp_info.h
 *
 * APIs corresponding to group general info.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMFP_EGR_GRP_INFO_H
#define BCMFP_EGR_GRP_INFO_H

#include <bcmltd/bcmltd_handler.h>

/*!
 * \brief FP ingress group information table validation.
 *
 * \param [in] unit Unit Number.
 * \param [in] opcode LT opcode.
 * \param [out] in Input field values.
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_validate(int unit,
                            bcmlt_opcode_t opcode,
                            const bcmltd_fields_t *in,
                            const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table insert.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values.
 * \param [out] out Output field values (not applicable).
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_insert(int unit,
                          const bcmltd_op_arg_t *op_arg,
                          const bcmltd_fields_t *in,
                          bcmltd_fields_t *out,
                          const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table update.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values.
 * \param [out] out Output field value (not applicable).
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_update(int unit,
                          const bcmltd_op_arg_t *op_arg,
                          const bcmltd_fields_t *in,
                          bcmltd_fields_t *out,
                          const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table delete.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values.
 * \param [out] out Output field values (not applicable).
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_delete(int unit,
                          const bcmltd_op_arg_t *op_arg,
                          const bcmltd_fields_t *in,
                          bcmltd_fields_t *out,
                          const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table lookup.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values.
 * \param [out] out Output field values.
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_lookup(int unit,
                          const bcmltd_op_arg_t *op_arg,
                          const bcmltd_fields_t *in,
                          bcmltd_fields_t *out,
                          const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table traverse first.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values (not applicable).
 * \param [out] out Output field values.
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_first(int unit,
                         const bcmltd_op_arg_t *op_arg,
                         const bcmltd_fields_t *in,
                         bcmltd_fields_t *out,
                         const bcmltd_generic_arg_t *arg);

/*!
 * \brief FP ingress group information table traverse next.
 *
 * \param [in] unit Unit Number.
 * \param [in] op_arg Operation arguments.
 * \param [in] in Input field values.
 * \param [out] out Output field values.
 * \param [in] arg Custom table handler arguments.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmfp_egr_grp_info_next(int unit,
                        const bcmltd_op_arg_t *op_arg,
                        const bcmltd_fields_t *in,
                        bcmltd_fields_t *out,
                        const bcmltd_generic_arg_t *arg);

#endif /* BCMFP_EGR_GRP_INFO_H */

