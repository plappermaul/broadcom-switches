/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File:        dnx_ppp_dispatch.h
 * Purpose:     dnx ppp driver BCM API dispatch table
 * Generator:   modules_dispatch.py
 */

#ifndef _BCM_INT_DNX_PPP_DISPATCH_H
#define _BCM_INT_DNX_PPP_DISPATCH_H

#include <bcm/types.h>
#include <bcm_int/dispatch.h>

#ifdef BCM_DNX_SUPPORT

extern int bcm_dnx_ppp_term_spoofing_check_delete(int,bcm_ppp_term_spoofing_check_t *);
extern int bcm_dnx_ppp_terminator_traverse(int,bcm_ppp_terminator_additional_info_t *,bcm_ppp_terminator_traverse_cb,void *);
extern int bcm_dnx_ppp_initiator_get(int,bcm_ppp_initiator_t *);
extern int bcm_dnx_ppp_initiator_traverse(int,bcm_ppp_initiator_additional_info_t *,bcm_ppp_initiator_traverse_cb,void *);
extern int bcm_dnx_ppp_initiator_create(int,bcm_ppp_initiator_t *);
extern int bcm_dnx_ppp_initiator_delete(int,bcm_ppp_initiator_t *);
extern int bcm_dnx_ppp_term_spoofing_check_add(int,bcm_ppp_term_spoofing_check_t *);
extern int bcm_dnx_ppp_terminator_get(int,bcm_ppp_terminator_t *);
extern int bcm_dnx_ppp_terminator_create(int,bcm_ppp_terminator_t *);
extern int bcm_dnx_ppp_terminator_delete(int,bcm_ppp_terminator_t *);
extern int bcm_dnx_ppp_term_spoofing_check_traverse(int,bcm_ppp_term_spoofing_check_additional_info_t *,bcm_ppp_term_spoofing_check_traverse_cb,void *);

#endif /* BCM_DNX_SUPPORT */
#endif /* !_BCM_INT_DNX_PPP_DISPATCH_H */