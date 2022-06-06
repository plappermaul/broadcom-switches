/** \file bcm_int/dnx/tsn/tsn.h
 * 
 *  
 */

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef _DNX_TSN_INCLUDED_
#define _DNX_TSN_INCLUDED_

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX family only!"
#endif

#include <bcm/tsn.h>
#include <soc/sand/shrextend/shrextend_debug.h>

/**
 * \brief
 *  TSN init function.
 *  Used for configurations for TSN features, required at init
 * \param [in] unit -
 *   The unit number.
 * \return
 *   See \ref shr_error_e
 * \remark
 *   None.
 * \see
 *   None.
 */
shr_error_e dnx_tsn_init(
    int unit);

#endif /* _DNX_TSN_INCLUDED_ */
