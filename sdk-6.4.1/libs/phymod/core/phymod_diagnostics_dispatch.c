/*
 *         
 * $Id: phymod.xml,v 1.1.2.5 Broadcom SDK $
 * 
 * $Copyright: Copyright 2012 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *         
 *     
 * DO NOT EDIT THIS FILE!
 *
 */

#include <phymod/phymod.h>
#include <phymod/phymod_diagnostics.h>
#include <phymod/phymod_diagnostics_dispatch.h>

#ifdef PHYMOD_EAGLE_SUPPORT
extern __phymod_diagnostics__dispatch__t__ phymod_diagnostics_eagle_diagnostics_driver;
#endif
#ifdef PHYMOD_FALCON_SUPPORT
extern __phymod_diagnostics__dispatch__t__ phymod_diagnostics_falcon_diagnostics_driver;
#endif
#ifdef PHYMOD_QSGMIIE_SUPPORT
extern __phymod_diagnostics__dispatch__t__ phymod_diagnostics_qsgmiie_diagnostics_driver;
#endif
#ifdef PHYMOD_TSCE_SUPPORT
extern __phymod_diagnostics__dispatch__t__ phymod_diagnostics_tsce_diagnostics_driver;
#endif
#ifdef PHYMOD_TSCF_SUPPORT
extern __phymod_diagnostics__dispatch__t__ phymod_diagnostics_tscf_diagnostics_driver;
#endif

__phymod_diagnostics__dispatch__t__* __phymod_diagnostics__dispatch__[phymodDispatchTypeCount] = {
#ifdef PHYMOD_EAGLE_SUPPORT
    &phymod_diagnostics_eagle_diagnostics_driver,
#endif
#ifdef PHYMOD_FALCON_SUPPORT
    &phymod_diagnostics_falcon_diagnostics_driver,
#endif
#ifdef PHYMOD_QSGMIIE_SUPPORT
    &phymod_diagnostics_qsgmiie_diagnostics_driver,
#endif
#ifdef PHYMOD_TSCE_SUPPORT
    &phymod_diagnostics_tsce_diagnostics_driver,
#endif
#ifdef PHYMOD_TSCF_SUPPORT
    &phymod_diagnostics_tscf_diagnostics_driver,
#endif

};

int phymod_slicer_position_t_validate(const phymod_slicer_position_t* phymod_slicer_position)
{
        
    if(phymod_slicer_position == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_slicer_position_t_init(phymod_slicer_position_t* phymod_slicer_position)
{

        
    
    if(phymod_slicer_position == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_slicer_position NULL parameter")));
    }

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_rx_slicer_position_set(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_slicer_position_t_validate(position)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("position validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_set) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_set(phy, flags, position);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_rx_slicer_position_set isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_rx_slicer_position_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_slicer_position_t* position)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(position == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("position NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_slicer_position_t_init(position)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("position initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_get(phy, flags, position);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_rx_slicer_position_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_rx_slicer_position_max_get(const phymod_phy_access_t* phy, uint32_t flags, const phymod_slicer_position_t* position_min, const phymod_slicer_position_t* position_max)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_slicer_position_t_validate(position_min)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("position_min validation failed")));
    }

    if(PHYMOD_E_OK != phymod_slicer_position_t_validate(position_max)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("position_max validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_max_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_rx_slicer_position_max_get(phy, flags, position_min, position_max);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_rx_slicer_position_max_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_prbs_poly_t_validate(phymod_prbs_poly_t phymod_prbs_poly)
{
        
    if(phymod_prbs_poly >= phymodPrbsPolyCount) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Parameter is out of range")));
    }
        
    return PHYMOD_E_NONE;
    
}

int phymod_prbs_t_validate(const phymod_prbs_t* phymod_prbs)
{
        
    if(phymod_prbs == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }

    if(PHYMOD_E_OK != phymod_prbs_poly_t_validate(phymod_prbs->poly)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("poly validation failed")));
    }

    switch(phymod_prbs->invert) {
        case 0:
        case 1:
            break;
        default:
            PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("invert not allowed value")));
            break;
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_prbs_t_init(phymod_prbs_t* phymod_prbs)
{

        
    
    if(phymod_prbs == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_prbs NULL parameter")));
    }
    phymod_prbs->poly = phymodPrbsPolyCount;
    phymod_prbs->invert = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_prbs_config_set(const phymod_phy_access_t* phy, uint32_t flags , const phymod_prbs_t* prbs)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_prbs_t_validate(prbs)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("prbs validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_config_set) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_config_set(phy, flags , prbs);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_prbs_config_set isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_prbs_config_get(const phymod_phy_access_t* phy, uint32_t flags , phymod_prbs_t* prbs)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(prbs == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("prbs NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_prbs_t_init(prbs)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("prbs initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_config_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_config_get(phy, flags , prbs);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_prbs_config_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_prbs_enable_set(const phymod_phy_access_t* phy, uint32_t flags , uint32_t enable)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    switch(enable) {
        case 0:
        case 1:
            break;
        default:
            PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("enable not allowed value")));
            break;
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_enable_set) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_enable_set(phy, flags , enable);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_prbs_enable_set isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_prbs_enable_get(const phymod_phy_access_t* phy, uint32_t flags , uint32_t* enable)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(enable == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("enable NULL parameter")));
    }
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_enable_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_enable_get(phy, flags , enable);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_prbs_enable_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_prbs_status_t_validate(const phymod_prbs_status_t* phymod_prbs_status)
{
        
    if(phymod_prbs_status == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_prbs_status_t_init(phymod_prbs_status_t* phymod_prbs_status)
{

        
    
    if(phymod_prbs_status == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_prbs_status NULL parameter")));
    }
    phymod_prbs_status->prbs_lock = 0;
    phymod_prbs_status->prbs_lock_loss = 0;
    phymod_prbs_status->error_count = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_prbs_status_get(const phymod_phy_access_t* phy, uint32_t flags, phymod_prbs_status_t* prbs_status)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(prbs_status == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("prbs_status NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_prbs_status_t_init(prbs_status)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("prbs_status initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_status_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_prbs_status_get(phy, flags, prbs_status);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_prbs_status_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_pattern_t_validate(const phymod_pattern_t* phymod_pattern)
{
        
    if(phymod_pattern == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_pattern_t_init(phymod_pattern_t* phymod_pattern)
{

        
    
    if(phymod_pattern == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_pattern NULL parameter")));
    }
    phymod_pattern->pattern = NULL;

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_pattern_config_set(const phymod_phy_access_t* phy, const phymod_pattern_t* pattern)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_pattern_t_validate(pattern)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("pattern validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_config_set) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_config_set(phy, pattern);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pattern_config_set isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pattern_config_get(const phymod_phy_access_t* phy, phymod_pattern_t* pattern)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(pattern == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("pattern NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_pattern_t_init(pattern)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("pattern initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_config_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_config_get(phy, pattern);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pattern_config_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pattern_enable_set(const phymod_phy_access_t* phy, uint32_t enable, const phymod_pattern_t* pattern)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_pattern_t_validate(pattern)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("pattern validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_enable_set) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_enable_set(phy, enable, pattern);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pattern_enable_set isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pattern_enable_get(const phymod_phy_access_t* phy, uint32_t* enable)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(enable == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("enable NULL parameter")));
    }
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_enable_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pattern_enable_get(phy, enable);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pattern_enable_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_core_diagnostics_t_validate(const phymod_core_diagnostics_t* phymod_core_diagnostics)
{
        
    if(phymod_core_diagnostics == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_core_diagnostics_t_init(phymod_core_diagnostics_t* phymod_core_diagnostics)
{

        
    
    if(phymod_core_diagnostics == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_core_diagnostics NULL parameter")));
    }
    phymod_core_diagnostics->temperature = 0;
    phymod_core_diagnostics->pll_range = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_core_diagnostics_get(const phymod_core_access_t* core, phymod_core_diagnostics_t* diag)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(diag == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("diag NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_core_diagnostics_t_init(diag)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("diag initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(core,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_core_diagnostics_get) {
        PHYMOD_LOCK_TAKE(core);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_core_diagnostics_get(core, diag);
        PHYMOD_LOCK_GIVE(core);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_core_diagnostics_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_diag_slicer_offset_t_validate(const phymod_diag_slicer_offset_t* phymod_diag_slicer_offset)
{
        
    if(phymod_diag_slicer_offset == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_diag_slicer_offset_t_init(phymod_diag_slicer_offset_t* phymod_diag_slicer_offset)
{

        
    
    if(phymod_diag_slicer_offset == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_diag_slicer_offset NULL parameter")));
    }
    phymod_diag_slicer_offset->offset_pe = 0;
    phymod_diag_slicer_offset->offset_ze = 0;
    phymod_diag_slicer_offset->offset_me = 0;
    phymod_diag_slicer_offset->offset_po = 0;
    phymod_diag_slicer_offset->offset_zo = 0;
    phymod_diag_slicer_offset->offset_mo = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_diag_eyescan_t_validate(const phymod_diag_eyescan_t* phymod_diag_eyescan)
{
        
    if(phymod_diag_eyescan == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_diag_eyescan_t_init(phymod_diag_eyescan_t* phymod_diag_eyescan)
{

        
    
    if(phymod_diag_eyescan == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_diag_eyescan NULL parameter")));
    }
    phymod_diag_eyescan->heye_left = 0;
    phymod_diag_eyescan->heye_right = 0;
    phymod_diag_eyescan->veye_upper = 0;
    phymod_diag_eyescan->veye_lower = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_osr_mode_t_validate(phymod_osr_mode_t phymod_osr_mode)
{
        
    if(phymod_osr_mode >= phymodOversampleModeCount) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Parameter is out of range")));
    }
        
    return PHYMOD_E_NONE;
    
}

int phymod_pmd_mode_t_validate(phymod_pmd_mode_t phymod_pmd_mode)
{
        
    if(phymod_pmd_mode >= phymodPmdModeCount) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Parameter is out of range")));
    }
        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_diagnostics_t_validate(const phymod_phy_diagnostics_t* phymod_phy_diagnostics)
{
        
    if(phymod_phy_diagnostics == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }

    if(PHYMOD_E_OK != phymod_osr_mode_t_validate(phymod_phy_diagnostics->osr_mode)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("osr_mode validation failed")));
    }

    if(PHYMOD_E_OK != phymod_pmd_mode_t_validate(phymod_phy_diagnostics->pmd_mode)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("pmd_mode validation failed")));
    }

    if(PHYMOD_E_OK != phymod_diag_slicer_offset_t_validate(&phymod_phy_diagnostics->slicer_offset)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("slicer_offset validation failed")));
    }

    if(PHYMOD_E_OK != phymod_diag_eyescan_t_validate(&phymod_phy_diagnostics->eyescan)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("eyescan validation failed")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_diagnostics_t_init(phymod_phy_diagnostics_t* phymod_phy_diagnostics)
{

        
    
    if(phymod_phy_diagnostics == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_phy_diagnostics NULL parameter")));
    }
    phymod_phy_diagnostics->signal_detect = 0;
    phymod_phy_diagnostics->vga_bias_reduced = 0;
    phymod_phy_diagnostics->postc_metric = 0;
    phymod_phy_diagnostics->osr_mode = 0;
    phymod_phy_diagnostics->pmd_mode = 0;
    phymod_phy_diagnostics->rx_lock = 0;
    phymod_phy_diagnostics->rx_ppm = 0;
    phymod_phy_diagnostics->tx_ppm = 0;
    phymod_phy_diagnostics->clk90_offset = 0;
    phymod_phy_diagnostics->clkp1_offset = 0;
    phymod_phy_diagnostics->p1_lvl = 0;
    phymod_phy_diagnostics->m1_lvl = 0;
    phymod_phy_diagnostics->dfe1_dcd = 0;
    phymod_phy_diagnostics->dfe2_dcd = 0;
    phymod_phy_diagnostics->slicer_target = 0;
    if(PHYMOD_E_OK != phymod_diag_slicer_offset_t_init(&phymod_phy_diagnostics->slicer_offset)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("slicer_offset initialization failed")));
    }

    if(PHYMOD_E_OK != phymod_diag_eyescan_t_init(&phymod_phy_diagnostics->eyescan)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("eyescan initialization failed")));
    }

    phymod_phy_diagnostics->state_machine_status = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_eyescan_options_t_validate(const phymod_phy_eyescan_options_t* phymod_phy_eyescan_options)
{
        
    if(phymod_phy_eyescan_options == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("NULL parameter")));
    }


        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_eyescan_options_t_init(phymod_phy_eyescan_options_t* phymod_phy_eyescan_options)
{

        
    
    if(phymod_phy_eyescan_options == NULL){
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("phymod_phy_eyescan_options NULL parameter")));
    }
    phymod_phy_eyescan_options->linerate_in_khz = 0;
    phymod_phy_eyescan_options->timeout_in_milliseconds = 0;
    phymod_phy_eyescan_options->horz_max = 0;
    phymod_phy_eyescan_options->horz_min = 0;
    phymod_phy_eyescan_options->hstep = 0;
    phymod_phy_eyescan_options->vert_max = 0;
    phymod_phy_eyescan_options->vert_min = 0;
    phymod_phy_eyescan_options->vstep = 0;
    phymod_phy_eyescan_options->mode = 0;

        
    return PHYMOD_E_NONE;
    
}

int phymod_phy_diagnostics_get(const phymod_phy_access_t* phy, phymod_phy_diagnostics_t* diag)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(diag == NULL) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("diag NULL parameter")));
    }
    if(PHYMOD_E_OK != phymod_phy_diagnostics_t_init(diag)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("diag initialization failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_diagnostics_get) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_diagnostics_get(phy, diag);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_diagnostics_get isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pmd_info_dump(const phymod_phy_access_t* phy, char* type)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pmd_info_dump) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pmd_info_dump(phy, type);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pmd_info_dump isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pcs_info_dump(const phymod_phy_access_t* phy, char* type)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pcs_info_dump) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pcs_info_dump(phy, type);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pcs_info_dump isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_meas_lowber_eye(const phymod_phy_access_t* phy, phymod_phy_eyescan_options_t eyescan_options, uint32_t* buffer)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_phy_eyescan_options_t_validate(&eyescan_options)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("eyescan_options validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_lowber_eye) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_lowber_eye(phy, eyescan_options, buffer);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_meas_lowber_eye isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_display_lowber_eye(const phymod_phy_access_t* phy, phymod_phy_eyescan_options_t eyescan_options, uint32_t* buffer)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    if(PHYMOD_E_OK != phymod_phy_eyescan_options_t_validate(&eyescan_options)) {
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("eyescan_options validation failed")));
    }

    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_lowber_eye) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_lowber_eye(phy, eyescan_options, buffer);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_display_lowber_eye isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_pmd_ber_end_cmd(const phymod_phy_access_t* phy, uint8_t supp_info, uint32_t timeout_ms)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pmd_ber_end_cmd) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_pmd_ber_end_cmd(phy, supp_info, timeout_ms);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_pmd_ber_end_cmd isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_meas_eye_scan_start(const phymod_phy_access_t* phy, uint8_t direction)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_eye_scan_start) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_eye_scan_start(phy, direction);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_meas_eye_scan_start isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_read_eye_scan_stripe(const phymod_phy_access_t* phy, uint32_t* buffer, uint16_t* status)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_read_eye_scan_stripe) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_read_eye_scan_stripe(phy, buffer, status);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_read_eye_scan_stripe isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_display_eye_scan_header(const phymod_phy_access_t* phy, int8_t i)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_header) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_header(phy, i);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_display_eye_scan_header isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_display_eye_scan_stripe(const phymod_phy_access_t* phy, int8_t y, uint32_t* buffer)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_stripe) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_stripe(phy, y, buffer);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_display_eye_scan_stripe isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_display_eye_scan_footer(const phymod_phy_access_t* phy, int8_t i)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_footer) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_display_eye_scan_footer(phy, i);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_display_eye_scan_footer isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_meas_eye_scan_done(const phymod_phy_access_t* phy)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_eye_scan_done) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_meas_eye_scan_done(phy);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_meas_eye_scan_done isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}


int phymod_phy_eye_scan_debug_info_dump(const phymod_phy_access_t* phy)
{

    phymod_dispatch_type_t __type__;
    int __rv__;
        
    
    /* Dispatch */
    PHYMOD_DRIVER_TYPE_GET(phy,&__type__);
    if(__type__ >= phymodDispatchTypeCount) { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_PARAM, (_PHYMOD_MSG("Driver is out of range")));
    }

    if(NULL != __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_eye_scan_debug_info_dump) {
        PHYMOD_LOCK_TAKE(phy);
        __rv__ = __phymod_diagnostics__dispatch__[__type__]->f_phymod_phy_eye_scan_debug_info_dump(phy);
        PHYMOD_LOCK_GIVE(phy);
        PHYMOD_IF_ERR_RETURN(__rv__);
    } else { 
        PHYMOD_RETURN_WITH_ERR(PHYMOD_E_UNAVAIL, (_PHYMOD_MSG("phymod_phy_eye_scan_debug_info_dump isn't implemented for driver type")));
    }
        
    return PHYMOD_E_NONE;
    
}

