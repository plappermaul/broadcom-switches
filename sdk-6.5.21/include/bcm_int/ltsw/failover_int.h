/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_FAILOVER_INT_H
#define BCMINT_LTSW_FAILOVER_INT_H

#include <sal/sal_types.h>

/*!
 * \brief Set PROT_NHOP_OFFSET field of LT TNL_CONTROL.
 *
 * \param [in] unit Unit Number.
 * \param [in] prot_offset Protection NH offset.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmint_failover_prot_offset_set(
    int unit,
    uint16_t prot_offset);

#endif /* BCMINT_LTSW_FAILOVER_INT_H */
