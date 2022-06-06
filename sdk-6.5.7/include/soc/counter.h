/*
 * $Id: counter.h,v 1.53 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        counter.h
 * Purpose:     Defines map of counter register addresses, as well
 *              as structures and routines for driver management of
 *              packet statistics counters.
 */

#ifndef _SOC_COUNTER_H
#define _SOC_COUNTER_H

#include <soc/defs.h>
#include <soc/types.h>
#include <sal/types.h>
#include <soc/sbusdma.h>

#ifdef BCM_IPROC_SUPPORT
#define SOC_REG_IS_COUNTER(unit, socreg) \
        ((SOC_REG_INFO(unit, socreg).flags & SOC_REG_FLAG_COUNTER) && \
         (SOC_REG_INFO(unit, socreg).regtype != soc_cpureg) && \
         (SOC_REG_INFO(unit, socreg).regtype != soc_iprocreg))
#else
#define SOC_REG_IS_COUNTER(unit, socreg) \
        ((SOC_REG_INFO(unit, socreg).flags & SOC_REG_FLAG_COUNTER) && \
         (SOC_REG_INFO(unit, socreg).regtype != soc_cpureg))
#endif

#define SOC_COUNTER_INVALID(unit, reg) \
        ((reg == INVALIDr) || !SOC_REG_IS_ENABLED(unit, reg))

#define SOC_COUNTER_EXTRA_CB_MAX         3

/*
 * Counter map structure.
 * Each chip has an array of these structures indexed by soc_ctr_type_t
 */

typedef struct soc_ctr_ref_s {
    soc_reg_t  reg;
    int        index;
} soc_ctr_ref_t;

typedef struct soc_cmap_s {
    soc_ctr_ref_t  *cmap_base;    /* The array of counters as they are DMA'd */
    int                cmap_size;    /* The number of elements in the array */
} soc_cmap_t;

/*
 * Non CMIC counter DMA supported counters
 * These counters are included by show counter output
 */
typedef enum soc_counter_non_dma_id_e {
    SOC_COUNTER_NON_DMA_START = NUM_SOC_REG,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT = SOC_COUNTER_NON_DMA_START,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT_UC,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE_UC,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_PKT_EXT,
    SOC_COUNTER_NON_DMA_EGR_PERQ_XMT_BYTE_EXT,
    SOC_COUNTER_NON_DMA_COSQ_DROP_PKT,
    SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE,
    SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_UC,
    SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_UC,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_ING,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_ING,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_ING_METER,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_ING_METER,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_IBP,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_CFAP,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_YELLOW,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_RED,
    SOC_COUNTER_NON_DMA_PORT_WRED_PKT_GREEN,
    SOC_COUNTER_NON_DMA_PORT_WRED_PKT_YELLOW,
    SOC_COUNTER_NON_DMA_PORT_WRED_PKT_RED,
    SOC_COUNTER_NON_DMA_DROP_RQ_PKT,
    SOC_COUNTER_NON_DMA_DROP_RQ_BYTE,
    SOC_COUNTER_NON_DMA_DROP_RQ_PKT_YELLOW,
    SOC_COUNTER_NON_DMA_DROP_RQ_PKT_RED,
    SOC_COUNTER_NON_DMA_POOL_PEAK,
    SOC_COUNTER_NON_DMA_POOL_CURRENT,
    SOC_COUNTER_NON_DMA_PG_MIN_PEAK,
    SOC_COUNTER_NON_DMA_PG_MIN_CURRENT,
    SOC_COUNTER_NON_DMA_PG_SHARED_PEAK,
    SOC_COUNTER_NON_DMA_PG_SHARED_CURRENT,
    SOC_COUNTER_NON_DMA_PG_HDRM_PEAK,
    SOC_COUNTER_NON_DMA_PG_HDRM_CURRENT,
    SOC_COUNTER_NON_DMA_QUEUE_PEAK,
    SOC_COUNTER_NON_DMA_QUEUE_CURRENT,
    SOC_COUNTER_NON_DMA_UC_QUEUE_PEAK,
    SOC_COUNTER_NON_DMA_UC_QUEUE_CURRENT,
    SOC_COUNTER_NON_DMA_EXT_QUEUE_PEAK,
    SOC_COUNTER_NON_DMA_EXT_QUEUE_CURRENT,
    SOC_COUNTER_NON_DMA_COSQ_WRED_PKT_RED,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_YELLOW,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_YELLOW,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_RED,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_RED,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_PKT_GREEN,
    SOC_COUNTER_NON_DMA_COSQ_ACCEPT_BYTE_GREEN,
    SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_YELLOW,
    SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_YELLOW,
    SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_RED,
    SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_RED,
    SOC_COUNTER_NON_DMA_COSQ_DROP_PKT_GREEN,
    SOC_COUNTER_NON_DMA_COSQ_DROP_BYTE_GREEN,
    SOC_COUNTER_NON_DMA_TX_LLFC_MSG_CNT,
    SOC_COUNTER_NON_DMA_MMU_QCN_CNM,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM0_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM0_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM0_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM0_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM1_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM1_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM1_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM1_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM2_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM2_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM2_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM2_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM3_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM3_LOW_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM3_HIGH_PRI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM3_LOW_PRI,
    SOC_COUNTER_NON_DMA_COSQ_GLOBAL_HDR_COUNT_X_PEAK,
    SOC_COUNTER_NON_DMA_COSQ_GLOBAL_HDR_COUNT_X_CURRENT,
    SOC_COUNTER_NON_DMA_COSQ_GLOBAL_HDR_COUNT_Y_PEAK,
    SOC_COUNTER_NON_DMA_COSQ_GLOBAL_HDR_COUNT_Y_CURRENT,
    SOC_COUNTER_NON_DMA_EGR_PERQ_ECN_MARKED,
    SOC_COUNTER_NON_DMA_PORT_WRED_DROP_PKT,
    SOC_COUNTER_NON_DMA_ING_FLEX_PKT,
    SOC_COUNTER_NON_DMA_ING_FLEX_BYTE,
    SOC_COUNTER_NON_DMA_EGR_FLEX_PKT,
    SOC_COUNTER_NON_DMA_EGR_FLEX_BYTE,
    SOC_COUNTER_NON_DMA_EFP_PKT,
    SOC_COUNTER_NON_DMA_EFP_BYTE,
    SOC_COUNTER_NON_DMA_SFLOW_ING_PKT,
    SOC_COUNTER_NON_DMA_SFLOW_FLEX_PKT,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_LO,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_LO,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSY_HI,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSY_HI,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS0,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS0,
    SOC_COUNTER_NON_DMA_PORT_DROP_PKT_OBM_LOSSLESS1,
    SOC_COUNTER_NON_DMA_PORT_DROP_BYTE_OBM_LOSSLESS1,
    SOC_COUNTER_NON_DMA_TAS_TXOVERRUN_EXPRESS,
    SOC_COUNTER_NON_DMA_TAS_TXOVERRUN_PREEMPT,
    SOC_COUNTER_NON_DMA_PREEMPT_MAC_MERGE_HOLD_CNT,
    SOC_COUNTER_NON_DMA_ING_RX_COUNTER_NON_PREEMPTABLE_CNT,
    SOC_COUNTER_NON_DMA_ING_RX_COUNTER_PREEMPTABLE_CNT,
    SOC_COUNTER_NON_DMA_EGR_TX_COUNTER_NON_PREEMPTABLE_CNT,
    SOC_COUNTER_NON_DMA_EGR_TX_COUNTER_PREEMPTABLE_CNT,
    SOC_COUNTER_NON_DMA_E2E_DROP_COUNT,
    SOC_COUNTER_NON_DMA_PORT_OBM_FC_EVENTS,
    SOC_COUNTER_NON_DMA_END
} soc_counter_non_dma_id_t;

/* flags for soc_counter_non_dma_t.flags */
#define _SOC_COUNTER_NON_DMA_VALID             0x0001
#define _SOC_COUNTER_NON_DMA_DO_DMA            0x0002
#define _SOC_COUNTER_NON_DMA_ALLOC             0x0004
#define _SOC_COUNTER_NON_DMA_PEAK              0x0008
#define _SOC_COUNTER_NON_DMA_CURRENT           0x0010
#define _SOC_COUNTER_NON_DMA_PERQ_REG          0x0020 /* for register with variable
                                                       * number of queue per port */
#define _SOC_COUNTER_NON_DMA_CLEAR_ON_READ     0x0040
#define _SOC_COUNTER_NON_DMA_EXTRA_COUNT       0x0080
#define _SOC_COUNTER_NON_DMA_CTR_EVICT         0x0100
#define _SOC_COUNTER_NON_DMA_SUBSET_PARENT     0x0200
#define _SOC_COUNTER_NON_DMA_SUBSET_CHILD      0x0400
#define _SOC_COUNTER_NON_DMA_SLOWDMA           0x0800
/* For counters mapped to queue flexibly,
 * such as QCN counters to UC queuesin TD2 */
#define _SOC_COUNTER_NON_DMA_FLEX_MAPPING      0x1000
#define _SOC_COUNTER_NON_DMA_OBM               0x2000
#define _SOC_COUNTER_NON_DMA_NO_SHOW_C         0x4000 /* Counters Not picked for
                                                       * Show counter output */

/* DMA Rate profile Macros */
#define _SOC_COUNTER_DMA_RATE_PROFILE0        0x0001
#define _SOC_COUNTER_DMA_RATE_PROFILE1        0x0002
#define _SOC_COUNTER_DMA_RATE_PROFILE2        0x0004
#define _SOC_COUNTER_DMA_RATE_PROFILE3        0x0008

#define _SOC_COUNTER_DMA_RATE_PROFILE_MAX     4
#define _SOC_COUNTER_DMA_RATE_PROFILE_OFFSET  0
#define _SOC_COUNTER_DMA_RATE_PROFILE_MASK    0x00ff
#define _SOC_COUNTER_DMA_RATE_PROFILE_ALL     _SOC_COUNTER_DMA_RATE_PROFILE_MASK

/* Macros for default counter / counter eviction config variables */
#define _SOC_CTR_THREAD_PRI                   50
/* hostbuf size should be in order of 2 power */
#define _SOC_CTR_EVICT_HOSTBUF_SIZE           8192
#define _SOC_CTR_EVICT_THREAD_PRI             50
#define _SOC_CTR_EVICT_ENTRIES_MAX            0

/* Support for sFlow counters */
typedef enum soc_sflow_ctr_type_e {
   SOC_SFLOW_CTR_TYPE_SAMPLE_POOL,
   SOC_SFLOW_CTR_TYPE_SAMPLE_POOL_SNAPSHOT,
   SOC_SFLOW_CTR_TYPE_SAMPLE_COUNT,
   SOC_SFLOW_CTR_TYPE_COUNT
} soc_sflow_ctr_type_t;

/* one structure per each soc_counter_non_dma_id_t */
typedef int (*soc_counter_hw_idx_get_f)(int unit, soc_port_t port,
                                        int index, int *qindex);
typedef struct soc_counter_non_dma_s {
    soc_reg_t id;
    uint32 flags;   /* _SOC_COUNTER_NON_DMA_XXX */
    uint16 dma_rate_profile; /* contains the profile info for DMA purpose */
    soc_pbmp_t pbmp;
    int base_index; /* first index into soc_control_t.counter_val[] and etc. */
    int entries_per_port; /* max possible number of entries per port */
    int num_entries;
    int num_valid_pipe;
    soc_mem_t mem;
    soc_reg_t reg;
    soc_field_t field;
    char *cname;
    uint32 *dma_buf[4];
    int dma_index_min[4];
    int dma_index_max[4];
    soc_mem_t dma_mem[4];
    sbusdma_desc_handle_t handle;

    /* below are for subset children */
    struct soc_counter_non_dma_s *extra_ctrs;
    uint32 extra_ctr_ct;

    /* Routine used to retrieve the table index
     * for flexible mapping Non-DMA counter */
    soc_counter_hw_idx_get_f soc_counter_hw_idx_get;

} soc_counter_non_dma_t;

typedef struct soc_counter_evict_s {
    int pkt_counter_id;   /* soc_counter_non_dma_id_t */
    int byte_counter_id;  /* soc_counter_non_dma_id_t */
    int offset[SOC_MAX_NUM_PIPES];        /* offset into above counter id buffer */
    int pool_id;
    soc_mem_t mem[SOC_MAX_NUM_PIPES];
} soc_counter_evict_t;

#define _SOC_CONTROLLED_COUNTER_FLAG_NOT_PRINTABLE 0x1
#define _SOC_CONTROLLED_COUNTER_FLAG_RX     0x2
#define _SOC_CONTROLLED_COUNTER_FLAG_TX     0x4
#define _SOC_CONTROLLED_COUNTER_FLAG_LOW    0x8
#define _SOC_CONTROLLED_COUNTER_FLAG_MEDIUM 0x10
#define _SOC_CONTROLLED_COUNTER_FLAG_HIGH   0x20
#define _SOC_CONTROLLED_COUNTER_FLAG_NIF    0x40
#define _SOC_CONTROLLED_COUNTER_FLAG_MAC    0x80
#define _SOC_CONTROLLED_COUNTER_FLAG_ILKN   0x100
#define _SOC_CONTROLLED_COUNTER_FLAG_INVALID 0xffffffff


/* controlled (programmable) counters*/
typedef int (*soc_get_counter_f)(int unit, int counter_id, int port, uint64* val);
typedef struct soc_controlled_counter_s {
    soc_get_counter_f controlled_counter_f;
    int counter_id;
    char* cname;
    char* short_cname; /*up to 18 chars*/
    uint32 flags;
    uint32 counter_idx;
} soc_controlled_counter_t;

#define _SOC_CONTROLLED_COUNTER_NOF(unit) SOC_CONTROL(unit)->soc_controlled_counter_all_num
#define _SOC_CONTROLLED_COUNTER_USE(unit, port) (                                                                       \
                                    SOC_IS_DFE(unit) ||                                                              \
                                    (SOC_IS_ARAD(unit) && (IS_SFI_PORT(unit, port) || IS_IL_PORT(unit, port)))          \
                                    )

#define COUNTER_IDX_NOT_COLLECTED 0xFFFFFFFF

#define COUNTER_IS_COLLECTED(ctr) (COUNTER_IDX_NOT_COLLECTED != ctr.counter_idx)

/*
 * Nomenclature:
 *
 *   Counter register: enum value such as RPKTr
 *   Counter port_offset: Physical S-Channel address with port included
 *   Counter offset: Physical S-Channel address, lower 8 bits only
 *   Counter port_index: Index of counter in DMA buffer given port
 *   Counter index: Index of counter in counter map
 */

#define COUNTER_OFF_MIN_STRATA          0x20   /* Strata stat reg offset */
#define COUNTER_OFF_MIN_DRACO           0x20   /* Draco stat offs, 0x20-0x75 */
#define COUNTER_OFF_MIN_TUCANA          0x20
#define COUNTER_OFF_MIN_DEFAULT         0x20   /* Default matches all above */
#define COUNTER_OFF_MIN_HERC            0x09   /* Herc stat offs, 0x09-0x49 */
#define COUNTER_OFF_MIN_LYNX            0x1d0  /* Lynx stat offs, 0x1d0-0x239 */
#define COUNTER_OFF_MIN_HERC15          0x1d0  /* Herc15 stat, 0x1d0-0x23d */


/*
 * Counter thread control
 */
#define SOC_COUNTER_F_DMA               0x00000001
#define SOC_COUNTER_F_SWAP64        0x00010000    /* DMA buf 64 bit vals
                             * must be word swapped
                             * (internal only)
                             */
#define SOC_COUNTER_F_HOLD              0x00020000      /* Special handling of
                                                         * HOLD counter
                             * (internal only)
                                                         */

typedef enum soc_ctr_type_e {
    SOC_CTR_TYPE_FE,
    SOC_CTR_TYPE_GE,
    SOC_CTR_TYPE_GFE,
    SOC_CTR_TYPE_HG,
    SOC_CTR_TYPE_XE,
    SOC_CTR_TYPE_CE,
    SOC_CTR_TYPE_CPU,
    SOC_CTR_TYPE_RX,
    SOC_CTR_TYPE_TX,
    SOC_CTR_NUM_TYPES   /* Last please */
} soc_ctr_type_t;

#define    SOC_CTR_TYPE_NAMES_INITIALIZER { \
    "FE",    \
    "GE",    \
    "GFE",    \
    "HG",    \
    "XE",    \
    "CE",    \
    "CPU",    \
    "RX",   \
    "TX",   \
    "XGE",    \
    }

typedef enum soc_ctr_instance_type_e {
    SOC_CTR_INSTANCE_TYPE_PORT,
    SOC_CTR_INSTANCE_TYPE_POOL,
    SOC_CTR_INSTANCE_TYPE_PIPE,
    SOC_CTR_INSTANCE_TYPE_XPE, /*Supports for mmu type port */
    SOC_CTR_INSTANCE_TYPE_POOL_PIPE, /* Support for pool_pipe combination */
    SOC_CTR_INSTANCE_TYPE_XPE_PORT, /* Support for xpe_port combination   */
    SOC_CTR_INSTANCE_TYPE_SFLOW /* Support for sFlow counters */
} soc_ctr_instance_type_t;

typedef struct soc_ctr_control_info_s{
    soc_ctr_instance_type_t instance_type;
    uint32 instance;
} soc_ctr_control_info_t;

/* Flags for Generic Get/MultiGet */
#define SOC_COUNTER_SYNC_ENABLE 0x1

typedef enum soc_ctr_ctrl_type_e {
    SOC_CTR_CTRL_CONFIG_DMA_ENABLE,            /* Enable/Disable Counter DMA */
    SOC_CTR_CTRL_CONFIG_DMA_MINIDX,            /* Config Min idx for DMA */
    SOC_CTR_CTRL_CONFIG_DMA_MAXIDX,            /* Config Max idx for DMA */
    SOC_CTR_CTRL_CONFIG_DMA_RATE_PROFILE_ALL,  /* DMA profile: 1 - Enable ALL,
                                                * 0 - Disable ALL profiles */
    SOC_CTR_CTRL_CONFIG_DMA_RATE_PROFILE_AUTO, /* AUTO profile choice */
    SOC_CTR_CTRL_CONFIG_DMA_RATE_PROFILE_WFS,  /* Selection based on Weights */
    SOC_CTR_CTRL_CONFIG_COUNT
}soc_ctr_ctrl_type_t;

typedef struct soc_ctr_ctrl_config_s {
    soc_ctr_ctrl_type_t config_type;
    int config_val;
}soc_ctr_ctrl_config_t;

typedef int (*soc_counter_dma_config_update)(int unit,
                                             soc_counter_non_dma_t *dma,
                                             uint32 profile, int val);
typedef int (*soc_counter_get_child_dma)(int unit, soc_reg_t id,
                                         soc_ctr_control_info_t ctrl_info,
                                         soc_counter_non_dma_t **child_dma);
typedef void (*soc_counter_collect_non_dma)(int unit, int tmp_port,
                                            int port_idx, int ar_idx,
                                            soc_reg_t ctr_reg);
typedef int (*soc_counter_port_obm_info_get)(int unit, soc_port_t port,
                                             int *obm_id, int *lane);
typedef struct soc_counter_control_s {
    soc_counter_dma_config_update   dma_config_update;
    soc_counter_get_child_dma       get_child_dma;
    soc_counter_collect_non_dma     collect_non_stat_dma;
    soc_counter_port_obm_info_get   port_obm_info_get; /* Get obm_id, lane_id
                                                        * for logical port */
    uint32                          total_dma_entries; /* total num of non stat
                                                        * dma entries */
} soc_counter_control_t;

#if defined(BCM_TOMAHAWK_SUPPORT)
extern int soc_th_ser_update_counter(int unit, int is_reg,
                                     soc_reg_t restore_reg,
                                     soc_mem_t restore_mem, int index,
                                     int copyno, soc_reg_t base_reg,
                                     soc_mem_t base_mem, int inst_num,
                                     int pipe, int restore_last);
#endif /* BCM_TOMAHAWK_SUPPORT */

extern soc_counter_control_t *soc_counter_control[SOC_MAX_NUM_DEVICES];
#define SOC_CTR_CTRL(unit) soc_counter_control[(unit)]


typedef struct soc_ctr_reg_desc {
    soc_regtype_t reg;
    uint8 width;
    uint16 entries;
    uint16 shift;
} soc_ctr_reg_desc_t;

#define MAX_CTR_REG_PER_BLK 20

typedef struct soc_blk_ctr_reg_desc {
    soc_block_t blk;
    soc_ctr_reg_desc_t desc[MAX_CTR_REG_PER_BLK];
} soc_blk_ctr_reg_desc_t;

typedef struct soc_blk_ctr_process {
    soc_block_t blk;
    uint16 bindex;
    uint16 entries;
    uint64 *buff;
    uint64 *hwval;
    uint64 *swval;
} soc_blk_ctr_process_t;

#define SOC_COUNTER_INTERVAL_DEFAULT     1000000
#define SOC_COUNTER_INTERVAL_SIM_DEFAULT 25000000

extern int soc_counter_attach(int unit);
extern int soc_counter_detach(int unit);
extern int soc_counter_start(int unit, uint32 flags,
                 int interval, pbmp_t pbmp);
extern int soc_counter_status(int unit, uint32 *flags,
                  int *interval, pbmp_t *pbmp);
extern int soc_counter_sync(int unit);
extern int soc_counter_stop(int unit);
extern int soc_counter_port_collect_enable_set(int unit,
		                           int port, int enable);
extern void soc_counter_ctr_reset_to_zero(int unit);
extern int soc_counter_non_dma_pbmp_update(int unit);
/*
 * Routines to fetch counter values.
 * These return non-zero if the counter changed since the last 'get'.
 */

extern int soc_counter_generic_multi_get(int unit, soc_reg_t ctr_reg,
                                         soc_ctr_control_info_t ctrl_info,
                                         uint32 flags, int ar_idx, int count,
                                         uint64 *val);
extern int soc_counter_generic_get_info(int unit,
                                        soc_ctr_control_info_t ctrl_info,
                                        soc_reg_t id, int *base_index,
                                        int *num_entries);
extern int soc_counter_generic_get(int unit, soc_reg_t ctr_reg,
                                   soc_ctr_control_info_t ctrl_info,
                                   uint32 flags, int ar_idx, uint64 *val);
extern int soc_counter_get(int unit, soc_port_t port, soc_reg_t ctr_reg,
                           int ar_idx, uint64 *val);
extern int soc_counter_get32(int unit, soc_port_t port, soc_reg_t ctr_reg,
                             int ar_idx, uint32 *val);
extern int soc_counter_get_zero(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                int ar_idx, uint64 *val);
extern int soc_counter_get32_zero(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                  int ar_idx, uint32 *val);
extern int soc_counter_get_rate(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                int ar_idx, uint64 *rate);
extern int soc_counter_get32_rate(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                  int ar_idx, uint32 *rate);
extern int soc_counter_sync_get(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                int ar_idx, uint64 *val);
extern int soc_counter_sync_get32(int unit, soc_port_t port, soc_reg_t ctr_reg,
                                  int ar_idx, uint32 *val);

extern int soc_counter_port_pbmp_add(int unit, int port);
extern int soc_counter_port_pbmp_remove(int unit, int port);


/*
 * Routines to set counter values, normally used to clear them to zero.
 * These work by reading the counters to synchronize the software and
 * hardware counters, then set the software counters.
 */

extern int soc_counter_generic_multi_set(int unit, soc_reg_t ctr_reg,
                                         soc_ctr_control_info_t ctrl_info,
                                         uint32 flags, int ar_idx, int count,
                                         uint64 *val);

extern int soc_counter_generic_set(int unit, soc_reg_t id,
                                    soc_ctr_control_info_t ctrl_info,
                                    uint32 flags, int ar_idx, uint64 val);

extern int soc_counter_config_multi_get(int unit, soc_reg_t ctr_reg, int count,
                                             soc_ctr_control_info_t *ctrl_info,
                                            soc_ctr_ctrl_config_t *ctrl_config);

extern int soc_counter_config_multi_set(int unit, soc_reg_t ctr_reg, int count,
                                             soc_ctr_control_info_t *ctrl_info,
                                            soc_ctr_ctrl_config_t *ctrl_config);

extern int soc_counter_set(int unit, soc_port_t port, soc_reg_t id,
                           int ar_idx, uint64 val);
extern int soc_counter_set32(int unit, soc_port_t port, soc_reg_t ctr_reg,
                             int ar_idx, uint32 val);
extern int soc_counter_set_by_port(int unit, pbmp_t pbmp, uint64 val);
extern int soc_counter_set32_by_port(int unit, pbmp_t pbmp, uint32 val);
extern int soc_counter_set_by_reg(int unit, soc_reg_t ctr_reg,
                                  int ar_idx, uint64 val);
extern int soc_counter_set32_by_reg(int unit, soc_reg_t ctr_reg,
                                    int ar_idx, uint32 val);
extern int soc_counter_clear_by_port_reg(int unit, soc_port_t port,
                                         soc_reg_t ctr_reg,
                                         int ar_idx, uint64 val);

extern int soc_controlled_counter_clear(int unit, soc_port_t port);
extern int soc_port_cmap_set(int unit, soc_port_t port, soc_ctr_type_t ctype);
extern soc_cmap_t *soc_port_cmap_get(int unit, soc_port_t port);

extern int soc_counter_idx_get(int unit, soc_reg_t reg, int ar_idx, int port);

#if defined(BCM_XGS5_SWITCH_PORT_SUPPORT)
extern int
soc_counter_port_sbusdma_desc_alloc(int unit, soc_port_t port);

extern int
soc_counter_port_sbusdma_desc_free(int unit, soc_port_t port);
#endif /* BCM_XGS5_SWITCH_PORT_SUPPORT */

#ifdef BCM_SBUSDMA_SUPPORT
extern int soc_blk_counter_set(int unit, soc_block_t blk, soc_reg_t ctr_reg,
                               int ar_idx, uint64 val);
extern int soc_blk_counter_get(int unit, soc_block_t blk, soc_reg_t ctr_reg,
                               int ar_idx, uint64 *val);
#endif

/* Counter eviction functions */
extern int soc_counter_eviction_sync(int unit);
/* For encoding different instance types into one instance for generic get/set
  */
extern int soc_counter_instance_encode(soc_ctr_control_info_t *input_ctrl_info,
                                       int count,
                                       soc_ctr_control_info_t *output_ctrl_info);

/*
 * Driver internal and debugging use only
 */

extern int soc_counter_autoz(int unit, int enable);
extern int soc_counter_timestamp_get(int unit, soc_port_t port,
                                     uint32 *timestamp);

/* For registering additional counter collection */
typedef void (*soc_counter_extra_f)(int unit);

extern int soc_counter_extra_register(int unit, soc_counter_extra_f fn);
extern int soc_counter_extra_unregister(int unit, soc_counter_extra_f fn);

#ifdef BROADCOM_DEBUG
extern void _soc_counter_verify(int unit); /* Verify counter map consistent */
#endif /* BROADCOM_DEBUG */

#ifdef BCM_DINO16_SUPPORT
/* bcm5396 MIB group */
#define SOC_COUNTER_DINO16_GP0     0
#define SOC_COUNTER_DINO16_GP1     1
#define SOC_COUNTER_DINO16_GP2     2
#endif /* BCM_DINO16_SUPPORT */

extern int soc_robo_port_cmap_set(int unit, soc_port_t port, soc_ctr_type_t ctype);
extern soc_cmap_t *soc_robo_port_cmap_get(int unit, soc_port_t port);

#ifdef BCM_TB_SUPPORT
extern int soc_tb_cmap_set(int unit, soc_ctr_type_t ctype);
extern soc_cmap_t *soc_tb_cmap_get(int unit, int type);
#endif

extern int soc_robo_counter_get(int unit, soc_port_t port,
    soc_reg_t ctr_reg, int sync_hw, uint64 *val);
extern int soc_robo_counter_get32(int unit, soc_port_t port, soc_reg_t ctr_reg,
                  uint32 *val);
extern int soc_robo_counter_get_zero(int unit, soc_port_t port,
             soc_reg_t ctr_reg, uint64 *val);
extern int soc_robo_counter_get32_zero(int unit, soc_port_t port,
               soc_reg_t ctr_reg, uint32 *val);
extern int soc_robo_counter_get_rate(int unit, soc_port_t port,
             soc_reg_t ctr_reg, uint64 *rate);
extern int soc_robo_counter_get32_rate(int unit, soc_port_t port,
               soc_reg_t ctr_reg, uint32 *rate);
extern int soc_robo_counter_set(int unit, soc_port_t port, soc_reg_t ctr_reg,
                uint64 val);
extern int soc_robo_counter_set32(int unit, soc_port_t port, soc_reg_t ctr_reg,
                  uint32 val);
extern int soc_robo_counter_set_by_port(int unit, pbmp_t pbmp, uint64 val);
extern int soc_robo_counter_set32_by_port(int unit, pbmp_t pbmp, uint32 val);
extern int soc_robo_counter_set_by_reg(int unit, soc_reg_t ctr_reg, uint64 val);
extern int soc_robo_counter_set32_by_reg(int unit, soc_reg_t ctr_reg, uint32 val);
extern int soc_robo_counter_start(int unit, uint32 flags, int interval, pbmp_t pbmp);
extern int soc_robo_counter_sync(int unit);
extern int soc_robo_counter_stop(int unit);

extern int soc_robo_counter_attach(int unit);
extern int soc_robo_counter_detach(int unit);

extern int soc_robo_counter_idx_get(int unit, soc_reg_t reg, int port);
extern int soc_robo_counter_status(int unit, uint32 *flags,
                  int *interval, pbmp_t *pbmp);

extern int soc_robo_counter_prev_get(int unit, int port, soc_reg_t ctr_reg, uint64 *val);
extern int soc_robo_counter_prev_set(int unit, int port, soc_reg_t ctr_reg, uint64 val);
extern int _soc_robo_counter_sw_table_set(int unit, soc_pbmp_t pbmp, uint64 val);

#endif  /* !_SOC_COUNTER_H */
