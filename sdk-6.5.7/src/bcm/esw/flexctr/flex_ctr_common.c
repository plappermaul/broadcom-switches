/*
 * $Id: flex_ctr_common.c,v 1.93 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        flex_ctr_common.c
 * Purpose:     Manage common functionality for flex counter implementation
 */

#include <shared/bsl.h>

#include <bcm_int/esw/flex_ctr.h>
#include <bcm_int/esw/stat.h>
#include <bcm_int/esw/virtual.h>
#include <bcm_int/esw/field.h>
#include <shared/idxres_afl.h>
#include <soc/scache.h>
#include <bcm_int/esw/mpls.h>
#if defined (BCM_TRIDENT2PLUS_SUPPORT) && defined (INCLUDE_L3)
#include <bcm_int/esw/triumph.h>
#endif
#ifdef BCM_TOMAHAWK_SUPPORT
#include <soc/tomahawk.h>
#include <bcm_int/esw/tomahawk.h>
#endif

#define _MAX_PIPES_PER_DEV(unit) ((SOC_INFO(unit).num_pipe))
#if defined(BCM_TRIDENT2_SUPPORT)
#include <soc/trident2.h>
#include <bcm_int/esw/triumph2.h>
#endif

uint8     bcm_stat_flex_packet_attr_type_names_t[][64] = {
          "bcmStatFlexPacketAttrTypeUncompressed",
          "bcmStatFlexPacketAttrTypeCompressed",
          "bcmStatFlexPacketAttrTypeUdf"};
static soc_reg_t _pool_ctr_register[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
#ifndef BCM_TOMAHAWK_SUPPORT
                                   [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {
#else
                                   [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {
#endif
                 {ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_0r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_1r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_2r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_3r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_4r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_5r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_6r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_7r,
#ifdef BCM_TRIUMPH3_SUPPORT
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_8r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_9r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_10r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_11r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_12r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_13r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_14r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_15r,
#ifdef BCM_TOMAHAWK_SUPPORT
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_16r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_17r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_18r,
                  ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_19r
#endif
                  },
#else
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},
#endif
                 {EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_0r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_1r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_2r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_3r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_4r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_5r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_6r,
                  EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_7r,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */}
                 };
static soc_mem_t _ctr_offset_table[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
#ifndef BCM_TOMAHAWK_SUPPORT
                                  [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {
#else
                                   [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {
#endif
                 {ING_FLEX_CTR_OFFSET_TABLE_0m,
                  ING_FLEX_CTR_OFFSET_TABLE_1m,
                  ING_FLEX_CTR_OFFSET_TABLE_2m,
                  ING_FLEX_CTR_OFFSET_TABLE_3m,
                  ING_FLEX_CTR_OFFSET_TABLE_4m,
                  ING_FLEX_CTR_OFFSET_TABLE_5m,
                  ING_FLEX_CTR_OFFSET_TABLE_6m,
                  ING_FLEX_CTR_OFFSET_TABLE_7m,
#ifdef BCM_TRIUMPH3_SUPPORT
                  ING_FLEX_CTR_OFFSET_TABLE_8m,
                  ING_FLEX_CTR_OFFSET_TABLE_9m,
                  ING_FLEX_CTR_OFFSET_TABLE_10m,
                  ING_FLEX_CTR_OFFSET_TABLE_11m,
                  ING_FLEX_CTR_OFFSET_TABLE_12m,
                  ING_FLEX_CTR_OFFSET_TABLE_13m,
                  ING_FLEX_CTR_OFFSET_TABLE_14m,
                  ING_FLEX_CTR_OFFSET_TABLE_15m,
#ifdef BCM_TOMAHAWK_SUPPORT
                  ING_FLEX_CTR_OFFSET_TABLE_16m,
                  ING_FLEX_CTR_OFFSET_TABLE_17m,
                  ING_FLEX_CTR_OFFSET_TABLE_18m,
                  ING_FLEX_CTR_OFFSET_TABLE_19m
#endif
                  },
#else
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},
#endif
                 {EGR_FLEX_CTR_OFFSET_TABLE_0m,
                  EGR_FLEX_CTR_OFFSET_TABLE_1m,
                  EGR_FLEX_CTR_OFFSET_TABLE_2m,
                  EGR_FLEX_CTR_OFFSET_TABLE_3m,
                  EGR_FLEX_CTR_OFFSET_TABLE_4m,
                  EGR_FLEX_CTR_OFFSET_TABLE_5m,
                  EGR_FLEX_CTR_OFFSET_TABLE_6m,
                  EGR_FLEX_CTR_OFFSET_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */}
                 };
static soc_mem_t _ctr_counter_table[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                   [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {
                 {ING_FLEX_CTR_COUNTER_TABLE_0m,
                  ING_FLEX_CTR_COUNTER_TABLE_1m,
                  ING_FLEX_CTR_COUNTER_TABLE_2m,
                  ING_FLEX_CTR_COUNTER_TABLE_3m,
                  ING_FLEX_CTR_COUNTER_TABLE_4m,
                  ING_FLEX_CTR_COUNTER_TABLE_5m,
                  ING_FLEX_CTR_COUNTER_TABLE_6m,
                  ING_FLEX_CTR_COUNTER_TABLE_7m,
#ifdef BCM_TRIUMPH3_SUPPORT
                  ING_FLEX_CTR_COUNTER_TABLE_8m,
                  ING_FLEX_CTR_COUNTER_TABLE_9m,
                  ING_FLEX_CTR_COUNTER_TABLE_10m,
                  ING_FLEX_CTR_COUNTER_TABLE_11m,
                  ING_FLEX_CTR_COUNTER_TABLE_12m,
                  ING_FLEX_CTR_COUNTER_TABLE_13m,
                  ING_FLEX_CTR_COUNTER_TABLE_14m,
                  ING_FLEX_CTR_COUNTER_TABLE_15m,
#ifdef BCM_TOMAHAWK_SUPPORT
                  ING_FLEX_CTR_COUNTER_TABLE_16m,
                  ING_FLEX_CTR_COUNTER_TABLE_17m,
                  ING_FLEX_CTR_COUNTER_TABLE_18m,
                  ING_FLEX_CTR_COUNTER_TABLE_19m
#endif
                  },
#else
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},
#endif
                 {EGR_FLEX_CTR_COUNTER_TABLE_0m,
                  EGR_FLEX_CTR_COUNTER_TABLE_1m,
                  EGR_FLEX_CTR_COUNTER_TABLE_2m,
                  EGR_FLEX_CTR_COUNTER_TABLE_3m,
                  EGR_FLEX_CTR_COUNTER_TABLE_4m,
                  EGR_FLEX_CTR_COUNTER_TABLE_5m,
                  EGR_FLEX_CTR_COUNTER_TABLE_6m,
                  EGR_FLEX_CTR_COUNTER_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */}
                 };
#ifdef BCM_TOMAHAWK_SUPPORT
static soc_mem_t _per_pipe_ctr_counter_table[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                   [SOC_MAX_NUM_PIPES]
                                   [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {
                  {{ING_FLEX_CTR_COUNTER_TABLE_0_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_1_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_2_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_3_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_4_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_5_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_6_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_7_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_8_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_9_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_10_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_11_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_12_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_13_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_14_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_15_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_16_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_17_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_18_PIPE0m,
                  ING_FLEX_CTR_COUNTER_TABLE_19_PIPE0m
                  },
                 {ING_FLEX_CTR_COUNTER_TABLE_0_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_1_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_2_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_3_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_4_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_5_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_6_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_7_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_8_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_9_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_10_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_11_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_12_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_13_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_14_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_15_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_16_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_17_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_18_PIPE1m,
                  ING_FLEX_CTR_COUNTER_TABLE_19_PIPE1m
                  },
                 {ING_FLEX_CTR_COUNTER_TABLE_0_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_1_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_2_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_3_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_4_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_5_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_6_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_7_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_8_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_9_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_10_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_11_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_12_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_13_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_14_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_15_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_16_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_17_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_18_PIPE2m,
                  ING_FLEX_CTR_COUNTER_TABLE_19_PIPE2m
                  },
                 {ING_FLEX_CTR_COUNTER_TABLE_0_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_1_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_2_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_3_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_4_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_5_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_6_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_7_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_8_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_9_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_10_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_11_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_12_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_13_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_14_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_15_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_16_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_17_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_18_PIPE3m,
                  ING_FLEX_CTR_COUNTER_TABLE_19_PIPE3m,
                  }},

                 /* GLOBAL view is used for egress counters */
                 {{EGR_FLEX_CTR_COUNTER_TABLE_0m,
                  EGR_FLEX_CTR_COUNTER_TABLE_1m,
                  EGR_FLEX_CTR_COUNTER_TABLE_2m,
                  EGR_FLEX_CTR_COUNTER_TABLE_3m,
                  EGR_FLEX_CTR_COUNTER_TABLE_4m,
                  EGR_FLEX_CTR_COUNTER_TABLE_5m,
                  EGR_FLEX_CTR_COUNTER_TABLE_6m,
                  EGR_FLEX_CTR_COUNTER_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},

                  {EGR_FLEX_CTR_COUNTER_TABLE_0m,
                  EGR_FLEX_CTR_COUNTER_TABLE_1m,
                  EGR_FLEX_CTR_COUNTER_TABLE_2m,
                  EGR_FLEX_CTR_COUNTER_TABLE_3m,
                  EGR_FLEX_CTR_COUNTER_TABLE_4m,
                  EGR_FLEX_CTR_COUNTER_TABLE_5m,
                  EGR_FLEX_CTR_COUNTER_TABLE_6m,
                  EGR_FLEX_CTR_COUNTER_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},

                  {EGR_FLEX_CTR_COUNTER_TABLE_0m,
                  EGR_FLEX_CTR_COUNTER_TABLE_1m,
                  EGR_FLEX_CTR_COUNTER_TABLE_2m,
                  EGR_FLEX_CTR_COUNTER_TABLE_3m,
                  EGR_FLEX_CTR_COUNTER_TABLE_4m,
                  EGR_FLEX_CTR_COUNTER_TABLE_5m,
                  EGR_FLEX_CTR_COUNTER_TABLE_6m,
                  EGR_FLEX_CTR_COUNTER_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */},

                  {EGR_FLEX_CTR_COUNTER_TABLE_0m,
                  EGR_FLEX_CTR_COUNTER_TABLE_1m,
                  EGR_FLEX_CTR_COUNTER_TABLE_2m,
                  EGR_FLEX_CTR_COUNTER_TABLE_3m,
                  EGR_FLEX_CTR_COUNTER_TABLE_4m,
                  EGR_FLEX_CTR_COUNTER_TABLE_5m,
                  EGR_FLEX_CTR_COUNTER_TABLE_6m,
                  EGR_FLEX_CTR_COUNTER_TABLE_7m,
                  0, 0, 0, 0, 0, 0, 0, 0 /* Kept it for future updates */}}
                 };
#endif
#define _CTR_COUNTER_TABLE_IFP(_unit, _dir, _pipe_num, _pool, _mem) {        \
    if (!SOC_IS_TOMAHAWKX(unit) || _bcm_esw_get_fp_mode_global(_unit,        \
                                           bcmFieldQualifyStageIngress) ||   \
            (bcmStatFlexDirectionIngress != _dir) ||                         \
            (FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE !=                   \
                           flex_pool_stat[_unit][bcmStatFlexDirectionIngress]\
                                         [0][_pool].used_by_tables)) {       \
         _mem = _ctr_counter_table[_dir][_pool];                             \
    } else {                                                                 \
         _mem = _per_pipe_ctr_counter_table[_dir][_pipe_num][_pool];         \
    }                                                                        \
} 

#define _CTR_COUNTER_TABLE_VFP(_unit, _dir, _pipe_num, _pool, _mem) {        \
    if (!SOC_IS_TOMAHAWKX(unit) || _bcm_esw_get_fp_mode_global(_unit,        \
                                           bcmFieldQualifyStageLookup) ||    \
            (bcmStatFlexDirectionIngress != _dir) ||                         \
            (FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE !=                   \
                           flex_pool_stat[_unit][bcmStatFlexDirectionIngress]\
                                         [0][_pool].used_by_tables)) {       \
         _mem = _ctr_counter_table[_dir][_pool];                             \
    } else {                                                                 \
         _mem = _per_pipe_ctr_counter_table[_dir][_pipe_num][_pool];         \
    }                                                                        \
} 

#define _CTR_COUNTER_TABLE_EM(_unit, _dir, _pipe_num, _pool, _mem) {         \
    if (!SOC_IS_TOMAHAWKX(unit) || _bcm_esw_get_fp_mode_global(_unit,         \
                                bcmFieldQualifyStageIngressExactMatch) ||    \
            (bcmStatFlexDirectionIngress != _dir) ||                         \
            (FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE !=                   \
                           flex_pool_stat[_unit][bcmStatFlexDirectionIngress]\
                                         [0][_pool].used_by_tables)) {       \
         _mem = _ctr_counter_table[_dir][_pool];                             \
    } else {                                                                 \
         _mem = _per_pipe_ctr_counter_table[_dir][_pipe_num][_pool];         \
    }                                                                        \
} 

#if defined(BCM_TRIDENT2_SUPPORT)
static soc_mem_t _ctr_counter_table_x[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                     [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {
                 {ING_FLEX_CTR_COUNTER_TABLE_0_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_1_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_2_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_3_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_4_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_5_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_6_Xm,
                  ING_FLEX_CTR_COUNTER_TABLE_7_Xm},
                 {EGR_FLEX_CTR_COUNTER_TABLE_0_Xm,
                  EGR_FLEX_CTR_COUNTER_TABLE_1_Xm,
                  EGR_FLEX_CTR_COUNTER_TABLE_2_Xm,
                  EGR_FLEX_CTR_COUNTER_TABLE_3_Xm}
                 };

static soc_mem_t _ctr_counter_table_y[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                     [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {
                 {ING_FLEX_CTR_COUNTER_TABLE_0_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_1_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_2_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_3_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_4_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_5_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_6_Ym,
                  ING_FLEX_CTR_COUNTER_TABLE_7_Ym},
                 {EGR_FLEX_CTR_COUNTER_TABLE_0_Ym,
                  EGR_FLEX_CTR_COUNTER_TABLE_1_Ym,
                  EGR_FLEX_CTR_COUNTER_TABLE_2_Ym,
                  EGR_FLEX_CTR_COUNTER_TABLE_3_Ym}
                 };
#endif  /* BCM_TRIDENT2_SUPPORT */

static soc_reg_t _pkt_selector_key_reg[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
#ifndef BCM_TOMAHAWK_SUPPORT
                                  [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {
#else
                                  [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {
#endif
                 {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_0r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_1r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_2r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_3r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_4r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_5r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_6r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_7r,
#ifdef BCM_TOMAHAWK_SUPPORT
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_8r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_9r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_10r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_11r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_12r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_13r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_14r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_15r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_16r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_17r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_18r,
                  ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_19r},
#else
                  0, 0, 0, 0, 0, 0, 0, 0},
#endif /* BCM_TOMAHAWK_SUPPORT */
                 {EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_0r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_1r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_2r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_3r,
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_4r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_5r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_6r,
                  EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_7r}
#else
                  0, 0, 0, 0}
#endif
                };

static soc_field_t _pkt_selector_x_en_field_name[BCM_STAT_FLEX_COUNTER_MAX_ATTR_SEL_BITS] = {
                   SELECTOR_0_ENf,
                   SELECTOR_1_ENf,
                   SELECTOR_2_ENf,
                   SELECTOR_3_ENf,
                   SELECTOR_4_ENf,
                   SELECTOR_5_ENf,
                   SELECTOR_6_ENf,
                   SELECTOR_7_ENf,
                   };
static soc_field_t _pkt_selector_for_bit_x_field_name[BCM_STAT_FLEX_COUNTER_MAX_ATTR_SEL_BITS] = {
                   SELECTOR_FOR_BIT_0f,
                   SELECTOR_FOR_BIT_1f,
                   SELECTOR_FOR_BIT_2f,
                   SELECTOR_FOR_BIT_3f,
                   SELECTOR_FOR_BIT_4f,
                   SELECTOR_FOR_BIT_5f,
                   SELECTOR_FOR_BIT_6f,
                   SELECTOR_FOR_BIT_7f,
                   };

#if defined(BCM_TOMAHAWK_SUPPORT) && defined(BCM_WARM_BOOT_SUPPORT)
static soc_mem_t _ifp_mem_arr[] = {
                               IFP_POLICY_TABLE_PIPE0m,
                               IFP_POLICY_TABLE_PIPE1m,
                               IFP_POLICY_TABLE_PIPE2m,
                               IFP_POLICY_TABLE_PIPE3m
                              };
static soc_mem_t _vfp_mem_arr[] = {
                               VFP_POLICY_TABLE_PIPE0m,
                               VFP_POLICY_TABLE_PIPE1m,
                               VFP_POLICY_TABLE_PIPE2m,
                               VFP_POLICY_TABLE_PIPE3m
                              };
#endif

static bcm_stat_flex_ingress_mode_t *flex_ingress_modes[SOC_MAX_NUM_DEVICES] = {NULL};
static bcm_stat_flex_egress_mode_t  *flex_egress_modes[SOC_MAX_NUM_DEVICES] = {NULL};
static bcm_stat_flex_custom_ingress_mode_t *flex_custom_ingress_modes[SOC_MAX_NUM_DEVICES] = {NULL};


static shr_aidxres_list_handle_t flex_aidxres_list_handle
                                 [SOC_MAX_NUM_DEVICES]
                                 [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                 [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
                                 [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL];
#else
                                 [BCM_STAT_FLEX_COUNTER_MAX_POOL];
#endif

static bcm_stat_flex_pool_stat_t flex_pool_stat
                                 [SOC_MAX_NUM_DEVICES]
                                 [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                 [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
                                 [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL];
#else
                                 [BCM_STAT_FLEX_COUNTER_MAX_POOL];
#endif
/* For TD2PLUS, Flex counters for OAM is used in Ingress only. Hence pool_group
 * database is only maintained on Ingress only. 
 */
static bcm_stat_flex_pool_group_t  flex_pool_group
                                   [SOC_MAX_NUM_DEVICES]
                                   [SOC_MAX_NUM_PIPES]
                                   [BCM_STAT_TH_FLEX_COUNTER_MAX_GROUPS];

static uint16 *flex_base_index_reference_count
                                 [SOC_MAX_NUM_DEVICES]
                                 [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                                 [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
                                 [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL];
#else
                                 [BCM_STAT_FLEX_COUNTER_MAX_POOL];
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
uint8 *_flex_stat_mode_base_map[SOC_MAX_NUM_DEVICES]
                               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
                               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
                               [BCM_STAT_FLEX_COUNTER_MAX_MODE];
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TOMAHAWK_SUPPORT)
/* Database to maintain ifp color map per counter */
static uint8 *ifp_color_map[SOC_MAX_NUM_DEVICES]
                           [SOC_MAX_NUM_PIPES]
                           [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL] = {{{NULL}}};
bcm_error_t _bcm_esw_custom_stat_group_id_retrieve(
    int unit,
    uint32 offset_mode,
    int  pipe,
    uint8  pool,
    uint32 base_idx,
    uint32 *stat_ctr_id);
bcm_error_t _bcm_esw_ifp_wb_color_map_set(
        int                   unit,
        uint32                pool_number,
        uint32                base_index,
        uint32                pipe_num,
        bcm_stat_flex_mode_t offset_mode,
        _bcm_ifp_color_map_t  *color_map);
#endif

extern 
bcm_error_t _bcm_esw_stat_attr_fill(
            int	                  unit,
            bcm_stat_group_mode_t group_mode,
            bcm_stat_flex_direction_t direction,
            bcm_stat_flex_attr_t     *attr,
            uint32 *num_counters);
static sal_mutex_t flex_stat_mutex[SOC_MAX_NUM_DEVICES] = {NULL};

/* Both ing_flex_ctr_counter_table_0_entry_t and egr_flex_ctr_table_0_entry_t
   have same contents so using one entry only */
static ing_flex_ctr_counter_table_0_entry_t
       *flex_temp_counter[SOC_MAX_NUM_DEVICES]
                         [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION] = {{NULL}};
#if defined(BCM_TRIDENT2_SUPPORT)
static ing_flex_ctr_counter_table_0_entry_t
       *flex_temp_counter_dual[SOC_MAX_NUM_DEVICES]
                              [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION] = {{NULL}};
static uint32 *flex_pkt_counter_x
               [BCM_MAX_NUM_UNITS]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
               [_BCM_FLEX_STAT_BUFFS] = {{{{NULL}}}};
static uint64 *flex_byte_counter_x
               [SOC_MAX_NUM_DEVICES]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
               [_BCM_FLEX_STAT_BUFFS] = {{{{NULL}}}};

static uint32 *flex_pkt_counter_y
               [BCM_MAX_NUM_UNITS]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
               [_BCM_FLEX_STAT_BUFFS] = {{{{NULL}}}};
static uint64 *flex_byte_counter_y
               [SOC_MAX_NUM_DEVICES]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
               [_BCM_FLEX_STAT_BUFFS] = {{{{NULL}}}};
static uint8 *flex_counter_toggle
               [BCM_MAX_NUM_UNITS]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [BCM_STAT_FLEX_COUNTER_MAX_POOL] = {{{NULL}}};
#endif

static uint64 *flex_byte_counter
               [SOC_MAX_NUM_DEVICES]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
               [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL]
#else
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
#endif
               = {{{{NULL}}}};
static uint32 *flex_packet_counter
               [SOC_MAX_NUM_DEVICES]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
               [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL]
#else
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
#endif
                = {{{{NULL}}}};
static uint64 *flex_packet64_counter
               [SOC_MAX_NUM_DEVICES]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
               [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL]
#else
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
#endif
               = {{{{NULL}}}};

#ifdef BCM_TOMAHAWK_SUPPORT
static uint64 *residual_byte_counter
               [BCM_UNITS_MAX]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
               [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL]
#else
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
#endif
               = {{{{NULL}}}};
static uint64 *residual_packet_counter
               [BCM_UNITS_MAX]
               [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
               [SOC_MAX_NUM_PIPES]
#ifdef BCM_TOMAHAWK_SUPPORT
               [BCM_STAT_TH_FLEX_COUNTER_MAX_POOL]
#else
               [BCM_STAT_FLEX_COUNTER_MAX_POOL]
#endif
               = {{{{NULL}}}};
#endif

#define BCM_STAT_FLEX_COUNTER_LOCK(unit) \
        sal_mutex_take(flex_stat_mutex[unit], sal_mutex_FOREVER);
#define BCM_STAT_FLEX_COUNTER_UNLOCK(unit) \
        sal_mutex_give(flex_stat_mutex[unit]);

static uint8 flex_directions[][8]={
             "Ingress",
             "Egress" };
#ifdef BCM_WARM_BOOT_SUPPORT
static uint8 _flex_group_mode_total_counters_info
              [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION]
              [bcmStatGroupModeCng+1]={
               /* #####################################      */
               /* INGRESS GROUP MODE ==> Total Counters      */
               /* #####################################      */
               {/* bcmStatGroupModeSingle */                 1,
                /* bcmStatGroupModeTrafficType */            3,
                /* bcmStatGroupModeDlfAll */                 2,
                /* bcmStatGroupModeDlfIntPri */              17,
                /* bcmStatGroupModeTyped */                  4,
                /* bcmStatGroupModeTypedAll */               5,
                /* bcmStatGroupModeTypedIntPri */            20,
                /* bcmStatGroupModeSingleWithControl */      2,
                /* bcmStatGroupModeTrafficTypeWithControl */ 4,
                /* bcmStatGroupModeDlfAllWithControl */      3,
                /* bcmStatGroupModeDlfIntPriWithControl */   18,
                /* bcmStatGroupModeTypedWithControl */       5,
                /* bcmStatGroupModeTypedAllWithControl */    6,
                /* bcmStatGroupModeTypedIntPriWithControl */ 21,
                /* bcmStatGroupModeDot1P */                  8,
                /* bcmStatGroupModeIntPri */                 16,
                /* bcmStatGroupModeIntPriCng */              64,
                /* bcmStatGroupModeSvpType */                2,
                /* bcmStatGroupModeDscp */                   64,
                /* bcmStatGroupModeDvpType */                0,
                /* bcmStatGroupModeCng */                    4},
               /* #####################################      */
               /* EGRESS GROUP MODE ==> Total Counters       */
               /* #####################################      */
               {/* bcmStatGroupModeSingle */                 1,
                /* bcmStatGroupModeTrafficType */            2,
                /* bcmStatGroupModeDlfAll */                 0,
                /* bcmStatGroupModeDlfIntPri */              0,
                /* bcmStatGroupModeTyped */                  0,
                /* bcmStatGroupModeTypedAll */               0,
                /* bcmStatGroupModeTypedIntPri */            18,
                /* bcmStatGroupModeSingleWithControl */      0,
                /* bcmStatGroupModeTrafficTypeWithControl */ 0,
                /* bcmStatGroupModeDlfAllWithControl */      0,
                /* bcmStatGroupModeDlfIntPriWithControl */   0,
                /* bcmStatGroupModeTypedWithControl */       0,
                /* bcmStatGroupModeTypedAllWithControl */    0,
                /* bcmStatGroupModeTypedIntPriWithControl */ 0,
                /* bcmStatGroupModeDot1P */                  8,
                /* bcmStatGroupModeIntPri */                 16,
                /* bcmStatGroupModeIntPriCng */              64,
                /* bcmStatGroupModeSvpType */                2,
                /* bcmStatGroupModeDscp */                   64,
                /* bcmStatGroupModeDvpType */                2,
                /* bcmStatGroupModeCng */                    4},
              };
#endif

typedef struct uncmprsd_attr_bits_selector_s {
    uint32 attr_bits;
    uint8  attr_name[20];
}uncmprsd_attr_bits_selector_t;

static uncmprsd_attr_bits_selector_t ing_uncmprsd_attr_bits_selector[]={
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_ATTR_BITS,           "CNG"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_IFP_CNG_ATTR_BITS,       "IFP CNG"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS,       "INT PRI"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_VLAN_FORMAT_ATTR_BITS,   "VLAN"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS,   "OUTER.1P"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INNER_DOT1P_ATTR_BITS,   "INNER.1P"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INGRESS_PORT_ATTR_BITS,  "PORT"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS,           "TOS_DSCP"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_TOS_ECN_ATTR_BITS,           "TOS_ECN"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS,"PktRes"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS,      "SVP"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_DROP_ATTR_BITS,          "DROP"},
       {BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_IP_PKT_ATTR_BITS,        "IP"}
};
static uncmprsd_attr_bits_selector_t egr_uncmprsd_attr_bits_selector[]={
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_ATTR_BITS,           "CNG"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS,       "INT PRI"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_VLAN_FORMAT_ATTR_BITS,   "VLAN"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS,   "OUTER.1P"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INNER_DOT1P_ATTR_BITS,   "INNER.1P"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_EGRESS_PORT_ATTR_BITS,   "PORT"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS,           "TOS_DSCP"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_TOS_ECN_ATTR_BITS,           "TOS_ECN"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS,"PktRes"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS,      "SVP"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_DVP_TYPE_ATTR_BITS,      "DVP"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_DROP_ATTR_BITS,          "DROP"},
       {BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_IP_PKT_ATTR_BITS,        "IP"}
};

#define _DEFINE_GET_INGRESS_VALUE(field_name,field_value) \
static  uint8 _bcm_esw_get_ing_##field_name##_value( \
                bcm_stat_flex_ing_pkt_attr_bits_t *ing_pkt_attr_bits) { \
                   return field_value; \
} \

#define DEFINE_GET_INGRESS_VALUE(field_name) \
        _DEFINE_GET_INGRESS_VALUE(field_name,ing_pkt_attr_bits->field_name)

#ifdef BCM_APACHE_SUPPORT
DEFINE_GET_INGRESS_VALUE(cng_3)
DEFINE_GET_INGRESS_VALUE(phb_3)
DEFINE_GET_INGRESS_VALUE(cng_2)
DEFINE_GET_INGRESS_VALUE(phb_2)
DEFINE_GET_INGRESS_VALUE(cng_1)
DEFINE_GET_INGRESS_VALUE(phb_1)
#endif
DEFINE_GET_INGRESS_VALUE(cng)
DEFINE_GET_INGRESS_VALUE(ifp_cng)
DEFINE_GET_INGRESS_VALUE(int_pri)
DEFINE_GET_INGRESS_VALUE(vlan_format)
DEFINE_GET_INGRESS_VALUE(outer_dot1p)
DEFINE_GET_INGRESS_VALUE(inner_dot1p)
DEFINE_GET_INGRESS_VALUE(ing_port)
DEFINE_GET_INGRESS_VALUE(tos_dscp)
DEFINE_GET_INGRESS_VALUE(tos_ecn)
DEFINE_GET_INGRESS_VALUE(pkt_resolution)
DEFINE_GET_INGRESS_VALUE(svp_type)
DEFINE_GET_INGRESS_VALUE(drop)
DEFINE_GET_INGRESS_VALUE(ip_pkt)

typedef struct _bcm_esw_get_ing_func_f {
    uint8 (*func)(bcm_stat_flex_ing_pkt_attr_bits_t *ing_pkt_attr_bits);
    uint8 func_desc[20];
}_bcm_esw_get_ing_func_t;
static _bcm_esw_get_ing_func_t _bcm_esw_get_ing_func[] = {
#ifdef BCM_APACHE_SUPPORT
    {_bcm_esw_get_ing_cng_3_value,"cng_3"},
    {_bcm_esw_get_ing_phb_3_value,"phb_3"},
    {_bcm_esw_get_ing_cng_2_value,"cng_2"},
    {_bcm_esw_get_ing_phb_2_value,"phb_2"},
    {_bcm_esw_get_ing_cng_1_value,"cng_1"},
    {_bcm_esw_get_ing_phb_1_value,"phb_1"},
#endif
    {_bcm_esw_get_ing_cng_value,"cng"},
    {_bcm_esw_get_ing_ifp_cng_value,"ifp_cng"},
    {_bcm_esw_get_ing_int_pri_value,"int_pri"},
    {_bcm_esw_get_ing_vlan_format_value,"vlan_format"},
    {_bcm_esw_get_ing_outer_dot1p_value,"outer_dot1p"},
    {_bcm_esw_get_ing_inner_dot1p_value,"inner_dot1p"},
    {_bcm_esw_get_ing_ing_port_value,"ing_port"},
    {_bcm_esw_get_ing_tos_dscp_value,"tos_dscp"},
    {_bcm_esw_get_ing_tos_ecn_value,"tos_ecn"},
    {_bcm_esw_get_ing_pkt_resolution_value,"pkt_resolutino"},
    {_bcm_esw_get_ing_svp_type_value,"svp"},
    {_bcm_esw_get_ing_drop_value,"drop"},
    {_bcm_esw_get_ing_ip_pkt_value,"ip_pkt"}
};

#define _DEFINE_GET_EGRESS_VALUE(field_name,field_value) \
static  uint8 _bcm_esw_get_egr_##field_name##_value( \
                bcm_stat_flex_egr_pkt_attr_bits_t *egr_pkt_attr_bits) { \
                   return field_value; \
} \

#define DEFINE_GET_EGRESS_VALUE(field_name) \
        _DEFINE_GET_EGRESS_VALUE(field_name,egr_pkt_attr_bits->field_name)

#ifdef BCM_APACHE_SUPPORT
DEFINE_GET_EGRESS_VALUE(cng_3)
DEFINE_GET_EGRESS_VALUE(phb_3)
DEFINE_GET_EGRESS_VALUE(cng_2)
DEFINE_GET_EGRESS_VALUE(phb_2)
DEFINE_GET_EGRESS_VALUE(cng_1)
DEFINE_GET_EGRESS_VALUE(phb_1)
#endif
DEFINE_GET_EGRESS_VALUE(cng)
DEFINE_GET_EGRESS_VALUE(int_pri)
DEFINE_GET_EGRESS_VALUE(vlan_format)
DEFINE_GET_EGRESS_VALUE(outer_dot1p)
DEFINE_GET_EGRESS_VALUE(inner_dot1p)
DEFINE_GET_EGRESS_VALUE(egr_port)
DEFINE_GET_EGRESS_VALUE(tos_dscp)
DEFINE_GET_EGRESS_VALUE(tos_ecn)
DEFINE_GET_EGRESS_VALUE(pkt_resolution)
DEFINE_GET_EGRESS_VALUE(svp_type)
DEFINE_GET_EGRESS_VALUE(dvp_type)
DEFINE_GET_EGRESS_VALUE(drop)
DEFINE_GET_EGRESS_VALUE(ip_pkt)

typedef struct _bcm_esw_get_egr_func_f {
    uint8 (*func)(bcm_stat_flex_egr_pkt_attr_bits_t *egr_pkt_attr_bits);
    uint8 func_desc[20];
}_bcm_esw_get_egr_func_t;
static _bcm_esw_get_egr_func_t _bcm_esw_get_egr_func[] = {
#ifdef BCM_APACHE_SUPPORT
    {_bcm_esw_get_egr_cng_3_value,"cng_3"},
    {_bcm_esw_get_egr_phb_3_value,"phb_3"},
    {_bcm_esw_get_egr_cng_2_value,"cng_2"},
    {_bcm_esw_get_egr_phb_2_value,"phb_2"},
    {_bcm_esw_get_egr_cng_1_value,"cng_1"},
    {_bcm_esw_get_egr_phb_1_value,"phb_1"},
#endif
    {_bcm_esw_get_egr_cng_value,"cng"},
    {_bcm_esw_get_egr_int_pri_value,"int_pri"},
    {_bcm_esw_get_egr_vlan_format_value,"vlan_format"},
    {_bcm_esw_get_egr_outer_dot1p_value,"outer_dot1p"},
    {_bcm_esw_get_egr_inner_dot1p_value,"inner_dot1p"},
    {_bcm_esw_get_egr_egr_port_value,"egr_port"},
    {_bcm_esw_get_egr_tos_dscp_value,"tos_dscp"},
    {_bcm_esw_get_egr_tos_ecn_value,"tos_ecn"},
    {_bcm_esw_get_egr_pkt_resolution_value,"pkt_resolutino"},
    {_bcm_esw_get_egr_svp_type_value,"svp"},
    {_bcm_esw_get_egr_dvp_type_value,"dvp"},
    {_bcm_esw_get_egr_drop_value,"drop"},
    {_bcm_esw_get_egr_ip_pkt_value,"ip_pkt"}
};

typedef struct flex_object_default_f {
    char obj_desc[20];
    soc_mem_t obj_mem;
    uint32 pool_number;
    uint32 pool_criteria;
}flex_object_default_t;


/* When adding a new object with description contains that of an existing one.
*  Please place the new object before the existing one.
*  For example, "vlanxlatescndlkup" should come before "vlan".
*/
static flex_object_default_t ing_flex_object_default[]={
    {
        "port",
        PORT_TABm,
        FLEX_COUNTER_DEFAULT_PORT_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_PORT_TABLE
    },
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    {
        "vlanxlatescndlkup",
        VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_VLAN_XLATE_SCND_LKUP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP
    },
#endif
    {
        "vlanxlate",
        VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE
    },
    {
        "vlan",
        VLAN_TABm,
        FLEX_COUNTER_DEFAULT_VLAN_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE
    },
    {
        "vfi",
        VFIm,
        FLEX_COUNTER_DEFAULT_VFI_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VFI_TABLE
    },
#if defined(BCM_METROLITE_SUPPORT)
    {
        "l3intf",
        L3_IIFm,
        FLEX_COUNTER_ML_DEFAULT_L3_IIF_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE
    },
    {
        "vrf",
        VRFm,
        FLEX_COUNTER_ML_DEFAULT_VRF_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VRF_TABLE
    },
#else
    {
        "l3intf",
        L3_IIFm,
        FLEX_COUNTER_DEFAULT_L3_IIF_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE
    },
    {
        "vrf",
        VRFm,
        FLEX_COUNTER_DEFAULT_VRF_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VRF_TABLE
    },
#endif
    {
        "policy",
        VFP_POLICY_TABLEm,
        FLEX_COUNTER_DEFAULT_VFP_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE
    },
#if defined(BCM_METROLITE_SUPPORT)
    {
        "mplsvclabel",
        SOURCE_VPm,
        FLEX_COUNTER_ML_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },
#else
    {
        "mplsvclabel",
        SOURCE_VPm,
        FLEX_COUNTER_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },
#endif
    {
        "mplsswitchlabel",
        MPLS_ENTRYm,
        FLEX_COUNTER_DEFAULT_MPLS_ENTRY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE
    },
#if defined(BCM_APACHE_SUPPORT)
    {
        "mplsswitchscndlkup",
        MPLS_ENTRYm,
        FLEX_COUNTER_DEFAULT_MPLS_ENTRY_SCND_LKUP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP
    },
#endif
    {
        "mplsfrrlabel",
        L3_TUNNELm,
        FLEX_COUNTER_DEFAULT_VLAN_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE
    },
#if defined(BCM_METROLITE_SUPPORT)
    {
        "l3host",
        EXT_IPV4_UCAST_WIDEm,
        FLEX_COUNTER_ML_DEFAULT_L3_ENTRY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE
    },
#else
    {
        "l3host",
        EXT_IPV4_UCAST_WIDEm,
        FLEX_COUNTER_DEFAULT_L3_ENTRY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE
    },
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
    {
        "trill",
        MPLS_ENTRY_EXTDm,
        FLEX_COUNTER_DEFAULT_MPLS_ENTRY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE
    },
    {
        "mimlookupid",
        MPLS_ENTRY_EXTDm,
        FLEX_COUNTER_DEFAULT_MPLS_ENTRY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE
    },
#endif
    {
        "l2gre",
        VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE
    },
    {
        "extpolicy",
        VFP_POLICY_TABLEm,
        FLEX_COUNTER_DEFAULT_VFP_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE
    },
#if defined(BCM_METROLITE_SUPPORT)
    {
        "vxlan",
        SOURCE_VPm,
        FLEX_COUNTER_ML_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },
#else
    {
        "vxlan",
        SOURCE_VPm,
        FLEX_COUNTER_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },
#endif
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_METROLITE_SUPPORT) 
#if defined(BCM_METROLITE_SUPPORT)
    {
        "vsan",
        ING_VSANm,
        FLEX_COUNTER_ML_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE
    },
    {
        "fcoe",
        L3_ENTRY_IPV4_MULTICASTm,
        FLEX_COUNTER_ML_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE
    },
    {
        "l3route",
        L3_DEFIPm,
        FLEX_COUNTER_ML_DEFAULT_L3_ROUTE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE
    },
#else 
    {
        "vsan",
        ING_VSANm,
        FLEX_COUNTER_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE
    },
    {
        "fcoe",
        L3_ENTRY_IPV4_MULTICASTm,
        FLEX_COUNTER_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE
    },
    {
        "l3route",
        L3_DEFIPm,
        FLEX_COUNTER_DEFAULT_L3_ROUTE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE
    },
#endif
#endif
#if defined(BCM_METROLITE_SUPPORT)
    {
        "niv",
        SOURCE_VPm,
        FLEX_COUNTER_ML_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },
#else
    {
        "niv",
        SOURCE_VPm,
        FLEX_COUNTER_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE
    },

#endif
#if defined(BCM_METROLITE_SUPPORT) || defined(BCM_TRIDENT2_SUPPORT)
#if  defined(BCM_METROLITE_SUPPORT)
    {
        "ipmc",
        L3_ENTRY_IPV4_MULTICASTm,
        FLEX_COUNTER_ML_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE
    },
#else
    {
        "ipmc",
        L3_ENTRY_IPV4_MULTICASTm,
        FLEX_COUNTER_DEFAULT_FCOE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE
    },
#endif
#endif
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT) || defined(BCM_METROLITE_SUPPORT)
#if  defined(BCM_METROLITE_SUPPORT)
    {
        "icap",
        IFP_POLICY_TABLEm,
        FLEX_COUNTER_ML_DEFAULT_IFP_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE
    },
#else 
    {
        "icap",
        IFP_POLICY_TABLEm,
        FLEX_COUNTER_DEFAULT_IFP_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE
    },
#endif
#endif
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_METROLITE_SUPPORT) 
#if  defined(BCM_METROLITE_SUPPORT)
    {
        "em",
        EXACT_MATCH_2_ENTRY_ONLYm,
        FLEX_COUNTER_ML_DEFAULT_EM_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE
    },
    {
        "agm",
        AGM_MONITOR_TABLEm,
        FLEX_COUNTER_ML_DEFAULT_AGM_MONITOR_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE
    }
#else
    {
        "em",
        EXACT_MATCH_2_ENTRY_ONLYm,
        FLEX_COUNTER_DEFAULT_EM_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE
    },
    {
        "agm",
        AGM_MONITOR_TABLEm,
        FLEX_COUNTER_DEFAULT_AGM_MONITOR_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE
    }
#endif
#endif
};

static flex_object_default_t egr_flex_object_default[]={
    {
        "port",
        EGR_PORTm,
        FLEX_COUNTER_DEFAULT_EGR_PORT_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_PORT_TABLE
    },
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_METROLITE_SUPPORT)
    {
        "vlanxlate",
        EGR_VLAN_XLATEm,
        FLEX_COUNTER_TD2_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE
    },
    {
        "mimlookupid",
        EGR_VLAN_XLATEm,
        FLEX_COUNTER_TD2_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE
    },
    {
        "l2gre",
        EGR_DVP_ATTRIBUTE_1m,
        FLEX_COUNTER_TD2_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE
    },
    {
        "vxlan",
        EGR_DVP_ATTRIBUTE_1m,
        FLEX_COUNTER_TD2_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE
    }, 
    {
        "l3nat",
        EGR_NAT_PACKET_EDIT_INFOm,
        FLEX_COUNTER_TD2_DEFAULT_L3_NAT_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE
    },
#else
#if defined(BCM_APACHE_SUPPORT)
    {
        "vlanxlatescndlkup",
        EGR_VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_EGR_VLAN_XLATE_SCND_LKUP_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE_FOR_SCND_LKUP
    },
#endif
    {
        "vlanxlate",
        EGR_VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE
    },
    {
        "mimlookupid",
        EGR_VLAN_XLATEm,
        FLEX_COUNTER_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE
    },
    {
        "l2gre",
        EGR_DVP_ATTRIBUTE_1m,
        FLEX_COUNTER_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE
    },
    {
        "vxlan",
        EGR_DVP_ATTRIBUTE_1m,
        FLEX_COUNTER_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE
    },
    {
        "l3nat",
        EGR_NAT_PACKET_EDIT_INFOm,
        FLEX_COUNTER_DEFAULT_L3_NAT_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE
    },
#endif
    {
        "vlan",
        EGR_VLANm,
        FLEX_COUNTER_DEFAULT_EGR_VLAN_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_TABLE
    },
    {
        "vfi",
        EGR_VFIm,
        FLEX_COUNTER_DEFAULT_EGR_VFI_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_VFI_TABLE
    },
    {
        "l3intf",
        EGR_L3_NEXT_HOPm,
        FLEX_COUNTER_DEFAULT_EGR_L3_NEXT_HOP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE
    },
    {
        "niv",
        EGR_L3_NEXT_HOPm,
        FLEX_COUNTER_DEFAULT_EGR_L3_NEXT_HOP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE
    },    
    {
        "wlan",
        EGR_L3_NEXT_HOPm,
        FLEX_COUNTER_DEFAULT_EGR_L3_NEXT_HOP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE
    },    
    {
        "mim",
        EGR_L3_NEXT_HOPm,
        FLEX_COUNTER_DEFAULT_EGR_L3_NEXT_HOP_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE
    },
#ifdef BCM_TRIDENT2PLUS_SUPPORT
    {
        "ecap",
        EFP_POLICY_TABLEm,
        FLEX_COUNTER_DEFAULT_EGR_EFP_POLICY_TABLE_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE
    },
    {
        "mplstunnel",
        EGR_IP_TUNNEL_MPLSm,
        FLEX_COUNTER_DEFAULT_EGR_IP_TUNNEL_MPLS_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_TABLE
    },
    {
        "mplstunnelscnd",
        EGR_IP_TUNNEL_MPLSm,
        FLEX_COUNTER_DEFAULT_EGR_IP_TUNNEL_MPLS_SCND_LBL_POOL_NUMBER,
        FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_SCND_LBL_TABLE
    }
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
};

typedef struct flex_object_config_f {
    soc_mem_t obj_mem;
    uint32 config_pool;
    uint32 share_criteria;
    uint32 exclude_criteria;
}flex_object_config_t;

#define INVALID_CONFIG 0

static flex_object_config_t ing_flex_object_config[]={
    {PORT_TABm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFP_POLICY_TABLEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VLAN_TABm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFIm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VRFm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {MPLS_ENTRYm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VLAN_XLATEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {L3_IIFm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {L3_TUNNELm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {L3_ENTRY_2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {L3_DEFIPm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {L3_ENTRY_IPV4_MULTICASTm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {ING_VSANm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXT_FP_POLICYm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {SOURCE_VPm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {FP_POLICY_TABLEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {IFP_POLICY_TABLEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {IFP_POLICY_TABLE_PIPE0m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {IFP_POLICY_TABLE_PIPE1m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {IFP_POLICY_TABLE_PIPE2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {IFP_POLICY_TABLE_PIPE3m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {AGM_MONITOR_TABLEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFP_POLICY_TABLE_PIPE0m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFP_POLICY_TABLE_PIPE1m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFP_POLICY_TABLE_PIPE2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {VFP_POLICY_TABLE_PIPE3m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_2_PIPE0m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_2_PIPE1m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_2_PIPE2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_2_PIPE3m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_4m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_4_PIPE0m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_4_PIPE1m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_4_PIPE2m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EXACT_MATCH_4_PIPE3m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG}
};

static flex_object_config_t egr_flex_object_config[]={
    {EGR_VLANm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_VFIm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_L3_NEXT_HOPm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_VLAN_XLATEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_PORTm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_DVP_ATTRIBUTE_1m, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_NAT_PACKET_EDIT_INFOm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EFP_POLICY_TABLEm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG},
    {EGR_IP_TUNNEL_MPLSm, INVALID_CONFIG, INVALID_CONFIG, INVALID_CONFIG}
};

static uint8 flex_objects[][64]={
             "bcmStatObjectIngPort",
             "bcmStatObjectIngVlan",
             "bcmStatObjectIngVlanXlate",
             "bcmStatObjectIngVfi",
             "bcmStatObjectIngL3Intf",
             "bcmStatObjectIngVrf",
             "bcmStatObjectIngPolicy",
             "bcmStatObjectIngMplsVcLabel",
             "bcmStatObjectIngMplsSwitchLabel",
             "bcmStatObjectEgrPort",
             "bcmStatObjectEgrVlan",
             "bcmStatObjectEgrVlanXlate",
             "bcmStatObjectEgrVfi",
             "bcmStatObjectEgrL3Intf",
             "bcmStatObjectIngMplsFrrLabel",
             "bcmStatObjectIngL3Host",
             "bcmStatObjectIngTrill",
             "bcmStatObjectIngMimLookupId",
             "bcmStatObjectIngL2Gre",
             "bcmStatObjectIngEXTPolicy",
             "bcmStatObjectEgrWlan",
             "bcmStatObjectEgrMim",
             "bcmStatObjectEgrMimLookupId",
             "bcmStatObjectEgrL2Gre",
             "bcmStatObjectIngVxlan",
             "bcmStatObjectIngVsan",
             "bcmStatObjectIngFcoe",
             "bcmStatObjectIngL3Route",
             "bcmStatObjectEgrVxlan",
             "bcmStatObjectEgrL3Nat",
             "bcmStatObjectIngNiv",
             "bcmStatObjectEgrNiv",
             "bcmStatObjectIngIpmc",
             "bcmStatObjectIngVxlanDip",
             "bcmStatObjIngIngressPolicy",
             "bcmStatObjectIngVlanXlateSecondLookup",
             "bcmStatObjectEgrVlanXlateSecondLookup",
             "bcmStatObjectIngMplsSwitchSecondLabel",
             "bcmStatObjectEgrMplsTunnelSecondLabel"};


static uint8 flex_group_modes[][48] = {
             "bcmStatGroupModeSingle = 0",
             "bcmStatGroupModeTrafficType = 1",
             "bcmStatGroupModeDlfAll = 2",
             "bcmStatGroupModeDlfIntPri = 3",
             "bcmStatGroupModeTyped = 4",
             "bcmStatGroupModeTypedAll = 5",
             "bcmStatGroupModeTypedIntPri = 6",
             "bcmStatGroupModeSingleWithControl = 7",
             "bcmStatGroupModeTrafficTypeWithControl = 8",
             "bcmStatGroupModeDlfAllWithControl = 9",
             "bcmStatGroupModeDlfIntPriWithControl = 10",
             "bcmStatGroupModeTypedWithControl = 11",
             "bcmStatGroupModeTypedAllWithControl = 12",
             "bcmStatGroupModeTypedIntPriWithControl = 13",
             "bcmStatGroupModeDot1P = 14",
             "bcmStatGroupModeIntPri = 15",
             "bcmStatGroupModeIntPriCng = 16",
             "bcmStatGroupModeSvpType = 17",
             "bcmStatGroupModeDscp = 18",
             "bcmStatGroupModeDvpType = 19",
             "bcmStatGroupModeCng = 20",
             "bcmStatGroupModeFlex1 = 21",
             "bcmStatGroupModeFlex2 = 22",
             "bcmStatGroupModeFlex3 = 23",
             "bcmStatGroupModeFlex4 = 24"};

/* The static array is mapped one to one with the stat objects */
static uint32 flex_used_by_table[]={
              FLEX_COUNTER_POOL_USED_BY_PORT_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VFI_TABLE,
              FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VRF_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_PORT_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_VFI_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE,
              FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EXT_FP_POLICY_TABLE,
              0,
              FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE,
              FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE,
              FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE,
              FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_TABLE,
              FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE,
              FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE,
              FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP,
              FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE_FOR_SCND_LKUP,
              FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP,
              FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_SCND_LBL_TABLE};

static uint32 *local_scache_ptr[SOC_MAX_NUM_DEVICES]={NULL};
static uint32 local_scache_size[SOC_MAX_NUM_DEVICES] = {0};

#ifdef BCM_WARM_BOOT_SUPPORT
#define BCM_WB_VERSION_1_3                SOC_SCACHE_VERSION(1,3)
#define BCM_WB_VERSION_1_4                SOC_SCACHE_VERSION(1,4)
#define BCM_WB_VERSION_1_5                SOC_SCACHE_VERSION(1,5)
#define BCM_WB_VERSION_1_6                SOC_SCACHE_VERSION(1,6)
#define BCM_STAT_WB_DEFAULT_VERSION            BCM_WB_VERSION_1_6
static soc_scache_handle_t handle[SOC_MAX_NUM_DEVICES] = {0};
static uint32              flex_scache_allocated_size[SOC_MAX_NUM_DEVICES] = {0};
static uint32              *flex_scache_ptr[SOC_MAX_NUM_DEVICES]={NULL};
#endif

typedef struct flex_counter_fields {
               uint32    offset_mode;
               uint32    pool_number;
               uint32    base_idx;
}flex_counter_fields_t;

static flex_counter_fields_t mpls_entry_flex_counter_fields[]={
                             /* MPLS View=0 */
                             {MPLS__FLEX_CTR_OFFSET_MODEf,
                              MPLS__FLEX_CTR_POOL_NUMBERf,
                              MPLS__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* MIM_NVP View=1 */
                             {MIM_NVP__FLEX_CTR_OFFSET_MODEf,
                              MIM_NVP__FLEX_CTR_POOL_NUMBERf,
                              MIM_NVP__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* MIM_ISID   View=2 */
                             {MIM_ISID__FLEX_CTR_OFFSET_MODEf,
                              MIM_ISID__FLEX_CTR_POOL_NUMBERf,
                              MIM_ISID__FLEX_CTR_BASE_COUNTER_IDXf},
#ifdef BCM_TRIUMPH3_SUPPORT
                             {TRILL__FLEX_CTR_OFFSET_MODEf,
                              TRILL__FLEX_CTR_POOL_NUMBERf,
                              TRILL__FLEX_CTR_BASE_COUNTER_IDXf},
                             {IPV4UC__FLEX_CTR_OFFSET_MODEf,
                              IPV4UC__FLEX_CTR_POOL_NUMBERf,
                              IPV4UC__FLEX_CTR_BASE_COUNTER_IDXf},
                             {IPV6UC__FLEX_CTR_OFFSET_MODEf,
                              IPV6UC__FLEX_CTR_POOL_NUMBERf,
                              IPV6UC__FLEX_CTR_BASE_COUNTER_IDXf} };
#else
                             {INVALIDf,
                              INVALIDf,
                              INVALIDf},
                             {INVALIDf,
                              INVALIDf,
                              INVALIDf},
                             {INVALIDf,
                              INVALIDf,
                              INVALIDf} };
#endif


static flex_counter_fields_t egr_l3_next_hop_flex_counter_fields[]={
                             /* L3 View=0 */
                             {FLEX_CTR_OFFSET_MODEf,
                              FLEX_CTR_POOL_NUMBERf,
                              FLEX_CTR_BASE_COUNTER_IDXf},
                             {L3__FLEX_CTR_OFFSET_MODEf,
                              L3__FLEX_CTR_POOL_NUMBERf,
                              L3__FLEX_CTR_BASE_COUNTER_IDXf},
#ifdef BCM_TRIUMPH3_SUPPORT
                             {WLAN__FLEX_CTR_OFFSET_MODEf,
                              WLAN__FLEX_CTR_POOL_NUMBERf,
                              WLAN__FLEX_CTR_BASE_COUNTER_IDXf},
#else
                             {INVALIDf,
                              INVALIDf,
                              INVALIDf},
#endif
                             {PROXY__FLEX_CTR_OFFSET_MODEf,
                              PROXY__FLEX_CTR_POOL_NUMBERf,
                              PROXY__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* MPLS View=1 */
                             {MPLS__FLEX_CTR_OFFSET_MODEf,
                              MPLS__FLEX_CTR_POOL_NUMBERf,
                              MPLS__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* SD_TAG View=2 */
                             {SD_TAG__FLEX_CTR_OFFSET_MODEf,
                              SD_TAG__FLEX_CTR_POOL_NUMBERf,
                              SD_TAG__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* MIM View=3 */
                             {MIM__FLEX_CTR_OFFSET_MODEf,
                              MIM__FLEX_CTR_POOL_NUMBERf,
                              MIM__FLEX_CTR_BASE_COUNTER_IDXf} };

static flex_counter_fields_t egr_vlan_xlate_flex_counter_fields[]={
                             /* Default VLAN_XLATE,VLAN_XLATE_DVP View=0 */
                             {XLATE__FLEX_CTR_OFFSET_MODEf,
                              XLATE__FLEX_CTR_POOL_NUMBERf,
                              XLATE__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* ISID_DVP_XLATE View=4 */
                             {MIM_ISID__FLEX_CTR_OFFSET_MODEf,
                              MIM_ISID__FLEX_CTR_POOL_NUMBERf,
                              MIM_ISID__FLEX_CTR_BASE_COUNTER_IDXf} };

static flex_counter_fields_t vlan_xlate_flex_counter_fields[]={
                             /* XLATE type = 13 */
                             {XLATE__FLEX_CTR_OFFSET_MODEf,
                              XLATE__FLEX_CTR_POOL_NUMBERf,
                              XLATE__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* L2GRE_DIP type = 13 */
                             {L2GRE_DIP__FLEX_CTR_OFFSET_MODEf,
                              L2GRE_DIP__FLEX_CTR_POOL_NUMBERf,
                              L2GRE_DIP__FLEX_CTR_BASE_COUNTER_IDXf},
                              /* VXLAN_DIP type = 18 */
                             {VXLAN_DIP__FLEX_CTR_OFFSET_MODEf,
                              VXLAN_DIP__FLEX_CTR_POOL_NUMBERf,
                              VXLAN_DIP__FLEX_CTR_BASE_COUNTER_IDXf} };

static flex_counter_fields_t egr_dvp_attribute_counter_fields[]={
                             /* TRILL vp type = 1 */
                             {TRILL__FLEX_CTR_OFFSET_MODEf,
                              TRILL__FLEX_CTR_POOL_NUMBERf,
                              TRILL__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* vxlan type = 2 */
                             {VXLAN__FLEX_CTR_OFFSET_MODEf,
                              VXLAN__FLEX_CTR_POOL_NUMBERf,
                              VXLAN__FLEX_CTR_BASE_COUNTER_IDXf},
                              /* L2Gre vp type = 3 */
                             {L2GRE__FLEX_CTR_OFFSET_MODEf,
                              L2GRE__FLEX_CTR_POOL_NUMBERf,
                              L2GRE__FLEX_CTR_BASE_COUNTER_IDXf} };

#ifdef BCM_TRIDENT2_SUPPORT

static flex_counter_fields_t l3_entry_ipv4_multicast_counter_fields[] = {
                             /* IPV4_UNICAST_EXT, type = 1 */
                             {IPV4UC_EXT__FLEX_CTR_OFFSET_MODEf,
                              IPV4UC_EXT__FLEX_CTR_POOL_NUMBERf,
                              IPV4UC_EXT__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* IPV4_MULTICAST, type = 4 */
                             {IPV4MC__FLEX_CTR_OFFSET_MODEf,
                              IPV4MC__FLEX_CTR_POOL_NUMBERf,
                              IPV4MC__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* FCOE_EXT, type = 13, 15, 17 */
                             {FCOE_EXT__FLEX_CTR_OFFSET_MODEf,
                              FCOE_EXT__FLEX_CTR_POOL_NUMBERf,
                              FCOE_EXT__FLEX_CTR_BASE_COUNTER_IDXf} };

static flex_counter_fields_t l3_entry_ipv6_multicast_counter_fields[] = {
                             /* IPV6_UNICAST_EXT, type = 3 */
                             {IPV6UC_EXT__FLEX_CTR_OFFSET_MODEf,
                              IPV6UC_EXT__FLEX_CTR_POOL_NUMBERf,
                              IPV6UC_EXT__FLEX_CTR_BASE_COUNTER_IDXf},
                             /* IPV6_MULTICAST, type = 5 */
                             {IPV6MC__FLEX_CTR_OFFSET_MODEf,
                              IPV6MC__FLEX_CTR_POOL_NUMBERf,
                              IPV6MC__FLEX_CTR_BASE_COUNTER_IDXf} };

#endif

#if defined (BCM_TRIDENT2PLUS_SUPPORT) && defined (INCLUDE_L3)
static flex_counter_fields_t ip_tunnel_mpls_entry_counter_fields[] = {
                             /* Label 1 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_0f,
                              FLEX_CTR_POOL_NUMBER_0f,
                              FLEX_CTR_BASE_COUNTER_IDX_0f},
                             /* Label 2 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_1f,
                              FLEX_CTR_POOL_NUMBER_1f,
                              FLEX_CTR_BASE_COUNTER_IDX_1f},
                             /* Label 3 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_2f,
                              FLEX_CTR_POOL_NUMBER_2f,
                              FLEX_CTR_BASE_COUNTER_IDX_2f},
                             /* Label 4 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_3f,
                              FLEX_CTR_POOL_NUMBER_3f,
                              FLEX_CTR_BASE_COUNTER_IDX_3f},
#ifdef BCM_APACHE_SUPPORT
                             /* Label 5 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_4f,
                              FLEX_CTR_POOL_NUMBER_4f,
                              FLEX_CTR_BASE_COUNTER_IDX_4f},
                             /* Label 6 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_5f,
                              FLEX_CTR_POOL_NUMBER_5f,
                              FLEX_CTR_BASE_COUNTER_IDX_5f},
                             /* Label 7 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_6f,
                              FLEX_CTR_POOL_NUMBER_6f,
                              FLEX_CTR_BASE_COUNTER_IDX_6f},
                             /* Label 8 Flex Counter fields */
                             {FLEX_CTR_OFFSET_MODE_7f,
                              FLEX_CTR_POOL_NUMBER_7f,
                              FLEX_CTR_BASE_COUNTER_IDX_7f}
#endif /* BCM_APACHE_SUPPORT */
                              };
static bcm_stat_object_t egr_mpls_tunnel_object[] = {
                         bcmStatObjectEgrMplsTunnelLabel,
                         bcmStatObjectEgrMplsTunnelSecondLabel};
#endif /* BCM_TRIDENT2PLUS_SUPPORT */


#define BCM_STAT_FLEX_WARMBOOT_READ_CHUNK 256

/* Flex counter collect routine periodically invoked by counter thread */
STATIC void
_bcm_esw_stat_flex_counter_collect(int                       unit,
                                   bcm_stat_flex_direction_t cnt_direction,
                                   uint32                    flexctr_pool,
                                   uint32                    counter_idx);

STATIC
int th_flex_counter_set(int unit, bcm_stat_flex_direction_t direction, int pipe_num,
                      int pool_id, uint32 index, bcm_stat_value_t *value_array,
                      int num_entries,  int byte_flag);

STATIC
int th_flex_centralized_counter_set(int unit, bcm_stat_flex_direction_t direction,
                                    int pipe_num, int pool_id, uint32 index,
                                    bcm_stat_value_t *value_array,
                                    int num_entries, int byte_flag);

/*
 * Function:
 *     _bcm_esw_get_fp_mode_global 
 * Description:
 *      Helper function to return 1 if configured in Global mode
 */
#if defined (BCM_TOMAHAWK_SUPPORT)
int
_bcm_esw_get_fp_mode_global(int unit, bcm_field_qualify_t stage) {
    bcm_field_group_oper_mode_t oper_mode = bcmFieldGroupOperModeGlobal;
    int rv = BCM_E_NONE;

    if (!SOC_IS_TOMAHAWKX(unit)) { 
        return 1;
    }

    if ((stage != bcmFieldQualifyStageIngress) &&
         !soc_feature(unit, soc_feature_field_exact_match_support)) {
        /* Do not call Field API for any stage other than Ingress,
           in low latency modes */
        return 1;
    }

    /* If field module not being initialized, return 1 */
#if defined(BCM_FIELD_SUPPORT)
    if (BCM_FAILURE(_field_is_inited(unit))) {
        return 1;
    }
#endif

    rv = _bcm_field_th_group_oper_mode_get(unit, stage, &oper_mode);
    if (!BCM_FAILURE(rv)) {
       if (oper_mode != bcmFieldGroupOperModeGlobal) {
           return 0;
       }
    }
    return 1; /*default*/
}
#endif

static int 
_bcm_esw_get_max_pool_groups(int unit) {
    
    return ((SOC_IS_TOMAHAWKX(unit)) ?  BCM_STAT_TH_FLEX_COUNTER_MAX_GROUPS: 
                                       BCM_STAT_FLEX_COUNTER_MAX_GROUPS);
}

#if defined (BCM_TOMAHAWK_SUPPORT)
void _bcm_esw_stat_dump_info(int  unit,int pipe,uint32  pool_number, uint32 base_index);
static int
_bcm_esw_is_fp_table(soc_mem_t ingress_table) {
    switch (ingress_table) {
       case IFP_POLICY_TABLEm:
       case VFP_POLICY_TABLEm:
       case IFP_POLICY_TABLE_PIPE0m:
       case VFP_POLICY_TABLE_PIPE0m:
       case EXACT_MATCH_2_PIPE0m:
       case EXACT_MATCH_4_PIPE0m:
       case IFP_POLICY_TABLE_PIPE1m:
       case VFP_POLICY_TABLE_PIPE1m:
       case EXACT_MATCH_2_PIPE1m:
       case EXACT_MATCH_4_PIPE1m:
       case IFP_POLICY_TABLE_PIPE2m:
       case VFP_POLICY_TABLE_PIPE2m:
       case EXACT_MATCH_2_PIPE2m:
       case EXACT_MATCH_4_PIPE2m:
       case IFP_POLICY_TABLE_PIPE3m:
       case VFP_POLICY_TABLE_PIPE3m:
       case EXACT_MATCH_2_PIPE3m:
       case EXACT_MATCH_4_PIPE3m:
           return 1;
       default:
           break;
    }
    return 0;
}

static int
_bcm_esw_get_fp_pipe_num(int unit, soc_mem_t ingress_table)
{
    int pipe_num = 0;

    switch (ingress_table) {
       case IFP_POLICY_TABLE_PIPE0m:
       case VFP_POLICY_TABLE_PIPE0m:
       case EXACT_MATCH_2_PIPE0m:
       case EXACT_MATCH_4_PIPE0m:
            pipe_num = 0;
         break;
       case IFP_POLICY_TABLE_PIPE1m:
       case VFP_POLICY_TABLE_PIPE1m:
       case EXACT_MATCH_2_PIPE1m:
       case EXACT_MATCH_4_PIPE1m:
            pipe_num = 1;
         break;
       case IFP_POLICY_TABLE_PIPE2m:
       case VFP_POLICY_TABLE_PIPE2m:
       case EXACT_MATCH_2_PIPE2m:
       case EXACT_MATCH_4_PIPE2m:
            pipe_num = 2;
         break;
       case IFP_POLICY_TABLE_PIPE3m:
       case VFP_POLICY_TABLE_PIPE3m:
       case EXACT_MATCH_2_PIPE3m:
       case EXACT_MATCH_4_PIPE3m:
            pipe_num = 3;
         break;
       default:
            pipe_num = 0;
            break;
    }
    return pipe_num; 
}

bcm_error_t
_bcm_esw_get_fp_pipe_pool_from_mode(int unit, int offset_mode, bcm_field_qualify_t stage,
                                                               int *pipe, int *pool) {
    uint32 hint_val = 0;
    bcm_error_t rv = BCM_E_CONFIG;
    _field_counter_pool_bmp_t pool_bmp;
    *pipe = 0;
    *pool = 0xff;
    if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        offset_mode -= BCM_CUSTOM_INGRESS_MODE_START;

        if ((flex_custom_ingress_modes[unit][offset_mode].hint.type 
                                  == bcmStatGroupAllocHintIngressFieldGroup) ||
            (flex_custom_ingress_modes[unit][offset_mode].hint.type
                                  == bcmStatGroupAllocHintExactMatchFieldGroup) || 
            (flex_custom_ingress_modes[unit][offset_mode].hint.type 
                                  == bcmStatGroupAllocHintVlanFieldGroup)) {
            if (!_bcm_esw_get_fp_mode_global(unit, stage)) {
                hint_val = flex_custom_ingress_modes[unit][offset_mode].hint.value;
                /* Get Pipe from group */
                rv = _bcm_field_group_flex_ctr_info_get(unit, hint_val, pipe, &pool_bmp);
                *pool = flex_custom_ingress_modes[unit][offset_mode].AllocPool;
            }
        }
    }

    if (*pool == -1) {
       *pool = 0xff;
    }

    return rv;
}

bcm_error_t
_bcm_esw_get_fp_pipe_from_mode(int unit, int offset_mode, 
                                bcm_field_qualify_t stage, int *pipe) {
    bcm_error_t rv = BCM_E_CONFIG;
    int pool = 0;
    *pipe = 0;
    rv = _bcm_esw_get_fp_pipe_pool_from_mode(unit, offset_mode, stage, pipe, &pool);
    return rv;
}

/*
 * Return 1 if the ingress_table is one of the
 * EXACT match tables
 */
static int 
_bcm_esw_is_exact_match(soc_mem_t ingress_table) {
    switch(ingress_table) {
        case EXACT_MATCH_2m:
        case EXACT_MATCH_2_PIPE0m:
        case EXACT_MATCH_2_PIPE1m:
        case EXACT_MATCH_2_PIPE2m:
        case EXACT_MATCH_2_PIPE3m:
        case EXACT_MATCH_4m:
        case EXACT_MATCH_4_PIPE0m:
        case EXACT_MATCH_4_PIPE1m:
        case EXACT_MATCH_4_PIPE2m:
        case EXACT_MATCH_4_PIPE3m:
             return 1;
        default:
             return 0;
    }
    return 0;
}
#endif

/*
 * Function:
 *      _bcm_esw_get_flex_counter_fields
 * Description:
 *      Gets flex counter fields(FLEX_CTR_OFFSET_MODEf,FLEX_CTR_POOL_NUMBERf
 *      and FLEX_CTR_BASE_COUNTER_IDXf) name for given table.
 *      This is important when a table has several views for flex counter
 *      field and EntryType or KeyType fieled is checked.
 * Parameters:
 *      unit                - (IN) unit number
 *      index               - (IN) Index of the Table
 *      object              - (IN) Accounting Object
 *      table               - (IN) Accounting Table
 *      data                - (IN) Accounting Table Data FLEX_CTR_OFFSET_MODE
 *      offset_mode_field   - (OUT) Field for FLEX_CTR_OFFSET_MODE
 *      pool_number_field   - (OUT) Field for FLEX_CTR_POOL_NUMBER
 *      base_idx_field      - (OUT) Field for FLEX_CTR_BASE_COUNTER_IDX
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */

static bcm_error_t
_bcm_esw_get_flex_counter_fields(
    int         unit,
    uint32     index,
    bcm_stat_object_t object,
    soc_mem_t   table,
    void        *data,
    soc_field_t *offset_mode_field,
    soc_field_t *pool_number_field,
    soc_field_t *base_idx_field)
{
    uint32 key_type=0;
    uint32 key_type_index=0;
    soc_field_t view_field;
    soc_mem_info_t *memp;

    int rv = BCM_E_NONE;
#if defined (BCM_TRIDENT2PLUS_SUPPORT) && defined (INCLUDE_L3)
    int label_count=0;
    int object_index=0;
#endif
    *offset_mode_field=FLEX_CTR_OFFSET_MODEf;
    *pool_number_field=FLEX_CTR_POOL_NUMBERf;
    *base_idx_field=FLEX_CTR_BASE_COUNTER_IDXf;
    switch(table) {
    case MPLS_ENTRYm:
    case MPLS_ENTRY_EXTDm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "MPLS_ENTRYm   ")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,KEY_TYPEf)) {
             view_field = KEY_TYPEf;
         } else if (soc_mem_field_valid(unit,table,KEY_TYPE_0f)) {
             view_field = KEY_TYPE_0f;
         } else if (soc_mem_field_valid(unit,table,KEY_TYPE_1f)) {
             view_field = KEY_TYPE_1f;
         } else if (soc_mem_field_valid(unit,table,ENTRY_TYPEf)) {
             view_field = ENTRY_TYPEf;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));
         if (sal_strcmp(memp->views[key_type],"MPLS") == 0) {
             key_type_index = 0;
         } else if (sal_strcmp(memp->views[key_type],"MIM_NVP") == 0) {
             key_type_index = 1;
         } else if (sal_strcmp(memp->views[key_type],"MIM_ISID") == 0) {
             key_type_index = 2;
         } else if (sal_strcmp(memp->views[key_type],"TRILL") == 0) {
             key_type_index = 3;
         } else if (sal_strcmp(memp->views[key_type],"IPV4UC") == 0) {
             key_type_index = 4;
         } else if (sal_strcmp(memp->views[key_type],"IPV6UC") == 0) {
             key_type_index = 5;
         } else {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "KEY TYPE NOT OK %d"),
                        key_type));
             return BCM_E_CONFIG;
         }
         *offset_mode_field= mpls_entry_flex_counter_fields[key_type_index].
                             offset_mode;
         *pool_number_field= mpls_entry_flex_counter_fields[key_type_index].
                             pool_number;
         *base_idx_field=    mpls_entry_flex_counter_fields[key_type_index].
                             base_idx;
         break;
    case L3_TUNNELm:
         if (soc_feature(unit, soc_feature_mpls_frr_lookup)) {
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "L3_TUNNELm   ")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,MODEf)) {
             view_field = MODEf;
         } else if (soc_mem_field_valid(unit, table, KEY_TYPEf)) {
             view_field = KEY_TYPEf;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));
         if ((sal_strcmp(memp->views[key_type+1],"MPLS") == 0) ||
                 (sal_strcmp(memp->views[key_type],"MPLS") == 0)) {
             key_type_index = 0;
         }  else {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "KEY TYPE NOT OK %d"),
                        key_type));
             return BCM_E_CONFIG;
         }
         *offset_mode_field= mpls_entry_flex_counter_fields[key_type_index].
                             offset_mode;
         *pool_number_field= mpls_entry_flex_counter_fields[key_type_index].
                             pool_number;
         *base_idx_field=    mpls_entry_flex_counter_fields[key_type_index].
                             base_idx;
         }
         break;
    case EGR_L3_NEXT_HOPm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "EGR_L3_NEXT_HOP   =>")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,ENTRY_TYPEf)) {
             view_field = ENTRY_TYPEf;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);
         if (sal_strcmp(memp->views[key_type],"LEGACY") == 0) {
             key_type_index = 0;
         } else if (sal_strcmp(memp->views[key_type],"L3") == 0) {
             key_type_index = 1;
         } else if (sal_strcmp(memp->views[key_type],"WLAN") == 0) {
             key_type_index = 2;
         } else if (sal_strcmp(memp->views[key_type],"PROXY") == 0) {
             key_type_index = 3;
         } else if (sal_strcmp(memp->views[key_type],"MPLS") == 0) {
             key_type_index = 4;
         } else if (sal_strcmp(memp->views[key_type],"SD_TAG") == 0) {
             key_type_index = 5;
         } else if (sal_strcmp(memp->views[key_type],"MIM") == 0) {
             key_type_index = 6;
         } else {
             return BCM_E_CONFIG;
         }
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));
         *offset_mode_field=egr_l3_next_hop_flex_counter_fields[key_type_index].
                             offset_mode;
         *pool_number_field=egr_l3_next_hop_flex_counter_fields[key_type_index].
                             pool_number;
         *base_idx_field   =egr_l3_next_hop_flex_counter_fields[key_type_index].
                             base_idx;
         break;
    case EGR_VLAN_XLATEm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "EGR_VLAN_XLATEm ==")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,ENTRY_TYPEf)) {
             view_field = ENTRY_TYPEf;
         } else if (soc_mem_field_valid(unit,table,KEY_TYPEf)) {
             view_field = KEY_TYPEf;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));
         if (sal_strcmp(memp->views[key_type],"XLATE") == 0) {
             key_type_index = 0;
         } else if (sal_strcmp(memp->views[key_type],"MIM_ISID") == 0) {
             key_type_index = 1;
         } else {
             return BCM_E_CONFIG;
         }
         *offset_mode_field= egr_vlan_xlate_flex_counter_fields[key_type_index].
                             offset_mode;
         *pool_number_field= egr_vlan_xlate_flex_counter_fields[key_type_index].
                             pool_number;
         *base_idx_field   = egr_vlan_xlate_flex_counter_fields[key_type_index].
                             base_idx;
         break;
    case VLAN_XLATEm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "VLAN_XLATEm ==")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,ENTRY_TYPEf)) {
             view_field = ENTRY_TYPEf;
         } else if (soc_mem_field_valid(unit,table,KEY_TYPEf)) {
             view_field = KEY_TYPEf;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);

#if defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit)) {
             /* In TR3, VLAN_XLATE (vlan_xlate_1) has even entry types,
              *  which supports flex counters
              */
             if (key_type & 0x1) {
                return BCM_E_CONFIG;
             }
        }
#endif

         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));
         if (sal_strcmp(memp->views[key_type],"XLATE") == 0) {
             key_type_index = 0;
         } else if (sal_strcmp(memp->views[key_type],"L2GRE_DIP") == 0) {
             key_type_index = 1;
         } else if (sal_strcmp(memp->views[key_type],"VXLAN_DIP") == 0) {
             key_type_index = 2;
         } else {
             return BCM_E_CONFIG;
         }
         *offset_mode_field= vlan_xlate_flex_counter_fields[key_type_index].
                             offset_mode;
         *pool_number_field= vlan_xlate_flex_counter_fields[key_type_index].
                             pool_number;
         *base_idx_field   = vlan_xlate_flex_counter_fields[key_type_index].
                             base_idx;
         break;
    case VLAN_XLATE_EXTDm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "VLAN_XLATE_EXTDm   ")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,KEY_TYPE_0f)) {
             view_field = KEY_TYPE_0f;
         } else if (soc_mem_field_valid(unit,table,KEY_TYPE_1f)) {
             view_field = KEY_TYPE_1f;
         } else {
             return BCM_E_CONFIG;
         }
         key_type=soc_mem_field32_get(unit,table,data,view_field);
         /* VLAN_XLATE_EXTD has only odd entry types */
         if (!(key_type & 0x1)) {
            return BCM_E_CONFIG;
         }
         break;
    case EGR_DVP_ATTRIBUTE_1m:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "EGR_DVP_ATTRIBUTE_1m   ")));
         memp = &SOC_MEM_INFO(unit, table);
         {
            egr_dvp_attribute_entry_t  egr_dvp_attribute;
            int vp_type;

            sal_memset(&egr_dvp_attribute, 0, sizeof(egr_dvp_attribute_entry_t));
            rv = soc_mem_read(unit, EGR_DVP_ATTRIBUTEm, MEM_BLOCK_ANY,
                              index, &egr_dvp_attribute);
            vp_type = soc_mem_field32_get(unit, EGR_DVP_ATTRIBUTEm,
                                         &egr_dvp_attribute, VP_TYPEf);
            key_type_index = 0;
            if (vp_type == 2) { /* vxlan */
                key_type_index = 1;
            } else if (vp_type == 3) { /* l2gre */
                key_type_index = 2;
            }
        }
         if (key_type_index) {
             *offset_mode_field= egr_dvp_attribute_counter_fields[key_type_index].
                                 offset_mode;
             *pool_number_field= egr_dvp_attribute_counter_fields[key_type_index].
                                 pool_number;
             *base_idx_field   = egr_dvp_attribute_counter_fields[key_type_index].
                                 base_idx;
        }
        break;
#if defined(BCM_KATANA2_SUPPORT)
    case EGR_VLANm:
        if (SOC_IS_KATANA2(unit)) {
            *offset_mode_field=EGR_FLEX_CTR_OFFSET_MODEf;
            *pool_number_field=EGR_FLEX_CTR_POOL_NUMBERf;
            *base_idx_field=EGR_FLEX_CTR_BASE_COUNTER_IDXf;
        }
        break;
#endif
#ifdef BCM_TRIDENT2_SUPPORT

    case L3_ENTRY_IPV4_MULTICASTm:

         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "L3_ENTRY_IPV4_MULTICAST  ")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,KEY_TYPE_0f)) {
             view_field = KEY_TYPE_0f;
         } else {
             return BCM_E_CONFIG;
         }

         key_type = soc_mem_field32_get(unit, table, data, view_field);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));

         if (key_type == TD2_L3_HASH_KEY_TYPE_V4UC_EXT) {
             key_type_index = 0;
         } else if ((key_type == TD2_L3_HASH_KEY_TYPE_V4MC) ||
                    (key_type == TD2_L3_HASH_KEY_TYPE_V4L2MC) ||
                    (key_type == TD2_L3_HASH_KEY_TYPE_V4L2VPMC)){
             key_type_index = 1;
         } else if ((key_type == TD2_L3_HASH_KEY_TYPE_FCOE_DOMAIN_EXT) ||
                    (key_type == TD2_L3_HASH_KEY_TYPE_FCOE_HOST_EXT) ||
                    (key_type == TD2_L3_HASH_KEY_TYPE_FCOE_SRC_MAP_EXT)) {
             key_type_index = 2;
         } else {
             return BCM_E_CONFIG;
         }
         *offset_mode_field =
             l3_entry_ipv4_multicast_counter_fields[key_type_index].offset_mode;
         *pool_number_field =
             l3_entry_ipv4_multicast_counter_fields[key_type_index].pool_number;
         *base_idx_field   =
             l3_entry_ipv4_multicast_counter_fields[key_type_index].base_idx;
         break;

    case L3_ENTRY_IPV6_MULTICASTm:

         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "L3_ENTRY_IPV6_MULTICAST  ")));
         memp = &SOC_MEM_INFO(unit, table);
         if (soc_mem_field_valid(unit,table,KEY_TYPE_0f)) {
             view_field = KEY_TYPE_0f;
         } else {
             return BCM_E_CONFIG;
         }

         key_type = soc_mem_field32_get(unit, table, data, view_field);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "key_type %d "),
                    key_type));

         if (key_type == TD2_L3_HASH_KEY_TYPE_V6UC_EXT) {
             key_type_index = 0;
         } else if ((key_type == TD2_L3_HASH_KEY_TYPE_V6MC) || 
                    (key_type == TD2_L3_HASH_KEY_TYPE_V6L2MC) ||
                    (key_type == TD2_L3_HASH_KEY_TYPE_V6L2VPMC)){
             key_type_index = 1;
         } else {
             return BCM_E_CONFIG;
         }
         *offset_mode_field =
             l3_entry_ipv6_multicast_counter_fields[key_type_index].offset_mode;
         *pool_number_field =
             l3_entry_ipv6_multicast_counter_fields[key_type_index].pool_number;
         *base_idx_field   =
             l3_entry_ipv6_multicast_counter_fields[key_type_index].base_idx;
         break;

    case EGR_NAT_PACKET_EDIT_INFOm:
         if (index & 1) {
             *offset_mode_field = FLEX_CTR_OFFSET_MODE_1f;
             *pool_number_field = FLEX_CTR_POOL_NUMBER_1f;
             *base_idx_field = FLEX_CTR_BASE_COUNTER_IDX_1f;
         } else {
             *offset_mode_field = FLEX_CTR_OFFSET_MODE_0f;
             *pool_number_field = FLEX_CTR_POOL_NUMBER_0f;
             *base_idx_field = FLEX_CTR_BASE_COUNTER_IDX_0f;
         }
         break;

    case L3_DEFIPm:
         if (index & 0x1) {
             *offset_mode_field = FLEX_CTR_OFFSET_MODE1f;
             *pool_number_field = FLEX_CTR_POOL_NUMBER1f;
             *base_idx_field = FLEX_CTR_BASE_COUNTER_IDX1f;
         } else {
             *offset_mode_field = FLEX_CTR_OFFSET_MODE0f;
             *pool_number_field = FLEX_CTR_POOL_NUMBER0f;
             *base_idx_field = FLEX_CTR_BASE_COUNTER_IDX0f;
         }
         break;
#endif
#if defined (BCM_TRIDENT2PLUS_SUPPORT) && defined (INCLUDE_L3)
    case EGR_IP_TUNNEL_MPLSm:
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "EGR_IP_TUNNEL_MPLS")));

         rv = _bcm_tr_mpls_tunnel_chain_label_count_get(unit, index,
                 &label_count);
         if (rv < 0) {
             return rv;
         }
         for (object_index=0;object_index < ((sizeof(egr_mpls_tunnel_object)/
                         sizeof(egr_mpls_tunnel_object[0])));object_index++) {
             if (object == egr_mpls_tunnel_object[object_index]) {
                 break;
             }
         }
         if ((object_index >= 1) &&
                 !(soc_feature(unit, soc_feature_multi_tunnel_label_count))) {
             return BCM_E_PARAM;
         }
         if (object_index < label_count) {
             key_type_index = (index % _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit)) +
                               label_count-(object_index+1);
         }
         *offset_mode_field = 
             ip_tunnel_mpls_entry_counter_fields
             [key_type_index].offset_mode;
         *pool_number_field = 
             ip_tunnel_mpls_entry_counter_fields
             [key_type_index].pool_number;
         *base_idx_field = 
             ip_tunnel_mpls_entry_counter_fields
             [key_type_index].base_idx;
         break;
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
    default:
         break;
    }
    /* Just a safety check */
    if ((soc_mem_field_valid(unit,table,*offset_mode_field) == FALSE) ||
        (soc_mem_field_valid(unit,table,*pool_number_field) == FALSE) ||
        (soc_mem_field_valid(unit,table,*base_idx_field) == FALSE)) {
        LOG_WARN(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "INTERNAL Error i.e. "
                              "required offset,pool,base_idx fields are not valid \n")));
        return BCM_E_INTERNAL;
    }
    return rv;
}
/*
 * Function:
 *      _bcm_esw_get_flex_counter_fields_values
 * Description:
 *      Gets flex counter fields(FLEX_CTR_OFFSET_MODEf,FLEX_CTR_POOL_NUMBERf
 *      and FLEX_CTR_BASE_COUNTER_IDXf) values for given table.
 *      This is important when a table has several views for flex counter
 *      field and EntryType or KeyType fieled is checked.
 * Parameters:
 *      unit                - (IN) unit number
 *      table               - (IN) Accounting Table
 *      data                - (IN) Accounting Table Data FLEX_CTR_OFFSET_MODE
 *      object              - (IN) Accounting Object
 *      offset_mode         - (OUT) FLEX_CTR_OFFSET_MODE field value
 *      pool_number         - (OUT) FLEX_CTR_POOL_NUMBER field value
 *      base_idx            - (OUT) FLEX_CTR_BASE_COUNTER_IDX field value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t
_bcm_esw_get_flex_counter_fields_values(
    int       unit,
    uint32    index,
    soc_mem_t table,
    void      *data,
    bcm_stat_object_t object,
    uint32    *offset_mode,
    uint32    *pool_number,
    uint32    *base_idx)
{
    soc_field_t offset_mode_field=FLEX_CTR_OFFSET_MODEf;
    soc_field_t pool_number_field=FLEX_CTR_POOL_NUMBERf;
    soc_field_t base_idx_field=FLEX_CTR_BASE_COUNTER_IDXf;
    BCM_IF_ERROR_RETURN(_bcm_esw_get_flex_counter_fields(
                        unit,
                        index,
                        object,
                        table,
                        data,
                        &offset_mode_field,
                        &pool_number_field,
                        &base_idx_field));
    *offset_mode =  soc_mem_field32_get(unit,table,data,offset_mode_field);
    *pool_number =  soc_mem_field32_get(unit,table,data,pool_number_field);
    *base_idx =  soc_mem_field32_get(unit,table,data,base_idx_field);
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_set_flex_counter_fields_values
 * Description:
 *      Sets flex counter fields(FLEX_CTR_OFFSET_MODEf,FLEX_CTR_POOL_NUMBERf
 *      and FLEX_CTR_BASE_COUNTER_IDXf) values for given table.
 *      This is important when a table has several views for flex counter
 *      field and EntryType or KeyType fieled is checked.
 * Parameters:
 *      unit                - (IN) unit number
 *      table               - (IN) Accounting Table
 *      data                - (IN) Accounting Table Data FLEX_CTR_OFFSET_MODE
 *      offset_mode         - (IN) FLEX_CTR_OFFSET_MODE field value
 *      pool_number         - (IN) FLEX_CTR_POOL_NUMBER field value
 *      base_idx            - (IN) FLEX_CTR_BASE_COUNTER_IDX field value
 *
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t
_bcm_esw_set_flex_counter_fields_values(
    int       unit,
    uint32    index,
    soc_mem_t table,
    void      *data,
    bcm_stat_object_t object,
    uint32    offset_mode,
    uint32    pool_number,
    uint32    base_idx)
{
    soc_field_t offset_mode_field=FLEX_CTR_OFFSET_MODEf;
    soc_field_t pool_number_field=FLEX_CTR_POOL_NUMBERf;
    soc_field_t base_idx_field=FLEX_CTR_BASE_COUNTER_IDXf;
    BCM_IF_ERROR_RETURN(_bcm_esw_get_flex_counter_fields(
                        unit,
                        index,
                        object,
                        table,
                        data,
                        &offset_mode_field,
                        &pool_number_field,
                        &base_idx_field));
    soc_mem_field32_set(unit,table,data,offset_mode_field,offset_mode);
    soc_mem_field32_set(unit,table,data,pool_number_field,pool_number);
    soc_mem_field32_set(unit,table,data,base_idx_field,base_idx);
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_insert_stat_id
 * Description:
 *      Inserts stat id in local scache table. Useful for WARM-BOOT purpose
 * Parameters:
 *      scache_ptr            - (IN) Local scache table pointer
 *      stat_counter_id       - (IN) Flex Stat Counter Id
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t
_bcm_esw_stat_flex_insert_stat_id(int unit, uint32 *scache_ptr,uint32 stat_counter_id)
{
    uint32 index=0;
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META("Inserting %d "),
               stat_counter_id));
    if (SOC_IS_TOMAHAWKX(unit)) {
        return BCM_E_NONE;
    }

    for (index=0;index<BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE;index++) {
         if (scache_ptr[index] == 0) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META("Inserted \n")));
             scache_ptr[index] = stat_counter_id;
             break;
         }
    }
    if (index == BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE) {
        return BCM_E_FAIL;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_delete_stat_id
 * Description:
 *      Deletes stat id from local scache table. Useful for WARM-BOOT purpose
 * Parameters:
 *      scache_ptr            - (IN) Local scache table pointer
 *      stat_counter_id       - (IN) Flex Stat Counter Id
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t
_bcm_esw_stat_flex_delete_stat_id(int unit,uint32 *scache_ptr,uint32 stat_counter_id)
{
    uint32 index=0;
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META("Deleting ID:%d "),
               stat_counter_id));

    if (SOC_IS_TOMAHAWKX(unit)) {
        return BCM_E_NONE;
    }
    for (index=0;index<BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE;index++) {
         if (scache_ptr[index] == stat_counter_id) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META("Deleted \n")));
             scache_ptr[index] = 0;
             break;
         }
    }
    if (index == BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE) {
        return BCM_E_FAIL;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_ingress_group_oam_status
 * Description:
 *      Get the current OAM capability of ingress flex counter pool group.
 * Parameters:
 *      unit                  - (IN) unit number
 *      group                 - (IN) pool group index
 *      enable                - (OUT) status of oam group
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t _bcm_esw_stat_flex_ingress_group_oam_status(
            int                       unit,
            uint32                    group,
            uint32                    *enable)
{
    uint32  flex_ctr_group_lm_config_value=0;
    uint32  enable_value;

    if (group >=
             soc_reg_field_length(unit, ING_FLEX_CTR_LM_CONFIGr, LM_ENABLEf)) {
        return BCM_E_PARAM;
    }

    if ( enable == NULL) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "...Enabling Flex Counter Group:%d for OAM \n"),
                              group));
        return BCM_E_PARAM;
    }
       /* First Get complete value of
       ING_FLEX_CTR_LM_CONFIGr value */
    SOC_IF_ERROR_RETURN(soc_reg32_get(
                        unit,
                        ING_FLEX_CTR_LM_CONFIGr,
                        REG_PORT_ANY, 0,
                        &flex_ctr_group_lm_config_value));
     enable_value = soc_reg_field_get(unit,
                                      ING_FLEX_CTR_LM_CONFIGr,
                                      flex_ctr_group_lm_config_value,
                                      LM_ENABLEf);
     *enable = enable_value & (1 << group);
    return BCM_E_NONE;
}

#ifdef BCM_WARM_BOOT_SUPPORT
/*
 * Function:
 *      _bcm_esw_stat_group_mode_attr_recover
 * Description:
 *      Recover stat group mode attr bitmap from H/W.
 * Parameters:
 *      unit                - (IN) unit number
 *      mode              - (IN) Stat flex counter mode
 *      ingress            -(IN) ingress-1, egress-0;
 *      num_selectors  -(IN) number of selectors;
 *      attr_level2       - (IN) attr selector from Level2 WB
 *      attr                 - (OUT) attr selector from Level2 and Level 1 WB
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static
bcm_error_t _bcm_esw_stat_group_mode_attr_recover(
             int unit,
             uint32 mode,
             int ingress,
             uint32 num_selectors,
             bcm_stat_group_mode_attr_selector_wb_t* attr_level2,
             bcm_stat_group_mode_attr_selector_t* attr)
{
    uint64 attr_sel_value;
    int i;
    uint32 bitmap = 0;
    /*currently only ingress support udf*/
    soc_reg_t _ingress_pkt_selector_key_reg[] = {
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_0r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_1r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_2r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_3r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_4r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_5r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_6r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_7r,
#if defined(BCM_TOMAHAWK_SUPPORT) || defined (BCM_APACHE_SUPPORT)
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_8r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_9r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_10r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_11r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_12r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_13r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_14r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_15r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_16r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_17r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_18r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_19r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_20r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_21r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_22r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_23r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_24r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_25r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_26r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_27r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_28r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_29r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_30r,
                     ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_31r
#endif
                     };

    if (ingress && SOC_REG_FIELD_VALID(unit,
                   _ingress_pkt_selector_key_reg[mode], USE_UDF_KEYf)) {
        BCM_IF_ERROR_RETURN(soc_reg_get(unit,
            _ingress_pkt_selector_key_reg[mode],REG_PORT_ANY, 0, &attr_sel_value));
        if (soc_reg64_field32_get(unit, _ingress_pkt_selector_key_reg[mode],
                attr_sel_value, USE_UDF_KEYf)) {
            for (i = 0; i < 8; i++) {
                if (soc_reg64_field32_get(unit, _ingress_pkt_selector_key_reg[mode],
                        attr_sel_value, _pkt_selector_x_en_field_name[i])) {
                    bitmap |= (1 << (soc_reg64_field32_get(
                                      unit,
                                      _ingress_pkt_selector_key_reg[mode],
                                      attr_sel_value,
                                      _pkt_selector_for_bit_x_field_name[i])-1));
                }
            }
        }
    }

    for (i=0; i < num_selectors; i++) {
        attr[i].counter_offset = attr_level2[i].counter_offset;
        attr[i].attr = attr_level2[i].attr;
        attr[i].attr_value = attr_level2[i].attr_value;
        if (attr[i].attr == bcmStatGroupModeAttrUdf) {
            attr[i].udf_bitmap = bitmap;
        } else {
            attr[i].udf_bitmap = 0;
        }
     }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_install_stat_id
 * Description:
 *      Install(ie. configures h/w) as per retrieved stat-id.
 *      Useful for WARM-BOOT purpose
 * Parameters:
 *      unit                  - (IN) unit number
 *      scache_ptr            - (IN) Local scache table pointer
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static
bcm_error_t _bcm_esw_stat_flex_install_stat_id(int unit,uint32  *scache_ptr, uint32 *per_pipe_mode_ifp, uint32 *per_pipe_mode_vfp)
{
    uint32                    index=0;
    uint32                    stat_counter_id=0;
    uint32                    pool_number=0;
    uint32                    base_index=0;
    bcm_stat_flex_mode_t      offset_mode=0;
    bcm_stat_object_t         object=bcmStatObjectIngPort;
    bcm_stat_group_mode_t     group_mode= bcmStatGroupModeSingle;
    bcm_stat_flex_direction_t direction=bcmStatFlexDirectionIngress;
    uint32                    total_counters=0;
    uint32 enable = 0, allocate_group_type = 0, pool_group_id = 0;
    bcm_stat_flex_group_mode_related_info_t *flex_group_mode_related_info =
                                 (bcm_stat_flex_group_mode_related_info_t *)
                                 ( scache_ptr +
                                   BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE);
    uint32 mode=0;
    uint32 flex_group_mode = 0;
    bcm_stat_group_mode_attr_selector_t *attr_selectors_tmp = NULL;
#if defined(BCM_TOMAHAWK_SUPPORT)
    uint32 stat_ctr_id;
    bcm_stat_flex_custom_mode_related_info_t *flex_custom_mode_related_info =
                                 NULL;
    bcm_stat_flex_counter_id_related_info_t *flex_stat_ctr_related_info =
                                 NULL;
    bcm_stat_flex_custom_ingress_mode_t flex_custom_ingress_mode;
#endif
    int pipe_num = 0;
    bcm_error_t rv = BCM_E_NONE;
    *per_pipe_mode_ifp = 0;
    *per_pipe_mode_vfp = 0;

    if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_3)) {
        bcm_stat_flex_attr_t attr;
        uint32 ncounters = 0;
        attr_selectors_tmp = sal_alloc(
            sizeof(bcm_stat_group_mode_attr_selector_t) * BCM_STAT_FLEX_MAX_SELECTORS,
            "group mode attr warmboot tmp memory");
        if (NULL == attr_selectors_tmp) {
            return BCM_E_MEMORY;
        }
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "NEW Version\n")));
        for (mode=0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE ; mode++) {
            rv = _bcm_esw_stat_group_mode_attr_recover(unit, mode, 1,
                            flex_group_mode_related_info->num_selectors,
                            flex_group_mode_related_info->attr_selectors,
                            attr_selectors_tmp);
            if (BCM_FAILURE(rv)) {
                sal_free(attr_selectors_tmp);
                return rv;
            }
            if (flex_group_mode_related_info->flags != 0) {
                (void)_bcm_esw_stat_flex_update_ingress_flex_info(unit, mode,
                    flex_group_mode_related_info->flags & 0xffff,
                    flex_group_mode_related_info->num_selectors,
                    attr_selectors_tmp);
                flex_group_mode = bcmStatGroupModeFlex1 + mode;
                flex_ingress_modes[unit][mode].total_counters =
                                   flex_group_mode_related_info->total_counters;
                if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_6)) {
                    flex_group_mode = (flex_group_mode_related_info->flags >> 16);
                    if (flex_group_mode < bcmStatGroupModeFlex1) {
                
                        rv = _bcm_esw_stat_attr_fill(unit, flex_group_mode,
                                                bcmStatFlexDirectionIngress, 
                                                &attr, &ncounters);
                        if (!BCM_FAILURE(rv)) {
                             flex_ingress_modes[unit][mode].ing_attr = attr.ing_attr;
                        }
                    }
                }
                _bcm_esw_stat_flex_set_group_mode(
                                 unit,bcmStatFlexDirectionIngress,
                                 mode, flex_group_mode);
            }
            flex_group_mode_related_info++;
       }
       for (mode=0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE ; mode++) {
             rv = _bcm_esw_stat_group_mode_attr_recover(unit, mode, 0,
                     flex_group_mode_related_info->num_selectors,
                     flex_group_mode_related_info->attr_selectors,
                     attr_selectors_tmp);
            if (BCM_FAILURE(rv)) {
                sal_free(attr_selectors_tmp);
                return rv;
            }
            if (flex_group_mode_related_info->flags != 0) {
                _bcm_esw_stat_flex_update_egress_flex_info(unit, mode,
                    flex_group_mode_related_info->flags & 0xffff,
                    flex_group_mode_related_info->num_selectors,
                    attr_selectors_tmp);
                flex_group_mode = bcmStatGroupModeFlex1 + mode;
                flex_egress_modes[unit][mode].total_counters =
                                   flex_group_mode_related_info->total_counters;
                if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_6)) {
                    flex_group_mode = (flex_group_mode_related_info->flags >> 16);
                    if (flex_group_mode < bcmStatGroupModeFlex1) {
                        attr.egr_attr = flex_egress_modes[unit][mode].egr_attr;
                        rv = _bcm_esw_stat_attr_fill(unit, flex_group_mode,
                                                bcmStatFlexDirectionEgress, 
                                                &attr, &ncounters);
                        if (!BCM_FAILURE(rv)) {
                            flex_egress_modes[unit][mode].egr_attr = attr.egr_attr;
                        }
                    }
                }
                _bcm_esw_stat_flex_set_group_mode(
                                 unit,bcmStatFlexDirectionEgress,
                                 mode, flex_group_mode);
            }
            flex_group_mode_related_info++;
       }
       sal_free(attr_selectors_tmp);
       if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) && 
           (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_4))) {
#if defined(BCM_TOMAHAWK_SUPPORT)
           flex_custom_mode_related_info = (void *)flex_group_mode_related_info;
           for (mode=BCM_CUSTOM_INGRESS_MODE_START; 
                mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE; mode++) {
             if (flex_custom_mode_related_info->custom_mode != 0) {
                 flex_custom_ingress_mode.offset_mode = flex_custom_mode_related_info->offset_mode;
                 flex_custom_ingress_mode.AllocPool = flex_custom_mode_related_info->AllocPool;
                 flex_custom_ingress_mode.used = 1;
                 flex_custom_ingress_mode.hint.value = 0;
                 flex_custom_ingress_mode.hint.type = 0;
 
                 if (flex_custom_mode_related_info->fg_id != 0xffffffff) {
                     flex_custom_ingress_mode.hint.type = bcmStatGroupAllocHintIngressFieldGroup;
                     flex_custom_ingress_mode.hint.value = flex_custom_mode_related_info->fg_id;
                 }
                 flex_custom_ingress_mode.num_selectors = 0;
                 flex_custom_ingress_mode.attr_selectors = NULL;
                 flex_custom_ingress_mode.total_counters = 0;

                 _bcm_esw_stat_flex_update_custom_ingress_mode_info(unit, flex_custom_mode_related_info->custom_mode, &flex_custom_ingress_mode);
             }
             flex_custom_mode_related_info++;
           }
           flex_stat_ctr_related_info = (void *)flex_custom_mode_related_info;
           for (stat_ctr_id=1; 
              (stat_ctr_id < BCM_MAX_STAT_COUNTER_IDS) ; stat_ctr_id++, flex_stat_ctr_related_info++) {
                 if (flex_stat_ctr_related_info->used) {

                     _bcm_fill_stat_counter_map(unit, stat_ctr_id, flex_stat_ctr_related_info->mode, 
                                       flex_stat_ctr_related_info->pool, 
                                       flex_stat_ctr_related_info->base_idx, 
                                       flex_stat_ctr_related_info->object, 
                                       flex_stat_ctr_related_info->group); 
                     stat_counter_map[unit][stat_ctr_id].pipe = 0;
                     if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_5)) {
                         if ((flex_stat_ctr_related_info->mode >= BCM_CUSTOM_INGRESS_MODE_START)) {
                             int mode_idx = flex_stat_ctr_related_info->mode - BCM_CUSTOM_INGRESS_MODE_START;
                             /* Top 2 bits encode pipe and remaining 6 bits for total counters */
                             flex_custom_ingress_modes[unit][mode_idx].total_counters = flex_stat_ctr_related_info->used & 0x3f;
                             stat_counter_map[unit][stat_ctr_id].pipe = (flex_stat_ctr_related_info->used >> 6);
                         }
                     } else if (flex_stat_ctr_related_info->mode >= BCM_CUSTOM_INGRESS_MODE_START) {
                             int mode_idx = flex_stat_ctr_related_info->mode - BCM_CUSTOM_INGRESS_MODE_START;
                             mode = flex_custom_ingress_modes[unit][mode_idx].offset_mode;
                             flex_custom_ingress_modes[unit][mode_idx].total_counters = flex_ingress_modes[unit][mode].total_counters;
                             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit, "PREVIOUS SCACHE\n")));

                     }

                     if (_bcm_esw_stat_validate_object(unit,
                         flex_stat_ctr_related_info->object,
                         &direction) != BCM_E_NONE) {
                         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Invalid object %d so skipping it \n"),
                                    object));
                         continue;
                     }

                     pool_number = flex_stat_ctr_related_info->pool;
                     base_index = flex_stat_ctr_related_info->base_idx;

                     if (flex_stat_ctr_related_info->mode >= BCM_CUSTOM_INGRESS_MODE_START) {
                        int mode_idx = flex_stat_ctr_related_info->mode - BCM_CUSTOM_INGRESS_MODE_START;
                        total_counters = flex_custom_ingress_modes[unit][mode_idx].total_counters;
                     } else {
                         int mode_idx = flex_stat_ctr_related_info->mode;
                         total_counters = flex_ingress_modes[unit][mode_idx].total_counters;
                     }

                     shr_aidxres_list_reserve_block(
                         flex_aidxres_list_handle[unit][direction][pipe_num][pool_number],
                         flex_stat_ctr_related_info->base_idx,
                         total_counters);
                 }
           }
           *per_pipe_mode_ifp = *(uint32 *)flex_stat_ctr_related_info;
           *per_pipe_mode_vfp = *((uint32 *)flex_stat_ctr_related_info + 1);
#endif
       }
    } else {
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "OLD Version so skipping NEW FLEX Layout..\n")));
    }

    if (!soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
       for (index=0;index<BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE;index++) {
            if (scache_ptr[index] != 0) {
                stat_counter_id = scache_ptr[index];
                _bcm_esw_stat_get_counter_id_info(
                         unit, stat_counter_id,&group_mode,&object,
                         &offset_mode,&pool_number,&base_index);
                if (_bcm_esw_stat_validate_object(unit,object,&direction) !=
                    BCM_E_NONE) {
                    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Invalid object %d so skipping it \n"),
                               object));
                    continue;
                }
                if (direction == bcmStatFlexDirectionIngress) {
                     /* Quite possible..no attachment till now */
                     if (flex_ingress_modes[unit][offset_mode].total_counters
                         == 0) {
                          flex_ingress_modes[unit][offset_mode].total_counters =
                               _flex_group_mode_total_counters_info
                               [bcmStatFlexDirectionIngress][group_mode];
                          flex_ingress_modes[unit][offset_mode].group_mode =
                               group_mode;
                     }
                     total_counters = flex_ingress_modes[unit][offset_mode].
                                      total_counters;
                     flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                                   [pool_number].used_entries   += total_counters;
                     if (object == bcmStatObjectIngFieldStageIngress) {
                         flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number].used_by_tables|=
                                    FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
                     } else {
                          flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number].used_by_tables|=
                                       flex_used_by_table[object];
                     }
                     SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                                   [pool_number].used_by_objects, object);
                     if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
                         pool_group_id = pool_number/_bcm_esw_get_max_pool_groups(unit);
                         /* Update the Flex flex_pool_group for oam_lm  for TD2Plus*/
                         if (!BCM_FAILURE(_bcm_esw_stat_flex_ingress_group_oam_status
                                     (unit, pool_group_id,
                                      &enable))) {
                             if (enable ) {
                                 allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OAM;
                             } else {
                                 allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OTHER;
                             }
                             /* Update the pool_group type */
                             flex_pool_group[unit][pipe_num][pool_group_id].group_type
                                 = allocate_group_type;
                         }
                     }

                } else {
                     direction = bcmStatFlexDirectionEgress;
                     /* Quite possible..no attachment till now */
                     if (flex_egress_modes[unit][offset_mode].total_counters
                         == 0) {
                          flex_egress_modes[unit][offset_mode].total_counters =
                               _flex_group_mode_total_counters_info
                               [bcmStatFlexDirectionEgress][group_mode];
                          flex_egress_modes[unit][offset_mode].group_mode =
                               group_mode;
                     }
                     total_counters = flex_egress_modes[unit][offset_mode].
                                      total_counters;
                     flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                                   [pool_number].used_entries   += total_counters;
                     flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                                   [pool_number].used_by_tables |=
                                   flex_used_by_table[object];
                }
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Installing: mode:%d group_mode:%d pool:%d"
                                       "object:%d base:%d\n"),offset_mode,group_mode,
                           pool_number, object,base_index));
                if (total_counters == 0) {
                    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Counter=0.Mode not configured in h/w."
                                           "skipping it\n")));
                    continue;
                }
                shr_aidxres_list_reserve_block(
                    flex_aidxres_list_handle[unit][direction][pipe_num][pool_number],
                    base_index,
                    total_counters);
                BCM_STAT_FLEX_COUNTER_LOCK(unit);
                if (direction == bcmStatFlexDirectionIngress) {
                    /* Skip ref count increment for attached counters
                     * since its already being done by flex_check_ingress_table
                     * and flex_check_egress_table
                     */
                    if (flex_base_index_reference_count[unit]
                        [bcmStatFlexDirectionIngress][pipe_num][pool_number]
                        [base_index] == 0) {
                        flex_ingress_modes[unit][offset_mode].reference_count++;
                    }
                } else {
                    if (flex_base_index_reference_count[unit]
                        [bcmStatFlexDirectionEgress][pipe_num][pool_number]
                        [base_index] == 0) {
                        flex_egress_modes[unit][offset_mode].reference_count++;
                    }
                }
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            }
        }
    }
    return BCM_E_NONE;
}
#endif

/*
 * Function:
 *      _bcm_esw_stat_flex_table_index_map
 * Description:
 *      Mapped the table index to table entry index on configured h/w.
 *      Useful in those tabe who has multi-items in each entry.
 * Parameters:
 *      unit                  - (IN) unit number
 *      table                 - (IN) Flex Table
 *      index                 - (IN) Flex Table Index
 * Return Value:
 *      index_mapped          - (OUT) Flex Table Entry Index
 * Notes:
 *
 */
STATIC int
_bcm_esw_stat_flex_table_index_map(
    int        unit,
    soc_mem_t  table,
    int        index)
{
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit)) {
        /* Two Items in Each Entry */
        if (table == EGR_NAT_PACKET_EDIT_INFOm ||
            table == L3_DEFIPm) {
            return index >> 1;
        }
    }
#endif

    return index;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_retrieve_group_mode
 * Description:
 *      Retrieves Flex group mode based on configured h/w.
 *      Useful in warm boot case
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data flow direction
 *      offset_mode           - (IN) Flex offset mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t
_bcm_esw_stat_flex_retrieve_group_mode(
    int                       unit,
    bcm_stat_flex_direction_t direction,
    uint32                    offset_mode)
{
    bcm_stat_flex_ing_attr_t                    *ing_attr=NULL;
    bcm_stat_flex_ing_uncmprsd_attr_selectors_t *ing_uncmprsd_attr_selectors =
                                                NULL;
    bcm_stat_flex_ing_cmprsd_attr_selectors_t   *ing_cmprsd_attr_selectors=NULL;
    bcm_stat_flex_ing_pkt_attr_bits_t           *ing_cmprsd_pkt_attr_bits=NULL;

    bcm_stat_flex_egr_attr_t                    *egr_attr=NULL;
    bcm_stat_flex_egr_uncmprsd_attr_selectors_t *egr_uncmprsd_attr_selectors =
                                                NULL;
    bcm_stat_flex_egr_cmprsd_attr_selectors_t   *egr_cmprsd_attr_selectors=NULL;
    bcm_stat_flex_egr_pkt_attr_bits_t           *egr_cmprsd_pkt_attr_bits=NULL;

    uint32 unknown_pkt_val    = _bcm_esw_stat_flex_get_pkt_res_value(
                                unit,_UNKNOWN_PKT);
    uint32 control_pkt_val    =  _bcm_esw_stat_flex_get_pkt_res_value(
                                unit,_CONTROL_PKT);
    uint32 l2uc_pkt_val       = _bcm_esw_stat_flex_get_pkt_res_value(
                                unit,_L2UC_PKT);
    uint32 known_l2mc_pkt_val = _bcm_esw_stat_flex_get_pkt_res_value(
                                unit,_KNOWN_L2MC_PKT);

    if (direction == bcmStatFlexDirectionIngress) {
        if (flex_ingress_modes[unit][offset_mode].available == 0) {
            return BCM_E_NONE;
        }
        ing_attr                    = &(flex_ingress_modes[unit][offset_mode].
                                        ing_attr);
        ing_uncmprsd_attr_selectors = &(ing_attr->uncmprsd_attr_selectors);
        ing_cmprsd_attr_selectors   = &(ing_attr->cmprsd_attr_selectors);
        ing_cmprsd_pkt_attr_bits    = &(ing_cmprsd_attr_selectors->
                                        pkt_attr_bits);

        switch(flex_ingress_modes[unit][offset_mode].total_counters) {
        case 1:
             /* bcmStatGroupModeSingle */
             return _bcm_esw_stat_flex_set_group_mode(
                                 unit,bcmStatFlexDirectionIngress,
                                 offset_mode,bcmStatGroupModeSingle);
        case 2:
             /* bcmStatGroupModeDlfAll,bcmStatGroupModeSingleWithControl,
                bcmStatGroupModeSvpType */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS) {
                 return _bcm_esw_stat_flex_set_group_mode(
                                 unit,bcmStatFlexDirectionIngress,
                                 offset_mode,bcmStatGroupModeSvpType);
             }
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                 if ((ing_uncmprsd_attr_selectors->
                      offset_table_map[unknown_pkt_val].offset == 1) &&
                     (ing_uncmprsd_attr_selectors->
                      offset_table_map[control_pkt_val].offset == 1)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeDlfAllWithControl);
                 }
                 if ((ing_uncmprsd_attr_selectors->
                      offset_table_map[unknown_pkt_val].offset == 0) &&
                     (ing_uncmprsd_attr_selectors->
                      offset_table_map[control_pkt_val].offset == 1)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeSingleWithControl);
                 }
             }
             return BCM_E_PARAM;
        case 3:
             /* bcmStatGroupModeTrafficType,bcmStatGroupModeDlfAllWithControl */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[known_l2mc_pkt_val].offset == 2) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeDlfAllWithControl);
                 }
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[known_l2mc_pkt_val].offset == 1) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeTrafficType);
                 }
             }
             return BCM_E_PARAM;
        case 4:
             /* bcmStatGroupModeTyped, bcmStatGroupModeTrafficTypeWithControl */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[known_l2mc_pkt_val].offset == 2) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeTyped);
                 }
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[known_l2mc_pkt_val].offset == 1) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeTrafficTypeWithControl);
                 }
             } else {
                 if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeCng);
                 }
             }
             return BCM_E_PARAM;
        case 5:
             /* bcmStatGroupModeTypedAll , bcmStatGroupModeTypedWithControl */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[l2uc_pkt_val].offset == 1) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeTyped);
                 }
                 if (ing_uncmprsd_attr_selectors->
                     offset_table_map[l2uc_pkt_val].offset == 2) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeTrafficTypeWithControl);
                 }
             }
             return BCM_E_PARAM;
        case 6:
             /* bcmStatGroupModeTypedAllWithControl */
            if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                return _bcm_esw_stat_flex_set_group_mode(
                           unit,
                           bcmStatFlexDirectionIngress,
                           offset_mode,
                           bcmStatGroupModeTypedAllWithControl);
                }
             return BCM_E_PARAM;
        case 8:
             /* bcmStatGroupModeDot1P */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS) {
                 return _bcm_esw_stat_flex_set_group_mode(
                            unit,
                            bcmStatFlexDirectionIngress,
                            offset_mode,
                            bcmStatGroupModeDot1P);
             }
             return BCM_E_PARAM;
        case 16:
             /* bcmStatGroupModeIntPri */
             if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS) {
                 return _bcm_esw_stat_flex_set_group_mode(
                            unit,
                            bcmStatFlexDirectionIngress,
                            offset_mode,
                            bcmStatGroupModeIntPri);
             }
             return BCM_E_PARAM;
        case 17:
             /* bcmStatGroupModeDlfIntPri */
             if (ing_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeCompressed) {
                 if ((ing_cmprsd_pkt_attr_bits->int_pri == 4) &&
                     (ing_cmprsd_pkt_attr_bits->pkt_resolution == 1)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeDlfIntPri);
                 }
             }
             return BCM_E_PARAM;
        case 18:
             /* bcmStatGroupModeDlfIntPriWithControl */
             if (ing_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeCompressed) {
                 if ((ing_cmprsd_pkt_attr_bits->int_pri == 4) &&
                     (ing_cmprsd_pkt_attr_bits->pkt_resolution == 2)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeDlfIntPriWithControl);
                 }
             }
             return BCM_E_PARAM;
        case 20:
             /* bcmStatGroupModeTypedIntPri */
             if (ing_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeCompressed) {
                 if ((ing_cmprsd_pkt_attr_bits->int_pri == 4) &&
                     (ing_cmprsd_pkt_attr_bits->pkt_resolution == 3)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeTypedIntPri);
                 }
             }
             return BCM_E_PARAM;
        case 21:
             /* bcmStatGroupModeTypedIntPriWithControl */
             if (ing_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeCompressed) {
                 if ((ing_cmprsd_pkt_attr_bits->int_pri == 4) &&
                     (ing_cmprsd_pkt_attr_bits->pkt_resolution == 3)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionIngress,
                                 offset_mode,
                                 bcmStatGroupModeTypedIntPriWithControl);
                 }
             }
             return BCM_E_PARAM;
        case 64:
             /* bcmStatGroupModeIntPriCng , bcmStatGroupModeDscp */
             if (ing_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeUncompressed) {
                 if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeDscp);
                 }
                 if (ing_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     (BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_ATTR_BITS|
                      BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS)) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionIngress,
                                offset_mode,
                                bcmStatGroupModeIntPriCng);
                 }
             }
             return BCM_E_PARAM;
        default:
             LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "INFO:No fixed Counter:%d so probably a FlexGroup\n"),
                                     flex_ingress_modes[unit][offset_mode].total_counters));
             return BCM_E_NONE;
        }
    } else {
        if (flex_egress_modes[unit][offset_mode].available == 0) {
            return BCM_E_NONE;
        }
        egr_attr                    = &(flex_egress_modes[unit][offset_mode].
                                        egr_attr);
        egr_uncmprsd_attr_selectors = &(egr_attr->uncmprsd_attr_selectors);
        egr_cmprsd_attr_selectors   = &(egr_attr->cmprsd_attr_selectors);
        egr_cmprsd_pkt_attr_bits    = &(egr_cmprsd_attr_selectors->
                                        pkt_attr_bits);
        switch(flex_egress_modes[unit][offset_mode].total_counters) {
        case 1:
             /* bcmStatGroupModeSingle, bcmStatGroupModeDlfAll ,
                bcmStatGroupModeSingleWithControl */
             if ((egr_attr->packet_attr_type ==
                  bcmStatFlexPacketAttrTypeUncompressed) &&
                 (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                  BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS)){
                  return _bcm_esw_stat_flex_set_group_mode(
                             unit,
                             bcmStatFlexDirectionEgress,
                             offset_mode,
                             bcmStatGroupModeSingle);
             }
             return BCM_E_PARAM;
        case 2:
             /* bcmStatGroupModeTrafficType, bcmStatGroupModeTyped ,
                bcmStatGroupModeTypedAll,
                bcmStatGroupModeTrafficTypeWithControl,
                bcmStatGroupModeDlfAllWithControl,
                bcmStatGroupModeTypedWithControl,
                bcmStatGroupModeTypedAllWithControl, bcmStatGroupModeSvpType ,
                bcmStatGroupModeDvpType  */
             if (egr_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeUncompressed) {
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_DVP_TYPE_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                         unit,bcmStatFlexDirectionEgress,
                                         offset_mode,bcmStatGroupModeDvpType);
                    }
                 if (egr_uncmprsd_attr_selectors->
                     uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeSvpType);
                 }
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                  BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeTrafficType);
                 }
             }
             return BCM_E_PARAM;
        case 4:
             if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                 BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeCng);
             }
             return BCM_E_PARAM;
        case 8:
             /* bcmStatGroupModeDot1P */
             if (egr_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeUncompressed) {
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeDot1P);
                 }
             }
             return BCM_E_PARAM;
        case 16:
             /* bcmStatGroupModeIntPri */
             if (egr_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeUncompressed) {
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeIntPri);
                 }
             }
             return BCM_E_PARAM;
        case 18:
             /* bcmStatGroupModeTypedIntPri */
             if (egr_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeCompressed) {
                 if ((egr_cmprsd_pkt_attr_bits->int_pri == 4) &&
                     (egr_cmprsd_pkt_attr_bits->pkt_resolution == 1)) {
                      return _bcm_esw_stat_flex_set_group_mode(
                                 unit,
                                 bcmStatFlexDirectionEgress,
                                 offset_mode,
                                 bcmStatGroupModeTypedIntPri);
                  }
             }
             return BCM_E_PARAM;
        case 64:
             /* bcmStatGroupModeIntPriCng , bcmStatGroupModeDscp */
             if (egr_attr->packet_attr_type ==
                 bcmStatFlexPacketAttrTypeUncompressed) {
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeDscp);
                 }
                 if (egr_uncmprsd_attr_selectors->uncmprsd_attr_bits_selector ==
                     (BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_ATTR_BITS|
                      BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS)) {
                     return _bcm_esw_stat_flex_set_group_mode(
                                unit,
                                bcmStatFlexDirectionEgress,
                                offset_mode,
                                bcmStatGroupModeIntPriCng);
                 }
             }
             return BCM_E_PARAM;
        default:
             LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "INFO:No fixed Counter:%d so probably a FlexGroup\n"),
                                     flex_ingress_modes[unit][offset_mode].total_counters));
             return BCM_E_NONE;
        }
    }
    return BCM_E_PARAM;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_oam_enable_ingress_group
 * Description:
 *      Enable/Disable ingress flex counter pool group.
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t _bcm_esw_stat_flex_oam_enable_ingress_group(
            int                       unit,
            uint32                    group,
            uint32                    enable)
{
    uint32  flex_ctr_group_lm_config_value=0;
    uint32  enable_value;

    if (group >= 
             soc_reg_field_length(unit, ING_FLEX_CTR_LM_CONFIGr, LM_ENABLEf)) { 
        return BCM_E_PARAM;
    }
    
    if ( enable ) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "...Enabling Flex Counter Group:%d for OAM \n"),
                              group));
    } else {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "...Disabling Flex Counter Group:%d for OAM \n"),
                              group));
    }
    /* First Get complete value of
       ING_FLEX_CTR_LM_CONFIGr value */
    SOC_IF_ERROR_RETURN(soc_reg32_get(
                        unit,
                        ING_FLEX_CTR_LM_CONFIGr,
                        REG_PORT_ANY, 0,
                        &flex_ctr_group_lm_config_value));
     enable_value = soc_reg_field_get(unit,
                                      ING_FLEX_CTR_LM_CONFIGr,
                                      flex_ctr_group_lm_config_value, 
                                      LM_ENABLEf);
    /* Next set field value for
       ING_FLEX_CTR_LM_CONFIGr:LM_ENABLE field*/
    if (enable) {
        enable_value |= 1 << group;
    }else {
        enable_value &= ~(1 << group);
    }
    soc_reg_field_set(unit,
                      ING_FLEX_CTR_LM_CONFIGr,
                      &flex_ctr_group_lm_config_value,
                      LM_ENABLEf,
                      enable_value);
    /* Finally set value for
       ING_FLEX_CTR_LM_CONFIGr:LM_ENABLE field*/
    SOC_IF_ERROR_RETURN(soc_reg32_set(unit,
                                      ING_FLEX_CTR_LM_CONFIGr,
                                      REG_PORT_ANY,
                                      0,
                                      flex_ctr_group_lm_config_value));
    return BCM_E_NONE;
}

/* Function to enable/disable a pool.
 * Call counter module to program the status in hardware
 */
static
soc_error_t soc_counter_pool_status_set(int unit, int direction,
                                        int index, uint32 enable)
{
    soc_ctr_control_info_t ctrl_info[2];
    soc_ctr_ctrl_config_t ctrl_config[2];
    soc_reg_t ctr_reg_pkt, ctr_reg_byte;
    int i;

    for (i = 0; i < 2; i++) {
        ctrl_info[i].instance_type = SOC_CTR_INSTANCE_TYPE_POOL;
        ctrl_info[i].instance = index;
    }
    ctrl_config[0].config_type = SOC_CTR_CTRL_CONFIG_DMA_ENABLE;
    ctrl_config[0].config_val = enable;

    ctrl_config[1].config_type = SOC_CTR_CTRL_CONFIG_DMA_RATE_PROFILE_ALL;
    ctrl_config[1].config_val = 1; /* Enable ALL profiles for EFP_PKT */

    if (direction == bcmStatFlexDirectionEgress) {
        ctr_reg_pkt = SOC_COUNTER_NON_DMA_EGR_FLEX_PKT;
        ctr_reg_byte = SOC_COUNTER_NON_DMA_EGR_FLEX_BYTE;
    } else {
        ctr_reg_pkt = SOC_COUNTER_NON_DMA_ING_FLEX_PKT;
        ctr_reg_byte = SOC_COUNTER_NON_DMA_ING_FLEX_BYTE;
    }

    SOC_IF_ERROR_RETURN
        (soc_counter_config_multi_set(unit, ctr_reg_pkt, 2, ctrl_info,
                                      ctrl_config));
    SOC_IF_ERROR_RETURN
        (soc_counter_config_multi_set(unit, ctr_reg_byte, 2,
                                      ctrl_info, ctrl_config));

    return SOC_E_NONE;
}

/* Function to Inform Counter module to enable DMA for Flex Pools.
 * Notes:
 *      Applicable in warm boot scenario only.
 *
 */
static
soc_error_t soc_counter_pool_bmap_status_set(int unit, int direction,
                                             uint32 pool_bmap, uint32 enable)
{
    int i = 0, max_pools;
    if (!soc_feature(unit, soc_feature_centralized_counter)) {
        return SOC_E_NONE;
    }

    if (direction == bcmStatFlexDirectionIngress) {
        max_pools = SOC_INFO(unit).num_flex_ingress_pools;
    } else {
        max_pools = SOC_INFO(unit).num_flex_egress_pools;
    }

    for (i = 0; i < max_pools; i++) {
        if (pool_bmap & (1 << i)) {
            SOC_IF_ERROR_RETURN
                (soc_counter_pool_status_set(unit, direction, i, enable));
        }
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_enable_pool
 * Description:
 *      Enable/Disable ingress/egress flex counter pool.
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t _bcm_esw_stat_flex_enable_pool(
            int                       unit,
            bcm_stat_flex_direction_t direction,
            soc_reg_t                 flex_pool_ctr_update_control_reg,
            uint32                    enable)
{
    uint32  flex_pool_ctr_update_control_reg_value=0;
    uint32  enable_value=1;
    uint32  num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32  index=0;

    num_pools[bcmStatFlexDirectionIngress]=
             SOC_INFO(unit).num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] =
             SOC_INFO(unit).num_flex_egress_pools;
    if (direction >= BCM_STAT_FLEX_COUNTER_MAX_DIRECTION) {
        return BCM_E_PARAM;
    }
    for(index=0;
        index < num_pools[direction];
        index++) {
        if (_pool_ctr_register[direction][index] ==
            flex_pool_ctr_update_control_reg ) {
            break;
        }
    }
    if (index == num_pools[direction]) {
         return BCM_E_PARAM;
    }
    if ( enable ) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "...Enabling pool:%s \n"),
                   SOC_REG_NAME(unit, flex_pool_ctr_update_control_reg)));
    } else {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "...Disabling pool:%s \n"),
                   SOC_REG_NAME(unit, flex_pool_ctr_update_control_reg)));
    }
    /* First Get complete value of
       EGR/ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_?r value */
    SOC_IF_ERROR_RETURN(soc_reg32_get(
                        unit,
                        flex_pool_ctr_update_control_reg,
                        REG_PORT_ANY, 0,
                        &flex_pool_ctr_update_control_reg_value));
    /* Next set field value for
       EGR/ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_?r:COUNTER_POOL_ENABLE field*/
    if (enable) {
        enable_value=1;
    }else {
        enable_value=0;
    }
    soc_reg_field_set(unit,
                      flex_pool_ctr_update_control_reg,
                      &flex_pool_ctr_update_control_reg_value,
                      COUNTER_POOL_ENABLEf,
                      enable_value);
    /* Finally set value for
       EGR/ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_?r:COUNTER_POOL_ENABLE field*/
    SOC_IF_ERROR_RETURN(soc_reg32_set(unit,
                                      flex_pool_ctr_update_control_reg,
                                      REG_PORT_ANY,
                                      0,
                                      flex_pool_ctr_update_control_reg_value));
    /* Call Counter module to program the hardware */
    if (soc_feature(unit, soc_feature_centralized_counter)) {
        return soc_counter_pool_status_set(unit, direction, index, enable);
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_retrieve_total_counters
 * Description:
 *      Retries total counter based on flex h/w configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static uint32
_bcm_esw_stat_flex_retrieve_total_counters(
    int                       unit,
    bcm_stat_flex_direction_t direction,
    uint32                    pool_number,
    uint32                    offset_mode)
{
    uint32                             index=0;
    ing_flex_ctr_offset_table_0_entry_t *flex_ctr_offset_table_entry=NULL;
    uint32                             count_enable=0;
    bcm_stat_flex_offset_table_entry_t *sw_offset_table=NULL;
    uint32                             offset=0;
    uint32                             total_counters_flag=0;
    uint32                             total_counters=0;
    uint32                             alloc_size=0;


    /* Actually we need only 256 offsets only but ...*/
    alloc_size =  soc_mem_index_count(unit,ING_FLEX_CTR_OFFSET_TABLE_0m) *
                  sizeof(ing_flex_ctr_offset_table_0_entry_t);

    flex_ctr_offset_table_entry = soc_cm_salloc(unit,alloc_size,
                                                "flex_ctr_offset_table_entry");
    if (flex_ctr_offset_table_entry == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Memory Allocation failed:flex_ctr_offset_table_entry\n")));
        return  BCM_E_INTERNAL;
    }
    sal_memset(flex_ctr_offset_table_entry, 0, alloc_size);
    /* 0-255,256-511,512-...*/
    if (soc_mem_read_range(unit,
                     _ctr_offset_table[direction][pool_number],
                     MEM_BLOCK_ANY,
                     (offset_mode <<8),
                     (offset_mode <<8)+ (256) - 1,
                     flex_ctr_offset_table_entry) != SOC_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Memory Reading failed:flex_ctr_offset_table_entry \n")));
        soc_cm_sfree(unit,flex_ctr_offset_table_entry);
        return 0;
    }
    if (direction == bcmStatFlexDirectionIngress) {
        switch(flex_ingress_modes[unit][offset_mode].ing_attr.
               packet_attr_type) {
        case bcmStatFlexPacketAttrTypeUncompressed:
             sw_offset_table = &flex_ingress_modes[unit][offset_mode].ing_attr.
                                uncmprsd_attr_selectors.offset_table_map[0];
             break;
        case bcmStatFlexPacketAttrTypeCompressed:
             sw_offset_table = &flex_ingress_modes[unit][offset_mode].ing_attr.
                                cmprsd_attr_selectors.offset_table_map[0];
             break;
        case bcmStatFlexPacketAttrTypeUdf:
        default:
             /* With current implemControl must not reach over here */
             soc_cm_sfree(unit,flex_ctr_offset_table_entry);
             return BCM_E_PARAM;
        }
    } else {
        switch(flex_egress_modes[unit][offset_mode].egr_attr.packet_attr_type) {
        case bcmStatFlexPacketAttrTypeUncompressed:
             sw_offset_table = &flex_egress_modes[unit][offset_mode].egr_attr.
                                uncmprsd_attr_selectors.offset_table_map[0];
             break;
        case bcmStatFlexPacketAttrTypeCompressed:
             sw_offset_table = &flex_egress_modes[unit][offset_mode].egr_attr.
                                cmprsd_attr_selectors.offset_table_map[0];
             break;
        case bcmStatFlexPacketAttrTypeUdf:
        default:
             /* With current implemControl must not reach over here */
             soc_cm_sfree(unit,flex_ctr_offset_table_entry);
             return BCM_E_PARAM;
        }
    }
    for (index=0;index<(256);index++) {
         count_enable=0;
         /*First Get complete value of EGR/ING_FLEX_CTR_OFFSET_TABLE_?m value */
         soc_mem_field_get(unit,
                           _ctr_offset_table[direction][pool_number],
                           (uint32 *)&flex_ctr_offset_table_entry[index],
                           COUNT_ENABLEf,&count_enable);
         soc_mem_field_get(unit,
                           _ctr_offset_table[direction][pool_number],
                           (uint32 *)&flex_ctr_offset_table_entry[index],
                           OFFSETf,
                           &offset);
         if (count_enable) {
             total_counters_flag=1;
             if (total_counters < offset) {
                 total_counters = offset;
             }
         }
         sw_offset_table[index].offset = offset;
         sw_offset_table[index].count_enable = count_enable;
    }
    soc_cm_sfree(unit,flex_ctr_offset_table_entry);
    return (total_counters+total_counters_flag);
}

/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_table
 * Description:
 *      Checkes flex egress table. If table were configured for flex counter
 *      updates s/w copy accordingly.
 *
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Egress  Table
 *      index                 - (IN) Flex Egress  Table Index
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only.
 *
 */
static void
_bcm_esw_stat_flex_check_egress_table(
    int       unit,
    soc_mem_t egress_table,
    int       min_index,
    int       max_index)
{
    uint32            offset_mode=0;
    uint32            pool_number=0;
    uint32            base_idx=0;
    uint32            egress_entry_data_size=0;
    void              *egress_entry_data=NULL;
    void              *egress_entry_full_data=NULL;
    bcm_stat_object_t object=bcmStatObjectEgrPort;
    int               index=0;
    uint32            counter_pool_bmap = 0; /* To enable counters */
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    void              *flex_entry_data = NULL;
    uint32            flex_entry_data_size=0;
#endif
    int               exact_index = 0;

    if (!((egress_table == EGR_VLANm) ||
          (egress_table == EGR_VFIm)  ||
          (egress_table == EGR_L3_NEXT_HOPm)  ||
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
          (egress_table == EGR_IP_TUNNEL_MPLSm) ||
          (egress_table == EFP_POLICY_TABLEm) ||
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
          (egress_table == EGR_DVP_ATTRIBUTE_1m)  ||
          (egress_table == EGR_NAT_PACKET_EDIT_INFOm)  ||
#endif /* BCM_TRIDENT2_SUPPORT */
          (egress_table == EGR_VLAN_XLATEm)  ||
          (egress_table == EGR_PORTm))) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Invalid Flex Counter Ingress Memory %s\n"),
                      SOC_MEM_UFNAME(unit, egress_table)));
           return;
    }

    if (!SOC_MEM_IS_VALID(unit, egress_table)) {
        /* This check is for low latency mode */
        return;
    }

    if (max_index > soc_mem_index_max(unit,egress_table)) {
        max_index = soc_mem_index_max(unit,egress_table);
    }
    egress_entry_data_size= WORDS2BYTES(BYTES2WORDS(
                                        SOC_MEM_INFO(unit,egress_table).bytes));
    egress_entry_full_data = soc_cm_salloc(unit,egress_entry_data_size *
                                  (max_index - min_index + 1),"egress_table");
    if (egress_entry_full_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, egress_table)));
        return ;
    }
    sal_memset(egress_entry_full_data,0,(egress_entry_data_size *
                                  (max_index - min_index + 1)));

    if (soc_mem_read_range(unit, egress_table, MEM_BLOCK_ANY,
                min_index,max_index, egress_entry_full_data) != SOC_E_NONE) {
        soc_cm_sfree(unit,egress_entry_full_data);
        return;
    }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (egress_table == EGR_IP_TUNNEL_MPLSm) {
        flex_entry_data_size = WORDS2BYTES(BYTES2WORDS(SOC_MEM_INFO(unit,
                        egress_table).bytes));
        flex_entry_data = sal_alloc(flex_entry_data_size,"flex-counter-table");
        if (flex_entry_data == NULL) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Failed to allocate memory for Table:%s "),
                     SOC_MEM_UFNAME(unit, egress_table)));
            return;
        }
    }
#endif
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    for(index=0; index <= (max_index - min_index) ; index++) {
        egress_entry_data = soc_mem_table_idx_to_pointer(unit, egress_table,
                            void *, egress_entry_full_data, index);
        if (soc_mem_field_valid(unit,egress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,egress_table,egress_entry_data,
                VALIDf)==0) {
                continue;
            }
        }
        if (egress_table == EGR_VFIm) {
            object = bcmStatObjectEgrVfi;
        } else if (egress_table == EGR_L3_NEXT_HOPm) {
            object = bcmStatObjectEgrL3Intf;
        } else if (_bcm_esw_stat_flex_get_egress_object(unit,egress_table,
            index+min_index,egress_entry_data,&object) != BCM_E_NONE) {
            continue;
        }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (egress_table == EGR_IP_TUNNEL_MPLSm) {
            exact_index = (index + min_index)/
                         _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
        } else
#endif
        {
            exact_index = index + min_index;
        }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (egress_table == EGR_IP_TUNNEL_MPLSm) {
            sal_memset(flex_entry_data,0,flex_entry_data_size);
            if (soc_mem_read(unit,egress_table,MEM_BLOCK_ANY,
                        _bcm_esw_stat_flex_table_index_map(unit,egress_table,
                            exact_index),flex_entry_data) == SOC_E_NONE) {
                if (soc_mem_field_valid(unit,egress_table,VALIDf)) {
                    if (soc_mem_field32_get(unit,egress_table,
                                flex_entry_data,VALIDf)==0) {
                        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Table %s  with index %d is Not valid \n"),
                                 SOC_MEM_UFNAME(unit, egress_table),exact_index));
                        sal_free(flex_entry_data);
                        return;
                    }
                }

                _bcm_esw_get_flex_counter_fields_values(
                        unit,index+min_index,egress_table,
                        flex_entry_data,object,
                        &offset_mode,&pool_number,&base_idx);
                if ((offset_mode == 0) && (base_idx == 0)) {
                    continue;
                }
            }
        } else
#endif
        {
            _bcm_esw_get_flex_counter_fields_values(
                    unit,exact_index,egress_table,
                    egress_entry_data,object,
                    &offset_mode,&pool_number,&base_idx);
            if ((offset_mode == 0) && (base_idx == 0)) {
                continue;
            }
        }
        flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress]
                                       [0][pool_number][base_idx]++;
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TRIDENT2X(unit) &&
            soc_feature(unit, soc_feature_advanced_flex_counter)) {
            _flex_stat_mode_base_map[unit][bcmStatFlexDirectionEgress]
                                  [pool_number][offset_mode][base_idx] = 1;
        }
#endif
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress]
                                           [0][pool_number][base_idx] == 1) {
            flex_egress_modes[unit][offset_mode].reference_count++;
        }
        if (flex_egress_modes[unit][offset_mode].total_counters == 0) {
            flex_egress_modes[unit][offset_mode].total_counters =
               _bcm_esw_stat_flex_retrieve_total_counters(
                  unit, bcmStatFlexDirectionEgress, pool_number, offset_mode);
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Max_offset_table_value %d\n"),
                       flex_egress_modes[unit][offset_mode].
                       total_counters));
        }
        shr_aidxres_list_reserve_block(
            flex_aidxres_list_handle
            [unit][bcmStatFlexDirectionEgress][0][pool_number],
            base_idx,
            flex_egress_modes[unit][offset_mode].total_counters);
        flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                      used_entries += flex_egress_modes[unit][offset_mode].
                                                       total_counters;
        flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                      attached_entries += flex_egress_modes[unit][offset_mode].
                                                       total_counters;
        flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                      used_by_tables |= flex_used_by_table[object];
        SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                      [pool_number].used_by_objects, object);
        /* Set pool bmap to enable Counters to perform DMA.
         * Used in Centralized Counter model
         */
        counter_pool_bmap |= 1 << pool_number;
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Table:%s index=%d mode:%d pool_number:%d base_idx:%d\n"),
                   SOC_MEM_UFNAME(unit, egress_table),
                   index+min_index,offset_mode,pool_number,base_idx));
        base_idx = 0, offset_mode = 0;
    }
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    soc_cm_sfree(unit,egress_entry_full_data);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (egress_table == EGR_IP_TUNNEL_MPLSm) {
        sal_free(flex_entry_data);
    }
#endif
    if (counter_pool_bmap) {
        int rv = SOC_E_NONE;
        rv = soc_counter_pool_bmap_status_set(unit, bcmStatFlexDirectionEgress,
                                              counter_pool_bmap, 1);
        if (rv != SOC_E_NONE) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Err when Enabling Pools for Counter DMA. "
                                 "Egress Pool Bmap 0x%x\n"),
                      counter_pool_bmap));
        }
    }
    return;
}
#ifdef BCM_WARM_BOOT_SUPPORT
#if defined (BCM_TOMAHAWK_SUPPORT)
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_em_table_entry
 * Description:
 *      Checks flex Exact Match ingress table entry. 
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Ingress  Table
 *      offset_mode           - (IN) Flex offset_mode programmed in this entry
 *      pool_number           - (IN) Flex pool_number programmed in this entry
 *      base_idx              - (IN) Flex base idx programmed in this entry
 * Return Value:
 *      BCM_E_NONE
 *      BCM_E_PARAM
 * Notes:
 *      Applicable in warm boot scenario only.
 *
 */
bcm_error_t
_bcm_esw_stat_flex_check_ingress_em_table_entry(
    int       unit,
    int       pipe_num,
    uint32    offset_mode,
    uint32    pool_number,
    uint32    base_idx)
{
    bcm_stat_object_t object=bcmStatObjectIngExactMatch;
    int rv = BCM_E_NONE;
    uint32 counter_pool_bmap = 0; /* To enable counters */
    uint32            total_counters = 0;

    BCM_STAT_FLEX_COUNTER_LOCK(unit);

        if ((offset_mode == 0) && (base_idx == 0)) {
             BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
             return BCM_E_PARAM;
        }
        flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number][base_idx]++;
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                           [pipe_num][pool_number][base_idx] == 1) {
            flex_ingress_modes[unit][offset_mode].reference_count++;
        }

        total_counters = flex_ingress_modes[unit][offset_mode].total_counters;

        if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_5) &&
            soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
            uint32 cmode = 0;
            bcm_error_t rv;
            uint32 stat_counter_id = 0;
 
            rv = _bcm_esw_custom_stat_group_id_retrieve(unit, offset_mode, pipe_num, 
                                                        pool_number, base_idx,
                                                        &stat_counter_id);
            if (rv !=BCM_E_NONE) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Invalid mode value %d %d %d\n"),
                       offset_mode, pool_number, base_idx));
            } else {
                cmode = stat_counter_map[unit][stat_counter_id].mode;
                total_counters = flex_custom_ingress_modes[unit][cmode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
            }
        }  else if (total_counters == 0) {
            total_counters =
                   _bcm_esw_stat_flex_retrieve_total_counters(
                    unit, bcmStatFlexDirectionIngress,
                    pool_number, offset_mode)  ;
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Max_offset_table_value %d\n"),
                       flex_ingress_modes[unit][offset_mode].
                       total_counters));
        }

        shr_aidxres_list_reserve_block(
            flex_aidxres_list_handle[unit][bcmStatFlexDirectionIngress][pipe_num]
                                    [pool_number],
            base_idx,
            total_counters);
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                      [pool_number].used_entries +=
                      total_counters;
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                      [pool_number].attached_entries +=
                      total_counters;
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                      used_by_tables |= FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE;
        SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                      [pool_number].used_by_objects, object);
        if (pipe_num != 0) {
            flex_pool_stat[unit][bcmStatFlexDirectionIngress][0][pool_number].
                          used_by_tables |= FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE;
            SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress][0]
                          [pool_number].used_by_objects, object);
        }
        /* Set pool bmap to enable Counters to perform DMA.
         * Used in Centralized Counter model
         */
        counter_pool_bmap |= 1 << pool_number;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    if (counter_pool_bmap) {
        int rv = SOC_E_NONE;
        rv = soc_counter_pool_bmap_status_set(unit, bcmStatFlexDirectionIngress,
                                              counter_pool_bmap, 1);
        if (rv != SOC_E_NONE) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Err when Enabling Pools for Counter DMA. "
                                 "EM Pool Bmap 0x%x\n"),
                      counter_pool_bmap));
        }
    }
    return rv;
}
#endif
#endif


/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_table
 * Description:
 *      Checkes flex ingress table. If table were configured for flex counter
 *      updates s/w copy accordingly.
 *
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Ingress  Table
 *      index                 - (IN) Flex Ingress  Table Index
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only.
 *
 */
static void
_bcm_esw_stat_flex_check_ingress_table(
    int       unit,
    soc_mem_t ingress_table,
    int       min_index,
    int       max_index)
{
    uint32            offset_mode=0;
    uint32            pool_number=0;
    uint32            base_idx=0;
    uint32            ingress_entry_data_size=0;
    void              *ingress_entry_data=NULL;
    void              *ingress_entry_full_data=NULL;
    bcm_stat_object_t object=bcmStatObjectIngPort;
    int               index=0;
    int               pipe_num = 0;
    uint32            allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_NONE;
    int               pool_group_id;
    uint32            enable, used_by_table = 0;
    uint32            counter_pool_bmap = 0; /* To enable counters */
    uint32            total_counters = 0;

    if (!((ingress_table == PORT_TABm) ||
          (ingress_table == VLAN_XLATEm)  ||
#if defined(BCM_TRIUMPH3_SUPPORT)
          (ingress_table == VLAN_XLATE_EXTDm)  ||
          (ingress_table == MPLS_ENTRY_EXTDm)  ||
          (ingress_table == EXT_FP_POLICYm)    ||
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
          (ingress_table == ING_VSANm)  ||
          (ingress_table == L3_ENTRY_IPV4_MULTICASTm)  ||
          (ingress_table == L3_ENTRY_IPV6_MULTICASTm)  ||
          (ingress_table == L3_DEFIPm) ||
          (ingress_table == L3_DEFIP_PAIR_128m)  ||
          (ingress_table == L3_DEFIP_ALPM_IPV4_1m) ||
          (ingress_table == L3_DEFIP_ALPM_IPV6_64_1m)  ||
          (ingress_table == L3_DEFIP_ALPM_IPV6_128m) ||
          (ingress_table == FP_POLICY_TABLEm)  ||
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined (BCM_TOMAHAWK_SUPPORT)
          (ingress_table == IFP_POLICY_TABLEm)  ||
          ((SOC_IS_TOMAHAWKX(unit)) &&
            ((ingress_table == IFP_POLICY_TABLE_PIPE0m)  ||
            (ingress_table == IFP_POLICY_TABLE_PIPE1m)  ||
            (ingress_table == IFP_POLICY_TABLE_PIPE2m)  ||
            (ingress_table == IFP_POLICY_TABLE_PIPE3m)  ||
            (ingress_table == VFP_POLICY_TABLE_PIPE0m)  ||
            (ingress_table == VFP_POLICY_TABLE_PIPE1m)  ||
            (ingress_table == VFP_POLICY_TABLE_PIPE2m)  ||
            (ingress_table == VFP_POLICY_TABLE_PIPE3m)  ||
            (ingress_table == AGM_MONITOR_TABLEm)))  ||
#endif
          (ingress_table == VFP_POLICY_TABLEm)  ||
          (ingress_table == MPLS_ENTRYm)  ||
          (ingress_table == SOURCE_VPm)  ||
          (ingress_table == L3_IIFm)  ||
          (ingress_table == VRFm)  ||
          (ingress_table == VFIm)  ||
          (ingress_table == VLAN_TABm))) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Invalid Flex Counter Ingress Memory %s\n"),
                      SOC_MEM_UFNAME(unit, ingress_table)));
           return;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
#endif

    if (!SOC_MEM_IS_VALID(unit, ingress_table)) {
        /* This check is for low latency mode */
        return; 
    }
    if (soc_mem_index_max(unit, ingress_table) < 0) {
        return;
    }

    ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                              SOC_MEM_INFO(unit, ingress_table).bytes));
    if (max_index > soc_mem_index_max(unit,ingress_table)) {
        max_index = soc_mem_index_max(unit,ingress_table);
    }

    ingress_entry_full_data = soc_cm_salloc(unit,ingress_entry_data_size *
                                  (max_index - min_index + 1),"ingress_table");
    if (ingress_entry_full_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, ingress_table)));
        return ;
    }
    sal_memset(ingress_entry_full_data,0,(ingress_entry_data_size *
                                  (max_index - min_index + 1)));

    if (soc_mem_read_range(unit, ingress_table, MEM_BLOCK_ANY,
                min_index,max_index, ingress_entry_full_data) != SOC_E_NONE) {
        soc_cm_sfree(unit,ingress_entry_full_data);
        return ;
    }

    BCM_STAT_FLEX_COUNTER_LOCK(unit);

   for(index=0; index <= (max_index - min_index) ; index++) {
       bcm_error_t rv = BCM_E_NONE;
       ingress_entry_data = soc_mem_table_idx_to_pointer(unit, ingress_table,
                            void *, ingress_entry_full_data, index);
        if (soc_mem_field_valid(unit,ingress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data,
                VALIDf)==0) {
                continue;
            }
        }
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit)) {
            if (soc_mem_field_valid(unit,ingress_table,VALID_0f)) {
                if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data,
                    VALID_0f)==0) {
                    continue;
                }
            }
            if (soc_mem_field_valid(unit,ingress_table,VALID_1f)) {
                if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data,
                    VALID_1f)==0) {
                    continue;
                }
            }
        }
#endif
        rv = _bcm_esw_get_flex_counter_fields_values(
               unit,index+min_index,ingress_table,ingress_entry_data,0,
               &offset_mode,&pool_number,&base_idx);
        if (rv != BCM_E_NONE) {
            continue;
        }
        if ((offset_mode == 0) && (base_idx == 0)) {
             continue;
        }
        /* object for these two tables rely on virtual module which is not
         * initialized yet, For the purpose here, just need to find any
         * valid object using the table
         */
        if (ingress_table == SOURCE_VPm) {
            object = bcmStatObjectIngMplsVcLabel;
        } else if (ingress_table == VFIm) {
            object = bcmStatObjectIngVfi;
        } else if (_bcm_esw_stat_flex_get_ingress_object(
            unit,ingress_table,index+min_index,ingress_entry_data,&object) !=BCM_E_NONE) {
            continue;
        }
        flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number][base_idx]++;
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TRIDENT2X(unit) &&
            soc_feature(unit, soc_feature_advanced_flex_counter)) {
            _flex_stat_mode_base_map[unit][bcmStatFlexDirectionIngress]
                                  [pool_number][offset_mode][base_idx] = 1;
        }
#endif
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                           [pipe_num][pool_number][base_idx] == 1) {
            flex_ingress_modes[unit][offset_mode].reference_count++;
        }
#ifdef BCM_WARM_BOOT_SUPPORT
#if defined (BCM_TOMAHAWK_SUPPORT)
        if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_5) &&
            soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
            (_bcm_esw_is_fp_table(ingress_table))) {
            uint32 cmode = 0;
            bcm_error_t rv;
            uint32 stat_counter_id = 0;
 
            rv = _bcm_esw_custom_stat_group_id_retrieve(unit, offset_mode, pipe_num, 
                                                        pool_number, base_idx,
                                                        &stat_counter_id);
            if (rv !=BCM_E_NONE) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Invalid mode value %d %d %d\n"),
                       offset_mode, pool_number, base_idx));
                total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
            } else {
                cmode = stat_counter_map[unit][stat_counter_id].mode;
                total_counters = flex_custom_ingress_modes[unit][cmode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
            }
        } else
#endif
#endif
        {
            if (flex_ingress_modes[unit][offset_mode].total_counters == 0) {
                flex_ingress_modes[unit][offset_mode].total_counters =
                       _bcm_esw_stat_flex_retrieve_total_counters(
                        unit, bcmStatFlexDirectionIngress,
                        pool_number, offset_mode)  ;
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Max_offset_table_value %d\n"),
                           flex_ingress_modes[unit][offset_mode].
                           total_counters));
            }
            total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
        }

        if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
            pool_group_id = pool_number/_bcm_esw_get_max_pool_groups(unit);
            /* Update the Flex flex_pool_group for oam_lm  for TD2Plus*/
            enable = 0;
            if (!BCM_FAILURE(_bcm_esw_stat_flex_ingress_group_oam_status
                                              (unit, pool_group_id,
                                              &enable))) {
                if (enable ) {
                    allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OAM;
                } else {                                      
                    allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OTHER;
                }                                             
                /* Update the pool_group type */      
                flex_pool_group[unit][pipe_num][pool_group_id].group_type 
                                                     = allocate_group_type;
            }
            if (flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [0][pool_number].used_by_tables == 0) {
                flex_pool_group[unit][pipe_num][pool_group_id].free_pools--;
            }
        }
        if (shr_aidxres_list_elem_state(flex_aidxres_list_handle[unit]
                                         [bcmStatFlexDirectionIngress]
                                         [pipe_num]
                                         [pool_number],
                                         base_idx) != BCM_E_EXISTS) {
            if (total_counters) {
                shr_aidxres_list_reserve_block(
                        flex_aidxres_list_handle[unit][bcmStatFlexDirectionIngress]
                        [pipe_num]
                        [pool_number],
                        base_idx,
                        total_counters);
                flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                    [pool_number].used_entries +=
                    total_counters;
            }
        }
        switch(ingress_table) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT) || defined(BCM_APACHE_SUPPORT)
            case FP_POLICY_TABLEm:
                 used_by_table = FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
                 break;
#endif
#if defined(BCM_TOMAHAWK_SUPPORT)
            case VFP_POLICY_TABLEm:
            case VFP_POLICY_TABLE_PIPE0m:
            case VFP_POLICY_TABLE_PIPE1m:
            case VFP_POLICY_TABLE_PIPE2m:
            case VFP_POLICY_TABLE_PIPE3m:
                 used_by_table = FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE;
                 break;
            case IFP_POLICY_TABLEm:
            case IFP_POLICY_TABLE_PIPE0m:
            case IFP_POLICY_TABLE_PIPE1m:
            case IFP_POLICY_TABLE_PIPE2m:
            case IFP_POLICY_TABLE_PIPE3m:
                 used_by_table = FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
                 break;
#endif
            default:
                 used_by_table = flex_used_by_table[object];
                 break;
        }
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                      [pool_number].attached_entries +=
                      total_counters;
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                      used_by_tables |= used_by_table;
        SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                      [pool_number].used_by_objects, object);
        if (pipe_num != 0) {
            flex_pool_stat[unit][bcmStatFlexDirectionIngress][0][pool_number].
                          used_by_tables |= used_by_table;
            SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress][0]
                          [pool_number].used_by_objects, object);
        }

        /* Set pool bmap to enable Counters to perform DMA.
         * Used in Centralized Counter model
         */
        counter_pool_bmap |= 1 << pool_number;
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Table:%s:index=%d mode:%d pool_number:%d base_idx:%d\n"),
                   SOC_MEM_UFNAME(unit, ingress_table),
                   index+min_index,offset_mode,pool_number, base_idx));
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit)) {
            if ((IFP_POLICY_TABLEm == ingress_table) ||
                (IFP_POLICY_TABLE_PIPE0m == ingress_table) ||
                (IFP_POLICY_TABLE_PIPE1m == ingress_table) ||
                (IFP_POLICY_TABLE_PIPE2m == ingress_table) ||
                (IFP_POLICY_TABLE_PIPE3m == ingress_table)) {

                uint32 count, index;
                _bcm_ifp_color_map_t  color_map;
                soc_field_t color_fields[BCM_MAX_IFP_COLORS] = {G_COUNTf,
                                                                Y_COUNTf,
                                                                R_COUNTf};
                uint8 color_map_mask[BCM_MAX_IFP_COLORS] = {BCM_STAT_FLEX_COLOR_GREEN,
                                                            BCM_STAT_FLEX_COLOR_YELLOW,
                                                            BCM_STAT_FLEX_COLOR_RED};
                bcm_error_t rv;
                sal_memset(&color_map, 0, sizeof(_bcm_ifp_color_map_t));
                for (index = 0; index < BCM_MAX_IFP_COLORS; index++) {
                     soc_IFP_POLICY_TABLEm_field_get(unit, ingress_entry_data,
                                                     color_fields[index], &count);
                    if (count > 0 && count <= BCM_MAX_IFP_COLOR_COUNTERS) {
                        /* count -1 is used to index the color_map array because
                         * the value 0 is reserved by hardware to indicate "do
                         * not count"
                         */
                        color_map.map[count - 1] |= color_map_mask[index];
                    }
                }
                rv = _bcm_esw_ifp_wb_color_map_set(unit, pool_number, base_idx, pipe_num, offset_mode, &color_map);
                if (BCM_FAILURE(rv)) {
                    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Unable to reconstruct ifp color map for"
                                          " policy table index - %d, pool - %d"
                                          " offset_mode - %d, base idx - %d"),
                               index + min_index, pool_number, offset_mode,
                               base_idx));
                }
            }
    }
#endif
    }

    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    soc_cm_sfree(unit,ingress_entry_full_data);

    if (counter_pool_bmap) {
        int rv = SOC_E_NONE;
        rv = soc_counter_pool_bmap_status_set(unit, bcmStatFlexDirectionIngress,
                                              counter_pool_bmap, 1);
        if (rv != SOC_E_NONE) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Err when Enabling Pools for Counter DMA. "
                                 "Ingress Pool Bmap 0x%x\n"),
                      counter_pool_bmap));
        }
    }
    return;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_init_uncmprsd_mode
 * Description:
 *      Configures flex hw for uncompressed mode
 * Parameters:
 *      unit                           - (IN) unit number
 *      mode                           - (IN) Flex Offset Mode
 *      direction                      - (IN) Flex Data flow direction(ing/egr)
 *      selector_count                 - (IN) Flex Key selector count
 *      selector_x_en_field_value      - (IN) Flex Key selector fields
 *      selector_for_bit_x_field_value - (IN) Flex Key selector field bits
 *
 * Return Value:
 *      None
 * Notes:
 *
 */
static void
_bcm_esw_stat_flex_init_uncmprsd_mode(
    int                       unit,
    int                       mode,
    bcm_stat_flex_direction_t direction,
    uint32                    selector_count,
    uint32                    selector_x_en_field_value[8],
    uint32                    selector_for_bit_x_field_value[8])
{
    uint32  uncmprsd_attr_bits_selector=0;
    uint32  index=0;

    if (direction == bcmStatFlexDirectionIngress) {
        flex_ingress_modes[unit][mode].available=1;
        /*Will be changed WhileReadingTables */
        flex_ingress_modes[unit][mode].reference_count=0;
        /* selector_count can not be used for deciding total counters !!! */
        /* flex_ingress_modes[unit][mode].total_counters=(1<< selector_count);*/
        flex_ingress_modes[unit][mode].ing_attr.packet_attr_type =
            bcmStatFlexPacketAttrTypeUncompressed;
        for (uncmprsd_attr_bits_selector=0,index=0;index<8;index++) {
             if(selector_x_en_field_value[index] != 0) {
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.ip_pkt_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_uncmprsd_bits_g.ip_pkt_pos+
                       ing_pkt_attr_uncmprsd_bits_g.ip_pkt-1))) { /* 0..0 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_IP_PKT_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.drop_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_uncmprsd_bits_g.drop_pos+
                       ing_pkt_attr_uncmprsd_bits_g.drop-1))) { /* 1..1 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_DROP_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.svp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_uncmprsd_bits_g.svp_type_pos+
                       ing_pkt_attr_uncmprsd_bits_g.svp_type-1))) { /* 3..8 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.pkt_resolution_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_uncmprsd_bits_g.pkt_resolution_pos+
                       ing_pkt_attr_uncmprsd_bits_g.pkt_resolution-1))) { /* 3..8 */
                     uncmprsd_attr_bits_selector|=
                   BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.tos_dscp_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.tos_dscp_pos+ing_pkt_attr_uncmprsd_bits_g.tos_dscp-1))) {
                     /* 9..16 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.tos_ecn_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.tos_ecn_pos+ing_pkt_attr_uncmprsd_bits_g.tos_ecn-1))) {
                     /* 9..16 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_TOS_ECN_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.ing_port_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.ing_port_pos+
                      ing_pkt_attr_uncmprsd_bits_g.ing_port-1))) { /* 17 .. 22 */
                     uncmprsd_attr_bits_selector|=
                      BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INGRESS_PORT_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.inner_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.inner_dot1p_pos +
                      ing_pkt_attr_uncmprsd_bits_g.inner_dot1p - 1))) { /* 23..25 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INNER_DOT1P_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.outer_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.outer_dot1p_pos +
                      ing_pkt_attr_uncmprsd_bits_g.outer_dot1p - 1))) { /* 26..28 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.vlan_format_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.vlan_format_pos +
                      ing_pkt_attr_uncmprsd_bits_g.vlan_format - 1))) { /* 29..30 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_VLAN_FORMAT_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.int_pri_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.int_pri_pos +
                      ing_pkt_attr_uncmprsd_bits_g.int_pri -1))) { /* 31 .. 34 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.ifp_cng_pos ) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.ifp_cng_pos +
                     ing_pkt_attr_uncmprsd_bits_g.ifp_cng - 1))) { /* 35..36 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_IFP_CNG_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_uncmprsd_bits_g.cng_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (ing_pkt_attr_uncmprsd_bits_g.cng_pos +
                      ing_pkt_attr_uncmprsd_bits_g.cng - 1))) { /* 37 .. 38 */
                     uncmprsd_attr_bits_selector|=
                       BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_ATTR_BITS;
                     continue;
                }
#ifdef BCM_APACHE_SUPPORT
                if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.phb_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.phb_1_pos +
                              ing_pkt_attr_uncmprsd_bits_g.phb_1 - 1))) { /* 42 .. 45 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PHB_1_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.cng_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.cng_1_pos +
                              ing_pkt_attr_uncmprsd_bits_g.cng_1 - 1))) { /* 46 .. 47 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_1_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.phb_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.phb_2_pos +
                              ing_pkt_attr_uncmprsd_bits_g.phb_2 - 1))) { /* 48 .. 51 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PHB_2_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.cng_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.cng_2_pos +
                              ing_pkt_attr_uncmprsd_bits_g.cng_2 - 1))) { /* 52 .. 53 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_2_ATTR_BITS;
                        continue ;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.phb_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.phb_3_pos +
                              ing_pkt_attr_uncmprsd_bits_g.phb_3 - 1))) { /* 54 .. 57 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_PHB_3_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                ing_pkt_attr_uncmprsd_bits_g.cng_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (ing_pkt_attr_uncmprsd_bits_g.cng_3_pos +
                              ing_pkt_attr_uncmprsd_bits_g.cng_3 - 1))) { /* 58 .. 59 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_ING_UNCOMPRESSED_USE_CNG_3_ATTR_BITS;
                        continue;
                    }
                }
#endif
             }
        }
        flex_ingress_modes[unit][mode].ing_attr.uncmprsd_attr_selectors.
                     uncmprsd_attr_bits_selector = uncmprsd_attr_bits_selector;
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "uncmprsd_attr_bits_selector:%x \n"),
                   uncmprsd_attr_bits_selector));
        return;
    }
    if (direction == bcmStatFlexDirectionEgress) {
        flex_egress_modes[unit][mode].available=1;
        /* Will be changed WhileReadingTables */
        flex_egress_modes[unit][mode].reference_count=0;
        flex_egress_modes[unit][mode].egr_attr.packet_attr_type =
                                      bcmStatFlexPacketAttrTypeUncompressed;
        for (uncmprsd_attr_bits_selector=0,index=0;index<8;index++) {
             if(selector_x_en_field_value[index] != 0) {
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.ip_pkt_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_uncmprsd_bits_g.ip_pkt_pos+
                       egr_pkt_attr_uncmprsd_bits_g.ip_pkt-1))) { /* 0..0 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_IP_PKT_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.drop_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_uncmprsd_bits_g.drop_pos+
                       egr_pkt_attr_uncmprsd_bits_g.drop-1))) { /* 1..1 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_DROP_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.dvp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_uncmprsd_bits_g.dvp_type_pos+
                       egr_pkt_attr_uncmprsd_bits_g.dvp_type-1))) { /* 3..8 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_DVP_TYPE_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.svp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_uncmprsd_bits_g.svp_type_pos+
                       egr_pkt_attr_uncmprsd_bits_g.svp_type-1))) { /* 3..8 */
                    uncmprsd_attr_bits_selector |=
                      BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_SVP_TYPE_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.pkt_resolution_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_uncmprsd_bits_g.pkt_resolution_pos+
                       egr_pkt_attr_uncmprsd_bits_g.pkt_resolution-1))) { /* 4..4 */
                    uncmprsd_attr_bits_selector |=
                    BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PKT_RESOLUTION_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.tos_dscp_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.tos_dscp_pos+egr_pkt_attr_uncmprsd_bits_g.tos_dscp-1))) {
                     /* 5..12 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_TOS_DSCP_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.tos_ecn_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.tos_ecn_pos+egr_pkt_attr_uncmprsd_bits_g.tos_ecn-1))) {
                     /* 5..12 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_TOS_ECN_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.egr_port_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.egr_port_pos+
                      egr_pkt_attr_uncmprsd_bits_g.egr_port-1))) { /* 13..18 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_EGRESS_PORT_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.inner_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.inner_dot1p_pos+
                      egr_pkt_attr_uncmprsd_bits_g.inner_dot1p-1))) { /* 19..21*/
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INNER_DOT1P_ATTR_BITS;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.outer_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.outer_dot1p_pos+
                      egr_pkt_attr_uncmprsd_bits_g.outer_dot1p-1))) { /* 22..24*/
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_OUTER_DOT1P_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.vlan_format_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.vlan_format_pos+
                      egr_pkt_attr_uncmprsd_bits_g.vlan_format-1))) { /* 25..26 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_VLAN_FORMAT_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.int_pri_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.int_pri_pos +
                      egr_pkt_attr_uncmprsd_bits_g.int_pri - 1))) { /* 27..30 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_INT_PRI_ATTR_BITS;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_uncmprsd_bits_g.cng_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                     (egr_pkt_attr_uncmprsd_bits_g.cng_pos +
                      egr_pkt_attr_uncmprsd_bits_g.cng - 1))) { /* 31..32 */
                     uncmprsd_attr_bits_selector |=
                       BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_ATTR_BITS;
                     continue;
                }
#ifdef BCM_APACHE_SUPPORT
                if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.phb_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.phb_1_pos +
                              egr_pkt_attr_uncmprsd_bits_g.phb_1 - 1))) { /* 42 .. 45 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PHB_1_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.cng_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.cng_1_pos +
                              egr_pkt_attr_uncmprsd_bits_g.cng_1 - 1))) { /* 46 .. 47 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_1_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.phb_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.phb_2_pos +
                              egr_pkt_attr_uncmprsd_bits_g.phb_2 - 1))) { /* 48 .. 51 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PHB_2_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.cng_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.cng_2_pos +
                              egr_pkt_attr_uncmprsd_bits_g.cng_2 - 1))) { /* 52 .. 53 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_2_ATTR_BITS;
                        continue ;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.phb_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.phb_3_pos +
                              egr_pkt_attr_uncmprsd_bits_g.phb_3 - 1))) { /* 54 .. 57 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_PHB_3_ATTR_BITS;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_uncmprsd_bits_g.cng_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_uncmprsd_bits_g.cng_3_pos +
                              egr_pkt_attr_uncmprsd_bits_g.cng_3 - 1))) { /* 58 .. 59 */
                        uncmprsd_attr_bits_selector |=
                            BCM_STAT_FLEX_EGR_UNCOMPRESSED_USE_CNG_3_ATTR_BITS;
                        continue;
                    }
                }
#endif
             }
        }
        flex_egress_modes[unit][mode].egr_attr.uncmprsd_attr_selectors.
                    uncmprsd_attr_bits_selector = uncmprsd_attr_bits_selector;
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "uncmprsd_attr_bits_selector:%x \n"),
                   uncmprsd_attr_bits_selector));
        return;
    }
    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Ooops. Control Must not reach over here \n")));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_init_cmprsd_mode
 * Description:
 *      Configures flex hw for compressed mode
 * Parameters:
 *      unit                           - (IN) unit number
 *      mode                           - (IN) Flex Offset Mode
 *      direction                      - (IN) Flex Data flow direction(ing/egr)
 *      selector_count                 - (IN) Flex Key selector count
 *      selector_x_en_field_value      - (IN) Flex Key selector fields
 *      selector_for_bit_x_field_value - (IN) Flex Key selector field bits
 *
 * Return Value:
 *      None
 * Notes:
 *
 */
static void
_bcm_esw_stat_flex_init_cmprsd_mode(
    int                       unit,
    int                       mode,
    bcm_stat_flex_direction_t direction,
    uint32                    selector_count,
    uint32                    selector_x_en_field_value[8],
    uint32                    selector_for_bit_x_field_value[8])
{
    uint32  index=0;

    uint32                            alloc_size=0;
    uint32                            map_value=0;
    ing_flex_ctr_pkt_pri_map_entry_t  *ing_pkt_pri_map_dma=NULL;
    ing_flex_ctr_pkt_res_map_entry_t  *ing_pkt_res_map_dma=NULL;
    ing_flex_ctr_port_map_entry_t     *ing_port_map_dma=NULL;
    ing_flex_ctr_pri_cng_map_entry_t  *ing_pri_cng_map_dma=NULL;
    ing_flex_ctr_tos_map_entry_t      *ing_tos_map_dma=NULL;
    egr_flex_ctr_pkt_pri_map_entry_t  *egr_pkt_pri_map_dma=NULL;
    egr_flex_ctr_pkt_res_map_entry_t  *egr_pkt_res_map_dma=NULL;
    egr_flex_ctr_port_map_entry_t     *egr_port_map_dma=NULL;
    egr_flex_ctr_pri_cng_map_entry_t  *egr_pri_cng_map_dma=NULL;
    egr_flex_ctr_tos_map_entry_t      *egr_tos_map_dma=NULL;

    if (direction == bcmStatFlexDirectionIngress) {
       /* memset(&flex_ingress_modes[unit][mode],0,
         sizeof(flex_ingress_modes[unit][mode])); */
       flex_ingress_modes[unit][mode].available=1;
       /*Will be changed WhileReadingTables */
       flex_ingress_modes[unit][mode].reference_count=0;
       flex_ingress_modes[unit][mode].total_counters= 0;
       flex_ingress_modes[unit][mode].ing_attr.packet_attr_type =
            bcmStatFlexPacketAttrTypeCompressed;
       for (index=0;index<8;index++) {
            if(selector_x_en_field_value[index] != 0) {
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.ip_pkt_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.ip_pkt_pos+
                       ing_pkt_attr_cmprsd_bits_g.ip_pkt-1))) { /* 0..0 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.ip_pkt = 1;
                   continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.drop_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.drop_pos+
                       ing_pkt_attr_cmprsd_bits_g.drop-1))) { /* 1..1 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.drop = 1;
                   continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.svp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.svp_type_pos+
                       ing_pkt_attr_cmprsd_bits_g.svp_type-1))) { /* 1..1 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.svp_type = 1;
                   continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.pkt_resolution_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.pkt_resolution_pos+
                       ing_pkt_attr_cmprsd_bits_g.pkt_resolution-1))) { /* 3..8 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.pkt_resolution++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.tos_dscp_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.tos_dscp_pos+
                       ing_pkt_attr_cmprsd_bits_g.tos_dscp-1))) { /* 9..16 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.tos_dscp++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.tos_ecn_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.tos_ecn_pos+
                       ing_pkt_attr_cmprsd_bits_g.tos_ecn-1))) { /* 9..16 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.tos_ecn++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.ing_port_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.ing_port_pos+
                       ing_pkt_attr_cmprsd_bits_g.ing_port-1))) { /* 17..22 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.ing_port++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.inner_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.inner_dot1p_pos+
                       ing_pkt_attr_cmprsd_bits_g.inner_dot1p-1))) { /* 23..25 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.inner_dot1p++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.outer_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.outer_dot1p_pos+
                       ing_pkt_attr_cmprsd_bits_g.outer_dot1p-1))) { /* 26..28 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.outer_dot1p++;
                   continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.vlan_format_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.vlan_format_pos+
                       ing_pkt_attr_cmprsd_bits_g.vlan_format-1))) { /* 29..30 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.vlan_format++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.int_pri_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.int_pri_pos+
                       ing_pkt_attr_cmprsd_bits_g.int_pri-1))) { /* 31..34 */
                    flex_ingress_modes[unit][mode].ing_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.int_pri++;
                    continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.ifp_cng_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.ifp_cng_pos+
                       ing_pkt_attr_cmprsd_bits_g.ifp_cng-1))) { /* 35..36 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.ifp_cng++;
                   continue;
               }
               if ((selector_for_bit_x_field_value[index] >=
                     ing_pkt_attr_cmprsd_bits_g.cng_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (ing_pkt_attr_cmprsd_bits_g.cng_pos+
                       ing_pkt_attr_cmprsd_bits_g.cng-1))) { /* 37..38 */
                   flex_ingress_modes[unit][mode].ing_attr.
                    cmprsd_attr_selectors.pkt_attr_bits.cng++;
                   continue;
               }
#ifdef BCM_APACHE_SUPPORT
               if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.phb_1_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.phb_1_pos +
                             ing_pkt_attr_cmprsd_bits_g.phb_1 - 1))) { /* 42 .. 45 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.phb_1++;
                       continue;
                   }
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.cng_1_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.cng_1_pos +
                             ing_pkt_attr_cmprsd_bits_g.cng_1 - 1))) { /* 46 .. 47 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.cng_1++;
                       continue;
                   }
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.phb_2_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.phb_2_pos +
                             ing_pkt_attr_cmprsd_bits_g.phb_2 - 1))) { /* 48 .. 51 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.phb_2++;
                       continue;
                   }
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.cng_2_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.cng_2_pos +
                             ing_pkt_attr_cmprsd_bits_g.cng_2 - 1))) { /* 52 .. 53 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.cng_2++;
                       continue ;
                   }
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.phb_3_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.phb_3_pos +
                             ing_pkt_attr_cmprsd_bits_g.phb_3 - 1))) { /* 54 .. 57 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.phb_3++;
                       continue;
                   }
                   if ((selector_for_bit_x_field_value[index] >=
                               ing_pkt_attr_cmprsd_bits_g.cng_3_pos) &&
                           (selector_for_bit_x_field_value[index] <=
                            (ing_pkt_attr_cmprsd_bits_g.cng_3_pos +
                             ing_pkt_attr_cmprsd_bits_g.cng_3 - 1))) { /* 58 .. 59 */
                       flex_ingress_modes[unit][mode].ing_attr.
                           cmprsd_attr_selectors.pkt_attr_bits.cng_3++;
                       continue;
                   }
               }
#endif
           }
       }
#ifdef BCM_APACHE_SUPPORT
       if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.cng_3 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   cng_3 = ing_pkt_attr_cmprsd_bits_g.cng_3;
           }
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.phb_3 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   phb_3 = ing_pkt_attr_cmprsd_bits_g.phb_3;
           }
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.cng_2 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   cng_2 = ing_pkt_attr_cmprsd_bits_g.cng_2;
           }
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.phb_2 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   phb_2 = ing_pkt_attr_cmprsd_bits_g.phb_2;
           }
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.cng_1 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   cng_1 = ing_pkt_attr_cmprsd_bits_g.cng_1;
           }
           if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   pkt_attr_bits.phb_1 ) {
               flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                   phb_1 = ing_pkt_attr_cmprsd_bits_g.phb_1;
           }
       }
#endif
       if ((flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.cng ) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.ifp_cng ) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.int_pri ))         {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "cng:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.cng));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ifp_cng:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.ifp_cng));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "int_pri:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.int_pri));
            alloc_size = soc_mem_index_count(unit,ING_FLEX_CTR_PRI_CNG_MAPm) *
                         sizeof(ing_flex_ctr_pri_cng_map_entry_t);
            ing_pri_cng_map_dma = soc_cm_salloc( unit, alloc_size,
                                                 "ING_FLEX_CTR_PRI_CNG_MAPm");
            if (ing_pri_cng_map_dma == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "ING_FLEX_CTR_PRI_CNG_MAPm:DMAAllocationFail\n")));
                return;
            }
            sal_memset(ing_pri_cng_map_dma, 0, alloc_size);
            if (soc_mem_read_range(
                    unit,
                    ING_FLEX_CTR_PRI_CNG_MAPm,
                    MEM_BLOCK_ANY,
                    soc_mem_index_min(unit,ING_FLEX_CTR_PRI_CNG_MAPm),
                    soc_mem_index_max(unit,ING_FLEX_CTR_PRI_CNG_MAPm),
                    ing_pri_cng_map_dma) != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "ING_FLEX_CTR_PRI_CNG_MAPm:Read failuer \n")));
                soc_cm_sfree(unit,ing_pri_cng_map_dma);
                return;
            }
            for (index=0;
                 index< soc_mem_index_count(unit,ING_FLEX_CTR_PRI_CNG_MAPm);
                 index++) {
                 soc_mem_field_get( unit, ING_FLEX_CTR_PRI_CNG_MAPm,
                      (uint32 *)&ing_pri_cng_map_dma[index], PRI_CNG_FNf,
                      &map_value);
                 flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                                   pri_cnf_attr_map[index]=(uint8)map_value;
                 if (map_value) {
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Index:%dValue:%d\n"),
                                index,map_value));
                 }
            }
            soc_cm_sfree(unit,ing_pri_cng_map_dma);
       }
       if ((flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.vlan_format) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.outer_dot1p) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.inner_dot1p)) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "vlan_format:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.vlan_format));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "outer_dot1p:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.outer_dot1p));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "inner_dot1p:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.inner_dot1p));
            alloc_size = soc_mem_index_count(unit,ING_FLEX_CTR_PKT_PRI_MAPm) *
                         sizeof(ing_flex_ctr_pkt_pri_map_entry_t);
            ing_pkt_pri_map_dma = soc_cm_salloc( unit, alloc_size,
                                                 "ING_FLEX_CTR_PKT_PRI_MAPm");
            if (ing_pkt_pri_map_dma == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "ING_FLEX_CTR_PKT_PRI_MAPm:DMAAllocationFail\n")));
                return;
            }
            sal_memset(ing_pkt_pri_map_dma, 0, alloc_size);
            if (soc_mem_read_range(
                    unit,
                    ING_FLEX_CTR_PKT_PRI_MAPm,
                    MEM_BLOCK_ANY,
                    soc_mem_index_min(unit,ING_FLEX_CTR_PKT_PRI_MAPm),
                    soc_mem_index_max(unit,ING_FLEX_CTR_PKT_PRI_MAPm),
                    ing_pkt_pri_map_dma) != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "ING_FLEX_CTR_PKT_PRI_MAPm:Read failuer \n")));
                soc_cm_sfree(unit,ing_pkt_pri_map_dma);
                return;
            }
            for (index=0;
                 index< soc_mem_index_count(unit,ING_FLEX_CTR_PKT_PRI_MAPm);
                 index++) {
                 soc_mem_field_get( unit, ING_FLEX_CTR_PKT_PRI_MAPm,
                      (uint32 *)&ing_pkt_pri_map_dma[index], PKT_PRI_FNf,
                      &map_value);
                 flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                                   pkt_pri_attr_map[index]=(uint8)map_value;
                 if (map_value){
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Index:%dValue:%d\n"),
                                index,map_value));
                 }
            }
            soc_cm_sfree(unit,ing_pkt_pri_map_dma);
       }
       if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
           pkt_attr_bits.ing_port) {
           LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "ing_port:%d \n"),
                      flex_ingress_modes[unit][mode].
                      ing_attr.cmprsd_attr_selectors.
                      pkt_attr_bits.ing_port));
           alloc_size = soc_mem_index_count(unit,ING_FLEX_CTR_PORT_MAPm) *
                         sizeof(ing_flex_ctr_port_map_entry_t);
           ing_port_map_dma = soc_cm_salloc( unit, alloc_size,
                                             "ING_FLEX_CTR_PORT_MAPm");
           if (ing_port_map_dma == NULL) {
               LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "ING_FLEX_CTR_PORT_MAPm:DMA Allocation failuer\n")));
               return;
           }
           sal_memset(ing_port_map_dma, 0, alloc_size);
           if (soc_mem_read_range(
                   unit,
                   ING_FLEX_CTR_PORT_MAPm,
                   MEM_BLOCK_ANY,
                   soc_mem_index_min(unit,ING_FLEX_CTR_PORT_MAPm),
                   soc_mem_index_max(unit,ING_FLEX_CTR_PORT_MAPm),
                   ing_port_map_dma) != SOC_E_NONE) {
               LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "ING_FLEX_CTR_PORT_MAPm:Read failuer \n")));
               soc_cm_sfree(unit,ing_port_map_dma);
               return;
           }
           for (index=0;
                index< soc_mem_index_count(unit,ING_FLEX_CTR_PORT_MAPm);
                index++) {
                soc_mem_field_get( unit, ING_FLEX_CTR_PORT_MAPm,
                      (uint32 *)&ing_port_map_dma[index], PRI_CNG_FNf,
                      &map_value);
                flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                                  port_attr_map[index]=(uint8)map_value;
                if (map_value) {
                    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Index:%dValue:%d\n"),
                               index,map_value));
                }
           }
           soc_cm_sfree(unit,ing_port_map_dma);
       }
       if ((flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.tos_dscp ) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
            pkt_attr_bits.tos_ecn)) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "tos_dscp:%d tos_ecn:%d \n"),
                      flex_ingress_modes[unit][mode].ing_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.tos_dscp,
                      flex_ingress_modes[unit][mode].ing_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.tos_ecn));
           alloc_size = soc_mem_index_count(unit,ING_FLEX_CTR_TOS_MAPm) *
                        sizeof(ing_flex_ctr_tos_map_entry_t);
           ing_tos_map_dma = soc_cm_salloc( unit, alloc_size,
                                            "ING_FLEX_CTR_TOS_MAPm");
           if (ing_tos_map_dma == NULL) {
               LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "ING_FLEX_CTR_TOS_MAP:DMA Allocation failuer \n")));
               return;
           }
           sal_memset(ing_tos_map_dma, 0, alloc_size);
           if(soc_mem_read_range(
                  unit,
                  ING_FLEX_CTR_TOS_MAPm,
                  MEM_BLOCK_ANY,
                  soc_mem_index_min(unit,ING_FLEX_CTR_TOS_MAPm),
                  soc_mem_index_max(unit,ING_FLEX_CTR_TOS_MAPm),
                  ing_tos_map_dma) != SOC_E_NONE) {
              LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "ING_FLEX_CTR_TOS_MAPm:Read failuer \n")));
              soc_cm_sfree(unit,ing_tos_map_dma);
              return;
           }
           for (index=0;
                index< soc_mem_index_count(unit,ING_FLEX_CTR_TOS_MAPm);
                index++) {
                soc_mem_field_get( unit, ING_FLEX_CTR_TOS_MAPm,
                      (uint32 *)&ing_tos_map_dma[index], TOS_FNf,
                      &map_value);
                flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                                   tos_attr_map[index]=(uint8)map_value;
                if (map_value) {
                   LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                             (BSL_META_U(unit,
                                         "Index:%dValue:%d\n"),
                              index,map_value));
                }
            }
            soc_cm_sfree(unit,ing_tos_map_dma);
       }
       if ((flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
             pkt_attr_bits.pkt_resolution) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
             pkt_attr_bits.svp_type) ||
           (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
             pkt_attr_bits.drop)) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "pkt_resolution:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.pkt_resolution));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "svp_type:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.svp_type));
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "drop:%d \n"),
                       flex_ingress_modes[unit][mode].ing_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.drop));
            alloc_size = soc_mem_index_count(unit,ING_FLEX_CTR_PKT_RES_MAPm) *
                         sizeof(ing_flex_ctr_pkt_res_map_entry_t);
            ing_pkt_res_map_dma = soc_cm_salloc( unit, alloc_size,
                                                 "ING_FLEX_CTR_PKT_RES_MAPm");
            if (ing_pkt_res_map_dma == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "ING_FLEX_CTR_PKT_RES_MAPm:DMA AllocationFail\n")));
                return;
            }
            sal_memset(ing_pkt_res_map_dma, 0, alloc_size);
            if(soc_mem_read_range(
                   unit,
                   ING_FLEX_CTR_PKT_RES_MAPm,
                   MEM_BLOCK_ANY,
                   soc_mem_index_min(unit,ING_FLEX_CTR_PKT_RES_MAPm),
                   soc_mem_index_max(unit,ING_FLEX_CTR_PKT_RES_MAPm),
                   ing_pkt_res_map_dma) != SOC_E_NONE) {
               LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "ING_FLEX_CTR_PKT_RES_MAPm:Read failuer \n")));
               soc_cm_sfree(unit,ing_pkt_res_map_dma);
               return;
            }
            for(index=0;
                index< soc_mem_index_count(unit,ING_FLEX_CTR_PKT_RES_MAPm);
                index++) {
                soc_mem_field_get( unit, ING_FLEX_CTR_PKT_RES_MAPm,
                     (uint32 *)&ing_pkt_res_map_dma[index], PKT_RES_FNf,
                     &map_value);
                flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
                                   pkt_res_attr_map[index]=(uint8)map_value;
                if (map_value) {
                    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Index:%dValue:%d\n"),
                               index,map_value));
                }
            }
            soc_cm_sfree(unit,ing_pkt_res_map_dma);
       }
       if (flex_ingress_modes[unit][mode].ing_attr.cmprsd_attr_selectors.
           pkt_attr_bits.ip_pkt) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "ip_pkt:%d \n"),
                      flex_ingress_modes[unit][mode].ing_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.ip_pkt));
       }
       return;
    }
    if (direction == bcmStatFlexDirectionEgress) {
        flex_egress_modes[unit][mode].available=1;
        /*Will be changed WhileReadingTables */
        flex_egress_modes[unit][mode].reference_count=0;
        flex_egress_modes[unit][mode].egr_attr.packet_attr_type =
                                      bcmStatFlexPacketAttrTypeCompressed;
        for (index=0;index<8;index++) {
             if(selector_x_en_field_value[index] != 0) {
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.ip_pkt_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.ip_pkt_pos+
                       egr_pkt_attr_cmprsd_bits_g.ip_pkt-1))) { /* 0..0 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.ip_pkt = 1;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.drop_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.drop_pos+
                       egr_pkt_attr_cmprsd_bits_g.drop-1))) { /* 1..1 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.drop = 1;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.dvp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.dvp_type_pos+
                       egr_pkt_attr_cmprsd_bits_g.dvp_type-1))) { /* 2..2 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.dvp_type = 1;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.svp_type_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.svp_type_pos+
                       egr_pkt_attr_cmprsd_bits_g.svp_type-1))) { /* 3..3 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.svp_type = 1;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.pkt_resolution_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.pkt_resolution_pos+
                       egr_pkt_attr_cmprsd_bits_g.pkt_resolution-1))) { /* 4..4 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.
                    pkt_attr_bits.pkt_resolution = 1;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.tos_dscp_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.tos_dscp_pos+
                       egr_pkt_attr_cmprsd_bits_g.tos_dscp-1))) { /* 5..12 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.tos_dscp++;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.tos_ecn_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.tos_ecn_pos+
                       egr_pkt_attr_cmprsd_bits_g.tos_ecn-1))) { /* 5..12 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.tos_ecn++;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.egr_port_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.egr_port_pos+
                       egr_pkt_attr_cmprsd_bits_g.egr_port-1))) { /* 13..18 */
                    flex_egress_modes[unit][mode].egr_attr.
                     cmprsd_attr_selectors.pkt_attr_bits.egr_port++;
                    continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.inner_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.inner_dot1p_pos+
                       egr_pkt_attr_cmprsd_bits_g.inner_dot1p-1))) { /* 19..21 */
                     flex_egress_modes[unit][mode].egr_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.inner_dot1p++;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.outer_dot1p_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.outer_dot1p_pos+
                       egr_pkt_attr_cmprsd_bits_g.outer_dot1p-1))) { /* 22..24 */
                     flex_egress_modes[unit][mode].egr_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.outer_dot1p++;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.vlan_format_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.vlan_format_pos+
                       egr_pkt_attr_cmprsd_bits_g.vlan_format-1))) { /* 25..26 */
                     flex_egress_modes[unit][mode].egr_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.vlan_format++;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.int_pri_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.int_pri_pos+
                       egr_pkt_attr_cmprsd_bits_g.int_pri-1))) { /* 27..30 */
                     flex_egress_modes[unit][mode].egr_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.int_pri++;
                     continue;
                }
                if ((selector_for_bit_x_field_value[index] >=
                     egr_pkt_attr_cmprsd_bits_g.cng_pos) &&
                    (selector_for_bit_x_field_value[index] <=
                      (egr_pkt_attr_cmprsd_bits_g.cng_pos+
                       egr_pkt_attr_cmprsd_bits_g.cng-1))) { /* 31..32 */
                     flex_egress_modes[unit][mode].egr_attr.
                      cmprsd_attr_selectors.pkt_attr_bits.cng++;
                     continue;
                }
#ifdef BCM_APACHE_SUPPORT
                if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.phb_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.phb_1_pos +
                              egr_pkt_attr_cmprsd_bits_g.phb_1 - 1))) { /* 42 .. 45 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.phb_1++;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.cng_1_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.cng_1_pos +
                              egr_pkt_attr_cmprsd_bits_g.cng_1 - 1))) { /* 46 .. 47 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.cng_1++;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.phb_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.phb_2_pos +
                              egr_pkt_attr_cmprsd_bits_g.phb_2 - 1))) { /* 48 .. 51 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.phb_2++;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.cng_2_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.cng_2_pos +
                              egr_pkt_attr_cmprsd_bits_g.cng_2 - 1))) { /* 52 .. 53 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.cng_2++;
                        continue ;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.phb_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.phb_3_pos +
                              egr_pkt_attr_cmprsd_bits_g.phb_3 - 1))) { /* 54 .. 57 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.phb_3++;
                        continue;
                    }
                    if ((selector_for_bit_x_field_value[index] >=
                                egr_pkt_attr_cmprsd_bits_g.cng_3_pos) &&
                            (selector_for_bit_x_field_value[index] <=
                             (egr_pkt_attr_cmprsd_bits_g.cng_3_pos +
                              egr_pkt_attr_cmprsd_bits_g.cng_3 - 1))) { /* 58 .. 59 */
                        flex_egress_modes[unit][mode].egr_attr.
                            cmprsd_attr_selectors.pkt_attr_bits.cng_3++;
                        continue;
                    }
                }
#endif
            }
        }
#ifdef BCM_APACHE_SUPPORT
        if (soc_feature(unit,soc_feature_flex_ctr_mpls_3_label_count)) {
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.cng_3 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    cng_3 = egr_pkt_attr_cmprsd_bits_g.cng_3;
            }
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.phb_3 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    phb_3 = egr_pkt_attr_cmprsd_bits_g.phb_3;
            }
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.cng_2 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    cng_2 = egr_pkt_attr_cmprsd_bits_g.cng_2;
            }
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.phb_2 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    phb_2 = egr_pkt_attr_cmprsd_bits_g.phb_2;
            }
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.cng_1 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    cng_1 = egr_pkt_attr_cmprsd_bits_g.cng_1;
            }
            if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    pkt_attr_bits.phb_1 ) {
                flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                    phb_1 = egr_pkt_attr_cmprsd_bits_g.phb_1;
            }
        }
#endif
       if ((flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
              pkt_attr_bits.cng) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
              pkt_attr_bits.int_pri) ) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "cng:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.cng));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "int_pri:%d \n"),
                        flex_egress_modes[unit][mode].
                        egr_attr.cmprsd_attr_selectors.pkt_attr_bits.
                        int_pri));
             alloc_size = soc_mem_index_count(unit,EGR_FLEX_CTR_PRI_CNG_MAPm) *
                          sizeof(egr_flex_ctr_pri_cng_map_entry_t);
             egr_pri_cng_map_dma = soc_cm_salloc( unit, alloc_size,
                                                  "EGR_FLEX_CTR_PRI_CNG_MAPm");
             if (egr_pri_cng_map_dma == NULL) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_PRI_CNG_MAPm:DMAAllocationFail\n")));
                 return;
             }
 
             sal_memset(egr_pri_cng_map_dma, 0, alloc_size);

             if (soc_mem_read_range(
                     unit,
                     EGR_FLEX_CTR_PRI_CNG_MAPm,
                     MEM_BLOCK_ANY,
                     soc_mem_index_min(unit,EGR_FLEX_CTR_PRI_CNG_MAPm),
                     soc_mem_index_max(unit,EGR_FLEX_CTR_PRI_CNG_MAPm),
                     egr_pri_cng_map_dma) != SOC_E_NONE) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_PRI_CNG_MAPm:Read failuer \n")));
                 soc_cm_sfree(unit,egr_pri_cng_map_dma);
                 return;
             }
             for (index=0;
                  index< soc_mem_index_count(unit,EGR_FLEX_CTR_PRI_CNG_MAPm);
                  index++) {
                  soc_mem_field_get( unit, EGR_FLEX_CTR_PRI_CNG_MAPm,
                       (uint32 *)&egr_pri_cng_map_dma[index], PRI_CNG_FNf,
                       &map_value);
                  flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                                   pri_cnf_attr_map[index]=(uint8)map_value;
                  if (map_value) {
                      LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Index:%dValue:%d\n"),
                                 index,map_value));
                  }
             }
             soc_cm_sfree(unit,egr_pri_cng_map_dma);
        }
        if ((flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.vlan_format) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.outer_dot1p) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.inner_dot1p)) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "vlan_format:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.vlan_format));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "outer_dot1p:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.outer_dot1p));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "inner_dot1p:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.inner_dot1p));
             alloc_size = soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_PRI_MAPm) *
                          sizeof(egr_flex_ctr_pkt_pri_map_entry_t);
             egr_pkt_pri_map_dma = soc_cm_salloc( unit, alloc_size,
                                                 "EGR_FLEX_CTR_PKT_PRI_MAPm");
             if (egr_pkt_pri_map_dma == NULL) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_PKT_PRI_MAPm:DMAAllocationFail\n")));
                 return;
             }
             sal_memset(egr_pkt_pri_map_dma, 0, alloc_size);
             if (soc_mem_read_range(
                     unit,
                     EGR_FLEX_CTR_PKT_PRI_MAPm,
                     MEM_BLOCK_ANY,
                     soc_mem_index_min(unit,EGR_FLEX_CTR_PKT_PRI_MAPm),
                     soc_mem_index_max(unit,EGR_FLEX_CTR_PKT_PRI_MAPm),
                     egr_pkt_pri_map_dma) != SOC_E_NONE) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_PKT_PRI_MAPm:Read failuer \n")));
                 soc_cm_sfree(unit,egr_pkt_pri_map_dma);
                 return;
             }
             for (index=0;
                  index< soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_PRI_MAPm);
                  index++) {
                  soc_mem_field_get( unit, EGR_FLEX_CTR_PKT_PRI_MAPm,
                       (uint32 *)&egr_pkt_pri_map_dma[index], PKT_PRI_FNf,
                       &map_value);
                  flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                                   pkt_pri_attr_map[index]=(uint8)map_value;
                  if (map_value) {
                      LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Index:%dValue:%d\n"),
                                 index,map_value));
                  }
             }
             soc_cm_sfree(unit,egr_pkt_pri_map_dma);
        }
        if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
            pkt_attr_bits.egr_port) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "egr_port:%d \n"),
                       flex_egress_modes[unit][mode].
                       egr_attr.cmprsd_attr_selectors.
                       pkt_attr_bits.egr_port));
            alloc_size = soc_mem_index_count(unit,EGR_FLEX_CTR_PORT_MAPm) *
                         sizeof(egr_flex_ctr_port_map_entry_t);
            egr_port_map_dma = soc_cm_salloc( unit, alloc_size,
                                              "EGR_FLEX_CTR_PORT_MAPm");
            if (egr_port_map_dma == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "EGR_FLEX_CTR_PORT_MAPm:DMA AllocationFail\n")));
                return;
            }
            sal_memset(egr_port_map_dma, 0, alloc_size);
            if (soc_mem_read_range(
                    unit,
                    EGR_FLEX_CTR_PORT_MAPm,
                    MEM_BLOCK_ANY,
                    soc_mem_index_min(unit,EGR_FLEX_CTR_PORT_MAPm),
                    soc_mem_index_max(unit,EGR_FLEX_CTR_PORT_MAPm),
                    egr_port_map_dma) != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "EGR_FLEX_CTR_PKT_PRI_MAPm:Read failuer \n")));
                soc_cm_sfree(unit,egr_port_map_dma);
                return;
            }
            for (index=0;
                 index< soc_mem_index_count(unit,EGR_FLEX_CTR_PORT_MAPm);
                 index++) {
                 soc_mem_field_get( unit, EGR_FLEX_CTR_PORT_MAPm,
                      (uint32 *)&egr_port_map_dma[index], PORT_FNf,
                      &map_value);
                 flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                                  port_attr_map[index] = (uint8)map_value;
                 if (map_value) {
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Index:%dValue:%d\n"),
                                index,map_value));
                 }
            }
            soc_cm_sfree(unit,egr_port_map_dma);
        }
        if ((flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.tos_dscp)  ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.tos_ecn)) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "tos_dscp:%d tos_ecn:%d \n"),
                       flex_egress_modes[unit][mode].egr_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.tos_dscp,
                       flex_egress_modes[unit][mode].egr_attr.
                       cmprsd_attr_selectors.pkt_attr_bits.tos_ecn));
            alloc_size = soc_mem_index_count(unit,EGR_FLEX_CTR_TOS_MAPm) *
                         sizeof(egr_flex_ctr_tos_map_entry_t);
            egr_tos_map_dma = soc_cm_salloc( unit, alloc_size,
                                             "EGR_FLEX_CTR_TOS_MAPm");
            if (egr_tos_map_dma == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "EGR_FLEX_CTR_TOS_MAPm:DMA Allocation failuer\n")));
                return;
            }
            sal_memset(egr_tos_map_dma, 0, alloc_size);
            if(soc_mem_read_range(
                   unit,
                   EGR_FLEX_CTR_TOS_MAPm,
                   MEM_BLOCK_ANY,
                   soc_mem_index_min(unit,EGR_FLEX_CTR_TOS_MAPm),
                   soc_mem_index_max(unit,EGR_FLEX_CTR_TOS_MAPm),
                   egr_tos_map_dma) != SOC_E_NONE) {
               LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "EGR_FLEX_CTR_TOS_MAPm:Read failuer \n")));
               soc_cm_sfree(unit,egr_tos_map_dma);
               return;
            }
            for (index=0;
                 index< soc_mem_index_count(unit,EGR_FLEX_CTR_TOS_MAPm);
                 index++) {
                 soc_mem_field_get( unit, EGR_FLEX_CTR_TOS_MAPm,
                      (uint32 *)&egr_tos_map_dma[index], TOS_FNf, &map_value);
                 flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                                  tos_attr_map[index] = (uint8)map_value;
                 if (map_value) {
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Index:%dValue:%d\n"),
                                index,map_value));
                 }
            }
            soc_cm_sfree(unit,egr_tos_map_dma);
        }
        if ((flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.pkt_resolution) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.svp_type) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.dvp_type) ||
            (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
             pkt_attr_bits.drop) ) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "pkt_resolution:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.
                        pkt_resolution));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "svp_type:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.svp_type));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "dvp_type:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.dvp_type));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "drop:%d \n"),
                        flex_egress_modes[unit][mode].egr_attr.
                        cmprsd_attr_selectors.pkt_attr_bits.drop));
             alloc_size = soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_RES_MAPm) *
                          sizeof(egr_flex_ctr_pkt_res_map_entry_t);
             egr_pkt_res_map_dma = soc_cm_salloc( unit, alloc_size,
                                                 "EGR_FLEX_CTR_PKT_RES_MAPm");
             if (egr_pkt_res_map_dma == NULL) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_PKT_RES_MAPm:DMAAllocationFail\n")));
                 return;
             }
             sal_memset(egr_pkt_res_map_dma, 0, alloc_size);
             if (soc_mem_read_range(
                     unit,
                     EGR_FLEX_CTR_PKT_RES_MAPm,
                     MEM_BLOCK_ANY,
                     soc_mem_index_min(unit,EGR_FLEX_CTR_PKT_RES_MAPm),
                     soc_mem_index_max(unit,EGR_FLEX_CTR_PKT_RES_MAPm),
                     egr_pkt_res_map_dma) != BCM_E_NONE) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "EGR_FLEX_CTR_TOS_MAPm:Read failuer \n")));
                 soc_cm_sfree(unit,egr_pkt_res_map_dma);
                 return;
             }
             for (index=0;
                  index< soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_RES_MAPm);
                  index++) {
                  soc_mem_field_get( unit, EGR_FLEX_CTR_PKT_RES_MAPm,
                      (uint32 *)&egr_pkt_res_map_dma[index], PKT_RES_FNf,
                      &map_value);
                  flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
                                   pkt_res_attr_map[index]= (uint8)map_value;

                  if (map_value) {
                      LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Index:%dValue:%d\n"),
                                 index,map_value));
                  }
             }
             soc_cm_sfree(unit,egr_pkt_res_map_dma);
        }
        if (flex_egress_modes[unit][mode].egr_attr.cmprsd_attr_selectors.
            pkt_attr_bits.ip_pkt ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ip_pkt:%d \n"),
                       flex_egress_modes[unit][mode].
                       egr_attr.cmprsd_attr_selectors.
                       pkt_attr_bits.ip_pkt));
        }
        return;
    }
    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Ooops. Control Must not reach over here \n")));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_init_udf_mode
 * Description:
 *      Configures flex hw for udf mode
 * Parameters:
 *      unit                           - (IN) unit number
 *      mode                           - (IN) Flex Offset Mode
 *      direction                      - (IN) Flex Data flow direction(ing/egr)
 *      selector_count                 - (IN) Flex Key selector count
 *      selector_x_en_field_value      - (IN) Flex Key selector fields
 *      selector_for_bit_x_field_value - (IN) Flex Key selector field bits
 *
 * Return Value:
 *      None
 * Notes:
 *      Currently not being used!
 *
 */
static void
_bcm_esw_stat_flex_init_udf_mode(
    int                       unit,
    int                       mode,
    bcm_stat_flex_direction_t direction,
    uint32                    selector_count,
    uint32                    selector_x_en_field_value[8],
    uint32                    selector_for_bit_x_field_value[8])
{
    uint16  udf0=0;
    uint16  udf1=0;
    uint32  index=0;
    for (udf0=0,udf1=0,index=0;index<8;index++) {
         if (selector_for_bit_x_field_value[index] != 0) {
             if (selector_for_bit_x_field_value[index] <=16) {
                 udf0 |= (1 << (selector_for_bit_x_field_value[index]-1));
             } else {
                 udf1 |= (1 << (selector_for_bit_x_field_value[index]- 16 - 1));
             }
         }
    }
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "UDF0 : 0x%04x UDF1 0x%04x \n"),
               udf0,udf1));
    if (direction == bcmStatFlexDirectionIngress) {
        flex_ingress_modes[unit][mode].available=1;
        /*Will be changed WhileReadingTables */
        flex_ingress_modes[unit][mode].reference_count=0;
        flex_ingress_modes[unit][mode].total_counters= ( 1 << selector_count);
        flex_ingress_modes[unit][mode].ing_attr.packet_attr_type =
             bcmStatFlexPacketAttrTypeUdf;
        flex_ingress_modes[unit][mode].ing_attr.udf_pkt_attr_selectors.
             udf_pkt_attr_bits.udf0 = udf0;
        flex_ingress_modes[unit][mode].ing_attr.udf_pkt_attr_selectors.
             udf_pkt_attr_bits.udf1 = udf1;
        return;
    }
    if (direction == bcmStatFlexDirectionEgress) {
        flex_egress_modes[unit][mode].available=1;
        /*Will be changed WhileReadingTables */
        flex_egress_modes[unit][mode].reference_count=0;
        flex_egress_modes[unit][mode].total_counters= ( 1 << selector_count);
        flex_egress_modes[unit][mode].egr_attr.packet_attr_type =
             bcmStatFlexPacketAttrTypeUdf;
        flex_egress_modes[unit][mode].egr_attr.udf_pkt_attr_selectors.
             udf_pkt_attr_bits.udf0 = udf0;
        flex_egress_modes[unit][mode].egr_attr.udf_pkt_attr_selectors.
             udf_pkt_attr_bits.udf1 = udf1;
        return;
    }
    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Ooops. Control Must not reach over here \n")));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_l3_next_hop_table
 * Description:
 *      Checks egress_l3_next_hop table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_egress_l3_next_hop_table(int unit)
{
    uint32    index=0;
    /*egr_l3_next_hop_entry_t  egr_l3_next_hop_entry_v; */
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_L3_NEXT_HOPm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index < soc_mem_index_count(unit,EGR_L3_NEXT_HOPm));

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_L3_NEXT_HOP %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_mpls_tunnel_table
 * Description:
 *      Checks egress_mpls_tunnel table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
void
_bcm_esw_stat_flex_check_egress_mpls_tunnel_table(int unit)
{
#ifdef BCM_TRIDENT2PLUS_SUPPORT
    uint32    index=0;
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_IP_TUNNEL_MPLSm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index < soc_mem_index_count(unit,EGR_IP_TUNNEL_MPLSm));

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_IP_TUNNEL_MPLS %d entries..\n"),
               index-1));
#endif
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_vlan_table
 * Description:
 *      Checks egress_vlan table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 *
 */
static void
_bcm_esw_stat_flex_check_egress_vlan_table(int unit)
{
    uint32    index=0;
    /*egr_vlan_entry_t  egr_vlan_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_VLANm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index< soc_mem_index_count(unit,EGR_VLANm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_VLAN %d entries..\n"),
               index-1));
}

/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_nat_pkt_edit_table
 * Description:
 *      Checks egress_nat_packet_edit table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 *
 */
static void
_bcm_esw_stat_flex_check_egress_nat_pkt_edit_table(int unit)
{
    uint32    index=0;

    if (soc_feature(unit, soc_feature_nat)) {
        do {
            _bcm_esw_stat_flex_check_egress_table(unit,EGR_NAT_PACKET_EDIT_INFOm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index< soc_mem_index_count(unit,EGR_NAT_PACKET_EDIT_INFOm));
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR, (BSL_META_U(unit,
                          "Checked EGRESS: EGR_NAT_PACKET_EDIT_INFO %d entries..\n"),
               index-1));
    }
}

/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_vlan_table
 * Description:
 *      Checks ingress vlan table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 *
 */
static void
_bcm_esw_stat_flex_check_ingress_vlan_table(int unit)
{
    uint32    index=0;
    /*vlan_tab_entry_t  vlan_tab_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,VLAN_TABm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while(index<soc_mem_index_count(unit,VLAN_TABm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:VLAN_TAB %d entries..\n"),
               index-1));
}

#ifdef BCM_TOMAHAWK_SUPPORT
#ifdef BCM_WARM_BOOT_SUPPORT
static void
_bcm_esw_stat_flex_check_ingress_agm_table(int unit)
{
    uint32    index=0;
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,AGM_MONITOR_TABLEm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while(index<soc_mem_index_count(unit,AGM_MONITOR_TABLEm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:Agg Monitor %d entries..\n"),
               index-1));
}
#endif
#endif

/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_vrf_table
 * Description:
 *      Checks ingress vrf table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_vrf_table(int unit)
{
    uint32    index=0;
    /*vrf_entry_t  vrf_entry_v;*/

    if (!SOC_MEM_IS_VALID(unit, VRFm)) {
        return; 
    }

    if (soc_mem_index_count(unit, VRFm) <= 0) {
        return;
    }

    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,VRFm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,VRFm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:VRF %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_vfi_table
 * Description:
 *      Checks egress vfi table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_egress_vfi_table(int unit)
{
    uint32    index=0;
    /*egr_vfi_entry_t  egr_vfi_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_VFIm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,EGR_VFIm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_VFI %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_vfi_table
 * Description:
 *      Checks ingress vfi table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
 _bcm_esw_stat_flex_check_ingress_vfi_table(int unit)
{
    uint32    index=0;
    /*vfi_entry_t  vfi_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,VFIm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,VFIm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:VFI %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_l3_iif_table
 * Description:
 *      Checks ingress l3_iif table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_l3_iif_table(int unit)
{
    uint32    index=0;
    /*iif_entry_t  iif_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,L3_IIFm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,L3_IIFm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:L3_IIF %d entries..\n"),
               index-1));
}

/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_l3_routing_table
 * Description:
 *      Checks ingress L3_ENTRY_IPV4_MULTICAST table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
#ifdef BCM_TOMAHAWK_SUPPORT
static void
_bcm_esw_stat_flex_check_ingress_l3_routing_table(int unit)
{
    uint32    index=0;
    /*iif_entry_t  iif_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit, L3_ENTRY_IPV4_MULTICASTm,
            index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit, L3_ENTRY_IPV4_MULTICASTm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS: L3_ENTRY_IPV4_MULTICAST %d entries..\n"),
               index-1));
}
#endif

/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_source_vp_table
 * Description:
 *      Checks ingress source_vp table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_source_vp_table(int unit)
{
    uint32    index=0;
    /*source_vp_entry_t  source_vp_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,SOURCE_VPm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,SOURCE_VPm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:SOURCE_VP %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_mpls_entry_table
 * Description:
 *      Checks ingress mpls_entry table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_mpls_entry_table(int unit)
{
    uint32    index=0;
    /*mpls_entry_entry_t  mpls_entry_entry_v;*/
    if (!SOC_IS_TRIUMPH3(unit)) {
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit,MPLS_ENTRYm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index<soc_mem_index_count(unit,MPLS_ENTRYm));
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (SOC_IS_TRIUMPH3(unit)) {
        index = 0;
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit,MPLS_ENTRY_EXTDm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index<soc_mem_index_count(unit,MPLS_ENTRY_EXTDm));
    }
#endif
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:MPLS_ENTRY %d entries..\n"),
               index-1));
}

#ifdef BCM_WARM_BOOT_SUPPORT
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_vfp_policy_table
 * Description:
 *      Checks ingress vfp_policy table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_vfp_policy_table(int unit, uint32 per_pipe_mode)
{
    uint32    index=0;
    uint32    max_index;
#ifdef BCM_TOMAHAWK_SUPPORT
    int       pipe_num = 0;
#endif

    max_index = soc_mem_index_count(unit, VFP_POLICY_TABLEm);

    /*vfp_policy_table_entry_t  vfp_policy_table_entry_v;*/
    if (!per_pipe_mode) {
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit,VFP_POLICY_TABLEm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index < max_index);
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Checked INGRESS:VFP_POLICY_TABLE %d entries..\n"),
                   index-1));
        return;
    }
#ifdef BCM_TOMAHAWK_SUPPORT
    if (SOC_IS_TOMAHAWKX(unit)) {
       /* Per-pipe mode */
       for(pipe_num =0; pipe_num < 4; ++pipe_num) {
          index = 0;
          do {
             _bcm_esw_stat_flex_check_ingress_table(unit,_vfp_mem_arr[pipe_num],
                 index, index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
             index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
          } while (index < max_index);

          LOG_DEBUG(BSL_LS_BCM_FLEXCTR, (BSL_META_U(unit,
                  "Checked INGRESS:VFP_POLICY_TABLE %d entries..\n"), index-1));
       }
    }
#endif
}
#endif

#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT) \
    || defined(BCM_APACHE_SUPPORT)
#ifdef BCM_WARM_BOOT_SUPPORT

#define BCM_FLEX_CHECK_INGRESS_TABLE(_unit, _mem, _index) {                    \
    _index=0;                                                                       \
    do {                                                                            \
        _bcm_esw_stat_flex_check_ingress_table(_unit, _mem,                         \
             _index, _index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);               \
        _index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;                                \
    } while (_index < soc_mem_index_count(_unit, IFP_POLICY_TABLEm));               \
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,                                                   \
              (BSL_META_U(_unit,                                                    \
                          "Checked INGRESS:IFP_POLICY_TABLE %d entries..\n"),       \
                          _index-1));                                               \
}

/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_ifp_policy_table
 * Description:
 *      Checks ifp_policy table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_ifp_policy_table(int unit, uint32 per_pipe_mode)
{
    uint32    index=0;
#if defined(BCM_TOMAHAWK_SUPPORT)
    if (SOC_IS_TOMAHAWKX(unit)) {

        int pipe_num = 0;

        if (!per_pipe_mode) {
            BCM_FLEX_CHECK_INGRESS_TABLE(unit, IFP_POLICY_TABLEm, index);
            return;
        }

        /* Per-pipe mode */
        for(pipe_num =0; pipe_num < 4; ++pipe_num) {
            BCM_FLEX_CHECK_INGRESS_TABLE(unit, _ifp_mem_arr[pipe_num], index);
        }
    } else
#endif
    {
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit, FP_POLICY_TABLEm,
                index, index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
             index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index < soc_mem_index_count(unit, FP_POLICY_TABLEm));
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                            "Checked INGRESS:FP_POLICY_TABLE %d entries..\n"),
                            index-1));
    }

    return;

}
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_efp_policy_table
 * Description:
 *      Checks efp_policy table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_egress_efp_policy_table(int unit)
{
    uint32    index=0;
    do {
        _bcm_esw_stat_flex_check_egress_table(unit, EFP_POLICY_TABLEm,
                index, index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index < soc_mem_index_count(unit, EFP_POLICY_TABLEm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
            (BSL_META_U(unit,
                        "Checked EGRESS:EFP_POLICY_TABLE %d entries..\n"),
             index-1));

    return;

}
#endif
#endif
#endif
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_vlan_xlate_table
 * Description:
 *      Checks egress vlan_xlate table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_egress_vlan_xlate_table(int unit)
{
    uint32    index=0;
    /*egr_vlan_xlate_entry_t  egr_vlan_xlate_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_VLAN_XLATEm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,EGR_VLAN_XLATEm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_VLAN_XLATE %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_vlan_xlate_table
 * Description:
 *      Checks ingress vlan_xlate table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_vlan_xlate_table(int unit)
{
    uint32    index=0;
    /*vlan_xlate_entry_t  vlan_xlate_entry_v;*/
    if (!SOC_IS_TRIUMPH3(unit)) {
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit,VLAN_XLATEm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index<soc_mem_index_count(unit,VLAN_XLATEm));
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if (SOC_IS_TRIUMPH3(unit)) {
        index= 0;
        do {
            _bcm_esw_stat_flex_check_ingress_table(unit,VLAN_XLATE_EXTDm,
                 index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
            index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
        } while (index<soc_mem_index_count(unit,VLAN_XLATE_EXTDm));
    }
#endif
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:VLAN_XLATE_TABLE %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_egress_port_table
 * Description:
 *      Checks egress port table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_egress_port_table(int unit)
{
    uint32    index=0;
    /*egr_port_entry_t  egr_port_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_egress_table(unit,EGR_PORTm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while (index<soc_mem_index_count(unit,EGR_PORTm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked EGRESS:EGR_PORT %d entries..\n"),
               index-1));
}
/*
 * Function:
 *      _bcm_esw_stat_flex_check_ingress_port_table
 * Description:
 *      Checks ingress port table for flex configuration
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      None
 * Notes:
 *      Applicable in warm boot scenario only!
 */
static void
_bcm_esw_stat_flex_check_ingress_port_table(int unit)
{
    uint32    index=0;
    /*port_tab_entry_t  port_tab_entry_v;*/
    do {
        _bcm_esw_stat_flex_check_ingress_table(unit,PORT_TABm,
             index,index + BCM_STAT_FLEX_WARMBOOT_READ_CHUNK - 1);
        index += BCM_STAT_FLEX_WARMBOOT_READ_CHUNK;
    } while(index<soc_mem_index_count(unit,PORT_TABm));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Checked INGRESS:PORT_TABLE %d entries..\n"),
               index-1));
}

/* Helper function for _bcm_esw_stat_flex_set */
STATIC int
_bcm_esw_stat_flex_set_mem_update(int unit, soc_mem_t table, int pipe_num, uint32 index,
                                  soc_mem_t mem, uint32 start, uint32 entry_cnt,
                                  void *entry, int dir, uint32 pool_id, int byte_flag
                                  )
{
    int i;
    uint32 zero=0;
    uint32 one=1;
    uint32 max_packet_mask = 0;
    uint64 max_byte_mask;
    uint32 hw_val[2];
    bcm_stat_flex_counter_value_t flex_values  = {0};
#if defined(BCM_TRIDENT2_SUPPORT)
    soc_mem_t mem_dual = 0;
#endif

    COMPILER_64_ZERO(max_byte_mask);
    COMPILER_64_SET(max_byte_mask, zero, one);
    COMPILER_64_SHL(max_byte_mask,soc_mem_field_length(unit, mem, BYTE_COUNTERf));
    COMPILER_64_SUB_32(max_byte_mask, one);
    if (pipe_num < 0) {
        pipe_num = 0;
    }

    max_packet_mask = (1 << soc_mem_field_length(unit, mem, PACKET_COUNTERf));
    max_packet_mask -= 1;

#define FLEX_SET_BYTE_COUNTER_UPDATE \
    do { \
        /* Mask to possible max values */ \
        COMPILER_64_AND(flex_values.byte_counter_value, max_byte_mask); \
        /* Update Soft Copy */ \
        COMPILER_64_SET(flex_byte_counter[unit][dir][pipe_num][pool_id][start+i], \
                        COMPILER_64_HI(flex_values.byte_counter_value), \
                        COMPILER_64_LO(flex_values.byte_counter_value)); \
        /* Change Read Hw Copy */ \
        hw_val[0] = COMPILER_64_HI(flex_values.byte_counter_value); \
        hw_val[1] = COMPILER_64_LO(flex_values.byte_counter_value); \
        soc_mem_field_set(unit, mem, (uint32 *)&entry_temp[i], \
                          BYTE_COUNTERf, hw_val); \
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR, \
                  (BSL_META_U(unit, \
                              "Byte Count Value\t:TABLE:%sINDEX:%d COUTER-%d" \
                               "(@Pool:%dDirection:%dActualOffset%d)" \
                               " : %x:%x \n"),SOC_MEM_UFNAME(unit, table), \
                   index, i, pool_id, dir, start + i, \
                   COMPILER_64_HI(flex_values.byte_counter_value), \
                   COMPILER_64_LO(flex_values.byte_counter_value))); \
    } while (0)

#define FLEX_SET_BYTE_COUNTER_UPDATE_TD2 \
    if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) { \
        int j; \
        for (j = 0; j < _BCM_FLEX_STAT_BUFFS; j++) { \
            COMPILER_64_SET(flex_byte_counter_x[unit][dir][pool_id][j][start+i], \
                            COMPILER_64_HI(flex_values.byte_counter_value), \
                            COMPILER_64_LO(flex_values.byte_counter_value)); \
            COMPILER_64_ZERO(flex_byte_counter_y[unit][dir][pool_id][j][start+i]); \
        } \
    }

#define FLEX_SET_PACKET_COUNTER_UPDATE \
    do { \
        flex_values.pkt_counter_value &= max_packet_mask; \
        /* Update Soft 64-bits Copy */ \
        COMPILER_64_SET(flex_packet64_counter[unit][dir][pipe_num][pool_id][start+i], \
                    0, flex_values.pkt_counter_value); \
        /* Update Soft Copy */ \
        flex_packet_counter[unit][dir][pipe_num][pool_id][start+i] = \
                    flex_values.pkt_counter_value; \
        /* Change Read Hw Copy */ \
        soc_mem_field_set(unit, mem, (uint32 *)&entry_temp[i], \
                          PACKET_COUNTERf, &flex_values.pkt_counter_value); \
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR, \
                  (BSL_META_U(unit, \
                              "Packet Count Value\t:TABLE:%sINDEX:%d" \
                               "COUTER-%d (@Pool:%dDirection:%d" \
                               "ActualOffset%d) : %x \n"), SOC_MEM_UFNAME(unit, table), \
                   index, i, pool_id, dir, start + i, \
                   flex_values.pkt_counter_value)); \
    } while (0)

#define FLEX_SET_PACKET_COUNTER_UPDATE_TD2 \
    if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) { \
        int j; \
        for (j = 0; j < _BCM_FLEX_STAT_BUFFS; j++) { \
            flex_pkt_counter_x[unit][dir][pool_id][j][start+i] \
                = flex_values.pkt_counter_value; \
            flex_pkt_counter_y[unit][dir][pool_id][j][start+i] = 0; \
        } \
    }

    /* Type of entry is ing_flex_ctr_counter_table_0_entry_t, and entry also
     * could be used for egress flex counter whose type should be
     * egr_flex_ctr_counter_table_0_entry_t, but since their entry size is the
     * same, so we don't need to distinguish them */

    /* If entry_cnt less than 8, memory range operation is not efficient */
    if (entry_cnt <= 8) {
        for (i = 0 ; i < entry_cnt ; i++) {
            ing_flex_ctr_counter_table_0_entry_t *entry_temp = entry;
            if (byte_flag != -1) {
                BCM_IF_ERROR_RETURN(soc_mem_read(unit, mem, MEM_BLOCK_ANY,
                                                 start + i, &entry_temp[i]));
            }

            /* Bytes Counter update */
            if (byte_flag == 1 || byte_flag == -1) {
                FLEX_SET_BYTE_COUNTER_UPDATE;
#if defined(BCM_TRIDENT2_SUPPORT)
                FLEX_SET_BYTE_COUNTER_UPDATE_TD2;
#endif
            }

            /* Packet Counter update */
            if (byte_flag == 0 || byte_flag == -1) {
                FLEX_SET_PACKET_COUNTER_UPDATE;
#if defined(BCM_TRIDENT2_SUPPORT)
                FLEX_SET_PACKET_COUNTER_UPDATE_TD2;
#endif
            }

            /* TD2 counter value is set in X memory, clear Y memory */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                mem = _ctr_counter_table_x[dir][pool_id];
            }
#endif
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem, MEM_BLOCK_ANY,
                                              start + i, &entry_temp[i]));
#if defined(BCM_TRIDENT2_SUPPORT)
            if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                mem_dual =_ctr_counter_table_y[dir][pool_id];
                sal_memset(&entry_temp[i], 0,
                        sizeof(ing_flex_ctr_counter_table_0_entry_t));
                BCM_IF_ERROR_RETURN(soc_mem_write(unit, mem_dual, MEM_BLOCK_ANY,
                                                  start + i, &entry_temp[i]));
            }
#endif
        }
        return BCM_E_NONE;
    }

    /* If we want to update both byte and packet fields,
     * we don't need to read the original value */
    if (byte_flag != -1) {
        if (soc_mem_read_range(unit, mem, MEM_BLOCK_ANY, start,
                               start+entry_cnt-1, entry) != BCM_E_NONE) {
            return BCM_E_INTERNAL;
        }
    }

    for (i = 0 ; i < entry_cnt ; i++) {
        ing_flex_ctr_counter_table_0_entry_t *entry_temp = entry;
        /* Bytes Counter update */
        if (byte_flag == 1 || byte_flag == -1) {
            FLEX_SET_BYTE_COUNTER_UPDATE;
#if defined(BCM_TRIDENT2_SUPPORT)
            FLEX_SET_BYTE_COUNTER_UPDATE_TD2;
#endif
        }

        /* Packet Counter update */
        if (byte_flag == 0 || byte_flag == -1) {
            FLEX_SET_PACKET_COUNTER_UPDATE;
#if defined(BCM_TRIDENT2_SUPPORT)
            FLEX_SET_PACKET_COUNTER_UPDATE_TD2;
#endif
        }
    }

    /* TD2 counter value is set in X memory, clear Y memory */
#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
        mem = _ctr_counter_table_x[dir][pool_id];
    }
#endif
    /* Update Hw Copy */
    if (soc_mem_write_range(unit, mem, MEM_BLOCK_ALL, start, start+entry_cnt-1,
                            entry) != BCM_E_NONE) {
        return BCM_E_INTERNAL;
    }

#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
        mem_dual =_ctr_counter_table_y[dir][pool_id];
        /* Update Hw Copy */
        sal_memset(entry, 0, entry_cnt * sizeof(ing_flex_ctr_counter_table_0_entry_t));
        if (soc_mem_write_range(unit, mem_dual, MEM_BLOCK_ALL, start, start+entry_cnt-1,
                                entry) != BCM_E_NONE) {
            return BCM_E_INTERNAL;
        }
    }
#endif
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_set
 * Description:
 *      Set flex counter values
 * Parameters:
 *      unit                  - (IN) unit number
 *      index                 - (IN) Flex Accounting Table Index
 *      table                 - (IN) Flex Accounting Table
 *      byte_flag             - (IN) Byte/Packet Flag
 *      flex_ctr_offset_info  - (IN) Information about flex counter values
 *      flex_entries          - (IN) Information about flex entries
 *      offset_mode           - (IN) FLEX_CTR_OFFSET_MODE field value
 *      pool_number           - (IN) FLEX_CTR_POOL_NUMBER field value
 *      base_idx              - (IN) FLEX_CTR_BASE_COUNTER_IDX field value
 *      direction             - (IN) Flex Data flow direction
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t
_bcm_esw_stat_flex_set(
    int                             unit,
    uint32                          index,
    soc_mem_t                       table,
    uint32                          byte_flag,
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info,
    uint32                          *flex_entries,
    uint32                          offset_mode,
    uint32                          pool_number,
    uint32                          base_idx ,
    bcm_stat_flex_direction_t       direction)
{
    soc_mem_t                 mem;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    int                       pipe_num = 0;

    if (flex_temp_counter[unit][direction] == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Not initilized or attached yet\n")));
        return BCM_E_CONFIG;
    }

    if (direction == bcmStatFlexDirectionIngress) {
#if defined (BCM_TOMAHAWK_SUPPORT)
        pipe_num = _bcm_esw_get_fp_pipe_num(unit, table);
        if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
            (_bcm_esw_is_fp_table(table))) {
            uint32 cmode = 0;
            bcm_error_t rv;
            uint32 stat_counter_id;

            rv = _bcm_esw_custom_stat_group_id_retrieve(unit, offset_mode, pipe_num,
                pool_number, base_idx,&stat_counter_id);
            if (rv == BCM_E_NONE) {
                cmode = stat_counter_map[unit][stat_counter_id].mode;
                total_entries = flex_custom_ingress_modes[unit][cmode - BCM_CUSTOM_INGRESS_MODE_START].
                    total_counters;
            } else {
                 total_entries = flex_ingress_modes[unit][offset_mode].total_counters;
            }
        } else
#endif
        {
            total_entries = flex_ingress_modes[unit][offset_mode].total_counters;
        }
    } else {
        total_entries = flex_egress_modes[unit][offset_mode].total_counters;
    }
    offset_index = 0;

    *flex_entries = total_entries;

    if (SOC_IS_TOMAHAWKX(unit)) {
        return th_flex_counter_set(unit, direction, pipe_num, pool_number,
                                   base_idx+offset_index,
                                   (bcm_stat_value_t *)NULL,
                                   total_entries, byte_flag);
    }

    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    mem = _ctr_counter_table[direction][pool_number];

    if (BCM_FAILURE(_bcm_esw_stat_flex_set_mem_update(unit, table, pipe_num, index,
                            mem, base_idx+offset_index, total_entries,
                            flex_temp_counter[unit][direction], direction,
                            pool_number, byte_flag))) {
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        return BCM_E_INTERNAL;
    }

    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_ext_stat_flex_set
 * Description:
 *      Set flex counter values for external fp policy
 * Parameters:
 *      unit                  - (IN) unit number
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      offset_mode           - (IN) Flex offset mode for Accounting Object
 *      table                 - (IN) Flex Accounting Table
 *      byte_flag             - (IN) Byte/Packet Flag
 *      flex_ctr_offset_info  - (IN) Information about flex counter values
 *      flex_entries          - (IN) Information about flex entries
 *      flex_values           - (IN) Information about flex entries values
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t
_bcm_esw_ext_stat_flex_set(
    int                             unit,
    uint32                          pool_number,
    uint32                          base_idx,
    bcm_stat_flex_mode_t            offset_mode,
    soc_mem_t                       table,
    uint32                          byte_flag,
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info,
    uint32                          *flex_entries,
    bcm_stat_flex_counter_value_t   *flex_values)
{
    soc_mem_t                 mem;
    uint32                    loop=0;
    bcm_stat_flex_direction_t direction = bcmStatFlexDirectionIngress;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    uint32                    zero=0;
    uint32                    one=1;
    uint32                    max_packet_mask=0;
    uint64                    max_byte_mask;
    uint32                    hw_val[2];

    mem = _ctr_counter_table[direction][0];
    COMPILER_64_SET(max_byte_mask, zero, one);
    COMPILER_64_SHL(max_byte_mask,soc_mem_field_length(unit,mem,BYTE_COUNTERf));
    COMPILER_64_SUB_32(max_byte_mask,one);

    max_packet_mask = (1 << soc_mem_field_length(unit,mem,PACKET_COUNTERf));
    max_packet_mask -= 1;

    if ((offset_mode == 0) && (base_idx == 0)) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "EXT_FP_POLICY table: pool_number:%d: IsNotConfigured"
                                " for flex counter \n"),
                    pool_number));
         /* Either Not configured or deallocated before */
         return BCM_E_NOT_FOUND;
    }

    /* EXT Stat's are currently supported on EXT_FP_POLICYm (Ingress Direction)
     * */
    if (direction == bcmStatFlexDirectionIngress) {
        total_entries= flex_ingress_modes[unit][offset_mode].total_counters;
    } 

    if (flex_ctr_offset_info.all_counters_flag) {
        offset_index = 0;
    } else {
        offset_index = flex_ctr_offset_info.offset_index;
        if (offset_index >= total_entries) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Wrong OFFSET_INDEX.Must be <Total Counters %d \n"),
                       total_entries));
            return BCM_E_PARAM;
        }
        total_entries = 1;
    }

    *flex_entries = total_entries;

    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    mem = _ctr_counter_table[direction][pool_number];
    if (soc_mem_read_range(unit,
                           mem,
                           MEM_BLOCK_ANY,
                           base_idx+offset_index,
                           base_idx+offset_index+total_entries-1,
                           flex_temp_counter[unit][direction]) != BCM_E_NONE) {
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        return BCM_E_INTERNAL;
    }

    for (loop = 0 ; loop < total_entries ; loop++) {
         if (byte_flag == 1) {
             /* Mask to possible max values */
             COMPILER_64_AND(flex_values[loop].byte_counter_value,
                                 max_byte_mask);
             /* Update Soft Copy */
             COMPILER_64_SET(flex_byte_counter[unit][direction][0]
                                 [pool_number][base_idx+offset_index+loop],
                                 COMPILER_64_HI(flex_values[loop].
                                                byte_counter_value),
                                 COMPILER_64_LO(flex_values[loop].
                                                byte_counter_value));
             /* Change Read Hw Copy */
             hw_val[0] = COMPILER_64_HI(flex_values[loop].
                                            byte_counter_value);
             hw_val[1] = COMPILER_64_LO(flex_values[loop].
                                            byte_counter_value);
             soc_mem_field_set(
                         unit,mem,
                         (uint32 *)&flex_temp_counter[unit][direction][loop],
                         BYTE_COUNTERf, hw_val);
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Byte Count Value\t:TABLE:EXT_FP_POLICY COUNTER-%d"
                                    "(@ Pool:%d Direction:%d ActualOffset%d)"
                                    " : %x:%x \n"),
                        loop, pool_number, direction,
                        base_idx+offset_index+loop,
                        COMPILER_64_HI(flex_values[loop].
                        byte_counter_value),
                        COMPILER_64_LO(flex_values[loop].
                        byte_counter_value)));
        } else {
            flex_values[loop].pkt_counter_value &= max_packet_mask;
            /* Update Soft Copy 64-bit */
            COMPILER_64_SET(flex_packet64_counter[unit][direction][0]
                            [pool_number][base_idx+offset_index+loop],
                            0,
                            flex_values[loop].
                            pkt_counter_value);

            /* Update Soft Copy */
            flex_packet_counter[unit][direction][0][pool_number]
                                   [base_idx+offset_index+loop] =
                                   flex_values[loop].pkt_counter_value;
            /* Change Read Hw Copy */
            soc_mem_field_set(
                            unit,
                            mem,
                            (uint32 *)&flex_temp_counter[unit][direction][loop],
                            PACKET_COUNTERf,
                            &flex_values[loop].pkt_counter_value);
                    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "Packet Count Value\t:TABLE:EXT_FP_POLICY "
                                           "COUNTER-%d (@ Pool:%d Direction:%d "
                                           "ActualOffset%d) : %x \n"),
                               loop, pool_number,direction,
                               base_idx+offset_index+loop,
                               flex_values[loop].pkt_counter_value));
                }
        }

    /* Update Hw Copy */
    if (soc_mem_write_range(unit,mem,MEM_BLOCK_ALL,
                    base_idx+offset_index,
                    base_idx+offset_index+total_entries-1,
                    flex_temp_counter[unit][direction]) != BCM_E_NONE) {
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        return BCM_E_INTERNAL;
    }
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}


#ifdef BCM_WARM_BOOT_SUPPORT
/*
 * Function:
 *      _bcm_esw_stat_flex_sync
 * Description:
 *      Sync flex s/w copy to h/w copy
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_sync(int unit)
{
    bcm_stat_flex_group_mode_related_info_t *flex_group_mode_related_info =
                                 (bcm_stat_flex_group_mode_related_info_t *)
                                 (&flex_scache_ptr[unit][0] +
                                   BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE);

    uint32 mode=0;
#ifdef BCM_TOMAHAWK_SUPPORT
    uint32 stat_ctr_id = 1;
    bcm_stat_flex_custom_mode_related_info_t *flex_custom_mode_related_info =
                                 NULL;
    bcm_stat_flex_counter_id_related_info_t *flex_stat_ctr_related_info =
                                 NULL;
    uint32 *per_pipe_mode = NULL;
#endif
    uint32 sel=0;
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "_bcm_esw_stat_flex_sync \n")));
    if (local_scache_ptr[unit]  == NULL) {
        return BCM_E_INIT;
    }
    if ((handle[unit] == 0) ||
        (flex_scache_allocated_size[unit] == 0) ||
        (flex_scache_ptr[unit] == NULL)) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Scache memory was not allocate in init!! \n")));
         return BCM_E_CONFIG;
    }
    sal_memcpy(&flex_scache_ptr[unit][0],
               &local_scache_ptr[unit][0],
               BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE);
    for (mode=0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE ; mode++) {
         if (flex_ingress_modes[unit][mode].flags != 0) {
             flex_group_mode_related_info->flags = flex_ingress_modes[unit]
                                                   [mode].flags;
             flex_group_mode_related_info->num_selectors =
                                                   flex_ingress_modes[unit]
                                                   [mode].num_selectors;
             flex_group_mode_related_info->total_counters =
                                                   flex_ingress_modes[unit]
                                                   [mode].total_counters;
             flex_group_mode_related_info->flags |= (flex_ingress_modes[unit]
                                                   [mode].group_mode << 16);
             for (sel=0;
                  sel < flex_group_mode_related_info->num_selectors;
                  sel++) {
                  flex_group_mode_related_info->attr_selectors[sel].counter_offset =
                      flex_ingress_modes[unit][mode].attr_selectors[sel].counter_offset;
                  flex_group_mode_related_info->attr_selectors[sel].attr =
                      flex_ingress_modes[unit][mode].attr_selectors[sel].attr;
                  flex_group_mode_related_info->attr_selectors[sel].attr_value =
                      flex_ingress_modes[unit][mode].attr_selectors[sel].attr_value;
              }
         }
         flex_group_mode_related_info++;
    }
    for (mode=0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE ; mode++) {
         if (flex_egress_modes[unit][mode].flags != 0) {
             flex_group_mode_related_info->flags = flex_egress_modes[unit]
                                                   [mode].flags;
             flex_group_mode_related_info->num_selectors =
                                                   flex_egress_modes[unit]
                                                   [mode].num_selectors;
             flex_group_mode_related_info->total_counters =
                                                   flex_egress_modes[unit]
                                                   [mode].total_counters;
             flex_group_mode_related_info->flags |= (flex_egress_modes[unit]
                                                   [mode].group_mode << 16);
             for (sel=0;
                  sel < flex_group_mode_related_info->num_selectors;
                  sel++) {
                  flex_group_mode_related_info->attr_selectors[sel].counter_offset=
                      flex_egress_modes[unit][mode].attr_selectors[sel].counter_offset;
                  flex_group_mode_related_info->attr_selectors[sel].attr=
                      flex_egress_modes[unit][mode].attr_selectors[sel].attr;
                  flex_group_mode_related_info->attr_selectors[sel].attr_value=
                      flex_egress_modes[unit][mode].attr_selectors[sel].attr_value;
              }
         }
         flex_group_mode_related_info++;
    }

    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
                   (BCM_STAT_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_4)) {
#ifdef BCM_TOMAHAWK_SUPPORT
        flex_custom_mode_related_info = (void *)flex_group_mode_related_info;
        for (mode=BCM_CUSTOM_INGRESS_MODE_START; 
           (mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE) ; mode++) {
             int mode_idx = mode - BCM_CUSTOM_INGRESS_MODE_START;
             if (flex_custom_ingress_modes[unit][mode_idx].used != 0) {
                 int mode_idx = mode - BCM_CUSTOM_INGRESS_MODE_START;
                 flex_custom_mode_related_info->custom_mode = mode;
                 flex_custom_mode_related_info->offset_mode = 
                             flex_custom_ingress_modes[unit][mode_idx].offset_mode;

                 flex_custom_mode_related_info->AllocPool = 0;
                 flex_custom_mode_related_info->fg_id = 0xffffffff;
                 if ((flex_custom_ingress_modes[unit][mode_idx].hint.type == 
                                     bcmStatGroupAllocHintIngressFieldGroup) ||
                     (flex_custom_ingress_modes[unit][mode_idx].hint.type == 
                                     bcmStatGroupAllocHintExactMatchFieldGroup) ||
                     (flex_custom_ingress_modes[unit][mode_idx].hint.type == 
                                     bcmStatGroupAllocHintVlanFieldGroup)) {
                     flex_custom_mode_related_info->AllocPool = 
                              flex_custom_ingress_modes[unit][mode_idx].AllocPool;
                     flex_custom_mode_related_info->fg_id = 
                              flex_custom_ingress_modes[unit][mode_idx].hint.value;

                 }
             } else {
                 flex_custom_mode_related_info->custom_mode = 0;
             }
             flex_custom_mode_related_info++;
        }
        flex_stat_ctr_related_info = (void *)flex_custom_mode_related_info;
        for (stat_ctr_id=1; 
           (stat_ctr_id < BCM_MAX_STAT_COUNTER_IDS) ; stat_ctr_id++, flex_stat_ctr_related_info++) {
             bcm_stat_counter_map_t stat_ctr_map;
             _bcm_get_stat_counter_map(unit, stat_ctr_id, &stat_ctr_map);
             flex_stat_ctr_related_info->used = stat_ctr_map.used;
             if (stat_ctr_map.used != 0) {
                 flex_stat_ctr_related_info->mode = stat_ctr_map.mode;
                 flex_stat_ctr_related_info->base_idx = stat_ctr_map.base_idx;
                 flex_stat_ctr_related_info->object = stat_ctr_map.object;
                 flex_stat_ctr_related_info->group = stat_ctr_map.group;
                 flex_stat_ctr_related_info->pool = stat_ctr_map.pool;
                 if ((stat_ctr_map.mode >= BCM_CUSTOM_INGRESS_MODE_START) && 
                     (stat_ctr_map.mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
                      flex_stat_ctr_related_info->used = (stat_ctr_map.pipe << 6) |
                           (flex_custom_ingress_modes[unit][stat_ctr_map.mode - BCM_CUSTOM_INGRESS_MODE_START].total_counters);

                 } else if (stat_ctr_map.mode < BCM_STAT_FLEX_COUNTER_MAX_MODE){
                      flex_stat_ctr_related_info->used = 
                         (flex_ingress_modes[unit][stat_ctr_map.mode].total_counters);
                 }
             }
        }
        per_pipe_mode = (uint32 *)flex_stat_ctr_related_info;
        *per_pipe_mode = !_bcm_esw_get_fp_mode_global(unit, bcmFieldQualifyStageIngress);
        per_pipe_mode = ((uint32 *)flex_stat_ctr_related_info)+1;
        *per_pipe_mode = !_bcm_esw_get_fp_mode_global(unit, bcmFieldQualifyStageLookup);
#endif
    }

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "OK \n")));
    return BCM_E_NONE;
}
#endif

#define CONFIG_ING_OBJECT 0
#define CONFIG_EGR_OBJECT 1

/*
 * Function:
 *      _bcm_esw_stat_flex_cleanup
 * Description:
 *      Clean and free all allocated  flex resources in the specified pipe
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_cleanup_pipe(int unit, int pipe_num, 
                                            bcm_stat_flex_direction_t direction)
{
    uint32      pool_id=0;

    uint32      num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
#if defined(BCM_TRIDENT2_SUPPORT)
    int         buffer_id;
    bcm_stat_flex_mode_t offset_mode;
#endif /* BCM_TRIDENT2_SUPPORT */
    int         pool_group_id;

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "_bcm_esw_stat_flex_cleanup \n")));
    num_pools[bcmStatFlexDirectionIngress]=SOC_INFO(unit).
                                           num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] =SOC_INFO(unit).
                                           num_flex_egress_pools;
    if (flex_stat_mutex[unit] != NULL) {
        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        soc_counter_extra_unregister(unit, _bcm_esw_stat_flex_callback);
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        sal_mutex_destroy(flex_stat_mutex[unit]);
        flex_stat_mutex[unit] = NULL;
    }

    if (direction != bcmStatFlexDirectionIngress) {
        pipe_num = 0;
    }

#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
        if (flex_temp_counter_dual[unit][direction] != NULL){
            soc_cm_sfree(unit,flex_temp_counter_dual[unit][direction]);
            flex_temp_counter_dual[unit][direction]=NULL;
        }
    }
#endif /* BCM_TRIDENT2_SUPPORT*/

    if (flex_temp_counter[unit][direction] != NULL){
        soc_cm_sfree(unit,flex_temp_counter[unit][direction]);
        flex_temp_counter[unit][direction]=NULL;
    }
    if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
        if (direction == bcmStatFlexDirectionIngress) {
            for (pool_group_id=0;
                 pool_group_id < _bcm_esw_get_max_pool_groups(unit);
                 pool_group_id++) {
                if (BCM_E_PARAM ==  
                    _bcm_esw_stat_flex_oam_enable_ingress_group(unit,
                                                              pool_group_id,
                                                              0)) {
                    continue;
                }
                flex_pool_group[unit][pipe_num][pool_group_id].group_type =
                     BCM_FLEX_POOL_GROUP_USED_BY_NONE;
                flex_pool_group[unit][pipe_num][pool_group_id].free_pools  =
                     BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
                flex_pool_group[unit][pipe_num][pool_group_id].base_pool = 
                     pool_group_id * BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
            }
        }
    }
    for (pool_id=0;pool_id<num_pools[direction];pool_id++) {
         _bcm_esw_stat_flex_enable_pool(
                  unit,
                  direction,
                  _pool_ctr_register[direction][pool_id],
                  0);
         if (flex_aidxres_list_handle[unit][direction][pipe_num][pool_id] != 0) {
             shr_aidxres_list_destroy(flex_aidxres_list_handle[unit]
                                      [direction][pipe_num][pool_id]);
             flex_aidxres_list_handle[unit][direction][pipe_num][pool_id] = 0;
         }
         flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_tables = 0;
         flex_pool_stat[unit][direction][pipe_num][pool_id].used_entries = 0;
         flex_pool_stat[unit][direction][pipe_num][pool_id].attached_entries = 0;
         SHR_BITCLR_RANGE(flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_objects, 0, bcmIntStatObjectMaxValue);
         if (flex_byte_counter[unit][direction][pipe_num][pool_id] != NULL) {
             soc_cm_sfree(unit,
                          flex_byte_counter[unit][direction][pipe_num][pool_id]);
             flex_byte_counter[unit][direction][pipe_num][pool_id] = NULL;
         }
         if (flex_packet_counter[unit][direction][pipe_num][pool_id] != NULL) {
             soc_cm_sfree(unit,
                          flex_packet_counter[unit][direction][pipe_num][pool_id]);
             flex_packet_counter[unit][direction][pipe_num][pool_id] = NULL;
         }
         if (flex_packet64_counter[unit][direction][pipe_num][pool_id] != NULL) {
             soc_cm_sfree(unit,
                          flex_packet64_counter[unit][direction][pipe_num][pool_id]);
             flex_packet64_counter[unit][direction][pipe_num][pool_id] = NULL;
         }

#ifdef BCM_TOMAHAWK_SUPPORT
         if (SOC_IS_TOMAHAWKX(unit)) {
             if (residual_byte_counter[unit][direction][pipe_num][pool_id] != NULL) {
                 soc_cm_sfree(unit,
                              residual_byte_counter[unit][direction][pipe_num]
                              [pool_id]);
                 residual_byte_counter[unit][direction][pipe_num][pool_id] = NULL;
             }
             if (residual_packet_counter[unit][direction][pipe_num][pool_id] !=
                 NULL) {
                 soc_cm_sfree(unit,
                         residual_packet_counter[unit][direction]
                         [pipe_num][pool_id]);
                 residual_packet_counter[unit][direction][pipe_num][pool_id] = NULL;
             }
             if (bcmStatFlexDirectionIngress == direction) {
                 if (NULL != ifp_color_map[unit][pipe_num][pool_id]) {
                     soc_cm_sfree(unit, ifp_color_map[unit][pipe_num][pool_id]);
                     ifp_color_map[unit][pipe_num][pool_id] = NULL;
                 }
             }
         }
#endif

         if(flex_base_index_reference_count[unit][direction][pipe_num][pool_id]
            != NULL) {
            sal_free(flex_base_index_reference_count[unit][direction][pipe_num]
                     [pool_id]);
            flex_base_index_reference_count[unit][direction][pipe_num][pool_id]=NULL;
         }
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TRIDENT2X(unit) &&
        soc_feature(unit, soc_feature_advanced_flex_counter)) {
        for (offset_mode = 0; offset_mode < BCM_STAT_FLEX_COUNTER_MAX_MODE;
             offset_mode++) {
            if(_flex_stat_mode_base_map[unit][direction][pool_id][offset_mode]
                                                                    != NULL) {
                sal_free(_flex_stat_mode_base_map[unit][direction][pool_id]
                    [offset_mode]);
                _flex_stat_mode_base_map[unit][direction][pool_id]
                    [offset_mode] = NULL;
            }
        }
    }
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
       if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
           for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                buffer_id++) {
               if (flex_byte_counter_x[unit][direction][pool_id][buffer_id]
                                                                 != NULL) {
                   soc_cm_sfree(unit,
                                flex_byte_counter_x[unit][direction]
                                [pool_id][buffer_id]);
                   flex_byte_counter_x[unit][direction][pool_id][buffer_id]
                       = NULL;
               }
               if (flex_byte_counter_y[unit][direction][pool_id][buffer_id]
                                                                 != NULL) {
                   soc_cm_sfree(unit,
                                flex_byte_counter_y[unit][direction]
                                [pool_id][buffer_id]);
                   flex_byte_counter_y[unit][direction][pool_id][buffer_id]
                       = NULL;
               }
               if (flex_pkt_counter_x[unit][direction][pool_id][buffer_id]
                                                                 != NULL) {
                   soc_cm_sfree(unit,
                                flex_pkt_counter_x[unit][direction]
                                [pool_id][buffer_id]);
                   flex_pkt_counter_x[unit][direction][pool_id][buffer_id]
                       = NULL;
               }
               if (flex_pkt_counter_y[unit][direction][pool_id][buffer_id]
                                                                 != NULL) {
                   soc_cm_sfree(unit,
                                flex_pkt_counter_y[unit][direction]
                                [pool_id][buffer_id]);
                   flex_pkt_counter_y[unit][direction][pool_id][buffer_id]
                       = NULL;
               }
           }
           if (flex_counter_toggle[unit][direction][pool_id] != NULL) {
                   soc_cm_sfree(unit,
                            flex_counter_toggle[unit][direction]
                            [pool_id]);
                   flex_counter_toggle[unit][direction][pool_id] = NULL;
           }
       }
#endif /* BCM_TRIDENT2_SUPPORT*/
    }
    if (local_scache_ptr[unit] != NULL) {
        sal_free(local_scache_ptr[unit]);
        local_scache_ptr[unit] = NULL;
    }
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "OK \n")));
    return BCM_E_NONE;
}

bcm_error_t _bcm_esw_stat_flex_cleanup(int unit)
{
#ifdef BCM_TOMAHAWK_SUPPORT
   int pipe_num = 0;

   if (SOC_IS_TOMAHAWKX(unit)) {
      for(pipe_num = 0; pipe_num < _MAX_PIPES_PER_DEV(unit); ++pipe_num) {
          _bcm_esw_stat_flex_cleanup_pipe(unit, pipe_num, bcmStatFlexDirectionIngress);
      }
      _bcm_esw_stat_flex_cleanup_pipe(unit, pipe_num, bcmStatFlexDirectionEgress);
   } else {
#endif
      _bcm_esw_stat_flex_cleanup_pipe(unit, 0, bcmStatFlexDirectionIngress);
      _bcm_esw_stat_flex_cleanup_pipe(unit, 0, bcmStatFlexDirectionEgress);

#ifdef BCM_TOMAHAWK_SUPPORT
   }
#endif
    if (flex_ingress_modes[unit] != NULL) {
        sal_free(flex_ingress_modes[unit]);
        flex_ingress_modes[unit] = NULL;
    }
    if (flex_custom_ingress_modes[unit] != NULL) {
        sal_free(flex_custom_ingress_modes[unit]);
        flex_custom_ingress_modes[unit] = NULL;
    }
    if (flex_egress_modes[unit] != NULL) {
        sal_free(flex_egress_modes[unit]);
        flex_egress_modes[unit] = NULL;
    }

#if defined(BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
        stat_ctr_map_deinit(unit);
    }
#endif

    return BCM_E_NONE;
}



STATIC bcm_error_t _bcm_esw_stat_flex_object_config_init(int unit, int flag)
{
    int i;
    int count1 = COUNTOF(ing_flex_object_config);
    int count2 = COUNTOF(egr_flex_object_config);

    if (flag == CONFIG_ING_OBJECT) {
        for (i = 0; i < count1; i++) {
            ing_flex_object_config[i].config_pool = INVALID_CONFIG;
            ing_flex_object_config[i].share_criteria = INVALID_CONFIG;
            ing_flex_object_config[i].exclude_criteria = INVALID_CONFIG;
        }
    } else {
        for (i = 0; i < count2; i++) {
            egr_flex_object_config[i].config_pool = INVALID_CONFIG;
            egr_flex_object_config[i].share_criteria = INVALID_CONFIG;
            egr_flex_object_config[i].exclude_criteria = INVALID_CONFIG;
        }    
    }

    return BCM_E_NONE;
}


STATIC bcm_error_t _bcm_esw_stat_flex_object_config_get(
                int unit,
                soc_mem_t mem,
                flex_object_config_t *object_config
                )
{
    int i;
    soc_mem_t config_mem;
    int count_ing = COUNTOF(ing_flex_object_config);
    int count_egr = COUNTOF(egr_flex_object_config);
    int rv1, rv2;

    switch(mem) {
        case MPLS_ENTRYm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case MPLS_ENTRY_EXTDm:
#endif
            config_mem = MPLS_ENTRYm;
            break;
        case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case VLAN_XLATE_EXTDm:
#endif
            config_mem = VLAN_XLATEm;
            break;
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case EXT_IPV4_UCAST_WIDEm:
        case EXT_IPV6_128_UCAST_WIDEm:
            config_mem = L3_ENTRY_2m;
            break;
#if defined(BCM_TRIDENT2_SUPPORT)
        case L3_DEFIPm:
        case L3_DEFIP_PAIR_128m:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64_1m:
        case L3_DEFIP_ALPM_IPV6_128m:
            config_mem = L3_DEFIPm;
            break;
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
            config_mem = L3_ENTRY_IPV4_MULTICASTm;
            break;
#endif
        default:
            config_mem = mem;
            break;
    }

    for (i = 0; i < count_ing; i++) {
        if (config_mem == ing_flex_object_config[i].obj_mem) {
            *object_config = ing_flex_object_config[i];
            rv1 = BCM_E_NONE;
            break;
        }
    }

    if (i >= count_ing) {
        rv1 = BCM_E_NOT_FOUND;
    }

    for (i = 0; i < count_egr; i++) {
        if (config_mem == egr_flex_object_config[i].obj_mem) {
            *object_config = egr_flex_object_config[i];
            rv2 = BCM_E_NONE;
            break;
        }
    }

    if (i >= count_egr) {
        rv2 = BCM_E_NOT_FOUND;
    }

    if ((rv1 == BCM_E_NOT_FOUND) && (rv2 == BCM_E_NOT_FOUND)) {
        object_config = NULL;
        return BCM_E_NOT_FOUND;
    }

    return BCM_E_NONE;
}

STATIC bcm_error_t _bcm_esw_stat_flex_object_config_set(
                int unit,
                soc_mem_t mem,
                flex_object_config_t object_config
                )
{
    int i;
    soc_mem_t config_mem;
    int count_ing = COUNTOF(ing_flex_object_config);
    int count_egr = COUNTOF(egr_flex_object_config);
    int rv1, rv2;

    switch(mem) {
        case MPLS_ENTRYm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case MPLS_ENTRY_EXTDm:
#endif
            config_mem = MPLS_ENTRYm;
            break;
        case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case VLAN_XLATE_EXTDm:
#endif
            config_mem = VLAN_XLATEm;
            break;
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case EXT_IPV4_UCAST_WIDEm:
        case EXT_IPV6_128_UCAST_WIDEm:
            config_mem = L3_ENTRY_2m;
            break;
#if defined(BCM_TRIDENT2_SUPPORT)
        case L3_DEFIPm:
        case L3_DEFIP_PAIR_128m:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64_1m:
        case L3_DEFIP_ALPM_IPV6_128m:
            config_mem = L3_DEFIPm;
            break;
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
            config_mem = L3_ENTRY_IPV4_MULTICASTm;
            break;
#endif
        default:
            config_mem = mem;
            break;
    }

    for (i = 0; i < count_ing; i++) {
        if (config_mem == ing_flex_object_config[i].obj_mem) {
            ing_flex_object_config[i] = object_config;
            rv1 = BCM_E_NONE;
            break;
        }
    }

    if (i >= count_ing) {
        rv1 = BCM_E_NOT_FOUND;
    }

    for (i = 0; i < count_egr; i++) {
        if (config_mem == egr_flex_object_config[i].obj_mem) {
            egr_flex_object_config[i] = object_config;
            rv2 = BCM_E_NONE;
            break;
        }
    }

    if (i >= count_egr) {
        rv2 = BCM_E_NOT_FOUND;
    }    

    if ((rv1 == BCM_E_NOT_FOUND) && (rv2 == BCM_E_NOT_FOUND)) {
        return BCM_E_NOT_FOUND;
    }
    
    return BCM_E_NONE;
}

STATIC bcm_error_t _bcm_esw_stat_flex_object_update(
                int unit,
                SHR_BITDCL *object_bmp,
                int share,
                int flag)
{
    int i, j, num = 0;
    uint32 config_criteria, config_pool = 0;
    soc_mem_t mem;
    int count;
    flex_object_default_t object_default[bcmStatObjectMaxValue];
    flex_object_config_t object_config = {0};

    if (flag == CONFIG_ING_OBJECT) {
        count = COUNTOF(ing_flex_object_default);
        sal_memcpy(object_default, ing_flex_object_default, 
                   count * sizeof(flex_object_default_t));
    } else if (flag == CONFIG_EGR_OBJECT) {
        count = COUNTOF(egr_flex_object_default);
        sal_memcpy(object_default, egr_flex_object_default, 
                   count * sizeof(flex_object_default_t));
    } else {
        return BCM_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        if (SHR_BITGET(object_bmp, i)) {
            num++;
            /*set config pool for sharing*/
            config_pool = object_default[i].pool_number;

            BCM_IF_ERROR_RETURN(
                _bcm_esw_stat_flex_object_config_get(unit,
                                                     object_default[i].obj_mem, 
                                                     &object_config));

            /*already set*/
            if ((share && (object_config.share_criteria != 0)) ||
                (!share && (object_config.exclude_criteria != 0))) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "\"%s\" has already been set or "
                                      "duplicated object in HW\n"), 
                                      object_default[i].obj_desc));
                return BCM_E_INTERNAL;
            }
        }
    }

    if (num <= 1) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Too little objects in the property string. "
                              "Object count %d\n"), 
                              num));
        return BCM_E_INTERNAL;
    }

    for (i = 0; i < count; i++) {
        config_criteria = 0;
        if (SHR_BITGET(object_bmp, i)) {
            mem = object_default[i].obj_mem;
            for (j = 0; j < count; j++) {
                if (j == i) {
                    continue;
                }

                if (SHR_BITGET(object_bmp, j) &&
                    (mem != object_default[j].obj_mem)) {
                    config_criteria |= object_default[j].pool_criteria;
                }
            }

            BCM_IF_ERROR_RETURN(
                _bcm_esw_stat_flex_object_config_get(unit,
                                                     mem, &object_config));

            if (share) {
                object_config.share_criteria |= config_criteria;
                object_config.config_pool = config_pool;
            }
            else {
                object_config.exclude_criteria |= config_criteria;
            }

            BCM_IF_ERROR_RETURN(
                _bcm_esw_stat_flex_object_config_set(unit,
                                                     mem, object_config));

        }
    }

    return BCM_E_NONE;
}


STATIC bcm_error_t _bcm_esw_stat_flex_property_get(
                int unit,
                char *property_str,
                int flag)
{
    char *sub_str = property_str;
    int i, j, share, str_len, share_len, split_len;
    int count;
    flex_object_default_t flex_obj[bcmStatObjectMaxValue];
    SHR_BITDCL *object_bmp = NULL, *object_share = NULL, *object_exclude = NULL;
    int rv = BCM_E_NONE;

    if (flag == CONFIG_ING_OBJECT) {
        count = COUNTOF(ing_flex_object_default);
        sal_memcpy(flex_obj, ing_flex_object_default, 
                   count * sizeof(flex_object_default_t));
    } else if (flag == CONFIG_EGR_OBJECT) {
        count = COUNTOF(egr_flex_object_default);
        sal_memcpy(flex_obj, egr_flex_object_default, 
                   count * sizeof(flex_object_default_t));
    } else {
        rv = BCM_E_PARAM;
        goto cleanup;
    }

    object_bmp = sal_alloc(SHR_BITALLOCSIZE(count), "object");
    if (object_bmp == NULL) {
        rv = BCM_E_MEMORY;
        goto cleanup;
    }
    object_share = sal_alloc(SHR_BITALLOCSIZE(count), "object_share");
    if (object_share == NULL) {
        rv = BCM_E_MEMORY;
        goto cleanup;
    }    
    object_exclude = sal_alloc(SHR_BITALLOCSIZE(count), "object_exclude");
    if (object_exclude == NULL) {
        rv = BCM_E_MEMORY;
        goto cleanup;
    }    
    
    sal_memset(object_share, 0, _SHR_BITDCLSIZE(count) * sizeof(SHR_BITDCL));
    sal_memset(object_exclude, 0, _SHR_BITDCLSIZE(count) * sizeof(SHR_BITDCL));

    share_len = sal_strlen("share");
    split_len = sal_strlen("split");

    /*initialize the object config*/
    BCM_IF_ERROR_RETURN(_bcm_esw_stat_flex_object_config_init(unit, flag));

    /*share(vlan,vfi);split(vlan,ipmc)
     *Specifies ingress objects sharing same pool or using exclusive pool.
     *example:
     *ing_share_flex_counter_pool=split(vlan,vfi) make vlan, vfi use exclusive pool.
     *ing_share_flex_counter_pool=share(vlan,vfi) make vlan, vfi use same pool.
     *an object can appear in both share case and split case.
     *but in each case it can appear one time at most.
     *use token between each groups and use comma between each objects.
     *eg: ing_share_flex_counter_pool=split(A,B,C);share(C,D);
     *some objects may use the same HW table, so only the first one will take effect.
     *all valid object name: ',
     *port, vlan, vlanxlate, vfi, l3intf, vrf, policy, mplsvclabel, mplsswitchlabel, mplsfrrlabel,
     *l3host, trill, mimlookupid, l2gre, extpolicy, vxlan, vsan, fcoe, l3route, niv, ipmc.
     */

    /*Parse the property syntex*/
    while (*sub_str != '\0') {
        if (*sub_str == ';' || *sub_str == ' ') {
            sub_str ++;
            continue;
        }

        /*Initialization*/
        sal_memset(object_bmp, 0, _SHR_BITDCLSIZE(count) * sizeof(SHR_BITDCL));
        i = 0;

        if (!sal_memcmp(sub_str, "share", share_len)) {
            share = TRUE;
            sub_str += share_len;
        }
        else if (!sal_memcmp(sub_str, "split", split_len)) {
            share = FALSE;
            sub_str += split_len;
        }
        else {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ERROR! Missing share information. "
                                  "Soc property \"%s\"\n"), 
                                  property_str));
            rv = BCM_E_FAIL;
            goto cleanup;
        }

        if (*sub_str == '(') {
            sub_str++;
        }
        else {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ERROR! Illegal syntax. Soc property \"%s\"\n"), 
                                  property_str)); 
            rv = BCM_E_FAIL;
            goto cleanup;
        }

        while (i < count) {
            str_len = sal_strlen(flex_obj[i].obj_desc);
            if (!sal_memcmp(sub_str,
                    flex_obj[i].obj_desc, str_len)) {
                SHR_BITSET(object_bmp, i);
                sub_str += str_len;

                if (*sub_str == ')') {
                    /*Check if there are duplicated objects*/
                    for (j = 0; j < count; j++) {
                        if (share) {
                            if (SHR_BITGET(object_bmp, j) &&
                                SHR_BITGET(object_share, j)) {
                                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                          (BSL_META_U(unit,
                                                      "ERROR! Duplicated object. "
                                                      "Soc property \"%s\"\n"), 
                                                      property_str)); 
                                 rv = BCM_E_FAIL;
                                 goto cleanup;
                            }
                        }
                        else {
                            if (SHR_BITGET(object_bmp, j) &&
                                SHR_BITGET(object_exclude, j)) {
                                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                          (BSL_META_U(unit,
                                                      "ERROR! Duplicated object. "
                                                      "Soc property \"%s\"\n"), 
                                                      property_str)); 
                                 rv = BCM_E_FAIL;
                                 goto cleanup;
                            }
                        }
                    }

                    /*Do the further process*/
                    rv = _bcm_esw_stat_flex_object_update(unit, object_bmp, 
                                                          share, flag);
                    if (BCM_FAILURE(rv)) {
                        goto cleanup;
                    }

                    /*save the configuration stat*/
                    if (share) {
                        SHR_BITOR_RANGE
                            (object_bmp, object_share, 0, count, object_share);
                    }
                    else {
                        SHR_BITOR_RANGE
                            (object_bmp, object_exclude, 0, count, object_exclude);
                    }
                    sub_str++;
                    break;
                }
                else if (*sub_str == ',') {
                    /*parse next object*/
                    sub_str ++;
                    i = 0;
                    continue;
                }
                else {
                    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                              (BSL_META_U(unit,
                                          "ERROR! Illegal string information. "
                                          "Soc property \"%s\"\n"), 
                                          property_str));
                    rv = BCM_E_FAIL;
                    goto cleanup;
                }
            }
            i++;
        }

        if(i >= count) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ERROR! Object not found. Soc property \"%s\" \"%s\"\n"),
                                  property_str, sub_str));
            rv = BCM_E_NOT_FOUND;
            goto cleanup;
        }
    }

cleanup:
    if (object_bmp != NULL) {
        sal_free(object_bmp);
    }
    if (object_share != NULL) {
        sal_free(object_share);
    }     
    if (object_exclude != NULL) {
        sal_free(object_exclude);
    }     
    return rv;
}

#ifdef BCM_TOMAHAWK_SUPPORT
/*
 * Function:
 *      is_legacy_ifp_color_mode_configured
 * Description:
 *      Determine whether a "legacy" IFP color based counter has
 *      been configured. When configured, such a mode uses
 *      0 selectors because counter offsets are provided directly
 *      by the IFP_POLICY_TABLE. However the COUNT_ENABLEf in the
 *      ING_FLEX_CTR_OFFSET_TABLE_* table will be set.
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) offset mode
 *      is_configured         - (OUT) set to TRUE/FALSE based on
 *                              whether legacy color based counter
 *                              mode is configured or not
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
static bcm_error_t is_legacy_ifp_color_mode_configured(int unit, uint32 mode,
                                                       uint8 *is_configured) {
    uint32 index = 0, count_enable = 0, alloc_size = 0;
    ing_flex_ctr_offset_table_0_entry_t *flex_ctr_offset_table_entry = NULL;


    if (NULL == is_configured) {
        return BCM_E_PARAM;
    }
    *is_configured = FALSE;
    alloc_size =  BCM_STAT_FLEX_MAX_COUNTER *
                  sizeof(ing_flex_ctr_offset_table_0_entry_t);
    flex_ctr_offset_table_entry = soc_cm_salloc(unit, alloc_size,
                                                "flex_ctr_offset_table_entry");
    if (NULL == flex_ctr_offset_table_entry) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Memory Allocation failed:flex_ctr_offset_table_entry\n")));
        return BCM_E_MEMORY;
    }
    sal_memset(flex_ctr_offset_table_entry, 0, alloc_size);
    /* Since all pools have their corresponding counter offset tables
     * programmed identically, we can examine pool 0 to see if any
     * counters have their COUNT_ENABLEf set, for the given mode.
     */
    if (soc_mem_read_range(unit,
                           _ctr_offset_table[bcmStatFlexDirectionIngress][0],
                           MEM_BLOCK_ANY,
                           (mode << BCM_STAT_FLEX_MAX_PKT_ATTR_SEL_KEY_SIZE),
                           (mode << BCM_STAT_FLEX_MAX_PKT_ATTR_SEL_KEY_SIZE) +
                           BCM_STAT_FLEX_MAX_COUNTER - 1,
                           flex_ctr_offset_table_entry) != SOC_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Memory Reading failed:flex_ctr_offset_table_entry \n")));
        soc_cm_sfree(unit, flex_ctr_offset_table_entry);
        return BCM_E_INTERNAL;
    }

    for (index = 0; index < BCM_STAT_FLEX_MAX_COUNTER; index++) {
         count_enable = 0;
         soc_mem_field_get(unit,
                           _ctr_offset_table[bcmStatFlexDirectionIngress][0],
                           (uint32 *)&flex_ctr_offset_table_entry[index],
                           COUNT_ENABLEf, &count_enable);
         if (count_enable) {
             soc_cm_sfree(unit, flex_ctr_offset_table_entry);
             flex_ctr_offset_table_entry = NULL;
             *is_configured = TRUE;
             break;
         }
    }
    if (NULL != flex_ctr_offset_table_entry) {
        soc_cm_sfree(unit, flex_ctr_offset_table_entry);
    }
    return BCM_E_NONE;
}
#endif
/*
 * Function:
 *      _bcm_esw_stat_flex_init
 * Description:
 *      Initialize and allocate all required flex resources
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_init(int unit)
{
    bcm_stat_flex_direction_t    direction=bcmStatFlexDirectionIngress;
    /* sal_usecs_t stime; */
    uint32    mode=0;

    uint32    selector_count=0;
    uint32    index=0;
    uint32    selector_x_en_field_value[8]={0};
    uint32    selector_for_bit_x_field_value[8]={0};
    uint32    IsAnyModeConfigured=0;
    uint32    pool_id=0;

    uint32    num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32    size_pool[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32    alloc_size=0;
    uint64    selector_key_value;
    int       num_ctr;
#ifdef BCM_WARM_BOOT_SUPPORT
    int       stable_size=0;
    int       stable_used_size=0;
#endif
    bcm_error_t rv=BCM_E_NONE;
    int num_pipes = 1, pipe_num;
#ifdef BCM_WARM_BOOT_SUPPORT
    uint32     per_pipe_mode_ifp = 0, per_pipe_mode_vfp = 0;
#endif

#if defined(BCM_TRIDENT2_SUPPORT)
    int       buffer_id = 0;
    bcm_stat_flex_mode_t offset_mode;
#endif /* BCM_TRIDENT2_SUPPORT*/
  char *config_str;
    uint32 pool_group_id;

#ifdef BCM_WARM_BOOT_SUPPORT
    int used_pool = 0;
#endif

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "_bcm_esw_stat_flex_init \n")));
    _bcm_esw_stat_flex_init_pkt_attr_bits(unit);
    _bcm_esw_stat_flex_init_pkt_res_fields(unit);

    _bcm_esw_stat_flex_cleanup(unit);

    COMPILER_64_ZERO(selector_key_value);

#if defined(BCM_TOMAHAWK_SUPPORT)
    /*IFP supports dynamic switching of modes.
      Always allocate memory for 4 pipes. 
     */
    if (SOC_IS_TOMAHAWKX(unit)) {
        num_pipes = SOC_INFO(unit).num_pipe;
    }

#endif
    num_pools[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            num_flex_egress_pools;
    size_pool[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            size_flex_egress_pool;

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "INFO: ingress_pools:%d num_flex_egress_pools:%di "
                          "size_flex_ingress_pool:%d size_flex_egress_pool:%d"
                          "MAX_DIRECTION %d MAX_MODE %d MAX_POOL %d\n"),
               num_pools[bcmStatFlexDirectionIngress],
               num_pools[bcmStatFlexDirectionEgress],
               size_pool[bcmStatFlexDirectionIngress],
               size_pool[bcmStatFlexDirectionEgress],
               BCM_STAT_FLEX_COUNTER_MAX_DIRECTION,
               BCM_STAT_FLEX_COUNTER_MAX_MODE,
               BCM_STAT_FLEX_COUNTER_MAX_POOL));
    if ((num_pools[bcmStatFlexDirectionIngress] == 0) ||
        (num_pools[bcmStatFlexDirectionEgress] == 0)) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "INFO:Number of CounterPools missing.PleaseDefine it\n")));
         return BCM_E_INTERNAL;
    }
    if ((num_pools[bcmStatFlexDirectionIngress] >
                          ((SOC_IS_TOMAHAWKX(unit))? BCM_STAT_TH_FLEX_COUNTER_MAX_POOL: 
                                                    BCM_STAT_FLEX_COUNTER_MAX_POOL)) ||
        (num_pools[bcmStatFlexDirectionEgress] >
         BCM_STAT_FLEX_COUNTER_MAX_POOL)) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "INFO: Number of pools exceeding its max value %d \n"),
                    BCM_STAT_FLEX_COUNTER_MAX_POOL));
         return BCM_E_INTERNAL;
    }
    if (flex_stat_mutex[unit] == NULL) {
        flex_stat_mutex[unit]= sal_mutex_create("Advanced flex counter mutex");
        if (flex_stat_mutex[unit] == NULL) {
            _bcm_esw_stat_flex_cleanup(unit);
            return BCM_E_MEMORY;
        }
    }
    flex_ingress_modes[unit] = (bcm_stat_flex_ingress_mode_t *) sal_alloc(
                                sizeof(bcm_stat_flex_ingress_mode_t) *
                                BCM_STAT_FLEX_COUNTER_MAX_MODE,
                                "flex_ingress_modes");
    if (flex_ingress_modes[unit] == NULL) {
        _bcm_esw_stat_flex_cleanup(unit);
        return BCM_E_MEMORY;
    }
    flex_custom_ingress_modes[unit] = (bcm_stat_flex_custom_ingress_mode_t *) sal_alloc(
                                sizeof(bcm_stat_flex_custom_ingress_mode_t) *
                                BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE,
                                "flex_ingress_modes");
    if (flex_custom_ingress_modes[unit] == NULL) {
        _bcm_esw_stat_flex_cleanup(unit);
        return BCM_E_MEMORY;
    }
    flex_egress_modes[unit] = (bcm_stat_flex_egress_mode_t *) sal_alloc(
                                sizeof(bcm_stat_flex_egress_mode_t) *
                                BCM_STAT_FLEX_COUNTER_MAX_MODE,
                                "flex_egress_modes");
    if (flex_egress_modes[unit] == NULL) {
        _bcm_esw_stat_flex_cleanup(unit);
        return BCM_E_MEMORY;
    }

#if defined(BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
        stat_ctr_map_init(unit);
    }
#endif

    /* Initialize Ingress Mode structures*/
    for (mode = 0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE; mode++) {
         sal_memset(&flex_ingress_modes[unit][mode],0,
                    sizeof(bcm_stat_flex_ingress_mode_t));
    }
    /* Initialize Custom Ingress Mode structures*/
    for (mode = 0; mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE-BCM_CUSTOM_INGRESS_MODE_START; mode++) {
         sal_memset(&flex_custom_ingress_modes[unit][mode],0,
                    sizeof(bcm_stat_flex_custom_ingress_mode_t));
         flex_custom_ingress_modes[unit][mode].AllocPool = -1;
    }
    /* Initialize Egress Mode structures*/
    for (mode = 0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE; mode++) {
         sal_memset(&flex_egress_modes[unit][mode],0,
                    sizeof(bcm_stat_flex_egress_mode_t));
    }
    for (direction = bcmStatFlexDirectionIngress;
         direction < bcmStatFlexDirectionCount;
         direction++) {
         alloc_size = sizeof(ing_flex_ctr_counter_table_0_entry_t) *
                             size_pool[direction];
#if defined(BCM_TRIDENT2_SUPPORT)
         if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
             if (flex_temp_counter_dual[unit][direction] != NULL){
                 LOG_WARN(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "WARN:Freeing flex_temp_counter AllocatedMemory\n")));
                 soc_cm_sfree(unit,flex_temp_counter_dual[unit][direction]);
             }
             flex_temp_counter_dual[unit][direction] = soc_cm_salloc(
                                                unit,
                                                alloc_size,
                                                "Advanced FlexTemp packet counter");
             if (flex_temp_counter_dual[unit][direction] == NULL) {
                 LOG_WARN(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Advanced Flex current PacketCounterAllocationFail"
                                      "for unit:%d, dir:%d pool:%d\n"),
                           unit,direction,pool_id));
                 _bcm_esw_stat_flex_cleanup(unit);
                 return BCM_E_MEMORY;
             }
             sal_memset(flex_temp_counter_dual[unit][direction], 0, alloc_size);
         }
#endif /* BCM_TRIDENT2_SUPPORT*/

         if (flex_temp_counter[unit][direction] != NULL){
             LOG_WARN(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "WARN:Freeing flex_temp_counter AllocatedMemory\n")));
             soc_cm_sfree(unit,flex_temp_counter[unit][direction]);
         }
         flex_temp_counter[unit][direction] = soc_cm_salloc(
                                            unit,
                                            alloc_size,
                                            "Advanced FlexTemp packet counter");
         if (flex_temp_counter[unit][direction] == NULL) {
             LOG_WARN(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Advanced Flex current PacketCounterAllocationFail"
                                  "for unit:%d, dir:%d pool:%d\n"),
                       unit,direction,pool_id));
             _bcm_esw_stat_flex_cleanup(unit);
             return BCM_E_MEMORY;
         }
         sal_memset(flex_temp_counter[unit][direction], 0, alloc_size);
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Temp counter size:%d \n"),
                    alloc_size));
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Byte counter size:%d \n"),
                    (int)sizeof(uint64)*size_pool[direction]));
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Packet counter size:%d\n"),
                    (int)sizeof(uint32)*size_pool[direction]));
         /* Initialize the Flex Counter Group Database */
         if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
             if (direction == bcmStatFlexDirectionIngress) {
                 for (pool_group_id=0;
                      pool_group_id < _bcm_esw_get_max_pool_groups(unit);
                      pool_group_id++) {
                     if (BCM_E_PARAM == 
                             _bcm_esw_stat_flex_oam_enable_ingress_group(
                                                                unit,
                                                                pool_group_id,
                                                                0)) {
                          continue;
                      }
                     for(pipe_num = 0; pipe_num < num_pipes; ++pipe_num) {
                         flex_pool_group[unit][pipe_num][pool_group_id].group_type =
                                       BCM_FLEX_POOL_GROUP_USED_BY_NONE;
                         flex_pool_group[unit][pipe_num][pool_group_id].free_pools  =
                                       BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
                         flex_pool_group[unit][pipe_num][pool_group_id].base_pool =
                              pool_group_id * BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
                     }
                 }
             }
         }
         for (pool_id = 0; pool_id < num_pools[direction]; pool_id++) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   ".")));
             /* Disable all counter pools */
             _bcm_esw_stat_flex_enable_pool(
                     unit,
                     direction,
                     _pool_ctr_register[direction][pool_id],
                 0);
             for(pipe_num = 0; pipe_num < num_pipes; ++pipe_num) {
                 if ((pipe_num > 0) && (direction != bcmStatFlexDirectionIngress)) {
                    break;
                 }
                  /* Destroy if any exist */
                  shr_aidxres_list_destroy(flex_aidxres_list_handle[unit]
                                           [direction][pipe_num][pool_id]);

                  /* use the least one of the configured pool size and mem table */
                  num_ctr = soc_mem_index_count(unit,
                                   _ctr_counter_table[direction][pool_id]);
                  if (size_pool[direction] < num_ctr) {
                      num_ctr = size_pool[direction];
                  }

                  /* Create it */
                  if (shr_aidxres_list_create(
                         &flex_aidxres_list_handle[unit][direction][pipe_num][pool_id],
                         0,num_ctr-1,
                         0,num_ctr-1,
                         8, /* Max 256 counters */
                         "flex-counter") != BCM_E_NONE) {
                      LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Unrecoverable error. "
                                            "Couldn'tCreate AllignedList:FlexCounter\n")));
                      _bcm_esw_stat_flex_cleanup(unit);
                      return BCM_E_INTERNAL;
                  }
                  /* Reserver first two counters entries i.e. not to be used. */
                  /* Flexible counter updates only if counter_base_index is != 0 */
                  /* Refer: Arch-spec section 4.1 */
                  shr_aidxres_list_reserve_block(
                              flex_aidxres_list_handle[unit][direction][pipe_num][pool_id],
                              0,
                              (1 << 1));
                  flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_tables = 0;
                  flex_pool_stat[unit][direction][pipe_num][pool_id].used_entries = 0;
                  flex_pool_stat[unit][direction][pipe_num][pool_id].attached_entries = 0;
                  SHR_BITCLR_RANGE(flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_objects,
                                   0, bcmStatObjectMaxValue);
#if defined(BCM_TOMAHAWK_SUPPORT)
                  if (SOC_IS_TOMAHAWKX(unit)) {
                      /* Ingress pools 12 - 20 are reserved for use by IFP only.
                       * Update pool attributes to reflect the same.
                       */
                      if (bcmStatFlexDirectionIngress == direction &&
                          (pool_id >= BCM_FLEX_STAT_TH_RSVD_IFP_POOL_MIN &&
                           pool_id <= BCM_FLEX_STAT_TH_RSVD_IFP_POOL_MAX)) {
                          flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_tables =
                          FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
                          SHR_BITSET(flex_pool_stat[unit][direction][pipe_num][pool_id].used_by_objects,
                                      bcmStatObjectIngFieldStageIngress);
                      }
                      /* Allocate memory for the ifp color map database
                       * for the unit being initialized. This color map
                       * database is referred to at the time of attaching
                       * flex counters to IFP_POLICY_TABLE, in order to
                       * determine how to program the color based offset
                       * fields
                       */
                      if (bcmStatFlexDirectionIngress == direction) {
                          if (ifp_color_map[unit][pipe_num][pool_id] != NULL) {
                              LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                       (BSL_META_U(unit,
                                                   "WARN:Freeing Allocated IFP"
                                                   " color map database\n")));
                              soc_cm_sfree(unit, ifp_color_map[unit][pipe_num][pool_id]);
                          }
                          ifp_color_map[unit][pipe_num][pool_id] =
                               soc_cm_salloc(unit, sizeof(uint8) * num_ctr,
                                             "IFP Color map database");
                          if (NULL == ifp_color_map[unit][pipe_num][pool_id]) {
                              LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                        (BSL_META_U(unit,
                                                    "Allocation failed for IFP"
                                                    " color map database\n")));
                              _bcm_esw_stat_flex_cleanup(unit);
                              return BCM_E_MEMORY;
                          }
                          sal_memset(ifp_color_map[unit][pipe_num][pool_id], 0,
                                     sizeof(uint8) * num_ctr);
                      }
                  }
#endif
                  if (flex_byte_counter[unit][direction][pipe_num][pool_id] != NULL) {
                      LOG_WARN(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "WARN:Freeing AllocatedByteCountersMemory\n")));
                      soc_cm_sfree(unit,
                                   flex_byte_counter[unit][direction][pipe_num][pool_id]);
                  }
                  flex_byte_counter[unit][direction][pipe_num][pool_id] =
                       soc_cm_salloc(unit, sizeof(uint64) * num_ctr,
                                     "Advanced Flex byte counter");
                  if (flex_byte_counter[unit][direction][pipe_num][pool_id] == NULL) {
                      LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Advanced Flex ByteCounterAllocationFailed for"
                                            "unit:%d,dir:%d pool:%d:pipe:%d\n"),
                                 unit, direction, pool_id,pipe_num));
                      _bcm_esw_stat_flex_cleanup(unit);
                      return BCM_E_MEMORY;
                  }
                  sal_memset(flex_byte_counter[unit][direction][pipe_num][pool_id], 0,
                             sizeof(uint64) * num_ctr);
    
                  if (flex_packet_counter[unit][direction][pipe_num][pool_id] != NULL) {
                      LOG_WARN(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "WARN:FreeingAllocatedPacketCountersMemory\n")));
                      soc_cm_sfree(unit,
                                   flex_packet_counter[unit][direction][pipe_num][pool_id]);
                  }
                  flex_packet_counter[unit][direction][pipe_num][pool_id] =
                       soc_cm_salloc(unit, sizeof(uint32) * num_ctr,
                                     "Advanced Flex packet counter");
                  if (flex_packet_counter[unit][direction][pipe_num][pool_id] == NULL) {
                      LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Advanced Flex ByteCounterAllocation failed for"
                                            "unit:%d, dir:%d pool:%d\n"),
                                 unit, direction, pool_id));
                      _bcm_esw_stat_flex_cleanup(unit);
                      return BCM_E_MEMORY;
                  }
                  sal_memset(flex_packet_counter[unit][direction][pipe_num][pool_id], 0,
                             sizeof(uint32) * num_ctr);
    
                  if (flex_packet64_counter[unit][direction][pipe_num][pool_id] != NULL) {
                      LOG_WARN(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "WARN:FreeingAllocatedPacketCountersMemory\n")));
                      soc_cm_sfree(unit,
                                   flex_packet64_counter[unit][direction][pipe_num][pool_id]);
                  }
                  flex_packet64_counter[unit][direction][pipe_num][pool_id] =
                       soc_cm_salloc(unit, sizeof(uint64) * num_ctr,
                                     "Advanced Flex packet counter");
                  if (flex_packet64_counter[unit][direction][pipe_num][pool_id] == NULL) {
                      LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Advanced Flex ByteCounterAllocation failed for"
                                            "unit:%d, dir:%d pool:%d\n"),
                                 unit, direction, pool_id));
                      _bcm_esw_stat_flex_cleanup(unit);
                      return BCM_E_MEMORY;
                  }
                  sal_memset(flex_packet64_counter[unit][direction][pipe_num][pool_id], 0,
                             sizeof(uint64) * num_ctr);
    
#ifdef BCM_TOMAHAWK_SUPPORT
                  if (SOC_IS_TOMAHAWKX(unit)) {
                      if (residual_packet_counter[unit][direction][pipe_num][pool_id] !=
                          NULL) {
                          LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "WARN:Free residual pkt counter mem\n")));
                          soc_cm_sfree(unit,
                                       residual_packet_counter[unit][direction][pipe_num][pool_id]);
                      }
                      residual_packet_counter[unit][direction][pipe_num][pool_id] =
                           soc_cm_salloc(unit, sizeof(uint64) * num_ctr,
                                         "Advanced Flex packet counter");
                      if (residual_packet_counter[unit][direction][pipe_num][pool_id] ==
                          NULL) {
                          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                    (BSL_META_U(unit,
                                                "residual pkt counter Allocation failed for"
                                                "unit:%d, dir:%d pool:%d\n"),
                                     unit,direction, pool_id));
                          _bcm_esw_stat_flex_cleanup(unit);
                          return BCM_E_MEMORY;
                      }
                      sal_memset(residual_packet_counter[unit][direction][pipe_num][pool_id],
                                 0, sizeof(uint64) * num_ctr);

                      if (residual_byte_counter[unit][direction][pipe_num][pool_id] != NULL) {
                          LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "WARN:Free residual byte counter mem\n")));
                          soc_cm_sfree(unit,
                                       residual_byte_counter[unit][direction]
                                       [pipe_num][pool_id]);
                      }
                      residual_byte_counter[unit][direction][pipe_num][pool_id] =
                           soc_cm_salloc(unit,sizeof(uint64) * num_ctr,
                                         "residual byte counter");
                      if (residual_byte_counter[unit][direction][pipe_num][pool_id] == NULL) {
                          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                    (BSL_META_U(unit,
                                                "residual ByteCounterAllocationFailed for"
                                                "unit:%d,dir:%d pool:%d:\n"),
                                     unit, direction, pool_id));
                          _bcm_esw_stat_flex_cleanup(unit);
                          return BCM_E_MEMORY;
                      }
                      sal_memset(residual_byte_counter[unit][direction][pipe_num][pool_id], 0,
                                 sizeof(uint64) * num_ctr);
                  }
#endif

                  if (flex_base_index_reference_count[unit][direction]
                                                     [pipe_num][pool_id] != NULL) {
                      LOG_WARN(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "WARN:Freeing Allocated"
                                            "flex_base_index_reference_count memory\n")));
                      sal_free(flex_base_index_reference_count[unit]
                                                              [direction][pipe_num][pool_id]);
                  }
                  flex_base_index_reference_count[unit][direction][pipe_num][pool_id] =
                       sal_alloc(sizeof(uint16) * num_ctr,
                                 "BaseIndexAllocation");
                  if (flex_packet_counter[unit][direction][pipe_num][pool_id] == NULL){
                      LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "BaseIndex allocation failed for"
                                            "unit:%d, dir:%d pool:%d\n"),
                                 unit,direction, pool_id));
                      _bcm_esw_stat_flex_cleanup(unit);
                      return BCM_E_MEMORY;
                  }
                  sal_memset(flex_base_index_reference_count[unit][direction]
                             [pipe_num][pool_id], 0, sizeof(uint16) * num_ctr);

#if defined(BCM_TRIDENT2_SUPPORT)
                  if (SOC_IS_TRIDENT2X(unit) &&
                      soc_feature(unit, soc_feature_advanced_flex_counter)) {
                      for (offset_mode = 0; offset_mode < BCM_STAT_FLEX_COUNTER_MAX_MODE;
                           offset_mode++) {
                          if (_flex_stat_mode_base_map[unit][direction]
                              [pool_id][offset_mode] != NULL) {
                              LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                       (BSL_META_U(unit, "WARN:Freeing Allocated"
                                            "_flex_stat_mode_base_map memory\n")));
                              sal_free(_flex_stat_mode_base_map[unit][direction]
                                       [pool_id][offset_mode]);
                          }
                          _flex_stat_mode_base_map[unit][direction]
                              [pool_id][offset_mode] = sal_alloc(sizeof(uint8) * num_ctr,
                              "ModeBaseMapAllocation");
                          if (_flex_stat_mode_base_map[unit][direction]
                                  [pool_id][offset_mode] == NULL){
                              LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                        (BSL_META_U(unit,
                                                    "ModeBaseMap allocation failed for"
                                                    "unit:%d, dir:%d pool:%d\n"),
                                                    unit,direction, pool_id));
                              _bcm_esw_stat_flex_cleanup(unit);
                              return BCM_E_MEMORY;
                          }
                          sal_memset(_flex_stat_mode_base_map[unit][direction]
                                     [pool_id][offset_mode], 0, sizeof(uint8) * num_ctr);
                      }
                  }
#endif
             } /* PIPE */
#if defined(BCM_TRIDENT2_SUPPORT)
                if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                    for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                         buffer_id++) {
                        if (flex_byte_counter_x[unit][direction][pool_id][buffer_id]
                                                                          != NULL) {
                            LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "WARN:Freeing Allocated Byte Counters"
                                                 " Memory\n")));
                            soc_cm_sfree(unit,
                                         flex_byte_counter_x[unit][direction]
                                         [pool_id][buffer_id]);
                        }
                        flex_byte_counter_x[unit][direction][pool_id][buffer_id] =
                             soc_cm_salloc(unit, sizeof(uint64) * size_pool[direction],
                                           "Advanced Flex byte counter x");
                        if (flex_byte_counter_x[unit][direction][pool_id][buffer_id]
                                                                          == NULL) {
                             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                       (BSL_META_U(unit,
                                                   "Advanced Flex ByteCounterAllocation"
                                                   " Failed "
                                                   "for unit:%d,dir:%d pool:%d buff:%d:\n"),
                                        unit, direction, pool_id, buffer_id));
                             _bcm_esw_stat_flex_cleanup(unit);
                             return BCM_E_MEMORY;
                        }
                        sal_memset(flex_byte_counter_x[unit][direction][pool_id]
                                   [buffer_id], 0, sizeof(uint64) * size_pool[direction]);
    
                        if (flex_byte_counter_y[unit][direction][pool_id][buffer_id]
                                                                          != NULL) {
                            LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "WARN:Freeing Allocated Byte Counters"
                                                 " Memory\n")));
                            soc_cm_sfree(unit,
                             flex_byte_counter_y[unit][direction][pool_id][buffer_id]);
                        }
                        flex_byte_counter_y[unit][direction][pool_id][buffer_id] =
                              soc_cm_salloc(unit, sizeof(uint64) * size_pool[direction],
                                            "Advanced Flex byte counter y");
                        if (flex_byte_counter_y[unit][direction][pool_id][buffer_id]
                                                                          == NULL) {
                            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                      (BSL_META_U(unit,
                                                  "Advanced Flex ByteCounterAllocationFailed"
                                                  " for unit:%d,dir:%d pool:%d: buff:%d:\n"),
                                       unit, direction, pool_id, buffer_id));
                            _bcm_esw_stat_flex_cleanup(unit);
                            return BCM_E_MEMORY;
                        }
                        sal_memset(flex_byte_counter_y[unit][direction][pool_id]
                                   [buffer_id], 0, sizeof(uint64) * size_pool[direction]);
    
                        if (flex_pkt_counter_x[unit][direction][pool_id][buffer_id]
                                                                          != NULL) {
                            LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "WARN:Freeing AllocatedByteCountersMemory"
                                                 "\n")));
                            soc_cm_sfree(unit,
                                         flex_pkt_counter_x[unit][direction]
                                         [pool_id][buffer_id]);
                        }
                        flex_pkt_counter_x[unit][direction][pool_id][buffer_id] =
                             soc_cm_salloc(unit, sizeof(uint32) * size_pool[direction],
                                           "Advanced Flex packet counter x");
                        if (flex_pkt_counter_x[unit][direction][pool_id][buffer_id]
                                                                          == NULL) {
                            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                      (BSL_META_U(unit,
                                                  "Advanced Flex ByteCounterAllocationFailed"
                                                  " for unit:%d,dir:%d pool:%d: buff:%d:\n"),
                                       unit, direction, pool_id, buffer_id));
                            _bcm_esw_stat_flex_cleanup(unit);
                            return BCM_E_MEMORY;
                        }
                        sal_memset(flex_pkt_counter_x[unit][direction][pool_id]
                                   [buffer_id], 0, sizeof(uint32) * size_pool[direction]);
    
                        if (flex_pkt_counter_y[unit][direction][pool_id][buffer_id]
                                                                          != NULL) {
                            LOG_WARN(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "WARN:Freeing AllocatedByteCountersMemory"
                                                 "\n")));
                            soc_cm_sfree(unit,
                                         flex_pkt_counter_y[unit][direction]
                                         [pool_id][buffer_id]);
                        }
                        flex_pkt_counter_y[unit][direction][pool_id][buffer_id] =
                             soc_cm_salloc(unit, sizeof(uint32) * size_pool[direction],
                                           "Advanced Flex packet counter y");
                        if (flex_pkt_counter_y[unit][direction][pool_id][buffer_id]
                                                                          == NULL) {
                            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                      (BSL_META_U(unit,
                                                  "Advanced Flex ByteCounterAllocationFailed"
                                                  " for unit:%d,dir:%d pool:%d: buff:%d:\n"),
                                       unit, direction, pool_id, buffer_id));
                            _bcm_esw_stat_flex_cleanup(unit);
                            return BCM_E_MEMORY;
                        }
                        sal_memset(flex_pkt_counter_y[unit][direction][pool_id]
                                   [buffer_id], 0, sizeof(uint32) * size_pool[direction]);
                    }

                    if (flex_counter_toggle[unit][direction][pool_id] != NULL) {
                        LOG_WARN(BSL_LS_BCM_FLEXCTR,
                            (BSL_META_U(unit,
                                "WARN:Freeing AllocatedToggleMemory"
                                "\n")));
                        soc_cm_sfree(unit,
                          flex_counter_toggle[unit][direction][pool_id]);
                    }
                    flex_counter_toggle[unit][direction][pool_id] =
                        soc_cm_salloc(unit, sizeof(uint8) * size_pool[direction],
                            "Advanced Flex counter toggle");
                    if (flex_counter_toggle[unit][direction][pool_id]== NULL) {
                        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                            (BSL_META_U(unit,
                                "Advanced Flex Counter Toggle AllocationFailed"
                                " for unit:%d,dir:%d pool:%d:\n"),
                                unit, direction, pool_id));
                        _bcm_esw_stat_flex_cleanup(unit);
                        return BCM_E_MEMORY;
                    }
                    sal_memset(flex_counter_toggle[unit][direction][pool_id],
                        0, sizeof(uint8) * size_pool[direction]);
                }
#endif /* BCM_TRIDENT2_SUPPORT*/
         }
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "\n")));
    }

    /* First retrieve mode related information */
    if (SOC_WARM_BOOT(unit)) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "WARM booting...")));
        for (direction = bcmStatFlexDirectionIngress;
             direction < bcmStatFlexDirectionCount;
             direction++) {
             for (mode = 0; mode < BCM_STAT_FLEX_COUNTER_MAX_MODE; mode++) {
                if (SOC_REG_IS_64(unit,
                                  _pkt_selector_key_reg[direction][mode])) {
                    SOC_IF_ERROR_RETURN(soc_reg64_get(
                      unit,
                      _pkt_selector_key_reg[direction][mode],
                      REG_PORT_ANY,
                      0,
                                &selector_key_value));
                 for (selector_count=0,index=0;index<8;index++) {
                      selector_x_en_field_value[index]=
                                 soc_reg64_field32_get(
                                     unit,
                                    _pkt_selector_key_reg[direction][mode],
                                     selector_key_value,
                                     _pkt_selector_x_en_field_name[index]);
                      selector_count +=selector_x_en_field_value[index];
                      selector_for_bit_x_field_value[index]=0;
                      if (selector_x_en_field_value[index] != 0) {
                          selector_for_bit_x_field_value[index]=
                          soc_reg64_field32_get(
                              unit,
                              _pkt_selector_key_reg[direction][mode],
                              selector_key_value,
                              _pkt_selector_for_bit_x_field_name[index]);
                      }
                 }
                 if (selector_count > 0) {
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Selector count :%d .. "),
                                selector_count));
                     IsAnyModeConfigured=1;
                     if (soc_reg64_field32_get(
                             unit,
                             _pkt_selector_key_reg[direction][mode],
                             selector_key_value,USE_UDF_KEYf) == 1) {
                         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Direction:%d-Mode:%d UDF MODE is "
                                                "configured \n"),direction,mode));
                         _bcm_esw_stat_flex_init_udf_mode(
                                  unit,
                                  mode,
                                  direction,
                                  selector_count,
                                  selector_x_en_field_value,
                                  selector_for_bit_x_field_value);
                     } else {
                         if (soc_reg64_field32_get(
                             unit,
                             _pkt_selector_key_reg[direction][mode],
                             selector_key_value,USE_COMPRESSED_PKT_KEYf) == 1) {
                             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                       (BSL_META_U(unit,
                                                   "Direction:%d-Mode:%d"
                                                    "COMPRESSED MODE is "
                                                    "configured \n"),direction,mode));
                             _bcm_esw_stat_flex_init_cmprsd_mode(
                                      unit,
                                      mode,
                                      direction,
                                      selector_count,
                                      selector_x_en_field_value,
                                      selector_for_bit_x_field_value);
                         } else {
                             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                       (BSL_META_U(unit,
                                                   "Direction:%d-Mode:%d"
                                                    "UNCOMPRESSED MODE is "
                                                    "configured \n"),direction,mode));
                             _bcm_esw_stat_flex_init_uncmprsd_mode(
                                      unit,
                                      mode,
                                      direction,
                                      selector_count,
                                      selector_x_en_field_value,
                                      selector_for_bit_x_field_value);
                         }
                     }
                 } else {
#ifdef BCM_TOMAHAWK_SUPPORT
                     /* Ensure current mode is not being used for "legacy" IFP
                      * color based counters where packet attribute selectors are
                      * not used and counter offsets are provided directly by the
                      * ifp_policy_table
                      */
                     if (SOC_IS_TOMAHAWKX(unit)) {
                         if (bcmStatFlexDirectionIngress == direction) {
                             bcm_error_t rv;
                             uint8 is_configured;

                             rv = is_legacy_ifp_color_mode_configured(unit, mode,
                                                                      &is_configured);
                             if ((BCM_E_NONE == rv) && (TRUE == is_configured)) {
                                 IsAnyModeConfigured = 1;
                                 _bcm_esw_stat_flex_init_uncmprsd_mode(
                                          unit,
                                          mode,
                                          direction,
                                          selector_count,
                                          selector_x_en_field_value,
                                          selector_for_bit_x_field_value);
                             }
                         }
                     }
#endif
                     LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                               (BSL_META_U(unit,
                                           "Direction:%d-Mode:%d is unconfigured \n"),
                                direction,mode));
                 }
             }
        }
    }
    }
    /* stime = sal_time_usecs(); */
    if (IsAnyModeConfigured) {
        /* Get Ingress table info */

        /* 1) Ingress: Port Table */
        _bcm_esw_stat_flex_check_ingress_port_table(unit);
        /* LOG_CLI((BSL_META_U(unit,
                               "FLEX: 1.Taken Time: %10d usec\n "),
                    SAL_USECS_SUB(sal_time_usecs(), stime))); */

        /* 2) Ingress: VLAN_XLATE Table */
        _bcm_esw_stat_flex_check_ingress_vlan_xlate_table(unit);
        /* 4) Ingress: MPLS_ENTRY Table */
        _bcm_esw_stat_flex_check_ingress_mpls_entry_table(unit);
        /* 5) Ingress: SOURCE_VP Table */
        _bcm_esw_stat_flex_check_ingress_source_vp_table(unit);
        /* 6) Ingress: L3_IIF */
        _bcm_esw_stat_flex_check_ingress_l3_iif_table(unit);
        /* 7) Ingress: VFI */
        _bcm_esw_stat_flex_check_ingress_vfi_table(unit);
        /* 8) Ingress: VRF */
        _bcm_esw_stat_flex_check_ingress_vrf_table(unit);
        /* 9) Ingress: VLAN */
        _bcm_esw_stat_flex_check_ingress_vlan_table(unit);
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit)) {
            /* 10) Ingress: L3 Routing Table */
            _bcm_esw_stat_flex_check_ingress_l3_routing_table(unit);
        }
#endif

        /* Get Egress table info */

        /* 1) EGR_L3_NEXT_HOP */
        _bcm_esw_stat_flex_check_egress_l3_next_hop_table(unit);
        /* 2) EGR_VFI */
        _bcm_esw_stat_flex_check_egress_vfi_table(unit);
        /* 3) EGR_PORT */
        _bcm_esw_stat_flex_check_egress_port_table(unit);
        /* 4) EGR_VLAN */
        _bcm_esw_stat_flex_check_egress_vlan_table(unit);
        /* 5) EGR_VLAN_XLATE */
        _bcm_esw_stat_flex_check_egress_vlan_xlate_table(unit);
        /* 6) EGR_NAT_PACKET_EDIT_INFO */
        _bcm_esw_stat_flex_check_egress_nat_pkt_edit_table(unit);
   
        for (direction=bcmStatFlexDirectionIngress;direction<2;direction++) {
             for (mode=0;mode<BCM_STAT_FLEX_COUNTER_MAX_MODE;mode++) {
                  _bcm_esw_stat_flex_retrieve_group_mode(unit, direction,mode);
             }
        }
    }

    if (!soc_feature(unit, soc_feature_centralized_counter)) {
        /* Do Callback registration only for Legacy counter collection model
         * and not for centralized counter collection model.
         * In centralized model, Counter module takes care of collection
         */
        soc_counter_extra_register(unit, _bcm_esw_stat_flex_callback);
    }

    config_str = soc_property_get_str(unit, spn_ING_SHARE_FLEX_COUNTER_POOL);
    if (config_str != NULL) {
        rv = _bcm_esw_stat_flex_property_get(unit, config_str, CONFIG_ING_OBJECT);
        if (rv != BCM_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ERR: Parsing property string(ing) failed")));
            _bcm_esw_stat_flex_cleanup(unit);
            return rv;
        }
    }
    config_str = soc_property_get_str(unit, spn_EGR_SHARE_FLEX_COUNTER_POOL);
    if (config_str != NULL) {
        rv = _bcm_esw_stat_flex_property_get(unit, config_str, CONFIG_EGR_OBJECT);
        if (rv != BCM_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "ERR: Parsing property string(egr) failed")));
            _bcm_esw_stat_flex_cleanup(unit);
            return rv;
        }
    }

    if(local_scache_ptr[unit] != NULL) {
       LOG_WARN(BSL_LS_BCM_FLEXCTR,
                (BSL_META_U(unit,
                            "WARN: Freeing flex_scache_ptr existing memory")));
       sal_free(local_scache_ptr[unit]);
    }
    local_scache_size[unit] = sizeof(uint32)*BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE;
    local_scache_ptr[unit] = sal_alloc(local_scache_size[unit],"Flex scache memory");
    if (local_scache_ptr[unit]  == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "ERR: Couldnot allocate flex_scache_ptr existing memory")));
        _bcm_esw_stat_flex_cleanup(unit);
        return BCM_E_MEMORY;
    }
    sal_memset(local_scache_ptr[unit], 0, local_scache_size[unit]);
#ifdef BCM_WARM_BOOT_SUPPORT
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "WARM Booting... \n")));
    /* rv = soc_scache_recover(unit); */
    flex_scache_allocated_size[unit] = 0;
    SOC_SCACHE_HANDLE_SET(handle[unit], unit, BCM_MODULE_STAT,
                          _BCM_STAT_WARM_BOOT_CHUNK_FLEX);
    SOC_SCACHE_MODULE_MAX_PARTITIONS_SET(unit, BCM_MODULE_STAT, 1);
    if (!SOC_WARM_BOOT(unit)) {
        local_scache_size[unit] +=  sizeof(bcm_stat_flex_group_mode_related_info_t) *
                                    BCM_STAT_FLEX_COUNTER_MAX_MODE *
                                    BCM_STAT_FLEX_COUNTER_MAX_DIRECTION;

        if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
                    (BCM_STAT_WB_DEFAULT_VERSION >= BCM_WB_VERSION_1_4)) {
            local_scache_size[unit] +=  sizeof(bcm_stat_flex_custom_mode_related_info_t) *
                                        (BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE -
                                        BCM_CUSTOM_INGRESS_MODE_START) + 2*sizeof(uint32);
            local_scache_size[unit] +=
                                    sizeof(bcm_stat_flex_counter_id_related_info_t) *
                                    (BCM_MAX_STAT_COUNTER_IDS-1);
        }
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "ColdBoot: Modified Local Scache Size %d \n"),
                   local_scache_size[unit]));
        SOC_IF_ERROR_RETURN(soc_stable_size_get(unit, &stable_size));
        if (stable_size == 0) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "STABLE size is zero.Probably NotConfigured yet\n")));
            return BCM_E_NONE;
        }
        SOC_IF_ERROR_RETURN(soc_stable_used_get(unit, &stable_used_size));
        if ((stable_size - stable_used_size) < local_scache_size[unit]) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Not enough scache memory left...\n")));
             _bcm_esw_stat_flex_cleanup(unit);
             return BCM_E_CONFIG;
        }
        rv = soc_scache_alloc(unit, handle[unit], local_scache_size[unit]);
        if (!((rv== BCM_E_NONE) || (rv == BCM_E_EXISTS))) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Seems to be some internal problem:.\n")));
             _bcm_esw_stat_flex_cleanup(unit);
             return rv;
        }
        SOC_IF_ERROR_RETURN(soc_scache_ptr_get(
                            unit,
                            handle[unit],
                            (uint8 **)&flex_scache_ptr[unit],
                            &flex_scache_allocated_size[unit]));
    } else {
        rv = soc_scache_ptr_get(
                            unit,
                            handle[unit],
                            (uint8 **)&flex_scache_ptr[unit],
                            &flex_scache_allocated_size[unit]);
        /* You may get  BCM_E_NOT_FOUND for level 1 warm boot */
        if (rv == BCM_E_NOT_FOUND) {
             LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Seems to be Level-1 Warm boot...continuing..\n")));
             return BCM_E_NONE;
        }
        if (rv != BCM_E_NONE) {
             LOG_WARN(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Seems to be some internal problem:.\n")));
             _bcm_esw_stat_flex_cleanup(unit);
             return rv;
        }

        if (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_3)) {
            local_scache_size[unit] +=
                                    sizeof(bcm_stat_flex_group_mode_related_info_t) *
                                    BCM_STAT_FLEX_COUNTER_MAX_MODE *
                                    BCM_STAT_FLEX_COUNTER_MAX_DIRECTION;
        }

        if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
           (_bcm_esw_stat_sync_version_above_equal(unit,BCM_WB_VERSION_1_4))) {
            local_scache_size[unit] +=
                                    sizeof(bcm_stat_flex_counter_id_related_info_t) *
                                    (BCM_MAX_STAT_COUNTER_IDS-1);

            local_scache_size[unit] +=
                                    sizeof(bcm_stat_flex_custom_mode_related_info_t) *
                                    (BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE - BCM_CUSTOM_INGRESS_MODE_START)
                                    + 2*sizeof(uint32);

            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "WarmBoot: Modified Local Scache Size %d\n"),
                       local_scache_size[unit]));
        }
        sal_memcpy(&local_scache_ptr[unit][0],
                   &flex_scache_ptr[unit][0],
                   BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE);
    }
    if (flex_scache_ptr[unit] == NULL) {
           _bcm_esw_stat_flex_cleanup(unit);
           return BCM_E_MEMORY;
    }
    if ((!SOC_WARM_BOOT(unit)) &&
        (flex_scache_allocated_size[unit] != local_scache_size[unit])) {
        _bcm_esw_stat_flex_cleanup(unit);
        return BCM_E_INTERNAL;
    }
    if (SOC_WARM_BOOT(unit)) {
        BCM_IF_ERROR_RETURN(_bcm_esw_stat_flex_install_stat_id(
                                     unit,flex_scache_ptr[unit], &per_pipe_mode_ifp, &per_pipe_mode_vfp));
        /* 3) Ingress: VFP_POLICY_TABLE Table */
        _bcm_esw_stat_flex_check_ingress_vfp_policy_table(unit, per_pipe_mode_vfp);

#if defined(BCM_TOMAHAWK_SUPPORT)
        if (IsAnyModeConfigured && SOC_IS_TOMAHAWKX(unit)) {
        /* 10) Ingress: IFP_POLICY_TABLE Table */
            _bcm_esw_stat_flex_check_ingress_ifp_policy_table(unit,
                                            per_pipe_mode_ifp);
        /* 11) Ingress: AGM */
            _bcm_esw_stat_flex_check_ingress_agm_table(unit);
        }
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        if (SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_APACHE(unit)) {
            _bcm_esw_stat_flex_check_ingress_ifp_policy_table(unit,
                                                per_pipe_mode_ifp);
            _bcm_esw_stat_flex_check_egress_efp_policy_table(unit);
        }
#endif

        for (direction = bcmStatFlexDirectionIngress;
             direction < bcmStatFlexDirectionCount;
             direction++) {
            for (pool_id = 0; pool_id < num_pools[direction]; pool_id++) {
                used_pool = 0;
                for(pipe_num = 0; pipe_num < num_pipes; pipe_num++) {
                    if (flex_pool_stat[unit][direction][pipe_num]
                            [pool_id].attached_entries != 0) {
                        used_pool = 1;
                        break;
                    }
                }
                if (used_pool) {
                    _bcm_esw_stat_flex_enable_pool(unit, direction,
                            _pool_ctr_register[direction][pool_id], 1);
                }
            }
        }

    }
    /* soc_scache_commit(unit); */
#else
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "COLD Booting... \n")));
#endif
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "OK \n")));

    /* Just an Info */
    if (SOC_CONTROL(unit)->tdma_enb == 0) { /* enabled */
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "WARNING:DMA will not be used for BulkMemoryReading\n")));
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_update_udf_selector_keys
 * Description:
 *      Update UDF selector keys
 * Parameters:
 *      unit                      - (IN) unit number
 *      direction                 - (IN) Flex Data Flow Direction
 *      pkt_attr_selector_key_reg - (IN) Flex Packet Attribute Seletor Key Reg
 *      udf_pkt_attr_selectors    - (IN) Flex Udf Packet Seletors
 *      total_udf_bits            - (OUT) Flex Udf Reserved bits
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *      Currently not being used
 */
bcm_error_t _bcm_esw_stat_flex_update_udf_selector_keys(
            int                                    unit,
            bcm_stat_flex_direction_t              direction,
            soc_reg_t                              pkt_attr_selector_key_reg,
            bcm_stat_flex_udf_pkt_attr_selectors_t *udf_pkt_attr_selectors,
            uint32                                 *total_udf_bits)
{
    uint32 udf_valid_bits=0;
    uint8  udf_value_bit_position=0;
    uint8  key_bit_selector_position=0;
    uint16 udf_value=0;
    uint64 pkt_attr_selector_key_reg_value;
    uint32 index=0;

    COMPILER_64_ZERO(pkt_attr_selector_key_reg_value);

    (*total_udf_bits)=0;
    if (direction >= BCM_STAT_FLEX_COUNTER_MAX_DIRECTION) {
        return BCM_E_PARAM;
    }
    for(index=0; index < 8; index++) {
        if (_pkt_selector_key_reg[direction][index] ==
            pkt_attr_selector_key_reg ) {
            break;
        }
    }
    if (index == 8) {
         return BCM_E_PARAM;
    }

    /* Sanity Check: Get total number of udf bits */
    udf_value = udf_pkt_attr_selectors->udf_pkt_attr_bits.udf0;
    for (udf_value_bit_position=0;
         udf_value_bit_position<16;
         udf_value_bit_position++) {
         if (udf_value & 0x1) {
             (*total_udf_bits)++;
         }
         udf_value = udf_value >> 1;
    }
    if (*total_udf_bits > 8) {
        return BCM_E_PARAM;
    }
    udf_value = udf_pkt_attr_selectors->udf_pkt_attr_bits.udf1;
    for (udf_value_bit_position=0;
         udf_value_bit_position<16;
         udf_value_bit_position++) {
         if(udf_value & 0x1) {
            (*total_udf_bits)++;
         }
         udf_value = udf_value >> 1;
    }

    /*First Get complete value of ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_?r value */
    SOC_IF_ERROR_RETURN(soc_reg_get(
                        unit,
                        pkt_attr_selector_key_reg,
                        REG_PORT_ANY,
                        0,
                        &pkt_attr_selector_key_reg_value));

    /* Next set field value for
       ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_?r:SELECTOR_KEY field*/
    soc_reg64_field32_set(unit, pkt_attr_selector_key_reg,
                          &pkt_attr_selector_key_reg_value, USE_UDF_KEYf, 1);
    if (udf_pkt_attr_selectors->udf_pkt_attr_bits.udf0 != 0) {
        udf_valid_bits |= 0x1;
    }
    if (udf_pkt_attr_selectors->udf_pkt_attr_bits.udf1 != 0) {
        udf_valid_bits |= 0x2;
    }
    soc_reg64_field32_set(unit, pkt_attr_selector_key_reg,
                          &pkt_attr_selector_key_reg_value,
                          USER_SPECIFIED_UDF_VALIDf, udf_valid_bits);

    /* Now update selector keys */
    if (udf_pkt_attr_selectors->drop_enable) {
       (*total_udf_bits)++;
       BCM_IF_ERROR_RETURN(
               _bcm_esw_stat_flex_update_selector_keys_enable_fields(
                        unit,
                        pkt_attr_selector_key_reg,
                        &pkt_attr_selector_key_reg_value,
                        0,
                        1,     /* Total Bits */
                        0x1,     /* MASK */
                        &key_bit_selector_position));
    }
    if ((*total_udf_bits) > 8) {
         return BCM_E_PARAM;
    }

    udf_value = udf_pkt_attr_selectors->udf_pkt_attr_bits.udf0;
    for (udf_value_bit_position=0;
         udf_value_bit_position<16;
         udf_value_bit_position++) {
         if (udf_value & 0x1) {
           BCM_IF_ERROR_RETURN(
               _bcm_esw_stat_flex_update_selector_keys_enable_fields(
                        unit,
                        pkt_attr_selector_key_reg,
                        &pkt_attr_selector_key_reg_value,
                        udf_value_bit_position+1,
                        1,     /* Total Bits */
                        0x1,     /* MASK */
                        &key_bit_selector_position));
         }
         udf_value = udf_value >> 1;
    }
    udf_value = udf_pkt_attr_selectors->udf_pkt_attr_bits.udf1;
    for (;udf_value_bit_position<32; udf_value_bit_position++) {
         if(udf_value & 0x1) {
            BCM_IF_ERROR_RETURN(
               _bcm_esw_stat_flex_update_selector_keys_enable_fields(
                        unit,
                        pkt_attr_selector_key_reg,
                        &pkt_attr_selector_key_reg_value,
                        udf_value_bit_position+1,
                        1,    /* Total Bits */
                        0x1,    /* MASK */
                        &key_bit_selector_position));
         }
         udf_value = udf_value >> 1;
    }
    /* Finally set value for ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_?r */
    SOC_IF_ERROR_RETURN(soc_reg_set(unit, pkt_attr_selector_key_reg,
                                    REG_PORT_ANY, 0,
                                    pkt_attr_selector_key_reg_value));
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_update_offset_table
 * Description:
 *      Update flex Offset table
 * Parameters:
 *      unit                      - (IN) unit number
 *      direction                 - (IN) Flex Data Flow Direction
 *      flex_ctr_offset_table_mem - (IN) Flex Offset Table
 *      mode                      - (IN) Flex Mode
 *      total_counters            - (IN) Flex Total Counters
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_update_offset_table(
            int                                unit,
            bcm_stat_flex_direction_t          direction,
            soc_mem_t                          flex_ctr_offset_table_mem,
            bcm_stat_flex_mode_t               mode,
            uint32                             total_counters,
            bcm_stat_flex_offset_table_entry_t offset_table_map[256])
{
    uint32                              index=0;
    uint32                              zero=0;
    uint32                              count_enable=1;
    uint32                              offset_value=0;
    ing_flex_ctr_offset_table_0_entry_t *offset_table_entry_v=NULL;
    uint32                              num_pools[
                                        BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];

    num_pools[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             num_flex_egress_pools;

    if (direction >= BCM_STAT_FLEX_COUNTER_MAX_DIRECTION) {
        return BCM_E_PARAM;
    }
    for(index = 0; index < num_pools[direction]; index++) {
        if (_ctr_offset_table[direction][index] == flex_ctr_offset_table_mem ) {
            break;
        }
    }
    if (index == num_pools[direction]) {
         return BCM_E_PARAM;
    }
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "...Updating offset_table:%s:%d \n"),
               SOC_MEM_UFNAME(unit,flex_ctr_offset_table_mem),
               (int)sizeof(ing_flex_ctr_offset_table_0_entry_t)));
    offset_table_entry_v = soc_cm_salloc(unit,
                               sizeof(ing_flex_ctr_offset_table_0_entry_t)*
                               BCM_STAT_FLEX_MAX_COUNTERS_PER_MODE,
                               "offset_table_entry");
    if (offset_table_entry_v == NULL) {
        return BCM_E_MEMORY;
    }
    sal_memset(offset_table_entry_v, 0, (sizeof(ing_flex_ctr_offset_table_0_entry_t)*
                               BCM_STAT_FLEX_MAX_COUNTERS_PER_MODE));
    if (soc_mem_read_range(unit,
                           flex_ctr_offset_table_mem,
                           MEM_BLOCK_ANY,
                           (mode << 8),
                           (mode << 8) + BCM_STAT_FLEX_MAX_COUNTERS_PER_MODE - 1,
                           offset_table_entry_v ) != BCM_E_NONE){
        soc_cm_sfree(unit,offset_table_entry_v);
        return BCM_E_INTERNAL;
    }
    /*for(index=0;index<255;index++) */
    for (index = 0; index < total_counters; index++) {
         /* Set OFFSETf */
         if (offset_table_map == NULL) {
             offset_value = index;
             count_enable = 1;
         } else {
             offset_value = offset_table_map[index].offset;
             count_enable = offset_table_map[index].count_enable;
         }
         soc_mem_field_set(
                 unit,
                 flex_ctr_offset_table_mem,
                 (uint32 *)&offset_table_entry_v[index],
                 OFFSETf,
                 &offset_value);
         /* Set COUNT_ENABLEf */
         soc_mem_field_set(
                 unit,
                 flex_ctr_offset_table_mem,
                 (uint32 *)&offset_table_entry_v[index],
                 COUNT_ENABLEf,
                 &count_enable);
    }
    /* Clear remaining entries */
    for (; index < BCM_STAT_FLEX_MAX_COUNTERS_PER_MODE; index++) {
         /* Set OFFSETf=zero */
         soc_mem_field_set(
                 unit,
                 flex_ctr_offset_table_mem,
                 (uint32 *)&offset_table_entry_v[index],
                 OFFSETf,
                 &zero);
         /* Set COUNT_ENABLEf=zero */
         soc_mem_field_set(
                 unit,
                 flex_ctr_offset_table_mem,
                 (uint32 *)&offset_table_entry_v[index],
                 COUNT_ENABLEf,
                 &zero);
    }
    if (soc_mem_write_range(    unit,
                flex_ctr_offset_table_mem,
                MEM_BLOCK_ALL,
                (mode << 8),
                (mode << 8) + BCM_STAT_FLEX_MAX_COUNTERS_PER_MODE - 1,
                offset_table_entry_v ) != BCM_E_NONE){
        soc_cm_sfree(unit,offset_table_entry_v);
        return BCM_E_INTERNAL;
    }
    soc_cm_sfree(unit,offset_table_entry_v);
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_update_selector_keys_enable_fields
 * Description:
 *      Enable flex selector keys fields
 * Parameters:
 * unit                              - (IN) unit number
 * pkt_attr_selector_key_reg         - (IN) Flex Packet Attribute Selector
 * pkt_attr_selector_key_reg_value   - (IN) Flex Packet Attribute SelectorVal
 * ctr_pkt_attr_bit_position         - (IN) Flex Packet CounterAttr Bit Position
 * ctr_pkt_attr_total_bits           - (IN) Flex Packet Counter Attr Total Bits
 * pkt_attr_field_mask_v             - (IN) Flex Packet Attr Field Mask
 * ctr_current_bit_selector_position - (IN/OUT) Current BitSelector Position
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_update_selector_keys_enable_fields(
           int       unit,
           soc_reg_t pkt_attr_selector_key_reg,
           uint64    *pkt_attr_selector_key_reg_value,
           uint32    ctr_pkt_attr_bit_position,
           uint32    ctr_pkt_attr_total_bits,
           uint8     pkt_attr_field_mask_v,
           uint8     *ctr_current_bit_selector_position)
{
    uint32 index=0;
    uint8  field_mask=0;
    uint8  total_field_bits=0;
    uint8  field_index[8]={0};

    if ((*ctr_current_bit_selector_position) + ctr_pkt_attr_total_bits > 8) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Total bits exceeding 8 \n")));
         return BCM_E_INTERNAL;
    }
    field_mask = pkt_attr_field_mask_v;
    total_field_bits = 0;

    for (index=0;index < 8 ; index++) {
        if (field_mask & 0x1) {
            field_index[index] = index;
            total_field_bits++;
        }
        field_mask = field_mask >> 1;
    }
    if (total_field_bits != ctr_pkt_attr_total_bits ) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Total bits exceeding not matching with mask bits \n")));
        return BCM_E_INTERNAL;
    }

    for (index=0;index<ctr_pkt_attr_total_bits;index++) {
         soc_reg64_field32_set(
                   unit,
                   pkt_attr_selector_key_reg,
                   pkt_attr_selector_key_reg_value,
                   _pkt_selector_x_en_field_name[
                    *ctr_current_bit_selector_position],
                   1);
         soc_reg64_field32_set(
                   unit,
                   pkt_attr_selector_key_reg,
                   pkt_attr_selector_key_reg_value,
                   _pkt_selector_for_bit_x_field_name[
                    *ctr_current_bit_selector_position],
                   ctr_pkt_attr_bit_position+field_index[index]);
         /*ctr_pkt_attr_bit_position+index); */
         (*ctr_current_bit_selector_position) +=1;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_egress_reserve_mode
 * Description:
 *      Reserve flex egress mode
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      total_counters        - (IN) Flex Total Counters for given mode
 *      egr_attr              - (IN) Flex Egress Attributes
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_egress_reserve_mode(
            int                      unit,
            bcm_stat_flex_mode_t     mode,
            uint32                   total_counters,
            bcm_stat_flex_egr_attr_t *egr_attr)
{
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    flex_egress_modes[unit][mode].available=1;
    flex_egress_modes[unit][mode].total_counters=total_counters;
    flex_egress_modes[unit][mode].egr_attr=*egr_attr;
    flex_ingress_modes[unit][mode].flags |= BCM_STAT_GROUP_MODE_EGRESS;
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_ingress_reserve_mode
 * Description:
 *      Reserve flex ingress mode
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      total_counters        - (IN) Flex Total Counters for given mode
 *      ing_attr              - (IN) Flex Ingress Attributes
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_ingress_reserve_mode(
            int                      unit,
            bcm_stat_flex_mode_t     mode,
            uint32                   total_counters,
            bcm_stat_flex_ing_attr_t *ing_attr)
{
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    flex_ingress_modes[unit][mode].available=1;
    flex_ingress_modes[unit][mode].total_counters=total_counters;
    flex_ingress_modes[unit][mode].ing_attr=*ing_attr;
    flex_ingress_modes[unit][mode].flags |= BCM_STAT_GROUP_MODE_INGRESS;
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_unreserve_flags
 * Description:
 *      UnReserve flex (ingress/egress) flags
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data Flow Direction
 *      mode                  - (IN) Flex Offset Mode
 *      flags                 - (IN) Flags
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 * 
 */
bcm_error_t _bcm_esw_stat_flex_unreserve_flags(
            int                       unit, 
            bcm_stat_flex_mode_t      mode,
            uint32                    flags)
{
    if (flags & BCM_STAT_GROUP_MODE_CAPABILITY_OAM) {
        flex_ingress_modes[unit][mode].oam_flags = 0;
    } else {
        flex_ingress_modes[unit][mode].flags = 0;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_unreserve_mode
 * Description:
 *      UnReserve flex (ingress/egress) mode
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data Flow Direction
 *      mode                  - (IN) Flex Offset Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_unreserve_mode(
            int                       unit,
            bcm_stat_flex_direction_t direction,
            bcm_stat_flex_mode_t      mode)
{
    uint32                                    index=0;
    bcm_stat_flex_packet_attr_type_t          packet_attr_type;
    bcm_stat_flex_ing_cmprsd_attr_selectors_t *ing_cmprsd_attr_selectors= NULL;
    bcm_stat_flex_ing_pkt_attr_bits_t         ing_pkt_attr_bits={0};
    bcm_stat_flex_egr_cmprsd_attr_selectors_t *egr_cmprsd_attr_selectors= NULL;
    bcm_stat_flex_egr_pkt_attr_bits_t         egr_pkt_attr_bits={0};
    uint8                                     zero=0;
    uint64                                    pkt_attr_selector_key_reg_value;
    uint32 num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint8 counter_group_num = BCM_STAT_DEF_FLEX_COUNTER_MAX_GROUPS;

    COMPILER_64_ZERO(pkt_attr_selector_key_reg_value);

    num_pools[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                             num_flex_egress_pools;

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    switch(direction) {
    case bcmStatFlexDirectionIngress:
         if (flex_ingress_modes[unit][mode].available == 0) {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "flex counter mode %d not configured yet\n"),
                        mode));
             return BCM_E_NOT_FOUND;
         }
         if (flex_ingress_modes[unit][mode].reference_count != 0) {
             /* not an error in case two or more counters are created before
              * deleting one.
              */
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "FlexCounterMode:%d:IsBeingUsed.ReferenceCount:%d:\n"),
                        mode,flex_ingress_modes[unit][mode].reference_count));
             return BCM_E_BUSY;
         } else {
                if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
                   if (_bcm_esw_stat_flex_delete_ingress_flags(
                                  unit, mode,
                                  BCM_STAT_GROUP_MODE_INGRESS) != BCM_E_NONE) {
                       return BCM_E_INTERNAL;
                   }
               }
         }

         if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
             if (flex_ingress_modes[unit][mode].flags  ||
                 flex_ingress_modes[unit][mode].oam_flags) {
                 /* Delete the mode only if no flags exist.
                  */
                 LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "FlexCounterMode:%d:IsBeingUsed.Flags:%d::IsBeingUsed.OamFlags:%d\n"),
                                        mode,flex_ingress_modes[unit][mode].flags,
                                        flex_ingress_modes[unit][mode].oam_flags));
                 return BCM_E_EXISTS;
             }
         } 
         /* Step2:  Reset selector keys */
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit)) {
            counter_group_num = BCM_STAT_TR3_FLEX_COUNTER_MAX_GROUPS;
        }
#endif
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            counter_group_num = BCM_STAT_TH_FLEX_COUNTER_MAX_GROUPS;
        }
#endif
#ifdef BCM_METROLITE_SUPPORT
        if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
            counter_group_num = BCM_STAT_DEF_FLEX_COUNTER_MAX_GROUPS;
        }
#endif
        for ( index = 0; index < counter_group_num; index++) {
            SOC_IF_ERROR_RETURN(soc_reg_set(
                        unit,
                        _pkt_selector_key_reg[bcmStatFlexDirectionIngress]
                        [mode + index*BCM_STAT_FLEX_COUNTER_MAX_MODE],
                        REG_PORT_ANY,
                        0,
                        pkt_attr_selector_key_reg_value));
        }

         /* Step3: Reset Offset table filling */
         for (index=0;index<num_pools[bcmStatFlexDirectionIngress]; index++) {
              BCM_IF_ERROR_RETURN(_bcm_esw_stat_flex_update_offset_table(
                                  unit,
                                  bcmStatFlexDirectionIngress,
                                  _ctr_offset_table
                                    [bcmStatFlexDirectionIngress][index],
                                  mode,
                                  0,
                                  NULL));
         }
         /* Step4: Cleanup based on mode */
         packet_attr_type= flex_ingress_modes[unit][mode].
                            ing_attr.packet_attr_type;
         switch(packet_attr_type) {
         case bcmStatFlexPacketAttrTypeUncompressed:
              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\n Unreserving Ingress uncmprsd mode \n")));
              break;
         case bcmStatFlexPacketAttrTypeUdf:
              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\n Unreserving Ingress udf mode \n")));
              break;
         case bcmStatFlexPacketAttrTypeCompressed:
              ing_cmprsd_attr_selectors= &(flex_ingress_modes[unit][mode].
                                           ing_attr.cmprsd_attr_selectors);
              ing_pkt_attr_bits=ing_cmprsd_attr_selectors->pkt_attr_bits;
              zero=0;

              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\n Unreserving Ingress cmprsd mode \n")));

              /* Step3: Cleanup map array */
              if ((ing_pkt_attr_bits.cng != 0) ||
                  (ing_pkt_attr_bits.ifp_cng)  ||
                  (ing_pkt_attr_bits.int_pri)) {
                  for (index=0;
                       index< soc_mem_index_count(unit,ING_FLEX_CTR_PRI_CNG_MAPm);
                       index++) {
                       soc_mem_write(unit,ING_FLEX_CTR_PRI_CNG_MAPm,
                                     MEM_BLOCK_ALL, index, &zero);
                  }
              }
              if ((ing_pkt_attr_bits.vlan_format != 0) ||
                  (ing_pkt_attr_bits.outer_dot1p) ||
                  (ing_pkt_attr_bits.inner_dot1p)) {
                   for (index=0;
                        index< soc_mem_index_count(unit,ING_FLEX_CTR_PKT_PRI_MAPm);
                        index++) {
                        soc_mem_write(unit, ING_FLEX_CTR_PKT_PRI_MAPm,
                                      MEM_BLOCK_ALL, index, &zero);
                   }
              }
              if (ing_pkt_attr_bits.ing_port != 0) {
                  for (index=0;
                       index< soc_mem_index_count(unit,ING_FLEX_CTR_PORT_MAPm);
                       index++) {
                   soc_mem_write(unit, ING_FLEX_CTR_PORT_MAPm,
                                MEM_BLOCK_ALL, index, &zero);
                  }
              }
              if ((ing_pkt_attr_bits.tos_dscp != 0) ||
                  (ing_pkt_attr_bits.tos_ecn != 0)) {
                  for (index=0;
                       index< soc_mem_index_count(unit,ING_FLEX_CTR_TOS_MAPm);
                       index++) {
                       soc_mem_write(unit, ING_FLEX_CTR_TOS_MAPm,
                                     MEM_BLOCK_ALL, index, &zero);
                  }
              }
              if ((ing_pkt_attr_bits.pkt_resolution != 0) ||
                  (ing_pkt_attr_bits.svp_type) ||
                  (ing_pkt_attr_bits.drop)) {
                   for (index=0;
                        index< soc_mem_index_count(unit,ING_FLEX_CTR_PKT_RES_MAPm);
                        index++) {
                        soc_mem_write(unit, ING_FLEX_CTR_PKT_RES_MAPm,
                                      MEM_BLOCK_ALL, index, &zero);
                   }
              }
              break;
         }
         flex_ingress_modes[unit][mode].available=0;
         flex_ingress_modes[unit][mode].total_counters=0;
         flex_ingress_modes[unit][mode].flags = 0;
         if (flex_ingress_modes[unit][mode].attr_selectors  != NULL) {
             sal_free(flex_ingress_modes[unit][mode].attr_selectors);
             flex_ingress_modes[unit][mode].num_selectors = 0;
             flex_ingress_modes[unit][mode].attr_selectors = NULL;
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Ingress: Freed Attr Selectors \n")));
         }
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "\n Done \n")));
         break;
    case bcmStatFlexDirectionEgress:
         if (flex_egress_modes[unit][mode].available==0) {
             return BCM_E_NOT_FOUND;
         }
         if (flex_egress_modes[unit][mode].reference_count != 0) {
             return BCM_E_INTERNAL;
         }

         /* Step2:  Reset selector keys */
         SOC_IF_ERROR_RETURN(soc_reg_set(unit,
                             _pkt_selector_key_reg[bcmStatFlexDirectionEgress]
                                                  [mode],
                             REG_PORT_ANY,
                             0,
                             pkt_attr_selector_key_reg_value));
#if defined(BCM_KATANA_SUPPORT) || defined(BCM_TRIUMPH3_SUPPORT)
         if (!SOC_IS_METROLITE(unit) &&
             (SOC_IS_KATANAX(unit) || SOC_IS_TRIUMPH3(unit))) {
             SOC_IF_ERROR_RETURN(soc_reg_set(unit,
                                 _pkt_selector_key_reg[bcmStatFlexDirectionEgress]
                                 [mode+BCM_STAT_FLEX_COUNTER_MAX_MODE],
                                 REG_PORT_ANY,
                                 0,
                                 pkt_attr_selector_key_reg_value));
          }
#endif
         /* Step3: Reset Offset table filling */
         for (index=0; index < num_pools[bcmStatFlexDirectionEgress]; index++) {
              BCM_IF_ERROR_RETURN(_bcm_esw_stat_flex_update_offset_table(
                                   unit,bcmStatFlexDirectionEgress,
                                   _ctr_offset_table[bcmStatFlexDirectionEgress]
                                                    [index],
                                   mode,
                                   0,
                                   NULL));
         }
         /* Step4: Cleanup based on mode */
         packet_attr_type = flex_egress_modes[unit][mode].egr_attr.
                            packet_attr_type;
         switch(packet_attr_type) {
         case bcmStatFlexPacketAttrTypeUncompressed:
              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\nUnreserving Egress uncmprsd mode \n")));
              break;
         case bcmStatFlexPacketAttrTypeUdf:
              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\n Unreserving Egress UDF mode \n")));
              break;
         case bcmStatFlexPacketAttrTypeCompressed:
              egr_cmprsd_attr_selectors= &(flex_egress_modes[unit][mode].
                                           egr_attr.cmprsd_attr_selectors);
              egr_pkt_attr_bits= egr_cmprsd_attr_selectors->pkt_attr_bits;
              LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "\n Unreserving Egress cmprsd mode \n")));

              /* Step3: Cleanup map array */
              if ((egr_pkt_attr_bits.cng != 0) ||
                  (egr_pkt_attr_bits.int_pri)) {
                   for(index=0;
                       index< soc_mem_index_count(unit,EGR_FLEX_CTR_PRI_CNG_MAPm);
                       index++) {
                       soc_mem_write(unit, EGR_FLEX_CTR_PRI_CNG_MAPm,
                                     MEM_BLOCK_ALL, index, &zero);
                   }
              }
              if ((egr_pkt_attr_bits.vlan_format != 0) ||
                  (egr_pkt_attr_bits.outer_dot1p) ||
                  (egr_pkt_attr_bits.inner_dot1p)) {
                   for(index=0;
                       index< soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_PRI_MAPm);
                       index++) {
                       soc_mem_write(unit, EGR_FLEX_CTR_PKT_PRI_MAPm,
                                     MEM_BLOCK_ALL, index, &zero);
                   }
              }
              if (egr_pkt_attr_bits.egr_port != 0) {
                  for(index=0;
                      index< soc_mem_index_count(unit,EGR_FLEX_CTR_PORT_MAPm);
                      index++) {
                      soc_mem_write(unit, EGR_FLEX_CTR_PORT_MAPm,
                                    MEM_BLOCK_ALL, index, &zero);
                  }
              }
              if ((egr_pkt_attr_bits.tos_dscp != 0) ||
                  (egr_pkt_attr_bits.tos_ecn != 0)) {
                  for(index=0;
                      index< soc_mem_index_count(unit,EGR_FLEX_CTR_TOS_MAPm);
                      index++) {
                            soc_mem_write(
                                unit,
                                EGR_FLEX_CTR_TOS_MAPm,
                                MEM_BLOCK_ALL,
                                index,
                                &zero);
                           }
                        }
              if ((egr_pkt_attr_bits.pkt_resolution != 0) ||
                  (egr_pkt_attr_bits.svp_type) ||
                  (egr_pkt_attr_bits.dvp_type) ||
                  (egr_pkt_attr_bits.drop)) {
                   for(index=0;
                       index< soc_mem_index_count(unit,EGR_FLEX_CTR_PKT_RES_MAPm);
                       index++) {
                        soc_mem_write(unit, EGR_FLEX_CTR_PKT_RES_MAPm,
                                      MEM_BLOCK_ALL, index, &zero);
                   }
              }
              break;
         }
         flex_egress_modes[unit][mode].available=0;
         flex_egress_modes[unit][mode].total_counters=0;
         if (flex_egress_modes[unit][mode].attr_selectors  != NULL) {
             sal_free(flex_egress_modes[unit][mode].attr_selectors);
             flex_egress_modes[unit][mode].flags = 0;
             flex_egress_modes[unit][mode].num_selectors = 0;
             flex_egress_modes[unit][mode].attr_selectors = NULL;
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Egress: Freed Attr Selectors \n")));
         }
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "\n Done \n")));
         break;
    default:
         return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}
bcm_error_t _bcm_esw_stat_flex_update_ingress_flex_info(
            int                                 unit,
            bcm_stat_flex_mode_t                mode,
            uint32                              flags,
            uint32                              num_selectors,
            bcm_stat_group_mode_attr_selector_t *attr_selectors)
{
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_ingress_modes[unit][mode].available==0) {
        return BCM_E_NOT_FOUND;
    }
    flex_ingress_modes[unit][mode].flags = flags;
    if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
        if (flags & BCM_STAT_GROUP_MODE_CAPABILITY_OAM) {
            flex_ingress_modes[unit][mode].oam_flags = flags;
        }
    }   
    if ((NULL == flex_ingress_modes[unit][mode].attr_selectors) &&
         (flex_ingress_modes[unit][mode].attr_selectors =
         sal_alloc(num_selectors*sizeof(bcm_stat_group_mode_attr_selector_t),
                  "attr_selectors")) == NULL) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Memory allocation failed \n")));
         return BCM_E_MEMORY;
    }
    flex_ingress_modes[unit][mode].num_selectors = num_selectors;

    sal_memset(flex_ingress_modes[unit][mode].attr_selectors, 0,
               num_selectors * sizeof(bcm_stat_group_mode_attr_selector_t));
    sal_memcpy(flex_ingress_modes[unit][mode].attr_selectors,
               attr_selectors,
               num_selectors * sizeof(bcm_stat_group_mode_attr_selector_t));
    return BCM_E_NONE;
}
bcm_error_t _bcm_esw_stat_flex_update_egress_flex_info(
            int                                 unit,
            bcm_stat_flex_mode_t                mode,
            uint32                              flags,
            uint32                              num_selectors,
            bcm_stat_group_mode_attr_selector_t *attr_selectors)
{
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_egress_modes[unit][mode].available==0) {
        return BCM_E_NOT_FOUND;
    }
    flex_egress_modes[unit][mode].flags = flags;
    flex_egress_modes[unit][mode].num_selectors = num_selectors;
    if ((flex_egress_modes[unit][mode].attr_selectors =
         sal_alloc(num_selectors*sizeof(bcm_stat_group_mode_attr_selector_t),
                  "attr_selectors")) == NULL) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Memory allocation failed \n")));
         return BCM_E_MEMORY;
    }
    sal_memset(flex_egress_modes[unit][mode].attr_selectors, 0,
               num_selectors * sizeof(bcm_stat_group_mode_attr_selector_t));
    sal_memcpy(flex_egress_modes[unit][mode].attr_selectors,
               attr_selectors,
               num_selectors * sizeof(bcm_stat_group_mode_attr_selector_t));
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_get_ingress_mode_info
 * Description:
 *      Get Ingress Mode related information
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      flex_ingress_mode     - (OUT) Flex Ingress Mode Info
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_ingress_mode_info(
            int                          unit,
            bcm_stat_flex_mode_t         mode,
            bcm_stat_flex_ingress_mode_t *flex_ingress_mode)
{
    if (mode <= (BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE-1) &&
       (mode >= BCM_CUSTOM_INGRESS_MODE_START)) {
        mode = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].offset_mode;
    } else if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_ingress_modes[unit][mode].available==0) {
        return BCM_E_NOT_FOUND;
    }
    *flex_ingress_mode= flex_ingress_modes[unit][mode];
    return BCM_E_NONE;
}


#if defined (BCM_TOMAHAWK_SUPPORT)

bcm_error_t _bcm_esw_custom_stat_group_id_retrieve(
    int unit,
    uint32 offset_mode,
    int  pipe,
    uint8  pool,
    uint32 base_idx,
    uint32 *stat_ctr_id)
{
    
    int ing_mode = 0, i;
    bcm_error_t rv=BCM_E_NOT_FOUND;

    *stat_ctr_id = 0;
    if (!soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
        return BCM_E_CONFIG;
    }

    for(i = 1; i < BCM_MAX_STAT_COUNTER_IDS; ++i) {
        if (!(stat_counter_map[unit]) || (stat_counter_map[unit][i].mode >=
                              BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE) ||
            (stat_counter_map[unit][i].mode < BCM_CUSTOM_INGRESS_MODE_START)) {
            continue;
        }
        ing_mode = stat_counter_map[unit][i].mode;

        if (offset_mode < BCM_CUSTOM_INGRESS_MODE_START) {
            ing_mode = stat_counter_map[unit][i].mode - BCM_CUSTOM_INGRESS_MODE_START;
            ing_mode = flex_custom_ingress_modes[unit][ing_mode].offset_mode;
        }

        if (stat_counter_map[unit] && stat_counter_map[unit][i].used) {

            if ((ing_mode == offset_mode) && 
                (stat_counter_map[unit][i].pool == pool) && 
                (stat_counter_map[unit][i].base_idx == base_idx)) {

                    if (pipe != stat_counter_map[unit][i].pipe) {
                        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                                  (BSL_META_U(unit,
                                              "pipes %d %d %x %x for stat ctr %x"),
                                   pipe, stat_counter_map[unit][i].pipe, base_idx, pool, *stat_ctr_id));
                        continue;
                    }
                *stat_ctr_id = i;
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "reconstructed id for mode - %d, pool - %d"
                                      " mode - %d, base idx - %d stat ctr %x"),
                           ing_mode, pool, offset_mode,
                           base_idx, *stat_ctr_id));
                return BCM_E_NONE;
            }
            continue;
        }
    }
    return rv;
}
#endif

/*
 * Function:
 *      _bcm_esw_stat_flex_get_ingress_mode_info
 * Description:
 *      Get Ingress Mode related information
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      flex_ingress_mode     - (OUT) Flex Ingress Mode Info
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_custom_ingress_mode_info(
            int                          unit,
            bcm_stat_flex_mode_t         mode,
            bcm_stat_flex_custom_ingress_mode_t *flex_ingress_mode)
{
    if (mode > (BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE-1) ||
       (mode <BCM_CUSTOM_INGRESS_MODE_START)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }

    mode -= BCM_CUSTOM_INGRESS_MODE_START;

    *flex_ingress_mode= flex_custom_ingress_modes[unit][mode];
    if (flex_custom_ingress_modes[unit][mode].used==0) {
        return BCM_E_NOT_FOUND;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_get_ingress_mode_info
 * Description:
 *      Get Ingress Mode related information
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      flex_ingress_mode     - (OUT) Flex Ingress Mode Info
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_update_custom_ingress_mode_info(
            int                          unit,
            bcm_stat_flex_mode_t         mode,
            bcm_stat_flex_custom_ingress_mode_t *flex_ingress_mode)
{
    if ((!flex_ingress_mode) || (mode > (BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE-1)) ||
       (mode <BCM_CUSTOM_INGRESS_MODE_START)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }

    mode -= BCM_CUSTOM_INGRESS_MODE_START;
    flex_custom_ingress_modes[unit][mode] = *flex_ingress_mode;
    return BCM_E_NONE;
}


/*
 * Function:
 *      _bcm_esw_stat_flex_get_egress_mode_info
 * Description:
 *      Get Egress Mode related information
 * Parameters:
 *      unit                  - (IN) unit number
 *      mode                  - (IN) Flex Offset Mode
 *      flex_egress_mode      - (OUT) Flex Egress Mode Info
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_egress_mode_info(
            int                         unit,
            bcm_stat_flex_mode_t        mode,
            bcm_stat_flex_egress_mode_t *flex_egress_mode)
{
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_egress_modes[unit][mode].available==0) {
        return BCM_E_NOT_FOUND;
    }
    *flex_egress_mode = flex_egress_modes[unit][mode];
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_get_available_mode
 * Description:
 *      Get Free available Offset Mode
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data Flow Direction
 *      mode                  - (OUT) Flex Offset Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_available_mode(
            int                       unit,
            bcm_stat_flex_direction_t direction,
            bcm_stat_flex_mode_t      *mode)
{
    uint32 mode_index=1;
    switch(direction) {
    case bcmStatFlexDirectionIngress:
         for (mode_index=0;
              mode_index < BCM_STAT_FLEX_COUNTER_MAX_MODE ;
              mode_index++) {
              if (flex_ingress_modes[unit][mode_index].available==0) {
                  *mode=mode_index;
                  return BCM_E_NONE;
              }
         }
         break;
    case bcmStatFlexDirectionEgress:
         for (mode_index=0;
              mode_index < BCM_STAT_FLEX_COUNTER_MAX_MODE ;
              mode_index++) {
              if (flex_egress_modes[unit][mode_index].available==0) {
                  *mode=mode_index;
                  return BCM_E_NONE;
              }
         }
         break;
    default:
         return BCM_E_PARAM;
    }
    return BCM_E_FULL;
}

#define BCM_TH_LOCAL_PIPE_MODE(_unit_, _pool_id_) \
         ((!_bcm_esw_get_fp_mode_global(_unit_, bcmFieldQualifyStageIngress) &&   \
                 (FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE ==                   \
                           flex_pool_stat[_unit_][bcmStatFlexDirectionIngress]    \
                                         [0][_pool_id_].used_by_tables)) ||       \
         (!_bcm_esw_get_fp_mode_global(_unit_, bcmFieldQualifyStageLookup) &&     \
                 (FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE ==                   \
                           flex_pool_stat[_unit_][bcmStatFlexDirectionIngress]    \
                                         [0][_pool_id_].used_by_tables))     ||   \
         (!_bcm_esw_get_fp_mode_global(_unit_, bcmFieldQualifyStageIngressExactMatch) && \
                 (FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE ==                    \
                           flex_pool_stat[_unit_][bcmStatFlexDirectionIngress]    \
                                         [0][_pool_id_].used_by_tables)))
   

/*
 * Function:
 *      th_flex_counter_set
 * Description:
 *      Program the given value to the counter specified by the parameters
 *      direction, pool_id and index in both software and hardware locations.
 *      This function only applies to Tomahawk device.
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) ingress or egress
 *      pool_id               - (IN) the flex counter table pool id
 *      index                 - (IN) the flex counter table index
 *      value                 - (IN) the given value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */

STATIC
int th_flex_counter_set(int unit, bcm_stat_flex_direction_t direction, int pipe_num,
                      int pool_id, uint32 index, bcm_stat_value_t *value_array,
                      int num_entries,  int byte_flag)
{
#ifdef BCM_TOMAHAWK_SUPPORT
    soc_mem_t mem;
    int mem_id;
    int pipe;
    soc_control_t *soc;
    soc_counter_evict_t *evict;
    uint32 evt_pkt_base;
    uint32 evt_byte_base;
    int cnt;
    uint32                    hw_val[2];
    uint32                    max_packet_mask=0;
    uint64                    max_byte_mask;
    uint32                    zero=0;
    uint32                    one=1;
    bcm_stat_value_t value;
    bcm_stat_value_t *value_ptr;
    uint32                    hw_val_zero[2] = {0,0};
    bcm_stat_value_t value_zero;
    int pipe_local_mode  = 0;

    if (soc_feature(unit, soc_feature_centralized_counter)) {
        /* For Centralized Counter model, call the new function */
        if (!BCM_TH_LOCAL_PIPE_MODE(unit, pool_id)) {
            /* Pass pipe_num as -1 if in Global mode */
            pipe_num = -1;
        }
        return th_flex_centralized_counter_set(unit, direction, pipe_num,
                                               pool_id, index, value_array,
                                               num_entries, byte_flag);
    }

    if (num_entries < 1) {
        return BCM_E_NONE;
    }

    if ( (direction == bcmStatFlexDirectionIngress) &&
           (BCM_TH_LOCAL_PIPE_MODE(unit, pool_id))) {
        pipe_local_mode = 1;
    }

    sal_memset(&value_zero,0,sizeof(bcm_stat_value_t));

    soc = SOC_CONTROL(unit);
    if (direction == bcmStatFlexDirectionIngress) {
        mem_id = pool_id + 1;
    } else {
        mem_id = pool_id + 1 + SOC_TH_STAT_ING_FLEX_POOL_MAX;
    }
    /* During unit detach, counter thread is shutdown prior to
     * flexstat module being detached. Modules such as field that
     * use flexstats call this function after counter thread is
     * shutdown. In this scenario, evict is NULL. Check for this.
     */
    if (NULL == soc->counter_evict) {
        return BCM_E_INTERNAL;
    }
    evict = &soc->counter_evict[mem_id];
    mem = evict->mem[pipe_num];
    COMPILER_64_SET(max_byte_mask, zero, one);
    COMPILER_64_SHL(max_byte_mask,soc_mem_field_length(unit,mem,BYTE_COUNTERf));
    COMPILER_64_SUB_32(max_byte_mask,one);

    max_packet_mask = (1 << soc_mem_field_length(unit,mem,PACKET_COUNTERf));
    max_packet_mask -= 1;

    BCM_STAT_FLEX_COUNTER_LOCK(unit);

    /* set the software copy */
    /* Clear residual packet and byte counters in addition to flex database.
     * When counter eviction is enabled, residual counters could be non-zero,
     * hence if they are not cleared, counters in flex database will be incorrectly
     * updated during the next polling cycle to include residuals from before
     * the set operation was performed
     */
    for (cnt = 0; cnt < num_entries; cnt++) {
        value_ptr = value_array == NULL? &value_zero: value_array+cnt;
        /* byte_flag = 1  => Update byte counter only
         * byte_flag = -1 => Update byte and packet counter
         */
        if (1 == byte_flag || -1 == byte_flag) {
            COMPILER_64_SET(value.bytes,
                            COMPILER_64_HI(value_ptr->bytes),
                            COMPILER_64_LO(value_ptr->bytes)); 
            COMPILER_64_SET(flex_byte_counter[unit][direction][pipe_num][pool_id]
                                             [index + cnt],
                            COMPILER_64_HI(value.bytes),
                            COMPILER_64_LO(value.bytes));
            COMPILER_64_ZERO(residual_byte_counter[unit][direction][pipe_num][pool_id]
                                                  [index + cnt]);
            /* Mask count value prior to writing hardware */
            COMPILER_64_AND(value.bytes, max_byte_mask);
        }
        /* byte_flag = 0  => Update packet counter only
         * byte_flag = -1 => Update byte and packet counter
         */
        if (0 == byte_flag || -1 == byte_flag) {
            value.packets = value_ptr->packets; 
            flex_packet_counter[unit][direction][pipe_num][pool_id]
                               [index + cnt] = value.packets;
            COMPILER_64_SET(flex_packet64_counter[unit][direction][pipe_num][pool_id]
                                                 [index + cnt],0,
                            value.packets);
            COMPILER_64_ZERO(residual_packet_counter[unit][direction][pipe_num][pool_id]
                                                    [index + cnt]);
            /* Mask count value prior to writing hardware */
            value.packets &= max_packet_mask;
        }
    }

    for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
         if (pipe_local_mode && (pipe != pipe_num)) {
             continue;
         }

         mem = evict->mem[pipe];

        if (num_entries > 1) {
            if (soc_mem_read_range(unit,
                               mem,
                               MEM_BLOCK_ANY,
                               index,
                               index+num_entries-1,
                          flex_temp_counter[unit][direction]) != BCM_E_NONE) {
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                return BCM_E_INTERNAL;
            }
        } else {
            if (soc_mem_read( unit, mem, MEM_BLOCK_ANY, index,
                    flex_temp_counter[unit][direction]) != BCM_E_NONE){
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                return BCM_E_INTERNAL;
            }
        }

        evt_pkt_base =
           soc->counter_non_dma[evict->pkt_counter_id -
              SOC_COUNTER_NON_DMA_START].base_index +
                     evict->offset[pipe];

        evt_byte_base =
           soc->counter_non_dma[evict->byte_counter_id -
              SOC_COUNTER_NON_DMA_START].base_index +
                          evict->offset[pipe];

        for (cnt = 0; cnt < num_entries; cnt++) {
            value_ptr = value_array == NULL? &value_zero: value_array+cnt;
            /* byte_flag = 0  => Update packet counter only
             * byte_flag = -1 => Update byte and packet counter
             */
            if (1 == byte_flag || -1 == byte_flag) {
                COMPILER_64_ZERO(soc->counter_sw_val[evt_byte_base+index+cnt]);
                if ((pipe == 0) || pipe_local_mode) {
                    /* Change Hw Copy */
                    COMPILER_64_SET(value.bytes,
                                    COMPILER_64_HI(value_ptr->bytes),
                                    COMPILER_64_LO(value_ptr->bytes));
                    COMPILER_64_AND(value.bytes, max_byte_mask);
                    hw_val[0] = COMPILER_64_LO(value.bytes);
                    hw_val[1] = COMPILER_64_HI(value.bytes);
                    soc_mem_field_set(unit, mem,
                        (uint32 *)&flex_temp_counter[unit][direction][cnt],
                        BYTE_COUNTERf, hw_val);
                } else {
                    soc_mem_field_set(unit, mem,
                        (uint32 *)&flex_temp_counter[unit][direction][cnt],
                        BYTE_COUNTERf, hw_val_zero);
                }
            }
            /* byte_flag = 0  => Update packet counter only
             * byte_flag = -1 => Update byte and packet counter
             */
            if (0 == byte_flag || -1 == byte_flag) {
                COMPILER_64_ZERO(soc->counter_sw_val[evt_pkt_base+index+cnt]);
                if ((pipe == 0) || pipe_local_mode) {
                    value.packets = value_ptr->packets;
                    value.packets &= max_packet_mask;
                    soc_mem_field_set(unit, mem,
                        (uint32 *)&flex_temp_counter[unit][direction][cnt],
                        PACKET_COUNTERf, &(value.packets));
                } else {
                    soc_mem_field_set(unit, mem,
                        (uint32 *)&flex_temp_counter[unit][direction][cnt],
                        PACKET_COUNTERf, &hw_val_zero[0]);
                }
            }
        }
        if (num_entries > 1) {
            if (soc_mem_write_range(unit,mem,MEM_BLOCK_ALL,
                    index,
                    index+num_entries-1,
                    flex_temp_counter[unit][direction]) != BCM_E_NONE) {
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                return BCM_E_INTERNAL;
            }
        } else {
            if (soc_mem_write(unit,mem,MEM_BLOCK_ALL, index,
                        flex_temp_counter[unit][direction]) != BCM_E_NONE){
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                return BCM_E_INTERNAL;
            }
        }
    }
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
#else
    return BCM_E_UNAVAIL;
#endif
}

/*
 * Function:
 *      th_flex_centralized_counter_set
 * Description:
 *      Program the given value to the counter specified by the parameters
 *      direction, pool_id and index in both software and hardware locations.
 *      This function only applies to Tomahawk device.
 *
 *      Note: In the centralized counter model, counters are set/get from
 *      counter module using soc_counter_generic_get/_set api.
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) ingress or egress
 *      pool_id               - (IN) the flex counter table pool id
 *      index                 - (IN) the flex counter table index
 *      value                 - (IN) the given value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */

STATIC
int th_flex_centralized_counter_set(int unit,
                                    bcm_stat_flex_direction_t direction,
                                    int pipe_num,
                                    int pool_id, uint32 index,
                                    bcm_stat_value_t *value_array,
                                    int num_entries, int byte_flag)
{
#ifdef BCM_TOMAHAWK_SUPPORT
    soc_control_t *soc;
    int cnt, rv;
    bcm_stat_value_t value;
    bcm_stat_value_t *value_ptr;
    bcm_stat_value_t value_zero;
    soc_counter_non_dma_id_t  pkt_ctr_id, byte_ctr_id;
    soc_ctr_control_info_t ctrl_info[2], ctrl_info_out;

    if (num_entries < 1) {
        return BCM_E_NONE;
    }

    ctrl_info[0].instance_type = SOC_CTR_INSTANCE_TYPE_POOL;
    ctrl_info[0].instance = pool_id;

    if (pipe_num == -1) {
        ctrl_info_out = ctrl_info[0];
    } else {
        ctrl_info[1].instance_type = SOC_CTR_INSTANCE_TYPE_PIPE;
        ctrl_info[1].instance = pipe_num;
        soc_counter_instance_encode(ctrl_info, 2, &ctrl_info_out);
    }

    if (direction == bcmStatFlexDirectionEgress) {
        pkt_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_PKT;
        byte_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_BYTE;
    } else {
        pkt_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_PKT;
        byte_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_BYTE;
    }

    /* During unit detach, counter thread is shutdown prior to
     * flexstat module being detached. Modules such as field that
     * use flexstats call this function after counter thread is
     * shutdown. In this scenario, evict is NULL. Check for this.
     */
    soc = SOC_CONTROL(unit);
    if (NULL == soc->counter_evict) {
        return BCM_E_INTERNAL;
    }

    sal_memset(&value_zero, 0, sizeof(bcm_stat_value_t));
    for (cnt = 0; cnt < num_entries; cnt++) {
        value_ptr = value_array == NULL? &value_zero: value_array+cnt;

        if ((byte_flag == 1) || (byte_flag == -1)) { /* byte only */
            COMPILER_64_SET(value.bytes, COMPILER_64_HI(value_ptr->bytes),
                          COMPILER_64_LO(value_ptr->bytes));
            rv = soc_counter_generic_set(unit, byte_ctr_id, ctrl_info_out,
                                         0, index + cnt, value.bytes);
            if (rv != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit, "Unable to set byte counter values unit - %d,"
                                           " pool - %d, counter index - %d cnt - %d\n"),
                                   unit, pool_id, index, cnt));
                return BCM_E_FAIL;
            }
        }
        if ((byte_flag == 0) || (byte_flag == -1)) { /* packet only */
            uint64 temp64;
            COMPILER_64_SET(temp64, 0, value_ptr->packets);
            rv = soc_counter_generic_set(unit, pkt_ctr_id, ctrl_info_out,
                                         0, index + cnt, temp64);
            if (rv != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                (BSL_META_U(unit, "Unable to set packet counter values unit - %d,"
                                      " pool - %d, counter index - %d cnt - %d\n"),
                               unit, pool_id, index, cnt ));
                return BCM_E_FAIL;
            }
        }
    }

    return BCM_E_NONE;
#else
    return BCM_E_UNAVAIL;
#endif
}

/*
 * Function:
 *      _bcm_esw_stat_counter_set
 * Description:
 *      Set Flex Counter Values
 * Parameters:
 *      unit                  - (IN) unit number
 *      index                 - (IN) Flex Accounting Table Index
 *      table                 - (IN) Flex Accounting Table
 *      byte_flag             - (IN) Byte/Packet Flag
 *      counter               - (IN) Counter Index
 *      values                - (IN) Counter Value
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_counter_set(
            int              unit,
            uint32           index,
            soc_mem_t        table,
            bcm_stat_object_t object,
            uint32           byte_flag,
            uint32           counter_index,
            bcm_stat_value_t *value)
{
    soc_mem_t                 mem;
#if defined(BCM_TRIDENT2_SUPPORT)
    soc_mem_t                 mem_dual = 0;
    int                       buffer_id = 0;
#endif /* BCM_TRIDENT2_SUPPORT*/
    uint32                    offset_mode=0;
    int                       pool_number=0;
    uint32                    base_idx=0;
    bcm_stat_flex_direction_t direction = bcmStatFlexDirectionIngress;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    uint32                    zero=0;
    uint32                    one=1;
    uint32                    entry_data_size=0;
    void                      *entry_data=NULL;
    uint32                    max_packet_mask=0;
    uint64                    max_byte_mask;
    uint32                    hw_val[2];
    int                       pipe_num = 0;
    uint32                    exact_index=0;

    COMPILER_64_ZERO(max_byte_mask);
    exact_index = index;

    switch(table) {
    case PORT_TABm:
    case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case VLAN_XLATE_EXTDm:
    case MPLS_ENTRY_EXTDm:
    case L3_TUNNELm:
    case L3_ENTRY_2m:
    case L3_ENTRY_4m:
    case EXT_IPV4_UCAST_WIDEm:
    case EXT_IPV6_128_UCAST_WIDEm:
    case EXT_FP_POLICYm:
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case ING_VSANm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
    case L3_DEFIPm:
    case L3_DEFIP_PAIR_128m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
#endif
    case VFP_POLICY_TABLEm:
#if defined (BCM_TOMAHAWK_SUPPORT)
    case VFP_POLICY_TABLE_PIPE0m:
    case VFP_POLICY_TABLE_PIPE1m:
    case VFP_POLICY_TABLE_PIPE2m:
    /* coverity[UNTERMINATED_CASE] */
    case VFP_POLICY_TABLE_PIPE3m:
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, table);
#endif
    /* coverity[FALLTHROUGH] */
    case MPLS_ENTRYm:
    case SOURCE_VPm:
    case L3_IIFm:
    case VRFm:
    case VFIm:
    case VLAN_TABm:
         direction= bcmStatFlexDirectionIngress;
         break;
    case EGR_VLANm:
    case EGR_VFIm:
    case EGR_L3_NEXT_HOPm:
    case EGR_VLAN_XLATEm:
    case EGR_PORTm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case EGR_DVP_ATTRIBUTE_1m:
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case EGR_NAT_PACKET_EDIT_INFOm:
#endif
         direction= bcmStatFlexDirectionEgress;
         break;
    case EGR_IP_TUNNEL_MPLSm:
         direction= bcmStatFlexDirectionEgress;
         index = index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
         break;
    case IFP_POLICY_TABLEm:
#if defined (BCM_TOMAHAWK_SUPPORT)
    case IFP_POLICY_TABLE_PIPE0m:
    case IFP_POLICY_TABLE_PIPE1m:
    case IFP_POLICY_TABLE_PIPE2m:
    case IFP_POLICY_TABLE_PIPE3m:
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, table);
#endif
         direction= bcmStatFlexDirectionIngress;
         break;
    case AGM_MONITOR_TABLEm:
         direction= bcmStatFlexDirectionIngress;
         break;
    default:
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid Flex Counter Memory %s\n"),
                    SOC_MEM_UFNAME(unit, table)));
         return BCM_E_PARAM;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    _CTR_COUNTER_TABLE_IFP(unit, direction, pipe_num, 0, mem);
#else
    mem = _ctr_counter_table[direction][0];
#endif

    COMPILER_64_SET(max_byte_mask, zero, one);
    COMPILER_64_SHL(max_byte_mask,soc_mem_field_length(unit,mem,BYTE_COUNTERf));
    COMPILER_64_SUB_32(max_byte_mask,one);

    max_packet_mask = (1 << soc_mem_field_length(unit,mem,PACKET_COUNTERf));
    max_packet_mask -= 1;

    entry_data_size = WORDS2BYTES(BYTES2WORDS(SOC_MEM_INFO(unit,table).bytes));
    entry_data = sal_alloc(entry_data_size,"flex-counter-table");
    if (entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, table)));
        return BCM_E_INTERNAL;
    }
    if (flex_temp_counter[unit][direction] == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Not initilized or attached yet\n")));
        sal_free(entry_data);
        return BCM_E_CONFIG;
    }
    sal_memset(entry_data,0,entry_data_size);

    if (soc_mem_read(unit,table,MEM_BLOCK_ANY,
                _bcm_esw_stat_flex_table_index_map(unit,table,index),
                entry_data) == SOC_E_NONE) {
        if (soc_mem_field_valid(unit,table,VALIDf)) {
            if (soc_mem_field32_get(unit,table,entry_data,VALIDf)==0) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, table),index));
                sal_free(entry_data);
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                unit,exact_index,table,entry_data,object,&offset_mode,
                (uint32 *)&pool_number,&base_idx);
        if ((offset_mode == 0) && (base_idx == 0)) {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Table:%s:Index:%d:IsNotConfiguredForFlexCounter \n"),
                        SOC_MEM_UFNAME(unit, table),index));
             sal_free(entry_data);
             /* Either Not configured or deallocated before */
             return BCM_E_NOT_FOUND;
        }
        if (direction == bcmStatFlexDirectionIngress) {
            total_entries= flex_ingress_modes[unit][offset_mode].total_counters;
        } else {
            total_entries = flex_egress_modes[unit][offset_mode].total_counters;
        }
        offset_index = counter_index;
        if (offset_index >= total_entries) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Wrong OFFSET_INDEX. Must be < Total Counters %d \n"),
                       total_entries));
            sal_free(entry_data);
            return BCM_E_PARAM;
        }

        if (SOC_IS_TOMAHAWKX(unit)) {
            sal_free(entry_data);
            return th_flex_counter_set(unit, direction, pipe_num, pool_number,
                      base_idx+offset_index, value, 1, byte_flag);
        }

        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        mem = _ctr_counter_table[direction][pool_number];
#if defined(BCM_TRIDENT2_SUPPORT)
        if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
            mem = _ctr_counter_table_x[direction][pool_number];
            mem_dual = _ctr_counter_table_y[direction][pool_number];
        } else
#endif
        {
#if defined (BCM_TOMAHAWK_SUPPORT)
            _CTR_COUNTER_TABLE_IFP(unit, direction, pipe_num, pool_number, mem);
#endif
        }
        if (soc_mem_read(
                    unit,
                    mem,
                    MEM_BLOCK_ANY,
                    base_idx+offset_index,
                    flex_temp_counter[unit][direction]) != BCM_E_NONE){
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            sal_free(entry_data);
            return BCM_E_INTERNAL;
        }
#if defined(BCM_TRIDENT2_SUPPORT)
        if (mem_dual) {
            if (soc_mem_read(
                    unit,
                    mem_dual,
                    MEM_BLOCK_ANY,
                    base_idx+offset_index,
                    flex_temp_counter_dual[unit][direction]) != BCM_E_NONE) {
                BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                sal_free(entry_data);
                return BCM_E_INTERNAL;
            }
        }
#endif /* BCM_TRIDENT2_SUPPORT*/

        if (byte_flag == 1) {
            /* Mask to possible max values */
            COMPILER_64_AND(value->bytes, max_byte_mask);
            /* Update Soft Copy */
            COMPILER_64_SET(flex_byte_counter[unit][direction][pipe_num][pool_number]
                                             [base_idx+offset_index],
                            COMPILER_64_HI(value->bytes),
                            COMPILER_64_LO(value->bytes));
            /* Change Read Hw Copy */
            hw_val[0] = COMPILER_64_LO(value->bytes);
            hw_val[1] = COMPILER_64_HI(value->bytes);
            soc_mem_field_set(
                    unit,
                    mem,
                    (uint32 *)&flex_temp_counter[unit][direction][0],
                    BYTE_COUNTERf,
                    hw_val);
#if defined(BCM_TRIDENT2_SUPPORT)
            if (mem_dual) {
                /* assume total counter value are set in X pipe hardware counter
                 * table. Clear the Y pipe table
                 */
                for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                     buffer_id++) {
                    COMPILER_64_SET(flex_byte_counter_x[unit][direction]
                                    [pool_number][buffer_id]
                                    [base_idx+offset_index],
                                    COMPILER_64_HI(value->bytes),
                                    COMPILER_64_LO(value->bytes));
                    COMPILER_64_ZERO(flex_byte_counter_y[unit][direction]
                                     [pool_number][buffer_id]
                                     [base_idx+offset_index]);
                }
                hw_val[0] = 0;
                hw_val[1] = 0;
                soc_mem_field_set(
                    unit,
                    mem_dual,
                    (uint32 *)&flex_temp_counter_dual[unit][direction][0],
                    BYTE_COUNTERf,
                    hw_val);
            }
#endif /* BCM_TRIDENT2_SUPPORT*/
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Byte Count Value\t:TABLE:%sINDEX:%d "
                                   "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                       SOC_MEM_UFNAME(unit, table),
                       index,
                       pool_number,
                       direction,
                       base_idx+offset_index,
                       COMPILER_64_HI(value->bytes),
                       COMPILER_64_LO(value->bytes)));
        } else {
            value->packets &= max_packet_mask;
            /* Update Soft Copy */
            flex_packet_counter[unit][direction][pipe_num][pool_number]
                               [base_idx+offset_index] = value->packets;

            /* Update Soft Copy */
            COMPILER_64_SET(flex_packet64_counter[unit][direction][pipe_num][pool_number]
                                             [base_idx+offset_index],0,
                             value->packets);
            /* Change Read Hw Copy */
            soc_mem_field_set(
                    unit,
                    mem,
                    (uint32 *)&flex_temp_counter[unit][direction][0],
                    PACKET_COUNTERf,
                    &(value->packets));
#if defined(BCM_TRIDENT2_SUPPORT)
            if (mem_dual) {
                for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                     buffer_id++) {
                    flex_pkt_counter_x[unit][direction][pool_number]
                                      [buffer_id][base_idx+offset_index]
                                      = value->packets;
                    flex_pkt_counter_y[unit][direction][pool_number]
                                      [buffer_id][base_idx+offset_index]
                                      = 0;
                }
                hw_val[0] = 0;
                soc_mem_field_set(
                    unit,
                    mem_dual,
                    (uint32 *)&flex_temp_counter_dual[unit][direction][0],
                    PACKET_COUNTERf,
                    &hw_val[0]);
            }
#endif /* BCM_TRIDENT2_SUPPORT*/
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Packet Count Value\t:TABLE:%sINDEX:%d "
                                   "(@Pool:%dDirection:%dActualOffset%d) : %x \n"),
                       SOC_MEM_UFNAME(unit, table),
                       index,
                       pool_number,
                       direction,
                       base_idx+offset_index,
                       value->packets));
        }
        /* Update Hw Copy */
        if (soc_mem_write(unit,mem,MEM_BLOCK_ALL,
                    base_idx+offset_index,
                    &(flex_temp_counter[unit][direction][0])) != BCM_E_NONE){
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            sal_free(entry_data);
            return BCM_E_INTERNAL;
        }
#if defined(BCM_TRIDENT2_SUPPORT)
        if (mem_dual) {
            if (soc_mem_write(unit,mem_dual,MEM_BLOCK_ALL,
                    base_idx+offset_index,
                    &(flex_temp_counter_dual[unit][direction][0])) != BCM_E_NONE){
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            sal_free(entry_data);
            return BCM_E_INTERNAL;
        }
    }
#endif /* BCM_TRIDENT2_SUPPORT*/
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        sal_free(entry_data);
        return BCM_E_NONE;
    }
    sal_free(entry_data);
    return BCM_E_FAIL;
}

STATIC
bcm_error_t th_flex_centralized_counter_get(int unit,
                                            int sync_mode,
                                            bcm_stat_flex_direction_t  direction,
                                            uint32       pool_number,
                                            int         pipe_num,
                                            uint32           index,
                                            uint32           byte_flag,
                                            bcm_stat_value_t *value)
{
#if defined (BCM_TOMAHAWK_SUPPORT)
    soc_ctr_control_info_t     ctrl_info[2], ctrl_info_out;
    soc_counter_non_dma_id_t  pkt_ctr_id, byte_ctr_id;
    uint64 pkt_ctr_new, byte_ctr_new;
    int rv;
    uint32 flags = 0;

    ctrl_info[0].instance_type = SOC_CTR_INSTANCE_TYPE_POOL;
    ctrl_info[0].instance = pool_number;

    if (pipe_num == -1) {
        ctrl_info_out = ctrl_info[0];
    } else {
        ctrl_info[1].instance_type = SOC_CTR_INSTANCE_TYPE_PIPE;
        ctrl_info[1].instance = pipe_num;
        soc_counter_instance_encode(ctrl_info, 2, &ctrl_info_out);
    }

    if (direction == bcmStatFlexDirectionEgress) {
        pkt_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_PKT;
        byte_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_BYTE;
    } else {
        pkt_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_PKT;
        byte_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_BYTE;
    }

    if (sync_mode) {
        flags |= SOC_COUNTER_SYNC_ENABLE;
    }

    if (byte_flag == 1) {
        rv = soc_counter_generic_get(unit, byte_ctr_id, ctrl_info_out,
                                     flags, index, &byte_ctr_new);
        if (rv != SOC_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                (BSL_META_U(unit, "Unable to retrieve byte counter values unit - %d,"
                    " pool - %d, counter index - %d\n"),
                    unit, pool_number, index));
            return BCM_E_FAIL;
        }
        COMPILER_64_SET(value->bytes, COMPILER_64_HI(byte_ctr_new),
        COMPILER_64_LO(byte_ctr_new));
    } else {
        rv = soc_counter_generic_get(unit, pkt_ctr_id, ctrl_info_out,
                                     flags, index, &pkt_ctr_new);
        if (rv != SOC_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                (BSL_META_U(unit, "Unable to retrieve packet counter values unit - %d,"
                    " pool - %d, counter index - %d\n"),
                    unit, pool_number, index));
            return BCM_E_FAIL;
        }
        value->packets = COMPILER_64_LO(pkt_ctr_new); /* 32 bit for */
        COMPILER_64_SET(value->packets64, COMPILER_64_HI(pkt_ctr_new),
        COMPILER_64_LO(pkt_ctr_new));
    }
    return BCM_E_NONE;
#else
    return BCM_E_UNAVAIL;
#endif /* BCM_TOMAHAWK_SUPPORT */
}

/*
 * Function:
 *      _bcm_esw_stat_counter_get
 * Description:
 *      Get Flex Counter Values
 *      if sync_mode is set, sync the sw accumulated count
 *      with hw count value first, else return sw count.
 *
 * Parameters:
 *      unit                  - (IN) unit number
 *      sync_mode             - (IN) hwcount is to be synced to sw count
 *      index                 - (IN) Flex Accounting Table Index
 *      table                 - (IN) Flex Accounting Table
 *      byte_flag             - (IN) Byte/Packet Flag
 *      counter               - (IN) Counter Index
 *      values                - (OUT) Counter Value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_counter_get(
            int              unit,
            int              sync_mode,
            uint32           index,
            soc_mem_t        table,
            bcm_stat_object_t object,
            uint32           byte_flag,
            uint32           counter_index,
            bcm_stat_value_t *value)
{
    uint32                    loop=0;
    uint32                    offset_mode=0;
    uint32                    pool_number=0;
    uint32                    base_idx=0;
    bcm_stat_flex_direction_t direction = bcmStatFlexDirectionIngress;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    uint32                    entry_data_size=0;
    void                      *entry_data=NULL;
    int                       pipe_num = 0;
    uint32                    exact_index=0;

    exact_index = index;

    switch(table) {
    case PORT_TABm:
    case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case VLAN_XLATE_EXTDm:
    case MPLS_ENTRY_EXTDm:
    case L3_TUNNELm:
    case L3_ENTRY_2m:
    case L3_ENTRY_4m:
    case EXT_IPV4_UCAST_WIDEm:
    case EXT_IPV6_128_UCAST_WIDEm:
    case EXT_FP_POLICYm:
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case ING_VSANm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
    case L3_DEFIPm:
    case L3_DEFIP_PAIR_128m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
#endif
    case VFP_POLICY_TABLEm:
#if defined (BCM_TOMAHAWK_SUPPORT)
    case VFP_POLICY_TABLE_PIPE0m:
    case VFP_POLICY_TABLE_PIPE1m:
    case VFP_POLICY_TABLE_PIPE2m:
    /* coverity[UNTERMINATED_CASE] */
    case VFP_POLICY_TABLE_PIPE3m:
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, table);
#endif
    /* coverity[FALLTHROUGH] */
    case MPLS_ENTRYm:
    case SOURCE_VPm:
    case L3_IIFm:
    case VRFm:
    case VFIm:
    case VLAN_TABm:
         direction= bcmStatFlexDirectionIngress;
         break;
    case EGR_VLANm:
    case EGR_VFIm:
    case EGR_L3_NEXT_HOPm:
    case EGR_VLAN_XLATEm:
    case EGR_PORTm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case EGR_DVP_ATTRIBUTE_1m:
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case EGR_NAT_PACKET_EDIT_INFOm:
#endif
         direction= bcmStatFlexDirectionEgress;
         break;
#if defined (BCM_TOMAHAWK_SUPPORT)
    case IFP_POLICY_TABLE_PIPE0m:
    case IFP_POLICY_TABLE_PIPE1m:
    case IFP_POLICY_TABLE_PIPE2m:
    /* coverity[UNTERMINATED_CASE] */
    case IFP_POLICY_TABLE_PIPE3m:
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, table);
#endif
    /* coverity[FALLTHROUGH] */
    case IFP_POLICY_TABLEm:
         direction= bcmStatFlexDirectionIngress;
         break;
    case AGM_MONITOR_TABLEm:
         direction= bcmStatFlexDirectionIngress;
         break;
#if defined (BCM_TRIDENT2PLUS_SUPPORT)
    case EGR_IP_TUNNEL_MPLSm:
         direction= bcmStatFlexDirectionEgress;
         index = index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
         break;
#endif
    default:
         LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Invalid Flex Counter Memory %s\n"),
                      SOC_MEM_UFNAME(unit, table)));
         return BCM_E_PARAM;
    }
    entry_data_size = WORDS2BYTES(BYTES2WORDS(SOC_MEM_INFO(unit,table).bytes));
    entry_data = sal_alloc(entry_data_size,"flex-counter-table");
    if (entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, table)));
        return BCM_E_INTERNAL;
    }
    sal_memset(entry_data,0,entry_data_size);

    if (soc_mem_read(unit,table,MEM_BLOCK_ANY,
                _bcm_esw_stat_flex_table_index_map(unit,table,index),
                entry_data) == SOC_E_NONE) {
        if (soc_mem_field_valid(unit,table,VALIDf)) {
            if (soc_mem_field32_get(unit,table,entry_data,VALIDf)==0) {
                LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                            (BSL_META_U(unit,
                                        "Table %s  with index %d is Not valid \n"),
                             SOC_MEM_UFNAME(unit, table),index));
                sal_free(entry_data);
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                 unit,
                 exact_index,
                 table,
                 entry_data,
                 object,
                 &offset_mode,
                 &pool_number,
                 &base_idx);
        if ((offset_mode == 0) && (base_idx == 0)) {
             LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Table:%s:Index:%d:is NotConfiguredForFlexCounter \n"),
                          SOC_MEM_UFNAME(unit, table),index));
             sal_free(entry_data);
             /* Either Not configured or deallocated before */
             return BCM_E_NOT_FOUND;
         }
         if (direction == bcmStatFlexDirectionIngress) {
             total_entries = flex_ingress_modes[unit][offset_mode].
                             total_counters;
         } else {
             total_entries = flex_egress_modes[unit][offset_mode].
                             total_counters;
         }
         offset_index = counter_index;
         if (offset_index >= total_entries) {
             LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Invalid offset_index[%d]. total_entries[%d]\n"),
                          offset_index, total_entries));
             sal_free(entry_data);
             return BCM_E_PARAM;
         }

#if defined(BCM_TOMAHAWK_SUPPORT)
        if (soc_feature(unit, soc_feature_centralized_counter)) {
            if(!BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
                pipe_num = -1;
            }
            sal_free(entry_data);
            return th_flex_centralized_counter_get(unit, sync_mode, direction,
                                                   pool_number, pipe_num,
                                                   base_idx + offset_index,
                                                   byte_flag, value);
        }
#endif /* BCM_TOMAHAWK_SUPPORT */

         BCM_STAT_FLEX_COUNTER_LOCK(unit);

         /* sync the software counter with hardware counter */
         if (sync_mode == 1) {
             _bcm_esw_stat_flex_counter_collect(unit, direction,
                                                pool_number,
                                                base_idx + offset_index + loop);
         }

         bcm_stat_value_t_init(value);
         if (byte_flag == 1) {
             COMPILER_64_SET(value->bytes,
                      COMPILER_64_HI(flex_byte_counter[unit]
                                     [direction][pipe_num][pool_number]
                                     [base_idx+offset_index+loop]),
                      COMPILER_64_LO(flex_byte_counter[unit]
                                     [direction][pipe_num][pool_number]
                                     [base_idx+offset_index+loop]));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Byte Count Value\t:TABLE:%sINDEX:%d COUTER-%d"
                                    "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                        SOC_MEM_UFNAME(unit, table),
                        index,
                        loop,
                        pool_number,
                        direction,
                        base_idx+offset_index+loop,
                        COMPILER_64_HI(value->bytes),
                        COMPILER_64_LO(value->bytes)));
         } else {
             value->packets= flex_packet_counter[unit][direction][pipe_num][pool_number]
                                                [base_idx+offset_index+loop];
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Packet Count Value\t:TABLE:%sINDEX:%d COUTER-%d"
                                    "(@Pool:%dDirection:%dActualOffset%d) : %x \n"),
                        SOC_MEM_UFNAME(unit, table),
                        index,
                        loop,
                        pool_number,
                        direction,
                        base_idx+offset_index+loop,
                        value->packets));
             COMPILER_64_SET(value->packets64,
                      COMPILER_64_HI(flex_packet64_counter[unit]
                                     [direction][pipe_num][pool_number]
                                     [base_idx+offset_index+loop]),
                      COMPILER_64_LO(flex_packet64_counter[unit]
                                     [direction][pipe_num][pool_number]
                                     [base_idx+offset_index+loop]));
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Packet64 Count Value\t:TABLE:%sINDEX:%d COUNTER-%d"
                                    "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                        SOC_MEM_UFNAME(unit, table),
                        index,
                        loop,
                        pool_number,
                        direction,
                        base_idx+offset_index+loop,
                        COMPILER_64_HI(value->packets64),
                        COMPILER_64_LO(value->packets64)));
         }
         BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
         sal_free(entry_data);
         return BCM_E_NONE;
    }
    sal_free(entry_data);
    return BCM_E_FAIL;
}

/*
 * Function:
 *      _bcm_esw_stat_counter_raw_get
 * Description:
 *      Get Flex Counter Values
 * Parameters:
 *      unit                  - (IN) unit number
 *      stat_counter_id       - (IN) Stat Counter Id
 *      byte_flag             - (IN) Byte/Packet Flag
 *      counter               - (IN) Counter Index
 *      values                - (OUT) Counter Value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes: Must be called carefully.
 *        Preference should be given to _bcm_esw_stat_counter_get()
 *
 */
bcm_error_t _bcm_esw_stat_counter_raw_get(
            int              unit,
            int              sync_mode,
            uint32           stat_counter_id,
            uint32           byte_flag,
            uint32           counter_index,
            bcm_stat_value_t *value)
{
    uint32                    offset_mode=0;
    uint32                    pool_number=0;
    uint32                    base_idx=0;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    bcm_stat_flex_direction_t direction=bcmStatFlexDirectionIngress;
    bcm_stat_object_t         object=bcmStatObjectIngPort;
    bcm_stat_group_mode_t     group_mode= bcmStatGroupModeSingle;
    int                       pipe_num = 0;
#if defined (BCM_TOMAHAWK_SUPPORT)
    bcm_field_qualify_t       stage;
    uint32                    cmode  = 0;
#endif

    _bcm_esw_stat_get_counter_id_info(unit, stat_counter_id, &group_mode,&object,
                                      &offset_mode,&pool_number,&base_idx);

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id)) {
        stage = _bcm_esw_stat_get_field_stage_from_stat_ctr(unit, stat_counter_id);
        _bcm_esw_get_fp_pipe_from_mode(unit, offset_mode, stage, &pipe_num);
        if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
            (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
            cmode = offset_mode - BCM_CUSTOM_INGRESS_MODE_START;
            offset_mode = flex_custom_ingress_modes[unit][cmode].
                                                offset_mode;
            total_entries = flex_custom_ingress_modes[unit][cmode].
                                          total_counters;
        }
    }
#endif

    /* Validate object id first */
    if (_bcm_esw_stat_validate_object(unit,object,&direction) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid bcm_stat_object_t passed %d \n"),
                   object));
        return BCM_E_PARAM;
    }
    /* Validate group_mode */
    if (_bcm_esw_stat_validate_group(unit,group_mode) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid bcm_stat_group_mode_t passed %d \n"),
                   group_mode));
        return BCM_E_PARAM;
    }
    if (flex_base_index_reference_count[unit][direction]
                                       [pipe_num][pool_number][base_idx] == 0) {
        return BCM_E_PARAM;
    }
    if (direction == bcmStatFlexDirectionIngress) {
        if (total_entries == 0) {
            total_entries = flex_ingress_modes[unit][offset_mode].
                                          total_counters;
        }
    } else {
        total_entries = flex_egress_modes[unit][offset_mode].
                                         total_counters;
    }

    offset_index = counter_index;
    if (offset_index >= total_entries) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong OFFSET_INDEX.Must be < Total Counters %d \n"),
                   total_entries));
        return BCM_E_PARAM;
    }

#if defined(BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_centralized_counter)) {
        if (!BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
            /* Pass pipe_num as -1 if in Global mode */
            pipe_num = -1;
        }
        return th_flex_centralized_counter_get(unit, sync_mode, direction,
                                               pool_number, pipe_num,
                                               base_idx + offset_index,
                                               byte_flag, value);
    }
#endif /* BCM_TOMAHAWK_SUPPORT */

    BCM_STAT_FLEX_COUNTER_LOCK(unit);

    /* sync the software counter with hardware counter */
    if (sync_mode == 1) {
            _bcm_esw_stat_flex_counter_collect(unit, direction,
                                               pool_number, base_idx + offset_index);
    }
    if (byte_flag == 1) {
        COMPILER_64_SET(value->bytes,
                 COMPILER_64_HI(flex_byte_counter[unit]
                                [direction][pipe_num][pool_number]
                                [base_idx+offset_index]),
                 COMPILER_64_LO(flex_byte_counter[unit]
                                [direction][pipe_num][pool_number]
                                [base_idx+offset_index]));
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Byte Count Value\t:COUTER-%d"
                               "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                   offset_index,
                   pool_number,
                   direction,
                   base_idx+offset_index,
                   COMPILER_64_HI(value->bytes),
                   COMPILER_64_LO(value->bytes)));
    } else {
        value->packets= flex_packet_counter[unit][direction][pipe_num][pool_number]
                                           [base_idx+offset_index];

        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Packet Count Value\t:COUTER-%d"
                               "(@Pool:%dDirection:%dActualOffset%d) : %x \n"),
                   offset_index,
                   pool_number,
                   direction,
                   base_idx+offset_index,
                   value->packets));
        COMPILER_64_SET(value->packets64,
                 COMPILER_64_HI(flex_packet64_counter[unit]
                                [direction][pipe_num][pool_number]
                                [base_idx+offset_index]),
                 COMPILER_64_LO(flex_packet64_counter[unit]
                                [direction][pipe_num][pool_number]
                                [base_idx+offset_index]));
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Byte Count Value\t:COUNTER-%d"
                               "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                   offset_index,
                   pool_number,
                   direction,
                   base_idx+offset_index,
                   COMPILER_64_HI(value->packets64),
                   COMPILER_64_LO(value->packets64)));

    }
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_counter_raw_set
 * Description:
 *      Set Flex Counter Values
 * Parameters:
 *      unit                  - (IN) unit number
 *      stat_counter_id       - (IN) Stat Counter Id
 *      byte_flag             - (IN) Byte/Packet Flag
 *      counter               - (IN) Counter Index
 *      values                - (IN) Counter Value
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes: Must be called carefully.
 *        Preference should be given to _bcm_esw_stat_counter_set()
 *
 */
bcm_error_t _bcm_esw_stat_counter_raw_set(
            int              unit,
            uint32           stat_counter_id,
            uint32           byte_flag,
            uint32           counter_index,
            bcm_stat_value_t *value)
{
    soc_mem_t                 mem;
#if defined(BCM_TRIDENT2_SUPPORT)
    soc_mem_t                 mem_dual = 0;
    int                       buffer_id = 0;
#endif /* BCM_TRIDENT2_SUPPORT*/

    uint32                    zero=0;
    uint32                    one=1;
    uint32                    offset_mode=0;
    uint32                    pool_number=0;
    uint32                    base_idx=0;
    uint32                    total_entries=0;
    uint32                    offset_index=0;
    uint32                    max_packet_mask=0;
    uint64                    max_byte_mask;
    uint32                    hw_val[2];
    bcm_stat_flex_direction_t direction=bcmStatFlexDirectionIngress;
    bcm_stat_object_t         object=bcmStatObjectIngPort;
    bcm_stat_group_mode_t     group_mode= bcmStatGroupModeSingle;
    int                       pipe_num = 0;
#if defined (BCM_TOMAHAWK_SUPPORT)
    bcm_field_qualify_t       stage;
    uint32                    cmode = 0;
#endif

    _bcm_esw_stat_get_counter_id_info(unit, stat_counter_id, &group_mode,&object,
                                      &offset_mode,&pool_number,&base_idx);
    /* Validate object id first */
    if (_bcm_esw_stat_validate_object(unit,object,&direction) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid bcm_stat_object_t passed %d \n"),
                   object));
        return BCM_E_PARAM;
    }
    /* Validate group_mode */
    if (_bcm_esw_stat_validate_group(unit,group_mode) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid bcm_stat_group_mode_t passed %d \n"),
                   group_mode));
        return BCM_E_PARAM;
    }
    if (direction == bcmStatFlexDirectionIngress) {
        if (offset_mode < BCM_STAT_FLEX_COUNTER_MAX_MODE) {
            total_entries = flex_ingress_modes[unit][offset_mode].
                                          total_counters;
        }
#ifdef BCM_TOMAHAWK_SUPPORT
        if (SOC_IS_TOMAHAWKX(unit)) {
            stage = _bcm_esw_stat_get_field_stage_from_stat_ctr(unit, stat_counter_id);
            _bcm_esw_get_fp_pipe_from_mode(unit, offset_mode, stage, &pipe_num);
            if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
                (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
                cmode = offset_mode - BCM_CUSTOM_INGRESS_MODE_START;
                offset_mode = flex_custom_ingress_modes[unit][cmode].offset_mode;
                total_entries = flex_custom_ingress_modes[unit][cmode].
                                          total_counters;

            }
        }
#endif
    } else {
        total_entries = flex_egress_modes[unit][offset_mode].
                                         total_counters;
    }
    if (flex_base_index_reference_count[unit][direction][pipe_num]
                                       [pool_number][base_idx] == 0) {
        return BCM_E_PARAM;
    }
    offset_index = counter_index;
    if (offset_index >= total_entries) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong OFFSET_INDEX.Must be < Total Counters %d \n"),
                   total_entries));
        return BCM_E_PARAM;
    }
    if (flex_temp_counter[unit][direction] == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Not initilized or attached yet\n")));
        return BCM_E_CONFIG;
    }

    if (SOC_IS_TOMAHAWKX(unit)) {
        return th_flex_counter_set(unit, direction, pipe_num, pool_number,
                  base_idx+offset_index, value, 1, byte_flag);
    }

    /* NOT Tomahawk */
    mem = _ctr_counter_table[direction][0];
    COMPILER_64_SET(max_byte_mask, zero, one);
    COMPILER_64_SHL(max_byte_mask,soc_mem_field_length(unit,mem,BYTE_COUNTERf));
    COMPILER_64_SUB_32(max_byte_mask,one);

    max_packet_mask = (1 << soc_mem_field_length(unit,mem,PACKET_COUNTERf));
    max_packet_mask -= 1;

    BCM_STAT_FLEX_COUNTER_LOCK(unit);
#if defined(BCM_TRIDENT2_SUPPORT)
    if (soc_feature(unit, soc_feature_advanced_flex_counter)
        && soc_feature(unit, soc_feature_two_ingress_pipes)) {
        mem = _ctr_counter_table_x[direction][pool_number];
        mem_dual = _ctr_counter_table_y[direction][pool_number];
    } else
#endif
    {
        mem = _ctr_counter_table[direction][pool_number];
    }
    if (soc_mem_read(
                    unit,
                    mem,
                    MEM_BLOCK_ANY,
                    base_idx+offset_index,
                    flex_temp_counter[unit][direction]) != BCM_E_NONE){
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            return BCM_E_INTERNAL;
    }
#if defined(BCM_TRIDENT2_SUPPORT)
    if (mem_dual) {
        if (soc_mem_read(
                    unit,
                    mem_dual,
                    MEM_BLOCK_ANY,
                    base_idx+offset_index,
                    flex_temp_counter_dual[unit][direction]) != BCM_E_NONE) {
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            return BCM_E_INTERNAL;
        }
    }
#endif /* BCM_TRIDENT2_SUPPORT*/
    if (byte_flag == 1) {
        /* Mask to possible max values */
        COMPILER_64_AND(value->bytes, max_byte_mask);
        /* Update Soft Copy */
        COMPILER_64_SET(flex_byte_counter[unit][direction][pipe_num][pool_number]
                                         [base_idx+offset_index],
                        COMPILER_64_HI(value->bytes),
                         COMPILER_64_LO(value->bytes));
        /* Change Read Hw Copy */
        hw_val[0] = COMPILER_64_LO(value->bytes);
        hw_val[1] = COMPILER_64_HI(value->bytes);
        soc_mem_field_set(
                unit,
                mem,
                (uint32 *)&flex_temp_counter[unit][direction][0],
                BYTE_COUNTERf,
                hw_val);

#if defined(BCM_TRIDENT2_SUPPORT)
        if (mem_dual) {
            /* Set the Y-pipe value to 0 so that it does not contribute
               to the sum value or the global view */
            for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                     buffer_id++) {
                COMPILER_64_SET(flex_byte_counter_x[unit][direction]
                                [pool_number][buffer_id]
                                [base_idx+offset_index],
                                COMPILER_64_HI(value->bytes),
                                COMPILER_64_LO(value->bytes));
                COMPILER_64_ZERO(flex_byte_counter_y[unit][direction]
                                 [pool_number][buffer_id]
                                 [base_idx+offset_index]);
            }
            hw_val[0] = 0;
            hw_val[1] = 0;
            soc_mem_field_set(
                unit,
                mem_dual,
                (uint32 *)&flex_temp_counter_dual[unit][direction][0],
                BYTE_COUNTERf,
                hw_val);
        }
#endif /* BCM_TRIDENT2_SUPPORT */
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Byte Count Value\t:COUTER-%d"
                               "(@Pool:%dDirection:%dActualOffset%d) : %x:%x \n"),
                   offset_index,
                   pool_number,
                   direction,
                   base_idx+offset_index,
                   COMPILER_64_HI(value->bytes),
                   COMPILER_64_LO(value->bytes)));
    } else {
        value->packets &= max_packet_mask;
        /* Update Soft Copy */
        flex_packet_counter[unit][direction][pipe_num][pool_number]
                           [base_idx+offset_index] = value->packets;
        /* Update Soft Copy */
        COMPILER_64_SET(flex_packet64_counter[unit][direction][pipe_num][pool_number]
                                         [base_idx+offset_index],0,
                         value->packets);

        /* Change Read Hw Copy */
        soc_mem_field_set(
                unit,
                mem,
                (uint32 *)&flex_temp_counter[unit][direction][0],
                PACKET_COUNTERf,
                &(value->packets));
#if defined(BCM_TRIDENT2_SUPPORT)
        if (mem_dual) {
            /* Set the Y-pipe value to 0 so that it does not contribute
               to the sum value or the global view */
            for (buffer_id = 0; buffer_id < _BCM_FLEX_STAT_BUFFS;
                     buffer_id++) {
                flex_pkt_counter_x[unit][direction][pool_number]
                                  [buffer_id][base_idx+offset_index]
                                  = value->packets;
                flex_pkt_counter_y[unit][direction][pool_number]
                                  [buffer_id][base_idx+offset_index]
                                  = 0;
            }
            hw_val[0] = 0;
            soc_mem_field_set(
                unit,
                mem_dual,
                (uint32 *)&flex_temp_counter_dual[unit][direction][0],
                PACKET_COUNTERf,
                &hw_val[0]);
        }
#endif /* BCM_TRIDENT2_SUPPORT */
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Packet Count Value\t:COUTER-%d"
                               "(@Pool:%dDirection:%dActualOffset%d) : %x \n"),
                   offset_index,
                   pool_number,
                   direction,
                   base_idx+offset_index,
                   value->packets));
    }
    /* Update Hw Copy */
    if (soc_mem_write(unit,mem,MEM_BLOCK_ALL,
                base_idx+offset_index,
                flex_temp_counter[unit][direction]) != BCM_E_NONE){
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        return BCM_E_INTERNAL;
    }
#if defined(BCM_TRIDENT2_SUPPORT)
    if (mem_dual) {
        if (soc_mem_write(unit,mem_dual,MEM_BLOCK_ALL,
                    base_idx+offset_index,
                    flex_temp_counter_dual[unit][direction]) != BCM_E_NONE){
            BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
            return BCM_E_INTERNAL;
        }
    }
#endif /* BCM_TRIDENT2_SUPPORT*/
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}

/*
 * Function:
 *      tomahawk_stat_flex_callback
 * Description:
        This is the tomahawk specific version of stat flex callback function
 * Parameters:
 *      unit                  - (IN) unit number
 *
 * Return Value:
 *      none
 * Notes:
 *
 * This function updates packet, byte counter values maintained in the flex
 * counter software database. The ASCII diagram below illustrates how this
 * accumulation is carried out
 *
 * packet_count, byte_count
 *  .----------.
 *  | H/W Flex |
 *  | counter  |------------------        flex_packet_counter
 *  | Residual |                  |         ------------
 *  | Value    |                  |        | Flex       |
 *  .----------.                  \/       | counter    |
 *                                +------->| pkt, byte  |
 *  .----------.                  /\       | software   |
 *  | S/W Cache|                  |        | database   |
 *  | evicted  |                  |         ------------
 *  | counter  |------------------        flex_packet64_counter
 *  | Value    |                          flex_byte_counter
 *  .----------.                          residual_packet_counter
 * pkt_ctr_new, byte_ctr_new              residual_byte_counter
 *
 * For each invocation of this function, the following happens
 * a) Residual counter values are DMA'd from H/W for all pipes into
 *    ctr_tbl_hw.
 * b) pkt_ctr_new and byte_ctr_new are computed as the sum of all evicted
 *    counter values across all pipes. Evicted counter values are stored
 *    in soc->counter_sw_val[].
 * c) packet_count and byte_count are computed as the sum of all residual
 *    counter values across all pipes. These are the values that have been
 *    DMA'd in step (a).
 * d) The next step is dependent on whether counter eviction is enabled or not,
 *    both scenarios are described below
 *
 *  Counter eviction is DISABLED
 *  ----------------------------
 *  For this case, packet_count, byte_count completely contain the current count
 *  value and hence they can be assigned to the flex counter database fields
 *  flex_packet_counter, flex_packet64_counter and flex_byte_counter. Since the
 *  hardware counters are < 64 bits, software also handles wrap around correction.
 *
 *  Counter eviction is ENABLED
 *  ---------------------------
 *  For this case, packet_count, byte_count only contain the residual counter values,
 *  post eviction. pkt_ctr_new and byte_ctr_new contain the evicted values. Hence the
 *  total pkt/byte counter value is computed as a sum of pkt_ctr_new, packet_count and
 *  byte_ctr_new, byte_count respectively. When counter eviction is enabled, software
 *  has to accumulate the value of count, because H/W is cleared after every read.
 *  The residual_packet_counter and residual_byte_counter databases are also updated
 *  appropriately based on the residual counter values that were read during the iteration.
 */

STATIC
void tomahawk_stat_flex_callback (int unit)
{
#ifdef BCM_TOMAHAWK_SUPPORT
    int alloc_size;
    soc_mem_t mem=INVALIDm;
    int mem_id;
    int pipe;
    uint64 pkt_ctr_new[SOC_MAX_NUM_PIPES];
    uint64 byte_ctr_new[SOC_MAX_NUM_PIPES];
    uint32 pkt_ctr_hw;
    uint64 byte_ctr_hw, max_byte_count;
    uint32 num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32 pool_id;
    uint32 packet_count[4], old_pkt_count;
    uint32 index, max_packet_count = 0;
    uint32 byte_count_read[2];
    uint32 flex_ctr_update_control_reg_value=0;
    uint64 byte_count[4], old_byte_count, temp_count;
    soc_control_t *soc;
    soc_counter_evict_t *evict;
    bcm_stat_flex_direction_t direction;
    ing_flex_ctr_counter_table_0_entry_t *ctr_tbl_hw
                    [BCM_STAT_FLEX_COUNTER_MAX_DIRECTION][SOC_MAX_NUM_PIPES];
    int pipe_dma_done[SOC_MAX_NUM_PIPES];
    uint32 size_pool[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    soc_ctr_control_info_t ctrl_info;
    soc_counter_non_dma_id_t pkt_ctr_id;
    soc_counter_non_dma_id_t byte_ctr_id;
    int rv, maxpipe = 1; /*maxpipe is > 1 only for multipipe devices */
    size_pool[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            size_flex_egress_pool;

    soc = SOC_CONTROL(unit);

    for (direction = bcmStatFlexDirectionIngress;
         direction < bcmStatFlexDirectionCount;
         direction++) {
        for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
            alloc_size= sizeof(ing_flex_ctr_counter_table_0_entry_t)*size_pool
                           [direction];
            ctr_tbl_hw[direction][pipe] = soc_cm_salloc(unit, alloc_size,
                                 "flex counter table dma");
            if (NULL == ctr_tbl_hw[direction][pipe]) {
                goto cleanup;
            }
            sal_memset(ctr_tbl_hw[direction][pipe], 0, alloc_size);
        }
    }

    num_pools[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            num_flex_egress_pools;

    COMPILER_64_ZERO(max_byte_count);
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    for (direction = bcmStatFlexDirectionIngress;
         direction < bcmStatFlexDirectionCount;
         direction++) {
         if (flex_temp_counter[unit][direction] == NULL) {
             LOG_WARN(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Not Initilized or attached \n")));
             continue;
         }
         for (pool_id = 0; pool_id < num_pools[direction]; pool_id++) {
             maxpipe = 1;
             if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_id)) {
                 /* In this mode, counters must be collected independently for each pipe. */
                 maxpipe = NUM_PIPE(unit);
             }
             if (soc_reg32_get(
                            unit,
                            _pool_ctr_register[direction][pool_id],
                            REG_PORT_ANY,
                            0,
                            &flex_ctr_update_control_reg_value) != SOC_E_NONE) {
                 continue;
             }
             if (soc_reg_field_get(
                                unit,
                                _pool_ctr_register[direction][pool_id],
                                flex_ctr_update_control_reg_value,
                                COUNTER_POOL_ENABLEf) == 0) {
                 continue;
             }
             /* read from eviction counter data base */
             ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_POOL;
             ctrl_info.instance = pool_id;
             if (direction == bcmStatFlexDirectionEgress) {
                 mem_id = pool_id + 1 + SOC_TH_STAT_ING_FLEX_POOL_MAX;
                 pkt_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_PKT;
                 byte_ctr_id = SOC_COUNTER_NON_DMA_EGR_FLEX_BYTE;
             } else {
                 mem_id = pool_id + 1;
                 pkt_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_PKT;
                 byte_ctr_id = SOC_COUNTER_NON_DMA_ING_FLEX_BYTE;
                 if (maxpipe > 1) {
                     ctrl_info.instance_type = SOC_CTR_INSTANCE_TYPE_POOL_PIPE;
                     ctrl_info.instance = pool_id << 4;
                 }
             }

             if (NULL == soc->counter_evict) {
                 return;
             }
             evict = &soc->counter_evict[mem_id];

             for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
                  mem = evict->mem[pipe];
                  pipe_dma_done[pipe] = FALSE;
                  max_packet_count = (1 << soc_mem_field_length(unit, mem,
                                                               PACKET_COUNTERf));
                  max_packet_count -= 1;
                  COMPILER_64_SET(max_byte_count, 0, 1);
                  COMPILER_64_SHL(max_byte_count, soc_mem_field_length(unit, mem,
                                                                       BYTE_COUNTERf));
                  COMPILER_64_SUB_32(max_byte_count, 1);

                 /* dma the current hw counter value which is not accounted for
                  * in counter eviction processing.
                  */
                 if (soc_mem_read_range(
                          unit,
                          mem,
                          MEM_BLOCK_ANY,
                          soc_mem_index_min(unit,mem),
                          soc_mem_index_max(unit,mem),
                          ctr_tbl_hw[direction][pipe]) == BCM_E_NONE) {
                     pipe_dma_done[pipe] = TRUE;
                 }
              }

              for (index = soc_mem_index_min(unit,mem);
                   index <= soc_mem_index_max(unit,mem);
                   index++) {
                  for (pipe = 0; pipe < maxpipe; pipe++) {
                       COMPILER_64_ZERO(pkt_ctr_new[pipe]);
                       COMPILER_64_ZERO(byte_ctr_new[pipe]);

                       if (maxpipe > 1) {
                           ctrl_info.instance = ((pool_id << 4) | pipe);
                       }
                       rv = soc_counter_generic_get(unit, pkt_ctr_id, ctrl_info,
                                                    0, index, &pkt_ctr_new[pipe]);
                       if (rv != SOC_E_NONE) {
                           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "Unable to retrieve evicted"
                                                 " packet counter values unit - %d,"
                                                 " pool - %d, counter index - %d\n"),
                                      unit, pool_id, index));
                           continue;
                       }
                       rv = soc_counter_generic_get(unit, byte_ctr_id, ctrl_info,
                                                    0, index, &byte_ctr_new[pipe]);
                       if (rv != SOC_E_NONE) {
                           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                     (BSL_META_U(unit,
                                                 "Unable to retrieve evicted"
                                                 " byte counter values unit - %d,"
                                                 " pool - %d, counter index - %d\n"),
                                      unit, pool_id, index));
                           continue;
                       }
                   }

                   COMPILER_64_ZERO(byte_count[0]);
                   packet_count[0] = 0;
                   /* add up all pipes for hw counter */
                   /* If the pool is allocated to IFP, and IFP is in pipe local mode
                      store it in individual counters instead of adding up.
                    */
                   for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
                       mem = evict->mem[pipe];
                       /*coverity[uninit_use : FALSE] */
                       if (pipe_dma_done[pipe] == FALSE) {
                           continue;
                       }

                       soc_mem_field_get(
                               unit,
                               mem,
                               (uint32 *)&ctr_tbl_hw[direction][pipe][index],
                               PACKET_COUNTERf,
                               &pkt_ctr_hw);

                       if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_id)) {
                           packet_count[pipe] = pkt_ctr_hw;
                       } else {
                           packet_count[0] += pkt_ctr_hw;
                       }


                       soc_mem_field_get(
                             unit,
                             mem,
                             (uint32 *)&ctr_tbl_hw[direction][pipe][index],
                             BYTE_COUNTERf,
                             byte_count_read);
                       COMPILER_64_SET(byte_ctr_hw, byte_count_read[1],
                                       byte_count_read[0]);
                       if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_id)) {
                           COMPILER_64_SET(byte_count[pipe], byte_count_read[1],
                                           byte_count_read[0]);
                       } else {
                           COMPILER_64_ADD_64(byte_count[0],byte_ctr_hw);
                       }
                   }

                   /* Device can operate in a mode with counter eviction
                    * enabled or counter eviction disabled. When counter
                    * eviction is disabled, accumulation occurs in hardware.
                    * When counter eviction is enabled, CLEAR_ON_READ is set,
                    * which causes h/w to be cleared on every read. Hence
                    * software has to accumalate residual values. Total counter
                    * value is then computed as the sum of accumulated residuals
                    * and evicted value obtained from counter module.
                    */
                   if (0 == soc->ctr_evict_interval) {
                       /* Account for possibility of overflow when
                        * counter eviction is disabled
                        */

                       for (pipe = 0; pipe < maxpipe; pipe++) {
                           COMPILER_64_ZERO(temp_count);
                           COMPILER_64_ADD_64(temp_count, flex_packet64_counter[unit]
                                             [direction][pipe][pool_id][index]);
                           COMPILER_64_SUB_64(temp_count, pkt_ctr_new[pipe]);
                           COMPILER_64_SUB_64(temp_count, residual_packet_counter[unit]
                                              [direction][pipe][pool_id][index]);
                           old_pkt_count = COMPILER_64_LO(temp_count);
                           if (old_pkt_count > packet_count[pipe]) {
                               packet_count[pipe] += (max_packet_count - old_pkt_count);
                           }

                           COMPILER_64_ZERO(temp_count);
                           COMPILER_64_ADD_64(temp_count, flex_byte_counter[unit]
                                             [direction][pipe][pool_id][index]);
                           COMPILER_64_SUB_64(temp_count, byte_ctr_new[pipe]);
                           COMPILER_64_SUB_64(temp_count, residual_byte_counter[unit]
                                              [direction][pipe][pool_id][index]);
                           COMPILER_64_ZERO(old_byte_count);
                           COMPILER_64_ADD_64(old_byte_count, temp_count);
                           if (COMPILER_64_GT(old_byte_count, byte_count[pipe])) {
                               COMPILER_64_ZERO(temp_count);
                               COMPILER_64_ADD_64(temp_count, max_byte_count);
                               COMPILER_64_SUB_64(temp_count, old_byte_count);
                               COMPILER_64_ADD_64(byte_count[pipe], temp_count);
                           }
                       }
                    }
                   /* The counter value is comprised of two parts, evicted value
                    * plus the residual value in the hardware table. Here we
                    * retrieve these two value at the same time, add up
                    * and save to the global counter array
                    */
                   for (pipe = 0; pipe < maxpipe; pipe++) {
                       COMPILER_64_ZERO(flex_packet64_counter[unit]
                                        [direction][pipe][pool_id][index]);
                       COMPILER_64_ADD_64(flex_packet64_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          pkt_ctr_new[pipe]);
                       COMPILER_64_ADD_64(flex_packet64_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          residual_packet_counter[unit]
                                          [direction][pipe][pool_id][index]);
                       COMPILER_64_ADD_32(flex_packet64_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          packet_count[pipe]);
                       flex_packet_counter[unit][direction][pipe][pool_id][index]
                           = COMPILER_64_LO(flex_packet64_counter[unit]
                                            [direction][pipe][pool_id][index]);
                       COMPILER_64_ZERO(flex_byte_counter[unit]
                                        [direction][pipe][pool_id][index]);
                       COMPILER_64_ADD_64(flex_byte_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          byte_ctr_new[pipe]);
                       COMPILER_64_ADD_64(flex_byte_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          residual_byte_counter[unit]
                                          [direction][pipe][pool_id][index]);
                       COMPILER_64_ADD_64(flex_byte_counter[unit]
                                          [direction][pipe][pool_id][index],
                                          byte_count[pipe]);
                       if (soc->ctr_evict_interval) {
                           /* Update packet and byte counter residuals only
                            * if counter eviction is enabled
                            */
                           COMPILER_64_ADD_32(residual_packet_counter[unit]
                                              [direction][pipe][pool_id][index],
                                              packet_count[pipe]);
                           COMPILER_64_ADD_64(residual_byte_counter[unit]
                                              [direction][pipe][pool_id][index],
                                              byte_count[pipe]);
                       }
                  }
            }
         }
    }
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

cleanup:    
    for (direction = bcmStatFlexDirectionIngress;
         direction < bcmStatFlexDirectionCount;
         direction++) {
        for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
            if (ctr_tbl_hw[direction][pipe] != NULL) {
                soc_cm_sfree(unit, ctr_tbl_hw[direction][pipe]);
            }
        }
    }
#endif
    return;
}

/*      _bcm_flex_stat_data32_rollover
 * Description:
 *      deal with the n-bits(n <= 32) data rollover case
 *      and add the diff to the 64-bits data.
 * Parameters:
 *      pre                  - (IN) previous n-bits data
 *      cur                  - (IN) current n-bits data
 *      roll_size          - (IN) the size when n-bits data rollovers
 *      sum                - (OUT) the summary of diff between pre and cur.
 * Return Value:
 *      None
 * Notes:
 *      None
 */
STATIC void
_bcm_flex_stat_data32_rollover(uint32 pre, uint32 cur,
                               uint64 roll_size, uint64 *sum)
{
    uint64  cur64, pre64;

    if (cur == pre) {
        return;
    } else if (cur > pre) {
        COMPILER_64_SET(cur64, 0, (cur - pre));
    } else {
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META("Roll over  happend \n")));
        COMPILER_64_SET(cur64, 0, cur);
        COMPILER_64_SET(pre64, 0, pre);
        COMPILER_64_ADD_64(cur64, roll_size);
        COMPILER_64_SUB_64(cur64, pre64);
    }

    COMPILER_64_ADD_64(*sum, cur64);
    return;
}

/*
 * Function:
 *      _bcm_flex_stat_data64_rollover
 * Description:
 *      deal with the n-bits(32< n < 64) data rollover case
 *      and add the diff to the 64-bits data.
 * Parameters:
 *      pre                  - (IN) previous n-bits data
 *      cur                  - (IN) current n-bits data
 *      roll_size          - (IN) the size when n-bits data rollovers
 *      sum                - (OUT) the summary of diff between pre and cur.
 * Return Value:
 *      None
 * Notes:
 *      None
 */
STATIC void
_bcm_flex_stat_data64_rollover(uint64 pre, uint64 cur,
                               uint64 roll_size, uint64 *sum)
{
    uint64  max_byte_mask;

    if (sum == NULL) {
        return;
    }

    COMPILER_64_ZERO(max_byte_mask);
    COMPILER_64_SET(max_byte_mask,
                    COMPILER_64_HI(roll_size),
                    COMPILER_64_LO(roll_size));

    COMPILER_64_SUB_32(max_byte_mask,1);

    COMPILER_64_AND(pre, max_byte_mask);
    COMPILER_64_AND(cur, max_byte_mask);
    if (COMPILER_64_GT(pre, cur)) {
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META("Roll over  happend \n")));
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META("...Read Byte Count    : %x:%x\n"),
                     COMPILER_64_HI(cur),
                     COMPILER_64_LO(cur)));
        COMPILER_64_ADD_64(cur, roll_size);
        COMPILER_64_SUB_64(cur,pre);
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META("...Diffed Byte Count    : %x:%x\n"),
                     COMPILER_64_HI(cur),
                     COMPILER_64_LO(cur)));
    } else {
        COMPILER_64_SUB_64(cur, pre);
    }
    /* Add difference (if it is) */
    if (!COMPILER_64_IS_ZERO(cur)) {
        COMPILER_64_ADD_64(*sum, cur);
        LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                    (BSL_META("New Byte Count Value : %x:%x\n"),
                     COMPILER_64_HI(*sum),
                     COMPILER_64_LO(*sum)));
    }
    return;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_counter_collect
 * Description:
 *      Flex Counter accumulation routine called periodically
 *      to sync s/w copy with h/w copy.
 * Parameters:
 *      unit                  - (IN) unit number
 *      cnt_direction         - (IN) counter direction
 *                                   if -1 accumulate for all counters
 *                                      for ingress and egress else
 *                                   retrieve a specific counter
 *      flexctr_poll          - (IN) Ingress or Egress flex counter pool
 *                                   if -1 Both Ingress and Egress pools
 *                                   else Ingress or Egress pool.
 *      counter_idx           - (IN) counter index.
 *                                   if -1 accumulate for all counters
 *                                   else retrieve a specific counter.
 * Return Value:
 *      None
 * Notes:
 *       This routine accumulates all the flex counters and
 *       update the software copy of the flex counters.
 *       Counter thread invokes periodically this routine.
 *       If counter index is not -1 then only a specific counter
 *       value is read from harware and synced to the software copy.
 *       used by all the counter_sync_get API's
 *
 */
STATIC void
_bcm_esw_stat_flex_counter_collect(int                       unit,
                                   bcm_stat_flex_direction_t cnt_direction,
                                   uint32                    flexctr_pool,
                                   uint32                    counter_idx)
{
    soc_mem_t mem;
#if defined(BCM_TRIDENT2_SUPPORT)
    soc_mem_t mem_dual = 0;
#endif /* BCM_TRIDENT2_SUPPORT*/

    uint32    num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32    pool_id = 0;
    bcm_stat_flex_direction_t direction = bcmStatFlexDirectionIngress;
    uint32    packet_count=0;
    uint32    index;
    uint32    zero=0;
    uint32    one=1;
    uint32    flex_ctr_update_control_reg_value=0;
    uint64    byte_count;
    uint64    prev_masked_byte_count;
    uint64    max_byte_size;
    uint64    max_byte_mask;
    soc_memacc_t memacc_pkt_x;
    soc_memacc_t memacc_byte_x;
#if defined(BCM_TRIDENT2_SUPPORT)
    soc_memacc_t memacc_pkt_y;
    soc_memacc_t memacc_byte_y;
    uint64       *byte_cur_ptr_x, *byte_cur_ptr_y;
    uint32       *pkt_cur_ptr_x, *pkt_cur_ptr_y;
    uint8     toggle = 0;
#endif
    uint64    packet64_count;
    uint64    prev_masked_packet64_count;
    uint64    max_packet64_size;
    uint64    max_packet64_mask;
    uint32    mem_index_min;
    uint32    mem_index_max;
    uint32    max_directions = BCM_STAT_FLEX_COUNTER_MAX_DIRECTION;
    int pipe_num = 0;

    if (SOC_IS_TOMAHAWKX(unit)) {
        tomahawk_stat_flex_callback(unit);
        return;
    }

    COMPILER_64_ZERO(byte_count);
    COMPILER_64_ZERO(prev_masked_byte_count);
    COMPILER_64_ZERO(max_byte_size);
    COMPILER_64_ZERO(max_byte_mask);

    COMPILER_64_ZERO(packet64_count);
    COMPILER_64_ZERO(prev_masked_packet64_count);
    COMPILER_64_ZERO(max_packet64_size);
    COMPILER_64_ZERO(max_packet64_mask);


    num_pools[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            num_flex_egress_pools;
    /*
    size_pool[bcmStatFlexDirectionIngress]= SOC_INFO(unit).
                                            size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress] = SOC_INFO(unit).
                                            size_flex_egress_pool;
     */

    /*
     * if counter_idx != -1, request is to sync
     * sw count with hw count of a specific counter
     */
    if (counter_idx != -1) {
        if ((cnt_direction < bcmStatFlexDirectionIngress) ||
            (cnt_direction > bcmStatFlexDirectionEgress)) {
            return;
        }

        direction = cnt_direction;
        max_directions = direction + 1;

        /* coverity[unsigned_compare : FALSE] */
        if ((flexctr_pool > num_pools[direction]) || (flexctr_pool < 0)) {
            return;
        }
        pool_id = flexctr_pool;
        num_pools[direction] = pool_id + 1;
    } else {
        flexctr_pool = 0;
    }

    /*Not Tomahawk */
    mem = _ctr_counter_table[direction][pool_id];

    COMPILER_64_SET(max_byte_size, zero, one);
    COMPILER_64_SHL(max_byte_size,soc_mem_field_length(unit,mem,BYTE_COUNTERf));

    COMPILER_64_SET(max_byte_mask,
                    COMPILER_64_HI(max_byte_size),
                    COMPILER_64_LO(max_byte_size));
    COMPILER_64_SUB_32(max_byte_mask,one);


    COMPILER_64_SET(max_packet64_size, zero, one);
    COMPILER_64_SHL(max_packet64_size,soc_mem_field_length(unit,mem,PACKET_COUNTERf));

    COMPILER_64_SET(max_packet64_mask,
                    COMPILER_64_HI(max_packet64_size),
                    COMPILER_64_LO(max_packet64_size));
    COMPILER_64_SUB_32(max_packet64_mask,one);

    for ( ; direction < max_directions; direction++) {
        if (flex_temp_counter[unit][direction] == NULL) {
            LOG_WARN(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Not Initilized or attached \n")));
            continue;
        }
#if defined(BCM_TRIDENT2_SUPPORT)
        if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
            if (flex_temp_counter_dual[unit][direction] == NULL) {
                LOG_WARN(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Not Initilized or attached \n")));
                continue;
            }
         }
#endif
        for (pool_id = flexctr_pool; pool_id < num_pools[direction]; pool_id++){
            if (soc_reg32_get(
                            unit,
                            _pool_ctr_register[direction][pool_id],
                            REG_PORT_ANY,
                            0,
                            &flex_ctr_update_control_reg_value) != SOC_E_NONE) {
                continue;
            }
            if (soc_reg_field_get(
                            unit,
                            _pool_ctr_register[direction][pool_id],
                            flex_ctr_update_control_reg_value,
                            COUNTER_POOL_ENABLEf) == 0) {
                continue;
            }
#if defined(BCM_TRIDENT2_SUPPORT)
            if (!SOC_IS_TOMAHAWKX(unit) && soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                mem = _ctr_counter_table_x[direction][pool_id];
                mem_dual = _ctr_counter_table_y[direction][pool_id];

            } else
#endif
            {
#if defined(BCM_TOMAHAWK_SUPPORT)
                _CTR_COUNTER_TABLE_IFP(unit, direction, pipe_num, pool_id, mem);
#else
                mem = _ctr_counter_table[direction][pool_id];
#endif
            }

            mem_index_min = soc_mem_index_min(unit, mem);
            mem_index_max = soc_mem_index_max(unit, mem);

            /*
             * if counter_idx != -1, retrieve specific
             * counter index from the flex counter memory pool.
             */
            if (counter_idx != -1) {
                if ((counter_idx < mem_index_min) ||
                    (counter_idx > mem_index_max)) {
                    return;
                }
                mem_index_min = mem_index_max = counter_idx;
            }

            /* read all the counters or a specific counter from memory */
            if (counter_idx == -1 ) {
                if (soc_mem_read_range(unit, mem, MEM_BLOCK_ANY,
                                       mem_index_min, mem_index_max,
                                       flex_temp_counter[unit][direction])
                                       != BCM_E_NONE) {
                    return;
                }
            } else {
                if (soc_mem_read(unit, mem, MEM_BLOCK_ANY,
                                 counter_idx,
                                &flex_temp_counter[unit][direction][counter_idx])
                                 != BCM_E_NONE) {
                    return;
                }
            }

            if (((soc_memacc_init(unit, mem, PACKET_COUNTERf, &memacc_pkt_x))) ||
                ((soc_memacc_init(unit, mem, BYTE_COUNTERf, &memacc_byte_x)))) {
                  return;
            }

#if defined(BCM_TRIDENT2_SUPPORT)
            if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                /* read all the counters or a specific counter from memory */
                if (counter_idx == -1 ) {
                    if (soc_mem_read_range(
                                       unit, mem_dual, MEM_BLOCK_ANY,
                                       mem_index_min, mem_index_max,
                                       flex_temp_counter_dual[unit][direction])
                                       != BCM_E_NONE) {
                        return;
                    }
                } else {
                    if (soc_mem_read(unit, mem_dual, MEM_BLOCK_ANY,
                          counter_idx,
                          &flex_temp_counter_dual[unit][direction][counter_idx])
                                 != BCM_E_NONE) {
                        return;
                    }
                }

                if (((soc_memacc_init(unit, mem_dual, PACKET_COUNTERf, &memacc_pkt_y))) ||
                    ((soc_memacc_init(unit, mem_dual, BYTE_COUNTERf, &memacc_byte_y)))) {
                    return;
                }

                if (flex_byte_counter_x[unit][direction][pool_id][0] == NULL) {
                    LOG_WARN(BSL_LS_BCM_FLEXCTR,
                             (BSL_META_U(unit,
                                         "Not Initilized or attached \n")));
                    continue;
                }

                if (flex_pkt_counter_x[unit][direction][pool_id][0] == NULL) {
                    LOG_WARN(BSL_LS_BCM_FLEXCTR,
                             (BSL_META_U(unit,
                                         "Not Initilized or attached \n")));
                    continue;
                }

                if (flex_byte_counter_y[unit][direction][pool_id][0] == NULL) {
                    LOG_WARN(BSL_LS_BCM_FLEXCTR,
                             (BSL_META_U(unit,
                                         "Not Initilized or attached \n")));
                    continue;
                }

                if (flex_pkt_counter_y[unit][direction][pool_id][0] == NULL) {
                    LOG_WARN(BSL_LS_BCM_FLEXCTR,
                             (BSL_META_U(unit,
                                         "Not Initilized or attached \n")));
                    continue;
                }
            }
#endif /* BCM_TRIDENT2_SUPPORT*/

            for (index = mem_index_min; index <= mem_index_max; index++) {
                packet_count = soc_memacc_field32_get(&memacc_pkt_x,
                            (uint32 *)&flex_temp_counter[unit][direction][index]);
#if defined(BCM_TRIDENT2_SUPPORT)
                if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                    uint32 dual_packet_count;
                    toggle = flex_counter_toggle[unit][direction][pool_id][index];
                    pkt_cur_ptr_x =
                        flex_pkt_counter_x[unit][direction][pool_id][toggle]
                        + index;
                    *pkt_cur_ptr_x = packet_count;
                    _bcm_flex_stat_data32_rollover(flex_pkt_counter_x[unit]
                                                   [direction][pool_id][!toggle]
                                                   [index],
                                                   *pkt_cur_ptr_x,
                                                   max_packet64_size,
                                                   flex_packet64_counter[unit]
                                                   [direction][pipe_num][pool_id] + index
                                                   );

                    dual_packet_count = soc_memacc_field32_get(&memacc_pkt_y,
                                (uint32 *)&flex_temp_counter_dual[unit][direction][index]);
                    pkt_cur_ptr_y =
                        flex_pkt_counter_y[unit][direction][pool_id][toggle]
                        + index;
                    *pkt_cur_ptr_y = dual_packet_count;
                    _bcm_flex_stat_data32_rollover(flex_pkt_counter_y[unit]
                                                   [direction][pool_id]
                                                   [!toggle][index],
                                                   *pkt_cur_ptr_y,
                                                   max_packet64_size,
                                                   flex_packet64_counter[unit]
                                                   [direction][pipe_num][pool_id] + index
                                                   );
                    packet_count += dual_packet_count;
                }
#endif /* BCM_TRIDENT2_SUPPORT*/
                if (flex_packet_counter[unit][direction][pipe_num][pool_id][index]
                    != packet_count) {
                    LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                (BSL_META_U(unit,
                                            "Direction:%dPool:%d==>"
                                             "Old Packet Count Value\t:"
                                             "Index:%d %x\tNew Packet Count Value %x\n"),
                                 direction,
                                 pool_id,
                                 index,
                                 flex_packet_counter[unit]
                                 [direction][pipe_num][pool_id][index],
                                 packet_count));
                    flex_packet_counter[unit][direction][pipe_num][pool_id][index] =
                               packet_count;
                }

                soc_memacc_field64_get(&memacc_byte_x,
                            (uint32 *)&flex_temp_counter[unit][direction][index],
                            &byte_count);

#if defined(BCM_TRIDENT2_SUPPORT)
                if (soc_feature(unit, soc_feature_advanced_flex_counter_dual_pipe)) {
                    uint64 dual_byte_count;

                    byte_cur_ptr_x =
                        flex_byte_counter_x[unit][direction][pool_id][toggle]
                        + index;
                    COMPILER_64_SET(*byte_cur_ptr_x,
                                    COMPILER_64_HI(byte_count),
                                    COMPILER_64_LO(byte_count));
                    _bcm_flex_stat_data64_rollover(flex_byte_counter_x[unit]
                                                   [direction][pool_id]
                                                   [!toggle][index],
                                                   *byte_cur_ptr_x,
                                                   max_byte_size,
                                                   flex_byte_counter[unit]
                                                   [direction][0][pool_id] + index
                                                   );

                    soc_memacc_field64_get(&memacc_byte_y,
                        (uint32 *)&flex_temp_counter_dual[unit][direction][index],
                        &dual_byte_count);
                    byte_cur_ptr_y =
                        flex_byte_counter_y[unit][direction][pool_id][toggle]
                        + index;
                    COMPILER_64_SET(*byte_cur_ptr_y,
                                    COMPILER_64_HI(dual_byte_count),
                                    COMPILER_64_LO(dual_byte_count));
                    _bcm_flex_stat_data64_rollover(flex_byte_counter_y[unit]
                                                   [direction][pool_id]
                                                   [!toggle][index],
                                                   *byte_cur_ptr_y,
                                                   max_byte_size,
                                                   flex_byte_counter[unit]
                                                   [direction][0][pool_id] + index
                                                   );
                    flex_counter_toggle[unit][direction][pool_id][index]= !toggle;
                    continue;
                }
#endif /* BCM_TRIDENT2_SUPPORT*/


                   COMPILER_64_SET(packet64_count,0, packet_count);
                   COMPILER_64_SET(prev_masked_packet64_count,
                                   COMPILER_64_HI(flex_packet64_counter[unit]
                                                  [direction][pipe_num][pool_id][index]),
                                   COMPILER_64_LO(flex_packet64_counter[unit]
                                                  [direction][pipe_num][pool_id][index]));
                   COMPILER_64_AND(prev_masked_packet64_count,max_packet64_mask);
                   if (COMPILER_64_GT(prev_masked_packet64_count, packet64_count)) {
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Roll over  happend \n")));
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "...Read Packet64 Count    : %x:%x\n"),
                                    COMPILER_64_HI(packet64_count),
                                    COMPILER_64_LO(packet64_count)));
                       COMPILER_64_ADD_64(packet64_count,max_packet64_size);
                       COMPILER_64_SUB_64(packet64_count,prev_masked_packet64_count);
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "...Diffed 64-Packet Count    : %x:%x\n"),
                                    COMPILER_64_HI(packet64_count),
                                    COMPILER_64_LO(packet64_count)));
                   } else {
                       COMPILER_64_SUB_64(packet64_count,prev_masked_packet64_count);
                   }
                   /* Add difference (if it is) */
                   if (!COMPILER_64_IS_ZERO(packet64_count)) {
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Direction:%dPool:%d==>"
                                                "Old 64-Packet Count Value\t"
                                                ":Index:%d %x:%x\t"),
                                    direction,pool_id,index,
                                    COMPILER_64_HI(flex_packet64_counter[unit]
                                    [direction][pipe_num][pool_id][index]),
                                    COMPILER_64_LO(flex_packet64_counter[unit]
                                    [direction][pipe_num][pool_id][index])));
                       COMPILER_64_ADD_64(flex_packet64_counter[unit]
                                          [direction][pipe_num][pool_id][index],
                                          packet64_count);
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "New 64-Packet Value : %x:%x\n"),
                                    COMPILER_64_HI(flex_packet64_counter[unit]
                                    [direction][pipe_num][pool_id][index]),
                                    COMPILER_64_LO(flex_packet64_counter[unit][direction]
                                    [pipe_num][pool_id][index])));
                   }



                   COMPILER_64_SET(prev_masked_byte_count,
                                   COMPILER_64_HI(flex_byte_counter[unit]
                                                  [direction][pipe_num][pool_id][index]),
                                   COMPILER_64_LO(flex_byte_counter[unit]
                                                  [direction][pipe_num][pool_id][index]));
                   COMPILER_64_AND(prev_masked_byte_count,max_byte_mask);
                   if (COMPILER_64_GT(prev_masked_byte_count, byte_count)) {
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Roll over  happend \n")));
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "...Read Byte Count    : %x:%x\n"),
                                    COMPILER_64_HI(byte_count),
                                    COMPILER_64_LO(byte_count)));
                       COMPILER_64_ADD_64(byte_count,max_byte_size);
                       COMPILER_64_SUB_64(byte_count,prev_masked_byte_count);
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "...Diffed Byte Count    : %x:%x\n"),
                                    COMPILER_64_HI(byte_count),
                                    COMPILER_64_LO(byte_count)));
                   } else {
                       COMPILER_64_SUB_64(byte_count,prev_masked_byte_count);
                   }
                   /* Add difference (if it is) */
                   if (!COMPILER_64_IS_ZERO(byte_count)) {
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Direction:%dPool:%d==>"
                                                "Old Byte Count Value\t"
                                                ":Index:%d %x:%x\t"),
                                    direction,pool_id,index,
                                    COMPILER_64_HI(flex_byte_counter[unit]
                                    [direction][pipe_num][pool_id][index]),
                                    COMPILER_64_LO(flex_byte_counter[unit]
                                    [direction][pipe_num][pool_id][index])));
                       COMPILER_64_ADD_64(flex_byte_counter[unit]
                                          [direction][pipe_num][pool_id][index],
                                          byte_count);
                       LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "New Byte Count Value : %x:%x\n"),
                                    COMPILER_64_HI(flex_byte_counter[unit]
                                    [direction][pipe_num][pool_id][index]),
                                    COMPILER_64_LO(flex_byte_counter[unit][direction]
                                    [pipe_num][pool_id][index])));
                   }
              }
         }
    }
    return;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_callback
 * Description:
 *      Flex Counter threads callback function. It is called periodically
 *      to sync s/w copy with h/w copy.
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
void _bcm_esw_stat_flex_callback(int unit)
{
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    _bcm_esw_stat_flex_counter_collect(unit, -1, -1, -1);
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    return;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_get_table_info
 * Description:
 *      Get Table related Information based on accounting object value
 * Parameters:
 *      object                - (IN) Flex Accounting Object
 *      table                 - (OUT) Flex Accounting Table
 *      direction             - (OUT) Flex Data flow direction
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_table_info(
            int                       unit,
            bcm_stat_object_t         object,
            uint32                    expected_num_tables,
            uint32                    *actual_num_tables,
            soc_mem_t                 *table,
            bcm_stat_flex_direction_t *direction)
{
    if (expected_num_tables < 1) {
        return BCM_E_PARAM;
    }
    switch((int32)object) {
    case bcmStatObjectIngPort:
         *table=PORT_TABm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngVlan:
         *table=VLAN_TABm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngVlanXlate:
    case bcmStatObjectIngVlanXlateSecondLookup:
#if defined(BCM_TRIUMPH3_SUPPORT)
         if (SOC_IS_TRIUMPH3(unit)) {
             *table=VLAN_XLATE_EXTDm;
             *direction= bcmStatFlexDirectionIngress;
         } else
#endif
         {
             *table=VLAN_XLATEm;
             *direction= bcmStatFlexDirectionIngress;
         }
         break;
    case bcmStatObjectIngVfi:
         *table=VFIm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngL3Intf:
         *table=L3_IIFm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngVrf:
         *table=VRFm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngPolicy:
         *table=VFP_POLICY_TABLEm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngNiv:
    case bcmStatObjectIngMplsVcLabel:
    case bcmStatObjectIngGport:
         *table=SOURCE_VPm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngMplsSwitchLabel:
    case bcmStatObjectIngMplsSwitchSecondLabel:
#if defined(BCM_TRIUMPH3_SUPPORT)
         if (SOC_IS_TRIUMPH3(unit)) {
             *table=MPLS_ENTRY_EXTDm;
             *direction= bcmStatFlexDirectionIngress;
         } else
#endif
         {
             *table=MPLS_ENTRYm;
             *direction= bcmStatFlexDirectionIngress;
         }
         break;
#if defined(BCM_TRIUMPH3_SUPPORT)
    case bcmStatObjectIngMplsFrrLabel:
         *table=L3_TUNNELm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngL3Host:

        if (!soc_feature(unit, soc_feature_l3_extended_host_entry)) {
            return BCM_E_PARAM;
        }

         if (SOC_IS_TD2_TT2(unit)) {
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_ENTRY_IPV4_MULTICASTm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)]=L3_ENTRY_IPV6_MULTICASTm;
             }
         } else {
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=EXT_IPV4_UCAST_WIDEm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=EXT_IPV6_128_UCAST_WIDEm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_ENTRY_2m;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)]=L3_ENTRY_4m;
             }
         }
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngTrill:
         *table=MPLS_ENTRY_EXTDm;
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngMimLookupId:
#if defined(BCM_TRIUMPH3_SUPPORT)
         if (SOC_IS_TRIUMPH3(unit)) {
             *table=MPLS_ENTRY_EXTDm;
             *direction= bcmStatFlexDirectionIngress;
         } else
#endif
         {
             *table=MPLS_ENTRYm;
             *direction= bcmStatFlexDirectionIngress;
         }
         break;
    case bcmStatObjectIngL2Gre:
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)++]=SOURCE_VPm;
         }
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)]=VFIm;
         }
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngEXTPolicy:
         *table=EXT_FP_POLICYm;
         *direction= bcmStatFlexDirectionIngress;
         break;
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case  bcmStatObjectIngVxlan:
        if (SOC_IS_TD2_TT2(unit)) {
             if ((*actual_num_tables) < expected_num_tables) {
                  table[(*actual_num_tables)++]=SOURCE_VPm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)]=VFIm;
             }
             *direction= bcmStatFlexDirectionIngress;
        }
         break;
    case  bcmStatObjectIngVsan:
         if (!SOC_IS_TOMAHAWKX(unit) && SOC_IS_TD2_TT2(unit)) {
             *table=ING_VSANm;
             *direction= bcmStatFlexDirectionIngress;
        }
         break;
    case  bcmStatObjectIngFcoe:
        if (SOC_IS_TD2_TT2(unit)) {
             *table=L3_ENTRY_IPV4_MULTICASTm;
             *direction= bcmStatFlexDirectionIngress;
        }
         break;
    case  bcmStatObjectIngL3Route:
        if (SOC_IS_TD2_TT2(unit)) {
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_DEFIPm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_DEFIP_PAIR_128m;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_DEFIP_ALPM_IPV4_1m;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_DEFIP_ALPM_IPV6_64_1m;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)]=L3_DEFIP_ALPM_IPV6_128m;
             }
             *direction= bcmStatFlexDirectionIngress;
        }
         break;
    case bcmStatObjectIngIpmc:
         if (SOC_IS_TD2_TT2(unit)) {
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=L3_ENTRY_IPV4_MULTICASTm;
             }
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)]=L3_ENTRY_IPV6_MULTICASTm;
             }
         }
         *direction= bcmStatFlexDirectionIngress;
         break;
    case bcmStatObjectIngVxlanDip:
         if (SOC_IS_TD2_TT2(unit)) {
             if ((*actual_num_tables) < expected_num_tables) {
                 table[(*actual_num_tables)++]=VLAN_XLATEm;
             }
             *direction= bcmStatFlexDirectionIngress;
         }
         break;
#endif /* BCM_TRIDENT2_SUPPORT */
    case bcmIntStatObjectIngAgm:
    case bcmIntStatObjectIngAgmSecondLookup:
         *table=AGM_MONITOR_TABLEm;
         *direction= bcmStatFlexDirectionIngress;
         break;

    case bcmStatObjectEgrPort:
         *table=EGR_PORTm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrVlan:
         *table=EGR_VLANm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrVlanXlate:
    case bcmStatObjectEgrVlanXlateSecondLookup:
         *table=EGR_VLAN_XLATEm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrVfi:
         *table=EGR_VFIm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrNiv:
    case bcmStatObjectEgrL3Intf:
         *table=EGR_L3_NEXT_HOPm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrWlan:
         *table=EGR_L3_NEXT_HOPm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrMim:
         *table=EGR_L3_NEXT_HOPm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrMimLookupId:
         *table=EGR_VLAN_XLATEm;
         *direction= bcmStatFlexDirectionEgress;
         break;
    case bcmStatObjectEgrL2Gre:
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)++]=EGR_DVP_ATTRIBUTE_1m;
         }
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)]=EGR_VFIm;
         }
         *direction= bcmStatFlexDirectionEgress;
         break;
#if defined(BCM_TRIDENT2_SUPPORT)
    case  bcmStatObjectEgrVxlan:
        if (SOC_IS_TD2_TT2(unit)) {
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)++]=EGR_DVP_ATTRIBUTE_1m;
         }
         if ((*actual_num_tables) < expected_num_tables) {
              table[(*actual_num_tables)++]=EGR_L3_NEXT_HOPm;
         }
         if ((*actual_num_tables) < expected_num_tables) {
             table[(*actual_num_tables)]=EGR_VFIm;
         }
         *direction= bcmStatFlexDirectionEgress;
        }
         break;
    case  bcmStatObjectEgrL3Nat:
        if (SOC_IS_TD2_TT2(unit)) {
            *table=EGR_NAT_PACKET_EDIT_INFOm;
            *direction= bcmStatFlexDirectionEgress;
        }
         break;
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    case  bcmStatObjectEgrFieldStageEgress:
        if (SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) {
            *table=EFP_POLICY_TABLEm;
            *direction= bcmStatFlexDirectionEgress;
        }
    break;
    case  bcmStatObjectEgrMplsTunnelLabel:
    case  bcmStatObjectEgrMplsTunnelSecondLabel:
        if (SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_APACHE(unit)) {
            *table=EGR_IP_TUNNEL_MPLSm;
            *direction= bcmStatFlexDirectionEgress;
        }
    break;
#endif /* BCM_TRIDENT2PLUS_SUPPORT */ 
    default:
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid Object is passed %d\n"),
                    object));
         return BCM_E_PARAM;
    }
    (*actual_num_tables)++;
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_detach_egress_table_counters
 * Description:
 *      Detach i.e. Disable Egresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_detach_egress_table_counters(
            int       unit,
            bcm_stat_object_t object_id,
            soc_mem_t egress_table,
            uint32    index)
{
    uint32                          offset_mode=0;
    uint32                          pool_number=0;
    uint32                          base_idx=0;
    uint32                          egress_entry_data_size=0;
    uint32                          flex_entries=0;
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info={0};
    void                            *egress_entry_data=NULL;
    bcm_stat_object_t               object=bcmStatObjectEgrPort;
    uint32                          stat_counter_id=0;
    uint32                          exact_index=0;

    if (!((egress_table == EGR_VLANm) ||
          (egress_table == EGR_VFIm)  ||
          (egress_table == EGR_L3_NEXT_HOPm) ||
          (egress_table == EGR_VLAN_XLATEm)  ||
#if defined(BCM_TRIUMPH3_SUPPORT)
          (egress_table == EGR_DVP_ATTRIBUTE_1m) ||
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
          (egress_table == EGR_NAT_PACKET_EDIT_INFOm) ||
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
          (egress_table == EFP_POLICY_TABLEm) ||
          (egress_table == EGR_IP_TUNNEL_MPLSm) ||
#endif /* BCM_TRIDENT2PLUS_SUPPORT */ 
          (egress_table == EGR_PORTm))) {
           LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "Invalid Flex Counter Egress Memory %s\n"),
                      SOC_MEM_UFNAME(unit, egress_table)));
           return BCM_E_PARAM;
    }

    exact_index = index;
    if (egress_table == EGR_IP_TUNNEL_MPLSm) {
        index = index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
    }

    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Deallocating EGRESS counter for Table %s with index %d \n"),
               SOC_MEM_UFNAME(unit, egress_table),index));
    egress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                             SOC_MEM_INFO(unit, egress_table).bytes));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Deallocating EgressCounter Table:%s:with"
                           "index:%d:ENTRY_BYTES:%d\n"),
               SOC_MEM_UFNAME(unit, egress_table),
               index,egress_entry_data_size));
    egress_entry_data = sal_alloc(egress_entry_data_size,"egress_table");
    if (egress_entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, egress_table)));
         return BCM_E_INTERNAL;
    }
    sal_memset(egress_entry_data,0,egress_entry_data_size);

    if (soc_mem_read(unit, egress_table, MEM_BLOCK_ANY,
                 _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                 egress_entry_data) == SOC_E_NONE) {
        if (soc_mem_field_valid(unit, egress_table, VALIDf)) {
            if (soc_mem_field32_get(unit, egress_table, egress_entry_data,
                                    VALIDf)==0) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, egress_table), index));
                sal_free(egress_entry_data);
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                 unit,exact_index,egress_table,egress_entry_data,object_id,
                 &offset_mode,&pool_number,&base_idx);
        if ((offset_mode == 0) && (base_idx == 0)) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Table:%s:Index:%d:is NotConfiguredForFlexCtrYet\n"),
                        SOC_MEM_UFNAME(unit, egress_table), index));
             sal_free(egress_entry_data);
             return BCM_E_NOT_FOUND;/*Either NotConfigured/DeallocatedBefore */
        }

        /* Decrement reference counts */
        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress][0]
                                       [pool_number][base_idx]--;
        flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                       attached_entries -= flex_egress_modes[unit]
                                           [offset_mode].total_counters;
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);


        /* Clear Counter Values when reference count is zero */
        if ((flex_base_index_reference_count[unit]
                [bcmStatFlexDirectionEgress][0] [pool_number][base_idx]) == 0 ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Clearing Counter Tables %s contents:Offset:%d Len:%d\n"),
                       SOC_MEM_UFNAME(unit, egress_table),
                       base_idx,
                       flex_egress_modes[unit][offset_mode].total_counters));
            flex_ctr_offset_info.all_counters_flag =  1;
            _bcm_esw_stat_flex_set(unit, index, egress_table, -1,
                                   flex_ctr_offset_info,
                                   &flex_entries,
                                   offset_mode,pool_number,base_idx,
                                   bcmStatFlexDirectionEgress);
        }

        /* Reset flex fields */
        _bcm_esw_set_flex_counter_fields_values(unit,
                                                exact_index,
                                                egress_table,
                                                egress_entry_data,
                                                object_id,0,0,0);
        if (soc_mem_write(unit, egress_table, MEM_BLOCK_ALL,
                      _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                      egress_entry_data) != SOC_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table:%s:Index:%d: encounter some problem \n"),
                       SOC_MEM_UFNAME(unit, egress_table), index));
            sal_free(egress_entry_data);
            return    BCM_E_INTERNAL;
        }
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Deallocated Table:%s:Index:%d:mode:%d"
                               "reference_count %d \n"),
                   SOC_MEM_UFNAME(unit, egress_table),index,offset_mode,
                   flex_base_index_reference_count[unit]
                   [bcmStatFlexDirectionEgress][0][pool_number][base_idx]));
        if (object_id != bcmStatObjectEgrMplsTunnelSecondLabel) {
            if(_bcm_esw_stat_flex_get_egress_object(unit,egress_table,
                        index,egress_entry_data,&object) != BCM_E_NONE) {
                sal_free(egress_entry_data);
                return BCM_E_INTERNAL;
            }
        } else {
            object = object_id;
        }
        sal_free(egress_entry_data);

        /* Disable flex pool for this stat on no flex entries */
        if (flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                           attached_entries  == 0) {
            _bcm_esw_stat_flex_enable_pool(
                    unit,bcmStatFlexDirectionEgress,
                    _pool_ctr_register[bcmStatFlexDirectionEgress][pool_number],
                    0);
        }
        _bcm_esw_stat_get_counter_id(
                      unit, flex_egress_modes[unit][offset_mode].group_mode,
                      object,offset_mode,pool_number,
                      base_idx,&stat_counter_id);
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress]
                                           [0][pool_number][base_idx] == 0) {
            if (_bcm_esw_stat_flex_insert_stat_id(unit,
                        local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "WARMBOOT:Couldnot add entry in scache memory."
                                      "Attach it\n")));
            }
        }
        return BCM_E_NONE;
    }
    sal_free(egress_entry_data);
    return    BCM_E_NOT_FOUND;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_destroy_egress_table_counters
 * Description:
 *      Destroy Egresss accounting table's statistics completely
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Accounting Table
 *      offset_mode           - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_destroy_egress_table_counters(
            int                  unit,
            soc_mem_t            egress_table,
            int                  lookup,
            bcm_stat_object_t    object,
            bcm_stat_flex_mode_t offset_mode,
            uint32               base_idx,
            uint32               pool_number)
{
    uint32                          free_count=0;
    uint32                          alloc_count=0;
    uint32                          largest_free;
    uint32                          used_by_table=0;
    uint32                          stat_counter_id=0;

    if (flex_egress_modes[unit][offset_mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex CounterMode:%d:Not configured yet\n"),
                   offset_mode));
        return BCM_E_NOT_FOUND;
    }
    if (shr_aidxres_list_elem_state(flex_aidxres_list_handle
                                    [unit][bcmStatFlexDirectionEgress][0]
                                    [pool_number],base_idx) != BCM_E_EXISTS) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong base index %u \n"),
                   base_idx));
        return BCM_E_NOT_FOUND;
    }
    if (flex_base_index_reference_count[unit]
        [bcmStatFlexDirectionEgress][0][pool_number][base_idx] != 0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Reference count is  %d.. Please detach entries first..\n"),
                   flex_base_index_reference_count[unit]
                   [bcmStatFlexDirectionEgress][0][pool_number][base_idx]));
        return BCM_E_BUSY;
    }
    switch(egress_table) {
    case EGR_VLANm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_TABLE;
         break;
    case EGR_VFIm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_VFI_TABLE;
         break;
    case EGR_L3_NEXT_HOPm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE;
         break;
    case EGR_VLAN_XLATEm:
         if (lookup == 1) {
             used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE;
         } else if (lookup == 2) {
             used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE_FOR_SCND_LKUP;
         }
         break;
    case EGR_PORTm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_PORT_TABLE;
         break;
#if defined(BCM_TRIUMPH3_SUPPORT)
    case EGR_DVP_ATTRIBUTE_1m:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE;
         break;
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case EGR_NAT_PACKET_EDIT_INFOm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE;
         break;
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    case EFP_POLICY_TABLEm:
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE;
         break;
     case EGR_IP_TUNNEL_MPLSm:
         if (lookup == 1) {
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_TABLE;
         } else if (lookup == 2) {
         used_by_table=FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_SCND_LBL_TABLE;
         }
         break;
#endif /* BCM_TRIDENT2PLUS_SUPPORT */ 
    default:
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid Flex Counter Egress Memory %s\n"),
                    SOC_MEM_UFNAME(unit, egress_table)));
         return BCM_E_PARAM;
    }

    /* Free pool list */
    if (shr_aidxres_list_free(flex_aidxres_list_handle
                              [unit][bcmStatFlexDirectionEgress][0][pool_number],
                              base_idx) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Freeing memory Table:%s:encounter problem due entry not found or due to some other issue  \n"),
                   SOC_MEM_UFNAME(unit, egress_table)));
        return BCM_E_NOT_FOUND;
    }
    _bcm_esw_stat_get_counter_id(
                  unit, flex_egress_modes[unit][offset_mode].group_mode,
                  object,offset_mode,pool_number,base_idx,&stat_counter_id);
    if (_bcm_esw_stat_flex_delete_stat_id(unit,
                 local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "WARMBOOT: Couldnot Delete entry in scache memory.\n")));
    }
    shr_aidxres_list_state(flex_aidxres_list_handle
                           [unit][bcmStatFlexDirectionEgress][0][pool_number],
                           NULL,NULL,NULL,NULL,
                           &free_count,&alloc_count,&largest_free,NULL);
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Pool status free_count:%d alloc_count:%d largest_free:%d"
                           "used_by_tables:%d used_entries:%d\n"),
               free_count,alloc_count,largest_free,
               flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
               [pool_number].used_by_tables,
               flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
               [pool_number].used_entries));
    flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                  used_entries -= flex_egress_modes[unit]
                                  [offset_mode].total_counters;
    if (flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
        used_entries == 0) {
        flex_pool_stat[unit][bcmStatFlexDirectionEgress]
                      [0][pool_number].used_by_tables &= ~used_by_table;
        SHR_BITCLR(flex_pool_stat[unit][bcmStatFlexDirectionEgress]
                      [0][pool_number].used_by_objects, object);
    }
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_egress_modes[unit][offset_mode].reference_count--;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_detach_ingress_table_counters1
 * Description:
 *      Detach i.e. Disable Igresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_detach_ingress_table_counters1(
            int                 unit,
            soc_mem_t           ingress_table,
            uint32              index,
            bcm_stat_object_t   object)
{
    uint32                          offset_mode = 0;
    uint32                          pool_number = 0;
    uint32                          base_idx = 0;
    uint32                          ingress_entry_data_size = 0;
    uint32                          flex_entries = 0;
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info = {0};
    void                            *ingress_entry_data = NULL;
    uint32                          stat_counter_id = 0;
    int                             pipe_num = 0;
    int                             pipe = 0, maxpipe = 1, unused_pool;
    uint32                          total_counters = 0;
    if (!((ingress_table == PORT_TABm) ||
         (ingress_table == VLAN_XLATEm)  ||
#if defined(BCM_TRIUMPH3_SUPPORT)
         (ingress_table == VLAN_XLATE_EXTDm)  ||
         (ingress_table == MPLS_ENTRY_EXTDm)  ||
         (ingress_table == L3_TUNNELm)  ||
         (ingress_table == L3_ENTRY_2m)  ||
         (ingress_table == L3_ENTRY_4m)  ||
         (ingress_table == EXT_IPV4_UCAST_WIDEm)  ||
         (ingress_table == EXT_IPV6_128_UCAST_WIDEm)  ||
         (ingress_table == EXT_FP_POLICYm)  ||
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
         (ingress_table == ING_VSANm) ||
         (ingress_table == L3_ENTRY_IPV4_MULTICASTm) ||
         (ingress_table == L3_ENTRY_IPV6_MULTICASTm) ||
         (ingress_table == L3_DEFIPm) ||
         (ingress_table == L3_DEFIP_PAIR_128m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV4_1m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV6_64_1m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV6_128m) ||
#endif
#if  defined (BCM_TRIDENT2PLUS_SUPPORT)
         (ingress_table == FP_POLICY_TABLEm)          ||
#endif
#if defined(BCM_TOMAHAWK_SUPPORT) 
         (ingress_table == IFP_POLICY_TABLEm)          ||
         (SOC_IS_TOMAHAWKX(unit) &&
         ((ingress_table == IFP_POLICY_TABLE_PIPE0m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE1m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE2m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE3m)   ||
          (ingress_table == AGM_MONITOR_TABLEm))) ||
#endif
         (ingress_table == VFP_POLICY_TABLEm)  ||
#if defined(BCM_TOMAHAWK_SUPPORT) 
        (SOC_IS_TOMAHAWKX(unit) &&
         ((ingress_table == VFP_POLICY_TABLE_PIPE0m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE1m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE2m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE3m))) ||
#endif
         (ingress_table == MPLS_ENTRYm)  ||
         (ingress_table == SOURCE_VPm)  ||
         (ingress_table == L3_IIFm)  ||
         (ingress_table == VRFm)  ||
         (ingress_table == VFIm)  ||
         (ingress_table == VLAN_TABm))) {
          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Invalid Flex Counter Ingress Memory %s\n"),
                     SOC_MEM_UFNAME(unit, ingress_table)));
          return BCM_E_PARAM;
    }
#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
#endif
    ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                              SOC_MEM_INFO(unit, ingress_table).bytes));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Deallocating IngressCounter Table:%s:Index:%d:"
                           " ENTRY_BYTES:%d \n"),
               SOC_MEM_UFNAME(unit, ingress_table),
               index,ingress_entry_data_size));
    ingress_entry_data = sal_alloc(ingress_entry_data_size,"ingress_table");
    if (ingress_entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, ingress_table)));
        return BCM_E_INTERNAL;
    }
    sal_memset(ingress_entry_data,0,ingress_entry_data_size);
#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_th_ifp_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                 _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                 ingress_entry_data) == SOC_E_NONE) {
#else
    if (soc_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                 _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                 ingress_entry_data) == SOC_E_NONE) {
#endif /* BCM_TOMAHAWK_SUPPORT */
        if (soc_mem_field_valid(unit,ingress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data,
                                    VALIDf) == 0) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                sal_free(ingress_entry_data);
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                 unit,index,ingress_table,ingress_entry_data, object,
                 &offset_mode,&pool_number,&base_idx);
        if ((offset_mode == 0) && (base_idx == 0)) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Table:%s:Index %d IsNotConfiguredForFlexCounter\n"),
                        SOC_MEM_UFNAME(unit, ingress_table),index));
             sal_free(ingress_entry_data);
             return BCM_E_NOT_FOUND;/*Either NotConfigured/deallocated before*/
        }

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
        (_bcm_esw_is_fp_table(ingress_table))) {
        uint32 cmode = 0;
        bcm_error_t rv;
 
        rv = _bcm_esw_custom_stat_group_id_retrieve(unit, offset_mode, pipe_num, 
                                                    pool_number, base_idx,
                                                    &stat_counter_id);
        if (rv !=BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "pipe %x Invalid mode value %d %d %d\n"),
                   pipe_num, offset_mode, pool_number, base_idx));
            _bcm_esw_stat_get_counter_id(
                          unit, flex_ingress_modes[unit][offset_mode].group_mode,
                          object,offset_mode,pool_number,base_idx,&stat_counter_id);
            total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
        } else {
            cmode = stat_counter_map[unit][stat_counter_id].mode;
            total_counters = flex_custom_ingress_modes[unit][cmode - BCM_CUSTOM_INGRESS_MODE_START].
                                            total_counters;
        }
    } else
#endif
    {
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][offset_mode].group_mode,
                  object,offset_mode,pool_number,base_idx,&stat_counter_id);
    total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
    }


        /* Decrement reference count */
        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                       [pool_number][base_idx]--;
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                       attached_entries -= total_counters;
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

        /* Clear Counter Values */
        if ((flex_base_index_reference_count[unit]
                [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]) == 0 ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Clearing Counter Tables %s contents:Offset:%d Len:%d\n"),
                       SOC_MEM_UFNAME(unit, ingress_table),
                       base_idx,
                       total_counters));
            flex_ctr_offset_info.all_counters_flag =  1;
            _bcm_esw_stat_flex_set(unit,index,ingress_table, -1,
                                   flex_ctr_offset_info,&flex_entries,
                                   offset_mode,pool_number,base_idx,
                                   bcmStatFlexDirectionIngress);
        }

        /* Reset Table Values */
        _bcm_esw_set_flex_counter_fields_values(
                 unit,index,ingress_table,ingress_entry_data,object,0,0,0);
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit) &&
            ((IFP_POLICY_TABLEm == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE0m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE1m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE2m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE3m == ingress_table))) {
            uint32 zero = 0;

            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            G_COUNTf, &zero);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            Y_COUNTf, &zero);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            R_COUNTf, &zero);
        }
#endif
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (soc_th_ifp_mem_write(unit,ingress_table, MEM_BLOCK_ALL,
                          _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                          ingress_entry_data) != SOC_E_NONE) {
#else
        if (soc_mem_write(unit,ingress_table, MEM_BLOCK_ALL,
                          _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                          ingress_entry_data) != SOC_E_NONE) {
#endif /* BCM_TOMAHAWK_SUPPORT */
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table:%s:Index %d encounter some problem \n"),
                       SOC_MEM_UFNAME(unit, ingress_table),index));
            sal_free(ingress_entry_data);
            return    BCM_E_INTERNAL;
        }

       LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "Deallocated for Table:%sIndex:%d:"
                              "mode %d reference_count %d\n"),
                  SOC_MEM_UFNAME(unit, ingress_table),index,offset_mode,
                  flex_base_index_reference_count[unit]
                  [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]));
        sal_free(ingress_entry_data);

#if defined(BCM_TOMAHAWK_SUPPORT)
        if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
            /* In this mode, counters must be collected independently for each pipe. */
            maxpipe = NUM_PIPE(unit);
        }

#endif
        unused_pool = 1;
        for(pipe = 0; pipe < maxpipe; ++pipe) {
            if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe][pool_number].
                           attached_entries != 0) {
                unused_pool = 0;
                break;
            }
        }

        /* Disable flex pool for this stat on no flex entries */
        if (unused_pool) {
            _bcm_esw_stat_flex_enable_pool(
                     unit,
                     bcmStatFlexDirectionIngress,
                     _pool_ctr_register[bcmStatFlexDirectionIngress]
                                       [pool_number],
                     0);
        }
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                           [pipe_num][pool_number][base_idx] == 0) {
            if (_bcm_esw_stat_flex_insert_stat_id(unit,
                        local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "WARMBOOT: Couldnot add entry in scache memory."
                                      "Attach it\n")));
            }
        }
        return BCM_E_NONE;
    }
    sal_free(ingress_entry_data);
    return    BCM_E_NOT_FOUND;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_detach_ingress_table_counters
 * Description:
 *      Detach i.e. Disable Igresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_detach_ingress_table_counters(
            int       unit,
            soc_mem_t ingress_table,
            uint32    index)
{
    uint32                          offset_mode = 0;
    uint32                          pool_number = 0;
    uint32                          base_idx = 0;
    uint32                          ingress_entry_data_size = 0;
    uint32                          flex_entries = 0;
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info = {0};
    void                            *ingress_entry_data = NULL;
    bcm_stat_object_t               object=bcmStatObjectIngPort;
    uint32                          stat_counter_id = 0;
    int                             pipe_num = 0;
    int                             pipe = 0, maxpipe = 1, unused_pool;
    uint32                          total_counters = 0;
    if (!((ingress_table == PORT_TABm) ||
         (ingress_table == VLAN_XLATEm)  ||
#if defined(BCM_TRIUMPH3_SUPPORT)
         (ingress_table == VLAN_XLATE_EXTDm)  ||
         (ingress_table == MPLS_ENTRY_EXTDm)  ||
         (ingress_table == L3_TUNNELm)  ||
         (ingress_table == L3_ENTRY_2m)  ||
         (ingress_table == L3_ENTRY_4m)  ||
         (ingress_table == EXT_IPV4_UCAST_WIDEm)  ||
         (ingress_table == EXT_IPV6_128_UCAST_WIDEm)  ||
         (ingress_table == EXT_FP_POLICYm)  ||
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
         (ingress_table == ING_VSANm) ||
         (ingress_table == L3_ENTRY_IPV4_MULTICASTm) ||
         (ingress_table == L3_ENTRY_IPV6_MULTICASTm) ||
         (ingress_table == L3_DEFIPm) ||
         (ingress_table == L3_DEFIP_PAIR_128m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV4_1m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV6_64_1m) ||
         (ingress_table == L3_DEFIP_ALPM_IPV6_128m) ||
#endif
#if  defined (BCM_TRIDENT2PLUS_SUPPORT)
         (ingress_table == FP_POLICY_TABLEm)          ||
#endif
#if defined(BCM_TOMAHAWK_SUPPORT) 
         (ingress_table == IFP_POLICY_TABLEm)          ||
         (SOC_IS_TOMAHAWKX(unit) &&
         ((ingress_table == IFP_POLICY_TABLE_PIPE0m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE1m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE2m)   ||
          (ingress_table == IFP_POLICY_TABLE_PIPE3m)   ||
          (ingress_table == AGM_MONITOR_TABLEm))) ||
#endif
         (ingress_table == VFP_POLICY_TABLEm)  ||
#if defined(BCM_TOMAHAWK_SUPPORT) 
        (SOC_IS_TOMAHAWKX(unit) &&
         ((ingress_table == VFP_POLICY_TABLE_PIPE0m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE1m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE2m)   ||
          (ingress_table == VFP_POLICY_TABLE_PIPE3m))) ||
#endif
         (ingress_table == MPLS_ENTRYm)  ||
         (ingress_table == SOURCE_VPm)  ||
         (ingress_table == L3_IIFm)  ||
         (ingress_table == VRFm)  ||
         (ingress_table == VFIm)  ||
         (ingress_table == VLAN_TABm))) {
          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Invalid Flex Counter Ingress Memory %s\n"),
                     SOC_MEM_UFNAME(unit, ingress_table)));
          return BCM_E_PARAM;
    }
#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
#endif
    ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                              SOC_MEM_INFO(unit, ingress_table).bytes));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Deallocating IngressCounter Table:%s:Index:%d:"
                           " ENTRY_BYTES:%d \n"),
               SOC_MEM_UFNAME(unit, ingress_table),
               index,ingress_entry_data_size));
    ingress_entry_data = sal_alloc(ingress_entry_data_size,"ingress_table");
    if (ingress_entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, ingress_table)));
        return BCM_E_INTERNAL;
    }
    sal_memset(ingress_entry_data,0,ingress_entry_data_size);
#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_th_ifp_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                 _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                 ingress_entry_data) == SOC_E_NONE) {
#else
    if (soc_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                 _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                 ingress_entry_data) == SOC_E_NONE) {
#endif /* BCM_TOMAHAWK_SUPPORT */
        if (soc_mem_field_valid(unit,ingress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data,
                                    VALIDf) == 0) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                sal_free(ingress_entry_data);
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                 unit,index,ingress_table,ingress_entry_data,0,
                 &offset_mode,&pool_number,&base_idx);
        if ((offset_mode == 0) && (base_idx == 0)) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Table:%s:Index %d IsNotConfiguredForFlexCounter\n"),
                        SOC_MEM_UFNAME(unit, ingress_table),index));
             sal_free(ingress_entry_data);
             return BCM_E_NOT_FOUND;/*Either NotConfigured/deallocated before*/
        }

        if(_bcm_esw_stat_flex_get_ingress_object(
           unit,ingress_table,index,ingress_entry_data,&object) != BCM_E_NONE) {
           sal_free(ingress_entry_data);
           return BCM_E_INTERNAL;
        }
#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
        (_bcm_esw_is_fp_table(ingress_table))) {
        uint32 cmode = 0;
        bcm_error_t rv;
 
        rv = _bcm_esw_custom_stat_group_id_retrieve(unit, offset_mode, pipe_num, 
                                                    pool_number, base_idx,
                                                    &stat_counter_id);
        if (rv !=BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "pipe %x Invalid mode value %d %d %d\n"),
                   pipe_num, offset_mode, pool_number, base_idx));
            _bcm_esw_stat_get_counter_id(
                          unit, flex_ingress_modes[unit][offset_mode].group_mode,
                          object,offset_mode,pool_number,base_idx,&stat_counter_id);
            total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
        } else {
            cmode = stat_counter_map[unit][stat_counter_id].mode;
            total_counters = flex_custom_ingress_modes[unit][cmode - BCM_CUSTOM_INGRESS_MODE_START].
                                            total_counters;
        }
    } else
#endif
    {
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][offset_mode].group_mode,
                  object,offset_mode,pool_number,base_idx,&stat_counter_id);
    total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
    }


        /* Decrement reference count */
        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                       [pool_number][base_idx]--;
        flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                       attached_entries -= total_counters;
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

        /* Clear Counter Values */
        if ((flex_base_index_reference_count[unit]
                [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]) == 0 ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Clearing Counter Tables %s contents:Offset:%d Len:%d\n"),
                       SOC_MEM_UFNAME(unit, ingress_table),
                       base_idx,
                       total_counters));
            flex_ctr_offset_info.all_counters_flag =  1;
            _bcm_esw_stat_flex_set(unit,index,ingress_table, -1,
                                   flex_ctr_offset_info,&flex_entries,
                                   offset_mode,pool_number,base_idx,
                                   bcmStatFlexDirectionIngress);
        }

        /* Reset Table Values */
        _bcm_esw_set_flex_counter_fields_values(
                 unit,index,ingress_table,ingress_entry_data,0,0,0,0);
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit) &&
            ((IFP_POLICY_TABLEm == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE0m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE1m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE2m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE3m == ingress_table))) {
            uint32 zero = 0;

            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            G_COUNTf, &zero);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            Y_COUNTf, &zero);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data,
                                            R_COUNTf, &zero);
        }
#endif
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (soc_th_ifp_mem_write(unit,ingress_table, MEM_BLOCK_ALL,
                          _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                          ingress_entry_data) != SOC_E_NONE) {
#else
        if (soc_mem_write(unit,ingress_table, MEM_BLOCK_ALL,
                          _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                          ingress_entry_data) != SOC_E_NONE) {
#endif /* BCM_TOMAHAWK_SUPPORT */
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table:%s:Index %d encounter some problem \n"),
                       SOC_MEM_UFNAME(unit, ingress_table),index));
            sal_free(ingress_entry_data);
            return    BCM_E_INTERNAL;
        }

       LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "Deallocated for Table:%sIndex:%d:"
                              "mode %d reference_count %d\n"),
                  SOC_MEM_UFNAME(unit, ingress_table),index,offset_mode,
                  flex_base_index_reference_count[unit]
                  [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]));
        sal_free(ingress_entry_data);

#if defined(BCM_TOMAHAWK_SUPPORT)
        if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
            /* In this mode, counters must be collected independently for each pipe. */
            maxpipe = NUM_PIPE(unit);
        }

#endif
        unused_pool = 1;
        for(pipe = 0; pipe < maxpipe; ++pipe) {
            if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe][pool_number].
                           attached_entries != 0) {
                unused_pool = 0;
                break;
            }
        }

        /* Disable flex pool for this stat on no flex entries */
        if (unused_pool) {
            _bcm_esw_stat_flex_enable_pool(
                     unit,
                     bcmStatFlexDirectionIngress,
                     _pool_ctr_register[bcmStatFlexDirectionIngress]
                                       [pool_number],
                     0);
        }
        if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                           [pipe_num][pool_number][base_idx] == 0) {
            if (_bcm_esw_stat_flex_insert_stat_id(unit,
                        local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
                LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "WARMBOOT: Couldnot add entry in scache memory."
                                      "Attach it\n")));
            }
        }
        return BCM_E_NONE;
    }
    sal_free(ingress_entry_data);
    return    BCM_E_NOT_FOUND;
}

#if defined (BCM_TOMAHAWK_SUPPORT)
/*
 * Function:
 *      _bcm_esw_stat_flex_detach_ingress_table_counters_sw
 * Description:
 *      Detach i.e. Disable Ingress accounting table's statistics.
 *      This function is used for Exact Match entries 
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_detach_ingress_table_counters_sw(
            int       unit,
            soc_mem_t ingress_table,
            uint32    index,
            bcm_stat_flex_mode_t offset_mode,
            uint32    base_idx,
            uint32    pool_number)
{
    uint32                          flex_entries = 0;
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info = {0};
    uint32                          stat_counter_id = 0;
    bcm_stat_object_t               object;
    int                             pipe_num = 0;
    bcm_stat_flex_custom_ingress_mode_t cmode;
    uint32 mode_id = 0;
    int                             pipe = 0, maxpipe = 1, unused_pool;
    int total_counters = 0;
    if (!_bcm_esw_is_exact_match(ingress_table)) {
          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "This function is for detaching SW data structures only: table %s\n"),
                     SOC_MEM_UFNAME(unit, ingress_table)));
          return BCM_E_PARAM;
    }

    if (offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) {
        if(_bcm_esw_stat_flex_get_custom_ingress_mode_info(unit, offset_mode, &cmode)
                                                                == BCM_E_NONE) {
            total_counters = flex_custom_ingress_modes[unit][offset_mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
            mode_id = cmode.offset_mode;
        }
    } else {
        total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
    }

    object = bcmStatObjectIngExactMatch;

    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
    if (((offset_mode == 0) && (base_idx == 0)) ||
          (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                   [pool_number][base_idx] <= 0)) {
         LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Table:%s:Index %d IsNotConfiguredForFlexCounter\n"),
                    SOC_MEM_UFNAME(unit, ingress_table),index));
         return BCM_E_NOT_FOUND;/*Either NotConfigured/deallocated before*/
    }

    /* Decrement reference count */
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                   [pool_number][base_idx]--;
    flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                   attached_entries -= total_counters;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    /* Clear Counter Values */
    if ((flex_base_index_reference_count[unit]
            [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]) == 0 ) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Clearing Counter Tables %s contents:Offset:%d Len:%d\n"),
                   SOC_MEM_UFNAME(unit, ingress_table),
                   base_idx,
                   total_counters));
        flex_ctr_offset_info.all_counters_flag =  1;
        _bcm_esw_stat_flex_set(unit,index,ingress_table, -1,
                               flex_ctr_offset_info,&flex_entries,
                               mode_id,pool_number,base_idx,
                               bcmStatFlexDirectionIngress);
    }

   LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
             (BSL_META_U(unit,
                         "Deallocated for Table:%sIndex:%d:"
                          "mode %d reference_count %d\n"),
              SOC_MEM_UFNAME(unit, ingress_table),index,offset_mode,
              flex_base_index_reference_count[unit]
              [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]));

    if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
        /* In this mode, counters must be collected independently for each pipe. */
        maxpipe = NUM_PIPE(unit);
    }

    unused_pool = 1;

    for(pipe = 0; pipe < maxpipe; ++pipe) {
        if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe][pool_number].
                       attached_entries != 0) {
            unused_pool = 0;
            break;
        }
    }
        
    /* Disable flex pool for this stat on no flex entries */
    if (unused_pool) {
        _bcm_esw_stat_flex_enable_pool(
                 unit,
                 bcmStatFlexDirectionIngress,
                 _pool_ctr_register[bcmStatFlexDirectionIngress]
                                   [pool_number],
                 0);
    }
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][offset_mode].group_mode,
                  object,offset_mode,pool_number,base_idx,&stat_counter_id);
    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number][base_idx] == 0) {
        if (_bcm_esw_stat_flex_insert_stat_id(unit,
                    local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
            LOG_VERBOSE(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                     "WARMBOOT: Couldnot add entry in scache memory."
                                      "Attach it\n")));
        }
    }
    return BCM_E_NONE;
}
#endif

/*
 * Function:
 *      _bcm_esw_stat_flex_destroy_ingress_table_counters
 * Description:
 *      Destroy Igresss accounting table's statistics completely
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      offset_mode           - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_destroy_ingress_table_counters(
            int                  unit,
            soc_mem_t            ingress_table,
            int                  lookup,
            bcm_stat_object_t    object,
            bcm_stat_flex_mode_t offset_mode,
            uint32               base_idx,
            uint32               pool_number)
{
    uint32 free_count = 0, alloc_count = 0, largest_free = 0;
    uint32 used_by_table = 0, stat_counter_id = 0;
    uint32 pool_group_id;
    int pipe_num = 0;    
    uint32 orig_mode = offset_mode;
    int total_counters = 0;

#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
    if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START)  &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][offset_mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
        offset_mode = flex_custom_ingress_modes[unit][offset_mode - 
                                 BCM_CUSTOM_INGRESS_MODE_START].offset_mode;
    } else {
#endif
        total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
#if defined(BCM_TOMAHAWK_SUPPORT)
    }
#endif

    if (flex_ingress_modes[unit][offset_mode].available==0) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex counter mode %d not configured yet\n"),
                   offset_mode));
        return BCM_E_NOT_FOUND;
    }

    if (shr_aidxres_list_elem_state(
            flex_aidxres_list_handle[unit][bcmStatFlexDirectionIngress]
            [pipe_num][pool_number],
            base_idx) != BCM_E_EXISTS) {
        if (BCM_SUCCESS(_field_is_inited(unit))) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit, "Wrong base index %u \n"),
                        base_idx));
        }
        return BCM_E_NOT_FOUND;
    }
    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number][base_idx] != 0) {
        if (BCM_SUCCESS(_field_is_inited(unit))) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Reference count is  %d.. Please detach entries first..\n"),
                   flex_base_index_reference_count[unit]
                        [bcmStatFlexDirectionIngress][pipe_num][pool_number][base_idx]));
        }
        return BCM_E_BUSY;
    }
    switch(ingress_table) {
    case PORT_TABm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_PORT_TABLE;
         break;
    case VFP_POLICY_TABLEm:
#if defined(BCM_TOMAHAWK_SUPPORT)
        case VFP_POLICY_TABLE_PIPE0m:
        case VFP_POLICY_TABLE_PIPE1m:
        case VFP_POLICY_TABLE_PIPE2m:
        case VFP_POLICY_TABLE_PIPE3m:
#endif
         used_by_table = FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE;
         break;
         /* VLAN and VFI shares same pool */
    case VLAN_TABm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE;
         break;
    case VFIm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_VFI_TABLE;
         break;
         /* VRF and MPLS_VC_LABEL shares same pool */
    case VRFm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_VRF_TABLE;
         break;
    case MPLS_ENTRYm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case MPLS_ENTRY_EXTDm:
#endif
         if  (lookup == 1) {
             used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE;
         } else if (lookup == 2) {
#if defined(BCM_APACHE_SUPPORT)
             used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP;
#endif
         }
         break;
    case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
    case VLAN_XLATE_EXTDm:
#endif
         if  (lookup == 1) {
             used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE;
         }
         else if (lookup == 2) {
             used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP;
         }
         break;
         /* L3_IIF and SOURCE_VP shares same pool*/
    case L3_IIFm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE;
         break;
    case SOURCE_VPm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE;
         break;
#if defined(BCM_TRIUMPH3_SUPPORT)
    case L3_TUNNELm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE;
         break;
    case L3_ENTRY_2m:
    case L3_ENTRY_4m:
    case EXT_IPV4_UCAST_WIDEm:
    case EXT_IPV6_128_UCAST_WIDEm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE;
         break;
    case EXT_FP_POLICYm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_EXT_FP_POLICY_TABLE;
         break;
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    case ING_VSANm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE;
         break;
    case L3_ENTRY_IPV4_MULTICASTm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE;
         break;
    case L3_DEFIPm:
    case L3_DEFIP_PAIR_128m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE;
         break;
#endif
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
#if defined(BCM_TOMAHAWK_SUPPORT)
    case IFP_POLICY_TABLE_PIPE0m:
    case IFP_POLICY_TABLE_PIPE1m:
    case IFP_POLICY_TABLE_PIPE2m:
    case IFP_POLICY_TABLE_PIPE3m:
         /* Check to see if counter allocation is being requested
          * from a specific pool
          */

         /* Get Pipe from ingress_table */
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
         /* Fall through */
#endif
    case IFP_POLICY_TABLEm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
         break;
#if defined(BCM_TOMAHAWK_SUPPORT)
    case EXACT_MATCH_2m:
    case EXACT_MATCH_2_PIPE0m:
    case EXACT_MATCH_2_PIPE1m:
    case EXACT_MATCH_2_PIPE2m:
    case EXACT_MATCH_2_PIPE3m:
         /* Get Pipe from ingress_table */
         pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
         used_by_table = FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE;
         break;
    case AGM_MONITOR_TABLEm:
         used_by_table = FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE;
         break;
#endif
#endif
    default:
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid Table is passed %d \n"),
                    ingress_table));
         return BCM_E_INTERNAL;
    }
    if (shr_aidxres_list_free(flex_aidxres_list_handle
                              [unit][bcmStatFlexDirectionIngress][pipe_num][pool_number],
                              base_idx) != BCM_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Freeing memory Table:%s:encounter some problem due to entry not found or some other issue  \n"),
                   SOC_MEM_UFNAME(unit, ingress_table)));
        return    BCM_E_NOT_FOUND;
    }
#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
       (orig_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (orig_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        bcm_error_t rv = BCM_E_CONFIG;
 
        rv = _bcm_esw_custom_stat_group_id_retrieve(unit, orig_mode, pipe_num, 
                                  pool_number, base_idx, &stat_counter_id);
        if (rv !=BCM_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid mode value %d %d %d\n"),
                   orig_mode, pool_number, base_idx));
        }
    } else
#endif
    {  
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][offset_mode].group_mode,
                  object,orig_mode,pool_number,base_idx,&stat_counter_id);
    }

    if (_bcm_esw_stat_flex_delete_stat_id(unit,
                 local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                 (BSL_META_U(unit,
                             "WARMBOOT: Couldnot Delete entry in scache memory.\n")));
    }
    shr_aidxres_list_state(flex_aidxres_list_handle
                           [unit][bcmStatFlexDirectionIngress][pipe_num][pool_number],
                           NULL,NULL,NULL,NULL,
                           &free_count,&alloc_count,&largest_free,NULL);
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Current Pool status free_count:%d alloc_count:%d"
                           "largest_free:%d used_by_tables:%d used_entries:%d\n"),
               free_count,alloc_count,largest_free,
               flex_pool_stat[unit][bcmStatFlexDirectionIngress]
               [pipe_num][pool_number].used_by_tables,
               flex_pool_stat[unit][bcmStatFlexDirectionIngress]
               [pipe_num][pool_number].used_entries));
    flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                  [pool_number].used_entries -= total_counters;
    if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
        used_entries == 0) {
#if defined(BCM_TOMAHAWK_SUPPORT)
        /* If this is a reserved pool then do not update
         * used_by_tables info
         */
        if (!(SOC_IS_TOMAHAWKX(unit) &&
            (pool_number >= BCM_FLEX_STAT_TH_RSVD_IFP_POOL_MIN)))
#endif
        {
            flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [pipe_num][pool_number].used_by_tables &= ~used_by_table;
            SHR_BITCLR(flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                       [pipe_num][pool_number].used_by_objects, object);
            flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [0][pool_number].used_by_tables &= ~used_by_table;
            SHR_BITCLR(flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                       [0][pool_number].used_by_objects, object);
            /* Decrement the free pool from the pool group database
             * and if all the group pools are empty then remove the config from
             * ING_FLEX_CTR_LM_CONFIG register.
             * Clear the Group_type.
             */
            if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
                pool_group_id = pool_number/_bcm_esw_get_max_pool_groups(unit);
                flex_pool_group[unit][pipe_num][pool_group_id].free_pools++; 
                if(flex_pool_group[unit][pipe_num][pool_group_id].free_pools
                            ==  BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP) {
                    BCM_IF_ERROR_RETURN(
                           _bcm_esw_stat_flex_oam_enable_ingress_group(
                                                              unit,
                                                              pool_group_id,
                                                              0));
                    flex_pool_group[unit][pipe_num][pool_group_id].group_type =
                                               BCM_FLEX_POOL_GROUP_USED_BY_NONE;
                }
            }
        }
    }
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_ingress_modes[unit][offset_mode].reference_count--;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return    BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_attach_egress_table_counters1
 * Description:
 *      Atach i.e. Enable Egresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_egress_table_counters1(
            int                  unit,
            soc_mem_t            egress_table,
            uint32               index,
            bcm_stat_flex_mode_t mode,
            uint32               base_idx,
            uint32               pool_number,
            void                *egress_entry_data1)
{
    uint32               egress_entry_data_size=0;
    void                 *egress_entry_data=NULL;
    void                 *egress_entry_data_temp=NULL;
    bcm_stat_flex_mode_t offset_mode_l={0};
    bcm_stat_object_t    object=bcmStatObjectEgrPort;
    uint32               stat_counter_id=0;
    uint32               base_idx_l=0;
    uint32               pool_number_l=0;

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_egress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex CounterMode:%d:Not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }
    if (shr_aidxres_list_elem_state(flex_aidxres_list_handle
                                    [unit][bcmStatFlexDirectionEgress]
                                    [0][pool_number],base_idx) != BCM_E_EXISTS) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong base index %u \n"),
                   base_idx));
        return BCM_E_NOT_FOUND;
    }
    egress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                             SOC_MEM_INFO(unit, egress_table).bytes));
    if (egress_entry_data1  == NULL ) {
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          ".Allocating EgressCounter Table:%s:Index:%d:Mode:%d"
                           " ENTRY_BYTES %d\n"),
               SOC_MEM_UFNAME(unit,egress_table),index,mode,
               egress_entry_data_size));
        egress_entry_data = sal_alloc(egress_entry_data_size,"egress_table");
        if (egress_entry_data == NULL) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Failed to allocate memory for Table:%s "),
                     SOC_MEM_UFNAME(unit, egress_table)));
            return BCM_E_INTERNAL;
        }
        sal_memset(egress_entry_data,0,SOC_MEM_INFO(unit, egress_table).bytes);

        if (soc_mem_read(unit, egress_table, MEM_BLOCK_ANY,
                    _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                    egress_entry_data) != SOC_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Read failure for Table %s with index %d \n"),
                     SOC_MEM_UFNAME(unit, egress_table),index));
            sal_free(egress_entry_data);
            return BCM_E_INTERNAL;
        }
        egress_entry_data_temp = egress_entry_data;
    } else {
        egress_entry_data_temp = egress_entry_data1;
    }
    if (soc_mem_field_valid(unit,egress_table,VALIDf)) {
        if (soc_mem_field32_get(unit,egress_table,egress_entry_data_temp,
                                VALIDf)==0) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table %s  with index %d is Not valid \n"),
                       SOC_MEM_UFNAME(unit, egress_table),index));
            if (egress_entry_data1 == NULL) {
                sal_free(egress_entry_data);
            }
            return BCM_E_PARAM;
        }
    }
    _bcm_esw_get_flex_counter_fields_values(
                 unit,index,egress_table,egress_entry_data_temp,0,
                 &offset_mode_l,&pool_number_l,&base_idx_l);
    if (base_idx_l != 0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Table:%s HasAlreadyAllocatedWithIndex:%d base %d mode %d."
                               "First dealloc it \n"),SOC_MEM_UFNAME(unit, egress_table),
                   index,base_idx_l,offset_mode_l));
        if (egress_entry_data1 == NULL) {
            sal_free(egress_entry_data);
        }
        return BCM_E_EXISTS;/*Either Not configured or deallocated before*/
    }
    _bcm_esw_set_flex_counter_fields_values(
             unit,index,egress_table,egress_entry_data_temp,0,
             mode,pool_number,base_idx);

    if (egress_entry_data1 == NULL) {
        if (soc_mem_write(unit,egress_table, MEM_BLOCK_ALL,
                    _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                    egress_entry_data_temp) != SOC_E_NONE) {
            if (egress_entry_data1 == NULL) {
                sal_free(egress_entry_data);
            }
            return BCM_E_INTERNAL;
        }
    }

    if(_bcm_esw_stat_flex_get_egress_object(unit,egress_table,
       index,egress_entry_data_temp,&object) != BCM_E_NONE) {
        if (egress_entry_data1 == NULL) {
            sal_free(egress_entry_data);
        }
       return BCM_E_INTERNAL;
    }

    if (egress_entry_data1 == NULL) {
        sal_free(egress_entry_data);
    }

    _bcm_esw_stat_get_counter_id(unit,
                  flex_egress_modes[unit][mode].group_mode,
                  object,mode,pool_number,base_idx,&stat_counter_id);

    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress][0]
                                       [pool_number][base_idx] == 0) {
        if (_bcm_esw_stat_flex_delete_stat_id(unit,local_scache_ptr[unit],
                                              stat_counter_id) != BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                        (BSL_META_U(unit,
                                    "WARMBOOT: Could not delete entry in scache memory.\n")));
        }
    }

    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress][0]
                                   [pool_number][base_idx]++;
    if (flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
         attached_entries == 0) {

        _bcm_esw_stat_flex_enable_pool(
                 unit,bcmStatFlexDirectionEgress,
                 _pool_ctr_register[bcmStatFlexDirectionEgress][pool_number],1);
    }
    flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                       attached_entries += flex_egress_modes[unit]
                                           [mode].total_counters;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_attach_egress_table_counters
 * Description:
 *      Atach i.e. Enable Egresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_egress_table_counters(
            int                  unit,
            soc_mem_t            egress_table,
            uint32               index,
            bcm_stat_object_t    object_id,
            bcm_stat_flex_mode_t mode,
            uint32               base_idx,
            uint32               pool_number)
{
    uint32               egress_entry_data_size=0;
    void                 *egress_entry_data=NULL;
    bcm_stat_flex_mode_t offset_mode_l={0};
    bcm_stat_object_t    object=bcmStatObjectEgrPort;
    uint32               stat_counter_id=0;
    uint32               base_idx_l=0;
    uint32               pool_number_l=0;
    uint32               exact_index=0;

    exact_index = index;
    if (egress_table == EGR_IP_TUNNEL_MPLSm) {
        index = index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
    }
    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_egress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex CounterMode:%d:Not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }
    if (shr_aidxres_list_elem_state(flex_aidxres_list_handle
                                    [unit][bcmStatFlexDirectionEgress]
                                    [0][pool_number],base_idx) != BCM_E_EXISTS) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong base index %u \n"),
                   base_idx));
        return BCM_E_NOT_FOUND;
    }
    egress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                             SOC_MEM_INFO(unit, egress_table).bytes));
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          ".Allocating EgressCounter Table:%s:Index:%d:Mode:%d"
                           " ENTRY_BYTES %d\n"),
               SOC_MEM_UFNAME(unit,egress_table),index,mode,
               egress_entry_data_size));
    egress_entry_data = sal_alloc(egress_entry_data_size,"egress_table");
    if (egress_entry_data == NULL) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Failed to allocate memory for Table:%s "),
                   SOC_MEM_UFNAME(unit, egress_table)));
        return BCM_E_INTERNAL;
    }
    sal_memset(egress_entry_data,0,SOC_MEM_INFO(unit, egress_table).bytes);

    if (soc_mem_read(unit, egress_table, MEM_BLOCK_ANY,
                _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                egress_entry_data) != SOC_E_NONE) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Read failure for Table %s with index %d \n"),
                   SOC_MEM_UFNAME(unit, egress_table),index));
        sal_free(egress_entry_data);
        return BCM_E_INTERNAL;
    }
    if (soc_mem_field_valid(unit,egress_table,VALIDf)) {
        if (soc_mem_field32_get(unit,egress_table,egress_entry_data,
                                VALIDf)==0) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table %s  with index %d is Not valid \n"),
                       SOC_MEM_UFNAME(unit, egress_table),index));
            sal_free(egress_entry_data);
            return BCM_E_PARAM;
        }
    }
    _bcm_esw_get_flex_counter_fields_values(
                 unit,exact_index,egress_table,
                 egress_entry_data,object_id,&offset_mode_l,
                 &pool_number_l,&base_idx_l);
    if (base_idx_l != 0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Table:%s HasAlreadyAllocatedWithIndex:%d base %d mode %d."
                               "First dealloc it \n"),SOC_MEM_UFNAME(unit, egress_table),
                   index,base_idx_l,offset_mode_l));
        sal_free(egress_entry_data);
        return BCM_E_EXISTS;/*Either Not configured or deallocated before*/
    }
    _bcm_esw_set_flex_counter_fields_values(
             unit,exact_index,egress_table,egress_entry_data,object_id,
             mode,pool_number,base_idx);
    if (soc_mem_write(unit,egress_table, MEM_BLOCK_ALL,
                       _bcm_esw_stat_flex_table_index_map(unit,egress_table,index),
                       egress_entry_data) != SOC_E_NONE) {
        sal_free(egress_entry_data);
        return BCM_E_INTERNAL;
    }
    if (object_id != bcmStatObjectEgrMplsTunnelSecondLabel) {
        if(_bcm_esw_stat_flex_get_egress_object(unit,egress_table,
                    index,egress_entry_data,&object) != BCM_E_NONE) {
            sal_free(egress_entry_data);
            return BCM_E_INTERNAL;
        }
    } else {
        object = object_id;
    }
    sal_free(egress_entry_data);
    _bcm_esw_stat_get_counter_id(
                  unit, flex_egress_modes[unit][mode].group_mode,
                  object,mode,pool_number,base_idx,&stat_counter_id);
    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress][0]
                                       [pool_number][base_idx] == 0) {
        if (_bcm_esw_stat_flex_delete_stat_id(unit,local_scache_ptr[unit],
                                              stat_counter_id) != BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "WARMBOOT:Couldnot Delete entry in scache memory\n")));
        }
    }
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_base_index_reference_count[unit][bcmStatFlexDirectionEgress][0]
                                   [pool_number][base_idx]++;
    if (flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
         attached_entries == 0) {

        _bcm_esw_stat_flex_enable_pool(
                 unit,bcmStatFlexDirectionEgress,
                 _pool_ctr_register[bcmStatFlexDirectionEgress][pool_number],1);
    }
    flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                       attached_entries += flex_egress_modes[unit]
                                           [mode].total_counters;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_pool_operation
 * Description:
 *      Allocated or Deallocate Pool Counters
 * Parameters:
 *      unit                  - (IN) unit number
 *      flex_pool_attributes  - (IN) Flex Pool Attributes
 *           module_type           - (IN) Flex Module Type (Not Used Currently)
 *           pool_size             - (IN) Flex Pool Size
 *           flags                 - (IN) Combination of below flags
 *                                        1)Either
 *                                               BCM_FLEX_INGRESS_POOL(default)
 *                                                    OR
 *                                               BCM_FLEX_EGRESS_POOL
 *                                        2)Either
 *                                               BCM_FLEX_ALLOCATE_POOL(default)
 *                                                    OR
 *                                               BCM_FLEX_DEALLOCATE_POOL
 *                                        3)Either
 *                                               BCM_FLEX_SHARED_POOL(default)
 *                                                    OR
 *                                               BCM_FLEX_NOT_SHARED_POOL
 *                                          (NotApplicable in deallocation case)
 *           pool_id               - (IN/OUT) Flex Pool ID
 *           offset                - (IN/OUT) Flex Pool Base Index
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t  _bcm_esw_stat_flex_pool_operation(
              int                            unit,
              bcm_stat_flex_pool_attribute_t *flex_pool_attribute)
{
    bcm_stat_flex_direction_t direction = bcmStatFlexDirectionIngress;
    uint32                    num_flex_pools=SOC_INFO(unit).
                                             num_flex_ingress_pools;
    uint32                    size_pool= SOC_INFO(unit).size_flex_ingress_pool;
    uint32                    block_factor=15;
    uint32                    pool_number_l=0;
    uint32                    default_pool_number=0;
    uint32                    used_by_table_selection_criteria=
                              TABLE_INDEPENDENT_POOL_MASK;
    uint32                    shared_pool=1;
    uint32                    base_idx_l=0;
    uint32                    alloc_count=0;

    if (flex_pool_attribute == NULL) {
        return BCM_E_PARAM;
    }
    if ((flex_pool_attribute->flags & BCM_FLEX_EGRESS_POOL) ==
         BCM_FLEX_EGRESS_POOL) {
         direction = bcmStatFlexDirectionEgress;
         num_flex_pools=SOC_INFO(unit).num_flex_egress_pools;
         size_pool= SOC_INFO(unit).size_flex_egress_pool;
    }
    if ((flex_pool_attribute->flags & BCM_FLEX_NOT_SHARED_POOL) ==
         BCM_FLEX_NOT_SHARED_POOL) {
         shared_pool=0;
    }
    if (flex_pool_attribute->pool_size > size_pool) {
        return BCM_E_PARAM;
    }
    if ((flex_pool_attribute->flags & BCM_FLEX_DEALLOCATE_POOL) ==
         BCM_FLEX_DEALLOCATE_POOL) {
         pool_number_l =  (flex_pool_attribute->pool_id);
         if (pool_number_l > num_flex_pools) {
             return BCM_E_PARAM;
         }
         if (flex_pool_stat[unit][direction][0][pool_number_l].
             used_by_tables != TABLE_INDEPENDENT_POOL_MASK) {
             return BCM_E_PARAM;
         }
         BCM_STAT_FLEX_COUNTER_LOCK(unit);
         if (shr_aidxres_list_free(
             flex_aidxres_list_handle[unit][direction][0][pool_number_l],
             flex_pool_attribute->offset) != BCM_E_NONE) {
             BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
             return BCM_E_PARAM;
        }
        flex_pool_stat[unit][direction][0][pool_number_l].
                      used_entries -= flex_pool_attribute->pool_size;
        shr_aidxres_list_state(flex_aidxres_list_handle
                               [unit][direction][0][pool_number_l],
                               NULL,NULL,NULL,NULL,
                               NULL,&alloc_count,NULL,NULL);
        if ((flex_pool_stat[unit][direction][0][pool_number_l].
             used_entries == 0) ||
             (alloc_count == 0)) {
             _bcm_esw_stat_flex_enable_pool(
                  unit,
                  direction,
                  _pool_ctr_register[direction][pool_number_l],0);
             flex_pool_stat[unit][direction][0][pool_number_l].used_entries=0;
             flex_pool_stat[unit][direction][0][pool_number_l].used_by_tables = 0;
             SHR_BITCLR_RANGE(flex_pool_stat[unit][direction][0][pool_number_l].
                                used_by_objects, 0, bcmIntStatObjectMaxValue);
             /* Destroy Existing One */
             shr_aidxres_list_destroy(flex_aidxres_list_handle[unit]
                                      [direction][0][pool_number_l]);
             /* Re Create it back to original one */
             if (shr_aidxres_list_create(&flex_aidxres_list_handle
                 [unit][direction][0][pool_number_l],
                 0,size_pool-1,
                 0,size_pool-1,
                 8, /* Max 256 counters */
                 "flex-counter") != BCM_E_NONE) {
                 LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                           (BSL_META_U(unit,
                                       "Unrecoverable error. Couldn'tCreate "
                                        "AllignedList:FlexCounter\n")));
                 BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                 return BCM_E_INTERNAL;
             }
             shr_aidxres_list_reserve_block(
                 flex_aidxres_list_handle[unit][direction][0][pool_number_l],
                 0, (1 << 1));
        }
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
    } else {
        BCM_STAT_FLEX_COUNTER_LOCK(unit);
        pool_number_l =  (flex_pool_attribute->pool_id);
        do {
           if ((flex_pool_stat[unit][direction][0][pool_number_l].
                used_by_tables == 0) ||
                ((shared_pool == 1) &&
                 (flex_pool_stat[unit][direction][0][pool_number_l].
                  used_by_tables & used_by_table_selection_criteria))) {

                if ((flex_pool_stat[unit][0][direction][pool_number_l].
                     used_by_tables == 0)) {
                     /* Destroy Existing One */
                     shr_aidxres_list_destroy(flex_aidxres_list_handle[unit]
                                              [direction][0][pool_number_l]);
                     /* Re Create it */
                     if (shr_aidxres_list_create(&flex_aidxres_list_handle
                         [unit][direction][0][pool_number_l],
                         0,size_pool-1,
                         0,size_pool-1,
                         block_factor,
                         "flex-counter") != BCM_E_NONE) {
                         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                   (BSL_META_U(unit,
                                               "Unrecoverable error. Couldn'tCreate "
                                                "AllignedList:FlexCounter\n")));
                         BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                         return BCM_E_INTERNAL;
                   }
                }
                shr_aidxres_list_state(flex_aidxres_list_handle
                                       [unit][direction][0][pool_number_l],
                                       NULL,NULL,NULL,NULL,
                                       NULL,&alloc_count,NULL,NULL);
                if (shr_aidxres_list_alloc_block(
                    flex_aidxres_list_handle[unit][direction][0][pool_number_l],
                    flex_pool_attribute->pool_size,
                    &base_idx_l) == BCM_E_NONE) {
                    if ((flex_pool_stat[unit][direction][0][pool_number_l].
                         used_entries == 0) ||
                         (alloc_count == 0)) {
                         _bcm_esw_stat_flex_enable_pool(
                              unit,
                              direction,
                              _pool_ctr_register[direction][pool_number_l],1);
                    }
                    flex_pool_stat[unit][direction][0][pool_number_l].
                         used_by_tables = TABLE_INDEPENDENT_POOL_MASK;
                    flex_pool_stat[unit][direction][0][pool_number_l].
                         used_entries += flex_pool_attribute->pool_size;
                    flex_pool_attribute->offset  = (int)base_idx_l;
                    flex_pool_attribute->pool_id = (int)pool_number_l;
                    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
                    return BCM_E_NONE;
                }
            } else {
               BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
               return BCM_E_RESOURCE;
            }
            pool_number_l = (pool_number_l+1) % num_flex_pools;
        } while(pool_number_l != default_pool_number);
        BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
        return BCM_E_RESOURCE;
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_create_egress_table_counters
 * Description:
 *      Create and Reserve Flex Counter Space for Egresss accounting table
 * Parameters:
 *      unit                  - (IN) unit number
 *      egress_table          - (IN) Flex Accounting Table
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_create_egress_table_counters(
            int                  unit,
            soc_mem_t            egress_table,
            int                  lookup,
            bcm_stat_object_t    object,
            bcm_stat_flex_mode_t mode,
            uint32               *base_idx,
            uint32               *pool_number)
{
    uint32            base_idx_l=0;
    uint32            pool_number_l=0;
    uint32            default_pool_number=0;
    uint32            used_by_table=0;
    uint32            used_by_table_selection_criteria=0;
    uint32            free_count=0;
    uint32            alloc_count=0;
    uint32            largest_free=0;
    uint32            stat_counter_id=0;
    uint32            num_flex_egress_pools = SOC_INFO(unit).
                                              num_flex_egress_pools;
    flex_object_config_t object_config = {0};
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    /* Used to indicate if counter allocation for EFP
     * should be carried out from a specific pool
     */
    uint32            allocate_from_specific_pool = 0;
#endif

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_egress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex CounterMode:%d:Not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }
    /* Below case statement can be avoided by passing arguements for pool_number
       and selection criteria But keeping it for better understanding
       at same place */
    switch(egress_table) {
    case EGR_VLANm:
         default_pool_number = pool_number_l =
             FLEX_COUNTER_DEFAULT_EGR_VLAN_TABLE_POOL_NUMBER;
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_TABLE;
         break;
    case EGR_VFIm:
         default_pool_number = pool_number_l =
             FLEX_COUNTER_DEFAULT_EGR_VFI_TABLE_POOL_NUMBER;
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_VFI_TABLE;
         break;
    case EGR_L3_NEXT_HOPm:
         default_pool_number = pool_number_l =
             FLEX_COUNTER_DEFAULT_EGR_L3_NEXT_HOP_TABLE_POOL_NUMBER;
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE;
         break;
    case EGR_VLAN_XLATEm:
#if defined(BCM_TRIDENT2_SUPPORT)
         if (SOC_IS_TD2_TT2(unit)) {
             if (lookup == 1) {
                 default_pool_number = pool_number_l =
                     FLEX_COUNTER_TD2_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER;
                 used_by_table_selection_criteria = used_by_table =
                     FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE;
             } else if (lookup == 2) {
#if defined(BCM_APACHE_SUPPORT)
                 if (soc_feature(unit, soc_feature_egr_vlan_xlate_second_lookup)) {
                     default_pool_number = pool_number_l =
                         FLEX_COUNTER_DEFAULT_EGR_VLAN_XLATE_SCND_LKUP_POOL_NUMBER;
                     used_by_table_selection_criteria = used_by_table =
                         FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE_FOR_SCND_LKUP;
                 }
#endif
             }
         } else
#endif
         {
             default_pool_number = pool_number_l =
                 FLEX_COUNTER_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER;
             used_by_table_selection_criteria = used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE;
#if defined(BCM_METROLITE_SUPPORT)
             if (SOC_IS_METROLITE(unit)) {
                 default_pool_number = pool_number_l =
                     FLEX_COUNTER_TD2_DEFAULT_EGR_VLAN_XLATE_TABLE_POOL_NUMBER;
             }
#endif
         }
         break;
   case EGR_PORTm:
         default_pool_number = pool_number_l =
             FLEX_COUNTER_DEFAULT_EGR_PORT_TABLE_POOL_NUMBER;
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_PORT_TABLE;
         break;
   case EGR_DVP_ATTRIBUTE_1m:
         default_pool_number = pool_number_l =
           FLEX_COUNTER_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_METROLITE_SUPPORT)
         if (SOC_IS_TD2_TT2(unit) || SOC_IS_METROLITE(unit)) {
             default_pool_number = pool_number_l =
                 FLEX_COUNTER_TD2_DEFAULT_EGR_DVP_ATTRIBUTE_1_TABLE_POOL_NUMBER;
         }
#endif
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE;
         break;
   case EGR_NAT_PACKET_EDIT_INFOm:
         default_pool_number = pool_number_l =
           FLEX_COUNTER_DEFAULT_L3_NAT_TABLE_POOL_NUMBER;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_METROLITE_SUPPORT)
         if (SOC_IS_TD2_TT2(unit) || SOC_IS_METROLITE(unit)) {
            default_pool_number = pool_number_l =
                FLEX_COUNTER_TD2_DEFAULT_L3_NAT_TABLE_POOL_NUMBER;
         }
#endif
         used_by_table_selection_criteria = used_by_table =
             FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE;
         break;
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
   case EFP_POLICY_TABLEm:
         if (SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) {
             if (-1 != *pool_number) {
                 if (*pool_number < num_flex_egress_pools) {
                     if (FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE !=
                         flex_pool_stat[unit][bcmStatFlexDirectionEgress]
                                       [0][*pool_number].used_by_tables) {
                         /* Pool from which counter allocation is being
                          * requested is not used by EFP_POLICY_TABLE
                          */
                         return BCM_E_PARAM;
                     }
                     allocate_from_specific_pool = 1;
                     default_pool_number = pool_number_l = *pool_number;
                 } else {
                     /* Invalid pool_number specified */
                     return BCM_E_PARAM;
                 }
             } else {
                 default_pool_number = pool_number_l =
                     FLEX_COUNTER_DEFAULT_EGR_EFP_POLICY_TABLE_POOL_NUMBER;
             }
             used_by_table_selection_criteria = used_by_table =
                 FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE;
         } else {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit, "Invalid Table is passed %d \n"),
                        egress_table));
             return BCM_E_PARAM;
         }
         break;
   case EGR_IP_TUNNEL_MPLSm:
         if (SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_APACHE(unit)) {
         if (lookup == 1) {
            default_pool_number = pool_number_l =
                 FLEX_COUNTER_DEFAULT_EGR_IP_TUNNEL_MPLS_POOL_NUMBER;
            used_by_table_selection_criteria = used_by_table =
                FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_TABLE;
         } else if (lookup == 2) {
              default_pool_number = pool_number_l =
              FLEX_COUNTER_DEFAULT_EGR_IP_TUNNEL_MPLS_SCND_LBL_POOL_NUMBER;
              used_by_table_selection_criteria = used_by_table =
              FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_SCND_LBL_TABLE;
         }
         } else {
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit, "Invalid Table is passed %d \n"),
                        egress_table));
             return BCM_E_PARAM;
         }
         break;
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
   default:
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid Table is passed %d \n"),
                    egress_table));
         return BCM_E_INTERNAL;
   }
    
   BCM_IF_ERROR_RETURN(
       _bcm_esw_stat_flex_object_config_get(unit,
                                            egress_table, &object_config));

   if (object_config.share_criteria != INVALID_CONFIG) {
       used_by_table_selection_criteria |= object_config.share_criteria;
       default_pool_number=pool_number_l = object_config.config_pool;
   }
   if (object_config.exclude_criteria != INVALID_CONFIG) {
       used_by_table_selection_criteria &= ~(object_config.exclude_criteria);
   }
    
   do {
      /* Either free or being used by port table only */
      if ((flex_pool_stat[unit][bcmStatFlexDirectionEgress]
           [0][pool_number_l].used_by_tables == 0) ||
          (flex_pool_stat[unit][bcmStatFlexDirectionEgress]
           [0][pool_number_l].used_by_tables & used_by_table_selection_criteria)) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
            if ((SOC_IS_TD2P_TT2P(unit)) &&
                !allocate_from_specific_pool &&
                (bcmStatObjectEgrFieldStageEgress == object)) {
                if (0 != flex_pool_stat[unit][bcmStatFlexDirectionEgress]
                                       [0][pool_number_l].used_entries) {
                    /* For a non specific pool allocation
                     * request, allocate from an unused pool only.
                     */
                    pool_number_l = (pool_number_l + 1) %
                                    num_flex_egress_pools;
                    if (pool_number_l == default_pool_number) {
                        break;
                    } else {
                        continue;
                    }
                }
            }
#endif
           if (shr_aidxres_list_alloc_block(flex_aidxres_list_handle
                   [unit][bcmStatFlexDirectionEgress][0][pool_number_l],
                   flex_egress_modes[unit][mode].total_counters,
                   &base_idx_l) == BCM_E_NONE) {
               flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                    [pool_number_l].used_by_tables |= used_by_table;
               SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                    [pool_number_l].used_by_objects, object);
               flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                    [pool_number_l].used_entries += flex_egress_modes
                                                    [unit][mode].total_counters;
               LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Allocated  counter Table:%s with pool_number:%d"
                                      "mode:%d base_idx:%d ref_count %d\n"),
                          SOC_MEM_UFNAME(unit, egress_table),
                          pool_number_l,mode,base_idx_l,
                          flex_egress_modes[unit][mode].reference_count));
               shr_aidxres_list_state(
                   flex_aidxres_list_handle[unit][bcmStatFlexDirectionEgress][0]
                                           [pool_number_l],
                   NULL,NULL,NULL,NULL,&free_count,&alloc_count,&largest_free,
                   NULL);
               LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Current Pool status free_count:%d alloc_count:%d"
                                      "largest_free:%d used_by_tables:%d"
                                      "used_entries:%d\n"),
                          free_count,alloc_count,largest_free,
                          flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                          [pool_number_l].used_by_tables,
                          flex_pool_stat[unit][bcmStatFlexDirectionEgress][0]
                          [pool_number_l].used_entries));
               *base_idx = base_idx_l;
               *pool_number = pool_number_l;
               _bcm_esw_stat_get_counter_id(
                             unit, flex_egress_modes[unit][mode].group_mode,
                             object,mode,pool_number_l,base_idx_l,
                             &stat_counter_id);
               BCM_STAT_FLEX_COUNTER_LOCK(unit);
               flex_egress_modes[unit][mode].reference_count++;
               BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
               if (_bcm_esw_stat_flex_insert_stat_id(unit,
                   local_scache_ptr[unit], stat_counter_id) != BCM_E_NONE) {
                   LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                            (BSL_META_U(unit,
                                        "WARMBOOT:Couldnot add entry in scache memory"
                                         "Attach it\n")));
               }
               return BCM_E_NONE;
           }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
           else {
               if ((SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) &&
                   allocate_from_specific_pool &&
                   (bcmStatObjectEgrFieldStageEgress == object)) {
                   return BCM_E_RESOURCE;
               }
           }
#endif
      }
      pool_number_l = (pool_number_l+1) % num_flex_egress_pools;
    } while(pool_number_l != default_pool_number);
    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Pools exhausted for Table:%s\n"),
               SOC_MEM_UFNAME(unit, egress_table)));
    return BCM_E_RESOURCE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_attach_ingress_table_counters_update
 * Description:
 *      Attach i.e. Update Ingresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      mode                  - (IN) Flex offset mode for Accounting Object
 * Return Value:
 *      BCM_E_NONE
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_ingress_table_counters_update(
            int                  unit,
            uint32               pool_number,
            uint32               pipe_num,
            uint32               base_idx,
            bcm_stat_flex_mode_t mode)
{
    int    pipe = 0, maxpipe = 1, unused_pool = 1;
    int    total_counters = 0;
    /* coverity :: 21245 */      
    if (pool_number > 19) {
        /* Return with no update */
        return BCM_E_NONE;
    }

    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                   [pool_number][base_idx]++;

#if defined(BCM_TOMAHAWK_SUPPORT)
    if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
        /* In this mode, counters must be collected independently for each pipe. */
        maxpipe = NUM_PIPE(unit);
    }

#endif
    unused_pool = 1;

    for(pipe = 0; pipe < maxpipe; ++pipe) {
        if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe]
                       [pool_number].attached_entries != 0) {
            unused_pool = 0;
            break;
        }
    }

    if (unused_pool) {
        _bcm_esw_stat_flex_enable_pool(
               unit,bcmStatFlexDirectionIngress,
               _pool_ctr_register[bcmStatFlexDirectionIngress][pool_number],1);
    } 

#if defined(BCM_TOMAHAWK_SUPPORT)
    if ((mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
    } else {
#endif
        total_counters = flex_ingress_modes[unit][mode].total_counters;
#if defined(BCM_TOMAHAWK_SUPPORT)
    }
#endif

    flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                       attached_entries += total_counters;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_detach_ingress_table_counters_update
 * Description:
 *      Manage flex reference count, clear counter values in the hardware
 *      and disable flex pool for the stat applicable
 * Parameters:
 *      unit                  - (IN) unit number
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      mode                  - (IN) Flex offset mode for Accounting Object
 * Return Value:
 *      BCM_E_NONE
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_detach_ingress_table_counters_update(
            int                  unit,
            uint32               pool_number,
            uint32               base_idx,
            bcm_stat_flex_mode_t mode)
{
    uint32                          flex_entries=0;
    uint32                          alloc_size=0;
    bcm_stat_flex_ctr_offset_info_t flex_ctr_offset_info={0};
    bcm_stat_flex_counter_value_t   *flex_counter_value=NULL;
    int pipe_num = 0;
    int                             pipe = 0, maxpipe = 1, unused_pool;
    int total_counters = 0;
    /* coverity :: 21246 */
    if (pool_number > 15) {
        /* Return with no update */
        return BCM_E_NONE;
    }

#if defined(BCM_TOMAHAWK_SUPPORT)
    if ((mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
    } else {
#endif
        total_counters = flex_ingress_modes[unit][mode].total_counters;
#if defined(BCM_TOMAHAWK_SUPPORT)
    }
#endif

#if defined (BCM_TOMAHAWK_SUPPORT)
         if ((FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE ==                 
                           flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                         [0][pool_number].used_by_tables)) {
             _bcm_esw_get_fp_pipe_from_mode(unit, mode, 
                         bcmFieldQualifyStageIngress, &pipe_num);
         } else if ((FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE ==                 
                           flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                         [0][pool_number].used_by_tables)) {
             _bcm_esw_get_fp_pipe_from_mode(unit, mode, 
                         bcmFieldQualifyStageLookup, &pipe_num);
         }

#endif
    /* Decrement reference count */
    BCM_STAT_FLEX_COUNTER_LOCK(unit);
    flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number][base_idx]--;
    flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number].
                       attached_entries -= total_counters;
    BCM_STAT_FLEX_COUNTER_UNLOCK(unit);

    /* Clear Counter Values */
    if ((flex_base_index_reference_count[unit]
            [bcmStatFlexDirectionIngress] [pipe_num][pool_number][base_idx]) == 0 ) {

        LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Clearing Counter Tables for EXT_FP_POLICY: Offset:%d "
                               "Len:%d\n"),
                   base_idx,
                   total_counters));

        flex_ctr_offset_info.all_counters_flag =  1;
        alloc_size = sizeof(bcm_stat_flex_counter_value_t) *
                      total_counters;

        flex_counter_value = sal_alloc(alloc_size,"counter-table-values");
        if (flex_counter_value == NULL) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Failed: AllocateCounterMemoryForTable:EXT_FP_POLICY: "
                                   "pool_number:%d mode:%d base_idx:%d\n"),
                       pool_number, mode, base_idx));
            return BCM_E_INTERNAL;
        }

        sal_memset(flex_counter_value,0,alloc_size);
        _bcm_esw_ext_stat_flex_set(unit, pool_number, base_idx, mode,
                                   EXT_FP_POLICYm, 1, flex_ctr_offset_info,
                                   &flex_entries, flex_counter_value);
        _bcm_esw_ext_stat_flex_set(unit, pool_number, base_idx,mode,
                                   EXT_FP_POLICYm, 0, flex_ctr_offset_info,
                                   &flex_entries, flex_counter_value);
        sal_free(flex_counter_value);
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (BCM_TH_LOCAL_PIPE_MODE(unit, pool_number)) {
        /* In this mode, counters must be collected independently for each pipe. */
        maxpipe = NUM_PIPE(unit);
    }

#endif
    unused_pool = 1;

    for(pipe = 0; pipe < maxpipe; ++pipe) {
        if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe][pool_number].
                       attached_entries != 0) {
            unused_pool = 0;
            break;
        }
    }
        
    /* Disable flex pool for this stat on no flex entries */
    if (unused_pool) {
        _bcm_esw_stat_flex_enable_pool(
                 unit,
                 bcmStatFlexDirectionIngress,
                 _pool_ctr_register[bcmStatFlexDirectionIngress][pool_number],
                 0);
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_flex_attach_ingress_table_counters2
 * Description:
 *      Atach i.e. Enable Ingresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *      ingress_entry_data1   - (IN) Entry Data(Null or Valid)
 *                                   Null, table will be modified
 *                                   NON-Null, table won't be modified
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *          ###############################################################
 *          BE CAREFULL while using this routine(i.e. if possible don't use)
 *          ###############################################################
 *          Currently being used from field module to avoid two write operations
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_ingress_table_counters2(
            int                  unit,
            soc_mem_t            ingress_table,
            uint32               index,
            bcm_stat_flex_mode_t mode,
            uint32               base_idx,
            uint32               pool_number,
            bcm_stat_object_t    object,
            void                 *ingress_entry_data1)
{
    uint32               ingress_entry_data_size=0;
    void                 *ingress_entry_data=NULL;
    void                 *ingress_entry_data_temp=NULL;
    bcm_stat_flex_mode_t offset_mode_l={0};
    uint32               stat_counter_id=0;
    uint32               base_idx_l=0;
    uint32               pool_number_l=0;
    int                  pipe_num = 0;
#if defined (BCM_TOMAHAWK_SUPPORT)
    int                  total_counters = 0;
#endif
    uint32               cmode = mode;

#if defined (BCM_TOMAHAWK_SUPPORT)
    if ((mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
        mode = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                offset_mode;
    } else {
        total_counters = flex_ingress_modes[unit][mode].total_counters;
    }
#endif

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_ingress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex counter mode %d not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
#endif

    if (shr_aidxres_list_elem_state(
            flex_aidxres_list_handle[unit][bcmStatFlexDirectionIngress]
            [pipe_num][pool_number],
            base_idx) != BCM_E_EXISTS) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong base index %u \n"),
                   base_idx));
        return BCM_E_NOT_FOUND;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (!_bcm_esw_is_exact_match(ingress_table)) {
#endif
        /* No HW setup for EXACT match */
        ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                                  SOC_MEM_INFO(unit, ingress_table).bytes));
        if (ingress_entry_data1  == NULL ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  ".Attaching INGRESS counter for Table:%s with index:%d"
                                   "mode:%d ENTRY_BYTES %d \n"),
                       SOC_MEM_UFNAME(unit,ingress_table),index,mode,
                       ingress_entry_data_size));
            ingress_entry_data = sal_alloc(ingress_entry_data_size,"ingress_table");
            if (ingress_entry_data == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Failed to allocate memory for Table:%s "),
                           SOC_MEM_UFNAME(unit, ingress_table)));
                return BCM_E_INTERNAL;
            }
            sal_memset(ingress_entry_data,0,
                       SOC_MEM_INFO(unit, ingress_table).bytes);
            if (soc_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                             _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                             ingress_entry_data) != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Read failure for Table %s with index %d \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                sal_free(ingress_entry_data);
                return    BCM_E_INTERNAL;
            }
            ingress_entry_data_temp = ingress_entry_data;
        } else {
            ingress_entry_data_temp = ingress_entry_data1;
        }
        if (soc_mem_field_valid(unit,ingress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data_temp,
                VALIDf)==0) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                if (ingress_entry_data1 == NULL) {
                    sal_free(ingress_entry_data);
                }
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                   unit,index,ingress_table,ingress_entry_data_temp, object,
                   &offset_mode_l,&pool_number_l,&base_idx_l);
        if (base_idx_l != 0) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table:%s Has already allocated with index:%d"
                                   "base %d mode %d."
                                   "First dealloc it \n"), SOC_MEM_UFNAME(unit, ingress_table),
                       index,base_idx_l,offset_mode_l));
            if (ingress_entry_data1 == NULL) {
                sal_free(ingress_entry_data);
            }
            return BCM_E_EXISTS;/*Either NotConfigured or deallocated before */
        }
        _bcm_esw_set_flex_counter_fields_values(
             unit,index,ingress_table,ingress_entry_data_temp,
             object,mode,pool_number,base_idx);
#if defined(BCM_TOMAHAWK_SUPPORT)
        /* Appropriately program G_COUNT, Y_COUNT and R_COUNT fields based on
         * ifp_color_map that has been setup
         */
        if (SOC_IS_TOMAHAWKX(unit) &&
            ((0 == flex_ingress_modes[unit][mode].num_selectors) ||
             ((flex_ingress_modes[unit][mode].num_selectors > 0) && 
             (flex_ingress_modes[unit][mode].attr_selectors[0].attr 
                       == bcmStatGroupModeAttrFieldIngressColor))) &&
            ((IFP_POLICY_TABLEm == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE0m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE1m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE2m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE3m == ingress_table))) {
            int ctr_idx, color_mask;
            uint32 g_count = 0, y_count = 0, r_count = 0;
            uint8 color_map_mask[BCM_MAX_IFP_COLORS] = {BCM_STAT_FLEX_COLOR_GREEN,
                                                        BCM_STAT_FLEX_COLOR_YELLOW,
                                                        BCM_STAT_FLEX_COLOR_RED};

            for (ctr_idx = 0;
                 ctr_idx < total_counters;
                 ctr_idx++) {
                if (0 != ifp_color_map[unit][pipe_num][pool_number][base_idx + ctr_idx]) {
                    for (color_mask = 0; color_mask < BCM_MAX_IFP_COLORS; color_mask++) {
                        switch (ifp_color_map[unit][pipe_num][pool_number][base_idx + ctr_idx]
                                & color_map_mask[color_mask]) {
                            /* Checking arguments once again here, although this
                             * should be checked while setting up color map
                             */
                            case BCM_STAT_FLEX_COLOR_GREEN:
                                if (0 == g_count) {
                                    g_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color green used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            case BCM_STAT_FLEX_COLOR_YELLOW:
                                if (0 == y_count) {
                                    y_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color yellow used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            case BCM_STAT_FLEX_COLOR_RED:
                                if (0 == r_count) {
                                    r_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color red used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            default:
                                if (0 != (color_map_mask[color_mask] &
                                    ifp_color_map[unit][pipe_num][pool_number]
                                    [base_idx + ctr_idx])) {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    return BCM_E_PARAM;
                                }
                                break;
                        }
                    }
                } else {
                    if (ingress_entry_data1 == NULL) {
                        sal_free(ingress_entry_data);
                    }
                    /* IFP color map not setup correctly */
                    return BCM_E_INTERNAL;
                }
            }
            if ((0 == g_count) && (0 == y_count) && (0 == r_count)) {
                /* IFP color map not setup correctly */
                if (ingress_entry_data1 == NULL) {
                    sal_free(ingress_entry_data);
                }
                return BCM_E_INTERNAL;
            }
            /* Program g_count, y_count and r_count fields appropriately */
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            G_COUNTf, &g_count);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            Y_COUNTf, &y_count);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            R_COUNTf, &r_count);
        }
#endif
        if (ingress_entry_data1 == NULL) {
            if (soc_mem_write(unit, ingress_table, MEM_BLOCK_ALL,
                              _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                              ingress_entry_data_temp) != SOC_E_NONE) {
                sal_free(ingress_entry_data);
                return    BCM_E_INTERNAL;
            }
        }
        if (ingress_entry_data1 == NULL) {
            sal_free(ingress_entry_data);
        }
#if defined (BCM_TOMAHAWK_SUPPORT)
    }
#endif

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
       (cmode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (cmode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        bcm_error_t rv = BCM_E_CONFIG;
 
        rv = _bcm_esw_custom_stat_group_id_retrieve(unit, cmode, pipe_num, 
                               pool_number, base_idx, &stat_counter_id);
        if (rv !=BCM_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid mode value %d %d %d\n"),
                   cmode, pool_number, base_idx));
        }
    } else
#endif
    {
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][mode].group_mode,
                  object,mode,pool_number,base_idx,&stat_counter_id);
    }

    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                       [pool_number][base_idx] == 0) {
        if (_bcm_esw_stat_flex_delete_stat_id(unit,
            local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "WARMBOOT:Couldnot Delete entry in scache memory\n")));
        }
    }

    if (_bcm_esw_stat_flex_attach_ingress_table_counters_update(
        unit, pool_number, pipe_num, base_idx, cmode) != BCM_E_NONE) {
        return BCM_E_INTERNAL;
    }

    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_attach_ingress_table_counters1
 * Description:
 *      Atach i.e. Enable Ingresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *      ingress_entry_data1   - (IN) Entry Data(Null or Valid)
 *                                   Null, table will be modified
 *                                   NON-Null, table won't be modified
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *          ###############################################################
 *          BE CAREFULL while using this routine(i.e. if possible don't use)
 *          ###############################################################
 *          Currently being used from field module to avoid two write operations
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_ingress_table_counters1(
            int                  unit,
            soc_mem_t            ingress_table,
            uint32               index,
            bcm_stat_flex_mode_t mode,
            uint32               base_idx,
            uint32               pool_number,
            void                 *ingress_entry_data1)
{
    uint32               ingress_entry_data_size=0;
    void                 *ingress_entry_data=NULL;
    void                 *ingress_entry_data_temp=NULL;
    bcm_stat_flex_mode_t offset_mode_l={0};
    bcm_stat_object_t    object=bcmStatObjectIngPort;
    uint32               stat_counter_id=0;
    uint32               base_idx_l=0;
    uint32               pool_number_l=0;
    int                  pipe_num = 0;
#if defined (BCM_TOMAHAWK_SUPPORT)
    int                  total_counters = 0;
#endif
    uint32               cmode = mode;

#if defined (BCM_TOMAHAWK_SUPPORT)
    if ((mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                total_counters;
        mode = flex_custom_ingress_modes[unit][mode - BCM_CUSTOM_INGRESS_MODE_START].
                                                offset_mode;
    } else {
        total_counters = flex_ingress_modes[unit][mode].total_counters;
    }
#endif

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_ingress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex counter mode %d not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    pipe_num = _bcm_esw_get_fp_pipe_num(unit, ingress_table);
#endif

    if (shr_aidxres_list_elem_state(
            flex_aidxres_list_handle[unit][bcmStatFlexDirectionIngress]
            [pipe_num][pool_number],
            base_idx) != BCM_E_EXISTS) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Wrong base index %u \n"),
                   base_idx));
        return BCM_E_NOT_FOUND;
    }

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (!_bcm_esw_is_exact_match(ingress_table)) {
#endif
        /* No HW setup for EXACT match */
        ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                                  SOC_MEM_INFO(unit, ingress_table).bytes));
        if (ingress_entry_data1  == NULL ) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  ".Attaching INGRESS counter for Table:%s with index:%d"
                                   "mode:%d ENTRY_BYTES %d \n"),
                       SOC_MEM_UFNAME(unit,ingress_table),index,mode,
                       ingress_entry_data_size));
            ingress_entry_data = sal_alloc(ingress_entry_data_size,"ingress_table");
            if (ingress_entry_data == NULL) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Failed to allocate memory for Table:%s "),
                           SOC_MEM_UFNAME(unit, ingress_table)));
                return BCM_E_INTERNAL;
            }
            sal_memset(ingress_entry_data,0,
                       SOC_MEM_INFO(unit, ingress_table).bytes);
            if (soc_mem_read(unit, ingress_table, MEM_BLOCK_ANY,
                             _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                             ingress_entry_data) != SOC_E_NONE) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Read failure for Table %s with index %d \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                sal_free(ingress_entry_data);
                return    BCM_E_INTERNAL;
            }
            ingress_entry_data_temp = ingress_entry_data;
        } else {
            ingress_entry_data_temp = ingress_entry_data1;
        }
        if (soc_mem_field_valid(unit,ingress_table,VALIDf)) {
            if (soc_mem_field32_get(unit,ingress_table,ingress_entry_data_temp,
                VALIDf)==0) {
                LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                          (BSL_META_U(unit,
                                      "Table %s  with index %d is Not valid \n"),
                           SOC_MEM_UFNAME(unit, ingress_table),index));
                if (ingress_entry_data1 == NULL) {
                    sal_free(ingress_entry_data);
                }
                return BCM_E_PARAM;
            }
        }
        _bcm_esw_get_flex_counter_fields_values(
                   unit,index,ingress_table,ingress_entry_data_temp,0,
                   &offset_mode_l,&pool_number_l,&base_idx_l);
        if (base_idx_l != 0) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "Table:%s Has already allocated with index:%d"
                                   "base %d mode %d."
                                   "First dealloc it \n"), SOC_MEM_UFNAME(unit, ingress_table),
                       index,base_idx_l,offset_mode_l));
            if (ingress_entry_data1 == NULL) {
                sal_free(ingress_entry_data);
            }
            return BCM_E_EXISTS;/*Either NotConfigured or deallocated before */
        }
        _bcm_esw_set_flex_counter_fields_values(
             unit,index,ingress_table,ingress_entry_data_temp,0,
             mode,pool_number,base_idx);
#if defined(BCM_TOMAHAWK_SUPPORT)
        /* Appropriately program G_COUNT, Y_COUNT and R_COUNT fields based on
         * ifp_color_map that has been setup
         */
        if (SOC_IS_TOMAHAWKX(unit) &&
            ((0 == flex_ingress_modes[unit][mode].num_selectors) ||
             ((flex_ingress_modes[unit][mode].num_selectors > 0) && 
             (flex_ingress_modes[unit][mode].attr_selectors[0].attr 
                       == bcmStatGroupModeAttrFieldIngressColor))) &&
            ((IFP_POLICY_TABLEm == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE0m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE1m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE2m == ingress_table) ||
             (IFP_POLICY_TABLE_PIPE3m == ingress_table))) {
            int ctr_idx, color_mask;
            uint32 g_count = 0, y_count = 0, r_count = 0;
            uint8 color_map_mask[BCM_MAX_IFP_COLORS] = {BCM_STAT_FLEX_COLOR_GREEN,
                                                        BCM_STAT_FLEX_COLOR_YELLOW,
                                                        BCM_STAT_FLEX_COLOR_RED};

            for (ctr_idx = 0;
                 ctr_idx < total_counters;
                 ctr_idx++) {
                if (0 != ifp_color_map[unit][pipe_num][pool_number][base_idx + ctr_idx]) {
                    for (color_mask = 0; color_mask < BCM_MAX_IFP_COLORS; color_mask++) {
                        switch (ifp_color_map[unit][pipe_num][pool_number][base_idx + ctr_idx]
                                & color_map_mask[color_mask]) {
                            /* Checking arguments once again here, although this
                             * should be checked while setting up color map
                             */
                            case BCM_STAT_FLEX_COLOR_GREEN:
                                if (0 == g_count) {
                                    g_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color green used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            case BCM_STAT_FLEX_COLOR_YELLOW:
                                if (0 == y_count) {
                                    y_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color yellow used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            case BCM_STAT_FLEX_COLOR_RED:
                                if (0 == r_count) {
                                    r_count = ctr_idx + 1;
                                } else {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    /* Color red used more than once. */
                                    return BCM_E_PARAM;
                                }
                                continue;
                            default:
                                if (0 != (color_map_mask[color_mask] &
                                    ifp_color_map[unit][pipe_num][pool_number]
                                    [base_idx + ctr_idx])) {
                                    if (ingress_entry_data1 == NULL) {
                                        sal_free(ingress_entry_data);
                                    }
                                    return BCM_E_PARAM;
                                }
                                break;
                        }
                    }
                } else {
                    if (ingress_entry_data1 == NULL) {
                        sal_free(ingress_entry_data);
                    }
                    /* IFP color map not setup correctly */
                    return BCM_E_INTERNAL;
                }
            }
            if ((0 == g_count) && (0 == y_count) && (0 == r_count)) {
                /* IFP color map not setup correctly */
                if (ingress_entry_data1 == NULL) {
                    sal_free(ingress_entry_data);
                }
                return BCM_E_INTERNAL;
            }
            /* Program g_count, y_count and r_count fields appropriately */
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            G_COUNTf, &g_count);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            Y_COUNTf, &y_count);
            soc_IFP_POLICY_TABLEm_field_set(unit, ingress_entry_data_temp,
                                            R_COUNTf, &r_count);
        }
#endif
        if (ingress_entry_data1 == NULL) {
            if (soc_mem_write(unit, ingress_table, MEM_BLOCK_ALL,
                              _bcm_esw_stat_flex_table_index_map(unit,ingress_table,index),
                              ingress_entry_data_temp) != SOC_E_NONE) {
                sal_free(ingress_entry_data);
                return    BCM_E_INTERNAL;
            }
        }
        if(_bcm_esw_stat_flex_get_ingress_object(
           unit,ingress_table,index,ingress_entry_data_temp,&object) !=BCM_E_NONE) {
           if (ingress_entry_data1 == NULL) {
               sal_free(ingress_entry_data);
           }
           return BCM_E_INTERNAL;
        }
        if (ingress_entry_data1 == NULL) {
            sal_free(ingress_entry_data);
        }
#if defined (BCM_TOMAHAWK_SUPPORT)
    }
#endif

#if defined (BCM_TOMAHAWK_SUPPORT)
    if (soc_feature(unit, soc_feature_flex_counter_opaque_stat_id) &&
       (cmode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (cmode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        bcm_error_t rv = BCM_E_CONFIG;
 
        rv = _bcm_esw_custom_stat_group_id_retrieve(unit, cmode, pipe_num, 
                               pool_number, base_idx, &stat_counter_id);
        if (rv !=BCM_E_NONE) {
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid mode value %d %d %d\n"),
                   cmode, pool_number, base_idx));
        }
    } else
#endif
    {
    _bcm_esw_stat_get_counter_id(
                  unit, flex_ingress_modes[unit][mode].group_mode,
                  object,mode,pool_number,base_idx,&stat_counter_id);
    }

    if (flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress][pipe_num]
                                       [pool_number][base_idx] == 0) {
        if (_bcm_esw_stat_flex_delete_stat_id(unit,
            local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                     (BSL_META_U(unit,
                                 "WARMBOOT:Couldnot Delete entry in scache memory\n")));
        }
    }

    if (_bcm_esw_stat_flex_attach_ingress_table_counters_update(
        unit, pool_number, pipe_num, base_idx, cmode) != BCM_E_NONE) {
        return BCM_E_INTERNAL;
    }

    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_attach_ingress_table_counters
 * Description:
 *      Atach i.e. Enable Ingresss accounting table's statistics
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      index                 - (IN) Flex Accounting Table's Index
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *      Calls _bcm_esw_stat_flex_attach_ingress_table_counters1() with
 *      NULL as ingress_entry_data i.e. ingress table will be modified
 *
 */
bcm_error_t _bcm_esw_stat_flex_attach_ingress_table_counters(
            int                  unit,
            soc_mem_t            ingress_table,
            uint32               index,
            bcm_stat_flex_mode_t mode,
            uint32               base_idx,
            uint32               pool_number)
{
   return _bcm_esw_stat_flex_attach_ingress_table_counters1(
          unit, ingress_table, index, mode, base_idx, pool_number, NULL);

}

/*
 * Function:
 *      _bcm_esw_stat_flex_create_ingress_table_counters
 * Description:
 *      Create and Reserve Flex Counter Space for Ingresss accounting table
 * Parameters:
 *      unit                  - (IN) unit number
 *      ingress_table         - (IN) Flex Accounting Table
 *      mode                  - (IN) Flex offset mode for Accounting Object
 *      base_idx              - (IN) Flex Base Index for Accounting Object
 *      pool_number           - (IN) Flex Pool Number for Accounting Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_create_ingress_table_counters(
            int                  unit,
            soc_mem_t            ingress_table,
            int                  lookup,
            bcm_stat_object_t    object,
            bcm_stat_flex_mode_t mode_id,
            uint32               *base_idx,
            uint32               *pool_number)
{
    uint32            base_idx_l=0;
    uint32            pool_number_l=0;
    uint32            default_pool_number=0;
    uint32            used_by_table=0;
    uint32            used_by_table_selection_criteria=0;
    uint32            free_count=0;
    uint32            alloc_count=0;
    uint32            largest_free=0;
    uint32            stat_counter_id=0;
    uint32            num_flex_ingress_pools=SOC_INFO(unit).
                                               num_flex_ingress_pools;
    int               pipe_num = 0;
    uint32            used_by_flag = 0;
    int               total_counters = 0;
    flex_object_config_t object_config = {0};
    bcm_field_qualify_t field_stage = 0; 
    /* Used to indicate if counter allocation for IFP
     * should be carried out from a specific pool
     */
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    uint32            allocate_from_specific_pool = 0;
#endif

#if defined(BCM_TOMAHAWK_SUPPORT)
    int            pool = 0;
#endif
    uint32            mode = mode_id;                 
    uint32            allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_NONE;
    uint32            pool_group_id = 0;
    uint32            pool_group_type;                
    uint32            oam_mode = 0;
                                                      
#if defined(BCM_TOMAHAWK_SUPPORT)
       if (SOC_IS_TOMAHAWKX(unit) &&
                   (ingress_table != IFP_POLICY_TABLEm) && 
                   (ingress_table != IFP_POLICY_TABLE_PIPE0m) && 
                   (ingress_table != IFP_POLICY_TABLE_PIPE1m) && 
                   (ingress_table != IFP_POLICY_TABLE_PIPE2m) && 
                   (ingress_table != IFP_POLICY_TABLE_PIPE3m)) {
           num_flex_ingress_pools -= 8; /* Last 8 are for IFP use only */
       }
#endif

    if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
        if (BCM_STAT_GROUP_MODE_ID_OAM_SUFFIX ==      
            (mode_id & BCM_STAT_GROUP_MODE_ID_OAM_MASK)) {
            mode = mode_id & ~(BCM_STAT_GROUP_MODE_ID_OAM_MASK);
            oam_mode = 1;                             
        }                                             
    }
 
#if defined (BCM_TOMAHAWK_SUPPORT)
    if ((mode_id >= BCM_CUSTOM_INGRESS_MODE_START)  &&
        (mode_id < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][mode_id - 
                                 BCM_CUSTOM_INGRESS_MODE_START].total_counters;
        mode = flex_custom_ingress_modes[unit][mode_id - 
                                 BCM_CUSTOM_INGRESS_MODE_START].offset_mode;
    } else {
#endif
        total_counters = flex_ingress_modes[unit][mode].total_counters;
#if defined (BCM_TOMAHAWK_SUPPORT)
    }
#endif

    if (mode > (BCM_STAT_FLEX_COUNTER_MAX_MODE-1)) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "Invalid flex counter mode value %d \n"),
                   mode));
        return BCM_E_PARAM;
    }
    if (flex_ingress_modes[unit][mode].available==0) {
        LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                  (BSL_META_U(unit,
                              "flex counter mode %d not configured yet\n"),
                   mode));
        return BCM_E_NOT_FOUND;
    }

    /* Below case statement can be avoided by passing arguements for
       pool_number and selection criteria But keeping it for better
       understanding at same place */
    switch(ingress_table) {
        case PORT_TABm:
             default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_PORT_TABLE_POOL_NUMBER;
             used_by_table_selection_criteria=used_by_table =
                     FLEX_COUNTER_POOL_USED_BY_PORT_TABLE;
             break;
            /* VLAN and VFI shares same pool */
        case VLAN_TABm:
             default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_VLAN_TABLE_POOL_NUMBER;
             used_by_table = FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE;
             used_by_table_selection_criteria=
                     (FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE |
                      FLEX_COUNTER_POOL_USED_BY_VFI_TABLE);
             break;
        case VFIm:
             default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_VFI_TABLE_POOL_NUMBER;
             used_by_table = FLEX_COUNTER_POOL_USED_BY_VFI_TABLE;
             used_by_table_selection_criteria=
                     (FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE |
                      FLEX_COUNTER_POOL_USED_BY_VFI_TABLE );
             break;
            /* VRF and MPLS_VC_LABEL shares same pool */
        case VRFm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_VRF_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_VRF_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_VRF_TABLE;
             used_by_table_selection_criteria=FLEX_COUNTER_POOL_USED_BY_VRF_TABLE;
             break;

            /* VLAN_XLATE and MPLS_TABLE shares same pool */
        case MPLS_ENTRYm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case MPLS_ENTRY_EXTDm:
#endif
             if (lookup == 1) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_MPLS_ENTRY_TABLE_POOL_NUMBER;
                 used_by_table =
                     FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE;
                 if (oam_mode && soc_feature(unit,
                     soc_feature_oam_split_counter_pools)) {
                     used_by_table_selection_criteria =
                         FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE;
                 } else {
                     used_by_table_selection_criteria=
                         (FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE|
                          FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE);
                 }
             } else if (lookup == 2) {
#if defined(BCM_APACHE_SUPPORT)
                 if (soc_feature(unit,
                         soc_feature_mpls_entry_second_label_lookup)) {
                     default_pool_number=pool_number_l=
                         FLEX_COUNTER_DEFAULT_MPLS_ENTRY_SCND_LKUP_TABLE_POOL_NUMBER;
                     used_by_table =
                         FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP;
                     used_by_table_selection_criteria=
                         (FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP|
                          FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE|
                          FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE);
                 }
#endif
             }
             break;
        case VLAN_XLATEm:
#if defined(BCM_TRIUMPH3_SUPPORT)
        case VLAN_XLATE_EXTDm:
#endif
             if (lookup == 1) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_VLAN_XLATE_TABLE_POOL_NUMBER;
                 used_by_table = FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE;
                 if (oam_mode && soc_feature(unit,
                     soc_feature_oam_split_counter_pools)) {
                     used_by_table_selection_criteria =
                         FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE;
                 } else {
                     used_by_table_selection_criteria=
                         (FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE|
                          FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE);
                 }
             } else if (lookup == 2) {
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                 if (soc_feature(unit,soc_feature_ing_vlan_xlate_second_lookup)) {
                     default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_VLAN_XLATE_SCND_LKUP_TABLE_POOL_NUMBER;
                     used_by_table =
                         FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP;
                     used_by_table_selection_criteria=
                         (FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP|
                          FLEX_COUNTER_POOL_USED_BY_VRF_TABLE);
                 }
#endif
             }
             break;
            /* L3_IIF and SOURCE_VP shares same pool*/
        case L3_IIFm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_L3_IIF_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_L3_IIF_TABLE_POOL_NUMBER;
             }
             /* Time being keeping separate default pool for l3_iif and source_vp */
             used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE;
             /* But IfRequired L3_IIF & SOURCE_VP counters can be shared in
                same pool*/
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE |
                     FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE ;
             break;
        case L3_TUNNELm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                  default_pool_number=pool_number_l=
                      FLEX_COUNTER_ML_DEFAULT_L3_TUNNEL_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_L3_TUNNEL_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE;
             break;
        case L3_ENTRY_2m:
        case L3_ENTRY_4m:
        case EXT_IPV4_UCAST_WIDEm:
        case EXT_IPV6_128_UCAST_WIDEm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_L3_ENTRY_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_L3_ENTRY_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE;
             break;
#if defined(BCM_TRIDENT2_SUPPORT)
        case L3_DEFIPm:
        case L3_DEFIP_PAIR_128m:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64_1m:
        case L3_DEFIP_ALPM_IPV6_128m:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_L3_ROUTE_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_L3_ROUTE_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_L3_ROUTE_TABLE;
             break;
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_FCOE_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                    FLEX_COUNTER_DEFAULT_FCOE_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE;
             break;
        case ING_VSANm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_FCOE_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                    FLEX_COUNTER_DEFAULT_FCOE_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_VSAN_TABLE;
             break;
#endif
        case EXT_FP_POLICYm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_EXT_FP_POLICY_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_EXT_FP_POLICY_TABLE_POOL_NUMBER;
             }
             used_by_table = FLEX_COUNTER_POOL_USED_BY_EXT_FP_POLICY_TABLE;
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_EXT_FP_POLICY_TABLE;
             break;
        case SOURCE_VPm:
#if defined(BCM_METROLITE_SUPPORT)
             if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_ML_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER;
             } else
#endif
             {
                 default_pool_number=pool_number_l=
                     FLEX_COUNTER_DEFAULT_SOURCE_VP_TABLE_POOL_NUMBER;
             }
             /* Time being keeping separate default pool for source vp and l3_iif */
             used_by_table = FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE;
             /* But IfRequired SOURCE_VP & L3_IIF counters CanBeShared in same
                pool!*/
             used_by_table_selection_criteria=
                     FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE |
                     FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE ;
             break;
        case VFP_POLICY_TABLEm:
            field_stage = bcmFieldQualifyStageLookup;
            used_by_flag = FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE;
            /* coverity[unterminated case : FALSE] */
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
        case FP_POLICY_TABLEm:
        case IFP_POLICY_TABLEm:
            field_stage = _bcm_esw_stat_get_field_stage_from_table(ingress_table);
            /* coverity[unterminated case : FALSE] */
#if defined(BCM_TOMAHAWK_SUPPORT)
        case IFP_POLICY_TABLE_PIPE0m:
        case IFP_POLICY_TABLE_PIPE1m:
        case IFP_POLICY_TABLE_PIPE2m:
        case IFP_POLICY_TABLE_PIPE3m:
        case EXACT_MATCH_2m:
        case EXACT_MATCH_2_PIPE0m:
        case EXACT_MATCH_2_PIPE1m:
        case EXACT_MATCH_2_PIPE2m:
        case EXACT_MATCH_2_PIPE3m:
        case VFP_POLICY_TABLE_PIPE0m:
        case VFP_POLICY_TABLE_PIPE1m:
        case VFP_POLICY_TABLE_PIPE2m:
        case VFP_POLICY_TABLE_PIPE3m:
             /* Get Pipe from ingress_table */
            field_stage = _bcm_esw_stat_get_field_stage_from_table(ingress_table);

            _bcm_esw_get_fp_pipe_pool_from_mode(unit, mode_id, 
                                field_stage, &pipe_num, &pool);
#endif
            if (field_stage == bcmFieldQualifyStageIngress) {
                used_by_flag = FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE;
            } else if (field_stage == bcmFieldQualifyStageIngressExactMatch){
                used_by_flag = FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE;
            } else if (field_stage == bcmFieldQualifyStageLookup) {
                used_by_flag = FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE;
            }
             used_by_table = used_by_flag;

#endif
             used_by_table_selection_criteria = used_by_flag;
             /* Check to see if counter allocation is being requested
              * from a specific pool
              */
             if (0xff != *pool_number) {
                 if (*pool_number < num_flex_ingress_pools) {
                     if ((flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][*pool_number].used_by_tables != 0) &&
                         (used_by_flag !=
                         flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][*pool_number].used_by_tables)) {
                         /* Pool in pipe 0 from which counter allocation is being
                          * requested is not used by IFP_POLICY_TABLE
                          */
                         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                                (BSL_META_U(unit,
                                             "Invalid pool:%d pipe_num:%d\n\r"),
                                             *pool_number, pipe_num));
                         return BCM_E_PARAM;
                     }
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
                     allocate_from_specific_pool = 1;
#endif
                     default_pool_number = pool_number_l = *pool_number;
                 } else {
                     /* Invalid pool_number specified */
                     LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                                            (BSL_META_U(unit,
                                            "Invalid pool:%d pipe_num:%d\n\r"),
                                             *pool_number, pipe_num));
                     return BCM_E_PARAM;
                 }
             } else {
                 if (field_stage == bcmFieldQualifyStageIngress) {
#if defined(BCM_METROLITE_SUPPORT)
                     if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                         default_pool_number = pool_number_l = 
                             FLEX_COUNTER_ML_DEFAULT_IFP_POLICY_TABLE_POOL_NUMBER;
                     } else
#endif
                     {
                         default_pool_number = pool_number_l = 
                            FLEX_COUNTER_DEFAULT_IFP_POLICY_TABLE_POOL_NUMBER;
                     }
                 } else if (field_stage == bcmFieldQualifyStageIngressExactMatch) {
#if defined(BCM_METROLITE_SUPPORT)
                     if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                         default_pool_number = pool_number_l = 
                             FLEX_COUNTER_ML_DEFAULT_EM_POLICY_TABLE_POOL_NUMBER;
                    } else
#endif
                    {
                         default_pool_number = pool_number_l = 
                            FLEX_COUNTER_DEFAULT_EM_POLICY_TABLE_POOL_NUMBER;
                    }
                 } else if (field_stage == bcmFieldQualifyStageLookup) {
                     default_pool_number = pool_number_l = 
                        FLEX_COUNTER_DEFAULT_VFP_POLICY_TABLE_POOL_NUMBER;
                 }
             }
             break;
#if defined(BCM_TOMAHAWK_SUPPORT)
        case AGM_MONITOR_TABLEm:
#if defined(BCM_METROLITE_SUPPORT)
            if (soc_feature(unit, soc_feature_flex_stat_ing_pools_4)) {
                default_pool_number = pool_number_l =
                    FLEX_COUNTER_ML_DEFAULT_AGM_MONITOR_TABLE_POOL_NUMBER;
                used_by_table =
                      FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE;
                used_by_table_selection_criteria=
                      FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE;
            } else
#endif
            {
                if (lookup == 1) {
                     default_pool_number=pool_number_l=
                        FLEX_COUNTER_DEFAULT_AGM_MONITOR_TABLE_POOL_NUMBER;
                    used_by_table =
                          FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE;
                    used_by_table_selection_criteria=
                          FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE;
                } else if (lookup == 2) {
                     default_pool_number=pool_number_l=
                        FLEX_COUNTER_DEFAULT_AGM_MONITOR_TABLE_SCND_LKUP_POOL_NUMBER;
                     used_by_table =
                        FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE_FOR_SCND_LKUP;
                     used_by_table_selection_criteria=
                        FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE_FOR_SCND_LKUP;
                }
            }
            break;
#endif
        default:
             LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Invalid Table is passed %d \n"),
                        ingress_table));
             return BCM_E_INTERNAL;
    }

    BCM_IF_ERROR_RETURN(
        _bcm_esw_stat_flex_object_config_get(unit,
                                             ingress_table, &object_config));

    if (object_config.share_criteria != INVALID_CONFIG) {
        used_by_table_selection_criteria |= object_config.share_criteria;
        default_pool_number=pool_number_l = object_config.config_pool;
    }
    if (object_config.exclude_criteria != INVALID_CONFIG) {
        used_by_table_selection_criteria &= ~(object_config.exclude_criteria);
    }

    if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
        /* For TD2Plus, always start from the first pool and traverse all
         * the pools to identify the correct pool for allocation.
         */                                           
        /* Note: Eventhough for each object the default pool number is defined 
         * , (e.g) for IFP, it is reserved a value of
         * FLEX_COUNTER_DEFAULT_IFP_POLICY_TABLE_POOL_NUMBER, The allocation of
         * pools in TD2Plus starts from zero. This is because of the following
         * reasons.                                   
         * 1. Limited Pools are available for TD2Plus (8 pools).
         * 2. These counter pools needs to be divided for regular as well as 
         * for OAM operation.                         
         * 3. So For TD2Plus, the pools are allocated in the same order as group
         * creation.                                  
         * 4. During Initialization, Specific pools are not reserved for OAM, 
         * They are determined dynamically (i.e) When the group mode is created
         * with OAM capability then the ING_FLEX_CTR_LM_CONFIG register is 
         * programed based on the available group.    
         */                                           
        default_pool_number = pool_number_l = 0;      
                                                      
        /* Check the required group type.             
         */                                           
        if (flex_ingress_modes[unit][mode].oam_flags && oam_mode) {
            allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OAM;
        } else {                                      
            allocate_group_type = BCM_FLEX_POOL_GROUP_USED_BY_OTHER;
        }                                             
                                                      
        /* Check if the Flex counter pool group is already allocated 
         * for the required group mode.               
         */                                           
        for ( pool_group_id = 0;                      
              pool_group_id < _bcm_esw_get_max_pool_groups(unit); 
              pool_group_id++) {                      
            if ((flex_pool_group[unit][pipe_num][pool_group_id].group_type == 
                                                    allocate_group_type) &&
                  flex_pool_group[unit][pipe_num][pool_group_id].free_pools) {
                 default_pool_number = pool_number_l =  
                                flex_pool_group[unit][pipe_num][pool_group_id].base_pool;
                break;                                
            }
        }
    }

    do {
       /* Either free or being used by port table only */
       if ((flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num]
                          [pool_number_l].used_by_tables == 0) ||
           (flex_pool_stat[unit][bcmStatFlexDirectionIngress][pipe_num][pool_number_l].
            used_by_tables & used_by_table_selection_criteria)) {
                                                      
            /* Determine the Pool Group and check if the pool group 
             * is eligible for allocation.            
             */                                       
            pool_group_id = pool_number_l /
                             BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
                                                      
            if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
                pool_group_type = flex_pool_group[unit][pipe_num]
                                                 [pool_group_id].group_type; 
                if (!((BCM_FLEX_POOL_GROUP_USED_BY_NONE == 
                                                    pool_group_type)|| 
                      (allocate_group_type == pool_group_type ))) {
                    pool_number_l = ((pool_group_id + 1) *
                    BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP) % 
                                    num_flex_ingress_pools;
                    continue;                         
                }
            }
                                                      
#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit) &&
                !allocate_from_specific_pool &&
                ((bcmStatObjectIngFieldStageIngress == object) ||
                 ((bcmStatObjectIngPolicy == object) && (mode_id >=
                 BCM_CUSTOM_INGRESS_MODE_START)) ||
                 (bcmStatObjectIngExactMatch == object))) {
                    /* Pool num comes from field module.
                     */
                if (0 != flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                                       [pipe_num][pool_number_l].used_entries) {
                    /* For a non specific pool allocation
                     * request, allocate from an unused pool only.
                     */
                    pool_number_l = (pool_number_l + 1) %
                                    num_flex_ingress_pools;
                    if (pool_number_l == default_pool_number) {
                        break;
                    } else {
                        continue;
                    }
                }
            }
#endif
            if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
                /* Update the pool_group type */      
                flex_pool_group[unit][pipe_num][pool_group_id].group_type 
                                                     = allocate_group_type;
                /* If the pool is newly reserved, update the 
                 * free_pool database                 
                 */                                   
                if (flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [0][pool_number_l].used_by_tables == 0) {
                    if ((flex_pool_group[unit][pipe_num][pool_group_id].free_pools == 
                                      BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP) &&
                        (allocate_group_type ==       
                                      BCM_FLEX_POOL_GROUP_USED_BY_OAM)) {
                        BCM_IF_ERROR_RETURN(          
                               _bcm_esw_stat_flex_oam_enable_ingress_group(unit,
                                                                  pool_group_id,
                                                                  1));
                    }
                    flex_pool_group[unit][pipe_num][pool_group_id].free_pools--;
                }
           }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)               
                /* In TD2Plus, We have already decided the pool to be used for
                 * OAM counters . As per the improvement requirement for
                 * FP, Flex Counters for OAM should not extend to next pool.
                 */
                if (flex_pool_group[unit][0][pool_group_id].group_type == 
                                          BCM_FLEX_POOL_GROUP_USED_BY_OAM) {
                    allocate_from_specific_pool = 1;
                }
#endif /* BCM_TRIDENT2PLUS_SUPPORT */ 
       
           if (shr_aidxres_list_alloc_block(flex_aidxres_list_handle[unit]
                   [bcmStatFlexDirectionIngress][pipe_num][pool_number_l],
                   total_counters,
                   &base_idx_l) == BCM_E_NONE) {
               flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                    [pipe_num][pool_number_l].used_by_tables |= used_by_table;

               if (pipe_num != 0) {
                   /* The pool in pipe 0 also reserved for IFP if the pool is reserved for IFP
                    * in any other pipe */
                   flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                        [0][pool_number_l].used_by_tables |= used_by_table;
               }
#if defined BCM_TOMAHAWK_SUPPORT
               if (SOC_IS_TOMAHAWKX(unit) &&
                   (_bcm_esw_get_fp_mode_global(unit, field_stage) == 1)) {
                  /* If pool belongs to Global mode, reserve it for all pipes. */
                  int pipe;

                  for(pipe = 0; pipe < _MAX_PIPES_PER_DEV(unit); pipe++) {
                     flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                      [pipe][pool_number_l].used_by_tables |= used_by_table;
                  }
               }
#endif /* BCM_TOMAHAWK_SUPPORT */
               SHR_BITSET(flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                    [pipe_num][pool_number_l].used_by_objects, object);
               flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                    [pipe_num][pool_number_l].used_entries += total_counters;
               LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Allocated  counter for Table:%s "
                                      "pool_number:%d mode:%d base_idx:%d ref_cnt:%d\n"),
                          SOC_MEM_UFNAME(unit, ingress_table),
                          pool_number_l,mode,base_idx_l,
                          flex_ingress_modes[unit][mode].reference_count));
               shr_aidxres_list_state(flex_aidxres_list_handle[unit]
                   [bcmStatFlexDirectionIngress][pipe_num][pool_number_l],
                   NULL,NULL,NULL,NULL,&free_count,&alloc_count,&largest_free,
                   NULL);

               LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                         (BSL_META_U(unit,
                                     "Current Pool status free_count:%d alloc_count:%d"
                                      "largest_free:%d used_by_tables:%d"
                                      "used_entries:%d\n"),
                          free_count,alloc_count,largest_free,
                          flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [pipe_num][pool_number_l].used_by_tables,
                          flex_pool_stat[unit][bcmStatFlexDirectionIngress]
                          [pipe_num][pool_number_l].used_entries));
               *pool_number    = pool_number_l;
               *base_idx       = base_idx_l;
               _bcm_esw_stat_get_counter_id(
                             unit, flex_ingress_modes[unit][mode].group_mode,
                             object,mode,pool_number_l,base_idx_l,
                             &stat_counter_id);
               BCM_STAT_FLEX_COUNTER_LOCK(unit);
               flex_ingress_modes[unit][mode].reference_count++;
               BCM_STAT_FLEX_COUNTER_UNLOCK(unit);
               if (_bcm_esw_stat_flex_insert_stat_id(unit,
                   local_scache_ptr[unit],stat_counter_id) != BCM_E_NONE) {
                   LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                            (BSL_META_U(unit,
                                        "WARMBOOT:Couldnot add entry in scache memory"
                                         ".Attach it\n")));
               }
               return BCM_E_NONE;
           }
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
           else {
               if (SOC_IS_TOMAHAWKX(unit) &&
                   allocate_from_specific_pool &&
                   ((bcmStatObjectIngFieldStageIngress == object) ||
                    (bcmStatObjectIngPolicy == object) ||
                    (bcmStatObjectIngExactMatch == object))) {
                      return BCM_E_RESOURCE;
               }
               if (SOC_IS_TD2P_TT2P(unit) &&
                   allocate_from_specific_pool) { 
                   return BCM_E_RESOURCE;
               }
           }
#endif
       }
      
       pool_number_l = (pool_number_l + 1) % num_flex_ingress_pools;

    } while (pool_number_l != default_pool_number);
    LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Pools exhausted for Table:%s\n"),
               SOC_MEM_UFNAME(unit,ingress_table)));
    return BCM_E_RESOURCE;
}

#if defined(BCM_TOMAHAWK_SUPPORT)

bcm_error_t _bcm_esw_ifp_wb_color_map_set(
        int                   unit,
        uint32                pool_number,
        uint32                base_index,
        uint32                pipe_num,
        bcm_stat_flex_mode_t offset_mode,
        _bcm_ifp_color_map_t  *color_map)
{
    uint32 ctr_idx, color_mask;
    soc_mem_t mem;
    uint32 total_counters = 0;
    uint32 num_flex_ingress_pools = SOC_INFO(unit).num_flex_ingress_pools;
    uint8 g_count = 0xFF, y_count = 0xFF, r_count = 0xFF;
    uint8 color_map_mask[BCM_MAX_IFP_COLORS] = {BCM_STAT_FLEX_COLOR_GREEN,
                                                BCM_STAT_FLEX_COLOR_YELLOW,
                                                BCM_STAT_FLEX_COLOR_RED};

    if (NULL == color_map) {
        return BCM_E_PARAM;
    }
     mem = _ctr_counter_table[bcmStatFlexDirectionIngress][pool_number];

    if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        offset_mode = flex_custom_ingress_modes[unit][offset_mode - 
                                 BCM_CUSTOM_INGRESS_MODE_START].offset_mode;

    }
    /* Validate retrieved field values */
    if (offset_mode >= BCM_STAT_FLEX_COUNTER_MAX_MODE) {
        return BCM_E_PARAM;
    }
    if (pool_number >= num_flex_ingress_pools) {
        return BCM_E_PARAM;
    }

    if (base_index >= soc_mem_index_count(unit, mem)) {
        return BCM_E_PARAM;
    }
 

    for (ctr_idx = 0; ctr_idx < BCM_MAX_IFP_COLOR_COUNTERS; ctr_idx++) {
        if (0 != color_map->map[ctr_idx]) {
            if ((ctr_idx > 0) &&
                (0 == total_counters)) {
                /* Incorrect specification of color map. At least one
                 * preceding element is 0 */
                return BCM_E_PARAM;
            }
            for (color_mask = 0; color_mask < BCM_MAX_IFP_COLORS; color_mask++) {
                switch (color_map->map[ctr_idx] & color_map_mask[color_mask]) {
                    case BCM_STAT_FLEX_COLOR_GREEN:
                        if (0xFF == g_count) {
                            g_count = ctr_idx;
                        } else {
                            /* Color green used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    case BCM_STAT_FLEX_COLOR_YELLOW:
                        if (0xFF == y_count) {
                            y_count = ctr_idx;
                        } else {
                            /* Color yellow used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    case BCM_STAT_FLEX_COLOR_RED:
                        if (0xFF == r_count) {
                            r_count = ctr_idx;
                        } else {
                            /* Color red used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    default:
                        if (0 != (color_map->map[ctr_idx] &
                            color_map_mask[color_mask])) {
                            return BCM_E_PARAM;
                        }
                        break;
                }
            }
            total_counters++;
        }
    }
    for (ctr_idx = 0; ctr_idx < total_counters; ctr_idx++) {
        if (NULL == ifp_color_map[unit][pipe_num][pool_number]) {
            /* This should not happen. Error during init ? */
            return BCM_E_INTERNAL;
        }
        ifp_color_map[unit][pipe_num][pool_number][base_index + ctr_idx] =
            color_map->map[ctr_idx];
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_ifp_color_map_set
 * Description:
 *      Specify color map for an IFP stat group. This color
 *      map is used during the stat_attach call, in order to
 *      program G_COUNT, Y_COUNT, R_COUNT appropriately in the
 *      IFP_POLICY_TABLE.
 * Parameters:
 *      unit                  - (IN) unit number
 *      stat_counter_id       - (IN) stat id returned by
 *                              stat_group_create call
 *      color_map             - (IN) Array specifying mapping
 *                              of color to counter offset
 * Return Value:
 *      BCM_E_XXX
 */
bcm_error_t _bcm_esw_ifp_color_map_set(
        int                   unit,
        uint32                stat_counter_id,
        _bcm_ifp_color_map_t  *color_map)
{
    uint32 pool_number, base_index, ctr_idx, color_mask;
    soc_mem_t mem;
    uint32 total_counters = 0;
    bcm_stat_group_mode_t group;
    bcm_stat_object_t object;
    bcm_stat_flex_mode_t offset_mode;
    uint32 num_flex_ingress_pools = SOC_INFO(unit).num_flex_ingress_pools;
    uint8 g_count = 0xFF, y_count = 0xFF, r_count = 0xFF;
    uint8 color_map_mask[BCM_MAX_IFP_COLORS] = {BCM_STAT_FLEX_COLOR_GREEN,
                                                BCM_STAT_FLEX_COLOR_YELLOW,
                                                BCM_STAT_FLEX_COLOR_RED};
    int pipe_num = 0;
    bcm_field_qualify_t stage;
    bcm_stat_flex_custom_ingress_mode_t *cmode = NULL;

    if (NULL == color_map) {
        return BCM_E_PARAM;
    }
    _bcm_esw_stat_get_counter_id_info(
        unit, stat_counter_id, &group, &object,
        &offset_mode, &pool_number, &base_index);

     mem = _ctr_counter_table[bcmStatFlexDirectionIngress][pool_number];

    stage = _bcm_esw_stat_get_field_stage_from_stat_ctr(unit, stat_counter_id);
    _bcm_esw_get_fp_pipe_from_mode(unit, offset_mode, stage, &pipe_num);
    if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        cmode = &flex_custom_ingress_modes[unit][offset_mode - 
                                 BCM_CUSTOM_INGRESS_MODE_START];
        offset_mode = flex_custom_ingress_modes[unit][offset_mode - 
                                 BCM_CUSTOM_INGRESS_MODE_START].offset_mode;

    }
    /* Validate retrieved field values */
    if (!((group >= (bcm_stat_group_mode_t) bcmStatGroupModeFlex1) &&
        (group <= (bcm_stat_group_mode_t) bcmStatGroupModeFlex4))) {
        return BCM_E_PARAM;
    }
    if ((bcmStatObjectIngFieldStageIngress != object) &&
        (bcmStatObjectIngExactMatch != object)) {
        return BCM_E_PARAM;
    }
    if (offset_mode >= BCM_STAT_FLEX_COUNTER_MAX_MODE) {
        return BCM_E_PARAM;
    }
    if (pool_number >= num_flex_ingress_pools) {
        return BCM_E_PARAM;
    }

    if (base_index >= soc_mem_index_count(unit, mem)) {
        return BCM_E_PARAM;
    }
 

    for (ctr_idx = 0; ctr_idx < BCM_MAX_IFP_COLOR_COUNTERS; ctr_idx++) {
        if (0 != color_map->map[ctr_idx]) {
            if ((ctr_idx > 0) &&
                (0 == total_counters)) {
                /* Incorrect specification of color map. At least one
                 * preceding element is 0 */
                return BCM_E_PARAM;
            }
            for (color_mask = 0; color_mask < BCM_MAX_IFP_COLORS; color_mask++) {
                switch (color_map->map[ctr_idx] & color_map_mask[color_mask]) {
                    case BCM_STAT_FLEX_COLOR_GREEN:
                        if (0xFF == g_count) {
                            g_count = ctr_idx;
                        } else {
                            /* Color green used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    case BCM_STAT_FLEX_COLOR_YELLOW:
                        if (0xFF == y_count) {
                            y_count = ctr_idx;
                        } else {
                            /* Color yellow used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    case BCM_STAT_FLEX_COLOR_RED:
                        if (0xFF == r_count) {
                            r_count = ctr_idx;
                        } else {
                            /* Color red used more than once. */
                            return BCM_E_PARAM;
                        }
                        continue;
                    default:
                        if (0 != (color_map->map[ctr_idx] &
                            color_map_mask[color_mask])) {
                            return BCM_E_PARAM;
                        }
                        break;
                }
            }
            total_counters++;
        }
    }
    if ((0 == total_counters) ||
        (cmode && cmode->total_counters != total_counters) ||
        (!cmode && (total_counters != flex_ingress_modes[unit]
        [offset_mode].total_counters))) {
        /* Number of counters specified in color map does not match
         * number of counters for the given mode.
         */
        return BCM_E_PARAM;
    }
    for (ctr_idx = 0; ctr_idx < total_counters; ctr_idx++) {
        if (NULL == ifp_color_map[unit][pipe_num][pool_number]) {
            /* This should not happen. Error during init ? */
            return BCM_E_INTERNAL;
        }
        ifp_color_map[unit][pipe_num][pool_number][base_index + ctr_idx] =
            color_map->map[ctr_idx];
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_ifp_color_map_get
 * Description:
 *      Read color map for an IFP stat group.
 * Parameters:
 *      unit                  - (IN) unit number
 *      stat_counter_id       - (IN) stat id returned by
 *                              stat_group_create call
 *      color_map             - (OUT) Array specifying mapping
 *                              of color to counter offset
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_ifp_color_map_get(
        int                   unit,
        uint32                stat_counter_id,
        _bcm_ifp_color_map_t  *color_map)
{
    uint32 pool_number, base_index, ctr_idx;
    soc_mem_t  mem;
    uint32 total_counters = 0;
    bcm_stat_group_mode_t group;
    bcm_stat_object_t object;
    bcm_stat_flex_mode_t offset_mode;
    uint32 num_flex_ingress_pools = SOC_INFO(unit).num_flex_ingress_pools;
    int pipe_num = 0;
    bcm_field_qualify_t stage;

    if (NULL == color_map) {
        return BCM_E_PARAM;
    }
    _bcm_esw_stat_get_counter_id_info(
        unit, stat_counter_id, &group, &object,
        &offset_mode, &pool_number, &base_index);
    /* Validate retrieved field values */
    if (!((group >= (bcm_stat_group_mode_t) bcmStatGroupModeFlex1) &&
        (group <= (bcm_stat_group_mode_t) bcmStatGroupModeFlex4))) {
        return BCM_E_PARAM;
    }
    if ((bcmStatObjectIngFieldStageIngress != object) &&
        (bcmStatObjectIngExactMatch != object)) {
        return BCM_E_PARAM;
    }
    if (pool_number >= num_flex_ingress_pools) {
        return BCM_E_PARAM;
    }

     mem = _ctr_counter_table[bcmStatFlexDirectionIngress][pool_number];
    stage = _bcm_esw_stat_get_field_stage_from_stat_ctr(unit, stat_counter_id);
    _bcm_esw_get_fp_pipe_from_mode(unit, offset_mode, stage, &pipe_num);

    if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
        total_counters = flex_custom_ingress_modes[unit][offset_mode].total_counters;
    } else if (offset_mode >= BCM_STAT_FLEX_COUNTER_MAX_MODE) {
        return BCM_E_PARAM;
    } else {
        total_counters = flex_ingress_modes[unit][offset_mode].total_counters;
    }

    _CTR_COUNTER_TABLE_IFP(unit, bcmStatFlexDirectionIngress, pipe_num, pool_number, mem);

    if (base_index >= soc_mem_index_count(unit, mem)) {
        return BCM_E_PARAM;
    }
    if (0 == flex_base_index_reference_count[unit][bcmStatFlexDirectionIngress]
        [pipe_num][pool_number][base_index]) {
        /* Base index not in use */
        return BCM_E_PARAM;
    }

    if (NULL == ifp_color_map[unit][pipe_num][pool_number]) {
        /* This should not happen. Possible init failure */
        return BCM_E_INTERNAL;
    }
    if (total_counters > BCM_MAX_IFP_COLOR_COUNTERS) {
        /* Error mismatch in number of counters */
        return BCM_E_PARAM;
    }

    for (ctr_idx = 0; ctr_idx < total_counters; ctr_idx++) {
        color_map->map[ctr_idx] = ifp_color_map[unit][pipe_num][pool_number]
                                  [base_index + ctr_idx];
    }
    return BCM_E_NONE;
}
#endif
/*
 * Function:
 *      _bcm_esw_stat_flex_set_group_mode
 * Description:
 *      Set Flex group mode  in s/w copy  for reference
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data Flow Direction
 *      offset_mode           - (IN) Flex Offset Mode
 *      group_mode            - (IN) Flex Group Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_set_group_mode(
            int                       unit,
            bcm_stat_flex_direction_t direction,
            uint32                    offset_mode,
            bcm_stat_group_mode_t     group_mode)
{
    /* Better to check */
    if(!((group_mode >= bcmStatGroupModeSingle) &&
         (group_mode <= (bcm_stat_group_mode_t) bcmStatGroupModeFlex4))) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid bcm_stat_group_mode_t passed %d \n"),
                    group_mode));
         return BCM_E_PARAM;
    }
    if (direction == bcmStatFlexDirectionIngress) {
#if defined (BCM_TOMAHAWK_SUPPORT)
        if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
            offset_mode = flex_custom_ingress_modes[unit][offset_mode - 
                                     BCM_CUSTOM_INGRESS_MODE_START].offset_mode;
    }
#endif
        flex_ingress_modes[unit][offset_mode].group_mode = group_mode;
    } else {
        flex_egress_modes[unit][offset_mode].group_mode = group_mode;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_reset_group_mode
 * Description:
 *      ReSet Flex group mode  in s/w copy  for reference
 * Parameters:
 *      unit                  - (IN) unit number
 *      direction             - (IN) Flex Data Flow Direction
 *      offset_mode           - (IN) Flex Offset Mode
 *      group_mode            - (IN) Flex Group Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_reset_group_mode(
            int                       unit,
            bcm_stat_flex_direction_t direction,
            uint32                    offset_mode,
            bcm_stat_group_mode_t     group_mode)
{
    /* Better to check */
    if(!((group_mode >= bcmStatGroupModeSingle) &&
         (group_mode <= (bcm_stat_group_mode_t) bcmStatGroupModeFlex4))) {
         LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                   (BSL_META_U(unit,
                               "Invalid bcm_stat_group_mode_t passed %d \n"),
                    group_mode));
         return BCM_E_PARAM;
    }
    if (direction == bcmStatFlexDirectionIngress) {
#if defined (BCM_TOMAHAWK_SUPPORT)
        if ((offset_mode >= BCM_CUSTOM_INGRESS_MODE_START) &&
        (offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE)) {
            offset_mode = flex_custom_ingress_modes[unit][offset_mode -
                                             BCM_CUSTOM_INGRESS_MODE_START].offset_mode;
    }
#endif
        flex_ingress_modes[unit][offset_mode].group_mode = 0;
    } else {
        flex_egress_modes[unit][offset_mode].group_mode = 0;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_show_mode_info
 * Description:
 *      Show Mode information
 * Parameters:
 *      unit                  - (IN) unit number
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *      For Debugging purpose only
 */
void _bcm_esw_stat_flex_show_mode_info(int unit)
{
    uint32                    mode=0;
    LOG_CLI((BSL_META_U(unit,
                        "#####################  INGRESS  #########################\n")));
    LOG_CLI((BSL_META_U(unit,
                        "Mode:\t\tReference_Count\t\tTotal_Counters\t\tGroup_Mode \n")));
    for (mode=0;mode<BCM_STAT_FLEX_COUNTER_MAX_MODE;mode++) {
         if (flex_ingress_modes[unit][mode].available) {
             LOG_CLI((BSL_META_U(unit,
                                 "%u\t\t%u\t\t%u\t\t%s\n"), mode,
                      flex_ingress_modes[unit][mode].reference_count,
                      flex_ingress_modes[unit][mode].total_counters,
                      flex_group_modes[flex_ingress_modes[unit][mode].
                      group_mode]));
         } else {
             LOG_CLI((BSL_META_U(unit,
                                 "%u===UNCONFIGURED====\n"), mode));
         }
    }
    LOG_CLI((BSL_META_U(unit,
                        "#####################  EGRESS  ##########################\n")));
    LOG_CLI((BSL_META_U(unit,
                        "Mode:\t\tReference_Count\t\tTotal_Counters\t\tGroup_Mode \n")));
    for (mode=0;mode<BCM_STAT_FLEX_COUNTER_MAX_MODE;mode++) {
         if (flex_egress_modes[unit][mode].available) {
             LOG_CLI((BSL_META_U(unit,
                                 "%u\t\t%u\t\t%u\t\t%s\n"), mode,
                      flex_egress_modes[unit][mode].reference_count,
                      flex_egress_modes[unit][mode].total_counters,
                      flex_group_modes[flex_egress_modes[unit][mode].group_mode]));
         } else {
             LOG_CLI((BSL_META_U(unit,
                                 "%u===UNCONFIGURED====\n"), mode));
         }
    }
}
/*
 * Function:
 *      _bcm_esw_stat_flex_get_ingress_object
 * Description:
 *      Get Ingress Object  corresponding to given Ingress Flex Table
 * Parameters:
 *      ingress_table                  - (IN) Flex Ingress Table
 *      object                         - (OUT) Flex Ingress Object
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_ingress_object(
            int               unit,
            soc_mem_t         ingress_table,
            uint32            table_index,
            void              *ingress_entry,
            bcm_stat_object_t *object)
{
    soc_mem_info_t *memp;
    void           *entry=NULL;
    uint32         ingress_entry_data_size=0;
    uint32         key_type = 0;

    memp = &SOC_MEM_INFO(unit, ingress_table);

    if (memp->flags & SOC_MEM_FLAG_MULTIVIEW) {
        if (ingress_entry != NULL) {
            entry = ingress_entry;
        } else {
            ingress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                                      SOC_MEM_INFO(unit, ingress_table).bytes));
            entry = sal_alloc(ingress_entry_data_size,"ingress_table");
            if (entry == NULL) {
                return BCM_E_MEMORY;
            }
            if (soc_mem_read(unit, ingress_table, MEM_BLOCK_ANY,table_index,
                             entry) != SOC_E_NONE) {
                sal_free(entry);
                return BCM_E_INTERNAL;
            }
        }
        if (soc_mem_field_valid(unit, ingress_table, KEY_TYPEf)) {
            soc_mem_field_get(unit, ingress_table, entry, KEY_TYPEf, &key_type);
        } else if (soc_mem_field_valid(unit, ingress_table, KEY_TYPE_0f)) {
            soc_mem_field_get(unit, ingress_table, entry,
                              KEY_TYPE_0f, &key_type);
        } else if (soc_mem_field_valid(unit, ingress_table,ENTRY_TYPEf)) {
            soc_mem_field_get(unit, ingress_table, entry,
                              ENTRY_TYPEf, &key_type);
        }
        if (ingress_entry == NULL) {
            sal_free(entry);
        }
    }
    switch(ingress_table) {
    case PORT_TABm:
         *object=bcmStatObjectIngPort;
         break;
    case VLAN_TABm:
         *object=bcmStatObjectIngVlan;
         break;
    case VLAN_XLATEm:
#if defined(BCM_TRIDENT2_SUPPORT)
         if (sal_strcmp(memp->views[key_type],"VXLAN_DIP")  == 0) {
             *object = bcmStatObjectIngVxlanDip;
         } else
#endif /* BCM_TRIDENT2_SUPPORT */
         if (sal_strcmp(memp->views[key_type],"L2GRE_DIP")  == 0) {
             *object = bcmStatObjectIngL2Gre;
         } else {
             *object=bcmStatObjectIngVlanXlate;
         }
         break;
#if defined(BCM_TRIUMPH3_SUPPORT)
    case VLAN_XLATE_EXTDm:
         *object=bcmStatObjectIngVlanXlate;
         break;
#endif
    case VFIm:
        *object=bcmStatObjectIngVfi;
        break;
    case L3_IIFm:
         *object=bcmStatObjectIngL3Intf;
         break;
    case VRFm:
         *object=bcmStatObjectIngVrf;
         break;
    case SOURCE_VPm:
#if defined(BCM_TRIUMPH3_SUPPORT) && defined(INCLUDE_L3)
        if (_bcm_vp_used_get(unit, table_index,_bcmVpTypeL2Gre)) {
             *object = bcmStatObjectIngL2Gre;
        } else if (_bcm_vp_used_get(unit, table_index,_bcmVpTypeVxlan)) {
            *object=bcmStatObjectIngVxlan;
        } else if (_bcm_vp_used_get(unit, table_index,_bcmVpTypeNiv)) {
            *object=bcmStatObjectIngNiv;
        } else
#endif
        {
            *object=bcmStatObjectIngMplsVcLabel;
        }
         break;
    case MPLS_ENTRYm:
         *object=bcmStatObjectIngMplsSwitchLabel;
         break;
#if defined(BCM_TRIUMPH3_SUPPORT)
    case MPLS_ENTRY_EXTDm:
         if (sal_strcmp(memp->views[key_type],"TRILL") == 0) {
             *object=bcmStatObjectIngTrill;
         } else if (sal_strcmp(memp->views[key_type],"MIM_ISID") == 0) {
             *object=bcmStatObjectIngMimLookupId;
         } else {
             *object=bcmStatObjectIngMplsSwitchLabel;
         }
         break;
#endif
    case VFP_POLICY_TABLEm:
    case VFP_POLICY_TABLE_PIPE0m:
    case VFP_POLICY_TABLE_PIPE1m:
    case VFP_POLICY_TABLE_PIPE2m:
    case VFP_POLICY_TABLE_PIPE3m:
         *object=bcmStatObjectIngPolicy;
         break;
    case EXT_FP_POLICYm:
         *object=bcmStatObjectIngEXTPolicy;
         break;
    case L3_TUNNELm:
         *object=bcmStatObjectIngMplsFrrLabel;
         break;
    case L3_ENTRY_2m:
         *object=bcmStatObjectIngL3Host;
         break;
    case L3_ENTRY_4m:
         *object=bcmStatObjectIngL3Host;
         break;
    case EXT_IPV4_UCAST_WIDEm:
         *object=bcmStatObjectIngL3Host;
         break;
    case EXT_IPV6_128_UCAST_WIDEm:
         *object=bcmStatObjectIngL3Host;
         break;
#if defined(BCM_TRIDENT2_SUPPORT)
    case ING_VSANm:
        if (SOC_IS_TD2_TT2(unit)) {
            *object = bcmStatObjectIngVsan;
        }
         break;
    case L3_ENTRY_IPV4_MULTICASTm:
        if (SOC_IS_TD2_TT2(unit)) {
            if (sal_strcmp(memp->views[key_type],"FCOE_EXT") == 0) {
                *object=bcmStatObjectIngFcoe;
            } else if (sal_strcmp(memp->views[key_type],"IPV4UC_EXT") == 0) {
                *object=bcmStatObjectIngL3Host;
            } else if (sal_strcmp(memp->views[key_type],"IPV4MC") == 0) {
                *object=bcmStatObjectIngIpmc;
            }
        }
         break;
    case L3_ENTRY_IPV6_MULTICASTm:
        if (SOC_IS_TD2_TT2(unit)) {
            if (sal_strcmp(memp->views[key_type],"IPV6UC_EXT") == 0) {
                *object=bcmStatObjectIngL3Host;
            } else if (sal_strcmp(memp->views[key_type],"IPV6MC") == 0) {
                *object=bcmStatObjectIngIpmc;
            }
        }
         break;
    case L3_DEFIPm:
    case L3_DEFIP_PAIR_128m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
        if (SOC_IS_TD2_TT2(unit)) {
            *object = bcmStatObjectIngL3Route;
        }
         break;
#endif /* BCM_TRIDENT2_SUPPORT */

#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    case FP_POLICY_TABLEm:
         if (SOC_IS_TD2P_TT2P(unit)) {
             *object = bcmStatObjectIngFieldStageIngress;
         }
         break;
#endif
#if defined (BCM_TOMAHAWK_SUPPORT)
    case IFP_POLICY_TABLEm:
         if (SOC_IS_TOMAHAWKX(unit)) {
             *object = bcmStatObjectIngFieldStageIngress;
         }
         break;
    case IFP_POLICY_TABLE_PIPE0m:
    case IFP_POLICY_TABLE_PIPE1m:
    case IFP_POLICY_TABLE_PIPE2m:
    case IFP_POLICY_TABLE_PIPE3m:
         if (SOC_IS_TOMAHAWKX(unit)) {
             *object = bcmStatObjectIngFieldStageIngress;
         }
         break;
    case EXACT_MATCH_2m:
    case EXACT_MATCH_2_PIPE0m:
    case EXACT_MATCH_2_PIPE1m:
    case EXACT_MATCH_2_PIPE2m:
    case EXACT_MATCH_2_PIPE3m:
    case EXACT_MATCH_4m:
    case EXACT_MATCH_4_PIPE0m:
    case EXACT_MATCH_4_PIPE1m:
    case EXACT_MATCH_4_PIPE2m:
    case EXACT_MATCH_4_PIPE3m:
         if (SOC_IS_TOMAHAWKX(unit)) {
             *object = bcmStatObjectIngExactMatch;
         }
         break;
    case AGM_MONITOR_TABLEm:
         if (SOC_IS_TOMAHAWKX(unit)) {
             *object = bcmIntStatObjectIngAgm;
         }
         break;
#endif

    default:
         return BCM_E_INTERNAL;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_get_egress_object
 * Description:
 *      Get Egress Object  corresponding to given Egress Flex Table
 * Parameters:
 *      egress_table                   - (IN) Flex Egress Table
 *      object                         - (OUT) Flex Egress Object
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_egress_object(
            int               unit,
            soc_mem_t         egress_table,
            uint32            table_index,
            void              *egress_entry,
            bcm_stat_object_t *object)
{
    soc_mem_info_t *memp;
    void           *entry=NULL;
    uint32         egress_entry_data_size=0;
    uint32         key_type = 0;
#if defined(BCM_TRIUMPH3_SUPPORT) && defined(INCLUDE_L3)
    uint32         dvp = 0;
#endif
    memp = &SOC_MEM_INFO(unit, egress_table);

    if (memp->flags & SOC_MEM_FLAG_MULTIVIEW) {
        if (egress_entry != NULL) {
            entry = egress_entry;
        } else {
            egress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                                      SOC_MEM_INFO(unit, egress_table).bytes));
            entry = sal_alloc(egress_entry_data_size,"egress_table");
            if (entry == NULL) {
                return BCM_E_MEMORY;
            }
            if (soc_mem_read(unit, egress_table, MEM_BLOCK_ANY,table_index,
                             entry) != SOC_E_NONE) {
                sal_free(entry);
                return BCM_E_INTERNAL;
            }
        }

        if (soc_mem_field_valid(unit, egress_table, KEY_TYPEf)) {
            soc_mem_field_get(unit, egress_table, entry, KEY_TYPEf, &key_type);
        } else if (soc_mem_field_valid(unit, egress_table, KEY_TYPE_0f)) {
            soc_mem_field_get(unit, egress_table, entry,
                              KEY_TYPE_0f, &key_type);
        } else if (soc_mem_field_valid(unit, egress_table,ENTRY_TYPEf)) {
            soc_mem_field_get(unit, egress_table, entry,
                              ENTRY_TYPEf, &key_type);
        }
#if defined(BCM_TRIUMPH3_SUPPORT) && defined(INCLUDE_L3)
        if ((egress_table == EGR_L3_NEXT_HOPm) &&
            (sal_strcmp(memp->views[key_type],"SD_TAG") == 0)) {
            soc_mem_field_get(unit, egress_table, entry,SD_TAG__DVPf, &dvp);
        }
#endif
        if (egress_entry == NULL) {
            sal_free(entry);
        }
    }

    switch(egress_table) {
    case EGR_PORTm:
         *object=bcmStatObjectEgrPort;
         break;
    case EGR_VLANm:
         *object=bcmStatObjectEgrVlan;
         break;
    case EGR_VLAN_XLATEm:
         if (sal_strcmp(memp->views[key_type],"MIM_ISID") == 0) {
             *object=bcmStatObjectEgrMimLookupId;
         } else {
             *object=bcmStatObjectEgrVlanXlate;
         }
         break;
    case EGR_VFIm:
         *object=bcmStatObjectEgrVfi;
         break;
    case EGR_L3_NEXT_HOPm:
         if (sal_strcmp(memp->views[key_type],"WLAN") == 0) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "WLAN view")));
             *object=bcmStatObjectEgrWlan;
         } else if (sal_strcmp(memp->views[key_type],"MIM") == 0) {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "MIM view")));
             *object=bcmStatObjectEgrMim;
        }
#if defined(BCM_TRIUMPH3_SUPPORT) && defined(INCLUDE_L3)
        else if (sal_strcmp(memp->views[key_type],"SD_TAG") == 0) {
            LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                      (BSL_META_U(unit,
                                  "SD_TAG view\n")));
            if (_bcm_vp_used_get(unit, dvp,_bcmVpTypeVxlan)) {
                *object=bcmStatObjectEgrVxlan;
            } else if (_bcm_vp_used_get(unit, dvp,_bcmVpTypeNiv)) {
                *object=bcmStatObjectEgrNiv;
            } else {
                *object=bcmStatObjectEgrL3Intf;
            }
         }
#endif
		 else {
             LOG_DEBUG(BSL_LS_BCM_FLEXCTR,
                       (BSL_META_U(unit,
                                   "Other view %s \n"),
                        memp->views[key_type]));
             *object=bcmStatObjectEgrL3Intf;
         }

         break;
    case EGR_DVP_ATTRIBUTE_1m:
        egress_table = EGR_DVP_ATTRIBUTEm;
        memp = &SOC_MEM_INFO(unit, egress_table);
        if (memp->flags & SOC_MEM_FLAG_MULTIVIEW) {
            egress_entry_data_size = WORDS2BYTES(BYTES2WORDS(
                                      SOC_MEM_INFO(unit, egress_table).bytes));
            entry = sal_alloc(egress_entry_data_size,"egress_table");
            if (entry == NULL) {
                return BCM_E_MEMORY;
            }
            if (soc_mem_read(unit, egress_table, MEM_BLOCK_ANY,table_index,
                             entry) != SOC_E_NONE) {
                sal_free(entry);
                return BCM_E_INTERNAL;
            }

            if (soc_mem_field_valid(unit, egress_table, VP_TYPEf)) {
                soc_mem_field_get(unit, egress_table, entry, VP_TYPEf, &key_type);
            }
            sal_free(entry);
        }
#if defined(BCM_TRIDENT2_SUPPORT)
         if (sal_strcmp(memp->views[key_type],"VXLAN") == 0) {
            *object = bcmStatObjectEgrVxlan;
         } else
#endif /* BCM_TRIDENT2_SUPPORT */
         if (sal_strcmp(memp->views[key_type],"L2GRE") == 0) {
            *object = bcmStatObjectEgrL2Gre;
         }
         break;
#if defined(BCM_TRIDENT2_SUPPORT)
    case  EGR_NAT_PACKET_EDIT_INFOm:
        if (SOC_IS_TD2_TT2(unit)) {
            *object = bcmStatObjectEgrL3Nat;
        }
        break;
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TRIDENT2PLUS_SUPPORT)                 
    case  EFP_POLICY_TABLEm:                          
        if (SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) {
            *object = bcmStatObjectEgrFieldStageEgress;
        } else {                                      
             return BCM_E_INTERNAL;
        }                                             
        break;                                        
    case  EGR_IP_TUNNEL_MPLSm:                        
        if (SOC_IS_TRIDENT2PLUS(unit) || SOC_IS_APACHE(unit)) {
            *object = bcmStatObjectEgrMplsTunnelLabel;
        } else {                                      
             return BCM_E_INTERNAL;                   
        }                                             
        break;                                        
#endif /* BCM_TRIDENT2_SUPPORT */                     

    default:
         return BCM_E_INTERNAL;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_flex_get_counter_id
 * Description:
 *      Get Stat Counter Id for given accounting table and index
 * Parameters
 *      unit                  - (IN) unit number
 *      num_of_tables         - (IN) Number of Accounting Tables
 *      table_info            - (IN) Tables Info(Name,direction and index)
 *      num_stat_counter_ids  - (OUT) Number of Stat Counter Ids
 *      stat_counter_id       - (OUT) Stat Counter Ids
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_counter_id(
            int                        unit,
            uint32                     num_of_tables,
            bcm_stat_flex_table_info_t *table_info,
            uint32                     *num_stat_counter_ids,
            uint32                     *stat_counter_id)
{
    int                        index=0;
    uint32                     offset_mode=0;
    uint32                     pool_number=0;
    uint32                     base_idx=0;
    void                       *entry_data=NULL;
    uint32                     entry_data_size=0;
    bcm_stat_object_t          object=bcmStatObjectIngPort;
    bcm_stat_group_mode_t      group= bcmStatGroupModeSingle;
    uint32                     exact_index=0;

    if ((num_stat_counter_ids == NULL) || (stat_counter_id == NULL)) {
        return BCM_E_PARAM;
    }

    exact_index = table_info[index].index;
    if (table_info[index].table == EGR_IP_TUNNEL_MPLSm) {
        table_info[index].index =
        table_info[index].index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
    }

    for (index=0;index < num_of_tables ;index++) {
        entry_data_size = WORDS2BYTES(BYTES2WORDS(
                       SOC_MEM_INFO(unit,table_info[index].table).bytes));
        entry_data = sal_alloc(entry_data_size,"vrf_table");
        if (entry_data == NULL) {
            return BCM_E_MEMORY;
        }
        sal_memset(entry_data,0,
                SOC_MEM_INFO(unit, table_info[index].table).bytes);
        if (soc_mem_read(unit, table_info[index].table, MEM_BLOCK_ANY,
                        _bcm_esw_stat_flex_table_index_map(unit,
                        table_info[index].table,
                        table_info[index].index), entry_data) == SOC_E_NONE) {
            if (soc_mem_field_valid(unit,table_info[index].table,VALIDf)) {
                if (soc_mem_field32_get(unit,table_info[index].table,
                    entry_data,VALIDf)==0) {
                    sal_free(entry_data);
                    continue;
                }
            }
            _bcm_esw_get_flex_counter_fields_values(
                        unit, exact_index,table_info[index].table,
                        entry_data,0,&offset_mode, &pool_number, &base_idx);
            if ((offset_mode == 0) && (base_idx == 0)) {
                sal_free(entry_data);
                continue;
            }

            if (table_info[index].direction == bcmStatFlexDirectionIngress) {
                if (_bcm_esw_stat_flex_get_ingress_object(
                            unit,table_info[index].table,table_info[index].index,
                            entry_data,&object) != BCM_E_NONE) {
                    sal_free(entry_data);
                    return BCM_E_NONE;
                }
                group=flex_ingress_modes[unit][offset_mode].group_mode;
            } else {
                if (_bcm_esw_stat_flex_get_egress_object(
                        unit,table_info[index].table,table_info[index].index,
                        entry_data,&object) != BCM_E_NONE) {
                    sal_free(entry_data);
                    return BCM_E_NONE;
                }
                group=flex_egress_modes[unit][offset_mode].group_mode;
            }
            sal_free(entry_data);
            _bcm_esw_stat_get_counter_id(
                                        unit, group,object,
                                        offset_mode,pool_number,base_idx,
                                        &stat_counter_id[index]);
            (*num_stat_counter_ids)++;
        }
    }
    if ((*num_stat_counter_ids) == 0) {
         return BCM_E_NOT_FOUND;
    }
    return BCM_E_NONE;
}

#if defined(BCM_TRIDENT2PLUS_SUPPORT) && defined (INCLUDE_L3)
/*
 * Function:
 *      _bcm_esw_stat_flex_get_counter_id
 * Description:
 *      Get Stat Counter Id for given accounting table and index
 * Parameters
 *      unit                  - (IN) unit number
 *      num_of_tables         - (IN) Number of Accounting Tables
 *      table_info            - (IN) Tables Info(Name,direction and index)
 *      num_stat_counter_ids  - (OUT) Number of Stat Counter Ids
 *      stat_counter_id       - (OUT) Stat Counter Ids
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_flex_get_multi_counter_id(
        int                        unit,
        bcm_stat_flex_table_info_t *table_info,
        int                        *num_stat_counter_ids,
        uint32                     *stat_counter_id)
{
    int                        index=0;
    int                        object_index=0;
    int                        counter_index=0;
    uint32                     offset_mode=0;
    uint32                     pool_number=0;
    uint32                     base_idx=0;
    void                       *entry_data=NULL;
    uint32                     entry_data_size=0;
    bcm_stat_group_mode_t      group= bcmStatGroupModeSingle;
    uint32                     exact_index=0;

    if (((*num_stat_counter_ids != -1) && (stat_counter_id == NULL)) ||
        (*num_stat_counter_ids == 0)) {
        return BCM_E_PARAM;
    }
    switch (table_info[index].table) {
        case EGR_IP_TUNNEL_MPLSm:
            exact_index = table_info[index].index;
            table_info[index].index =
                table_info[index].index / _BCM_MPLS_NUM_MPLS_ENTRIES_PER_INDEX(unit);
            entry_data_size = WORDS2BYTES(BYTES2WORDS(
                        SOC_MEM_INFO(unit,table_info[index].table).bytes));
            entry_data = sal_alloc(entry_data_size,"flex-counter-table");
            if (entry_data == NULL) {
                return BCM_E_MEMORY;
            }
            sal_memset(entry_data,0,
                    SOC_MEM_INFO(unit, table_info[index].table).bytes);
            if (soc_mem_read(unit, table_info[index].table, MEM_BLOCK_ANY,
                        _bcm_esw_stat_flex_table_index_map(unit,
                            table_info[index].table,
                            table_info[index].index), entry_data) == SOC_E_NONE) {
                if (soc_mem_field_valid(unit,table_info[index].table,VALIDf)) {
                    if (soc_mem_field32_get(unit,table_info[index].table,
                                entry_data,VALIDf)==0) {
                        sal_free(entry_data);
                        return BCM_E_PARAM;
                    }
                }
            }

            for (object_index=0;object_index < ((sizeof(egr_mpls_tunnel_object)/
                        sizeof(egr_mpls_tunnel_object[0])));object_index++) {
                _bcm_esw_get_flex_counter_fields_values(
                        unit, exact_index,
                        table_info[index].table,
                        entry_data,egr_mpls_tunnel_object[object_index],
                        &offset_mode, &pool_number, &base_idx);
                if ((offset_mode != 0) && (base_idx != 0)) {
                    continue;
                }

                group=flex_egress_modes[unit][offset_mode].group_mode;
                if (*num_stat_counter_ids != -1) {
                    _bcm_esw_stat_get_counter_id(
                            unit, group,egr_mpls_tunnel_object[object_index],
                            offset_mode,pool_number,base_idx,
                            &stat_counter_id[counter_index]);
                }
                counter_index++;
                if (*num_stat_counter_ids != -1 &&
                    *num_stat_counter_ids == counter_index) {
                    break;
                }
            }
            break;
        default:
            LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Invalid Table is passed %d \n"),
                     table_info[index].table));
            sal_free(entry_data);
            return BCM_E_INTERNAL;
    }
    sal_free(entry_data);
    if (*num_stat_counter_ids == -1) {
        *num_stat_counter_ids = counter_index;
    } else if (counter_index != *num_stat_counter_ids) {
        return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}
#endif

/*
 * Function:
 *      _bcm_esw_stat_get_ingress_object
 * Description:
 *      Get Ingress Object available in give table
 * Parameters:
 *      unit                  - (IN) unit number
 *      pool_number           - (IN) Flex Pool number
 *      num_objects           - (OUT) Number of Flex Object found in given pool
 *      object                - (OUT) Flex Objects found in given pool
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *      For debuging purpose only
 *
 */
void _bcm_esw_stat_get_ingress_object(
     int               unit,
     uint32            pool_number,
     uint32            *num_objects,
     bcm_stat_object_t *object)
{
     int index=0;
     uint32            ingress_table_masks[]={
                       FLEX_COUNTER_POOL_USED_BY_PORT_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VFP_POLICY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VLAN_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VFI_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VRF_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_L3_IIF_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_SOURCE_VP_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_L3_TUNNEL_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_L3_ENTRY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EXT_FP_POLICY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_FCOE_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_IFP_POLICY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_AGM_MONITOR_TABLE_FOR_SCND_LKUP,
                       FLEX_COUNTER_POOL_USED_BY_EM_POLICY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_VLAN_XLATE_TABLE_FOR_SCND_LKUP,
                       FLEX_COUNTER_POOL_USED_BY_MPLS_ENTRY_TABLE_FOR_SCND_LKUP
     };
     bcm_stat_object_t ingress_table_objects[]={
                       bcmStatObjectIngPort,
                       bcmStatObjectIngPolicy,
                       bcmStatObjectIngVlan,
                       bcmStatObjectIngVfi,
                       bcmStatObjectIngVrf,
                       bcmStatObjectIngMplsSwitchLabel,
                       bcmStatObjectIngVlanXlate,
                       bcmStatObjectIngL3Intf,
                       bcmStatObjectIngMplsVcLabel,
                       bcmStatObjectIngGport,
                       bcmStatObjectIngMplsFrrLabel,
                       bcmStatObjectIngL3Host,
                       bcmStatObjectIngEXTPolicy,
                       bcmStatObjectIngIpmc,
                       bcmStatObjectIngFieldStageIngress,
                       bcmIntStatObjectIngAgm,
                       bcmIntStatObjectIngAgmSecondLookup,
                       bcmStatObjectIngExactMatch,
                       bcmStatObjectIngVlanXlateSecondLookup,
                       bcmStatObjectIngMplsSwitchSecondLabel
     };
     *num_objects=0;
     for(index=0;
         index<sizeof(ingress_table_objects)/sizeof(ingress_table_objects[0]);
         index++) {
         if (flex_pool_stat[unit][bcmStatFlexDirectionIngress][0][pool_number].
                           used_by_tables & ingress_table_masks[index]) {
             object[*num_objects]=ingress_table_objects[index];
             (*num_objects)++;
         }
     }
}
/*
 * Function:
 *      _bcm_esw_stat_get_egress_object
 * Description:
 *      Get Egress Object available in give table
 * Parameters:
 *      unit                  - (IN) unit number
 *      pool_number           - (IN) Flex Pool number
 *      num_objects           - (OUT) Number of Flex Object found in given pool
 *      object                - (OUT) Flex Objects found in given pool
 *
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *      For debuging purpose only
 *
 */
void _bcm_esw_stat_get_egress_object(
     int               unit,
     uint32            pool_number,
     uint32            *num_objects,
     bcm_stat_object_t *object)
{
     int index=0;
     uint32            egress_table_masks[]={
                       FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_VFI_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_L3_NEXT_HOP_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_VLAN_XLATE_TABLE_FOR_SCND_LKUP,
                       FLEX_COUNTER_POOL_USED_BY_EGR_PORT_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_DVP_ATTRIBUTE_1_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_L3_NAT_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_EFP_POLICY_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_TABLE,
                       FLEX_COUNTER_POOL_USED_BY_EGR_IP_TUNNEL_MPLS_SCND_LBL_TABLE
     };
     bcm_stat_object_t egress_table_objects[]={
                       bcmStatObjectEgrVlan,
                       bcmStatObjectEgrVfi,
                       bcmStatObjectEgrL3Intf,
                       bcmStatObjectEgrVlanXlate,
                       bcmStatObjectEgrVlanXlateSecondLookup,
                       bcmStatObjectEgrPort,
                       bcmStatObjectEgrL2Gre,
                       bcmStatObjectEgrVxlan,
                       bcmStatObjectEgrL3Nat,         
                       bcmStatObjectEgrFieldStageEgress,
                       bcmStatObjectEgrMplsTunnelLabel,
                       bcmStatObjectEgrMplsTunnelSecondLabel
     };
     *num_objects=0;
     for(index=0;
         index<sizeof(egress_table_objects)/sizeof(egress_table_objects[0]);
         index++) {
         if (flex_pool_stat[unit][bcmStatFlexDirectionEgress][0][pool_number].
                           used_by_tables & egress_table_masks[index]) {
             object[*num_objects]=egress_table_objects[index];
             (*num_objects)++;
         }
     }
}
/*
 * Function:
 *      _bcm_esw_stat_validate_group
 * Description:
 *      Validates passed stat group
 * Parameters:
 *      unit                  - (IN) unit number
 *      group                 - (IN) Flex Group Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_validate_group(
            int                   unit,
            bcm_stat_group_mode_t group)
{
    if(!((group >= bcmStatGroupModeSingle) &&
         (group <= (bcm_stat_group_mode_t) bcmStatGroupModeFlex4))) {
          LOG_ERROR(BSL_LS_BCM_FLEXCTR,
                    (BSL_META_U(unit,
                                "Invalid bcm_stat_group_mode_t passed %d \n"),group));
          return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}
/*
 * Function:
 *      _bcm_esw_stat_total_counters_get
 * Description:
 *      Fetches total counters that have been created with the
 *      given stat counter id
 * Parameters:
 *      unit                  - (IN)  unit number
 *      stat_counter_id       - (IN)  Stat Counter Id
 *      total_counters        - (OUT) Total Counters
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_total_counters_get(
            int    unit,
            uint32 stat_counter_id,
            uint32 *total_counters)
{
    bcm_stat_flex_direction_t    direction = bcmStatFlexDirectionIngress;
    uint32                       mode=0;
    uint32                       base_index=0;
    uint32                       pool_number=0;
    bcm_error_t                  rv=BCM_E_NONE;
    bcm_stat_group_mode_t        group_mode= bcmStatGroupModeSingle;
    bcm_stat_object_t            object=bcmStatObjectIngPort;
    bcm_stat_flex_ingress_mode_t *flex_ingress_mode=NULL;
    bcm_stat_flex_egress_mode_t  *flex_egress_mode=NULL;

    _bcm_esw_stat_get_counter_id_info(unit, stat_counter_id,&group_mode,&object,
                                      &mode,&pool_number,&base_index);
    rv = _bcm_esw_stat_validate_object(unit,object,&direction);

    if (rv != BCM_E_NONE) {
        return rv;
    }
    rv = _bcm_esw_stat_validate_group(unit,group_mode);
    if (rv != BCM_E_NONE) {
        return rv;
    }
    if (direction == bcmStatFlexDirectionIngress) {
        flex_ingress_mode = sal_alloc(sizeof(bcm_stat_flex_ingress_mode_t),
                                      "flex_ingress_mod");
        if (flex_ingress_mode == NULL) {
            return BCM_E_MEMORY;
        }

        sal_memset(flex_ingress_mode, 0, sizeof(bcm_stat_flex_ingress_mode_t));

        rv = _bcm_esw_stat_flex_get_ingress_mode_info(
                  unit,mode,flex_ingress_mode);
        if (BCM_SUCCESS(rv)) {
            *total_counters = flex_ingress_mode->total_counters;
        }
        sal_free(flex_ingress_mode);
        BCM_IF_ERROR_RETURN(rv);
     } else {
        flex_egress_mode = sal_alloc(sizeof(bcm_stat_flex_egress_mode_t),
                                      "flex_egress_mod");
        if (flex_egress_mode == NULL) {
            return BCM_E_MEMORY;
        }

        sal_memset(flex_egress_mode, 0, sizeof(bcm_stat_flex_egress_mode_t));

        rv = _bcm_esw_stat_flex_get_egress_mode_info(
                  unit,mode,flex_egress_mode);
        if (BCM_SUCCESS(rv)) {
            *total_counters = flex_egress_mode->total_counters;
        }
        sal_free(flex_egress_mode);
        BCM_IF_ERROR_RETURN(rv);
     }
     return rv;
}
/*
 * Function:
 *      _bcm_esw_stat_validate_object
 * Description:
 *      Validates passed stat object and returns corresponding object's
 *      direction
 * Parameters:
 *      unit                  - (IN) unit number
 *      object                - (IN) Flex Accounting object
 *      direction             - (OUT) Flex Object's direction
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t _bcm_esw_stat_validate_object(
            int                       unit,
            bcm_stat_object_t         object,
            bcm_stat_flex_direction_t *direction)
{
    if ((object >= bcmStatObjectIngPort) &&
        (object <= bcmStatObjectIngMplsSwitchLabel)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)                 
    if (SOC_IS_TRIDENT2PLUS(unit)  &&                 
        (object == bcmStatObjectIngMplsFrrLabel)) {   
        return BCM_E_PARAM;                           
    }                                                 
#endif /* BCM_TRIDENT2PLUS_SUPPORT */
#if defined(BCM_KATANA2_SUPPORT)
    if (SOC_IS_KATANA2(unit) && (object == bcmStatObjectIngMplsFrrLabel)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif
#if defined(BCM_TRIUMPH3_SUPPORT)
    if ((SOC_IS_TRIUMPH3(unit) || SOC_IS_TD2_TT2(unit)) &&
        (object >= bcmStatObjectIngMplsFrrLabel) &&
        (object <= bcmStatObjectIngEXTPolicy)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) &&
        (object >= bcmStatObjectIngVxlan) &&
        (object <= bcmStatObjectIngL3Route)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif /* BCM_TRIDENT2_SUPPORT */
    /* additional valid ingress objects check */
	if (object == bcmStatObjectIngNiv) {
	    *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
	}

#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) &&
        (object == bcmStatObjectIngIpmc)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif /* BCM_TRIDENT2_SUPPORT */

#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    if ((SOC_IS_TOMAHAWKX(unit) || SOC_IS_TD2P_TT2P(unit) ||
         SOC_IS_APACHE(unit))&&
        (bcmStatObjectIngFieldStageIngress == object)) {
        *direction = bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif

#if defined(BCM_TOMAHAWK_SUPPORT)
    if ((SOC_IS_TOMAHAWKX(unit)) && (object == (bcm_stat_object_t)bcmIntStatObjectIngAgm)) {
        *direction = bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif

#if defined(BCM_TOMAHAWK_SUPPORT)
    if ((SOC_IS_TOMAHAWKX(unit)) &&
        (object == (bcm_stat_object_t)bcmIntStatObjectIngAgmSecondLookup)) {
        *direction = bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif

#if defined(BCM_TOMAHAWK_SUPPORT)
    if ((SOC_IS_TOMAHAWKX(unit)) && (object == (bcm_stat_object_t)bcmStatObjectIngExactMatch)) {
        *direction = bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif

#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) &&
        (object == bcmStatObjectIngVxlanDip)) {
        *direction=bcmStatFlexDirectionIngress;
        return BCM_E_NONE;
    }
#endif /* BCM_TRIDENT2_SUPPORT */

    if ((object >= bcmStatObjectEgrPort) &&
        (object <= bcmStatObjectEgrL3Intf)) {
        *direction=bcmStatFlexDirectionEgress;
        return BCM_E_NONE;
    }
#if defined(BCM_TRIUMPH3_SUPPORT)
    if ((SOC_IS_TRIUMPH3(unit) || SOC_IS_TD2_TT2(unit)) &&
        (object >= bcmStatObjectEgrWlan) &&
        (object <= bcmStatObjectEgrL2Gre)) {
        *direction=bcmStatFlexDirectionEgress;
        return BCM_E_NONE;
    }
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
    if (SOC_IS_TD2_TT2(unit) &&
        (object >= bcmStatObjectEgrL2Gre) &&
        (object <= bcmStatObjectEgrL3Nat)) {
        *direction=bcmStatFlexDirectionEgress;
        return BCM_E_NONE;
    }
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TRIDENT2PLUS_SUPPORT)                 
    if ((SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) &&
        ((object == bcmStatObjectEgrFieldStageEgress) ||
        (object == bcmStatObjectEgrMplsTunnelLabel) ||
        (object == bcmStatObjectEgrMplsTunnelSecondLabel))) {
        *direction=bcmStatFlexDirectionEgress;        
        return BCM_E_NONE;                            
    }                                                 
#endif /* BCM_TRIDENT2PLUS_SUPPORT */

	/* additional valid egress objects check */
	if (object == bcmStatObjectEgrNiv) {
	    *direction=bcmStatFlexDirectionEgress;
	    return BCM_E_NONE;
	}
   
        if (object == bcmStatObjectIngGport) {
            *direction= bcmStatFlexDirectionIngress;
            return BCM_E_NONE;
        }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        if ((soc_feature(unit,soc_feature_ing_vlan_xlate_second_lookup)) &&
            (object == bcmStatObjectIngVlanXlateSecondLookup)) {
            *direction= bcmStatFlexDirectionIngress;
            return BCM_E_NONE;
        }
#endif
#if defined(BCM_APACHE_SUPPORT)
        if ((soc_feature(unit, soc_feature_egr_vlan_xlate_second_lookup)) &&
            (object == bcmStatObjectEgrVlanXlateSecondLookup)) {
            *direction= bcmStatFlexDirectionEgress;
            return BCM_E_NONE;
        }
        if ((soc_feature(unit,soc_feature_mpls_entry_second_label_lookup)) &&
            (object == bcmStatObjectIngMplsSwitchSecondLabel)) {
            *direction= bcmStatFlexDirectionIngress;
            return BCM_E_NONE;
        }
#endif
    LOG_ERROR(BSL_LS_BCM_FLEXCTR,
              (BSL_META_U(unit,
                          "Invalid bcm_stat_object_t passed %d \n"),object));
    return BCM_E_PARAM;
}

/*
 * Function:
 *	_bcm_esw_stat_flex_pipe_pool_info_multi_get
 * Description:
 *	Retrieves the flex counter details for a given direction and pipe
 * Parameters:
 *
 *  unit             : (IN)  UNIT number
 *  pipe_num        : (IN)  pipe number
 *  direction        : (IN)  Direction (Ingress/egress)
 *  num_pools        : (IN)  Passing a 0, then actual_num_pools will return the number of pools that are available.
 *                         return the number of pools that are available.
 *                         Passing n will return up to n pools in the return array
 *  flex_pool_stat   : (INOUT)  array that provides the pool info
 *  actual_num_pools : (OUT)  Returns actual no of pools available for a given direction
 *
 * Returns:
 *	BCM_E_*
 *
 */
int _bcm_esw_stat_flex_pipe_pool_info_multi_get(
    int unit,
    bcm_stat_flex_direction_t direction,
    uint32 pipe_num,
    uint32 num_pools,
    bcm_stat_flex_pool_stat_info_t *flex_pool_stat_info,
    uint32 *actual_num_pools)
{
    uint32  num_pools_arr[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32  size_pool[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32  poolIndx = 0;
    uint32  lIndx = 0;
    uint32 num_ctr = 0;

    /* Coverity :: 21295 */
    if((direction != bcmStatFlexDirectionEgress) &&
	   (direction != bcmStatFlexDirectionIngress) ) {
        return BCM_E_PARAM;
    }

    if( pipe_num >= NUM_PIPE(unit)) {
        return BCM_E_PARAM;
    }

    num_pools_arr[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                                 num_flex_ingress_pools;
    num_pools_arr[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                                 num_flex_egress_pools;
    size_pool[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             size_flex_egress_pool;

    /*chk the value of num_pools*/
    if(num_pools == 0) {
    /*return the actual_num_of pools available for the given direction */
 	    *actual_num_pools = num_pools_arr[direction];
    }  else { /*fill the structure and send it back */
        if(num_pools > num_pools_arr[direction] ) {
	        return BCM_E_PARAM;
	    }
	    for(poolIndx = 0; poolIndx < num_pools; poolIndx++)
	    {
	        flex_pool_stat_info[poolIndx].pool_id = poolIndx;

            /* use the least one of the configured pool size and mem table */
            num_ctr = soc_mem_index_count(unit,
                             _ctr_counter_table[direction][poolIndx]);
            if (size_pool[direction] < num_ctr) {
                num_ctr = size_pool[direction];
            }

	        flex_pool_stat_info[poolIndx].total_entries = num_ctr;
	        flex_pool_stat_info[poolIndx].used_entries = flex_pool_stat[unit]
                                                         [direction][pipe_num][poolIndx].
                                                         used_entries;
	        flex_pool_stat_info[poolIndx].attached_entries=flex_pool_stat[unit]
                                                           [direction][pipe_num]
                                                           [poolIndx].
                                                           attached_entries;
	        flex_pool_stat_info[poolIndx].
                free_entries = flex_pool_stat_info[poolIndx].total_entries -
                               flex_pool_stat_info[poolIndx].used_entries;
            for(lIndx = 0 ; lIndx < _SHR_BITDCLSIZE(bcmStatObjectMaxValue);
                lIndx++)
            {
                
                flex_pool_stat_info[poolIndx].
                    used_by_objects[lIndx]     =  flex_pool_stat[unit]
                                                  [direction][pipe_num][poolIndx].
                                                  used_by_objects[lIndx];
            }
	    }
    } /* end of if(num_pools == 0) */
    return BCM_E_NONE;
}

/*
 * Function:
 *	_bcm_esw_stat_flex_pool_info_multi_get
 * Description:
 *	Retrieves the flex counter details for a given direction
 * Parameters:
 *
 *  unit             : (IN)  UNIT number
 *  direction        : (IN)  Direction (Ingress/egress)
 *  num_pools        : (IN)  Passing a 0, then actual_num_pools will return the number of pools that are available.
 *                         return the number of pools that are available.
 *                         Passing n will return up to n pools in the return array
 *  flex_pool_stat   : (INOUT)  array that provides the pool info
 *  actual_num_pools : (OUT)  Returns actual no of pools available for a given direction
 *
 * Returns:
 *	BCM_E_*
 *
 */
int _bcm_esw_stat_flex_pool_info_multi_get(
    int unit,
    bcm_stat_flex_direction_t direction,
    uint32 num_pools,
    bcm_stat_flex_pool_stat_info_t *flex_pool_stat_info,
    uint32 *actual_num_pools)
{
    return _bcm_esw_stat_flex_pipe_pool_info_multi_get(unit, direction, 0, num_pools,
                flex_pool_stat_info, actual_num_pools);
}

/*
 * Function:
 *      _bcm_esw_stat_object_pool_num_get
 * Description:
 *      Get counter pool Index from Flex counter Ingress Stat Object.
 * Parameters:
 *      unit            - (IN) unit number
 *      object          - (IN) stat object
 *      pool_num        - (OUT) Counter pool used.
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t
_bcm_esw_stat_object_pool_num_get(
    int unit,
    bcm_stat_object_t object,
    uint32 *pool_num)
{
    uint32   index;
    uint32   pool_number;
    uint32   pool_group_id;
    uint32   num_objects = 0;
    bcm_stat_flex_direction_t direction;
    uint32                    num_pools;
    bcm_stat_object_t         object_list[
                                 BCM_STAT_FLEX_COUNTER_MAX_INGRESS_TABLE];

    /* Parameter validation checks */
    if (!SOC_UNIT_VALID(unit)) {
        return BCM_E_UNIT;
    }

    if ((NULL == pool_num)){
        return BCM_E_PARAM;
    }

    /* Validate object */
    BCM_IF_ERROR_RETURN(_bcm_esw_stat_validate_object(unit, 
                                                      object,
                                                      &direction));

    if (soc_feature(unit, soc_feature_use_flex_ctr_oam_lm)) {
        if (direction == bcmStatFlexDirectionIngress) {
            num_pools = SOC_INFO(unit).num_flex_ingress_pools;
            for (pool_number = 0; pool_number < num_pools; pool_number++) {
                _bcm_esw_stat_get_ingress_object(unit, pool_number, &num_objects,
                                                 &object_list[0]);
                for (index = 0; index < num_objects; index++) {
                    if (object_list[index] == object) {
                        /* In TD2Plus, for OAM counters one group object can be in
                         * one pool.
                         */
                        pool_group_id = pool_number /
                            BCM_STAT_FLEX_COUNTER_POOLS_PER_GROUP;
                        if (flex_pool_group[unit][0][pool_group_id].group_type ==
                            BCM_FLEX_POOL_GROUP_USED_BY_OAM) {
                            *pool_num = pool_number;
                            return BCM_E_NONE;
                        }
                    }
                }
            }
        } else {
            num_pools = SOC_INFO(unit).num_flex_egress_pools;
            for (pool_number = 0; pool_number < num_pools; pool_number++) {
                _bcm_esw_stat_get_egress_object(unit, pool_number, &num_objects,
                                                 &object_list[0]);
                for (index = 0; index < num_objects; index++) {
                    if (object_list[index] == object) {
                        /* In TD2Plus, for OAM counters one group object can be in
                         * one pool.
                         */
                            *pool_num = pool_number;
                            return BCM_E_NONE;
                    }
                }
            }
        }
    }

    return BCM_E_NOT_FOUND;
}

bcm_error_t
_bcm_flex_ctr_field_stat_types_get(int unit, uint32 flex_sid, 
                                   bcm_stat_group_mode_attr_selector_t *attr_selectors,
                                   uint32                              *num_selectors)
{
    uint32                    pool_number=0;
    uint32                    base_index=0;
    bcm_stat_flex_mode_t      offset_mode=0;
    bcm_stat_object_t         object_l=bcmStatObjectIngPort;
    bcm_stat_group_mode_t     group_l= bcmStatGroupModeSingle;
    int i;
    bcm_error_t status = BCM_E_NONE;

    _bcm_esw_stat_get_counter_id_info(
                  unit, flex_sid,&group_l,&object_l,
                  &offset_mode,&pool_number,&base_index);

    if ((offset_mode < BCM_STAT_FLEX_CUSTOM_INGRESS_COUNTER_MAX_MODE) &&
       (offset_mode >= BCM_CUSTOM_INGRESS_MODE_START)) {
       offset_mode -= BCM_CUSTOM_INGRESS_MODE_START;
       if (flex_custom_ingress_modes[unit][offset_mode].attr_selectors) {
           *num_selectors = flex_custom_ingress_modes[unit][offset_mode].num_selectors;
           for(i = 0; i < *num_selectors; ++i ) {
               attr_selectors[i] = flex_custom_ingress_modes[unit][offset_mode].attr_selectors[i];
           }
           return status;
       }
       offset_mode = flex_custom_ingress_modes[unit][offset_mode].offset_mode;
    }
 
    *num_selectors = flex_ingress_modes[unit][offset_mode].num_selectors;
    for(i = 0; i < *num_selectors; ++i ) {
        attr_selectors[i] = flex_ingress_modes[unit][offset_mode].attr_selectors[i];
    }
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_id_get_all
 * Description:
 *      Get all stat ids attached to the stat object.
 * Parameters:
 *      unit            - (IN) unit number
 *      object          - (IN) stat object
 *      stat_max        - (IN) max stat id count
 *      stat_array      - (OUT) array of stat ids
 *      stat_count      - (OUT) actual stat id count
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
bcm_error_t
_bcm_esw_stat_id_get_all(
    int unit,
    bcm_stat_object_t object,
    int stat_max,
    uint32 *stat_array,
    int *stat_count)
{
    uint32   index, array_index = 0;
    uint32   pool_number, base_index;
    uint32   num_objects = 0;
    uint32   stat_counter_id = 0;
    bcm_stat_flex_mode_t    offset_mode = 0;
    bcm_stat_object_t       object_l;
    bcm_stat_group_mode_t   group_l;
    bcm_stat_flex_direction_t direction;
    uint32                    num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32                    size_pool[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    bcm_stat_object_t         object_list[
                                 BCM_STAT_FLEX_COUNTER_MAX_EGRESS_TABLE +
                                 BCM_STAT_FLEX_COUNTER_MAX_INGRESS_TABLE];

    /* Parameter validation checks */
    if (!SOC_UNIT_VALID(unit)) {
        return BCM_E_UNIT;
    }

    if ((NULL == stat_count) || (stat_max < 0) ){
        return BCM_E_PARAM;
    }

    /* Validate object */
    BCM_IF_ERROR_RETURN(_bcm_esw_stat_validate_object(unit, object, &direction));

    /*
     * Loop through ingress and egress counter pools to get base/group index,
     * to retrieve all attached stat objects.
     */
    num_pools[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             num_flex_egress_pools;
    size_pool[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             size_flex_egress_pool;

    /* Loop all ingress and Egress Pools */
    for (direction = 0; direction < BCM_STAT_FLEX_COUNTER_MAX_DIRECTION;
         direction++ )
    {
        /* Loop all offset/Group Modes */
        for (offset_mode = 0; offset_mode < BCM_STAT_FLEX_COUNTER_MAX_MODE;
              offset_mode++)
        {
            if (direction == bcmStatFlexDirectionIngress) {
                if (flex_ingress_modes[unit][offset_mode].available == 0) {
                      continue;
                }
                group_l=flex_ingress_modes[unit][offset_mode].group_mode;
            } else {
                if (flex_egress_modes[unit][offset_mode].available == 0 ) {
                      continue;
                }
                group_l=flex_egress_modes[unit][offset_mode].group_mode;
            }
            for (pool_number = 0; pool_number < num_pools[direction];
                 pool_number++)
            {
                for (base_index = 0; base_index < size_pool[direction];
                     base_index++)
                {
                    if (flex_base_index_reference_count[unit][direction][0]
                                        [pool_number][base_index] == 0) {
                        continue;
                    }
                    /* Get All Attached objects */
                    if(direction == bcmStatFlexDirectionIngress) {
                       _bcm_esw_stat_get_ingress_object(
                                unit,
                                pool_number,
                                &num_objects,
                                &object_list[0]);
                    } else { /* Egress direction */
                       _bcm_esw_stat_get_egress_object(
                                unit,
                                pool_number,
                                &num_objects,
                                &object_list[0]);
                    }
                    /*
                     * Loop through for the given object to get the
                     * attached stat counter ids
                     */
                    for(index = 0; index < num_objects; index++)
                    {
                        object_l = object_list[index];
                        if (object_l != object) {
                            continue;
                        }
                        _bcm_esw_stat_counter_id_retrieve(
                                    unit, group_l, object_l, offset_mode,
                                    pool_number, base_index,
                                    &stat_counter_id);
                        if (stat_counter_id == 0) {
                            continue;
                        }
                        if ((stat_max > 0) && (NULL != stat_array)) {
                            /* Assign the attached stat_id for the object */
                            stat_array[array_index] = stat_counter_id;
                            array_index++;
                            stat_max--;
                        }
                    }
                } /* Pool counter base */
            } /* Pools */
        }   /* Offset/Group modes */
    } /* Pool directions */
    *stat_count = array_index;
    return BCM_E_NONE;
}

/*
 * Function:
 *      _bcm_esw_stat_group_dump_info
 * Description:
 *      Dump Useful Info about configured group
 * Parameters:
 *      unit                  - (IN) unit number
 *      all_flag              - (IN) If 1, object and group_mode are ignored
 *      object                - (IN) Flex Accounting object
 *      group                 - (IN) Flex Group Mode
 * Return Value:
 *      BCM_E_XXX
 * Notes:
 *
 */
void _bcm_esw_stat_group_dump_info(
     int                   unit,
     int                   all_flag,
     bcm_stat_object_t     object,
     bcm_stat_group_mode_t group)
{
    uint32                    num_objects=0;
    bcm_stat_object_t         ing_object_list[
                                 BCM_STAT_FLEX_COUNTER_MAX_INGRESS_TABLE];
    bcm_stat_object_t         egr_object_list[
                                 BCM_STAT_FLEX_COUNTER_MAX_EGRESS_TABLE];
    uint32                    index=0;
    uint32                    stat_counter_id=0;
    uint32                    pool_number=0;
    uint32                    base_index=0;
    bcm_stat_flex_mode_t      offset_mode=0;
    bcm_stat_object_t         object_l=bcmStatObjectIngPort;
    bcm_stat_group_mode_t     group_l= bcmStatGroupModeSingle;
    bcm_stat_flex_direction_t direction=bcmStatFlexDirectionIngress;
    uint32                    total_counters=0;
    uint32                    num_pools[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    uint32                    size_pool[BCM_STAT_FLEX_COUNTER_MAX_DIRECTION];
    bcm_stat_flex_ing_attr_t  *ing_attr=NULL;
    bcm_stat_flex_egr_attr_t  *egr_attr=NULL;
    uint32                    attr_index=0;
    uint32                    total_attrs=0;
    uint8                     attr_value=0;

    if (!soc_feature(unit,soc_feature_advanced_flex_counter)) {
        LOG_CLI((BSL_META_U(unit,
                            "Not Available ...\n")));
        return;
    }

    LOG_CLI((BSL_META_U(unit,
                        "Not attached(MAX=%d) Stat counter Id info \n"),
             BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE));
    for (index=0;index<BCM_STAT_FLEX_COUNTER_MAX_SCACHE_SIZE;index++) {
         if (local_scache_ptr[unit][index] != 0) {
             stat_counter_id = local_scache_ptr[unit][index];
             _bcm_esw_stat_get_counter_id_info(
                           unit, stat_counter_id,&group_l,&object_l,
                           &offset_mode,&pool_number,&base_index);
             if (_bcm_esw_stat_validate_object(unit,object_l,&direction)
                 != BCM_E_NONE) {
                 LOG_CLI((BSL_META_U(unit,
                                     "\tInvalid object %d so skipping it \n"),object_l));
                 continue;
             }
             if (direction == bcmStatFlexDirectionIngress) {
                  total_counters = flex_ingress_modes[unit][offset_mode].
                                   total_counters;
             } else {
                  total_counters = flex_egress_modes[unit][offset_mode].
                                  total_counters;
             }
             if ((all_flag == TRUE) ||
                 ((object_l == object) &&
                  (group_l == group))) {
                   LOG_CLI((BSL_META_U(unit,
                                       "\tstat_counter_id = %d=0x%x \n"),
                            stat_counter_id,stat_counter_id));
                   LOG_CLI((BSL_META_U(unit,
                                       "\t\tDirection:%s mode:%d group_mode:%s"
                                       "\n\t\tpool:%d object:%s base:%d"
                                       " total_counters=%d\n"),
                            flex_directions[direction],offset_mode,
                            flex_group_modes[group_l],
                            pool_number,flex_objects[object_l],
                            base_index,total_counters));
             }
         }
    }
    LOG_CLI((BSL_META_U(unit,
                        "Atached Stat counter Id info \n")));
    num_pools[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             num_flex_ingress_pools;
    num_pools[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             num_flex_egress_pools;
    size_pool[bcmStatFlexDirectionIngress] = SOC_INFO(unit).
                                             size_flex_ingress_pool;
    size_pool[bcmStatFlexDirectionEgress]  = SOC_INFO(unit).
                                             size_flex_egress_pool;

    for (direction=0;
         direction<BCM_STAT_FLEX_COUNTER_MAX_DIRECTION;
         direction++) {
         for (offset_mode=0;
              offset_mode<BCM_STAT_FLEX_COUNTER_MAX_MODE;
              offset_mode++) {
              LOG_CLI((BSL_META_U(unit,
                                  "=============================================\n")));
              if (direction == bcmStatFlexDirectionIngress) {
                  if (flex_ingress_modes[unit][offset_mode].available == 0) {
                      continue;
                  }
                  total_counters=flex_ingress_modes[unit][offset_mode].
                                                   total_counters;
                  group_l=flex_ingress_modes[unit][offset_mode].group_mode;
                  ing_attr = &flex_ingress_modes[unit][offset_mode].ing_attr;
                  switch(ing_attr->packet_attr_type) {
                  case bcmStatFlexPacketAttrTypeUncompressed:
                       LOG_CLI((BSL_META_U(unit,
                                           "IngressPacketAttributMode:Uncompressed\n")));
                       LOG_CLI((BSL_META_U(unit,
                                           "Attr_bits_selector:%x \n"),
                                ing_attr->uncmprsd_attr_selectors.
                                uncmprsd_attr_bits_selector));
                       total_attrs = sizeof(ing_uncmprsd_attr_bits_selector)/
                                     sizeof(ing_uncmprsd_attr_bits_selector[0]);
                       for(attr_index=0; attr_index<total_attrs; attr_index++) {
                           if(ing_attr->uncmprsd_attr_selectors.
                              uncmprsd_attr_bits_selector &
                              ing_uncmprsd_attr_bits_selector[attr_index].
                              attr_bits)
                              LOG_CLI((BSL_META_U(unit,
                                                  "-->%s bit used\n"),
                                       ing_uncmprsd_attr_bits_selector[attr_index].
                                       attr_name));
                       }
                       break;
                  case bcmStatFlexPacketAttrTypeCompressed:
                       LOG_CLI((BSL_META_U(unit,
                                           "IngressPacket Attribut Mode:Compressed\n")));
                       total_attrs=sizeof(_bcm_esw_get_ing_func)/
                                   sizeof(_bcm_esw_get_ing_func[0]);
                       for(attr_index=0;attr_index<total_attrs;attr_index++) {
                          if((attr_value=_bcm_esw_get_ing_func[attr_index].func(
                             &(ing_attr->cmprsd_attr_selectors.
                               pkt_attr_bits)))){
                             LOG_CLI((BSL_META_U(unit,
                                                 "-->%s:%d\n"),
                                      _bcm_esw_get_ing_func[attr_index].func_desc,
                                      attr_value));
                          }
                       }
                       break;
                  default:
                       LOG_CLI((BSL_META_U(unit,
                                           "Not Implemented yet")));
                  }
              } else {
                  if (flex_egress_modes[unit][offset_mode].available == 0 ) {
                      continue;
                  }
                  egr_attr = &flex_egress_modes[unit][offset_mode].egr_attr;
                  switch(egr_attr->packet_attr_type) {
                  case bcmStatFlexPacketAttrTypeUncompressed:
                       LOG_CLI((BSL_META_U(unit,
                                           "EgressPacketAttributMode:Uncompressed\n")));
                       LOG_CLI((BSL_META_U(unit,
                                           "Attr_bits_selector:%x \n"),
                                egr_attr->uncmprsd_attr_selectors.
                                uncmprsd_attr_bits_selector));
                       total_attrs = sizeof(egr_uncmprsd_attr_bits_selector)/
                                     sizeof(egr_uncmprsd_attr_bits_selector[0]);
                       for(attr_index=0; attr_index<total_attrs; attr_index++) {
                           if(egr_attr->uncmprsd_attr_selectors.
                              uncmprsd_attr_bits_selector &
                              egr_uncmprsd_attr_bits_selector[attr_index].
                              attr_bits)
                              LOG_CLI((BSL_META_U(unit,
                                                  "-->%s bit used\n"),
                                       egr_uncmprsd_attr_bits_selector[attr_index].
                                       attr_name));
                       }
                       break;
                  case bcmStatFlexPacketAttrTypeCompressed:
                       LOG_CLI((BSL_META_U(unit,
                                           "EgrressPacket Attribut Mode:Compressed\n")));
                       total_attrs=sizeof(_bcm_esw_get_egr_func)/
                                   sizeof(_bcm_esw_get_egr_func[0]);
                       for(attr_index=0;attr_index<total_attrs;attr_index++) {
                          if((attr_value=_bcm_esw_get_egr_func[attr_index].func(
                             &(egr_attr->cmprsd_attr_selectors.
                               pkt_attr_bits)))){
                              LOG_CLI((BSL_META_U(unit,
                                                  "-->%s:%d\n"),
                                       _bcm_esw_get_egr_func[attr_index].func_desc,
                                       attr_value));
                          }
                       }
                       break;
                  default:
                       LOG_CLI((BSL_META_U(unit,
                                           "Not Implemented yet")));
                  }
                  total_counters=flex_egress_modes[unit][offset_mode].
                                                  total_counters;
                  group_l=flex_egress_modes[unit][offset_mode].group_mode;
              }
              LOG_CLI((BSL_META_U(unit,
                                  "-->Direction:%s offset mode=%d\n"
                                  "-->group_mode:%s total_counters=%d \n"),
                       flex_directions[direction], offset_mode,
                       flex_group_modes[group_l], total_counters));
              for (pool_number=0;
                   pool_number<num_pools[direction];
                   pool_number++) {
                   for (base_index=0;
                        base_index<size_pool[direction];
                        base_index++) {
                        if (flex_base_index_reference_count[unit][direction]
                                            [pool_number][base_index] != 0) {
                            if(direction==bcmStatFlexDirectionIngress) {
                               _bcm_esw_stat_get_ingress_object(
                                        unit,
                                        pool_number,
                                        &num_objects,
                                        &ing_object_list[0]);
                              for(index=0;index<num_objects;index++) {
                                  object_l=ing_object_list[index];
                                  _bcm_esw_stat_get_counter_id(
                                                unit, group_l,object_l,offset_mode,
                                                pool_number,base_index,
                                                &stat_counter_id);
                                  if (num_objects != 1) {
                                      LOG_CLI((BSL_META_U(unit,
                                                          "-->Probable..")));
                                  }
                                  if ((all_flag == TRUE) ||
                                      ((object_l == object) &&
                                       (group_l == group))) {
                                        LOG_CLI((BSL_META_U(unit,
                                                            "\tstat counter id %d (0x%x) "
                                                            "object=%s base index=%d (0x%x)\n"),
                                                 stat_counter_id,stat_counter_id,
                                                 flex_objects[object_l],
                                                 base_index,base_index));
                                  }
                              }
                        } else {
                            _bcm_esw_stat_get_egress_object(
                                     unit, pool_number,
                                     &num_objects,&egr_object_list[0]);
                            for (index=0; index<num_objects; index++) {
                                 object_l = egr_object_list[index];
                                 _bcm_esw_stat_get_counter_id(
                                                unit, group_l,object_l,offset_mode,
                                                pool_number,base_index,
                                                &stat_counter_id);
                                 if (num_objects != 1) {
                                     LOG_CLI((BSL_META_U(unit,
                                                         "Probable..")));
                                 }
                                 if ((all_flag == TRUE) ||
                                     ((object_l == object) &&
                                      (group_l == group))) {
                                       LOG_CLI((BSL_META_U(unit,
                                                           "\tstat counter id %d (0x%x) "
                                                           "object=%s base index=%d (0x%x)\n"),
                                                stat_counter_id,stat_counter_id,
                                                flex_objects[object_l],
                                                base_index,base_index));
                                 }
                            }
                        }
                   }
                }
              }
              LOG_CLI((BSL_META_U(unit,
                                  "=============================================\n")));
         }
    }
}


