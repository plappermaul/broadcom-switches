/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * $Id: $
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
 * File:        allmems_g.i
 * Purpose:     Independent memory descriptions.
 */

#if defined(BCM_88640_A0)
    { /* SOC_MEM_INT_GLOBALMETERPROFILESm */ 
        /* mem            GLOBALMETERPROFILESm */
        /* flags       */ SOC_MEM_FLAG_VALID,   /* 0x2 */
        /* cmp_fn      */ NULL,
        /* *null_entry */ NULL,
        /* index_min   */ 0,
        /* index_max   */ 7,
        /* minblock    */ 20,
        /* maxblock    */ 20,
        /* blocks      */ 0x100000,
        /* blocks_hi   */ 0,
        /* base        */ 0x001d0000,
        /* gran        */ 1,
        /* bytes       */ 4,
        /* nFields     */ 6,
        /* *fields     */ soc_GLOBALMETERPROFILESm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_88640_A0)
    { /* SOC_MEM_INT_GLOBALMETERSTATUSm */ 
        /* mem            GLOBALMETERSTATUSm */
        /* flags       */ SOC_MEM_FLAG_VALID,   /* 0x2 */
        /* cmp_fn      */ NULL,
        /* *null_entry */ NULL,
        /* index_min   */ 0,
        /* index_max   */ 7,
        /* minblock    */ 20,
        /* maxblock    */ 20,
        /* blocks      */ 0x100000,
        /* blocks_hi   */ 0,
        /* base        */ 0x001e0000,
        /* gran        */ 1,
        /* bytes       */ 5,
        /* nFields     */ 2,
        /* *fields     */ soc_GLOBALMETERSTATUSm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_88230_A0) || defined(BCM_88230_B0) || \
    defined(BCM_88230_C0)
    { /* SOC_MEM_INT_GLOBAL_STATSm */ 
        /* mem            GLOBAL_STATSm */
        /* flags       */ SOC_MEM_FLAG_VALID,   /* 0x2 */
        /* cmp_fn      */ NULL,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 16,
        /* minblock    */ 28,
        /* maxblock    */ 28,
        /* blocks      */ 0x10000000,
        /* blocks_hi   */ 0,
        /* base        */ 0x0b000000,
        /* gran        */ 1,
        /* bytes       */ 8,
        /* nFields     */ 2,
        /* *fields     */ soc_GLOBAL_STATSm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56634_A0) || defined(BCM_56634_B0)
    { /* SOC_MEM_INT_GPORT_EHG_RX_TUNNEL_DATAm */ 
        /* mem            GPORT_EHG_RX_TUNNEL_DATAm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 3,
        /* maxblock    */ 4,
        /* blocks      */ 0x18,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000000,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_DATAm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56524_A0) || defined(BCM_56524_B0)
    { /* SOC_MEM_INT_GPORT_EHG_RX_TUNNEL_DATA_BCM56524_A0m */ 
        /* mem            GPORT_EHG_RX_TUNNEL_DATAm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 2,
        /* maxblock    */ 3,
        /* blocks      */ 0xc,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000000,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_DATAm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56634_A0) || defined(BCM_56634_B0)
    { /* SOC_MEM_INT_GPORT_EHG_RX_TUNNEL_MASKm */ 
        /* mem            GPORT_EHG_RX_TUNNEL_MASKm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 3,
        /* maxblock    */ 4,
        /* blocks      */ 0x18,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000100,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_MASKm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56524_A0) || defined(BCM_56524_B0)
    { /* SOC_MEM_INT_GPORT_EHG_RX_TUNNEL_MASK_BCM56524_A0m */ 
        /* mem            GPORT_EHG_RX_TUNNEL_MASKm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 2,
        /* maxblock    */ 3,
        /* blocks      */ 0xc,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000100,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_MASKm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56634_A0) || defined(BCM_56634_B0)
    { /* SOC_MEM_INT_GPORT_EHG_TX_TUNNEL_DATAm */ 
        /* mem            GPORT_EHG_TX_TUNNEL_DATAm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 3,
        /* maxblock    */ 4,
        /* blocks      */ 0x18,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000200,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_DATAm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56524_A0) || defined(BCM_56524_B0)
    { /* SOC_MEM_INT_GPORT_EHG_TX_TUNNEL_DATA_BCM56524_A0m */ 
        /* mem            GPORT_EHG_TX_TUNNEL_DATAm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          SOC_MEM_FLAG_SER_ECC_CORRECTABLE,     /* 0x122 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 47,
        /* minblock    */ 2,
        /* maxblock    */ 3,
        /* blocks      */ 0xc,
        /* blocks_hi   */ 0,
        /* base        */ 0x00000200,
        /* gran        */ 1,
        /* bytes       */ 18,
        /* nFields     */ 4,
        /* *fields     */ soc_GPORT_EHG_RX_TUNNEL_DATAm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_88230_A0) || defined(BCM_88230_B0) || \
    defined(BCM_88230_C0)
    { /* SOC_MEM_INT_GROUP_MAX_SHAPER_TABLEm */ 
        /* mem            GROUP_MAX_SHAPER_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID,   /* 0x2 */
        /* cmp_fn      */ NULL,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 99,
        /* minblock    */ 15,
        /* maxblock    */ 15,
        /* blocks      */ 0x8000,
        /* blocks_hi   */ 0,
        /* base        */ 0x0e000000,
        /* gran        */ 1,
        /* bytes       */ 8,
        /* nFields     */ 7,
        /* *fields     */ soc_GROUP_MAX_SHAPER_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_88230_A0) || defined(BCM_88230_B0) || \
    defined(BCM_88230_C0)
    { /* SOC_MEM_INT_GROUP_MEMBER_TABLEm */ 
        /* mem            GROUP_MEMBER_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID,   /* 0x2 */
        /* cmp_fn      */ NULL,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 527,
        /* minblock    */ 15,
        /* maxblock    */ 15,
        /* blocks      */ 0x8000,
        /* blocks_hi   */ 0,
        /* base        */ 0x0f000000,
        /* gran        */ 1,
        /* bytes       */ 1,
        /* nFields     */ 2,
        /* *fields     */ soc_GROUP_MEMBER_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56640_A0)
    { /* SOC_MEM_INT_GTP_PORT_TABLEm */ 
        /* mem            GTP_PORT_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE,        /* 0x22 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 3,
        /* minblock    */ 7,
        /* maxblock    */ 7,
        /* blocks      */ 0x80,
        /* blocks_hi   */ 0,
        /* base        */ 0x14480000,
        /* gran        */ 1,
        /* bytes       */ 5,
        /* nFields     */ 3,
        /* *fields     */ soc_GTP_PORT_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56340_A0)
    { /* SOC_MEM_INT_GTP_PORT_TABLE_BCM56340_A0m */ 
        /* mem            GTP_PORT_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE,        /* 0x22 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 3,
        /* minblock    */ 4,
        /* maxblock    */ 4,
        /* blocks      */ 0x10,
        /* blocks_hi   */ 0,
        /* base        */ 0x14480000,
        /* gran        */ 1,
        /* bytes       */ 5,
        /* nFields     */ 3,
        /* *fields     */ soc_GTP_PORT_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56450_A0) || defined(BCM_56450_B0)
    { /* SOC_MEM_INT_GTP_PORT_TABLE_BCM56450_A0m */ 
        /* mem            GTP_PORT_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE,        /* 0x22 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 3,
        /* minblock    */ 8,
        /* maxblock    */ 8,
        /* blocks      */ 0x100,
        /* blocks_hi   */ 0,
        /* base        */ 0x14380000,
        /* gran        */ 1,
        /* bytes       */ 5,
        /* nFields     */ 3,
        /* *fields     */ soc_GTP_PORT_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */

#if defined(BCM_56850_A0)
    { /* SOC_MEM_INT_GTP_PORT_TABLE_BCM56850_A0m */ 
        /* mem            GTP_PORT_TABLEm */
        /* flags       */ SOC_MEM_FLAG_VALID |
                          SOC_MEM_FLAG_CACHABLE |
                          (3 << SOC_MEM_FLAG_ACC_TYPE_SHIFT),   /* 0x22 */
        /* cmp_fn      */ _soc_mem_cmp_undef,
        /* *null_entry */ _soc_mem_entry_null_zeroes,
        /* index_min   */ 0,
        /* index_max   */ 3,
        /* minblock    */ 10,
        /* maxblock    */ 10,
        /* blocks      */ 0x400,
        /* blocks_hi   */ 0,
        /* base        */ 0x14400000,
        /* gran        */ 1,
        /* bytes       */ 5,
        /* nFields     */ 3,
        /* *fields     */ soc_GTP_PORT_TABLEm_fields,
        /* *views      */ NULL,
        /* snoop_cb    */ NULL, 
        /* snoop_user_data */ NULL, 
        /* snoop_flags */ 0,
    },
#endif /* chips */
