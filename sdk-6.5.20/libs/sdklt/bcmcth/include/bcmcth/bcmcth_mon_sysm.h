/*! \file bcmcth_mon_sysm.h
 *
 * BCMCTH System manager initialization
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMCTH_MON_SYSM_H
#define BCMCTH_MON_SYSM_H

/*!
 * \brief Start Monitor component.
 *
 * Register Monitor component with System Manager.
 *
 * \retval SHR_E_NONE   No errors.
 * \retval SHR_E_PARAM  Component not recognized.
 * \retval SHR_E_EXISTS Component already registered.
 */
extern int
bcmcth_mon_start(void);

#endif /* BCMCTH_MON_SYSM_H */
