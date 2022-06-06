/*! \file bcmcth_fd_attach.c
 *
 * BCMCTH Flex Digest attach/detach APIs. This is the interface calls to the
 * system manager.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_debug.h>
#include <bcmdrd/bcmdrd_dev.h>
#include <bcmcth/bcmcth_flex_digest.h>
#include <bcmcth/bcmcth_flex_digest_drv.h>

/* Debug log target definition */
#define BSL_LOG_MODULE BSL_LS_BCMCTH_FLEXDIGEST

/*******************************************************************************
* Public functions
 */
int
bcmcth_flex_digest_attach(int unit, bool warm)
{
    bcmdrd_dev_type_t dev_type;

    SHR_FUNC_ENTER(unit);

    dev_type = bcmdrd_dev_type(unit);
    if (dev_type == BCMDRD_DEV_T_NONE) {
        SHR_ERR_EXIT(SHR_E_UNIT);
    }

    /* Initialize device-specific driver */
    SHR_IF_ERR_EXIT
        (bcmcth_flex_digest_drv_init(unit, warm));

exit:
    SHR_FUNC_EXIT();
}

int
bcmcth_flex_digest_detach(int unit)
{
    bcmdrd_dev_type_t dev_type;

    SHR_FUNC_ENTER(unit);

    dev_type = bcmdrd_dev_type(unit);
    if (dev_type == BCMDRD_DEV_T_NONE) {
        SHR_ERR_EXIT(SHR_E_UNIT);
    }

    /* Device-specific driver cleanup */
    SHR_IF_ERR_EXIT
        (bcmcth_flex_digest_drv_cleanup(unit));

exit:
    SHR_FUNC_EXIT();
}
