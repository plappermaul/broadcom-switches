/*! \file bcmltx_ctr_port_q_pt_suppress.h
 *
 * CTR pt suppress transform handler.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMLTX_CTR_PORT_Q_PT_SUPPRESS_H
#define BCMLTX_CTR_PORT_Q_PT_SUPPRESS_H

#include <bcmltd/bcmltd_handler.h>

/*!
 * \brief Field forward transform.
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
bcmltx_ctr_port_q_pt_suppress_transform(int unit,
                                        const bcmltd_fields_t *in,
                                        bcmltd_fields_t *out,
                                        const bcmltd_transform_arg_t *arg);

#endif /* BCMLTX_CTR_PORT_Q_PT_SUPPRESS_H */



