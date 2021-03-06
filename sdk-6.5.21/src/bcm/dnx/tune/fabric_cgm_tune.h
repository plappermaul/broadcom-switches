/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*
 * ! \file bcm/dnx/tune/fabric_cgm_tune.h
 * Reserved.$ 
 */

#ifndef _DNX_FABRIC_CGM_TUNE_H_INCLUDED_
/*
 * { 
 */
#define _DNX_FABRIC_CGM_TUNE_H_INCLUDED_

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif

/**
 * \brief
 *   Perform initial tuning of fabric CGM module.
 * 
 * \param [in] unit -
 *   The unit number.
 * \return
 *   \retval See \ref shr_error_e
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_tune_fabric_cgm_set(
    int unit);

/** } */
#endif /*_DNX_FABRIC_CGM_TUNE_H_INCLUDED_*/
