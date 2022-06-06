/*! \file bcm56880_a0_tm_oobfc.h
 *
 * File containing OOBFC related defines and internal function for
 * bcm56880_a0.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCM56880_A0_TM_OOBFC_H
#define BCM56880_A0_TM_OOBFC_H

#include <bcmpc/bcmpc_types.h>
#include <bcmlrd/bcmlrd_types.h>
#include <bcmtm/oobfc/bcmtm_oobfc_internal.h>

/*!
 * \brief Get OOBFC driver for the device.
 *
 * \param [in] unit Logical unit ID.
 * \param [out] oobfc_drv OOBFC driver.
 *
 * \retval SHR_E_NONE No error.
 * \retval !SHR_E_NONE Error code.
 */
extern int
bcm56880_a0_tm_oobfc_drv_get(int unit, void *oobfc_drv);

#endif /* BCM56880_A0_TM_OOBFC_H */
