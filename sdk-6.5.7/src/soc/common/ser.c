/*
 * $Id: ser.c,v 1.63 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * SER common code.
 *
 * Common code for memory SER protection.
 */

#include <shared/bsl.h>

#include <sal/core/libc.h>
#include <sal/core/dpc.h>
#include <soc/drv.h>
#include <soc/debug.h>
#include <soc/util.h>
#include <soc/mem.h>
#include <soc/l2x.h>
#include <soc/l2u.h>
#include <soc/l3x.h>
#include <soc/dma.h>
#include <soc/soc_ser_log.h>
#ifdef BCM_CMICM_SUPPORT
#include <soc/cmicm.h>
#endif

#if defined(BCM_TRIUMPH2_SUPPORT)
#include <soc/triumph2.h>
#include <soc/triumph.h>
#endif /* BCM_TRIUMPH2_SUPPORT */
#if defined(BCM_TRIDENT_SUPPORT)
#include <soc/trident.h>
#endif /* BCM_TRIDENT_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
#include <soc/trident2.h>
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TRIUMPH3_SUPPORT)
#include <soc/triumph3.h>
#endif /* BCM_TRIUMPH3_SUPPORT */
#if defined(BCM_HELIX4_SUPPORT)
#include <soc/helix4.h>
#endif /* BCM_HELIX4_SUPPORT */
#if defined(BCM_KATANA_SUPPORT)
#include <soc/katana.h>
#endif /* BCM_KATANA_SUPPORT */
#if defined(BCM_KATANA2_SUPPORT)
#include <soc/katana2.h>
#endif /* BCM_KATANA2_SUPPORT */
#if defined(BCM_ENDURO_SUPPORT)
#include <soc/enduro.h>
#endif /* BCM_ENDURO_SUPPORT */
#if defined(BCM_HURRICANE2_SUPPORT)
#include <soc/hurricane2.h>
#endif /* BCM_HURRICANE2_SUPPORT */
#if defined(BCM_GREYHOUND_SUPPORT)
#include <soc/greyhound.h>
#endif /* BCM_GREYHOUND_SUPPORT */
#if defined(BCM_TOMAHAWK_SUPPORT)
#include <soc/tomahawk.h>
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_HURRICANE3_SUPPORT)
#include <soc/hurricane3.h>
#endif /* BCM_HURRICANE3_SUPPORT */
#if defined(BCM_APACHE_SUPPORT)
#include <soc/apache.h>
#endif /* BCM_APACHE_SUPPORT */


#if defined(BCM_XGS_SUPPORT) || defined(BCM_SIRIUS_SUPPORT)

#define _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, field) \
    if (SOC_MEM_FIELD_VALID(unit, mem, field)) {          \
        soc_mem_field32_set(unit, mem, entry, field, 0);  \
    }

#define _SOC_DRV_MEM_CHK_L2_MEM(mem) \
        (mem == L2Xm || mem == L2_ENTRY_1m || mem == L2_ENTRY_2m)

/* Keep this size in sync with the items in the 2 lists below */
#define SOC_SER_REG_CACHE_MAX 30

#define _SOC_SER_CACHE_REG_CASES        \
    case DMVOQ_WRED_CONFIGr:            \
    case GLOBAL_SP_WRED_CONFIGr:        \
    case MAXBUCKETCONFIG_64r:           \
    case MINBUCKETCONFIG_64r:           \
    case PG_RESET_FLOOR_CELLr:          \
    case PG_RESET_OFFSET_CELLr:         \
    case PORT_SHARED_LIMIT_CELLr:       \
    case PORT_SP_WRED_CONFIGr:          \
    case S2_MAXBUCKETCONFIG_64r:        \
    case S2_MINBUCKETCONFIG_64r:        \
    case S3_MAXBUCKETCONFIG_64r:        \
    case S3_MINBUCKETCONFIG_64r:        \
    case S2_COSWEIGHTSr:                \
    case S3_COSWEIGHTSr:                \
    case WRED_CONFIGr:                  \
    case ING_TRILL_ADJACENCYr:          \
    case EGR_VLAN_CONTROL_2r:           \
    case EGR_PVLAN_EPORT_CONTROLr:      \
    case E2EFC_CNT_SET_LIMITr:          \
    case E2EFC_CNT_RESET_LIMITr:        \
    case E2EFC_CNT_DISC_LIMITr:         \
    case CPUMAXBUCKETCONFIG_64r:        \
    case EGR_1588_LINK_DELAY_64r:       \
    case STORM_CONTROL_METER_CONFIGr:   \
    case EGR_SF_SRC_MODID_CHECKr:       \
    case EGR_VLAN_CONTROL_3r:           \
    case SFLOW_ING_THRESHOLDr:          \
    case PG_SHARED_LIMIT_CELLr:         \
    case EGR_VLAN_CONTROL_1r:           \
    case EGR_IPMC_CFG2r:                \


#define _SOC_TH_ACC_TYPE_FOR_PIPE(pipe) \
    (((pipe) == 0)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_0 : \
     ((pipe) == 1)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_1 : \
     ((pipe) == 2)? _SOC_MEM_ADDR_ACC_TYPE_PIPE_2 : \
                    _SOC_MEM_ADDR_ACC_TYPE_PIPE_3)
#if defined(BCM_TOMAHAWK_SUPPORT)
#define _SOC_TH_ACC_TYPE_DUPLICATE 9
#endif /* BCM_TOMAHAWK_SUPPORT */

#if defined(ALPM_ENABLE)
#define _ALPM_LOCK(unit) \
    L3_LOCK(unit); \
    _alpm_lock = 1
#define _ALPM_UNLOCK(unit) \
    if (_alpm_lock) { \
        L3_UNLOCK(unit); \
    }
#else
#define _ALPM_LOCK(unit)
#define _ALPM_UNLOCK(unit)
#endif /* ALPM_ENABLE */


#define _L2_LOCK(unit) \
    MEM_LOCK(unit, L2Xm);

#define _L2_UNLOCK(unit, lock) \
    if ((lock)) { \
        MEM_UNLOCK(unit, L2Xm); \
    }

#define _SOC_SER_NUM_EM_ENTRIES_PER_LP_ENTRY 4

#define _SOC_FPEM_EM2(pipe, mem_mode) \
    ((mem_mode == _SOC_SER_MEM_MODE_GLOBAL)? EXACT_MATCH_2m \
                  : ((pipe == 1)? EXACT_MATCH_2_PIPE1m : \
                     (pipe == 2)? EXACT_MATCH_2_PIPE2m : \
                     (pipe == 3)? EXACT_MATCH_2_PIPE3m : \
                                  EXACT_MATCH_2_PIPE0m))

#define _SOC_FPEM_EM4(pipe, mem_mode) \
    ((mem_mode == _SOC_SER_MEM_MODE_GLOBAL)? EXACT_MATCH_4m \
                  : ((pipe == 1)? EXACT_MATCH_4_PIPE1m : \
                     (pipe == 2)? EXACT_MATCH_4_PIPE2m : \
                     (pipe == 3)? EXACT_MATCH_4_PIPE3m : \
                                  EXACT_MATCH_4_PIPE0m))

#define _SOC_FPEM_EM2_ENTRY_ONLY(pipe, mem_mode) \
    ((mem_mode == _SOC_SER_MEM_MODE_GLOBAL)? EXACT_MATCH_2_ENTRY_ONLYm \
                  : ((pipe == 1)? EXACT_MATCH_2_ENTRY_ONLY_PIPE1m : \
                     (pipe == 2)? EXACT_MATCH_2_ENTRY_ONLY_PIPE2m : \
                     (pipe == 3)? EXACT_MATCH_2_ENTRY_ONLY_PIPE3m : \
                                  EXACT_MATCH_2_ENTRY_ONLY_PIPE0m))

#define _SOC_FPEM_EM4_ENTRY_ONLY(pipe, mem_mode) \
    ((mem_mode == _SOC_SER_MEM_MODE_GLOBAL)? EXACT_MATCH_4_ENTRY_ONLYm \
                  : ((pipe == 1)? EXACT_MATCH_4_ENTRY_ONLY_PIPE1m : \
                     (pipe == 2)? EXACT_MATCH_4_ENTRY_ONLY_PIPE2m : \
                     (pipe == 3)? EXACT_MATCH_4_ENTRY_ONLY_PIPE3m : \
                                  EXACT_MATCH_4_ENTRY_ONLY_PIPE0m))

#define _SOC_FPEM_ECC(pipe, mem_mode) \
    ((mem_mode == _SOC_SER_MEM_MODE_GLOBAL)? FPEM_ECCm \
                  : ((pipe == 1)? FPEM_ECC_PIPE1m : \
                     (pipe == 2)? FPEM_ECC_PIPE2m : \
                     (pipe == 3)? FPEM_ECC_PIPE3m : \
                                  FPEM_ECC_PIPE0m))

#define _SOC_MEM_CHK_FPEM_MEM(mem) \
        (mem == EXACT_MATCH_2m || \
         mem == EXACT_MATCH_2_PIPE0m || mem == EXACT_MATCH_2_PIPE1m || \
         mem == EXACT_MATCH_2_PIPE2m || mem == EXACT_MATCH_2_PIPE3m || \
         mem == EXACT_MATCH_4m || \
         mem == EXACT_MATCH_4_PIPE0m || mem == EXACT_MATCH_4_PIPE1m || \
         mem == EXACT_MATCH_4_PIPE2m || mem == EXACT_MATCH_4_PIPE3m)

static int _soc_ser_reg_cache_list[SOC_SER_REG_CACHE_MAX] = {
    DMVOQ_WRED_CONFIGr,             /* 1 */
    GLOBAL_SP_WRED_CONFIGr,         /* 2 */
    MAXBUCKETCONFIG_64r,            /* 3 */
    MINBUCKETCONFIG_64r,            /* 4 */
    PG_RESET_FLOOR_CELLr,           /* 5 */
    PG_RESET_OFFSET_CELLr,          /* 6 */
    PORT_SHARED_LIMIT_CELLr,        /* 7 */
    PORT_SP_WRED_CONFIGr,           /* 8 */
    S2_MAXBUCKETCONFIG_64r,         /* 9 */
    S2_MINBUCKETCONFIG_64r,         /* 10 */
    S3_MAXBUCKETCONFIG_64r,         /* 11 */
    S3_MINBUCKETCONFIG_64r,         /* 12 */
    S2_COSWEIGHTSr,                 /* 13 */
    S3_COSWEIGHTSr,                 /* 14 */
    WRED_CONFIGr,                   /* 15 */
    ING_TRILL_ADJACENCYr,           /* 16 */
    EGR_VLAN_CONTROL_2r,            /* 17 */
    EGR_PVLAN_EPORT_CONTROLr,       /* 18 */
    E2EFC_CNT_SET_LIMITr,           /* 19 */
    E2EFC_CNT_RESET_LIMITr,         /* 20 */
    E2EFC_CNT_DISC_LIMITr,          /* 21 */
    CPUMAXBUCKETCONFIG_64r,         /* 22 */
    EGR_1588_LINK_DELAY_64r,        /* 23 */
    STORM_CONTROL_METER_CONFIGr,    /* 24 */
    EGR_SF_SRC_MODID_CHECKr,        /* 25 */
    EGR_VLAN_CONTROL_3r,            /* 26 */
    SFLOW_ING_THRESHOLDr,           /* 27 */
    PG_SHARED_LIMIT_CELLr,          /* 28 */
    EGR_VLAN_CONTROL_1r,            /* 29 */
    EGR_IPMC_CFG2r                  /* 30 */
};

#define _SOC_SER_CLEAR_REG_CASES    \
    case GLOBAL_SP_WRED_AVG_QSIZEr: \
    case PORT_SP_WRED_AVG_QSIZEr:   \
    case WRED_AVG_QSIZEr:           \
    case VOQ_WRED_AVG_QSIZEr:       \
    case MINBUCKETr:                \
    case MAXBUCKETr:                \
    case S3_MINBUCKETr:             \
    case S3_MAXBUCKETr:             \
    case S2_MINBUCKETr:             \
    case S2_MAXBUCKETr:             \
    case S3_WERRCOUNTr:             \
    case S2_WERRCOUNTr:             \
    case CPUMAXBUCKETr:

#define _SOC_MAX_PORTS_NUMS    170 
#define _SOC_SER_REG_INDEX_MAX  79 

typedef struct _soc_ser_reg_cache_s {
    uint64 data[_SOC_MAX_PORTS_NUMS][_SOC_SER_REG_INDEX_MAX];
} _soc_ser_reg_cache_t;
typedef struct _soc_ser_reg_cache_array_s {
     _soc_ser_reg_cache_t reg_cache[SOC_SER_REG_CACHE_MAX];
} _soc_ser_reg_cache_array_t;
_soc_ser_reg_cache_array_t *_soc_ser_reg_cache[SOC_MAX_NUM_DEVICES] = {NULL};

typedef struct mem_ser_info {
    uint32    next_idx;
    soc_mem_t ser_mem;
    uint32    hw_idx;
    uint32    entry_data[SOC_MAX_MEM_WORDS];
} mem_ser_info_t;

typedef struct _mem_ser_info_head_t {
    mem_ser_info_t   *base_addr;
    uint32            head_idx;
    uint32            tail_idx;
    int               counter;
} mem_ser_info_head_t;

typedef struct soc_ser_alpm_cache_flag_s {
    uint32      flag_defip;
    uint32      flag_defip_128;
    uint32      flag_defip_alpm_v4;
    uint32      flag_defip_alpm_v4_1;
    uint32      flag_defip_alpm_v6;
    uint32      flag_defip_alpm_v6_1;
    uint32      flag_defip_alpm_v6_128;
    uint32      flag_defip_aux;
} soc_ser_alpm_cache_flag_t;

static mem_ser_info_head_t mem_ser_info_head[SOC_MAX_NUM_DEVICES];

#define SER_MEM_INFO_MAX_NUM     32
#define SER_MEM_INFO_BASE_ADDR(unit)     mem_ser_info_head[unit].base_addr
#define SER_MEM_INFO_HEAD_IDX(unit)      mem_ser_info_head[unit].head_idx
#define SER_MEM_INFO_TAIL_IDX(unit)      mem_ser_info_head[unit].tail_idx
#define SER_MEM_INFO_COUNTER(unit)       mem_ser_info_head[unit].counter
#define SER_MEM_INFO_NEXT_IDX(unit, cur_idx)      mem_ser_info_head[unit].base_addr[cur_idx].next_idx

static soc_ser_functions_t *_soc_ser_functions[SOC_MAX_NUM_DEVICES];
static soc_oam_event_functions_t *_soc_oam_event_functions[SOC_MAX_NUM_DEVICES];
static soc_ser_alpm_cache_flag_t alpm_no_cache_flag[SOC_MAX_NUM_DEVICES];

/* scrub_load: 0=scrub, 1=load cache */
int soc_ser_reg_load_scrub(int unit, int scrub_load)
{
    uint64 rval64;
    soc_reg_t reg;
    int r, i, blk, port, mindex;
    soc_block_types_t regblktype;

    /* Access all cacheable regs (store in cache if requested) */
    for (r = 0; r < SOC_SER_REG_CACHE_MAX; r++) {
        reg = _soc_ser_reg_cache_list[r];
        if (!SOC_REG_IS_VALID(unit, reg)) {
            continue;
        }
        if (SOC_REG_INFO(unit, reg).regtype == soc_genreg) {
            SOC_IF_ERROR_RETURN
                (soc_reg_get(unit, reg, REG_PORT_ANY, 0, &rval64));
            if (scrub_load) {
                soc_ser_reg_cache_set(unit, reg, 0, 0, rval64);
            }
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "Reg: %s port:%d index:%d\n"), SOC_REG_NAME(unit, reg), 0, 0));
        } else if (SOC_REG_INFO(unit, reg).regtype == soc_cosreg) {
            regblktype = SOC_REG_INFO(unit, reg).block;
            for (i = 0; i < NUM_COS(unit); i++) {
                SOC_BLOCKS_ITER(unit, blk, regblktype) {
                    port = SOC_BLOCK_PORT(unit, blk);
                    SOC_IF_ERROR_RETURN
                        (soc_reg_get(unit, reg, port, i, &rval64));
                    if (scrub_load) {
                        soc_ser_reg_cache_set(unit, reg, port, i, rval64);
                    }
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "Reg: %s port:%d index:%d\n"), SOC_REG_NAME(unit, reg), port, i));
                }
            }
        } else if ((SOC_REG_INFO(unit, reg).regtype == soc_portreg) ||
                   (SOC_REG_INFO(unit, reg).regtype == soc_ppportreg)) {
            mindex = 1;
            switch (reg) {
            case DMVOQ_WRED_CONFIGr:
            case S3_COSWEIGHTSr: mindex = 72; break;
            case PG_RESET_OFFSET_CELLr:
            case PORT_SHARED_LIMIT_CELLr:
            case WRED_CONFIGr: mindex = 8; break;
            case PORT_SP_WRED_CONFIGr:
            case S2_MINBUCKETCONFIG_64r:
            case S2_MAXBUCKETCONFIG_64r:
            case S3_MINBUCKETCONFIG_64r:
            case S3_MAXBUCKETCONFIG_64r: mindex = 4; break;
            case S2_COSWEIGHTSr: mindex = 12; break;
            default: break;
            }
            PBMP_ALL_ITER(unit, port) {
                switch (reg) {
                case DMVOQ_WRED_CONFIGr:
                    if (!SOC_INFO(unit).port_num_ext_cosq[port]) {
                        continue;
                    }
                    break;
                case MINBUCKETCONFIG_64r:
                case MINBUCKETr:
                case MAXBUCKETCONFIG_64r:
                case MAXBUCKETr:
#ifdef BCM_ENDURO_SUPPORT
                    if (SOC_IS_ENDURO(unit)) {
                        if (IS_CPU_PORT(unit, port)) {
                            continue;
                        } else if (IS_XQ_PORT(unit, port)) {
                            mindex = 26;
                        } else {
                            mindex = 8;
                        }
                    } else {
#endif
                    if (IS_CPU_PORT(unit, port)) {
                        mindex = 48;
                    } else if (IS_LB_PORT(unit, port)) {
                        mindex = 5;
                    } else if (!SOC_INFO(unit).port_num_ext_cosq[port]) {
                        mindex = 15;
                    } else {
                        mindex = 79;
                    }
#ifdef BCM_TRIUMPH_SUPPORT
                    if (SOC_IS_TRIUMPH(unit)) {
                        if (IS_CPU_PORT(unit, port)) {
                            continue;
                        }
                    }
#endif
#ifdef BCM_ENDURO_SUPPORT
                    }
#endif
                    break;
                case PORT_SP_WRED_CONFIGr:
                    if (IS_CPU_PORT(unit, port) || IS_LB_PORT(unit, port)) {
                        continue;
                    }
                    break;
                case S2_MINBUCKETCONFIG_64r:
                case S2_MAXBUCKETCONFIG_64r:
                case S3_MINBUCKETCONFIG_64r:
                case S3_MAXBUCKETCONFIG_64r:
                case S2_COSWEIGHTSr:
                    if (IS_CPU_PORT(unit, port)) {
                        continue;
                    }
                    break;
                case S3_COSWEIGHTSr:
                    if (!SOC_INFO(unit).port_num_ext_cosq[port]) {
                        mindex = 8;
                    }
                    break;
                case WRED_CONFIGr:
                    if (IS_CPU_PORT(unit, port) || IS_LB_PORT(unit, port)) {
                        continue;
                    }
                    break;
                default: break;
                }
                for (i = 0; i < mindex; i++) {
                    SOC_IF_ERROR_RETURN
                        (soc_reg_get(unit, reg, port, i, &rval64));
                    if (scrub_load) {
                        soc_ser_reg_cache_set(unit, reg, port, i, rval64);
                    }
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "Reg: %s port:%d index:%d\n"), SOC_REG_NAME(unit, reg), port, i));
                }
            }
        }
    }
    return SOC_E_NONE;
}

int
soc_ser_reg_cache_clear(int unit, soc_reg_t reg, int port)
{
    int i;

    if (reg == INVALIDr) {
        sal_memset(&_soc_ser_reg_cache[unit]->reg_cache, 0, sizeof(_soc_ser_reg_cache_t));
    } else {
        for (i = 0; i < SOC_SER_REG_CACHE_MAX; i++) {
            if (reg == _soc_ser_reg_cache_list[i]) {
                if (port == REG_PORT_ANY) {
                    sal_memset(&_soc_ser_reg_cache[unit]->reg_cache[i].data[0][0],
                               0, sizeof(_soc_ser_reg_cache_t));
                } else {
                    sal_memset(&_soc_ser_reg_cache[unit]->reg_cache[i].data[port][0],
                               0, sizeof(_soc_ser_reg_cache_t) / _SOC_MAX_PORTS_NUMS);
                }
                break;
            }
        }
    }
    return SOC_E_NONE;
}

int
soc_ser_reg_cache_init(int unit)
{
    if (_soc_ser_reg_cache[unit] == NULL) {
        _soc_ser_reg_cache[unit] = sal_alloc(sizeof(_soc_ser_reg_cache_array_t), "reg-cache");
    }
    if (SOC_WARM_BOOT(unit) && (SOC_CONTROL(unit)->mem_scache_ptr == NULL)) {
        return soc_ser_reg_load_scrub(unit, 1);
    } else {
        return soc_ser_reg_cache_clear(unit, INVALIDr, REG_PORT_ANY);
    }
}

void
soc_ser_reg_cache_info(int unit, int *count, int *size)
{
    *count = SOC_SER_REG_CACHE_MAX;
    *size = sizeof(_soc_ser_reg_cache_array_t);
}

int
soc_ser_reg_cache_set(int unit, soc_reg_t reg, int port, int index, uint64 data)
{
    int i;

    if (_soc_ser_reg_cache[unit] == NULL) {
        return SOC_E_NONE;
    }
    switch (reg) {
    _SOC_SER_CACHE_REG_CASES
        break;
    default:
        return SOC_E_NONE;
    }
    assert(port < _SOC_MAX_PORTS_NUMS);
    assert(index < _SOC_SER_REG_INDEX_MAX);
    if (port >= _SOC_MAX_PORTS_NUMS) {
        return SOC_E_PARAM;
    }
    if (index >= _SOC_SER_REG_INDEX_MAX) {
        return SOC_E_PARAM;
    }
    for (i = 0; i < SOC_SER_REG_CACHE_MAX; i++) {
        if (reg == _soc_ser_reg_cache_list[i]) {
            if (port == REG_PORT_ANY) {
                port = 0;
            }
            if (index < 0) {
                index = 0;
            }
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "Set cache: reg:%d port:%d index:%d "
                                    "data:0x%x%x\n"), reg, port, index,
                         COMPILER_64_HI(data), COMPILER_64_LO(data)));
            _soc_ser_reg_cache[unit]->reg_cache[i].data[port][index] = data;
            break;
        }
    }
    return SOC_E_NONE;
}

int
soc_ser_reg32_cache_set(int unit, soc_reg_t reg, int port, int index, uint32 data)
{
    uint64 val;

    if (_soc_ser_reg_cache[unit] == NULL) {
        return SOC_E_NONE;
    }
    COMPILER_64_SET(val, 0, data);
    return soc_ser_reg_cache_set(unit, reg, port, index, val);
}

int
soc_ser_reg_cache_get(int unit, soc_reg_t reg, int port, int index, uint64 *data)
{
    int i;

    if (_soc_ser_reg_cache[unit] == NULL) {
        return SOC_E_UNAVAIL;
    }
    switch (reg) {
    _SOC_SER_CACHE_REG_CASES
        break;
    default:
        return SOC_E_UNAVAIL;
    }
    assert(port < _SOC_MAX_PORTS_NUMS);
    assert(index < _SOC_SER_REG_INDEX_MAX);
    if (port >= _SOC_MAX_PORTS_NUMS) {
        return SOC_E_PARAM;
    }
    if (index >= _SOC_SER_REG_INDEX_MAX) {
        return SOC_E_PARAM;
    }
    for (i = 0; i < SOC_SER_REG_CACHE_MAX; i++) {
        if (reg == _soc_ser_reg_cache_list[i]) {
            if (port == REG_PORT_ANY) {
                port = 0;
            }
            if (index < 0) {
                index = 0;
            }
            *data = _soc_ser_reg_cache[unit]->reg_cache[i].data[port][index];
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "Get cache: reg:%d port:%d index:%d "
                                    "data:0x%x%x\n"), reg, port, index,
                         COMPILER_64_HI(*data), COMPILER_64_LO(*data)));
            break;
        }
    }
    return SOC_E_NONE;
}

int
soc_ser_reg32_cache_get(int unit, soc_reg_t reg, int port, int index, uint32 *data)
{
    uint64 val;

    if (_soc_ser_reg_cache[unit] == NULL) {
        return SOC_E_UNAVAIL;
    }
    COMPILER_64_ZERO(val);
    SOC_IF_ERROR_RETURN(soc_ser_reg_cache_get(unit, reg, port, index, &val));
    *data = COMPILER_64_LO(val);
    return SOC_E_NONE;
}

int SOC_REG_IS_DYNAMIC(int unit, soc_reg_t reg)
{
    switch (reg) {
    _SOC_SER_CLEAR_REG_CASES
        return TRUE;
    default: return FALSE;
    }
}

int
soc_ser_reg_clear(int unit, soc_reg_t reg, int port, int index, uint64 data)
{
    if (SOC_REG_IS_DYNAMIC(unit, reg)) {
        SOC_IF_ERROR_RETURN(soc_reg_set_nocache(unit, reg, port, index, data));
    }
    return SOC_E_NONE;
}

void
soc_ser_function_register(int unit, soc_ser_functions_t *functions)
{
    _soc_ser_functions[unit] = functions;
}

void
soc_oam_event_function_register(int unit, soc_oam_event_functions_t* functions)
{
    _soc_oam_event_functions[unit] = functions;
}


int
soc_ser_populate_tcam_log(int unit, soc_mem_t mem, soc_acc_type_t target_pipe, int index)
{
    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_populate_tcam_log_f != NULL)) {
        return (*(_soc_ser_functions[unit]->_soc_ser_populate_tcam_log_f))
            (unit, mem, target_pipe, index);
    }
    /*Should return 0 as the invalid log id*/
    return 0;
}

int
soc_ser_stat_error(int unit, int port)
{
    int fixed = 0;
    soc_stat_t *stat = SOC_STAT(unit);

    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_stat_nack_f != NULL)) {
        (*(_soc_ser_functions[unit]->_soc_ser_stat_nack_f))(unit, &fixed);
    } else {
        return SOC_E_UNAVAIL;
    }

    if (!fixed) {
        return SOC_E_INTERNAL;
    }
    stat->ser_err_stat++;
    return SOC_E_NONE;
}

int
soc_process_ser_parity_error(int unit,
                             _soc_ser_parity_info_t *_ser_parity_info,
                             int parity_err_type)
{
    int info_ix, index_min, index_max, ser_mem_count, copyno, mem_index, rv;
    uint32 start_addr, end_addr;
    uint32 fail_count, addr;
    uint32 pipe_acc;
    _soc_ser_parity_info_t *cur_spi;
    _soc_ser_correct_info_t spci = {0};
    soc_stat_t *stat = SOC_STAT(unit);

    /* coverity[result_independent_of_operands] */
    SOC_IF_ERROR_RETURN(READ_CMIC_SER_FAIL_CNTr(unit, &fail_count));

    if (!fail_count) {
        LOG_ERROR(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "SER parity failure without valid count\n")));
    } else {
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(READ_CMIC_SER_FAIL_ENTRYr(unit, &addr));

        info_ix = 0;
        while (_ser_parity_info[info_ix].mem != INVALIDm) {
            cur_spi = &(_ser_parity_info[info_ix]);
            index_min = soc_mem_index_min(unit, cur_spi->mem);
            index_max = soc_mem_index_max(unit, cur_spi->mem);
            pipe_acc = cur_spi->ser_flags & _SOC_SER_FLAG_ACC_TYPE_MASK;
            ser_mem_count = index_max - index_min + 1;
            if (!ser_mem_count) {
                info_ix++;
                continue;
            }
            SOC_MEM_BLOCK_ITER(unit, cur_spi->mem, copyno) {
                break;
            }
            start_addr =
                soc_mem_addr(unit, cur_spi->mem, 0, copyno, index_min);
            end_addr =
                soc_mem_addr(unit, cur_spi->mem, 0, copyno, index_max);

            if (0 != pipe_acc) {
                /* Override ACC_TYPE in addresses */
                start_addr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                                _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
                start_addr |= (pipe_acc & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                    _SOC_MEM_ADDR_ACC_TYPE_SHIFT;

                end_addr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                                _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
                end_addr |= (pipe_acc & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                    _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
            }

            if ((addr >= start_addr) && (addr <= end_addr)) {
                /* Addr in memory range */
                mem_index = (addr - start_addr) + index_min;
#ifdef BCM_KATANA_SUPPORT
                if (0 != (cur_spi->ser_flags & _SOC_SER_FLAG_REMAP_READ)) {
                    if (SOC_IS_KATANA(unit)) {
                        mem_index =
                            soc_kt_l3_defip_index_remap(unit, cur_spi->mem, 
                                                         mem_index);
                    }
                }
#endif /* BCM_KATANA_SUPPORT */
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "%s entry %d TCAM parity error\n"),
                           SOC_MEM_NAME(unit, cur_spi->mem),
                           mem_index));
                soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                   parity_err_type, addr,
                                   0);
                stat->ser_err_tcam++;
                spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
                spci.reg = INVALIDr;
                spci.mem = cur_spi->mem;
                spci.blk_type = copyno;
                spci.index = mem_index;
                spci.detect_time = sal_time_usecs();
                spci.log_id = soc_ser_populate_tcam_log(unit,
                    spci.mem, pipe_acc, spci.index);
                rv = soc_ser_correction(unit, &spci);
                if (spci.log_id != 0) {
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                            SOC_SWITCH_EVENT_DATA_ERROR_LOG,
                            spci.log_id, 0);
                }
                if (SOC_FAILURE(rv)) {
                    /* Report failed to correct event flag to
                     * application */
                    soc_event_generate(unit,
                                SOC_SWITCH_EVENT_PARITY_ERROR,
                                SOC_SWITCH_EVENT_DATA_ERROR_FAILEDTOCORRECT,
                                addr,
                                0);
                    return rv;
                }
                break;
            }
            info_ix++;
        }
    }
	/* coverity[result_independent_of_operands] */
    SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_FAIL_ENTRYr(unit, 0));
    SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_FAIL_CNTr(unit, 0));
    return SOC_E_NONE;
}

int
soc_process_cmicm_ser_parity_error(int unit,
                                   _soc_ser_parity_info_t *_ser_parity_info,
                                   int parity_err_type)
{
    uint8 at, fail = 0;
    int info_ix, index_min, index_max, phys_index_min, phys_index_max;
    int ser_mem_count, copyno, mem_index, rv;
    uint32 start_addr, end_addr;
    uint32 fail_count, addr;
    _soc_ser_parity_info_t *cur_spi;
    _soc_ser_correct_info_t spci = {0};
    soc_stat_t *stat = SOC_STAT(unit);

    /* First work on ser0 */
    /*coverity[result_independent_of_operands]*/
    SOC_IF_ERROR_RETURN(READ_CMIC_SER0_FAIL_CNTr(unit, &fail_count));
    if (fail_count) {
        fail++;
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(READ_CMIC_SER0_FAIL_ENTRYr(unit, &addr));
        info_ix = 0;
        while (_ser_parity_info[info_ix].mem != INVALIDm) {
            cur_spi = &(_ser_parity_info[info_ix]);
            if (cur_spi->cmic_ser_id == 1) {
                info_ix++;
                continue;
            }
            phys_index_min = index_min =
                soc_mem_index_min(unit, cur_spi->mem);
            phys_index_max = index_max =
                soc_mem_index_max(unit, cur_spi->mem);
            if (0 != (cur_spi->ser_flags & _SOC_SER_FLAG_REMAP_READ)) {
#ifdef BCM_TRIUMPH3_SUPPORT
                if (SOC_IS_TRIUMPH3(unit)) {
                    phys_index_min =
                        soc_tr3_l3_defip_index_map(unit,
                                                   cur_spi->mem,
                                                   index_min);
                    phys_index_max =
                        soc_tr3_l3_defip_index_map(unit,
                                                   cur_spi->mem,
                                                   index_max);
                }
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
                if (SOC_IS_KATANA2(unit)) {
                    phys_index_min =
                        soc_kt2_l3_defip_index_map(unit,
                                                   cur_spi->mem,
                                                   index_min);
                    phys_index_max =
                        soc_kt2_l3_defip_index_map(unit,
                                                   cur_spi->mem,
                                                   index_max);
                }
#endif /* BCM_KATANA2_SUPPORT */
            }
            ser_mem_count = phys_index_max - phys_index_min + 1;
            if (!ser_mem_count) {
                info_ix++;
                continue;
            }
            SOC_MEM_BLOCK_ITER(unit, cur_spi->mem, copyno) {
                break;
            }
            start_addr =
                soc_mem_addr_get(unit, cur_spi->mem, 0, copyno,
                                 phys_index_min, &at);
            end_addr =
                soc_mem_addr_get(unit, cur_spi->mem, 0, copyno,
                                 phys_index_max, &at);
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "Mem: %s addr: %x start: %x end: %x\n"),
                         SOC_MEM_NAME(unit, cur_spi->mem), addr,
                         start_addr, end_addr));

            if ((addr >= start_addr) && (addr <= end_addr)) {
                /* Addr in memory range */
                mem_index = (addr - start_addr) + phys_index_min;
                if (0 != (cur_spi->ser_flags & _SOC_SER_FLAG_REMAP_READ)) {
                    /* Now, mem_index is the physical index.
                     * The correction routines expect the logical index.
                     * Reverse the mapping.
                     */
#ifdef BCM_TRIUMPH3_SUPPORT
                    if (SOC_IS_TRIUMPH3(unit)) {
                        mem_index =
                            soc_tr3_l3_defip_index_remap(unit, cur_spi->mem,
                                                         mem_index);
                    }
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
                    if (SOC_IS_KATANA2(unit)) {
                        mem_index =
                            soc_kt2_l3_defip_index_remap(unit, cur_spi->mem,
                                                         mem_index);
                    }
#endif /* BCM_KATANA2_SUPPORT */
                }
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "ser0 %s entry %d TCAM parity error\n"),
                           SOC_MEM_NAME(unit, cur_spi->mem),
                           mem_index));
                soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                   SOC_SWITCH_EVENT_DATA_ERROR_PARITY,
                                   cur_spi->mem | SOC_SER_ERROR_DATA_ID_OFFSET_SET,
                                   mem_index);
                stat->ser_err_tcam++;
                spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
                spci.reg = INVALIDr;
                spci.mem = cur_spi->mem;
                spci.blk_type = copyno;
                spci.index = mem_index;
                spci.detect_time = sal_time_usecs();
                spci.log_id = soc_ser_log_create_entry(unit,
                    sizeof(soc_ser_log_tlv_generic_t) +
                    sizeof(soc_ser_log_tlv_memory_t) +
                    sizeof(soc_ser_log_tlv_hdr_t) *3);
                rv = soc_ser_correction(unit, &spci);
                if (spci.log_id != 0) {
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                       SOC_SWITCH_EVENT_DATA_ERROR_LOG,
                                       spci.log_id, 0);
                }
                if (SOC_FAILURE(rv)) {
                    /* Report failed to correct event flag to
                     * application */
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                            SOC_SWITCH_EVENT_DATA_ERROR_FAILEDTOCORRECT,
                            cur_spi->mem | SOC_SER_ERROR_DATA_ID_OFFSET_SET,
                            mem_index);
                    return rv;
                }
                break;
            }
            info_ix++;
        }
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_FAIL_ENTRYr(unit, 0));
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_FAIL_CNTr(unit, 0));
    }

    /* Then work on ser1 */
    /*coverity[result_independent_of_operands]*/
    SOC_IF_ERROR_RETURN(READ_CMIC_SER1_FAIL_CNTr(unit, &fail_count));
    if (fail_count) {
        fail++;
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(READ_CMIC_SER1_FAIL_ENTRYr(unit, &addr));
        info_ix = 0;
        while (_ser_parity_info[info_ix].mem != INVALIDm) {
            cur_spi = &(_ser_parity_info[info_ix]);
            if (cur_spi->cmic_ser_id == 0) {
                info_ix++;
                continue;
            }
            index_min = soc_mem_index_min(unit, cur_spi->mem);
            index_max = soc_mem_index_max(unit, cur_spi->mem);
            SOC_MEM_BLOCK_ITER(unit, cur_spi->mem, copyno) {
                break;
            }
            start_addr =
                soc_mem_addr_get(unit, cur_spi->mem, 0, copyno, index_min, &at);
            end_addr =
                soc_mem_addr_get(unit, cur_spi->mem, 0, copyno, index_max, &at);

            if ((addr >= start_addr) && (addr <= end_addr)) {
                /* Addr in memory range */
                mem_index = (addr - start_addr) + index_min;
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "ser1 %s entry %d TCAM parity error\n"),
                           SOC_MEM_NAME(unit, cur_spi->mem),
                           mem_index));
                soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                   SOC_SWITCH_EVENT_DATA_ERROR_PARITY,
                                   cur_spi->mem | SOC_SER_ERROR_DATA_ID_OFFSET_SET,
                                   mem_index);
                stat->ser_err_tcam++;
                sal_memset(&spci, 0, sizeof(spci));
                spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
                spci.mem = cur_spi->mem;
                spci.blk_type = copyno;
                spci.index = mem_index;
                spci.detect_time = sal_time_usecs();
                spci.log_id = soc_ser_log_create_entry(unit,
                    sizeof(soc_ser_log_tlv_generic_t) +
                    sizeof(soc_ser_log_tlv_memory_t) +
                    sizeof(soc_ser_log_tlv_hdr_t) *3);
                rv = soc_ser_correction(unit, &spci);
                if (spci.log_id != 0) {
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                       SOC_SWITCH_EVENT_DATA_ERROR_LOG,
                                       spci.log_id, 0);
                }
                if (SOC_FAILURE(rv)) {
                    /* Report failed to correct event flag to
                     * application */
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                            SOC_SWITCH_EVENT_DATA_ERROR_FAILEDTOCORRECT,
                            cur_spi->mem | SOC_SER_ERROR_DATA_ID_OFFSET_SET,
                            mem_index);
                    return rv;
                }
                break;
            }
            info_ix++;
        }
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_FAIL_ENTRYr(unit, 0));
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_FAIL_CNTr(unit, 0));
    }
    if (!fail) {
        LOG_ERROR(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "SER parity failure without valid count\n")));
    }
    return SOC_E_NONE;
}

int
soc_ser_mem_nack(void *unit_vp, void *addr_vp, void *d2,
                 void *d3, void *d4)
{
    int unit = PTR_TO_INT(unit_vp);

    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_mem_nack_f != NULL)) {
        sal_dpc(*(_soc_ser_functions[unit]->_soc_ser_mem_nack_f), unit_vp,
                addr_vp, d2, d3, d4);
        return TRUE;
    }

    return FALSE;
}

int
soc_ser_parity_error_intr(int unit)
{
    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_parity_error_intr_f != NULL)) {
        soc_intr_disable(unit, IRQ_MEM_FAIL);
        sal_dpc(*(_soc_ser_functions[unit]->_soc_ser_parity_error_intr_f),
                INT_TO_PTR(unit), 0, 0, 0, 0);
        return TRUE;
    } else if ((_soc_oam_event_functions[unit] != NULL) &&
               (_soc_oam_event_functions[unit]->_soc_oam_event_intr_f != NULL)) {
        soc_intr_disable(unit, IRQ_MEM_FAIL);
        sal_dpc(*(_soc_oam_event_functions[unit]->_soc_oam_event_intr_f),
                INT_TO_PTR(unit), 0, 0, 0, 0);
        return TRUE;
    }
    return FALSE;
}

int
soc_ser_parity_error_cmicm_intr(void *unit_vp, void *d1, void *d2,
                                void *d3, void *d4)
{
    int unit = PTR_TO_INT(unit_vp);

    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_parity_error_cmicm_intr_f != NULL)) {
        sal_dpc(*(_soc_ser_functions[unit]->_soc_ser_parity_error_cmicm_intr_f),
                unit_vp, d1, d2, d3, d4);
        return TRUE;
    } else if ((_soc_oam_event_functions[unit] != NULL) &&
               (_soc_oam_event_functions[unit]->_soc_oam_event_intr_f != NULL)) {
        sal_dpc(*(_soc_oam_event_functions[unit]->_soc_oam_event_intr_f),
                unit_vp, d1, d2, d3, d4);
        return TRUE;
    }

    return FALSE;
}

void
soc_ser_fail(void *unit_vp, void *addr_vp, void *d2,
             void *d3, void *d4)

{
    int unit = PTR_TO_INT(unit_vp);

    if ((_soc_ser_functions[unit] != NULL) &&
        (_soc_ser_functions[unit]->_soc_ser_fail_f != NULL)) {
        (*(_soc_ser_functions[unit]->_soc_ser_fail_f))(unit);
    }

    return;
}

STATIC int
_soc_ser_granularity(_soc_ser_parity_mode_t parity_mode)
{
    int ser_mem_granularity;

    switch (parity_mode) {
    case _SOC_SER_PARITY_MODE_2BITS:
        ser_mem_granularity = 2;
        break;
    case _SOC_SER_PARITY_MODE_4BITS:
        ser_mem_granularity = 4;
        break;
    case _SOC_SER_PARITY_MODE_8BITS:
        ser_mem_granularity = 8;
        break;
    case _SOC_SER_PARITY_MODE_1BIT:
    default:
        ser_mem_granularity = 1;
        break;
    }
    return ser_mem_granularity;
}

int
soc_ser_mem_clear(int unit, _soc_ser_parity_info_t *_ser_parity_info,
                  soc_mem_t mem)
{
    int info_ix, ser_mem_count, ser_mem_granularity;
    uint32 addr_valid = 0;
    _soc_ser_parity_info_t *cur_spi;

    /* coverity[result_independent_of_operands] */
    SOC_IF_ERROR_RETURN
        (READ_CMIC_SER_PROTECT_ADDR_RANGE_VALIDr(unit, &addr_valid));

    if (!addr_valid) {
        /* No SER protection, do nothing */
        return SOC_E_NONE;
    }

    info_ix = 0;
    while (_ser_parity_info[info_ix].mem != INVALIDm) {
        if (_ser_parity_info[info_ix].mem == mem) {
            break;
        }
        info_ix++;
    }

    if ((_ser_parity_info[info_ix].mem != INVALIDm) &&
        (addr_valid & (1 << info_ix))) {
        cur_spi = &(_ser_parity_info[info_ix]);

        addr_valid &= ~(1 << info_ix);
        /* Disable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        ser_mem_granularity =
            _soc_ser_granularity(cur_spi->parity_mode);

        /* Flush SER memory segment for the table */
        for (ser_mem_count = cur_spi->ser_section_start;
             ser_mem_count < cur_spi->ser_section_end;
             ser_mem_count += ser_mem_granularity) {
            /* coverity[result_independent_of_operands] */
            SOC_IF_ERROR_RETURN
                (WRITE_CMIC_SER_MEM_ADDRr(unit, ser_mem_count));
            /* coverity[result_independent_of_operands] */
            SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_MEM_DATAr(unit, 0));
        }

        addr_valid |= (1 << info_ix);
        /* Enable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "\t%s: SER[%d-%d]\n"),
                     SOC_MEM_NAME(unit, cur_spi->mem),
                     cur_spi->ser_section_start, cur_spi->ser_section_end));
    }

    return SOC_E_NONE;
}

int
soc_cmicm_ser_mem_clear(int unit, _soc_ser_parity_info_t *_ser_parity_info,
                        soc_mem_t mem)
{
    int info_ix, ser_mem_count, ser_mem_granularity;
    uint32 addr_valid = 0;
    _soc_ser_parity_info_t *cur_spi;

    /* First work on ser0 */
    SOC_IF_ERROR_RETURN
        (READ_CMIC_SER0_PROTECT_ADDR_RANGE_VALIDr(unit, &addr_valid));

    if (!addr_valid) {
        /* No SER protection, do nothing */
        goto check_ser1;
    }

    info_ix = 0;
    while (_ser_parity_info[info_ix].mem != INVALIDm) {
        if ((_ser_parity_info[info_ix].mem == mem) &&
            (_ser_parity_info[info_ix].cmic_ser_id == 0)) {
            break;
        }
        info_ix++;
    }

    if ((_ser_parity_info[info_ix].mem != INVALIDm) &&
        (addr_valid & (1 << info_ix))) {
        cur_spi = &(_ser_parity_info[info_ix]);

        addr_valid &= ~(1 << info_ix);
        /* Disable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER0_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        ser_mem_granularity =
            _soc_ser_granularity(cur_spi->parity_mode);

        /* Flush SER memory segment for the table */
        for (ser_mem_count = cur_spi->ser_section_start;
             ser_mem_count < cur_spi->ser_section_end;
             ser_mem_count += ser_mem_granularity) {
            SOC_IF_ERROR_RETURN
                (WRITE_CMIC_SER0_MEM_ADDRr(unit, ser_mem_count));
            SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_MEM_DATAr(unit, 0));
        }

        addr_valid |= (1 << info_ix);
        /* Enable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER0_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "\t%s: SER0[%d-%d]\n"),
                     SOC_MEM_NAME(unit, cur_spi->mem),
                     cur_spi->ser_section_start, cur_spi->ser_section_end));
    }

check_ser1:
    /* Then work on ser1 */
    /* coverity[result_independent_of_operands] */
    SOC_IF_ERROR_RETURN
        (READ_CMIC_SER1_PROTECT_ADDR_RANGE_VALIDr(unit, &addr_valid));

    if (!addr_valid) {
        /* No SER protection, do nothing */
        return SOC_E_NONE;
    }

    info_ix = 0;
    while (_ser_parity_info[info_ix].mem != INVALIDm) {
        if ((_ser_parity_info[info_ix].mem == mem) &&
            (_ser_parity_info[info_ix].cmic_ser_id == 1)) {
            break;
        }
        info_ix++;
    }

    if ((_ser_parity_info[info_ix].mem != INVALIDm) &&
        (addr_valid & (1 << info_ix))) {
        cur_spi = &(_ser_parity_info[info_ix]);

        addr_valid &= ~(1 << info_ix);
        /* Disable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER0_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        ser_mem_granularity =
            _soc_ser_granularity(cur_spi->parity_mode);

        /* Flush SER memory segment for the table */
        for (ser_mem_count = cur_spi->ser_section_start;
             ser_mem_count < cur_spi->ser_section_end;
             ser_mem_count += ser_mem_granularity) {
            /* coverity[result_independent_of_operands] */
            SOC_IF_ERROR_RETURN
                (WRITE_CMIC_SER1_MEM_ADDRr(unit, ser_mem_count));
            /* coverity[result_independent_of_operands] */
            SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_MEM_DATAr(unit, 0));
        }

        addr_valid |= (1 << info_ix);
        /* Enable SER protection on this memory */
        /*coverity[result_independent_of_operands]*/
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER1_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid));

        LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "\t%s: SER0[%d-%d]\n"),
                     SOC_MEM_NAME(unit, cur_spi->mem),
                     cur_spi->ser_section_start, cur_spi->ser_section_end));
    }
    return SOC_E_NONE;
}

int
soc_ser_init(int unit, _soc_ser_parity_info_t *_ser_parity_info, int max_mem)
{
    int info_ix, o_info_ix;
    int index_min, index_max, copyno, ser_mem_granularity;
    int i, ser_mem_count, ser_mem_total = 0;
    uint32 start_addr, end_addr, ser_mem_addr = 0, reg_addr;
    uint32 addr_valid0 = 0;
    uint32 acc_type;
    uint8 wb_flag = 0;
#ifdef BCM_CMICM_SUPPORT
    uint8 at;
    uint32 addr_valid1 = 0, ser_mem_addr0 = 0, ser_mem_addr1 = 0;
    uint32 ser_mem_total0 = 0, ser_mem_total1 = 0;
#endif /* BCM_CMICM_SUPPORT */
    _soc_ser_parity_info_t *cur_spi, *multi_pipe_spi;

    LOG_VERBOSE(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "Unit %d: SER parity initialization:\n"), unit));

    soc_ser_function_register(unit, NULL);

    info_ix = 0;
    while (_ser_parity_info[info_ix].mem != INVALIDm) {
        cur_spi = &(_ser_parity_info[info_ix]);

        if (cur_spi->ser_flags & _SOC_SER_FLAG_SW_COMPARE) {
            if (SOC_MEM_INFO(unit, cur_spi->mem).flags & SOC_MEM_FLAG_BE) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "\tSW SER init of Big Endian TCAMs is not supported: %s\n"),
                           SOC_MEM_NAME(unit, cur_spi->mem)));
                return SOC_E_INTERNAL;
            }

            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tSkipping HW SER init of manual scan mem: %s\n"),
                         SOC_MEM_NAME(unit, cur_spi->mem)));
            SOC_MEM_INFO(unit, cur_spi->mem).flags |= SOC_MEM_FLAG_SER_ENGINE;
            info_ix++;
            continue;
        }

        index_min = soc_mem_index_min(unit, cur_spi->mem);
        index_max = soc_mem_index_max(unit, cur_spi->mem);
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT) || \
    defined(BCM_KATANA_SUPPORT)
        if (soc_feature(unit, soc_feature_l3_defip_map) &&
            ((cur_spi->mem == L3_DEFIP_PAIR_128m) ||
             (cur_spi->mem == L3_DEFIPm))) {
            /* Override SOC memory impression for overlay
             * LPM tables. */
            index_min = 0;
            index_max = (SOC_CONTROL(unit)->l3_defip_max_tcams *
                         SOC_CONTROL(unit)->l3_defip_tcam_size) - 1;
            if (cur_spi->mem == L3_DEFIP_PAIR_128m) {
                index_max /= 2;
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_KATANA2_SUPPORT */
        ser_mem_count = index_max - index_min + 1;
        if (!ser_mem_count) {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tSkipping empty mem: %s\n"),
                         SOC_MEM_NAME(unit, cur_spi->mem)));
            info_ix++;
            continue;
        }

        ser_mem_granularity = _soc_ser_granularity(cur_spi->parity_mode);
        ser_mem_count *= ser_mem_granularity;

        /* Round up to words for simplicity */
        ser_mem_count = 32 * ((ser_mem_count + 31) / 32);
        if ((ser_mem_count + ser_mem_total) > max_mem) {
            /* Can't fit requested parity bits in SER memory */
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "SER mem full: Skipping further config.\n")));
            break;
        }

        /* Parity bits fit, so on with the config */
        SOC_MEM_BLOCK_ITER(unit, cur_spi->mem, copyno) {
            break;
        }
#ifdef BCM_CMICM_SUPPORT
        if (soc_feature(unit, soc_feature_cmicm) && !SOC_IS_KATANA(unit)) {
            start_addr = soc_mem_addr_get(unit, cur_spi->mem, 0, copyno,
                                          index_min, &at);
            end_addr = soc_mem_addr_get(unit, cur_spi->mem, 0, copyno,
                                        index_max, &at);
        } else
#endif /* BCM_CMICM_SUPPORT */
        {
            start_addr = soc_mem_addr(unit, cur_spi->mem, 0, copyno, index_min);
            end_addr = soc_mem_addr(unit, cur_spi->mem, 0, copyno, index_max);
        }

        /* Flush SER memory */
#ifdef BCM_CMICM_SUPPORT
        if (soc_feature(unit, soc_feature_cmicm) && !SOC_IS_KATANA(unit)) {
            if (cur_spi->cmic_ser_id) {
                ser_mem_addr1 = ser_mem_total1; /* Previous total */
                ser_mem_total1 += ser_mem_count; /* New total */
                if (!SOC_IS_RELOADING(unit) && !SOC_WARM_BOOT(unit)) {
                    for (i=ser_mem_addr1; i<ser_mem_total1; i++) {
                        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_MEM_ADDRr(unit, i));
                        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_MEM_DATAr(unit, 0));
                    }
                }
                /* Record section for mem_clear use */
                cur_spi->ser_section_start = ser_mem_addr1;
                cur_spi->ser_section_end = ser_mem_total1 - ser_mem_granularity;
            } else {
                ser_mem_addr0 = ser_mem_total0; /* Previous total */
                ser_mem_total0 += ser_mem_count; /* New total */
                if (!SOC_IS_RELOADING(unit) && !SOC_WARM_BOOT(unit)) {
                    for (i=ser_mem_addr0; i<ser_mem_total0; i++) {
                        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_MEM_ADDRr(unit, i));
                        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_MEM_DATAr(unit, 0));
                    }
                }
                /* Record section for mem_clear use */
                cur_spi->ser_section_start = ser_mem_addr0;
                cur_spi->ser_section_end = ser_mem_total0 - ser_mem_granularity;
            }
            ser_mem_total += ser_mem_count; /* New total */
        } else
#endif /* BCM_CMICM_SUPPORT */
        {
            ser_mem_addr = ser_mem_total; /* Previous total */
            ser_mem_total += ser_mem_count; /* New total */
            if (!SOC_IS_RELOADING(unit) && !SOC_WARM_BOOT(unit)) {
                for (i=ser_mem_addr; i<ser_mem_total; i++) {
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_MEM_ADDRr(unit, i));
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_MEM_DATAr(unit, 0));
                }
            }
            /* Record section for mem_clear use */
            cur_spi->ser_section_start = ser_mem_addr;
            cur_spi->ser_section_end = ser_mem_total - ser_mem_granularity;
        }

        /* For multiple pipeline units, different pipeline's hardware
         * instances are duplicates.  The SER parity bits are thus
         * shared, but the configuration registers in the CMIC must be
         * different to capture the different SBUS ranges of the
         * read accesses.
         */
        multi_pipe_spi = cur_spi;
        o_info_ix = info_ix;
        if (SOC_WARM_BOOT(unit) && SOC_IS_KATANA(unit)) {
            wb_flag = 1;
        }
        while (cur_spi->mem == multi_pipe_spi->mem) {
            if (cur_spi != multi_pipe_spi) {
                info_ix++; /* Advance the index */
                if (0 != (multi_pipe_spi->ser_flags &
                          _SOC_SER_FLAG_MULTI_PIPE)) {
                    /* Get the non-default pipeline access type */
                    acc_type = multi_pipe_spi->ser_flags &
                        _SOC_SER_FLAG_ACC_TYPE_MASK;

                    /* Override ACC_TYPE in address */
                    /* Note, this function does not support the
                     * extended sbus addressing for multiple pipeline
                     * devices.
                     * soc_generic_ser_init _must_ be used
                     * for such devices.
                     */
                    start_addr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                                    _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
                    start_addr |=
                        (acc_type & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                        _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
                    end_addr &= ~(_SOC_MEM_ADDR_ACC_TYPE_MASK <<
                                    _SOC_MEM_ADDR_ACC_TYPE_SHIFT);
                    end_addr |=
                        (acc_type & _SOC_MEM_ADDR_ACC_TYPE_MASK) <<
                        _SOC_MEM_ADDR_ACC_TYPE_SHIFT;
                } else {
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "SER mem muliple pipeline mismatch %s vs. %s.\n"),
                                 SOC_MEM_NAME(unit, cur_spi->mem),
                                 SOC_MEM_NAME(unit, multi_pipe_spi->mem)));
                    return SOC_E_INTERNAL;
                }
            }

            reg_addr =
                soc_reg_addr(unit, multi_pipe_spi->start_addr_reg,
                             REG_PORT_ANY, 0);
            if (!wb_flag) {
                /* coverity[result_independent_of_operands] */
                SOC_IF_ERROR_RETURN(soc_pci_write(unit, reg_addr, start_addr));
            }
            reg_addr =
                soc_reg_addr(unit, multi_pipe_spi->end_addr_reg,
                             REG_PORT_ANY, 0);
            if (!wb_flag) {
                /* coverity[result_independent_of_operands] */
                SOC_IF_ERROR_RETURN(soc_pci_write(unit, reg_addr, end_addr));
            }
            reg_addr =
                soc_reg_addr(unit, multi_pipe_spi->cmic_mem_addr_reg,
                             REG_PORT_ANY, 0);


#ifdef BCM_CMICM_SUPPORT
            if (soc_feature(unit, soc_feature_cmicm) &&
                !SOC_IS_KATANA(unit)) {
                if (multi_pipe_spi->cmic_ser_id) {
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN(soc_pci_write(unit, reg_addr,
                                                      ser_mem_addr1));
                    addr_valid1 |= (1 << info_ix);
                } else {
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN(soc_pci_write(unit, reg_addr,
                                                      ser_mem_addr0));
                    addr_valid0 |= (1 << info_ix);
                }
            } else
#endif /* BCM_CMICM_SUPPORT */
            {
                if (!wb_flag) {
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN(soc_pci_write(unit, reg_addr,
                                                  ser_mem_addr));
                }
                addr_valid0 |= (1 << info_ix);
            }

            if (multi_pipe_spi->bit_offset != -1) {
                if (!wb_flag) {
                    /* coverity[result_independent_of_operands] */
                    SOC_IF_ERROR_RETURN
                    (soc_pci_write(unit, soc_reg_addr(unit,
                                         multi_pipe_spi->entry_len_reg,
                                         REG_PORT_ANY, 0),
                                   multi_pipe_spi->bit_offset));
                }
            }
            SOC_IF_ERROR_RETURN
                (soc_reg_field32_modify(unit, multi_pipe_spi->parity_mode_reg,
                                        REG_PORT_ANY, multi_pipe_spi->parity_mode_field,
                                        multi_pipe_spi->parity_mode));

            multi_pipe_spi = &(_ser_parity_info[info_ix + 1]);
        }

#ifdef BCM_CMICM_SUPPORT
        if (soc_feature(unit, soc_feature_cmicm) && !SOC_IS_KATANA(unit)) {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\t%s(%d-%d): Sbus range (0x%08x-0x%08x) SER[%d][%d](0x%03x)\n"),
                         SOC_MEM_NAME(unit, cur_spi->mem),
                         index_min, index_max, start_addr, end_addr,
                         cur_spi->cmic_ser_id, o_info_ix,
                         cur_spi->cmic_ser_id ?
                         ser_mem_addr1 : ser_mem_addr0));
        } else
#endif /* BCM_CMICM_SUPPORT */
        {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\t%s(%d-%d): Sbus range (0x%08x-0x%08x) SER[%d](0x%03x)\n"),
                         SOC_MEM_NAME(unit, cur_spi->mem),
                         index_min, index_max, start_addr, end_addr,
                         o_info_ix, ser_mem_addr));
        }
        if (cur_spi->bit_offset != -1) {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tBits protected per entry: %d\n"),
                         cur_spi->bit_offset));
        }
        SOC_MEM_INFO(unit, cur_spi->mem).flags |= SOC_MEM_FLAG_SER_ENGINE;
        info_ix++;
    }

#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm) && !SOC_IS_KATANA(unit)) {
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_FAIL_ENTRYr(unit, 0));
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER0_FAIL_CNTr(unit, 0));
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_FAIL_ENTRYr(unit, 0));
        SOC_IF_ERROR_RETURN(WRITE_CMIC_SER1_FAIL_CNTr(unit, 0));
    } else
#endif /* BCM_CMICM_SUPPORT */
    {
        if (!wb_flag) {
            SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_FAIL_ENTRYr(unit, 0));
            SOC_IF_ERROR_RETURN(WRITE_CMIC_SER_FAIL_CNTr(unit, 0));
        }
    }

    /* Enable SER protection last */
#ifdef BCM_CMICM_SUPPORT
    if (soc_feature(unit, soc_feature_cmicm) && !SOC_IS_KATANA(unit)) {
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER0_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid0));
        /* coverity[result_independent_of_operands] */
        SOC_IF_ERROR_RETURN
            (WRITE_CMIC_SER1_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid1));
    } else
#endif /* BCM_CMICM_SUPPORT */
    {
        if (!wb_flag) {
            /* coverity[result_independent_of_operands] */
            SOC_IF_ERROR_RETURN
                (WRITE_CMIC_SER_PROTECT_ADDR_RANGE_VALIDr(unit, addr_valid0));
        }
    }
    return SOC_E_NONE;
}

STATIC int
_soc_generic_ser_granularity(_soc_ser_protection_type_t prot_type,
                             _soc_ser_protection_mode_t prot_mode,
                             __soc_ser_start_end_bits_t *bits)
{
    int i, words = 0, ser_mem_gran;

    for (i = 0; i < SOC_NUM_GENERIC_PROT_SECTIONS; i++) {
        if (bits[i].start_bit >= 0) {
            words++;
        }
    }
    if (prot_type == _SOC_SER_TYPE_PARITY) {
        switch (prot_mode) {
        case _SOC_SER_PARITY_2BITS:
            ser_mem_gran = 2;
            break;
        case _SOC_SER_PARITY_4BITS:
            ser_mem_gran = 4;
            break;
        case _SOC_SER_PARITY_8BITS:
            ser_mem_gran = 8;
            break;
        case _SOC_SER_PARITY_1BIT:
        default:
            ser_mem_gran = 1;
            break;
        }
        return ser_mem_gran;
    } else {
        switch (prot_mode) {
        case _SOC_SER_ECC_2FLD:
            ser_mem_gran = 2;
            break;
        case _SOC_SER_ECC_4FLD:
            ser_mem_gran = 4;
            break;
        case _SOC_SER_ECC_1FLD:
        default:
            ser_mem_gran = 1;
            break;
        }
        return ser_mem_gran * words * 9;
    }
}

int
soc_generic_ser_process_error(int unit, _soc_generic_ser_info_t *ser_info,
                              int err_type)
{
    uint8 at;
    int info_ix, hw_ix, acc_type, rv;
    int index_min, index_max, phys_index_min, phys_index_max;
    int ser_mem_count, copyno, mem_index;
    uint32 start_addr, end_addr, count;
    uint32 ser_err[2], addr;
    _soc_generic_ser_info_t *cur_ser_info;
    _soc_ser_correct_info_t spci = {0};
    soc_stat_t *stat = SOC_STAT(unit);
    ser_result_0_entry_t err_result;
    soc_mem_t cur_mem;
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT) || defined(BCM_APACHE_SUPPORT)
    uint8 *corrupt = NULL;
    int is_l3_defip = FALSE;
#endif
#if defined(ALPM_ENABLE)
    uint8 _alpm_lock = 0;
#endif /* ALPM_ENABLE */
    SOC_IF_ERROR_RETURN(READ_SER_ERROR_0r(unit, &ser_err[0]));
    SOC_IF_ERROR_RETURN(READ_SER_ERROR_1r(unit, &ser_err[1]));

    if (!soc_reg_field_get(unit, SER_ERROR_0r, ser_err[0], ERROR_0_VALf) &&
        !soc_reg_field_get(unit, SER_ERROR_1r, ser_err[1], ERROR_1_VALf)) {
        LOG_ERROR(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "SER parity failure without valid error !!\n")));
        return SOC_E_NONE;
    }
    do {
        if (soc_reg_field_get(unit, SER_ERROR_0r, ser_err[0],
                              ERROR_0_VALf)) {
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, SER_RESULT_0m, MEM_BLOCK_ANY,
                              0, &err_result));
            hw_ix = soc_mem_field32_get(unit, SER_RESULT_0m,
                                        &err_result, RANGEf);
            addr = soc_mem_field32_get(unit, SER_RESULT_0m,
                                       &err_result, SBUS_ADDRf);
            acc_type = soc_mem_field32_get(unit, SER_RESULT_0m,
                                       &err_result, ACC_TYPEf);
            SOC_IF_ERROR_RETURN(WRITE_SER_ERROR_0r(unit, 0));
        } else {
            SOC_IF_ERROR_RETURN
                (soc_mem_read(unit, SER_RESULT_1m, MEM_BLOCK_ANY,
                              0, &err_result));
            hw_ix = soc_mem_field32_get(unit, SER_RESULT_1m,
                                        &err_result, RANGEf);
            addr = soc_mem_field32_get(unit, SER_RESULT_1m,
                                       &err_result, SBUS_ADDRf);
            acc_type = soc_mem_field32_get(unit, SER_RESULT_1m,
                                       &err_result, ACC_TYPEf);
            SOC_IF_ERROR_RETURN(WRITE_SER_ERROR_1r(unit, 0));
        }

        info_ix = 0;
        while (ser_info[info_ix].mem != INVALIDm) {
            if (0 == (ser_info[info_ix].ser_flags & _SOC_SER_FLAG_SW_COMPARE)) {
                if ((ser_info[info_ix].ser_hw_index != -1) &&
                    (ser_info[info_ix].ser_hw_index == hw_ix)) {
                    break;
                }
            }
            info_ix++;
        }
        if (ser_info[info_ix].mem == INVALIDm) {
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "SER parity failure with invalid mem range !!\n")));
            return SOC_E_NONE;
        }
        cur_ser_info = &(ser_info[info_ix]);
        phys_index_min = index_min =
            soc_mem_index_min(unit, cur_ser_info->mem);
        phys_index_max = index_max =
            soc_mem_index_max(unit, cur_ser_info->mem);
        if (0 != (cur_ser_info->ser_flags & _SOC_SER_FLAG_REMAP_READ)) {
#ifdef BCM_TRIDENT2_SUPPORT
            if (SOC_IS_TD2_TT2(unit)) {
                phys_index_min =
                    soc_trident2_l3_defip_index_map(unit,
                                                    cur_ser_info->mem,
                                                    index_min);
                phys_index_max =
                    soc_trident2_l3_defip_index_map(unit,
                                                    cur_ser_info->mem,
                                                    index_max);
            }
#endif /* BCM_TRIDENT2_SUPPORT */
#ifdef BCM_APACHE_SUPPORT
            if (SOC_IS_APACHE(unit)) {
                phys_index_min =
                    soc_apache_l3_defip_index_map(unit,
                                                  cur_ser_info->mem,
                                                  index_min);
                phys_index_max =
                    soc_apache_l3_defip_index_map(unit,
                                                  cur_ser_info->mem,
                                                  index_max);
            }
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_HELIX4_SUPPORT
            if (SOC_IS_HELIX4(unit)) {
                /* Helix4 uses the Triumph3 mapping logic */
                phys_index_min =
                    soc_tr3_l3_defip_index_map(unit,
                                               cur_ser_info->mem,
                                               index_min);
                phys_index_max =
                    soc_tr3_l3_defip_index_map(unit,
                                               cur_ser_info->mem,
                                               index_max);
            }
#endif /* BCM_HELIX4_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
            if (SOC_IS_KATANA2(unit)) {
                phys_index_min =
                    soc_kt2_l3_defip_index_map(unit,
                                               cur_ser_info->mem,
                                               index_min);
                phys_index_max =
                    soc_kt2_l3_defip_index_map(unit,
                                               cur_ser_info->mem,
                                               index_max);
            }
#endif /* BCM_KATANA2_SUPPORT */
        }
        ser_mem_count = phys_index_max - phys_index_min + 1;
        if (!ser_mem_count) {
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "SER parity failure with unavailable mem range !!\n")));
            return SOC_E_NONE;
        }
        SOC_MEM_BLOCK_ITER(unit, cur_ser_info->mem, copyno) {
            break;
        }
        start_addr = soc_mem_addr_get(unit, cur_ser_info->mem, 0, copyno,
                                       phys_index_min, &at);
        end_addr = soc_mem_addr_get(unit, cur_ser_info->mem, 0, copyno,
                                     phys_index_max, &at);
        if ((addr >= start_addr) && (addr <= end_addr)) {
            /* Addr in memory range */
            mem_index = (addr - start_addr) + phys_index_min;
            if (0 != (cur_ser_info->ser_flags & _SOC_SER_FLAG_REMAP_READ)) {
                /* Now, mem_index is the physical index.
                 * The correction routines expect the logical index.
                 * Reverse the mapping.
                 */
#ifdef BCM_APACHE_SUPPORT
                if (SOC_IS_APACHE(unit)) {
                    mem_index =
                        soc_apache_l3_defip_index_remap(unit,
                                                        cur_ser_info->mem,
                                                        mem_index);
                }
#endif /* BCM_APACHE_SUPPORT */
#ifdef BCM_HELIX4_SUPPORT
                if (SOC_IS_HELIX4(unit)) {
                    /* Helix4 uses the Triumph3 remapping logic */
                    /* Coverity
                     * return value of mem_index from TRIDENT2
                     * case will not affect HELIX4_SUPPORT case
                     */
                    /* coverity[negative_returns] */
                    mem_index =
                        soc_tr3_l3_defip_index_remap(unit,
                                                     cur_ser_info->mem,
                                                     mem_index);
                }
#endif /* BCM_HELIX4_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
                if (SOC_IS_KATANA2(unit)) {
                    /* coverity[negative_returns] */
                    mem_index =
                        soc_kt2_l3_defip_index_remap(unit,
                                                     cur_ser_info->mem,
                                                     mem_index);
                }
#endif /* BCM_KATANA2_SUPPORT */
            }

            cur_mem = cur_ser_info->mem;
#if defined(SOC_UNIQUE_ACC_TYPE_ACCESS)
            if ((cur_ser_info->ser_flags & _SOC_SER_FLAG_ACC_TYPE_CHK) &&
                soc_feature(unit, soc_feature_unique_acc_type_access) &&
                (SOC_MEM_UNIQUE_ACC(unit, cur_ser_info->mem) != NULL)) {
                    cur_mem = SOC_MEM_UNIQUE_ACC(unit,
                                                 cur_ser_info->mem)[acc_type];
                    if (!SOC_MEM_IS_VALID(unit, cur_mem)) {
                        LOG_WARN(BSL_LS_SOC_SER,
                                  (BSL_META_U(unit, "Assume _PIPE0 view for ser_correction - could not find unique view for mem %s, acc_type %d \n"),
                                   SOC_MEM_NAME(unit, cur_ser_info->mem), acc_type));
                        cur_mem = SOC_MEM_UNIQUE_ACC(
                                      unit, cur_ser_info->mem)[0];
                    }
            }
#endif /* SOC_UNIQUE_ACC_TYPE_ACCESS */

#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
            if (cur_mem == L3_DEFIP_PAIR_128m || cur_mem == L3_DEFIPm) {
                /* We cannot be here for any L3_DEFIP_ALPM views or
                 * for L3_DEFIP_AUX_TABLE views */
                is_l3_defip = TRUE;
                _ALPM_LOCK(unit);
            }
        }
#endif

#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
            MEM_LOCK(unit, cur_mem);
            corrupt = SOC_MEM_STATE(unit, cur_mem).corrupt[copyno];
            /* For L3_DEFIP* tables, we cannot use cache for SER correction on
             * multi-pipe devices. Because the data may be updated by H/W.
             */ 
            if (is_l3_defip && NUM_PIPE(unit) > 1) {
                corrupt = NULL;
            }
            /* coverity[negative_shift] */
            if ((corrupt == NULL) || (TCAM_CORRUPT_MAP_TST(corrupt, mem_index))) {
#endif
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "Unit %d: %s entry %d TCAM parity error\n"),
                           unit, SOC_MEM_NAME(unit, cur_mem),
                           mem_index));
                /* The value of err_type vary depending different chip since the
                 * err_type is defined in the chip related code specially. The
                 * original code just used the err_type as the event type directly,
                 * as a result, the events reported in different chip are different.
                 * In fact the only thing needed here is a 
                 * SOC_SWITCH_EVENT_DATA_ERROR_PARITY raised. 
                 * So err_type is never known.
                 */
                soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR, 
                                   SOC_SWITCH_EVENT_DATA_ERROR_PARITY, addr, 0);
                stat->ser_err_tcam++;
                spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
                spci.reg = INVALIDr;
                spci.mem = cur_mem;
                spci.blk_type = copyno;
                spci.index = mem_index;
                spci.acc_type = acc_type;
                spci.addr = addr;

#if defined(BCM_TOMAHAWK_SUPPORT)
                if (SOC_IS_TOMAHAWKX(unit)) {
                    if (acc_type <= _SOC_MEM_ADDR_ACC_TYPE_PIPE_3) {
                        spci.pipe_num = acc_type;
                    } else {
                        spci.pipe_num = 0;
                    }
                } else
#endif /* BCM_TOMAHAWK_SUPPORT */
                {
                    spci.pipe_num =
                        (_SOC_MEM_ADDR_ACC_TYPE_PIPE_Y == acc_type) ? 1 : 0;
                }
                spci.detect_time = sal_time_usecs();
                spci.log_id = soc_ser_log_create_entry(unit,
                    sizeof(soc_ser_log_tlv_generic_t) +
                    sizeof(soc_ser_log_tlv_memory_t) +
                    sizeof(soc_ser_log_tlv_hdr_t) *3);
                rv = soc_ser_correction(unit, &spci);
                if (spci.log_id != 0) {
                    soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                            SOC_SWITCH_EVENT_DATA_ERROR_LOG,
                            spci.log_id, 0);
                }
                if (SOC_FAILURE(rv)) {
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)

                    MEM_UNLOCK(unit, cur_mem);
#endif
                    /* Report failed to correct event flag to
                     * application */
                    soc_event_generate(unit,
                                SOC_SWITCH_EVENT_PARITY_ERROR,
                                SOC_SWITCH_EVENT_DATA_ERROR_FAILEDTOCORRECT,
                                addr,
                                0);

                    _ALPM_UNLOCK(unit);
                    return rv;
                }
#if defined(BCM_TRIDENT2_SUPPORT) || defined(BCM_HELIX4_SUPPORT) || \
    defined(BCM_KATANA2_SUPPORT) || defined(BCM_HURRICANE2_SUPPORT) || \
    defined(BCM_GREYHOUND_SUPPORT)
                /* Clear corrupt bit after correction, no matter how many times
                 * the parity error at this index in this table has been detected.
                 */
                if (corrupt != NULL) {
                    TCAM_CORRUPT_MAP_CLR(corrupt, mem_index);
                }
                LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "Unit %d: %s entry %d TCAM parity error indicating bit is cleared.\n"),
                             unit, SOC_MEM_NAME(unit, cur_mem),
                             mem_index));
            }
        MEM_UNLOCK(unit, cur_mem);
#endif
        _ALPM_UNLOCK(unit);
        }
        SOC_IF_ERROR_RETURN(READ_SER_ERROR_0r(unit, &ser_err[0]));
        SOC_IF_ERROR_RETURN(READ_SER_ERROR_1r(unit, &ser_err[1]));
    } while (soc_reg_field_get(unit, SER_ERROR_0r, ser_err[0], ERROR_0_VALf) ||
             soc_reg_field_get(unit, SER_ERROR_1r, ser_err[1], ERROR_1_VALf));
    SOC_IF_ERROR_RETURN(READ_SER_MISSED_EVENTr(unit, &count));
    if (count) {
        LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "Overflow events: %d.\n"), count));
    }
    return SOC_E_NONE;
}

int
soc_generic_ser_init(int unit, _soc_generic_ser_info_t *ser_info)
{
    int info_ix, hw_ser_ix;
    int index_min, index_max, copyno, ser_mem_gran;
    int i = 0, ser_mem_count;
    uint32 rval = 0, start_addr, end_addr;
    uint32 addr_valid = 0, ser_mem_addr = 0, ser_mem_total = 0;
    uint32 acc_type, at_bmp;
    _soc_generic_ser_info_t *cur_ser_info;
    ser_memory_entry_t ser_mem;
    uint8 at, alias;

    static soc_reg_t range_cfg[] = {
        SER_RANGE_0_CONFIGr, SER_RANGE_1_CONFIGr, SER_RANGE_2_CONFIGr,
        SER_RANGE_3_CONFIGr,
        SER_RANGE_4_CONFIGr, SER_RANGE_5_CONFIGr, SER_RANGE_6_CONFIGr,
        SER_RANGE_7_CONFIGr,
        SER_RANGE_8_CONFIGr, SER_RANGE_9_CONFIGr, SER_RANGE_10_CONFIGr,
        SER_RANGE_11_CONFIGr,
        SER_RANGE_12_CONFIGr, SER_RANGE_13_CONFIGr, SER_RANGE_14_CONFIGr,
        SER_RANGE_15_CONFIGr,
        SER_RANGE_16_CONFIGr, SER_RANGE_17_CONFIGr, SER_RANGE_18_CONFIGr,
        SER_RANGE_19_CONFIGr,
        SER_RANGE_20_CONFIGr, SER_RANGE_21_CONFIGr, SER_RANGE_22_CONFIGr,
        SER_RANGE_23_CONFIGr,
        SER_RANGE_24_CONFIGr, SER_RANGE_25_CONFIGr, SER_RANGE_26_CONFIGr,
        SER_RANGE_27_CONFIGr,
        SER_RANGE_28_CONFIGr, SER_RANGE_29_CONFIGr, SER_RANGE_30_CONFIGr,
        SER_RANGE_31_CONFIGr
    };
    static soc_reg_t addr_mask[] = {
        SER_RANGE_0_ADDR_MASKr, SER_RANGE_1_ADDR_MASKr, SER_RANGE_2_ADDR_MASKr,
        SER_RANGE_3_ADDR_MASKr,
        SER_RANGE_4_ADDR_MASKr, SER_RANGE_5_ADDR_MASKr, SER_RANGE_6_ADDR_MASKr,
        SER_RANGE_7_ADDR_MASKr,
        SER_RANGE_8_ADDR_MASKr, SER_RANGE_9_ADDR_MASKr, SER_RANGE_10_ADDR_MASKr,
        SER_RANGE_11_ADDR_MASKr,
        SER_RANGE_12_ADDR_MASKr, SER_RANGE_13_ADDR_MASKr, SER_RANGE_14_ADDR_MASKr,
        SER_RANGE_15_ADDR_MASKr,
        SER_RANGE_16_ADDR_MASKr, SER_RANGE_17_ADDR_MASKr, SER_RANGE_18_ADDR_MASKr,
        SER_RANGE_19_ADDR_MASKr,
        SER_RANGE_20_ADDR_MASKr, SER_RANGE_21_ADDR_MASKr, SER_RANGE_22_ADDR_MASKr,
        SER_RANGE_23_ADDR_MASKr,
        SER_RANGE_24_ADDR_MASKr, SER_RANGE_25_ADDR_MASKr, SER_RANGE_26_ADDR_MASKr,
        SER_RANGE_27_ADDR_MASKr,
        SER_RANGE_28_ADDR_MASKr, SER_RANGE_29_ADDR_MASKr, SER_RANGE_30_ADDR_MASKr,
        SER_RANGE_31_ADDR_MASKr
    };
    static soc_reg_t range_start[] = {
        SER_RANGE_0_STARTr, SER_RANGE_1_STARTr, SER_RANGE_2_STARTr,
        SER_RANGE_3_STARTr,
        SER_RANGE_4_STARTr, SER_RANGE_5_STARTr, SER_RANGE_6_STARTr,
        SER_RANGE_7_STARTr,
        SER_RANGE_8_STARTr, SER_RANGE_9_STARTr, SER_RANGE_10_STARTr,
        SER_RANGE_11_STARTr,
        SER_RANGE_12_STARTr, SER_RANGE_13_STARTr, SER_RANGE_14_STARTr,
        SER_RANGE_15_STARTr,
        SER_RANGE_16_STARTr, SER_RANGE_17_STARTr, SER_RANGE_18_STARTr,
        SER_RANGE_19_STARTr,
        SER_RANGE_20_STARTr, SER_RANGE_21_STARTr, SER_RANGE_22_STARTr,
        SER_RANGE_23_STARTr,
        SER_RANGE_24_STARTr, SER_RANGE_25_STARTr, SER_RANGE_26_STARTr,
        SER_RANGE_27_STARTr,
        SER_RANGE_28_STARTr, SER_RANGE_29_STARTr, SER_RANGE_30_STARTr,
        SER_RANGE_31_STARTr
    };
    static soc_reg_t range_end[] = {
        SER_RANGE_0_ENDr, SER_RANGE_1_ENDr, SER_RANGE_2_ENDr, SER_RANGE_3_ENDr,
        SER_RANGE_4_ENDr, SER_RANGE_5_ENDr, SER_RANGE_6_ENDr, SER_RANGE_7_ENDr,
        SER_RANGE_8_ENDr, SER_RANGE_9_ENDr, SER_RANGE_10_ENDr, SER_RANGE_11_ENDr,
        SER_RANGE_12_ENDr, SER_RANGE_13_ENDr, SER_RANGE_14_ENDr, SER_RANGE_15_ENDr,
        SER_RANGE_16_ENDr, SER_RANGE_17_ENDr, SER_RANGE_18_ENDr, SER_RANGE_19_ENDr,
        SER_RANGE_20_ENDr, SER_RANGE_21_ENDr, SER_RANGE_22_ENDr, SER_RANGE_23_ENDr,
        SER_RANGE_24_ENDr, SER_RANGE_25_ENDr, SER_RANGE_26_ENDr, SER_RANGE_27_ENDr,
        SER_RANGE_28_ENDr, SER_RANGE_29_ENDr, SER_RANGE_30_ENDr, SER_RANGE_31_ENDr
    };
    static soc_reg_t range_result[] = {
        SER_RANGE_0_RESULTr, SER_RANGE_1_RESULTr, SER_RANGE_2_RESULTr,
        SER_RANGE_3_RESULTr,
        SER_RANGE_4_RESULTr, SER_RANGE_5_RESULTr, SER_RANGE_6_RESULTr,
        SER_RANGE_7_RESULTr,
        SER_RANGE_8_RESULTr, SER_RANGE_9_RESULTr, SER_RANGE_10_RESULTr,
        SER_RANGE_11_RESULTr,
        SER_RANGE_12_RESULTr, SER_RANGE_13_RESULTr, SER_RANGE_14_RESULTr,
        SER_RANGE_15_RESULTr,
        SER_RANGE_16_RESULTr, SER_RANGE_17_RESULTr, SER_RANGE_18_RESULTr,
        SER_RANGE_19_RESULTr,
        SER_RANGE_20_RESULTr, SER_RANGE_21_RESULTr, SER_RANGE_22_RESULTr,
        SER_RANGE_23_RESULTr,
        SER_RANGE_24_RESULTr, SER_RANGE_25_RESULTr, SER_RANGE_26_RESULTr,
        SER_RANGE_27_RESULTr,
        SER_RANGE_28_RESULTr, SER_RANGE_29_RESULTr, SER_RANGE_30_RESULTr,
        SER_RANGE_31_RESULTr
    };
    static soc_reg_t prot_word[][SOC_NUM_GENERIC_PROT_SECTIONS] = {
        { SER_RANGE_0_PROT_WORD_0r, SER_RANGE_0_PROT_WORD_1r,
          SER_RANGE_0_PROT_WORD_2r, SER_RANGE_0_PROT_WORD_3r },
        { SER_RANGE_1_PROT_WORD_0r, SER_RANGE_1_PROT_WORD_1r,
          SER_RANGE_1_PROT_WORD_2r, SER_RANGE_1_PROT_WORD_3r },
        { SER_RANGE_2_PROT_WORD_0r, SER_RANGE_2_PROT_WORD_1r,
          SER_RANGE_2_PROT_WORD_2r, SER_RANGE_2_PROT_WORD_3r },
        { SER_RANGE_3_PROT_WORD_0r, SER_RANGE_3_PROT_WORD_1r,
          SER_RANGE_3_PROT_WORD_2r, SER_RANGE_3_PROT_WORD_3r },
        { SER_RANGE_4_PROT_WORD_0r, SER_RANGE_4_PROT_WORD_1r,
          SER_RANGE_4_PROT_WORD_2r, SER_RANGE_4_PROT_WORD_3r },
        { SER_RANGE_5_PROT_WORD_0r, SER_RANGE_5_PROT_WORD_1r,
          SER_RANGE_5_PROT_WORD_2r, SER_RANGE_5_PROT_WORD_3r },
        { SER_RANGE_6_PROT_WORD_0r, SER_RANGE_6_PROT_WORD_1r,
          SER_RANGE_6_PROT_WORD_2r, SER_RANGE_6_PROT_WORD_3r },
        { SER_RANGE_7_PROT_WORD_0r, SER_RANGE_7_PROT_WORD_1r,
          SER_RANGE_7_PROT_WORD_2r, SER_RANGE_7_PROT_WORD_3r },
        { SER_RANGE_8_PROT_WORD_0r, SER_RANGE_8_PROT_WORD_1r,
          SER_RANGE_8_PROT_WORD_2r, SER_RANGE_8_PROT_WORD_3r },
        { SER_RANGE_9_PROT_WORD_0r, SER_RANGE_9_PROT_WORD_1r,
          SER_RANGE_9_PROT_WORD_2r, SER_RANGE_9_PROT_WORD_3r },
        { SER_RANGE_10_PROT_WORD_0r, SER_RANGE_10_PROT_WORD_1r,
          SER_RANGE_10_PROT_WORD_2r, SER_RANGE_10_PROT_WORD_3r },
        { SER_RANGE_11_PROT_WORD_0r, SER_RANGE_11_PROT_WORD_1r,
          SER_RANGE_11_PROT_WORD_2r, SER_RANGE_11_PROT_WORD_3r },
        { SER_RANGE_12_PROT_WORD_0r, SER_RANGE_12_PROT_WORD_1r,
          SER_RANGE_12_PROT_WORD_2r, SER_RANGE_12_PROT_WORD_3r },
        { SER_RANGE_13_PROT_WORD_0r, SER_RANGE_13_PROT_WORD_1r,
          SER_RANGE_13_PROT_WORD_2r, SER_RANGE_13_PROT_WORD_3r },
        { SER_RANGE_14_PROT_WORD_0r, SER_RANGE_14_PROT_WORD_1r,
          SER_RANGE_14_PROT_WORD_2r, SER_RANGE_14_PROT_WORD_3r },
        { SER_RANGE_15_PROT_WORD_0r, SER_RANGE_15_PROT_WORD_1r,
          SER_RANGE_15_PROT_WORD_2r, SER_RANGE_15_PROT_WORD_3r },
        { SER_RANGE_16_PROT_WORD_0r, SER_RANGE_16_PROT_WORD_1r,
          SER_RANGE_16_PROT_WORD_2r, SER_RANGE_16_PROT_WORD_3r },
        { SER_RANGE_17_PROT_WORD_0r, SER_RANGE_17_PROT_WORD_1r,
          SER_RANGE_17_PROT_WORD_2r, SER_RANGE_17_PROT_WORD_3r },
        { SER_RANGE_18_PROT_WORD_0r, SER_RANGE_18_PROT_WORD_1r,
          SER_RANGE_18_PROT_WORD_2r, SER_RANGE_18_PROT_WORD_3r },
        { SER_RANGE_19_PROT_WORD_0r, SER_RANGE_19_PROT_WORD_1r,
          SER_RANGE_19_PROT_WORD_2r, SER_RANGE_19_PROT_WORD_3r },
        { SER_RANGE_20_PROT_WORD_0r, SER_RANGE_20_PROT_WORD_1r,
          SER_RANGE_20_PROT_WORD_2r, SER_RANGE_20_PROT_WORD_3r },
        { SER_RANGE_21_PROT_WORD_0r, SER_RANGE_21_PROT_WORD_1r,
          SER_RANGE_21_PROT_WORD_2r, SER_RANGE_21_PROT_WORD_3r },
        { SER_RANGE_22_PROT_WORD_0r, SER_RANGE_22_PROT_WORD_1r,
          SER_RANGE_22_PROT_WORD_2r, SER_RANGE_22_PROT_WORD_3r },
        { SER_RANGE_23_PROT_WORD_0r, SER_RANGE_23_PROT_WORD_1r,
          SER_RANGE_23_PROT_WORD_2r, SER_RANGE_23_PROT_WORD_3r },
        { SER_RANGE_24_PROT_WORD_0r, SER_RANGE_24_PROT_WORD_1r,
          SER_RANGE_24_PROT_WORD_2r, SER_RANGE_24_PROT_WORD_3r },
        { SER_RANGE_25_PROT_WORD_0r, SER_RANGE_25_PROT_WORD_1r,
          SER_RANGE_25_PROT_WORD_2r, SER_RANGE_25_PROT_WORD_3r },
        { SER_RANGE_26_PROT_WORD_0r, SER_RANGE_26_PROT_WORD_1r,
          SER_RANGE_26_PROT_WORD_2r, SER_RANGE_26_PROT_WORD_3r },
        { SER_RANGE_27_PROT_WORD_0r, SER_RANGE_27_PROT_WORD_1r,
          SER_RANGE_27_PROT_WORD_2r, SER_RANGE_27_PROT_WORD_3r },
        { SER_RANGE_28_PROT_WORD_0r, SER_RANGE_28_PROT_WORD_1r,
          SER_RANGE_28_PROT_WORD_2r, SER_RANGE_28_PROT_WORD_3r },
        { SER_RANGE_29_PROT_WORD_0r, SER_RANGE_29_PROT_WORD_1r,
          SER_RANGE_29_PROT_WORD_2r, SER_RANGE_29_PROT_WORD_3r },
        { SER_RANGE_30_PROT_WORD_0r, SER_RANGE_30_PROT_WORD_1r,
          SER_RANGE_30_PROT_WORD_2r, SER_RANGE_30_PROT_WORD_3r },
        { SER_RANGE_31_PROT_WORD_0r, SER_RANGE_31_PROT_WORD_1r,
          SER_RANGE_31_PROT_WORD_2r, SER_RANGE_31_PROT_WORD_3r }
    };

    static soc_reg_t range_addr_bits[] = {
        SER_RANGE_0_ADDR_BITSr, SER_RANGE_1_ADDR_BITSr,
        SER_RANGE_2_ADDR_BITSr, SER_RANGE_3_ADDR_BITSr,
        SER_RANGE_4_ADDR_BITSr, SER_RANGE_5_ADDR_BITSr,
        SER_RANGE_6_ADDR_BITSr, SER_RANGE_7_ADDR_BITSr,
        SER_RANGE_8_ADDR_BITSr, SER_RANGE_9_ADDR_BITSr,
        SER_RANGE_10_ADDR_BITSr, SER_RANGE_11_ADDR_BITSr,
        SER_RANGE_12_ADDR_BITSr, SER_RANGE_13_ADDR_BITSr,
        SER_RANGE_14_ADDR_BITSr, SER_RANGE_15_ADDR_BITSr,
        SER_RANGE_16_ADDR_BITSr, SER_RANGE_17_ADDR_BITSr,
        SER_RANGE_18_ADDR_BITSr, SER_RANGE_19_ADDR_BITSr,
        SER_RANGE_20_ADDR_BITSr, SER_RANGE_21_ADDR_BITSr,
        SER_RANGE_22_ADDR_BITSr, SER_RANGE_23_ADDR_BITSr,
        SER_RANGE_24_ADDR_BITSr, SER_RANGE_25_ADDR_BITSr,
        SER_RANGE_26_ADDR_BITSr, SER_RANGE_27_ADDR_BITSr,
        SER_RANGE_28_ADDR_BITSr, SER_RANGE_29_ADDR_BITSr,
        SER_RANGE_30_ADDR_BITSr, SER_RANGE_31_ADDR_BITSr
    };

    static soc_reg_t range_disable[] = {
        SER_RANGE_0_DISABLEr, SER_RANGE_1_DISABLEr,
        SER_RANGE_2_DISABLEr, SER_RANGE_3_DISABLEr,
        SER_RANGE_4_DISABLEr, SER_RANGE_5_DISABLEr,
        SER_RANGE_6_DISABLEr, SER_RANGE_7_DISABLEr,
        SER_RANGE_8_DISABLEr, SER_RANGE_9_DISABLEr,
        SER_RANGE_10_DISABLEr, SER_RANGE_11_DISABLEr,
        SER_RANGE_12_DISABLEr, SER_RANGE_13_DISABLEr,
        SER_RANGE_14_DISABLEr, SER_RANGE_15_DISABLEr,
        SER_RANGE_16_DISABLEr, SER_RANGE_17_DISABLEr,
        SER_RANGE_18_DISABLEr, SER_RANGE_19_DISABLEr,
        SER_RANGE_20_DISABLEr, SER_RANGE_21_DISABLEr,
        SER_RANGE_22_DISABLEr, SER_RANGE_23_DISABLEr,
        SER_RANGE_24_DISABLEr, SER_RANGE_25_DISABLEr,
        SER_RANGE_26_DISABLEr, SER_RANGE_27_DISABLEr,
        SER_RANGE_28_DISABLEr, SER_RANGE_29_DISABLEr,
        SER_RANGE_30_DISABLEr, SER_RANGE_31_DISABLEr
    };

    LOG_VERBOSE(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "SER engine init:\n")));

    info_ix = 0;
    hw_ser_ix = 0;
    sal_memset(&ser_mem, 0, sizeof(ser_mem));
    /* Clear parity/ecc memory */
    if (!SAL_BOOT_SIMULATION || SAL_BOOT_QUICKTURN) {
        SOC_IF_ERROR_RETURN
            (soc_mem_clear(unit, SER_MEMORYm, COPYNO_ALL, TRUE));
    }
    while (ser_info[info_ix].mem != INVALIDm) {
        alias = FALSE;
        cur_ser_info = &(ser_info[info_ix]);
        cur_ser_info->ser_hw_index = -1;
        if ((info_ix > 0) &&
            (cur_ser_info->ser_flags & _SOC_SER_FLAG_CONFIG_SKIP)) {
            /* This entry does not require separate range in ser_engine.
             * But, this entry is used by memscan and therefore must have all
             * the info needed by memscan.
             * This entry should inherit property of previous entry as they map
             * to same range in ser_engine */
            cur_ser_info->ser_section_start =
                ser_info[info_ix - 1].ser_section_start;
            cur_ser_info->ser_section_end =
                ser_info[info_ix - 1].ser_section_end;
            cur_ser_info->ser_hw_index =
                ser_info[info_ix - 1].ser_hw_index;
            info_ix++;
            continue;
        }

        if (cur_ser_info->ser_flags & _SOC_SER_FLAG_SW_COMPARE) {
            if (SOC_MEM_INFO(unit, cur_ser_info->mem).flags &
                SOC_MEM_FLAG_BE) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "\tSW SER init of Big Endian TCAMs is not supported: %s\n"),
                           SOC_MEM_NAME(unit, cur_ser_info->mem)));
                return SOC_E_INTERNAL;
            }

            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tSkipping HW SER init of manual scan mem: %s\n"),
                         SOC_MEM_NAME(unit, cur_ser_info->mem)));
            SOC_MEM_INFO(unit, ser_info[info_ix].mem).flags |= SOC_MEM_FLAG_SER_ENGINE;
            info_ix++;
            continue;
        }

        index_min = soc_mem_index_min(unit, cur_ser_info->mem);
        index_max = soc_mem_index_max(unit, cur_ser_info->mem);
#if defined(BCM_TRIUMPH3_SUPPORT) || defined(BCM_KATANA2_SUPPORT)
        if (soc_feature(unit, soc_feature_l3_defip_map) &&
            ((cur_ser_info->mem == L3_DEFIP_PAIR_128m) ||
             (cur_ser_info->mem == L3_DEFIPm))) {
            /* Override SOC memory impression for overlay
             * LPM tables. */
            index_min = 0;
            index_max = (SOC_CONTROL(unit)->l3_defip_max_tcams *
                         SOC_CONTROL(unit)->l3_defip_tcam_size) - 1;
            if (cur_ser_info->mem == L3_DEFIP_PAIR_128m) {
                index_max /= 2;
            }
        }
#endif /* BCM_TRIUMPH3_SUPPORT || BCM_KATANA2_SUPPORT*/

        ser_mem_count = index_max - index_min + 1;
        if (!ser_mem_count || !SOC_MEM_IS_VALID(unit, cur_ser_info->mem)) {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tSkipping empty mem: %s\n"),
                         SOC_MEM_NAME(unit, cur_ser_info->mem)));
#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit)) {
                /* For different latency modes, we want to keep constant map
                 * between info_ix and hw_ser_ix. In other words, if some mem
                 * is not enabled in lower latency mode, we want to keep the
                 * bit position for that mem in SER_RANGE_ENABLE constant
                 * - because in tomahawk/ser.c, this bit position is hard-coded
                 * based on max_latency_mode. Otherwise, we will have to keep
                 * track of this position for mem based on latency modes.
                 */
                LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "\tRange[%2d](0x%04x, %08d): %s(%d-%d): "
                                        "Sbus range (0x%08x-0x%08x), info_ix=%d, <======= skipped\n"),
                             hw_ser_ix, 0, 0,
                             SOC_MEM_NAME(unit, cur_ser_info->mem),
                             index_min, index_max, 0, 0, info_ix));
                hw_ser_ix++;
            }
#endif
            info_ix++;
            continue;
        }

        ser_mem_gran = _soc_generic_ser_granularity(cur_ser_info->prot_type,
                                                    cur_ser_info->prot_mode,
                                                    cur_ser_info->start_end_bits);
        ser_mem_count *= ser_mem_gran;

        /* Round up to entry size for simplicity */
        ser_mem_count = ((ser_mem_count + 31) / 32); /* Changed from 36 per DE input */

        if (cur_ser_info->alias_mem != INVALIDm) {
            /* See if the aliased memory has already been configured */
            for (i = 0; i < info_ix; i++) {
                if (ser_info[i].mem == cur_ser_info->alias_mem) {
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "\tSER alias mem.\n")));
                    alias = TRUE;
                    break;
                }
            }
        }

        if (!alias && (cur_ser_info->ser_flags & _SOC_SER_FLAG_ACC_TYPE_CHK)) {
           ser_mem_count *= cur_ser_info->num_instances;
        }

        if (!alias &&
            (cur_ser_info->ser_flags & _SOC_SER_FLAG_MULTI_PIPE)) {
            i = info_ix - 1;
            if ((i >= 0) && (ser_info[i].mem == cur_ser_info->mem) &&
                !(ser_info[i].ser_flags & _SOC_SER_FLAG_CONFIG_SKIP)) {
                /* For multiple pipeline units, different pipeline's hardware
                 * instances are duplicates.  The SER parity bits are thus
                 * shared, but the configuration registers in the CMIC
                 * must be different to capture the different SBUS ranges
                 * of the read accesses.
                 */
                LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "\tSER multiple pipeline mem.\n")));
                /* Get the non-default pipeline access type */
                acc_type = cur_ser_info->ser_flags &
                    _SOC_SER_FLAG_ACC_TYPE_MASK;

                /* Copy info for mem clear */
                cur_ser_info->ser_section_start =
                    ser_info[i].ser_section_start;
                cur_ser_info->ser_section_end =
                    ser_info[i].ser_section_end;
                cur_ser_info->ser_hw_index =
                    ser_info[i].ser_hw_index;

                /* For multi-pipe, we can use the access type bitmap
                 * point multiple SBUS accesses to the same HW
                 * range checker.
                 * The HW index advanced when the first entry was set,
                 * so we need to use the previous valid HW index. */
                if (0 == hw_ser_ix) {
                    return SOC_E_INTERNAL;
                }
                SOC_IF_ERROR_RETURN
                    (soc_reg32_set(unit, range_cfg[hw_ser_ix - 1],
                                   REG_PORT_ANY, 0, rval));
                at_bmp =
                    soc_reg_field_get(unit, SER_RANGE_0_CONFIGr, rval,
                                      ACC_TYPEf);
                at_bmp |= (1 << acc_type);
                soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval,
                                  ACC_TYPEf, at_bmp);
                SOC_IF_ERROR_RETURN
                    (soc_reg32_set(unit, range_cfg[hw_ser_ix - 1],
                                   REG_PORT_ANY, 0, rval));
                info_ix++;
                /* Don't advance hw_ser_ix again in case of designs with
                 * more than two pipes */
                continue;
            }
        }

        if (!alias && ((ser_mem_count + ser_mem_total) >
                       soc_mem_index_count(unit, SER_MEMORYm))) {
            /* Can't fit requested parity bits in SER memory */
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "SER mem full: Skipping further config.\n")));
            break;
        }

        /* Parity bits fit, so on with the config */
        SOC_MEM_BLOCK_ITER(unit, cur_ser_info->mem, copyno) {
            break;
        }
        start_addr = soc_mem_addr_get(unit, cur_ser_info->mem, 0, copyno,
                                      index_min, &at);
        end_addr = soc_mem_addr_get(unit, cur_ser_info->mem, 0, copyno,
                                    index_max, &at);

        /* Flush SER memory if not aliased */
        ser_mem_addr = alias ? ser_info[i].ser_section_start :
                               ser_mem_total; /* Previous total */
        if (alias) {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tReuse SER_MEM (%d) from index: %d - %d\n"),
                         ser_mem_count, ser_info[i].ser_section_start,
                         ser_info[i].ser_section_end));
        } else {
            ser_mem_total += ser_mem_count; /* New total */
            LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "\tClear SER_MEM (%d) from index: %d - %d\n"),
                         ser_mem_count, ser_mem_addr, ser_mem_total-1));
            for (i = ser_mem_addr; i < ser_mem_total; i++) {
                /* mem clear */
                SOC_IF_ERROR_RETURN
                    (WRITE_SER_MEMORYm(unit, MEM_BLOCK_ALL, i, &ser_mem));
            }
        }

        /* Record section for mem_clear use */
        cur_ser_info->ser_section_start = ser_mem_addr;
        cur_ser_info->ser_section_end = alias ? ser_info[i].ser_section_end :
                                                (ser_mem_total-1);
        cur_ser_info->ser_hw_index = hw_ser_ix;

        SOC_IF_ERROR_RETURN
            (soc_reg32_set(unit, range_start[hw_ser_ix], REG_PORT_ANY, 0,
                           start_addr));
        SOC_IF_ERROR_RETURN
            (soc_reg32_set(unit, range_end[hw_ser_ix], REG_PORT_ANY, 0,
                           end_addr));
        if (SOC_REG_IS_VALID(unit, range_disable[0])) {
            if (cur_ser_info->ser_flags & _SOC_SER_FLAG_RANGE_DISABLE) {
                SOC_IF_ERROR_RETURN
                    (soc_reg32_set(unit, range_addr_bits[info_ix], REG_PORT_ANY, 0,
                               cur_ser_info->addr_start_bit));
                SOC_IF_ERROR_RETURN
                    (soc_reg32_set(unit, range_disable[info_ix], REG_PORT_ANY, 0,
                               cur_ser_info->addr_mask));
            }
        } else {
            SOC_IF_ERROR_RETURN
                (soc_reg32_set(unit, addr_mask[hw_ser_ix], REG_PORT_ANY, 0,
                           cur_ser_info->addr_mask ?
                           cur_ser_info->addr_mask : 0xffffffff));
        }
        SOC_IF_ERROR_RETURN
            (soc_reg32_set(unit, range_result[hw_ser_ix], REG_PORT_ANY, 0,
                           ser_mem_addr));
        if ((!(cur_ser_info->ser_flags & _SOC_SER_FLAG_VIEW_DISABLE)) &&
            (!(SOC_MEM_INFO(unit, cur_ser_info->mem).flags &
               SOC_MEM_FLAG_SER_PARITY_ENABLE_SKIP))) {
            addr_valid |= (1 << hw_ser_ix);
        }

        for (i = 0; i < SOC_NUM_GENERIC_PROT_SECTIONS; i++) {
            if (cur_ser_info->start_end_bits[i].start_bit != -1) {
                rval = 0;
                soc_reg_field_set(unit, SER_RANGE_0_PROT_WORD_0r, &rval,
                                  DATA_STARTf,
                                  cur_ser_info->start_end_bits[i].start_bit);
                soc_reg_field_set(unit, SER_RANGE_0_PROT_WORD_0r, &rval,
                                  DATA_ENDf,
                                  cur_ser_info->start_end_bits[i].end_bit);
                SOC_IF_ERROR_RETURN
                    (soc_reg32_set(unit, prot_word[hw_ser_ix][i],
                                   REG_PORT_ANY, 0, rval));
            }
        }
        rval = 0;
        soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, BLOCKf,
                          SOC_BLOCK2SCH(unit, copyno));
        if (soc_reg_field_valid(unit, SER_RANGE_0_CONFIGr, ACC_TYPEf)) {
            soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, ACC_TYPEf,
                              1 << at);
        }
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (soc_reg_field_valid(unit, SER_RANGE_0_CONFIGr, ACC_TYPE_MODEf)) {
            soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, ACC_TYPE_MODEf,
                              (cur_ser_info->ser_flags & _SOC_SER_FLAG_ACC_TYPE_CHK)? 1 : 0);
        }
#endif /* BCM_TOMAHAWK_SUPPORT */
        soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, PROT_TYPEf,
                          cur_ser_info->prot_type);
        soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, PROT_MODEf,
                          cur_ser_info->prot_mode);
        soc_reg_field_set(unit, SER_RANGE_0_CONFIGr, &rval, INTERLEAVE_MODEf,
                          cur_ser_info->intrlv_mode);
        SOC_IF_ERROR_RETURN
            (soc_reg32_set(unit, range_cfg[hw_ser_ix], REG_PORT_ANY, 0, rval));

        LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "\tRange[%2d](0x%04x, %08d): %s(%d-%d): "
                                "Sbus range (0x%08x-0x%08x), info_ix=%d\n"),
                     hw_ser_ix, ser_mem_addr, ser_mem_addr,
                     SOC_MEM_NAME(unit, cur_ser_info->mem),
                     index_min, index_max, start_addr, end_addr, info_ix));
        SOC_MEM_INFO(unit, ser_info[info_ix].mem).flags |= SOC_MEM_FLAG_SER_ENGINE;
#if defined(SOC_UNIQUE_ACC_TYPE_ACCESS)
        if (soc_feature(unit, soc_feature_unique_acc_type_access) &&
            (SOC_MEM_UNIQUE_ACC(unit, ser_info[info_ix].mem) != NULL)) {
            int pipe;
            soc_mem_t mem1 = ser_info[info_ix].mem;
            for (pipe = 0; pipe < NUM_PIPE(unit); pipe++) {
                SOC_MEM_INFO(unit,
                             SOC_MEM_UNIQUE_ACC(unit, mem1)[pipe]
                             ).flags |= SOC_MEM_FLAG_SER_ENGINE;
            }
            /* Non-tcam-iterations loop should also skip the _PIPE0,1,2,3,etc
             * views */
        }
#endif /* SOC_UNIQUE_ACC_TYPE_ACCESS */
        info_ix++;
        hw_ser_ix++;
    }

    SOC_IF_ERROR_RETURN
        (WRITE_SER_RING_ERR_CTRLr(unit, 0xf));
    /* Enable SER protection last */
    SOC_IF_ERROR_RETURN
        (WRITE_SER_RANGE_ENABLEr(unit, addr_valid));
    if (soc_feature(unit, soc_feature_ser_hw_bg_read)) {
        SOC_IF_ERROR_RETURN
            (WRITE_SER_CONFIG_REGr(unit, 0x3));
    } else {
        SOC_IF_ERROR_RETURN
            (WRITE_SER_CONFIG_REGr(unit, 0x1));
    }
    /* Kick off s/w background scan if applicable */
    if (!soc_feature(unit, soc_feature_ser_hw_bg_read)) {

    }
    return SOC_E_NONE;
}

int
soc_generic_ser_at_map_init(int unit, uint32 map[], int items)
{
    int i = 0;
    uint32 fld_val;
    ser_acc_type_map_entry_t ser_acc_type;

    if (items > soc_mem_index_count(unit, SER_ACC_TYPE_MAPm)) {
        return SOC_E_PARAM;
    }
    while (items--) {
        sal_memset(&ser_acc_type, 0, sizeof(ser_acc_type));
        fld_val = map[i];
        soc_mem_field_set(unit, SER_ACC_TYPE_MAPm,
                          (uint32 *)&ser_acc_type, COMPRESSED_ACC_TYPEf, &fld_val);
        SOC_IF_ERROR_RETURN
            (soc_mem_write(unit, SER_ACC_TYPE_MAPm, MEM_BLOCK_ALL, i++,
                           &ser_acc_type));
    }
    return SOC_E_NONE;
}

int
soc_generic_ser_mem_scan_start(int unit)
{
#ifdef INCLUDE_MEM_SCAN
    /* Kick off s/w background scan */
    if (soc_property_get(unit, spn_MEM_SCAN_ENABLE, SAL_BOOT_SIMULATION ? 0 : 1)) {
        sal_usecs_t interval = 0;
        int rate = 0;

        if (soc_mem_scan_running(unit, &rate, &interval)) {
            if (soc_mem_scan_stop(unit)) {
                return SOC_E_INTERNAL;
            }
        }
        rate = rate ? rate : soc_property_get(unit, spn_MEM_SCAN_RATE,
                                              SOC_MEMSCAN_RATE_DEFAULT);
        interval = interval ? interval :
                              soc_property_get(unit, spn_MEM_SCAN_INTERVAL,
                              SAL_BOOT_SIMULATION ? SOC_MEMSCAN_INTERVAL_SIM_DEFAULT :
                              SOC_MEMSCAN_INTERVAL_DEFAULT);
        if (soc_mem_scan_start(unit, rate, interval)) {
            return SOC_E_INTERNAL;
        }
    }
#endif /* INCLUDE_MEM_SCAN */
    return SOC_E_NONE;
}

int
soc_generic_sram_mem_scan_start(int unit)
{
#ifdef BCM_SRAM_SCAN_SUPPORT
    if (soc_property_get(unit, spn_SRAM_SCAN_ENABLE, SAL_BOOT_SIMULATION ? 0 : 
                          (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TD2P_TT2P(unit)
                           || SOC_IS_APACHE(unit)))) {
        sal_usecs_t interval = 0;
        int rate = 0;

        if (soc_sram_scan_running(unit, &rate, &interval)) {
            if (soc_sram_scan_stop(unit)) {
                return SOC_E_INTERNAL;
            }
        }
        rate = rate ? rate : soc_property_get(unit, spn_SRAM_SCAN_RATE,
                                              SOC_SRAMSCAN_RATE_DEFAULT);
        interval = interval ? interval :
                              soc_property_get(unit, spn_SRAM_SCAN_INTERVAL,
                              SAL_BOOT_SIMULATION ? SOC_SRAMSCAN_INTERVAL_SIM_DEFAULT :
                              SOC_SRAMSCAN_INTERVAL_DEFAULT);
        if (soc_sram_scan_start(unit, rate, interval)) {
            return SOC_E_INTERNAL;
        }
    }
#endif
    return SOC_E_NONE;
}

int
soc_generic_ser_mem_scan_stop(int unit)
{
#ifdef INCLUDE_MEM_SCAN
    sal_usecs_t interval = 0;
    int rate = 0;

    if (soc_mem_scan_running(unit, &rate, &interval)) {
        if (soc_mem_scan_stop(unit)) {
            return SOC_E_INTERNAL;
        }
    }
#endif /* INCLUDE_MEM_SCAN */
    return SOC_E_NONE;
}

int
soc_generic_sram_mem_scan_stop(int unit)
{
#ifdef BCM_SRAM_SCAN_SUPPORT
    sal_usecs_t interval = 0;
    int rate = 0;

    if (soc_sram_scan_running(unit, &rate, &interval)) {
        if (soc_sram_scan_stop(unit)) {
            return SOC_E_INTERNAL;
        }
    }
#endif /* INCLUDE_SRAM_SCAN */
    return SOC_E_NONE;
}

int
soc_generic_ser_mem_update(int unit, soc_mem_t mem, int mode)
{
    int rv = SOC_E_NONE;
#ifdef INCLUDE_MEM_SCAN
    int i, info_ix = 0, hw_ser_ix = 0;
    _soc_generic_ser_info_t *ser_info;
    uint32 range_enable;
    
    /* 1. It is possible that mem is marked invalid due to latency mode (eg: TH)
     * 2. It is possible that parity is disabled for this mem by property */
    if (!SOC_MEM_IS_VALID(unit, mem) ||
        (SOC_MEM_INFO(unit, mem).flags & SOC_MEM_FLAG_SER_PARITY_ENABLE_SKIP)) {
        LOG_VERBOSE(BSL_LS_SOC_SER,
                  (BSL_META_U(unit, "mem_update DISABLED for mem = %s as "
                   "mem is INVALID or parity is disabled by config property \n"),
                   SOC_MEM_NAME(unit,mem)));
       return rv;
    }

    if (soc_property_get(unit, spn_MEM_SCAN_ENABLE, SAL_BOOT_SIMULATION ? 0 : 1)) {
        SOC_IF_ERROR_RETURN(soc_generic_ser_mem_scan_stop(unit));
        /* search the memory and update the engine config */
        ser_info = soc_mem_scan_ser_info_get(unit);
        if (ser_info == NULL) {
            if (soc_property_get(unit, spn_PARITY_ENABLE, TRUE)) {
                rv = SOC_E_INIT;
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit, "mem_update FAILED for mem = %s as "
                           "ser_info is NULL\n"), SOC_MEM_NAME(unit,mem)));
            }
            goto _error;
        }
        while (ser_info[info_ix].mem != INVALIDm) {
            /* Note: This will be the global view index */
            if (ser_info[info_ix].mem == mem) {
                rv = READ_SER_RANGE_ENABLEr(unit, &range_enable);
                if (SOC_FAILURE(rv)) {
                    goto _error;
                }
                /* ASSUMPTION: _soc_th_tcam_ser_info_template[] table
                 * lists entries for global, unique view consecutively at
                 * {info_ix +0, +1, +2, +3}, info_ix +4 respectively and
                 * because no ranges are configured for info_ix +1, +2, +3
                 * so enable bits for global, unique views 
                 * will be at consecutive hw_ser_ix, hw_ser_ix+1 positions
                 * in SER_RANGE_ENABLE REGISTER 
                 *
                 * Summary:
                 *
                 * Global view:
                 * info_ix +0, +1, +2, +3 point to 4 entries in _template[]
                 * hw_ser_ix will point to enable bit in SER_RANGE_CONTROL
                 *
                 * Unique view:
                 * info_ix+NUM_PIPE will point to entry in _template[]
                 * hw_ser_ix+1 will point to enable bit for unique view because
                 * entries at info_ix +1, +2, +3 do not need separate ranges in
                 * ser_engine
                 *
                 * memscan:
                 * Irrespective of global/unique mode, memscan always scans
                 * entries at info_ix +0, +1, +2, +3
                 *
                 */ 
                hw_ser_ix = ser_info[info_ix].ser_hw_index;
                LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit, "mem_update for mem = %s, "
                             "range_enable = 0x%x,  hw_ser_ix = %0d, "
                             "request to change mode to %s \n"),
                             SOC_MEM_NAME(unit,mem), range_enable, hw_ser_ix,
                             mode? "PIPE_UNIQUE" : "GLOBAL"));
                if (mode == _SOC_SER_MEM_MODE_PIPE_UNIQUE) {
                    /* Break before make to avoid any spurious schan 
                     * transaction errors 
                     */
                    range_enable &= ~(1 << hw_ser_ix); /* disable global */
                    range_enable |= (1 << (hw_ser_ix + 1)); /* enable unique */

                    for (i = 0; i < NUM_PIPE(unit); i++) {
                        ser_info[info_ix + i].ser_dynamic_state |=
                            _SOC_SER_STATE_PIPE_MODE_UNIQUE;
                    }
                } else {
                    range_enable &= ~(1 << (hw_ser_ix + 1)); /* disable unique */
                    range_enable |= (1 << hw_ser_ix); /* enable global */

                    for (i = 0; i < NUM_PIPE(unit); i++) {
                        ser_info[info_ix + i].ser_dynamic_state &=
                            ~_SOC_SER_STATE_PIPE_MODE_UNIQUE;
                    }
                }
                rv = WRITE_SER_RANGE_ENABLEr(unit, range_enable);
                LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit, "mem_update for mem = %s, "
                             "range_enable = 0x%x,  hw_ser_ix = %0d\n"),
                             SOC_MEM_NAME(unit,mem), range_enable, hw_ser_ix));
                if (SOC_FAILURE(rv)) {
                    goto _error;
                }
                break;
            }
            info_ix++;
        }
_error:
        SOC_IF_ERROR_RETURN(soc_generic_ser_mem_scan_start(unit));
    }
#endif /* INCLUDE_MEM_SCAN */
    return rv;
}

STATIC int
_soc_ser_sync_mac_limits(int unit, soc_mem_t mem)
{
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_ser_sync_mac_limits "
                    "routine\n")));
#ifdef BCM_XGS_SWITCH_SUPPORT
    
    SOC_IF_ERROR_RETURN(soc_l2x_freeze(unit));
    if (_SOC_DRV_MEM_CHK_L2_MEM(mem)) {
        /* Read L2 table from hardware and update the */
        /* Decrement port/trunk and vlan/vfi counts */
#if defined(BCM_TRIUMPH3_SUPPORT)
        if (SOC_IS_TRIUMPH3(unit)) {
            soc_tr3_l2_entry_limit_count_update(unit);
        }
#endif
#if defined(BCM_GREYHOUND_SUPPORT)
        if (SOC_IS_GREYHOUND(unit) || SOC_IS_HURRICANE3(unit)) {
            soc_gh_l2_entry_limit_count_update(unit);
        }
#endif
#if defined(BCM_TRIUMPH2_SUPPORT)
        if (SOC_IS_TRIUMPH2(unit) ||
           SOC_IS_KATANA(unit) ||
           SOC_IS_APOLLO(unit)) {
            soc_triumph_l2_entry_limit_count_update(unit);
        }
#endif
    } else {
        /* Calculate and restore port/trunk and vlan/vfi counts */
        if (mem == VLAN_OR_VFI_MAC_COUNTm) {
#if defined(BCM_TRIUMPH3_SUPPORT)
            if (SOC_IS_TRIUMPH3(unit)) {
                soc_tr3_l2_entry_limit_count_update(unit);
            }
#endif
#if defined(BCM_TRIUMPH2_SUPPORT)
        if (SOC_IS_TRIUMPH2(unit) ||
            SOC_IS_KATANA(unit) ||
            SOC_IS_APOLLO(unit)) {
            soc_triumph_l2_entry_limit_count_update(unit);
        }
#endif
        }
    }
    SOC_IF_ERROR_RETURN(soc_l2x_thaw(unit));
#endif /* BCM_XGS_SWITCH_SUPPORT */
    return SOC_E_NONE;
}

STATIC int
_soc_ser_check_hard_fault(int unit, soc_mem_t mem, int at,
                          int copyno, int index, uint32 *cptr,
                          int at_other, int pipe, int is_physical_index)
{
    int rv;
    uint8 hf = FALSE;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 flags = SOC_MEM_NO_FLAGS;
    uint32 bytes = soc_mem_entry_bytes(unit, mem);

    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_ser_check_hard_fault "
                    "routine\n")));

    if (is_physical_index) {
        flags = SOC_MEM_DONT_MAP_INDEX;
    }
    /* Read back to ensure correction happened properly OR if there
     * is a hard fault.
     */
    if (((1 == pipe) && 
         ((_SOC_ACC_TYPE_PIPE_BCAST == at) ||
          (_SOC_ACC_TYPE_PIPE_SBS == at))) ||
         (_SOC_ACC_TYPE_PIPE_Y == at)) { /* Use h/w (other pipe) cache */
        IP_ARBITER_LOCK(unit);
#if defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
            soc_trident_pipe_select(unit, TRUE, 1);
            soc_trident_pipe_select(unit, FALSE, 1);
        }
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
            soc_trident2_pipe_select(unit, TRUE, 1);
            soc_trident2_pipe_select(unit, FALSE, 1);
        } 
#endif
        rv = soc_mem_pipe_select_read(unit, flags, mem,
                                      copyno, at, index, entry);
#if defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
            soc_trident_pipe_select(unit, TRUE, 0);
            soc_trident_pipe_select(unit, FALSE, 0);
        }
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TITAN2X(unit)) {
            soc_trident2_pipe_select(unit, TRUE, 0);
            soc_trident2_pipe_select(unit, FALSE, 0);
        }
#endif
        IP_ARBITER_UNLOCK(unit);
    } else {
        rv = soc_mem_read_extended(unit, flags | SOC_MEM_SCHAN_ERR_RETURN,
                                   mem, 0, copyno, index, entry);
    }
    if (SOC_FAILURE(rv)) {
        /* Obvious hard fault */
        hf = TRUE;
        LOG_ERROR(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "Hard fault detected (read) at: %s.%s[%d] !!\n"),
                   SOC_MEM_NAME(unit, mem),
                   SOC_BLOCK_NAME(unit, copyno),
                   index));
    } else {
        /* Check for multi bit or other forms of un-detectable errors
           by comparing read back data from h/w with cache(s/w or h/w) */
        if (cptr != NULL) {
            if (at != -1) {
                soc_mem_parity_field_clear(unit, mem, entry, cptr);
                if (sal_memcmp(entry, cptr, bytes) != 0) {
#if defined(BCM_TRIDENT_SUPPORT) && defined(INCLUDE_MEM_SCAN)
                    uint32 mask[SOC_MAX_MEM_WORDS];
                    uint32 dw = 0;
                    uint32 entry_dw = soc_mem_entry_words(unit, mem);
                    uint32 mask_length = SOC_MAX_MEM_WORDS * sizeof(uint32);

                    sal_memset((void *)mask, 0xff, mask_length);
                    soc_mem_scan_mask_get(unit, mem, copyno, at, mask, mask_length);
                    for (dw = 0; dw < entry_dw; dw++) {
                        if (((entry[dw] ^ cptr[dw]) &
                             mask[dw]) != 0) {
                            break;
                        }
                    }
                    if (dw < entry_dw)
#endif
                    {
                        hf = TRUE;
                        LOG_ERROR(BSL_LS_SOC_SER,
                                  (BSL_META_U(unit,
                                              "Hard fault detected (pipe compare) "
                                              "at: %s.%s[%d] !!\n"),
                                   SOC_MEM_NAME(unit, mem),
                                   SOC_BLOCK_NAME(unit, copyno),
                                   index));
                    }
                }
            } else {
                
                
            }
        }
    }
    if (hf == TRUE) {
        /* Raise a hard fault event and disable parity if applicable */
        soc_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                           SOC_SWITCH_EVENT_DATA_ERROR_FATAL, mem,
                           index);
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit)) {
                soc_th_mem_parity_control(unit, mem, copyno, FALSE, FALSE);
            } else
#endif
            {
                soc_trident2_mem_parity_control(unit, mem, copyno, FALSE);
            }
        }
#endif
#if defined(BCM_TRIDENT_SUPPORT)
        if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
            _soc_trident_mem_parity_control(unit, mem, copyno, FALSE);
        }
#endif
    }
    return rv;
}

STATIC int
_soc_ser_correction_event_generate(int unit, soc_switch_event_t event, _soc_ser_correct_info_t *si, 
                   uint32 arg1, uint32 arg2)
{
    if (soc_property_get(unit, spn_ENHANCED_SER_CORRECTION_EVENT_REPORT, 0)) {
        if (si != NULL) {
            if (si->parity_type == SOC_PARITY_TYPE_ECC) {
                if (si->double_bit) {
                    soc_event_generate(unit, event, SOC_SWITCH_EVENT_DATA_ERROR_ECC_2BIT_CORRECTED, arg1, arg2);
                } else {
                    soc_event_generate(unit, event, SOC_SWITCH_EVENT_DATA_ERROR_ECC_1BIT_CORRECTED, arg1, arg2);
                }
            } else {
                soc_event_generate(unit, event, SOC_SWITCH_EVENT_DATA_ERROR_PARITY_CORRECTED, arg1, arg2);
            }
        }
    } else {
        soc_event_generate(unit, event, SOC_SWITCH_EVENT_DATA_ERROR_CORRECTED, arg1, arg2);
    }
    return SOC_E_NONE;
}

STATIC int
_soc_ser_recovery_hw_cache(int unit, int pipe, soc_mem_t mem, int copyno,
                           int index, _soc_ser_correct_info_t *si, int is_physical_index)
{
    int acc_type=0, cache_acc_type=0;
    uint32 entry[SOC_MAX_MEM_WORDS];
    uint32 flags = SOC_MEM_NO_FLAGS;
    soc_stat_t *stat = SOC_STAT(unit);
    int rv;
    void *null_entry;
    int entry_dw;
#if defined(BCM_TOMAHAWK_SUPPORT)
    int restore_pipe = 0;
#endif /* BCM_TOMAHAWK_SUPPORT */
    soc_mem_t report_mem = mem;
    int report_index = index;

    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_ser_recovery_hw_cache "
                    "routine\n")));

    if ((SOC_IS_TD2P_TT2P(unit) ||
         SOC_IS_TD2_TT2(unit) || SOC_IS_TD_TT(unit)) && \
        (!SOC_IS_APACHE(unit))) {
        if (pipe == SOC_PIPE_ANY) {
            pipe = 0;
        }
        switch (mem) {
        case L3_DEFIP_ALPM_RAWm:
        case L3_DEFIP_ALPM_IPV4m:
        case L3_DEFIP_ALPM_IPV4_1m:
        case L3_DEFIP_ALPM_IPV6_64m:
        case L3_DEFIP_ALPM_IPV6_64_1m:
        case L3_DEFIP_ALPM_IPV6_128m:
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
        case L3_DEFIP_ALPM_ECCm:
        case L2_ENTRY_LPm:
        case L2_ENTRY_ISS_LPm:
        case L3_ENTRY_LPm:
        case L3_ENTRY_ISS_LPm:
        case VLAN_XLATE_LPm:
        case EGR_VLAN_XLATE_LPm:
#endif /* BCM_TOMAHAWK_SUPPORT */
        case L3_DEFIP_AUX_TABLEm:
        case L3_DEFIP_PAIR_128m:
        case L3_DEFIPm:
        case L3_DEFIP_128_DATA_ONLYm:
        case L3_DEFIP_PAIR_128_DATA_ONLYm:
        case L3_DEFIP_DATA_ONLYm:
#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit)) {
                if (NUM_PIPE(unit) == 1) {
                    return SOC_E_UNAVAIL;
                }
                if (pipe >= NUM_PIPE(unit)) {
                    return SOC_E_PARAM;
                }
                restore_pipe = pipe ? 0 : 1;
                acc_type = _SOC_TH_ACC_TYPE_FOR_PIPE(pipe);
                cache_acc_type = _SOC_TH_ACC_TYPE_FOR_PIPE(restore_pipe);
                /* LOG_VERBOSE(BSL_LS_SOC_SER,
                 *             (BSL_META_U(unit,
                 *                         "th_dbg: pipe=%0d, restore_pipe=%0d, acc_type=%0d, cache_acc_type=%0d\n"),
                 *              unit, pipe, restore_pipe, acc_type,
                 *              cache_acc_type));
                 */
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                if (pipe == 0) {
                    acc_type = _SOC_MEM_ADDR_ACC_TYPE_PIPE_X;
                    cache_acc_type = _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y;
                } else if (pipe == 1) {
                    acc_type = _SOC_MEM_ADDR_ACC_TYPE_PIPE_Y;
                    cache_acc_type = _SOC_MEM_ADDR_ACC_TYPE_PIPE_X;
                } else {
                    return SOC_E_PARAM;
                }
            }
            if (is_physical_index) {
                flags = SOC_MEM_DONT_MAP_INDEX;
            }
            rv = soc_mem_pipe_select_read(unit, flags, mem,
                                          copyno, cache_acc_type,
                                          index, entry);
            if (SOC_FAILURE(rv)) {
                null_entry = soc_mem_entry_null(unit, mem);
                sal_memcpy(entry, null_entry, soc_mem_entry_words(unit, mem) *
                           sizeof(uint32));
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "Un-expected error in %s[%d] blk: %s at same "
                                      "index[%d] in multiple pipes.\n"),
                           SOC_MEM_NAME(unit, mem), mem,
                           SOC_BLOCK_NAME(unit, copyno), index));
            }
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HITf);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT0f);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT1f);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT_0f);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT_1f);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT_2f);
            _SOC_ENTRY_FIELD_CHK_CLR(unit, mem, entry, HIT_3f);
            SOC_IF_ERROR_RETURN
                (soc_mem_pipe_select_write(unit, flags, mem,
                                           copyno, acc_type, index, entry));
            /* If mem == L3_DEFIP_ALPM_RAWm, it means we are recovering
             * the ALPM bucket which is unused, the memory view of this
             * bucket is not determined, so we used raw view for recovering,
             * and since it's not used, event generation is not necessary.
             */
            if (mem != L3_DEFIP_ALPM_RAWm) {
#if defined(BCM_TRIDENT2_SUPPORT)
                /* For L3_DEFIP/L3_DEFIP_DATA_ONLY view the input index is physical 
                 * index, so need to remap to logical index. For L3_DEFIP_PAIR_128_DATA_ONLY view
                 * when correcting the view will be changed to L3_DEFIP_DATA_ONLY.
                 * For L3_DEFIP_PAIR_128 view, When L3_ALPM_ENABLE=2,  the physical index
                 * is equal to logical index, but When L3_ALPM_ENABLE=1, the physical index
                 * is not equal to logical index, so need to remap physical index to logical one.
                 */
                if (is_physical_index && SOC_IS_TD2_TT2(unit) && !SOC_IS_APACHE(unit)) {
                    if (mem == L3_DEFIPm || mem == L3_DEFIP_DATA_ONLYm) {
                        report_index =
                            soc_trident2_l3_defip_mem_index_get(unit,
                                                                index,
                                                                &report_mem);
                        if (report_mem == L3_DEFIP_PAIR_128m) {
                            report_mem = L3_DEFIP_PAIR_128_DATA_ONLYm;
                        } else if (report_mem != mem) {
                            report_mem = mem;
                        }
                    } else if (mem == L3_DEFIP_PAIR_128m) {
                        report_index=
                            soc_trident2_l3_defip_index_remap(unit,
                                                              mem,
                                                              index);
                    }
                }
#endif
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                                   si, report_mem, report_index);
            }
#if defined(BCM_TOMAHAWK_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit)) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "RESTORE pipe %0d [from pipe %0d]: %s[%d] "
                                      "blk: %s index: %d\n"),
                           pipe, restore_pipe, SOC_MEM_NAME(unit, report_mem),
                           report_mem, SOC_BLOCK_NAME(unit, copyno), report_index));
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "RESTORE[from %c pipe]: %s[%d] "
                                      "blk: %s index: %d\n"),
                           pipe ? 'X':'Y', SOC_MEM_NAME(unit, report_mem),
                           report_mem, SOC_BLOCK_NAME(unit, copyno), report_index));
            }
            stat->ser_err_restor++;
            if (NULL != si) {
                entry_dw = soc_mem_entry_words(unit, mem);
                if ((si->log_id != 0) &&
                    (si->flags & SOC_SER_LOG_WRITE_CACHE)) {
                    if (si->index == index) {
                        (void)soc_ser_log_add_tlv(unit, si->log_id,
                                                  SOC_SER_LOG_TLV_CACHE,
                                                  entry_dw*4, entry);
                    } else {
                        LOG_VERBOSE(BSL_LS_SOC_SER,
                            (BSL_META_U(unit,
                                        "skipped updating tlv_cache index: %d\n"),
                             index));
                    }
                }
            }
            return _soc_ser_check_hard_fault(unit, mem, acc_type, copyno,
                                             index, entry, cache_acc_type,
                                             pipe, is_physical_index);
        default:
            return SOC_E_UNAVAIL;
        }
    }
    return SOC_E_UNAVAIL;
}

#if defined(BCM_TOMAHAWK_SUPPORT)
STATIC int
_soc_ser_mem_mode_get(int unit, soc_mem_t mem) {
    int rv;
    int tmp_mem_mode = _SOC_SER_MEM_MODE_GLOBAL;
    rv = soc_th_ser_mem_mode_get(unit, mem, &tmp_mem_mode);
    if ((rv == SOC_E_NONE) && (tmp_mem_mode == _SOC_SER_MEM_MODE_PIPE_UNIQUE)) {
        return _SOC_SER_MEM_MODE_PIPE_UNIQUE;
    } else {
        if (rv != SOC_E_NONE) {
            LOG_ERROR(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "Could not determine unique-global mode for"
                            "mem %s - will assume global mode !!\n"),
                 SOC_MEM_NAME(unit, mem)));
        }
        return _SOC_SER_MEM_MODE_GLOBAL;
    }
}
#endif /* BCM_TOMAHAWK_SUPPORT */

#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
/* Following maps ecc_entry_index to lp_entry_index */
STATIC int
_soc_fv_lp_index_remap(int fpem_ecc_index, int em2)
{
    int uft_bank, fpem_ecc_index_in_uft_bank, lp_bkt_adr;

    /* Each uft bank has 32K _ecc entries */
    uft_bank = fpem_ecc_index / 32768;

    /* address within uft_bank */
    fpem_ecc_index_in_uft_bank = fpem_ecc_index % 32768; 

    if (em2) {
        /* Each em2 entry has 2 fpem_ecc entries and every
         * lp_bkt has 4 em2 entries and thus 8 _ecc entries.
         * Each uft_bank has 8K lp entries */
        lp_bkt_adr = fpem_ecc_index_in_uft_bank / 8;
        return (uft_bank * 8192) + lp_bkt_adr;
    } else {
        /* Each em4 entry has 4 fpem_ecc entries and every
         * lp_bkt has 4 em4 entries and thus 16 _ecc entries.
         * Each uft_bank has 8K lp entries */
        lp_bkt_adr = fpem_ecc_index_in_uft_bank / 16;
        return (uft_bank * 8192) + lp_bkt_adr + 4096;
    }
}
#endif

/* hw_cpu: Need special handling for hw vs cpu error on some devices/mems.
           hw=0, cpu=1.
*/
STATIC int
_soc_ser_overlay_mem_correction(int unit, int hw_cpu, int pipe, int sblk, int addr,
                                soc_mem_t reported_mem, int copyno, int index,
                                _soc_ser_correct_info_t *si)
{
#define _SOC_SER_MEM_OVERLAY_SET 6
    soc_mem_t mem = INVALIDm;
    soc_mem_t mem_for_write = INVALIDm;
    void *null_entry;
    int rv, entry_dw;
    uint8 fix = 0, *vmap;
    int i, m, num_mems, vindex = 0, index_for_write = 0;
    int clear_mems = 1;
    soc_stat_t *stat = SOC_STAT(unit);
    uint32 entry[SOC_MAX_MEM_WORDS], *cache;
    int num_indexes[_SOC_SER_MEM_OVERLAY_SET] = {1, 1, 1, 1, 1, 1};
    int iratio[_SOC_SER_MEM_OVERLAY_SET] = {1, 1, 1, 1, 1, 1};
    soc_mem_t mem_list[_SOC_SER_MEM_OVERLAY_SET] =
                       {INVALIDm, INVALIDm, INVALIDm, INVALIDm};
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    soc_field_t ecc_data_field = INVALIDf;
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_TOMAHAWK_SUPPORT)
    int fpem_mem_mode = _SOC_SER_MEM_MODE_GLOBAL;
#endif
#if defined(BCM_TRIDENT2_SUPPORT)
        int defip_cam_size = SOC_L3_DEFIP_TCAM_DEPTH_GET(unit);
        int phy_index = 0;
#endif

    int mem_lock = 0;
    int reported_mem_is_fpem_lp = 0; /* reported_mem in ser_fifo */
    int use_pipe_select_write = 0, acc_type = 0;

    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered "
                    "_soc_ser_overlay_mem_correction routine\n")));

    if (copyno == MEM_BLOCK_ANY) {
        copyno = SOC_MEM_BLOCK_ANY(unit, reported_mem);
    }

    mem_list[0] = reported_mem;
    switch (reported_mem) {
    case MODPORT_MAP_SWm:
        if (SOC_IS_TRX(unit) &&
            !(SOC_IS_TRIUMPH3(unit) || SOC_IS_TD_TT(unit) || SOC_IS_TD2_TT2(unit) ||
              SOC_IS_KATANA2(unit) || SOC_IS_GREYHOUND(unit) ||
              SOC_IS_HURRICANEX(unit))) {
            num_mems = 1;
            mem_list[0] = MODPORT_MAPm;
            break;
        } else {
            LOG_VERBOSE(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "th_dbg: returning SOC_E_UNAVAIL "
                            "for MODPORT_MAP_SW\n")));
            return SOC_E_UNAVAIL;
        }
    case MODPORT_MAP_M0m:
    case MODPORT_MAP_M1m:
    case MODPORT_MAP_M2m:
    case MODPORT_MAP_M3m:
        num_mems = 1;
        mem_list[0] = MODPORT_MAP_MIRRORm;
        break;
    case ESM_PKT_TYPE_ID_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = ESM_PKT_TYPE_IDm;
        break;
    case ING_SNAT_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = ING_SNATm;
        break;
    case VLAN_SUBNET_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = VLAN_SUBNETm;
        break;
    case MY_STATION_TCAM_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = MY_STATION_TCAMm;
        break;
    case L2_USER_ENTRY_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = L2_USER_ENTRYm;
        break;
    case L3_TUNNEL_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = L3_TUNNELm;
        break;
#if defined(BCM_TOMAHAWK_SUPPORT)
/* swname for following is L2_USER_ENTRY_DATA_ONLYm which is
 * already covered above
    case L2_USER_ENTRY_DATAm:
        num_mems = 1;
        mem_list[0] = L2_USER_ENTRYm;
        break;
*/
    case SRC_COMPRESSION_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = SRC_COMPRESSIONm;
        if (_SOC_SER_MEM_MODE_PIPE_UNIQUE ==
                _soc_ser_mem_mode_get(unit, mem_list[0]) && 
            (SOC_MEM_UNIQUE_ACC(unit, mem_list[0]) != NULL)  &&
            (pipe <  NUM_PIPE(unit))) {
            mem_list[0] = SOC_MEM_UNIQUE_ACC(unit, mem_list[0])[pipe];
        }
        break;
    case DST_COMPRESSION_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = DST_COMPRESSIONm;
        if (_SOC_SER_MEM_MODE_PIPE_UNIQUE ==
                _soc_ser_mem_mode_get(unit, mem_list[0]) && 
            (SOC_MEM_UNIQUE_ACC(unit, mem_list[0]) != NULL)  &&
            (pipe <  NUM_PIPE(unit))) {
            mem_list[0] = SOC_MEM_UNIQUE_ACC(unit, mem_list[0])[pipe];
        }
        break;
    case EXACT_MATCH_LOGICAL_TABLE_SELECT_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = EXACT_MATCH_LOGICAL_TABLE_SELECTm;
        if (_SOC_SER_MEM_MODE_PIPE_UNIQUE ==
                _soc_ser_mem_mode_get(unit, mem_list[0]) && 
            (SOC_MEM_UNIQUE_ACC(unit, mem_list[0]) != NULL)  &&
            (pipe <  NUM_PIPE(unit))) {
            mem_list[0] = SOC_MEM_UNIQUE_ACC(unit, mem_list[0])[pipe];
        }
        break;
    case IFP_LOGICAL_TABLE_SELECT_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = IFP_LOGICAL_TABLE_SELECTm;
        if (_SOC_SER_MEM_MODE_PIPE_UNIQUE ==
                _soc_ser_mem_mode_get(unit, mem_list[0]) && 
            (SOC_MEM_UNIQUE_ACC(unit, mem_list[0]) != NULL)  &&
            (pipe <  NUM_PIPE(unit))) {
            mem_list[0] = SOC_MEM_UNIQUE_ACC(unit, mem_list[0])[pipe];
        }
        break;
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_TOMAHAWK2_SUPPORT)
    case ING_VP_VLAN_MEMBERSHIP_ECCm:
        num_mems = 1;
        mem_list[0] = ING_VP_VLAN_MEMBERSHIPm;
        break;
    case EGR_VP_VLAN_MEMBERSHIP_ECCm:
        num_mems = 1;
        mem_list[0] = EGR_VP_VLAN_MEMBERSHIPm;
        break;
    case ING_DNAT_ADDRESS_TYPE_ECCm:
        num_mems = 1;
        mem_list[0] = ING_DNAT_ADDRESS_TYPEm;
        break;
#endif
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    case MY_STATION_TCAM_2_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = MY_STATION_TCAM_2m;
        break;
    case SUBPORT_TAG_SGPP_MAP_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = SUBPORT_TAG_SGPP_MAPm;
        break;
#endif
    case L2_ENTRY_1m:
        num_mems = 2;
        mem_list[1] = L2_ENTRY_2m; iratio[1] = 2;
        break;
    case MPLS_ENTRYm:
    case MPLS_ENTRY_1m:
        if (soc_feature(unit, soc_feature_ism_memory)) {
            num_mems = 2;
            mem_list[1] = MPLS_ENTRY_EXTDm; iratio[1] = 2;
            break;
        } else {
            return SOC_E_UNAVAIL;
        }
#if defined(BCM_TOMAHAWK_SUPPORT)
    case MPLS_ENTRY_ECCm:
        num_mems = 1;
        mem_list[0] = MPLS_ENTRYm;
        break;
#endif /* BCM_TOMAHAWK_SUPPORT */
    case VLAN_XLATE_1m:
        num_mems = 2;
        mem_list[1] = VLAN_XLATE_EXTDm; iratio[1] = 2;
        break;
    case L3_ENTRY_1m:
        num_mems = 3;
        mem_list[1] = L3_ENTRY_2m; iratio[1] = 2;
        mem_list[2] = L3_ENTRY_4m; iratio[2] = 4;
        break;
    case EGR_IP_TUNNELm:
        if (SOC_IS_TRIDENT(unit) &&
            !(soc_feature(unit, soc_feature_special_egr_ip_tunnel_ser))) {
            num_mems = index ? 6 : 3;
            index *= 4; /* To keep the index ratio based logic below, consistent */
            num_indexes[0] = 4; num_indexes[1] = 4;
            num_indexes[2] = 2; num_indexes[3] = 4;
            num_indexes[4] = 4; num_indexes[5] = 2;
            mem_list[0] = EGR_IP_TUNNELm;
            mem_list[1] = EGR_IP_TUNNEL_MPLSm;
            mem_list[2] = EGR_IP_TUNNEL_IPV6m; iratio[2] = 2;
            mem_list[3] = EGR_IP_TUNNELm; iratio[3] = 4;
            mem_list[4] = EGR_IP_TUNNEL_MPLSm; iratio[4] = 4;
            mem_list[5] = EGR_IP_TUNNEL_IPV6m; iratio[5] = 4;
        } else if (soc_feature(unit, soc_feature_special_egr_ip_tunnel_ser)) {
            /* Note: This is an exception case where the wider memories index is
             *       used for reporting the error on a hw lookup while view name
             *       received is always EGR_IP_TUNNEL in the correction routine.
             *       On a cpu access error the actual view used for access is
             *       used for reporting the error index but again the view name
             *       is always EGR_IP_TUNNEL.
             *       So for a reported index 'n' we will need to check/fix
             *       index n/2 and n*2 as well.
             */
            num_mems = index ? 6 : 3;
            index *= 2; /* To keep the index ratio based logic below, consistent */
            num_indexes[0] = 2; num_indexes[1] = 2;
            num_indexes[3] = 2; num_indexes[4] = 2;
            mem_list[0] = EGR_IP_TUNNELm;
            mem_list[1] = EGR_IP_TUNNEL_MPLSm;
            mem_list[2] = EGR_IP_TUNNEL_IPV6m; iratio[2] = 2;
            mem_list[3] = EGR_IP_TUNNELm; iratio[3] = 2;
            mem_list[4] = EGR_IP_TUNNEL_MPLSm; iratio[4] = 2;
            mem_list[5] = EGR_IP_TUNNEL_IPV6m; iratio[5] = 4;
        } else {
            num_mems = 3;
            mem_list[1] = EGR_IP_TUNNEL_MPLSm;
            mem_list[2] = EGR_IP_TUNNEL_IPV6m; iratio[2] = 2;
#if defined(BCM_APACHE_SUPPORT) || defined(BCM_TOMAHAWK2_SUPPORT)
            if (SOC_IS_APACHE(unit) || SOC_IS_TOMAHAWK2(unit)) {
                /* Index reported for EGR_IP_TUNNEL_MPLSm is different for apache.
                 * Index reported is changed by factor of two.
                 * Adjust index ratio accordingly 
                 */
                iratio[1] = 2;
            }
#endif /* BCM_APACHE_SUPPORT */
        }
        break;
    case L3_ENTRY_ONLYm:
        if (SOC_MEM_IS_VALID(unit, L3_ENTRY_IPV4_UNICASTm)) {
            mem_list[0] = L3_ENTRY_IPV4_UNICASTm;
        }
        /* passthru */
        /* coverity[fallthrough: FALSE] */
    case L3_ENTRY_IPV4_UNICASTm:
        num_mems = 4;
        mem_list[0] = L3_ENTRY_IPV4_UNICASTm; iratio[0] = 1;
#ifdef BCM_SCORPION_SUPPORT
        if (SOC_IS_SCORPION(unit)) {
            mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 1;
        } else
#endif
        {
            mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 2;
        }
        mem_list[2] = L3_ENTRY_IPV6_UNICASTm; iratio[2] = 2;
        mem_list[3] = L3_ENTRY_IPV6_MULTICASTm; iratio[3] = 4;
        break;
    case L3_ENTRY_IPV4_MULTICASTm:
        /* The iratio array is for IPv4_UC view, so need to convert the index to right one */
        index *= 2;
        num_mems = 4;
        mem_list[0] = L3_ENTRY_IPV4_UNICASTm; iratio[0] = 1;
        mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 2;
        mem_list[2] = L3_ENTRY_IPV6_UNICASTm; iratio[2] = 2;
        mem_list[3] = L3_ENTRY_IPV6_MULTICASTm; iratio[3] = 4;
        break;
    case L3_ENTRY_IPV6_UNICASTm:
        /* The iratio array is for IPv4_UC view, so need to convert the index to right one */
        index *= 2;
        num_mems = 4;
        mem_list[0] = L3_ENTRY_IPV4_UNICASTm; iratio[0] = 1;
        mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 2;
        mem_list[2] = L3_ENTRY_IPV6_UNICASTm; iratio[2] = 2;
        mem_list[3] = L3_ENTRY_IPV6_MULTICASTm; iratio[3] = 4;
        break;
    case L3_ENTRY_IPV6_MULTICASTm:
        /* The iratio array is for IPv4_UC view, so need to convert the index to right one */
        index *= 4;
        num_mems = 4;
        mem_list[0] = L3_ENTRY_IPV4_UNICASTm; iratio[0] = 1;
        mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 2;
        mem_list[2] = L3_ENTRY_IPV6_UNICASTm; iratio[2] = 2;
        mem_list[3] = L3_ENTRY_IPV6_MULTICASTm; iratio[3] = 4;
        break;
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    case L3_ENTRY_ONLY_ECCm:
        num_mems = 4;
        mem_list[0] = L3_ENTRY_IPV4_UNICASTm; iratio[0] = 1;
        mem_list[1] = L3_ENTRY_IPV4_MULTICASTm; iratio[1] = 2;
        mem_list[2] = L3_ENTRY_IPV6_UNICASTm; iratio[2] = 2;
        mem_list[3] = L3_ENTRY_IPV6_MULTICASTm; iratio[3] = 4;
        break;
#endif
#if defined(BCM_TOMAHAWK_SUPPORT)
    case FPEM_ECCm:
        fpem_mem_mode = _soc_ser_mem_mode_get(unit, EXACT_MATCH_2m);
            /* Note: Assume both EM2, EM4 are always in same mode */
        num_mems = 2;
        mem_list[0] = _SOC_FPEM_EM2(pipe, fpem_mem_mode); iratio[0] = 2;
        mem_list[1] = _SOC_FPEM_EM4(pipe, fpem_mem_mode); iratio[1] = 4;
        break;
    case EXACT_MATCH_2m:
        reported_mem_is_fpem_lp = 1;
        /* means mem reported in ser_fifo is FPEM_LPm */
        fpem_mem_mode = _soc_ser_mem_mode_get(unit, reported_mem);
        num_mems = 1;
        mem_list[0] = _SOC_FPEM_EM2(pipe, fpem_mem_mode); iratio[0] = 2;
        num_indexes[0] = _SOC_SER_NUM_EM_ENTRIES_PER_LP_ENTRY;
        /* each FPEM_LP entry covers 4 EM2 entries */
        break;
    case EXACT_MATCH_4m:
        reported_mem_is_fpem_lp = 1;
        /* means mem reported in ser_fifo is FPEM_LPm */
        fpem_mem_mode = _soc_ser_mem_mode_get(unit, reported_mem);
        num_mems = 1;
        mem_list[0] = _SOC_FPEM_EM4(pipe, fpem_mem_mode); iratio[0] = 4;
        num_indexes[0] = _SOC_SER_NUM_EM_ENTRIES_PER_LP_ENTRY;
        /* each FPEM_LP entry covers 4 EM4 entries */
        break;
#endif /* BCM_TOMAHAWK_SUPPORT */
    case L3_DEFIP_128_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = L3_DEFIP_128m;
        break;
#if defined(BCM_TRIDENT2_SUPPORT)
    case L3_DEFIP_PAIR_128_DATA_ONLYm:
        num_mems = 1;
        mem_list[0] = L3_DEFIP_PAIR_128_DATA_ONLYm;
        if (SOC_IS_APACHE(unit)) {
            /* Using cache restore for Apache */
            mem_list[0] = L3_DEFIP_PAIR_128m;
        } else if (SOC_IS_TD2_TT2(unit)) {
            num_indexes[0] = 2;
            index = (index / defip_cam_size) * defip_cam_size * 2 +
                    (index % defip_cam_size);
        }
        break;
#endif /* BCM_TRIDENT2_SUPPORT */
    case L3_DEFIP_DATA_ONLYm:
        if (soc_feature(unit, soc_feature_l3_defip_map)) {
            num_mems = 1;
#ifdef BCM_TRIUMPH3_SUPPORT
            if ((SOC_IS_TRIUMPH3(unit)) || ((SOC_IS_HELIX4(unit)) &&
                (!soc_feature(unit, soc_feature_l3_256_defip_table))) ) {
                index = soc_tr3_l3_defip_mem_index_get(unit, index,
                                                       &mem_list[0]);
            } else
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA_SUPPORT
            if (SOC_IS_KATANA(unit)) {
                index = soc_kt_l3_defip_mem_index_get(unit, index,
                                                      &mem_list[0]);
            } else
#endif /* BCM_TRIUMPH3_SUPPORT */
#ifdef BCM_KATANA2_SUPPORT
            if (SOC_IS_KATANA2(unit)) {
                index = soc_kt2_l3_defip_index_map(unit, mem_list[0], index);
                index = soc_kt2_l3_defip_mem_index_get(unit, index,
                                                       &mem_list[0]);
                if (mem_list[0] == L3_DEFIP_DATA_ONLYm) {
                    mem_list[0] = L3_DEFIPm;
                } else if (mem_list[0] == L3_DEFIP_PAIR_128_DATA_ONLYm) {
                    mem_list[0] = L3_DEFIP_PAIR_128m;
                }
            } else
#endif /* BCM_KATANA2_SUPPORT */
#if defined(BCM_APACHE_SUPPORT)
            if (SOC_IS_APACHE(unit)) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "ap_dbg: "
                                "phys_index = %0d for mem = %0d\n"),
                     index, mem_list[0]));
                /* The index is logical index, we need convert to physical index first*/
                index = soc_apache_l3_defip_index_map(unit, mem_list[0], index);

                index = soc_apache_l3_defip_mem_index_get(unit, index,
                                                          &mem_list[0]);
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "ap_dbg: "
                                "logical_index = %0d  for mem = %0d\n"),
                     index, mem_list[0]));
            } else
#endif /* BCM_APACHE_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TD2_TT2(unit)) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: "
                                "phys_index = %0d for mem = %0d\n"),
                     index, mem_list[0]));

                (void)soc_trident2_l3_defip_mem_index_get(unit, index,
                                                            &mem_list[0]);
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: "
                                "logical_index = %0d  for mem = %0d\n"),
                     index, mem_list[0]));
                /* Lets only correct the data portion of the TCAM to avoid
                   getting into deadlock situation in bulk op retry for TD2.
                   Note that mem cache is not used for this memory anyways. */
                if (mem_list[0] == L3_DEFIPm) {
                    mem_list[0] = L3_DEFIP_DATA_ONLYm;
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "th_dbg: mem_for_restore=%0d:"
                                    "L3_DEFIP_DATA_ONLYm\n"), mem_list[0]));
                } else if (mem_list[0] == L3_DEFIP_PAIR_128m) {
                    mem_list[0] = L3_DEFIP_PAIR_128_DATA_ONLYm;
                    num_indexes[0] = 2;
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                        (BSL_META_U(unit,
                                    "th_dbg: mem_for_restore=%0d:"
                                    "L3_DEFIP_PAIR_128_DATA_ONLYm\n"),
                         mem_list[0]));
                }
                if (SOC_CONTROL(unit)->alpm_bulk_retry) {
                    sal_sem_give(SOC_CONTROL(unit)->alpm_bulk_retry);
                }
            } else
#endif /* BCM_TRIDENT2_SUPPORT */
#if defined(BCM_TRIUMPH_SUPPORT)
            if (SOC_IS_TRIUMPH(unit)) {
                num_mems = 1;
                mem_list[0] = L3_DEFIPm;
                break;
            } else
#endif      /* BCM_TRIUMPH_SUPPORT */
            {
                
                return SOC_E_UNAVAIL;
            }
            break;
        } else 
#if defined(BCM_TRIUMPH_SUPPORT) || defined(BCM_ENDURO_SUPPORT)
        if (SOC_IS_TRIUMPH(unit) || SOC_IS_ENDURO(unit) || SOC_IS_GREYHOUND(unit) ||
            SOC_IS_HURRICANE3(unit)) {
#ifdef BCM_ENDURO_SUPPORT
            if (SOC_IS_ENDURO(unit)) {
                index = soc_enduro_l3_defip_index_map(unit, mem_list[0], index);    
            }
#endif /* BCM_ENDURO_SUPPORT */
            num_mems = 1;
            mem_list[0] = L3_DEFIPm;
            break;
        }  else 
#endif
        {
            return SOC_E_UNAVAIL;
        }
    default:
        return SOC_E_UNAVAIL;
    }
    for (m = 0; m < num_mems; m++) {
        mem = mem_list[m];
        entry_dw = soc_mem_entry_words(unit, mem);

        switch (mem) {
        case L2_ENTRY_1m:
        case L2_ENTRY_2m:
        case L3_ENTRY_ONLYm:
        case L3_ENTRY_IPV4_UNICASTm:
        case L3_ENTRY_IPV4_MULTICASTm:
        case L3_ENTRY_IPV6_UNICASTm:
        case L3_ENTRY_IPV6_MULTICASTm:
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
        case VLAN_SUBNETm:
            /* Take lock to ensure that cache is updated coherently */
            MEM_LOCK(unit, mem);
            mem_lock = 1;
            break;
        default:
            break;
        }

        /* coverity[negative_returns] */
        cache = SOC_MEM_STATE(unit, mem).cache[copyno];
        vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
        vindex = index/iratio[m];
        for (i = 0; i < num_indexes[m]; i++) {
            if (cache != NULL && CACHE_VMAP_TST(vmap, vindex+i)) {
                sal_memcpy(entry, cache + ((vindex+i) * entry_dw),
                           entry_dw * 4);
                mem_for_write = mem;
                index_for_write = vindex + i;
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
                if (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TRIDENT2PLUS(unit) \
                    || SOC_IS_APACHE(unit)) {
                    switch (mem) {
                    case L3_ENTRY_IPV4_UNICASTm:
                        ecc_data_field = ECC_DATAf;
                        soc_mem_field_get(unit, mem,
                                          cache + ((vindex+i) * entry_dw),
                                          ecc_data_field, entry);
                        mem_for_write = L3_ENTRY_ONLY_ECCm;
                        index_for_write = index + i;
                        break;
                    case L3_ENTRY_IPV4_MULTICASTm:
                    case L3_ENTRY_IPV6_UNICASTm:
                    case L3_ENTRY_IPV6_MULTICASTm:
                        switch(index % iratio[m]) {
                        case 1:  ecc_data_field = ECC_DATA1f; break;
                        case 2:  ecc_data_field = ECC_DATA2f; break;
                        case 3:  ecc_data_field = ECC_DATA3f; break;
                        default: ecc_data_field = ECC_DATA0f; break;
                        }
                        soc_mem_field_get(unit, mem,
                                          cache + ((vindex+i) * entry_dw),
                                          ecc_data_field, entry);
                        mem_for_write = L3_ENTRY_ONLY_ECCm;
                        index_for_write = index + i;
                        break;
#if defined(BCM_TOMAHAWK_SUPPORT)
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
                        if (!reported_mem_is_fpem_lp) {
                            /* reported_mem is FPEM_ECC */
                            switch (index % iratio[m]) {
                            case 1:  ecc_data_field = ECC_DATA1f; break;
                            case 2:  ecc_data_field = ECC_DATA2f; break;
                            case 3:  ecc_data_field = ECC_DATA3f; break;
                            default: ecc_data_field = ECC_DATA0f; break;
                            }
                            soc_mem_field_get(unit, mem,
                                              cache + ((vindex+i)*entry_dw),
                                              ecc_data_field, entry);
                            mem_for_write = _SOC_FPEM_ECC(pipe,
                                                          fpem_mem_mode);
                            index_for_write = index + i;
                        }
                        break;
#endif /* BCM_TOMAHAWK_SUPPORT */
                    case MPLS_ENTRYm:
                        if (SOC_IS_TOMAHAWKX(unit)) {
                            mem_for_write = MPLS_ENTRY_ECCm;
                        }
                        break;
                    default:
                        break;
                    }
                }
#endif /* (BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT) */
                rv = soc_mem_write(unit, mem_for_write, copyno,
                                   index_for_write, entry);
                if (NULL != si) {
                    if ((si->log_id != 0) &&
                        (si->flags & SOC_SER_LOG_WRITE_CACHE)) {
                        if (si->index == index_for_write) {
                            (void)soc_ser_log_add_tlv(unit, si->log_id,
                                                      SOC_SER_LOG_TLV_CACHE,
                                                      soc_mem_entry_words(unit, mem_for_write)*4,
                                                      entry);
                        } else {
                            LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "skipped updating tlv_cache index: %d\n"),
                                 index_for_write));
                        }
                    }
                }
                if (rv < 0) {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CACHE_RESTORE %s.%s[%d] failed: %s\n"),
                               SOC_MEM_NAME(unit, mem_for_write),
                               SOC_BLOCK_NAME(unit, copyno), index_for_write,
                               soc_errmsg(rv)));
                    if (mem_lock) {
                        MEM_UNLOCK(unit, mem);
                    }

                    return rv;
                }
                fix++;
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                                   si, mem, vindex);
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CACHE_RESTORE: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                           SOC_MEM_NAME(unit, mem_for_write),
                           mem_for_write, SOC_BLOCK_NAME(unit, copyno),
                           index_for_write, sblk, addr));
                stat->ser_err_restor++;
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: this CACHE_RESTORE was "
                                "from overlay_mem_correction\n")));
            }
            if (cache == NULL) {
                /* Do hw cache (from the other pipe) based correction if s/w mem
                   cache is not available */
#if defined(BCM_TRIDENT2_SUPPORT)
                if (((mem_list[m] == L3_DEFIP_PAIR_128_DATA_ONLYm) ||
                      mem_list[m] == L3_DEFIP_DATA_ONLYm)
                     && SOC_IS_TD2_TT2(unit) && !SOC_IS_APACHE(unit)) {
                    mem = L3_DEFIP_DATA_ONLYm;
                    if (mem_list[m] == L3_DEFIP_PAIR_128_DATA_ONLYm) {
                        phy_index= ((vindex / (defip_cam_size * 2)) * defip_cam_size * 2)
                                    + (vindex % defip_cam_size) + (i * defip_cam_size);
                    } else {
                        phy_index = vindex;
                    }
                    rv = _soc_ser_recovery_hw_cache(unit, pipe, mem, copyno,
                                                    phy_index, si, TRUE);
                } else
#endif
                {
                    rv = _soc_ser_recovery_hw_cache(unit, pipe, mem, copyno,
                                                    vindex+i, si, FALSE);
                }
                if (!(rv == SOC_E_NONE || rv == SOC_E_UNAVAIL)) {
                    return rv;
                } else if (rv == SOC_E_NONE) {
                    fix++;
                }
            }
            if (reported_mem_is_fpem_lp && (fix > 0)) {
                /* reported_mem_is_fpem_lp implies that we are doing indirect
                 * restore of LP control word by writing to one of the
                 * functional views (EXACT_MATCH_2,4) 
                 * We need to restore only one of the 4 entries for
                 * corresponding LP control word to be recomputed.
                 * Thus, after we do the 1st restore - exit.
                 *
                 * reported_mem_is_fpem_lp also implies that 'mem' is either
                 * EXACT_MATCH_2m or
                 * EXACT_MATCH_2_PIPE0m or
                 * EXACT_MATCH_2_PIPE1m or
                 * EXACT_MATCH_2_PIPE2m or
                 * EXACT_MATCH_2_PIPE3m or
                 * EXACT_MATCH_4m or
                 * EXACT_MATCH_4_PIPE0m or
                 * EXACT_MATCH_4_PIPE1m or
                 * EXACT_MATCH_4_PIPE2m or
                 * EXACT_MATCH_4_PIPE3m
                 */
                break;
            }
        } /* each index */
        if (mem_lock) {
            MEM_UNLOCK(unit, mem);
            mem_lock = 0;
        }
        
        if ((mem == L3_ENTRY_ONLYm) || (mem == L3_ENTRY_IPV4_UNICASTm) || 
            (mem == L3_ENTRY_IPV4_MULTICASTm) || (mem == L3_ENTRY_IPV6_UNICASTm) || 
            (mem == L3_ENTRY_IPV6_MULTICASTm) ||
            _SOC_MEM_CHK_FPEM_MEM(mem)) {
            if (fix != 0) {
                break;
            }
        } 
    } /* each mem */
    if (fix == 0) {
        LOG_VERBOSE(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "th_dbg: overlay_mem_correction falling "
                        "back to CLEAR_RESTORE\n")));
        /* Fall back to clear - this is always the widest view */
#if defined(BCM_TRX_SUPPORT)
        /* For L3_ENTRY_*, clear the entry in L3_ENTRY_IPV4_UNICAST view */
        if (SOC_IS_TRX(unit)) {
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
            if (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TRIDENT2PLUS(unit) || 
                SOC_IS_APACHE(unit)) {
                vindex = index;
                switch (mem) {
                case MPLS_ENTRYm:
                    if (SOC_IS_TOMAHAWKX(unit)) {
                        mem = MPLS_ENTRY_ECCm;
                    }
                    break;
                case L3_ENTRY_IPV6_MULTICASTm:
                    mem = L3_ENTRY_ONLY_ECCm;
                    break;
                case EXACT_MATCH_2m:
                    /* we can be here only when (reported_mem ==  FPEM_LPm) */
                    /* Write to ENTRY_ONLY view to prevent cache from
                     * recording that there is EXACT_MATCH_2 type entry at this
                     * location.
                     * Clear this FPEM_LPm location
                     * Because at corresponding locatons in fv, we may have EM4
                     * entries, so this LP entry is NULL */
                    mem = FPEM_LPm;
                    vindex = _soc_fv_lp_index_remap(index, 1);
                    break;
                case EXACT_MATCH_2_PIPE0m:
                case EXACT_MATCH_2_PIPE1m:
                case EXACT_MATCH_2_PIPE2m:
                case EXACT_MATCH_2_PIPE3m:
                    /* Can be here only when (reported_mem ==  FPEM_LPm) */
                    /* Write to ENTRY_ONLY view to prevent cache from
                     * recording that there is EXACT_MATCH_2 type entry at this
                     * location.
                     * Clear this FPEM_LPm location
                     * Because at corresponding locatons in fv, we may have EM4
                     * entries, so this LP entry is NULL */
                    mem = FPEM_LPm;
                    vindex = _soc_fv_lp_index_remap(index, 1);
                    use_pipe_select_write = 1;
                    acc_type = _SOC_TH_ACC_TYPE_FOR_PIPE(pipe);
                    break;
                case EXACT_MATCH_4m:
                    if (!reported_mem_is_fpem_lp) {
                        /* reported_mem is FPEM_ECCm */
                        mem = FPEM_ECCm;
                    } else { /* (reported_mem == FPEM_LPm) */
                        /* Write to ENTRY_ONLY view to prevent cache from
                         * recording that there is EXACT_MATCH_4 type entry at
                         * this location.
                         * Clear this FPEM_LPm location
                         * Because at corresponding locatons in fv, we may have EM2
                         * entries, so this LP entry is NULL */
                        mem = FPEM_LPm;
                        vindex = _soc_fv_lp_index_remap(index, 0);
                    }
                    break;
                case EXACT_MATCH_4_PIPE0m:
                case EXACT_MATCH_4_PIPE1m:
                case EXACT_MATCH_4_PIPE2m:
                case EXACT_MATCH_4_PIPE3m:
                    if (!reported_mem_is_fpem_lp) {
                        /* reported_mem is FPEM_ECCm */
                        mem = _SOC_FPEM_ECC(pipe,
                                            _SOC_SER_MEM_MODE_PIPE_UNIQUE);
                    } else { /* (reported_mem == FPEM_LPm) */
                        /* Write to ENTRY_ONLY view to prevent cache from
                         * recording that there is EXACT_MATCH_4 type entry at
                         * this location.
                         * Clear this FPEM_LPm location
                         * Because at corresponding locatons in fv, we may have EM2
                         * entries, so this LP entry is NULL */
                        mem = FPEM_LPm;
                        vindex = _soc_fv_lp_index_remap(index, 0);
                        use_pipe_select_write = 1;
                        acc_type = _SOC_TH_ACC_TYPE_FOR_PIPE(pipe);
                    }
                    break;
                case EGR_IP_TUNNEL_IPV6m:
                    mem = reported_mem;
                    vindex = index;
                    break;
                default:
                    break;
                }
            } else
#endif /* BCM_TOMAHAWK_SUPPORT */
            {
                if (mem == L3_ENTRY_IPV6_MULTICASTm) {
                    mem = L3_ENTRY_ONLYm;
                    vindex = index;
                }
            }
        }
        if (SOC_IS_TRIUMPH3(unit) || SOC_IS_HELIX4(unit)) {
            if (mem == L3_ENTRY_4m) {
                mem = L3_ENTRY_1m;
                vindex = index;
            }
        }
        if (SOC_IS_KATANA2(unit) || SOC_IS_TRIDENT(unit)) {
            mem = mem_list[0];
            vindex = index;
        }
        
        if (EGR_IP_TUNNELm == reported_mem) {
            if (SOC_IS_TRIDENT(unit) &&
                !soc_feature(unit, soc_feature_special_egr_ip_tunnel_ser)) {
                mem_list[0] = EGR_IP_TUNNELm;
                mem_list[1] = EGR_IP_TUNNEL_IPV6m;
                iratio[0] = 4;
                iratio[1] = 4;
            } else if (soc_feature(unit, soc_feature_special_egr_ip_tunnel_ser)){
                mem_list[0] = EGR_IP_TUNNELm;
                mem_list[1] = EGR_IP_TUNNEL_IPV6m;
                iratio[0] = 2;
                iratio[1] = 2;
            } else {
                mem_list[0] = EGR_IP_TUNNELm;
                mem_list[1] = EGR_IP_TUNNEL_IPV6m;
                iratio[0] = 1;
                iratio[1] = 2;
            }

            clear_mems = 2;

            if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TOMAHAWKX(unit)) {
                int neighbour_index;
                soc_mem_t check_mem_list[2] =
                              {EGR_IP_TUNNELm, EGR_IP_TUNNEL_MPLSm};

                neighbour_index = index % 2? index -1 : index + 1;

                /* Check if the neighbour is valid or not for
                * EGR_IP_TUNNEL or EGR_IP_TUNNEL_MPLS view.
                * If yes, do not clear EGR_IP_TUNNEL_IPV6 view.
                * If no, clear EGR_IP_TUNNEL_IPV6 view.
                */
                for (i = 0; i < 2; i++) {
                    cache = SOC_MEM_STATE(unit, check_mem_list[i]).cache[copyno];
                    vmap = SOC_MEM_STATE(unit, check_mem_list[i]).vmap[copyno];

                    if (cache != NULL && CACHE_VMAP_TST(vmap, neighbour_index)) {
                        clear_mems = 1;
                        break;
                    }
                }
            }
        }
#endif
        for (i = 0; i < clear_mems; i++) {
            if (EGR_IP_TUNNELm == reported_mem) {
                mem = mem_list[i];
                vindex = index / iratio[i];
            }
            null_entry = soc_mem_entry_null(unit, mem);
            if (use_pipe_select_write) {
                rv = soc_mem_pipe_select_write(unit, SOC_MEM_NO_FLAGS, mem,
                                               copyno, acc_type, vindex,
                                               null_entry);
            } else {
                rv = soc_mem_write(unit, mem, copyno, vindex, null_entry);
            }
            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CLEAR_RESTORE %s.%s[%d] failed: %s\n"),
                           SOC_MEM_NAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno),
                           vindex, soc_errmsg(rv)));
                return rv;
            }
            _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                               si, mem, vindex);
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "CLEAR_RESTORE: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                       SOC_MEM_NAME(unit, mem), mem,
                       SOC_BLOCK_NAME(unit, copyno),
                       vindex, sblk, addr));
            LOG_VERBOSE(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "th_dbg: overlay_mem_correction did "
                            "CLEAR_RESTORE !!\n")));
            if (_SOC_DRV_MEM_CHK_L2_MEM(mem) &&
                soc_feature(unit, soc_feature_mac_learn_limit)) {
                /* coverity[stack_use_overflow] */
                if ((rv =_soc_ser_sync_mac_limits(unit, mem)) != SOC_E_NONE) {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "L2 mac limit sync failed !!\n")));
                    return rv;
                }
            }
        }
        stat->ser_err_clear++;
    }
    return SOC_E_NONE;
}

STATIC int
_soc_ser_sram_correction(int unit, int pipe, int sblk, int addr, soc_mem_t mem,
                         int copyno, int index, uint8 mode,
                         _soc_ser_correct_info_t *si)
{
#define _SOC_SER_SRAM_CHK_RETURN(str) \
        if (rv != SOC_E_NONE) {\
            LOG_ERROR(BSL_LS_SOC_SER, \
                      (BSL_META_U(unit, \
                                  "SER SRAM correction encoutered error(%d) in %s\n"), \
                       rv, str));\
            if (mem_to_lock != INVALIDm) { \
                MEM_UNLOCK(unit, mem_to_lock); \
            } \
            return rv;\
        }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
#define _SOC_TD2P_L2X_THAW_CHK(rv)  \
        if ((mem == L2_ENTRY_ONLY_ECCm) && soc_feature(unit, soc_feature_td2p_a0_sw_war)) { \
            if (rv != SOC_E_NONE) { \
                if (SOC_FAILURE(soc_l2x_thaw(unit))) { \
                    LOG_WARN(BSL_LS_SOC_SER, \
                             (BSL_META_U(unit, \
                                         "L2 thaw failed in L2_ENTRY_ONLY_ECCm correction\n"))); \
                } \
            } \
        } 
#endif

    void *null_entry;
    int is_l3_defip = FALSE;
    uint8 *vmap, at = 0, _l2_lock = 0;
#if defined(ALPM_ENABLE)
    uint8 _alpm_lock = 0;
#endif /* ALPM_ENABLE */
    int rv, i, j, start_index, entry_dw, block;
    soc_stat_t *stat = SOC_STAT(unit);
    _soc_ser_sram_info_t sram_info;
    uint32 entry[SOC_MAX_MEM_WORDS], rval, *cache, raddr = 0;
    soc_mem_t mem_to_restore = mem, mem_to_lock = mem, mem_for_cache_read = mem;
    soc_mem_t mem_to_report = mem;
#if defined(BCM_TOMAHAWK_SUPPORT)
    int fpem_mem_mode = _SOC_SER_MEM_MODE_GLOBAL;
#endif /* BCM_TOMAHAWK_SUPPORT */
    int use_overlay_mem_correction = 0;
    int use_recovery_hw_cache = 0;
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_ser_sram_correction "
                    "routine\n")));

    if (!SOC_MEM_IS_VALID(unit, mem) || index < 0 ||
        !(mode == _SOC_SER_SRAM_CORRECTION_MODE_0 ||
          mode == _SOC_SER_SRAM_CORRECTION_MODE_1)) {
        return SOC_E_PARAM;
    }
    /* Determine RAM count and indexes for all RAMs */
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit)) {
            SOC_IF_ERROR_RETURN
                (soc_tomahawk_mem_sram_info_get(unit, mem, index, &sram_info));
        } else
#endif /* BCM_TOMAHAWK_SUPPORT */
#if defined(BCM_APACHE_SUPPORT)
        if (SOC_IS_APACHE(unit)) {
            SOC_IF_ERROR_RETURN
                (_soc_apache_mem_sram_info_get(unit, mem, index, &sram_info));
        } else 
#endif /* BCM_APACHE_SUPPORT */
#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) { /* Tomahawk check is made earlier */
            SOC_IF_ERROR_RETURN
                (_soc_trident2_mem_sram_info_get(unit, mem, index, &sram_info));
        } else
#endif /* BCM_TRIDENT2_SUPPORT */
    {
        return SOC_E_UNAVAIL;
    }
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TD2P_TT2P(unit)) {
        if ((mem == L2_ENTRY_LPm) || (mem == L2_ENTRY_ISS_LPm)) {
            _L2_LOCK(unit);
            _l2_lock = 1;
        }
    }
#endif /* BCM_TOMAHAWK_SUPPORT */

    /* Update all RAM indexes with data from cache or null entry */
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT)
    if (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) {
        switch (mem) {
        case L2_ENTRY_ONLY_ECCm:
            mem_to_restore = L2_ENTRY_ONLY_ECCm;
            mem_to_lock = L2Xm;
            mem_for_cache_read = L2Xm;
            mem_to_report = L2Xm;
            break;
        case L3_ENTRY_ONLY_ECCm:
        case FPEM_ECCm:
        case FPEM_LPm:
            mem_to_lock = INVALIDm;
            use_overlay_mem_correction = 1;
            break;
        case L2_ENTRY_LPm:
        case L2_ENTRY_ISS_LPm:
        case L3_ENTRY_LPm:
        case L3_ENTRY_ISS_LPm:
        case VLAN_XLATE_LPm:
        case EGR_VLAN_XLATE_LPm:
            mem_to_lock = INVALIDm;
            use_recovery_hw_cache = 1;
            break;
        case L3_DEFIP_ALPM_ECCm:
            mem_to_lock = INVALIDm;
            use_recovery_hw_cache = 1;
            is_l3_defip = TRUE;
            break;
        case VLAN_XLATE_ECCm:
            mem_to_restore = VLAN_XLATE_ECCm;
            mem_to_lock = VLAN_XLATEm;
            mem_for_cache_read = VLAN_XLATEm;
            mem_to_report = VLAN_XLATEm;
            break;
        case EGR_VLAN_XLATE_ECCm:
            mem_to_restore = EGR_VLAN_XLATE_ECCm;
            mem_to_lock = EGR_VLAN_XLATEm;
            mem_for_cache_read = EGR_VLAN_XLATEm;
            mem_to_report = EGR_VLAN_XLATEm;
            break;
        default:
            break;
        }
        sal_memset(entry, 0, 4*soc_mem_entry_words(unit, mem_to_restore));

        if (SOC_IS_APACHE(unit)) {
            /* Poor chap, only guy with single pipe in
               whole of new generation guys */
            use_recovery_hw_cache = 0;
        }
    }
#endif /* BCM_TOMAHAWK_SUPPORT */

#if defined(BCM_TRIDENT2_SUPPORT)
        if (SOC_IS_TD2_TT2(unit)) {
            if (mem == L3_DEFIP_ALPM_IPV4m || mem == L3_DEFIP_ALPM_IPV4_1m ||
                mem == L3_DEFIP_ALPM_IPV6_64m || mem == L3_DEFIP_ALPM_IPV6_64_1m ||
                mem == L3_DEFIP_ALPM_IPV6_128m || mem == L3_DEFIP_PAIR_128m ||
                mem == L3_DEFIPm || mem == L3_DEFIP_AUX_TABLEm) {
                /* Take ALPM_LOCK during correction for ALPM related tables */
                mem_to_lock = INVALIDm;
                is_l3_defip = TRUE;
            }
        }
#endif

    if (mem_to_lock != INVALIDm) {
        MEM_LOCK(unit, mem_to_lock);
    } else if (is_l3_defip) {
        _ALPM_LOCK(unit);
    }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    /* This is a S/W workaround for Trident2+ A0. Disabled learning and aging before Reading/Writing
       L2_ENTRY_ONLY_ECCm*/
    if ((mem == L2_ENTRY_ONLY_ECCm) && soc_feature(unit, soc_feature_td2p_a0_sw_war)) {
        if (SOC_FAILURE(soc_l2x_freeze(unit))) {
            LOG_WARN(BSL_LS_SOC_SER,
                     (BSL_META_U(unit,
                                 "L2 freeze failed in L2_ENTRY_ONLY_ECCm correction\n")));
        }
    }
#endif
    cache = SOC_MEM_STATE(unit, mem_for_cache_read).cache[copyno];
    vmap = SOC_MEM_STATE(unit, mem_for_cache_read).vmap[copyno];
    entry_dw = soc_mem_entry_words(unit, mem_for_cache_read);
    if ((mode == _SOC_SER_SRAM_CORRECTION_MODE_0) &&
        (sram_info.disable_reg != INVALIDr)) {
        rv = soc_reg32_get(unit, sram_info.disable_reg, REG_PORT_ANY, 0, &rval);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _SOC_TD2P_L2X_THAW_CHK(rv);
#endif
        _SOC_SER_SRAM_CHK_RETURN("reg get");
        soc_reg_field_set(unit, sram_info.disable_reg, &rval,
                          sram_info.disable_field, 1);
        raddr = soc_reg_addr_get(unit, sram_info.disable_reg, REG_PORT_ANY, 0,
                                 SOC_REG_ADDR_OPTION_WRITE, &block, &at);
        at = 3;
#if defined(BCM_TOMAHAWK_SUPPORT)
        if (SOC_IS_TOMAHAWKX(unit)) {
            at = _SOC_TH_ACC_TYPE_DUPLICATE;
            if ((mem == FPEM_ECCm) || (mem == FPEM_LPm)) {
                fpem_mem_mode = _soc_ser_mem_mode_get(unit, EXACT_MATCH_2m);
                    /* Note: Assume both EM2, EM4 are always in same mode */
                if (fpem_mem_mode == _SOC_SER_MEM_MODE_PIPE_UNIQUE) {
                   at = _SOC_TH_ACC_TYPE_FOR_PIPE(pipe);
                }
            }
        }
#endif /* BCM_TOMAHAWK_SUPPORT */
        rv = _soc_reg32_set(unit, block, at, raddr, rval);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _SOC_TD2P_L2X_THAW_CHK(rv);
#endif
        _SOC_SER_SRAM_CHK_RETURN("reg set");
    }
    for (i = 0; i < sram_info.ram_count; i++) {
        /* coverity[uninit_use] */
        start_index = sram_info.mem_indexes[i][0];
        if ((mem == L3_ENTRY_IPV4_UNICASTm) || use_overlay_mem_correction) {
            /* To avoid the unnecessary event of correction for Trident2 */
            use_overlay_mem_correction = 1;
            if (mem == FPEM_LPm) {
                if (sram_info.view[i] == FPEM_LPm) {
                    null_entry = soc_mem_entry_null(unit, FPEM_LPm);
                    rv = soc_mem_write(unit, FPEM_LPm, copyno, start_index,
                                       null_entry);
                            /* Note: we are clearing unused index - so ok to
                             * clear in all pipes - no need to restrict access
                             * to per pipe when,
                             * (fpem_mem_mode == _SOC_SER_MEM_MODE_PIPE_UNIQUE)
                             */

                    _SOC_SER_SRAM_CHK_RETURN("mem write");
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CLEAR_RESTORE: %s[%d] start_index: %d\n"),
                               SOC_MEM_NAME(unit, FPEM_LPm), FPEM_LPm,
                               start_index));
                } else {
                    rv = _soc_ser_overlay_mem_correction(unit, 0, pipe, sblk,
                                                         addr,
                                                         sram_info.view[i],
                                                         copyno, start_index,
                                                         si);
                }
            } else {
                rv = _soc_ser_overlay_mem_correction(unit, 0, pipe, sblk, addr,
                                                     mem, copyno, start_index,
                                                     si);
            }
            if (!(rv == SOC_E_NONE || rv == SOC_E_UNAVAIL)) {
                    
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "SER SRAM correction encoutered error(%d) "
                                      "in overlay mem correction\n"),
                           rv));
                if (mem_to_lock != INVALIDm) {
                    MEM_UNLOCK(unit, mem_to_lock);
                }
                return rv;
            }
        } else if (((mem == L3_DEFIP_ALPM_IPV4m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v4 == 1)) ||
                   ((mem == L3_DEFIP_ALPM_IPV4_1m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v4_1 == 1)) ||
                   ((mem == L3_DEFIP_ALPM_IPV6_64m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6 == 1)) ||
                   ((mem == L3_DEFIP_ALPM_IPV6_64_1m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6_1 == 1)) ||
                   ((mem == L3_DEFIP_ALPM_IPV6_128m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6_128 == 1)) ||
                   ((mem == L3_DEFIP_PAIR_128m) && (alpm_no_cache_flag[unit].flag_defip_128 == 1)) ||
                   ((mem == L3_DEFIPm) && (alpm_no_cache_flag[unit].flag_defip == 1)) ||
                   ((mem == L3_DEFIP_AUX_TABLEm) && (alpm_no_cache_flag[unit].flag_defip_aux == 1)) ||
                   use_recovery_hw_cache) {
            /* To avoid the unnecessary event of correction for Trident2 */
            use_recovery_hw_cache = 1;
            for (j = 0; j < sram_info.index_count[i]; j++) {
                rv = _soc_ser_recovery_hw_cache(unit, pipe, sram_info.view[i],
                                                copyno, sram_info.mem_indexes[i][j],
                                                si, FALSE);
                if (!(rv == SOC_E_NONE || rv == SOC_E_UNAVAIL)) {
                    
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "SER SRAM correction encoutered error(%d) "
                                          "in recovery from hw cache\n"),
                               rv));
                    if (mem_to_lock != INVALIDm) {
                        MEM_UNLOCK(unit, mem_to_lock);
                    }
                    _ALPM_UNLOCK(unit)
                    _L2_UNLOCK(unit, _l2_lock)

                    return rv;
                }
            }
        } else if ((sram_info.view[i] == L3_DEFIP_ALPM_IPV4m ||
                    sram_info.view[i] == L3_DEFIP_ALPM_IPV4_1m ||
                    sram_info.view[i] == L3_DEFIP_ALPM_IPV6_64m ||
                    sram_info.view[i] == L3_DEFIP_ALPM_IPV6_64_1m ||
                    sram_info.view[i] == L3_DEFIP_ALPM_IPV6_128m ||
                    sram_info.view[i] == L3_DEFIP_ALPM_RAWm)) {
            cache = SOC_MEM_STATE(unit, sram_info.view[i]).cache[copyno];
            vmap = SOC_MEM_STATE(unit, sram_info.view[i]).vmap[copyno];
            entry_dw = soc_mem_entry_words(unit, sram_info.view[i]);
            for (j = 0; j < sram_info.index_count[i]; j++) {
                if (cache != NULL && CACHE_VMAP_TST(vmap, sram_info.mem_indexes[i][j])) {
                    sal_memcpy(entry, cache + sram_info.mem_indexes[i][j] * entry_dw, entry_dw * 4);
                    rv = soc_mem_write(unit, sram_info.view[i], copyno, sram_info.mem_indexes[i][j], entry);
                    _SOC_SER_SRAM_CHK_RETURN("mem write");
                    LOG_WARN(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CACHE_RESTORE: %s[%d] start_index: %d\n"),
                               SOC_MEM_NAME(unit, sram_info.view[i]),
                               sram_info.view[i], sram_info.mem_indexes[i][j]));
                    stat->ser_err_restor++;
                    if (NULL != si) {
                        if ((si->log_id != 0) &&
                            (si->flags & SOC_SER_LOG_WRITE_CACHE)) {
                            if (si->index == sram_info.mem_indexes[i][j]) {
                                (void)soc_ser_log_add_tlv(unit, si->log_id,
                                                          SOC_SER_LOG_TLV_CACHE,
                                                          entry_dw*4, entry);
                            } else {
                                LOG_VERBOSE(BSL_LS_SOC_SER,
                                    (BSL_META_U(unit,
                                                "skipped updating tlv_cache index: %d\n"),
                                     sram_info.mem_indexes[i][j]));
                            }
                        }
                    }
                } else { /* Fall back to clear */
                    if (NUM_PIPE(unit) == 1) {
                        null_entry = soc_mem_entry_null(unit, sram_info.view[i]);
                        rv = soc_mem_write(unit, sram_info.view[i], copyno, sram_info.mem_indexes[i][j], null_entry);
                    } else {
                        rv = _soc_ser_recovery_hw_cache(unit, pipe, sram_info.view[i],
                                                        copyno, sram_info.mem_indexes[i][j],
                                                        si, FALSE);
                    }
                    _SOC_SER_SRAM_CHK_RETURN("mem write");
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CLEAR_RESTORE: %s[%d] start_index: %d\n"),
                               SOC_MEM_NAME(unit, sram_info.view[i]),
                               sram_info.view[i], sram_info.mem_indexes[i][j]));
                    stat->ser_err_clear++;
                }
            }
        } else {
            if (cache != NULL && CACHE_VMAP_TST(vmap, start_index)) {
                sal_memcpy(entry, cache + start_index * entry_dw, entry_dw * 4);
                rv = soc_mem_write(unit, mem_to_restore, copyno, start_index, entry);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                _SOC_TD2P_L2X_THAW_CHK(rv);
#endif
                _SOC_SER_SRAM_CHK_RETURN("mem write");
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CACHE_RESTORE: %s[%d] start_index: %d\n"),
                           SOC_MEM_NAME(unit, mem_to_restore),
                           mem_to_restore, start_index));
                stat->ser_err_restor++;
                if (NULL != si) {
                    if ((si->log_id != 0) &&
                        (si->flags & SOC_SER_LOG_WRITE_CACHE)) {
                        if (si->index == start_index) {
                            (void)soc_ser_log_add_tlv(unit, si->log_id,
                                                      SOC_SER_LOG_TLV_CACHE,
                                                      entry_dw*4, entry);
                        } else {
                            LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "skipped updating tlv_cache index: %d\n"),
                                 start_index));
                        }
                    }
                }
            } else { /* Fall back to clear */
                null_entry = soc_mem_entry_null(unit, mem_to_restore);
                rv = soc_mem_write(unit, mem_to_restore, copyno, start_index, null_entry);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
                _SOC_TD2P_L2X_THAW_CHK(rv);
#endif
                _SOC_SER_SRAM_CHK_RETURN("mem write");
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CLEAR_RESTORE: %s[%d] start_index: %d\n"),
                           SOC_MEM_NAME(unit, mem_to_restore),
                           mem_to_restore, start_index));
                stat->ser_err_clear++;
            }
        }
    }
    if ((mode == _SOC_SER_SRAM_CORRECTION_MODE_0) &&
        (sram_info.disable_reg != INVALIDr)) {
        soc_reg_field_set(unit, sram_info.disable_reg, &rval,
                          sram_info.disable_field, 0);
        rv = _soc_reg32_set(unit, block, at, raddr, rval);
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
        _SOC_TD2P_L2X_THAW_CHK(rv);
#endif
        _SOC_SER_SRAM_CHK_RETURN("reg set");
    }
#if defined(BCM_TRIDENT2PLUS_SUPPORT)
    /*TD2+ SW WAR. Enabled learning and aging */
    if ((mem == L2_ENTRY_ONLY_ECCm) && soc_feature(unit, soc_feature_td2p_a0_sw_war)) {
        if (SOC_FAILURE(soc_l2x_thaw(unit))) {
            LOG_WARN(BSL_LS_SOC_SER,
                     (BSL_META_U(unit,
                                 "L2 thaw failed in L2_ENTRY_ONLY_ECCm correction\n")));
        }
    }
#endif
    if (mem_to_lock != INVALIDm) {
        MEM_UNLOCK(unit, mem_to_lock);
    }
    _ALPM_UNLOCK(unit)
    _L2_UNLOCK(unit, _l2_lock)

    /* resume alpm operations that could have been terminated because of ser
     * problem
     */
    switch (mem) {
    case L3_DEFIP_ALPM_IPV4m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
    case L3_DEFIP_ALPM_ECCm:
        if (SOC_CONTROL(unit)->alpm_lookup_retry &&
            SHR_BITGET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_LOOKUP)) {
            sal_sem_give(SOC_CONTROL(unit)->alpm_lookup_retry);
        }
        if (SOC_CONTROL(unit)->alpm_insert_retry &&
            SHR_BITGET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_INSERT)) {
            sal_sem_give(SOC_CONTROL(unit)->alpm_insert_retry);
        }
        if (SOC_CONTROL(unit)->alpm_delete_retry &&
            SHR_BITGET(&(SOC_CONTROL(unit)->alpm_mem_ops), _SOC_ALPM_DELETE)) {
            sal_sem_give(SOC_CONTROL(unit)->alpm_delete_retry);
        }
        break;
    default: break;
    }
    if (!(use_overlay_mem_correction || use_recovery_hw_cache)) {
        _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                           si, mem_to_report, index);
        stat->ser_err_corr++;
    }
    return SOC_E_NONE;
}

int
soc_ser_sram_correction(int unit, int pipe, int sblk, int addr, soc_mem_t mem,
                        int copyno, int index, _soc_ser_correct_info_t *si)
{
    int rv;
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered soc_ser_sram_correction "
                    "routine\n")));

    
    switch (mem) {
    case L3_ENTRY_ONLYm:
    case L3_ENTRY_IPV4_UNICASTm:
    case L3_ENTRY_IPV4_MULTICASTm:
    case L3_ENTRY_IPV6_UNICASTm:
    case L3_ENTRY_IPV6_MULTICASTm:
        rv = _soc_ser_sram_correction(unit, pipe, sblk, addr,
                                      L3_ENTRY_IPV4_UNICASTm, copyno, index,
                                      _SOC_SER_SRAM_CORRECTION_MODE_0, si);
        if (rv == SOC_E_NONE || rv != SOC_E_UNAVAIL) {
            return rv;
        }
        break;
    case L2Xm:
    case L3_DEFIP_ALPM_IPV4m:
    case L3_DEFIP_ALPM_IPV4_1m:
    case L3_DEFIP_ALPM_IPV6_64m:
    case L3_DEFIP_ALPM_IPV6_64_1m:
    case L3_DEFIP_ALPM_IPV6_128m:
    case L3_DEFIP_ALPM_RAWm:
    case VLAN_XLATEm:
    case EGR_VLAN_XLATEm:
        return _soc_ser_sram_correction(unit, pipe, sblk, addr, mem, copyno,
                                        index, _SOC_SER_SRAM_CORRECTION_MODE_0,
                                        si);
#if defined(BCM_TOMAHAWK_SUPPORT) || defined(BCM_TRIDENT2PLUS_SUPPORT) || \
    defined(BCM_APACHE_SUPPORT)
    case L2_ENTRY_ONLY_ECCm:
    case L2_ENTRY_LPm:
    case L2_ENTRY_ISS_LPm:
    case L3_ENTRY_ONLY_ECCm:
    case L3_ENTRY_LPm:
    case L3_ENTRY_ISS_LPm:
    case FPEM_ECCm:
    case FPEM_LPm:
    case L3_DEFIP_ALPM_ECCm:
    case VLAN_XLATE_ECCm:
    case VLAN_XLATE_LPm:
    case EGR_VLAN_XLATE_ECCm:
    case EGR_VLAN_XLATE_LPm:
        if (SOC_IS_TOMAHAWKX(unit) || SOC_IS_TD2P_TT2P(unit) || SOC_IS_APACHE(unit)) {
            return _soc_ser_sram_correction(unit, pipe, sblk, addr, mem, copyno,
                                            index, _SOC_SER_SRAM_CORRECTION_MODE_0, si);
        }
        else {
            break;
        }
#endif /* BCM_TOMAHAWK_SUPPORT */
    case ING_L3_NEXT_HOPm:
        return _soc_ser_sram_correction(unit, pipe, sblk, addr, mem, copyno,
                                        index, _SOC_SER_SRAM_CORRECTION_MODE_1,
                                        si);
    case L3_IPMCm:
    case L2MCm:
        if (!SOC_IS_TOMAHAWKX(unit)) {
            return _soc_ser_sram_correction(unit, pipe, sblk, addr, mem, copyno,
                                            index,
                                            _SOC_SER_SRAM_CORRECTION_MODE_1,
                                            si);
        }
        break;
    default: break;
    }
    return SOC_E_UNAVAIL;
}

int
soc_mem_alpm_aux_table_correction(int unit, int pipe, int index, soc_mem_t mem)
{
    int rv = SOC_E_UNAVAIL;
#if defined(BCM_TRIDENT2_SUPPORT)
    int bidx, sblk = 0, blk;
    _soc_ser_correct_info_t spci;
#endif

#if defined(BCM_TRIDENT2_SUPPORT)
    blk = SOC_BLK_IPIPE;
    SOC_BLOCK_ITER(unit, bidx, blk) {
        sblk = SOC_BLOCK2SCH(unit, bidx);
        break;
    }

    if (SOC_IS_TD2_TT2(unit)) {
        sal_memset(&spci, 0, sizeof(spci));
        spci.flags = SOC_SER_SRC_MEM | SOC_SER_REG_MEM_KNOWN;
        spci.reg = INVALIDr;
        spci.mem = mem;
        spci.pipe_num = pipe;
        spci.blk_type = blk;
        spci.sblk = sblk;
        spci.index = index;
        spci.parity_type = SOC_PARITY_TYPE_PARITY;

        LOG_ERROR(BSL_LS_SOC_SER,
              (BSL_META_U(unit,
                          "Inline correction: mem:%s pipe:%d index:%d\n"),
                          SOC_MEM_NAME(unit, mem), pipe, index));

        return soc_ser_correction(unit, &spci);
    } else
#endif
    {
        return rv;
    }
}

int
_soc_ser_lp_mem_correction(int unit, soc_mem_t mem, int index)
{
#ifdef BCM_XGS_SWITCH_SUPPORT
    int rv, rv1;
    soc_mem_t hash_mem;
    uint32 entry[SOC_MAX_MEM_WORDS];
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered soc_ser_lp_mem_correction "
                    "routine\n")));

    switch (mem) {
    case L2_ENTRY_LPm: hash_mem = L2Xm; break;
    case L3_ENTRY_LPm: hash_mem = L3_ENTRY_IPV4_UNICASTm; break;
    
    case VLAN_XLATE_LPm: hash_mem = VLAN_XLATEm; break;
    case EGR_VLAN_XLATE_LPm: hash_mem = EGR_VLAN_XLATEm; break;
    default: return SOC_E_PARAM;
    }

    if (hash_mem == L2Xm) {
        rv1 = soc_l2x_freeze(unit);
        if (SOC_FAILURE(rv1)) {
            LOG_WARN(BSL_LS_SOC_SER,
                     (BSL_META_U(unit,
                                 "L2 freeze failed in LP mem correction\n")));
        }
    } else {
        MEM_LOCK(unit, hash_mem);
    }
    rv = soc_mem_read(unit, hash_mem, MEM_BLOCK_ALL, index*4, entry);
    
    if (SOC_FAILURE(rv)) {
        if (hash_mem == L2Xm) {
            rv1 = soc_l2x_thaw(unit);
            if (SOC_FAILURE(rv1)) {
                LOG_WARN(BSL_LS_SOC_SER,
                         (BSL_META_U(unit,
                                     "L2 thaw failed in LP mem correction\n")));
            }
        } else {
            MEM_UNLOCK(unit, hash_mem);
        }
        return rv;
    }
    rv = soc_mem_write(unit, hash_mem, MEM_BLOCK_ALL, index*4, entry);
    if (SOC_FAILURE(rv)) {
        if (hash_mem == L2Xm) {
            rv1 = soc_l2x_thaw(unit);
            if (SOC_FAILURE(rv1)) {
                LOG_WARN(BSL_LS_SOC_SER,
                         (BSL_META_U(unit,
                                     "L2 thaw failed in LP mem correction\n")));
            }
        } else {
            MEM_UNLOCK(unit, hash_mem);
        }
        return rv;
    }
    if (hash_mem == L2Xm) {
        rv1 = soc_l2x_thaw(unit);
        if (SOC_FAILURE(rv1)) {
            LOG_WARN(BSL_LS_SOC_SER,
                     (BSL_META_U(unit,
                                 "L2 thaw failed in LP mem correction\n")));
        }
    } else {
        MEM_UNLOCK(unit, hash_mem);
    }
#endif /* BCM_XGS_SWITCH_SUPPORT */
    return SOC_E_NONE;
}

STATIC void
_soc_ser_ism_correction(int unit)
{
#define _SOC_SER_ISM_DMA_CHUNK_SIZE 1024
    uint32 *tbl_chnk;
    int rv, m, buf_size, chunksize;
    int chnk_idx, chnk_idx_max, mem_idx_max;
    static soc_mem_t ism_mems[] = {
        VLAN_XLATEm,
        L2_ENTRY_1m,
        L3_ENTRY_1m,
        MPLS_ENTRYm,
        EGR_VLAN_XLATEm
    };
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered soc_ser_ism_mem_correction "
                    "routine\n")));

    chunksize = _SOC_SER_ISM_DMA_CHUNK_SIZE;
    buf_size = 4 * SOC_MAX_MEM_WORDS * chunksize;
    tbl_chnk = soc_cm_salloc(unit, buf_size, "ism ser correction");
    if (NULL == tbl_chnk) {
        LOG_ERROR(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "Memory allocation failure in ser ism "
                              "correction !!\n")));
        return;
    }

    for (m=0; m<COUNTOF(ism_mems); m++) {
        if (!soc_mem_index_count(unit, ism_mems[m])) {
            continue;
        }
        mem_idx_max = soc_mem_index_max(unit, ism_mems[m]);
        MEM_LOCK(unit, ism_mems[m]);
        for (chnk_idx = soc_mem_index_min(unit, ism_mems[m]);
             chnk_idx <= mem_idx_max; chnk_idx += chunksize) {
            sal_memset((void *)tbl_chnk, 0, buf_size);
            chnk_idx_max = ((chnk_idx + chunksize) <= mem_idx_max) ?
                (chnk_idx + chunksize - 1) : mem_idx_max;
            rv = soc_mem_read_range(unit, ism_mems[m], MEM_BLOCK_ANY, chnk_idx,
                                    chnk_idx_max, tbl_chnk);
            if (SOC_FAILURE(rv)) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "DMA failure in ser ism "
                                      "correction for %s mem !!\n"),
                           SOC_MEM_NAME(unit, ism_mems[m])));
                MEM_UNLOCK(unit, ism_mems[m]);
                soc_cm_sfree(unit, tbl_chnk);
                return;
            }
        }
        MEM_UNLOCK(unit, ism_mems[m]);
    }
    soc_cm_sfree(unit, tbl_chnk);
}

STATIC int check_parity(uint32 *p32, int n32)
{
    uint32 parity = 0;
    uint32 tmpI   = 0;

    for (n32--; n32>=0; n32--) {

        tmpI=p32[n32];
        tmpI=tmpI^(tmpI>>1);
        tmpI=tmpI^(tmpI>>2);
        tmpI=tmpI^(tmpI>>4);
        tmpI=tmpI^(tmpI>>8);
        tmpI=tmpI^(tmpI>>16);

        parity = parity ^ (tmpI&1);
    }

    return parity&1;
}
#ifdef _SER_TIME_STAMP
extern sal_usecs_t ser_time_1;
sal_usecs_t ser_time_5;
#endif

int
_soc_oam_ser_correction(int unit, soc_mem_t mem, int index)
{
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_oam_ser_correction "
                    "routine\n")));
#ifdef BCM_TRIUMPH3_SUPPORT
    if (SOC_IS_TRIUMPH3(unit)) {
        return soc_tr3_oam_ser_process(unit, mem, index);
    }
#endif
#ifdef BCM_HURRICANE2_SUPPORT
    if (SOC_IS_HURRICANE2(unit)) {
        return soc_hurricane2_oam_ser_process(unit, mem, index);
    }
#endif
#ifdef BCM_GREYHOUND_SUPPORT
        if (SOC_IS_GREYHOUND(unit)) {
            return soc_greyhound_oam_ser_process(unit, mem, index);
        }
#endif
#ifdef BCM_HURRICANE3_SUPPORT
        if (SOC_IS_HURRICANE3(unit)) {
            return soc_hr3_oam_ser_process(unit, mem, index);
        }
#endif
#ifdef BCM_KATANA2_SUPPORT
    if (SOC_IS_KATANA2(unit)) {
        return soc_kt2_oam_ser_process(unit, mem, index);
    }
#endif /* BCM_KATANA2_SUPPORT */

#ifdef BCM_TRIUMPH2_SUPPORT
    if (SOC_IS_TRIUMPH2(unit) || SOC_IS_APOLLO(unit)) {
        return soc_triumph2_oam_ser_process(unit, mem, index);
    }
#endif

    LOG_ERROR(BSL_LS_SOC_SER,
              (BSL_META_U(unit,
                          "SER Handling routine not avaialable\n")));
    return SOC_E_UNAVAIL;
}


STATIC int
_soc_ser_ipfix_correction(int unit, soc_mem_t mem)
{
    int port;
    int ingress_stage = 0;
    int rv;

    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "th_dbg: entered _soc_ser_ipfix_correction "
                    "routine\n")));
    /*
     * Also need to clear the
     * 1. ingress/egress fifo counter
     * 2. ingress/egress port record count
     * 3. ingress/egress port sampling count
     */
    ingress_stage = (mem == ING_IPFIX_SESSION_TABLEm) ? 1 : 0;

    if (ingress_stage) {
        rv = WRITE_ING_IPFIX_EXPORT_FIFO_COUNTERr(unit, 0);
        if (rv < 0) {
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "ING_IPFIX_EXPORT_FIFO_COUNTERr clear failed: %s\n"),
                       soc_errmsg(rv)));
            return rv;
        }
    } else {
        rv = WRITE_EGR_IPFIX_EXPORT_FIFO_COUNTERr(unit, 0);
        if (rv < 0) {
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "EGR_IPFIX_EXPORT_FIFO_COUNTERr clear failed: %s\n"),
                       soc_errmsg(rv)));
            return rv;
        }
    }

    PBMP_PORT_ITER(unit, port) {
        if (ingress_stage) {

            rv = WRITE_ING_IPFIX_PORT_RECORD_COUNTr(unit, port, 0);
            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "ING_IPFIX_PORT_RECORD_COUNTr clear for "
                                      "port(%d) failed: %s\n"),
                           port, soc_errmsg(rv)));
                return rv;
            }

            rv = WRITE_ING_IPFIX_PORT_SAMPLING_COUNTERr(unit, port, 0);
            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "ING_IPFIX_PORT_SAMPLING_COUNTERr clear for "
                                      "port(%d) failed: %s\n"),
                           port, soc_errmsg(rv)));
                return rv;
            }
        } else {

            rv = WRITE_EGR_IPFIX_PORT_RECORD_COUNTr(unit, port, 0);
            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "EGR_IPFIX_PORT_RECORD_COUNTr clear for "
                                      "port(%d) failed: %s\n"),
                           port, soc_errmsg(rv)));
                return rv;
            }

            rv = WRITE_EGR_IPFIX_PORT_SAMPLING_COUNTERr(unit, port, 0);
            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "EGR_IPFIX_PORT_SAMPLING_COUNTERr clear for "
                                      "port(%d) failed: %s\n"),
                           port, soc_errmsg(rv)));
                return rv;
            }
        }
    }
    return SOC_E_NONE;
}
/* Top level SER correction routine */
int
soc_ser_correction(int unit, _soc_ser_correct_info_t *si)
{
    soc_mem_t mem = si->mem;
    int rv, index = si->index, entry_dw, copyno = 0;
    soc_reg_t reg = si->reg;
    soc_port_t port = si->port;
    void *null_entry = NULL;
    uint32 entry[SOC_MAX_MEM_WORDS], *cache = NULL;
    uint8 *vmap;
    soc_stat_t *stat = SOC_STAT(unit);
    int is_l3_defip = FALSE;
    soc_ser_log_tlv_generic_t log_generic;
#if defined(ALPM_ENABLE)
    uint8 _alpm_lock = 0;
#endif /* ALPM_ENABLE */
#ifdef BCM_TRIUMPH3_SUPPORT
    uint32 rval;
#endif

    if (!SOC_SER_CORRECTION_SUPPORT(unit)) {
        return SOC_E_NONE;
    }
    if (SOC_HW_ACCESS_DISABLE(unit)) {
        LOG_WARN(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "SER_CORRECTION: reg/mem:%d hardware access disable\n"),
                   (si->flags & SOC_SER_SRC_MEM) ? si->mem : si->reg));
        return SOC_E_NONE;
    }

    if (si->log_id != 0) {
        log_generic.flags = 0;
        if (si->flags & SOC_SER_ERR_CPU) {
            log_generic.flags |= SOC_SER_LOG_FLAG_ERR_SRC;
        }
        if (si->flags & SOC_SER_ERR_MULTI) {
            log_generic.flags |= SOC_SER_LOG_FLAG_MULTIBIT;
        }
        if (si->double_bit != 0) {
            log_generic.flags |= SOC_SER_LOG_FLAG_DOUBLEBIT;
        }
        log_generic.time = si->detect_time;
        log_generic.boot_count = soc_ser_log_get_boot_count(unit);
        log_generic.address = si->addr;
        log_generic.acc_type = si->acc_type;
        log_generic.block_type = si->blk_type;
        log_generic.parity_type = si->parity_type;
        log_generic.ser_response_flag = 0;
        log_generic.corrected = SOC_SER_UNCORRECTED;
        log_generic.pipe_num = si->pipe_num;
    }

    LOG_ERROR(BSL_LS_SOC_SER,
              (BSL_META_U(unit,
                          "SER_CORRECTION: reg/mem:%d btype:%d sblk:%d at:%d "
                          "stage:%d addr:0x%08x port: %d index: %d\n"),
               (si->flags & SOC_SER_SRC_MEM) ? si->mem : si->reg, si->blk_type,
               si->sblk, si->acc_type, si->stage, si->addr, si->port, si->index));

    if (si->flags & SOC_SER_SRC_MEM) { /* Handle mems */
        soc_ser_log_tlv_memory_t log_mem;
        log_mem.index = index;

        if (!(si->flags & SOC_SER_REG_MEM_KNOWN)) {
            /* Decode memory from address details */
            mem = soc_addr_to_mem_extended(unit, si->sblk, si->acc_type, si->addr);
            if (mem == INVALIDm) {
                if (si->log_id != 0) {
                    log_mem.memory = mem;
                    log_generic.corrected = SOC_SER_UNCORRECTED;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_MEMORY,
                        sizeof(soc_ser_log_tlv_memory_t), &log_mem);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                if (soc_feature(unit, soc_feature_two_ingress_pipes) &&
                    soc_feature(unit, soc_feature_sbusdma)) {
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "memory not decoded [%d %d 0x%8x].\n"),
                                 si->sblk, si->acc_type, si->addr));
                    return SOC_E_NOT_FOUND;
                } else {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "memory not decoded [%d %d 0x%8x].\n"),
                               si->sblk, si->acc_type, si->addr));
                    stat->ser_err_sw++;
                    return SOC_E_INTERNAL;
                }
            }
        }
        /* In TR3 and HX4, there are several memory names VLAN_XLATEm, 
         * VLAN_XLATE_1m, VLAN_XLATE_EXTDm for the physical VLAN_XLATE 
         * table, refering to single or double base entry views. 
         * When parity errors are detected in the physical table, 
         * _soc_ser_overlay_mem_correction should be invoked with the parameter 
         * "reported_mem" as VLAN_XLATE_1m to correct errors in appropiate table
         * views. The routine soc_addr_to_mem_extended always returns VLAN_XLATEm, 
         * not VLAN_XLATE_1m. So overriding memory name is added here.
         * The long term way to handle these things which are per device is 
         * to register a per device memory check and over-ride function and 
         * call it from soc_ser_correction() */
        if (soc_feature(unit, soc_feature_ism_memory) && mem == VLAN_XLATEm) {
            mem = VLAN_XLATE_1m;
        }
        if (!SOC_MEM_IS_VALID(unit, mem)) {
            return SOC_E_INTERNAL;
        }
        if (si->log_id != 0) {
            if (si->flags & SOC_SER_LOG_MEM_REPORTED) {
                log_mem.memory = si->mem_reported;
            } else {
                log_mem.memory = mem;
            }
            log_generic.ser_response_flag = SOC_MEM_SER_CORRECTION_TYPE(unit, mem);
            soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_MEMORY,
                sizeof(soc_ser_log_tlv_memory_t), &log_mem);
        }

        if (si->sblk) {
            SOC_MEM_BLOCK_ITER(unit, mem, copyno) {
                if (SOC_BLOCK2OFFSET(unit, copyno) == si->sblk) {
                    break;
                }
            }
        } else {
            copyno = SOC_MEM_BLOCK_ANY(unit, mem);
        }
        stat->ser_err_mem++;
        LOG_WARN(BSL_LS_SOC_SER,
                  (BSL_META_U(unit,
                              "mem: %d=%s blkoffset:%d\n"), mem,
                   SOC_MEM_NAME(unit, mem), copyno));
        entry_dw = soc_mem_entry_words(unit, mem);
        if (soc_feature(unit, soc_feature_shared_hash_mem)) {
            rv = soc_ser_sram_correction(unit, si->pipe_num, si->sblk, si->addr,
                                         mem, copyno, index, si);
            if (rv == SOC_E_NONE || rv != SOC_E_UNAVAIL) {
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    log_generic.ser_response_flag = SOC_MEM_FLAG_SER_CACHE_RESTORE;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            }
        }
        if (soc_feature(unit, soc_feature_fp_meter_ser_verify) &&
            (FP_METER_TABLEm == mem)) {
            int i=0;

            /* Get data even on parity error */
            if (SOC_E_NONE == soc_mem_read_extended(unit, SOC_MEM_SCHAN_ERR_RETURN,
                                                    mem, 0, copyno, index, entry)) {
                if (!check_parity(entry, entry_dw)) {
                    if (si->log_id != 0) {
                        log_generic.corrected = SOC_SER_UNKNOWN;
                        soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                            sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    }
                    /* No error case. Print and return */
                    LOG_WARN(BSL_LS_SOC_SER,
                             (BSL_META_U(unit,
                                         "NO PARITY ERROR: reg/mem:%d btype:%d sblk:%d at:%d "
                                         "stage:%d addr:0x%08x index: %d\n"),
                              (si->flags & SOC_SER_SRC_MEM) ? si->mem : si->reg,
                              si->blk_type, si->sblk, si->acc_type,
                              si->stage, si->addr, si->index));

                    for (i=0;i<entry_dw;i++) {
                        LOG_WARN(BSL_LS_SOC_SER,
                                 (BSL_META_U(unit,
                                             "NO PARITY ERROR: Entry[%d]:%08X\n"), i, entry[i]));
                    }
                    return SOC_E_NONE;
                }
            }
        } else if (soc_feature(unit, soc_feature_ism_memory) &&
                   (mem == RAW_ENTRY_TABLEm)) {
            /* Packet lookup encountered an error - dump mems from cpu and do
               normal correction */
            _soc_ser_ism_correction(unit);
            if (si->log_id != 0) {
                log_generic.corrected = SOC_SER_UNKNOWN;
                soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                    sizeof(soc_ser_log_tlv_generic_t), &log_generic);
            }
            return SOC_E_NONE;
        }

        if (SOC_IS_TD2_TT2(unit)) {
            if (mem == L3_DEFIP_ALPM_IPV4m || mem == L3_DEFIP_ALPM_IPV4_1m ||
                mem == L3_DEFIP_ALPM_IPV6_64m || mem == L3_DEFIP_ALPM_IPV6_64_1m ||
                mem == L3_DEFIP_ALPM_IPV6_128m || mem == L3_DEFIP_AUX_TABLEm ||
                mem == L3_DEFIPm || mem == L3_DEFIP_PAIR_128m) {
                /* Take advantage of SER write cache flow */
                is_l3_defip = TRUE;
            }
        }

        switch (SOC_MEM_SER_CORRECTION_TYPE(unit, mem)) {
        case SOC_MEM_FLAG_SER_ENTRY_CLEAR:
            LOG_VERBOSE(BSL_LS_SOC_SER,
                (BSL_META_U(unit,
                            "th_dbg: SER_CORRECTION_TYPE = "
                            "ENTRY_CLEAR for mem = %0d\n"), mem));
#ifdef BCM_TRIUMPH3_SUPPORT
            if (SOC_IS_TRIUMPH3(unit)) {
                SOC_IF_ERROR_RETURN(READ_MISCCONFIGr(unit, &rval));
                soc_reg_field_set(unit, MISCCONFIGr, &rval, PARITY_STAT_CLEARf, 1);
                SOC_IF_ERROR_RETURN(WRITE_MISCCONFIGr(unit, rval));
            }
#endif

            if (si->flags & SOC_SER_ALSO_UPDATE_SW_COUNTER) {
                rv = SOC_E_FAIL;
#ifdef BCM_TOMAHAWK_SUPPORT
                if (SOC_IS_TOMAHAWKX(unit)) {
                    rv = soc_th_ser_update_counter(
                            unit,
                            0,               /* is_reg */
                            INVALIDr,        /* restore_reg */
                            si->mem,         /* restore_mem */
                            si->index,
                            copyno,
                            INVALIDr,        /* base_reg */
                            si->counter_base_mem, /* base_mem */
                            si->counter_inst, /* inst_num */
                            si->pipe_num,
                            SOC_SER_COUNTER_CORRECTION(unit)); /* restore_last */
                }
#endif
            } else {
                null_entry = soc_mem_entry_null(unit, mem);
                rv = soc_mem_write(unit, mem, copyno, index, null_entry);
            }

            if (rv < 0) {
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "ENTRY_CLEAR %s.%s[%d] failed: %s\n"),
                           SOC_MEM_NAME(unit, mem),
                           SOC_BLOCK_NAME(unit, copyno), index, soc_errmsg(rv)));
                if (si->log_id != 0) {
                    log_generic.corrected = SOC_SER_UNCORRECTED;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            }
#if defined(BCM_TRIDENT_SUPPORT)
            if (SOC_IS_TRIDENT2X(unit) || SOC_IS_TRIDENT(unit)) {
                soc_ser_counter_info_set(unit, mem, index, null_entry);
            }
#endif
#ifdef BCM_TRIUMPH3_SUPPORT
            if (SOC_IS_TRIUMPH3(unit)) {
                soc_reg_field_set(unit, MISCCONFIGr, &rval, PARITY_STAT_CLEARf, 0);
                SOC_IF_ERROR_RETURN(WRITE_MISCCONFIGr(unit, rval));

            }
#endif
            _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                               si, mem, index);
            LOG_ERROR(BSL_LS_SOC_SER,
                      (BSL_META_U(unit,
                                  "ENTRY_CLEAR: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                       SOC_MEM_NAME(unit, mem), mem,
                       SOC_BLOCK_NAME(unit, copyno), index, si->sblk, si->addr));

            /* Handle IPFIX ser */
            if (mem == ING_IPFIX_SESSION_TABLEm ||
                mem == EGR_IPFIX_SESSION_TABLEm) {
                if ((rv = _soc_ser_ipfix_correction(unit, mem)) < 0) {
                    if (si->log_id != 0) {
                        log_generic.corrected = SOC_SER_UNCORRECTED;
                        soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                            sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    }
                    return rv;
                }
            }

            stat->ser_err_clear++;
            if (si->log_id != 0) {
                log_generic.corrected = SOC_SER_CORRECTED;
                soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                    sizeof(soc_ser_log_tlv_generic_t), &log_generic);
            }
            return SOC_E_NONE;
        case SOC_MEM_FLAG_SER_ECC_CORRECTABLE:
            if (SOC_MEM_SER_CORRECTION_TYPE(unit, mem) ==
                SOC_MEM_FLAG_SER_ECC_CORRECTABLE) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: SER_CORRECTION_TYPE = "
                                "ECC_CORRECTABLE for mem = %0d\n"), mem));
            }
            stat->ser_err_ecc++;
            /* passthru */
            /* coverity[fallthrough: FALSE] */
        case SOC_MEM_FLAG_SER_CACHE_RESTORE:
            if (SOC_MEM_SER_CORRECTION_TYPE(unit, mem) ==
                SOC_MEM_FLAG_SER_CACHE_RESTORE) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: SER_CORRECTION_TYPE = "
                                "CACHE_RESTORE for mem = %0d\n"), mem));
            }
            /* passthru */
            /* coverity[fallthrough: FALSE] */
        case SOC_MEM_FLAG_SER_WRITE_CACHE_RESTORE:
            if (SOC_MEM_SER_CORRECTION_TYPE(unit, mem) ==
                SOC_MEM_FLAG_SER_WRITE_CACHE_RESTORE) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: SER_CORRECTION_TYPE = "
                                "WRITE_CACHE_RESTORE for mem = %0d\n"), mem));
            }

            /* In TD2, the HW does SBUS MEMWR error checking
             * on the L3_ENTRY table in general. Include both
             * XOR and non-XOR mems.
             */ 
#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TRIDENT2(unit) || SOC_IS_TITAN2(unit)) {
                soc_trident2_l3_memwr_parity_check(unit, mem, TRUE);
            }
#endif /* BCM_TRIDENT2_SUPPORT */
            rv = _soc_ser_overlay_mem_correction(unit, si->flags & SOC_SER_ERR_CPU ? 1 : 0,
                                                 si->pipe_num, si->sblk, si->addr, mem,
                                                 copyno, index, si);

#if defined(BCM_TRIDENT_SUPPORT)
            if (SOC_IS_TRIDENT(unit) || SOC_IS_TITAN(unit)) {
                if (rv == SOC_E_UNAVAIL)  {
                    rv = soc_trident_ser_dual_pipe_correction(unit, mem, copyno, index, si);
                }
            }
#endif

#if defined(BCM_TRIDENT2_SUPPORT)
            if (SOC_IS_TRIDENT2(unit) || SOC_IS_TITAN2(unit)) {
                soc_trident2_l3_memwr_parity_check(unit, mem, FALSE);
            }
#endif /* BCM_TRIDENT2_SUPPORT */
            if (rv == SOC_E_NONE || rv != SOC_E_UNAVAIL) {
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            }
            /* Take lock to ensure that cache won't be changed during correction */
            if (is_l3_defip) {
                _ALPM_LOCK(unit);
            } else {
                MEM_LOCK(unit, mem);
            }
            cache = SOC_MEM_STATE(unit, mem).cache[copyno];
            vmap = SOC_MEM_STATE(unit, mem).vmap[copyno];
            if (is_l3_defip && (NUM_PIPE(unit) > 1)) {
                /* For L3_DEFIP* tables, we can't use cache for SER correction
                 * on multi-pipe devices
                 */
                if (((mem == L3_DEFIPm) && (alpm_no_cache_flag[unit].flag_defip == 1)) ||
                    ((mem == L3_DEFIP_PAIR_128m) && (alpm_no_cache_flag[unit].flag_defip_128 == 1)) ||
                    ((mem == L3_DEFIP_ALPM_IPV4m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v4 == 1)) ||
                    ((mem == L3_DEFIP_ALPM_IPV4_1m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v4_1 == 1)) ||
                    ((mem == L3_DEFIP_ALPM_IPV6_64m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6 == 1)) ||
                    ((mem == L3_DEFIP_ALPM_IPV6_64_1m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6_1 == 1)) ||
                    ((mem == L3_DEFIP_ALPM_IPV6_128m) && (alpm_no_cache_flag[unit].flag_defip_alpm_v6_128 == 1)) ||
                    ((mem == L3_DEFIP_AUX_TABLEm) && (alpm_no_cache_flag[unit].flag_defip_aux == 1)) ||
                    ((cache != NULL) && (!CACHE_VMAP_TST(vmap, index)))) {
                    cache = NULL;
                }
            }
            if ((cache == NULL) && (NUM_PIPE(unit) > 1)) {
#if defined(BCM_TRIDENT2_SUPPORT)
                if (SOC_IS_TD2_TT2(unit) &&
                    (mem == L3_DEFIPm ||
                     mem == L3_DEFIP_PAIR_128m)) {
                    rv = _soc_ser_recovery_hw_cache(unit, si->pipe_num, mem,
                                                    copyno, index, si, TRUE);
                } else
#endif
                {
                    rv = _soc_ser_recovery_hw_cache(unit, si->pipe_num, mem,
                                                    copyno, index, si, FALSE);
                }
                if (rv == SOC_E_NONE || rv != SOC_E_UNAVAIL) {
                    if (rv == SOC_E_NONE && mem == L3_DEFIP_AUX_TABLEm &&
                        SOC_IS_TD2_TT2(unit)) {
                        if (SOC_CONTROL(unit)->alpm_bulk_retry) {
                            sal_sem_give(SOC_CONTROL(unit)->alpm_bulk_retry);
                        }
                    }
                    if (si->log_id != 0) {
                        log_generic.corrected = (rv == SOC_E_NONE);
                        soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                            sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    }
                    if (is_l3_defip) {
                        _ALPM_UNLOCK(unit)
                    } else {
                        MEM_UNLOCK(unit,mem);
                    }
                    return rv;
                }
            }
            if (cache != NULL && CACHE_VMAP_TST(vmap, index)) {
                sal_memcpy(entry, cache + index * entry_dw, entry_dw * 4);
                if ((si->log_id != 0) &&
                    (si->flags & SOC_SER_LOG_WRITE_CACHE)) {
                    (void)soc_ser_log_add_tlv(unit, si->log_id,
                                              SOC_SER_LOG_TLV_CACHE,
                                              entry_dw*4, entry);
                }
                if ((rv = soc_mem_write(unit, mem, copyno, index, entry)) < 0) {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CACHE_RESTORE %s.%s[%d] failed: %s\n"),
                               SOC_MEM_NAME(unit, mem),
                               SOC_BLOCK_NAME(unit, copyno),
                               index, soc_errmsg(rv)));

                    MEM_UNLOCK(unit, mem);

                    if (si->log_id != 0) {
                        log_generic.corrected = (rv == SOC_E_NONE);
                        soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                            sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    }
                    return rv;
                }
                if (si->log_id != 0) {
                    log_generic.corrected = SOC_SER_CORRECTED;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                                   si, mem, index);
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CACHE_RESTORE: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                           SOC_MEM_NAME(unit, mem), mem,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, si->sblk, si->addr));
                stat->ser_err_restor++;
#ifdef _SER_TIME_STAMP
                ser_time_5 = SAL_USECS_SUB(sal_time_usecs(), ser_time_1);
                LOG_CLI((BSL_META_U(unit,
                                    "Total: %d\n"), ser_time_5));
#endif
            } else { /* Fall back to clear */
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: "
                                "main routine falling back to clear\n")));
                null_entry = soc_mem_entry_null(unit, mem);
                if ((rv = soc_mem_write(unit, mem, copyno, index, null_entry)) < 0) {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "CLEAR_RESTORE %s.%s[%d] failed: %s\n"),
                               SOC_MEM_NAME(unit, mem),
                               SOC_BLOCK_NAME(unit, copyno),
                               index, soc_errmsg(rv)));

                    if (is_l3_defip) {
                        _ALPM_UNLOCK(unit)
                    } else {
                        MEM_UNLOCK(unit, mem);
                    }

                    if (si->log_id != 0) {
                        log_generic.corrected = SOC_SER_UNCORRECTED;
                        soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                            sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    }
                    return rv;
                }

                if (si->log_id != 0) {
                    log_generic.corrected = SOC_SER_CORRECTED;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR,
                                                   si, mem, index);
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "CLEAR_RESTORE: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                           SOC_MEM_NAME(unit, mem), mem,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, si->sblk, si->addr));
                if (_SOC_DRV_MEM_CHK_L2_MEM(mem) &&
                    soc_feature(unit, soc_feature_mac_learn_limit)) {
                    if ((rv =_soc_ser_sync_mac_limits(unit, mem)) != SOC_E_NONE) {
                        LOG_ERROR(BSL_LS_SOC_SER,
                                  (BSL_META_U(unit,
                                              "L2 mac limit sync failed !!\n")));
                        if (is_l3_defip) {
                            _ALPM_UNLOCK(unit)
                        } else {
                            MEM_UNLOCK(unit, mem);
                        }
                        return rv;
                    }
                }
                stat->ser_err_clear++;
#ifdef _SER_TIME_STAMP
                ser_time_5 = SAL_USECS_SUB(sal_time_usecs(), ser_time_1);
                LOG_CLI((BSL_META_U(unit,
                                    "Total: %d\n"), ser_time_5));
#endif
            }
            if (cache != NULL) {
                rv = _soc_ser_check_hard_fault(unit, mem, si->acc_type, copyno,
                                                 index,
                                                 null_entry == NULL ?
                                                 entry : null_entry, 0,
                                                 si->pipe_num, FALSE);
                if (is_l3_defip) {
                    _ALPM_UNLOCK(unit)
                } else {
                    MEM_UNLOCK(unit, mem);
                }

                return rv;
            }
            
            if (is_l3_defip) {
                _ALPM_UNLOCK(unit)
            } else {
                MEM_UNLOCK(unit, mem);
            }
            return SOC_E_NONE;
        case SOC_MEM_FLAG_SER_SPECIAL:
            if (SOC_MEM_SER_CORRECTION_TYPE(unit, mem) ==
                SOC_MEM_FLAG_SER_SPECIAL) {
                LOG_VERBOSE(BSL_LS_SOC_SER,
                    (BSL_META_U(unit,
                                "th_dbg: SER_CORRECTION_TYPE = "
                                "SPECIAL for mem = %0d\n"), mem));
            }
            stat->ser_err_spe++;
            
            switch (mem) {
            case L2_ENTRY_LPm:
            case L3_ENTRY_LPm:
            case VLAN_XLATE_LPm:
            case EGR_VLAN_XLATE_LPm:
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "SER_SPECIAL: %s[%d] blk: %s index: %d : [%d][%x]\n"),
                           SOC_MEM_NAME(unit, mem), mem,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, si->sblk, si->addr));
                rv = _soc_ser_lp_mem_correction(unit, mem, index);
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            case PORT_OR_TRUNK_MAC_COUNTm:
            case VLAN_OR_VFI_MAC_COUNTm:
                rv = _soc_ser_sync_mac_limits(unit, mem);
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            case MA_STATEm:
            case RMEPm:
                rv = _soc_oam_ser_correction(unit, mem, index);
                if (rv != SOC_E_NONE) {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "Error in SER Handling rv=%d\n"),
                               rv));
                }
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return rv;
            default:
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "Unknown ser correction !!\n")));
                stat->ser_err_sw++;
                if (si->log_id != 0) {
                    log_generic.corrected = SOC_SER_UNCORRECTED;
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                }
                return SOC_E_INTERNAL;
            }
        default:
            if (si->log_id != 0) {
                log_generic.corrected = SOC_SER_UNCORRECTED;
                soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                    sizeof(soc_ser_log_tlv_generic_t), &log_generic);
            }
            stat->ser_err_sw++;
            return SOC_E_INTERNAL;
        }
    } else { /* Handle regs */
        uint64 rval64;
        soc_ser_log_tlv_register_t log_reg;
        if (si->log_id != 0) {
            log_reg.index = index;
        }

        stat->ser_err_reg++;
        if (!(si->flags & SOC_SER_REG_MEM_KNOWN)) {
             soc_regaddrinfo_t ainfo;

             /* Decode register from address details */
             soc_regaddrinfo_extended_get(unit, &ainfo, si->sblk,
                                          si->acc_type, si->addr);
             reg = ainfo.reg;
             port = ainfo.port;
             if (si->log_id != 0) {
                log_reg.port = port;
                log_reg.reg = reg;
             }
             if (reg == INVALIDr || port == -1) {
                if (si->log_id != 0) {
                   log_generic.corrected = SOC_SER_UNCORRECTED;
                   soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                       sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                   soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                       sizeof(soc_ser_log_tlv_register_t), &log_reg);
                }
                if (soc_feature(unit, soc_feature_two_ingress_pipes) &&
                    soc_feature(unit, soc_feature_sbusdma)) {
                    LOG_VERBOSE(BSL_LS_SOC_SER,
                                (BSL_META_U(unit,
                                            "register not decoded [%d %d 0x%8x].\n"),
                                 si->sblk, si->acc_type, si->addr));
                    return SOC_E_NOT_FOUND;
                } else {
                    LOG_ERROR(BSL_LS_SOC_SER,
                              (BSL_META_U(unit,
                                          "register not decoded [%d %d 0x%8x].\n"),
                               si->sblk, si->acc_type, si->addr));
                    stat->ser_err_sw++;
                    return SOC_E_INTERNAL;
                }
             }
             index = ainfo.idx >= 0 ? ainfo.idx : 0;
             LOG_ERROR(BSL_LS_SOC_SER,
                       (BSL_META_U(unit,
                                   "reg: %d=%s port: %d index: %d\n"),
                        reg, SOC_REG_NAME(unit, reg), ainfo.port, index));
        }
        if (reg == INVALIDr || port == -1) {
            if (si->log_id != 0) {
               log_generic.corrected = SOC_SER_UNCORRECTED;
               soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                   sizeof(soc_ser_log_tlv_generic_t), &log_generic);
               soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                   sizeof(soc_ser_log_tlv_register_t), &log_reg);
            }
            return SOC_E_PARAM;
        }
        if (si->blk_type >= 0) {
            SOC_BLOCK_ITER(unit, copyno, si->blk_type) {
                if (SOC_BLOCK_TYPE(unit, copyno) == si->blk_type) {
                    break;
                }
            }
        }
        if (!SOC_REG_IS_VALID(unit, reg)) {
            return SOC_E_INTERNAL;
        }
        if (SOC_REG_IS_COUNTER(unit, reg)) {
            if (SOC_SER_COUNTER_CORRECTION(unit) == 0) {
                COMPILER_64_ZERO(rval64);
                /* Clear counter in h/w and s/w */
                rv = soc_counter_set(unit, port, reg, index, rval64);

                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                        sizeof(soc_ser_log_tlv_register_t), &log_reg);
                }

                SOC_IF_ERROR_RETURN(rv);
                LOG_WARN(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "COUNTER_CLEAR: %s[%d] blk: %s index: %d : port[%d]\n"),
                           SOC_REG_NAME(unit, reg), reg,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, port));
                stat->ser_err_clear++;
            } else {
                /* Restore last s/w counter value in h/w */
                rv = soc_counter_get(unit, port, reg, index, &rval64);

                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                        sizeof(soc_ser_log_tlv_register_t), &log_reg);
                }

                SOC_IF_ERROR_RETURN(rv);
                SOC_IF_ERROR_RETURN(soc_counter_set(unit, port, reg, index, rval64));
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "COUNTER_RESTORE: %s[%d] blk: %s index: %d : port[%d]\n"),
                           SOC_REG_NAME(unit, reg), reg,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, port));
                stat->ser_err_restor++;
            }
        } else {
            rv = soc_ser_reg_cache_get(unit, reg, port, index, &rval64);
            if (rv == SOC_E_UNAVAIL) {
                rv = soc_ser_reg_clear(unit, reg, port, index, rval64);
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                        sizeof(soc_ser_log_tlv_register_t), &log_reg);
                }

                SOC_IF_ERROR_RETURN(rv);
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR, si,
                                       reg | SOC_SER_ERROR_DATA_REG_ID_OFFSET_SET,
                                       port << SOC_SER_ERROR_PIPE_BP | index);
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "REG_CLEAR: %s[%d] blk: %s index: %d : port[%d]\n"),
                           SOC_REG_NAME(unit, reg), reg,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, port));
                stat->ser_err_clear++;
            } else {
                rv = soc_reg_set_nocache(unit, reg, port, index, rval64);
                if (si->log_id != 0) {
                    log_generic.corrected = (rv == SOC_E_NONE);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_GENERIC,
                        sizeof(soc_ser_log_tlv_generic_t), &log_generic);
                    soc_ser_log_add_tlv(unit, si->log_id, SOC_SER_LOG_TLV_REGISTER,
                        sizeof(soc_ser_log_tlv_register_t), &log_reg);
                }

                SOC_IF_ERROR_RETURN(rv);
                _soc_ser_correction_event_generate(unit, SOC_SWITCH_EVENT_PARITY_ERROR, si,
                                       reg | SOC_SER_ERROR_DATA_REG_ID_OFFSET_SET,
                                       port << SOC_SER_ERROR_PIPE_BP | index);
                LOG_ERROR(BSL_LS_SOC_SER,
                          (BSL_META_U(unit,
                                      "REG_RESTORE: %s[%d] blk: %s index: %d : port[%d]\n"),
                           SOC_REG_NAME(unit, reg), reg,
                           SOC_BLOCK_NAME(unit, copyno),
                           index, port));
                stat->ser_err_restor++;
            }
        }
    }
    return SOC_E_NONE;
}

static int
_soc_ser_mem_info_init(int unit)
{
    uint32    size_ser_info = sizeof(mem_ser_info_t), i = 0;

    SER_MEM_INFO_BASE_ADDR(unit) =
            (mem_ser_info_t *)sal_alloc(size_ser_info * SER_MEM_INFO_MAX_NUM,
                                        "mem_ser_info");
    if (SER_MEM_INFO_BASE_ADDR(unit) == NULL) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "ERR: allocate memory space fail\n")));
        return SOC_E_MEMORY;
    }
    sal_memset(SER_MEM_INFO_BASE_ADDR(unit), 0, size_ser_info * SER_MEM_INFO_MAX_NUM);

    for (i = 0; i < SER_MEM_INFO_MAX_NUM - 1; i++) {
        SER_MEM_INFO_NEXT_IDX(unit, i) = i + 1;
    }
    SER_MEM_INFO_NEXT_IDX(unit, i) = 0;
    SER_MEM_INFO_HEAD_IDX(unit) = 0;
    SER_MEM_INFO_TAIL_IDX(unit) = 0;
    return SOC_E_NONE;
}

void
soc_ser_mem_info_dump(int unit)
{
    int        counter;
    uint32     tail_idx, cur_idx;
    mem_ser_info_t *ser_info_p = NULL;

    if(!bsl_check(bslLayerSoc, bslSourceSer, bslSeverityNormal, unit)) {
        return;
    }
    counter = SER_MEM_INFO_COUNTER(unit);
    cur_idx = SER_MEM_INFO_HEAD_IDX(unit);
    tail_idx = SER_MEM_INFO_TAIL_IDX(unit);

    LOG_DEBUG(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "tail-idx=[%d], head-idx=[%d], maximum=[%d], used-num=[%d]\n"),
         tail_idx, cur_idx,
         SER_MEM_INFO_MAX_NUM, SER_MEM_INFO_COUNTER(unit)));

    while (cur_idx != tail_idx || counter) {
        ser_info_p = SER_MEM_INFO_BASE_ADDR(unit) + cur_idx;
        LOG_DEBUG(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "[mem=%s][idx=%d] at ser_mem_info[%d]\n"),
             SOC_MEM_NAME(unit, ser_info_p->ser_mem), ser_info_p->hw_idx, cur_idx));
        cur_idx = SER_MEM_INFO_NEXT_IDX(unit, cur_idx);
        counter--;
    }
    assert(counter == 0);
}

STATIC void
_soc_ser_mem_info_insert(int unit, soc_mem_t rst_mem, uint32 hw_idx,
                                uint32 *entry_data)
{
    mem_ser_info_t *mem_ser_info_addr = NULL;
    uint32         entry_bsize = soc_mem_entry_bytes(unit, rst_mem);
    uint32         tail_idx = SER_MEM_INFO_TAIL_IDX(unit);
    uint32         old_hw_idx = 0;
    soc_mem_t      old_rst_mem;

    mem_ser_info_addr = SER_MEM_INFO_BASE_ADDR(unit) + tail_idx;

    soc_ser_mem_info_dump(unit);

    old_hw_idx = mem_ser_info_addr->hw_idx;
    old_rst_mem = mem_ser_info_addr->ser_mem;
    mem_ser_info_addr->ser_mem = rst_mem;
    mem_ser_info_addr->hw_idx = hw_idx;
    sal_memset(mem_ser_info_addr->entry_data, 0, SOC_MAX_MEM_WORDS);
    sal_memcpy(mem_ser_info_addr->entry_data, entry_data, entry_bsize);

    if (SER_MEM_INFO_COUNTER(unit) < SER_MEM_INFO_MAX_NUM) {
        SER_MEM_INFO_COUNTER(unit)++;
        LOG_INFO(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "store [mem=%s] [idx=%d] at ser_mem_info[%d]\n"),
             SOC_MEM_NAME(unit, rst_mem), hw_idx, tail_idx));
    } else {
        /* overlay */
        assert(SER_MEM_INFO_TAIL_IDX(unit) == SER_MEM_INFO_HEAD_IDX(unit));
        LOG_INFO(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "overlay [mem=%s][idx=%d] to [mem=%s][idx=%d] at ser_mem_info[%d]\n"),
             SOC_MEM_NAME(unit, rst_mem), hw_idx,
             SOC_MEM_NAME(unit, old_rst_mem), old_hw_idx, tail_idx));
        SER_MEM_INFO_HEAD_IDX(unit) = SER_MEM_INFO_NEXT_IDX(unit, SER_MEM_INFO_HEAD_IDX(unit));
    }
    SER_MEM_INFO_TAIL_IDX(unit) = SER_MEM_INFO_NEXT_IDX(unit, tail_idx);

    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "tail-idx=[%d], head-idx=[%d], maximum=[%d], used-num=[%d]\n"),
         SER_MEM_INFO_TAIL_IDX(unit), SER_MEM_INFO_HEAD_IDX(unit),
         SER_MEM_INFO_MAX_NUM, SER_MEM_INFO_COUNTER(unit)));
}
STATIC void
_soc_ser_mem_info_delete(int unit, uint32 cur_idx, uint32 pre_idx)
{
    mem_ser_info_t *mem_ser_info_addr = NULL;
    uint32         tail_idx, pre_head_idx;
    soc_mem_t      rst_mem;
    uint32         hw_idx;
    int            counter;
    if (SER_MEM_INFO_COUNTER(unit) <= 0) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "There is fatal error!!!\n")));
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "tail-idx=[%d], head-idx=[%d], maximum=[%d], used-num=[%d]\n"),
             SER_MEM_INFO_TAIL_IDX(unit), SER_MEM_INFO_HEAD_IDX(unit),
             SER_MEM_INFO_MAX_NUM, SER_MEM_INFO_COUNTER(unit)));
        return;
    }

    soc_ser_mem_info_dump(unit);

    mem_ser_info_addr = SER_MEM_INFO_BASE_ADDR(unit) + cur_idx;
    rst_mem = mem_ser_info_addr->ser_mem;
    hw_idx = mem_ser_info_addr->hw_idx;
    mem_ser_info_addr->ser_mem = 0;
    mem_ser_info_addr->hw_idx = 0;
    sal_memset(mem_ser_info_addr->entry_data, 0, SOC_MAX_MEM_WORDS);

    tail_idx = SER_MEM_INFO_TAIL_IDX(unit);
    /* full */
    if (SER_MEM_INFO_COUNTER(unit) == SER_MEM_INFO_MAX_NUM) {
        assert(SER_MEM_INFO_TAIL_IDX(unit) == SER_MEM_INFO_HEAD_IDX(unit));

        if (cur_idx == tail_idx) {
            SER_MEM_INFO_HEAD_IDX(unit) = SER_MEM_INFO_NEXT_IDX(unit, cur_idx);
        } else if (SER_MEM_INFO_NEXT_IDX(unit, cur_idx) == tail_idx) {
            SER_MEM_INFO_TAIL_IDX(unit) = cur_idx;
        }  else {
            /* find previous index of head index */
            pre_head_idx = tail_idx; /* tail index is equal to head index */
            counter = SER_MEM_INFO_COUNTER(unit);
            while (SER_MEM_INFO_NEXT_IDX(unit, pre_head_idx) != tail_idx) {
                counter--;
                pre_head_idx = SER_MEM_INFO_NEXT_IDX(unit, pre_head_idx);
            }
            assert(counter == 1);

            SER_MEM_INFO_TAIL_IDX(unit) = cur_idx;
            SER_MEM_INFO_NEXT_IDX(unit, pre_idx) = SER_MEM_INFO_NEXT_IDX(unit, cur_idx);
            SER_MEM_INFO_NEXT_IDX(unit, pre_head_idx) = cur_idx;
            SER_MEM_INFO_NEXT_IDX(unit, cur_idx) = tail_idx;
        }
    } else {
        if (SER_MEM_INFO_NEXT_IDX(unit, cur_idx) == SER_MEM_INFO_TAIL_IDX(unit)) {
            SER_MEM_INFO_TAIL_IDX(unit) = cur_idx;
        } else if (cur_idx == SER_MEM_INFO_HEAD_IDX(unit)) {
            SER_MEM_INFO_HEAD_IDX(unit) = SER_MEM_INFO_NEXT_IDX(unit, cur_idx);
        } else {
            /* pre_idx is equal to cur_idx only when cur_idx == SER_MEM_INFO_HEAD_IDX(unit) */
            assert(pre_idx != cur_idx);
            SER_MEM_INFO_NEXT_IDX(unit, pre_idx) = SER_MEM_INFO_NEXT_IDX(unit, cur_idx);
            SER_MEM_INFO_NEXT_IDX(unit, cur_idx) = SER_MEM_INFO_NEXT_IDX(unit, tail_idx);
            SER_MEM_INFO_NEXT_IDX(unit, tail_idx) = cur_idx;
        }
    }
    SER_MEM_INFO_COUNTER(unit)--;
    LOG_INFO(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "delete [mem=%s][idx=%d] at ser_mem_info[%d]\n"),
         SOC_MEM_NAME(unit, rst_mem), hw_idx, cur_idx));
    LOG_VERBOSE(BSL_LS_SOC_SER,
        (BSL_META_U(unit,
                    "tail-idx=[%d], head-idx=[%d], maximum=[%d], used-num=[%d]\n"),
         SER_MEM_INFO_TAIL_IDX(unit), SER_MEM_INFO_HEAD_IDX(unit),
         SER_MEM_INFO_MAX_NUM, SER_MEM_INFO_COUNTER(unit)));
}

int
soc_ser_counter_info_set(int unit, soc_mem_t rst_mem,
                         uint32 hw_idx, uint32 *entry_data)
{
    mem_ser_info_t *mem_ser_info_addr = NULL;
    soc_mem_t mem_ser_list[] = {EFP_COUNTER_TABLE_Xm,
                                EFP_COUNTER_TABLE_Ym,
                                EFP_COUNTER_TABLEm,
                                FP_COUNTER_TABLE_Xm,
                                FP_COUNTER_TABLE_Ym,
                                FP_COUNTER_TABLEm};
    uint32    i, mem_num = sizeof(mem_ser_list)/sizeof(mem_ser_list[0]);
    int       rv = SOC_E_NONE;
    uint32    idx, pre_idx;

    for (i = 0; i < mem_num; i++) {
        if (mem_ser_list[i] == rst_mem) {
            break;
        }
    }
    if (i >= mem_num) {
        return SOC_E_UNAVAIL;
    }

    if (entry_data == NULL) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "pointer entry_data is NULL\n")));
        return SOC_E_PARAM;
    }

    if (soc_mem_index_count(unit, rst_mem) <= hw_idx) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "store mem %s, max index=%d, wrong index=%d\n"),
             SOC_MEM_NAME(unit, rst_mem), soc_mem_index_count(unit, rst_mem), hw_idx));
        return SOC_E_INTERNAL;
    }

    SOC_MEM_SER_INFO_LOCK(unit);

    if (SER_MEM_INFO_BASE_ADDR(unit) == NULL) {
        rv = _soc_ser_mem_info_init(unit);
        if (rv != SOC_E_NONE) {
            SOC_MEM_SER_INFO_UNLOCK(unit);
            return rv;
        }
    }
    idx = SER_MEM_INFO_HEAD_IDX(unit);
    pre_idx = idx;

    for (i = 0; i < SER_MEM_INFO_COUNTER(unit); i++) {
        mem_ser_info_addr = SER_MEM_INFO_BASE_ADDR(unit);
        mem_ser_info_addr += idx;
        if (mem_ser_info_addr->ser_mem == rst_mem &&
            mem_ser_info_addr->hw_idx ==  hw_idx) {
             break;
        }
        pre_idx = idx;
        idx = SER_MEM_INFO_NEXT_IDX(unit, idx);
    }
    if (i < SER_MEM_INFO_COUNTER(unit)) { /* found */
        /* delete old info */
        _soc_ser_mem_info_delete(unit, idx, pre_idx);
        /* insert new info at tail */
        _soc_ser_mem_info_insert(unit, rst_mem, hw_idx, entry_data);
    } else {
        /* not found */
        assert(idx == SER_MEM_INFO_TAIL_IDX(unit));
        /* insert new info at tail */
        _soc_ser_mem_info_insert(unit, rst_mem, hw_idx, entry_data);
    }
    SOC_MEM_SER_INFO_UNLOCK(unit);
    return SOC_E_NONE;
}

int
soc_ser_counter_info_get(int unit, soc_mem_t rst_mem,
                         int hw_idx, uint32 *entry_data)
{
    mem_ser_info_t *mem_ser_info_addr = NULL;
    uint32 entry_bsize = soc_mem_entry_bytes(unit, rst_mem);
    soc_mem_t mem_ser_list[] = {EFP_COUNTER_TABLE_Xm,
                                EFP_COUNTER_TABLE_Ym,
                                EFP_COUNTER_TABLEm,
                                FP_COUNTER_TABLE_Xm,
                                FP_COUNTER_TABLE_Ym,
                                FP_COUNTER_TABLEm};
    int    i = 0, mem_num = sizeof(mem_ser_list)/sizeof(mem_ser_list[0]);
    uint32 idx, pre_idx;

    for (i = 0; i < mem_num; i++) {
        if (mem_ser_list[i] == rst_mem) {
            break;
        }
    }
    if (i >= mem_num) {
        return SOC_E_UNAVAIL;
    }
    if (entry_data == NULL) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "pointer entry_data is NULL\n")));
        return SOC_E_PARAM;
    }
    if (soc_mem_index_count(unit, rst_mem) <= hw_idx) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "restore mem %s, max index=%d, wrong index=%d\n"),
             SOC_MEM_NAME(unit, rst_mem), soc_mem_index_count(unit, rst_mem), hw_idx));
        return SOC_E_INTERNAL;
    }

    SOC_MEM_SER_INFO_LOCK(unit);

    if (SER_MEM_INFO_BASE_ADDR(unit) == NULL) {
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "ser mem info is not initialized\n")));
        SOC_MEM_SER_INFO_UNLOCK(unit);
        return SOC_E_NOT_FOUND;
    }

    idx = SER_MEM_INFO_HEAD_IDX(unit);
    pre_idx = idx;

    for (i = 0; i < SER_MEM_INFO_COUNTER(unit); i++) {
        mem_ser_info_addr = SER_MEM_INFO_BASE_ADDR(unit);
        mem_ser_info_addr += idx;
        if (mem_ser_info_addr->ser_mem == rst_mem &&
            mem_ser_info_addr->hw_idx ==  hw_idx) {
             break;
        }
        pre_idx = idx;
        idx = SER_MEM_INFO_NEXT_IDX(unit, idx);
    }

    /* found */
    if (i < SER_MEM_INFO_COUNTER(unit)) {
        sal_memcpy(entry_data, mem_ser_info_addr->entry_data, entry_bsize);
        _soc_ser_mem_info_delete(unit, idx, pre_idx);
        SOC_MEM_SER_INFO_UNLOCK(unit);
        return SOC_E_NONE;
    } else {
        assert(idx == SER_MEM_INFO_TAIL_IDX(unit));
        LOG_WARN(BSL_LS_SOC_SER,
            (BSL_META_U(unit,
                        "not found mem %s, hw idx=%d\n"),
             SOC_MEM_NAME(unit, rst_mem), hw_idx));

        SOC_MEM_SER_INFO_UNLOCK(unit);
        return SOC_E_NOT_FOUND;
    }
}

void soc_ser_alpm_cache_check(int unit)
{
    if (soc_property_get(unit, spn_MEM_CHECK_NOCACHE_OVERRIDE, 0)) {
        alpm_no_cache_flag[unit].flag_defip = soc_property_get(unit, "mem_nocache_L3_DEFIP", 1);
        alpm_no_cache_flag[unit].flag_defip_128 = soc_property_get(unit, "mem_nocache_L3_DEFIP_PAIR_128", 1);
        alpm_no_cache_flag[unit].flag_defip_alpm_v4 = soc_property_get(unit, "mem_nocache_L3_DEFIP_ALPM_IPV4", 1);
        alpm_no_cache_flag[unit].flag_defip_alpm_v4_1 = soc_property_get(unit, "mem_nocache_L3_DEFIP_ALPM_IPV4_1", 1);
        alpm_no_cache_flag[unit].flag_defip_alpm_v6 = soc_property_get(unit, "mem_nocache_L3_DEFIP_ALPM_IPV6_64", 1);
        alpm_no_cache_flag[unit].flag_defip_alpm_v6_1 = soc_property_get(unit, "mem_nocache_L3_DEFIP_ALPM_IPV6_64_1", 1);
        alpm_no_cache_flag[unit].flag_defip_alpm_v6_128 = soc_property_get(unit, "mem_nocache_L3_DEFIP_ALPM_IPV6_128", 1);
        alpm_no_cache_flag[unit].flag_defip_aux = soc_property_get(unit, "mem_nocache_L3_DEFIP_AUX_TABLE", 1);
    } else {
        alpm_no_cache_flag[unit].flag_defip = 1;
        alpm_no_cache_flag[unit].flag_defip_128 = 1;
        alpm_no_cache_flag[unit].flag_defip_alpm_v4 = 1;
        alpm_no_cache_flag[unit].flag_defip_alpm_v4_1 = 1;
        alpm_no_cache_flag[unit].flag_defip_alpm_v6 = 1;
        alpm_no_cache_flag[unit].flag_defip_alpm_v6_1 = 1;
        alpm_no_cache_flag[unit].flag_defip_alpm_v6_128 = 1;
        alpm_no_cache_flag[unit].flag_defip_aux = 1;
    }
}

#endif /* BCM_XGS_SUPPORT || BCM_SIRIUS_SUPPORT */
