/*
 * $Id: mdebug.h 1.5 Broadcom SDK $
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
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __TKS_MDEBUG_H__
#define __TKS_MDEBUG_H__

#include <soc/cmmdebug.h>

#define TKS_DBG_DISCOVER        SOC_CM_MDEBUG_OPT_ENC(0)
#define TKS_DBG_DISCV           SOC_CM_MDEBUG_OPT_ENC(1)
#define TKS_DBG_TOPOLOGY        SOC_CM_MDEBUG_OPT_ENC(2)
#define TKS_DBG_TOPOV           SOC_CM_MDEBUG_OPT_ENC(3)
#define TKS_DBG_STKTASK         SOC_CM_MDEBUG_OPT_ENC(4)
#define TKS_DBG_STKV            SOC_CM_MDEBUG_OPT_ENC(5)
#define TKS_DBG_TX              SOC_CM_MDEBUG_OPT_ENC(6)
#define TKS_DBG_NH              SOC_CM_MDEBUG_OPT_ENC(7)
#define TKS_DBG_NHV             SOC_CM_MDEBUG_OPT_ENC(8)
#define TKS_DBG_ATP             SOC_CM_MDEBUG_OPT_ENC(9)
#define TKS_DBG_ATPV            SOC_CM_MDEBUG_OPT_ENC(10)
#define TKS_DBG_C2C             SOC_CM_MDEBUG_OPT_ENC(11)
#define TKS_DBG_C2CV            SOC_CM_MDEBUG_OPT_ENC(12)
#define TKS_DBG_CTPKT           SOC_CM_MDEBUG_OPT_ENC(13)
#define TKS_DBG_CTPKTV          SOC_CM_MDEBUG_OPT_ENC(14)
#define TKS_DBG_TUNNEL          SOC_CM_MDEBUG_OPT_ENC(15)
#define TKS_DBG_TUNNELV         SOC_CM_MDEBUG_OPT_ENC(16)

#define TKS_DBG_COUNT           17

#define TKS_DBG_ERR             SOC_CM_DBG_ERR
#define TKS_DBG_WARN            SOC_CM_DBG_WARN
#define TKS_DBG_NORMAL          SOC_CM_DBG_NORMAL
#define TKS_DBG_VERBOSE         SOC_CM_DBG_VERBOSE
#define TKS_DBG_VVERBOSE        SOC_CM_DBG_VVERBOSE

#define TKS_DBG_NAMES   \
    "DISCover",         \
    "DISCV",            \
    "TOPOlogy",         \
    "TOPOV",            \
    "STKtask",          \
    "STKV",             \
    "TX",               \
    "NH",               \
    "NHV",              \
    "ATP",              \
    "ATPV",             \
    "C2C",              \
    "C2CV",             \
    "CTPKT",            \
    "CTPKTV",           \
    "TUNNEL",           \
    "TUNNELV",          \
    ""

/*
 * Proper use of debug print macros requires parentheses, e.g.:
 *
 *     TKS_OPT_WARN(("Problem %d with unit %d\n", pr, unit));
 *     TKS_WARN(("Problem %d with unit %d\n", pr, unit));
 *     TKS_DEBUG(TKS_DBG_FOO, ("Problem %d with unit %d\n", pr, unit));
 */

extern soc_cm_mdebug_config_t tks_mdebug_config;

extern int (*tks_mdebug_print)(const char *format, ...)
    COMPILER_ATTRIBUTE_FUNCTION_POINTER ((format (printf, 1, 2)));

#if defined(BROADCOM_DEBUG)

#define TKS_DEBUG_CHECK(enc_) \
    soc_cm_mdebug_check(&tks_mdebug_config, enc_)

#define TKS_DEBUG(enc_, stuff_) \
    if (TKS_DEBUG_CHECK(enc_) && tks_mdebug_print != 0) \
	(*tks_mdebug_print) stuff_

/*
 * Option-specific debug print macros.
 * Source file must define TKS_DEBUG_OPTION to the desired debug
 * option before using these macros, e.g.:
 *
 * #define TKS_DEBUG_OPTION TKS_DBG_DISCOVER
 */

#define TKS_OPT_ERR(stuff_) \
    TKS_DEBUG(TKS_DEBUG_OPTION | TKS_DBG_ERR, stuff_)
#define TKS_OPT_WARN(stuff_) \
    TKS_DEBUG(TKS_DEBUG_OPTION | TKS_DBG_WARN, stuff_)
#define TKS_OPT_NORM(stuff_) \
    TKS_DEBUG(TKS_DEBUG_OPTION | TKS_DBG_NORMAL, stuff_)
#define TKS_OPT_VERB(stuff_) \
    TKS_DEBUG(TKS_DEBUG_OPTION | TKS_DBG_VERBOSE, stuff_)
#define TKS_OPT_VVERB(stuff_) \
    TKS_DEBUG(TKS_DEBUG_OPTION | TKS_DBG_VVERBOSE, stuff_)

/* Module-wide debug print macros (do not require any options enabled) */
#define TKS_ERR(stuff_) \
    TKS_DEBUG(TKS_DBG_ERR, stuff_)
#define TKS_WARN(stuff_) \
    TKS_DEBUG(TKS_DBG_WARN, stuff_)
#define TKS_NORM(stuff_) \
    TKS_DEBUG(TKS_DBG_NORMAL, stuff_)
#define TKS_VERB(stuff_) \
    TKS_DEBUG(TKS_DBG_VERBOSE, stuff_)
#define TKS_VVERB(stuff_) \
    TKS_DEBUG(TKS_DBG_VVERBOSE, stuff_)

extern int tks_mdebug_init(void);

#else

#define TKS_DEBUG_CHECK(enc_) 0
#define TKS_DEBUG(flags_, stuff_)

#define TKS_OPT_ERR(stuff_)
#define TKS_OPT_WARN(stuff_)
#define TKS_OPT_NORM(stuff_)
#define TKS_OPT_VERB(stuff_)
#define TKS_OPT_VVERB(stuff_)

#define TKS_ERR(stuff_)
#define TKS_WARN(stuff_)
#define TKS_NORM(stuff_)
#define TKS_VERB(stuff_)
#define TKS_VVERB(stuff_)

#endif /* defined(BROADCOM_DEBUG) */

#endif /* __TKS_MDEBUG_H__ */
